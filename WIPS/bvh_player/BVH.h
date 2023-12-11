/**
***  BVH����t�@�C���̓ǂݍ��݁E�`��N���X
***  Copyright (c) 2004-2007, Masaki OSHITA (www.oshita-lab.org)
**/


#ifndef  _BVH_H_
#define  _BVH_H_


#include <vector>
#include <map>
#include <string>

using namespace  std;



//
//  BVH�`���̃��[�V�����f�[�^
//
class  BVH
{
  public:
	/*  �����p�\����  */

	// �`�����l���̎��
	enum  ChannelEnum
	{
		X_ROTATION, Y_ROTATION, Z_ROTATION,
		X_POSITION, Y_POSITION, Z_POSITION
	};
	struct  Joint;

	// �`�����l�����
	struct  Channel
	{
		// �Ή��֐�
		Joint *              joint;
		
		// �`�����l���̎��
		ChannelEnum          type;

		// �`�����l���ԍ�
		int                  index;
	};

	// �֐ߏ��
	struct  Joint
	{
		// �֐ߖ�
		string               name;
		// �֐ߔԍ�
		int                  index;

		// �֐ߊK�w�i�e�֐߁j
		Joint *              parent;
		// �֐ߊK�w�i�q�֐߁j
		vector< Joint * >    children;

		// �ڑ��ʒu
		double               offset[3];

		// ���[�ʒu���������ǂ����̃t���O
		bool                 has_site;
		// ���[�ʒu
		double               site[3];

		// ��]��
		vector< Channel * >  channels;
	};


  private:
	// ���[�h�������������ǂ����̃t���O
	bool                     is_load_success;

	/*  �t�@�C���̏��  */
	string                   file_name;   // �t�@�C����
	string                   motion_name; // ���얼

	/*  �K�w�\���̏��  */
	int                      num_channel; // �`�����l����
	vector< Channel * >      channels;    // �`�����l����� [�`�����l���ԍ�]
	vector< Joint * >        joints;      // �֐ߏ�� [�p�[�c�ԍ�]
	map< string, Joint * >   joint_index; // �֐ߖ�����֐ߏ��ւ̃C���f�b�N�X

	/*  ���[�V�����f�[�^�̏��  */
	int                      num_frame;   // �t���[����
	double                   interval;    // �t���[���Ԃ̎��ԊԊu
	double *                 motion;      // [�t���[���ԍ�][�`�����l���ԍ�]


  public:
	// �R���X�g���N�^�E�f�X�g���N�^
	BVH();
	BVH( const char * bvh_file_name );
	~BVH();

	// �S���̃N���A
	void  Clear();

	// BVH�t�@�C���̃��[�h
	void  Load( const char * bvh_file_name );

  public:
	/*  �f�[�^�A�N�Z�X�֐�  */

	// ���[�h�������������ǂ������擾
	bool  IsLoadSuccess() const { return is_load_success; }

	// �t�@�C���̏��̎擾
	const string &  GetFileName() const { return file_name; }
	const string &  GetMotionName() const { return motion_name; }

	// �K�w�\���̏��̎擾
	const int       GetNumJoint() const { return  joints.size(); }
	const Joint *   GetJoint( int no ) const { return  joints[no]; }
	const int       GetNumChannel() const { return  channels.size(); }
	const Channel * GetChannel( int no ) const { return  channels[no]; }

	const Joint *   GetJoint( const string & j ) const  {
		map< string, Joint * >::const_iterator  i = joint_index.find( j );
		return  ( i != joint_index.end() ) ? (*i).second : NULL; }
	const Joint *   GetJoint( const char * j ) const  {
		map< string, Joint * >::const_iterator  i = joint_index.find( j );
		return  ( i != joint_index.end() ) ? (*i).second : NULL; }

	// ���[�V�����f�[�^�̏��̎擾
	int     GetNumFrame() const { return  num_frame; }
	double  GetInterval() const { return  interval; }
	double  GetMotion( int f, int c ) const { return  motion[ f*num_channel + c ]; }

	// ���[�V�����f�[�^�̏��̕ύX
	void  SetMotion( int f, int c, double v ) { motion[ f*num_channel + c ] = v; }

  public:
	/*  �p���̕`��֐�  */
	
	// �w��t���[���̎p����`��
	void  RenderFigure( int frame_no, float scale = 1.0f );

	// �w�肳�ꂽBVH���i�E�p����`��i�N���X�֐��j
	static void  RenderFigure( const Joint * root, const double * data, float scale = 1.0f );

	// BVH���i�̂P�{�̃����N��`��i�N���X�֐��j
	static void  RenderBone( float x0, float y0, float z0, float x1, float y1, float z1 );
};



#endif // _BVH_H_
