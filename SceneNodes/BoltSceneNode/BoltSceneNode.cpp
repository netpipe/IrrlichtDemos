#include "BoltSceneNode.h" 
namespace irr 
{ 
    namespace scene 
    { 
        CBoltSceneNode::CBoltSceneNode( scene::ISceneNode* parent, scene::ISceneManager *mgr, s32 id,char* szBeam ) : ISceneNode( parent, mgr, id ) 
        { 
            //ctor 
            material.Wireframe = false; 
            material.Lighting = false; 
			material.ZWriteEnable = false;            
			material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
            material.setTexture(0, mgr->getVideoDriver( )->getTexture( szBeam )); 
			m_thickness = 5.0f; 
            AutomaticCullingState = EAC_FRUSTUM_SPHERE; 
        } 

        CBoltSceneNode::~CBoltSceneNode() 
        { 
            //dtor 
        } 

        void CBoltSceneNode::OnRegisterSceneNode(void) 
        { 
            if ( IsVisible ) 
            { 
                SceneManager->registerNodeForRendering( this ); 
				 ISceneNode::OnRegisterSceneNode(); 
            } 			
        } 

        void CBoltSceneNode::OnAnimate(u32 timeMs) 
        { 

            if ( IsVisible ) 
            { 
                for (u32 i=0;i<boltarray.size();i++) 
                { 
                    boltarray[i].Update(timeMs); 
                } 
            } 

            ISceneNode::OnAnimate(timeMs); 
        } 

		void CBoltSceneNode::setLine(core::vector3df start, core::vector3df end, u32 updateTime, u32 height, u32 parts, u32 bolts, bool steddyend, f32 thick, video::SColor color) 
        { 
           setPosition(start); 
			m_thickness = thick;
            srand(parts); 
            boltarray.clear(); 

            m_start = core::vector3df(0,0,0); 
            m_end = end-start; 
            m_parts = parts; 
            material.DiffuseColor = color; 

            core::vector3df dir = end-start; 
            dir/=(f32)parts; 

            for (u32 i=0;i<bolts;i++) 
            { 
                //printf("add Bolt\n"); 
                boltarray.push_back(Bolt(height, updateTime, steddyend)); 

                for (u32 a=0;a<parts;a++) 
                { 
                    //printf("add Part[%i] ", a); 
                    boltarray[i].points.push_back(m_start+dir*a); 
                    if (a == parts-1 || a == 0) 
                        boltarray[i].delta.push_back(0.0f); 
                    else 
                    { 
                        u32 h = height*2; 
                        f32 d = (rand()%h); 
                        d-=(f32)height; 
                        //printf("Delta: %f", d); 
                        boltarray[i].delta.push_back(d); 
                    } 
                    //printf("\n"); 
                } 

                u32 vec = rand()%2; 
            //printf("vec: %i\n", vec); 

            u32 cord = rand()%10+1; 

                if (dir.X != 0) 
                { 
                    if (vec == 0) 
                        boltarray[i].normal = core::vector3df(-dir.Y/dir.X*cord,cord,0); 
                    else 
                        boltarray[i].normal = core::vector3df(-dir.Z/dir.X*cord,0,cord); 
                } 
                else if (dir.Y != 0) 
                { 
                    if (vec == 0) 
                        boltarray[i].normal = core::vector3df(cord,-dir.X/dir.Y*cord,0); 
                    else 
                        boltarray[i].normal = core::vector3df(0,-dir.Z/dir.Y*cord,cord); 
                } 
                else if (dir.Z != 0) 
                { 
                    if (vec == 0) 
                        boltarray[i].normal = core::vector3df(cord,0,-dir.X/dir.Z*cord); 
                    else 
                        boltarray[i].normal = core::vector3df(0,cord,-dir.Y/dir.Z*cord); 
                } 
                boltarray[i].normal.normalize(); 
            } 
        } 
		
		void CBoltSceneNode::drawbeam(Bolt beam)
		{
			video::IVideoDriver* driver = SceneManager->getVideoDriver(); 
            driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation); 
			core::vector3df d_start;
			core::vector3df d_end;
			video::S3DVertex store_vertices[4]; //luu lai vi tri 
			//
			for (u32 i=0;i<beam.points.size()-1;i++) 
			{
				d_start = beam.points[i]+beam.normal*beam.delta[i];
				d_end =  beam.points[i+1]+beam.normal*beam.delta[i+1];
				core::vector3df direction = d_end-d_start; 
				direction.normalize(); 
				core::vector3df savesatrt = d_start; 
				AbsoluteTransformation.transformVect(savesatrt); 
				core::vector3df lookdir = savesatrt-SceneManager->getActiveCamera()->getAbsolutePosition(); 
				if(lookdir.getLength() < 20) 
					return; 
				lookdir.normalize(); 

				core::vector3df view(SceneManager->getActiveCamera()->getTarget() - SceneManager->getActiveCamera()->getAbsolutePosition()); 
				view.normalize(); 

				f32 angle = lookdir.dotProduct(direction)/*/(lookdir.getLength()*direction.getLength())*/; 

				if(angle < 0.9999f && angle > -0.9999f) 
				{ 
					core::vector3df updown = direction.crossProduct(lookdir); 
					updown.normalize(); 
					core::vector3df normal = direction.crossProduct(updown); 
					video::S3DVertex vertices[4]; 
					u16 indices[] = {0,1,2,1,3,2}; 

					vertices[0] = video::S3DVertex(d_end-updown*m_thickness*0.5f, normal, video::SColor(255,255,255,255), core::vector2d<f32>(0,0)); 
					vertices[1] = video::S3DVertex(d_end+updown*m_thickness*0.5f, normal, video::SColor(255,255,255,255), core::vector2d<f32>(0,1)); 
					vertices[2] = video::S3DVertex(d_start-updown*m_thickness*0.5f, normal, video::SColor(255,255,255,255), core::vector2d<f32>(1,0)); 
					vertices[3] = video::S3DVertex(d_start+updown*m_thickness*0.5f, normal, video::SColor(255,255,255,255), core::vector2d<f32>(1,1)); 
					if (i==0) 
					{
						store_vertices[0] = vertices[0];	store_vertices[1] = vertices[1];
						store_vertices[2] = vertices[2];	store_vertices[3] = vertices[3];						
					}
					else
					{
						store_vertices[2] = store_vertices[0];	 store_vertices[3] = store_vertices[1];
						store_vertices[0] = vertices[0];			 store_vertices[1] = vertices[1];
						if (i>=beam.points.size()-2)
						{
							store_vertices[0] = video::S3DVertex(d_end, normal, video::SColor(255,255,255,255), core::vector2d<f32>(0.5f,0.5f)); 
							store_vertices[1] = video::S3DVertex(d_end, normal, video::SColor(255,255,255,255), core::vector2d<f32>(0.5f,0.5f)); 
						}
					}					
					driver->setMaterial(material); 
					driver->drawIndexedTriangleList(&store_vertices[0], 4, &indices[0], 2);                 
				} 			
			}//for
		}

        void CBoltSceneNode::render(void) 
        { 
            video::IVideoDriver* driver = SceneManager->getVideoDriver(); 
            driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation); 
            driver->setMaterial(material); 

            for (u32 i=0;i<boltarray.size();i++) 
            { 
                boltarray[i].draw(driver, material.DiffuseColor); 
				drawbeam(boltarray[i]);
			}
        } 

        const core::aabbox3d<f32>& CBoltSceneNode::getBoundingBox() const 
        { 
            return Box; 
        } 

        u32 CBoltSceneNode::getMaterialCount() const 
        { 
            return 1; 
        } 

        video::SMaterial& CBoltSceneNode::getMaterial(u32 i) 
        { 
            return material; 
        } 
    } 
} 

