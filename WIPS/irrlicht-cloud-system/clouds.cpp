#include <irrlicht.h>
#include "clouds.h"
#include "common.h"
#include <time.h>
#include <string>
#include <math.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

// les nuages ont un materiau
SMaterial cloud_material;                // material used by clouds by default
bool cloud_material_initialized = false; // 1 if the default material has been initialized.
int cloud_material_texture_x_div;
int cloud_material_texture_y_div;

// initialize cloud_material with the texture to be used (grayscale image)
void setup_cloud_material(IVideoDriver * driver, ITexture * texture, int texture_x_div, int texture_y_div) {
	cloud_material_initialized = true;
	cloud_material_texture_x_div = texture_x_div;
	cloud_material_texture_y_div = texture_y_div;
	
	// getting shader support
	video::IGPUProgrammingServices * gpu = driver->getGPUProgrammingServices();
	
	// see if it is possible to use shader
	bool cant_do = true;
	if (gpu) {                    /// should works with EDT_OPENGL driver type
		cant_do = false; // provisionally accept
		if (!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1)) {
			printf("queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) failed\n");
			cant_do = true;
		}
		if (!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1)) {
			printf("queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1) failed\n");
			cant_do = true;
		}
	}
	
	if (cant_do) {
		fprintf(stderr, "warning: can't use shader for clouds\n");
		return;
	}
	else 
	{
		// compiling shader
		cloud_material.MaterialType = (E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles(
			(std::string(CLOUD_SHADER_PATH)+"cloud.vert").c_str(), "main", EVST_VS_1_1,
			(std::string(CLOUD_SHADER_PATH)+"cloud.frag").c_str(), "main", EPST_PS_1_1,
			0, EMT_TRANSPARENT_ALPHA_CHANNEL);
		// setup material parameters
		cloud_material.Lighting = false;
		cloud_material.MaterialTypeParam = video::pack_textureBlendFunc(video::EBF_SRC_ALPHA, 
						video::EBF_ONE_MINUS_SRC_ALPHA,
						video::EMFN_MODULATE_1X, video::EAS_TEXTURE | video::EAS_VERTEX_COLOR);
		cloud_material.setFlag(video::EMF_BACK_FACE_CULLING, true);
		cloud_material.setFlag(video::EMF_BLEND_OPERATION, true);
		cloud_material.setTexture(0, texture);
	}
}



CloudFace::CloudFace(ISceneNode* parent,  ISceneManager* mgr, SMaterial * mat, 
					 vector3df position, vector3df scale, 
					 float uv1x, float uv1y, float uv2x, float uv2y)
:  ISceneNode(parent, mgr)
{
	//material.Wireframe = false;
	// S3DVertex (f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, SColor c, f32 tu, f32 tv)
	vertices[0] =  S3DVertex(-0.5,-0.5, 0,  0,0,-1,	 SColor(0,0,0,0), uv1x, uv2y);
	vertices[1] =  S3DVertex(-0.5, 0.5, 0,  0,0,-1,	 SColor(0,1,0,0), uv1x, uv1y);
	vertices[2] =  S3DVertex( 0.5,-0.5, 0,  0,0,-1,	 SColor(0,1,0,0), uv2x, uv2y);
	vertices[3] =  S3DVertex( 0.5, 0.5, 0,  0,0,-1,	 SColor(0,1,0,0), uv2x, uv1y);
	box = aabbox3df(vector3df(-0.5,-0.5,0), vector3df(0.5,0.5,0));
	material = mat;
	setPosition(position);
	setScale(scale);
}

void CloudFace::render()
{
	const u16 indices[] = {0,1,3,   0,3,2}; // order of indices of points decide the normal of the plane
	static IVideoDriver* driver = SceneManager->getVideoDriver();
	driver->setMaterial(*material); // send the material used for render to the graphical card
	driver->setTransform( ETS_WORLD, AbsoluteTransformation); // send world transform to pipeline
	driver->drawVertexPrimitiveList(&vertices[0], 4, &indices[0], 4,  EVT_STANDARD,  EPT_TRIANGLES,  EIT_16BIT);
}



CloudNode::CloudNode(ISceneNode * parent, ISceneManager * mgr, vector3df position, vector3df size_)
: ISceneNode(parent, mgr)

{
	size = size_;
	setPosition(position);
	setScale(size);
	box = aabbox3df(-size/2, size/2);
	if (!cloud_material_initialized) 
		fprintf(stderr, "warning: cloud default material 'SMaterial cloud_material' has not been initialized\n");
	material = cloud_material;
	faces = 0;
	faces_number = 0;
	texture_x_div = cloud_material_texture_x_div;
	texture_y_div = cloud_material_texture_y_div;
	change_interval = CLOUD_DEFAULT_CHANGE_INTERVAL;
	
	int i;
	for (i=0; i<CLOUD_POLES; i++)
	{
		poles[i].X = size.X * (0.5 - float(rand())/RAND_MAX);
		poles[i].Y = size.Y * (0.5 - float(rand())/RAND_MAX);
		poles[i].Z = size.Z * (0.5 - float(rand())/RAND_MAX);
	}
	/* // make sure, poles will be distributed on the whole cloud (stupid for a small number of pole)
	poles[0].X = -size.X/2;
	poles[1].Z = -size.Z/2;
	poles[2].X =  size.X/2;
	poles[3].Z =  size.Z/2;
	*/
}


void CloudNode::OnRegisterSceneNode()
{
	if (IsVisible) SceneManager->registerNodeForRendering(this);
	
	unsigned int i;
	unsigned int time = time_ms();
	float f;
	if (change_time <= time) {
		update_geometry(0);
		change_time = time + change_interval;
	}
	update_time = time;

	// render facenodes in proper order :
	// sort by distance to camera
	#ifdef CLOUD_CUSTOM_ALPHA_SORT
	unsigned int j, e;
	ICameraSceneNode * camera = SceneManager->getActiveCamera();
	vector3df camdir(0,0,1);
	camera->getAbsoluteTransformation().rotateVect(camdir);
	vector3df campos = camera->getAbsolutePosition();
	CloudFace * temp;
	for (i=0; i<faces_number; i++)
	{
		e = i;
		for (j=i+1; j<faces_number; j++)
		{
			if ((faces[e]->getAbsolutePosition()-campos).dotProduct(camdir) < (faces[j]->getAbsolutePosition()-campos).dotProduct(camdir))
				e = j;
		} // e is now the index of the closest object in range [i, faces_number]
		// then exchange it.
		if (e!=i) 
		{
			temp = faces[e];
			faces[e] = faces[i];
			faces[i] = temp;
		} // list is now shorted, from 0 to i
		// then register face number i to be rendered.
		SceneManager->registerNodeForRendering(faces[i]);
		
	} // children are now sorted
	#else
	ISceneNode::OnRegisterSceneNode();
	#endif
}

void CloudNode::setScale(const vector3df & scale)
{
	size = scale;
	box.MinEdge.set(-scale/2);
	box.MaxEdge.set( scale/2);
}

void CloudNode::update_geometry(ISceneNode * camera)
{
	if (camera == 0) camera = SceneManager->getActiveCamera();
	// make sure we do have the good coordinates
	camera->updateAbsolutePosition();
	updateAbsolutePosition();
	// calculate number of faces to have, in function of the distance
	float distance = std::max(
		0.01f, 
		camera->getAbsolutePosition().getDistanceFrom(getAbsolutePosition()) - size.getLength()
	);
	if (distance == 0) return;
	float q = distance/last_distance;
	if (q < 1.6 & q > 0.6) return;
	last_distance = distance;

	unsigned int new_faces_number = CLOUD_FACES_MAX * atan(size.getLength()/distance);
	int i;
	if (new_faces_number > CLOUD_FACES_MAX) new_faces_number = CLOUD_FACES_MAX;
	if (new_faces_number != faces_number)
	{
		float scale_factor = (0.2 + 1 / float(new_faces_number));
		float scale = size.getLength() * scale_factor;
		
		for (i=0; i<std::min(faces_number, new_faces_number); i++)
			faces[i]->setScale(vector3df(scale*(1.+float(rand())/RAND_MAX)));
		
		if (new_faces_number > faces_number)
		{
			vector3df pos, pos1;
			int img, pole;
			float uvx, uvy;
			float xunit = 1./texture_x_div;
			float yunit = 1./texture_y_div;
			// get old cloud faces
			CloudFace ** new_faces = (CloudFace**) malloc(new_faces_number * sizeof(CloudFace*));
			memcpy(new_faces, faces, faces_number * sizeof(CloudFace*));
			// initialize the first cloud face if there is no face existing
			for (i=faces_number; i<new_faces_number; i++)
			{
				// part of the texture used
				img = texture_x_div * texture_y_div * (float(rand())/RAND_MAX);
				uvx = (img % texture_x_div) * xunit;
				uvy = (img / texture_x_div) * yunit;
				// pole used
				int pole = i % CLOUD_POLES;
				// offset of the face
				pos1 = size * 0.5 * vector3df(0.5- float(rand())/RAND_MAX, 
													0.5- float(rand())/RAND_MAX, 
													0.5- float(rand())/RAND_MAX);
				pos = poles[pole] + pos1;
				new_faces[i] = new CloudFace(this, SceneManager, &material, 
											pos, vector3df(scale*(0.5+float(rand())/RAND_MAX)), 
											uvx, uvy, uvx+xunit, uvy+yunit);
			}
			if (faces) free(faces);
			faces = new_faces; // official faces_number is changed after to avoid multi-threading issues
			faces_number = new_faces_number;
		}
		else if (new_faces_number < faces_number)
		{
			int old_faces_number = faces_number; // official faces_number is changed before to avoid multi-threading issues
			faces_number = new_faces_number;
			for (i=new_faces_number; i<old_faces_number; i++)
			{
				faces[i]->remove();
				delete faces[i];
			}
			faces = (CloudFace**) realloc(faces, new_faces_number * sizeof(CloudFace*));
		}
	}
}
