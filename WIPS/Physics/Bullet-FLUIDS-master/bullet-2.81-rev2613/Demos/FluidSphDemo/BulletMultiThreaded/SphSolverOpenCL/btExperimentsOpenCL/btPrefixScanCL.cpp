#include "btPrefixScanCL.h"
#include "btFillCL.h"
#ifdef _WIN32
#define BT_PREFIXSCAN_PROG_PATH "./Demos/FluidSphDemo/BulletMultiThreaded/SphSolverOpenCL/btExperimentsOpenCL/PrefixScanKernels.cl"
#else
#define BT_PREFIXSCAN_PROG_PATH "./Demos/FluidSphDemo/BulletMultiThreaded/SphSolverOpenCL/btExperimentsOpenCL/PrefixScanKernels.cl"
#endif

#include "btLauncherCL.h"
#include "btOpenCLUtils.h"
#include "PrefixScanKernelsCL.h"

btPrefixScanCL::btPrefixScanCL(cl_context ctx, cl_device_id device, cl_command_queue queue, int size)
:m_commandQueue(queue)
{
	const char* scanKernelSource = prefixScanKernelsCL;
	cl_int pErrNum;
	char* additionalMacros=0;

	m_workBuffer = new btOpenCLArray<unsigned int>(ctx,queue,size);
	cl_program scanProg = btOpenCLUtils::compileCLProgramFromString( ctx, device, scanKernelSource, &pErrNum,additionalMacros, BT_PREFIXSCAN_PROG_PATH);
	btAssert(scanProg);

	m_localScanKernel = btOpenCLUtils::compileCLKernelFromString( ctx, device, scanKernelSource, "LocalScanKernel", &pErrNum, scanProg,additionalMacros );
	btAssert(m_localScanKernel );
	m_blockSumKernel = btOpenCLUtils::compileCLKernelFromString( ctx, device, scanKernelSource, "TopLevelScanKernel", &pErrNum, scanProg,additionalMacros );
	btAssert(m_blockSumKernel );
	m_propagationKernel = btOpenCLUtils::compileCLKernelFromString( ctx, device, scanKernelSource, "AddOffsetKernel", &pErrNum, scanProg,additionalMacros );
	btAssert(m_propagationKernel );
}


btPrefixScanCL::~btPrefixScanCL()
{
	delete m_workBuffer;
	clReleaseKernel(m_localScanKernel);
	clReleaseKernel(m_blockSumKernel);
	clReleaseKernel(m_propagationKernel);
}

template<class T>
T btNextPowerOf2(T n)
{
	n -= 1;
	for(int i=0; i<sizeof(T)*8; i++)
		n = n | (n>>i);
	return n+1;
}

void btPrefixScanCL::execute(btOpenCLArray<unsigned int>& src, btOpenCLArray<unsigned int>& dst, int n, unsigned int* sum)
{
	
//	btAssert( data->m_option == EXCLUSIVE );
	const unsigned int numBlocks = (const unsigned int)( (n+BLOCK_SIZE*2-1)/(BLOCK_SIZE*2) );

	dst.resize(src.size());
	m_workBuffer->resize(src.size());

	btInt4 constBuffer;
	constBuffer.x = n;
	constBuffer.y = numBlocks;
	constBuffer.z = (int)btNextPowerOf2( numBlocks );

	btOpenCLArray<unsigned int>* srcNative = &src;
	btOpenCLArray<unsigned int>* dstNative = &dst;
	
	{
		btBufferInfoCL bInfo[] = { btBufferInfoCL( dstNative->getBufferCL() ), btBufferInfoCL( srcNative->getBufferCL() ), btBufferInfoCL( m_workBuffer->getBufferCL() ) };

		btLauncherCL launcher( m_commandQueue, m_localScanKernel );
		launcher.setBuffers( bInfo, sizeof(bInfo)/sizeof(btBufferInfoCL) );
		launcher.setConst(  constBuffer );
		launcher.launch1D( numBlocks*BLOCK_SIZE, BLOCK_SIZE );
	}

	{
		btBufferInfoCL bInfo[] = { btBufferInfoCL( m_workBuffer->getBufferCL() ) };

		btLauncherCL launcher( m_commandQueue, m_blockSumKernel );
		launcher.setBuffers( bInfo, sizeof(bInfo)/sizeof(btBufferInfoCL) );
		launcher.setConst( constBuffer );
		launcher.launch1D( BLOCK_SIZE, BLOCK_SIZE );
	}
	

	if( numBlocks > 1 )
	{
		btBufferInfoCL bInfo[] = { btBufferInfoCL( dstNative->getBufferCL() ), btBufferInfoCL( m_workBuffer->getBufferCL() ) };
		btLauncherCL launcher( m_commandQueue, m_propagationKernel );
		launcher.setBuffers( bInfo, sizeof(bInfo)/sizeof(btBufferInfoCL) );
		launcher.setConst( constBuffer );
		launcher.launch1D( (numBlocks-1)*BLOCK_SIZE, BLOCK_SIZE );
	}


	if( sum )
	{
		clFinish(m_commandQueue);
		dstNative->copyToHostPointer(sum,1,n-1,true);
	}

}