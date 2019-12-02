#version 130

varying vec4 vertex;
varying float scale;

void main()
{
	// for the fragment
	gl_TexCoord[0].st = vec2(gl_MultiTexCoord0);
	// the model view matrix (from world to screen) have a translation transform and a rotation transform.
	// the rotation transform is the mat3 of the center: columns 0, 1, 2 and lines 0, 1, 2
	// to abort rotation of the object in view, we set this matrix to identity matrix.
	// The identity matrix has in factor, the scale coefficient.

	mat4 ModelView = gl_ModelViewMatrix;
	mat3 transposed = transpose(mat3(ModelView));
	// matrix[col][lin]
	float scale = length(transposed[0]);
	ModelView[0][0] = scale;
	ModelView[0][1] = 0.;
	ModelView[0][2] = 0.;
	
	ModelView[1][0] = 0.;
	ModelView[1][1] = scale;
	ModelView[1][2] = 0.;
	
	ModelView[2][0] = 0.;
	ModelView[2][1] = 0.;
	ModelView[2][2] = scale;
	
	vertex = ModelView * gl_Vertex;
	gl_Position = gl_ProjectionMatrix * ModelView * gl_Vertex;
}