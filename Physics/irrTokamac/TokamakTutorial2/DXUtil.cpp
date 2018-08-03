

#include "Main.h"

#include "DXUtil.h"


// Matrix for DX operations
//D3DXMATRIX mDXMatrix;
#define DXSTACKSIZE 40
D3DXMATRIX mDXMatrixStack[DXSTACKSIZE];
int mDXStackPointer=0;



bool BuildVertexBuffer(IDirect3DVertexBuffer8 **pNewBuff, strVertex *pVertexData, int iVertexCount)
{

	HRESULT hr;
	unsigned char *vb_vertices;

	//Create a vertex buffer to hold the data
	hr=gD3DDevice->CreateVertexBuffer(iVertexCount * sizeof(strVertex), //Size of memory to be allocated
								   D3DUSAGE_WRITEONLY,  //Specify write only, it's faster
								   D3D8T_CUSTOMVERTEX,  //Our custom vertex specifier (coordinates & a colour)
								   D3DPOOL_MANAGED,     //Tell DirectX to manage the memory of this resource
								   pNewBuff);              //Pointer to our Vertex Buffer, after this call
														//It will point to a valid vertex buffer
	if(FAILED(hr))
	{
		OutputDebugString("Error creating vertex buffer (BuildVertexBuffer)");
		exit(5);
	}


	if (pVertexData != NULL)
	{
		//Now we have our Vertex Buffers, but they're empty. To put our data into them
		//we Lock the Vertex Buffer so Direct3D knows we're modifying it, then we copy
		//our data in and Unlock it so Direct3D knows we're done.
		hr=(*pNewBuff)->Lock(0, //Offset, we want to start at the beginning
						 0, //SizeToLock, 0 means lock the whole thing
						 &vb_vertices, //If successful, this will point to the data in the VB
						 0);  //Flags, nothing special
		if(FAILED(hr))
		{
			OutputDebugString("Error Locking vertex buffer (BuildVertexBuffer)");
			exit(5);
		}

		//vb_vertices now points to our vertices inside the Vertex buffer, so
		//to fill in our VB, we copy to vb_vertices.
		memcpy(vb_vertices, pVertexData, iVertexCount * sizeof(strVertex) );

		//Unlock so Direct3D knows we're done and can do any behind-the-scenes magic required
		(*pNewBuff)->Unlock();
	}


	return true;
}


void ScaleVertexBuffer(IDirect3DVertexBuffer8 *pVertexBuff, float fScaleFactor, int iVertexCount)
{
	int i;
	strVertex *pVertexData;
	HRESULT hr;

	// Lock the vertex buffer
	hr=pVertexBuff->Lock(0, //Offset, we want to start at the beginning
					 0, //SizeToLock, 0 means lock the whole thing
					 (unsigned char**)&pVertexData, //If successful, this will point to the data in the VB
					 0);  //Flags, nothing special
	if(FAILED(hr))
	{
		OutputDebugString("Error Locking vertex buffer (ScaleVertexBuffer)");
		exit(5);
	}

	for (i=0; i<iVertexCount; i++)
	{
		pVertexData->fX *= fScaleFactor;
		pVertexData->fY *= fScaleFactor;
		pVertexData->fZ *= fScaleFactor;

		pVertexData += 1;
	}

	// Unlock the buffer
	pVertexBuff->Unlock();
}




// DX functions


void dxApplyMatrix(IDirect3DDevice8 *D3DDevice)
{
	// Set the transformation matrix for the device
	D3DDevice->SetTransform(D3DTS_WORLD, &mDXMatrixStack[mDXStackPointer]);
}


void dxLoadIdentity(void)
{
	// Load an identity matrix to the current matrix stack element
	D3DXMatrixIdentity(&mDXMatrixStack[mDXStackPointer]);
}


void dxTranslatef(float fX, float fY, float fZ)
{
	D3DXMATRIX matrixTemp;

	// Multiply the current matrix by the required translation matrix
	D3DXMatrixMultiply(&mDXMatrixStack[mDXStackPointer], D3DXMatrixTranslation(&matrixTemp, fX, fY, fZ), &mDXMatrixStack[mDXStackPointer]);
}


void dxRotatef(float fAngle, float fX, float fY, float fZ)
{
	D3DXMATRIX matrixTemp;

	// Vector representing the axis of rotation
	D3DXVECTOR3 rotMatrix(fX, fY, fZ);

	// Scale the angle from degrees to radians
	fAngle *= DEG2RAD;

	D3DXMatrixMultiply(&mDXMatrixStack[mDXStackPointer], D3DXMatrixRotationAxis(&matrixTemp, &rotMatrix, fAngle), &mDXMatrixStack[mDXStackPointer]);
}

void dxScalef(float fX, float fY, float fZ)
{
	D3DXMATRIX matrixTemp;

	// Scale the current matrix
	D3DXMatrixMultiply(&mDXMatrixStack[mDXStackPointer], D3DXMatrixScaling(&matrixTemp, fX, fY, fZ), &mDXMatrixStack[mDXStackPointer]);
}


bool dxPushMatrix(void)
{
	// Is there any room left on the stack?
	if (mDXStackPointer < DXSTACKSIZE-1)
	{
		// Yes, so increase the stack pointer
		mDXStackPointer += 1;

		// Copy the previous matrix to this stack position
		mDXMatrixStack[mDXStackPointer] = mDXMatrixStack[mDXStackPointer-1];

		// All ok
		return true;
	}

	// No space on stack
	return false;
}

bool dxPopMatrix(void)
{
	// Is there anything remaining on the stack?
	if (mDXStackPointer > 0)
	{
		// Yes, so decrease the stack pointer
		mDXStackPointer -= 1;

		// All ok
		return true;
	}

	// Nothing left on stack
	return false;
}


