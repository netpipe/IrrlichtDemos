
#include "blokus.h"

c8 Block[22][5][5] =
{
	//22�ڂ͈ꎞ�ۑ��p
	//23�ڂ̓_�~�[
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
	0,0,1,0,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
	0,0,1,1,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
	0,1,1,1,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,1,0,0,	//����������
	0,0,1,1,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
	1,1,1,1,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,1,0,0,	//����������
	0,1,1,1,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,0,1,0,	//����������
	0,1,1,1,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,1,1,0,	//����������
	0,1,1,0,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,1,1,0,0,	//����������
	0,1,1,0,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
	1,1,1,1,1,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,1,0,0,0,	//����������
	1,1,1,1,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,1,0,1,0,	//����������
	0,1,1,1,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,1,1,0,0,	//����������
	0,0,1,0,0,	//����������
	0,0,1,1,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,1,1,0,0,	//����������
	0,0,1,1,0,	//����������
	0,0,1,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,0,1,0,	//����������
	0,0,1,1,0,	//����������
	0,0,1,0,0,	//����������
	0,0,1,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,1,0,0,	//����������
	0,1,1,0,0,	//����������
	0,1,1,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,1,0,0,	//����������
	0,0,1,0,0,	//����������
	0,0,1,0,0,	//����������
	0,0,1,1,0,	//����������

	0,0,0,0,0,	//����������
	0,0,1,0,0,	//����������
	0,1,1,1,0,	//����������
	0,0,1,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,1,1,0,	//����������
	0,1,1,0,0,	//����������
	0,1,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,1,1,1,0,	//����������
	0,1,0,0,0,	//����������
	0,1,0,0,0,	//����������
	0,0,0,0,0,	//����������

	0,0,0,0,0,	//����������
	0,0,1,0,0,	//����������
	0,0,1,0,0,	//����������
	0,1,1,1,0,	//����������
	0,0,0,0,0,	//����������

	//22�ڂ͈ꎞ�ۑ��p
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
	0,0,0,0,0,	//����������
};

//�u���b�N�T�C�Y
#define BSIZE 15

//�`��ʒu
#define PX    10	//�{�[�h�̕`��ʒu
#define PX2   360	//�v���C���[�u���b�N�̕`��ʒu
#define PY    60	//�S�̂̕`��ʒu

//�{�[�h�T�C�Y
#define BOARD 320	//�S�̂̃T�C�Y
#define SPACE 10	//���͂̋󂫃X�y�[�X

//����
#define TIME 6

s32 BlockNo = 0;
s32 P_BLOCK[6][21];	//�c���Ă���u���b�N
c8  P_BLOCK_DETAIL[6][22][5][5];	//�莝���̃u���b�N(�e���̉�]�Ȃǂ��L��)
c8  P_LIFE[6];	//�������
s32 P_SCORE[6];	//�_��
s32 P_TIME[6];	//�c�莞��
c8  P_COUNT[6];	//�g�p�u���b�N�̐�
s32 TURN = 0;	//���݂̃^�[��

c8 board[20][20];
SColor col[6] = {0xFFFFFFFF,0xFF0000FF,0xFFFF0000,0xFF00FF00,0xFFFFFF00,0x00000000};

typedef struct POINT {
  long x;
  long y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;


//-------------------
//�J���[�ݒ�F�ʏ�
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
//�J���[�ݒ�F�Â�
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
//���̃u���b�N����
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
//�O�̃u���b�N����
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
//��]�E���]����
//0:X����]
//1:Y����]
//2:����]
//3:�E��]
//-------------------
void rotate(s32 mode, s32 num)
{
	s32 x, y;
	c8 buf[5][5];

	//������������������̂ŁA�f�[�^����U�ޔ�
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
//���̃^�[���ݒ�
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
				TURN = 5;//�Q�[���I��

			if(block_next() == 23)
				TURN = 5;//�Q�[���I��
			break;
		}
	}

	//���̃u���b�N����
	if(old != TURN)
		block_next();
}

//�z�u����ۂ̊p�`�F�b�N
bool ptCheck(s32 mousex, s32 mousey, s32 num, s32 color)
{
	s32 x, y;

	//�����ݒ�
	s32 mx = (mousex-PX-SPACE)/BSIZE - 2;
	s32 my = (mousey-PY-SPACE)/BSIZE - 2;

	bool bOK = false;

	s32 i,cnt = 0;
	POINT pt[5];

	//���̏ꏊ�Ɛ����擾
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

	//�ŏ��̊p�̃`�F�b�N
	if(P_COUNT[color] == 0)
	{
		for(i=0;i<cnt;i++)
		{
			//�ǂ̊p���g���Ă�OK
			if(board[ 0][ 0] == 0 && pt[i].x ==  0 && pt[i].y ==  0)return true;
			if(board[ 0][19] == 0 && pt[i].x == 19 && pt[i].y ==  0)return true;
			if(board[19][19] == 0 && pt[i].x == 19 && pt[i].y == 19)return true;
			if(board[19][ 0] == 0 && pt[i].x ==  0 && pt[i].y == 19)return true;

			/*
			//�p�̎w��
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

	//�e���ɑ΂��ă`�F�b�N
	for(i=0;i<cnt;i++)
	{
		//�p�Ɗp�łȂ����邩�`�F�b�N
		if((pt[i].y-1 >= 0)  && (pt[i].x-1 >= 0)  && (board[pt[i].y-1][pt[i].x-1]%10 == color))
			bOK = true;	//����
		if((pt[i].y-1 >= 0)  && (pt[i].x+1 <= 19) && (board[pt[i].y-1][pt[i].x+1]%10 == color))
			bOK = true;	//�E��
		if((pt[i].y+1 <= 19) && (pt[i].x-1 >= 0)  && (board[pt[i].y+1][pt[i].x-1]%10 == color))
			bOK = true;	//����
		if((pt[i].y+1 <= 19) && (pt[i].x+1 <= 19) && (board[pt[i].y+1][pt[i].x+1]%10 == color))
			bOK = true;	//�E��

		//�ׂ����Ă��铯���F�́������邩�`�F�b�N
		if((pt[i].y-1 >= 0)  && (board[pt[i].y-1][pt[i].x  ]%10 == color))
			return false;	//��
		if((pt[i].y+1 <= 19) && (board[pt[i].y+1][pt[i].x  ]%10 == color))
			return false;	//��
		if((pt[i].x-1 >= 0)  && (board[pt[i].y  ][pt[i].x-1]%10 == color))
			return false;	//��
		if((pt[i].x+1 <= 19) && (board[pt[i].y  ][pt[i].x+1]%10 == color))
			return false;	//�E
	}

	//�p�Ɗp�łȂ����邩��Ԃ�
	return bOK;
}

//�Q�[��������
void game_init()
{
	int i;

	//������Ԃ̏�����
	memset(P_LIFE, 0x00, sizeof(P_LIFE));

	//�g�p�u���b�N���̏�����
	memset(P_COUNT, 0x00, sizeof(P_COUNT));

	//�_���̏�����
	memset(P_SCORE, 0x00, sizeof(P_SCORE));

	//�{�[�h�̏�����
	memset(board, 0x00, sizeof(board));

	//�v���C���[�̃u���b�N�̏�����
	for(i=0;i<5;i++){
		memset(P_BLOCK[i], 0x00, sizeof(P_BLOCK[i]));
		memcpy(P_BLOCK_DETAIL[i],Block,sizeof(Block));
		P_TIME[i] = TIME*60;
	}

	BlockNo = 0;

	TURN = 0;
}

//�Q�[���J�n
void game_start()
{
	game_init();

	//���Ԍ���
	TURN = 1;//rand()%4+1;

//	enemy(TURN);
}

//�{�[�h�̕`��
void game_board()
{
	s32 x, y;
	s32 point  = 0;

	//��U��ʏ���
	DrawImage(driver,"data/board.png", PX,PY,BOARD,BOARD,0,0,false,0xFFFFFFFF);

	//�u���b�N�̕`��
	for(x=0;x<20;x++)
	{
		for(y=0;y<20;y++)
		{
			point = board[y][x];
			if(point != 0)
			{
				//��`�h��Ԃ�
				//FillRect(driver,PX+SPACE+BSIZE*x, PY+SPACE+BSIZE*y, BSIZE, BSIZE, set_col1(point));

				//�摜�g�p
				DrawImage(driver,"data/board.png",PX+SPACE+BSIZE*x,PY+SPACE+BSIZE*y,BSIZE,BSIZE,256,320,false,set_col1(point));
			}
		}
	}
}

//���ݑI�����Ă���s�[�X�̕`��
void game_peace(s32 mousex, s32 mousey, s32 num, s32 turn)
{
	s32 x, y, color;

	//����̍��W���v�Z
	s32 tx = (mousex-PX-SPACE)/ BSIZE - 2;
	s32 ty = (mousey-PY-SPACE)/ BSIZE - 2;

	if(turn == 0 || turn == 5)
		return;

	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			//�󔒂łȂ���΁A�`�悷��
			if(P_BLOCK_DETAIL[turn][num][x][y] == 1)
			{
				if( tx+y >= 0 && tx+y <= 19 && ty+x >= 0 && ty+x <= 19)
				{
					//�w�i�F�ɑΉ���������`��
					color = board[ty+x][tx+y];
					FillRect(driver, (tx+y)*BSIZE+PX+SPACE, (ty+x)*BSIZE+PY+SPACE, BSIZE, BSIZE, set_col2(color));

					//�g��`��
					DrawRect(driver, (tx+y)*BSIZE+PX+SPACE, (ty+x)*BSIZE+PY+SPACE, BSIZE, BSIZE, set_col1(turn));
				}
			}
		}
	}
}

//�u���b�N�P�̂̕`��
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
			//�u���b�N�f�[�^�������
			if(P_BLOCK_DETAIL[playerNum][blockNum][y][x] != 0)
			{
				//�F�ݒ�
				if(TURN == playerNum)
					col = set_col1(playerNum);
				else
					col = set_col2(playerNum);


				//�v���C���[�̃u���b�N�������
				if(P_BLOCK[playerNum][blockNum] == 0)
					 FillRect(driver,
						px+size*x + blockNum%11*base + 7+1,
						py+size*y + blockNum/11*base + 19+1,
						size, size, col);
			}
		}
	}
}

//�S�v���C���[�̎c��u���b�N�`��
void game_block(s32 color)
{
	s32 base = 22;
	s32 i,num;
	SColor col;
	SColor font;
	c8 name[6][16] = {"","BLUE","GREEN","RED","YELLOW",""};

	for(i=1;i<5;i++)
	{
		//�F�ݒ�
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

		//�g
		DrawImage(driver,"data/board.png",PX2,PY+80*(i-1),256,70,0,320,false,col);

		//�u���b�N�`��
		for(num=0;num<21;num++)
			block(i, num, PX2, PY+80*(i-1));

		//���ݑI�����Ă���s�[�X
		if(TURN == i)
			DrawRect(driver,PX2+BlockNo%11*base+7,PY+BlockNo/11*base+19+80*(i-1),base,base,0xFFFFFFFF);

		//���O
		bold(driver,PX2+20, PY+80*(i-1)+2,0,font,name[i]);
		//����
		bold(driver,PX2+145,PY+80*(i-1)+2,0,font," 5'00");
		//�_��
		bold(driver,PX2+220,PY+80*(i-1)+2,0,font,"%3d", P_SCORE[i]);

		//�M�u�A�b�v���
		if(P_LIFE[i] == 1)
			DrawImage(driver,"data/giveup.png",PX2+64,PY+80*(i-1),128,80,0,0,false);
	}
}

//�M�u�A�b�v
void game_giveup(s32 turn)
{
	P_LIFE[turn] = 1;
	nextPlayer();
}

//�_���v�Z
s32 set_score(s32 turn, s32 blockNum)
{
	s32 score = 0;
	s32 x,y;

	//�u���b�N�̐������_�����Z
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			score += Block[blockNum][x][y];
		}
	}

	//�u���b�N��S���u������{�[�i�X
	if(P_COUNT[turn] == 21)
	{
		score += 15;

		//1�u���b�N�ŃN���A�����炳��Ƀ{�[�i�X
		if(blockNum == 0)
			score += 5;
	}

	return score;
}


//�}�E�X�̍��{�^�������������̏���
s32 game_click(s32 mousex, s32 mousey, s32 num, s32 col)
{
	s32 i, x, y;
	s32 tx = (mousex-PX-SPACE)/ BSIZE - 2;
	s32 ty = (mousey-PY-SPACE)/ BSIZE - 2;
	s32 size = 22;
	s32 y1,y2;
	s32 score = 0;

	//-------------------------------------
	//�u���b�N�G���A�̏ꍇ�F�u���b�N�I��
	//-------------------------------------
	y1 = PY + 80*(TURN-1) + 19;
	y2 = y1 + 44;
	if(mousex >= PX2+7 && mousex < PX2+7+242 && mousey > y1 && mousey < y2)
	{
		//�Q�[���I�����ɂ͑I��s��
		if(num == 22)
			return 1;

		//���W���Đݒ�
		tx = (mousex - (PX2+7)) / size;
		ty = (mousey -  y1) / size;

		//�u���b�N�ԍ��擾
		i = tx + ty*11;
		if(i < 0)
			i = 0;

		if(P_BLOCK[TURN][i] == 0)
			BlockNo = i;

//		sound_PlaySe(2);

		return 2;
	}

	//-------------------------------------
	//�G���A�O��I�������ꍇ�̃G���[����
	//-------------------------------------
	//��[
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
	//���[
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
	//���[
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
	//�E�[
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
	//�ړ_�`�F�b�N
	//-------------------------------------
	if(!ptCheck(mousex, mousey, num, col))
	{
//		sound_PlaySe(3);
		return 3;
	}

	//-------------------------------------
	//�`��G���A�`�F�b�N
	//-------------------------------------
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			//�`�悵�����ꏊ�ɂ��łɑ��݂���ꍇ�̓G���[
			if(P_BLOCK_DETAIL[TURN][num][y][x] == 1 && board[ty+y][tx+x] != 0)
			{
//				sound_PlaySe(3);
				return 3;
			}
		}
	}

	//-------------------------------------
	//�`�揈��
	//-------------------------------------
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			if(P_BLOCK_DETAIL[TURN][num][y][x] == 1)
				board[ty+y][tx+x] = P_BLOCK_DETAIL[TURN][num][y][x] * col;
		}
	}

	//�u���b�N���g�p�ς݂�
	P_BLOCK[TURN][BlockNo] = 1;

	//�u���b�N�g�p���J�E���g�A�b�v
	P_COUNT[TURN]++;

	//�_�����Z
	P_SCORE[TURN] += set_score(TURN, BlockNo);

	nextPlayer();

//	sound_PlaySe(0);
	return 1;
}

//����������ԕ\��
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
