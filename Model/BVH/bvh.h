
#ifndef  _BVH_H_
#define  _BVH_H_


#include <vector>
#include <map>
#include <string>

using namespace std;

enum  Channel {
	XRotation, YRotation, ZRotation,
	XPosition, YPosition, ZPosition
};

struct  Joint {
	std::string               m_Name;
	int                  m_ChanelIndex;
	float               m_Offset[3];
	vector< Joint * >    m_Childrens;
	vector< Channel>  m_Channels;
};

using namespace  std;
class  BVH {
public:
	bool m_LoadSuccess;
	Joint* m_pRootJoint;
	vector<float*> m_Frames;
	float m_Interval;

	// Redundant vector of all joints.
	//vector<Joint*> m_Joints;
public:
	BVH();
	BVH(const char * bvh_file_name);
	~BVH();

	void  Clear();

	void  Load(const char * bvh_file_name);

public:
	bool  IsLoadSuccess() const {
		return m_LoadSuccess;
	}

	int     GetNumFrame() const { return  m_Frames.size(); }
	float  GetInterval() const { return  m_Interval; }

	//void  RenderFigure(int frame_no, float scale = 1.0f);
	//static void  RenderFigure(const Joint * root, const float * data, float scale = 1.0f);
	//static void  RenderBone(float x1, float y1, float z1);
};

#endif // _BVH_H_
