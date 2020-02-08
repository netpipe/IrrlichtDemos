#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(Game* game,
	const irr::core::stringc& name,
	const irr::core::stringc& meshPath,
	const irr::core::stringc& texture,
	const irr::core::vector3df& position,
	const irr::core::vector3df& rotation,
	const irr::core::vector3df& scale,
		
	const irr::f32 density, const irr::f32 weight,
	const bool createBody = true, const bool gravity = true
) :
	IrrlichtObject(game, name, meshPath, texture, position, rotation, scale),
	OdeObject(density, weight),

	game(game)
{
	init(createBody, gravity);
}

void GameObject::update(const dBodyID body)
{
	irr::scene::ISceneNode* node = ((GameObject*)dBodyGetData(body))->node;
   	if (!node) return;

   	const irr::core::matrix4& mat = getTransformationMatrix(body);
   	node->setRotation(mat.getRotationDegrees());
   	node->setPosition(mat.getTranslation());
}

const irr::core::matrix4 GameObject::getTransformationMatrix(const dBodyID body)
{
	const dReal* p = dBodyGetPosition(body);
	const dReal* r = dBodyGetRotation(body);

	irr::f32 m[16];
	m[ 0] = r[ 0]; m[ 1] = r[ 4]; m[ 2] = r[ 8]; m[ 3] = 0;
	m[ 4] = r[ 1]; m[ 5] = r[ 5]; m[ 6] = r[ 9]; m[ 7] = 0;
	m[ 8] = r[ 2]; m[ 9] = r[ 6]; m[10] = r[10]; m[11] = 0;
	m[12] = p[ 0]; m[13] = p[ 1]; m[14] = p[ 2]; m[15] = 1;

	irr::core::matrix4 mat;
	mat.setM(m);

	return mat;
}


const void GameObject::makeTriMeshData()
{
	irr::u32 indexcount = 0;
	irr::u32 vertexcount = 0;

	// count vertices and indices
	irr::u32 i;
	for (i = 0; i < mesh->getMeshBufferCount(); i++)
    {
		const irr::scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);

		indexcount += mb->getIndexCount();
		vertexcount += mb->getVertexCount();
	}

	// build structure for ode trimesh geom
	this->vertices = new dVector4[vertexcount];
	this->indices = new dTriIndex[indexcount];

	irr::u32 i_off = 0, i_cnt = 0, v_off = 0;

	for (irr::u32 i = 0; i < mesh->getMeshBufferCount(); ++i)
    {
		const irr::scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);
		const irr::u16* mb_indices = mb->getIndices();

		for (irr::u32 j = 0; j < mb->getIndexCount(); ++j)
			indices[i_off++] = i_cnt + mb_indices[j];

		i_cnt += mb->getIndexCount();

		const irr::core::vector3df& scale = this->node->getScale();

		if (mb->getVertexType() == irr::video::EVT_STANDARD)
		{
			const irr::video::S3DVertex* mb_vertices = (irr::video::S3DVertex*)mb->getVertices();
			irr::u32 j;
			for(j = 0; j < mb->getVertexCount(); ++j)
			{
				vertices[v_off][0] = mb_vertices[j].Pos.X*scale.X;
				vertices[v_off][1] = mb_vertices[j].Pos.Y*scale.Y;
				vertices[v_off][2] = mb_vertices[j].Pos.Z*scale.Z;
				v_off++;
			}
		}
		else if (mb->getVertexType() == irr::video::EVT_2TCOORDS)
		{
			const irr::video::S3DVertex2TCoords* mb_vertices = (irr::video::S3DVertex2TCoords*)mb->getVertices();
			irr::u32 j;
			for (j = 0; j < mb->getVertexCount(); ++j)
			{
				vertices[v_off][0] = mb_vertices[j].Pos.X*scale.X;
				vertices[v_off][1] = mb_vertices[j].Pos.Y*scale.Y;
				vertices[v_off][2] = mb_vertices[j].Pos.Z*scale.Z;
				v_off++;
			}
		}
    }

	this->data = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple(this->data, (const dReal*)this->vertices, vertexcount, (const dTriIndex*)this->indices, indexcount);
}

const void GameObject::init(const bool createBody, const bool gravityMode)
{
	const irr::core::vector3df& pos = this->node->getPosition();
	const irr::core::vector3df& rotationRAD = (this->node->getRotation()*irr::core::DEGTORAD);
	const irr::core::quaternion irrQuat(rotationRAD);
	dQuaternion odeQuat;
	odeQuat[0] = irrQuat.W;
	odeQuat[1] = irrQuat.X;
	odeQuat[2] = irrQuat.Y;
	odeQuat[3] = irrQuat.Z;

	if (createBody)
	{
		this->body = dBodyCreate(this->game->getPhysics()->getWorld());
		dBodySetData(this->body, this);

		this->internalSetUpMass();
		this->mass.adjust(this->weight);
		dBodySetMass(this->body, &this->mass);
		dBodySetPosition(this->body, pos.X, pos.Y, pos.Z);
		dBodySetQuaternion(this->body, odeQuat);

		dBodySetGravityMode(this->body, gravityMode);
		dBodySetMovedCallback(this->body, &this->update);
	}

	this->internalCreateGeom();
	dGeomSetData(this->geom, this);
	dGeomSetBody(this->geom, this->body);
	dGeomSetPosition(this->geom, pos.X, pos.Y, pos.Z);
	dGeomSetQuaternion(this->geom, odeQuat);
}

const void GameObject::internalSetUpMass()
{
	const irr::core::vector3df& extent = (this->node->getBoundingBox()).getExtent();
	this->mass.setBox(this->density, extent.X, extent.Y, extent.Z);		
}

const void GameObject::internalCreateGeom()
{
	makeTriMeshData();
	this->geom = dCreateTriMesh(this->game->getPhysics()->getSpace(), this->data, 0, 0, 0);
}
