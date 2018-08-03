
#ifndef DXUTIL_H
#define DXUTIL_H


bool BuildVertexBuffer(IDirect3DVertexBuffer8 **pNewBuff, strVertex *pVertexData, int iVertexCount);
void ScaleVertexBuffer(IDirect3DVertexBuffer8 *pVertexBuff, float fScaleFactor, int iVertexCount);


// DX functions


void dxApplyMatrix(IDirect3DDevice8 *D3DDevice);
void dxLoadIdentity(void);
void dxTranslatef(float fX, float fY, float fZ);
void dxRotatef(float fAngle, float fX, float fY, float fZ);
void dxScalef(float fX, float fY, float fZ);
bool dxPushMatrix(void);
bool dxPopMatrix(void);
#define DEG2RAD 0.01745f

#endif



