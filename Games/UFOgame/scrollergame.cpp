#ifndef scroller_CPP
#define scroller_CPP
// Wave based back view 3d scroller game
// code by G Davidson

#include "scrollergame.h"

void loadResources()
{
    // load all the textures
	for (s32 g=0; g<MAX_TEXTURES; ++g)
	{
		char tmp[64];
		sprintf(tmp, "data/texture/%s", texturenames[g]);
		video::ITexture* t = driver->getTexture(tmp);
		gametextures.push_back(t);
	}

	// load the game meshes
	for (s32 g=0; g<MAX_MESHES; ++g)
	{
		char tmp[64];
		sprintf(tmp, "data/mesh/%s", meshnames[g]);
		scene::IAnimatedMesh* t = device->getSceneManager()->getMesh(tmp);
		gamemeshes.push_back(t);
	}
}

void dropResources()
{
    // drop textures
  //  for (s32 g=0; g<gametextures.size(); ++g)
//	{
    //    gametextures[g]->drop();
//	}
	gametextures.clear();
}

void makeWaves()
{
     for (int n=0;n<2;n++)
     {
         core::array<SEnemy> *ta = new core::array<SEnemy>;
         SEnemy     *te;
         f32 z = 20;
         switch (n)
         {
             case 0:

                  te = new SEnemy;
                  te->pos        = core::vector3df(0,0,6000);
                  te->rotation   = core::vector3df(0,0,0);
                  te->enemytype  = EN_GUN_TURRET;
                  ta->push_back(*te);

                  for (int j=-10;j<11;j+=3)
                  {
                          te = new SEnemy;
                          te->pos        = core::vector3df(j*100,0,10000);
                          te->rotation   = core::vector3df(0,0,0);
                          te->enemytype  = EN_GUN_TURRET;
                          ta->push_back(*te);
                  }


                  for (f32 k=0;k<500;k+=z)
                  {
                      for (f32 j=-10;j<10;j+=5)
                      {
                          te = new SEnemy;
                          te->pos        = core::vector3df(j*100+((rand()*100)%80),0,15000+k * 50);
                          te->rotation   = core::vector3df(0,0,0);
                          te->enemytype  = EN_GUN_TURRET;
                          ta->push_back(*te);
                      }
                      z-=0.2;
                  }

                  break;
             case 1:
                  break;
         }
         waves.push_back(*ta);
     }
}



// weapons - rockets

CScrollingWorldSceneNode *terrain;

core::array<CGameSceneNode*>      collisionlist;  // list of nodes to check for bullet collision
CGameSceneNode                   *player;
CGameCamera                      *camera;
scene::CParticleSystem2SceneNode *exhaust;
scene::CParticleSystem2SceneNode *smoke;

int                              currentwave;
int                              currentenemy;
f32                              nextenemyz;
bool                             nomorewaves;

// collision list code

CGameSceneNode* checkCollision(core::line3d<f32> l,f32 &size, CGameSceneNode*obj)
{
    int n = 0;
    // loop through the list of collidables
    int maxcol = collisionlist.size();

    // speed up collision a bit by only checking radiuses if we moved less than size

    if (l.getLengthSQ() > size*size)
    {
        while (n < maxcol)
        {
            // cant collide with self
            if (collisionlist[n] == obj)
            {
               n++;
               continue;
            }

            core::vector3df pos = collisionlist[n]->getPosition();

            if ((l.getClosestPoint(pos)-pos).getLengthSQ() < (collisionlist[n]->size +size) * (collisionlist[n]->size +size))
            {
               // return the selected node
               return collisionlist[n];
            }
            n++;
        }
    }
    else
    {
        while (n < maxcol)
        {
            // cant collide with self
            if (collisionlist[n] == obj)
            {
               n++;
               continue;
            }

            core::vector3df pos = collisionlist[n]->getPosition();

            if ( (l.end-pos).getLengthSQ() < (collisionlist[n]->size +size) * (collisionlist[n]->size +size))
            {
               // return the selected node
               return collisionlist[n];
            }
            n++;
        }
    }
    // there was no collision
    return 0;
}

void addToCollisionList(CGameSceneNode *node)
{
    // push the node to the end of the list
    collisionlist.push_back(node);
}

void removeFromCollisionList(CGameSceneNode *node)
{
    // find the node
    int n = collisionlist.binary_search(node);
    // delete it
    if (n != -1)
        collisionlist.erase(n);
}

//
//  GAME NODE
//

        CGameSceneNode::CGameSceneNode(CGameSceneNode *parent, scene::ISceneManager*   smgr,  s32 id):
                    scene::ISceneNode(parent, smgr, id)
        {
            // printf("CGameSceneNode\n");
            dead = false;
            hasweapon = false;
            hasenergy = false;
            size =1.0;
            scale=1.0;

            if (!parent)
                setParent(smgr->getRootSceneNode());
        }

        void CGameSceneNode::OnPreRender()
        {
            // printf("CGameSceneNode::OnPreRender\n");
            // do we render children?
           	if (IsVisible)
        	{
    			SceneManager->registerNodeForRendering(this);
//    			ISceneNode::OnPreRender();
        	}
        }

        void CGameSceneNode::OnPostRender(u32 t)
        {
           	if (hasenergy)
           	{
                energybar->setSize(core::dimension2d<f32>((energy/maxenergy)*30,energybar->getSize().Height));
            }
//            ISceneNode::OnPostRender(t);
        }

		void CGameSceneNode::render()
        {
            // pure gamenodes dont get rendered

            /*
                video::IVideoDriver* driver = SceneManager->getVideoDriver();
                video::SMaterial m;
                m.Lighting = false;
    		    driver->setMaterial(m);
                driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
                driver->draw3DBox(BBox, video::SColor(0,255,255,255));
            */
        }

		const core::aabbox3d<f32>& CGameSceneNode::getBoundingBox()  const
		{
            // return the bbox
            return BBox;
        }

        // move the node depending on its velocity
        void CGameSceneNode::Move(u32 t)
        {
            // printf("CGameSceneNode::Move\n");
            core::vector3df   pos      = getPosition();
            core::vector3df   newpos   = getPosition() + velocity*t;

            bool              outFalling;
            core::triangle3df triout;
            core::vector3df   colpoint;
 core::vector3df   HitPoint1;
 irr::scene::ISceneNode* outNode;

            if (movement == GM_FLYING)
            {
                // flying objects aren't allowed to hit the ground

                // check for collision with ground
                colpoint =
                    SceneManager->getSceneCollisionManager()->getCollisionResultPosition(
                         terrain->getSelector(pos.Z), pos, core::vector3df(size/100,size/100,size/100),
                         velocity*t, triout,HitPoint1, outFalling,outNode);

                if (colpoint != newpos)
                {
                    this->Hit(velocity.getLength()*100, 1000, colpoint);
                    velocity *=-0.5;
                    velocity += (colpoint - newpos).normalize();
                }

                CGameSceneNode *tmp = checkCollision(core::line3d<f32>(pos, pos + velocity),size,this);
                if (tmp != 0)
                {
                     tmp->Hit(velocity.getLength()*100,1000, getAbsolutePosition());
                     this->Hit(velocity.getLength()*100,1000, tmp->getPosition());
                     velocity *=-0.5f;
                     while ( tmp->getPosition().getDistanceFrom(newpos) < tmp->size + size)
                     {
                           if ( velocity.getLength() < 1) velocity += velocity;
                           newpos += velocity;
                     }
                     //velocity -= (newpos-pos).normalize();
                }

                // move the node
                setPosition(newpos);

            }
            else if (movement == GM_ROLLING)
            {
                // driving objects don't get hurt by the ground
                // check for collision with ground
                colpoint =
                    SceneManager->getSceneCollisionManager()->getCollisionResultPosition(
                         terrain->getSelector(pos.Z), pos, core::vector3df(size, size, size),
                         velocity*t, triout, HitPoint1,outFalling,outNode, 0.0005f,core::vector3df(0.0f, -5.0f, 0.0f) );

                if (!outFalling)
                {
                    colpoint.Y += 0.5;
                }
                setPosition(colpoint);
            }
            else if (movement == GM_MOVEONLY)
            {
                 printf("bang!!!\n");
                 setPosition(newpos);
            }


        }

        // hits the game node from a direction for a duration
		f32 CGameSceneNode::Hit(f32 strength, s32 speed, core::vector3df attackerpos)
		{
            if (!hasenergy) return 0.0f;

            // printf("CGameSceneNode::Hit\n");
            f32 r = strength;
            if (hasshield && !dead)
            {
                r = shield->Hit(strength, speed, attackerpos);
                if (r > 0)
                {
                      // smoke!
                      Smoke(strength, speed, attackerpos);
                }
            }
            energy -=r;
            if (energy < 0)
            {
                r = -energy;
                if (dead)
                {
                    // the grand finale explosion
                    Destroy(r, speed, attackerpos);
                }
                else
                {
                    //dead = true;
                    if (hasshield)
                        shield->energybar->setVisible(false);
                    Die(r, speed, attackerpos);
                }
            }
            else
                r = 0;
            return r;
        }
        // this makes a ground node explode,
        void CGameSceneNode::Die(f32 strength, s32 speed, core::vector3df attackerpos)
        {
            // lose control
            dead = true;
            // smoke
            Smoke(strength, speed, attackerpos);
            // explode


        }
        void CGameSceneNode::Smoke(f32 strength, s32 speed, core::vector3df attackerpos)
        {
            new CExplosionSceneNode( 0, SceneManager, 0, this->getAbsolutePosition(), this->velocity, 22,2, 100, 500);
        }
        void CGameSceneNode::Destroy(f32 strength, s32 speed, core::vector3df attackerpos)
        {
            new CExplosionSceneNode( 0, SceneManager, 0, this->getAbsolutePosition(), this->velocity, 26, 2, 100, 500);
            SceneManager->addToDeletionQueue(this);
            printf("dropped a node\n");
        }



//       _____
//     /      \
//    | /\ /\  |
//    | \/ \/  |     enemy nodes
//    |        |
//    \_/\_/\_/
//

        CEnemySceneNode::CEnemySceneNode( CGameSceneNode* parent, scene::ISceneManager*   smgr,
                                          s32 id, core::vector3df pos, s32 enemytype):

                                          CGameSceneNode(parent, smgr, id)
        {
            type = enemytype;

            size = 5;
            range = 3000;

            scene::ISceneNode *tmp;
            switch(enemytype)
            {
                case EN_SCENERY:
                     // ignored for now
                     // set ai class
                     aiclass  = AI_LAME;
                     movement = GM_STATIC;
                     break;

                case EN_NUKE:
                     // set ai class
                     aiclass  = AI_MISSILE;
                     movement = GM_STATIC;    // missiles are static until launch

                     // add rocket mesh [5]
                     tmp = SceneManager->addAnimatedMeshSceneNode( gamemeshes[5], this);
                     tmp->setMaterialTexture(0, gametextures[16] );
                     tmp->setRotation(core::vector3df(0,0,62.44));
                     tmp->setPosition(core::vector3df(1.73,2.64,0));
                     tmp->setScale(core::vector3df(5,5,5));

                     // add the booster
                     tmp = new CBoosterSceneNode( tmp, smgr, 0, exhaust, this, core::aabbox3d<f32>(-0.2f,-0.2f,-0.2f, 0.2f,0.2f,0.2f), core::vector3df(0.0f,-0.0f,0.0f),
                                                  50, 100, video::SColor(255, 0, 0, 0), video::SColor(255, 255, 255, 255), 200, 300, 0);
                     tmp->drop();

                     // set specs
                     maxrotation = 0.01;
                     speed       = 1.0;

                     energy    = 10.0f;
                     maxenergy = 10.0f;

                     break;

                case EN_NUKE_LAUNCHER:
                     // set ai class
                     aiclass  = AI_CAR;
                     movement = GM_ROLLING;

                     // create launcher mesh


                     // add a rocket
                     tmp = new CEnemySceneNode(this, SceneManager, 0, core::vector3df(0,0,0),EN_NUKE);
                     tmp->drop();

                     break;

                case EN_GUN_TURRET:

                     scale = 40;
                     size  = 40;

                     energy    = 300.0f;
                     maxenergy = 300.0f;

                     // set ai class
                     aiclass  = AI_TURRET;
                     movement = GM_STATIC;

                     // add gun base [2]
                     tmp = SceneManager->addAnimatedMeshSceneNode( gamemeshes[2], this, -1, core::vector3df(0,-0.02,0));
                     tmp->setMaterialTexture(0, gametextures[13] );

                     // add gun head [3]
                     tmp = SceneManager->addAnimatedMeshSceneNode( gamemeshes[3], this);
                     tmp->setMaterialTexture(0, gametextures[14] );

                     h_aim = tmp;

                     // add gun nozzle [4]
                     tmp = SceneManager->addAnimatedMeshSceneNode( gamemeshes[4], h_aim);
                     tmp->setMaterialTexture(0, gametextures[15] );

                     v_aim = tmp;

                     falling = true;

                     // add a shield
                     hasshield = false; // true;
                     //shield    = new CShieldSceneNode( this, smgr, 0, 100, 0.1, 1.5, 1, 7, 0);
                     //shield->drop();

                     // time taken to reload
                     reloadtime = 50;

                     break;

            }

            setPosition(pos);
            setScale(core::vector3df(scale,scale,scale));

            hasenergy = true;
            energybar = smgr->addBillboardSceneNode( this, core::dimension2d<f32>(30.0f * (size/scale),1.5f * (size/scale)),core::vector3df(0.0f,size/scale,0.0f));
//            energybar = smgr->addBillboardSceneNode( this, core::dimension2d<f32>(size*30,size*1.5),core::vector3df(0,size-0.1,0));
            energybar->setMaterialTexture(0, gametextures[8] );
            energybar->setMaterialFlag(video::EMF_LIGHTING, false);
            addToCollisionList(this);
            nextfiretime = 0;
        }

        CEnemySceneNode::~CEnemySceneNode()
        {
            // remove from collision list (if it dropped off the map)
            removeFromCollisionList(this);
        }

        // this makes a ground node explode, killing it
        void CEnemySceneNode::Destroy(f32 strength, s32 speed, core::vector3df attackerpos)
        {
            // remove from collision list
            removeFromCollisionList(this);
            this->CGameSceneNode::Destroy(strength, speed, attackerpos);
        }

        void CEnemySceneNode::Aim()
        {

             core::vector3df aim = player->getAbsolutePosition() - getAbsolutePosition();
             aim = aim.getHorizontalAngle();
             if (h_aim)
                 h_aim->setRotation(core::vector3df(0.0f,aim.Y + 180.0f,0.0f));
             if (v_aim)
                 v_aim->setRotation(core::vector3df( -aim.X ,0.0f,0.0f));

             //Hit(0.05,10,player->getAbsolutePosition());
        }

        void CEnemySceneNode::OnPostRender(u32 t)
        {
            if (falling)
            {
                updateAbsolutePosition();
                falling = false;
                core::vector3df pos  = getAbsolutePosition();
                core::vector3df pos2 = terrain->getHeightAtXZ(pos.X,pos.Z);
                pos.Y = pos2.Y + size;

                //if (pos.Y -= size/2)

                setPosition(pos);
            }
            f32 x = getAbsolutePosition().Z - player->getPosition().Z;
            if ( fabs(x) < range )
            {
                Aim();

                // fire

                if (nextfiretime < t)
                {

                    nextfiretime = t+reloadtime + (rand()%(reloadtime/2));
                    core::vector3df laserspeed= core::vector3df(0,0,-3);
                    core::vector3df laserposa = core::vector3df(0,0,-220);

                    // rotations
                    laserspeed.rotateYZBy(v_aim->getRotation().X, core::vector3df(0,0,0) );
                    laserspeed.rotateXZBy(-h_aim->getRotation().Y, core::vector3df(0,0,0) );
                    laserspeed += velocity;

                    laserposa.rotateYZBy(v_aim->getRotation().X, core::vector3df(0,0,0) );
                    laserposa.rotateXZBy(-h_aim->getRotation().Y, core::vector3df(0,0,0) );

                    laserposa += getAbsolutePosition();

                    new CBulletSceneNode( this, SceneManager, 0, laserposa, laserspeed, 0, 27, 4, 30);
                }
            }
            else if (x < -200)
            {
                 removeFromCollisionList(this);
                 SceneManager->addToDeletionQueue(this);
            }

            CGameSceneNode::OnPostRender(t);
        }


//
//      _____
//     | |   \
//     |_|___/    Bullets
//

        CBulletSceneNode::CBulletSceneNode(CGameSceneNode* Parent, scene::ISceneManager*   smgr,  s32 id,
                                           core::vector3df position, core::vector3df vel,
                                           s32 meshno, s32 spriteno, s32 spritecount, f32 impactenergy):
                    CGameSceneNode(0, smgr, id), spritenumber(spriteno), sprites(spritecount)
        {
            // set variables
            parent = Parent;
            setPosition(position);
            velocity = vel;
            hasenergy = false;          // they dont smoke, collide, or have energy bars
            energy   = impactenergy;    // ...so we use energy variable as the payload
            lasttime = 0;
irr::scene::ISceneNode* outNode;

            setAutomaticCulling( scene::EAC_OFF );
            setDebugDataVisible(true);

            makeBullet();

            // work out collision point

            core::line3d<f32>     p = core::line3d<f32>(position,position+vel*3000); // 3 seconds max
            core::triangle3d<f32> tri;
            impact=false;

            BBox = core::aabbox3d<f32>(core::vector3df(-1,-1,-1),core::vector3df(1,1,1));

            if (terrain->selector[0] != NULL)
            {
                // check the ground
                if (SceneManager->getSceneCollisionManager()->getCollisionPoint(p, terrain->selector[0], endpoint, tri,outNode))
                {
                    // we found the end point
                    impact = true;
                }
            }
            if (!impact && terrain->selector[1] != NULL)
            {
                // check the ground
                if (SceneManager->getSceneCollisionManager()->getCollisionPoint(p, terrain->selector[1], endpoint, tri,outNode))
                {
                    // we found the end point
                    impact = true;
                }
            }
            u32 time = device->getTimer()->getTime();
            // work out time to impact
            if (impact)
            {
                endtime = time + (u32)(core::line3d<f32>(position,endpoint).getLength() / velocity.getLength());
            }
            else
            {
                endtime = time + 3000;
            }
        }

        void CBulletSceneNode::makeBullet()
        {
            // create the bullet
            scene::ISceneNode *tmp = SceneManager->addBillboardSceneNode( this, core::dimension2d<f32>(50.0f,50.0f) );
            tmp->setMaterialFlag(video::EMF_LIGHTING,false);
            tmp->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
            tmp->setMaterialTexture(0,gametextures[spritenumber]);
            core::array<video::ITexture*> textures;
            if (sprites >0)
            {
                for (int n = spritenumber; n<spritenumber+sprites; n++)
                    textures.push_back(gametextures[n]);
                scene::ISceneNodeAnimator * anim = SceneManager->createTextureAnimator( textures, 100 / sprites );
                tmp->addAnimator(anim);
                anim->drop();
            }
        }

        void CBulletSceneNode::OnPostRender(u32 t)
        {

            if (lasttime == 0)
            {
                lasttime = t;
                return;
            }

            u32 timepassed = t-lasttime;
            lasttime = t;

            if (t > endtime)
            {
                if (impact)  // decal code
                {
                    // this bullet has just landed on the floor at its endpoint
                    // it should make a decal and rotate it by endrot
                    // add decal to the terrain node
                    core::vector3df endrot(180,0,90);
                    scene::ISceneNodeAnimator *tmpa = SceneManager->createDeleteAnimator(5000);
                    scene::ISceneNode         *tmpo = SceneManager->addAnimatedMeshSceneNode(gamemeshes[0], 0, 0, endpoint, endrot);
                    tmpo->addAnimator(tmpa);
                    tmpo->setScale(core::vector3df(10,10,10));
                    tmpo->setRotation(endrot);
                    tmpo->setMaterialTexture(0, gametextures[31] );
                    tmpo->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                    tmpo->setMaterialFlag(video::EMF_LIGHTING, false);
                    tmpa->drop();

                    // printf("todo: decal code\n");
                }
                SceneManager->addToDeletionQueue(this);
            }
            else
            {
                core::vector3df p  = getPosition();
                core::vector3df p2 = p + velocity*timepassed;
                // check for collision with collision list
                CGameSceneNode *n = checkCollision(core::line3d<f32>(p,p2), size,parent);

                if (n != 0)
                {
                    // impact, explosion!
                    n->Hit(energy, 300, p);
                    SceneManager->addToDeletionQueue(this);
                }
                else
                {
                    // move the node
                    setPosition(p2);
                }
            }
            this->CGameSceneNode::OnPostRender(t);
        }


        CExplosionSceneNode::CExplosionSceneNode( CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id,
                                           core::vector3df position, core::vector3df vel,
                                           s32 spriteno, s32 spritecount, f32 e, u32 lifespan):
                                               CGameSceneNode(parent, smgr, id), spritenumber(spriteno)
        {

           movement = GM_FLYING;

           setPosition(position);
           velocity = vel;
           hasenergy = false;          // they dont smoke, collide, or have energy bars
           lasttime = 0;

//           setAutomaticCulling( EAC_OFF );
           setDebugDataVisible(true);

           energy = maxenergy = e;
           CreateExplosionParticles();

           scene::ISceneNodeAnimator* ani = SceneManager->createDeleteAnimator(lifespan);
           this->addAnimator(ani);
           ani->drop();

           BBox = core::aabbox3d<f32>(position + core::vector3df(-1,-1,-1),position + core::vector3df(1,1,1));

        }

        scene::IParticleSystemSceneNode* CExplosionSceneNode::CreateExplosionParticles()
        {
        	scene::IParticleSystemSceneNode* ps;
        	ps = SceneManager->addParticleSystemSceneNode(false,this);
        	ps->setMaterialFlag(video::EMF_LIGHTING, false);
        	ps->setPosition(core::vector3df(0,0,0));
        	ps->setParticleSize(core::dimension2d<f32>(100, 100));
        	ps->setMaterialTexture(0, gametextures[spritenumber]);
        	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
        	scene::IParticleEmitter* emb = ps->createBoxEmitter(
        		core::aabbox3d<f32>(-7.00f,-7.00f,-7.00f,7.00f,7.00f,7.00f),
        		core::vector3df(0.00f,0.30f,0.00f),
        		60,100,
        		video::SColor(0,10,10,10), video::SColor(0,100,100,100),
        		200,500,90);
        	ps->setEmitter(emb);
        	emb->drop();
        	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(video::SColor(0,0,0,0),200);
        	ps->addAffector(paf);
        	paf->drop();

            scene::ISceneNodeAnimator* ani = SceneManager->createDeleteAnimator(500);
            ps->addAnimator(ani);
            ani->drop();

        	return ps;
        }

       void CExplosionSceneNode::OnPostRender(u32 t)
       {
            //this->Move(t);
            this->CGameSceneNode::OnPostRender(t);
       }


//
//  /\/\/\       ______
//  \/\/\/\/\/\ |      \ Boosters
//       \/\/\/ |______/



    CBoosterSceneNode::CBoosterSceneNode(scene::ISceneNode *parent, scene::ISceneManager*   smgr,  s32 id,
                    scene::CParticleSystem2SceneNode *psystem, CGameSceneNode *follows,
                    core::aabbox3d<f32> box,  core::vector3df direction, u32 minParticlesPerSecond,
                    u32 maxParticlePerSecond,	video::SColor minStartColor,
                    video::SColor maxStartColor, u32 lifeTimeMin, u32 lifeTimeMax,
                	s32 maxAngleDegrees)   :
                            scene::ISceneNode( parent, smgr, id),
                            Box(box), Direction(direction), MinParticlesPerSecond(minParticlesPerSecond),
                            MaxParticlesPerSecond(maxParticlePerSecond),
                            MinStartColor(minStartColor), MaxStartColor(maxStartColor),
                            MinLifeTime(lifeTimeMin), MaxLifeTime(lifeTimeMax), Time(0), Emitted(0),
                            MaxAngleDegrees(maxAngleDegrees)
        {
            lastEmitTime = device->getTimer()->getTime();

            ps = psystem;
            gameobject = follows;
        }


    //! Prepares an array with new particles to emitt into the system
    //! and returns how much new particles there are.
    s32 CBoosterSceneNode::emitt(u32 now, u32 timeSinceLastCall, scene::SParticle2*& outArray)
    {
    	Time += timeSinceLastCall;

    	u32 pps = (MaxParticlesPerSecond - MinParticlesPerSecond);
    	f32 perSecond = pps ? (f32)MinParticlesPerSecond + (rand() % pps) : MinParticlesPerSecond;
    	f32 everyWhatMillisecond = 1000.0f / perSecond;

    	if (Time > everyWhatMillisecond)
    	{
            Particles.set_used(0);
    		s32 amount = (s32)((Time / everyWhatMillisecond) + 0.5f);
            Time = 0;
    		scene::SParticle2 p;
    		core::vector3df extend = Box.getExtent();

    		if (amount > (s32)MaxParticlesPerSecond*2)
    			amount = MaxParticlesPerSecond * 2;

    		f64 len = Direction.getLength();

    		for (s32 i=0; i<amount; ++i)
    		{
    			p.pos.X = Box.MinEdge.X + fmodf((f32)rand(), extend.X);
    			p.pos.Y = Box.MinEdge.Y + fmodf((f32)rand(), extend.Y);
    			p.pos.Z = Box.MinEdge.Z + fmodf((f32)rand(), extend.Z);

    			p.startTime = now;
    			p.vector = Direction ;

    			if (MaxAngleDegrees)
    			{
    				core::vector3df tgt =  Direction;
    				tgt.rotateXYBy((rand()%(MaxAngleDegrees*2)) - MaxAngleDegrees, core::vector3df(0,0,0));
    				tgt.rotateYZBy((rand()%(MaxAngleDegrees*2)) - MaxAngleDegrees, core::vector3df(0,0,0));
    				p.vector = tgt;
    			}

    			if (MaxLifeTime - MinLifeTime == 0)
    				p.endTime = now + MinLifeTime;
    			else
    				p.endTime = now + MinLifeTime + (rand() % (MaxLifeTime - MinLifeTime));

    			p.color = MinStartColor.getInterpolated(
    				MaxStartColor, (rand() % 100) / 100.0f);

    			p.startColor = p.color;
    			p.vector.normalize();
    			p.startVector = p.vector;
                Particles.push_back(p);

    		}
            outArray = Particles.pointer();
    		return Particles.size();
    	}

    	return 0;
    }

    void CBoosterSceneNode::render()
    {
        // doesnt actually exist :)
    }

    const core::aabbox3d<f32>& CBoosterSceneNode::getBoundingBox() const
    {
        return Box;
    }

    void CBoosterSceneNode::OnPostRender(u32 t)
        {
            if (IsVisible)
            {

//                scene::ISceneNode::OnPostRender(t);

                // make some particles

                scene::SParticle2* array = 0;
                s32 newParticles = emitt(t, t-lastEmitTime, array);

                core::matrix4 m = AbsoluteTransformation;

                if (newParticles && array)
                    for (s32 i=0; i<newParticles && ps->Particles.size() < 16250; ++i)
                    {

                        //array[i].startVector = core::vector3df(0,0,0);
                        //array[i].vector = core::vector3df(0,0,0);

                        m.rotateVect(array[i].startVector);
                        //if (ps->ParticlesAreGlobal)

                        array[i].startVector.Z = array[i].vector.Z = gameobject->velocity.Z * 0.7; //+ array[i].startVector ;
    //                    array[i].startVector /= 1000;
    //                    array[i].vector /= 1000;
                        AbsoluteTransformation.transformVect(array[i].pos);
                        ps->Particles.push_back(array[i]);
                    }
            }
            lastEmitTime = t;
        }




//    ____
//   /    \
//  |      |   --- Shield
//   \____/


        CShieldSceneNode::CShieldSceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,
                         s32                id,     f32                     startenergy,
                         f32                RechargeRate,
                         f32                scale,  s32                     imagestart,
                         s32                imagecount,s32 sparkle):

            CGameSceneNode(parent, smgr, id)
        {
            // printf("CShieldSceneNode::CShieldSceneNode\n");

            // shields don't move or collide with anything
            movement = GM_STATIC;

            // get the driver
            video::IVideoDriver* driver = smgr->getVideoDriver();
            // load the mesh
            mesh = gamemeshes[0]; // smgr->getMesh("data/mesh/shieldsphere.x");

            // no light
            light = NULL;
            // start of images
            for (int n = imagestart; n < imagestart + imagecount; n++)
            {
        		textures.push_back(gametextures[n]);
            }
        	// set the bbox
        	size   = scale*0.876;
        	BBox = core::aabbox3d<f32>(core::vector3df(scale,scale,scale),core::vector3df(-scale,-scale,-scale));

        	maxenergy = energy = startenergy;
            rechargerate = RechargeRate;

        	// add particle system
        	ps = smgr->addParticleSystemSceneNode(false,this);
            ps->setPosition(core::vector3df(0,0,0));
        	ps->setMaterialFlag(video::EMF_LIGHTING, false);
        	ps->setMaterialTexture(0, gametextures[sparkle]);
        	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        	ps->setParticleSize(core::dimension2d<f32>(scale, scale));
        	ps->setParticlesAreGlobal(false);

            // add energy bar
            hasenergy = true;
            energybar = smgr->addBillboardSceneNode( this, core::dimension2d<f32>((parent->size/parent->scale)*30,(parent->size/parent->scale)*1.5),core::vector3df(0,(parent->size/parent->scale)*0.9,0));
            energybar->setMaterialTexture(0, gametextures[9] );
            energybar->setMaterialFlag(video::EMF_LIGHTING, false);
//            energybar->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);


        	emitterdeltime =0;

        }
        CShieldSceneNode::~CShieldSceneNode()
        {
            // printf("CShieldSceneNode::~CShieldSceneNode\n");

            // drop the mesh
            //mesh->drop();

        	// clear the texture array
            //textures.clear();

            // drop the particle system
            //ps->drop();

        }

        void CShieldSceneNode::OnPostRender(u32 t)
        {
            if (lasttime==0)
            {
                lasttime = t;
                return;
            }
            u32 timepassed = t-lasttime;
            lasttime = t;

            // printf("CShieldSceneNode::OnPostRender\n");

            if (emitterdeltime != 0)
            {
                if (t > emitterdeltime)
                {
                    // remove emitter
                    ps->setEmitter(0);
                    // remove light
                    SceneManager->addToDeletionQueue(light);
                    light = NULL;
                    // we skip this branch next time
                    emitterdeltime = 0;
                }
                else
                {
                    // fade light
                    lightdata->Radius = 10-10 * (f32(t-lightstarttime) / f32(emitterdeltime-lightstarttime));
                }
            }

            // recharge shields

            energy += rechargerate * timepassed;
            if (energy > maxenergy) energy = maxenergy;

            CGameSceneNode::OnPostRender(t);
        }

        void CShieldSceneNode::OnPreRender()
        {
            // do we render?
           	if (IsVisible)
        	{
    			SceneManager->registerNodeForRendering(this);
//    			ISceneNode::OnPreRender();
        	}
        }

		void CShieldSceneNode::render()
        {
            // printf("CShieldSceneNode::Render\n");
            // we don't render this node

            /*
            // debugging - draw aabbox
                video::IVideoDriver* driver = SceneManager->getVideoDriver();
                video::SMaterial m;
                m.Lighting = false;
    		    driver->setMaterial(m);
                driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
                driver->draw3DBox(BBox, video::SColor(0,255,255,255));
            */
        }

		const core::aabbox3d<f32>& CShieldSceneNode::getBoundingBox()  const
		{
            // return the bbox
            return BBox;
        }

        // returns the remainder
        f32 CShieldSceneNode::Hit(f32 strength, s32 speed, core::vector3df attackerpos)
        {
            // printf("CShieldSceneNode::Hit\n");
            // calculate damage
            //    drop out if shields are down
            if (energy < 0)
                return strength;
            else
                energy -= strength;

            // get the rotation of the impact
            core::vector3df angle,selfpos= getAbsolutePosition();
            float x,y,z;
            x = attackerpos.X - selfpos.X;
            y = attackerpos.Y - selfpos.Y;
            z = attackerpos.Z - selfpos.Z;
            //   angle in X-Z plane
            angle.Y = atan2 (x, z);
            angle.Y *= 57.2957795; //converting from rad to degrees
            //   just making sure angle is somewhere between 0-360 degrees

            if(angle.Y < 0) angle.Y += 360;
            if(angle.Y >= 360) angle.Y -= 360;
            //   angle in Y-Z plane while Z and X axes are already rotated around Y
            float z1 = sqrt(x*x + z*z);
            angle.X = atan2 (z1, y);
            angle.X *= 57.2957795f; //converting from rad to degrees

            angle.X +=270;
            //   just making sure angle is somewhere between 0-360 degrees
            if(angle.X < 0) angle.X += 360;
            if(angle.X >= 360) angle.X -= 360;

            // create new shield node
            scene::ISceneNode *t =
                SceneManager->addAnimatedMeshSceneNode( mesh, this, -1,
                                    core::vector3df(0,0,0), angle, core::vector3df(size,size,size));
    		t->setMaterialFlag(video::EMF_LIGHTING, false);
    		t->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR); //DD_COLOR);
            t->getMaterial(0).BackfaceCulling = false;
            t->setMaterialTexture(0,textures[0]);

            // add the texture animator
            scene::ISceneNodeAnimator * anim = SceneManager->createTextureAnimator(textures, speed / textures.size() );
            t->addAnimator(anim);
            //     drop it
            anim->drop();

            // add a deletion animator
            anim = SceneManager->createDeleteAnimator (speed);
            t->addAnimator(anim);
            //     drop it
            anim->drop();

            // set the particle system's emitter
            //   when to delete it
            emitterdeltime = device->getTimer()->getTime() + speed;
            selfpos = getPosition();
            core::vector3df p2 = attackerpos - getAbsolutePosition();
            p2.normalize();

            scene::IParticleEmitter *em =
                        ps->createBoxEmitter(
                              core::aabbox3d<f32>(-size,-size,-size,size,size,size),
                              -p2/100, 100, 200, video::SColor(255,255,255,255),
                              200, 80,100);
            //core::aabbox3d<f32>(p2.X*size+selfpos.X-size/2,p2.Y*size+selfpos.Y-size/2,p2.Z*size +selfpos.Z-size/2,
            //p2.X*size+selfpos.X+size/2,p2.Y*size+selfpos.Y+size/2,p2.Z*size+selfpos.Z+size/2),

        	ps->setEmitter(em);
        	em->drop();

        	// add light
        	if (light != NULL)
        	{
                SceneManager->addToDeletionQueue(light);
            }
            light     = SceneManager->addLightSceneNode( this, p2, video::SColorf(1000,1000,5000) , 10.0f,-1 );
            lightdata = &light->getLightData();
            lightstarttime = device->getTimer()->getTime();

        	return 0;
        }




//             ___       _
//   /\   _  /    \    /  \
//  /  \/  \/      \/\/    \
// /        Terrain         \


        CScrollingWorldSceneNode::CScrollingWorldSceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id):
                    CGameSceneNode(parent, smgr, id)
        {
            // printf("CScrollingWorldSceneNode::CScrollingWorldSceneNode\n");

            // the terrain doesnt collide with its self, it doesnt move either
            movement = GM_STATIC;

            // get the driver
            video::IVideoDriver* driver = smgr->getVideoDriver();

            // set up variables
            farpoint= -50;
            current = 0;

            nextenemyz = waves[0] [0].pos.Z;

            // load all data
            for (s32 n = 0; n<3; n++)
            {
                // load mesh
                c8 tmp[64];
                sprintf(tmp, "data/mesh/level%d.obj", n);
                mesh[n]     = smgr->getMesh(tmp);
                sprintf(tmp, "data/texture/level%d.bmp", n);
                texture[n]  = driver->getTexture(tmp);
            }

        	current     =-1;
        	ground[0]   = NULL;
        	ground[1]   = NULL;
        	selector[0] = NULL;
        	selector[1] = NULL;
        }

        CScrollingWorldSceneNode::~CScrollingWorldSceneNode()
        {
            // printf("~CScrollingWorldSceneNode\n");
            if (ground[0] != NULL)
            {
                SceneManager->addToDeletionQueue(ground[0]);
                ground[0] = NULL;
            }
           /* if (selector[0] != NULL)
            {
                selector[0]->drop();
                selector[0] = NULL;
            } */
            if (ground[1] != NULL)
            {
                SceneManager->addToDeletionQueue(ground[1]);
                ground[1] = NULL;
            }
          /*  if (selector[1] != NULL)
            {
                selector[1]->drop();
                selector[1] = NULL;
            } */

            //for (s32 n = 0; n < 3; n++)
            //{
                //mesh[n]->drop();
                // dont think i have to drop textures
                //texture[n]->drop();
            //}
        }

        void CScrollingWorldSceneNode::Move(u32 t)
        {
            // we don't move this node.
            // this is here to make sure it doesnt accidently get called
        }

        void CScrollingWorldSceneNode::OnPostRender(u32 time)
        {
            // printf("CScrollingWorldSceneNode::OnPostRender\n");
            checkGround();
            this->CGameSceneNode::OnPostRender(time);
        }

        // returns the triangle selector associated with a Z value
        scene::ITriangleSelector* CScrollingWorldSceneNode::getSelector(f32 Z)
        {
            // printf("CScrollingWorldSceneNode::getSelector\n");
            if (Z > join)
               return selector[1];
            else
               return selector[0];
        }

        // returns the triangle selector associated with a Z value
        core::vector3df CScrollingWorldSceneNode::getHeightAtXZ(f32 X, f32 Z)
        {
            int i = Z > join ? 1 : 0; // triangle selector number
irr::scene::ISceneNode* outNode;
            selector[i];

            core::line3d<f32> line = core::line3d<f32>(X,10000.0f,Z,X,-10000.0f,Z);
            core::vector3df   intersection;
            core::triangle3df tri;

            SceneManager->getSceneCollisionManager()->getCollisionPoint(line, selector[i], intersection, tri,outNode);

            // printf("collision at %f, %f, %f from %f, %f\n", intersection.X, intersection.Y, intersection.Z, X, Z);

            return intersection;
        }

        bool CScrollingWorldSceneNode::checkGround()
        {
             bool r = false;

            // printf("CScrollingWorldSceneNode::checkGround\n");
            if (camera->cam->getAbsolutePosition().Z + camera->cam->getFarValue() > farpoint)
            {
                // printf("setup\n");
                // we drop the back one, if there is one
                if (ground[0])
                {
                    SceneManager->addToDeletionQueue(ground[0]);
                    ground[0] = NULL;
                }
                if (selector[0])
                {
                   /* selector[0]->drop(); */
                    selector[0] = NULL;
                }
                // shuffle the last one, if there is one
                if (ground[1])
                    ground[0]=ground[1];
                if (selector[1])
                    selector[0]=selector[1];

                // increase the section number
                current +=1;
                // add the next scene node
                ground[1] = SceneManager->addAnimatedMeshSceneNode( mesh[ terrainorder[current]],this);
                // printf("add\n");
                ground[1]->setScale(core::vector3df(100, 100, 100));
                ground[1]->setPosition(core::vector3df(0,-200,
                                              farpoint + ground[1]->getBoundingBox().MaxEdge.X*100 ) );
                ground[1]->setRotation(core::vector3df(0,90,0));

                ground[1]->setMaterialTexture(0, texture[ terrainorder[current] ]);

                // create triangle selector
                selector[1] = SceneManager->createTriangleSelector(mesh[terrainorder[current]]->getMesh(0),ground[1]);
                ground[1]->setTriangleSelector(selector[1]);
                selector[1]->drop();

                // set the join value to the last far value
                join=farpoint;
                // move the far value in to the distance
                farpoint = ground[1]->getPosition().Z + ground[1]->getBoundingBox().MaxEdge.X*100;

                // printf("current = %d, far = %f, edge = %f \n",current,farpoint,this->getBoundingBox().MaxEdge.Z);

				r = true;
            }

            // add any enemy nodes!
            if (nextenemyz <= camera->cam->getAbsolutePosition().Z + camera->cam->getFarValue() && !nomorewaves)
            {
               printf("new enemy!\n");
                // next wave?
               if (waves[currentwave].size() < currentenemy+1 )
               {
                   currentenemy = 0;
                   currentwave++;
                   // last wave?
                   if (waves.size() > currentwave)
                   {
                      printf("end of the game!!\n");
                      nomorewaves=true;
                      return r;
                   }
               }
               else
               {
                   printf("spitting out enemy node %d/%d in wave %d/%d @ %f\n",currentenemy,waves[currentwave].size(),currentwave,waves.size(), nextenemyz);
                   new CEnemySceneNode(this,SceneManager,0,waves[currentwave] [currentenemy].pos,waves[currentwave] [currentenemy].enemytype );

                   if (waves[currentwave].size() > currentenemy+1 )
                   {
                       nextenemyz = waves[currentwave] [currentenemy].pos.Z;
                   }
                   currentenemy++;
                  // system("pause");
                   //nomorewaves=true;
               }
            }

			return r;
        }



//     _
//   _/_\_
// (______)    -- Player ship
//

        CPlayerSceneNode::CPlayerSceneNode(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id):
                    CGameSceneNode(parent, smgr, id)
        {
            // the ship flies, obviously
            movement = GM_FLYING;

            // get the driver
            video::IVideoDriver* driver = smgr->getVideoDriver();

            // set up variables

            energy  =maxenergy= 1000;
            playerspeed     = 0.8;
            maxvel          = 2.0;
            maxdistx        = 1000;
            maxdisty        = 400;
            velocity        = core::vector3df(0,0,0);
            lasttime        = 0;
            nextfiretime    = 0;
            size            = 100;
            scale           = 100;

            // load the mesh
            scene::IAnimatedMesh *mesh = gamemeshes[1]; // smgr->getMesh("data/mesh/playership.obj");

            // add the scene node
            ship = SceneManager->addAnimatedMeshSceneNode( mesh, this);
        	ship->setMaterialTexture(0, gametextures[11] ); //  driver->getTexture("data/texture/playership.bmp") );

        	ship->setPosition(core::vector3df(0,0,0));
            ship->setRotation(core::vector3df(0,90,0));
            ship->setScale(core::vector3df(0.5,0.5,0.5));

            // get ships bbox
            BBox = core::aabbox3d<f32>(ship->getBoundingBox());

            // add the shield
            hasshield = true;
            shield    = new CShieldSceneNode(this, smgr, 0, 200, 0.1, 1.4, 1,7,0);
            shield->drop();

            // add the target
            scene::ISceneNode *starget = smgr->addBillboardSceneNode(ship, core::dimension2d<f32>(50, 50), core::vector3df(0,0,50));
            video::ITexture *ttex = gametextures[12]; // driver->getTexture("data/texture/target.bmp");
            driver->makeColorKeyTexture( ttex, core::position2d<s32>(10,10) );
            starget->setMaterialTexture(0, ttex );
            starget->setMaterialFlag(video::EMF_LIGHTING, false);
            starget->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

            starget = smgr->addBillboardSceneNode(ship, core::dimension2d<f32>(50, 50), core::vector3df(0,0,75));
            starget->setMaterialTexture(0, gametextures[12] );
            starget->setMaterialFlag(video::EMF_LIGHTING, false);
            starget->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
            starget = smgr->addBillboardSceneNode(ship, core::dimension2d<f32>(50, 50), core::vector3df(0,0,100));
            starget->setMaterialTexture(0, gametextures[12] );
            starget->setMaterialFlag(video::EMF_LIGHTING, false);
            starget->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
            starget = smgr->addBillboardSceneNode(ship, core::dimension2d<f32>(50, 50), core::vector3df(0,0,125));
            starget->setMaterialTexture(0, gametextures[12] );
            starget->setMaterialFlag(video::EMF_LIGHTING, false);
            starget->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);


            // add energy bar
            hasenergy = true;
            energybar = smgr->addBillboardSceneNode( this, core::dimension2d<f32>((size/scale)*30,(size/scale)*1.5),core::vector3df(0,size/scale,-0.01));
            energybar->setMaterialTexture(0, gametextures[8] );
            energybar->setMaterialFlag(video::EMF_LIGHTING, false);

            // light
            smgr->addLightSceneNode( this, core::vector3df(0,0,-6), video::SColorf(1,1,128),100);

            // boosters

            jets[0] = new CBoosterSceneNode( ship, smgr, 0, exhaust, this, core::aabbox3d<f32>(-2.762,-0.02,-0.1, -0.6775*2,0.02,-0.4), core::vector3df(0,0,0),50, 100, video::SColor(255, 0, 0, 0), video::SColor(255, 255, 255, 255), 200, 300, 0);
            jets[0]->drop();
            jets[1] = new CBoosterSceneNode( ship, smgr, 0, exhaust, this, core::aabbox3d<f32>(-0.4, -0.4, -0.1,   0.4, 0.4, 0.1),      core::vector3df(0,0,0),50, 100, video::SColor(255, 0, 0, 0), video::SColor(255, 255, 255, 255), 200, 300, 0);
            jets[1]->drop();
            jets[2] = new CBoosterSceneNode( ship, smgr, 0, exhaust, this, core::aabbox3d<f32>(1.355,-0.02,-0.1,  2.762,0.02,0.1),      core::vector3df(0,0,0),50, 100, video::SColor(255, 0, 0, 0), video::SColor(255, 255, 255, 255), 200, 300, 0);
            jets[2]->drop();

            addToCollisionList(this);

        }

        CPlayerSceneNode::~CPlayerSceneNode()
        {
            //shield->remove();
        }

        void CPlayerSceneNode::Smoke(f32 strength, s32 speed, core::vector3df attackerpos)
        {
             camera->shake = (attackerpos-getAbsolutePosition()).normalize() * strength / 10.0f;
             CGameSceneNode::Smoke(strength,speed,attackerpos);
        }


        void CPlayerSceneNode::Destroy(f32 strength, s32 speed, core::vector3df attackerpos)
        {
            new CExplosionSceneNode( 0, SceneManager, 0, this->getAbsolutePosition(), this->velocity, 26,2, 100, 500);
            removeFromCollisionList(this);
            setVisible(false);
        }

        void CPlayerSceneNode::OnPostRender(u32 time)
        {
            if (!isVisible()) return;

            if (lasttime==0)
            {
                lasttime = time;
                return;
            }
            u32 timepassed = time-lasttime;
            lasttime = time;

            if (!dead)
            {
                // process keyboard input and adjust X and Y velocity
                if (keys[KEY_KEY_A])
                {
                    velocity.X -= 0.008f*timepassed;
                    if (velocity.X < -maxvel)
                       velocity.X = -maxvel;
                }
                if (keys[KEY_KEY_D])
                {
                    velocity.X += 0.008f*timepassed;
                    if (velocity.X > maxvel)
                        velocity.X = maxvel;
                }
                if (keys[KEY_KEY_S])
                {
                    velocity.Y -= 0.008f*timepassed;
                    if (velocity.Y < -maxvel)
                        velocity.Y = -maxvel;
                }
                if (keys[KEY_KEY_W])
                {
                    velocity.Y += 0.008f*timepassed;
                    if (velocity.Y > maxvel)
                       velocity.Y = maxvel;
                }

                if (!rmouse)
                {
                    velocity.X += cpos.X*mousesensitivity.X;
                    velocity.Y -= cpos.Y*mousesensitivity.Y;
                    if (velocity.X > maxvel) velocity.X = maxvel;
                    if (velocity.Y > maxvel) velocity.Y = maxvel;
                    if (velocity.X < -maxvel) velocity.X = -maxvel;
                    if (velocity.Y < -maxvel) velocity.Y = -maxvel;
                }

                shiprot.X += cpos.X*mousesensitivity.X;
                shiprot.Y -= cpos.Y*mousesensitivity.Y;
                if (shiprot.X > maxvel) shiprot.X = maxvel;
                if (shiprot.Y > maxvel) shiprot.Y = maxvel;
                if (shiprot.X < -maxvel) shiprot.X = -maxvel;
                if (shiprot.Y < -maxvel) shiprot.Y = -maxvel;

                if (keys[KEY_KEY_P] || lmouse)
                {

                    if (time > nextfiretime)
                    {
                        nextfiretime = time + 150;
                        core::vector3df laserspeed= core::vector3df(0,0,10);
                        core::vector3df laserposa = core::vector3df(-16.2,5.23,16.18);
                        core::vector3df laserposb = core::vector3df(16.2,5.23,16.18);

                        // rotations
                        laserspeed.rotateYZBy(ship->getRotation().X, core::vector3df(0,0,0) );
                        laserspeed.rotateXZBy(-ship->getRotation().Y, core::vector3df(0,0,0) );

                        laserspeed += velocity;

                        laserposa.rotateYZBy(ship->getRotation().X, core::vector3df(0,0,0) );
                        laserposa.rotateXZBy(-ship->getRotation().Y, core::vector3df(0,0,0) );

                        laserposb.rotateYZBy(ship->getRotation().X, core::vector3df(0,0,0) );
                        laserposb.rotateXZBy(-ship->getRotation().Y, core::vector3df(0,0,0) );

                        laserposa += getAbsolutePosition();
                        laserposb += getAbsolutePosition();

                        new CBulletSceneNode(this, SceneManager, 0, laserposa, laserspeed + velocity, 0, 0, 1, 200);
                        new CBulletSceneNode(this, SceneManager, 0, laserposb, laserspeed + velocity, 0, 0, 1, 200);
                    }
                }
            }
            else
            {
                // fall
                velocity.Y -= 0.004f*timepassed;
            }
            // z velocity is constant
            velocity.Z += 0.005*timepassed;
            if (velocity.Z > playerspeed) velocity.Z = playerspeed;

            // air resistance, for each milisec we remove x%
            if (!rmouse)
            {
               shiprot  *= powf(0.996f,timepassed);
               velocity *= powf(0.996f,timepassed);
            }
            /*
               for (int n=0; n < timepassed; n++)
               {
                   shiprot  *= 0.996;
                   velocity *= 0.996;
               } */

            // move the player, check for collision, etc
            Move(timepassed);

            // constrain ship to box?
            core::vector3df newpos    = getPosition();

            if (!dead)
            {
                bool            poschange = false;

                if      (newpos.X < -maxdistx) { poschange=true; newpos.X = -maxdistx; velocity.X=0.0f; }
                else if (newpos.X >  maxdistx) { poschange=true; newpos.X =  maxdistx; velocity.X=0.0f; }
                if      (newpos.Y < -maxdisty) { poschange=true; newpos.Y = -maxdisty; velocity.Y=0.0f; }
                else if (newpos.Y >  maxdisty) { poschange=true; newpos.Y =  maxdisty; velocity.Y=0.0f; }

                if (poschange) setPosition(newpos);
            }

            ship->setRotation(core::vector3df(-shiprot.Y*20,shiprot.X*20,shiprot.Z*20)); //core::vector3df(velocity.Y*-20,velocity.X*20,0));

            this->CGameSceneNode::OnPostRender(time);

        }


//  ______________   _
// |              | / |
// |              |K  |  --- Camera node
// |______________| \_|
//


        CGameCamera::CGameCamera(CGameSceneNode* parent, scene::ISceneManager*   smgr,  s32 id):
                CGameSceneNode(parent, smgr, id)
        {
            // add camera
            cam = smgr->addCameraSceneNode(this);
            shake = core::vector3df(0,0,0);
        }

        void CGameCamera::OnPostRender(u32 t)
        {
            core::vector3df newpos = player->getPosition();
            // move camera (and camera shake)

            u32 timepassed = t-lasttime;
            lasttime =t;

            if (lastframe != t/100)
            {
                lastframe = t/100;
                shake = -shake * 0.9;
            }
            f32 phase = (t % 100) /50;

            this->setPosition(newpos + core::vector3df(0,0,-200) -
                    core::vector3df(newpos.X * 0.15,newpos.Y * 0.3,0) + (shake * phase) );
            this->cam->setTarget(newpos -
                    core::vector3df(newpos.X * 0.15,newpos.Y * 0.3,0) + (shake * phase) );

            this->CGameSceneNode::OnPostRender(t);
        }

        #endif
