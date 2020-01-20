#include "Car.h"


	Car::Car(IrrlichtDevice* irrDevice,CIrrOdeWorld *world, const irr::io::path & modelFileName  ,const irr::io::path & textureFileName , MyEventReceiver* rec ) : maxSpeed(35)
	{
		worldNode = world;
		this->receiver = rec;
		dev = irrDevice;
		driver = dev->getVideoDriver();
		smgr = dev->getSceneManager();

		mesh = smgr->getMesh(modelFileName);			//выставляем модель из файла
		texture = driver->getTexture(textureFileName);	//выставляем текстуры из файла
		
				

	


		carBody = reinterpret_cast<CIrrOdeBody *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),worldNode));
		carBody->setPosition(vector3df(0,0,0));

		sceneNode = smgr->addAnimatedMeshSceneNode(mesh,carBody);
		
		
		sceneNode->setScale(sceneNode->getScale()*30.f);
		sceneNode->setMaterialTexture(0,texture);
		sceneNode->setMaterialFlag(video::EMF_LIGHTING, false);

		
	
    
		
		carGeom = reinterpret_cast<CIrrOdeGeomBox *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_BOX_NAME),sceneNode));
		carGeom->getSurfaceParameters(0)->setMu(1000);		
		//carGeom->setCollide(true);       
		//carGeom->setMassTotal(10);
		carGeom->drop();

			//***************ДОБАВЛЯЕМ КОЛЕСА*************************
			vector3df wheelsarr[4];// массив с расстояниями до колес
			/*
			wheelsarr[0] = vector3df(10.5,-6,18);		// правое переднее
			wheelsarr[1] = vector3df(-10,-6,18);			// левое переднее
			wheelsarr[2] = vector3df(10.5,-6,-16);		//левое заднее
			wheelsarr[3] = vector3df(-10,-6,-16);		//хм...
			*/

			wheelsarr[0] = vector3df(15.5,-6,25);		// правое переднее
			wheelsarr[1] = vector3df(-20,-6,30);			// левое переднее
			wheelsarr[2] = vector3df(20.5,-6,-26);		//левое заднее
			wheelsarr[3] = vector3df(-20,-6,-26);

			for(int i = 0; i < 4; i++)
			{
				wheels[i] = new Wheel(dev,carBody,sceneNode->getPosition(),wheelsarr[i]);
			}
		
			
		//************************************************************
		
		acceleration = 5.3;	//ускорение в м/с = 20 км/ч2 то есть до сотни за 5 секунд	
		speed = 0;
		brakeDeceleration = 14;
		collisionDamage = 0;
		
		steeringCoef = 25;// коеффициент поворота угла в секунду в градусах
		rotation.set(0,90,0);
		u32 lastActionTime = GT();		// не используется
		collisionCoef = 0.8;		//примерый коэфф трения сухого асфальта
	}

const f32 Car::SLOW_COEF = 0.01;
const f32 Car::REAL_COEF = 3;	
	void Car::SetMesh(const irr::io::path & modelFileName)
	{		
		
		mesh = smgr->getMesh(modelFileName);	// добавляем новую
		if(mesh)
		{
			sceneNode->remove();			//удаляем старую модель
			sceneNode = smgr->addAnimatedMeshSceneNode(mesh);	
		}

	}
	void Car::SetTexture(const irr::io::path & textureFileName )
	{		
		
		texture = driver->getTexture(textureFileName);			// выставляем новые текстуры
		if(texture)
		sceneNode->setMaterialTexture(0,texture);
	}
	void Car::OnCollision()
	{
		if(++collisionDamage > 3)		//после трех столкновений заменяем текстуры
			sceneNode->setMaterialTexture(0,driver->getTexture("../../media/car_05_damaged.psd"));// пока что так
	}

	void Car::Update()// вызывается все время в цикле для перерисовки машины
	{
		//cout<<speed*3.6<<"kmH"<<endl;
		//sceneNode->setRotation(vector3df(0,rotation.Z,0));
		/*
			vector3df nextPos = sceneNode->getPosition();
			//if( GT() - lastActionTime > 10)		//каждые 10 мс машинка тормозит от трения
				//speed = (speed/abs(speed))*abs((abs(speed)-collisionCoef*acceleration*SLOW_COEF));

			nextPos.X += REAL_COEF*SLOW_COEF*speed*sin(rotation.Z/ 57.295785);			
			nextPos.Z += REAL_COEF*SLOW_COEF*speed*cos(rotation.Z/ 57.295785);
			//carBody->setPosition(sceneNode->getPosition());
		//cout<<"np.x = "<<sin(rotation.Z/ 57.295785)<<endl;
		//	sceneNode->setPosition(nextPos);
			for(int i = 0; i < 4; i++)
				wheels[i]->Rotate(sceneNode->getPosition(), /*pow(-1.0,i+1)**///rotation, pow(-1.0,i)*speed);
									
		

	}

	Car::~Car()
	{

		sceneNode->remove();
		for(int i = 0; i < 4; i++)
				delete wheels[i];
		//mesh->drop();
	//	texture->drop();
		
		//sceneNode->drop();
		//delete [] name;
	}
