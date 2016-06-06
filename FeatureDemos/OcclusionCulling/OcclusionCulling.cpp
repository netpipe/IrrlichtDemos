/* Culling with Portals by Burt McCauley */

#include "OcclusionCulling.h"

/*----------------------------------------------------------------------------*/
OcclusionCulling::OcclusionCulling(ISceneManager* smgra, char* bxpfilename)
{
 smgr = smgra;

 // hardcoded values. faster
 listMDL.push_back("data/models/occlusion/room1/room1.b3d");
 listMDL.push_back("data/models/occlusion/room2/canape.b3d");
 listMDL.push_back("data/models/occlusion/room3/table1.b3d");

 Sleaf L;
      L.point1 = vector3df(0,0,0);
      L.point2 = vector3df(1024,256,256);
      L.mdl.push_back(createSmodel(0, vector3df(600,0,128), 0,0));
      //L.mdl.push_back(createSmodel(2, vector3df(512,0,200), 270,0));
      world.push_back(L);

 Sleaf K;
      K.point1 = vector3df(0,0,256);
      K.point2 = vector3df(1024,256,512);
      K.mdl.push_back(createSmodel(1, vector3df(512,0,384), 0,0));
      world.push_back(K);

 Sleaf M;
      M.point1 = vector3df(0,0,512);
      M.point2 = vector3df(1024,256,768);
      M.mdl.push_back(createSmodel(2, vector3df(512,0,640), 270,0));
      //M.mdl.push_back(createSmodel(1, vector3df(512,0,600), 270,0));
      //M.mdl.push_back(createSmodel(1, vector3df(512,0,680), 270,0));
      world.push_back(M);

 Sleaf N;
      N.point1 = vector3df(0,0,768);
      N.point2 = vector3df(1024,256,1024);
      //N.mdl.push_back(createSmodel(0, vector3df(512,0,900), 270,0));
      world.push_back(N);

 writeBXPfile(bxpfilename);

 // read bxp file (bugged)
 //loadBXPFile(bxpfilename);

 print_stats();

 currentLeaf = 0;
 loadLeaf(currentLeaf);
 //loadLeaf(1); loadLeaf(2); loadLeaf(3);
}

/*----------------------------------------------------------------------------*/
void OcclusionCulling::loadBXPFile(char* filename)
{
 printf("Loading %s", filename);

 FILE *fp = fopen(filename, "rb");
 SBXPheader H;
 fread(&H,sizeof(struct SBXPheader),1,fp);
 printf("[%d] leaves\n[%d] models in models list (%d bytes)\n", H.nbleaves, H.nbmdl, H.MDLlistSize);

 // models list
 char* s = (char*)malloc(H.MDLlistSize)+1;
 if (s == NULL) exit(44);
 fread(s, H.MDLlistSize, 1, fp); // lit 29 octets
 s[H.MDLlistSize] = 0;
 //printf("MODELS\n[%s]\n", s);

 // split string to add models to models list
 char x[64]; // <- filename limited to 64 bytes: models/machin.b3d is 18.
 u8 b = 0;
 for (u16 n=0; n < H.MDLlistSize; n++)
 {
  if (s[n]==';')
  {
   x[b] = 0;
   //printf("----push_back MDL [%s]\n", x);
   this->listMDL.push_back(strdup(x));
   b = 0;
  }
  else x[b++] = s[n];
 }

 //load leaves
 for (u16 n=0; n < H.nbleaves; n++)
 {
   SBXPleaf L;
   fread(&L,sizeof(struct SBXPleaf),1,fp);
   //printf("LEAF %d (%.2f,%.2f,%.2f) (%.2f,%.2f,%.2f) %d models\n", n,
           //L.point1.X, L.point1.Y, L.point1.Z,
           //L.point2.X, L.point2.Y, L.point2.Z, L.nbmdls);
   Sleaf LEAF;
          LEAF.point1 = vector3df(L.point1);
          LEAF.point2 = vector3df(L.point2);

   // models of this leaf
   for (u16 n=0; n < L.nbmdls; n++)
   {
    SBXPmdl M;
    fread(&M,sizeof(struct SBXPmdl),1,fp);
    // add this model to leaf information
    Smodel* MDL = createSmodel(M.id, vector3df(M.pos.X, M.pos.Y, M.pos.Z), M.angle, M.upvector);
    LEAF.mdl.push_back(MDL);
    //printf("\tidml:%d (%.2f,%.2f,%.2f) %d]%d]\n",
                //MDL->id, MDL->pos.X, MDL->pos.Y, MDL->pos.Z, MDL->angle, MDL->upvector);
   }

   world.push_back(LEAF); //insert leaf to world
 }

 fclose(fp);
}

/*----------------------------------------------------------------------------*/
Smodel* OcclusionCulling::loadModel(u16 id, vector3df pos, u16 angle, u16 upvector)
{
 Smodel* x = createSmodel(id, pos, angle, upvector);

 IAnimatedMesh* mesh = smgr->getMesh(listMDL[id]);
                  //mesh->setHardwareMappingHint(EHM_STATIC);
 //printf("loading model [%s]\n", listMDL[id]); getchar();
 x->node = smgr->addAnimatedMeshSceneNode(mesh);
 if (x->node)
 {
   x->node->setPosition(pos);
   x->node->setRotation(vector3df(0,angle,upvector));
 }
 mesh->drop();
 return x;
}

/*----------------------------------------------------------------------------*/
Smodel* OcclusionCulling::createSmodel(u16 id, vector3df pos, u16 angle, u16 upvector)
{
 Smodel* x = new Smodel;
         x->pos = vector3df(pos);
         x->angle = angle;
         x->upvector = upvector;
         x->id = id;
 return x;
}

/* Display Leaf/Models information -------------------------------------------*/
void OcclusionCulling::print_stats()
{
 printf("\n*** MODELS LIST ***\n");
 for (u8 n=0; n<listMDL.size(); n++)
  printf("\t[%s]\n", listMDL[n]);
 printf("\n*** LEAF LIST ***\n");
 for (u16 n=0; n < world.size(); n++)
 {
  printf("> LEAF %d [%d mdls]  (%.2f,%.2f,%.2f) (%.2f,%.2f,%.2f)\n",
           n, world[n].mdl.size(),
           world[n].point1.X, world[n].point1.Y, world[n].point1.Z,
           world[n].point2.X, world[n].point2.Y, world[n].point2.Z);
  for (u16 i=0; i < world[n].mdl.size(); i++)
   printf("\tmdl [%d] [idmdl:%d] (%.2f,%.2f,%.2f) %d %d\n", i, world[n].mdl[i]->id,
             world[n].mdl[i]->pos.X, world[n].mdl[i]->pos.Y, world[n].mdl[i]->pos.Z,
             world[n].mdl[i]->angle, world[n].mdl[i]->upvector);
 }
 printf("*****************\n\n");
}



/*----------------------------------------------------------------------------*/
void OcclusionCulling::update(vector3df pos)
{
  // Am I still in my leaf
  if (  pos.X >= world[currentLeaf].point1.X && pos.X < world[currentLeaf].point2.X &&
		pos.Y >= world[currentLeaf].point1.Y && pos.Y < world[currentLeaf].point2.Y &&
		pos.Z >= world[currentLeaf].point1.Z && pos.Z < world[currentLeaf].point2.Z)
    ;//printf("in leaf %d\n", currentLeaf);
  else
  {
    //printf("out of %d (%d leafs) ...\n", currentLeaf, world.size());
    // look in the leaves list to see in what leaf I am now.
    // Slow down danger, if there are tons of leaves, thus particular algorithm
    for (u16 n=0; n < world.size(); n++)
    {
      //printf("X %.2f >= %.2f ? %.2f < %.2f ?\n", pos.X, world[n].point1.X, pos.X, world[n].point2.X);
      if (pos.X >= world[n].point1.X && pos.X < world[n].point2.X &&
		  pos.Y >= world[n].point1.Y && pos.Y < world[n].point2.Y &&
		  pos.Z >= world[n].point1.Z && pos.Z < world[n].point2.Z)
      {
       //printf("from/to %d/%d >> load/unload %d/%d\n", currentLeaf,n, n, currentLeaf);
       // load new leaf, unload previous leaf
       unloadLeaf(currentLeaf);
       loadLeaf(n);
       currentLeaf = n;
       break;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
void OcclusionCulling::loadLeaf(s16 n)
{
  //printf(" >> loading leaf %d %d mdls\n", n, world[n].mdl.size());
  // load and models of this leaf
  for (u16 i=0; i < world[n].mdl.size(); i++)
  {
     u16 id = world[n].mdl[i]->id;
     //printf(" >> load mdl %d [%s]\n", id, listMDL[id]);
     IAnimatedMesh* mesh = smgr->getMesh(listMDL[id]);
     world[n].mdl[i]->node = smgr->addAnimatedMeshSceneNode(mesh);
     if (world[n].mdl[i]->node)
     {
       world[n].mdl[i]->node->setPosition(world[n].mdl[i]->pos);
       world[n].mdl[i]->node->setRotation(vector3df(0,world[n].mdl[i]->angle,world[n].mdl[i]->upvector));
     }
     mesh->drop();
  }

  // create light

  // apply shaders

  // set collision

}

/*----------------------------------------------------------------------------*/
void OcclusionCulling::unloadLeaf(s16 n)
{
  //unload models from leaf n
  for (u16 i=0; i < world[n].mdl.size(); i++)
  {
     //supprime le node sinon ils vont s'accumuler
     if (world[n].mdl[i]->node)
     {
      //world[currentLeaf].mdl[n]->node->setVisible(false);
      world[n].mdl[i]->node->grab();
      world[n].mdl[i]->node->remove();
      //world[currentLeaf].mdl[n]->node->drop();
      //world[currentLeaf].mdl[n]->node = 0;
     }

     //disable light

     //disable shaders

  }
}

/*----------------------------------------------------------------------------*/
void OcclusionCulling::writeBXPfile(char *filename)
{
 SBXPheader H;
        H.nbleaves = world.size();
        H.nbmdl = listMDL.size();

  u32 size = 0;
  for (u16 n=0; n < listMDL.size(); n++)
    size += strlen(listMDL[n]) + 1;
        H.MDLlistSize = size;

 FILE *fp = fopen(filename, "wb");

 // header
 fwrite(&H,1,sizeof(struct SBXPheader),fp);

 // models list
 for (u8 n=0; n<listMDL.size(); n++)
 {
  fwrite(listMDL[n],strlen(listMDL[n]),1,fp);
  fwrite(";",1,1,fp);
  //printf("\t[%s] %d bytes\n", listMDL[n], strlen(listMDL[n]));
 }

 // les leafs
 for (u16 n=0; n < world.size(); n++)
 {
  SBXPleaf L;
     L.point1.X = world[n].point1.X;
     L.point1.Y = world[n].point1.Y;
     L.point1.Z = world[n].point1.Z;

     L.point2.X = world[n].point2.X;
     L.point2.Y = world[n].point2.Y;
     L.point2.Z = world[n].point2.Z;

     L.nbmdls = world[n].mdl.size();

     fwrite(&L,sizeof(struct SBXPleaf),1,fp);

  /*printf("%.2f,%.2f,%.2f)%.2f,%.2f,%.2f)%d\n",
           world[n].point1.X, world[n].point1.Y, world[n].point1.Z,
           world[n].point2.X, world[n].point2.Y, world[n].point2.Z, world[n].mdl.size());*/

  for (u16 i=0; i < world[n].mdl.size(); i++)
  {
   /*printf("\t%d]%.2f,%.2f,%.2f)%d)%d\n", world[n].mdl[i]->id,
             world[n].mdl[i]->pos.X, world[n].mdl[i]->pos.Y, world[n].mdl[i]->pos.Z,
             world[n].mdl[i]->angle, world[n].mdl[i]->upvector);*/
   SBXPmdl M;
           M.id       = world[n].mdl[i]->id;
           M.pos      = vector3df(world[n].mdl[i]->pos);
           M.angle    = world[n].mdl[i]->angle;
           M.upvector = world[n].mdl[i]->upvector;
   fwrite(&M,sizeof(struct SBXPmdl),1,fp);
  }
 }

 fclose(fp);
}

/*----------------------------------------------------------------------------*/
OcclusionCulling::~OcclusionCulling()
{
 // for each leaf, remove its nodes
 for (u16 n=0; n < world.size(); n++)
 {
  for (u16 i=0; i < world[n].mdl.size(); i++)
  {
   if (world[n].mdl[i]->node)
   {
    printf("REMOVING node from leaf %d\n", n);
    world[n].mdl[i]->node->grab();
    world[n].mdl[i]->node->remove();
    world[n].mdl[i]->node->drop();
   }
  }
 }
}
