#include "TerrainNode.h"

SceneGraphNodes::TerrainNode::TerrainNode(const GLuint &VBO, const GLuint &EBO, const unsigned int &Size)
{
    vboID = VBO;
    eboID = EBO;
    size = Size;
}

SceneGraphNodes::TerrainNode::~TerrainNode()
{
    if (parent)
        parent->delayRemoveChild(this);
}

void SceneGraphNodes::TerrainNode::execute()
{
    cleanup();

	glBindBuffer ( GL_ARRAY_BUFFER, vboID );

	glVertexPointer ( 3, GL_FLOAT, 5 * sizeof ( GLfloat ), ((GLubyte*)NULL + 0 ));
	glTexCoordPointer ( 2, GL_FLOAT, 5 * sizeof ( GLfloat ), ((GLubyte*)NULL + (3 * sizeof(GLfloat))));

	glEnableClientState ( GL_VERTEX_ARRAY );
	glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, eboID );

	glDrawElements ( GL_QUADS, size, GL_UNSIGNED_INT, ((GLubyte*)NULL + 0));

	glDisableClientState ( GL_VERTEX_ARRAY );
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
}
