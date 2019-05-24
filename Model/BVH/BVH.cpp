
#include <fstream>
#include <string.h>

#include "bvh.h"

BVH::BVH() {
	Clear();
}

BVH::BVH(const char * bvh_file_name) {
	Clear();
	Load(bvh_file_name);
}

BVH::~BVH() {
	Clear();
}

void  BVH::Clear() {
//	for (int i = 0; i < m_Joints.size(); i++)
	//	delete  m_Joints[i];
	for (unsigned int i = 0; i < m_Frames.size(); i++)
		delete m_Frames[i];

	m_LoadSuccess = false;

	//m_Joints.clear();
	m_Frames.clear();
	m_pRootJoint = NULL;
}

void  BVH::Load(const char * bvh_file_name) {
#define  BUFFER_LENGTH  1024*32

	ifstream  file;
	char      line[BUFFER_LENGTH];
	char *    token;
	char      separater[] = " :,\t";
	vector<Joint*>   jointStack;
	Joint*   pJoint = NULL;
	Joint *   new_joint = NULL;
	float    x, y, z;
	int channelCount = 0;
	Clear();

	//	m_FileName = bvh_file_name;

	file.open(bvh_file_name, ios::in);
	if (file.is_open() == 0)  return;

	while (!file.eof()) {
		if (file.eof()){
            file.close();
            return;
		}
			//goto bvh_error;

		file.getline(line, BUFFER_LENGTH);
		token = strtok(line, separater);

		if (token == NULL)  continue;

		if (strcmp(token, "{") == 0) {
			jointStack.push_back(pJoint);
			pJoint = new_joint;
			continue;
		}
		if (strcmp(token, "}") == 0) {
			pJoint = jointStack.back();
			jointStack.pop_back();
			continue;
		}

		if (strcmp(token, "ROOT") == 0
			|| strcmp(token, "JOINT") == 0
			 || strcmp(token, "End") == 0) {
			new_joint = new Joint();
			new_joint->m_Offset[0] = 0.0;  new_joint->m_Offset[1] = 0.0;  new_joint->m_Offset[2] = 0.0;
			new_joint->m_ChanelIndex = channelCount;
			//m_Joints.push_back(new_joint);
			if (strcmp(token, "ROOT") == 0) {
				m_pRootJoint = new_joint;
			}
			if (pJoint)
				pJoint->m_Childrens.push_back(new_joint);

			token = strtok(NULL, "");
			while (*token == ' ')  token++;
			new_joint->m_Name = token;
			continue;
		}

		if (strcmp(token, "OFFSET") == 0) {
			token = strtok(NULL, separater);
			x = token ? float(atof(token)) : 0.0f;
			token = strtok(NULL, separater);
			y = token ? float(atof(token)) : 0.0f;
			token = strtok(NULL, separater);
			z = token ? float(atof(token)) : 0.0f;

			pJoint->m_Offset[0] = x;
			pJoint->m_Offset[1] = y;
			pJoint->m_Offset[2] = z;
			continue;
		}

		if (strcmp(token, "CHANNELS") == 0) {
			token = strtok(NULL, separater);
			int jointChannelCount = token ? atoi(token) : 0;

			for (int i = 0; i<jointChannelCount; i++) {
				token = strtok(NULL, separater);
				if (strcmp(token, "Xrotation") == 0) {
					pJoint->m_Channels.push_back(XRotation);
				} else if (strcmp(token, "Yrotation") == 0) {
					pJoint->m_Channels.push_back(YRotation);
				} else if (strcmp(token, "Zrotation") == 0) {
					pJoint->m_Channels.push_back(ZRotation);
				} else if (strcmp(token, "Xposition") == 0) {
					pJoint->m_Channels.push_back(XPosition);
				} else if (strcmp(token, "Yposition") == 0) {
					pJoint->m_Channels.push_back(YPosition);
				} else if (strcmp(token, "Zposition") == 0) {
					pJoint->m_Channels.push_back(ZPosition);
				}
			}
			channelCount += jointChannelCount;
		}
		if (strcmp(token, "MOTION") == 0)
			break;
	}

	file.getline(line, BUFFER_LENGTH);
	token = strtok(line, separater);
	if (strcmp(token, "Frames") != 0){
        file.close();
        return;
	}
	token = strtok(NULL, separater);
	if (token == NULL){
                file.close();
        return;

	}
	int numFrame = atoi(token);

	file.getline(line, BUFFER_LENGTH);
	token = strtok(line, ":");
	if (strcmp(token, "Frame Time") != 0){
        file.close();
        return;
	}
	token = strtok(NULL, separater);
	if (token == NULL){
       file.close();
        return;

	}
	m_Interval = float(atof(token));

	for (int i = 0; i<numFrame; i++) {
		file.getline(line, BUFFER_LENGTH);
		token = strtok(line, separater);
		float* frame = new float[channelCount];
		for (int j = 0; j < channelCount; j++) {
			if (token == NULL){
                file.close();
                return;
			}

			frame[j] = float(atof(token));
			token = strtok(NULL, separater);
		}
		m_Frames.push_back(frame);
	}
	file.close();
	m_LoadSuccess = true;
}


/*void  BVH::RenderFigure(int frameNo, float scale) {
	RenderFigure(m_pRootJoint, m_Frames[frameNo], scale);
}

void  BVH::RenderFigure(const Joint* joint, const float * data, float scale) {
	glPushMatrix();
	float translate[3] = { 0.0f, 0.0f, 0.0f };
	float rotate[3] = { 0.0f, 0.0f, 0.0f };

	for (unsigned int i = 0; i < joint->m_Channels.size(); i++) {
		float value = data[joint->m_ChanelIndex + i];
		Channel channel = joint->m_Channels[i];
		switch (channel) {
		case XPosition:
			translate[0] = value;
			break;
		case YPosition:
			translate[1] = value;
			break;
		case ZPosition:
			translate[2] = value;
			break;
		}
	}
	glTranslatef((translate[0] + joint->m_Offset[0]) * scale, (translate[1] + joint->m_Offset[1]) * scale, (translate[2] + joint->m_Offset[2]) * scale);
	for (unsigned int i = 0; i < joint->m_Channels.size(); i++) {
		float value = data[joint->m_ChanelIndex + i];
		Channel channel = joint->m_Channels[i];
		switch (channel) {
		case XRotation:
			glRotatef(value, 1.0f, 0.0f, 0.0f);
			break;
		case YRotation:
			glRotatef(value, 0.0f, 1.0f, 0.0f);
			break;
		case ZRotation:
			glRotatef(value, 0.0f, 0.0f, 1.0f);
			break;
		}
	}

	for (unsigned int i = 0; i<joint->m_Childrens.size(); i++) {
		Joint *  child = joint->m_Childrens[i];
		RenderBone(child->m_Offset[0] * scale, child->m_Offset[1] * scale, child->m_Offset[2] * scale);
	}

	for (unsigned int i = 0; i<joint->m_Childrens.size(); i++) {
		Joint *  child = joint->m_Childrens[i];
		if(child->m_Childrens.size() != 0)
			RenderFigure(child, data, scale);
	}
	glPopMatrix();
}

void  BVH::RenderBone(float x1, float y1, float z1) {
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(x1, y1, z1);
	glEnd();
}
*/
