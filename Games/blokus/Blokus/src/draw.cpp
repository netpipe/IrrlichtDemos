
#include <math.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>

#include "draw.h"

//使用するフォント
#define JPFONT		"data/japanese.png"
#define SYSFONT1	"data/font.bmp"

#define TYPE_CNT 9
float rad = (float)(PI / 180);	//radian
IGUIFont* sysFont;

char *names[] = {"PS_SOLID","PS_DASH","PS_DOT","PS_DASHDOT",
		"PS_DAHDOTDOT",	"PS_NULL", "PS_INSIDEFRAME", "PS_USERSTYLE", "PS_SPECIAL"};

//draw系初期化関数
bool drawInit(IrrlichtDevice *device, IVideoDriver* driver)
{
	makePattern(driver);

	return fontInit(device, driver);
}

//システム用フォントを初期化する
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

//ドットパターン作成（Rectの下地で使用）
void makePattern(IVideoDriver* driver)
{
	unsigned short types[] = {
		0xFFFF, //PS_SOLID       全て塗りつぶし サイズ：16*1
		0xFCFC, //PS_DASH        -------        サイズ：16*1
		0x6666, //PS_DOT         .......        サイズ：16*1
		0xFFF6, //PS_DASHDOT     _._._._        サイズ：16*1
		0xFFB6, //PAS_DASHDOTDOT _.._.._        サイズ：16*1
		0xFFFF, //PS_NULL        網目           サイズ： 2*2
		0xFFFF, //PS_INSIDEFRAME 枠を描画
		0xFFFF, //PS_USERSTYLE   特殊横縞       サイズ： 1*8
		0xFFFF  //PS_SPECIAL     特殊縦横縞     サイズ： 4*4
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

		//PS_NULL:網目
		if(num == 5)
		{
			data[ 7]=(char)0x00;
			data[11]=(char)0x00;
			image = driver->createImageFromData(
				ECF_A8R8G8B8,dimension2d<u32>(2,2),data);
		}
		//PS_USERSTYLE:横縞
		else if(num == 7)
		{
			data[19]=(char)0x80;
			data[23]=(char)0x80;
			data[27]=(char)0x80;
			data[31]=(char)0x80;
			image = driver->createImageFromData(
				ECF_A8R8G8B8,dimension2d<u32>(1,8),data);
		}
		//PS_SPECIAL:縦横縞
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

//画像サイズ取得
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

//画像描画
void DrawImage(IVideoDriver* driver, char* filename, int x, int y, int w, int h, int u, int v,
		   bool bColorKey, SColor colLU, SColor colLD, SColor colRD, SColor colRU,float sx, float sy)
{
	ITexture* texture = driver->getTexture(filename);
	SColor colors[] = {colLU,colLD,colRD,colRU};//左上：左下：右下：右上
	if(colLD == 0)
		colors[1] = colors[2] = colors[3] = colLU;
	if(bColorKey)
	{
		//カラーキー(色指定)
		driver->makeColorKeyTexture(texture, 0x00000000);
		//カラーキー(場所指定)
		//driver-&gtmakeColorKeyTexture(texture, position2d<s32>(0,0));
	}

	driver->draw2DImage(texture, rect<s32>(x,y,x+w*sx,y+h*sy), rect<s32>(u,v,u+w,v+h), 0, colors, true);
}

//線描画
void DrawLine(IVideoDriver* driver, int x1, int y1, int x2, int y2, SColor col)
{
	driver->draw2DLine(position2d<s32>(x1, y1), position2d<s32>(x2, y2), col);
}

//矩形描画
void DrawRect(IVideoDriver* driver, int x, int y, int w, int h, SColor col)
{
	s32 x1 = x;
	s32 x2 = x+w;
	s32 y1 = y+1;
	s32 y2 = y+h+1;

	//常に左上を基準にする
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

	DrawLine(driver, x1,  y1,  x2,  y1,  col);//上
	DrawLine(driver, x1,  y2-1,x2,  y2-1,col);//下
	DrawLine(driver, x1,  y1,  x1,  y2,  col);//左
	DrawLine(driver, x2-1,y1,  x2-1,y2,  col);//右

	//driver->draw2DPolygon(position2d<s32>(x+w/2,y+h/2),w/2,col,4);
}

//矩形描画（単色塗りつぶし）
void FillRect(IVideoDriver* driver, int x, int y, int w, int h, SColor col, int type)
{
	if(type >= TYPE_CNT)
		type = 0;
	if(type == PS_INSIDEFRAME)
		DrawRect(driver, x,y,w,h,col);
	else DrawImage(driver, names[type], x,y,w,h,0,0,false,col);
}

//矩形描画（複数色塗りつぶし）
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

//円描画
void DrawOval(IVideoDriver* driver, int cx, int cy, float radius, SColor col, int count)
{
//オリジナル
//	if(count < 3)
//		count = 3;
//	driver->draw2DPolygon(position2d<s32>(cx,cy), radius, col, count);

	float rad = (float)(PI / 180);	//radian
	float sx = cx+radius;
	float sy = cy;
	float x,y, ox = sx,oy = sy;
	s32 i;
	u16 point = count;		//インデックス数(角の数)
	if(point < 3)point = 3;
	float angle = 360/point;


	//頂点を作成する
	for(i=angle;i<360;i+=angle)
	{
		x = (cos(i*rad) * radius) + cx;
		y = (sin(i*rad) * radius) + cy;

		//微調整
	//	if(x - (int)x > 0.4)x++;
	//	if(y - (int)y > 0.4)y++;
		DrawLine(driver, ox,oy,x,y,col);

		ox = x;
		oy = y;
	}
	DrawLine(driver, ox,oy,sx,sy,col);
}

//円描画（塗りつぶし）
void FillOval(IVideoDriver* driver, int cx, int cy, float radius, SColor col, int count)
{
	matrix4 world, pos, Ortho;

	dimension2d<u32> screen = driver->getScreenSize();

	//属性設定
	SMaterial Material;
	Material.setFlag(EMF_LIGHTING,false);			//ライト設定
	Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
	driver->setMaterial(Material);

	//三角形作成
	u16 point    = count;		//インデックス数(角の数)
	if(point < 3)point = 3;
	if(point > 72)point = 72;
	u16 angle    = 360/point;	//角度の最低は5(配列72個までにしたので)
	u16 radiusx  = radius;		//半径X
	u16 radiusy  = radius;		//半径Y
	SColor color = col;			//色(アルファは0x81以上のみ有効、それ以外は透明)

	u16 i;
	float x,y;
	u16 triList[72*3];
	S3DVertex triVer[72+1];

	memset(triList,0x00,72*3*sizeof(u16));
	triVer[0] = S3DVertex(0,0,0, 0,0,0, color, 0,0);//中心

	//頂点を作成する
	for(i=1;i<=point;i++)
	{
		x = (cos(i*angle*rad) * radiusx);
		y = (sin(i*angle*rad) * radiusy);
		triVer[i] = S3DVertex(x,y,0,  0,0,0, color, 0,0);

		triList[i*3-2] = i+1;
		triList[i*3-1] = i;
	}
	//最後だけリスト調整
	triList[i*3-5] = 1;
	triList[i*3-4] = i-1;


	//位置設定（中心を基準に設定）
	float px = ((cx+0/2))/((double)screen.Width/2) - 1.0;
	float py = 1.0 - ((cy+0/2))/((double)screen.Height/2);
	pos.setTranslation(vector3df(px,py,0));//Z座標での位置指定はダメ

	//スクリーン設定
	//初期値以外を書き換え（W,0,0,0,0,H,0,0,0,0,1,0,0,0,0,1）
	Ortho(0,0) = (double)2/(double)screen.Width;
	Ortho(1,1) = (double)2/(double)screen.Height;

	//ワールドに反映
	driver->setTransform(ETS_VIEW, world);
	driver->setTransform(ETS_PROJECTION, world);
	world = pos*Ortho;//*rotate*scale;
	driver->setTransform(ETS_WORLD, world);

	//描画
	driver->drawIndexedTriangleList(triVer, point+1, triList, point);
}

//文字描画（第1水準の日本語までOK、第2水準は別途画像を用意）
position2d<s32> Printf(IVideoDriver* driver, int x, int y, Rect *pRect, SColor color, const char *fmt, ...)
{
	int i,len;
	va_list ap;
	char tmp[1024];
	char *tex = JPFONT;
	position2d<s32> pos;//返り値で使用
	int left,right,top,bottom;//描画範囲
	dimension2d<u32> screen = driver->getScreenSize();

	//書式付文字列を調整
	memset(tmp, 0x00, sizeof(tmp));
	va_start(ap, fmt);
	vsprintf(tmp, fmt, ap);
	va_end(ap);
	len = strlen(tmp);

	//折り返し地点の設定
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

	int posx = 0;			//文字の描画位置
	int posy = 0;			//文字の描画位置
	int tx, ty;				//画像読み込み位置
	int tw;					//文字の幅　　半角文字は8、全角文字は16
	int th = 16;			//文字の高さ　16固定
	unsigned char buf[2];	//取得した文字を一時保存

	for(i=0;i<len;i++)
	{
		buf[0] = tmp[i]&0xFF;

		//折り返し調整
		if(x + posx+9 > right)
		{
			posx = 0;
			posy += th;
			x = left;
		}
		if(y + posy+8 > bottom)
			break;


		//2バイトチェック(SJIS)
		if((buf[0] >= 0x80 && buf[0] <= 0x9F) || buf[0] >= 0xE0)
		{
			//2バイト文字処理
			tw = 16;
			buf[1] = tmp[++i]&0xFF;//次のバッファ取得
			//画像位置計算
			tx = ((buf[1]-0x40)%64)*th;
			ty = (1+(buf[0]-0x81)*3+(buf[1]-0x40)/64)*th;
			DrawImage(driver, tex,x+posx,y+posy,tw,th,tx,ty,false,color,color,color,color,1,1);
			posx += tw;
		}
		else
		{
			//1バイト文字処理
			tw = 8;
			switch(buf[0])
			{
				//改行
				case '\n':
					posx = 0;		//開始位置を0にする
					posy += th;		//文字の高さ分ずらす
					//x = left;
					break;
				//タブ
				case '\t':
					posx += tw*4;	//半角文字4文字分ずらす
					break;
				//通常文字
				default:
					//画像位置計算
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

//文字描画（太字）
position2d<s32> bold(IVideoDriver* driver, int x, int y, Rect *pRect, SColor color, const char *fmt, ...)
{
	s32 line = 0;
	va_list ap;
	char tmp[1024];
	position2d<s32> pos;

	memset(tmp, 0x00, sizeof(tmp));

	//書式付文字列を調整
	va_start(ap, fmt);
	vsprintf(tmp, fmt, ap);
	va_end(ap);

	Printf(driver,x,y,pRect,color,tmp);
	pos = Printf(driver,x+1,y,pRect,color,tmp);

	return pos;
}

//システム用の文字描画（英数字のみ）
void sysPrintf(int x, int y, SColor color, const char *fmt, ...)
{
	int len;
	va_list ap;
	char tmp[256];

	memset(tmp, 0x00, sizeof(tmp));

	//書式付文字列を調整
	va_start(ap, fmt);
	vsprintf(tmp, fmt, ap);
	va_end(ap);

	sysFont->draw(sjis_to_utf16be(tmp, &len),rect<s32>(x,y,0,0),color);
}

//UTF-16BE → SJIS へ文字列のコード変換
char* utf16be_to_sjis(const wchar_t *pUcsStr, int *nBytesOut)
{
	char *pAnsiStr = NULL;
	int nLen;

	if(!pUcsStr)
		return NULL;

	setlocale(LC_ALL, "Japanese");//これがないとUnicodeに変換されない！

	nLen = wcslen(pUcsStr);

	if(pUcsStr[0] == 0xfeff || pUcsStr[0] == 0xfffe)
	{
		pUcsStr++; //先頭にBOM(Byte Order Mark)があれば，スキップする
		nLen--;
	}

	pAnsiStr = (char *)calloc((nLen+1), sizeof(wchar_t));
	if(!pAnsiStr)
		return NULL;

	//1文字ずつ変換する
	//まとめて変換すると、変換不能文字への対応が困難なので
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
			default: //変換不能
				pAnsiStr[nMbpos++] = ' ';
				break;
		}
	}
	pAnsiStr[nMbpos] = '\0';

	delete [] pcMbchar;

	*nBytesOut = nMbpos;

	return pAnsiStr;
}

//SJIS → UTF-16BEへ文字列のコード変換
wchar_t* sjis_to_utf16be(const char *pAnsiStr, int *nBytesOut)
{
    int len;
    wchar_t *pUcsStr = NULL;

    if(!pAnsiStr)
		return NULL;

    setlocale(LC_ALL, "Japanese");  //これがないとUnicodeに変換されない！

    len = strlen(pAnsiStr);
    *nBytesOut = sizeof(wchar_t)*(len);

    pUcsStr = (wchar_t *)calloc(*nBytesOut + 2, 1);
    if(!pUcsStr)
		return NULL;

    mbstowcs(pUcsStr, pAnsiStr, len+1);

    return pUcsStr;
}


