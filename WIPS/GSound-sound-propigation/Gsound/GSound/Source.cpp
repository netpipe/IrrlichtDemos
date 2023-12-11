#include "Source.h"
#include "SoundFile.h"
#include "Ray.h"
#include "iostream"
using namespace std;

Source::Source(void)
{
	position.x=5;
	position.y=5;
}


Source::~Source(void)
{
}

/*void Source::PropagateSoundFile(SoundFile soundfile)
{
	//Assuming Propogate sample like "bib" and store it in sound Array wiht angle index, and for the next sample "bob" I overwrite on the same array
	for (int i = 0; i < soundfile.Length; i++)
	{
		//propagate the "bib" throughout the room and save from 1 -> maxDepth ray's Parameter in the array to deal with it later "delay issues and listner real-time problem"
		//angle here is the base_source raya angle
		//Œ“‰  ﬂ·‘Ì «‰ ‘«— ·’Ê  »ÌÌ» »ﬂ· «·œÌ»’«   »⁄« Ê
		for (int angle = 0; angle < 360; angle+=24)
		{
			Ray ray= Ray(position,angle,1,10);
			sound[angle].push_back(ray);
			while (ray.Weigtht>0){
				if (ray.Detect_Object())
				{
					Position Common_solution = ray.Detect_Object(1);
					Ray TempRay=ray.RelfetiveRay(Common_solution);
					sound[angle].push_back(TempRay);
				}
				else 
				{
					break;
				}
			}
		}
	}
}*/