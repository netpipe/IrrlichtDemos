#include "TextureFactory.h"
#include "../FileSystem/FileSystem.h"
#include <cstdio>

#ifdef __OSX__
#include <libpng/png.h>
#else
#include "png/png.h"
#endif

TextureFactory* TextureFactory::instance = NULL;

using GUI::Console;

TextureFactory::TextureFactory(const std::string &workingDir)
{
    console << Console::normal << Console::high << "Initializing Texture Factory" << Console::endl();
    workingDirectory = workingDir;
    instance = this;
    anisotropicLevel = 1.0f;
    anisotropicFilteringEnabled = false;
    compressTextures = false;
}

TextureFactory::~TextureFactory()
{
    flushFactory();
}

int TextureFactory::loadPNG(const std::string& filename)
{
    png_structp png_ptr;
    png_infop info_ptr;
    png_infop end_ptr;

    GLubyte	*image_data;
    GLbyte sig[8];

    GLint bit_depth;
    GLint color_type;

    unsigned long width;
    unsigned long height;
    unsigned int rowbytes;
    GLuint component;

    image_data=NULL;
    unsigned int i;
    png_bytepp row_pointers=NULL;

    FILE* infile = fopen(filename.c_str(), "rb");
    if (!infile)
    {
        console << Console::warning << Console::medium << "Cannot open texture: " << filename << Console::endl();
        return PNG_FILEIOERROR;
    }

    // Reads the file to see if the PNG signature exists
    fread(sig, 1, 8, infile);
    if (!png_check_sig((GLubyte *) sig, 8))
    {
        console << Console::warning << Console::medium << filename << " is not a valid PNG" << Console::endl();
        fclose(infile);
        return PNG_FILEIOERROR;
    }

    // Creates a structure to read the PNG information
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fclose(infile);
        return PNG_MALLOCERROR;
    }

    info_ptr=png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(infile);
        return PNG_MALLOCERROR;
    }

    end_ptr = png_create_info_struct(png_ptr);
    if (!end_ptr)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(infile);
        return PNG_MALLOCERROR;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(infile);
        return PNG_JUMPSETERROR;
    }

    png_ptr->io_ptr = (png_voidp)infile;

    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, 8);

    // Start the PNG information reading
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *)&width, (png_uint_32 *)&height, (int *)&bit_depth, (int *)&color_type, NULL, NULL, NULL);
    width = width;
    height = height;

    // Set up the alpha for textures
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        component = GL_RGBA;
    else
        component = GL_RGB;

    png_read_update_info(png_ptr, info_ptr);

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // Allocate the data required to store the image
    if ((image_data=new GLubyte[rowbytes * height]) == NULL)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 4;
    }

    if ((row_pointers=new png_bytep[height* sizeof(png_bytep)]) == NULL)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        delete [] image_data;
        image_data = NULL;
        return 4;
    }

    for (i=0; i<height; ++i)
        row_pointers[height -1 -i] = image_data + i*rowbytes;

    // Finally read the image itself
    png_read_image(png_ptr, row_pointers);

    delete [] row_pointers;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(infile);

    GLuint texID;

    // Texture Generation
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (GLEW_EXT_texture_filter_anisotropic && anisotropicFilteringEnabled)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicLevel);

    if (compressTextures)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height, 0, component, GL_UNSIGNED_BYTE, image_data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, component, GL_UNSIGNED_BYTE, image_data);

    // Generate mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, component, width, height, component, GL_UNSIGNED_BYTE, image_data);

    delete [] image_data;

    textures[filename] = texID;
    textureRefs[filename] = 1;

    return PNG_OK;
}

//! Texture loading function (Can be appended to include more file formats)
void TextureFactory::loadTexture(const std::string& textureName)
{
    if (textureName == "")
        return;
    std::string texname(std::string("./") + workingDirectory + std::string("/Textures/") + textureName); // Should be changed to wherever the data belongs

    console << Console::log << Console::silent << "Loading texture: " << texname << Console::endl();

    std::map<std::string, GLuint>::iterator value  = textures.find(texname);
    if (value != textures.end())
    {
        ++textureRefs[texname];
        return;
    }

    if ((textureName).find(".png") != std::string::npos)
        loadPNG(texname);
    else
        console << Console::lowish << Console::warning << "TextureFactory->Loading of file: " << textureName << " failed: Filetype unsupported" << Console::endl();

    return; // return an error number -1
}

//! Applies a texture specified with it's name
void TextureFactory::deleteTexture(const std::string &filename)
{
    if (textures.find(filename) != textures.end() && textureRefs[filename] == 1)
    {
        glDeleteTextures(1, (const GLuint*)&textures[filename]);
        textureRefs.erase(filename);
        textures.erase(filename);
    }
    else
        --textureRefs[filename];
}

//! Flushes all the textures from the texture factory as well as in video memory
void TextureFactory::flushFactory()
{
    unsigned int refCount = 0;
    unsigned int *texs;
    texs = new unsigned int[textures.size()];
    std::map<std::string, GLuint>::iterator it = textures.begin();
    while (it != textures.end())
    {
        texs[refCount] = (*it).second;
        it++;
        refCount++;
    }
    glDeleteTextures(refCount, (GLuint*)texs);
    textures.clear();
    delete [] texs;
}

//! Reloads all the textures in the texture factory
void TextureFactory::reloadTextures()
{
    std::map<std::string, GLuint> temp;

    temp = textures;

    unsigned short subStart = strlen((std::string("./") + workingDirectory + std::string("/Textures/")).c_str());

    flushFactory();
    std::map<std::string, GLuint>::iterator it = temp.begin();
    while (it != temp.end())
    {
        loadTexture(((*it).first).substr(subStart));
        it ++;
    }
}

