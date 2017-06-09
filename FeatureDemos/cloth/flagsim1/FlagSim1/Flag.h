
#include "vectors.h"

/***************************

  This file defines the class
  CFlag which does the complete
  flag simulation.

***************************/

struct SVertex
{
	SF3dVector pos;
	SF3dVector normal;
	SF2dVector TexCoords;
};

class CFlag
{
protected:
	bool m_Initialized;
	int m_DetailX, m_DetailY;
	float m_spring_const;
	float m_Damping;

	float m_DirectDistance;
	float m_AslantDistance;

	SVertex * m_Vertices;
	int m_NumVertices;

	SF3dVector * m_Velocity; //same length like m_Vertices

	GLuint * m_Indices;
	int m_NumIndices;

private:
	SF3dVector GetForce(int xVertex, int yVertex, SF3dVector OtherVertexPos, float StandardDistance);
	void UpdateNormals();


public:
	CFlag();
	void Initialize(int DetailX, int DetailY, float FlagSize, float spring_const, float damping);

	void UpdateVertices( float dtime, SF3dVector gravity, SF3dVector wind);
	void Render();
	
};