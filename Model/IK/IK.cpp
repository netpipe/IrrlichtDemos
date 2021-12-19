 //https://www.youtube.com/watch?v=JHk3Qe7kS-k&feature=youtu.be 
//http://irrlicht.sourceforge.net/forum/viewtopic.php?f=9&t=50615&p=298282#p298282

//usage 
//        vector3df target = vNode->getAbsolutePosition();
//    inverseKinematicAxisAngle(target);


/*  make one of these animators and volia
    vNode = mVudu->device->getSceneManager()->addMeshSceneNode(mesh, NULL, -1, core::vector3df(0, 0, 0));
     vNode->setScale(core::vector3df(0.5f, 0.5f, 0.5f));  // easier to adapt on testing than camera-code
 
     vNode->setMaterialFlag(E_MATERIAL_FLAG::EMF_LIGHTING, false);
 
 
scene::ISceneNodeAnimator * flyCircleAnimator = smgr->createFlyCircleAnimator(core::vector3df(0.f, 17.f, 5.0f), 3.0f, 0.002f, vector3df(0, 0, 1));
    vNode->addAnimator(flyCircleAnimator);
    flyCircleAnimator->drop();
*/

 #include "Eigen/Eigen"
 
    void ResetBonePosition(ISceneNode *Node)
        {
            Node->setRotation(vector3df(0, 0, 0));
            Node->updateAbsolutePosition();
            core::list<ISceneNode*>::ConstIterator it = Node->getChildren().begin();
            for (; it != Node->getChildren().end(); ++it)
            {
                UpdateAbsoluteTransformationAndChildren((*it));
            }
 
        }
 
  void LoadCharacter( irr::scene::ISceneManager* smgr, irr::core::stringc path)
     {
 
            irr::scene::IAnimatedMesh* mesh = smgr->getMesh(path);
            if (!mesh)
            {
                dev->drop();
                return;
              }    
            character = smgr->addAnimatedMeshSceneNode(mesh);
             if (character)
             {
                 character ->setJointMode(scene::EJUOR_CONTROL);
                  ResetBonePosition(character );
                 character ->setMaterialFlag(irr::video::E_MATERIAL_FLAG::EMF_LIGHTING, true);
             }
 
          }
 
//Function to compute jacobian
//Only rotation. If you want translation just add three more dof and use the calculated axis values.
 Eigen::Matrix<double, 3, Eigen::Dynamic>  :ComputeJacobiAxisAngleMethod(std::vector<irr::scene::ISceneNode *> joints,  std::vector<  irr::core::vector3df> lstRot, Eigen::Matrix<double, 3, Eigen::Dynamic> jacob, vector3df preViousEndEffectorPos, vector3df target, int &smallAngleCount)
     {
         int jointsCount = joints.size();
         vector3df e = preViousEndEffectorPos;
   
         for (int j = 0; j < jointsCount; ++j)
         {
           
             vector3df jPos = joints[j]->getAbsolutePosition();
 
             vector3df axis = ((e - jPos).crossProduct(target - jPos))*lstRot[j];
             axis.normalize();
 
             vector3df JabValue = axis.crossProduct(e - jPos);
             JabValue.normalize();
 
                 if (JabValue.getLength() <= 0.00001)
                  {
                      jacob(0, j) = 0;
                      jacob(1, j) = 0;
                      jacob(2, j) = 0;
                  smallAngleCount++;
                  continue;
                  }
 
             jacob(0, j) = JabValue.X;
             jacob(1, j) = JabValue.Y;
             jacob(2, j) = JabValue.Z;
         }
 
         return jacob;
     }
 
 void inverseKinematicAxisAngle(vector3df &target)
     {
 
         irr::scene::IBoneSceneNode *base = character ->getJointNode("Rshoulder");   //Make sure your b3d file has this otherwise you will have a segment fault error. Sorry I don't check for error.
         irr::scene::IBoneSceneNode *endEffector =character ->getJointNode("Rhand"); //Make sure your b3d file has this otherwise you will have a segment fault error.
        ISceneNode *current = endEffector;
 
         std::vector<int> jointRotDof;
         int count = endEffector->getChildren().getSize();
 
         int jointscount = 0;
         f32 epsilon = 0.05;
         f32 alpha = 0.5;   //For iteration update. Play around with this parameter for smooth movement
 
         std::vector<ISceneNode *> joints;
         jointscount = 1;
         current = endEffector;
         while (current != base)   //Get joint count to base joint. You would want to refactor and move this code elsewhere.
         {
             jointscount++;
             current = current->getParent();
         }
 
         int totalDof = 0;
         current = endEffector;
   
         std::vector <vector3df> lstRot;
         //Set joint rotation capability.  You would want to refactor and move this code elsewhere. Maybe at init or create set function.
         for (int i = 0; i < jointscount; ++i)
         {
             if (irr::core::stringc(current->getName()) == "Rhand" || i == 0)
             {
 
                 joints.push_back(current);  //ef
 
                  vector3df axis { 0, 0, 0 };
                  lstRot.push_back(axis);
             }
 
             else if (irr::core::stringc(current->getName()) == "Relbow")
             {
                 joints.insert(joints.begin(),current);  //ef
                 lstRot.insert(lstRot.begin(), vector3df(0, 1, 1));
                //Or try this.... lstRot.insert(lstRot.begin(), vector3df(1, 1, 1));
 
             }
             else if (irr::core::stringc(current->getName()) == "Rwrist")
             {
                 joints.insert(joints.begin(),current);  //ef
                 lstRot.insert(lstRot.begin(),vector3df(1, 0, 0));
 
             }
             else
             {
                 joints.insert(joints.begin(), current);
                 vector3df axis{1, 1, 1 };
                 lstRot.insert(lstRot.begin(), axis);
 
             }
 
             current = current->getParent();
         }
 
 
         //Need to do this in irrlicht to reset joints to zero pos.
         //Otherwise matrix or quaternion calculation will cause a small twist,
         //results in error after several revolutions.
         for (int j = 0; j < jointscount; ++j)
         {
             joints[j]->setRotation(vector3df(0, 0, 0));
             joints[j]->getParent()->updateAbsolutePosition();
             joints[j]->updateAbsolutePosition();
             ((irr::scene::IBoneSceneNode *) joints[j])->updateAbsolutePositionOfAllChildren();
         }
 
        // jointscount = joints.size();
         ISceneNode *lastjoint = endEffector;
         Eigen::MatrixXd jacob(3, jointscount);
         Eigen::MatrixXd delta_pos_new(6, jointscount);
 
         int maxIteration = 300;
         vector3df e = lastjoint->getAbsolutePosition();
 
         double distance = e.getDistanceFrom(target);
 
 
         //IK loop using jacobian transpose.
         //To change it to other method. Just updating the equation where I change calculate dangle
         while (distance >= epsilon && maxIteration > 0)
         {
             maxIteration--;
 
             vector3df deltaE = target - e;
             Eigen::Vector3d edeltaE(deltaE.X, deltaE.Y, deltaE.Z);
             //Compute Jacobian by change in end effector position.
             int smallAngleCount = 0;
 
             //Calculate jacobian the simple way.
             jacob = ComputeJacobiAxisAngleMethod(joints, lstRot, jacob, e, target, smallAngleCount);
             if (smallAngleCount == jointscount)
             {
                 return;
             }
   
             Eigen::MatrixXd transp = jacob.transpose();
             //Compute angle by transpose jacobian matrix.
             Eigen::MatrixXd dangle = transp*edeltaE;   //For other method, just update this equation.
           
             int index = -1;
             for (int j = 0; j < jointscount; ++j)
             {
                 float outAngle = dangle(j);
                 vector3df jPos = joints[j]->getAbsolutePosition();
                 vector3df p1 = e - jPos;
                 p1.normalize();
                 vector3df p2 = target - jPos;
                 p2.normalize();
 
                 vector3df axis = (p1).crossProduct(p2);    //Joint rotation axis.
                 axis = axis*lstRot[j];                     //Readjust to the correct rotatable axis.
                 axis.normalize();
 
                 irr::core::quaternion q;
                 q = q.fromAngleAxis(outAngle*DEGTORAD*alpha, axis);    //We can remove the alpha value or set it to 1 above.
                 q.normalize();
 
                 vector3df rot = joints[j]->getRotation();
   
 
                 quaternion q2;
                 q2.set(rot*DEGTORAD);
                 q2.normalize();
                 q2 = q2*q;
                 rot =  q2.getMatrix().getRotationDegrees();
 
                 TrimVectorValues(rot);
                 //Check for constraint here.
                 joints[j]->setRotation(rot);
                 joints[j]->updateAbsolutePosition();
       
             }
 
             e = joints[jointscount - 1]->getAbsolutePosition();
             distance = (double)e.getDistanceFrom(target);
 
         }
 
 
     }
 
 
