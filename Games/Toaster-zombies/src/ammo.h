        if(bulletcounter != 0)
        {
            bpos1 = bullet[bulletcounter-1].Bulletnode->getPosition() - bullet[bulletcounter-1].heading * bullet[bulletcounter-1].velocity * frameDeltaTime * .5;
            bpos2 = bullet[bulletcounter-1].Bulletnode->getPosition() + bullet[bulletcounter-1].heading * 10000.f;
        }
        else
        {
            bpos1 = vector3df(0,0,0);
            bpos2 = vector3df(0,0,0);
        }

        if(player.level < killcount)
        {
            player.level = killcount;
            switch(player.level)
            {
            case 3:
                gun[PISTOL].cooldown = 500;
                gun[PISTOL].ammo = gun[PISTOL].maxammo;
                break;
            case 5:
                gun[UZI].unlocked = true;
                gun[UZI].ammo = gun[UZI].maxammo;
                break;
            case 8:
                gun[PISTOL].damage *= 2;
                gun[PISTOL].ammo = gun[PISTOL].maxammo;
                break;
            case 10:
                gun[SHOTGUN].unlocked = true;
                gun[SHOTGUN].ammo = gun[SHOTGUN].maxammo;
                break;
            case 13:
                gun[UZI].cooldown = 150;
                gun[UZI].ammo = gun[UZI].maxammo;
                break;
            case 15:
                gun[BARREL].unlocked = true;
                gun[BARREL].ammo = gun[BARREL].maxammo;
                break;
            case 17:
                gun[UZI].maxammo *= 2;
                gun[UZI].ammo = gun[UZI].maxammo;
                break;
            case 18:
                gun[SHOTGUN].cooldown = 300;
                gun[SHOTGUN].ammo = gun[SHOTGUN].maxammo;
                break;
            case 21:
                gun[SHOTGUN].maxammo *= 2;
                gun[SHOTGUN].ammo = gun[SHOTGUN].maxammo;
                break;
            case 23:
                gun[UZI].range *= 2;
                gun[UZI].ammo = gun[UZI].maxammo;
                break;
            case 26:
                gun[BARREL].maxammo *= 2;
                gun[BARREL].ammo = gun[BARREL].maxammo;
                break;
            case 30:
                gun[FAKEWALL].unlocked = true;
                gun[FAKEWALL].ammo = gun[FAKEWALL].maxammo;
                break;
            case 32:
                for(k=0; k<200; k++)
                    barrels[k].range = 130;
                gun[BARREL].ammo = gun[BARREL].maxammo;
                break;
            case 35:
                gun[SHOTGUN].range *= 2;
                gun[SHOTGUN].ammo = gun[SHOTGUN].maxammo;
                break;
            case 36:
                gun[BARREL].maxammo *= 2;
                gun[BARREL].ammo = gun[BARREL].maxammo;
                break;
            case 37:
                gun[FAKEWALL].maxammo *= 2;
                gun[FAKEWALL].ammo = gun[FAKEWALL].maxammo;
                break;
            case 39:
                gun[UZI].maxammo *= 2;
                gun[UZI].ammo = gun[UZI].maxammo;
                break;
            case 40:
                gun[CLAYMORE].unlocked = true;
                gun[CLAYMORE].ammo = gun[CLAYMORE].maxammo;
                break;
            case 41:
                gun[SHOTGUN].maxammo *= 2;
                gun[SHOTGUN].ammo = gun[SHOTGUN].maxammo;
                break;
            case 43:
                gun[SHOTGUN].cooldown = 175;
                gun[SHOTGUN].ammo = gun[SHOTGUN].maxammo;
                break;
            case 44:
                for(k=0; k<200; k++)
                    barrels[k].range = 180;
                gun[BARREL].ammo = gun[BARREL].maxammo;
                break;
            case 47:
                for(k=0; k<200; k++)
                    mines[k].subs = 3;
                gun[CLAYMORE].ammo = gun[CLAYMORE].maxammo;
                break;
            case 48:
                gun[UZI].damage *= 2;
                gun[UZI].ammo = gun[UZI].maxammo;
                break;
            case 50:
                gun[ROCKET].unlocked = true;
                gun[ROCKET].ammo = gun[ROCKET].maxammo;
                break;
            case 53:
                gun[FAKEWALL].maxammo *= 2;
                gun[FAKEWALL].ammo = gun[FAKEWALL].maxammo;
                break;
            case 54:
                gun[CLAYMORE].maxammo *= 2;
                gun[CLAYMORE].ammo = gun[CLAYMORE].maxammo;
                break;
            case 55:
                gun[CHARGE].unlocked = true;
                gun[CHARGE].ammo = gun[CHARGE].maxammo;
                break;
            case 56:
                gun[SHOTGUN].damage *= 2;
                gun[SHOTGUN].ammo = gun[SHOTGUN].maxammo;
                break;
            case 58:
                for(k=0; k<200; k++)
                    mines[k].range = 130;
                gun[CLAYMORE].ammo = gun[CLAYMORE].maxammo;
                break;
            case 59:
                gun[ROCKET].cooldown = 400;
                gun[ROCKET].ammo = gun[ROCKET].maxammo;
                break;
            case 61:
                gun[UZI].range *= 10;
                gun[UZI].ammo = gun[UZI].maxammo;
                break;
            case 62:
                for(k=0; k<200; k++)
                    mines[k].range = 180;
                gun[CLAYMORE].ammo = gun[CLAYMORE].maxammo;
                break;
            case 63:
                for(k=0; k<200; k++)
                    charges[k].subs = 3;
                gun[CHARGE].ammo = gun[CHARGE].maxammo;
                break;
            case 64:
                gun[CLAYMORE].maxammo *= 2;
                gun[CLAYMORE].ammo = gun[CLAYMORE].maxammo;
                break;
            case 66:
                gun[ROCKET].maxammo *= 2;
                gun[ROCKET].ammo = gun[ROCKET].maxammo;
                break;
            case 68:
                gun[CHARGE].maxammo *= 2;
                gun[CHARGE].ammo = gun[CHARGE].maxammo;
                break;
            case 70:
                gun[RAILGUN].unlocked = true;
                gun[RAILGUN].ammo = gun[RAILGUN].maxammo;
                break;
            case 72:
//in the hitcheck explosion creator, the range of the explosion gets set to 130
                gun[ROCKET].ammo = gun[ROCKET].maxammo;
            case 74:
                for(k=0; k<200; k++)
                    charges[k].range = 130;
                gun[CHARGE].ammo = gun[CHARGE].maxammo;
                break;
            case 76:
                gun[CHARGE].maxammo *= 2;
                gun[CHARGE].ammo = gun[CHARGE].maxammo;
                break;
            case 78:
                gun[RAILGUN].cooldown = 200;
                gun[RAILGUN].ammo = gun[RAILGUN].maxammo;
                break;
            case 80:
                gun[RAILGUN].maxammo *= 2;
                gun[RAILGUN].ammo = gun[RAILGUN].maxammo;
                break;
            case 85:
                gun[ROCKET].maxammo *= 2;
                gun[ROCKET].ammo = gun[ROCKET].maxammo;
                break;
            case 90:
                gun[UZI].damage *= 2;
                gun[UZI].ammo = gun[UZI].maxammo;
                break;
            case 95:
                for(k=0; k<200; k++)
                    charges[k].range = 180;
                gun[CHARGE].ammo = gun[CHARGE].maxammo;
                break;
            case 100:
                gun[RAILGUN].cooldown = 100;
                gun[RAILGUN].ammo = gun[RAILGUN].maxammo;
                break;
            case 105:
//in the hitcheck explosion creator, the range of the explosion gets set to 180
                gun[ROCKET].ammo = gun[ROCKET].maxammo;
                break;
            case 110:
                gun[RAILGUN].maxammo *= 2;
                gun[RAILGUN].ammo = gun[RAILGUN].maxammo;
                break;
            case 115:
//in the hitcheck explosion creator, the subs of the explosion gets set to 3;
                gun[ROCKET].ammo = gun[ROCKET].maxammo;
                break;
            case 120:
                gun[ROCKET].cooldown = 200;
                gun[ROCKET].ammo = gun[ROCKET].maxammo;
                break;
            case 125:
                gun[RAILGUN].range = 2000;
                gun[RAILGUN].ammo = gun[RAILGUN].maxammo;
                break;
            }
        }
