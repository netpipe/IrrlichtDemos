
#include "blokus.h"

c8 Block[22][5][5] =
{
	//22個目は一時保存用
	//23個目はダミー
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
	0,0,1,0,0,	//□□■□□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
	0,0,1,1,0,	//□□■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
	0,1,1,1,0,	//□■■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,1,0,0,	//□□■□□
	0,0,1,1,0,	//□□■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
	1,1,1,1,0,	//■■■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,1,0,0,	//□□■□□
	0,1,1,1,0,	//□■■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,0,1,0,	//□□□■□
	0,1,1,1,0,	//□■■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,1,1,0,	//□□■■□
	0,1,1,0,0,	//□■■□□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,1,1,0,0,	//□■■□□
	0,1,1,0,0,	//□■■□□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
	1,1,1,1,1,	//■■■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,1,0,0,0,	//□■□□□
	1,1,1,1,0,	//■■■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,1,0,1,0,	//□■□■□
	0,1,1,1,0,	//□■■■□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,1,1,0,0,	//□■■□□
	0,0,1,0,0,	//□□■□□
	0,0,1,1,0,	//□□■■□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,1,1,0,0,	//□■■□□
	0,0,1,1,0,	//□□■■□
	0,0,1,0,0,	//□□■□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,0,1,0,	//□□□■□
	0,0,1,1,0,	//□□■■□
	0,0,1,0,0,	//□□■□□
	0,0,1,0,0,	//□□■□□

	0,0,0,0,0,	//□□□□□
	0,0,1,0,0,	//□□■□□
	0,1,1,0,0,	//□■■□□
	0,1,1,0,0,	//□■■□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,1,0,0,	//□■□□□
	0,0,1,0,0,	//□■□□□
	0,0,1,0,0,	//□■□□□
	0,0,1,1,0,	//□■■□□

	0,0,0,0,0,	//□□□□□
	0,0,1,0,0,	//□□■□□
	0,1,1,1,0,	//□■■■□
	0,0,1,0,0,	//□□■□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,1,1,0,	//□□■■□
	0,1,1,0,0,	//□■■□□
	0,1,0,0,0,	//□■□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,1,1,1,0,	//□■■■□
	0,1,0,0,0,	//□■□□□
	0,1,0,0,0,	//□■□□□
	0,0,0,0,0,	//□□□□□

	0,0,0,0,0,	//□□□□□
	0,0,1,0,0,	//□□■□□
	0,0,1,0,0,	//□□■□□
	0,1,1,1,0,	//□■■■□
	0,0,0,0,0,	//□□□□□

	//22個目は一時保存用
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
	0,0,0,0,0,	//□□□□□
};

//ブロックサイズ
#define BSIZE 15

//描画位置
#define PX    10	//ボードの描画位置
#define PX2   360	//プレイヤーブロックの描画位置
#define PY    60	//全体の描画位置

//ボードサイズ
#define BOARD 320	//全体のサイズ
#define SPACE 10	//周囲の空きスペース

//時間
#define TIME 6

s32 BlockNo = 0;
s32 P_BLOCK[6][21];	//残っているブロック
c8  P_BLOCK_DETAIL[6][22][5][5];	//手持ちのブロック(各自の回転なども記憶)
c8  P_LIFE[6];	//生存状態
s32 P_SCORE[6];	//点数
s32 P_TIME[6];	//残り時間
c8  P_COUNT[6];	//使用ブロックの数
s32 TURN = 0;	//現在のターン

c8 board[20][20];
SColor col[6] = {0xFFFFFFFF,0xFF0000FF,0xFFFF0000,0xFF00FF00,0xFFFFFF00,0x00000000};

typedef struct POINT {
  long x;
  long y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;


//-------------------
//カラー設定：通常
//-------------------
SColor set_col1(s32 value)
{
	switch(value)
	{
		case  0: return 0xFF999999;

		case  1: return 0xFF3030FF;
		case  2: return 0xFF30FF30;
		case  3: return 0xFFFF3030;
		case  4: return 0xFFFFFF30;

		case  5: return 0x00000000;

		case 11: return 0xFFCCCCFF;
		case 12: return 0xFFCCFFCC;
		case 13: return 0xFFFFCCCC;
		case 14: return 0xFFFFFFCC;

		default: return 0xFFEEEEEE;
	}
}

//-------------------
//カラー設定：暗め
//-------------------
SColor set_col2(s32 value)
{
	switch(value)
	{
		case  0: return 0xFF999999;

		case  1: return 0xFF000040;
		case  2: return 0xFF004000;
		case  3: return 0xFF400000;
		case  4: return 0xFF404000;

		case  5: return 0x00000000;

		case 11: return 0xFF3333CC;
		case 12: return 0xFF33CC33;
		case 13: return 0xFFCC3333;
		case 14: return 0xFFCCCC33;

		default: return 0xFF999999;
	}
}

//-------------------
//次のブロック検索
//-------------------
s32 block_next()
{
	s32 cnt = 0;

	while(P_BLOCK[TURN][cnt] == 1)
	{
		cnt++;
		if(cnt == 21)
		{
			BlockNo = 23;
			return BlockNo;
		}
	}
	BlockNo = cnt;

	return BlockNo;
}

//-------------------
//前のブロック検索
//-------------------
s32 block_back()
{
	s32 cnt = 20;

	while(P_BLOCK[TURN][cnt] == 1)
	{
		cnt--;
		if(cnt == -1)
		{
			BlockNo = 23;
			return BlockNo;
		}
	}
	BlockNo = cnt;

	return BlockNo;
}

//-------------------
//回転・反転処理
//0:X軸回転
//1:Y軸回転
//2:左回転
//3:右回転
//-------------------
void rotate(s32 mode, s32 num)
{
	s32 x, y;
	c8 buf[5][5];

	//メモリを書き換えるので、データを一旦退避
	memcpy(buf, P_BLOCK_DETAIL[TURN][num], sizeof(P_BLOCK_DETAIL[TURN][num]));

	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			switch(mode)
			{
				case 0: P_BLOCK_DETAIL[TURN][num][x][y] = buf[x][4-y]; break;
				case 1: P_BLOCK_DETAIL[TURN][num][x][y] = buf[4-x][y]; break;
				case 2: P_BLOCK_DETAIL[TURN][num][x][y] = buf[y][4-x]; break;
				case 3: P_BLOCK_DETAIL[TURN][num][x][y] = buf[4-y][x]; break;
			}
		}
	}
}

//-------------------
//次のターン設定
//-------------------
void nextPlayer()
{
	s32 old = TURN;

	TURN++;
	if(TURN > 4)
		TURN = 1;

	while(P_LIFE[TURN] == 1 || P_COUNT[TURN] >= 21)
	{
		TURN++;

		if(TURN > 4)
			TURN = 1;

		if(old == TURN)
		{
			if(P_LIFE[TURN] == 1)
				TURN = 5;//ゲーム終了

			if(block_next() == 23)
				TURN = 5;//ゲーム終了
			break;
		}
	}

	//次のブロック検索
	if(old != TURN)
		block_next();
}

//配置する際の角チェック
bool ptCheck(s32 mousex, s32 mousey, s32 num, s32 color)
{
	s32 x, y;

	//左上を設定
	s32 mx = (mousex-PX-SPACE)/BSIZE - 2;
	s32 my = (mousey-PY-SPACE)/BSIZE - 2;

	bool bOK = false;

	s32 i,cnt = 0;
	POINT pt[5];

	//■の場所と数を取得
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			if(P_BLOCK_DETAIL[color][num][x][y] == 1)
			{
				pt[cnt].x = mx + y;
				pt[cnt].y = my + x;
				cnt++;
			}
		}
	}

	//最初の角のチェック
	if(P_COUNT[color] == 0)
	{
		for(i=0;i<cnt;i++)
		{
			//どの角を使ってもOK
			if(board[ 0][ 0] == 0 && pt[i].x ==  0 && pt[i].y ==  0)return true;
			if(board[ 0][19] == 0 && pt[i].x == 19 && pt[i].y ==  0)return true;
			if(board[19][19] == 0 && pt[i].x == 19 && pt[i].y == 19)return true;
			if(board[19][ 0] == 0 && pt[i].x ==  0 && pt[i].y == 19)return true;

			/*
			//角の指定
			switch(color)
			{
				case 1: if(board[ 0][ 0] == 0 && pt[i].x ==  0 && pt[i].y ==  0)return true; break;
				case 2: if(board[ 0][19] == 0 && pt[i].x == 19 && pt[i].y ==  0)return true; break;
				case 3: if(board[19][19] == 0 && pt[i].x == 19 && pt[i].y == 19)return true; break;
				case 4: if(board[19][ 0] == 0 && pt[i].x ==  0 && pt[i].y == 19)return true; break;
			}
			*/
		}
	}

	//各■に対してチェック
	for(i=0;i<cnt;i++)
	{
		//角と角でつなげられるかチェック
		if((pt[i].y-1 >= 0)  && (pt[i].x-1 >= 0)  && (board[pt[i].y-1][pt[i].x-1]%10 == color))
			bOK = true;	//左上
		if((pt[i].y-1 >= 0)  && (pt[i].x+1 <= 19) && (board[pt[i].y-1][pt[i].x+1]%10 == color))
			bOK = true;	//右上
		if((pt[i].y+1 <= 19) && (pt[i].x-1 >= 0)  && (board[pt[i].y+1][pt[i].x-1]%10 == color))
			bOK = true;	//左下
		if((pt[i].y+1 <= 19) && (pt[i].x+1 <= 19) && (board[pt[i].y+1][pt[i].x+1]%10 == color))
			bOK = true;	//右下

		//隣あっている同じ色の■があるかチェック
		if((pt[i].y-1 >= 0)  && (board[pt[i].y-1][pt[i].x  ]%10 == color))
			return false;	//上
		if((pt[i].y+1 <= 19) && (board[pt[i].y+1][pt[i].x  ]%10 == color))
			return false;	//下
		if((pt[i].x-1 >= 0)  && (board[pt[i].y  ][pt[i].x-1]%10 == color))
			return false;	//左
		if((pt[i].x+1 <= 19) && (board[pt[i].y  ][pt[i].x+1]%10 == color))
			return false;	//右
	}

	//角と角でつなげられるかを返す
	return bOK;
}

//ゲーム初期化
void game_init()
{
	int i;

	//生存状態の初期化
	memset(P_LIFE, 0x00, sizeof(P_LIFE));

	//使用ブロック数の初期化
	memset(P_COUNT, 0x00, sizeof(P_COUNT));

	//点数の初期化
	memset(P_SCORE, 0x00, sizeof(P_SCORE));

	//ボードの初期化
	memset(board, 0x00, sizeof(board));

	//プレイヤーのブロックの初期化
	for(i=0;i<5;i++){
		memset(P_BLOCK[i], 0x00, sizeof(P_BLOCK[i]));
		memcpy(P_BLOCK_DETAIL[i],Block,sizeof(Block));
		P_TIME[i] = TIME*60;
	}

	BlockNo = 0;

	TURN = 0;
}

//ゲーム開始
void game_start()
{
	game_init();

	//順番決め
	TURN = 1;//rand()%4+1;

//	enemy(TURN);
}

//ボードの描画
void game_board()
{
	s32 x, y;
	s32 point  = 0;

	//一旦画面消去
	DrawImage(driver,"data/board.png", PX,PY,BOARD,BOARD,0,0,false,0xFFFFFFFF);

	//ブロックの描画
	for(x=0;x<20;x++)
	{
		for(y=0;y<20;y++)
		{
			point = board[y][x];
			if(point != 0)
			{
				//矩形塗りつぶし
				//FillRect(driver,PX+SPACE+BSIZE*x, PY+SPACE+BSIZE*y, BSIZE, BSIZE, set_col1(point));

				//画像使用
				DrawImage(driver,"data/board.png",PX+SPACE+BSIZE*x,PY+SPACE+BSIZE*y,BSIZE,BSIZE,256,320,false,set_col1(point));
			}
		}
	}
}

//現在選択しているピースの描画
void game_peace(s32 mousex, s32 mousey, s32 num, s32 turn)
{
	s32 x, y, color;

	//左上の座標を計算
	s32 tx = (mousex-PX-SPACE)/ BSIZE - 2;
	s32 ty = (mousey-PY-SPACE)/ BSIZE - 2;

	if(turn == 0 || turn == 5)
		return;

	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			//空白でなければ、描画する
			if(P_BLOCK_DETAIL[turn][num][x][y] == 1)
			{
				if( tx+y >= 0 && tx+y <= 19 && ty+x >= 0 && ty+x <= 19)
				{
					//背景色に対応した■を描画
					color = board[ty+x][tx+y];
					FillRect(driver, (tx+y)*BSIZE+PX+SPACE, (ty+x)*BSIZE+PY+SPACE, BSIZE, BSIZE, set_col2(color));

					//枠を描画
					DrawRect(driver, (tx+y)*BSIZE+PX+SPACE, (ty+x)*BSIZE+PY+SPACE, BSIZE, BSIZE, set_col1(turn));
				}
			}
		}
	}
}

//ブロック単体の描画
void block(s32 playerNum, s32 blockNum, s32 px, s32 py)
{
	s32 base = 22;
	s32 size = 4;
	s32 x, y;
	SColor col;

	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			//ブロックデータがあれば
			if(P_BLOCK_DETAIL[playerNum][blockNum][y][x] != 0)
			{
				//色設定
				if(TURN == playerNum)
					col = set_col1(playerNum);
				else
					col = set_col2(playerNum);


				//プレイヤーのブロックがあれば
				if(P_BLOCK[playerNum][blockNum] == 0)
					 FillRect(driver,
						px+size*x + blockNum%11*base + 7+1,
						py+size*y + blockNum/11*base + 19+1,
						size, size, col);
			}
		}
	}
}

//全プレイヤーの残りブロック描画
void game_block(s32 color)
{
	s32 base = 22;
	s32 i,num;
	SColor col;
	SColor font;
	c8 name[6][16] = {"","BLUE","GREEN","RED","YELLOW",""};

	for(i=1;i<5;i++)
	{
		//色設定
		if(TURN == i)
		{
			col  = set_col1(i);
			font = 0xFF000000;
		}
		else
		{
			col  = set_col2(i);
			font = 0xFF808080;
		}

		//枠
		DrawImage(driver,"data/board.png",PX2,PY+80*(i-1),256,70,0,320,false,col);

		//ブロック描画
		for(num=0;num<21;num++)
			block(i, num, PX2, PY+80*(i-1));

		//現在選択しているピース
		if(TURN == i)
			DrawRect(driver,PX2+BlockNo%11*base+7,PY+BlockNo/11*base+19+80*(i-1),base,base,0xFFFFFFFF);

		//名前
		bold(driver,PX2+20, PY+80*(i-1)+2,0,font,name[i]);
		//時間
		bold(driver,PX2+145,PY+80*(i-1)+2,0,font," 5'00");
		//点数
		bold(driver,PX2+220,PY+80*(i-1)+2,0,font,"%3d", P_SCORE[i]);

		//ギブアップ状態
		if(P_LIFE[i] == 1)
			DrawImage(driver,"data/giveup.png",PX2+64,PY+80*(i-1),128,80,0,0,false);
	}
}

//ギブアップ
void game_giveup(s32 turn)
{
	P_LIFE[turn] = 1;
	nextPlayer();
}

//点数計算
s32 set_score(s32 turn, s32 blockNum)
{
	s32 score = 0;
	s32 x,y;

	//ブロックの数だけ点数加算
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			score += Block[blockNum][x][y];
		}
	}

	//ブロックを全部置いたらボーナス
	if(P_COUNT[turn] == 21)
	{
		score += 15;

		//1ブロックでクリアしたらさらにボーナス
		if(blockNum == 0)
			score += 5;
	}

	return score;
}


//マウスの左ボタンを押した時の処理
s32 game_click(s32 mousex, s32 mousey, s32 num, s32 col)
{
	s32 i, x, y;
	s32 tx = (mousex-PX-SPACE)/ BSIZE - 2;
	s32 ty = (mousey-PY-SPACE)/ BSIZE - 2;
	s32 size = 22;
	s32 y1,y2;
	s32 score = 0;

	//-------------------------------------
	//ブロックエリアの場合：ブロック選択
	//-------------------------------------
	y1 = PY + 80*(TURN-1) + 19;
	y2 = y1 + 44;
	if(mousex >= PX2+7 && mousex < PX2+7+242 && mousey > y1 && mousey < y2)
	{
		//ゲーム終了時には選択不可
		if(num == 22)
			return 1;

		//座標を再設定
		tx = (mousex - (PX2+7)) / size;
		ty = (mousey -  y1) / size;

		//ブロック番号取得
		i = tx + ty*11;
		if(i < 0)
			i = 0;

		if(P_BLOCK[TURN][i] == 0)
			BlockNo = i;

//		sound_PlaySe(2);

		return 2;
	}

	//-------------------------------------
	//エリア外を選択した場合のエラー処理
	//-------------------------------------
	//上端
	if((mousey-(PY+SPACE)) / BSIZE < 2)
	{
		for(i=0;i<5;i++){
			if(P_BLOCK_DETAIL[TURN][num][1 - (mousey-(PY+SPACE)) / BSIZE][i] == 1)
			{
//				sound_PlaySe(3);
				return 3;
			}
		}
	}
	//下端
	if((mousey-(PY+SPACE)) / BSIZE > 17)
	{
		for(i=0;i<5;i++){
			if(P_BLOCK_DETAIL[TURN][num][22 - (mousey-(PY+SPACE)) / BSIZE][i] == 1)
			{
//				sound_PlaySe(3);
				return 3;
			}
		}
	}
	//左端
	if((mousex-(PX+SPACE)) / BSIZE < 2)
	{
		for(i=0;i<5;i++){
			if(P_BLOCK_DETAIL[TURN][num][i][1 - (mousex-(PX+SPACE)) / BSIZE] == 1)
			{
//				sound_PlaySe(3);
				return 3;
			}
		}
	}
	//右端
	if((mousex-(PX+SPACE)) / BSIZE > 17)
	{
		for(i=0;i<5;i++){
			if(P_BLOCK_DETAIL[TURN][num][i][22 - (mousex-(PX+SPACE)) / BSIZE] == 1)
			{
//				sound_PlaySe(3);
				return 3;
			}
		}
	}

	//-------------------------------------
	//接点チェック
	//-------------------------------------
	if(!ptCheck(mousex, mousey, num, col))
	{
//		sound_PlaySe(3);
		return 3;
	}

	//-------------------------------------
	//描画エリアチェック
	//-------------------------------------
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			//描画したい場所にすでに存在する場合はエラー
			if(P_BLOCK_DETAIL[TURN][num][y][x] == 1 && board[ty+y][tx+x] != 0)
			{
//				sound_PlaySe(3);
				return 3;
			}
		}
	}

	//-------------------------------------
	//描画処理
	//-------------------------------------
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			if(P_BLOCK_DETAIL[TURN][num][y][x] == 1)
				board[ty+y][tx+x] = P_BLOCK_DETAIL[TURN][num][y][x] * col;
		}
	}

	//ブロックを使用済みに
	P_BLOCK[TURN][BlockNo] = 1;

	//ブロック使用数カウントアップ
	P_COUNT[TURN]++;

	//点数加算
	P_SCORE[TURN] += set_score(TURN, BlockNo);

	nextPlayer();

//	sound_PlaySe(0);
	return 1;
}

//勝ち負け状態表示
void game_state(s32 alpha, s32 state)
{
	SColor col = alpha<<24 | 0x00FFFFFF;
	switch(state)
	{
		case 0://READY
			DrawImage(driver,"data/ready.png",  PX,   PY+96,320,128,  0,  0,false,col);
			break;
		case 1://GO
			DrawImage(driver,"data/ready.png",  PX+64,PY+96,192,128,320,  0,false,col);
			break;
		case 2://WIN
			DrawImage(driver,"data/winlose.png",PX+34,PY+96,256,128,  0,  0, false,col);
			break;
		case 3://LOSE
			DrawImage(driver,"data/winlose.png",PX+34,PY+96,256,128,  0,128, false,col);
			break;
		case 4://END
			DrawImage(driver,"data/winlose.png",PX+34,PY+96,256,128,  0,256, false,col);
			break;
	}
}
