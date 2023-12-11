#include "joystick.hpp"


KJoystick::KJoystick(): sukurta(false)
{
	memset(sp, 0, 32);
	memset(asis, 0, 8 * sizeof(float));
}


KJoystick::~KJoystick()
{
}


void KJoystick::Sukurti(int _num)
{
	if(sukurta)
		Naikinti();
	num = _num;
	joyGetDevCaps(num, &inf, sizeof(JOYCAPS));
	infSpSk = inf.wNumButtons;
	infAsiuSk = inf.wNumAxes;
	infPav = inf.szPname;
	infReg = inf.szRegKey;
	infDrv = inf.szOEMVxD;
	params.dwSize = sizeof(JOYINFOEX);
	params.dwFlags = JOY_RETURNALL;
	sukurta = true;
	Kalibruoti();
}


void KJoystick::Kalibruoti()
{
	//Gaunami parametrai
	joyGetPosEx(num, &params);
	//Irasomos reiksmes
	asiuRibos[0][0] = inf.wXmin;
	asiuRibos[0][1] = inf.wXmax;
	asiuRibos[0][2] = params.dwXpos;
	asiuRibos[1][0] = inf.wYmin;
	asiuRibos[1][1] = inf.wYmax;
	asiuRibos[1][2] = params.dwYpos;
	asiuRibos[2][0] = inf.wZmin;
	asiuRibos[2][1] = inf.wZmax;
	asiuRibos[2][2] = params.dwZpos;
	asiuRibos[3][0] = inf.wRmin;
	asiuRibos[3][1] = inf.wRmax;
	asiuRibos[3][2] = params.dwRpos;
	asiuRibos[4][0] = inf.wUmin;
	asiuRibos[4][1] = inf.wUmax;
	asiuRibos[4][2] = params.dwUpos;
	asiuRibos[5][0] = inf.wVmin;
	asiuRibos[5][1] = inf.wVmax;
	asiuRibos[5][2] = params.dwVpos;
	
	Logas::Log("Kalibruojama (%i): %i %i", num, params.dwXpos, asiuRibos[0][2]);
}


void KJoystick::Atnaujinti()
{
	int i, j = 1;
	joyGetPosEx(num, &params);
	//Atnaujinti spaudukus
	for(i = 0; i < 32; i++){
		if(params.dwButtons & j){
			//Ijungti
			if(sp[i] < 2)
				sp[i] = 2;
			else
				sp[i] = 3;
		}
		else{
			//Isjungti
			if(sp[i] > 1)
				sp[i] = 1;
			else
				sp[i] = 0;
		}
		j <<= 1;
	}
	//Atnaujinti asis
	asis[0] = params.dwXpos;
	asis[1] = params.dwYpos;
	asis[2] = params.dwZpos;
	asis[3] = params.dwRpos;
	asis[4] = params.dwUpos;
	asis[5] = params.dwVpos;
	for(int i = 0; i < 8; i++){
		//Logas::Log("%i %i %i %i", asiuRibos[i][0], asiuRibos[i][1], asiuRibos[i][2], int(asis[i]));
		if(asis[i] >= asiuRibos[i][2])	//Teigiama kryptis
			asis[i] = (asis[i] - asiuRibos[i][2]) / float(asiuRibos[i][1] - asiuRibos[i][2]);
		else							//Neigiama kryptis
			asis[i] = (asiuRibos[i][2] - asis[i]) / float(asiuRibos[i][0] - asiuRibos[i][2]);
	}
	/*asis[0] = (params.dwXpos >= 0) ? (float(params.dwXpos) / float(inf.wXmax)) : (float(params.dwXpos) / float(inf.wXmin));
	asis[1] = (params.dwYpos >= 0) ? (float(params.dwYpos) / float(inf.wYmax)) : (float(params.dwYpos) / float(inf.wYmin));
	asis[2] = (params.dwZpos >= 0) ? (float(params.dwZpos) / float(inf.wZmax)) : (float(params.dwZpos) / float(inf.wZmin));
	asis[3] = (params.dwRpos >= 0) ? (float(params.dwRpos) / float(inf.wRmax)) : (float(params.dwRpos) / float(inf.wRmin));
	asis[4] = (params.dwUpos >= 0) ? (float(params.dwUpos) / float(inf.wUmax)) : (float(params.dwUpos) / float(inf.wUmin));
	asis[5] = (params.dwVpos >= 0) ? (float(params.dwVpos) / float(inf.wVmax)) : (float(params.dwVpos) / float(inf.wVmin));
	*/
	
	/*dwYpoz
	dwZpos
	dwRpos
	dwUpos
	dwVpos
	dwPOV*/
	/*Logas::Log("JoyPoz: %i (%i %i), %i (%i %i), %i (%i %i), %i (%i %i)",
		params.dwXpos, inf.wXmax, inf.wXmin,
		params.dwYpos, inf.wYmax, inf.wYmin,
		params.dwZpos, inf.wZmax, inf.wZmin,
		params.dwRpos, inf.wRmax, inf.wRmin);*/
}


void KJoystick::Naikinti()
{
}
