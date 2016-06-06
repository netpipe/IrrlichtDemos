#ifndef CSHIELDMANAGER_H_INCLUDED
#define CSHIELDMANAGER_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

//! The Shield Manager manages shields, impacts and rendering those.
/** All shields and impacts are structs sorted in an array. The shields
are managed in an array attribute of the manager and the impact in an array of
the respective shield.
The manager also has a drawAll method like the scene manager. It is used the same way.
You can add any mesh node as a shield. However, it is recomended that you use nodes that are
transparent with EMT_TRANSPARENT_ADD_COLOR or EMT_TRANSPARENT_ALPHA_CHANNEL. \n
The first texture of the node should be completely transparent (layer 0). \n
The second will be used as a base color map by the shader (layer 1). \n
The third texture will be used for a gradient effect (layer 2). \n*/
class CShieldManager: public video::IShaderConstantSetCallBack
{
private:
    //!Struct to handle the impacts
	struct SImpact
	{
	    //Position of the impact in local node space
		core::vector3df pos;
		//Color of the impact
		video::SColorf color;
		//Maximum radius
		f32 radius;
		//To determine when to delete the impact from the shield's array
		u32 startTime;
		u32 endTime;
	};

    //!Struct to handle the shield nodes
	struct SShield
	{
	    //The node
	    scene::ISceneNode *shieldNode;
	    //The array of impacts on this shield
	    core::array<SImpact> array_Impacts;
	};

public:
	//!Constructor
	/**Sets up the manager and creates the shader
        \param psmgr SceneManager of the device
        \param ptimer Timer of the device*/
	CShieldManager(scene::ISceneManager* psmgr, irr::ITimer *ptimer);

	//!Destructor
	~CShieldManager();

    //!Renders the shields
	void drawAll();

	//!Sets the uniform variables for the shader
	virtual void OnSetConstants(video::IMaterialRendererServices* services,s32 userData);

    //!Add a new shield
    /**\return true if it is a new shield, false if the shield already exists
        \param node The node that shall act as a shield */
	bool addShield(scene::ISceneNode *node);

    //!Remove a shield
    /**\return true if it shield was removed, false if the shield was not found
        \param node The shield that shall be removed */
	bool removeShield(scene::ISceneNode *node);

	//!Add an impact in object space to the shield
	/**\return true if the shield exists and the impact has been added
        \param node The node that acts as a shield
        \param position The position of the impact on the shield in the shieldnode's local space
        \param radius The maximum radius of the impact
        \param duration The impact's lifespan
        \param color The Color of the impact*/
	bool addLocalImpact(const scene::ISceneNode *node, const core::vector3df position,const f32 radius=1, const u32 duration=500, const video::SColorf color = video::SColorf(1,1,1,1));

	//!Add an impact in global space to the shield
	/**\return true if the shield exists and the impact has been added
        \param node The node that acts as a shield
        \param position The position of the impact on the shield in global space
        \param radius The maximum radius of the impact
        \param duration The impact's lifespan
        \param color The Color of the impact*/
	bool addGlobalImpact(const scene::ISceneNode *node, core::vector3df &position,const f32 radius=1, const u32 duration=500, const video::SColorf color = video::SColorf(1,1,1,1));

	//!Let the manager determine the impact position by using a given ray
	/**\return the local position of the impact when successful, the ray's end otherwise
        \param node The node that acts as a shield
        \param ray The ray that hits the shield
        \param radius The maximum radius of the impact
        \param duration The impact's lifespan
        \param color The Color of the impact*/
	core::vector3df addImpact(const scene::ISceneNode *node, const core::line3df ray, const f32 radius=1, const u32 duration=500, const video::SColorf color = video::SColorf(1,1,1,1));

private:
    //To check if the shields are currently visible
    scene::ISceneManager* smgr;

    //To draw the shields/impacts
    video::IVideoDriver* driver;

    //To erase impacts after their lifetime
    irr::ITimer *timer;

    //Manage the shields
	core::array<SShield> array_Shields;

    //The shield shader
    int shaderMaterial;

	//The following attributes parameters for the OnSetConstants method
	//Radius of the current impact
	f32 tmp_radius;
	//Position of the current impact
	core::vector3df tmp_position;
	//Color of the current impact
	video::SColorf tmp_color;
};

#endif // CSHIELDMANAGER_H_INCLUDED
