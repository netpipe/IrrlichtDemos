
#include "btRadixSort32CL.h"
#include "btLauncherCL.h"
#include "btOpenCLUtils.h"
#include "btPrefixScanCL.h"
#include "btFillCL.h"

#ifdef _WIN32
#define RADIXSORT32_PATH "./Demos/FluidSphDemo/BulletMultiThreaded/SphSolverOpenCL/btExperimentsOpenCL/RadixSort32Kernels.cl"
#else
#define RADIXSORT32_PATH "./Demos/FluidSphDemo/BulletMultiThreaded/SphSolverOpenCL/btExperimentsOpenCL/RadixSort32Kernels.cl"
#endif

#include "RadixSort32KernelsCL.h"

btRadixSort32CL::btRadixSort32CL(cl_context ctx, cl_device_id device, cl_command_queue queue, int initialCapacity)
:m_commandQueue(queue)
{
	m_workBuffer1 = new btOpenCLArray<unsigned int>(ctx,queue);
	m_workBuffer2 = new btOpenCLArray<unsigned int>(ctx,queue);
	m_workBuffer3 = new btOpenCLArray<btSortData>(ctx,queue);
	m_workBuffer4 = new btOpenCLArray<btSortData>(ctx,queue);
	


	if (initialCapacity>0)
	{
		m_workBuffer1->resize(initialCapacity);
		m_workBuffer3->resize(initialCapacity);
		m_workBuffer4->resize(initialCapacity);
	}

	m_scan = new btPrefixScanCL(ctx,device,queue);
	m_fill = new btFillCL(ctx,device,queue);
	
	const char* additionalMacros = "";
	const char* srcFileNameForCaching="";

	cl_int pErrNum;
	const char* kernelSource = radixSort32KernelsCL;
	
	cl_program sortProg = btOpenCLUtils::compileCLProgramFromString( ctx, device, kernelSource, &pErrNum,additionalMacros, RADIXSORT32_PATH);
	btAssert(sortProg);

	m_streamCountSortDataKernel = btOpenCLUtils::compileCLKernelFromString( ctx, device, kernelSource, "StreamCountSortDataKernel", &pErrNum, sortProg,additionalMacros );
	btAssert(m_streamCountSortDataKernel );
//this kernel doesn't work on Apple, use a slower serial fallback for now
//
//todo: detect this at runtime or fix the kernel to work cross-platform
#if !defined(__APPLE__1) && !defined(CL_PLATFORM_INTEL)
	m_sortAndScatterSortDataKernel = btOpenCLUtils::compileCLKernelFromString( ctx, device, kernelSource, "SortAndScatterSortDataKernel", &pErrNum, sortProg,additionalMacros );
#else
    m_sortAndScatterSortDataKernel = btOpenCLUtils::compileCLKernelFromString( ctx, device, kernelSource, "SortAndScatterSortDataKernelSerial", &pErrNum, sortProg,additionalMacros );
#endif
	btAssert(m_sortAndScatterSortDataKernel);
	m_prefixScanKernel = btOpenCLUtils::compileCLKernelFromString( ctx, device, kernelSource, "PrefixScanKernel", &pErrNum, sortProg,additionalMacros );
	btAssert(m_prefixScanKernel);
		
}

btRadixSort32CL::~btRadixSort32CL()
{
	delete m_scan;
	delete m_fill;
	delete m_workBuffer1;
	delete m_workBuffer2;
	delete m_workBuffer3;
	delete m_workBuffer4;

	clReleaseKernel(m_streamCountSortDataKernel);
	clReleaseKernel(m_sortAndScatterSortDataKernel);
	clReleaseKernel(m_prefixScanKernel);
}

void btRadixSort32CL::executeHost(btAlignedObjectArray<btSortData>& inout, int sortBits /* = 32 */)
{
	int n = inout.size();
	const int BITS_PER_PASS = 8;
	const int NUM_TABLES = (1<<BITS_PER_PASS);


	int tables[NUM_TABLES];
	int counter[NUM_TABLES];

	btSortData* src = &inout[0];
	btAlignedObjectArray<btSortData> workbuffer;
	workbuffer.resize(inout.size());
	btSortData* dst = &workbuffer[0];

	int count=0;
	for(int startBit=0; startBit<sortBits; startBit+=BITS_PER_PASS)
	{
		for(int i=0; i<NUM_TABLES; i++)
		{
			tables[i] = 0;
		}

		for(int i=0; i<n; i++)
		{
			int tableIdx = (src[i].m_key >> startBit) & (NUM_TABLES-1);
			tables[tableIdx]++;
		}
//#define TEST
#ifdef TEST
		printf("histogram size=%d\n",NUM_TABLES);
		for (int i=0;i<NUM_TABLES;i++)
		{
			if (tables[i]!=0)
			{
				printf("tables[%d]=%d]\n",i,tables[i]);
			}

		}
#endif //TEST
		//	prefix scan
		int sum = 0;
		for(int i=0; i<NUM_TABLES; i++)
		{
			int iData = tables[i];
			tables[i] = sum;
			sum += iData;
			counter[i] = 0;
		}

		//	distribute
		for(int i=0; i<n; i++)
		{
			int tableIdx = (src[i].m_key >> startBit) & (NUM_TABLES-1);
			
			dst[tables[tableIdx] + counter[tableIdx]] = src[i];
			counter[tableIdx] ++;
		}

		btSwap( src, dst );
		count++;
	}

	if (count&1)
	{
		btAssert(0);//need to copy 

	}
}

void btRadixSort32CL::executeHost(btOpenCLArray<btSortData>& keyValuesInOut, int sortBits /* = 32 */)
{

	btAlignedObjectArray<btSortData> inout;
	keyValuesInOut.copyToHost(inout);

	executeHost(inout,sortBits);

	keyValuesInOut.copyFromHost(inout);
}

void btRadixSort32CL::execute(btOpenCLArray<unsigned int>& keysIn, btOpenCLArray<unsigned int>& keysOut, btOpenCLArray<unsigned int>& valuesIn, 
								btOpenCLArray<unsigned int>& valuesOut, int n, int sortBits)
{

}

//#define DEBUG_RADIXSORT
//#define DEBUG_RADIXSORT2


void btRadixSort32CL::execute(btOpenCLArray<btSortData>& keyValuesInOut, int sortBits /* = 32 */)
{
	
	int originalSize = keyValuesInOut.size();
	int workingSize = originalSize;
	
			
	int dataAlignment = DATA_ALIGNMENT;

#ifdef DEBUG_RADIXSORT2
    btAlignedObjectArray<btSortData>   test2;
    keyValuesInOut.copyToHost(test2);
    printf("numElem = %d\n",test2.size());
    for (int i=0;i<test2.size();i++)
    {
        printf("test2[%d].m_key=%d\n",i,test2[i].m_key);
        printf("test2[%d].m_value=%d\n",i,test2[i].m_value);
    }
#endif //DEBUG_RADIXSORT2
    
	btOpenCLArray<btSortData>* src = 0;

	if (workingSize%dataAlignment)
	{
		workingSize += dataAlignment-(workingSize%dataAlignment);
		m_workBuffer4->copyFromOpenCLArray(keyValuesInOut);
		m_workBuffer4->resize(workingSize);
		btSortData fillValue;
		fillValue.m_key = 0xffffffff;
		fillValue.m_value = 0xffffffff;

#define USE_BTFILL
#ifdef USE_BTFILL
		m_fill->execute((btOpenCLArray<btInt2>&)*m_workBuffer4,(btInt2&)fillValue,workingSize-originalSize,originalSize);
#else
		//fill the remaining bits (very slow way, todo: fill on GPU/OpenCL side)
		
		for (int i=originalSize; i<workingSize;i++)
		{
			m_workBuffer4->copyFromHostPointer(&fillValue,1,i);
		}
#endif//USE_BTFILL

		src = m_workBuffer4;
	} else
	{
		src = &keyValuesInOut;
		m_workBuffer4->resize(0);
	}
		
	btAssert( workingSize%DATA_ALIGNMENT == 0 );
	int minCap = NUM_BUCKET*NUM_WGS;


	int n = workingSize;

	m_workBuffer1->resize(minCap);
	m_workBuffer3->resize(workingSize);
	

//	ADLASSERT( ELEMENTS_PER_WORK_ITEM == 4 );
	btAssert( BITS_PER_PASS == 4 );
	btAssert( WG_SIZE == 64 );
	btAssert( (sortBits&0x3) == 0 );

	
	
	btOpenCLArray<btSortData>* dst = m_workBuffer3;

	btOpenCLArray<unsigned int>* srcHisto = m_workBuffer1;
	btOpenCLArray<unsigned int>* destHisto = m_workBuffer2;


	int nWGs = NUM_WGS;
	btConstData cdata;

	{
        int blockSize = ELEMENTS_PER_WORK_ITEM*WG_SIZE;//set at 256
     	int nBlocks = (n+blockSize-1)/(blockSize);
		cdata.m_n = n;
		cdata.m_nWGs = NUM_WGS;
		cdata.m_startBit = 0;
		cdata.m_nBlocksPerWG = (nBlocks + cdata.m_nWGs - 1)/cdata.m_nWGs;
		if( nBlocks < NUM_WGS )
		{
			cdata.m_nBlocksPerWG = 1;
			nWGs = nBlocks;
		}
	}

	int count=0;
	for(int ib=0; ib<sortBits; ib+=4)
	{
#ifdef DEBUG_RADIXSORT2
        keyValuesInOut.copyToHost(test2);
        printf("numElem = %d\n",test2.size());
        for (int i=0;i<test2.size();i++)
        {
            if (test2[i].m_key != test2[i].m_value)
            {
                printf("test2[%d].m_key=%d\n",i,test2[i].m_key);
                printf("test2[%d].m_value=%d\n",i,test2[i].m_value);
            }
        }
#endif //DEBUG_RADIXSORT2
        
		cdata.m_startBit = ib;
		
		{
			btBufferInfoCL bInfo[] = { btBufferInfoCL( src->getBufferCL(), true ), btBufferInfoCL( srcHisto->getBufferCL() ) };
			btLauncherCL launcher(m_commandQueue, m_streamCountSortDataKernel);

			launcher.setBuffers( bInfo, sizeof(bInfo)/sizeof(btBufferInfoCL) );
			launcher.setConst(  cdata );
			
			int num = NUM_WGS*WG_SIZE;
			launcher.launch1D( num, WG_SIZE );
		}

        
        
#ifdef DEBUG_RADIXSORT
		btAlignedObjectArray<unsigned int> testHist;
		srcHisto->copyToHost(testHist);
		printf("ib = %d, testHist size = %d, non zero elements:\n",ib, testHist.size());
		for (int i=0;i<testHist.size();i++)
		{
			if (testHist[i]!=0)
				printf("testHist[%d]=%d\n",i,testHist[i]);
		}
#endif //DEBUG_RADIXSORT
	
	

//fast prefix scan is not working properly on Mac OSX yet
#ifdef _WIN32
	bool fastScan=true;
#else
	bool fastScan=false;
#endif

		if (fastScan)
		{//	prefix scan group histogram
			btBufferInfoCL bInfo[] = { btBufferInfoCL( srcHisto->getBufferCL() ) };
			btLauncherCL launcher( m_commandQueue, m_prefixScanKernel );
			launcher.setBuffers( bInfo, sizeof(bInfo)/sizeof(btBufferInfoCL) );
			launcher.setConst(  cdata );
			launcher.launch1D( 128, 128 );
			destHisto = srcHisto;
		}else
		{
			//unsigned int sum; //for debugging
            m_scan->execute(*srcHisto,*destHisto,1920,0);//,&sum);
		}


#ifdef DEBUG_RADIXSORT
		destHisto->copyToHost(testHist);
		printf("ib = %d, testHist size = %d, non zero elements:\n",ib, testHist.size());
		for (int i=0;i<testHist.size();i++)
		{
			if (testHist[i]!=0)
				printf("testHist[%d]=%d\n",i,testHist[i]);
		}
        
        for (int i=0;i<testHist.size();i+=NUM_WGS)
		{
				printf("testHist[%d]=%d\n",i/NUM_WGS,testHist[i]);
		}

#endif //DEBUG_RADIXSORT

#define USE_GPU
#ifdef USE_GPU
        
		{//	local sort and distribute
			btBufferInfoCL bInfo[] = { btBufferInfoCL( src->getBufferCL(), true ), btBufferInfoCL( destHisto->getBufferCL(), true ), btBufferInfoCL( dst->getBufferCL() )};
			btLauncherCL launcher( m_commandQueue, m_sortAndScatterSortDataKernel );
			launcher.setBuffers( bInfo, sizeof(bInfo)/sizeof(btBufferInfoCL) );
			launcher.setConst(  cdata );
			launcher.launch1D( nWGs*WG_SIZE, WG_SIZE );
            
		}
#else
        {
#define NUM_TABLES 16
//#define SEQUENTIAL
#ifdef SEQUENTIAL
            int counter2[NUM_TABLES]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            int tables[NUM_TABLES];
            int startBit = ib;
            
            destHisto->copyToHost(testHist);
            btAlignedObjectArray<btSortData> srcHost;
            btAlignedObjectArray<btSortData> dstHost;
            dstHost.resize(src->size());
            
            src->copyToHost(srcHost);
            
            for (int i=0;i<NUM_TABLES;i++)
            {
                tables[i] = testHist[i*NUM_WGS];
            }
            
            //	distribute
            for(int i=0; i<n; i++)
            {
                int tableIdx = (srcHost[i].m_key >> startBit) & (NUM_TABLES-1);
                
                dstHost[tables[tableIdx] + counter2[tableIdx]] = srcHost[i];
                counter2[tableIdx] ++;
            }
            
            
#else
          
            int counter2[NUM_TABLES]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            
            int tables[NUM_TABLES];
             btAlignedObjectArray<btSortData> dstHostOK;
            dstHostOK.resize(src->size());

            destHisto->copyToHost(testHist);
            btAlignedObjectArray<btSortData> srcHost;
            src->copyToHost(srcHost);
        
            int blockSize = 256;
            int nBlocksPerWG = cdata.m_nBlocksPerWG;
            int startBit = ib;

            {
                for (int i=0;i<NUM_TABLES;i++)
                {
                    tables[i] = testHist[i*NUM_WGS];
                }
                
                //	distribute
                for(int i=0; i<n; i++)
                {
                    int tableIdx = (srcHost[i].m_key >> startBit) & (NUM_TABLES-1);
                    
                    dstHostOK[tables[tableIdx] + counter2[tableIdx]] = srcHost[i];
                    counter2[tableIdx] ++;
                }

            
            }
            
            
            btAlignedObjectArray<btSortData> dstHost;
            dstHost.resize(src->size());
            
            
            int counter[NUM_TABLES]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            
            
            
            for (int wgIdx=0;wgIdx<NUM_WGS;wgIdx++)
            {
              int counter[NUM_TABLES]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

              int nBlocks = (n)/blockSize - nBlocksPerWG*wgIdx;
                
              for(int iblock=0; iblock<btMin(cdata.m_nBlocksPerWG, nBlocks); iblock++)
              {
                for (int lIdx = 0;lIdx < 64;lIdx++)
                {
                    int addr = iblock*blockSize + blockSize*cdata.m_nBlocksPerWG*wgIdx + ELEMENTS_PER_WORK_ITEM*lIdx;
                    
                    //	MY_HISTOGRAM( localKeys.x ) ++ is much expensive than atomic add as it requires read and write while atomics can just add on AMD
                    //	Using registers didn't perform well. It seems like use localKeys to address requires a lot of alu ops
                    //	AMD: AtomInc performs better while NV prefers ++
                    for(int j=0; j<ELEMENTS_PER_WORK_ITEM; j++)
                    {
                        if( addr+j < n )
                        {
                          //  printf ("addr+j=%d\n", addr+j);
                            
                            int i = addr+j;
                            
                            int tableIdx = (srcHost[i].m_key >> startBit) & (NUM_TABLES-1);
                            
                            int destIndex = testHist[tableIdx*NUM_WGS+wgIdx] + counter[tableIdx];
                            
                            btSortData ok = dstHostOK[destIndex];
                                                    
                            if (ok.m_key != srcHost[i].m_key)
                            {
                                printf("ok.m_key = %d, srcHost[i].m_key = %d\n", ok.m_key,srcHost[i].m_key );
                                printf("(ok.m_value = %d, srcHost[i].m_value = %d)\n", ok.m_value,srcHost[i].m_value );
                            }
                            if (ok.m_value != srcHost[i].m_value)
                            {
                                
                               printf("ok.m_value = %d, srcHost[i].m_value = %d\n", ok.m_value,srcHost[i].m_value );
                                printf("(ok.m_key = %d, srcHost[i].m_key = %d)\n", ok.m_key,srcHost[i].m_key );

                            }
                   
                            dstHost[destIndex] = srcHost[i];
                            counter[tableIdx] ++;
                            
                        }
                    }
                }
              }
            }
            
         
#endif //SEQUENTIAL
            
            dst->copyFromHost(dstHost);
        }
#endif//USE_GPU
        
        
        
#ifdef DEBUG_RADIXSORT
		destHisto->copyToHost(testHist);
		printf("ib = %d, testHist size = %d, non zero elements:\n",ib, testHist.size());
		for (int i=0;i<testHist.size();i++)
		{
			if (testHist[i]!=0)
				printf("testHist[%d]=%d\n",i,testHist[i]);
		}
#endif //DEBUG_RADIXSORT
		btSwap(src, dst );
		btSwap(srcHisto,destHisto);

#ifdef DEBUG_RADIXSORT2
        keyValuesInOut.copyToHost(test2);
        printf("numElem = %d\n",test2.size());
        for (int i=0;i<test2.size();i++)
        {
            if (test2[i].m_key != test2[i].m_value)
            {
                printf("test2[%d].m_key=%d\n",i,test2[i].m_key);
                printf("test2[%d].m_value=%d\n",i,test2[i].m_value);
            }
        }
#endif //DEBUG_RADIXSORT2
        
        count++;
                
        
	}
	
   
    
	if (count&1)
	{
		btAssert(0);//need to copy from workbuffer to keyValuesInOut
	}

	if (m_workBuffer4->size())
	{
		m_workBuffer4->resize(originalSize);
		keyValuesInOut.copyFromOpenCLArray(*m_workBuffer4);
	}


#ifdef DEBUG_RADIXSORT
    keyValuesInOut.copyToHost(test2);
   
    printf("numElem = %d\n",test2.size());
    for (int i=0;i<test2.size();i++)
    {
        printf("test2[%d].m_key=%d\n",i,test2[i].m_key);
        printf("test2[%d].m_value=%d\n",i,test2[i].m_value);
    }
#endif    
	
}
