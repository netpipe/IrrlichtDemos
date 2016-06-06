#pragma once
#include "WorldObject.h"

class CBox :	public CWorldObject
{
public:
	CBox(const char* name);
};
class CConcave :	public CWorldObject
{
public:
	CConcave(const char* name);
};
class CCone :	public CWorldObject
{
public:
	CCone(const char* name);
};
class CConvex :	public CWorldObject
{
public:
	CConvex(const char* name);
};
class CCylinder :	public CWorldObject
{
public:
	CCylinder(const char* name);
};
class CPlane :	public CWorldObject
{
public:
	CPlane(const char* name);

};
class CSphere :	public CWorldObject
{
public:
	CSphere(const char* name);
};
class CTerrain :	public CWorldObject
{
public:
	CTerrain(const char* name);
};

class CCompound :	public CWorldObject
{
private:
	btCollisionShape* GetCollisionShape(const s32 shapeType, vector3df& extent);
public:
	CCompound(const char* name);

};
