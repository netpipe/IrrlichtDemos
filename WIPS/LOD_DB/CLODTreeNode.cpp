
#include "CLODTreeNode.h"
//#include <stdlib.h>
    LODTreeSceneNode::LODTreeSceneNode (
                      ISceneNode* parent, ISceneManager* mgr,
                      s32 id, vector3df pos, LODTreeSceneNode* rootnode,
                      int iDepth, int iLevel, f32 drawdistance) :
                          ISceneNode(mgr->getRootSceneNode(),mgr,id),
                          root(rootnode ? rootnode : this ),
                          Depth(iDepth),
                          Level(iLevel),
                          DrawDistance(drawdistance),
                          bbox(-drawdistance/2,pos.Y -3, -drawdistance/2,
                              +drawdistance/2,pos.Y +3, +drawdistance/2)

    {

    	#ifdef _DEBUG
        setDebugName("CLODTreeSceneNode");
        #endif

        this->setAutomaticCulling(EAC_OFF);

        // attempt to connect to the database
        if (iLevel == 0)
        {
           connect();
           root = this;
        }

        // flush children
        children[0] = children[1] = children[2] = children[3] = 0;
        setPosition(pos);
        updateAbsolutePosition();

        material.Lighting = false;
        material.DiffuseColor = video::SColor( 255, 255, 255, 0 );

        // create all nodes at this LOD...
        if (isConnected())
        {
           char *zErrMsg = 0;
           sqlite3_exec(root->db, "SELECT * FROM ENTITIES;", callback, 0, &zErrMsg);
        }
        // testnodes

        char *tname;
        ISceneNode *tmp;
        switch(iLevel)
        {
           case (6):
                tname = "plant.png"; break;
           case (5):
                tname = "house.png"; break;
           case (4):
                tname = "smalltree.png"; break;
           case (3):
                tname = "conifer.png"; break;
           case (2):
                tname = "forrest.png"; break;
           case (1):
                tname = "forrest.png"; break;
           case (0):
                tname = "mountain.png";
                // add floor
                tmp = mgr->addTestSceneNode( 1.0f, this, -1,
                           vector3df(0, 0, 0), vector3df(0, 0, 0),
                           vector3df(drawdistance, 0, drawdistance));
                tmp->setMaterialFlag(video::EMF_LIGHTING, false);
                tmp->setMaterialTexture(0,SceneManager->getVideoDriver()->getTexture("grass.png"));
                break;
           default:
                tname = "grass.png";
        }
        srand(Level);
        for (int n=0;n<4+(iLevel/2);n++)
        {
            IBillboardSceneNode* b = mgr->addBillboardSceneNode(this,core::dimension2d<f32>(drawdistance/10, drawdistance/10));
            b->setMaterialFlag(video::EMF_LIGHTING, false);
            b->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

            b->setPosition( vector3df(
                              (-drawdistance/2) + ((rand()%100) / 100.0) * f32(drawdistance),
                              drawdistance/20,
                              (-drawdistance/2) + ((rand()%100) / 100.0) * f32(drawdistance)
                                     )
                           );

            b->setMaterialTexture(0,SceneManager->getVideoDriver()->getTexture(tname));
        }

        setDebugDataVisible(root->isDebugDataVisible());
    }


    void LODTreeSceneNode::setDebugDataVisible( bool visible )
    {
        DebugDataVisible = visible;
        if ( Level > ( Depth - 2 ) ) return;
        for (int n = 0; n<4; n++)
        {
            if (children[n]) children[n]->setDebugDataVisible( visible );
        }
    }

// "INSERT INTO ENTITIES VALUES (", e.pid  ,",", e.uid  ,",", e.uid ,",",
//                                    e.flags   ,",", e.seed ,",",
//                                    e.p.X  ,",", e.p.Y  ,",", e.p.Z ,",",
//                                    e.r.X  ,",", e.r.Y  ,",", e.r.Z ,",",
//                                    e.s.X  ,",", e.s.Y  ,",", e.s.Z ,");"

// "SELECT * FROM ENTITIES WHERE (entities.pid = '" + pid + ');"

/*
static int addObjectInstance(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
*/

    void LODTreeSceneNode::_doChildren()
    {
         ICameraSceneNode *cam = SceneManager->getActiveCamera();
         if (cam && (Level < Depth -1 ))
         {
             vector3df campos = cam->getPosition();
             f32 xshift     = 0;
             f32 zshift     = 0;
             f32 size       = DrawDistance/2.0f;
             f32 size2      = size/2.0f;
             vector3df p    = getAbsolutePosition();
             vector3df cpos;

             for (int n=0; n<4; n++)
             {
                 xshift = f32( (n & 1) == 1) * size; // x
                 zshift = f32( (n & 2) == 2) * size; // z
                 // 0,0 - 1/4 + shift
                 cpos   = p + vector3df(-xshift+(size2),0,-zshift+(size2));
                 f32 d  = cpos.getDistanceFrom(campos);

                 if (children[n])
                 {
                     // may need to destroy it
                     // we dont like to destroy things early
                     if (d > size *2.2 )
                     {
                           children[n]->destroy();
                           children[n] = 0;
                     }
                     else
                     {
                           if ( root->isDebugDataVisible() ) {
                               SceneManager->getVideoDriver()->setMaterial( material );
                            	SceneManager->getVideoDriver()->draw3DBox( children[n]->getTransformedBoundingBox());
                           }
                           children[n]->_doChildren();
                     }
                 }
                 else
                 {
                     // may need to create it
                     if (d < size* 2.0)
                     {
                           children[n] = new
                                  LODTreeSceneNode( this, SceneManager, 0,
                                                    cpos, root, Depth,
                                                    Level + 1, size);
                           //children[n]->setPosition(cpos);
                           //printf("level = %d\n", Level);
                     }
                 }
             }
         }
    }


    // decides if children are visible
    void LODTreeSceneNode::OnPreRender()
    {
         if (this == root)
             _doChildren();
         // call prerender event
         ISceneNode::OnPreRender();
    }
    // render this scene node
    void LODTreeSceneNode::render()
    {
        if ( isDebugDataVisible() ) {
            SceneManager->getVideoDriver()->setMaterial( material );
    		SceneManager->getVideoDriver()->draw3DBox( bbox );
        }
    }
    //void LODTreeSceneNode::OnPostRender()
    //{
     //    // meh
    //}

    // adds an object to the database and tree
    bool LODTreeSceneNode::addNewObject(LODMARKER *l)
    {

    }
    //  writeObject - sets an objects position, rotation, scale, etc.
    //       all = false : does this update all nodes with this uid?
    //    offset = true  : does the update use relative distance?
    bool LODTreeSceneNode::writeObject (LODMARKER *l, bool all, bool offset)
    {
        // search the database for current object
        // loop through them
        //
    }


    // makes sure the database is connected
    bool LODTreeSceneNode::connect()
    {
       if (root->DBConnected == true)
       {
          printf("ERROR: database is already open\n");
      //    return true;
       }
       int rc = sqlite3_open("data.db", &(root->db));
       if( rc )
       {
          printf("ERROR: Can't open database: %s\n", sqlite3_errmsg(db));
          sqlite3_close(db);
          root->DBConnected = false;
       }
       else
       {
          root->DBConnected = true;
       }
       return root->DBConnected;
    }

    // refresh this node and all children
    bool LODTreeSceneNode::refresh()
    {

    }

    // returns true if db is connected
    bool LODTreeSceneNode::isConnected() const
    {
         return root->DBConnected;
    }
    // checks if childred are still in range
    bool LODTreeSceneNode::checkChildren()
    {

    }

    // destroys this node and all children
    bool LODTreeSceneNode::destroy()
    {
         // when the top level goes, so does the database
         if (this == root)
         {
            sqlite3_close(db);
         }
         // drop children

         // pop stack


         SceneManager->addToDeletionQueue(this);
    }

    const core::aabbox3d<f32>& LODTreeSceneNode::getBoundingBox() const
    {
          return bbox;
    }

