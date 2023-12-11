//!
//! SCENEREADER
//! Based on IrrXML, can parse a file a organize it on a tree-like fashion
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.

#ifndef _SCENEREADER_H_INCLUDED_
#define _SCENEREADER_H_INCLUDED_

#include "irrlicht.h"
#include "iostream"

using namespace irr;


class ParserData {
public:
    core::stringc name;
    core::stringc data;

    core::stringw toWideString( const core::stringc str );
    core::stringc toString( const core::stringw str );
    core::stringw floatToWChar( float value );
    core::stringc floatToc8( float value );
    int wcharToInt( core::stringw text );
    float wcharToFloat( core::stringw text );
    int IntValue();
    float FloatValue();
    float BoolValue();
    void setIntValue( int newValue ) {
        data = core::stringc( newValue );
    }
    void setFloatValue( float newValue ) {
        data = core::stringc( floatToc8( newValue ) );
    }
    void setBoolValue( bool newValue ) {
        if ( newValue ) {
            data = "true";
        } else {
            data = "false";
        }
    }
    core::stringc Value();
    core::stringw ValueW();
    core::stringw NameW();
};

class ParserLeaf {
public:
    ParserLeaf* parent;
    core::array< ParserLeaf* > childs;
    core::array< ParserData* > datas;
    core::stringc name;
    bool showDebug;

    ParserLeaf( ParserLeaf* Parent = 0 );
    ~ParserLeaf();
    void setShowDebug( bool value );
    ParserLeaf* addChild();
    ParserLeaf* addChild( core::stringc childname );
    ParserData* addData();
    ParserData* addData( core::stringc dataname );
    int childCount();
    int dataCount();
    core::stringw NameW();
    ParserLeaf* getChild( core::stringc LookName );
    ParserLeaf* getChild( int index );
    ParserData* getData( core::stringc LookName );
    ParserData* getData( int index );
};

class XMLParser {
private:

    int level;
    core::stringw strRep( core::stringw str, int numreps );

public:

    ParserLeaf RootLeaf;
    IrrlichtDevice* Device;
    bool showDebug;

    XMLParser( IrrlichtDevice* device );
    ~XMLParser();
    void setShowDebug( bool value );
    void parseFile( core::stringc FileName );

    void writeLeafChilds( io::IXMLWriter* xml, ParserLeaf* leaf ) {

        for ( int i = 0; i < leaf->childs.size(); i++ ) {
            ParserLeaf* l = leaf->childs[i];

            if ( ( l->childs.size() == 0 ) && ( l->datas.size() == 0 ) ) {
                if ( l->name.size() > 0 ) {
                    xml->writeElement( l->NameW().c_str(), true );
                    xml->writeLineBreak();
                };
                return;
            }

            if ( l->datas.size() == 0 )
                xml->writeElement(  l->NameW().c_str(), false );

            switch ( l->datas.size() ) {
                case 1:
                xml->writeElement(  l->NameW().c_str(), false,
                                    l->getData( 0 )->NameW().c_str(),
                                    l->getData( 0 )->ValueW().c_str() );
                break;
                case 2:
                xml->writeElement( l->NameW().c_str(), false,
                                   l->getData( 0 )->NameW().c_str(),
                                   l->getData( 0 )->ValueW().c_str(),
                                   l->getData( 1 )->NameW().c_str(),
                                   l->getData( 1 )->ValueW().c_str() );
                break;
                case 3:
                xml->writeElement(  l->NameW().c_str(), false,
                                    l->getData( 0 )->NameW().c_str(), l->getData( 0 )->ValueW().c_str(),
                                    l->getData( 1 )->NameW().c_str(), l->getData( 1 )->ValueW().c_str(),
                                    l->getData( 2 )->NameW().c_str(), l->getData( 2 )->ValueW().c_str() );
                break;
                case 4:
                xml->writeElement(  l->NameW().c_str(), false,
                                    l->getData( 0 )->NameW().c_str(),
                                    l->getData( 0 )->ValueW().c_str(),
                                    l->getData( 1 )->NameW().c_str(),
                                    l->getData( 1 )->ValueW().c_str(),
                                    l->getData( 2 )->NameW().c_str(),
                                    l->getData( 2 )->ValueW().c_str(),
                                    l->getData( 3 )->NameW().c_str(),
                                    l->getData( 3 )->ValueW().c_str() );
                break;
                case 5:
                xml->writeElement(  l->NameW().c_str(), false,
                                    l->getData( 0 )->NameW().c_str(),
                                    l->getData( 0 )->ValueW().c_str(),
                                    l->getData( 1 )->NameW().c_str(),
                                    l->getData( 1 )->ValueW().c_str(),
                                    l->getData( 2 )->NameW().c_str(),
                                    l->getData( 2 )->ValueW().c_str(),
                                    l->getData( 3 )->NameW().c_str(),
                                    l->getData( 3 )->ValueW().c_str(),
                                    l->getData( 4 )->NameW().c_str(),
                                    l->getData( 4 )->ValueW().c_str() );
            }

            writeLeafChilds( xml, l );

            if ( l->name.size() > 0 ) {
                xml->writeClosingTag( l->NameW().c_str() );
                xml->writeLineBreak();
            }

        }
    }

    void writeXML( core::stringc filename ) {
        if ( !Device ) printf ( "No device in writeXML\n" );
        io::IXMLWriter* xml = Device->getFileSystem()->createXMLWriter( filename.c_str() );

        xml->writeXMLHeader();
        xml->writeLineBreak();

        writeLeafChilds( xml, &RootLeaf );

        delete xml;
    }

};

struct SceneNodeHold
{
    scene::IAnimatedMesh* mesh;
    core::stringc meshfilename;
    scene::ISceneNode* node;
};

class XMLSceneReader : public XMLParser
{
public:

    core::array< SceneNodeHold* > nodes;

    SceneNodeHold *currentnode;

    XMLSceneReader( IrrlichtDevice* device ) : XMLParser( device )
    {}

    ~XMLSceneReader()
    {
        for ( int x = 0; x < nodes.size(); x++ )
            delete nodes[x];
        nodes.clear();
    }

    void addNewNode()
    {
        currentnode = new SceneNodeHold();
        currentnode->mesh = 0;
        currentnode->meshfilename = "";
        currentnode->node = 0;
        nodes.push_back( currentnode );
    }

    SceneNodeHold* getNodeHolder( core::stringc afilename )
    {
        afilename.make_upper();
        printf( "start getting holder\n" );
        printf( "Get node holder for %s\n", afilename.c_str() );
        for ( int x = 0; x < nodes.size(); x++ )
        {
            core::stringc fn = nodes[x]->meshfilename;
            fn.make_upper();
            printf( "current name >%s<   filename >%s<\n", fn.c_str(), afilename.c_str() );
//            if ( fn == afilename )
            {
                printf( "verdadeiro\n" );
                return nodes[x];
            }
        }
        printf( "Returning 0\n" );
        return 0;
    }

};

// class to read VisualEditor XML files
class VisualEditorReader: public XMLSceneReader {
private:

    IrrlichtDevice* Device;
    float wscale;
    core::array< core::stringc > searchpaths;

    scene::IAnimatedMesh* LoadMesh( core::stringc filename );
    video::ITexture* LoadTexture( core::stringc filename );
    scene::ISceneNode* ReadMainParameters( ParserLeaf* MainParametersLeaf );
    void ReadCoordinates( ParserLeaf* CoordinatesLeaf, scene::ISceneNode* readnode );
    void ReadColor( ParserLeaf* ColorLeaf, video::SColor* color );
    void ReadMaterialParameters( ParserLeaf* MatParams, scene::ISceneNode* readnode );
    void ReadTriangleSelectors( ParserLeaf* TriangleSelectorsLeaf, scene::ISceneNode* readnode );
    void ReadAnimators( ParserLeaf* AnimatorsLeaf, scene::ISceneNode* readnod );

public:

    VisualEditorReader( IrrlichtDevice* device, float WorldScale = 1.0f );
    void addSearchPath( core::stringc newPath );
    void loadLevel( core::stringc levelname );

};

#endif
