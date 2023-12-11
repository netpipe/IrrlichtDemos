
#include <math.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>

#include "draw.h"

//�g�p����t�H���g
#define JPFONT		"data/japanese.png"
#define SYSFONT1	"data/font.bmp"

#define TYPE_CNT 9
float rad = (float)(PI / 180);	//radian
IGUIFont* sysFont;

char *names[] = {"PS_SOLID","PS_DASH","PS_DOT","PS_DASHDOT",
		"PS_DAHDOTDOT",	"PS_NULL", "PS_INSIDEFRAME", "PS_USERSTYLE", "PS_SPECIAL"};

//draw�n�������֐�
bool drawInit(IrrlichtDevice *device, IVideoDriver* driver)
{
	makePattern(driver);

	return fontInit(device, driver);
}

//�V�X�e���p�t�H���g������������
bool fontInit(IrrlichtDevice *device, IVideoDriver* driver)
{
	ITexture* texture = driver->getTexture(JPFONT);
	IGUIEnvironment *Env = device->getGUIEnvironment();

	if(texture == NULL)
		return false;

	sysFont = Env->getBuiltInFont();
	//sysFont = Env->getFont(SYSFONT1);
	if(sysFont == NULL)
		return false;

	return true;
}

//�h�b�g�p�^�[���쐬�iRect�̉��n�Ŏg�p�j
void makePattern(IVideoDriver* driver)
{
	unsigned short types[] = {
		0xFFFF, //PS_SOLID       �S�ēh��Ԃ� �T�C�Y�F16*1
		0xFCFC, //PS_DASH        -------        �T�C�Y�F16*1
		0x6666, //PS_DOT         .......        �T�C�Y�F16*1
		0xFFF6, //PS_DASHDOT     _._._._        �T�C�Y�F16*1
		0xFFB6, //PAS_DASHDOTDOT _.._.._        �T�C�Y�F16*1
		0xFFFF, //PS_NULL        �Ԗ�           �T�C�Y�F 2*2
		0xFFFF, //PS_INSIDEFRAME �g��`��
		0xFFFF, //PS_USERSTYLE   ���ꉡ��       �T�C�Y�F 1*8
		0xFFFF  //PS_SPECIAL     ����c����     �T�C�Y�F 4*4
	};
	IImage *image;
	char data[16*1*4];//RGBA
	int num;

	for(num=0;num<TYPE_CNT;num++){
		memset(data,0xFF,sizeof(data));

		if((types[num] & 0x8000) == 0)data[ 3]=0;
		if((types[num] & 0x4000) == 0)data[ 7]=0;
		if((types[num] & 0x2000) == 0)data[11]=0;
		if((types[num] & 0x1000) == 0)data[15]=0;
		if((types[num] & 0x0800) == 0)data[19]=0;
		if((types[num] & 0x0400) == 0)data[23]=0;
		if((types[num] & 0x0200) == 0)data[27]=0;
		if((types[num] & 0x0100) == 0)data[31]=0;
		if((types[num] & 0x0080) == 0)data[35]=0;
		if((types[num] & 0x0040) == 0)data[39]=0;
		if((types[num] & 0x0020) == 0)data[43]=0;
		if((types[num] & 0x0010) == 0)data[47]=0;
		if((types[num] & 0x0008) == 0)data[51]=0;
		if((types[num] & 0x0004) == 0)data[55]=0;
		if((types[num] & 0x0002) == 0)data[59]=0;
		if((types[num] & 0x0001) == 0)data[63]=0;

		//PS_NULL:�Ԗ�
		if(num == 5)
		{
			data[ 7]=(char)0x00;
			data[11]=(char)0x00;
			image = driver->createImageFromData(
				ECF_A8R8G8B8,dimension2d<u32>(2,2),data);
		}
		//PS_USERSTYLE:����
		else if(num == 7)
		{
			data[19]=(char)0x80;
			data[23]=(char)0x80;
			data[27]=(char)0x80;
			data[31]=(char)0x80;
			image = driver->createImageFromData(
				ECF_A8R8G8B8,dimension2d<u32>(1,8),data);
		}
		//PS_SPECIAL:�c����
		else if(num == 8)
		{
			data[15]=(char)0xE0;
			data[31]=(char)0xE0;
			data[47]=(char)0xE0;
			data[51]=(char)0xE0;
			data[55]=(char)0xE0;
			data[59]=(char)0xE0;
			data[63]=(char)0xE0;
			image = driver->createImageFromData(
				ECF_A8R8G8B8,dimension2d<u32>(4,4),data);
		}
		else
			image = driver->createImageFromData(	ECF_A8R8G8B8,dimension2d<u32>(16,1),data);
		driver->addTexture(names[num], image);
	}
}

//�摜�T�C�Y�擾
dimension2d<s32> GetImageSize(IVideoDriver* driver,char* filename)
{
	ITexture* texture;
	dimension2d<s32> size(0,0);

	if(filename != NULL && strlen(filename) == 0)
		return size;

	texture = driver->getTexture(filename);

	if(texture != NULL)
		size = texture->getSize();

	return size;
}

//�摜�`��
void DrawImage(IVideoDriver* driver, char* filename, int x, int y, int w, int h, int u, int v,
		   bool bColorKey, SColor colLU, SColor colLD, SColor colRD, SColor colRU,float sx, float sy)
{
	ITexture* texture = driver->getTexture(filename);
	SColor colors[] = {colLU,colLD,colRD,colRU};//����F�����F�E���F�E��
	if(colLD == 0)
		colors[1] = colors[2] = colors[3] = colLU;
	if(bColorKey)
	{
		//�J���[�L�[(�F�w��)
		driver->makeColorKeyTexture(texture, 0x00000000);
		//�J���[�L�[(�ꏊ�w��)
		//driver-&gtmakeColorKeyTexture(texture, position2d<s32>(0,0));
	}

	driver->draw2DImage(texture, rect<s32>(x,y,x+w*sx,y+h*sy), rect<s32>(u,v,u+w,v+h), 0, colors, true);
}

//���`��
void DrawLine(IVideoDriver* driver, int x1, int y1, int x2, int y2, SColor col)
{
	driver->draw2DLine(position2d<s32>(x1, y1), position2d<s32>(x2, y2), col);
}

//��`�`��
void DrawRect(IVideoDriver* driver, int x, int y, int w, int h, SColor col)
{
	s32 x1 = x;
	s32 x2 = x+w;
	s32 y1 = y+1;
	s32 y2 = y+h+1;

	//��ɍ������ɂ���
	if(w < 0)
	{
		x1 = x+w;
		x2 = x;
	}
	if(h < 0)
	{
		y1 = y+h+1;
		y2 = y+1;
	}

	DrawLine(driver, x1,  y1,  x2,  y1,  col);//��
	DrawLine(driver, x1,  y2-1,x2,  y2-1,col);//��
	DrawLine(driver, x1,  y1,  x1,  y2,  col);//��
	DrawLine(driver, x2-1,y1,  x2-1,y2,  col);//�E

	//driver->draw2DPolygon(position2d<s32>(x+w/2,y+h/2),w/2,col,4);
}

//��`�`��i�P�F�h��Ԃ��j
void FillRect(IVideoDriver* driver, int x, int y, int w, int h, SColor col, int type)
{
	if(type >= TYPE_CNT)
		type = 0;
	if(type == PS_INSIDEFRAME)
		DrawRect(driver, x,y,w,h,col);
	else DrawImage(driver, names[type], x,y,w,h,0,0,false,col);
}

//��`�`��i�����F�h��Ԃ��j
void FillRect2(IVideoDriver* driver, int x, int y, int w, int h,
		SColor colLU, SColor colLD, SColor colRD, SColor colRU, int type)
{
	if(type >= TYPE_CNT)
		type = 0;
	if(type == PS_INSIDEFRAME)
		DrawRect(driver, x,y,w,h,colLD);
	else DrawImage(driver, names[type], x,y,w,h,0,0,false,colLU,colLD,colRD,colRU);

	//driver->draw2DRectangle(rect<s32>(x,y,x+w,y+h), colLU, colLD, colRD, colRU);
}

//�~�`��
void DrawOval(IVideoDriver* driver, int cx, int cy, float radius, SColor col, int count)
{
//�I���W�i��
//	if(count < 3)
//		count = 3;
//	driver->draw2DPolygon(position2d<s32>(cx,cy), radius, col, count);

	float rad = (float)(PI / 180);	//radian
	float sx = cx+radius;
	float sy = cy;
	float x,y, ox = sx,oy = sy;
	s32 i;
	u16 point = count;		//�C���f�b�N�X��(�p�̐�)
	if(point < 3)point = 3;
	float angle = 360/point;


	//���_���쐬����
	for(i=angle;i<360;i+=angle)
	{
		x = (cos(i*rad) * radius) + cx;
		y = (sin(i*rad) * radius) + cy;

		//������
	//	if(x - (int)x > 0.4)x++;
	//	if(y - (int)y > 0.4)y++;
		DrawLine(driver, ox,oy,x,y,col);

		ox = x;
		oy = y;
	}
	DrawLine(driver, ox,oy,sx,sy,col);
}

//�~�`��i�h��Ԃ��j
void FillOval(IVideoDriver* driver, int cx, int cy, float radius, SColor col, int count)
{
	matrix4 world, pos, Ortho;

	dimension2d<u32> screen = driver->getScreenSize();

	//�����ݒ�
	SMaterial Material;
	Material.setFlag(EMF_LIGHTING,false);			//���C�g�ݒ�
	Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
	driver->setMaterial(Material);

	//�O�p�`�쐬
	u16 point    = count;		//�C���f�b�N�X��(�p�̐�)
	if(point < 3)point = 3;
	if(point > 72)point = 72;
	u16 angle    = 360/point;	//�p�x�̍Œ��5(�z��72�܂łɂ����̂�)
	u16 radiusx  = radius;		//���aX
	u16 radiusy  = radius;		//���aY
	SColor color = col;			//�F(�A���t�@��0x81�ȏ�̂ݗL���A����ȊO�͓���)

	u16 i;
	float x,y;
	u16 triList[72*3];
	S3DVertex triVer[72+1];

	memset(triList,0x00,72*3*sizeof(u16));
	triVer[0] = S3DVertex(0,0,0, 0,0,0, color, 0,0);//���S

	//���_���쐬����
	for(i=1;i<=point;i++)
	{
		x = (cos(i*angle*rad) * radiusx);
		y = (sin(i*angle*rad) * radiusy);
		triVer[i] = S3DVertex(x,y,0,  0,0,0, color, 0,0);

		triList[i*3-2] = i+1;
		triList[i*3-1] = i;
	}
	//�Ōゾ�����X�g����
	triList[i*3-5] = 1;
	triList[i*3-4] = i-1;


	//�ʒu�ݒ�i���S����ɐݒ�j
	float px = ((cx+0/2))/((double)screen.Width/2) - 1.0;
	float py = 1.0 - ((cy+0/2))/((double)screen.Height/2);
	pos.setTranslation(vector3df(px,py,0));//Z���W�ł̈ʒu�w��̓_��

	//�X�N���[���ݒ�
	//�����l�ȊO�����������iW,0,0,0,0,H,0,0,0,0,1,0,0,0,0,1�j
	Ortho(0,0) = (double)2/(double)screen.Width;
	Ortho(1,1) = (double)2/(double)screen.Height;

	//���[���h�ɔ��f
	driver->setTransform(ETS_VIEW, world);
	driver->setTransform(ETS_PROJECTION, world);
	world = pos*Ortho;//*rotate*scale;
	driver->setTransform(ETS_WORLD, world);

	//�`��
	driver->drawIndexedTriangleList(triVer, point+1, triList, point);
}

//�����`��i��1�����̓��{��܂�OK�A��2�����͕ʓr�摜��p�Ӂj
position2d<s32> Printf(IVideoDriver* driver, int x, int y, Rect *pRect, SColor color, const char *fmt, ...)
{
	int i,len;
	va_list ap;
	char tmp[1024];
	char *tex = JPFONT;
	position2d<s32> pos;//�Ԃ�l�Ŏg�p
	int left,right,top,bottom;//�`��͈�
	dimension2d<u32> screen = driver->getScreenSize();

	//�����t������𒲐�
	memset(tmp, 0x00, sizeof(tmp));
	va_start(ap, fmt);
	vsprintf(tmp, fmt, ap);
	va_end(ap);
	len = strlen(tmp);

	//�܂�Ԃ��n�_�̐ݒ�
	if(pRect != NULL)
	{
		left   = pRect->x;
		right  = pRect->x + pRect->w;
		top    = pRect->y;
		bottom = pRect->y + pRect->h;
	}
	else
	{
		left   = 0;
		right  = screen.Width;
		top    = 0;
		bottom = screen.Height;
	}

	int posx = 0;			//�����̕`��ʒu
	int posy = 0;			//�����̕`��ʒu
	int tx, ty;				//�摜�ǂݍ��݈ʒu
	int tw;					//�����̕��@�@���p������8�A�S�p������16
	int th = 16;			//�����̍����@16�Œ�
	unsigned char buf[2];	//�擾�����������ꎞ�ۑ�

	for(i=0;i<len;i++)
	{
		buf[0] = tmp[i]&0xFF;

		//�܂�Ԃ�����
		if(x + posx+9 > right)
		{
			posx = 0;
			posy += th;
			x = left;
		}
		if(y + posy+8 > bottom)
			break;


		//2�o�C�g�`�F�b�N(SJIS)
		if((buf[0] >= 0x80 && buf[0] <= 0x9F) || buf[0] >= 0xE0)
		{
			//2�o�C�g��������
			tw = 16;
			buf[1] = tmp[++i]&0xFF;//���̃o�b�t�@�擾
			//�摜�ʒu�v�Z
			tx = ((buf[1]-0x40)%64)*th;
			ty = (1+(buf[0]-0x81)*3+(buf[1]-0x40)/64)*th;
			DrawImage(driver, tex,x+posx,y+posy,tw,th,tx,ty,false,color,color,color,color,1,1);
			posx += tw;
		}
		else
		{
			//1�o�C�g��������
			tw = 8;
			switch(buf[0])
			{
				//���s
				case '\n':
					posx = 0;		//�J�n�ʒu��0�ɂ���
					posy += th;		//�����̍��������炷
					//x = left;
					break;
				//�^�u
				case '\t':
					posx += tw*4;	//���p����4���������炷
					break;
				//�ʏ핶��
				default:
					//�摜�ʒu�v�Z
					tx = (buf[0]-0x20)*tw;
					ty = 0;
					DrawImage(driver, tex,x+posx,y+posy,tw,th,tx,ty,false,color,color,color,color,1,1);
					posx += tw;
					break;
			}
		}
	}

	pos.X = posx;
	pos.Y = posy/16;
	return pos;
}

//�����`��i�����j
position2d<s32> bold(IVideoDriver* driver, int x, int y, Rect *pRect, SColor color, const char *fmt, ...)
{
	s32 line = 0;
	va_list ap;
	char tmp[1024];
	position2d<s32> pos;

	memset(tmp, 0x00, sizeof(tmp));

	//�����t������𒲐�
	va_start(ap, fmt);
	vsprintf(tmp, fmt, ap);
	va_end(ap);

	Printf(driver,x,y,pRect,color,tmp);
	pos = Printf(driver,x+1,y,pRect,color,tmp);

	return pos;
}

//�V�X�e���p�̕����`��i�p�����̂݁j
void sysPrintf(int x, int y, SColor color, const char *fmt, ...)
{
	int len;
	va_list ap;
	char tmp[256];

	memset(tmp, 0x00, sizeof(tmp));

	//�����t������𒲐�
	va_start(ap, fmt);
	vsprintf(tmp, fmt, ap);
	va_end(ap);

	sysFont->draw(sjis_to_utf16be(tmp, &len),rect<s32>(x,y,0,0),color);
}

//UTF-16BE �� SJIS �֕�����̃R�[�h�ϊ�
char* utf16be_to_sjis(const wchar_t *pUcsStr, int *nBytesOut)
{
	char *pAnsiStr = NULL;
	int nLen;

	if(!pUcsStr)
		return NULL;

	setlocale(LC_ALL, "Japanese");//���ꂪ�Ȃ���Unicode�ɕϊ�����Ȃ��I

	nLen = wcslen(pUcsStr);

	if(pUcsStr[0] == 0xfeff || pUcsStr[0] == 0xfffe)
	{
		pUcsStr++; //�擪��BOM(Byte Order Mark)������΁C�X�L�b�v����
		nLen--;
	}

	pAnsiStr = (char *)calloc((nLen+1), sizeof(wchar_t));
	if(!pAnsiStr)
		return NULL;

	//1�������ϊ�����
	//�܂Ƃ߂ĕϊ�����ƁA�ϊ��s�\�����ւ̑Ή�������Ȃ̂�
	int nRet, i, nMbpos = 0;
	char *pcMbchar = new char[MB_CUR_MAX];

	for(i=0;i<nLen;i++)
	{
		nRet = wctomb(pcMbchar, pUcsStr[i]);
		switch(nRet)
		{
			case 1:
				pAnsiStr[nMbpos++] = pcMbchar[0];
				break;
			case 2:
				pAnsiStr[nMbpos++] = pcMbchar[0];
				pAnsiStr[nMbpos++] = pcMbchar[1];
				break;
			default: //�ϊ��s�\
				pAnsiStr[nMbpos++] = ' ';
				break;
		}
	}
	pAnsiStr[nMbpos] = '\0';

	delete [] pcMbchar;

	*nBytesOut = nMbpos;

	return pAnsiStr;
}

//SJIS �� UTF-16BE�֕�����̃R�[�h�ϊ�
wchar_t* sjis_to_utf16be(const char *pAnsiStr, int *nBytesOut)
{
    int len;
    wchar_t *pUcsStr = NULL;

    if(!pAnsiStr)
		return NULL;

    setlocale(LC_ALL, "Japanese");  //���ꂪ�Ȃ���Unicode�ɕϊ�����Ȃ��I

    len = strlen(pAnsiStr);
    *nBytesOut = sizeof(wchar_t)*(len);

    pUcsStr = (wchar_t *)calloc(*nBytesOut + 2, 1);
    if(!pUcsStr)
		return NULL;

    mbstowcs(pUcsStr, pAnsiStr, len+1);

    return pUcsStr;
}


