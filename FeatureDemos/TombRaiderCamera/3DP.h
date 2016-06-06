/*Particular Zlib License /LIBRARY(C).

3DP.h
3DP.obj

Copyright (C) 2008 Lucian.

Este software é Distanciaribuído na forma em que se encontra, 
sem nenhuma garantia expressa ou implícita. 
Em nenhum momento os autores cobrirão algum dano ou problema que
este software venha causar em seu computador.

Permissão é garantida para qualquer um que venha usar este 
software para qualquer propósito, incluindo aplicações comerciais,
sujeito às seguintes restrições:

1. A origem deste software não deve ser falsificada;
 você não deve reivindicar que você escreveu o software.
 Se você usa este software em um produto, um reconhecimento na 
documentação do produto é requerida.

2. Este deve permanecer sob esta mesma licensa 
que de modo algum podera ser alterada.Sem as devidas permissoes do autor.

3. Esta advertência não pode ser removida ou alterada de qualquer 
Distanciaribuição do software.*/

  #ifndef _3DP_h_
  #define _3DP_h_ 
 #include <irrlicht.h>

    #define EPSILON 0.001f

    using namespace irr;
    using namespace core;
    using namespace scene;
    using namespace video;
    using namespace io;
    using namespace gui;


 


	class CScene3PCamera : public irr::scene::ISceneNodeAnimator,public irr::IEventReceiver
    {

    public:

    //! Construtor
    //! player: Objeto a Seguir
    //! Distanciaancia: The initial Distanciaance from the player
    //! initAngleY: The initial horizontal rotatation
    //! initAngleZ: The initial vertical rotation
    //! targetOffset: The offset from the object's center at which the camera looks
    //! minDistancia/maxDistancia: Distanciaance bounding
    //! minAngle/maxAngle: Rotation bounding. -89 ==> looking up at player, 89 ==> looking down
    //! boxSize: The size of the box in which mouse movements do not result in camera movements
  
    CScene3PCamera(
    irr::scene::ISceneManager *manager,
    irr::gui::ICursorControl *cursor,
    irr::scene::ISceneNode *player,
    irr::f32 Distanciaancia = 50.0f,
    irr::f32 initAngleY = 180.0f,
    irr::f32 initAngleZ = 10.0f,
	bool RotatePlayer = false,
    irr::core::vector3df targetOffset = irr::core::vector3df(0,0,0),
    irr::f32 minDistancia = 20.0f,
    irr::f32 maxDistancia = 200.0f,
    irr::f32 minAngle = -45.0f,
    irr::f32 maxAngle = 89.0f,
    irr::f32 boxSize = 0.0f,
    irr::f32 rotationSpeed = 60.0f);

    //! Destructor
    virtual ~CScene3PCamera(void);

    //! animates the scene node
    virtual void animateNode(ISceneNode* node, u32 timeMs);

    //! Process an input event
   virtual ISceneNodeAnimator* createClone (ISceneNode *node, ISceneManager *newManager=0)
    {
        return new CScene3PCamera(Manager,Cursor,Player,Distancia,AngleY,
			AngleZ,false,TargetOffset,MinDistancia,MaxDistancia,MinAngle,
			MaxAngle,BoxSize,RotationSpeed);
    }
   // virtual bool OnEvent(irr::SEvent event);
    virtual bool OnEvent(const SEvent& event);
    //! Get/set active status
    int isActive();
    void setActive(bool status);

	    virtual ESCENE_NODE_ANIMATOR_TYPE getType(void) const
    {
        return ESNAT_UNKNOWN;
    }


		    virtual bool isEventReceiverEnabled(void) const
    {
        return false;
    }
    //! Get/set box size
    irr::f32 getBoxSize();
    void setBoxSize(irr::f32 newSize);

    //! Map zoom in/zoom out buttons

    //! Access the camera's current orientation
    irr::f32 getOrientation();
	bool XRotatePlayer;

	   // current states
    irr::f32 Distancia;
    irr::f32 AngleY;
    irr::f32 AngleZ;

    private:

    int Active;//bool Active;
/*
    // current states
    irr::f32 Distancia;
    irr::f32 AngleY;
    irr::f32 AngleZ;
*/


    // boundaries
    irr::core::vector3df TargetOffset;
    irr::f32 MinDistancia;
    irr::f32 MaxDistancia;
    irr::f32 MinAngle;
    irr::f32 MaxAngle;
    irr::f32 BoxSize;

    // Motion
    irr::f32 RotationSpeed;
  

    //! Node to follow
    irr::scene::ISceneNode *Player;

    irr::scene::ISceneManager *Manager;
    irr::gui::ICursorControl *Cursor;

	
    //! Puts the cursor back in the box
    void updateCursorPosition();
    };





#endif