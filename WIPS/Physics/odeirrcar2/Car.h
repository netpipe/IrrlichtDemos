#pragma once
#include "includes.h"
#include "MyEventReceiver.h"
#include "Wheel.h"
#define GT() dev->getTimer()->getTime() 

class Car
{									// физика машины совсем не соответствует правде и задается програмно (например отсутствует сила тяги двигателя и трения воздуха)
									// по этой же причине машина может стоя крутится на месте
protected:
public:
	core::vector3df rotation;		// вектор вращения машины во время руления(используется только для одной оси,
									//поэтому возможно создание не вектора а одной переменной)
	static const f32 SLOW_COEF;		// коеффициент замедления для скорости обновления величин = 0.01
									// т.к например обновление скорости происходит каждые 10 мс то для приведения величин
									//к нормальным в секунду (м/с) используется этот коефф
	static const f32 REAL_COEF;		// коеффициент для наглядности скорости (чтобы скорость перемещения машины примерно соотв. ее размеру

	char* name;						// имя машины нигде не используется
	f32 weight;						// вес машины будет нужен для физической детализации движения
	f32 acceleration;				//ускорение линейного движения в м/с2
	f32 brakeDeceleration;			//торможение в м/с2
	f32 collisionDamage;			// урон машины для изменения текстур на поврежденные (после 3-х столкновений)
	f32 speed;						// скорость машины
	const f32 maxSpeed;				// максимальная скорость машины будет например 120 км/ч или 35 м/с
	f32 steeringCoef;				// коеффициент руления - угол поворота руля в градусах
	u32 lastActionTime;				// не исользуется предполагалось будет
	f32 collisionCoef;				// коеффициент трения с дорогой будет нужен для физической детализации движения

	IAnimatedMesh* mesh;			// модель машины
	scene::ISceneNode *sceneNode;	// представитель сцены самой машины
	ITexture* texture;				// текстура машины
	
	CIrrOdeBody * carBody;                                       /*Здесь будит храниться Тело(Динамика)*/
	CIrrOdeGeomBox *carGeom;                                    /*Здесь будит храниться Геом-Каробка(Динамика)*/
	CIrrOdeWorld *worldNode;


	
	IrrlichtDevice* dev;			//укзатель на девайс для использования
	video::IVideoDriver* driver;	//для упрощения некоторых операций (чтобы не использовать dev->getVideoDriver() каждый раз
	scene::ISceneManager* smgr;		// для упрощения операций
	
	Wheel *wheels[4];

private:

public:
	MyEventReceiver* receiver;// не используется предполагалось использование
	Car(IrrlichtDevice* irrDevice,CIrrOdeWorld *world, const irr::io::path & modelFileName = "" ,const irr::io::path & textureFileName = "", MyEventReceiver* rec = 0);

	void SetMesh(const irr::io::path & modelFileName);// функция изменения модели (в случае когда модель автомобиля нужно будет заменит другой)

	void SetTexture(const irr::io::path & textureFileName );// изменение текстуры для текущей модели
	
	void OnCollision();				// для изменения текстур при столкновении и подсчета столкновений
	void setCameraToFollowing();	//выставляет камеру сцены на камеру следования за машиной
	void Car::ChangeCameraMode();	// изменяет камеру на привязанную к машине или нет
	virtual void Update();				//обновляет положение машины зависимо от ее скорости, угла и т.д.
	
	
	vector3df getPosition(){return sceneNode->getPosition();}
	bool IsCollision(ISceneNode* one)
	{
	 return (one->getTransformedBoundingBox().intersectsWithBox(this->sceneNode->getTransformedBoundingBox()));
	}
	~Car();
	
};

