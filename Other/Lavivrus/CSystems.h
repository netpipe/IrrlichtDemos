class CSystem{
public:
	char Name[10];
	char Species[10];
	uint Age;		// negative age for checking if avail .. ? just a thought.
	bool Enabled;
	bool Move_Right;
	bool Move_Left;
	bool Move_Forward;
	bool Move_Backward;
	bool Move_Jump;
	CSystem() : Speed(0.09f){};
	float Speed;
	float Agility;
	float Strenth;
	float Stamina;
	float Air;
	float Health;
	int PowerUp;
	int CWeapon;
	bool PWeapons[14];
};

CSystem P1;

void KeyDown( SDL_keysym *keysym ){
    switch ( keysym->sym )
	{
	case SDLK_ESCAPE:
	done=TRUE;
	break;
	case SDLK_F1:
	break;
	case SDLK_SPACE:
	P1.Move_Jump=TRUE;
	break;
	case SDLK_RIGHT:
	P1.Move_Right=TRUE;
	break;
	case SDLK_LEFT:
	P1.Move_Left=TRUE;
	break;
	case SDLK_UP:
	P1.Move_Forward=TRUE;
	break;
	case SDLK_DOWN:
	P1.Move_Backward=TRUE;
	break;

	}
}

void KeyUp(SDL_keysym *keysym){


 switch(keysym->sym)
	{
	case SDLK_ESCAPE:
	done=TRUE;
	break;
	case SDLK_F1:
	SDL_WM_ToggleFullScreen( surface );
	break;
	case SDLK_SPACE:
	P1.Move_Jump=FALSE;
	break;
	case SDLK_RIGHT:
	P1.Move_Right=FALSE;
	break;
	case SDLK_LEFT:
	P1.Move_Left=FALSE;
	break;
	case SDLK_UP:
	P1.Move_Forward=FALSE;
	break;
	case SDLK_DOWN:
	P1.Move_Backward=FALSE;
	break;
	}
}

void KeyBoardControl(){
	if (P1.Move_Forward){
		xpos += ( float )sin( yrot * piover180 ) * P1.Speed;
		zpos += ( float )cos( yrot * piover180 ) * P1.Speed;
		if ( walkbiasangle >= 359.0f )
			walkbiasangle = 0.0f;
	    	else
			walkbiasangle+= 10;
		walkbias = ( float )sin( walkbiasangle * piover180 ) / 20.0f;
	}

	if (P1.Move_Backward){
		xpos -= ( float )sin( yrot * piover180 ) * P1.Speed;
		zpos -= ( float )cos( yrot * piover180 ) * P1.Speed;
		if ( walkbiasangle <= 1.0f )
			walkbiasangle = 359.0f;
		else
			walkbiasangle -= 10;
			walkbias = ( float )sin( walkbiasangle * piover180 ) / 20.0f;
	}

	if (P1.Move_Jump){jump+=0.5;}

	if (P1.Move_Left){
		xpos += ( float )(sin((yrot+90)* piover180) * P1.Speed);
		zpos += ( float )(cos((yrot+90)* piover180) * P1.Speed);
	}

	if (P1.Move_Right){
		xpos -= ( float )(sin((yrot+90)* piover180) * P1.Speed);
		zpos -= ( float )(cos((yrot+90)* piover180) * P1.Speed);
	}

}
