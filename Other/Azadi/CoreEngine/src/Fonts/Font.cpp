#include "Font.h"
#include "../GUI/Console.h"
#include "../GUI/Drawing.h"

using GUI::Console;

inline unsigned int next_p2(unsigned int x)
{
    unsigned int res = 1;
    while(res<x) res*=2;
    return res;
}

Font* Font::instance = NULL;

Font::Font(const std::string &workingDir)
{
    bool ret;
    ret = FT_Init_FreeType(&library);
    if (ret)
    {
        console << Console::error << Console::highish << "Error initializing the Freetype library" << Console::endl();
        assert(!ret);
    }
    else
    {
        #ifndef NDEBUG
        console << Console::normal << Console::highish << "Initializing Font engine" << Console::endl();
        #endif
    }

    // loads the alphabet array with the printable alphanumeric characters
    for (int i=32; i< 126; i++)
        alphabet[i-32] = i;

    workingDirectory = workingDir;

    instance = this;
}

Font::~Font()
{

}

void Font::loadFont(const std::string& fontname)
{
    std::string fontName = std::string("./");
    fontName += workingDirectory;
    fontName += std::string("/Fonts/");
    fontName += fontname;

    std::map<std::string, fontSet>::iterator val = faces.find(fontname);
    if (val != faces.end())
        return;

    console << Console::log << Console::silent << "TextEngine->Loading : " << fontName << Console::endl();

    int ret;

    fontSet tempo;

    ret = FT_New_Face( library, fontName.c_str(), 0, &tempo.face);
    if (ret)
    {
        console << Console::error << Console::highish << "TextEngine->Font " << fontname << " could not be loaded" << Console::endl();
        assert(!ret);
    }
    faces[fontname] = tempo;
    console << Console::log << Console::silent << "TextEngine->" << fontname << " loaded" << Console::endl();
}

float Font::drawText(const std::string& text, const std::string& font, const float &pt, const unsigned int &maxWidth)
{
    float accTrans = 0;
    float advance;
    bool kern;
    unsigned int old_glyph;
    unsigned int cur_glyph;
    unsigned int maximumWidth = (maxWidth > 0)?maxWidth:2000;

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();

    kern = FT_HAS_KERNING(faces[font].face);

    for(unsigned int i=0; i< text.size(); i++)
    {
        if (text[i] == '\n')
        {
            glTranslatef(-accTrans, (float)faces[font].height[(int)pt], 0);
            accTrans = 0;
            continue;
        }

        if (accTrans >= maximumWidth)
            continue;

        if ((text[i] - 32) != 0)
            glCallList(faces[font].displayList[(int)pt][text[i]-32]);

        if (kern && i>0)
        {
            old_glyph = FT_Get_Char_Index(faces[font].face, text[i-1]);
            cur_glyph = FT_Get_Char_Index(faces[font].face, text[i]);
            FT_Vector newPos;
            FT_Get_Kerning(faces[font].face, old_glyph, cur_glyph, ft_kerning_default, &newPos);
            advance = (faces[font].advancex[(int)pt][text[i]-32])-(faces[font].bearingx[(int)pt][text[i]-32])+(newPos.x/64);
        }
        else
            advance = (faces[font].advancex[(int)pt][text[i]-32])-(faces[font].bearingx[(int)pt][text[i]-32]);
        glTranslatef(advance, 0.0f, 0.0f);
        accTrans+=advance;
    }
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    return accTrans;
}

float Font::textWidth(const std::string& text, const std::string& font, const float &pt)
{
    float accTrans = 0;
    float advance;
    bool kern;
    unsigned int old_glyph;
    unsigned int cur_glyph;

    kern = FT_HAS_KERNING(faces[font].face);

    for(unsigned int i=0; i< text.size(); i++)
    {
        if (kern && i>0)
        {
            old_glyph = FT_Get_Char_Index(faces[font].face, text[i-1]);
            cur_glyph = FT_Get_Char_Index(faces[font].face, text[i]);
            FT_Vector newPos;
            FT_Get_Kerning(faces[font].face, old_glyph, cur_glyph, ft_kerning_default, &newPos);
            advance = (faces[font].advancex[(int)pt][text[i]-32])-(faces[font].bearingx[(int)pt][text[i]-32])+(newPos.x/64);
        }
        else
            advance = (faces[font].advancex[(int)pt][text[i]-32])-(faces[font].bearingx[(int)pt][text[i]-32]);
        accTrans+=advance;
    }
    return accTrans;
}

float Font::drawText(float number, const std::string& font, const float &pt, const unsigned int &maxWidth)
{
    std::stringstream temp;
    temp << number;
    return drawText(temp.str(), font, pt, maxWidth);
}

float Font::drawText(int number, const std::string& font, const float &pt, const unsigned int &maxWidth)
{
    std::stringstream temp;
    temp << number;
    return drawText(temp.str(), font, pt, maxWidth);
}

void Font::genPointSize(std::string fontname, unsigned int pt)
{
    if (faces.find(fontname) == faces.end())
        loadFont(fontname);
    for (unsigned int check = 0; check < loadedSizes[fontname].size(); check++)
        if (loadedSizes[fontname][check] == pt)
            return;
    int ret = FT_Set_Char_Size(faces[fontname].face, 0, pt*64, 85, 86);
    if (ret)
        return;
    int width;
    int height;
    char tempoa;
    GLuint glyphTexID;
    FT_GlyphSlot slot;
    unsigned int i;
    faces[fontname].height[(int)pt]=0;
    for (i = 0; i < unsigned(94); i++)
    {
        tempoa = alphabet[i];
        ret = FT_Load_Glyph( faces[fontname].face, FT_Get_Char_Index(faces[fontname].face, alphabet[i]), FT_LOAD_DEFAULT);
        if (ret)
            break;

        FT_Glyph glyph;
        FT_Get_Glyph( faces[fontname].face->glyph, &glyph);

        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
        FT_BitmapGlyph glyphbit = (FT_BitmapGlyph)glyph;

        width = next_p2(glyphbit->bitmap.width);
        height = next_p2(glyphbit->bitmap.rows);

        unsigned char *tex = new unsigned char[2 * width * height];

        slot = faces[fontname].face->glyph;

        for (unsigned int ii=0; ii < unsigned(height); ii++)
            for (unsigned int iii=0; iii< unsigned(width); iii++)
            {
                tex[2*(iii + ii *width)] = tex[2*(iii + ii * width) + 1] = ((iii >= unsigned(glyphbit->bitmap.width)) || (ii >= unsigned(glyphbit->bitmap.rows)))? 0 : glyphbit->bitmap.buffer[iii + glyphbit->bitmap.width * ii];
            }

        glGenTextures(1, &glyphTexID);

        faces[fontname].advancex[(int)pt].push_back(faces[fontname].face->glyph->metrics.horiAdvance / 64);
        faces[fontname].bearingx[(int)pt].push_back(faces[fontname].face->glyph->metrics.horiBearingX / 64);
        faces[fontname].bearingy[(int)pt].push_back(faces[fontname].face->glyph->metrics.horiBearingY / 64);
        faces[fontname].tops[(int)pt].push_back(glyphbit->top-glyphbit->bitmap.rows);
        faces[fontname].x[(int)pt].push_back((float)glyphbit->bitmap.width / (float)width);
        faces[fontname].y[(int)pt].push_back((float)glyphbit->bitmap.rows / (float)height);
        if (faces[fontname].height[(int)pt] < faces[fontname].face->glyph->metrics.height/64)
            faces[fontname].height[(int)pt] = faces[fontname].face->glyph->metrics.height/64;

        glBindTexture(GL_TEXTURE_2D, glyphTexID);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, tex);

        faces[fontname].displayList[(int)pt].push_back(glGenLists(1));

        glNewList(faces[fontname].displayList[(int)pt][i], GL_COMPILE);

        glPushMatrix();
        glTranslatef(0.0f, (faces[fontname].tops[(int)pt][i]/4)-faces[fontname].bearingy[(int)pt][i], 0.0f);
        glBindTexture(GL_TEXTURE_2D, glyphTexID);
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0, 0.0f);
            glVertex2f(width, 0.0f);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(width, height);
            glTexCoord2f(0.0f, 1.0);
            glVertex2f(0.0f, height);
        }
        glEnd();
        glPopMatrix();

        glEndList();

        delete [] tex;
    }

    console << Console::log << Console::silent << "TextEngine->Loaded " << fontname << " - " << pt << " point glyphs" << Console::endl();

    loadedSizes[fontname].push_back(pt);
}

long Font::getFontHeight(const std::string &fontName, const float &pointSize)
{
    if (faces.find(fontName) != faces.end())
    {
        return faces[fontName].height[unsigned(pointSize)];
    }
    return 0;
}

