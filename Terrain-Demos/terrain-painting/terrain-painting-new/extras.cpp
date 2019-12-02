    void gameGFX_irr::circleOnTerrain(float x, float z, float Radius, float heightOverTerrain,int Segments, SColor color,scene::ITerrainSceneNode* terr)
    {
            float EachAngle;
            float a;
            float x1;
            float x2;
            float z1;
            float z2;
     
            EachAngle = 2.0 * PI_CONSTANT / (float)Segments;
     
            x2 = Radius;
            z2 = 0.0;
     
            for(a=0.0; a<= (2.0*PI_CONSTANT + EachAngle); a+=EachAngle) {
                    x1 = x2;
                    z1 = z2;
                    x2 = Radius * cos(a);
                    z2 = Radius * sin(a);
                    //line(x1+x, y1+y, x2+x, y2+y, color);
                    f32 h=terr->getHeight((x1+x)*MAP_SCALE,(z1+z)*MAP_SCALE);
                    f32 h2=terr->getHeight((x2+x)*MAP_SCALE,(z2+z)*MAP_SCALE);
                    line3d(vector3df((x1+x)*MAP_SCALE,h+heightOverTerrain,(z1+z)*MAP_SCALE),vector3df((x2+x)*MAP_SCALE,h2+heightOverTerrain,(z2+z)*MAP_SCALE),color);
            }
    } 
