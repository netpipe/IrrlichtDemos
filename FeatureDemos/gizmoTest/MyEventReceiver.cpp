#include "MyEventReceiver.h"

MyEventReceiver::MyEventReceiver(IrrlichtDevice *device)
{
	Device = device;
	CollMan = Device->getSceneManager()->getSceneCollisionManager(); 
        LMouseButtonDown  = false;
        SelectedNode      = 0;
        GizmoSceneNode = 0;
        distanceToSelectedNode = vector3df(0,0,0);
        currentDistanceToSelectedNode = vector3df(0,0,0);

        gui::IGUISkin* skin = Device->getGUIEnvironment()->getSkin();
        gui::IGUIFont* font = Device->getGUIEnvironment()->getFont("Font/SkyFont.xml");
        if(font)
            skin->setFont(font);

        initialCursorPositionText = Device->getGUIEnvironment()->addStaticText(L"initialCursorPosition: ", rect<s32>(10,10, 400, 30), true, true, 0, -1, false);
        currentCursorPositionText = Device->getGUIEnvironment()->addStaticText(L"currentCursorPosition: ", rect<s32>(10,40, 400, 60), true, true, 0, -1, false);
        initialObjectPositionText = Device->getGUIEnvironment()->addStaticText(L"initialObjectPosition: ", rect<s32>(10,70, 400, 90), true, true, 0, -1, false);
        posText = Device->getGUIEnvironment()->addStaticText(L"effectiveObjectPosition: ", rect<s32>(10,100,400,120), true, true, 0, -1, false);

        gizmoSceneManager = Device->getSceneManager()->createNewSceneManager();

        IAnimatedMesh* arrowMeshRed = gizmoSceneManager->addArrowMesh( "x_axisArrow",video::SColor(255, 255, 0, 0),video::SColor(255, 255, 0, 0));
        nodeGizmoX = gizmoSceneManager->addMeshSceneNode(arrowMeshRed, gizmoSceneManager->getRootSceneNode(), x_axis);
        if(nodeGizmoX)
        {
            nodeGizmoX->setMaterialFlag(video::EMF_LIGHTING, false);
            nodeGizmoX->setRotation(vector3df(0,0,-90));
            nodeGizmoX->setScale(vector3df(10,30,10));
            nodeGizmoX->setPosition(vector3df(0,0,0));
            nodeGizmoX->setVisible(false);
        }

        IAnimatedMesh* arrowMeshGreen = gizmoSceneManager->addArrowMesh( "y_axisArrow",video::SColor(255, 0, 255, 0),video::SColor(255, 0, 255, 0));
        nodeGizmoY = gizmoSceneManager->addMeshSceneNode(arrowMeshGreen, gizmoSceneManager->getRootSceneNode(), y_axis);
        if(nodeGizmoY)
        {
            nodeGizmoY->setMaterialFlag(video::EMF_LIGHTING, false);
            nodeGizmoY->setScale(vector3df(10,30,10));
            nodeGizmoY->setPosition(vector3df(0,0,0));
            nodeGizmoY->setVisible(false);
        }


        IAnimatedMesh* arrowMeshBlue = gizmoSceneManager->addArrowMesh( "z_axisArrow",video::SColor(255, 0, 0, 255),video::SColor(255, 0, 0, 255));
        nodeGizmoZ = gizmoSceneManager->addMeshSceneNode(arrowMeshBlue, gizmoSceneManager->getRootSceneNode(), z_axis);
        if(nodeGizmoZ)
        {
            nodeGizmoZ->setMaterialFlag(video::EMF_LIGHTING, false);
            nodeGizmoZ->setRotation(vector3df(90,0,0));
            nodeGizmoZ->setScale(vector3df(10,30,10));
            nodeGizmoZ->setPosition(vector3df(0,0,0));
            nodeGizmoZ->setVisible(false);
        }

//        IAnimatedMesh* XZplaneMesh = Device->getSceneManager()->addHillPlaneMesh( "XZplaneMesh", core::dimension2d<f32>(15.0f, 15.0f),core::dimension2d<u32>(1, 1));
//        nodeGizmoXZplane = Device->getSceneManager()->addMeshSceneNode(XZplaneMesh, Device->getSceneManager()->getRootSceneNode(), 0);
//        if(nodeGizmoXZplane)
//        {
//            nodeGizmoXZplane->setMaterialFlag(video::EMF_LIGHTING, false);
//            nodeGizmoXZplane->setPosition(vector3df(7,0,7));
//        }
}

MyEventReceiver::~MyEventReceiver(void)
{
}

bool MyEventReceiver::OnEvent(const SEvent& event)
{
   nodeGizmoX->updateAbsolutePosition();
   nodeGizmoY->updateAbsolutePosition();
   nodeGizmoZ->updateAbsolutePosition();

   switch(event.EventType)
   {
      case EET_GUI_EVENT :
       {
       }
       break;
      case EET_MOUSE_INPUT_EVENT :
      {
		  switch (event.MouseInput.Event)
                  {
                  case EMIE_LMOUSE_PRESSED_DOWN :
                      {
                          nodeGizmoX->updateAbsolutePosition();
                          nodeGizmoY->updateAbsolutePosition();
                          nodeGizmoZ->updateAbsolutePosition();

                          LMouseButtonDown = true;

                          SelectedNode = CollMan->getSceneNodeFromScreenCoordinatesBB(Device->getCursorControl()->getPosition(), selectedNode,true);
                          GizmoSceneNode = gizmoSceneManager->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(Device->getCursorControl()->getPosition(), x_axis | y_axis | z_axis,true);

                          if(SelectedNode && SelectedNode->getType() != ESNT_CAMERA)
                          {
                              nodeGizmoX->setPosition(SelectedNode->getAbsolutePosition());
                              nodeGizmoY->setPosition(SelectedNode->getAbsolutePosition());
                              nodeGizmoZ->setPosition(SelectedNode->getAbsolutePosition());
                              nodeGizmoX->setVisible(true);
                              nodeGizmoY->setVisible(true);
                              nodeGizmoZ->setVisible(true);

                              if(GizmoSceneNode)
                              {
                                  initialCursorPosition.X = event.MouseInput.X;
                                  initialCursorPosition.Y = event.MouseInput.Y;
                                  initialObjectPosition = gizmoSceneManager->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(GizmoSceneNode->getAbsolutePosition(), gizmoSceneManager->getActiveCamera());
                              }
                          }


                      }
                      break;
                  case EMIE_LMOUSE_LEFT_UP :
                      {
                          LMouseButtonDown = false;
                          return false;
                      }
                      break;

                  case EMIE_MOUSE_MOVED :
                      {
                          //Подсвечиваем стрелки
                          ISceneNode *hilightedArrow = gizmoSceneManager->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(Device->getCursorControl()->getPosition(), 0,true);
                          if(hilightedArrow)
                          {
                              if(hilightedArrow->getID() == x_axis)
                              {
                                  hilightedArrow->getMaterial(0).MaterialType = EMT_TRANSPARENT_ADD_COLOR;

                              }
                              else
                              {
                                  nodeGizmoX->setMaterialType(EMT_SOLID);

                              }

                              if(hilightedArrow->getID() == y_axis)
                              {
                                  hilightedArrow->getMaterial(0).MaterialType = EMT_TRANSPARENT_ADD_COLOR;

                              }
                              else
                              {
                                  nodeGizmoY->setMaterialType(EMT_SOLID);

                              }

                              if(hilightedArrow->getID() == z_axis)
                              {
                                  hilightedArrow->getMaterial(0).MaterialType = EMT_TRANSPARENT_ADD_COLOR;

                              }
                              else
                              {
                                  nodeGizmoZ->setMaterialType(EMT_SOLID);

                              }

                          }
                          //--------------------------------------------------------//

                          if (!LMouseButtonDown)
                              return false;

                          if (GizmoSceneNode && SelectedNode->getType() != ESNT_CAMERA)
                          {
                              vector3df p = GizmoSceneNode->getPosition();

                              plane3df const planeXZ(GizmoSceneNode->getAbsolutePosition(), vector3df(0.f, 1.f, 0.f));
                              plane3df const planeXY(GizmoSceneNode->getAbsolutePosition(), vector3df(1.f, 0.f, 0.f));

                              position2di currentCursorPosition;
                              currentCursorPosition.X = event.MouseInput.X;
                              currentCursorPosition.Y = event.MouseInput.Y;
                              //effectiveObjectPosition - Возможно тут ошибка в расчетах
                              position2di effectiveObjectPosition = initialObjectPosition + currentCursorPosition - initialCursorPosition;

                              core::stringw tmp_1(L"effectiveObjectPosition: X: ");
                              tmp_1 += effectiveObjectPosition.X;
                              tmp_1 += L" Y: ";
                              tmp_1 += effectiveObjectPosition.Y;
                              posText->setText(tmp_1.c_str());

                              core::stringw tmp_2(L"initialObjectPosition: X: ");
                              tmp_2 += initialObjectPosition.X;
                              tmp_2 += L" Y: ";
                              tmp_2 += initialObjectPosition.Y;
                              initialObjectPositionText->setText(tmp_2.c_str());

                              core::stringw tmp_3(L"initialCursorPosition: X: ");
                              tmp_3 += initialCursorPosition.X;
                              tmp_3 += L" Y: ";
                              tmp_3 += initialCursorPosition.Y;
                              initialCursorPositionText->setText(tmp_3.c_str());

                              core::stringw tmp_4(L"currentCursorPosition: X: ");
                              tmp_4 += currentCursorPosition.X;
                              tmp_4 += L" Y: ";
                              tmp_4 += currentCursorPosition.Y;
                              currentCursorPositionText->setText(tmp_4.c_str());



                              line3df ray(gizmoSceneManager->getSceneCollisionManager()->getRayFromScreenCoordinates(effectiveObjectPosition, gizmoSceneManager->getActiveCamera()));
                              vector3df intersectWithPlane;

//                              vector3d<f32> CamFwd  = vector3d<f32>((GizmoSceneNode->getAbsolutePosition()) - (Device->getSceneManager()->getActiveCamera()->getPosition())).normalize();
//                              vector3d<f32> subPos = GizmoSceneNode->getPosition() - Device->getSceneManager()->getActiveCamera()->getPosition();
//                              f32 ScaleFactor = vector3d<f32>(subPos).dotProduct(CamFwd);
//                              ScaleFactor = ScaleFactor * 0.017;

                              //GizmoSceneNode->setScale(gizmoInitScale * ScaleFactor);

                              if(planeXZ.getIntersectionWithLine(ray.start, ray.getVector(), intersectWithPlane))
                              {
                                  if(GizmoSceneNode->getID() == x_axis)
                                  {
                                      GizmoSceneNode->setPosition(vector3df(intersectWithPlane.X, GizmoSceneNode->getAbsolutePosition().Y, GizmoSceneNode->getAbsolutePosition().Z));
                                      nodeGizmoY->setPosition(GizmoSceneNode->getAbsolutePosition());
                                      nodeGizmoZ->setPosition(GizmoSceneNode->getAbsolutePosition());
                                      SelectedNode->setPosition(GizmoSceneNode->getAbsolutePosition());
                                  }

                                  if(GizmoSceneNode->getID() == z_axis)
                                  {
                                      GizmoSceneNode->setPosition(vector3df(GizmoSceneNode->getAbsolutePosition().X, GizmoSceneNode->getAbsolutePosition().Y, intersectWithPlane.Z));
                                      nodeGizmoX->setPosition(GizmoSceneNode->getAbsolutePosition());
                                      nodeGizmoY->setPosition(GizmoSceneNode->getAbsolutePosition());
                                      SelectedNode->setPosition(GizmoSceneNode->getAbsolutePosition());
                                  }
                              }

                              if(planeXY.getIntersectionWithLine(ray.start, ray.getVector(), intersectWithPlane))
                              {
                                  if(GizmoSceneNode->getID() == y_axis)
                                  {
                                      GizmoSceneNode->setPosition(vector3df(GizmoSceneNode->getAbsolutePosition().X, intersectWithPlane.Y, GizmoSceneNode->getAbsolutePosition().Z));
                                      nodeGizmoX->setPosition(GizmoSceneNode->getAbsolutePosition());
                                      nodeGizmoZ->setPosition(GizmoSceneNode->getAbsolutePosition());
                                      SelectedNode->setPosition(GizmoSceneNode->getAbsolutePosition());
                                  }
                              }

                          }
                      }
                      break;
		  default:
			  break;
		  }
      }
      break;

      default:
         break;
   }
   return 0;
}

ISceneNode* MyEventReceiver::getSelectedNode()
{
	if(SelectedNode)
        {
            return SelectedNode;
	}
}

ISceneManager* MyEventReceiver::getGizmoSceneManager()
{
    return gizmoSceneManager;
}
