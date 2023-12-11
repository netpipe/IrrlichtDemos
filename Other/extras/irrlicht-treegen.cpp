//http://www.irrlicht3d.de/forum/viewtopic.php?f=6&t=810
        #region CreateTree
        /// <summary>
        ///
        /// </summary>
        /// <param name="smgr"></param>
        /// <param name="driver"></param>
        /// <param name="BranchHeightLimiter">Branch generate on this height</param>
        /// <param name="BranchNumberIn3Site"></param>
        /// <param name="BranchTexturPath">Set 0 on string for the Default texture</param>
        /// <param name="BranchMeshPath">Path of the Branch Mesh</param>
        /// <param name="TreeTexturPath">Set 0 on string for the Default texture</param>
        /// <param name="TreeMeshPath"></param>
        /// <param name="v3d"></param>
        public void CreateTree(SceneManager smgr,VideoDriver driver,int BranchHeightLimiter,int BranchNumberIn3Site, string BranchTexturPath,string BranchMeshPath,string TreeTexturPath,string TreeMeshPath,Vector3D v3d)
        {
            AnimatedMesh TreeMesh = smgr.GetMesh(TreeMeshPath);
            AnimatedMeshSceneNode TreeMeshNode = smgr.AddAnimatedMeshSceneNode(TreeMesh);

            TreeMeshNode.Position = v3d;
           
            if (TreeMeshNode != null)
            {
                TreeMeshNode.SetMaterialFlag(MaterialFlag.Lighting, false);
                if (TreeTexturPath == "0")
                {
                }
                else
                TreeMeshNode.SetMaterialTexture(0, driver.GetTexture(TreeTexturPath));

                 //= BranchHeightLimiter;
            }

            int DefaultBranchHeightLimiter = BranchHeightLimiter;
            int DefaultBranchHeightLimiter2 = BranchHeightLimiter;
            bool HeightControler = false;
            bool HeightControler2 = false;
            bool HeightControler3 = false;

            for (int empty1 = 0; empty1 <= BranchNumberIn3Site; empty1++)
            {
                AnimatedMesh BranchMesh = smgr.GetMesh(BranchMeshPath);
                AnimatedMeshSceneNode BranchMeshNode = smgr.AddAnimatedMeshSceneNode(BranchMesh);

                BranchMeshNode.SetMaterialFlag(MaterialFlag.Lighting, false);
                BranchMeshNode.SetMaterialType(MaterialType.TransparentAlphaChannel);
                if (BranchTexturPath == "0")
                {
                }
                else
                BranchMeshNode.SetMaterialTexture(0, driver.GetTexture(BranchTexturPath));
                BranchMeshNode.Position = v3d;
                Random ran = new Random();
                int nRand = ran.Next(10, 360);
                //Heightlimiter
                Vector3D v = BranchMeshNode.Position;
                v.Y = BranchHeightLimiter;
                BranchMeshNode.Position = v;

                Vector3D vR = BranchMeshNode.Rotation;
                BranchMeshNode.Rotation = vR;

                if (HeightControler == true)
                {
                    BranchHeightLimiter += 49;
                    HeightControler = false;
                }
                if (HeightControler == false)
                {
                    BranchHeightLimiter -= 29;
                    HeightControler = true;
                }
            }

            for (int empty1 = 0; empty1 <= BranchNumberIn3Site; empty1++)
            {
                AnimatedMesh BranchMesh = smgr.GetMesh(BranchMeshPath);
                AnimatedMeshSceneNode BranchMeshNode = smgr.AddAnimatedMeshSceneNode(BranchMesh);
                BranchMeshNode.Rotation = new Vector3D(0, -120, 0);
                BranchMeshNode.SetMaterialFlag(MaterialFlag.Lighting, false);
                BranchMeshNode.SetMaterialType(MaterialType.TransparentAlphaChannel);

                if (BranchTexturPath == "0")
                {
                }
                else
                BranchMeshNode.SetMaterialTexture(0, driver.GetTexture(BranchTexturPath));
                BranchMeshNode.Position = v3d;

                Random ran = new Random();
                int nRand = ran.Next(10, 360);
                //Heightlimiter
                Vector3D v = BranchMeshNode.Position;
                v.Y = DefaultBranchHeightLimiter;
                BranchMeshNode.Position = v;

                Vector3D vR = BranchMeshNode.Rotation;
                BranchMeshNode.Rotation = vR;

                if (HeightControler2 == true)
                {
                    DefaultBranchHeightLimiter +=45;
                    HeightControler2 = false;
                }
                if (HeightControler2 == false)
                {
                    DefaultBranchHeightLimiter -= 25;
                    HeightControler2 = true;
                }
            }

            for (int empty1 = 0; empty1 <= BranchNumberIn3Site; empty1++)
            {
                AnimatedMesh BranchMesh = smgr.GetMesh(BranchMeshPath);
                AnimatedMeshSceneNode BranchMeshNode = smgr.AddAnimatedMeshSceneNode(BranchMesh);
                BranchMeshNode.Rotation = new Vector3D(0, -240, 0);
                BranchMeshNode.SetMaterialFlag(MaterialFlag.Lighting, false);
                BranchMeshNode.SetMaterialType(MaterialType.TransparentAlphaChannel);
                if (BranchTexturPath == "0")
                {
                }
                else
                BranchMeshNode.SetMaterialTexture(0, driver.GetTexture(BranchTexturPath));
                BranchMeshNode.Position = v3d;

                Random ran = new Random();
                int nRand = ran.Next(10, 360);
                //Heightlimiter
                Vector3D v = BranchMeshNode.Position;
                v.Y = DefaultBranchHeightLimiter2;
                BranchMeshNode.Position = v;

                Vector3D vR = BranchMeshNode.Rotation;
                BranchMeshNode.Rotation = vR;

                if (HeightControler3 == true)
                {
                    DefaultBranchHeightLimiter2 += 45;
                    HeightControler3 = false;
                }
                if (HeightControler3 == false)
                {
                    DefaultBranchHeightLimiter2 -= 25;
                    HeightControler3 = true;
                }
            }
        }
        #endregion CreateRandomTree
