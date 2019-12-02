#ifndef _clouds_h_
#define _clouds_h_


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

// clouds have a specific default material
extern SMaterial cloud_material;               // material used by clouds by default
extern bool cloud_material_initialized;        // 1 if the default material has been initialized.
extern int cloud_material_texture_x_div;
extern int cloud_material_texture_y_div;
// initialize cloud_material with the texture to be used (grayscale image)
void setup_cloud_material(IVideoDriver * driver, ITexture * texture, int texture_x_div=1, int texture_y_div=1);

#define CLOUD_SHADER_PATH "./"     // path to cloud shader (and maybe texture too).
#define CLOUD_FACES_MAX 100        // maximum number of CloudFace by CloudNode (Maximum precision, when the camera is near the cloud).
#define CLOUD_POLES 5              // higher than 4: the number of important points around what the cloud geometry is organized.
#define CLOUD_DEFAULT_CHANGE_INTERVAL 500 // (ms) default time between 2 update of the cloud speed and (at minimum, geometry).

#define CLOUD_CUSTOM_ALPHA_SORT // the cloud's faces will be sorted before each render to get a correct render, comment to disable sort.


// this class is only designed for cloud node, so don't use it
class CloudFace : public  ISceneNode
{
	aabbox3d<f32> box;
public:
	S3DVertex vertices[4];
	SMaterial * material;
	//vector3df speed;

	CloudFace(ISceneNode* parent,  ISceneManager* mgr, SMaterial * mat, 
			  vector3df position, vector3df scale, 
			  float uv1x=0, float uv1y=0, float uv2x=1, float uv2y=1);
	
	virtual void OnRegisterSceneNode()
	{
		if (IsVisible) SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}
	virtual void render();
	virtual const  aabbox3d<f32>& getBoundingBox() const { return box; }
	virtual const u32 getMaterialCount() { return 1; }
	virtual SMaterial & getMaterial(u32 i) { return *material; }
};


/*
  This class is the main object of the Cloud module, it is the node of a cloud.
  Let's use it with:
	CloudNode * cloud = new CloudNode(parent, scenemgr, pos, scale);
*/
class CloudNode : public ISceneNode
{
	aabbox3df box;                // area of the cloud: for irrlicht 
	unsigned int change_time;     // date of the next possible update of the cloud geometry (ms)
	unsigned int update_time;     // date of the last update render (ms)
	float last_distance;          // distance from the camera to the cloud during the last geometry update
	
	/*
	  The cloud is in reality a multitude of semi-transparent faces of different scales and textures.
	  They are placed above few points that determines the global geometry of the cloud, that never change : the poles
	  The face's number change with the distance to the camera (sort of LOD), to optimize the GPU comsuption.
	  Each change_interval, the evolution of the distance to the camera is evaluated to determines if it is necessary to update the cloud face number.
	*/
	CloudFace ** faces;
	unsigned int faces_number;
	vector3df poles[CLOUD_POLES]; // important points that determines the geometry of the cloud: a small number of points allow the cloud to keep his original form when the camera is far and the faces number is slow.
public:
	/*
	  The cloud texture is subdivided to containe multiple possible textures, in row and colums, 
	  The subdivision parameters are below.
	  The material should have been initialized by the functions higher: setup_cloud_material()
	  Else, It's necessary the following fields manually.
	*/
	SMaterial material;            // material of the cloud, using the texture
	unsigned char texture_x_div;   // number of cloud textures by line
	unsigned char texture_y_div;   // number of cloud textures by row
	
	int change_interval;   // (ms) minimum time between each update of the geometry, default is CLOUD_DEFAULT_CHANGE_INTERVAL
	//float activity;      // activity level of the cloud: cloudfaces's speeds
	vector3df size;        // scale of the cloud area (doesn't affect the faces scale)
	
	CloudNode(ISceneNode * parent, ISceneManager * mgr, vector3df position, vector3df size);
	void update_geometry(ISceneNode * camera);
	
	/// necessary methods
	virtual void OnRegisterSceneNode();
	virtual void render() {};
	virtual const  aabbox3d<f32>& getBoundingBox() const { return box; }
	virtual const u32 getMaterialCount() { return 1; }
	virtual SMaterial & getMaterial(u32 i) { return material; }
	
	/// overwritten methods
	virtual void setScale(const vector3df & scale);
};


#endif
