/*  
    Written by Asger Feldthaus
    
    February 2007
*/

#include "CTreeGenerator.h"

#include "CBillboardGroupSceneNode.h"

#include <math.h>
#include <stdlib.h>
#include <wchar.h>

namespace irr
{
namespace scene
{

void BuildCylinder(   
        core::array<video::S3DVertex>& vertexArray,
        core::array<u16>& indexArray,
        core::aabbox3d<f32>& boundingBox,
        
        f32 height,
        f32 startRadius,
        f32 endRadius,
        s32 radialSegments,
        
        const core::matrix4& transform,
        
        f32 startTCoordY = 0.0f,
        f32 endTCoordY = 1.0f,
        video::SColor startColor = video::SColor(255,255,255,255),
        video::SColor endColor = video::SColor(255,255,255,255) )
{
    u16 vertexIndex = vertexArray.size();
    
    s32 radialVertices = radialSegments + 1; // We want one additional vertex to make the texture wraps correctly
    
    // Place bottom cap
    for ( s32 i=0; i<radialVertices; i++ )
    {
        f32 angle = 2.0f * core::PI * i / (f32)( radialSegments );
        
        core::vector3df dir;
        dir.X = cos(angle);
        dir.Z = sin(angle);
        
        core::vector3df pos;
        core::vector3df normal = dir;

        pos = dir * startRadius;
        
        // Place bottom vertex
        transform.transformVect( pos );
        transform.rotateVect( normal );
        
        core::vector2df tcoord;
        tcoord.X = (f32)( i ) / (f32)( radialSegments );
        tcoord.Y = startTCoordY;
        
        vertexArray.push_back( video::S3DVertex(
            pos,
            normal,
            startColor,
            tcoord  ) );
        
        boundingBox.addInternalPoint( pos );
    }
    
    // Place top cap and indices
    for ( s32 i=0; i<radialVertices; i++ )
    {
        f32 angle = 2.0f * core::PI * i / (f32)( radialSegments );
        
        core::vector3df dir;
        dir.X = cos(angle);
        dir.Z = sin(angle);
        
        core::vector3df normal = dir;
        core::vector3df pos = dir * endRadius;
        pos.Y = height;
        
        transform.transformVect( pos ); 
        transform.rotateVect( normal );
        
        core::vector2df tcoord;
        tcoord.X = (f32)( i ) / (f32)( radialSegments );
        tcoord.Y = endTCoordY;
        
        vertexArray.push_back( video::S3DVertex(
            pos,
            normal,
            endColor,
            tcoord  ) );
        
        boundingBox.addInternalPoint(pos);
        
        // Add indices
        if ( i != radialVertices-1 )
        {
            s32 i2 = (i+1)%radialVertices;
            // Place the indices
            indexArray.push_back ( vertexIndex + i );
            indexArray.push_back ( vertexIndex + radialVertices + i );
            indexArray.push_back ( vertexIndex + i2 );
            
            indexArray.push_back ( vertexIndex + radialVertices + i );
            indexArray.push_back ( vertexIndex + radialVertices + i2 );
            indexArray.push_back ( vertexIndex + i2 );
        }
    }
}

CTreeGenerator::CTreeGenerator( ISceneManager* manager )
{
    SceneManager = manager;
    RootIdRef = -1;
    Levels = 4;
}

CTreeGenerator::~CTreeGenerator()
{
    // Delete all the branches
    core::list<SBranch*>::Iterator it = Branches.begin();
    while ( it != Branches.end() )
    {
        delete (*it);
        it = Branches.erase(it);
    }
}

void CTreeGenerator::addBranch( SBranch* branchType )
{
    Branches.push_back( branchType );
}

void CTreeGenerator::setRootIdRef( s32 rootIdRef )
{
    RootIdRef = rootIdRef;
}

inline CTreeGenerator::SValueRange<f32> getValueRangeFloatFromXML( io::IXMLReader* xml )
{
    CTreeGenerator::SValueRange<f32> range;
    if ( xml->getAttributeValue( L"value" ) != 0 )
    {
        range.IsValue = true;
        range.Value = xml->getAttributeValueAsFloat( L"value" );
        return range;
    }
    
    range.IsValue = false;
    range.Min = xml->getAttributeValueAsFloat( L"min" );
    range.Max = xml->getAttributeValueAsFloat( L"max" );
    
    return range;
}

inline CTreeGenerator::SValueRange<s32> getValueRangeIntFromXML( io::IXMLReader* xml )
{
    CTreeGenerator::SValueRange<s32> range;
    if ( xml->getAttributeValue( L"value" ) != 0 )
    {
        range.IsValue = true;
        range.Value = xml->getAttributeValueAsInt( L"value" );
        return range;
    }
    
    range.IsValue = false;
    range.Min = xml->getAttributeValueAsInt( L"min" );
    range.Max = xml->getAttributeValueAsInt( L"max" );
    
    return range;
}

inline f32 parseFloat( const wchar_t* &c )
{
    while ( *c == L',' || *c == L';' || *c == L' ' || *c == L'\t' )
        c++;
    
    core::stringw tmp = L"";
    while ( *c != 0 && *c != L',' && *c != L';' && *c != L' ' && *c != L'\t' )
    {
        tmp += *c;
        c++;
    }
    
    f32 f = 0.0f;
    swscanf( tmp.c_str(), L"%f", &f );
    return f;
}

inline core::vector3df getVector3DFromString( const wchar_t* c )
{
    core::vector3df vec;
    vec.X = parseFloat(c);
    vec.Y = parseFloat(c);
    vec.Z = parseFloat(c);
    return vec;
}

inline core::vector2df getVector2DFromString( const wchar_t* c )
{
    core::vector2df vec;
    vec.X = parseFloat(c);
    vec.Y = parseFloat(c);
    return vec;
}

void CTreeGenerator::loadFromXML( io::IXMLReader* xml )
{
    if ( xml == 0 )
        return;
    
    RootIdRef = -1;
    
    SBranch* currentBranch = 0;
    
    SBranchChild* currentChild = 0;
    
    SLeaf* currentLeaf = 0;
    
    while ( xml->read() )
    {
        io::EXML_NODE type = xml->getNodeType();
        
        core::stringw nodeName = L"";
        
        switch( type )
        {
            case io::EXN_ELEMENT:
                
                nodeName = xml->getNodeName();
                
                if ( nodeName == L"branch" && currentBranch == 0 && currentLeaf == 0 )
                {
                    currentBranch = new SBranch();
                    
                    if ( xml->getAttributeValue( L"id" ) != 0 )
                        currentBranch->Id = xml->getAttributeValueAsInt( L"id" );
                }
                else
                if ( nodeName == L"child" && currentBranch != 0 && currentChild == 0 && currentLeaf == 0 )
                {
                    currentChild = new SBranchChild();
                    
                    if ( xml->getAttributeValue( L"ref" ) != 0 )
                        currentChild->IdRef = xml->getAttributeValueAsInt( L"ref" );
                    
                    if ( xml->getAttributeValue( L"level" ) != 0 )
                    {
                        currentChild->RelativeLevel.IsValue = true;
                        currentChild->RelativeLevel.Value = xml->getAttributeValueAsInt( L"level" );
                    }
                }
                else
                if ( nodeName == L"leaf" && currentBranch != 0 && currentChild == 0 && currentLeaf == 0 )
                {
                    currentLeaf = new SLeaf();
                    
                    if ( xml->getAttributeValue( L"level" ) != 0 )
                    {
                        currentLeaf->LevelRange.IsValue = true;
                        currentLeaf->LevelRange.Value = xml->getAttributeValueAsInt( L"level" );
                    }
                }
                else
                if ( nodeName == L"length" && currentBranch != 0 && currentChild == 0 && currentLeaf == 0 )
                {
                    currentBranch->Length = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"radius" && currentBranch != 0 && currentChild == 0 && currentLeaf == 0 )
                {
                    currentBranch->Radius = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"radiusEnd" && currentBranch != 0 && currentChild == 0 && currentLeaf == 0 )
                {
                    currentBranch->RadiusEnd = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"lengthScale" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->LengthScale = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"radiusScale" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->RadiusScale = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"relativeLevel" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->RelativeLevel = getValueRangeIntFromXML( xml );
                }
                else
                if ( nodeName == L"levelRange" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->LevelRange = getValueRangeIntFromXML( xml );
                }
                else
                if ( nodeName == L"position" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->Position = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"orientation" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->Orientation = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"pitch" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->Pitch = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"gravity" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->GravityInfluence = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"count" && currentBranch != 0 && currentChild != 0 && currentLeaf == 0 )
                {
                    currentChild->Count = getValueRangeIntFromXML( xml );
                }
                else
                if ( nodeName == L"root" && currentBranch == 0 && currentChild == 0 && currentLeaf == 0 )
                {
                    RootIdRef = xml->getAttributeValueAsInt( L"ref" );
                    
                    if ( xml->getAttributeValue( L"levels" ) != 0 )
                    {
                        Levels = xml->getAttributeValueAsInt( L"levels" );
                    }
                }
                else
                if ( nodeName == L"roll" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Roll = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"axis" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->HasAxis = true;
                    currentLeaf->Axis = getVector3DFromString( xml->getAttributeValueSafe(L"value") );
                    currentLeaf->Axis.normalize();
                }
                else
                if ( nodeName == L"anchor" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Anchor = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"count" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Count = getValueRangeIntFromXML( xml );
                }
                else
                if ( nodeName == L"width" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Width = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"height" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Height = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"scale" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Scale = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"position" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Position = getValueRangeFloatFromXML( xml );
                }
                else
                if ( nodeName == L"levelRange" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->LevelRange = getValueRangeIntFromXML( xml );
                }
                else
                if ( nodeName == L"red" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Red = getValueRangeIntFromXML( xml );
                }
                else
                if ( nodeName == L"green" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Green = getValueRangeIntFromXML( xml );
                }
                else
                if ( nodeName == L"blue" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Blue = getValueRangeIntFromXML( xml );
                }
                else
                if ( nodeName == L"alpha" && currentBranch != 0 && currentChild == 0 && currentLeaf != 0 )
                {
                    currentLeaf->Alpha = getValueRangeIntFromXML( xml );
                }
                
                break;
            
            case io::EXN_ELEMENT_END:
                
                nodeName = xml->getNodeName();
                
                if ( nodeName == L"branch" && currentBranch != 0 )
                {
                    Branches.push_back( currentBranch );
                    currentBranch = 0;
                }
                else
                if ( nodeName == L"child" && currentBranch != 0 && currentChild != 0 )
                {
                    currentBranch->Children.push_back( *currentChild );
                    delete currentChild;
                    currentChild = 0;
                }
                else
                if ( nodeName == L"leaf" && currentBranch != 0 && currentLeaf != 0 )
                {
                    currentBranch->Leaves.push_back( *currentLeaf );
                    delete currentLeaf;
                    currentLeaf = 0;
                }
                
                break;
        }
    }
    
    if ( currentLeaf != 0 )
        delete currentLeaf;
    
    if ( currentChild != 0 )
        delete currentChild;
    
    if ( currentBranch != 0 )
        delete currentBranch;
}

STreeMesh* CTreeGenerator::generateTree( s32 radialSegments, s32 seed, bool addLeaves, s32 cutoffLevel )
{
    STreeMesh* treeMesh = new STreeMesh();
    
    Seed = seed;
    LeafSeed = seed;
    RadialSegments = radialSegments;
    CutoffLevel = cutoffLevel;
    AddLeaves = addLeaves;
    
    SMeshBuffer* buffer = new SMeshBuffer();
    
    treeMesh->setMeshBuffer( buffer );
    
    SBranch* root = getBranchWithId( RootIdRef );
    
    if ( root != 0 )
    {
        appendBranch( core::matrix4(), root, 1.0f, 1.0f, Levels, buffer, treeMesh->Leaves );
    }
    
    buffer->drop();
    
    return treeMesh;
}

CTreeGenerator::SBranch* CTreeGenerator::getBranchWithId( s32 id )
{
    core::list<SBranch*>::Iterator it = Branches.begin();
    while ( it != Branches.end() )
    {
        if ( (*it)->Id == id )
        {
            return (*it);
        }
        it++;
    }
    return 0;
}

inline f32 getRandomFloat( s32 seed, f32 min=0.0f, f32 max=1.0f )
{
    seed = (seed<<13) ^ seed;
    f32 x = ( 1.0f - ( (seed * (seed * seed * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    return (max-min)*(x+1)/2.0f + min;
}

inline s32 getRandomInt( s32 seed, s32 min, s32 max )
{
    return (s32)(getRandomFloat(seed,min,max) + 0.50f);
}

template<class T>
inline bool isValueInRange( T value, const CTreeGenerator::SValueRange<T>& range )
{
    if ( range.IsValue )
        return ( value == range.Value );
    
    return ( value >= range.Min && value <= range.Max );
}

inline f32 getFloatFromValueRange( const CTreeGenerator::SValueRange<f32>& range, s32 seed )
{
    if ( range.IsValue )
        return range.Value;
    
    return getRandomFloat( seed, range.Min, range.Max );
}

inline s32 getIntFromValueRange( const CTreeGenerator::SValueRange<s32>& range, s32 seed )
{
    if ( range.IsValue )
        return range.Value;
    
    return getRandomInt( seed, range.Min, range.Max );
}

inline void setMatrixVec( core::matrix4& mat, s32 column, const core::vector3df& vec )
{
    mat[column*4] = vec.X;
    mat[column*4+1] = vec.Y;
    mat[column*4+2] = vec.Z;
}

void CTreeGenerator::appendBranch( const core::matrix4& transform, SBranch* branch, f32 lengthScale, f32 radiusScale, s32 level, SMeshBuffer* buffer, core::array<STreeLeaf>& leaves )
{
    if ( level <= 0 )
        return; // Do nothing, this branch is invisible.
    
    // Add this branch
    f32 length = lengthScale * getFloatFromValueRange( branch->Length, Seed++ );
    
    f32 radius = getFloatFromValueRange( branch->Radius, Seed++ );
    
    f32 radiusEndScale = radiusScale * getFloatFromValueRange( branch->RadiusEnd, Seed++ );
    
    f32 radiusEnd = radius * radiusEndScale;
    
    radius = radiusScale * radius;
    
    if ( level == 1 )
        radiusEnd = 0;
    
    s32 radialSegments = (level*(RadialSegments-3))/Levels + 3;
    
    if ( level > CutoffLevel )
    {
        BuildCylinder(
            buffer->Vertices,
            buffer->Indices,
            buffer->BoundingBox,
            
            length,
            radius,
            radiusEnd,
            radialSegments,
            
            transform );
    }
    
    // Add children
    if ( level > 1 )
    {
        core::list<SBranchChild>::Iterator it = branch->Children.begin();
        while ( it != branch->Children.end() )
        {
            SBranchChild* child = &(*it);
            
            it++;
            
            SBranch* childBranch = getBranchWithId( child->IdRef );
            
            if ( childBranch == 0 )
                continue;
            
            if ( !isValueInRange( level, child->LevelRange ) )
                continue;
            
            s32 childLevel = level + getIntFromValueRange( child->RelativeLevel, Seed++ ); // RelativeLevel is usually negative, -1 in particular.
            
            s32 numChildren = getIntFromValueRange( child->Count, Seed++ );
            
            f32 positionRange = child->Position.Max - child->Position.Min;
            
            positionRange /= (f32)numChildren;
            
            f32 orientation = getRandomFloat( Seed++, 0, 360.0f );
            
            for ( s32 i=0; i<numChildren; i++ )
            {
                f32 childLengthScale = lengthScale * getFloatFromValueRange( child->LengthScale, Seed++ );
                
                orientation += getFloatFromValueRange( child->Orientation, Seed++ );
                
                // Clamp value between 0 and 360. This is needed for gravity to work properly
                if ( orientation < 0.0f )
                    orientation += 360.0f;
                else
                if ( orientation > 360.0f )
                    orientation -= 360.0f;
                
                f32 childOrientation = orientation;
                
                f32 gravity = getFloatFromValueRange( child->GravityInfluence, Seed++ );
                
                f32 childPitch = getFloatFromValueRange( child->Pitch, Seed++ );
                
                f32 childPosition = getFloatFromValueRange( child->Position, Seed++ );
                
                f32 position;
                
                if ( child->Position.IsValue )
                    position = child->Position.Value;
                else
                    position = (child->Position.Min + positionRange * i + positionRange * getRandomFloat( Seed++, 0, 1 ));
                
                f32 childRadiusScale = (radiusScale*(1.0f-position) + radiusEndScale*position) * getFloatFromValueRange( child->RadiusScale, Seed++ );
                
                // Build transformation matrix
                core::matrix4 mat;
                
                mat.setRotationDegrees( core::vector3df(childPitch, childOrientation, 0.0f) );
                
                mat[13] = length * position;
                
                mat = transform * mat;
                
                
                if ( gravity != 0.0f )
                {
                    // Do some extra work
                    
                    core::vector3df vDown = core::vector3df( 0, -1, 0 );
                    
                    core::vector3df vBranch = core::vector3df( 0, 1, 0 );
                    mat.rotateVect(vBranch);
                    
                    core::vector3df vSide;
                    
                    if ( fabs( vBranch.Y ) >= 0.9f )
                    {
                        vSide = core::vector3df( 1, 0, 0 );
                        mat.rotateVect(vSide);
                    }
                    else
                    {
                        vSide = vBranch.crossProduct(vDown);
                        vSide.normalize();
                    }
                    
                    vDown = vSide.crossProduct(vBranch);
                    vDown.normalize();
                    
                    setMatrixVec( mat, 0, vSide );
                    setMatrixVec( mat, 2, vDown );
                    
                    f32 dot = -vBranch.Y;
                    
                    if ( gravity < 0.0f )
                        dot = -dot;
                    
                    f32 angle = acos( dot );
                    
                    angle *= gravity;
                    
                    core::matrix4 mat2;
                    mat2.setRotationRadians( core::vector3df( angle,0,0 ) );
                    
                    mat = mat * mat2;
                }
                
                // Add the branch
                appendBranch( mat, childBranch, childLengthScale, childRadiusScale, childLevel, buffer, leaves );
            }
        }
    }
    
    // Add leaves
    if ( AddLeaves )
    {
        core::list<SLeaf>::Iterator lit = branch->Leaves.begin();
        while ( lit != branch->Leaves.end() )
        {
            SLeaf* leaf = &(*lit);
            lit++;
            
            if ( !isValueInRange( level, leaf->LevelRange ) )
                continue;
            
            s32 count = getIntFromValueRange( leaf->Count, LeafSeed++ );
            
            for ( s32 i=0; i<count; i++ )
            {
            
                f32 width = getFloatFromValueRange( leaf->Width, LeafSeed++ );
                f32 height = getFloatFromValueRange( leaf->Height, LeafSeed++ );
                f32 scale = getFloatFromValueRange( leaf->Scale, LeafSeed++ );
                f32 position = getFloatFromValueRange( leaf->Position, LeafSeed++ );
                f32 roll = getFloatFromValueRange( leaf->Roll, LeafSeed++ );
                f32 anchor = getFloatFromValueRange( leaf->Anchor, LeafSeed++ );
                
                core::matrix4 mat;
                
                mat[13] = length * position;
                
                mat = transform * mat;
                
                STreeLeaf treeLeaf;
                
                treeLeaf.Position = mat.getTranslation();
                
                treeLeaf.Color.setRed( getIntFromValueRange( leaf->Red, LeafSeed++ ) );
                treeLeaf.Color.setGreen( getIntFromValueRange( leaf->Green, LeafSeed++ ) );
                treeLeaf.Color.setBlue( getIntFromValueRange( leaf->Blue, LeafSeed++ ) );
                treeLeaf.Color.setAlpha( getIntFromValueRange( leaf->Alpha, LeafSeed++ ) );
                
                treeLeaf.Size = core::dimension2df(scale*width, scale*height);
                treeLeaf.Roll = roll;
                
                if ( leaf->HasAxis )
                {
                    treeLeaf.HasAxis = true;
                    treeLeaf.Axis = leaf->Axis;
                    treeLeaf.Position += leaf->Axis * height * scale * anchor / 2.0f;
                }
                else
                {
                    treeLeaf.HasAxis = false;
                }
                
                leaves.push_back( treeLeaf );
            }
            
        }
    }
}

} // namespace scene
} // namespace irr



