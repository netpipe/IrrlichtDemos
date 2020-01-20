#pragma once
class Wheel
{
private:
	IrrlichtDevice*		device;		
	IAnimatedMesh*		mesh;					// модель колеса
	scene::ISceneNode*	sceneNode;				// представитель сцены колеса
	ITexture*			texture;				// текстура колеса

	
	CIrrOdeBody*			wheelBody;  
    CIrrOdeGeomSphere*		wheelGeom;                        
    CIrrOdeJointHinge2*		wheelJointHinge;
    CIrrOdeMotor*			wheelMotor;



	vector3df	position;	
	vector3df	range;		// вектор расстояния от машины (прилепляет колеса к машине) НЕ НАДО НАВЕРНОЕ??!
	
public:
	// вектор rotation - вынужденный костыль потому что модель колеса состоит из двух непонятных частей,
	//поэтому приходится торчащий кусок прятать под машину,менять ничего не хотел
	Wheel(IrrlichtDevice* irrDevice, CIrrOdeBody* carBody, irr::core::vector3df carPosition,irr::core::vector3df range )
	{
		device = irrDevice;
		//driver = dev->getVideoDriver();
		//smgr = dev->getSceneManager();
		ISceneManager* smgr = device->getSceneManager();
		mesh = device->getSceneManager()->getMesh("models/wheelnew.obj");			//выставляем модель из файла
		texture = device->getVideoDriver()->getTexture("models/road.psd");	//выставляем текстуры из файла
		
		this->range = range;	
		


			
		wheelJointHinge = reinterpret_cast<CIrrOdeJointHinge2 *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_HINGE2_NAME),carBody));
    
		wheelJointHinge->setHingeAxis1(vector3df(0,1,0));
		wheelJointHinge->setHingeAxis2(vector3df(1,0,0));
		wheelJointHinge->setParameter(0,CIrrOdeJoint::eHiStop,0.5);
		wheelJointHinge->setParameter(0,CIrrOdeJoint::eLoStop,0.5);
		wheelJointHinge->setPosition(carPosition+range);
		//setPosition((MyCar.Car_AnimatedMeshSceneNode->getPosition())+(MyCar.Car_Scale * (MyCar.Car_AnimatedMeshSceneNode->getJointNode("b_whee1")->getPosition())));
	
		wheelBody = reinterpret_cast<CIrrOdeBody *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),wheelJointHinge));    
    
		
		sceneNode = smgr->addAnimatedMeshSceneNode(mesh,wheelBody);		
			
		//sceneNode->setPosition(carPosition/* + range*/);
		sceneNode->setScale(sceneNode->getScale()*30.f);
		sceneNode->setMaterialTexture(0,texture);
		sceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
		//sceneNode->setRotation(vector3df(0,90,0));
		

	
		wheelGeom = reinterpret_cast<CIrrOdeGeomSphere *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_SPHERE_NAME),sceneNode));
   
		wheelGeom->getSurfaceParameters(0)->setMu(1000);
		//wheelGeom->getSurfaceParameters(0)->setMu2(0);
		wheelGeom->setMassTotal(0.01);
		wheelGeom->drop();
		
		wheelMotor = reinterpret_cast<CIrrOdeMotor *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_MOTOR_NAME),carBody));
		wheelMotor->setJoint(wheelJointHinge);
		wheelMotor->setAutomaticCulling(EAC_BOX);
		wheelMotor->setAxis(1);
		wheelMotor->setForce(100.000000);
		wheelMotor->setVelocity(30.000);
		wheelMotor->setEnabled(false);// DF:SYQ ПАРАМЕТР!!!!!!!!!!!!!!!
	
	
	
		
		/*
	
		wheelJointHinge=reinterpret_cast<CIrrOdeJointHinge2 *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_HINGE2_NAME),carBody));
  
  //MyCar.test_1_JointHinge->setBody2(MyCar.Car_whell_1_OdeBody);
 wheelJointHinge->setHingeAxis1(vector3df(0,1,0));
  wheelJointHinge->setHingeAxis2(vector3df(1,0,0));
  //Brainsaw start
 wheelJointHinge->setParameter(0,CIrrOdeJoint::eHiStop,0.5);
  wheelJointHinge->setParameter(0,CIrrOdeJoint::eLoStop,0.5);
  //Brainsaw end
  wheelJointHinge->setPosition(carPosition + range);

  wheelBody = reinterpret_cast<CIrrOdeBody *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),wheelJointHinge));
  

 sceneNode=smgr->addAnimatedMeshSceneNode(mesh, wheelBody);
  
  //MyCar.Car_whell_1_AnimatedMeshSceneNode->setPosition((MyCar.Car_AnimatedMeshSceneNode->getPosition())+(MyCar.Car_Scale * (MyCar.Car_AnimatedMeshSceneNode->getJointNode("b_whee1")->getPosition())));
  

  wheelGeom=reinterpret_cast<CIrrOdeGeomSphere *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_SPHERE_NAME),sceneNode));
   wheelGeom->getSurfaceParameters(0)->setMu(1000.000000);
  wheelGeom->setMassTotal(0.010000);
  wheelGeom->drop();

  wheelMotor=reinterpret_cast<CIrrOdeMotor *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_MOTOR_NAME),carBody));
  wheelMotor->setJoint(wheelJointHinge);
  wheelMotor->setAutomaticCulling(EAC_BOX);
  wheelMotor->setAxis(1);
 wheelMotor->setForce(100.000000);
 wheelMotor->setVelocity(8.000000);
  wheelMotor->setEnabled(false);

  */

	}

	~Wheel()
	{
		sceneNode->remove();
	}
	/*vector3df rota(f32 rot, vector3df position)
	{//матрица вращения не используется
		
		f32 retvect[3] = {0};
		f32 rang[3] = {position.X,position.Y, position.Z};
		f32 matr[3][3];
		matr[0][0] = cos(( rot)/ 57.295785); matr[0][1] = 0; matr[0][2] = -sin(( rot)/ 57.295785);
		matr[1][0] = 0;						 matr[1][1] = 1; matr[1][2] = 0;
		matr[2][0] = sin(( rot)/ 57.295785); matr[2][1] = 0; matr[2][2] = cos(( rot)/ 57.295785);
			
		
		for(int i = 0; i < 3; i++)
		{
				for(int j = 0; j < 3; j++)
					retvect[i] += matr[i][j]*rang[j];
		}
		for(int i = 0; i < 3; i++)
			cout<<retvect[i]<<endl;
		cout<<endl;
		return vector3df (retvect[0],retvect[1],retvect[2]);
		
	}*/
	void Rotate(irr::core::vector3df carPosition,irr::core::vector3df rotate, f32 speed)
	{

		//wheelMotor->setForce(speed*10);
		//wheelMotor->setVelocity(speed);
		//wheelBody->setPosition(carPosition);
		/*
		vector3df newPos;

		newPos.X = carPosition.X + range.X*cos(( -rotate.Z)/ 57.295785) - range.Z*sin(( -rotate.Z)/ 57.295785);
		newPos.Z = carPosition.Z + range.X*sin(( -rotate.Z)/ 57.295785) + range.Z*cos(( -rotate.Z)/ 57.295785);
		newPos.Y = carPosition.Y + range.Y;		

		sceneNode->setPosition( newPos);
		*/
				//vector3df p = sceneNode->getPosition();
	//cout<<p.X<<" "<<p.Y<<" "<<p.Z<<endl;
		

	/*
		vector3df rot = sceneNode->getRotation() + vector3df(speed/4,0,0);
								
		
		if(cos(sceneNode->getRotation().Z/ 57.295785) == 1)
			rot.Y = rotate.Z;	
		else		
			rot.Y = -rotate.Z;
			
		
		sceneNode->setRotation(rot);
		*/


	}
};