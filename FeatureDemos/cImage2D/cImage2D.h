class cImage {
        irr::core::recti ImageRect, OrigImageRect, TextureRect, BoundRect;
        irr::core::position2di RotationPoint;
        irr::f32 Rotation, OldRotation;
        irr::video::ITexture* Texture;
        irr::core::aabbox3df Box;
        irr::video::S3DVertex Vertices[4];
        irr::video::SMaterial Material;
        bool Transparent;
        irr::video::SColor TransColor;
        irr::video::SColor Color;
        irr::core::vector2df Scale;
public:


        cImage(irr::video::ITexture* texture, irr::core::recti nsize, irr::core::recti texpos)
        {
                Color = irr::video::SColor(0,255,255,255);
                Rotation = 0.f;
                OldRotation = 1.f;
                Scale = irr::core::vector2df(1,1);
                Texture = texture;
                Material.Wireframe = false;
                Material.Lighting = false;
                Material.ZBuffer = irr::video::ECFN_NEVER;//irr::video::ECFN_NEVER
                Material.TextureLayer[0].Texture = Texture;
                ImageRect = nsize;
                OrigImageRect = ImageRect;
                TextureRect = texpos;
                irr::core::position2di ulc = texpos.UpperLeftCorner;
                irr::core::position2di lrc = texpos.LowerRightCorner;
                irr::core::dimension2du td = texture->getSize();

                Vertices[0] = irr::video::S3DVertex(-0.5,0.5,0, 1,1,0,
                        irr::video::SColor(255,255,255,255), (irr::f32)ulc.X/(irr::f32)td.Width, (irr::f32)ulc.Y/(irr::f32)td.Height);
                Vertices[1] = irr::video::S3DVertex(0.5,0.5,0, 1,0,0,
                        irr::video::SColor(255,255,255,255), (irr::f32)lrc.X/(irr::f32)td.Width, (irr::f32)ulc.Y/(irr::f32)td.Height);
                Vertices[2] = irr::video::S3DVertex(-0.5,-0.5,0, 0,1,1,
                        irr::video::SColor(255,255,255,255), (irr::f32)ulc.X/(irr::f32)td.Width, (irr::f32)lrc.Y/(irr::f32)td.Height);
                Vertices[3] = irr::video::S3DVertex(0.5,-0.5,0, 0,0,1,
                        irr::video::SColor(255,255,255,255), (irr::f32)lrc.X/(irr::f32)td.Width, (irr::f32)lrc.Y/(irr::f32)td.Height);

                Box.reset(Vertices[0].Pos);
                for (irr::u32 i=1; i<4; ++i)
                        Box.addInternalPoint(Vertices[i].Pos);
                OrigImageRect = ImageRect;
                switch(texture->getColorFormat())
                {
                case irr::video::ECF_A1R5G5B5:
                case irr::video::ECF_A8R8G8B8:
                case irr::video::ECF_A16B16G16R16F:
                case irr::video::ECF_A32B32G32R32F:
                        this->SetTransparent(true);
                        break;
                default:
                        this->SetTransparent(false);
                        break;
                }
        }

        void Draw(irr::scene::ISceneManager* SceneManager) {
                irr::u16 indices[] = {  0,1,2,  3,2,1};
                irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
                irr::core::line3df rays[4];
                irr::core::position2di pos[4];
                irr::core::position2di imagpos[4];
                imagpos[0] = ImageRect.UpperLeftCorner;
                imagpos[1] = irr::core::position2di(ImageRect.LowerRightCorner.X, ImageRect.UpperLeftCorner.Y);
                imagpos[2] = irr::core::position2di(ImageRect.UpperLeftCorner.X,ImageRect.LowerRightCorner.Y);
                imagpos[3] = irr::core::position2di(ImageRect.LowerRightCorner);
                irr::f32 gx, gy, lx, ly;
                gx = (irr::f32)imagpos[0].X;
                gy = (irr::f32)imagpos[0].Y;
                lx = (irr::f32)imagpos[0].X;
                ly = (irr::f32)imagpos[0].Y;
                for (irr::u16 x = 0; x < 4; x++) {
                        if (imagpos[x].X > gx)
                                gx = (irr::f32)imagpos[x].X;
                        if (imagpos[x].X < lx)
                                lx = (irr::f32)imagpos[x].X;
                        if (imagpos[x].Y > gy)
                                gy = (irr::f32)imagpos[x].Y;
                        if (imagpos[x].Y < ly)
                                ly = (irr::f32)imagpos[x].Y;
                }
                RotationPoint.X = (irr::s32)(lx+gx)/2;
                RotationPoint.Y = (irr::s32)(ly+gy)/2;
                irr::f32 angles[4];
                irr::f32 ed[4];
                for (irr::u16 x = 0; x < 4; x++) {
                        angles[x] = atan2((irr::f32)imagpos[x].Y-RotationPoint.Y,(irr::f32)imagpos[x].X-RotationPoint.X);
                        ed[x] = sqrt((irr::f32)((RotationPoint.X-imagpos[x].X)*(RotationPoint.X-imagpos[x].X)) + ((RotationPoint.Y-imagpos[x].Y)*(RotationPoint.Y-imagpos[x].Y)));
                        irr::f32 realang = angles[x] + Rotation*irr::core::DEGTORAD;
                        imagpos[x].X = RotationPoint.X+(irr::s32)(cos(realang)*ed[x]);
                        imagpos[x].Y = RotationPoint.Y+(irr::s32)(sin(realang)*ed[x]);
                }
                gx = (irr::f32)imagpos[0].X;
                gy = (irr::f32)imagpos[0].Y;
                lx = (irr::f32)imagpos[0].X;
                ly = (irr::f32)imagpos[0].Y;
                for (irr::u16 x = 0; x < 4; x++) {
                        if (imagpos[x].X > gx)
                                gx = (irr::f32)imagpos[x].X;
                        if (imagpos[x].X < lx)
                                lx = (irr::f32)imagpos[x].X;
                        if (imagpos[x].Y > gy)
                                gy = (irr::f32)imagpos[x].Y;
                        if (imagpos[x].Y < ly)
                                ly = (irr::f32)imagpos[x].Y;
                }
                BoundRect = irr::core::recti((irr::s32)lx,(irr::s32)ly,(irr::s32)gx,(irr::s32)gy);
                rays[0] = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(imagpos[0],SceneManager->getActiveCamera());
                rays[1] = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(imagpos[1],SceneManager->getActiveCamera());
                rays[2] = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(imagpos[2],SceneManager->getActiveCamera());
                rays[3] = SceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(imagpos[3],SceneManager->getActiveCamera());
                irr::f32 dist = rays[0].end.getDistanceFrom(SceneManager->getActiveCamera()->getPosition());
                irr::f32 div = dist/((SceneManager->getActiveCamera()->getNearValue()+1)*(SceneManager->getActiveCamera()->getNearValue()+1));
                for (irr::u16 x = 0; x < 4; x++) {
                        irr::f32 xd,yd,zd;
                        xd = rays[x].end.X - rays[x].start.X;
                        yd = rays[x].end.Y - rays[x].start.Y;
                        zd = rays[x].end.Z - rays[x].start.Z;

                        rays[x].start.X += (xd/div);
                        rays[x].start.Y += (yd/div);
                        rays[x].start.Z += (zd/div);
                        Vertices[x].Pos = rays[x].start;
                }
                driver->setMaterial(Material);
                irr::core::matrix4 mat;
                driver->setTransform(irr::video::ETS_WORLD, mat);
                driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 4);
        }

        void SetTransparent(bool trans) {
                Transparent = trans;
                if (Transparent)
                        Material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
                else
                        Material.MaterialType = irr::video::EMT_SOLID;
        };

        bool GetTransparent() { return Transparent; };

        void SetTransparentColor(irr::video::SColor trans, irr::video::IVideoDriver *driver) {
                TransColor = trans;
                driver->makeColorKeyTexture(Texture,TransColor);
        };

        void SetPosition(irr::core::position2di newpos) {
                irr::f32 xd, yd;
                xd = irr::f32(newpos.X - ImageRect.UpperLeftCorner.X);
                yd = irr::f32(newpos.Y - ImageRect.UpperLeftCorner.Y);
                ImageRect = irr::core::recti(ImageRect.UpperLeftCorner.X+(irr::s32)xd,ImageRect.UpperLeftCorner.Y+(irr::s32)yd,ImageRect.LowerRightCorner.X+(irr::s32)xd,ImageRect.LowerRightCorner.Y+(irr::s32)yd);
                OrigImageRect = irr::core::recti(OrigImageRect.UpperLeftCorner.X+(irr::s32)xd,OrigImageRect.UpperLeftCorner.Y+(irr::s32)yd,OrigImageRect.LowerRightCorner.X+(irr::s32)xd,OrigImageRect.LowerRightCorner.Y+(irr::s32)yd);
        }

        void SetColor(irr::video::SColor newc) {
                Color = newc;
                for (irr::u16 x = 0; x < 4; x++) {
                        Vertices[x].Color = Color;
                }
        }

        irr::video::SColor GetColor() { return Color; };

        irr::video::SColor GetTransparentColor() { return TransColor; };

        void SetBilinearFilter(bool on) { Material.TextureLayer[0].BilinearFilter = on; };

        void SetTrilinearFilter(bool on) { Material.TextureLayer[0].TrilinearFilter = on; };

        void SetAnisotropicFilter(bool on) { Material.TextureLayer[0].AnisotropicFilter = on; };

        bool GetBilinearFilter() { return Material.TextureLayer[0].BilinearFilter; };

        bool GetTrilinearFilter() { return Material.TextureLayer[0].TrilinearFilter; };

        bool GetAnisotropicFilter() { return Material.TextureLayer[0].AnisotropicFilter > 0; };

        void SetRotation(irr::f32 newrot) { Rotation = newrot; };

        irr::f32 GetRotation() { return Rotation; };

        int GetWidth() {
                 return (ImageRect.UpperLeftCorner.X-ImageRect.LowerRightCorner.X);
                 };

        int GetHeight() {
                 return (ImageRect.UpperLeftCorner.Y-ImageRect.LowerRightCorner.Y);
                 };

        irr::s32 GetOrigWidth() {
                return (OrigImageRect.UpperLeftCorner.X-ImageRect.LowerRightCorner.X);
                };

        irr::s32 GetOrigHeight() {
                 return (OrigImageRect.UpperLeftCorner.Y-ImageRect.LowerRightCorner.Y);
                 };

        void SetTexture(irr::video::ITexture* tex) { Texture = tex; Material.TextureLayer[0].Texture = Texture; };

        irr::core::recti GetBoundRect() { return BoundRect; };

        void SetScale(irr::core::vector2df news) {
                Scale = news;
                ImageRect = irr::core::recti(OrigImageRect.UpperLeftCorner.X,
                                              OrigImageRect.UpperLeftCorner.Y,
                                              OrigImageRect.UpperLeftCorner.X+(irr::s32)
                                             (this->GetOrigWidth()*news.X),
                                              OrigImageRect.UpperLeftCorner.Y+(irr::s32)
                                             (this->GetOrigHeight()*news.Y));
        }

        irr::video::ITexture *GetTexture() { return Texture; };

};
