    bool IsFaceAboveOrigin(vector3df v1, vector3df v2, vector3df v3)
    {
        vector3df a = v2 - v1;
        vector3df b = v3 - v1;
        double det0 = double(a.X)*b.Y - double(a.Y)*b.X;
        double det1 = double(v1.Y)*b.X - double(v1.X)*b.Y;
        if (det1 == 0.0) return false;
        double x = det1/det0;
        if (x <= 0.0) return false;
        double det2 = double(a.Y)*v1.X - double(a.x)*v1.Y;
        if (det2 == 0.0) return false;
        double y = det2/det0;
        if (y <= 0.0) return false;
        if (x+y >= 1.0) return false;
        double z = v1.Z + x*a.Z + y*b.Z;
        return ( z > 0.0);
    }
         
    bool IsPointInsideMeshBuffer(SMeshBuffer* meshbuffer, vector3df& p)
    {
        int numAbove = 0;
        int numindices = meshbuffer->getIndexCount();
        u16* indices = meshbuffer->getIndices();
        S3DVertex* verts = (S3DVertex*)meshbuffer->getVertices();
     
        for(int i=0; i < numindices; i+=3)
        {
            if (IsFaceAboveOrigin(  verts[indices[i]].pos - p,
                                    verts[indices[i+1]].pos  - p,
                                    verts[indices[i+2]].pos  - p) )
                numAbove++;
        }
     
        return ((numAbove % 2) == 1);
    }




IMeshBuffer* meshBuffer=o_mesh->getMeshBuffer(0);
printf("%i \n",o_mesh->getMeshBufferCount());
printf("%f  %f  %f \n", meshbox.MinEdge.X,meshbox.MinEdge.Y,meshbox.MinEdge.Z);
printf("%f  %f  %f \n", meshbox.MaxEdge.X,meshbox.MaxEdge.Y,meshbox.MaxEdge.Z);
 
f32 delta_x=(meshbox.MaxEdge.X-meshbox.MinEdge.X)/size_x;
f32 delta_y=(meshbox.MaxEdge.Y-meshbox.MinEdge.Y)/size_y;
f32 delta_z=(meshbox.MaxEdge.Z-meshbox.MinEdge.Z)/size_z;
 
for(int xx = 0; xx <= size_x; xx = xx + 1)
      for(int yy = 0; yy <= size_y; yy = yy + 1)
          for(int zz = 0; zz <=size_z; zz = zz + 1){
              int index=xx+yy*(size_x+1)+zz*(size_x+1)*(size_y+1);
 
             
 
            core::vector3df testpoint=core::vector3df(meshbox.MinEdge.X+xx*delta_x,meshbox.MinEdge.Y+yy*delta_y,meshbox.MinEdge.Z+zz*delta_z);
 
           
            if(IsPointInsideMeshBuffer(meshBuffer, testpoint)) {
              ISceneNode* kr=smgr->addCubeSceneNode(0.02f);
              kr->setPosition(testpoint);}
              }
