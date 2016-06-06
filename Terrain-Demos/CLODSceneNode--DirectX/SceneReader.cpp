//!
//! SCENEREADER
//! Based on IrrXML, can parse a file a organize it on a tree-like fashion
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.

#include "SceneReader.h"
#include "iostream"
using namespace irr;

core::stringw ParserData::toWideString( const core::stringc str ) {
    int len = str.size() + 1;
    wchar_t* buf = new wchar_t[len];
    mbstowcs( buf, str.c_str(), len );
    core::stringw wstr = buf;
    delete[] buf;
    return wstr;
}

core::stringc ParserData::toString( const core::stringw str ) {
    int len = str.size() + 1;
    c8* buf = new c8[len];
    wcstombs(buf,str.c_str(),len);
    core::stringc wstr = buf;
    delete[] buf;
    return wstr;
}

core::stringw ParserData::floatToWChar( float value ) {
    char px[100] = "      ";
    sprintf( px, "%.2f", value );
    return toWideString( px );
}

core::stringc ParserData::floatToc8( float value ) {
    char px[100] = "      ";
    sprintf( px, "%.2f", value );
    return px;
}

int ParserData::wcharToInt( core::stringw text ) {
    return atoi( toString( text ).c_str() );
}

float ParserData::wcharToFloat( core::stringw text ) {
    return atof( toString( text ).c_str() );
}

int ParserData::IntValue() {
    return atoi( data.c_str() );
}

float ParserData::FloatValue() {
    return atof( data.c_str() );
}

float ParserData::BoolValue() {
    return ( data.equals_ignore_case( "true" ) );
}

core::stringc ParserData::Value() {
    return data;
};

core::stringw ParserData::ValueW() {
    return toWideString( data );
}

core::stringw ParserData::NameW() {
    return toWideString( name );
}

///=============================================================================

ParserLeaf::ParserLeaf( ParserLeaf* Parent ) {
    parent = Parent;
    showDebug = false;
};

ParserLeaf::~ParserLeaf() {
};

void ParserLeaf::setShowDebug( bool value ) {
    showDebug = value;
    for ( int i = 0; i < childs.size(); i++ ) {
        childs[i]->setShowDebug( value );
    }
}

ParserLeaf* ParserLeaf::addChild() {
    ParserLeaf* child = new ParserLeaf( this );
    child->showDebug = showDebug;
    childs.push_back( child );
    return child;
};

ParserData* ParserLeaf::addData() {
    ParserData* data = new ParserData( );
    datas.push_back( data );
    return data;
};

ParserLeaf* ParserLeaf::addChild( core::stringc childname ) {
    ParserLeaf* p = addChild();
    p->name = childname;
    return p;
}

ParserData* ParserLeaf::addData( core::stringc dataname ) {
    ParserData* d = addData();
    d->name = dataname;
    return d;
}

int ParserLeaf::childCount() {
    return childs.size();
}

int ParserLeaf::dataCount()
{
    return datas.size();
}

ParserLeaf* ParserLeaf::getChild( core::stringc LookName ) {
    for ( int i = 0; i < childs.size(); i++ ) {
        if ( childs[i] ) {
            if ( LookName.equals_ignore_case( childs[ i ]->name ) ) {
                if ( showDebug )
                    printf( "Got child: %s\n", childs[i]->name.c_str() );
                return childs[i];
            }
        }
    }
    if ( showDebug )
        printf( "Child <%s> not found on <%s>\n", LookName.c_str(), name.c_str() );
    return 0;
}

ParserLeaf* ParserLeaf::getChild( int index ) {
    if ( ( index >= 0 ) && ( index < childCount() ) ) {
        if ( showDebug )
            printf(  "Got child %i: %s\n", index, childs[index]->name.c_str() );
        return childs[ index ];
    }
    if ( showDebug )
        printf(  "Got child: <null>\n" );
    return 0;
}

ParserData* ParserLeaf::getData( core::stringc LookName ) {
    for ( int i = 0; i < datas.size(); i++ ) {
        if ( LookName.equals_ignore_case( datas[ i ]->name ) ) {
            if ( showDebug )
                printf( "Got data name < %s > value < %s >\n", datas[ i ]->name.c_str(), datas[ i ]->Value().c_str() );
            return datas[i];
        }
    }
    if ( showDebug )
        printf( "Data not found for < %s > : look for %s \n", name.c_str(), LookName.c_str() );
    return 0;
}

ParserData* ParserLeaf::getData( int index ) {
    if ( ( index >= 0 ) && ( index < datas.size() ) ) {
        if ( showDebug )
            printf( "Got data name < %s > value < %s > index < %i >\n", datas[ index ]->name.c_str(), datas[ index ]->Value().c_str(), index );
        return datas[ index ];
    }
    if ( showDebug )
        printf( "Data not found for < %s > : index %i \n", name.c_str(), index );
    return 0;
}

core::stringw ParserLeaf::NameW() {
    int len = name.size() + 1;
    wchar_t* buf = new wchar_t[len];
    mbstowcs( buf, name.c_str(), len );
    core::stringw wstr = buf;
    delete[] buf;
    return wstr;
}

///=============================================================================

core::stringw XMLParser::strRep( core::stringw str, int numreps ) {
    core::stringw ret;
    for ( int i = 0; i < numreps; i++ ) {
        ret += str;
    }
    return ret;
}

XMLParser::XMLParser( IrrlichtDevice* device ) {
    Device = device;
    showDebug = false;
};

XMLParser::~XMLParser() {}
;

void XMLParser::setShowDebug( bool value ) {
    showDebug = value;
}

void XMLParser::parseFile( core::stringc FileName ) {
    if ( showDebug )
        printf( "Start parsing file %s\n", FileName.c_str() );

    ParserLeaf* CurrentLeaf = &RootLeaf;

    io::IFileSystem* fs = Device->getFileSystem();
    io::IXMLReader* xml = fs->createXMLReader( FileName.c_str() );

    core::stringc nodename;

    level = 0;

    // parse the file until end reached
    while(xml->read()) {
        nodename = xml->getNodeName();

        switch ( xml->getNodeType() ) {
            case io::EXN_NONE:
            break;
            case io::EXN_ELEMENT: {
                CurrentLeaf = CurrentLeaf->addChild();

                level ++;
                CurrentLeaf->name = xml->getNodeName();

                if ( showDebug ) {
                    wprintf( strRep(L" ", level*4).c_str() );
                    printf(  "%s\n", CurrentLeaf->name.c_str() );
                }

                for ( int i = 0; i < xml->getAttributeCount(); i++ ) {
                    ParserData* pdata = CurrentLeaf->addData();

                    pdata->name = xml->getAttributeName( i );
                    pdata->data = xml->getAttributeValue( i );

                    if ( showDebug ) {
                        wprintf( strRep(L" ", (level+1)*4).c_str() );
                        printf(  "%s: %s\n", pdata->name.c_str(), pdata->Value().c_str() );
                    }
                }

                // if its a empty element, add it and go back one level
                if ( xml->isEmptyElement() ) {
                    printf( "Emtpty element found.\n" );
                    if ( CurrentLeaf->parent ) {
                        CurrentLeaf = CurrentLeaf->parent;
                        level --;
                    }
                }

                break;

            }
            case io::EXN_ELEMENT_END: {
                if ( CurrentLeaf->parent ) {
                    CurrentLeaf = CurrentLeaf->parent;
                    level --;
                }
                break;
            }
            case io::EXN_TEXT: {
                ParserData* leaf = CurrentLeaf->addData( xml->getNodeData() );
                leaf->data = xml->getNodeData();
            }
            break;
            case io::EXN_COMMENT:
            break;
            case io::EXN_CDATA:
            break;
            case io::EXN_UNKNOWN:
            break;
        }
    }

    // delete the xml parser after usage
    delete xml;
    xml = 0;
    //        fs->changeWorkingDirectoryTo( temppath.c_str() );
};


///=============================================================================
core::stringc extractFileName( core::stringc filename )
{
    // retira / ou \\ do final do nome do arquivo
    int fl = filename.findLast( '/' );
    if ( fl == -1 )
        fl = filename.findLast( '\\' );
    if ( fl > -1 )
        filename = filename.subString( fl+1, filename.size() - fl );
    return filename;
}

scene::IAnimatedMesh* VisualEditorReader::LoadMesh( core::stringc filename ) {
    // retira / ou \\ do final do nome do arquivo
    int fl = filename.findLast( '/' );
    if ( fl == -1 )
        fl = filename.findLast( '\\' );
    if ( fl > -1 )
        filename = filename.subString( fl+1, filename.size() - fl );

    io::IFileSystem* fs = Device->getFileSystem();
    core::stringc fn = "";
    for ( int i = 0; i < searchpaths.size(); i++ ) {
        fn = searchpaths[ i ] + filename;
        if ( showDebug )
            printf(  fn.c_str() );
        if ( fs->existFile( fn.c_str() ) ) {
            if ( showDebug )
                printf(  "LoadMesh loaded %s\n", fn.c_str() );
            return Device->getSceneManager()->getMesh( fn.c_str() );
        }
    }
    if ( showDebug )
        printf( "Cannot load mesh %s\n", fn.c_str() );
    return 0;
}

video::ITexture* VisualEditorReader::LoadTexture( core::stringc filename ) {
    int fl = filename.findLast( '/' );

    if ( fl == -1 )
        fl = filename.findLast( '\\' );

    if ( fl > -1 )
        filename = filename.subString( fl+1, filename.size() - fl );

    io::IFileSystem* fs = Device->getFileSystem();
    core::stringc fn;
    for ( int i = 0; i < searchpaths.size(); i++ ) {
        fn = searchpaths[ i ] + filename;
        if ( fs->existFile( fn.c_str() ) ) {
            if ( showDebug )
                printf( "LoadTexture loaded %s\n", fn.c_str() );
            return Device->getVideoDriver()->getTexture( fn.c_str() );
        }
    }
    if ( showDebug )
        printf( "Cannot load texture %s\n", fn.c_str() );
    return 0;
}

scene::ISceneNode* VisualEditorReader::ReadMainParameters( ParserLeaf* MainParametersLeaf ) {
    scene::ISceneNode* retnode = 0;
    scene::ISceneNode* parnode = 0;

    ParserLeaf* l = MainParametersLeaf;
    core::stringc nodetype = l->getChild( L"TypeOfNode" )->getData( 0 )->Value();
    core::stringw nodename = l->getChild( L"NameOfNode" )->getData( 0 )->ValueW();
    int nodeid = l->getChild( L"IDOfNode" )->getData( 0 )->IntValue();

    scene::ISceneManager* smgr = Device->getSceneManager();

        // adds a new nodeholder
        addNewNode();

    // find node parent
    core::stringw nodepar = l->getChild( L"ParentOfNode" )->getData( 0 )->ValueW();
    int nodeparid = l->getChild( L"ParentOfNode" )->getData( 1 )->IntValue();
    if ( nodeparid > -1 )     parnode = Device->getSceneManager()->getSceneNodeFromId( nodeparid );
//    if ( nodepar.size() > 0 ) parnode = Device->getSceneManager()->getSceneNodeFromName( nodepar.c_str() );

    if ( nodetype == core::stringc( "irr::scene::CSkyBoxSceneNode" ) ) {
        retnode = smgr->addSkyBoxSceneNode( 0, 0, 0, 0, 0, 0, 0, -1 );
    }
    if ( ( nodetype == core::stringc( "irr::scene::COctTreeSceneNode" ) ) ||
         ( nodetype == core::stringc( "irr::scene::CAnimatedMeshSceneNode" ) ) ) {
        // have a file name? - its a mesh
        ParserLeaf* pathleaf = l->getChild( L"Path" );
        if ( pathleaf ) {
            core::stringc fn = l->getChild( L"Path" )->getData( L"Filename" )->Value();
            if ( showDebug )
                printf(  fn.c_str() );
            scene::IAnimatedMesh* mesh = LoadMesh( fn.c_str() );
            currentnode->mesh = mesh;
            currentnode->meshfilename = extractFileName( fn );
            retnode = smgr->addOctTreeSceneNode( mesh, 0, -1, 128 );
        }
    }
    if ( nodetype == core::stringc( "irr::scene::CLightSceneNode" ) ) {}
    if ( nodetype == core::stringc( "irr::scene::CParticleSceneNode" ) ) {}
    if ( nodetype == core::stringc( "irr::scene::CWaterSurfaceSceneNode" ) ) {
        ParserLeaf* WLeaf = l->getChild( "HillPlaneMesh" );
        if ( WLeaf ) {
            core::stringc MeshName  = WLeaf->getData( 0 )->Value();
            float TileSizeW         = WLeaf->getData( 1 )->FloatValue();
            float TileSizeH         = WLeaf->getData( 2 )->FloatValue();
            int TileCountW          = WLeaf->getData( 3 )->IntValue();
            int TileCountH          = WLeaf->getData( 4 )->IntValue();

            WLeaf = l->getChild( "HillPlaneMeshA" );
            float HillHeight        = WLeaf->getData( 0 )->FloatValue();
            float CountHillsW       = WLeaf->getData( 1 )->FloatValue();
            float CountHillsH       = WLeaf->getData( 2 )->FloatValue();
            float TexRepCW          = WLeaf->getData( 3 )->FloatValue();
            float TexRepCH          = WLeaf->getData( 4 )->FloatValue();
            WLeaf = WLeaf->parent;

            WLeaf = l->getChild( "WaterParams" );
            float WaveHeight        = WLeaf->getData( 0 )->FloatValue();
            float WaveSpeed         = WLeaf->getData( 1 )->FloatValue();
            float WaveLength        = WLeaf->getData( 2 )->FloatValue();

            scene::IAnimatedMesh* m = smgr->addHillPlaneMesh( MeshName.c_str(),
                                      core::dimension2d< f32 >( TileSizeW, TileSizeH ),
                                      core::dimension2d< u32 >( TileCountW, TileCountH ),
                                      0,
                                      HillHeight,
                                      core::dimension2d< f32 >( CountHillsW, CountHillsH ),
                                      core::dimension2d< f32 >( TexRepCW, TexRepCH ) );

            currentnode->mesh = m;

            retnode = smgr->addWaterSurfaceSceneNode( m->getMesh(0),
                      WaveHeight,
                      WaveSpeed, WaveLength, 0, -1 );
        }
    }
    if ( nodetype == core::stringc( "irr::scene::CBillboardSceneNode" ) ) {
        retnode = smgr->addBillboardSceneNode();
    }

    if ( retnode ) {
        retnode->setName( nodename.c_str() );
        retnode->setID( nodeid );
        if ( parnode ) {
            retnode->setParent( parnode );
            if ( showDebug )
                printf( "Parent of node %S changed to %S\n", retnode->getName(), parnode->getName() );
        };
    }

    currentnode->node = retnode;
    return retnode;
}

void VisualEditorReader::ReadCoordinates( ParserLeaf* CoordinatesLeaf, scene::ISceneNode* readnode ) {

    ParserLeaf* PosLeaf = CoordinatesLeaf->getChild( 0 );
    ParserLeaf* RotLeaf = CoordinatesLeaf->getChild( 1 );
    ParserLeaf* ScaLeaf = CoordinatesLeaf->getChild( 2 );
    // Billboards have a size node with Coordinates
    ParserLeaf* SizLeaf = CoordinatesLeaf->getChild( "Size" );

    readnode->setPosition( core::vector3df(
                                PosLeaf->getData( 0 )->FloatValue(),
                                PosLeaf->getData( 1 )->FloatValue(),
                                PosLeaf->getData( 2 )->FloatValue() ) * wscale );
    readnode->setRotation( core::vector3df(
                                RotLeaf->getData( 0 )->FloatValue(),
                                RotLeaf->getData( 1 )->FloatValue(),
                                RotLeaf->getData( 2 )->FloatValue() ) );

    float nscale = 1.0f;
    if ( readnode->getParent() == Device->getSceneManager()->getRootSceneNode() )
        nscale = wscale;

    readnode->setScale( core::vector3df(
                                ScaLeaf->getData( 0 )->FloatValue(),
                                ScaLeaf->getData( 1 )->FloatValue(),
                                ScaLeaf->getData( 2 )->FloatValue() ) * nscale );
    printf( "Scalled to %f  %f  %f\n", readnode->getScale().X, readnode->getScale().Y, readnode->getScale().Z );
    // Billboards have a size node with Coordinates
    if ( SizLeaf ) {
        scene::IBillboardSceneNode* bb = (scene::IBillboardSceneNode*)readnode;
        bb->setSize( core::dimension2d< f32 > (
                    SizLeaf->getData( 0 )->FloatValue() * wscale,
                    SizLeaf->getData( 1 )->FloatValue() * wscale ) );
    }
}

void VisualEditorReader::ReadColor( ParserLeaf* ColorLeaf, video::SColor* color ) {
    color->setAlpha( ColorLeaf->getData( 0 )->IntValue() );
    color->setRed( ColorLeaf->getData( 1 )->IntValue() );
    color->setGreen( ColorLeaf->getData( 2 )->IntValue() );
    color->setBlue( ColorLeaf->getData( 3 )->IntValue() );
}

void VisualEditorReader::ReadMaterialParameters( ParserLeaf* MatParams, scene::ISceneNode* readnode ) {
    ParserLeaf* l = MatParams;
    int count = l->getChild( 0 )->getData( 0 )->IntValue();
    video::SMaterial* mat;
    for ( int i = 1; i <= count; i++ ) {
        l = l->getChild( i );
        mat = &readnode->getMaterial( i-1 );
        mat->MaterialType = (video::E_MATERIAL_TYPE)l->getChild(0)->getData( 0 )->IntValue();
        mat->Wireframe =         l->getChild(1)->getData( 0 )->BoolValue();
        mat->GouraudShading =    l->getChild(2)->getData( 0 )->BoolValue();
        mat->Lighting =          l->getChild(3)->getData( 0 )->BoolValue();
        mat->ZBuffer =           l->getChild(4)->getData( 0 )->BoolValue();
        mat->ZWriteEnable =      l->getChild(5)->getData( 0 )->BoolValue();
        mat->BackfaceCulling =   l->getChild(6)->getData( 0 )->BoolValue();
///        mat->BilinearFilter =    l->getChild(7)->getData( 0 )->BoolValue();
///        mat->TrilinearFilter =   l->getChild(8)->getData( 0 )->BoolValue();
        mat->FogEnable =         l->getChild(9)->getData( 0 )->BoolValue();
        mat->Shininess =         l->getChild(10)->getData( 0 )->BoolValue();
        ReadColor( l->getChild( L"AmbientColor" ), &mat->AmbientColor );
        ReadColor( l->getChild( L"DiffuseColor" ), &mat->DiffuseColor );
        ReadColor( l->getChild( L"EmissiveColor" ), &mat->EmissiveColor );
        ReadColor( l->getChild( L"SpecularColor" ), &mat->SpecularColor );
        if ( l->getChild( L"Textures" )->childCount() > 0 ) {
            core::stringc texn = l->getChild( L"Textures" )->getChild( L"Texture1" )->getData( 0 )->Value();
///            mat->Texture1 = LoadTexture( texn );
            ParserLeaf* tex2 = l->getChild( L"Textures" )->getChild( L"Texture2" );
            if ( tex2 ) {
                texn = l->getChild( L"Textures" )->getChild( L"Texture2" )->getData( 0 )->Value();
///                mat->Texture2 = LoadTexture( texn );
            }
        }
        l = l->parent;
    }
}

void VisualEditorReader::ReadTriangleSelectors( ParserLeaf* TriangleSelectorsLeaf, scene::ISceneNode* readnode ) {}
;

void VisualEditorReader::ReadAnimators( ParserLeaf* AnimatorsLeaf, scene::ISceneNode* readnod ) {}
;

VisualEditorReader::VisualEditorReader( IrrlichtDevice* device, float WorldScale ) : XMLSceneReader( device ) {
    Device = device;
    wscale = WorldScale;
//    Device->getSceneManager()->getRootSceneNode()->setScale( core::vector3df( wscale, wscale, wscale ) );
}

void VisualEditorReader::addSearchPath( core::stringc newPath ) {
    searchpaths.push_back( newPath );
}

void VisualEditorReader::loadLevel( core::stringc levelname ) {
    parseFile( levelname );
    scene::ISceneManager* smgr = Device->getSceneManager();
    scene::ISceneNode* readnode;
    if ( showDebug )
        printf( "Start Reading File\n" );
    ParserLeaf* l = &RootLeaf;
    l = l->getChild( 0 );

    printf( "Starting %s | Count: %i\n", l->name.c_str(), l->childCount() );

    int ctypescount = l->childCount();

    for ( int ctypes = 1; ctypes < ctypescount; ctypes++ ) {
        l = l->getChild( ctypes );
        if ( l ) {
            if ( showDebug )
                printf( "=========================================================\n" );
            if ( showDebug )
                printf( "Starting %s | Count: %i\n", l->name.c_str(), l->childCount() );
            if ( showDebug )
                printf( "=========================================================\n" );
            for ( int ct = 0; ct < l->childCount(); ct++ ) {
                l = l->getChild( ct ); // Node
                readnode = ReadMainParameters( l->getChild( L"MainParameters" ) );
                ReadCoordinates( l->getChild( L"Coordinates" ), readnode );
                ReadMaterialParameters( l->getChild( L"MaterialParameters" ), readnode );
                ReadTriangleSelectors( l->getChild( L"TriangleSelectors" ), readnode );
                ReadAnimators( l->getChild( L"Animators" ), readnode );
                l = l->parent; // \Node
            }
        }
        l = l->parent;
    }
    if ( showDebug )
        printf( "End loading file...\n" );
};

