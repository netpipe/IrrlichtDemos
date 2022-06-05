int spawn(Cenemy &spawnee, Cenemy others[], vector3df spawns[])
{
    int ec;
    float edist;
    vector3df otherpos;
    int spawnpoint = rand() % 4;

    bool canspawn = true;
    for(ec = 0; ec < 200; ec++)
    {
        if(others[ec].health > 0 && canspawn)
        {
            otherpos = others[ec].Bodynode->getPosition();
            edist = sqrt((otherpos.X - spawns[spawnpoint].X) * (otherpos.X - spawns[spawnpoint].X) + (otherpos.Z - spawns[spawnpoint].Z) * (otherpos.Z - spawns[spawnpoint].Z));
            if(edist < 100)
                canspawn = false;
        }
    }
    if(canspawn)
    {
        spawnee.Bodynode->setVisible(true);
        spawnee.Bodynode->setPosition(spawns[spawnpoint] + vector3df(0,20,0));
        spawnee.cooldown = 500;
        spawnee.heading = vector3df(1,0,0);
        spawnee.health = 100;
        spawnee.anitime = 0;
        if(spawnee.devil == true)
        {
            spawnee.health *= 3.5;
            spawnee.movespeed *= 1.5f;
        }

    }
    return 0;
}


void update(Cenemy updatee, f32 FDT)
{
    if(updatee.animation[Is_Walking])
        updatee.Bodynode->setPosition(updatee.Bodynode->getPosition() + updatee.heading * FDT * updatee.movespeed);
    if(updatee.animation[Is_Attacking])
        updatee.Bodynode->setPosition(updatee.Bodynode->getPosition() + updatee.heading * FDT * updatee.movespeed / 3.f);
    if(updatee.animation[Is_Being_Attacked] || (updatee.animation[Is_Dead] && updatee.anitime < .1))
        updatee.Bodynode->setPosition(updatee.Bodynode->getPosition() + updatee.hitheading * FDT * 8.f * updatee.movespeed);

    if(updatee.heading.Z > 0)
        updatee.Bodynode->setRotation(vector3df(0,atan(updatee.heading.X / updatee.heading.Z) * 180.f / PI, 0) + updatee.bodyrotation);
    else
        updatee.Bodynode->setRotation(vector3df(0,(atan(updatee.heading.X / updatee.heading.Z) * 180.f / PI) + 180.f,0) + updatee.bodyrotation);
}

void separate(Cenemy &sepZombie, Cenemy otherzombie, float maxrotsep, float maxsep, float sepdist, f32 sepfdt)
{
    vector3df totank = otherzombie.Bodynode->getPosition() - sepZombie.Bodynode->getPosition();
    float hang, otang, pang;
    bool left = true;

    hang = atan(sepZombie.heading.X / sepZombie.heading.Z);
    if(sepZombie.heading.Z < 0)
        hang += PI;
    pang = hang + PI;
    if(pang >= 2.f*PI)
        pang -= 2.f*PI;

    otang = atan(totank.X / totank.Z);
    if(totank.Z < 0)
        otang += PI;

    if(hang <= PI)
    {
        if(otang < hang || otang > pang)
            left = false;
        else
            left = true;
    }
    if(hang > PI)
    {
        if(otang < hang && otang > pang)
            left = false;
        else
            left = true;
    }

    if(left)
        rotate(sepZombie, sepfdt, maxrotsep * (maxsep-sepdist)/maxsep);
    else
        rotate(sepZombie, sepfdt, maxrotsep * (maxsep-sepdist)/maxsep * -1);
}

void rotate(Cenemy &rTank, f32 timediff, float angleRotation)
{
//angleRotation is rad/sec
    float oldX;
    oldX = rTank.heading.X;
    rTank.heading.X = rTank.heading.X * cos(angleRotation * timediff) - rTank.heading.Z * sin(angleRotation * timediff);
    rTank.heading.Z = oldX * sin(angleRotation * timediff) + rTank.heading.Z * cos(angleRotation * timediff);
}



void center(Cenemy &cenZombie, vector3df apos, float maxrotcen, f32 cenfdt)
{
    vector3df tocenter = apos - cenZombie.Bodynode->getPosition();

    float dang, pdang, avgdang, difdang;

    dang = atan(cenZombie.heading.X / cenZombie.heading.Z);
    if(cenZombie.heading.Z < 0)
        dang += PI;
    pdang = dang + PI;
    if(pdang >= 2.f*PI)
        pdang -= 2.f*PI;

    avgdang = atan(tocenter.X / tocenter.Z);
    if(tocenter.Z < 0)
        avgdang += PI;

    difdang = dang - avgdang;
    if(difdang > PI)
        difdang -= 2.f * PI;
    if(difdang < -1.f * PI)
        difdang += 2.f * PI;

    rotate(cenZombie, cenfdt, maxrotcen * difdang/PI);
}

void animate(Cenemy &azombie, f32 frameDeltaTime, Cplayer &player, ICameraSceneNode* Camera, Cgun gun[], Cdeathpack &pack, int &pcount)
{
    if(azombie.animation[Is_Walking] || (azombie.devil && azombie.animation[Is_Attacking]))
    {
        int aspeed = 8;
        azombie.anitime += frameDeltaTime;
        vector3df bodyrotation = vector3df(7.5f + 7.5f * sin(aspeed * azombie.anitime * 2.f /** aspeed * PI*/), 0, 0);
        azombie.Bodynode->setRotation(bodyrotation);
        azombie.bodyrotation = bodyrotation;
        azombie.Footrnode->setRotation(vector3df(-20 * sin(aspeed * azombie.anitime), 0, 0));
        azombie.Footlnode->setRotation(vector3df(20 * sin(aspeed * azombie.anitime), 0, 0));
        azombie.Armlnode->setRotation(-bodyrotation);
        azombie.Armrnode->setRotation(-bodyrotation);
        azombie.Armlnode->setPosition(vector3df(0, 0.8f + 0.8f * sin(aspeed * azombie.anitime * 2.f + 2.f * aspeed * PI), 2.f + 2.f * sin(aspeed * azombie.anitime * 2.f + 2.f * aspeed * PI)) + vector3df(0,11.f,-5.f));
        azombie.Armrnode->setPosition(vector3df(0, 0.8f + 0.8f * sin(aspeed * azombie.anitime * 2.f + 2.f * aspeed * PI), 2.f + 2.f * sin(aspeed * azombie.anitime * 2.f + 2.f * aspeed * PI)) + vector3df(0,11.f,-5.f));
    }

    if(azombie.animation[Is_Attacking])
    {
        int aspeed = 15;
        if(azombie.devil)
            aspeed = 7;
        float animation_modifier = 1;
        if(azombie.devil == false)
        {
            vector3df bodyrotation = 1 * vector3df(10 + 15 * sin(aspeed * azombie.anitime), 0, 0);
            azombie.Bodynode->setRotation(bodyrotation);
            azombie.bodyrotation = bodyrotation;
            azombie.Armlnode->setRotation(-bodyrotation * .7f + vector3df(-30 * cos(aspeed * azombie.anitime) ,0,0));
            azombie.Armrnode->setRotation(-bodyrotation * .7f + vector3df(-30 * cos(aspeed * azombie.anitime) ,0,0));
            azombie.Armlnode->setPosition(vector3df(0, 0 * cos(aspeed * azombie.anitime), 2 + 2.f * sin(aspeed * azombie.anitime)) + vector3df(0,11.f,-5.f));
            azombie.Armrnode->setPosition(vector3df(0, 0 * cos(aspeed * azombie.anitime), 2 + 2.f * sin(aspeed * azombie.anitime)) + vector3df(0,11.f,-5.f));
            azombie.Bodynode->setPosition(azombie.Bodynode->getAbsolutePosition() + azombie.heading * (.02f * sin(aspeed * azombie.anitime + 2.f * aspeed * PI)));
            azombie.Footlnode->setRotation(-bodyrotation);
            azombie.Footrnode->setRotation(-bodyrotation);
            azombie.anitime += frameDeltaTime * animation_modifier;
        }


        if(azombie.anitime > .55 && azombie.devil == false)
        {
            attackhitcheck(azombie, player, Camera);
            azombie.animation[Is_Attacking] = false;
            azombie.animation[Is_Walking] = true;
        }
        if(azombie.anitime > 2 && azombie.devil == true)
        {
            attackhitcheck(azombie, player, Camera);
            azombie.animation[Is_Attacking] = false;
            azombie.animation[Is_Walking] = true;
        }
    }

    if(azombie.animation[Is_Being_Attacked])
    {
        if(azombie.anitime >= .08)
        {
            azombie.animation[Is_Being_Attacked] = false;
            azombie.animation[Is_Walking] = true;
        }

        azombie.anitime += frameDeltaTime;
    }

    if(azombie.animation[Is_Dead])
    {

        azombie.anitime += frameDeltaTime;
        if(azombie.anitime > .1 && azombie.dpcheck == false)
        {
            azombie.Bodynode->setPosition(azombie.Bodynode->getPosition() + vector3df(0,1000,0));
            int lolrand;
            lolrand = rand()%15;
            if(lolrand == 10 || azombie.devil)
            {
                createdeathpack(azombie, pack, gun);
                pcount += 1;
            }
            azombie.dpcheck = true;
        }
    }
}

void enemysetup(Cenemy &senemy)
{
    int p;
    senemy.health = -103;
    senemy.movespeed = 90;
    senemy.anitime = 0;
    senemy.cooldown = 0;
    senemy.devil = false;
    senemy.heading = vector3df(0,0,0);
    senemy.hitcool = 0;
    senemy.hitheading = vector3df(0,0,0);
    senemy.level = 0;
    for(p = 0; p < Animation_Number; p++)
        senemy.animation[p] = false;
    senemy.animation[Is_Walking] = true;
    senemy.Bodynode->setVisible(false);
    senemy.dpcheck = false;
}

Cenemy attackcheck(Cenemy enemy[], ICameraSceneNode* Camera)
{
    int c,k;
    float dist;
    for(c = 0; c < 200; c++)
        if(enemy[c].health > 0 && enemy[c].devil == false)
        {
            dist = sqrt(pow(Camera->getPosition().X - enemy[c].Bodynode->getPosition().X,2) + pow(Camera->getPosition().Z - enemy[c].Bodynode->getPosition().Z,2));
            if(dist <= 50 && enemy[c].animation[Is_Walking] == true)
            {
                for(k = 0; k < Animation_Number; k++)
                    enemy[c].animation[k] = false;
                enemy[c].animation[Is_Attacking] = true;
                enemy[c].animation[Is_Walking] = false;
                enemy[c].anitime = 0;
            }
        }
    return(*enemy);
}

void attackhitcheck(Cenemy enemy, Cplayer &player, ICameraSceneNode* Camera)
{

    if(sqrt(pow(Camera->getPosition().X - enemy.Bodynode->getPosition().X,2) + pow(Camera->getPosition().Z - enemy.Bodynode->getPosition().Z,2)) < 50 && enemy.devil == false)
    {
        vector3df toplayer = Camera->getPosition() - enemy.Bodynode->getPosition(), cenresult;
        float resultmag;

        cenresult = (100 * enemy.heading) + toplayer;
        resultmag = sqrt(cenresult.X * cenresult.X + cenresult.Z * cenresult.Z);
        if(resultmag > 100)
        {
            player.is_getting_ass_kicked = true;
            player.health -= 10.f * (1.f + enemy.level / 100.f);
            player.ass_kick = enemy.heading;
            player.ass_kick_timer = .15f;
        }

    }
}

Cenemy explosioncheck(Cenemy enemy[], Cplayer &player, Cexplosion &explosion, ICameraSceneNode* Camera, int &killcount)
{
    int c, k;
    float dist;
    for(c=0; c<200; c++)
        if(enemy[c].health > 0)
        {
//dist = sqrt(pow(enemy[c].Bodynode->getPosition().X - explosion.Explodenode->getPosition().X,2) + pow(enemy[c].Bodynode->getPosition().Z - explosion.Explodenode->getPosition().Z,2));
            dist = sqrt((enemy[c].Bodynode->getPosition().X - explosion.Explodenode->getPosition().X) * (enemy[c].Bodynode->getPosition().X - explosion.Explodenode->getPosition().X) + (enemy[c].Bodynode->getPosition().Z - explosion.Explodenode->getPosition().Z) * (enemy[c].Bodynode->getPosition().Z - explosion.Explodenode->getPosition().Z));
            if(dist <= explosion.range)
            {
                for(k = 0; k < Animation_Number; k++)
                    enemy[c].animation[k] = false;
                enemy[c].health -= explosion.damage - ((explosion.damage / 2.f) * (dist / explosion.range));
                if(enemy[c].health > 0)
                    enemy[c].animation[Is_Being_Attacked] = true;
                if(enemy[c].health <= 0)
                {
                    enemy[c].animation[Is_Being_Attacked] = false;
                    enemy[c].animation[Is_Dead] = true;
                    enemy[c].anitime = 0;
                    killcount += 1;
                }
                enemy[c].hitheading = vector3df(enemy[c].Bodynode->getPosition().X - explosion.Explodenode->getPosition().X,0,enemy[c].Bodynode->getPosition().Z - explosion.Explodenode->getPosition().Z);
                enemy[c].hitheading.normalize();
                enemy[c].anitime = 0;

            }
        }
    if(sqrt(pow(Camera->getPosition().X - explosion.Explodenode->getPosition().X,2) + pow(Camera->getPosition().Z - explosion.Explodenode->getPosition().Z,2)) <= explosion.range)
    {
        float expdamage = explosion.damage - ((explosion.damage / 2.f) * (dist / explosion.range));
        if(expdamage < 0)
            expdamage *= -1.f;
        player.health -= expdamage;
        player.is_getting_ass_kicked = true;
        player.ass_kick = vector3df(1,0,0);
        player.ass_kick_timer = .15f;
    }
    return(*enemy);
}

Cmine minetriggercheck(Cenemy enemy[], Cmine mine[], Cplayer player, Cexplosion &explosion, f32 framedt, int &expcount)
{
    int c,k;
    bool stop = false;
    for(k=0; k<200; k++)
    {
        if(mine[k].state == EXPLODED)
        {
            mine[k].state = INACTIVE;
            mine[k].Minenode->setVisible(false);
            createexplosion(explosion, mine[k].Minenode->getPosition(), mine[k].damage, mine[k].subs, mine[k].range);
        }
        if(mine[k].state == PLACED)
            for(c=0; c<200; c++)
                if(sqrt(pow(enemy[c].Bodynode->getPosition().X - mine[k].Minenode->getPosition().X,2) + pow(enemy[c].Bodynode->getPosition().Z - mine[k].Minenode->getPosition().Z,2)) <= 30)
                {
                    mine[k].timer = mine[k].dettime;
                    mine[k].state = TRIGGERED;
                }
        if(mine[k].state == TRIGGERED)
        {
            mine[k].timer -= framedt;
            if(mine[k].timer <= 0)
            {
                if(stop)
                    mine[k].state = EXPLODED;
                else
                {
                    mine[k].state = INACTIVE;
                    mine[k].Minenode->setVisible(false);
                    createexplosion(explosion, mine[k].Minenode->getPosition(), mine[k].damage, mine[k].subs, mine[k].range);
                    expcount += 1;
                    if(expcount > 199)
                        expcount = 0;
                }
            }
        }
    }
    return(*mine);
}




void devilattackcheck(Cenemy &devil, Cmine mine[], Cmine fakewall[], ICameraSceneNode* Camera, f32 fdt)
{
    //In here the mines are actually barrels... whatever, the return is correct
    int c;
    int closest = 999;
    float dist[200];
    bool shooting = false;


    if(750>= sqrt(pow(Camera->getPosition().X - devil.Bodynode->getPosition().X,2) + pow(Camera->getPosition().Z - devil.Bodynode->getPosition().Z,2)) && devil.animation[Is_Walking] == true)
    {
        devil.heading = vector3df(Camera->getPosition().X - devil.Bodynode->getPosition().X, 0 ,Camera->getPosition().Z - devil.Bodynode->getPosition().Z);
        devil.heading.normalize();
        update(devil,0);
        shooting = true;
    }
    if(shooting == false && devil.animation[Is_Walking] == true)
    {
        closest = 999;
        for(c=0; c<200; c++)
        {
            dist[c] = sqrt(pow(fakewall[c].Minenode->getPosition().X - devil.Bodynode->getPosition().X,2) + pow(fakewall[c].Minenode->getPosition().Z - devil.Bodynode->getPosition().Z,2));
            if(fakewall[c].state == PLACED && 500 >= dist[c])
                if(closest == 999)
                    closest = c;
                else if(dist[c] < dist[closest])
                    closest = c;

        }
        if(closest != 999)
        {
            devil.heading = vector3df(fakewall[closest].Minenode->getPosition().X - devil.Bodynode->getPosition().X, 0 ,fakewall[closest].Minenode->getPosition().Z - devil.Bodynode->getPosition().Z);
            devil.heading.normalize();
            update(devil,0);
            shooting = true;
        }
    }
    if(shooting == false && devil.animation[Is_Walking] == true)
    {
        closest = 999;
        for(c=0; c<200; c++)
        {
            dist[c] = sqrt(pow(mine[c].Minenode->getPosition().X - devil.Bodynode->getPosition().X,2) + pow(mine[c].Minenode->getPosition().Z - devil.Bodynode->getPosition().Z,2));
            if(mine[c].state == PLACED && 500 >= dist[c])
                if(closest == 999)
                    closest = c;
                else if(dist[c] < dist[closest])
                    closest = c;

        }
        if(closest != 999)
        {
            devil.heading = vector3df(mine[closest].Minenode->getPosition().X - devil.Bodynode->getPosition().X, 0 ,mine[closest].Minenode->getPosition().Z - devil.Bodynode->getPosition().Z);
            devil.heading.normalize();
            update(devil,0);
            shooting = true;
        }
    }
    if(shooting == true)
    {
        for(c=0; c<6; c++)
            devil.animation[c] = false;
        devil.animation[Is_Attacking] = true;
        devil.anitime = 0;
        devil.hitcool = 0;
    }
}
Cevents eventsetup(Cevents events[])
{
    int c,k;
    for(c=0; c<199; c++)
    {
        events[c].state = NOT_ACTIVE;
        for(k=0; k<10; k++)
        {
            events[c].complete[k] = false;
            events[c].eventpos[k] = vector3df(0,1000,0);
        }
    }
// xcv
    events[TUTORIAL].messagenum = 1;
    events[TUTORIAL].possloc = 1;
    events[TUTORIAL].range = 200;
    events[TUTORIAL].steps = 1;
    events[TUTORIAL].time = 2;
    events[TUTORIAL].eventpos[0] = vector3df(282, 0, 348);
    /*events[FIND_BLUEPRINTS].messagenum = 2;
    events[FIND_BLUEPRINTS].possloc = 1;
    events[FIND_BLUEPRINTS].range = 100;
    events[FIND_BLUEPRINTS].steps = 1;
    events[FIND_BLUEPRINTS].time = 1;
    events[FIND_BLUEPRINTS].eventpos[0] = vector3df(-1420, 0, 2000);*/
    events[HAMMER_NAILS].messagenum = 3;
    events[HAMMER_NAILS].possloc = 1;
    events[HAMMER_NAILS].range = 150;
    events[HAMMER_NAILS].steps = 1;
    events[HAMMER_NAILS].time = 3;
    events[HAMMER_NAILS].eventpos[0] = vector3df(-2819, 0, -1099);
    events[RETURN_HN].messagenum = 4;
    events[RETURN_HN].possloc = 1;
    events[RETURN_HN].range = 425;
    events[RETURN_HN].steps = 1;
    events[RETURN_HN].time = 2;
    events[RETURN_HN].eventpos[0] = vector3df(1300, 0, 1524);
    events[STEAL_FRIDGE].messagenum = 5;
    events[STEAL_FRIDGE].possloc = 5;
    events[STEAL_FRIDGE].range = 100;
    events[STEAL_FRIDGE].steps = 5;
    events[STEAL_FRIDGE].time = 4;
    events[STEAL_FRIDGE].eventpos[0] = vector3df(-934, 0, 2298);
    events[STEAL_FRIDGE].eventpos[1] = vector3df(-2233, 0, 1840);
    events[STEAL_FRIDGE].eventpos[2] = vector3df(-2047, 0, 999);
    events[STEAL_FRIDGE].eventpos[3] = vector3df(-1469, 0, 506);
    events[STEAL_FRIDGE].eventpos[4] = vector3df(-3236, 0, -2475);
    events[ASSEMBLE_CHASIS].messagenum = 6;
    events[ASSEMBLE_CHASIS].possloc = 5;
    events[ASSEMBLE_CHASIS].range = 425;
    events[ASSEMBLE_CHASIS].steps = 5;
    events[ASSEMBLE_CHASIS].time = 3;
    for(c=0; c<events[ASSEMBLE_CHASIS].steps; c++)
        events[ASSEMBLE_CHASIS].eventpos[c] = vector3df(1300, 0, 1524);
    events[STEAL_ENGINE].messagenum = 7;
    events[STEAL_ENGINE].possloc = 2;
    events[STEAL_ENGINE].range = 300;
    events[STEAL_ENGINE].steps = 2;
    events[STEAL_ENGINE].time = 5;
    events[STEAL_ENGINE].eventpos[0] = vector3df(-430, 0, -700);
    events[STEAL_ENGINE].eventpos[1] = vector3df(-2425, 0, 750);
    events[ASSEMBLE_ENGINE].messagenum = 8;
    events[ASSEMBLE_ENGINE].possloc = 1;
    events[ASSEMBLE_ENGINE].range = 425;
    events[ASSEMBLE_ENGINE].steps = 1;
    events[ASSEMBLE_ENGINE].time = 5;
    events[ASSEMBLE_ENGINE].eventpos[0] = vector3df(1300, 0, 1524);
    events[STEAL_TUB].messagenum = 9;
    events[STEAL_TUB].possloc = 1;
    events[STEAL_TUB].range = 200;
    events[STEAL_TUB].steps = 1;
    events[STEAL_TUB].time = 2;
    events[STEAL_TUB].eventpos[0] = vector3df(-781, 0, 1447);
    events[GET_GAS].messagenum = 10;
    events[GET_GAS].possloc = 1;
    events[GET_GAS].range = 200;
    events[GET_GAS].steps = 1;
    events[GET_GAS].time = 6;
    events[GET_GAS].eventpos[0] = vector3df(1834, 0, -380);
    events[RETURN_TUB].messagenum = 11;
    events[RETURN_TUB].possloc = 1;
    events[RETURN_TUB].range = 425;
    events[RETURN_TUB].steps = 1;
    events[RETURN_TUB].time = 2;
    events[RETURN_TUB].eventpos[0] = vector3df(1300, 0, 1524);
    events[STEAL_VACUUM].messagenum = 12;
    events[STEAL_VACUUM].possloc = 1;
    events[STEAL_VACUUM].range = 200;
    events[STEAL_VACUUM].steps = 1;
    events[STEAL_VACUUM].time = 1;
    events[STEAL_VACUUM].eventpos[0] = vector3df(-876, 0, 1550);
    events[ASSEMBLE_FUEL_PUMP].messagenum = 13;
    events[ASSEMBLE_FUEL_PUMP].possloc = 1;
    events[ASSEMBLE_FUEL_PUMP].range = 425;
    events[ASSEMBLE_FUEL_PUMP].steps = 1;
    events[ASSEMBLE_FUEL_PUMP].time = 6;
    events[ASSEMBLE_FUEL_PUMP].eventpos[0] = vector3df(1300, 0, 1524);
    events[STEAL_STAND].messagenum = 14;
    events[STEAL_STAND].possloc = 1;
    events[STEAL_STAND].range = 200;
    events[STEAL_STAND].steps = 1;
    events[STEAL_STAND].time = 2;
    events[STEAL_STAND].eventpos[0] = vector3df(-671, 0, -2000);
    events[ATTACH_WHEELS].messagenum = 15;
    events[ATTACH_WHEELS].possloc = 3;
    events[ATTACH_WHEELS].range = 425;
    events[ATTACH_WHEELS].steps = 3;
    events[ATTACH_WHEELS].time = 2;
    events[ATTACH_WHEELS].eventpos[0] = vector3df(1300, 0, 1524);
    events[ATTACH_WHEELS].eventpos[1] = vector3df(1300, 0, 1524);
    events[ATTACH_WHEELS].eventpos[2] = vector3df(1300, 0, 1524);
    events[STEAL_BED].messagenum = 16;
    events[STEAL_BED].possloc = 1;
    events[STEAL_BED].range = 150;
    events[STEAL_BED].steps = 1;
    events[STEAL_BED].time = 4;
    events[STEAL_BED].eventpos[0] = vector3df(-2425, 0, 1561);
    events[RETURN_BED].messagenum = 17;
    events[RETURN_BED].possloc = 1;
    events[RETURN_BED].range = 425;
    events[RETURN_BED].steps = 1;
    events[RETURN_BED].time = 1;
    events[RETURN_BED].eventpos[0] = vector3df(1300, 0, 1524);
    events[STEAL_SHUTTERS].messagenum = 18;
    events[STEAL_SHUTTERS].possloc = 10;
    events[STEAL_SHUTTERS].range = 200;
    events[STEAL_SHUTTERS].steps = 8;
    events[STEAL_SHUTTERS].time = 3;
    events[STEAL_SHUTTERS].eventpos[0] = vector3df(-1680, 0, 580);
    events[STEAL_SHUTTERS].eventpos[1] = vector3df(-1680, 0, 725);
    events[STEAL_SHUTTERS].eventpos[2] = vector3df(-1680, 0, 863);
    events[STEAL_SHUTTERS].eventpos[3] = vector3df(-1680, 0, 990);
    events[STEAL_SHUTTERS].eventpos[4] = vector3df(-2220, 0, 977);
    events[STEAL_SHUTTERS].eventpos[5] = vector3df(-2220, 0, 842);
    events[STEAL_SHUTTERS].eventpos[6] = vector3df(-2220, 0, 637);
    events[STEAL_SHUTTERS].eventpos[7] = vector3df(-2220, 0, 504);
    events[ASSEMBLE_PROP].messagenum = 19;
    events[ASSEMBLE_PROP].possloc = 4;
    events[ASSEMBLE_PROP].range = 425;
    events[ASSEMBLE_PROP].steps = 4;
    events[ASSEMBLE_PROP].time = 3;
    for(c=0; c<4; c++)
        events[ASSEMBLE_PROP].eventpos[c] = vector3df(1300, 0, 1524);
    events[STEAL_GATE].messagenum = 20;
    events[STEAL_GATE].possloc = 1;
    events[STEAL_GATE].range = 150;
    events[STEAL_GATE].steps = 1;
    events[STEAL_GATE].time = 2;
    events[STEAL_GATE].eventpos[0] = vector3df(-1690, 0, -258);
    events[ATTACH_ROTOR].messagenum = 21;
    events[ATTACH_ROTOR].possloc = 1;
    events[ATTACH_ROTOR].range = 425;
    events[ATTACH_ROTOR].steps = 1;
    events[ATTACH_ROTOR].time = 4;
    events[ATTACH_ROTOR].eventpos[0] = vector3df(1300, 0, 1524);
    events[STEAL_CONTROL].messagenum = 22;
    events[STEAL_CONTROL].possloc = 1;
    events[STEAL_CONTROL].range = 200;
    events[STEAL_CONTROL].steps = 1;
    events[STEAL_CONTROL].time = 4;
    events[STEAL_CONTROL].eventpos[0] = vector3df(-1425, 0, -318);
    events[INSTALL_CONTROL].messagenum = 23;
    events[INSTALL_CONTROL].possloc = 1;
    events[INSTALL_CONTROL].range = 425;
    events[INSTALL_CONTROL].steps = 1;
    events[INSTALL_CONTROL].time = 4;
    events[INSTALL_CONTROL].eventpos[0] = vector3df(1300, 0, 1524);
    events[CRANK_UP].messagenum = 24;
    events[CRANK_UP].possloc = 1;
    events[CRANK_UP].range = 425;
    events[CRANK_UP].steps = 1;
    events[CRANK_UP].time = 10;
    events[CRANK_UP].eventpos[0] = vector3df(1300, 0, 1524);
    events[TAKE_OFF].messagenum = 25;
    events[TAKE_OFF].possloc = 1;
    events[TAKE_OFF].range = 425;
    events[TAKE_OFF].steps = 1;
    events[TAKE_OFF].time = 1;
    events[TAKE_OFF].eventpos[0] = vector3df(1300, 0, 1524);

    return(*events);
}
