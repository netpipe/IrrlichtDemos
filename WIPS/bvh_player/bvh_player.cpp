/**
***  BVH動作ファイルの読み込み・再生のサンプルプログラム
***  Copyright (c) 2004-2007, Masaki OSHITA (www.oshita-lab.org)
**/


#ifdef  WIN32
	#include <windows.h>
#endif

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory.h>

#include "BVH.h"

#include <sys/stat.h>
#include <fcntl.h>


//
//  カメラ・GLUTの入力処理に関するグローバル変数
//

// カメラの回転のための変数
static float   camera_yaw = 0.0f;      // Ｙ軸を中心とする回転角度
static float   camera_pitch = -20.0f;  // Ｘ軸を中心とする回転角度
static float   camera_distance = 5.0f; // 中心からカメラの距離

// マウスのドラッグのための変数
static int     drag_mouse_r = 0; // 右ボタンがドラッグ中かどうかのフラグ（1:ドラッグ中, 0:非ドラッグ中）
static int     drag_mouse_l = 0; // 左ボタンがドラッグ中かどうかのフラグ（1:ドラッグ中, 0:非ドラッグ中）
static int     drag_mouse_m = 0; // 中ボタンがドラッグ中かどうかのフラグ（1:ドラッグ中, 0:非ドラッグ中）
static int     last_mouse_x, last_mouse_y; // 最後に記録されたマウスカーソルの座標

// ウィンドウのサイズ
static int     win_width, win_height;


//
//  アニメーション関連のグローバル変数
//

// アニメーション中かどうかを表すフラグ
bool   on_animation = true;

// アニメーションの再生時間
float  animation_time = 0.0f;

// 現在の表示フレーム番号
int    frame_no = 0;

// BVH動作データ
BVH *   bvh = NULL;



//
//  テキストを描画
//
void  drawMessage( int line_no, const char * message )
{
	int   i;
	if ( message == NULL )
		return;

	// 射影行列を初期化（初期化の前に現在の行列を退避）
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0.0, win_width, win_height, 0.0 );

	// モデルビュー行列を初期化（初期化の前に現在の行列を退避）
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	// Ｚバッファ・ライティングはオフにする
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );

	// メッセージの描画
	glColor3f( 1.0, 0.0, 0.0 );
	glRasterPos2i( 8, 24 + 18 * line_no );
	for ( i=0; message[i]!='\0'; i++ )
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, message[i] );

	// 設定を全て復元
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}


//
//  ウィンドウ再描画時に呼ばれるコールバック関数
//
void  display( void )
{
	// 画面をクリア
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	// 変換行列を設定（モデル座標系→カメラ座標系）
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0, 0.0, - camera_distance );
	glRotatef( - camera_pitch, 1.0, 0.0, 0.0 );
	glRotatef( - camera_yaw, 0.0, 1.0, 0.0 );
	glTranslatef( 0.0, -0.5, 0.0 );

	// 光源位置を再設定
	float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light0_position );

	// 地面を描画
	float  size = 1.5f;
	int  num_x = 10, num_z = 10;
	double  ox, oz;
	glBegin( GL_QUADS );
		glNormal3d( 0.0, 1.0, 0.0 );
		ox = -(num_x * size) / 2;
		for ( int x=0; x<num_x; x++, ox+=size )
		{
			oz = -(num_z * size) / 2;
			for ( int z=0; z<num_z; z++, oz+=size )
			{
				if ( ((x + z) % 2) == 0 )
					glColor3f( 1.0, 1.0, 1.0 );
				else
					glColor3f( 0.8, 0.8, 0.8 );
				glVertex3d( ox, 0.0, oz );
				glVertex3d( ox, 0.0, oz+size );
				glVertex3d( ox+size, 0.0, oz+size );
				glVertex3d( ox+size, 0.0, oz );
			}
		}
	glEnd();

	// キャラクタを描画
	glColor3f( 1.0f, 0.0f, 0.0f );
	if ( bvh )
		bvh->RenderFigure( frame_no, 0.02f );

	// 時間とフレーム番号を表示
	char  message[ 64 ];
	if ( bvh )
		sprintf( message, "%.2f (%d)", animation_time, frame_no );
	else
		sprintf( message, "Press 'L' key to Load a BVH file" );
	drawMessage( 0, message );

	// バックバッファに描画した画面をフロントバッファに表示
    glutSwapBuffers();
}


//
//  ウィンドウサイズ変更時に呼ばれるコールバック関数
//
void  reshape( int w, int h )
{
	// ウィンドウ内の描画を行う範囲を設定（ここではウィンドウ全体に描画）
    glViewport(0, 0, w, h);

	// カメラ座標系→スクリーン座標系への変換行列を設定
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45, (double)w/h, 1, 500 );

	// ウィンドウのサイズを記録（テキスト描画処理のため）
	win_width = w;
	win_height = h;
}


//
// マウスクリック時に呼ばれるコールバック関数
//
void  mouse( int button, int state, int mx, int my )
{
	// 左ボタンが押されたらドラッグ開始
	if ( ( button == GLUT_LEFT_BUTTON ) && ( state == GLUT_DOWN ) )
		drag_mouse_l = 1;
	// 左ボタンが離されたらドラッグ終了
	else if ( ( button == GLUT_LEFT_BUTTON ) && ( state == GLUT_UP ) )
		drag_mouse_l = 0;

	// 右ボタンが押されたらドラッグ開始
	if ( ( button == GLUT_RIGHT_BUTTON ) && ( state == GLUT_DOWN ) )
		drag_mouse_r = 1;
	// 右ボタンが離されたらドラッグ終了
	else if ( ( button == GLUT_RIGHT_BUTTON ) && ( state == GLUT_UP ) )
		drag_mouse_r = 0;

	// 中ボタンが押されたらドラッグ開始
	if ( ( button == GLUT_MIDDLE_BUTTON ) && ( state == GLUT_DOWN ) )
		drag_mouse_m = 1;
	// 中ボタンが離されたらドラッグ終了
	else if ( ( button == GLUT_MIDDLE_BUTTON ) && ( state == GLUT_UP ) )
		drag_mouse_m = 0;

	// 再描画
	glutPostRedisplay();

	// 現在のマウス座標を記録
	last_mouse_x = mx;
	last_mouse_y = my;
}


//
// マウスドラッグ時に呼ばれるコールバック関数
//
void  motion( int mx, int my )
{
	// 右ボタンのドラッグ中は視点を回転する
	if ( drag_mouse_r )
	{
		// 前回のマウス座標と今回のマウス座標の差に応じて視点を回転

		// マウスの横移動に応じてＹ軸を中心に回転
		camera_yaw -= ( mx - last_mouse_x ) * 1.0;
		if ( camera_yaw < 0.0 )
			camera_yaw += 360.0;
		else if ( camera_yaw > 360.0 )
			camera_yaw -= 360.0;

		// マウスの縦移動に応じてＸ軸を中心に回転
		camera_pitch -= ( my - last_mouse_y ) * 1.0;
		if ( camera_pitch < -90.0 )
			camera_pitch = -90.0;
		else if ( camera_pitch > 90.0 )
			camera_pitch = 90.0;
	}
	// 左ボタンのドラッグ中は視点とカメラの距離を変更する
	if ( drag_mouse_l )
	{
		// 前回のマウス座標と今回のマウス座標の差に応じて視点を回転

		// マウスの縦移動に応じて距離を移動
		camera_distance += ( my - last_mouse_y ) * 0.2;
		if ( camera_distance < 2.0 )
			camera_distance = 2.0;
	}

	// 今回のマウス座標を記録
	last_mouse_x = mx;
	last_mouse_y = my;

	// 再描画
	glutPostRedisplay();
}


//
//  キーボードのキーが押されたときに呼ばれるコールバック関数
//
void  keyboard( unsigned char key, int mx, int my )
{
	// s キーでアニメーションの停止・再開
	if ( key == 's' )
		on_animation = !on_animation;

	// n キーで次のフレーム
	if ( ( key == 'n' ) && !on_animation )
	{
		animation_time += bvh->GetInterval();
		frame_no ++;
		frame_no = frame_no % bvh->GetNumFrame();
	}

	// p キーで前のフレーム
	if ( ( key == 'p' ) && !on_animation && ( frame_no > 0 ) && bvh )
	{
		animation_time -= bvh->GetInterval();
		frame_no --;
		frame_no = frame_no % bvh->GetNumFrame();
	}

	// r キーでアニメーションのリセット
	if ( key == 'r' )
	{
		animation_time = 0.0f;
		frame_no = 0;
	}

	// l キーで再生動作の変更
	if ( key == 'l' )
	{
//#ifdef  WIN32
		const int  file_name_len = 256;
		char  file_name[ file_name_len ] = "./Male1_A1_Stand.bvh";//"B02.bvh";

//		// ファイルダイアログの設定
		//OPENFILENAME	open_file;

		// possibly messed up the file opening command it was for windows
		open open_file;
		memset( &open_file, 0, sizeof(open_file) );
//		open_file.lStructSize = sizeof(open_file);
//		open_file.hwndOwner = NULL;
//		open_file.lpstrFilter = "BVH Motion Data (*.bvh)\0*.bvh\0All (*.*)\0*.*\0";
//		open_file.nFilterIndex = 1;
//		open_file.lpstrFile = file_name;
//		open_file.nMaxFile = file_name_len;
//		open_file.lpstrTitle = "Select a BVH file";
//		open_file.lpstrDefExt = "bvh";
//		open_file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
//
//		// ファイルダイアログを表示
		BOOL  ret = GetOpenFileName( &open_file );
//
//		// ファイルが指定されたら新しい動作を設定
//		if( ret )
//		{
			// 動作データを読み込み
			if ( bvh )
				delete  bvh;
			bvh = new BVH( file_name );

			// 読み込みに失敗したら削除
			if ( !bvh->IsLoadSuccess() )
			{
				delete  bvh;
				bvh = NULL;
			}

			//	アニメーションをリセット
			animation_time = 0.0f;
			frame_no = 0;
//		}
//#endif
	}

	glutPostRedisplay();
}


//
//  アイドル時に呼ばれるコールバック関数
//
void  idle( void )
{
	// アニメーション処理
	if ( on_animation )
	{
#ifdef  WIN32
		// システム時間を取得し、前回からの経過時間に応じてΔｔを決定
		static DWORD  last_time = 0;
		DWORD  curr_time = timeGetTime();
		float  delta = ( curr_time - last_time ) * 0.001f;
		if ( delta > 0.03f )
			delta = 0.03f;
		last_time = curr_time;
		animation_time += delta;
#else
		// 固定のΔｔを使用
		animation_time += 0.03f;
#endif
		// 現在のフレーム番号を計算
		if ( bvh )
		{
			frame_no = animation_time / bvh->GetInterval();
			frame_no = frame_no % bvh->GetNumFrame();
		}
		else
			frame_no = 0;

		// 再描画の指示を出す（この後で再描画のコールバック関数が呼ばれる）
		glutPostRedisplay();
	}
}


//
//  環境初期化関数
//
void  initEnvironment( void )
{
	// 光源を作成する
	float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
	float  light0_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float  light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light0_position );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light0_diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, light0_specular );
	glLightfv( GL_LIGHT0, GL_AMBIENT, light0_ambient );
	glEnable( GL_LIGHT0 );

	// 光源計算を有効にする
	glEnable( GL_LIGHTING );

	// 物体の色情報を有効にする
	glEnable( GL_COLOR_MATERIAL );

	// Ｚテストを有効にする
	glEnable( GL_DEPTH_TEST );

	// 背面除去を有効にする
	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );

	// 背景色を設定
	glClearColor( 0.5, 0.5, 0.8, 0.0 );

	// 初期のBVH動作データを読み込み
	//bvh = new BVH( "./B02.bvh" );
}


//
//  メイン関数（プログラムはここから開始）
//
int  main( int argc, char ** argv )
{
#ifdef  WIN32
	// Visual Studio 2007 のバグで、マルチバイト文字で表されたパスに日本語を含むときに、
	// ファイルを正しく開けないため、問題を回避するために、正しいロケールを設定
	locale::global( locale("japanese") );
#endif

	// GLUTの初期化
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL );
	glutInitWindowSize( 640, 640 );
	glutInitWindowPosition( 0, 0 );
    glutCreateWindow("BVH Player Sample");

	// コールバック関数の登録
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutKeyboardFunc( keyboard );
	glutIdleFunc( idle );

	// 環境初期化
	initEnvironment();

	// GLUTのメインループに処理を移す
    glutMainLoop();
    return 0;
}

