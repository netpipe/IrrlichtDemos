int playersetup(Cplayer &player)
{
    player.health = 100;
    player.deathtimer = 0;
    player.invulntimer = 5;
    player.ass_kick_timer = 0;
    player.invuln = true;
    player.active_gun = PISTOL;
    cout << player.health << endl;
    return 0;
}
Cgun gunsetup(Cgun gun[])
{
//qwe
    int c;
    for(c = 1; c < 10; c++)
    {
        gun[c].ammo = 0;
        gun[c].gunnum = c;
        gun[c].unlocked = false;
        gun[c].firetimer = 0;
    }
    gun[PISTOL].ammo = -1;
    gun[PISTOL].cooldown = 300;
    gun[PISTOL].firetimer = 0;
    gun[PISTOL].gunname = "Pistol";
    gun[PISTOL].gunnum = 0;
    gun[PISTOL].range = 1000;
    gun[PISTOL].unlocked = true;
    gun[PISTOL].damage = 20;
    gun[UZI].cooldown = 250; // or 150?
    gun[SHOTGUN].cooldown = 500;
    gun[BARREL].cooldown = 500;
    gun[GRENADE].cooldown = 300;
    gun[FAKEWALL].cooldown = 500;
    gun[CLAYMORE].cooldown = 500;
    gun[ROCKET].cooldown = 650;
    gun[CHARGE].cooldown = 500;
    gun[RAILGUN].cooldown = 330;
    gun[UZI].gunname = "Uzi";
    gun[SHOTGUN].gunname = "Shotgun";
    gun[BARREL].gunname = "Barrel";
    gun[GRENADE].gunname = "Grenade";
    gun[FAKEWALL].gunname = "Fake Wall";
    gun[CLAYMORE].gunname = "Claymore";
    gun[ROCKET].gunname = "Rocket";
    gun[CHARGE].gunname = "Charge";
    gun[RAILGUN].gunname = "Railgun";
    gun[UZI].range = 750;
    gun[SHOTGUN].range = 500;
    gun[BARREL].range = 0;
    gun[GRENADE].range = 1000000000;
    gun[FAKEWALL].range = 0;
    gun[CLAYMORE].range = 0;
    gun[ROCKET].range = 1000000000;
    gun[CHARGE].range = 0;
    gun[RAILGUN].range = 1000;
    gun[UZI].damage = 20;
    gun[SHOTGUN].damage = 20;
    gun[BARREL].damage = 200;
    gun[GRENADE].damage = 100;
    gun[FAKEWALL].damage = 0;
    gun[CLAYMORE].damage = 100;
    gun[ROCKET].damage = 150;
    gun[CHARGE].damage = 100;
    gun[RAILGUN].damage = 50;
    gun[PISTOL].maxammo = 9001;
    gun[UZI].maxammo = 100;
    gun[SHOTGUN].maxammo = 20;
    gun[BARREL].maxammo = 10;
    gun[GRENADE].maxammo = 20;
    gun[FAKEWALL].maxammo = 10;
    gun[CLAYMORE].maxammo = 10;
    gun[CHARGE].maxammo = 10;
    gun[RAILGUN].maxammo = 20;
    gun[ROCKET].maxammo = 20;
    return(*gun);
}

int Createbullet(Cbullet &bullet, Cgun &gun, Cplayer, ICameraSceneNode* bcamera)
{
    bullet.gorigin = gun.gunnum;
    bullet.timer = 0;
    bullet.Bulletnode->setPosition(bcamera->getPosition() + vector3df(0,-20,0));
    bullet.velocity = 10000;
    bullet.Bulletnode->setRotation(bcamera->getRotation());
    if(bullet.gorigin == 7)
        bullet.velocity = 100;
    if(bullet.gorigin == 9)
        bullet.velocity = 20000;
    bullet.range = gun.range;
    bullet.damage = gun.damage;
    bullet.hit = false;
    vector3df shotspread;
    if(bullet.gorigin == SHOTGUN)
    {
        shotspread.X = (rand()% 10 - 5) / 0.5f;
        shotspread.Y = (rand()% 10 - 5) / 0.5f;
        shotspread.Z = (rand()% 10 - 5) / 0.5f;
    }
    else
    {
        shotspread.X = 0;
        shotspread.Y = 0;
        shotspread.Z = 0;
    }
    bullet.heading = (bcamera->getTarget() - bcamera->getAbsolutePosition()) + shotspread;
    bullet.heading.normalize();
    gun.firetimer = gun.cooldown;
    bullet.Bulletnode->setVisible(true);
    return 0;
}

void createmine(Cmine &mine, Cplayer player, Cgun &gun, ICameraSceneNode* Camera)
{
    mine.state = PLACED;
    vector3df front = Camera->getTarget() - Camera->getPosition();
    front.Y = 0;
    front.normalize();
    mine.Minenode->setPosition(Camera->getPosition() + 50 * front - vector3df(0,57,0));
    mine.Minenode->setVisible(true);
    mine.damage = gun.damage;
    if(mine.gorigin == CLAYMORE)
        mine.dettime = 1.1f;
    if(mine.gorigin == FAKEWALL)
        mine.dettime = 100;
    if(mine.gorigin == BARREL)
        mine.dettime = 0;
    if(mine.gorigin == CHARGE)
        mine.subs = 3;
    mine.timer = 0;
}

Cmine claymoresetup(Cmine clay[], IAnimatedMesh* claymesh, ISceneManager* smgr)
{
    int z;
    for(z = 0; z < 200; z++)
    {
        clay[z].gorigin = CLAYMORE;
        clay[z].state = INACTIVE;
        clay[z].Minenode = smgr->addAnimatedMeshSceneNode(claymesh, 0, -1, vector3df(0,1000,0));
        clay[z].Minenode->setVisible(false);
        clay[z].range = 80;
        clay[z].subs = 0;
        clay[z].blink = smgr->addLightSceneNode(clay[z].Minenode, vector3df(0,25,0), SColorf(0.f, 0.f, 1.f), 0.f);
    }
    return(*clay);
}
Cmine chargesetup(Cmine charges[], IAnimatedMesh* chargemesh, ISceneManager* smgr)
{
    int z;
    for(z = 0; z < 200; z++)
    {
        charges[z].gorigin = CHARGE;
        charges[z].state = INACTIVE;
        charges[z].Minenode = smgr->addAnimatedMeshSceneNode(chargemesh, 0, -1, vector3df(0,1000,0));
        charges[z].Minenode->setVisible(false);
        charges[z].range = 80;
        charges[z].subs = 0;
        charges[z].blink = smgr->addLightSceneNode(charges[z].Minenode, vector3df(0,25,0), SColorf(1.f, 1.f, 0.f), 0.f);
    }
    return(*charges);
}

Cmine barrelsetup(Cmine barrel[], IAnimatedMesh* barrelmesh, ISceneManager* smgr)
{
    int z;
    for(z = 0; z < 200; z++)
    {
        barrel[z].gorigin = BARREL;
        barrel[z].state = INACTIVE;
        barrel[z].Minenode = smgr->addAnimatedMeshSceneNode(barrelmesh, 0, -1, vector3df(0,1000,0));
        barrel[z].Minenode->setVisible(false);
        barrel[z].range = 80;
        barrel[z].subs = 0;
    }
    return(*barrel);
}

Cmine wallsetup(Cmine wallfake[], IAnimatedMesh* wallfakemesh, ISceneManager* smgr)
{
    int z;
    for(z = 0; z < 200; z++)
    {
        wallfake[z].gorigin = FAKEWALL;
        wallfake[z].state = INACTIVE;
        wallfake[z].Minenode = smgr->addAnimatedMeshSceneNode(wallfakemesh, 0, -1, vector3df(0,1000,0));
        wallfake[z].Minenode->setVisible(false);
    }
    return(*wallfake);
}

void createexplosion(Cexplosion &explosion, vector3df position, float damage, int subs, float range)
{
//subs = 0 is not a sub or creating a sub. subs = -1 means it is a sub so it sets the dettimer
    if(explosion.state == INACTIVE)
    {
        explosion.anitime = 0;
        explosion.damage = damage;
        explosion.subs = subs;
        if(explosion.subs == -1)
        {
            explosion.dettime = 1;
            explosion.Explodenode->setPosition(vector3df(position.X + ((rand() % 200) - 100.f), 0, position.Z + ((rand() % 200) - 100.f)));
        }
        else
        {
            explosion.dettime = 0;
            explosion.Explodenode->setPosition(position);
        }
        explosion.range = range;
        explosion.state = EXPLODING;
        explosion.Explodenode->setVisible(true);
    }
}

Cmine explosioncheckwall(Cmine wall[], Cexplosion &explosion, ICameraSceneNode* Camera)
{
    int c;
    float dist;
    for(c=0; c<200; c++)
        if(wall[c].state == PLACED)
        {
            dist = sqrt(pow(wall[c].Minenode->getPosition().X - explosion.Explodenode->getPosition().X,2) + pow(wall[c].Minenode->getPosition().Z - explosion.Explodenode->getPosition().Z,2));
            if(dist <= explosion.range)
            {
                wall[c].dettime -= explosion.damage - ((explosion.damage / 2.f) * (dist / explosion.range));
                if(wall[c].dettime <= 0)
                {
                    wall[c].state = INACTIVE;
                    wall[c].Minenode->setVisible(false);
                }
            }
        }
    return(*wall);
}

Cmine explosioncheckbarrel(Cmine barrel[], Cexplosion &explosion, Cexplosion &newexplosion, Cplayer player, ICameraSceneNode* Camera, int &expcount)
{
    int c;
    float dist;
    for(c=0; c<200; c++)
        if(barrel[c].state == PLACED)
        {
            dist = sqrt(pow(barrel[c].Minenode->getPosition().X - explosion.Explodenode->getPosition().X,2) + pow(barrel[c].Minenode->getPosition().Z - explosion.Explodenode->getPosition().Z,2));
            if(dist <= explosion.range)
            {
                barrel[c].state = INACTIVE;
                barrel[c].Minenode->setVisible(false);
                createexplosion(newexplosion, barrel[c].Minenode->getPosition(), barrel[c].damage, barrel[c].subs , barrel[c].range);
                expcount += 1;
                if(expcount > 199)
                    expcount = 0;
            }
        }
    return(*barrel);
}

void createdeathpack(Cenemy enemy, Cdeathpack &pack, Cgun gun[])
{
    int gavailable = 0, c;
    for(c=0; c<10; c++)
        if(gun[c].unlocked == true)
            gavailable += 1;
    pack.gorigin = rand() % gavailable; // a value of 0 here means it has the potential to be a life up. Obviously the pistol does not require additional ammo.
    while(pack.gorigin == GRENADE)
        pack.gorigin = rand() % gavailable;
    pack.packnode->setPosition(vector3df(enemy.Bodynode->getPosition().X, 0, enemy.Bodynode->getPosition().Z));
    pack.state = PLACED;
    pack.timer = 20;
    pack.packnode->setVisible(true);
}

Cgun collectdeathpack(Cplayer &player, Cdeathpack &pack, Cgun gun[], ICameraSceneNode* Camera, Cheadup &headup)
{
    if(30 >= sqrt(pow(Camera->getPosition().X - pack.packnode->getPosition().X,2) + pow(Camera->getPosition().Z - pack.packnode->getPosition().Z,2)))
    {
        pack.state = INACTIVE;
        pack.timer = -1;
        pack.packnode->setVisible(false);
        headup.timer = 20;
        if(player.health < 50)
            pack.gorigin = 0;
        if(player.health < 100 && pack.gorigin == 0)
        {
            player.health = 100;
            headup.text = "You have been healed";
            pack.gorigin = -1;
        }
        if(pack.gorigin == 0 && player.health == 100)
        {
            int gavailable = -1, c;
            for(c=0; c<10; c++)
                if(gun[c].unlocked == true)
                    gavailable += 1;
            if(gavailable > 0)
                pack.gorigin = (rand() % (gavailable)) + 1;
            if(gavailable == 0)
                headup.text = "You have no guns which need ammo";
            else
                while(pack.gorigin == GRENADE || pack.gorigin == PISTOL)
                    pack.gorigin = rand() % gavailable;
        }
        if(pack.gorigin >= 1)
        {
            gun[pack.gorigin].ammo = gun[pack.gorigin].maxammo;
            if(pack.gorigin == 1)
                headup.text = "You picked up UZI ammo";
            if(pack.gorigin == 2)
                headup.text = "You picked up SHOTGUN ammo";
            if(pack.gorigin == 3)
                headup.text = "You picked up BARREL ammo";
            if(pack.gorigin == 4)
                headup.text = "You picked up GRENADE ammo";
            if(pack.gorigin == 5)
                headup.text = "You picked up FAKE WALL ammo";
            if(pack.gorigin == 6)
                headup.text = "You picked up CLAYMORE ammo";
            if(pack.gorigin == 7)
                headup.text = "You picked up ROCKET ammo";
            if(pack.gorigin == 8)
                headup.text = "You picked up CHARGE ammo";
            if(pack.gorigin == 9)
                headup.text = "You picked up RAILGUN ammo";
            pack.gorigin = -1;
        }
    }
    return(*gun);
}

Cdeathpack setupdeathpacks(Cdeathpack pack[], ISceneManager* smgr)
{
    int c;
    IAnimatedMesh* pickupboxmesh = smgr->getMesh("pickup_box3.obj");
    for(c=0; c<200; c++)
    {
        pack[c].gorigin = -1;
        pack[c].state = INACTIVE;
        pack[c].packnode = smgr->addAnimatedMeshSceneNode(pickupboxmesh, 0, -1, vector3df(0, -200, 0));
        pack[c].packnode->setVisible(false);
        pack[c].packnode->setMaterialFlag(EMF_LIGHTING, false);
        pack[c].timer = -1;
    }
    return(*pack);
}


void ammocounter(int ammo, ITexture* numbers[], IVideoDriver* driver, int height, int killcount, float kktimer, int hp, int width, float actiontime, float actionlength)
{
    int hundreds, tens, ones, active, c = 0, xpos = 80;
    hundreds = (int)ammo / 100;
    tens = (int)(ammo - (100 * hundreds) ) / 10;
    ones = (int)(ammo - (100 * hundreds) - (10 * tens) );

    int khundreds, ktens, kones;
    khundreds = (int)killcount / 100;
    ktens = (int)(killcount - (100 * khundreds) ) / 10;
    kones = (int)(killcount - (100 * khundreds) - (10 * ktens) );

    driver->draw2DRectangle(SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), rect<s32>(width - 165, height - 30, width - 165 + kktimer * 70.f, height - 20), 0);
    driver->draw2DRectangle(SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), rect<s32>(width - 165, height - 50, width - 165 + (2 * (actiontime / actionlength)) * 70, height - 40), 0);

    for(c = 0; c < 6; c++)
    {
        if(c == 0)
            active = ones;
        if(c == 1)
            active = tens;
        if(c == 2)
            active = hundreds;
        if(c == 3)
        {
            active = kones;
            xpos = width + 50;
        }
        if(c == 4)
            active = ktens;
        if(c == 5)
            active = khundreds;

        switch(active)
        {
        case 0:
            driver->draw2DImage(numbers[0], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 1:
            driver->draw2DImage(numbers[1], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 2:
            driver->draw2DImage(numbers[2], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 3:
            driver->draw2DImage(numbers[3], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 4:
            driver->draw2DImage(numbers[4], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 5:
            driver->draw2DImage(numbers[5], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 6:
            driver->draw2DImage(numbers[6], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 7:
            driver->draw2DImage(numbers[7], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 8:
            driver->draw2DImage(numbers[8], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        case 9:
            driver->draw2DImage(numbers[9], position2d<s32>(xpos - 50 * c, height - 140), rect<s32>(0,0,109,132), 0, SColor(200, 2.55 * (100 - hp), 2.55 * hp, 0), true);
            break;
        }

    }

}






