
//RibbonTrailSceneNode for Irrlicht
//Paulo Rogério de Oliveira ( ProSoft )
//
//This scenenode can be used to generate a ribbon trail as it travel in space.
//Node position and rotation are stored at fixed distances, then a leftpoint and
//rightpoint is calculated relative to current node position and rotation.
//MaxQuads can be used to limit the number of quads generated. When this limit is taken,
//last 10 quads are faded out then removed when completelly transparent. The entire ribbontrail
//is braked in sections. Each section have a bounding box used for frustrum culling.
//
//void setMaxDistance( float value );
//Sets the distance the nodes are stored.
//Less distance < better quality < more quads < slow render.
//
//void setEnabled( bool value );
//Starts/Stops creating quads when node when node travels in space.
//True turns on creating quads, false turns off)
//
//void setPoint1( core::vector3df newpoint );
//void setPoint2( core::vector3df newpoint );
//Sets 2 points ( vectors ) relative to scenenode. Ribbon trail will be
//generated btw this 2 points, in global space
//
//void setStartingAlpha( u16 value );
//Trail will be semitransparent. Starting alpha sets the initial transparency.
//
//void setMaxQuads( u16 value );
//When total number of quads is greater then MaxQuads, last quads start being fadded and removed
//
//void setShowDebug( bool value )
//Starts rendering boundingboxes

//		RibbonTrailSceneNode^ swp1 = new RibbonTrailSceneNode( Device, Parent );
//      swp1->setMaterialTexture( 0, tex );
//      swp1->setPoint1( core::vector3df(  5, 0, 0 ) );
//      swp1->setPoint2( core::vector3df( -5, 0, 0 ) );
//      swp1->setMaxDistance( 3 );
//      swp1->setStartingAlpha( 100 );
//		swp1->setEnabled( true );

// You can use it as you want. If you find this code usefull, some credit will be apreciated.
// Questions, comments, requests: paulo_cmv@hotmail.com

#pragma once
using namespace irr;

class RibbonTrailSceneNode : public scene::ISceneNode
{
private:

    // struct holding infos about each node in stream
    struct vitems
    {
        core::vector3df position;
        core::vector3df rotation;
        core::vector3df rpos;
        core::vector3df lpos;
    };

    // each time Start() is called, a new section is created
    struct CSection
    {
        core::array< vitems* > items;
        core::array< video::S3DVertex > Vertices;
		core::aabbox3df BoundingBox;
    };

    // global indices for all sections. All indices is created on startup
    u16* Indices;
    // Array of sections
    core::array< CSection* > sections;
    CSection* firstsection;
    CSection* lastsection;

    void addQuad();
    void addVertices();
    void createIndices( int indcount );
    void addIndices( const int index );
    ISceneNode* getParentNode( ISceneNode* AParent, IrrlichtDevice* ADevice );

    IrrlichtDevice* Device;
    video::SMaterial material;

	int totalquads;
    int maxquads;
    float mindistance;

	core::vector3df point1;
    core::vector3df point2;

    vitems* lastnode;

    video::IVideoDriver* video;
    core::aabbox3d<f32> bbox;
    float lasttc;
    float startingalpha;
    bool Enabled; // change state

    float distanciapercorrida; // total distance traveled by this node
    void Update();
    // Transforms a POSITION vector relative to a given node to global coords
    core::vector3df TransformedPosition( core::vector3df APosition );

    //! Function to know if a given point at Position, turned Rotation degrees, can see point with a given FOV
    //! param Position: the position looker is
    //! param Rotation: the rotation looker is
    //! param point: the point looker is looking at
    //! param FOV: Field of View
    bool isPointInFieldOfView( core::vector3df Position, core::vector3df Rotation, core::vector3df point, float FOV = 30.0f );
public:

    RibbonTrailSceneNode( IrrlichtDevice* device, ISceneNode* ParentNode, s32 id = -1,
             core::vector3df position = core::vector3df(0,0,0),
             core::vector3df rotation = core::vector3df(0,0,0),
             core::vector3df scale = core::vector3df(1.0f, 1.0f, 1.0f) );

	~RibbonTrailSceneNode();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

    virtual video::SMaterial& getMaterial(s32 i);

    virtual s32 getMaterialCount();

    void setMaxDistance( float value );

    virtual void OnPreRender();

    void setEnabled( bool value );

    void setPoint1( core::vector3df newpoint );

    void setPoint2( core::vector3df newpoint );

    void setStartingAlpha( u16 value );

    void setMaxQuads( u16 value );

    virtual void render();

	void setShowDebug( bool visible );
};
