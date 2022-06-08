// PROGRAMME DE TEST, PORTAGE DU BUMP SOUS SDL.
// Auteur: Lepinay Laurent ( sigo )
// mail to: sigo@fleming.u-psud.fr

// A definir pr la compil linux.
#define LINUX_COMPIL

#include <SDL/SDL.h>
#include <math.h>
#include <stdio.h>

#ifndef LINUX_COMPIL
#include <windows.h> // pour exit()
#else
#include <unistd.h>
#endif





int RgbToInt( int r, int g, int b )
{
	return (r << 16) | (g << 8) | b;
}

//----------------------------------------------------------------------//
// loadpcx - Charge en memoire un fichier .PCX                          //
//----------------------------------------------------------------------//
int loadpcx(char *nomfich,unsigned long taille,int *image)
{
	unsigned char data, nb_octets;
	unsigned long index = 0;
	unsigned int  indexrle;
	FILE *fichpcx;

	if (!(fichpcx = fopen(nomfich, "rb"))) 
	{
		exit(1);
	}
	fseek(fichpcx, 128, SEEK_SET);
	
	do
	{
		fread(&data, 1, 1, fichpcx);
		if ((data & 0xC0) == 0xC0)
		{
			nb_octets = (data & 0x3F);
			fread(&data,1,1,fichpcx);
			for (indexrle=1;indexrle<=nb_octets;indexrle++) image[index++]= (data << 16) | (data << 8) | data ;
		}
		else image[index++] = (data << 16) | (data << 8) | data ;
	} while(index < taille);
	
	fclose(fichpcx);
	return 1;
}

int main( int argc, char* argv[] )
{
	// OLD BUMP PART
	int nEnvMap[256*256];
	int image[320*200];

	int pal[256];
	int x, y, i;
	int dX, dY;
	int lX, lY;
	float fTime = 0.0f;

	// SDL data
	int bQuit = 0;
	SDL_Surface *buffer = NULL;
	SDL_Event event;


	if( SDL_Init(SDL_INIT_VIDEO) == -1 )
	{
#ifndef LINUX_COMPIL
		MessageBox( NULL, "Erreur1", "Erreur", MB_OK );
#endif
		return -1;
	}
	
	if( SDL_InitSubSystem(SDL_INIT_VIDEO) == -1 )
	{
#ifndef LINUX_COMPIL
		MessageBox( NULL, "Erreur2", "Erreur", MB_OK );
#endif
		return -1;
	}
	
	if( ! (buffer = SDL_SetVideoMode(320, 200, 32, SDL_DOUBLEBUF))  )
	{
#ifndef LINUX_COMPIL
		MessageBox( NULL, "Erreur3", "Erreur", MB_OK );
#endif
		return -1;
	}
	
	// Generation de l'environment map.
	// Elle va nous donner le niveau d'ullimination
	// a utiliser pour une orientation de pixel.
	// Cette table na pas besoin d'être optimisée.
	for( y = 0; y < 256; y++ )
	{
		for( x = 0; x < 256; x++ )
		{
			// Le calcul de distance au centre se fait par 
			// rapport au centre (128,128), il faut donc
			// changer de repère.
			nEnvMap[ x + 256*y ] = 
				255 - 255 * sqrt( (x-128)*(x-128) + (y-128)*(y-128) )/
				            sqrt( 128*128 + 128*128 );
		}
	}

	// Génération d'une palette qui va nous donner la couleur
	// a utiliser en fonction d'un niveau d'ullimination donné.
	for( i = 0; i < 64; i++ )
	{
		pal[i] = 0;
	}
	for( i = 64; i < 128; i++ )
	{
		pal[i] = RgbToInt( 0, 0, (i-64)*4);
	}
	for( i = 128; i < 256; i++ )
	{
		pal[i] = RgbToInt( (i-128)*2, (i-128)*2, 255);
	}

	// Ouverture de l'image qui va être bumpée
	loadpcx("bump.pcx",320*200,image);
	
	
	lX = 160 + 80*cos(fTime += .1);
	lY = 100 + 80*sin(fTime);

	// Génération du bump mapping.
	do
	{
		SDL_LockSurface(buffer);

		for( y = 1; y < 199; y++ )
		{
			for( x = 1; x < 319; x++ )
			{
				// On calcule l'orientation du pixel.
				dX = (image[ x + 1 + 320 * y ]>>16) - (image[ x - 1 + 320 * y ]>>16);
				dY = (image[ x + 320 * ( y + 1 ) ]>>16) - (image[ x + 320 * ( y - 1) ]>>16);

				// On récupère le niveau d'illumination correspondant.
				// puis la couleur qui lui est associée.

				// On effectue un double changement de repère.
				dX = dX - ( lX - x );
				dY = dY - ( lY - y );

				// On clip les coordonées.
				if( dX <= -128 || dX >= 128 ) dX = dY = -128;
				if( dY <= -128 || dY >= 128 ) dX = dY = -128;

				// On retourne dans un repère classique.
				dX += 128;
				dY += 128;

				
				// UPDTATE dans pixel
				((int*)buffer->pixels)[ x + 320*y] = pal[nEnvMap[ dX + 256*dY]];

			}
		}

		SDL_UnlockSurface( buffer );

		//SDL_UpdateRects(buffer, 1, &rect);
		SDL_UpdateRect(buffer, 0, 0, 0, 0);




		lX = 160 + 80*cos(fTime += .1);
		lY = 100 + 80*sin(fTime);

		// Gestion des evenement, histoire qu'on se kasse
		// un jour de cette boucle.

		/* Check for events */
		while(SDL_PollEvent(&event))
		{  
			/* Loop until there are no events left on the queue */
			switch(event.type)
			{  
				case SDL_QUIT:  
				bQuit = 1;
				break;    
			}
		}


	}
	while(!bQuit);


	SDL_Quit();
	
	
	return 0;
}
