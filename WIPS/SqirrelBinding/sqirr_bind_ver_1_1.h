// Irrlicht bindings for Squirrel
/*------------------------------------------------------------------
// sqirr_bind.h
// Copyright (c) 2007
//   _________        ____________ 
//  /\   ___  \      /\____   ____\
//  \ \  \  \  \     \/___ \  \___/
//   \ \  \  \  \         \ \  \   
//    \ \  \  \  \         \ \  \
//     \ \  \__\  \         \ \  \
//      \ \_______/          \ \__\
//       \/_______/ uos       \/__/ echnologies
//
// Duos Technologies
// Jacksonville Florida
// http://www.duostech.com/
//
// Redistribution and use in source and binary forms, with or 
// without modification, are permitted for commercial and 
// non-commercial purposes, provided that the following 
// conditions are met:
//
// * Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// * The names of the developers or contributors may not be used to 
//   endorse or promote products derived from this software without 
//   specific prior written permission.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
//   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
//   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
//   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
//   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Version:     1.1.0
// Programmer:  Robert Umbehant 
//------------------------------------------------------------------
//
// Please send me improvements and/or bug fixes, Thanks!
//
//------------------------------------------------------------------
//
// Notes
//
// This is a first-pass at integrating squirrel and irrlicht.  I'm
// not that familiar with either.  Much of the design below may seem
// strange, but there were a lot of compromises to make :)
//
// >>> General
//
// * I'm using camel notation for functions exported to Squirrel,
//
// Even though these are 'grips', both squirrel and irrlicht have been
// a pleasure to work with!
//
// >>> Grips - Squirrel
//
// * Apparently, Squirrel won't hold a reference to an outside variable, it
//   always makes a copy.  If so, this really sucks.
// * SqPlus doesn't define const references as params, while C++ complains
//   if you pass temporary objects as non const param
// * Seem to be having trouble calling virtual functions from squirrel, 
//   crash-o-rama.
// * Can't seem to really effectively use inheritence in C++ and port it to
//   squirrel.
// * Please tell me I will eventually be able to figure out how to add multiple
//   constructors.
//
// >>> Grips - Irrlicht
//
// * No way to associate user object with ISceneNode, must use external mapping :(
// * Seems to be problems with the AntiAliasing sometimes.  To bad because
//   it really improves the way things look!  I think this is a Direct-X / video
//   driver issue, but it means we'll have to make anti-aliasing an option.
// * So far, I've not figured out how to make Irrlicht change resolution when
//   the window is resized.  The window changes size, but the image is actually
//   being 'stretched'.
//
//----------------------------------------------------------------*/

#pragma once

#define SQIRR_DECLARE_INSTANCE( c ) \
    }; DECLARE_INSTANCE_TYPE_NAME( sqirr::##c, c ); namespace sqirr {

namespace sqirr
{
    /// 3D vector class
    class CSiVector
    {
    private:
         
        irr::core::vector3df    m_v;

    public:

        CSiVector() {}
        CSiVector( const CSiVector &sv ) : m_v( sv.m_v ) {}
        CSiVector( const irr::core::vector3df &x_v ) : m_v( x_v ) {}
        CSiVector( float x, float y, float z ) : m_v( x, y, z ) {}
        irr::core::vector3df& v() { return m_v; }
        const irr::core::vector3df& c_v() const { return m_v; }

        /// *** Squirrel functions

        /// Register squirrel interface
        static void Register( SquirrelVM &vm )
        {
            SqPlus::SQClassDef< CSiVector >( vm, _T( "CSiVector" ) )
                    . staticFunc( &CSiVector::construct_CSiVector_1,    _T( "constructor" ) )
//                    . staticFunc( &CSiVector::construct_CSiVector_2,    _T( "constructor" ) )
                    . func      ( &CSiVector::set,                      _T( "set" ) )
                    . func      ( &CSiVector::x,                        _T( "x" ) )
                    . func      ( &CSiVector::y,                        _T( "y" ) )
                    . func      ( &CSiVector::z,                        _T( "z" ) )
                    . func      ( &CSiVector::inflate,                  _T( "inflate" ) )
                ;
        }

        CSiVector& operator = ( const CSiVector &x_v ) { m_v = x_v.m_v; return *this; }

        //----------------------------------------------------------
        // >>> CSiVector - Squirrel functions
        //----------------------------------------------------------

        //==============================================================
        // construct_CSiVector_1()
        //==============================================================
        /// CSiVector constructor
        /**
            \param [in] x   -   X coord
            \param [in] y   -   Y coord
            \param [in] z   -   Z coord

            \param [in] v   -   VM handle
        */
        static int construct_CSiVector_1( float x, float y, float z, HSQUIRRELVM v )
        {    return SqPlus::PostConstruct< CSiVector >( v, new CSiVector( x, y, z ), 
                        SqPlus::ReleaseClassPtr< CSiVector >::release ); 
        }

        static int construct_CSiVector_2( CSiVector &sv, HSQUIRRELVM v )
        {    return SqPlus::PostConstruct< CSiVector >( v, new CSiVector( sv ), 
                        SqPlus::ReleaseClassPtr< CSiVector >::release ); 
        }

        //==============================================================
        // set()
        //==============================================================
        /// Sets the vecotr to a particular position
        /**
            \param [in] x   -   X coord
            \param [in] y   -   Y coord
            \param [in] z   -   Z coord         
        */
        void set( float x, float y, float z ) { m_v.set( x, y, z ); }

        /// Returns the x coord
        float& x() { return m_v.X; }

        /// Returns the y coord
        float& y() { return m_v.Y; }

        /// Returns the z coord
        float& z() { return m_v.Z; }

        /// Inflates the vector by the specified multiplier
        void inflate( float v ) { m_v.X *= v; m_v.Y *= v; m_v.Z *= v; }

    };
    SQIRR_DECLARE_INSTANCE( CSiVector );


    /// 2D vector class
    class CSiVector2d
    {
    private:
        
        irr::core::vector2df    m_v;

    public:

        CSiVector2d() {}
        CSiVector2d( const irr::core::vector2df &x_v ) : m_v( x_v ) {}
        CSiVector2d( float x, float y ) : m_v( x, y ) {}
        irr::core::vector2df& v() { return m_v; }
        const irr::core::vector2df& c_v() const { return m_v; }

        /// *** Squirrel functions

        /// Register squirrel interface
        static void Register( SquirrelVM &vm )
        {
            SqPlus::SQClassDef< CSiVector2d >( vm, _T( "CSiVector2d" ) )
//                    . staticFunc( &CSiVector2d::construct_CSiVector2d_1,    _T( "constructor" ) )
                    . func      ( &CSiVector2d::set,                        _T( "set" ) )
                    . func      ( &CSiVector2d::x,                          _T( "x" ) )
                    . func      ( &CSiVector2d::y,                          _T( "y" ) )
                    . func      ( &CSiVector2d::inflate,                    _T( "inflate" ) )
                ;
        }

        CSiVector2d operator = ( const CSiVector2d &x_v ) { m_v = x_v.m_v; return *this; }
        CSiVector2d operator = ( CSiVector2d &x_v ) { m_v = x_v.m_v; return *this; }

        //----------------------------------------------------------
        // >>> CSiVector2d - Squirrel functions
        //----------------------------------------------------------

        //==============================================================
        // construct_CSiVector2d_1()
        //==============================================================
        /// CSiVector constructor
        /**
            \param [in] x   -   X coord
            \param [in] y   -   Y coord

            \param [in] v   -   VM handle
        */
        static int construct_CSiVector2d_1( float x, float y, HSQUIRRELVM v )
        {    return SqPlus::PostConstruct< CSiVector2d >( v, new CSiVector2d( x, y ), 
                        SqPlus::ReleaseClassPtr< CSiVector2d >::release ); 
        }

        //==============================================================
        // set()
        //==============================================================
        /// Sets the vecotr to a particular position
        /**
            \param [in] x   -   X coord
            \param [in] y   -   Y coord
        */
        void set( float x, float y ) { m_v.set( x, y ); }

        /// Returns the x coord
        float x() { return m_v.X; }

        /// Returns the y coord
        float y() { return m_v.Y; }

        /// Inflates the vector by the specified multiplier
        void inflate( float v ) { m_v.X *= v; m_v.Y *= v; }

    };
    SQIRR_DECLARE_INSTANCE( CSiVector2d );


    /// Line class
    class CSiLine
    {
    private:
        
        CSiVector               m_start;
        CSiVector               m_end;

    public:

        CSiLine() {}
        CSiLine( const irr::core::line3df line ) : m_start( line.start ), m_end( line.end ) {}
        CSiLine( const CSiVector start, const CSiVector end ) : m_start( start ), m_end( end ) {}
        CSiLine( float sx, float sy, float sz, float ex, float ey, float ez ) : 
            m_start( sx, sy, sz ), m_end( ex, ey, ez ) {}

        const CSiVector& c_start() { return m_start; }
        const CSiVector& c_end() { return m_end; }

        /// *** Squirrel functions

        /// Register squirrel interface
        static void Register( SquirrelVM &vm )
        {
            SqPlus::SQClassDef< CSiLine >( vm, _T( "CSiLine" ) )
                    . staticFunc( &CSiLine::construct_CSiLine_1,        _T( "constructor" ) )
                    . func      ( &CSiLine::set,                        _T( "set" ) )
                    . func      ( &CSiLine::start,                      _T( "start" ) )
                    . func      ( &CSiLine::end,                        _T( "end" ) )
                ;
        }

        CSiLine& operator = ( const CSiLine &x_line ) 
        {   m_start = x_line.m_start; m_end = x_line.m_end; return *this; }

        //----------------------------------------------------------
        // >>> CSiLine - Squirrel functions
        //----------------------------------------------------------

        //==============================================================
        // construct_CSiLine_1()
        //==============================================================
        /// CSiLine constructor
        /**
            \param [in] x   -   X coord
            \param [in] y   -   Y coord

            \param [in] v   -   VM handle
        */
        static int construct_CSiLine_1( CSiVector &start, CSiVector &end, HSQUIRRELVM v )
        {    return SqPlus::PostConstruct< CSiLine >( v, new CSiLine( start, end ), 
                        SqPlus::ReleaseClassPtr< CSiLine >::release ); 
        }

        //==============================================================
        // set()
        //==============================================================
        /// Sets the vecotr to a particular position
        /**
            \param [in] start   -   The start of the line
            \param [in] end     -   The end of the line
        */
        void set( CSiVector &start, CSiVector &end ) { m_start = start; m_end = end; }

        /// The start of the line
        CSiVector& start() { return m_start; }

        /// The end of the line
        CSiVector& end() { return m_end; }

    };
    SQIRR_DECLARE_INSTANCE( CSiLine );


    /// Holds data, this is because of the Squirrel no-outside-reference thing.
    class CSiData
    {
    public:

        struct SEventInfo
        {       
            SEventInfo() { pSoFunction = NULL; pSoRoot = NULL; pSoP1 = NULL; pSoP2 = NULL;  }
            ~SEventInfo() 
            { 
                if ( pSoFunction ) 
                {   delete pSoFunction; pSoFunction = NULL; } 

                if ( pSoRoot ) 
                {   delete pSoRoot; pSoRoot = NULL; } 

                if ( pSoP1 ) 
                {   delete pSoP1; pSoP1 = NULL; } 

                if ( pSoP2 ) 
                {   delete pSoP2; pSoP2 = NULL; } 

            }

            std::string         sCode;

            SquirrelObject      *pSoRoot;

            SquirrelObject      *pSoFunction;

            SquirrelObject      *pSoP1;

            SquirrelObject      *pSoP2;

        };

        /// Event map type
        typedef std::map< std::string, SEventInfo > t_EventMap;

    public:

        /// Node type
        long                        lType;
        
        /// Flags
        long                        lFlags;

        /// Rotation speed info
        CSiVector                   vSelRotate;

        /// Current rotation speed
        CSiVector                   vCurRotate;

        /// Original rotation value
        CSiVector                   vOrgRotation;

        /// Current scale
        CSiVector                   vSelScale;

        /// Current scale
        CSiVector                   vCurScale;

        /// Original scale
        CSiVector                   vOrgScale;

        /// Scale increment
        float                       fScaleInc;

        /// Non-zero if we're currently twirling
        BOOL                        bTwirling;

        /// Event map
        t_EventMap                  sEventMap;

    public:

        CSiData() : vSelRotate( 0, 8, 0 ) 
        {   lType = 0;
            lFlags = 0; 
            bTwirling = FALSE; 
        }
    };

    /// Texture
    class CSiTexture
    {
    private:

        irr::video::ITexture    *m_pObj;

    public:

        /// Register squirrel interface
        static void Register( SquirrelVM &vm )
        {
            SqPlus::SQClassDef< CSiTexture >( vm, _T( "CSiTexture" ) )
                ;
        }

        CSiTexture() { m_pObj = NULL; }
        CSiTexture( irr::video::ITexture *p ) { m_pObj = p; if ( p ) p->grab(); }
        CSiTexture( CSiTexture &t ) { m_pObj = t.GetObj(); if ( m_pObj ) m_pObj->grab(); }
        ~CSiTexture() { Destroy(); }
        void Destroy() { if ( m_pObj ) { m_pObj->drop(); m_pObj = NULL; } }

        CSiTexture& operator = ( const CSiTexture &t ) { Destroy(); SetObj( t.GetObj() ); return *this; }

        void SetObj( const irr::video::ITexture *p ) { Destroy(); m_pObj = (irr::video::ITexture*)p; if ( m_pObj ) m_pObj->grab(); }
        irr::video::ITexture* GetObj() const { return (irr::video::ITexture*)m_pObj; }
        BOOL IsValid() { return NULL != m_pObj; }
        
        //----------------------------------------------------------
        // >>> CSiTexture - Squirrel functions
        //----------------------------------------------------------


    };
    SQIRR_DECLARE_INSTANCE( CSiTexture );


    class CSiNode
    {
    public:

        enum
        {
            /// Shows selection box when mouse hovers
            eSelectionBox   = 0x00000001,

            /// Twirls the object when mouse hovers
            eTwirl          = 0x00000002,

            /// Scales the object larger when mouse hovers
            eScale          = 0x00000004,
        };

        enum
        {
            /// Camera
            eTypeCamera         = 0x00000001,

            /// The node contains a mesh
            eTypeMesh           = 0x00000002,

            /// The node contains an animated mesh
            eTypeAnimatedMesh   = 0x00000004,

            /// Text
            eTypeText           = 0x00000008,

            /// Mask for all types
            eTypeAll            = 0x7fffffff
        };

    private:

        // Squirrel VM handle
        HSQUIRRELVM                 m_hVm;

        // Pointer to scene node
        irr::scene::ISceneNode      *m_pObj;

        // I can't hate this enough...
        CSiData                     *m_pData;

    public:

        /// Register squirrel interface
        static void Register( SquirrelVM &vm )
        {
            SqPlus::SQClassDef< CSiNode >( vm, _T( "CSiNode" ) )
                    . func( &CSiNode::setPosition,          _T( "setPosition" ) )
                    . func( &CSiNode::getPosition,          _T( "getPosition" ) )
                    . func( &CSiNode::getAbsolutePosition,  _T( "getAbsolutePosition" ) )
                    . func( &CSiNode::setAbsolutePosition,  _T( "setAbsolutePosition" ) )
                    . func( &CSiNode::isValid,              _T( "isValid" ) )
                    . func( &CSiNode::setRotation,          _T( "setRotation" ) )
                    . func( &CSiNode::getRotation,          _T( "getRotation" ) )
                    . func( &CSiNode::setParent,            _T( "setParent" ) )
                    . func( &CSiNode::faceTarget,           _T( "faceTarget" ) )
                    . func( &CSiNode::faceTargetY,          _T( "faceTargetY" ) )
                    . func( &CSiNode::faceTargetX,          _T( "faceTargetX" ) )
                    . func( &CSiNode::setFlags,             _T( "setFlags" ) )
                    . func( &CSiNode::getFlags,             _T( "getFlags" ) )
                    . func( &CSiNode::setScale,             _T( "setScale" ) )
                    . func( &CSiNode::getScale,             _T( "getScale" ) )
                    . func( &CSiNode::setMaterialFlag,      _T( "setMaterialFlag" ) )
                    . func( &CSiNode::setMaterialType,      _T( "setMaterialType" ) )
                    . func( &CSiNode::setCallback,          _T( "setCallback" ) )
                    . func( &CSiNode::setCallbackFunction,  _T( "setCallbackFunction" ) )                    
                    . func( &CSiNode::setCallbackFunction1, _T( "setCallbackFunction1" ) )
                    . func( &CSiNode::setCallbackFunction2, _T( "setCallbackFunction2" ) ) 
                    . func( &CSiNode::setTexture,           _T( "setTexture" ) )
                    . func( &CSiNode::setVisible,           _T( "setVisible" ) )
                    . func( &CSiNode::isVisible,            _T( "isVisible" ) )
                    
                    . enumInt( eSelectionBox,               _T( "eSelectionBox" ) )
                    . enumInt( eTwirl,                      _T( "eTwirl" ) )
                    . enumInt( eScale,                      _T( "eScale" ) )

                    . enumInt( eTypeMesh,                   _T( "eTypeMesh" ) )
                    . enumInt( eTypeAll,                    _T( "eTypeAll" ) )

                    // Material flags
                    . enumInt( (int)irr::video::EMF_WIREFRAME,           _T( "EMF_WIREFRAME" ) )
                    . enumInt( (int)irr::video::EMF_POINTCLOUD,          _T( "EMF_POINTCLOUD" ) )
                    . enumInt( (int)irr::video::EMF_GOURAUD_SHADING,     _T( "EMF_GOURAUD_SHADING" ) )
                    . enumInt( (int)irr::video::EMF_LIGHTING,            _T( "EMF_LIGHTING" ) )
                    . enumInt( (int)irr::video::EMF_ZBUFFER,             _T( "EMF_ZBUFFER" ) )
                    . enumInt( (int)irr::video::EMF_ZWRITE_ENABLE,       _T( "EMF_ZWRITE_ENABLE" ) )
                    . enumInt( (int)irr::video::EMF_BACK_FACE_CULLING,   _T( "EMF_BACK_FACE_CULLING" ) )
                    . enumInt( (int)irr::video::EMF_BILINEAR_FILTER,     _T( "EMF_BILINEAR_FILTER" ) )
                    . enumInt( (int)irr::video::EMF_TRILINEAR_FILTER,    _T( "EMF_TRILINEAR_FILTER" ) )
                    . enumInt( (int)irr::video::EMF_ANISOTROPIC_FILTER,  _T( "EMF_ANISOTROPIC_FILTER" ) )
                    . enumInt( (int)irr::video::EMF_FOG_ENABLE,          _T( "EMF_FOG_ENABLE" ) )
                    . enumInt( (int)irr::video::EMF_NORMALIZE_NORMALS,   _T( "EMF_NORMALIZE_NORMALS" ) )
                    . enumInt( (int)irr::video::EMF_TEXTURE_WRAP,        _T( "EMF_TEXTURE_WRAP" ) )
                    . enumInt( (int)irr::video::EMF_MATERIAL_FLAG_COUNT, _T( "EMF_MATERIAL_FLAG_COUNT" ) )

                    // Material types
                    . enumInt( (int)irr::video::EMT_SOLID,                                  _T( "EMT_SOLID" ) )
                    . enumInt( (int)irr::video::EMT_SOLID_2_LAYER,                          _T( "EMT_SOLID_2_LAYER" ) )
                    . enumInt( (int)irr::video::EMT_LIGHTMAP,                               _T( "EMT_LIGHTMAP" ) )
                    . enumInt( (int)irr::video::EMT_LIGHTMAP_ADD,                           _T( "EMT_LIGHTMAP_ADD" ) )
                    . enumInt( (int)irr::video::EMT_LIGHTMAP_M2,                            _T( "EMT_LIGHTMAP_M2" ) )
                    . enumInt( (int)irr::video::EMT_LIGHTMAP_M4,                            _T( "EMT_LIGHTMAP_M4" ) )
                    . enumInt( (int)irr::video::EMT_LIGHTMAP_LIGHTING,                      _T( "EMT_LIGHTMAP_LIGHTING" ) )
                    . enumInt( (int)irr::video::EMT_LIGHTMAP_LIGHTING_M2,                   _T( "EMT_LIGHTMAP_LIGHTING_M2" ) )
                    . enumInt( (int)irr::video::EMT_LIGHTMAP_LIGHTING_M4,                   _T( "EMT_LIGHTMAP_LIGHTING_M4" ) )
                    . enumInt( (int)irr::video::EMT_DETAIL_MAP,                             _T( "EMT_DETAIL_MAP" ) )
                    . enumInt( (int)irr::video::EMT_SPHERE_MAP,                             _T( "EMT_SPHERE_MAP" ) )
                    . enumInt( (int)irr::video::EMT_REFLECTION_2_LAYER,                     _T( "EMT_REFLECTION_2_LAYER" ) )
                    . enumInt( (int)irr::video::EMT_TRANSPARENT_ADD_COLOR,                  _T( "EMT_TRANSPARENT_ADD_COLOR" ) )
                    . enumInt( (int)irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL,              _T( "EMT_TRANSPARENT_ALPHA_CHANNEL" ) )
                    . enumInt( (int)irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF,          _T( "EMT_TRANSPARENT_ALPHA_CHANNEL_REF" ) )
                    . enumInt( (int)irr::video::EMT_TRANSPARENT_VERTEX_ALPHA,               _T( "EMT_TRANSPARENT_VERTEX_ALPHA" ) )
                    . enumInt( (int)irr::video::EMT_TRANSPARENT_REFLECTION_2_LAYER,         _T( "EMT_TRANSPARENT_REFLECTION_2_LAYER" ) )
                    . enumInt( (int)irr::video::EMT_NORMAL_MAP_SOLID,                       _T( "EMT_NORMAL_MAP_SOLID" ) )
                    . enumInt( (int)irr::video::EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR,       _T( "EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR" ) )
                    . enumInt( (int)irr::video::EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA,    _T( "EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA" ) )
                    . enumInt( (int)irr::video::EMT_PARALLAX_MAP_SOLID,                     _T( "EMT_PARALLAX_MAP_SOLID" ) )
                    . enumInt( (int)irr::video::EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR,     _T( "EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR" ) )
                    . enumInt( (int)irr::video::EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA,  _T( "EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA" ) )
                    . enumInt( (int)irr::video::EMT_ONETEXTURE_BLEND,                       _T( "EMT_ONETEXTURE_BLEND" ) )

                    // Camera
                    . func( &CSiNode::setTarget,                _T( "setTarget" ) )

                    // Mesh

                    // Text
                    . func( &CSiNode::setText,                  _T( "setText" ) )
                    
                ;
        }

        CSiNode() { m_pObj = NULL; m_pData = NULL; m_hVm = NULL; }

        void Destroy() { m_pObj = NULL; m_pData = NULL; m_hVm = NULL; }

        void SetObj( HSQUIRRELVM x_hVm, CSiData *pData, irr::scene::ISceneNode *p ) 
        {   m_hVm = x_hVm; m_pData = pData; if ( m_pData ) m_pData->lType = 0; m_pObj = p; }
        
        void SetObj( HSQUIRRELVM x_hVm, CSiData *pData, irr::scene::ICameraSceneNode *p ) 
        {   m_hVm = x_hVm; 
            m_pData = pData; 
            if ( m_pData ) m_pData->lType = eTypeCamera; 
            m_pObj = p; 
        }
        void SetObj( HSQUIRRELVM x_hVm, CSiData *pData, irr::scene::IMeshSceneNode *p ) 
        {   m_hVm = x_hVm; 
            m_pData = pData; 
            if ( m_pData ) m_pData->lType = eTypeMesh; 
            m_pObj = p; 
        }
        void SetObj( HSQUIRRELVM x_hVm, CSiData *pData, irr::scene::IAnimatedMeshSceneNode *p ) 
        {   m_hVm = x_hVm; 
            m_pData = pData; 
            if ( m_pData ) m_pData->lType = eTypeAnimatedMesh; 
            m_pObj = p; 
        }
        void SetObj( HSQUIRRELVM x_hVm, CSiData *pData, irr::scene::ITextSceneNode *p ) 
        {   m_hVm = x_hVm; 
            m_pData = pData; 
            if ( m_pData ) m_pData->lType = eTypeText; 
            m_pObj = p; 
        }

        BOOL IsType( int nType )
        {   return ( m_pObj && m_pData && nType == m_pData->lType );
        }

        
        CSiData* GetData() { return m_pData; }            

        irr::scene::ISceneNode* GetObj() { return m_pObj; }

        // Camera
        irr::scene::ICameraSceneNode* AsCamera() 
        {   if ( !m_pData || eTypeCamera != m_pData->lType ) return NULL;
            return (irr::scene::ICameraSceneNode*)m_pObj; 
        }

        static BOOL TransitionVector( sqirr::CSiVector &a, sqirr::CSiVector &b, float div, float min )
        {
            float dx = b.v().X - a.v().X;
            float dy = b.v().Y - a.v().Y;
            float dz = b.v().Z - a.v().Z;

            if ( !dx && !dy && !dz ) 
                return FALSE;

            if ( div < 1 ) div = 1;

            if ( dx < min && dx > -min ) a.v().X = b.v().X;
            else a.v().X += dx / div;

            if ( dy < min && dy > -min ) a.v().Y = b.v().Y;
            else a.v().Y += dy / div;

            if ( dz < min && dz > -min ) a.v().Z = b.v().Z;
            else a.v().Z += dz / div;

            return TRUE;
        }

        void SetPosition( const CSiVector &v ) 
        {   if ( !m_pObj ) return;
            m_pObj->setPosition( v.c_v() ); 
            FaceTarget( CSiVector( 0, 0, 0 ) );
        }

        void FaceTarget( const CSiVector &v )
        {   if ( !m_pObj ) return;
            irr::core::vector3df rot, dif = m_pObj->getPosition() - v.c_v();
            rot.Y = atan2( dif.X, dif.Z ) * 180.f / irr::core::PI;
            rot.X = -atan2( dif.Y, sqrt( dif.X * dif.X + dif.Z * dif.Z ) ) * 180.f / irr::core::PI;
            m_pObj->setRotation( rot );
        }

        void FaceTargetY( const CSiVector &v )
        {   if ( !m_pObj ) return;
            irr::core::vector3df rot, dif = m_pObj->getPosition() - v.c_v();
            rot.Y = atan2( dif.X, dif.Z ) * 180.f / irr::core::PI;
            m_pObj->setRotation( rot );
        }

        void StartTwirl()
        {   if ( !m_pObj || !m_pData ) return;
            m_pData->vCurRotate.set( 0, 0, 0 );
            m_pData->vOrgRotation = getRotation();
            m_pData->vOrgScale = m_pObj->getScale();
            m_pData->vCurScale = m_pData->vOrgScale;
            m_pData->vSelScale = m_pData->vOrgScale;
            m_pData->vSelScale.inflate( 1.5 );          
            m_pData->fScaleInc = ( m_pData->vSelScale.x() - m_pData->vOrgScale.x() ) / 64;
            m_pData->bTwirling = TRUE;
        }

        void Twirl()
        {   if ( !m_pObj || !m_pData ) return;

            // Transition rotation speed
            TransitionVector( m_pData->vCurRotate, m_pData->vSelRotate, .01, .01 );            

            CSiVector v = getRotation();
            v.v().X += m_pData->vCurRotate.x();
            if ( 0 > v.v().X ) v.v().X += 360; 
            else if ( 360 <= v.v().X ) v.v().X = 0;

            v.v().Y += m_pData->vCurRotate.y();
            if ( 0 > v.v().Y ) v.v().Y += 360; 
            else if ( 360 <= v.v().Y ) v.v().Y = 0;

            v.v().Z += m_pData->vCurRotate.z();
            if ( 0 > v.v().Z ) v.v().Z += 360; 
            else if ( 360 <= v.v().Z ) v.v().Z = 0;

            setRotation( v );

            TransitionVector( m_pData->vCurScale, m_pData->vSelScale, 4, m_pData->fScaleInc );
            setScale( m_pData->vCurScale );

        }

        BOOL EndTwirl()
        {   if ( !m_pObj || !m_pData || !m_pData->bTwirling ) return TRUE;                        
            CSiVector v = getRotation();
            BOOL bDif = TransitionVector( v, m_pData->vOrgRotation, 4, 1 );
            if ( TransitionVector( m_pData->vCurScale, m_pData->vOrgScale, 4, m_pData->fScaleInc ) ) bDif = TRUE;
            setScale( m_pData->vCurScale );
            setRotation( v );
            if ( !bDif ) m_pData->bTwirling = FALSE;
            return !bDif;
        }

        //----------------------------------------------------------
        // >>> CSiNode - Squirrel functions
        //----------------------------------------------------------

        //==============================================================
        // isValid()
        //==============================================================
        /// Returns non-zero if the scene object is valid
        BOOL isValid() { return ( NULL != m_pObj ); }

        //==============================================================
        // setRotation()
        //==============================================================
        /// Sets the rotation of the scene node
        /**
            \param [in] v   -   Rotation vector in degrees ( 0 - 360 )
        */
        void setRotation( CSiVector &v )
        {   if ( m_pObj ) m_pObj->setRotation( v.c_v() );
        }

        //==============================================================
        // getRotation()
        //==============================================================
        /// Returns the current scene node rotation
        /**
            \return Rotation in degrees ( 0 - 360 )
        */
        CSiVector getRotation() 
        {   if ( m_pObj ) return m_pObj->getRotation();
            return CSiVector();
        }

        //==============================================================
        // setPosition()
        //==============================================================
        /// Sets the scene node position
        /**
            \param [in] v   -   Position vector
        */
        void setPosition( CSiVector &v )
        {   if ( !m_pObj ) return;
            m_pObj->setPosition( v.c_v() ); 
        }

        //==============================================================
        // setAbsolutePosition()
        //==============================================================
        /// Sets the scene node position
        /**
            \param [in] v   -   Position vector
        */
        void setAbsolutePosition( CSiVector &x_v )
        {   if ( !m_pObj ) return;

            irr::scene::ISceneNode *pParent =  m_pObj->getParent();
            if ( !pParent ) m_pObj->setPosition( x_v.c_v() );
            else
            {   irr::core::vector3df v( x_v.c_v() - pParent->getAbsolutePosition() );
                pParent->getAbsoluteTransformation().inverseRotateVect( v );
                m_pObj->setPosition( v ); 
            } // end else
            m_pObj->updateAbsolutePosition();
        }

        void setAbsolutePosition2( irr::scene::ISceneNode *pNode, irr::core::vector3df pos )
        {   if ( !pNode ) return;

            irr::scene::ISceneNode *pParent =  pNode->getParent();
            if ( !pParent ) pNode->setPosition( pos );
            else
            {   irr::core::vector3df v( pos - pParent->getAbsolutePosition() );
                pParent->getAbsoluteTransformation().inverseRotateVect( v );
                pNode->setPosition( v ); 
            } // end else
            pNode->updateAbsolutePosition();
        }


        //==============================================================
        // getPosition()
        //==============================================================
        /// Returns the scene node position vector
        CSiVector getPosition()
        {   if ( !m_pObj ) return CSiVector();
            return CSiVector( m_pObj->getPosition() ); 
        }

        //==============================================================
        // getAbsolutePosition()
        //==============================================================
        /// Returns the scene node position vector
        BOOL getAbsolutePosition( CSiVector &pos )
        {   if ( !m_pObj ) return FALSE;
            pos = m_pObj->getAbsolutePosition();
            return TRUE;
        }

        //==============================================================
        // setScale()
        //==============================================================
        /// Sets the scene node scale factor
        /**
            \param [in] v   -   Scene node scale factor
        */
        void setScale( CSiVector &v )
        {   if ( !m_pObj ) return;
            m_pObj->setScale( v.c_v() ); 
        }

        //==============================================================
        // getScale()
        //==============================================================
        /// Returns the scene node scale factor
        CSiVector getScale()
        {   if ( !m_pObj ) return CSiVector();
            return m_pObj->getScale(); 
        }

        //==============================================================
        // setMaterialFlag()
        //==============================================================
        /// Sets the material flags
        /**
            \param [in] lFlag   -   Material flag
            \param [in] lVal    -   Value
        */
        void setMaterialFlag( long lFlag, long lVal )
        {   if ( !m_pObj ) return;
            m_pObj->setMaterialFlag( (irr::video::E_MATERIAL_FLAG)lFlag, lVal );
        }

        //==============================================================
        // setMaterialType()
        //==============================================================
        /// Sets the material type
        /**
            \param [in] lType   -   Material type
        */
        void setMaterialType( long lType )
        {   if ( !m_pObj ) return;
            m_pObj->setMaterialType( (irr::video::E_MATERIAL_TYPE)lType );
        }

        //==============================================================
        // setParent()
        //==============================================================
        /// Sets the scene node parent
        /**
            \param [in] n   -   Parent node

            This node will be rotated, positioned, and scaled with it's
            parent.  Other attributes such as visibility are also 
            inherited.

        */
        void setParent( CSiNode &n )
        {   if ( m_pObj ) m_pObj->setParent( n.GetObj() );
        }

        //==============================================================
        // faceTarget()
        //==============================================================
        /// Rotates the node about the X and Y axis so it faces the target.
        /**
            \param [in] v   -   Point to face.

            Amounts to a pan and tilt

        */
        void faceTarget( CSiVector &v )
        {   if ( !m_pObj ) return;
            irr::core::vector3df rot, dif = m_pObj->getPosition() - v.c_v();
            rot.Y = atan2( dif.X, dif.Z ) * 180.f / irr::core::PI;
            rot.X = -atan2( dif.Y, sqrt( dif.X * dif.X + dif.Z * dif.Z ) ) * 180.f / irr::core::PI;
            m_pObj->setRotation( rot );
        }

        //==============================================================
        // faceTargetY()
        //==============================================================
        /// Rotates the node about the Y axis so it faces the target.
        /**
            \param [in] v   -   Point to face.

            Amounts to a pan only
        */
        void faceTargetY( CSiVector &v )
        {   if ( !m_pObj ) return;
            irr::core::vector3df rot, dif = m_pObj->getPosition() - v.c_v();
            rot.Y = atan2( dif.X, dif.Z ) * 180.f / irr::core::PI;
            m_pObj->setRotation( rot );
        }

        //==============================================================
        // faceTargetX()
        //==============================================================
        /// Rotates the node about the X axis so it faces the target.
        /**
            \param [in] v   -   Point to face.

            Amounts to a tilt only
        */
        void faceTargetX( CSiVector &v )
        {   if ( !m_pObj ) return;
            irr::core::vector3df rot, dif = m_pObj->getPosition() - v.c_v();
            rot.X = -atan2( dif.Y, sqrt( dif.X * dif.X + dif.Z * dif.Z ) ) * 180.f / irr::core::PI;
            m_pObj->setRotation( rot );
        }

        //==============================================================
        // setFlags()
        //==============================================================
        /// Sets scene node flags
        /**
            \param [in] lFlags  -   Flags
        */
        void setFlags( long lFlags ) 
        {   if ( m_pData ) m_pData->lFlags = lFlags; }

        //==============================================================
        // getFlags()
        //==============================================================
        /// Returns scene node flags
        long getFlags() 
        {   if ( m_pData ) return m_pData->lFlags; return 0; }

        //==============================================================
        // setCallback()
        //==============================================================
        /// Sets a piece of squirrel script that is executed in response
        /// to some action.
        /**
            \param [in] pAction     -   Action id
                                            "onclick"    -  LButton click
                                            "onhover"    -  Mouse hover
                                            "onleave"    -  Mouse leave
                                            "onrclick"   -  RButton click
                                            "onmclick"   -  MButton click
                                            "onscrollup" -  Mouse scroll up
                                            "onscrolldn" -  Mouse scroll down

            \param [in] pCode       -   Squirrel script code to be 
                                        executed.

            The action must occur while the mouse is positioned over the object.

            \note   You must call _irr.AcceptInput() to enable mouse interaction 
                    for callbacks to work.
            
        */
        BOOL setCallback( LPCTSTR pAction, LPCTSTR pCode )
        {   if ( !m_pData || !pAction || !pCode ) return FALSE;            
            m_pData->sEventMap[ pAction ].sCode = pCode;
            return TRUE;
        }

        //==============================================================
        // setCallbackFunction<n>()
        //==============================================================
        /// Sets a piece of squirrel script that is executed in response
        /// to some action.
        /**
            \param [in] pAction     -   Action id
            \param [in] pFunction   -   Squirrel function to be executed.
            \param [in] soP<n>        -   Squirrel object.  Passed as to
                                        the callbackfunction as the n^th
                                        parameter.

            \see setCallback()
            
        */
        BOOL setCallbackFunction( LPCTSTR pAction, SquirrelObject soFunction )
        {   if ( !m_pData || !pAction || NULL == &soFunction ) return FALSE;        
            m_pData->sEventMap[ pAction ].pSoFunction = new SquirrelObject( m_hVm );
            *m_pData->sEventMap[ pAction ].pSoFunction = soFunction;
            return TRUE;
        }
        BOOL setCallbackFunction1( LPCTSTR pAction, SquirrelObject soFunction, SquirrelObject soP1 )
        {   if ( !m_pData || !pAction || NULL == &soFunction || NULL == &soP1 ) return FALSE;        
            m_pData->sEventMap[ pAction ].pSoFunction = new SquirrelObject( m_hVm );
            *m_pData->sEventMap[ pAction ].pSoFunction = soFunction;
            m_pData->sEventMap[ pAction ].pSoP1 = new SquirrelObject( m_hVm );
            *m_pData->sEventMap[ pAction ].pSoP1 = soP1;
            return TRUE;
        }
        BOOL setCallbackFunction2( LPCTSTR pAction, SquirrelObject soFunction, SquirrelObject soP1, SquirrelObject soP2 )
        {   if ( !m_pData || !pAction || NULL == &soFunction || NULL == &soP1 || NULL == &soP2 ) return FALSE;        
            m_pData->sEventMap[ pAction ].pSoFunction = new SquirrelObject( m_hVm );
            *m_pData->sEventMap[ pAction ].pSoFunction = soFunction;
            m_pData->sEventMap[ pAction ].pSoP1 = new SquirrelObject( m_hVm );
            *m_pData->sEventMap[ pAction ].pSoP1 = soP1;
            m_pData->sEventMap[ pAction ].pSoP2 = new SquirrelObject( m_hVm );
            *m_pData->sEventMap[ pAction ].pSoP2 = soP2;
            return TRUE;
        }


	    //==============================================================
	    // setTexture()
	    //==============================================================
	    /// Sets the texture for the specified material(s)
	    /**
		    \param [in] lMaterial   -   Material index. -1 for all.
		    \param [in] st          -   Texture object
	    */
        void setTexture( long lMaterial, CSiTexture &st ) 
        {   if ( !m_pObj ) return;
            
            if ( 0 <= lMaterial )
                m_pObj->setMaterialTexture( lMaterial, st.GetObj() );

            else for ( int i = 0; i < m_pObj->getMaterialCount(); i++ )
                m_pObj->setMaterialTexture( i, st.GetObj() ); 
        }

	    //==============================================================
	    // setVisible()
	    //==============================================================
	    /// Sets the visibility of the object
	    /**
		    \param [in] bVisible    -   Non-zero to make the object visible.
	    */
        void setVisible( BOOL bVisible )
        {   if ( m_pObj ) m_pObj->setVisible( bVisible );
        }

	    //==============================================================
	    // isVisible()
	    //==============================================================
	    /// Returns non-zero if the object is visible
        BOOL isVisible() 
        {   if ( !m_pObj ) return FALSE;
            return m_pObj->isVisible();
        }

        //----------------------------------------------------------
        // Camera functions
        //----------------------------------------------------------

        void setTarget( CSiVector &v )
        {   
            if ( !m_pObj || !m_pData || eTypeCamera != m_pData->lType ) 
                return;
            
            ( (irr::scene::ICameraSceneNode*)m_pObj )->setTarget( v.c_v() );
        }

        //----------------------------------------------------------
        // Mesh functions
        //----------------------------------------------------------


        //----------------------------------------------------------
        // Text functions
        //----------------------------------------------------------

        void setText( LPCTSTR pText )
        {   
            if ( !m_pObj || !m_pData || eTypeText != m_pData->lType ) 
                return;
            
            ( (irr::scene::ITextSceneNode*)m_pObj )->setText( CComBSTR( pText ) );
        }


    };
    SQIRR_DECLARE_INSTANCE( CSiNode );


    /// Registers the Irrlicht interface classes to Squirrel
    static void Register( SquirrelVM &vm )
    {
        CSiVector::Register( vm );
        CSiVector2d::Register( vm ); );
        CSiTexture::Register( vm );
        CSiLine::Register( vm
        CSiNode::Register( vm );
    }

};

