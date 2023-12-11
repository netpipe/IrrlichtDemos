#ifndef TEXTUREFACTORY_H_04112006
#define TEXTUREFACTORY_H_04112006

#ifndef __APPLE__
    #include <GL/glew.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#else
    #include <GL/glew.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#include "../FileSystem/FileSystem.h"
#include "../Scripting/Headers/TextureFactory.h"
#include "../GUI/Console.h"
#include <map>

#define PNG_OK		 0
#define PNG_FILEIOERROR  1
#define PNG_MALLOCERROR  2
#define PNG_JUMPSETERROR 3

typedef struct Texture
{
    int TextureID;
    int width;
    int height;
};

//! Texture factory class, globalised with texture_factory
class TextureFactory : public azTextureFactory
{
	public:
		TextureFactory(const std::string &workingDirectory);
		~TextureFactory();
		void loadTexture(const std::string& filename);

		inline void applyTexture(const std::string &fileName)
        {
            if (fileName == "")
                return;
            std::string texname(std::string("./") + workingDirectory + std::string("/Textures/") + fileName);
            glBindTexture(GL_TEXTURE_2D, textures[texname]);
        }

        inline GLuint getTextureID(const std::string &fileName)
        {
            if (fileName == "")
                return 0;

            std::string texname(std::string("./") + workingDirectory + std::string("/Textures/") + fileName);
            return textures[texname];
        }

		void deleteTexture(const std::string& filename);
		void flushFactory();
		void reloadTextures();

		inline bool isCompressingTextures() { return compressTextures; }
		inline void toggleTextureCompression() { compressTextures = !compressTextures; }

		inline void setAnisotropicFilteringLevel(const float &level)
		{
		    GLfloat anisoMax;
		    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoMax);
            anisotropicLevel = level;
            if (anisotropicLevel < 1.0f)
            {
                anisotropicLevel = 1.0f;
                anisotropicFilteringEnabled = false;
            }
            else if (anisotropicLevel > anisoMax)
            {
                anisotropicLevel = anisoMax;
                console << GUI::Console::warning << GUI::Console::medium << "Anisotropic factor set too high. Setting to maximum allowable" << GUI::Console::endl();
                anisotropicFilteringEnabled = true;
            }
            else
                anisotropicFilteringEnabled = true;
        }

		inline void enableAnisotropicFilter(const bool &flag) { anisotropicFilteringEnabled = flag;}

		static TextureFactory* getInstance() { return instance; }
	private:
        static TextureFactory* instance;

        std::string workingDirectory;
		float anisotropicLevel;
		bool anisotropicFilteringEnabled;
		bool compressTextures;
		std::map<std::string,GLuint> textures;
		std::map<std::string,unsigned long int> textureRefs;
		int loadPNG(const std::string& filename);
		int loadJPEG(const std::string& filename);
};

#endif

