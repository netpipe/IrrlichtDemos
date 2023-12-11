/*  
    Written by Asger Feldthaus
    
    February 2007
*/

#ifndef _C_TREE_GENERATOR_H_
#define _C_TREE_GENERATOR_H_

#include <ISceneManager.h>
#include <IXMLReader.h>
#include <irrList.h>
#include <irrArray.h>
#include <SMesh.h>
#include <SMeshBuffer.h>
#include "STreeMesh.h"

namespace irr
{
namespace scene
{

/*!
    \brief Creates randomized tree meshes.
    
    CTreeGenerator represents one tree design, and can generate random trees based on this design, and a seed for random values.
    CTreeSceneNode can be used to represent a procedural LOD node of a tree.
 */
class CTreeGenerator : public IReferenceCounted
{
public:
    //! Creates a new tree generator. The generator will do nothing until initialized, either by loading the tree design from an XML file
    //! or by calling addBranch() and setRootIdRef() manually.
    //! \param manager: The scene manager. This is used to create the leaf billboard node.
    CTreeGenerator( ISceneManager* manager );
    
    //! Destructor
    virtual ~CTreeGenerator();
    
    //! Specifies whether a value should be random, or fixed. If random, it specifies the minimum and maximum values.
    template<class T>
    struct SValueRange
    {
        bool IsValue;
        union
        {
            T Min;
            T Value;
        };
        T Max;

        SValueRange() : IsValue(true), Value(0), Max(0) {}
        SValueRange( T defaultValue ) : IsValue(true), Value(defaultValue), Max(0) {}
        SValueRange( T min, T max ) : IsValue(false), Min(min), Max(max) {}
    };
    
    //! A group of children on a branch. Contains information about placement, direction, and scale, and the number of child branches.
    struct SBranchChild
    {
        s32 IdRef;
        SValueRange<s32> RelativeLevel;
        SValueRange<s32> LevelRange;
        SValueRange<f32> Position;
        SValueRange<f32> LengthScale;
        SValueRange<f32> RadiusScale;
        SValueRange<f32> Orientation;
        SValueRange<f32> Pitch;
        SValueRange<f32> GravityInfluence;
        SValueRange<s32> Count;

        SBranchChild()
            :   IdRef(-1),
                RelativeLevel(-1),
                LevelRange( 0, 100 ),
                Position( 0.0f, 1.0f ),
                LengthScale( 0.60f ),
                RadiusScale( 1.00f ),
                Orientation( 0.0f, 360.0f ),
                Pitch( 0.0f ),
                GravityInfluence( 0.0f ),
                Count(1) {}
    };
    
    //! A group of leaves on a branch.
    struct SLeaf
    {
        SValueRange<s32> LevelRange;
        SValueRange<f32> Width;
        SValueRange<f32> Height;
        SValueRange<f32> Scale;
        SValueRange<f32> Position;
        SValueRange<f32> Roll;
        core::vector3df Axis;
        bool HasAxis;
        SValueRange<f32> Anchor;
        SValueRange<s32> Count;
        SValueRange<s32> Red;
        SValueRange<s32> Green;
        SValueRange<s32> Blue;
        SValueRange<s32> Alpha;
        
        SLeaf()
            : LevelRange(0,100), Width(10), Height(10), Scale(1), Position(1), Roll(0,360), HasAxis(false), Anchor(0), Count(1),
            Red(255), Green(255), Blue(255), Alpha(255) {}
    };
    
    //! One type of branch. Contains information about the branch's size, children and leaves.
    struct SBranch
    {
        s32 Id;
        SValueRange<f32> Length;
        SValueRange<f32> Radius;
        SValueRange<f32> RadiusEnd;
        core::list<SBranchChild> Children;
        core::list<SLeaf> Leaves;
        
        SBranch()
            : Id(-1), Length(10.0), Radius(5.0), RadiusEnd(0.5) {}
    };
    
    //! Adds a branch type to the tree generator. To add this branch, another branch (or the root) must use its ID as child.
    //! \param branchType: The branch to add. You should not delete this pointer yourself and never add it to more than one tree generator.
    void addBranch( SBranch* branchType );
    
    //! Sets the ID of the branch to use as root. There can only be one root.
    void setRootIdRef( s32 rootIdRef );
    
    //! Loads all settings from an XML file. See the format of the XML file at the description of CTreeGenerator.
    void loadFromXML( io::IXMLReader* xml );
    
    //! Generates a tree and returns the trunk's mesh and leaves. You should drop the tree mesh when you are done using it.
    //! \param radialSegments: Number of radial segments at the root. Must be at least 3. A typical value is 8. Number of radial segments always decreases for higher branches.
    //! \param seed: Random number seed. All random numbers are based on this seed.
    //! \param addLeaves: If true, a scene node will be added with all the leaves. Call getLeafNode() to get it afterwards.
    //! \param cutoffLevel: The level where you want branches to be invisible. Good for LOD. The root has the highest level, and the tips have the lowest level. 0 will make no branches invisible.
    STreeMesh* generateTree( s32 radialSegments, s32 seed=0, bool addLeaves=true, s32 cutoffLevel=0 );
    
private:
    ISceneManager* SceneManager;
    
    bool AddLeaves;
    
    core::list<SBranch*> Branches;
    
    s32 RootIdRef;
    
    s32 RadialSegments;
    s32 Levels;
    s32 CutoffLevel;
    
    s32 Seed;
    s32 LeafSeed;
    
    SBranch* getBranchWithId( s32 id );
    
    void appendBranch( const core::matrix4& transform, SBranch* branch, f32 lengthScale, f32 radiusScale, s32 level, SMeshBuffer* buffer, core::array<STreeLeaf>& leaves );
};

} // namespace scene
} // namespace irr

#endif
