void draw3DCircle( video::IVideoDriver* driver, core::vector3df center, f32 radius, u32 vertexcount, core::vector3df normal, video::SColor color )
{
        core::array<video::S3DVertex> lstVertices;
        core::array<u16> lstIndices;
 
        // We want the vertex count to reflect a polygon
        if (vertexcount < 3) vertexcount = 3;
        if (vertexcount > 10000) vertexcount = 10000;
 
        normal.normalize();
 
        //Here we find the intersection of a plane whos normal is our normal vector,
        //with a sphere with radius 'radius'.
 
        //The equation of this circle is P + sV1 + tV2
        //where v1 id (0, c, -b) and v2 is (c, 0, -a), we need at least one nonzero one.
        core::vector3df v1 = core::vector3df(0, normal.Z, -normal.Y);
        core::vector3df v2 = core::vector3df(normal.Z, 0, -normal.X);
 
        if(v1 != core::vector3df(0,0,0))
        {
                v1 = v1 / sqrt(1 - (normal.X * normal.X));
                v2 = v1.crossProduct(normal);
        }
        else
        {
                v2 = v2 / sqrt(1 - (normal.Y * normal.Y));
                v1 = v2.crossProduct(normal);
        }
 
        lstIndices.clear();
        lstVertices.clear();
 
        video::S3DVertex vert;
        for(u32 x = 0; x < vertexcount; ++x)
        {
                vert.Pos = center + radius * cos(2 * core::PI / vertexcount * x) * v1 + radius * sin(2 * core::PI / vertexcount * x) * v2;
                vert.Color = color;
                lstIndices.push_back(lstVertices.size());
                lstVertices.push_back(vert);
                vert.Pos = center + radius * cos(2 * core::PI / vertexcount * (x + 1)) * v1 + radius * sin(2 * core::PI / vertexcount * (x + 1)) * v2;
                vert.Color = video::SColor(255, 255,255,255);
                lstIndices.push_back(lstVertices.size());
                lstVertices.push_back(vert);
        }
 
        // render
        driver->drawVertexPrimitiveList(&lstVertices[0], lstVertices.size(), &lstIndices[0], lstIndices.size() / 2, video::EVT_STANDARD, scene::EPT_LINES, video::EIT_16BIT);
}
