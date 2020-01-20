
#include "Litha.h"
#include "Level.h"
#include "RotateToAnimator.h"

class AlphaOnAnimator : public IMotionAnimator
{
	IUpdater *localUpdater;
	IMesh *mesh;
	u8 onAlpha;
	f32 alpha;
	
	f32 fadeSpeed;
	bool fadingOn;
	
	void SetMeshAlpha(u8 alpha)
	{
		ASSERT(mesh);
		scene::IMeshManipulator *mm = GetEngine()->GetIrrlichtDevice()->getSceneManager()->getMeshManipulator();
		mm->setVertexColorAlpha( mesh->GetIrrlichtNode()->getMesh(), alpha );
	}
	
public:
	// onAlpha is the maximum alpha value, when reached AlphaOn has finished.
	AlphaOnAnimator(IUpdater *localUpdater, u8 onAlpha)
		: localUpdater(localUpdater), onAlpha(onAlpha)
	{
		mesh = NULL;
		fadingOn = false;
		
		fadeSpeed = 256.f * 0.5f;
	}
	
	void FadeOn()
	{
		fadingOn = true;
	}
	
	void Reset()
	{
		SetMeshAlpha(0);
		alpha = 0.f;
		fadingOn = false;
	}
	
	void Init(ITransformable *target)
	{
		ASSERT( mesh = dynamic_cast<IMesh *>(target) );
	}
	
	void Animate(ITransformable *target, f32 dt)
	{
		if (fadingOn)
		{
			if (alpha < onAlpha)
			{
				alpha += fadeSpeed*dt;
				SetMeshAlpha( alpha > 0.f ? u8(alpha) : 0 );
			}
		}
	}
};


class GridBasedCharacterController : public ICharacterController
{
	IWorld *world;
	
	int idButtonWalkForwards;
	int idButtonWalkBackwards;
	int idButtonWalkLeft;
	int idButtonWalkRight;
	
	ITransformable *moveRelativeTo;
	
	Level *level;
	
	// cached button states
	struct
	{
		bool forwards;
		bool backwards;
		bool left;
		bool right;
	} buttonDownCache;
	
	bool isMovingIntoLocation;
	f32 finishEnteringTime;
	
	IMesh *gridArrowMesh;
	RotateToAnimator *rotateAnim;
	AlphaOnAnimator *alphaAnim;
	
public:

	GridBasedCharacterController(IWorld *world,
			int idButtonWalkForwards,int idButtonWalkBackwards, int idButtonWalkLeft, int idButtonWalkRight,
			ITransformable *moveRelativeTo,
			Level *level)
	{
		this->world = world;
		
		this->idButtonWalkForwards	= idButtonWalkForwards;
		this->idButtonWalkBackwards	= idButtonWalkBackwards;
		this->idButtonWalkLeft		= idButtonWalkLeft;
		this->idButtonWalkRight		= idButtonWalkRight;
		
		this->moveRelativeTo = moveRelativeTo;
		
		this->level = level;
		
		isMovingIntoLocation = false;
		
		gridArrowMesh = world->AddMesh("gridarrow.irrmesh");
		level->ApplyPlainAlphaShaders( gridArrowMesh, 100 );
		gridArrowMesh->SetVisible(false);
		
		rotateAnim = new RotateToAnimator(0);
		gridArrowMesh->AddAnimator(rotateAnim);
		alphaAnim = new AlphaOnAnimator(&level->GetUpdater(), 100);
		gridArrowMesh->AddAnimator(alphaAnim);
		alphaAnim->Reset();
	}
	
	~GridBasedCharacterController()
	{
		world->RemoveTransformable(gridArrowMesh);
		rotateAnim->drop();
		alphaAnim->drop();
	}
	
	void EndMove()
	{
		isMovingIntoLocation = false;
	}
	
	void Update(ICharacter *character)
	{
		// *camera* is facing (near) a move direction?
		bool atMoveAngle = false;
		
		
		if (moveRelativeTo)
		{
			core::vector3df relativeToRot = moveRelativeTo->GetRotation();
			core::vector2df moveVec = maths::get_horizontal_vec(relativeToRot);
			
			// Find if move direction is within a certain angle difference from any compass direction.
			
			const f32 maxVecDiff = 0.3f;
			
			core::vector2df rotVec = maths::get_horizontal_vec(relativeToRot);
			
			atMoveAngle = true;/*
					rotVec.getDistanceFrom(core::vector2df(1,0)) < maxVecDiff
					|| rotVec.getDistanceFrom(core::vector2df(-1,0)) < maxVecDiff
					|| rotVec.getDistanceFrom(core::vector2df(0,1)) < maxVecDiff
					|| rotVec.getDistanceFrom(core::vector2df(0,-1)) < maxVecDiff;*/
			
			
			// Confine movement to only North, East, South, or West movement.
			
			if ( fabs(moveVec.X) > fabs(moveVec.Y) )
			{
				moveVec.Y = 0.f;
				moveVec.normalize();
			}
			else
			{
				moveVec.X = 0.f;
				moveVec.normalize();
			}
			
			character->SetMoveVec( moveVec );
		}
		
		
		IInputProfile *inputProfile = world->GetInputProfile();
		
		if (!inputProfile)
			return;
		
		
		
		bool playerInputMoved = inputProfile->IsButtonDown(idButtonWalkForwards)
				|| inputProfile->IsButtonDown(idButtonWalkBackwards)
				|| inputProfile->IsButtonDown(idButtonWalkLeft)
				|| inputProfile->IsButtonDown(idButtonWalkRight);
		
		
		
		// Move detected? Continue moving for some time.
		if (playerInputMoved && !isMovingIntoLocation && inputProfile
				&& character->GetAttemptedMove().getLengthSQ() > 0.001f
				&& atMoveAngle)
		{
			// Cache button states
			// Move is continued automatically until the centre of the location is reached.
			buttonDownCache.forwards = inputProfile->IsButtonDown(idButtonWalkForwards);
			buttonDownCache.backwards = inputProfile->IsButtonDown(idButtonWalkBackwards);
			buttonDownCache.left = inputProfile->IsButtonDown(idButtonWalkLeft);
			buttonDownCache.right = inputProfile->IsButtonDown(idButtonWalkRight);
			
			// Find centre of location we are destined to enter, that is not the current location.
			core::vector3df actualMove = character->GetAttemptedMove();
			core::vector3df destPos = character->GetPosition() + actualMove;
			core::vector3di destMapCoord = level->GetCoord(destPos);
			core::vector3df destPosCentred( destMapCoord.X, destMapCoord.Y, destMapCoord.Z );
			
			// Distance from player to dest pos
			f32 continueDist = character->GetPosition().getDistanceFrom(destPosCentred);
			
			// Scale factor of time it takes to move one unit.
			// This is chosen experimentally...
			// It's dependent on player speed, but player has acceleration etc so this is only
			// an approximation.
			const f32 scaleFactor = 0.3f;
			
			finishEnteringTime = level->GetVirtualTime() + continueDist * scaleFactor;
			
			// Keep moving
			isMovingIntoLocation = true;
		}
		
		
		// show grid direction arrow?
		
		if (moveRelativeTo)
		{
			core::vector3di playerMapPos = level->GetCoord( character->GetPosition() );
			
			bool showGridArrow = !playerInputMoved && !isMovingIntoLocation
					&& (level->GetMap()->GetObject(playerMapPos) && !level->GetMap()->IsTraversing(playerMapPos));
			
			gridArrowMesh->SetVisible( showGridArrow );
			
			if (showGridArrow)
			{
				// What is the grid location camera is looking at?
				
				core::vector3df playerPosCentred = level->GetPosFromCoord(playerMapPos);
				
				core::vector3df arrowPos = playerPosCentred
						+ core::vector3df(0.f,-0.475,0.f);
				
				gridArrowMesh->SetPosition(arrowPos);
				
				/*
				gridArrowMesh->SetRotation(core::vector3df(0.f,
						maths::rotation_required_2d(character->GetMoveVec()),
						0.f));
				*/
				
				rotateAnim->SetFinalY( maths::rotation_required_2d(character->GetMoveVec()) );
				
				gridArrowMesh->ApplyTransformNow();
				
				alphaAnim->FadeOn();
			}
			else
			{
				alphaAnim->Reset();
			}
		}
		
		
		// Start with no motion...
		
		character->ClearMotion();
		
		
		if (isMovingIntoLocation)
		{
			// Moving into location. Just entered a location, so continue moving automatically for a small amount
			// of time (so player only ever stops at the centre of a location).
			
			// We set motion in the same manner as below, but using cached button states.
			
			// Maybe set medial motion
			
			if (buttonDownCache.forwards
					&& !buttonDownCache.backwards)
			{
				character->SetMedialMotion(1);
			}
			
			if (buttonDownCache.backwards
					&& !buttonDownCache.forwards)
			{
				character->SetMedialMotion(-1);
			}
			
			// Maybe set lateral motion
			
			if (buttonDownCache.left
					&& !buttonDownCache.right)
			{
				character->SetLateralMotion(-1);
			}
			
			if (buttonDownCache.right
					&& !buttonDownCache.left)
			{
				character->SetLateralMotion(1);
			}
			
			// Then check if time is actually over?
			if (level->GetVirtualTime() >= finishEnteringTime)
			{
				// Return to normal movement in later updates.
				isMovingIntoLocation = false;
			}
		}
		else if (atMoveAngle)
		{
			// Normal movement
			
			// Maybe set medial motion
			
			if (inputProfile->IsButtonDown(idButtonWalkForwards)
					&& !inputProfile->IsButtonDown(idButtonWalkBackwards))
			{
				character->SetMedialMotion(1);
			}
			
			if (inputProfile->IsButtonDown(idButtonWalkBackwards)
					&& !inputProfile->IsButtonDown(idButtonWalkForwards))
			{
				character->SetMedialMotion(-1);
			}
			
			// Maybe set lateral motion
			
			if (inputProfile->IsButtonDown(idButtonWalkLeft)
					&& !inputProfile->IsButtonDown(idButtonWalkRight))
			{
				character->SetLateralMotion(-1);
			}
			
			if (inputProfile->IsButtonDown(idButtonWalkRight)
					&& !inputProfile->IsButtonDown(idButtonWalkLeft))
			{
				character->SetLateralMotion(1);
			}
		}
	}
};

