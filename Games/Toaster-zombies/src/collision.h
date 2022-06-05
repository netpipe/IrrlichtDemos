Cenemy hitcheck(Cbullet &bullet, Cenemy enemy[], Cmine barrel[], Cmine fakewall[], int &zarg, int &zargnum, Cexplosion &explosion, int &explodenum, ICameraSceneNode* Camera, f32 frameps, Cplayer &player, int &killcount)
{
    zarg = 0;
    zargnum = 999;
    int closest = 999, klosest = 999, jlosest = 999, c,k,j, interps = (int)ceil((bullet.velocity/frameps) / 3.0f);
    float distance[200], tdist, tlimit = 0, blimit = 0, hitrange = 0;
    bool fixer;
    vector3df bodyframe[4], pseudobullet, pseudoenemy;
    for(k = 0; k < interps; k++)
    {
        fixer = false;
        tdist = 999;
        if(bullet.Bulletnode->getPosition().Y <= 80 && bullet.Bulletnode->getPosition().Y >= -20)
        {
            closest = 999;
            for(c = 0; c < 200; c++)
                if(enemy[c].health > 0)
                {
                    if(closest == 999)
                    {
                        closest = c;
                        distance[c] = sqrt(pow(enemy[c].Bodynode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(enemy[c].Bodynode->getPosition().X - bullet.Bulletnode->getPosition().X, 2));
                    }
                    else
                    {
                        distance[c] = sqrt(pow(enemy[c].Bodynode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(enemy[c].Bodynode->getPosition().X - bullet.Bulletnode->getPosition().X, 2));
                        if(distance[c] < distance[closest])
                            closest = c;
                    }
                }
        }
        if(closest == 999)
        {
            fixer = true;
            pseudoenemy = enemy[99].Bodynode->getPosition();
            enemy[99].Bodynode->setPosition(vector3df(12345,12345,12345));
        }
        if(bullet.Bulletnode->getPosition().Y <= 50 && bullet.Bulletnode->getPosition().Y >= -10)
        {
            klosest = 999;
            hitrange = 22.f;
            if(bullet.gorigin == ROCKET)
                hitrange += 7.f;
            for(c = 0; c < 200; c++)
                if(barrel[c].state == PLACED)
                {
                    if(klosest == 999)
                    {
                        klosest = c;
                        distance[c] = sqrt(pow(barrel[c].Minenode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(barrel[c].Minenode->getPosition().X - bullet.Bulletnode->getPosition().X, 2));
                    }
                    else
                    {
                        distance[c] = sqrt(pow(barrel[c].Minenode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(barrel[c].Minenode->getPosition().X - bullet.Bulletnode->getPosition().X, 2));
                        if(distance[c] < distance[klosest])
                            klosest = c;
                    }
                }
            if(klosest !=999 && bullet.Bulletnode->getPosition().Y <= barrel[klosest].Minenode->getPosition().Y + 48 && bullet.Bulletnode->getPosition().Y >= barrel[klosest].Minenode->getPosition().Y &&
            hitrange >= sqrt(pow(barrel[klosest].Minenode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(barrel[klosest].Minenode->getPosition().X - bullet.Bulletnode->getPosition().X, 2)) &&
            sqrt(pow(enemy[closest].Bodynode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(enemy[closest].Bodynode->getPosition().X - bullet.Bulletnode->getPosition().X, 2)) > sqrt(pow(barrel[klosest].Minenode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(barrel[klosest].Minenode->getPosition().X - bullet.Bulletnode->getPosition().X, 2)))
            {
                zarg = 1;
                zargnum = klosest;
                if(bullet.gorigin != 9)
                    bullet.hit = true;
                k = interps;
                closest = 999;
                klosest = 999;
            }
        }
        if(bullet.Bulletnode->getPosition().Y <= 62 && bullet.Bulletnode->getPosition().Y >= -10 && k != interps)
        {
            jlosest = 999;
            hitrange = 22.f;
            if(bullet.gorigin == ROCKET)
                hitrange += 7.f;
            for(c = 0; c < 200; c++)
                if(fakewall[c].state == PLACED)
                {
                    if(jlosest == 999)
                    {
                        jlosest = c;
                        distance[c] = sqrt(pow(fakewall[c].Minenode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(fakewall[c].Minenode->getPosition().X - bullet.Bulletnode->getPosition().X, 2));
                    }
                    else
                    {
                        distance[c] = sqrt(pow(fakewall[c].Minenode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(fakewall[c].Minenode->getPosition().X - bullet.Bulletnode->getPosition().X, 2));
                        if(distance[c] < distance[jlosest])
                            jlosest = c;
                    }
                }
            if(jlosest !=999 && bullet.Bulletnode->getPosition().Y <= fakewall[jlosest].Minenode->getPosition().Y + 62 && bullet.Bulletnode->getPosition().Y >= fakewall[jlosest].Minenode->getPosition().Y &&
            hitrange >= sqrt(pow(fakewall[jlosest].Minenode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(fakewall[jlosest].Minenode->getPosition().X - bullet.Bulletnode->getPosition().X, 2)) &&
            sqrt(pow(enemy[closest].Bodynode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(enemy[closest].Bodynode->getPosition().X - bullet.Bulletnode->getPosition().X, 2)) > sqrt(pow(fakewall[jlosest].Minenode->getPosition().Z - bullet.Bulletnode->getPosition().Z, 2) + pow(fakewall[jlosest].Minenode->getPosition().X - bullet.Bulletnode->getPosition().X, 2)))
            {
                zarg = 2;
                zargnum = jlosest;
                if(bullet.gorigin != 9)
                    bullet.hit = true;
                k = interps;
                closest = 999;
                klosest = 999;
            }
        }
        if(fixer == true)
        {
            closest = 999;
            enemy[99].Bodynode->setPosition(pseudoenemy);
            fixer = false;
        }
        if(closest != 999)
        {
            tdist = 0;
            if(enemy[closest].devil == true)
            {
                tlimit = enemy[closest].Bodynode->getPosition().Y + 33.f;
                blimit = enemy[closest].Bodynode->getPosition().Y - 16.f;
                hitrange = 85.6f;
                bodyframe[0].Z = enemy[closest].Bodynode->getPosition().Z + (11 * enemy[closest].heading.Z) - (14 * enemy[closest].heading.X);
                bodyframe[0].X = enemy[closest].Bodynode->getPosition().X + (14 * enemy[closest].heading.Z) + (11 * enemy[closest].heading.X);
                bodyframe[1].Z = enemy[closest].Bodynode->getPosition().Z - (11 * enemy[closest].heading.Z) - (14 * enemy[closest].heading.X);
                bodyframe[1].X = enemy[closest].Bodynode->getPosition().X + (14 * enemy[closest].heading.Z) - (11 * enemy[closest].heading.X);
                bodyframe[2].Z = enemy[closest].Bodynode->getPosition().Z - (11 * enemy[closest].heading.Z) + (14 * enemy[closest].heading.X);
                bodyframe[2].X = enemy[closest].Bodynode->getPosition().X - (14 * enemy[closest].heading.Z) - (11 * enemy[closest].heading.X);
                bodyframe[3].Z = enemy[closest].Bodynode->getPosition().Z + (11 * enemy[closest].heading.Z) + (14 * enemy[closest].heading.X);
                bodyframe[3].X = enemy[closest].Bodynode->getPosition().X - (14 * enemy[closest].heading.Z) + (11 * enemy[closest].heading.X);
            }
            else
            {
                tlimit = enemy[closest].Bodynode->getPosition().Y + 14.f;
                blimit = enemy[closest].Bodynode->getPosition().Y - 18.f;
                hitrange = 73.5f;
                bodyframe[0].Z = enemy[closest].Bodynode->getPosition().Z + (9 * enemy[closest].heading.Z) - (13 * enemy[closest].heading.X);
                bodyframe[0].X = enemy[closest].Bodynode->getPosition().X + (13 * enemy[closest].heading.Z) + (9 * enemy[closest].heading.X);
                bodyframe[1].Z = enemy[closest].Bodynode->getPosition().Z - (9 * enemy[closest].heading.Z) - (13 * enemy[closest].heading.X);
                bodyframe[1].X = enemy[closest].Bodynode->getPosition().X + (13 * enemy[closest].heading.Z) - (9 * enemy[closest].heading.X);
                bodyframe[2].Z = enemy[closest].Bodynode->getPosition().Z - (9 * enemy[closest].heading.Z) + (13 * enemy[closest].heading.X);
                bodyframe[2].X = enemy[closest].Bodynode->getPosition().X - (13 * enemy[closest].heading.Z) - (9 * enemy[closest].heading.X);
                bodyframe[3].Z = enemy[closest].Bodynode->getPosition().Z + (9 * enemy[closest].heading.Z) + (13 * enemy[closest].heading.X);
                bodyframe[3].X = enemy[closest].Bodynode->getPosition().X - (13 * enemy[closest].heading.Z) + (9 * enemy[closest].heading.X);
            }
            if(bullet.gorigin == ROCKET)
                hitrange += 7;
            for(j=0; j<4; j++)
                tdist += sqrt(pow(bodyframe[j].X - bullet.Bulletnode->getPosition().X, 2) + pow(bodyframe[j].Z - bullet.Bulletnode->getPosition().Z, 2));
        }
//if(tdist <= 73.5 && bullet.Bulletnode->getPosition().Y <= enemy[closest].Bodynode->getPosition().Y + 14.5 && bullet.Bulletnode->getPosition().Y >= enemy[closest].Bodynode->getPosition().Y - 14.5 && closest != 999) // 73.5 is the approximate sum of the max distance from all corners to one corner of this rectangle
        if(tdist <= hitrange && bullet.Bulletnode->getPosition().Y <= tlimit && bullet.Bulletnode->getPosition().Y >= blimit && closest != 999) // 73.5 is the approximate sum of the max distance from all corners to one corner of this rectangle
        {
            if(bullet.gorigin != DEVIL || enemy[closest].devil == false)
            {
                enemy[closest].animation[Is_Walking] = false;
                enemy[closest].animation[Is_Being_Attacked] = true;
                enemy[closest].anitime = 0;
                enemy[closest].health -= bullet.damage;
                enemy[closest].hitheading = bullet.heading;
                enemy[closest].hitheading.Y = 0;
                enemy[closest].hitheading.normalize();
                if(enemy[closest].health <= 0)
                {
                    killcount += 1;
                    enemy[closest].animation[Is_Being_Attacked] = false;
                    enemy[closest].animation[Is_Dead] = true;
                    enemy[closest].anitime = 0;
                }
                if(bullet.gorigin != 9)
                {
                    bullet.hit = true;
                    k = interps;
                }
            }

        }
        if(closest != 999 && k != interps)
        {
            tdist = 0;
            if(enemy[closest].devil == true)
            {
                tlimit = enemy[closest].Bodynode->getPosition().Y + 57.f;
                blimit = enemy[closest].Bodynode->getPosition().Y + 33.f;
                hitrange = 75.1f;
                bodyframe[0].Z = enemy[closest].Bodynode->getPosition().Z + (11 * enemy[closest].heading.Z) - (11 * enemy[closest].heading.X);
                bodyframe[0].X = enemy[closest].Bodynode->getPosition().X + (11 * enemy[closest].heading.Z) + (11 * enemy[closest].heading.X);
                bodyframe[1].Z = enemy[closest].Bodynode->getPosition().Z - (11 * enemy[closest].heading.Z) - (11 * enemy[closest].heading.X);
                bodyframe[1].X = enemy[closest].Bodynode->getPosition().X + (11 * enemy[closest].heading.Z) - (11 * enemy[closest].heading.X);
                bodyframe[2].Z = enemy[closest].Bodynode->getPosition().Z - (11 * enemy[closest].heading.Z) + (11 * enemy[closest].heading.X);
                bodyframe[2].X = enemy[closest].Bodynode->getPosition().X - (11 * enemy[closest].heading.Z) - (11 * enemy[closest].heading.X);
                bodyframe[3].Z = enemy[closest].Bodynode->getPosition().Z + (11 * enemy[closest].heading.Z) + (11 * enemy[closest].heading.X);
                bodyframe[3].X = enemy[closest].Bodynode->getPosition().X - (11 * enemy[closest].heading.Z) + (11 * enemy[closest].heading.X);
            }
            else
            {
                tlimit = enemy[closest].Bodynode->getPosition().Y + 57.f;
                blimit = enemy[closest].Bodynode->getPosition().Y + 33.f;
                hitrange = 73.f;
                bodyframe[0].Z = enemy[closest].Bodynode->getPosition().Z + (10 * enemy[closest].heading.Z) - (11 * enemy[closest].heading.X);
                bodyframe[0].X = enemy[closest].Bodynode->getPosition().X + (11 * enemy[closest].heading.Z) + (10 * enemy[closest].heading.X);
                bodyframe[1].Z = enemy[closest].Bodynode->getPosition().Z - (10 * enemy[closest].heading.Z) - (11 * enemy[closest].heading.X);
                bodyframe[1].X = enemy[closest].Bodynode->getPosition().X + (11 * enemy[closest].heading.Z) - (10 * enemy[closest].heading.X);
                bodyframe[2].Z = enemy[closest].Bodynode->getPosition().Z - (10 * enemy[closest].heading.Z) + (11 * enemy[closest].heading.X);
                bodyframe[2].X = enemy[closest].Bodynode->getPosition().X - (11 * enemy[closest].heading.Z) - (10 * enemy[closest].heading.X);
                bodyframe[3].Z = enemy[closest].Bodynode->getPosition().Z + (10 * enemy[closest].heading.Z) + (11 * enemy[closest].heading.X);
                bodyframe[3].X = enemy[closest].Bodynode->getPosition().X - (11 * enemy[closest].heading.Z) + (10 * enemy[closest].heading.X);
            }
            if(bullet.gorigin == ROCKET)
                hitrange += 7;
            for(j=0; j<4; j++)
                tdist += sqrt(pow(bodyframe[j].X - bullet.Bulletnode->getPosition().X, 2) + pow(bodyframe[j].Z - bullet.Bulletnode->getPosition().Z, 2));
        }
//if(tdist <= 73 && bullet.Bulletnode->getPosition().Y <= enemy[closest].Bodynode->getPosition().Y + 34 && bullet.Bulletnode->getPosition().Y >= enemy[closest].Bodynode->getPosition().Y + 16 && closest != 999 && k != interps) // 71.7 is the approximate sum of the max distance from all corners to one corner of this rectangle
        if(tdist <= hitrange && bullet.Bulletnode->getPosition().Y <= tlimit && bullet.Bulletnode->getPosition().Y >= blimit && closest != 999 && k != interps) // 71.7 is the approximate sum of the max distance from all corners to one corner of this rectangle
        {
            enemy[closest].animation[Is_Walking] = false;
            enemy[closest].animation[Is_Being_Attacked] = true;
            enemy[closest].anitime = 0;
            enemy[closest].health -= bullet.damage * 2.f;
            enemy[closest].hitheading = bullet.heading;
            enemy[closest].hitheading.Y = 0;
            enemy[closest].hitheading.normalize();
            if(enemy[closest].health <= 0)
            {
                enemy[closest].animation[Is_Being_Attacked] = false;
                enemy[closest].animation[Is_Dead] = true;
                enemy[closest].anitime = 0;
                killcount += 1;
            }
            if(bullet.gorigin != 9)
            {
                bullet.hit = true;
                k = interps;
            }

        }
        if(bullet.gorigin == DEVIL && bullet.hit == false && k != interps)
            if(20 >= sqrt(pow(Camera->getPosition().X-bullet.Bulletnode->getPosition().X,2) + pow(Camera->getPosition().Z-bullet.Bulletnode->getPosition().Z,2)))
            {
                bullet.hit = true;
                player.is_getting_ass_kicked = true;
                player.health -= bullet.damage;
                player.ass_kick = bullet.heading;
                player.ass_kick_timer = .15f;
            }
        if(bullet.Bulletnode->getPosition().Y <= 5 && bullet.hit == false && k!=interps)
            bullet.hit = true;

        if(k != interps)
        {
            pseudobullet = bullet.Bulletnode->getPosition();
            bullet.Bulletnode->setPosition(bullet.Bulletnode->getPosition() + (bullet.velocity / interps) * bullet.heading * 1.0 / frameps);
            if(bullet.Bulletnode->getPosition() != pseudobullet + (bullet.velocity / interps) * bullet.heading * 1.0 / frameps)
            {
                bullet.hit = true;
                k = interps;
            }

        }
    }
    if(bullet.gorigin == 7 && bullet.hit == true)
    {
        int subs = 0;
        float range = 80;
        if(player.level >= 115)
            subs = 3;
        if(player.level >= 72)
            range = 130;
        if(player.level >= 105)
            range = 180;
        createexplosion(explosion, bullet.Bulletnode->getPosition(), bullet.damage, subs, range);
        explodenum += 1;
        if(explodenum >199)
            explodenum = 0;
    }
    if(bullet.hit == true)
    {
        bullet.Bulletnode->setPosition(vector3df(0,10000,0));
        bullet.Bulletnode->setVisible(false);
    }
    return(*enemy);
}
