#include "World.h"
#include "Car.h"
#include <irrXML.h>
#include <ode/ode.h>

using namespace irr;
using namespace irr::io;
using namespace scene;
using namespace core;

const irr::io::path	c_models_dir = "./models/";
const irr::io::path	c_textures_dir = "./textures/";

/**
*	Perform computing physics
*/
class CPhysicSceneAnimator :
	public irr::scene::ISceneNodeAnimator
{
private:
	CWorld *m_world;

public:
	CPhysicSceneAnimator(CWorld *world) : m_world(world) {}

	//! Animates a scene node.
	/** \param node Node to animate.
	\param timeMs Current time in milli seconds. */
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs) {
		m_world->animate((float)timeMs * 0.00001f);
	}

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer after calling this. */
	virtual ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node,
			irr::scene::ISceneManager* newManager=0) {
		return (new CPhysicSceneAnimator(m_world));
	}
};


CWorld * CWorld::m_instance = NULL;

class GroundShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

        virtual void OnSetConstants(video::IMaterialRendererServices* services,
                        s32 userData)
        {
                video::IVideoDriver* driver = services->getVideoDriver();

                // set clip matrix
                core::matrix4 worldViewProj;
                worldViewProj = driver->getTransform(video::ETS_PROJECTION);
                worldViewProj *= driver->getTransform(video::ETS_VIEW);
                worldViewProj *= driver->getTransform(video::ETS_WORLD);

                services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

                // set index map steps variables
				vector3df indexSteps(16.0f, 16.0f,0.0f);
				services->setVertexShaderConstant("index_steps", &indexSteps.X, 2);

				// set map steps variables
				vector3df steps(4.0f, 4.0f,0.0f);
				services->setVertexShaderConstant("steps", &steps.X, 2);

				// set samplers
				int sampler = 0;
				services->setVertexShaderConstant("indexMap", (const f32*)&sampler, 1);

				sampler = 1;
				services->setVertexShaderConstant("map", (const f32*)&sampler, 1);
        }
};

CWorld* CWorld::create(irr::scene::ISceneManager * smgr, irr::io::IFileSystem * fs)
{
	return (m_instance ? m_instance : (m_instance = new CWorld(smgr, fs)));
}

CWorld::CWorld(irr::scene::ISceneManager * smgr, irr::io::IFileSystem * fs)
	: m_scene_manager(smgr), m_file_system(fs), m_running(false), m_ground(NULL)
{
	video::IVideoDriver * driver = smgr->getVideoDriver();

	// create ODE world, space and contact joint group
	m_world = dWorldCreate();
	m_contactgroup = dJointGroupCreate(0);
	m_space = dHashSpaceCreate(0);
	dSpaceSetCleanup(m_space, 1);

	dWorldSetGravity(m_world, 0.0f, -0.025f, 0.0f);

	// setup ambient lighting
	m_scene_manager->setAmbientLight(video::SColorf(0.5f, 0.5f, 0.5f));
	scene::ILightSceneNode *sun = m_scene_manager->addLightSceneNode(0, core::vector3df(10.0f, 10.0f, 10.0f));
	sun->setLightType(video::ELT_DIRECTIONAL);

	// setup skybox
	smgr->addSkyBoxSceneNode(driver->getTexture(c_textures_dir + "irrlicht2_up.jpg"),
														  driver->getTexture(c_textures_dir + "irrlicht2_dn.jpg"),
														  driver->getTexture(c_textures_dir + "irrlicht2_lf.jpg"),
														  driver->getTexture(c_textures_dir + "irrlicht2_rt.jpg"),
														  driver->getTexture(c_textures_dir + "irrlicht2_ft.jpg"),
														  driver->getTexture(c_textures_dir + "irrlicht2_bk.jpg"));

	// create ground
	smgr->addHillPlaneMesh("ground", core::dimension2d<f32>(50.0f, 50.0f),
															core::dimension2d<u32>(1, 1),
															0,0.0f,
															core::dimension2d<f32>(0.0f,0.0f),
															core::dimension2d<f32>(1.0f,1.0f));
	// create scene animator
	CPhysicSceneAnimator * animator = new CPhysicSceneAnimator(this);
	smgr->getRootSceneNode()->addAnimator(animator);
	animator->drop();

	// create world root
	m_world_root = m_scene_manager->addEmptySceneNode();
	m_cars_root = m_scene_manager->addEmptySceneNode();
}


CWorld::~CWorld(void)
{
	// clear ODE
	dJointGroupDestroy(m_contactgroup);
	dSpaceDestroy(m_space);
	dWorldDestroy(m_world);

	m_world_root->removeAll();
	m_cars_root->removeAll();

	m_instance = NULL;
}

void CWorld::collusionCallback(void *data, dGeomID o1, dGeomID o2)
{
	int i,n;

	const int N = 10;
	dContact contact[N];
	n = dCollide(o1,o2, N, &contact[0].geom, sizeof(dContact));
	if (n > 0) {
		for (i = 0; i < n; i++) {
			CCar *car1 = NULL,
				 *car2 = NULL;

			if (contact[i].geom.g1 != m_instance->m_ground && contact[i].geom.g2 != m_instance->m_ground)
			{
				car1 = (CCar*)dGeomGetData(contact[i].geom.g1);
				car2 = (CCar*)dGeomGetData(contact[i].geom.g2);

				if (car1)
					car1->damage();
				if (car2)
					car2->damage();
			}

			if (car1 || car2)
			{
				contact[i].surface.mode = dContactBounce | dContactSoftERP | dContactSoftCFM;
				contact[i].surface.bounce = 1.0f;
			}
			else
			{
				contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;
				contact[i].surface.mu = dInfinity;
				contact[i].surface.slip1 = 0.1f;
				contact[i].surface.slip2 = 0.1f;
			}

			contact[i].surface.soft_erp = 0.5f;
			contact[i].surface.soft_cfm = 0.3f;
			dJointID c = dJointCreateContact(m_instance->m_world,m_instance->m_contactgroup,&contact[i]);
			dJointAttach(c, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));


		}
	}
}

CCar* CWorld::createCar(const irr::io::path &path,const irr::core::vector3df &position,const irr::core::vector3df &rotation)
{
	float mass;
	core::vector3df node_position;

	video::IVideoDriver *driver = m_scene_manager->getVideoDriver();
	scene::IMeshCache * cache = m_scene_manager->getMeshCache();

	// create xml reader object
	IrrXMLReader *xml = createIrrXMLReader(path.c_str());

	//if (!xml)
//		return false;

	// get directory of file, this will be used during loading resources from that file
	irr::io::path dae_path, dae_base;

	// create new car object
	CCar * car = new CCar(this);

	// read xml elements
	while (xml->read())
	{
		if (xml->getNodeType() == EXN_ELEMENT)
		{
			if (strcmp(xml->getNodeName(), "collada") == 0)
			{
				dae_path = m_file_system->getAbsolutePath(c_models_dir + xml->getAttributeValue("path"));
				dae_base = dae_path + "#geom-";

				m_scene_manager->getMesh(dae_path);
			}
			if (strcmp(xml->getNodeName(), "clean") == 0)
			{
				// load clean car
				scene::IMesh * clean_model = cache->getMeshByName(dae_base + xml->getAttributeValue("model"));
				video::ITexture * clean_diffuse_map = driver->getTexture(c_textures_dir + xml->getAttributeValue("diffuse_map"));

				sscanf(xml->getAttributeValue("position"), "%f %f %f", &node_position.X, &node_position.Y, &node_position.Z);

				sscanf(xml->getAttributeValue("mass"), "%f", &mass);

				car->setChassis(clean_model, clean_diffuse_map, node_position, mass);

				// load glass
				scene::IMesh *glass = cache->getMeshByName(dae_base + xml->getAttributeValue("glass_model"));
				car->setGlass(glass);
			}
			else
			if (strcmp(xml->getNodeName(), "damaged") == 0)
			{
				// load damaged car
				scene::IMesh * damaged_model = m_scene_manager->getMesh(dae_base + xml->getAttributeValue("model"));
				video::ITexture * damaged_diffuse_map = driver->getTexture(c_textures_dir + xml->getAttributeValue("diffuse_map"));

				car->setDamagedChassis(damaged_model, damaged_diffuse_map);
			}
			else
			if (strcmp(xml->getNodeName(), "wheels") == 0)
			{
				// load wheel
				// we give model for front and back wheels (left or right doen't matter), but they must be poses in right place
				// according the car
				scene::IMesh *wheel = m_scene_manager->getMesh(dae_base + xml->getAttributeValue("model"));
				video::ITexture *wheel_diffuse_map = driver->getTexture(c_textures_dir + xml->getAttributeValue("diffuse_map"));

				sscanf(xml->getAttributeValue("mass"), "%f", &mass);

				car->setWheel(wheel, wheel_diffuse_map);

				sscanf(xml->getAttributeValue("f_l_wheel"), "%f %f %f", &node_position.X, &node_position.Y, &node_position.Z);
				car->addWheel(CCar::FRONT_LEFT, node_position, mass);

				sscanf(xml->getAttributeValue("f_r_wheel"), "%f %f %f", &node_position.X, &node_position.Y, &node_position.Z);
				car->addWheel(CCar::FRONT_RIGHT, node_position, mass);

				sscanf(xml->getAttributeValue("b_l_wheel"), "%f %f %f", &node_position.X, &node_position.Y, &node_position.Z);
				car->addWheel(CCar::BACK_LEFT, node_position, mass);

				sscanf(xml->getAttributeValue("b_r_wheel"), "%f %f %f", &node_position.X, &node_position.Y, &node_position.Z);
				car->addWheel(CCar::BACK_RIGHT, node_position, mass);
			}
		}
	}

	delete xml;

	car->setPosition(position);
	car->setRotation(rotation);

	return car;
}

void CWorld::animate(float time)
{
	if (!m_running)
		return;

	dSpaceCollide(m_space, 0, &collusionCallback);
	dWorldStep(m_world, time);
	dJointGroupEmpty(m_contactgroup);
}

//! create ODE box based on AABB
dGeomID	CWorld::createPhysicBox(const irr::scene::IMesh * mesh, const vector3df &pos)
{
	const irr::core::aabbox3df &aabb = mesh->getBoundingBox();
	irr::core::vector3df extend = aabb.getExtent();

	dGeomID box = dCreateBox(m_space, extend.X, extend.Y, extend.Z);
	dGeomSetPosition(box, pos.X, pos.Y+extend.Y*0.5f, pos.Z);

	return box;
}

//! create ODE TriMesh based on IMesh
dGeomID	CWorld::createPhysicMesh(const irr::scene::IMesh * mesh, const vector3df &pos){
	// do nothing if the mesh or node is NULL
	if(!mesh) return NULL;
	u32 i,j,ci,cif,cv;
	u32 indexcount=0;
	u32 vertexcount=0;

	// count vertices and indices
	for(i=0;i < mesh->getMeshBufferCount();i++){
		irr::scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);
		indexcount += mb->getIndexCount();
		vertexcount += mb->getVertexCount();
	}

	// build structure for ode trimesh geom
	dVector3 * vertices=new dVector3[vertexcount];
	dTriIndex * indices=new dTriIndex[indexcount];

	// fill trimesh geom
	ci=0; // current index in the indices array
	cif=0; // offset of the irrlicht-vertex-index in the vetices array
	cv=0; // current index in the vertices array
	for(i=0;i< mesh->getMeshBufferCount();i++){
		irr::scene::IMeshBuffer* mb = mesh->getMeshBuffer(i);

		// fill indices
		irr::u16* mb_indices=mb->getIndices();
		for(j=0; j < mb->getIndexCount(); j++){
			// scale the indices from multiple meshbuffers to single index array
			indices[ci]=cif+mb_indices[j];
			ci++;
		}
		// update the offset for the next meshbuffer
		cif=cif+mb->getVertexCount();
		// fill vertices
		if(mb->getVertexType()==irr::video::EVT_STANDARD){
			irr::video::S3DVertex* mb_vertices = (irr::video::S3DVertex*)mb->getVertices();
			for(j=0;j<mb->getVertexCount();j++){
				vertices[cv][0]=mb_vertices[j].Pos.X;
				vertices[cv][1]=mb_vertices[j].Pos.Y;
				vertices[cv][2]=mb_vertices[j].Pos.Z;
				cv++;
			}
		}
		else
		if(mb->getVertexType()==irr::video::EVT_2TCOORDS){
			irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
			for(j=0;j<mb->getVertexCount();j++){
				vertices[cv][0]=mb_vertices[j].Pos.X;
				vertices[cv][1]=mb_vertices[j].Pos.Y;
				vertices[cv][2]=mb_vertices[j].Pos.Z;
				cv++;
			}
		}
	}
	// build the trimesh data
	dTriMeshDataID data=dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple(data,(dReal*)vertices, vertexcount, indices, indexcount);

	// build the trimesh geom
	dGeomID geom=dCreateTriMesh(m_space,data,0,0,0);

	// set the geom position
	dGeomSetPosition(geom,pos.X,pos.Y,pos.Z);

	return geom;
}

void CWorld::clearScene()
{
	m_world_root->removeAll();
	m_cars_root->removeAll();

	// remove all geoms
	dSpaceDestroy(m_space);
	m_space = dHashSpaceCreate(0);
}

bool CWorld::loadScene(const irr::io::path &path)
{
	video::IVideoDriver *driver = m_scene_manager->getVideoDriver();

	// clear
	clearScene();

	// create xml reader object
	IrrXMLReader *xml = createIrrXMLReader(path.c_str());

	if (!xml)
		return false;

	// get directory of file, this will be used during loading resources from that file
	irr::io::path dae_path, dae_base;

	// read xml elements
	while (xml->read())
	{
		if (xml->getNodeType() == EXN_ELEMENT)
		{
			if (strcmp(xml->getNodeName(), "collada") == 0)
			{
				dae_path = m_file_system->getAbsolutePath(c_models_dir + xml->getAttributeValue("path"));
				dae_base = dae_path + "#geom-";

				m_scene_manager->getMesh(dae_path);
			}
			else if (strcmp(xml->getNodeName(), "camera") == 0)
			{
				sscanf(xml->getAttributeValue("position"), "%f %f %f", &m_default_camera_pos.X, &m_default_camera_pos.Y, &m_default_camera_pos.Z);
				sscanf(xml->getAttributeValue("target"), "%f %f %f", &m_default_camera_target.X, &m_default_camera_target.Y, &m_default_camera_target.Z);
			}
			else
			if (strcmp(xml->getNodeName(), "car") == 0)
			{
				core::vector3df pos(0.0f, 0.0f, 0.0f),
							    rot(0.0f, 0.0f, 0.0f);

				if (xml->getAttributeValue("position"))
					sscanf(xml->getAttributeValue("position"), "%f %f %f", &pos.X,&pos.Y,&pos.Z);
				if (xml->getAttributeValue("rotation"))
					sscanf(xml->getAttributeValue("rotation"), "%f %f %f", &rot.X,&rot.Y,&rot.Z);

				createCar(xml->getAttributeValue("path"), pos, rot);
			}
			if (strcmp(xml->getNodeName(), "node") == 0)
			{
				// load damaged car
				scene::IMesh * model = m_scene_manager->getMesh(dae_base + xml->getAttributeValue("model"));
				scene::IMesh * collusion_model = model;
				video::ITexture * diffuse_map = NULL;

				if (xml->getAttributeValue("diffuse_map"))
					diffuse_map = driver->getTexture(c_textures_dir + xml->getAttributeValue("diffuse_map"));

				// load collusion model
				if (xml->getAttributeValue("collusion_model"))
					collusion_model = m_scene_manager->getMesh(dae_base + xml->getAttributeValue("collusion_model"));

				core::vector3df pos(0.0f, 0.0f, 0.0f),
							    rot(0.0f, 0.0f, 0.0f),
								scale(1.0f, 1.0f, 1.0f);
				ISceneNode * node;

				if (xml->getAttributeValue("occlusion_type") != NULL && (strcmp(xml->getAttributeValue("occlusion_type"), "octree") == 0))
				{
					node = m_scene_manager->addOctreeSceneNode(model, m_world_root);
				}
				else
				{
					if (xml->getAttributeValue("position"))
						sscanf(xml->getAttributeValue("position"), "%f %f %f", &pos.X,&pos.Y,&pos.Z);
					if (xml->getAttributeValue("rotation"))
						sscanf(xml->getAttributeValue("rotation"), "%f %f %f", &rot.X,&rot.Y,&rot.Z);
					if (xml->getAttributeValue("scale"))
						sscanf(xml->getAttributeValue("scale"), "%f %f %f", &scale.X,&scale.Y,&scale.Z);

					node = m_scene_manager->addMeshSceneNode(model,m_world_root,-1,pos, rot, scale);
				}

				if (xml->getAttributeValue("map_tiling") != NULL)
				{
					sscanf(xml->getAttributeValue("map_tiling"), "%f %f", &scale.X,&scale.Y);

					node->getMaterial(0).getTextureMatrix(0).setTextureScale(scale.X, scale.Y);
				}

				node->setMaterialFlag(video::EMF_TEXTURE_WRAP, false);

				if (strcmp(xml->getAttributeValue("lighting"), "false") == 0)
					node->setMaterialFlag(video::EMF_LIGHTING, false);
				else
					node->setMaterialFlag(video::EMF_LIGHTING, true);

				node->setName(xml->getAttributeValue("name"));
				if (diffuse_map)
					node->setMaterialTexture(0, diffuse_map);

				dGeomID static_geom = NULL;

				if (xml->getAttributeValue("geom") && (strcmp(xml->getAttributeValue("geom"), "trimesh") == 0))
					static_geom = createPhysicMesh(collusion_model, pos);
				else
					static_geom = createPhysicBox(collusion_model, pos);

				if (xml->getAttributeValue("ground"))
					m_ground = static_geom;
			}
		}
	}

	delete xml;

	return true;
}
