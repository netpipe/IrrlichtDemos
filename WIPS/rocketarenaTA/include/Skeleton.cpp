/*

Skeletal human animation class - cpp-file

*/

#include "Skeleton.h"

// constructor
cSkeleton::cSkeleton() {
}

// destructor
cSkeleton::~cSkeleton() {
}

// Initialisation
void cSkeleton::Initialize(scene::IAnimatedMeshSceneNode *node, u32 setSpeed) {
	
	skeletonNode=node;
	
	if (!skeletonNode)
		return;

	i=360;
	k=180;
	a_temp=0;
	endup=false;
	animType = lastAnimType = CSK_ANIM_STAND;
	speed=setSpeed;
	
	for (u32 n=0; n < SKT_BONE_COUNT; n++) {
		bonesOsci.push_back(core::vector3df(0,0,0));
		bonesOsciPos.push_back(core::vector3df(0,0,0));
		bonesAnimRunOsci.push_back(core::vector3df(0,0,0));
	}
	
	// save bone names in boneName array
	for (int n=0; n < SKT_BONE_COUNT; n++) {

		switch (n) {
		// main items
		case SKT_BONE_TORSO:
			boneName.push_back((irr::c8*) "pelvis");
			break;
		case SKT_BONE_STOMACH:
			boneName.push_back((irr::c8*) "stomach");
			break;
		case SKT_BONE_BODY:
			boneName.push_back((irr::c8*) "body");
			break;
		case SKT_BONE_NECK:
			boneName.push_back((irr::c8*) "neck");
			break;
		case SKT_BONE_HEAD:
			boneName.push_back((irr::c8*) "head");
			break;
			
		// right arm items
		case SKT_BONE_ARM_R:
			boneName.push_back((irr::c8*) "upperArmL");
			break;
		case SKT_BONE_UARM_R:
			boneName.push_back((irr::c8*) "underArmL");
			break;
		case SKT_BONE_HAND_R:
			boneName.push_back((irr::c8*) "handL");
			break;
		case SKT_BONE_FINGER_R:
			boneName.push_back((irr::c8*) "fingerL");
			break;
					
		// left arm items
		case SKT_BONE_ARM_L:
			boneName.push_back((irr::c8*) "upperArmR");
			break;
		case SKT_BONE_UARM_L:
			boneName.push_back((irr::c8*) "underArmR");
			break;
		case SKT_BONE_HAND_L:
			boneName.push_back((irr::c8*) "handR");
			break;
		case SKT_BONE_FINGER_L:
			boneName.push_back((irr::c8*) "fingerR");
			break;
			
		// right leg items
		case SKT_BONE_LEG_R:
			boneName.push_back((irr::c8*) "legL");
			break;
		case SKT_BONE_SHIN_R:
			boneName.push_back((irr::c8*) "shinL");
			break;
		case SKT_BONE_FEET_R:
			boneName.push_back((irr::c8*) "feetL");
			break;
		case SKT_BONE_TOES_R:
			boneName.push_back((irr::c8*) "toesL");
			break;
		
		// left leg items
		case SKT_BONE_LEG_L:
			boneName.push_back((irr::c8*) "legR");
			break;
		case SKT_BONE_SHIN_L:
			boneName.push_back((irr::c8*) "shinR");
			break;
		case SKT_BONE_FEET_L:
			boneName.push_back((irr::c8*) "feetR");
			break;
		case SKT_BONE_TOES_L:
			boneName.push_back((irr::c8*) "toesR");
			break;
		}

	}

	// loading model


	if (skeletonNode) {
		skeletonNode->setFrameLoop( 0,0 );
		skeletonNode->setJointMode( irr::scene::EJUOR_CONTROL );
		
		// get all bone scene nodes
		for (int n=0; n < SKT_BONE_COUNT; n++) {
			bones.push_back(skeletonNode->getJointNode(boneName[n]));
		}

		// set player in stand position
		bones[SKT_BONE_ARM_R]->setRotation( vector3df(0,0,-22) );
		
		core::vector3df vec;
		vec = matrixRotation(bones[SKT_BONE_ARM_R]->getRotation(), core::vector3df(0,0,-144));
		bones[SKT_BONE_ARM_R]->setRotation(vec);
		
		vec = matrixRotation(bones[SKT_BONE_UARM_R]->getRotation(), core::vector3df(8,0,-10));
		bones[SKT_BONE_UARM_R]->setRotation(vec);

		vec = matrixRotation(bones[SKT_BONE_ARM_L]->getRotation(), core::vector3df(0,0,22));
		bones[SKT_BONE_ARM_L]->setRotation(vec);
		
		vec = matrixRotation(bones[SKT_BONE_UARM_L]->getRotation(), core::vector3df(8,0,10));
		bones[SKT_BONE_UARM_L]->setRotation(vec);
		
		// store all current bone animation for stand position
		bonesRot.clear();
		for (int n=0; n<SKT_BONE_COUNT; n++) {
			bonesRot.push_back( bones[n]->getRotation() );
			bonesPos.push_back( bones[n]->getPosition() );
		}
		
		for (int n=1; n<CSK_ANIM_COUNT; n++)
			startMode.push_back(true);
			
		for (int n=1; n<CSK_ANIM_COUNT; n++)
			normalise.push_back(false);
			
		startMode[CSK_ANIM_STAND] = false;
		startMode[CSK_ANIM_JUMP] = false;

	} 

}

// set the type of animation
void cSkeleton::setAnimType(CSK_ANIM setAnimType) {
	if (animType != CSK_ANIM_JUMP)
		animType = setAnimType;
		
	for (int n=1; n<CSK_ANIM_COUNT; n++)
		normalise[n] = false;
}

// get the current animation type
CSK_ANIM cSkeleton::getAnimType() const {
	return animType;
}

// animates the skeleton
void cSkeleton::animSkeleton() {
	
	u32 a = i%360;
	u32 b = (i-180)%360;

	f32 rad = 3.14159/180;

	f32 const alpha = a * rad;
	f32 const beta = b * rad;
	f32 const alphaTwo = 2 * alpha;
	f32 const betaTwo = 2 * beta;

	// oscillation functions
	f32 const sinAlpha = sin(alpha);
	f32 const sinBeta = sin(beta);
	f32 const sinAlphaTwo = sin(alphaTwo);
	f32 const cosAlphaTwo = cos(alphaTwo);
	f32 const sinBetaTwo = sin(betaTwo);
	f32 const cosAlpha = cos(alpha);
	f32 const cosBeta = cos(beta);

	f32 dryAbrasion;
	
	if (animType==CSK_ANIM_WALK && lastAnimType==CSK_ANIM_RUN && !endup) {
		a_temp=a;
		k=0;
		endup=true;
	}

	// setting variables for normalising bones
	if  (animType == CSK_ANIM_STAND && startMode[CSK_ANIM_STAND]) {
		
		// normalising from run mode
		if (lastAnimType == CSK_ANIM_RUN) {

			animType = CSK_ANIM_RUN;
			normalise[CSK_ANIM_RUN] = true;
			startMode[CSK_ANIM_RUN] = true;
			
		// normalising from walk mode
		} else {
			
			animType = CSK_ANIM_WALK;
			normalise[CSK_ANIM_WALK] = true;
			startMode[CSK_ANIM_WALK] = true;
			
		}
	}

	// this path setting variables for blending animation from run to walk
	if (animType==CSK_ANIM_WALK && lastAnimType==CSK_ANIM_RUN && k<180) {

		animType = CSK_ANIM_RUN;
		normalise[CSK_ANIM_RUN] = true;
		startMode[CSK_ANIM_RUN] = true;

		if (k > 160) {
			lastAnimType=CSK_ANIM_WALK;
			animType = CSK_ANIM_WALK;
			normalise[CSK_ANIM_WALK] = false;
			startMode[CSK_ANIM_WALK] = true;
		}
		
	} else
		endup=false;

	switch(animType) {

	// animation for standing --------------------------------------------------
	case CSK_ANIM_STAND:

		if (!startMode[CSK_ANIM_STAND]) {

			bonesOsci[SKT_BONE_ARM_R] = core::vector3df( 1.6f * sinAlpha , 0, 1.6f * sinAlphaTwo);
			bonesOsci[SKT_BONE_ARM_L] = core::vector3df( 1.5f * sinAlphaTwo , 0, 1.5f * sinAlpha);
		
			bonesOsci[SKT_BONE_BODY] = core::vector3df( .8f * sinBeta, 0, .8f * sinAlphaTwo);
			bonesOsci[SKT_BONE_NECK] = core::vector3df( 1.6f * sinAlpha, 0, 1.6f * sinBetaTwo );
			bonesOsci[SKT_BONE_HEAD] = core::vector3df( 1.3f * cosAlphaTwo, 0, 1.3f * sinBeta);

			bonesOsci[SKT_BONE_UARM_R] = core::vector3df( 1.1f * sinAlpha, 0, 1.1f * cosAlphaTwo );
			bonesOsci[SKT_BONE_UARM_L] = core::vector3df( 1.1f * sinBeta, 0, 1.1f * cosAlphaTwo );
			
			// setting bones to zero which are not needed
			bonesOsci[SKT_BONE_LEG_R] = core::vector3df(0,0,0);
			bonesOsci[SKT_BONE_LEG_L] = core::vector3df(0,0,0);
			bonesOsci[SKT_BONE_SHIN_R] = core::vector3df(0,0,0);
			bonesOsci[SKT_BONE_SHIN_L] = core::vector3df(0,0,0);
			bonesOsci[SKT_BONE_HAND_L] = core::vector3df(0,0,0);
			bonesOsci[SKT_BONE_HAND_R] = core::vector3df(0,0,0);
			bonesOsci[SKT_BONE_FINGER_L] = core::vector3df(0,0,0);
			bonesOsci[SKT_BONE_FINGER_R] = core::vector3df(0,0,0);

			k=180;
			i+=1;

			lastAnimType=CSK_ANIM_STAND;			

		}

		break;
		
	// animation for jumping --------------------------------------------------
	case CSK_ANIM_JUMP:

		if (startMode[CSK_ANIM_JUMP]) {
			
			if (a==180 || a==0) {
				startMode[CSK_ANIM_JUMP] = false;
				animType=CSK_ANIM_STAND;
				i=i-speed-speed;
				k=0;
			}

		} else {
			(a<=180) ? (i=180) : (i=360);
			startMode[CSK_ANIM_JUMP] = true;
			startMode[CSK_ANIM_RUN] = false;
		}

	// animation for running ---------------------------------------------------
	case CSK_ANIM_RUN:

		if (startMode[CSK_ANIM_RUN]) {
			
			if (normalise[CSK_ANIM_RUN]) {
				
				dryAbrasion = 1.f - k/180.f;

				k += speed;

				if (k>180) {
					startMode[CSK_ANIM_STAND] = false;
					normalise[CSK_ANIM_RUN] = false;
					i=360;
				}
				startMode[CSK_ANIM_RUN] = true;
				
			} else {

				// blending from walk
				if (lastAnimType==CSK_ANIM_WALK)
					k=90;

				startMode[CSK_ANIM_STAND] = true;
				dryAbrasion = 1.f - k/180.f;
				(k<0) ? (startMode[CSK_ANIM_RUN]=false) : (k-=speed);
					
			}

		} else {
			startMode[CSK_ANIM_STAND] = true;
			dryAbrasion = 1.f;
		}
		
		f32 amp;
		f32 multa,multb;

		// increase amplitude of legs through jump cycle
		if (animType==CSK_ANIM_JUMP) {
			amp=60.f;
			
			if (a>180) {
				multa=1.3f;
				multb=2.f;
			} else {
				multa=2.f;
				multb=1.3f;
			}
		} else {
			amp=80.f;
			multa=multb=1.f;
		}

		// LEGS
		bonesOsci[SKT_BONE_LEG_R] = core::vector3df( multb*-dryAbrasion * 46.f * sinAlpha , dryAbrasion*5.f*sinAlpha ,0);
		bonesOsci[SKT_BONE_LEG_L] = core::vector3df( multa*-dryAbrasion * 46.f * sinBeta , dryAbrasion*5.f*sinAlpha ,0);

		// SHINS
		if (a>90 && a<270) {
			bonesOsci[SKT_BONE_SHIN_L] = core::vector3df(dryAbrasion * (amp/2.f*cos(alphaTwo)-amp/2.f) ,0,0);
			bonesOsci[SKT_BONE_SHIN_R] = core::vector3df(-amp*dryAbrasion ,0,0);
		} else {
			bonesOsci[SKT_BONE_SHIN_L] = core::vector3df(-amp*dryAbrasion ,0,0);
			bonesOsci[SKT_BONE_SHIN_R] = core::vector3df(dryAbrasion * (amp/2.f*cos(betaTwo)-amp/2.f) ,0,0);
		}

		// NECK BODY HEAD
		bonesOsci[SKT_BONE_BODY] = core::vector3df(3.f * dryAbrasion, dryAbrasion * 13.f*sinBeta, 0);
		bonesOsci[SKT_BONE_NECK] = core::vector3df(-10.f * dryAbrasion, dryAbrasion * 7.f*sinAlpha, 0);
		bonesOsci[SKT_BONE_HEAD] = core::vector3df( dryAbrasion*1.5f*sinAlpha ,0,dryAbrasion*2.f*sinAlphaTwo);

		// ARMS
		bonesOsci[SKT_BONE_ARM_L] = core::vector3df( -dryAbrasion * (35.f*sinAlpha + 10.f) , 0,dryAbrasion*10.f*sinAlpha);
		bonesOsci[SKT_BONE_ARM_R] = core::vector3df( -dryAbrasion * (35.f*sinBeta + 10.f) ,0,dryAbrasion*10.f*sinAlpha);
		bonesOsciPos[SKT_BONE_ARM_L] = core::vector3df(0,-dryAbrasion*.035f*sinAlpha,-dryAbrasion*.035f*cosAlpha);
		bonesOsciPos[SKT_BONE_ARM_R] = core::vector3df(0,-dryAbrasion*.035f*sinBeta,-dryAbrasion*.035f*cosBeta);

		// UNDER ARMS
		bonesOsci[SKT_BONE_UARM_R] = core::vector3df( dryAbrasion * (30.f*sinAlpha + 50.f) ,0,dryAbrasion*10.f*sinBeta);
		bonesOsci[SKT_BONE_UARM_L] = core::vector3df( dryAbrasion * (30.f*sinBeta + 50.f) ,0,dryAbrasion*10.f*sinBeta);
			
		// MAIN ITEMS
		bonesOsci[SKT_BONE_TORSO] = core::vector3df(0, dryAbrasion * 15.f*sinAlpha , dryAbrasion*1.f*sinAlpha);
		bonesOsci[SKT_BONE_STOMACH] = core::vector3df(12.f * dryAbrasion, dryAbrasion * 15.f*sinBeta , 0);

		bonesOsci[SKT_BONE_FINGER_R] = core::vector3df(0,0,0);
		bonesOsci[SKT_BONE_FINGER_L] = core::vector3df(0,0,0);

		if (a < 172 || b < 172) {
			bonesOsciPos[SKT_BONE_TORSO] = core::vector3df(0,dryAbrasion*.1f*sinAlphaTwo,0);
			bonesOsciPos[SKT_BONE_BODY] = core::vector3df(0,dryAbrasion*.03f*sinAlphaTwo,0);
			bonesOsciPos[SKT_BONE_HEAD] = core::vector3df(0,-dryAbrasion*.03f*sinAlphaTwo,0);
		}

		startMode[CSK_ANIM_WALK]=true;

		// reduce the speed of jump cycle
		if (animType==CSK_ANIM_JUMP)
			i+= (u32) speed/2;
		else
			i+=speed;
		
		a_temp=a;
		
		lastAnimType = CSK_ANIM_RUN;

		break;
		
	// animation for walking ---------------------------------------------------
	case CSK_ANIM_WALK:

		if (startMode[CSK_ANIM_WALK]) {

			if (normalise[CSK_ANIM_WALK]) {

				dryAbrasion = 1 - k/180.f;
				k+=speed;

				if (k>180) {
					startMode[CSK_ANIM_STAND] = false;
					normalise[CSK_ANIM_WALK] = false;
					i=360;
				}
				startMode[CSK_ANIM_WALK] = true;

			} else {

				dryAbrasion = 1.f-k/180.f;
				startMode[CSK_ANIM_STAND] = true;
				(k<0) ? (startMode[CSK_ANIM_WALK]=false) : (k-=speed);

			}

		} else {
			startMode[CSK_ANIM_STAND] = true;
			dryAbrasion = 1.f;
		}

		// LEGS
		bonesOsci[SKT_BONE_LEG_R] = core::vector3df( -dryAbrasion*(28.f*sinAlpha + 0.f) , dryAbrasion*5.f*sinAlpha,0);
		bonesOsci[SKT_BONE_LEG_L] = core::vector3df( -dryAbrasion*(28.f*sinBeta + 0.f) , dryAbrasion*5.f*sinAlpha,0);
		
		// SHINS
		if (a>270 || a<90) {
			bonesOsci[SKT_BONE_SHIN_L] = core::vector3df( -dryAbrasion*60.f*cosAlpha ,0,0);
			bonesOsci[SKT_BONE_SHIN_R] = core::vector3df( -dryAbrasion*10.f*cosAlpha,0,0);
		} else {
			bonesOsci[SKT_BONE_SHIN_L] = core::vector3df( -dryAbrasion*10.f*cosBeta,0,0 );
			bonesOsci[SKT_BONE_SHIN_R] = core::vector3df( -dryAbrasion*60.f*cosBeta ,0,0);
		}
		
		// ARMS
		bonesOsci[SKT_BONE_ARM_R] = core::vector3df( -dryAbrasion*(15.f*sinBeta + 5.f) ,0,0);
		bonesOsci[SKT_BONE_ARM_L] = core::vector3df( -dryAbrasion*(15.f*sinAlpha + 5.f) ,0,0);
		
		// UNDER ARMS
		bonesOsci[SKT_BONE_UARM_R] = core::vector3df( dryAbrasion*(11.f*sinAlpha + 15.f) ,0,0);
		bonesOsci[SKT_BONE_UARM_L] = core::vector3df( dryAbrasion*(11.f*sinBeta + 15.f) ,0,0);
		
		// TORSO
		bonesOsci[SKT_BONE_TORSO] = core::vector3df(0, dryAbrasion*8.f*sinAlpha ,0);
		
		// BODY NECK HEAD
		bonesOsci[SKT_BONE_BODY] = core::vector3df(dryAbrasion*6.f, dryAbrasion*15.f*sinBeta ,0);
		bonesOsci[SKT_BONE_HEAD] = core::vector3df( 0, dryAbrasion * 2.f *sinAlpha ,0);
		
		if (a < 172 || b < 172) {
			bonesOsciPos[SKT_BONE_TORSO] = core::vector3df(0,dryAbrasion*.06f*sinAlphaTwo,0);
			bonesOsciPos[SKT_BONE_BODY] = core::vector3df(0,dryAbrasion*.01f*sinAlphaTwo,0);
			bonesOsciPos[SKT_BONE_HEAD] = core::vector3df(0,-dryAbrasion*.01f*sinAlphaTwo,0);
		}
		
		startMode[CSK_ANIM_RUN] = true;
		
		lastAnimType = CSK_ANIM_WALK;
		
		i+=speed;
		
		break;
		
	// animation for player turn ---------------------------------------------
	case CSK_ANIM_TURN:
		
		dryAbrasion = 1.f;
		
/*		// LEGS
		bonesOsci[SKT_BONE_LEG_R] = core::vector3df( -dryAbrasion*8.f*sinAlpha ,0,0);
		bonesOsci[SKT_BONE_LEG_L] = core::vector3df( -dryAbrasion*8.f*sinBeta ,0,0);
		
		bonesOsci[SKT_BONE_SHIN_L] = core::vector3df( -dryAbrasion*(10.f*sinAlpha + 10.f) ,0,0);
		bonesOsci[SKT_BONE_SHIN_R] = core::vector3df( -dryAbrasion*(10.f*sinBeta + 10.f) ,0,0);
*/		

		i+=speed;
		
		break;
		
	// animation for rocket ---------------------------------------------------
	case CSK_ANIM_ROCKET:
		
		bonesOsci[SKT_BONE_LEG_R] = core::vector3df(65,-35,70);
		bonesOsci[SKT_BONE_LEG_L] = core::vector3df(65,35,-70);
		
		bonesOsci[SKT_BONE_SHIN_R] = core::vector3df(-60,10,-65);
		bonesOsci[SKT_BONE_SHIN_L] = core::vector3df(-60,-10,65);

		bonesOsci[SKT_BONE_FEET_R] = core::vector3df(-10,0,50);
		bonesOsci[SKT_BONE_FEET_L] = core::vector3df(-10,0,-50);
		
		bonesOsci[SKT_BONE_TORSO] = core::vector3df(10,0,0);
		bonesOsci[SKT_BONE_STOMACH] = core::vector3df(35,0,0);
		bonesOsci[SKT_BONE_BODY] = core::vector3df(-5,0,0);
		bonesOsci[SKT_BONE_NECK] = core::vector3df(-15,0,0);
		bonesOsci[SKT_BONE_HEAD] = core::vector3df(-15,0,0);

		bonesOsci[SKT_BONE_ARM_R] = core::vector3df(85,25,-10);
		bonesOsci[SKT_BONE_ARM_L] = core::vector3df(87,-26,10);
		bonesOsci[SKT_BONE_UARM_R] = core::vector3df(10,60,10);
		bonesOsci[SKT_BONE_UARM_L] = core::vector3df(10,-60,-10);
		
		bonesOsci[SKT_BONE_FINGER_R] = core::vector3df(0,0,-110);
		bonesOsci[SKT_BONE_FINGER_L] = core::vector3df(0,0,110);
		
		i=360;
		k=180;
		
		break;
		
	// animation for jump off the rocket -------------------------
	case CSK_ANIM_JUMP_OFF_ROCKET:
		
		f32 f = .5f*(cosAlpha+1.f);
		
		bonesOsci[SKT_BONE_TORSO] = core::vector3df(20.f*(sinAlpha+1.f),0,0);
		bonesOsciPos[SKT_BONE_TORSO] = core::vector3df(0,1.f*sinAlpha,0);
		
		bonesOsci[SKT_BONE_ARM_R] = core::vector3df(20.f*(sinAlpha+1.f)+85.f,25,-10);
		bonesOsci[SKT_BONE_ARM_L] = core::vector3df(20.f*(sinAlpha+1.f)+87.f,-26,10);

		bonesOsci[SKT_BONE_LEG_R] = core::vector3df(65*f,-5*f,70*f);
		bonesOsci[SKT_BONE_LEG_L] = core::vector3df(65*f,5*f,-70*f);
		
		bonesOsci[SKT_BONE_SHIN_R] = core::vector3df(-10*f,10*f,-45*f);
		bonesOsci[SKT_BONE_SHIN_L] = core::vector3df(-10*f,-10*f,45*f);
			
		bonesOsci[SKT_BONE_FEET_R] = core::vector3df(-10*f,0,50*f);
		bonesOsci[SKT_BONE_FEET_L] = core::vector3df(-10*f,0,-50*f);
		
		// ARMS
		bonesAnimRunOsci[SKT_BONE_ARM_L] = core::vector3df( -10.f,0,0);
		bonesAnimRunOsci[SKT_BONE_ARM_R] = core::vector3df( -10.f,0,0);

		// UNDER ARMS
		bonesAnimRunOsci[SKT_BONE_UARM_R] = core::vector3df( 50.f ,0,0);
		bonesAnimRunOsci[SKT_BONE_UARM_L] = core::vector3df( 50.f ,0,0);
			
		// MAIN ITEMS
		bonesAnimRunOsci[SKT_BONE_TORSO] = core::vector3df(0,0,0);
		bonesAnimRunOsci[SKT_BONE_STOMACH] = core::vector3df(12.f,0, 0);
			
		// NECK BODY HEAD
		bonesAnimRunOsci[SKT_BONE_BODY] = core::vector3df(3.f, 0, 0);
		bonesAnimRunOsci[SKT_BONE_NECK] = core::vector3df(-10.f,0, 0);
		bonesAnimRunOsci[SKT_BONE_HEAD] = core::vector3df( 0,0,2.f);

		f32 dec = (f32) a/180.f;

		bonesOsci[SKT_BONE_SHIN_R] -= bonesOsci[SKT_BONE_SHIN_R] * dec;
		bonesOsci[SKT_BONE_SHIN_R] += core::vector3df(-10.f,0,0) * a/180.f;
		bonesOsci[SKT_BONE_SHIN_L] -= bonesOsci[SKT_BONE_SHIN_L] * dec;
		bonesOsci[SKT_BONE_SHIN_L] += core::vector3df(-60.f,0,0) * a/180.f;

		bonesOsci[SKT_BONE_ARM_L] -= bonesOsci[SKT_BONE_ARM_L] * dec;
		bonesOsci[SKT_BONE_ARM_L] += bonesAnimRunOsci[SKT_BONE_ARM_L] * a/180.f;
			
		bonesOsci[SKT_BONE_ARM_R] -= bonesOsci[SKT_BONE_ARM_R] * dec;
		bonesOsci[SKT_BONE_ARM_R] += bonesAnimRunOsci[SKT_BONE_ARM_R] * a/180.f;
			
		bonesOsci[SKT_BONE_UARM_R] -= bonesOsci[SKT_BONE_UARM_R] * dec;
		bonesOsci[SKT_BONE_UARM_R] += bonesAnimRunOsci[SKT_BONE_UARM_R] * a/180.f;

		bonesOsci[SKT_BONE_UARM_L] -= bonesOsci[SKT_BONE_UARM_L] * dec;
		bonesOsci[SKT_BONE_UARM_L] += bonesAnimRunOsci[SKT_BONE_UARM_L] * a/180.f;
			
		bonesOsci[SKT_BONE_LEG_R] -= bonesOsci[SKT_BONE_LEG_R] * dec;
		bonesOsci[SKT_BONE_LEG_L] -= bonesOsci[SKT_BONE_LEG_L] * dec;
			
		bonesOsci[SKT_BONE_TORSO] -= bonesOsci[SKT_BONE_TORSO] * dec;
		bonesOsci[SKT_BONE_TORSO] += bonesAnimRunOsci[SKT_BONE_TORSO] * a/180.f;

		bonesOsci[SKT_BONE_STOMACH] -= bonesOsci[SKT_BONE_STOMACH] * dec;
		bonesOsci[SKT_BONE_STOMACH] += bonesAnimRunOsci[SKT_BONE_STOMACH] * a/180.f;

		bonesOsci[SKT_BONE_NECK] -= bonesOsci[SKT_BONE_NECK] * dec;
		bonesOsci[SKT_BONE_NECK] += bonesAnimRunOsci[SKT_BONE_NECK] * a/180.f;

		bonesOsci[SKT_BONE_HEAD] -= bonesOsci[SKT_BONE_HEAD] * dec;
		bonesOsci[SKT_BONE_HEAD] += bonesAnimRunOsci[SKT_BONE_HEAD] * a/180.f;

		bonesOsci[SKT_BONE_BODY] -= bonesOsci[SKT_BONE_BODY] * dec;
		bonesOsci[SKT_BONE_BODY] += bonesAnimRunOsci[SKT_BONE_BODY] * a/180.f;

		normalise[CSK_ANIM_RUN]=true;
		startMode[CSK_ANIM_RUN]=true;
		
		if (a>=180)
			animType=CSK_ANIM_RUN;
		
		i+=speed;
		
		break;
		
	} // end animation type ---------------------------------------------------

	// apply all rotations to bones
	for (int n=0; n<SKT_BONE_COUNT; n++) {
		bones[n]->setRotation(matrixRotation(bonesRot[n], bonesOsci[n]));
		bones[n]->setPosition(bonesPos[n]+ bonesOsciPos[n]);
	}
	
}

scene::IAnimatedMeshSceneNode *cSkeleton::getSkeletonSceneNode() {
	return skeletonNode;
}

// returns motion variable
u32 cSkeleton::getMotionVariable() {
	return i;
}

