#include "klaviatura.hpp"


KKlaviatura::KKlaviatura()
{
}


KKlaviatura::~KKlaviatura()
{
}


void KKlaviatura::Down(short num)
{
	if(sp[num] < 2)
		sp[num] = 2;
}


void KKlaviatura::Up(short num)
{
	if(sp[num] > 1)
		sp[num] = 1;
}


void KKlaviatura::Atnaujinti()
{
	for(int i = 0; i < SP_SK; i++){
		if(sp[i] == 1)
			sp[i] = 0;
		if(sp[i] == 2)
			sp[i] = 3;
	}
}
