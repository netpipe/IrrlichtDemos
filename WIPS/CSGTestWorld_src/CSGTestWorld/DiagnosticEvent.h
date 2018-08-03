
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef DIAGNOSTICEVENT_H
#define DIAGNOSTICEVENT_H

#include "GX_RenderingTriangle.h"

class gxModel;
class Object3D;

class DiagnosticEvent
{
public:
	DiagnosticEvent();
	virtual ~DiagnosticEvent();
	
	void Update();
	void Render();

	void Clear();
	void AddTriangle(const mlVector3D & v1, const mlVector3D & v2, const mlVector3D & v3, const gxColor & col);
	void AddLine(const mlVector3D & v1, const mlVector3D & v2, const gxColor & col);
	void SetText(const char format[], ...);
	void SetObjects(Object3D * pObj1, Object3D * pObj2);

	void SetObjectWithHighlightedTriangle(Object3D * pObj, int nTriIndex);

	gxModel * GetModel();

	gxModel * m_pObject1;
	gxModel * m_pObject2;

	
private:

	char m_sText[1024];
	gxModel * m_pModel;
};

#endif // DIAGNOSTICEVENT_H

