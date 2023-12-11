/**
***  BVH����t�@�C���̓ǂݍ��݁E�`��N���X
***  Copyright (c) 2004-2007, Masaki OSHITA (www.oshita-lab.org)
**/


#include <fstream>
#include <string.h>
#include <stdlib.h>

#include "BVH.h"


// �R���g���N�^
BVH::BVH()
{
	motion = NULL;
	Clear();
}

// �R���g���N�^
BVH::BVH( const char * bvh_file_name )
{
	motion = NULL;
	Clear();

	Load( bvh_file_name );
}

// �f�X�g���N�^
BVH::~BVH()
{
	Clear();
}


// �S���̃N���A
void  BVH::Clear()
{
	int  i;
	for ( i=0; i<channels.size(); i++ )
		delete  channels[ i ];
	for ( i=0; i<joints.size(); i++ )
		delete  joints[ i ];
	if ( motion != NULL )
		delete  motion;

	is_load_success = false;

	file_name = "";
	motion_name = "";

	num_channel = 0;
	channels.clear();
	joints.clear();
	joint_index.clear();

	num_frame = 0;
	interval = 0.0;
	motion = NULL;
}



//
//  BVH�t�@�C���̃��[�h
//
void  BVH::Load( const char * bvh_file_name )
{
	#define  BUFFER_LENGTH  1024*4

	ifstream  file;
	char      line[ BUFFER_LENGTH ];
	char *    token;
	char      separater[] = " :,\t";
	vector< Joint * >   joint_stack;
	Joint *   joint = NULL;
	Joint *   new_joint = NULL;
	bool      is_site = false;
	double    x, y ,z;
	int       i, j;

	// ������
	Clear();

	// �t�@�C���̏��i�t�@�C�����E���얼�j�̐ݒ�
	file_name = bvh_file_name;
	const char *  mn_first = bvh_file_name;
	const char *  mn_last = bvh_file_name + strlen( bvh_file_name );
	if ( strrchr( bvh_file_name, '\\' ) != NULL )
		mn_first = strrchr( bvh_file_name, '\\' ) + 1;
	else if ( strrchr( bvh_file_name, '/' ) != NULL )
		mn_first = strrchr( bvh_file_name, '/' ) + 1;
	if ( strrchr( bvh_file_name, '.' ) != NULL )
		mn_last = strrchr( bvh_file_name, '.' );
	if ( mn_last < mn_first )
		mn_last = bvh_file_name + strlen( bvh_file_name );
	motion_name.assign( mn_first, mn_last );

	// �t�@�C���̃I�[�v��
	file.open( bvh_file_name, ios::in );
	if ( file.is_open() == 0 )  return; // �t�@�C�����J���Ȃ�������I��

	// �K�w���̓ǂݍ���
	while ( ! file.eof() )
	{
		// �t�@�C���̍Ō�܂ł��Ă��܂�����ُ�I��
		if ( file.eof() )  goto bvh_error;

		// �P�s�ǂݍ��݁A�擪�̒P����擾
		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separater );

		// ��s�̏ꍇ�͎��̍s��
		if ( token == NULL )  continue;

		// �֐߃u���b�N�̊J�n
		if ( strcmp( token, "{" ) == 0 )
		{
			// ���݂̊֐߂��X�^�b�N�ɐς�
			joint_stack.push_back( joint );
			joint = new_joint;
			continue;
		}
		// �֐߃u���b�N�̏I��
		if ( strcmp( token, "}" ) == 0 )
		{
			// ���݂̊֐߂��X�^�b�N������o��
			joint = joint_stack.back();
			joint_stack.pop_back();
			is_site = false;
			continue;
		}

		// �֐ߏ��̊J�n
		if ( ( strcmp( token, "ROOT" ) == 0 ) ||
		     ( strcmp( token, "JOINT" ) == 0 ) )
		{
			// �֐߃f�[�^�̍쐬
			new_joint = new Joint();
			new_joint->index = joints.size();
			new_joint->parent = joint;
			new_joint->has_site = false;
			new_joint->offset[0] = 0.0;  new_joint->offset[1] = 0.0;  new_joint->offset[2] = 0.0;
			new_joint->site[0] = 0.0;  new_joint->site[1] = 0.0;  new_joint->site[2] = 0.0;
			joints.push_back( new_joint );
			if ( joint )
				joint->children.push_back( new_joint );

			// �֐ߖ��̓ǂݍ���
			token = strtok( NULL, "" );
			while ( *token == ' ' )  token ++;
			new_joint->name = token;

			// �C���f�b�N�X�֒ǉ�
			joint_index[ new_joint->name ] = new_joint;
			continue;
		}

		// ���[���̊J�n
		if ( ( strcmp( token, "End" ) == 0 ) )
		{
			new_joint = joint;
			is_site = true;
			continue;
		}

		// �֐߂̃I�t�Z�b�g or ���[�ʒu�̏��
		if ( strcmp( token, "OFFSET" ) == 0 )
		{
			// ���W�l��ǂݍ���
			token = strtok( NULL, separater );
			x = token ? atof( token ) : 0.0;
			token = strtok( NULL, separater );
			y = token ? atof( token ) : 0.0;
			token = strtok( NULL, separater );
			z = token ? atof( token ) : 0.0;

			// �֐߂̃I�t�Z�b�g�ɍ��W�l��ݒ�
			if ( is_site )
			{
				joint->has_site = true;
				joint->site[0] = x;
				joint->site[1] = y;
				joint->site[2] = z;
			}
			else
			// ���[�ʒu�ɍ��W�l��ݒ�
			{
				joint->offset[0] = x;
				joint->offset[1] = y;
				joint->offset[2] = z;
			}
			continue;
		}

		// �֐߂̃`�����l�����
		if ( strcmp( token, "CHANNELS" ) == 0 )
		{
			// �`�����l������ǂݍ���
			token = strtok( NULL, separater );
			joint->channels.resize( token ? atoi( token ) : 0 );

			// �`�����l������ǂݍ���
			for ( i=0; i<joint->channels.size(); i++ )
			{
				// �`�����l���̍쐬
				Channel *  channel = new Channel();
				channel->joint = joint;
				channel->index = channels.size();
				channels.push_back( channel );
				joint->channels[ i ] = channel;

				// �`�����l���̎�ނ̔���
				token = strtok( NULL, separater );
				if ( strcmp( token, "Xrotation" ) == 0 )
					channel->type = X_ROTATION;
				else if ( strcmp( token, "Yrotation" ) == 0 )
					channel->type = Y_ROTATION;
				else if ( strcmp( token, "Zrotation" ) == 0 )
					channel->type = Z_ROTATION;
				else if ( strcmp( token, "Xposition" ) == 0 )
					channel->type = X_POSITION;
				else if ( strcmp( token, "Yposition" ) == 0 )
					channel->type = Y_POSITION;
				else if ( strcmp( token, "Zposition" ) == 0 )
					channel->type = Z_POSITION;
			}
		}

		// Motion�f�[�^�̃Z�N�V�����ֈڂ�
		if ( strcmp( token, "MOTION" ) == 0 )
			break;
	}


	// ���[�V�������̓ǂݍ���
	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, separater );
	if ( strcmp( token, "Frames" ) != 0 )  goto bvh_error;
	token = strtok( NULL, separater );
	if ( token == NULL )  goto bvh_error;
	num_frame = atoi( token );

	file.getline( line, BUFFER_LENGTH );
	token = strtok( line, ":" );
	if ( strcmp( token, "Frame Time" ) != 0 )  goto bvh_error;
	token = strtok( NULL, separater );
	if ( token == NULL )  goto bvh_error;
	interval = atof( token );

	num_channel = channels.size();
	motion = new double[ num_frame * num_channel ];

	// ���[�V�����f�[�^�̓ǂݍ���
	for ( i=0; i<num_frame; i++ )
	{
		file.getline( line, BUFFER_LENGTH );
		token = strtok( line, separater );
		for ( j=0; j<num_channel; j++ )
		{
			if ( token == NULL )
				goto bvh_error;
			motion[ i*num_channel + j ] = atof( token );
			token = strtok( NULL, separater );
		}
	}

	// �t�@�C���̃N���[�Y
	file.close();

	// ���[�h�̐���
	is_load_success = true;

	return;

bvh_error:
	file.close();
}



//
//  BVH���i�E�p���̕`��֐�
//

#include <math.h>
#include <GL/glut.h>


// �w��t���[���̎p����`��
void  BVH::RenderFigure( int frame_no, float scale )
{
	// BVH���i�E�p�����w�肵�ĕ`��
	RenderFigure( joints[ 0 ], motion + frame_no * num_channel, scale );
}


// �w�肳�ꂽBVH���i�E�p����`��i�N���X�֐��j
void  BVH::RenderFigure( const Joint * joint, const double * data, float scale )
{
	glPushMatrix();

	// ���[�g�֐߂̏ꍇ�͕��s�ړ���K�p
	if ( joint->parent == NULL )
	{
		glTranslatef( data[ 0 ] * scale, data[ 1 ] * scale, data[ 2 ] * scale );
	}
	// �q�֐߂̏ꍇ�͐e�֐߂���̕��s�ړ���K�p
	else
	{
		glTranslatef( joint->offset[ 0 ] * scale, joint->offset[ 1 ] * scale, joint->offset[ 2 ] * scale );
	}

	// �e�֐߂���̉�]��K�p�i���[�g�֐߂̏ꍇ�̓��[���h���W����̉�]�j
	int  i, j;
	for ( i=0; i<joint->channels.size(); i++ )
	{
		Channel *  channel = joint->channels[ i ];
		if ( channel->type == X_ROTATION )
			glRotatef( data[ channel->index ], 1.0f, 0.0f, 0.0f );
		else if ( channel->type == Y_ROTATION )
			glRotatef( data[ channel->index ], 0.0f, 1.0f, 0.0f );
		else if ( channel->type == Z_ROTATION )
			glRotatef( data[ channel->index ], 0.0f, 0.0f, 1.0f );
	}

	// �����N��`��
	// �֐ߍ��W�n�̌��_���疖�[�_�ւ̃����N��`��
	if ( joint->children.size() == 0 )
	{
		RenderBone( 0.0f, 0.0f, 0.0f, joint->site[ 0 ] * scale, joint->site[ 1 ] * scale, joint->site[ 2 ] * scale );
	}
	// �֐ߍ��W�n�̌��_���玟�̊֐߂ւ̐ڑ��ʒu�ւ̃����N��`��
	if ( joint->children.size() == 1 )
	{
		Joint *  child = joint->children[ 0 ];
		RenderBone( 0.0f, 0.0f, 0.0f, child->offset[ 0 ] * scale, child->offset[ 1 ] * scale, child->offset[ 2 ] * scale );
	}
	// �S�֐߂ւ̐ڑ��ʒu�ւ̒��S�_����e�֐߂ւ̐ڑ��ʒu�։~����`��
	if ( joint->children.size() > 1 )
	{
		// ���_�ƑS�֐߂ւ̐ڑ��ʒu�ւ̒��S�_���v�Z
		float  center[ 3 ] = { 0.0f, 0.0f, 0.0f };
		for ( i=0; i<joint->children.size(); i++ )
		{
			Joint *  child = joint->children[ i ];
			center[ 0 ] += child->offset[ 0 ];
			center[ 1 ] += child->offset[ 1 ];
			center[ 2 ] += child->offset[ 2 ];
		}
		center[ 0 ] /= joint->children.size() + 1;
		center[ 1 ] /= joint->children.size() + 1;
		center[ 2 ] /= joint->children.size() + 1;

		// ���_���璆�S�_�ւ̃����N��`��
		RenderBone(	0.0f, 0.0f, 0.0f, center[ 0 ] * scale, center[ 1 ] * scale, center[ 2 ] * scale );

		// ���S�_���玟�̊֐߂ւ̐ڑ��ʒu�ւ̃����N��`��
		for ( i=0; i<joint->children.size(); i++ )
		{
			Joint *  child = joint->children[ i ];
			RenderBone(	center[ 0 ] * scale, center[ 1 ] * scale, center[ 2 ] * scale,
				child->offset[ 0 ] * scale, child->offset[ 1 ] * scale, child->offset[ 2 ] * scale );
		}
	}

	// �q�֐߂ɑ΂��čċA�Ăяo��
	for ( i=0; i<joint->children.size(); i++ )
	{
		RenderFigure( joint->children[ i ], data, scale );
	}

	glPopMatrix();
}


// BVH���i�̂P�{�̃����N��`��i�N���X�֐��j
void  BVH::RenderBone( float x0, float y0, float z0, float x1, float y1, float z1 )
{
	// �^����ꂽ�Q�_�����ԉ~����`��

	// �~���̂Q�[�_�̏������_�E�����E�����̏��ɕϊ�
	GLdouble  dir_x = x1 - x0;
	GLdouble  dir_y = y1 - y0;
	GLdouble  dir_z = z1 - z0;
	GLdouble  bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );

	// �`��p�����^�̐ݒ�
	static GLUquadricObj *  quad_obj = NULL;
	if ( quad_obj == NULL )
		quad_obj = gluNewQuadric();
	gluQuadricDrawStyle( quad_obj, GLU_FILL );
	gluQuadricNormals( quad_obj, GLU_SMOOTH );

	glPushMatrix();

	// ���s�ړ���ݒ�
	glTranslated( x0, y0, z0 );

	// �ȉ��A�~���̉�]��\���s����v�Z

	// ������P�ʃx�N�g���ɐ��K��
	double  length;
	length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z );
	if ( length < 0.0001 ) {
		dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;
	}
	dir_x /= length;  dir_y /= length;  dir_z /= length;

	// ��Ƃ��邙���̌�����ݒ�
	GLdouble  up_x, up_y, up_z;
	up_x = 0.0;
	up_y = 1.0;
	up_z = 0.0;

	// �����Ƃ����̊O�ς��炘���̌������v�Z
	double  side_x, side_y, side_z;
	side_x = up_y * dir_z - up_z * dir_y;
	side_y = up_z * dir_x - up_x * dir_z;
	side_z = up_x * dir_y - up_y * dir_x;

	// ������P�ʃx�N�g���ɐ��K��
	length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z );
	if ( length < 0.0001 ) {
		side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;
	}
	side_x /= length;  side_y /= length;  side_z /= length;

	// �����Ƃ����̊O�ς��炙���̌������v�Z
	up_x = dir_y * side_z - dir_z * side_y;
	up_y = dir_z * side_x - dir_x * side_z;
	up_z = dir_x * side_y - dir_y * side_x;

	// ��]�s���ݒ�
	GLdouble  m[16] = { side_x, side_y, side_z, 0.0,
	                    up_x,   up_y,   up_z,   0.0,
	                    dir_x,  dir_y,  dir_z,  0.0,
	                    0.0,    0.0,    0.0,    1.0 };
	glMultMatrixd( m );

	// �~���̐ݒ�
	GLdouble radius= 0.01; // �~���̑���
	GLdouble slices = 8.0; // �~���̕��ˏ�̍ו����i�f�t�H���g12�j
	GLdouble stack = 3.0;  // �~���̗֐؂�̍ו����i�f�t�H���g�P�j

	// �~����`��
	gluCylinder( quad_obj, radius, radius, bone_length, slices, stack );

	glPopMatrix();
}



// End of BVH.cpp
