/**
***  BVH����t�@�C���̓ǂݍ��݁E�Đ��̃T���v���v���O����
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
//  �J�����EGLUT�̓��͏����Ɋւ���O���[�o���ϐ�
//

// �J�����̉�]�̂��߂̕ϐ�
static float   camera_yaw = 0.0f;      // �x���𒆐S�Ƃ����]�p�x
static float   camera_pitch = -20.0f;  // �w���𒆐S�Ƃ����]�p�x
static float   camera_distance = 5.0f; // ���S����J�����̋���

// �}�E�X�̃h���b�O�̂��߂̕ϐ�
static int     drag_mouse_r = 0; // �E�{�^�����h���b�O�����ǂ����̃t���O�i1:�h���b�O��, 0:��h���b�O���j
static int     drag_mouse_l = 0; // ���{�^�����h���b�O�����ǂ����̃t���O�i1:�h���b�O��, 0:��h���b�O���j
static int     drag_mouse_m = 0; // ���{�^�����h���b�O�����ǂ����̃t���O�i1:�h���b�O��, 0:��h���b�O���j
static int     last_mouse_x, last_mouse_y; // �Ō�ɋL�^���ꂽ�}�E�X�J�[�\���̍��W

// �E�B���h�E�̃T�C�Y
static int     win_width, win_height;


//
//  �A�j���[�V�����֘A�̃O���[�o���ϐ�
//

// �A�j���[�V���������ǂ�����\���t���O
bool   on_animation = true;

// �A�j���[�V�����̍Đ�����
float  animation_time = 0.0f;

// ���݂̕\���t���[���ԍ�
int    frame_no = 0;

// BVH����f�[�^
BVH *   bvh = NULL;



//
//  �e�L�X�g��`��
//
void  drawMessage( int line_no, const char * message )
{
	int   i;
	if ( message == NULL )
		return;

	// �ˉe�s����������i�������̑O�Ɍ��݂̍s���ޔ��j
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0.0, win_width, win_height, 0.0 );

	// ���f���r���[�s����������i�������̑O�Ɍ��݂̍s���ޔ��j
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	// �y�o�b�t�@�E���C�e�B���O�̓I�t�ɂ���
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );

	// ���b�Z�[�W�̕`��
	glColor3f( 1.0, 0.0, 0.0 );
	glRasterPos2i( 8, 24 + 18 * line_no );
	for ( i=0; message[i]!='\0'; i++ )
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, message[i] );

	// �ݒ��S�ĕ���
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}


//
//  �E�B���h�E�ĕ`�掞�ɌĂ΂��R�[���o�b�N�֐�
//
void  display( void )
{
	// ��ʂ��N���A
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	// �ϊ��s���ݒ�i���f�����W�n���J�������W�n�j
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0, 0.0, - camera_distance );
	glRotatef( - camera_pitch, 1.0, 0.0, 0.0 );
	glRotatef( - camera_yaw, 0.0, 1.0, 0.0 );
	glTranslatef( 0.0, -0.5, 0.0 );

	// �����ʒu���Đݒ�
	float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light0_position );

	// �n�ʂ�`��
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

	// �L�����N�^��`��
	glColor3f( 1.0f, 0.0f, 0.0f );
	if ( bvh )
		bvh->RenderFigure( frame_no, 0.02f );

	// ���Ԃƃt���[���ԍ���\��
	char  message[ 64 ];
	if ( bvh )
		sprintf( message, "%.2f (%d)", animation_time, frame_no );
	else
		sprintf( message, "Press 'L' key to Load a BVH file" );
	drawMessage( 0, message );

	// �o�b�N�o�b�t�@�ɕ`�悵����ʂ��t�����g�o�b�t�@�ɕ\��
    glutSwapBuffers();
}


//
//  �E�B���h�E�T�C�Y�ύX���ɌĂ΂��R�[���o�b�N�֐�
//
void  reshape( int w, int h )
{
	// �E�B���h�E���̕`����s���͈͂�ݒ�i�����ł̓E�B���h�E�S�̂ɕ`��j
    glViewport(0, 0, w, h);

	// �J�������W�n���X�N���[�����W�n�ւ̕ϊ��s���ݒ�
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45, (double)w/h, 1, 500 );

	// �E�B���h�E�̃T�C�Y���L�^�i�e�L�X�g�`�揈���̂��߁j
	win_width = w;
	win_height = h;
}


//
// �}�E�X�N���b�N���ɌĂ΂��R�[���o�b�N�֐�
//
void  mouse( int button, int state, int mx, int my )
{
	// ���{�^���������ꂽ��h���b�O�J�n
	if ( ( button == GLUT_LEFT_BUTTON ) && ( state == GLUT_DOWN ) )
		drag_mouse_l = 1;
	// ���{�^���������ꂽ��h���b�O�I��
	else if ( ( button == GLUT_LEFT_BUTTON ) && ( state == GLUT_UP ) )
		drag_mouse_l = 0;

	// �E�{�^���������ꂽ��h���b�O�J�n
	if ( ( button == GLUT_RIGHT_BUTTON ) && ( state == GLUT_DOWN ) )
		drag_mouse_r = 1;
	// �E�{�^���������ꂽ��h���b�O�I��
	else if ( ( button == GLUT_RIGHT_BUTTON ) && ( state == GLUT_UP ) )
		drag_mouse_r = 0;

	// ���{�^���������ꂽ��h���b�O�J�n
	if ( ( button == GLUT_MIDDLE_BUTTON ) && ( state == GLUT_DOWN ) )
		drag_mouse_m = 1;
	// ���{�^���������ꂽ��h���b�O�I��
	else if ( ( button == GLUT_MIDDLE_BUTTON ) && ( state == GLUT_UP ) )
		drag_mouse_m = 0;

	// �ĕ`��
	glutPostRedisplay();

	// ���݂̃}�E�X���W���L�^
	last_mouse_x = mx;
	last_mouse_y = my;
}


//
// �}�E�X�h���b�O���ɌĂ΂��R�[���o�b�N�֐�
//
void  motion( int mx, int my )
{
	// �E�{�^���̃h���b�O���͎��_����]����
	if ( drag_mouse_r )
	{
		// �O��̃}�E�X���W�ƍ���̃}�E�X���W�̍��ɉ����Ď��_����]

		// �}�E�X�̉��ړ��ɉ����Ăx���𒆐S�ɉ�]
		camera_yaw -= ( mx - last_mouse_x ) * 1.0;
		if ( camera_yaw < 0.0 )
			camera_yaw += 360.0;
		else if ( camera_yaw > 360.0 )
			camera_yaw -= 360.0;

		// �}�E�X�̏c�ړ��ɉ����Ăw���𒆐S�ɉ�]
		camera_pitch -= ( my - last_mouse_y ) * 1.0;
		if ( camera_pitch < -90.0 )
			camera_pitch = -90.0;
		else if ( camera_pitch > 90.0 )
			camera_pitch = 90.0;
	}
	// ���{�^���̃h���b�O���͎��_�ƃJ�����̋�����ύX����
	if ( drag_mouse_l )
	{
		// �O��̃}�E�X���W�ƍ���̃}�E�X���W�̍��ɉ����Ď��_����]

		// �}�E�X�̏c�ړ��ɉ����ċ������ړ�
		camera_distance += ( my - last_mouse_y ) * 0.2;
		if ( camera_distance < 2.0 )
			camera_distance = 2.0;
	}

	// ����̃}�E�X���W���L�^
	last_mouse_x = mx;
	last_mouse_y = my;

	// �ĕ`��
	glutPostRedisplay();
}


//
//  �L�[�{�[�h�̃L�[�������ꂽ�Ƃ��ɌĂ΂��R�[���o�b�N�֐�
//
void  keyboard( unsigned char key, int mx, int my )
{
	// s �L�[�ŃA�j���[�V�����̒�~�E�ĊJ
	if ( key == 's' )
		on_animation = !on_animation;

	// n �L�[�Ŏ��̃t���[��
	if ( ( key == 'n' ) && !on_animation )
	{
		animation_time += bvh->GetInterval();
		frame_no ++;
		frame_no = frame_no % bvh->GetNumFrame();
	}

	// p �L�[�őO�̃t���[��
	if ( ( key == 'p' ) && !on_animation && ( frame_no > 0 ) && bvh )
	{
		animation_time -= bvh->GetInterval();
		frame_no --;
		frame_no = frame_no % bvh->GetNumFrame();
	}

	// r �L�[�ŃA�j���[�V�����̃��Z�b�g
	if ( key == 'r' )
	{
		animation_time = 0.0f;
		frame_no = 0;
	}

	// l �L�[�ōĐ�����̕ύX
	if ( key == 'l' )
	{
//#ifdef  WIN32
		const int  file_name_len = 256;
		char  file_name[ file_name_len ] = "./Male1_A1_Stand.bvh";//"B02.bvh";

//		// �t�@�C���_�C�A���O�̐ݒ�
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
//		// �t�@�C���_�C�A���O��\��
		bool  ret = GetOpenFileName( &open_file );
//
//		// �t�@�C�����w�肳�ꂽ��V���������ݒ�
//		if( ret )
//		{
			// ����f�[�^��ǂݍ���
			if ( bvh )
				delete  bvh;
			bvh = new BVH( file_name );

			// �ǂݍ��݂Ɏ��s������폜
			if ( !bvh->IsLoadSuccess() )
			{
				delete  bvh;
				bvh = NULL;
			}

			//	�A�j���[�V���������Z�b�g
			animation_time = 0.0f;
			frame_no = 0;
//		}
//#endif
	}

	glutPostRedisplay();
}


//
//  �A�C�h�����ɌĂ΂��R�[���o�b�N�֐�
//
void  idle( void )
{
	// �A�j���[�V��������
	if ( on_animation )
	{
#ifdef  WIN32
		// �V�X�e�����Ԃ��擾���A�O�񂩂�̌o�ߎ��Ԃɉ����ă���������
		static DWORD  last_time = 0;
		DWORD  curr_time = timeGetTime();
		float  delta = ( curr_time - last_time ) * 0.001f;
		if ( delta > 0.03f )
			delta = 0.03f;
		last_time = curr_time;
		animation_time += delta;
#else
		// �Œ�̃������g�p
		animation_time += 0.03f;
#endif
		// ���݂̃t���[���ԍ����v�Z
		if ( bvh )
		{
			frame_no = animation_time / bvh->GetInterval();
			frame_no = frame_no % bvh->GetNumFrame();
		}
		else
			frame_no = 0;

		// �ĕ`��̎w�����o���i���̌�ōĕ`��̃R�[���o�b�N�֐����Ă΂��j
		glutPostRedisplay();
	}
}


//
//  ���������֐�
//
void  initEnvironment( void )
{
	// �������쐬����
	float  light0_position[] = { 10.0, 10.0, 10.0, 1.0 };
	float  light0_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float  light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float  light0_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light0_position );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light0_diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, light0_specular );
	glLightfv( GL_LIGHT0, GL_AMBIENT, light0_ambient );
	glEnable( GL_LIGHT0 );

	// �����v�Z��L���ɂ���
	glEnable( GL_LIGHTING );

	// ���̂̐F����L���ɂ���
	glEnable( GL_COLOR_MATERIAL );

	// �y�e�X�g��L���ɂ���
	glEnable( GL_DEPTH_TEST );

	// �w�ʏ�����L���ɂ���
	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );

	// �w�i�F��ݒ�
	glClearColor( 0.5, 0.5, 0.8, 0.0 );

	// ������BVH����f�[�^��ǂݍ���
	//bvh = new BVH( "./B02.bvh" );
}


//
//  ���C���֐��i�v���O�����͂�������J�n�j
//
int  main( int argc, char ** argv )
{
#ifdef  WIN32
	// Visual Studio 2007 �̃o�O�ŁA�}���`�o�C�g�����ŕ\���ꂽ�p�X�ɓ��{����܂ނƂ��ɁA
	// �t�@�C���𐳂����J���Ȃ����߁A����������邽�߂ɁA���������P�[����ݒ�
	locale::global( locale("japanese") );
#endif

	// GLUT�̏�����
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL );
	glutInitWindowSize( 640, 640 );
	glutInitWindowPosition( 0, 0 );
    glutCreateWindow("BVH Player Sample");

	// �R�[���o�b�N�֐��̓o�^
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutKeyboardFunc( keyboard );
	glutIdleFunc( idle );

	// ��������
	initEnvironment();

	// GLUT�̃��C�����[�v�ɏ������ڂ�
    glutMainLoop();
    return 0;
}

