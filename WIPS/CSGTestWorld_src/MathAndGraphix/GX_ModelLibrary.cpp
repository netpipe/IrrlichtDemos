
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_ModelLibrary.h"

#include "GX_Model.h"

#include "ML_Matrix.h"

gxModel * ModelMaker::MakeJetModel()
{
	gxModel * jetModel = new gxModel;

	jetModel->m_triangles.clear();

	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(255, 255, 10),
			mlTriangle(
				mlVector3D(0.0f, 0.0f, 60.0f),
				mlVector3D(-15.0f, 0.0f, 30.0f),
				mlVector3D(15.0f,0.0f,30.0f))));

	// These two triangles form the nose of the plane
	// ***********************************************

	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(120,32,0),
			mlTriangle(
				mlVector3D(15.0f,0.0f,30.0f),
				mlVector3D(0.0f, 15.0f, 30.0f),
				mlVector3D(0.0f, 0.0f, 60.0f))));

	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(255,0,0),
			mlTriangle(
				mlVector3D(0.0f, 0.0f, 60.0f),
				mlVector3D(0.0f, 15.0f, 30.0f),
				mlVector3D(-15.0f,0.0f,30.0f))));
		
	// ***********************************************

	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(0,255,0),
			mlTriangle(
				mlVector3D(-15.0f,0.0f,30.0f),
				mlVector3D(0.0f, 15.0f, 30.0f),
				mlVector3D(0.0f, 0.0f, -56.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(255,255,0),
			mlTriangle(
				mlVector3D(0.0f, 0.0f, -56.0f),
				mlVector3D(0.0f, 15.0f, 30.0f),
				mlVector3D(15.0f,0.0f,30.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(0, 255, 255),
			mlTriangle(
				mlVector3D(15.0f,0.0f,30.0f),
				mlVector3D(-15.0f, 0.0f, 30.0f),
				mlVector3D(0.0f, 0.0f, -56.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(32,128,255),
			mlTriangle(
				mlVector3D(0.0f,2.0f,27.0f),
				mlVector3D(-60.0f, 2.0f, -8.0f),
				mlVector3D(60.0f, 2.0f, -8.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(64,64,64),
			mlTriangle(
				mlVector3D(60.0f, 2.0f, -8.0f),
				mlVector3D(0.0f, 7.0f, -8.0f),
				mlVector3D(0.0f,2.0f,27.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(192,192,192),
			mlTriangle(
				mlVector3D(60.0f, 2.0f, -8.0f),
				mlVector3D(-60.0f, 2.0f, -8.0f),
				mlVector3D(0.0f,7.0f,-8.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(64,64,64),
			mlTriangle(
				mlVector3D(0.0f,2.0f,27.0f),
				mlVector3D(0.0f, 7.0f, -8.0f),
				mlVector3D(-60.0f, 2.0f, -8.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(255,128,255),
			mlTriangle(
				mlVector3D(-30.0f, -0.50f, -57.0f),
				mlVector3D(30.0f, -0.50f, -57.0f),
				mlVector3D(0.0f,-0.50f,-40.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(255,128,0),
			mlTriangle(
				mlVector3D(0.0f,-0.5f,-40.0f),
				mlVector3D(30.0f, -0.5f, -57.0f),
				mlVector3D(0.0f, 4.0f, -57.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(255,128,0),
			mlTriangle(
				mlVector3D(0.0f, 4.0f, -57.0f),
				mlVector3D(-30.0f, -0.5f, -57.0f),
				mlVector3D(0.0f,-0.5f,-40.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(121,12,64),
			mlTriangle(
				mlVector3D(30.0f,-0.5f,-57.0f),
				mlVector3D(-30.0f, -0.5f, -57.0f),
				mlVector3D(0.0f, 4.0f, -57.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(255,0,0),
			mlTriangle(
				mlVector3D(0.0f,0.5f,-40.0f),
				mlVector3D(3.0f, 0.5f, -57.0f),
				mlVector3D(0.0f, 25.0f, -65.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(255,0,0),
			mlTriangle(
				mlVector3D(0.0f, 25.0f, -65.0f),
				mlVector3D(-3.0f, 0.5f, -57.0f),
				mlVector3D(0.0f,0.5f,-40.0f))));
	
	jetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(32,32,128),
			mlTriangle(
				mlVector3D(3.0f,0.5f,-57.0f),
				mlVector3D(-3.0f, 0.5f, -57.0f),
				mlVector3D(0.0f, 25.0f, -65.0f))));
	
	mlTransform transform = mlTransformIdentity;
	transform.ApplyRotation(mlQuaternion(mlAxisX, mlDegrees(90.0f)));
	transform.ApplyRotation(mlQuaternion(mlAxisZ, mlDegrees(90.0f)));
	
	jetModel->m_correctionalTransform = transform;
	
	return jetModel;
}

gxModel * ModelMaker::MakeGridModel(const mlVector3D & corner, mlFloat length, mlFloat width, mlFloat spacing, mlVector3D normal)
{
	gxModel * gridModel = new gxModel;

	gridModel->m_triangles.clear();

	mlMatrix3x3 planeRotation;
	mlMatrixUtility::MatrixFromVectorRotation(planeRotation, mlVector3D(0.0f, 0.0f, 1.0f), normal);

	for(mlFloat x = 0.0f; x <= width; x += spacing)
	{
		mlVector3D pointA = planeRotation.Transform(mlVector3D(x, 0.0f, 0.0f));
		mlVector3D pointB = planeRotation.Transform(mlVector3D(x, length, 0.0f));

		gridModel->m_lines.push_back(
			gxRenderingLine(
				gxColor(90, 20, 150),
				corner + planeRotation.Transform(mlVector3D(x, 0.0f, 0.0f)),
				corner + planeRotation.Transform(mlVector3D(x, length, 0.0f))));
	}

	for(mlFloat y = 0.0f; y <= length; y += spacing)
	{
		mlVector3D pointA = planeRotation.Transform(mlVector3D(0.0f, y, 0.0f));
		mlVector3D pointB = planeRotation.Transform(mlVector3D(width, y, 0.0f));

		gridModel->m_lines.push_back(
			gxRenderingLine(
				gxColor(90, 20, 150),
				corner + planeRotation.Transform(mlVector3D(0.0f, y, 0.0f)),
				corner + planeRotation.Transform(mlVector3D(width, y, 0.0f))));
	}

	return gridModel;
}

gxModel * ModelMaker::MakeSheetModel(const mlVector3D & corner, mlFloat length, mlFloat width)
{
	gxModel * sheetModel = new gxModel;

	sheetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(50, 0, 70, 100),
			mlTriangle(
				corner,
				corner + mlVector3D(length, 0.0f, 0.0f),
				corner + mlVector3D(0.0f, width, 0.0f))));

	sheetModel->m_triangles.push_back(
		gxRenderingTriangle(
			gxColor(50, 0, 70, 100),
			mlTriangle(
				corner + mlVector3D(0.0f, width, 0.0f),
				corner + mlVector3D(length, 0.0f, 0.0f),
				corner + mlVector3D(length, width, 0.0f))));


	return sheetModel;
}

gxModel * ModelMaker::MakeCubeModel(const mlVector3D & corner, mlFloat length, const gxColor & color, const gxColor & lineColor)
{
	mlVector3D oppositeCorner = corner + mlVector3D(length, length, length);

	return MakeRectangleModel(corner, oppositeCorner, color, lineColor);
}

gxModel * ModelMaker::MakeRectangleModel(const mlVector3D & cornerA, const mlVector3D & cornerB, const gxColor & color, const gxColor & lineColor)
{
	gxModel * cubeModel = new gxModel;

	mlVector3D boxVertices[8];

	boxVertices[0] = mlVector3D(cornerA.x, cornerA.y, cornerA.z);
	boxVertices[1] = mlVector3D(cornerA.x, cornerA.y, cornerB.z);
	boxVertices[2] = mlVector3D(cornerA.x, cornerB.y, cornerB.z);
	boxVertices[3] = mlVector3D(cornerA.x, cornerB.y, cornerA.z);

	boxVertices[4] = mlVector3D(cornerB.x, cornerA.y, cornerA.z);
	boxVertices[5] = mlVector3D(cornerB.x, cornerA.y, cornerB.z);
	boxVertices[6] = mlVector3D(cornerB.x, cornerB.y, cornerB.z);
	boxVertices[7] = mlVector3D(cornerB.x, cornerB.y, cornerA.z);

	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[0],
				boxVertices[1])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[1],
				boxVertices[2])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[2],
				boxVertices[3])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[3],
				boxVertices[0])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[4],
				boxVertices[5])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[5],
				boxVertices[6])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[6],
				boxVertices[7])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[7],
				boxVertices[4])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[0],
				boxVertices[4])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[1],
				boxVertices[5])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[2],
				boxVertices[6])));
	cubeModel->m_lines.push_back(
		gxRenderingLine(
			lineColor,
			mlLine(
				boxVertices[3],
				boxVertices[7])));
				
	// todo : correct this, the winding order of some of these triangles is incorrect.
	// todo : auto generate any polytope
	// todo : when specifying a rectangle, check its normal against centroid of polytope
	// (just a dot product) if -ve, invert winding order.
	
	mlVector3D vBoxCentroid = (cornerA + cornerB) * 0.5f;
	
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[0],
				boxVertices[1],
				boxVertices[3]),
			vBoxCentroid,
			cubeModel,
			gxColorRed);
			
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[2],
				boxVertices[3],
				boxVertices[1]),
			vBoxCentroid,
			cubeModel,
			gxColorRed);

	AddRenderingTriangle(
			mlTriangle(
				boxVertices[2],
				boxVertices[7],
				boxVertices[6]),
			vBoxCentroid,
			cubeModel,
			color);
			
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[7],
				boxVertices[2],
				boxVertices[3]),
			vBoxCentroid,
			cubeModel,
			color);
			
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[4],
				boxVertices[5],
				boxVertices[7]),
			vBoxCentroid,
			cubeModel,
			color);
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[6],
				boxVertices[7],
				boxVertices[5]),
			vBoxCentroid,
			cubeModel,
			color);
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[4],
				boxVertices[5],
				boxVertices[1]),
			vBoxCentroid,
			cubeModel,
			color);
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[4],
				boxVertices[1],
				boxVertices[0]),
			vBoxCentroid,
			cubeModel,
			color);
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[0],
				boxVertices[7],
				boxVertices[4]),
			vBoxCentroid,
			cubeModel,
			color);
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[3],
				boxVertices[7],
				boxVertices[0]),
			vBoxCentroid,
			cubeModel,
			color);
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[2],
				boxVertices[5],
				boxVertices[6]),
			vBoxCentroid,
			cubeModel,
			color);
	AddRenderingTriangle(
			mlTriangle(
				boxVertices[2],
				boxVertices[5],
				boxVertices[1]),
			vBoxCentroid,
			cubeModel,
			color);

	return cubeModel;
}

void ModelMaker::AddRenderingTriangle(const mlTriangle & tTriangle, const mlVector3D & vPolytopeCentroid, gxModel * pModel, const gxColor & color)
{			
	mlVector3D vNormal = tTriangle.Normal();
	
	mlVector3D vCentroid = tTriangle.Centroid();
	
	mlFloat fDot = vNormal * (vCentroid - vPolytopeCentroid);
	
	//assert(fDot != 0.0f)
	
	if(fDot > 0.0f)
	{
		pModel->m_triangles.push_back(
			gxRenderingTriangle(
				color,
				tTriangle));
	}
	else
	{
		pModel->m_triangles.push_back(
			gxRenderingTriangle(
				color,
				tTriangle.Invert()));
	}

}
