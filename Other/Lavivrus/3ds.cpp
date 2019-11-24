//Basic Classes:
//Required for representing the loaded model
class vertex{
public:
   float x,y,z;
};
//MapCoord - for storing texture mapping coords
class mapcoord{
public:
   float u,v;
};ftell tutorial

//The three ints for the polygon
//represent the no.s(or rank) of it's 3 vertices
class polygon{
public:
   int a,b,c;
};

class object{
public:
   char name[20];
   int numVerts,numPolys;
   vertex v[3000];
   polygon p[3000];
   mapcoord m[3000];
};

//-----------------------------------------------------------------------------
// Name: Load3dsObject (object *, char *)
// Desc: Loads a 3ds object given a filename
//----------------------------ftell tutorial-------------------------------------------------

void Load3dsObject (object *obj, char *filename)
{
   FILE *file;         //Our file pointer

   char temp;         //Temporary char for reading name of object
   short chunkID;      //Stores ID of current chunk.
   int chunkLength;

   short useless;
   
   //Open our file for reading(r) and in binary mode(b)- "rb"
   file=fopen (filename, "rb");

   int i;

   //While current position is lesser than the total length
   while (ftell(file) < filelength (fileno (file))) 
   {
      fread (&chunkID, 2, 1, file);
      fread (&chunkLength, 4, 1,file);

      switch (chunkID)
        {
         case 0x4d4d:      //Skip these chunks
            break;   
         case 0x3d3d:
            break;
         
         case 0x4000:      //Chunk containing name
            for(i=0;i<20;i++)
            {
               fread (&temp, 1, 1, file);
                    obj->name[i]=temp;
               if(temp == '\0')break;
                }
            break;

         case 0x3f80:      //Skip again
            break;
         case 0x4100:
            break;
   
         case 0x4110:      //Chunk with num of vertices
                        //followed by their coordinates
            fread (&obj->numVerts, sizeof (unsigned short), 1, file);
            for (i=0; i<obj->numVerts; i++)
                {
               fread (&obj->v[i].x, sizeof(float), 1, file);
             
                    fread (&obj->v[i].y, sizeof(float), 1, file);
                
               fread (&obj->v[i].z, sizeof(float), 1, file);
             
            }
            break;

         case 0x4120:       //Chunk with numPolys and
                         //the indices
            fread (&obj->numPolys, sizeof (unsigned short), 1, file);
                for (i=0; i<obj->numPolys; i++)
                {
               fread (&obj->p[i].a, sizeof (unsigned short), 1, file);
            
               fread (&obj->p[i].b, sizeof (unsigned short), 1, file);
            
               fread (&obj->p[i].c, sizeof (unsigned short), 1, file);
               
               fread (&useless, sizeof (unsigned short), 1, file);
               
            }
            break;

      
         case 0x4140:      //Chunk with texture coords
            fread (&useless, sizeof (unsigned short), 1, file);
            for (i=0; i<obj->numVerts; i++)
            {
               fread (&obj->m[i].u, sizeof (float), 1, file);
            
                    fread (&obj->m[i].v, sizeof (float), 1, file);
            }
                break;

         default:
             fseek(file,chunkLength-6, SEEK_CUR);
        }
   }
   fclose (file);      
}

//-----------------------------------------------------------------------------
// Name: DrawObject(object *obj)
// Desc: Draws our loaded 3ds object
//-----------------------------------------------------------------------------

void DrawObject(object *obj)
{
   glBegin(GL_TRIANGLES);
    for (int i=0;i<obj->numPolys;i++)
    {
       glTexCoord2f( obj->m[ obj->p[i].a ].u,
                      obj->m[ obj->p[i].a ].v);
        glVertex3f( obj->v[ obj->p[i].a ].x,
                    obj->v[ obj->p[i].a ].y,
                    obj->v[ obj->p[i].a ].z);

      glTexCoord2f( obj->m[ obj->p[i].b ].u,
                      obj->m[ obj->p[i].b ].v);
        glVertex3f( obj->v[ obj->p[i].b ].x,
                    obj->v[ obj->p[i].b ].y,
                    obj->v[ obj->p[i].b ].z);

      glTexCoord2f( obj->m[ obj->p[i].c ].u,
                      obj->m[ obj->p[i].c ].v);
        glVertex3f( obj->v[ obj->p[i].c ].x,
                    obj->v[ obj->p[i].c ].y,
                    obj->v[ obj->p[i].c ].z);
   }
    glEnd();
} 
