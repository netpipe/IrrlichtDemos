#include <iostream>
#include <irrlicht/irrlicht.h>
//#include "CSkinnedMesh.h"
#include "an8parser.h"
#include <cmath>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void setAN8Track(an8::an8_element mesh_element)
{
    std::cout << "\n---TRACKS---\n";
    for(int n = 0; n < mesh_element.controller.size(); n++)
    {
        an8::an8_controller controller = mesh_element.controller[n];
        std::cout << "c[" << n << "] = " << controller.track.pointkey.size() << std::endl;
        if(controller.track.track_type == TRACK_TYPE_POINT)
        {
            for(int pkey_i = 0; pkey_i < controller.track.pointkey.size(); pkey_i++)
            {
                an8::an8_pointkey pkey = controller.track.pointkey[pkey_i];
                std::cout << pkey.frame_index << ": " << pkey.value.x << ", " << pkey.value.y << ", " << pkey.value.z << std::endl;
            }
        }
    }
}

bool getAN8Figure(an8::an8_project* p, std::string figure_name, an8::an8_figure* figure)
{
    for(int i = 0; i < p->figures.size(); i++)
    {
        if(p->figures[i].name.compare(figure_name)==0)
        {
            *figure = p->figures[i];
            return true;
        }
    }
    return false;
}

bool getAN8Object(an8::an8_project* p, std::string obj_name, an8::an8_object* obj)
{
    for(int i = 0; i < p->objects.size(); i++)
    {
        if(p->objects[i].name.compare(obj_name)==0)
        {
            *obj = p->objects[i];
            return true;
        }
    }
    return false;
}

//get all the mesh components of an object
void getAN8MeshList(an8::an8_project* p, vector<an8::an8_component>* componentList, std::vector<an8::an8_mesh>* meshList, an8::an8_irr_joint_data* joint_data)
{
    for(int c_index = 0; c_index < componentList->size(); c_index++)
    {
        an8::an8_component c = (*componentList)[c_index];

        if(c.type == AN8_COMPONENT_TYPE_MESH)
        {
            int current_namedobject_index = joint_data->namedobject.size()-1;
            if(current_namedobject_index >= 0)
                joint_data->namedobject[current_namedobject_index].meshList_index.push_back(meshList->size()); //store the index being pushed
            else
                std::cout << "No named object to add mesh index to" << std::endl;

            an8::an8_mesh mesh = c.mesh;
            meshList->push_back(mesh);
        }
        else if(c.type == AN8_COMPONENT_TYPE_NAMEDOBJECT)
        {
            joint_data->namedobject.push_back(c.named_object);

            an8::an8_object obj;
            getAN8Object(p, c.named_object.name, &obj);
            getAN8MeshList(p, &obj.component, meshList, joint_data);
        }
    }
}


irr::core::matrix4 rotateAxesXYZToEuler(const irr::core::vector3df& oldRotation, const irr::core::vector3df& rotationAngles, bool useLocalAxes)
{
    irr::core::matrix4 transformation;
    transformation.setRotationDegrees(oldRotation);
    irr::core::vector3df axisX(1,0,0), axisY(0,1,0), axisZ(0,0,1);
    irr::core::matrix4 matRotX, matRotY, matRotZ;

    if ( useLocalAxes )
    {
        transformation.rotateVect(axisX);
        transformation.rotateVect(axisY);
        transformation.rotateVect(axisZ);
    }

    matRotX.setRotationAxisRadians(rotationAngles.X*irr::core::DEGTORAD, axisX);
    matRotY.setRotationAxisRadians(rotationAngles.Y*irr::core::DEGTORAD, axisY);
    matRotZ.setRotationAxisRadians(rotationAngles.Z*irr::core::DEGTORAD, axisZ);

    irr::core::matrix4 newTransform = matRotZ * matRotY * matRotX * transformation;
    return newTransform;
}


double rc_Interpolate(double min_a, double max_a, double mid_a, double min_b, double max_b)
{
	return ( (mid_a-min_a)/(max_a-min_a)) * (max_b-min_b) + min_b;
}

double rc_Abs(double n)
{
    if(n < 0)
        return (n*-1);
    return n;
}


irr::f32 an8_calculate_figure_transform(video::S3DVertex* vertex, an8::an8_irr_joint_data joint_data, int debug_vertex_count)
{
    an8::an8_namedobject* namedobject = &joint_data.namedobject[joint_data.namedobject_index];

    //previous node position is px, py, pz
	//px = parent_x : py = parent_y : pz = parent_z
	scene::ISkinnedMesh::SJoint* pjoint = (scene::ISkinnedMesh::SJoint*)joint_data.joint;

	if(!pjoint)
    {
        std::cout << "NO PJOINT" << std::endl;
        return 0;
    }

    if(vertex->Pos.X <= -36)
        debug_vertex_count = 11;
    else
        debug_vertex_count = 0;

    if(debug_vertex_count == 11)
    {
        if(joint_data.bone.name.compare("bone03")!=0)
            debug_vertex_count = 0;
    }

	core::vector3df v_out= vertex->Pos;

	core::matrix4 transform_matrix = pjoint->GlobalMatrix;

	core::vector3df translate_vector = transform_matrix.getTranslation();

	//-----------namedobject transformations-----------------------------
	core::vector3df namedobject_translate_vector;

	namedobject_translate_vector.X = namedobject->base.origin.x;
	namedobject_translate_vector.Y = namedobject->base.origin.y;
	namedobject_translate_vector.Z = namedobject->base.origin.z;

	core::quaternion q(namedobject->base.orientation.x,
                       namedobject->base.orientation.y,
                       namedobject->base.orientation.z,
                       namedobject->base.orientation.w);
    core::vector3df namedobject_rotate_vector;
    q.toEuler(namedobject_rotate_vector);

    core::matrix4 namedobject_matrix;
    namedobject_matrix.setTranslation(namedobject_translate_vector);
    namedobject_matrix *= q.getMatrix();
    //--------------------------------------------------------------------


	transform_matrix.setTranslation(translate_vector);

	transform_matrix *= namedobject_matrix;

	transform_matrix.transformVect(vertex->Pos); //translate and rotate Z



	if(namedobject->name_block.name.compare("namedobject01")==0 && debug_vertex_count == 11)
	{
	    std::cout << std::endl;
	    std::cout << "Tranform: " << transform_matrix.getTranslation().X << ", "
                                  << transform_matrix.getTranslation().Y << ", "
                                  << transform_matrix.getTranslation().Z << ", " << std::endl;

        std::cout << "T_ROT: "    << transform_matrix.getRotationDegrees().X << ", "
                                  << transform_matrix.getRotationDegrees().Y << ", "
                                  << transform_matrix.getRotationDegrees().Z << ", " << std::endl;

        std::cout << "Vertex: " << vertex->Pos.X << ", " << vertex->Pos.Y << ", " << vertex->Pos.Z
                  << " To Local -> "
                  << v_out.X << ", " << v_out.Y << ", " << v_out.Z << std::endl;
	}


	//calculate weights


	//transform the vertex to its namedobject orientation
	// NOTE: In this orientation, the joint is pointing straight up so its much easier to check where the vertex is in the influence area
	core::matrix4 jm;
	//joint_data.joint->GlobalMatrix.getInverse(jm);
	//joint_data.joint->LocalMatrix.getInverse(jm);
	transform_matrix.getInverse(jm);

	jm *= namedobject_matrix;

	//namedobject_matrix.transformVect(v_out);
	jm.transformVect(v_out);

	v_out = v_out - joint_data.joint->LocalMatrix.getTranslation();

	if(debug_vertex_count == 11)
	std::cout << "Local Matrix = " << v_out.X << ", "
                                   << v_out.Y << ", "
                                   << v_out.Z << "  ----  " << joint_data.joint->LocalMatrix.getTranslation().Y
                                   << std::endl << std::endl;


    irr::f32 center0 = joint_data.bone.influence.center0;
	irr::f32 inRadius0 = joint_data.bone.influence.inRadius0;
	irr::f32 outRadius0 = joint_data.bone.influence.outRadius0;

	irr::f32 center1 = joint_data.bone.influence.center1;
	irr::f32 inRadius1 = joint_data.bone.influence.inRadius1;
	irr::f32 outRadius1 = joint_data.bone.influence.outRadius1;


	core::vector3df tvec(0,joint_data.bone.length,0);

	core::vector3df c0_vec(0, tvec.Y * center0, 0);
	core::vector3df c1_vec(0, tvec.Y * center1, 0);

	double dist_from_center = 0;

	double weight = 0;

	if( v_out.Y < c0_vec.Y ) // Then 'Point is below bottom part of cone area
    {
        dist_from_center = v_out.getDistanceFrom( c0_vec );

		if(debug_vertex_count == 11)
		{
		    std::cout << "dist_from_center = " << dist_from_center << std::endl;

            std::cout << "data: " << inRadius0 << ", " << outRadius0 << ", " << dist_from_center << std::endl;
            std::cout << "interpolate: " << 0 << ", " << rc_Abs(inRadius0-outRadius0) << ", " << rc_Abs(inRadius0-dist_from_center) << ", " << 0 << ", " << 1 << std::endl;
		}

		if(dist_from_center > outRadius0)
        {
            if(debug_vertex_count == 11)
                std::cout << "out radius = " << outRadius0 << std::endl;
			weight = 0;
        }
		else if(dist_from_center <= inRadius0)
        {
            weight = 1;
        }
        else
		{
		    weight = rc_Interpolate( inRadius0, outRadius0, dist_from_center, 1, 0);
		}
    }
	else if( v_out.Y > c1_vec.Y ) // Then 'Point is above top part of cone area
    {
        dist_from_center = v_out.getDistanceFrom( c1_vec );

        if(debug_vertex_count == 11)
		{
		    std::cout << "dist_from_center = " << dist_from_center << std::endl;

            std::cout << "Calc stuff: " << tvec.X << ", " << tvec.Y << ", " << tvec.Z << " --- " << center0 << ", " << center1 << std::endl;
            std::cout << "CVEC = " << c1_vec.X << ", " << c1_vec.Y << ", " << c1_vec.Z << std::endl;
            std::cout << "data: " << inRadius1 << ", " << outRadius1 << ", " << dist_from_center << std::endl;
            std::cout << "interpolate: " << 0 << ", " << rc_Abs(inRadius1-outRadius1) << ", " << rc_Abs(inRadius1-dist_from_center) << ", " << 0 << ", " << 1 << std::endl;
		}

		if(dist_from_center > outRadius1) // Then
        {
            if(debug_vertex_count == 11)
                std::cout << "out radius1 = " << outRadius1 << std::endl;
            weight = 0;
        }
		else if(dist_from_center <= inRadius1) // Then
        {
            weight = 1;
        }
        else
		{
		    weight = rc_Interpolate( inRadius1, outRadius1, dist_from_center, 1, 0);
		}
    }
	else
    {
		dist_from_center = v_out.getDistanceFrom(core::vector3df(0,v_out.Y,0));

		double inRadius2 = rc_Interpolate( c0_vec.Y, c1_vec.Y, v_out.Y, inRadius0, inRadius1);
		double outRadius2 = rc_Interpolate( c0_vec.Y, c1_vec.Y, v_out.Y, outRadius0, outRadius1);

		if(debug_vertex_count == 11)
		{
		    std::cout << "--dist_from_center = " << dist_from_center << std::endl;
            std::cout << "radius: " << inRadius0 << ", " << outRadius0 << ", " << inRadius1 << ", " << outRadius1 << std::endl;
            std::cout << std::endl;

            std::cout << "data: " << inRadius2 << ", " << outRadius2 << ", " << dist_from_center << std::endl;
            std::cout << "interpolate: " << 0 << ", " << rc_Abs(inRadius2-outRadius2) << ", " << rc_Abs(inRadius2-dist_from_center) << ", " << 0 << ", " << 1 << std::endl;
		}

		if(dist_from_center > outRadius2) // Then
        {
            if(debug_vertex_count == 11)
                std::cout << "out radius = " << outRadius2 << std::endl;
			weight = 0;
        }
		else if(dist_from_center <= inRadius2) // Then
        {
            weight = 1;
        }
        else
        {
            weight = rc_Interpolate( inRadius2, outRadius2, dist_from_center, 1, 0);
        }
    }

    if(debug_vertex_count == 11)
        std::cout << "Vertex Weight = " << weight << std::endl << std::endl;

    //if(joint_data.namedobject[joint_data.namedobject_index].base_bone.compare(joint_data.bone.name)==0)
      //  weight = 0.001;

	return weight;
}

std::string rc_replace(std::string src, std::string tgt, std::string rpc)
{
    if(tgt.length()==0)
        return src;
    size_t found_inc = rpc.length() > 0 ? rpc.length() : 1;
    size_t found = 0;
    found = src.find(tgt);
    while( found != std::string::npos && found < src.length())
    {
        src = src.substr(0,found) + rpc + src.substr(found + tgt.length());
        found = src.find(tgt,found+found_inc);
    }
    return src;
}

an8::an8_irr_joint_data* getJointData(an8::an8_project* p, int scene_index, int element_index, int controller_index, std::string* controller_axis)
{
    if(!p)
        return NULL;

    if(scene_index >= p->scenes.size())
        return NULL;

    an8::an8_scene* a_scene = &p->scenes[scene_index];

    if(element_index >= a_scene->elements.size())
        return NULL;

    an8::an8_element* a_element = &a_scene->elements[element_index];

    if(controller_index >= a_element->controller.size())
        return NULL;

    an8::an8_controller* a_controller = &a_element->controller[controller_index];
    an8::an8_track a_track = a_controller->track;

    std::string bone_name = rc_replace(a_controller->name, "joint.", "");

    if(bone_name.length() > 2)
    {
        controller_axis[0] = bone_name.substr(bone_name.length()-1,1);
        bone_name = bone_name.substr(0, bone_name.length()-2);
    }
    else
        return NULL;

    an8::an8_figure* a_figure = NULL;
    an8::an8_bone* bone = NULL;


    for(int figure_index = 0; figure_index < p->figures.size(); figure_index++)
    {
        a_figure = &p->figures[figure_index];

        for(int joint_data_index = 0; joint_data_index < p->irr_joint_data.size(); joint_data_index++)
        {
            an8::an8_irr_joint_data* joint_data = &p->irr_joint_data[joint_data_index];

            if(joint_data)
            {
                if(joint_data->bone.name.compare(bone_name)==0)
                {
                    return joint_data;
                }
            }

            std::cout << "Joint Data: " << joint_data->bone.name << " --- " << bone_name << std::endl;
        }
    }
    return NULL;
}


struct an8_rotation_key
{
    int32_t frame;
    std::string axis;
    double angle;
};

int an8_addKeys(an8::an8_project* p, scene::ISkinnedMesh* AnimatedMesh, int scene_index, int element_index)
{
    an8::an8_element* element = &p->scenes[scene_index].elements[element_index];

    if(element->element_type != ELEMENT_TYPE_FIGURE)
        return 0;

    an8::an8_irr_joint_data* joint_data = NULL;
    an8::an8_pointkey a_pkey;
    an8::an8_floatkey a_fkey;

    //return 0;


    std::vector< an8_rotation_key > bone_key;


    for(int controller_index = 0; controller_index < element->controller.size(); controller_index++ )
    {
        //std::string obj_name = element->obj_name;

        std::cout << "get controller and track" << std::endl;

        an8::an8_controller* controller = &element->controller[controller_index];
        an8::an8_track* track = &controller->track;

        if(!track)
            std::cout << "NO TRACK" << std::endl;
        else
            std::cout << "track loaded: " << controller->name << std::endl;

        if(track->track_type == TRACK_TYPE_FLOAT && controller->name.substr(0,5).compare("joint")==0)
        {
            //joint_data = NULL;

            std::string c_axis = "N";
            joint_data = getJointData(p, scene_index, element_index, controller_index, &c_axis);

            if(joint_data != NULL)
            {
                std::cout << "Joint data loaded" << std::endl;
                std::cout << "key on bone: " << joint_data->bone.name << std::endl << std::endl;
            }
            else
                continue;

            core::matrix4 rm = joint_data->joint->LocalMatrix;
            core::vector3df v;
            //rm.transformVect(v, core::vector3df(0,0,0));
            v = rm.getRotationDegrees();

            std::cout << "Joint Translate = " << v.X << ", " << v.Y << ", " << v.Z << std::endl;



            for(int key_index = 0; key_index < track->floatkey.size(); key_index++)
            {
                a_fkey = track->floatkey[key_index];
                irr::scene::ISkinnedMesh::SRotationKey* rkey = AnimatedMesh->addRotationKey(joint_data->joint);

                an8_rotation_key key;
                key.frame = a_fkey.frame_index;

                rkey->frame = (irr::f32)a_fkey.frame_index;
                if(c_axis.compare("X")==0)
                {
                    rkey->rotation.fromAngleAxis( core::degToRad(a_fkey.value), core::vector3df(1,0,0) );

                    key.axis = "X";
                    key.angle = a_fkey.value;
                }
                else if(c_axis.compare("Y")==0)
                {
                    rkey->rotation.fromAngleAxis( core::degToRad(a_fkey.value), core::vector3df(0,1,0) );

                    key.axis = "Y";
                    key.angle = a_fkey.value;
                }
                else if(c_axis.compare("Z")==0)
                {
                    rkey->rotation.fromAngleAxis( -1*core::degToRad(a_fkey.value), core::vector3df(0,0,1));

                    key.axis = "Z";
                    key.angle = a_fkey.value;
                }


                core::matrix4 m = rkey->rotation.getMatrix();
                std::cout << "rot key[" << joint_data->joint->RotationKeys.size()-1 << "] = frame(" << rkey->frame << ") rotation = " << m.getRotationDegrees().X << ", "
                                                                                                   << m.getRotationDegrees().Y << ", "
                                                                                                   << m.getRotationDegrees().Z << ", "
                                                                                                   << std::endl;
            }

            if(joint_data->bone.name.compare("bone02")==0 && c_axis.compare("Z")==0)
            {
                //std::cout << "\nstopping at BONE 2" << std::endl << std::endl;
                //break;
            }

        }
        else if(track->track_type == TRACK_TYPE_POINT && controller->name.compare("position")==0)
        {
            //Note: In anim8or, position keys are applied to the entire figure and not each bone so I am applying the position key to the root bone
            irr::scene::ISkinnedMesh::SJoint *root_joint;

            if(AnimatedMesh->getAllJoints().size() > 0)
                root_joint = AnimatedMesh->getAllJoints()[0];
            else
                continue;

            if(0)
            for(int key_index = 0; key_index < track->pointkey.size(); key_index++)
            {
                a_pkey = track->pointkey[key_index];
                irr::scene::ISkinnedMesh::SPositionKey* pkey = AnimatedMesh->addPositionKey(root_joint);

                pkey->frame = (irr::f32)a_pkey.frame_index;
                pkey->position.set( a_pkey.value.x, a_pkey.value.y, a_pkey.value.z );

                std::cout << "pos key[" << root_joint->PositionKeys.size()-1 << "] = frame(" << pkey->frame << ") position = " << pkey->position.X << ", "
                                                                                                   << pkey->position.Y << ", "
                                                                                                   << pkey->position.Z << ", "
                                                                                                   << std::endl;
            }

        }
        else if(track->track_type == TRACK_TYPE_FLOAT && controller->name.compare("scale")==0)
        {
            //Note: In anim8or, position keys are applied to the entire figure and not each bone so I am applying the scale key to the root bone
            irr::scene::ISkinnedMesh::SJoint *root_joint;

            if(AnimatedMesh->getAllJoints().size() > 0)
                root_joint = AnimatedMesh->getAllJoints()[0];
            else
                continue;

            for(int key_index = 0; key_index < track->floatkey.size(); key_index++)
            {
                a_fkey = track->floatkey[key_index];
                irr::scene::ISkinnedMesh::SScaleKey* skey = AnimatedMesh->addScaleKey(root_joint);

                skey->frame = (irr::f32)a_fkey.frame_index;
                skey->scale.set( a_fkey.value , a_fkey.value, a_fkey.value );

                std::cout << "scale key[" << root_joint->ScaleKeys.size()-1 << "] = frame(" << skey->frame << ") position = " << skey->scale.X << ", "
                                                                                                   << skey->scale.Y << ", "
                                                                                                   << skey->scale.Z << ", "
                                                                                                   << std::endl;
            }
        }

    }

    return 0;
}

void outputVec(core::vector3df v)
{
    std::cout << "(" << v.X << ", " << v.Y << ", " << v.Z << ") ";
}

scene::SSkinMeshBuffer* addAN8MeshBuffer(an8::an8_project* p, scene::ISkinnedMesh* AnimatedMesh, an8::an8_mesh mesh)
{
    scene::SSkinMeshBuffer* meshBuffer = AnimatedMesh->addMeshBuffer();

    //std::cout << "Mesh Info: " << mesh.points.size() << ", " << mesh.texcoords.size() << ", " << mesh.normals.size() << std::endl;

    core::array<video::S3DVertex> vertices;
    vertices.clear();

    video::S3DVertex v;

    //if(mesh.name.compare("test_mesh")!=0)
      //  return false;

    //-----Transformations applied to mesh in Object view in Anim8or-------
    core::matrix4 transform_matrix;
    transform_matrix.setTranslation( core::vector3df(mesh.base.origin.x,
                                                     mesh.base.origin.y,
                                                     mesh.base.origin.z));

    core::vector3df rot;
    core::quaternion quat( mesh.base.orientation.x, mesh.base.orientation.y, mesh.base.orientation.z, mesh.base.orientation.w );
    quat.toEuler(rot);

    transform_matrix.setRotationRadians(rot);

    //std::cout << "Translate: ";
    //outputVec(transform_matrix.getTranslation());
    //std::cout << std::endl;

    //std::cout << "Rotate: ";
    //outputVec(transform_matrix.getRotationDegrees());
    //std::cout << std::endl;

    //----------------------------------------------------------------------

    an8::an8_point3f default_normal;
    default_normal.x = 0;
    default_normal.y = 0;
    default_normal.z = 0;

    an8::an8_uv default_texture;
    default_texture.u = 0;
    default_texture.v = 0;

    core::array<irr::u16> indices;
    indices.clear();

    for(int i = 0; i < mesh.faces.size(); i++)
    {
        an8::an8_facedata face = mesh.faces[i];

        int vbuffer_start_index = vertices.size();

        for(int point_index = 0; point_index < face.num_points; point_index++)
        {
            int pi = face.point_data[point_index].point_index;
            an8::an8_point3f point = mesh.points[pi];

            //if(i == 1)
            //std::cout << "face_point[" << point_index << "] = ("
              //          << point.x << ", "
                //        << point.y << ", "
                  //      << point.z << ") " << std::endl;

            int normal_index = face.point_data[point_index].normal_index;
            an8::an8_point3f normal = default_normal;

            if(normal_index >= 0)
            {
                normal = mesh.normals[normal_index];
            }

            int texture_index = face.point_data[point_index].texture_index;
            an8::an8_uv texture = default_texture;

            if(texture_index >= 0)
            {
                texture = mesh.texcoords[texture_index];

                //if(i == 4)
                //std::cout << "(" << texture.u << ", " << texture.v << ")" << std::endl << std::endl;
            }

            v.Pos.set(point.x, point.y, -point.z);
            //std::cout << "DBG POINT: (" << v.Pos.X << ", " << v.Pos.Y << ", " << v.Pos.Z << ") to ";
            transform_matrix.transformVect(v.Pos);
            //std::cout << "(" << v.Pos.X << ", " << v.Pos.Y << ", " << v.Pos.Z << ")" << std::endl;

            //v.Normal.set(normal.x, normal.y, normal.z);
            v.TCoords.set(texture.u, 1-texture.v);
            v.Color.set(255,255,255,255);

            vertices.push_back(v);

            //Note: I could optimize a little by giving each vertex and id and only adding it if its
            //      not already in the array but I am not doing it because I want to set multiple normals
            //      for each vertex since anim8or outputs different normals depending on the face


            //On the fly triangulation
            if(point_index >= 2)
            {
                int vbuffer_index = vbuffer_start_index + point_index;

                indices.push_back(vbuffer_start_index);
                indices.push_back(vbuffer_index-1);
                indices.push_back(vbuffer_index);
            }

            //if(i == 2760)
            //    std::cout << "face_mt[" << point_index << "] = "
            //            << face.point_data[point_index].normal_index << ", "
            //            << face.point_data[point_index].texture_index << std::endl << std::endl;

        }
        //break;


    }

    if(indices.size() > 0)
    {
        //std::cout << "ADD VERTS TO BUFFER: " << indices.size() << std::endl << std::endl;

        for(int i = 0; i < vertices.size(); i++)
            meshBuffer->Vertices_Standard.push_back(vertices[i]);

        for(int i = 0; i < indices.size(); i++)
            meshBuffer->Indices.push_back(indices[i]);

        //meshBuffer->append((video::S3DVertex*)&vertices[0], vertices.size(), (irr::u16*)&indices[0], indices.size());
        //std::cout << "MeshBuffer Size = " << AnimatedMesh->getMeshBuffer(0)->getIndexCount() << std::endl << std::endl;
    }
    else
    {
        meshBuffer->drop();
        return NULL;
    }


    return meshBuffer;
}


core::vector3df vectorRadiansToDegrees(core::vector3df r)
{
    return core::vector3df( core::radToDeg(r.X), core::radToDeg(r.Y), core::radToDeg(r.Z) );
}

core::matrix4 calculate_bone_matrix(scene::ISkinnedMesh::SJoint* inJoint, an8::an8_bone* bone, an8::an8_bone* parent)
{
    //bone 4
    core::vector3df tvec( 0, parent->length, 0);
    core::quaternion q(parent->orientation.x,
                       parent->orientation.y,
                       parent->orientation.z,
                       parent->orientation.w);



    core::vector3df old_pos = parent->pos;
    core::vector3df t = tvec;
    core::matrix4 m = inJoint->GlobalMatrix;
    core::vector3df old_rotation = m.getRotationDegrees();

    //std::cout << "\nOLD ROT[" << bone->name <<"] = " << old_rotation.X << ", " << old_rotation.Y << ", " << old_rotation.Z << std::endl;

    core::vector3df angle;
    q.toEuler(angle);

    angle = vectorRadiansToDegrees(angle);


    m = rotateAxesXYZToEuler(old_rotation, angle, true);

    core::matrix4 tm;
    tm.setTranslation(old_pos);

    m = tm * m;
    //m.setTranslation(old_pos);
    //m.transformVect(t);
    //t += old_pos;

    return m;

}

irr::core::matrix4 rotateAxesXYZToEuler2(const irr::core::vector3df& oldRotation, const irr::core::vector3df& rotationAngles, bool useLocalAxes)
{
    irr::core::matrix4 transformation;
    transformation.setRotationDegrees(oldRotation);
    irr::core::vector3df axisX(1,0,0), axisY(0,1,0), axisZ(0,0,1);
    irr::core::matrix4 matRotX, matRotY, matRotZ;

    if ( useLocalAxes )
    {
        transformation.rotateVect(axisX);
        transformation.rotateVect(axisY);
        transformation.rotateVect(axisZ);
    }

    matRotX.setRotationAxisRadians(rotationAngles.X*irr::core::DEGTORAD, axisX);
    matRotY.setRotationAxisRadians(rotationAngles.Y*irr::core::DEGTORAD, axisY);
    matRotZ.setRotationAxisRadians(rotationAngles.Z*irr::core::DEGTORAD, axisZ);

    irr::core::matrix4 newTransform = matRotZ * matRotY * matRotX * transformation;
    return newTransform;
}

core::matrix4 calculate_bone_localMatrix(core::matrix4 transform_m, an8::an8_bone* bone, an8::an8_bone* parent)
{
    //bone 4
    core::vector3df tvec( 0, parent->length, 0);
    core::quaternion q(bone->orientation.x,
                       bone->orientation.y,
                       bone->orientation.z,
                       bone->orientation.w);



    core::vector3df old_pos = bone->pos;
    core::vector3df t = tvec;
    core::matrix4 m = transform_m;
    core::vector3df old_rotation = m.getRotationDegrees();

    core::vector3df angle;
    q.toEuler(angle);

    angle = vectorRadiansToDegrees(angle);

    m = rotateAxesXYZToEuler2(old_rotation, angle, true);

    //m.setTranslation(tvec);
    core::matrix4 tm;
    tm.setTranslation(tvec);
    m *= tm;

    t = core::vector3df(0,32.359,0);
    m.transformVect(t);

    std::cout << "TRANSFORM[" << bone->name << "] = " << t.X << ", " << t.Y << ", " << t.Z << std::endl;


    //m.transformVect(t);
    //t += old_pos;

    return transform_m;

}

//get the bone data for a figure
bool getAN8BoneNode(an8::an8_project* p, scene::ISkinnedMesh* AnimatedMesh, scene::ISkinnedMesh::SJoint* inJoint, an8::an8_bone bone, an8::an8_bone parent_bone)
{
	scene::ISkinnedMesh::SJoint *joint = AnimatedMesh->addJoint(inJoint);
	joint->Name = bone.name.c_str();

	//The root bone won't have a position despite whats in the *.an8 file
	joint->LocalMatrix.makeIdentity();
	joint->GlobalMatrix.makeIdentity();

	core::vector3df position(0,0,0);

	bone.rot = core::quaternion(0,0,0,0);

	if(inJoint)
    {
        joint->GlobalMatrix = calculate_bone_matrix(inJoint, &bone, &parent_bone);
        position.Y = parent_bone.length;
    }

    core::quaternion orient = core::quaternion(-bone.orientation.x,
                                               -bone.orientation.y,
                                               bone.orientation.z, //z needs to be reversed
                                               -bone.orientation.w);


    bone.rot = core::quaternion(-bone.orientation.x,
                                -bone.orientation.y,
                                bone.orientation.z, //z needs to be reversed
                                -bone.orientation.w);

    //bone.rot.set(joint->GlobalMatrix.getRotationDegrees());

    //bone.rot.X = -bone.rot.X;
    //bone.rot.Y = -bone.rot.Y;
    //bone.rot.Z = -bone.rot.Z;
    //bone.rot.W = -bone.rot.W;

	joint->GlobalMatrix.transformVect(position);
	bone.pos = position;
	bone.pos.Z = -bone.pos.Z;


    joint->Animatedrotation = bone.rot; //core::quaternion(0,0,0,-1);
    joint->Animatedposition = bone.pos;// - parent_bone.pos;


    //joint->LocalMatrix = calculate_bone_localMatrix(joint->GlobalMatrix, &bone, &parent_bone);

    std::cout << "BONE POS: " << bone.pos.X << ", " << bone.pos.Y << ", " << bone.pos.Z << std::endl;


    //Build LocalMatrix:

	core::matrix4 positionMatrix;
	positionMatrix.setTranslation( core::vector3df( 0, parent_bone.length, 0) );
	//positionMatrix.setTranslation( bone.pos );
	//positionMatrix.setTranslation(core::vector3df(0,0,0));

	core::matrix4 rotationMatrix;
	irr::core::matrix4 matRotX, matRotY, matRotZ;

	joint->Animatedrotation.getMatrix_transposed(rotationMatrix);
	//rotationMatrix = joint->Animatedrotation.getMatrix();


	//joint->LocalMatrix = rotationMatrix * positionMatrix;

	joint->LocalMatrix = positionMatrix * rotationMatrix;

	//core::vector3df vr;
	//bone.rot.toEuler(vr);
	//joint->LocalMatrix.setRotationRadians( core::vector3df(vr.X, vr.Y, -vr.Z));

	//core::vector3df v = rotationMatrix.getRotationDegrees();

	//std::cout << "BONE[" << bone.name << "] ROTM = " << v.X << ", " << v.Y << ", " << v.Z << std::endl;



    //Get Meshes  NOTE: a bone can have multiple meshes

    std::vector<an8::an8_mesh> meshList;

    an8::an8_irr_joint_data joint_data;
    joint_data.parent_joint = inJoint;
    joint_data.joint = joint;
    joint_data.bone = bone;

    getAN8MeshList(p, &bone.component, &meshList, &joint_data);


    //std::cout << "test on " << bone.name << ", " << bone.component.size() << std::endl;

    scene::SSkinMeshBuffer* meshBuffer;



    for(int i = 0; i < meshList.size(); i++)
    {
        //std::cout << "bone[" << bone.name << "] -> " << meshList[i].name << std::endl;

        meshBuffer = addAN8MeshBuffer(p, AnimatedMesh, meshList[i]);

        //loop through all the named objects for this joint
        for(int n_obj_index = 0; n_obj_index < joint_data.namedobject.size(); n_obj_index++)
        {
            an8::an8_namedobject* n_obj = &joint_data.namedobject[n_obj_index];

            //loop through all the meshList indices stored when getAN8MeshList() was called
            for(int m_index = 0; m_index < n_obj->meshList_index.size(); m_index++)
            {
                //if this current index is in the list then store the mesh buffer in the named object
                // NOTE: I need to store it here so I can reference it when applying weights later
                if(i == n_obj->meshList_index[m_index])
                {
                    n_obj->meshBufferList.push_back(meshBuffer);
                    n_obj->base_bone = bone.name;

                    //apply named object transformations from figure view
                    core::vector3df nobj_translate(n_obj->base.origin.x, n_obj->base.origin.y, -n_obj->base.origin.z);
                    meshBuffer->Transformation.setTranslation(nobj_translate);

                    core::quaternion nobj_rotate(-n_obj->base.orientation.x, -n_obj->base.orientation.y, n_obj->base.orientation.z, n_obj->base.orientation.w);
                    meshBuffer->Transformation *= nobj_rotate.getMatrix();
                }
            }
        }
    }

    meshList.clear();

    //loop through all the joint_data in the project up to this point
    for(int jd_index = 0; jd_index < p->irr_joint_data.size(); jd_index++)
    {
        //loop through the named objects in joint data
        for(int n_obj_index = 0; n_obj_index < p->irr_joint_data[jd_index].namedobject.size(); n_obj_index++)
        {
            an8::an8_namedobject* n_obj = &p->irr_joint_data[jd_index].namedobject[n_obj_index];

            //loop through the weightedBy to see if the current bone weights it
            for(int wb_index = 0; wb_index < n_obj->weightedBy.size(); wb_index++)
            {
                std::string wb = n_obj->weightedBy[wb_index].bone_name;

                //if the named object is weighted by the current bone then add the named object to this bones joint_data
                if(wb.compare(bone.name)==0)
                {
                    bool should_add = true;

                    //loop through named objects in joint to make sure the current object isn't in there
                    for(int jdn_index = 0; jdn_index < joint_data.namedobject.size(); jdn_index++)
                    {
                        if(joint_data.namedobject[jdn_index].name.compare(n_obj->name)==0)
                        {
                            should_add = false;
                            break;
                        }
                    }

                    if(should_add)
                        joint_data.namedobject.push_back(n_obj[0]);
                }
            }
        }
    }

    //push joint data onto the project tmp data list (Its ghetto but it works)
    p->irr_joint_data.push_back(joint_data);

    //string obj_name =


    //Get Keys

    //Get Animations

    //Add Child Bones

    //std::cout << "start: " << bone.name << ": " << bone.bone.size() << std::endl;
    for(int i = 0; i < bone.bone.size(); i++)
    {
        getAN8BoneNode(p, AnimatedMesh, joint, bone.bone[i], bone);
    }
    //std::cout << "end: " << bone.name << std::endl;

    return true;
}


struct old_mesh_buffer
{
    irr::u16 id;
    scene::SSkinMeshBuffer buffer;
};

irr::s32 idInList( std::vector< old_mesh_buffer > idList, irr::u16 id)
{
    irr::s32 list_index = -1;
    for(int i = 0; i < idList.size(); i++)
    {
        if(idList[i].id == id)
        {
            list_index = i;
            break;
        }
    }
    return list_index;
}


scene::IAnimatedMesh* loadAN8Scene(IrrlichtDevice* device, an8::an8_project a_file, std::string scene_name)
{
    int scene_index = -1;

    for(int i = 0; i < a_file.scenes.size(); i++)
    {
        if(a_file.scenes[i].name.compare(scene_name)==0)
        {
            scene_index = i;
            break;
        }
    }

    if(scene_index < 0)
    {
        std::cout << "No scene found" << std::endl;
        return NULL;
    }

    an8::an8_scene mesh_scene = a_file.scenes[scene_index];


    scene::ISceneManager* smgr = device->getSceneManager();

    scene::ISkinnedMesh* mesh = smgr->createSkinnedMesh();

    //tmp variables
    an8::an8_element element;
    an8::an8_figure figure;
    an8::an8_object obj;

    std::vector<an8::an8_mesh> meshList;
    std::vector<old_mesh_buffer> figure_transformed_meshBuffers;




    //std::cout << "DEBUG 1" <<  std::endl;
    //Make magic happen
    for(int i = 0; i < mesh_scene.elements.size(); i++)
    {
        std::cout << "i = " << mesh_scene.elements[i].name << std::endl;

        element = mesh_scene.elements[i];

        switch(mesh_scene.elements[i].element_type)
        {
            case ELEMENT_TYPE_FIGURE:

                std::cout << "obj_name = " << element.obj_name << std::endl;

                if(!getAN8Figure(&a_file, element.obj_name, &figure))
                    continue;

                a_file.node_figure = figure;

                figure.bone.length = 0;

                getAN8BoneNode(&a_file, mesh, (scene::ISkinnedMesh::SJoint*)0, figure.bone, figure.bone);
                //break;


                //std::cout << "obj<" << element.obj_name << "> meshbuffers = " << mesh->getMeshBufferCount() << std::endl << std::endl;

                //std::cout << "loop data: " << a_file.irr_joint_data.size() << ", " << a_file.irr_joint_data[0].namedobject.size() << std::endl << std::endl;

                for(int n = 0; n < a_file.irr_joint_data.size(); n++)
                {
                    //std::cout << "Bone[" << a_file.irr_joint_data[n].bone.name << "]" << std::endl;
                    //std::cout << "Num Weighted Objects = " << a_file.irr_joint_data[n].namedobject.size() << std::endl << std::endl;



                    for(int n_obj_index = 0; n_obj_index < a_file.irr_joint_data[n].namedobject.size(); n_obj_index++)
                    {
                        an8::an8_namedobject* n_obj = &a_file.irr_joint_data[n].namedobject[n_obj_index];

                        a_file.irr_joint_data[n].namedobject_index = n_obj_index;

                        //std::cout << "Object[" << n_obj->name_block.name << " -> " << n_obj->name << "]: MeshBuffers = " << n_obj->meshBufferList.size() << std::endl;

                        //continue;

                        for(int mbuffer_index = 0; mbuffer_index < n_obj->meshBufferList.size(); mbuffer_index++)
                        {
                            scene::SSkinMeshBuffer* mesh_buffer = n_obj->meshBufferList[mbuffer_index];

                            irr::u16 mesh_buffer_id = -1;

                            //loop through the meshes to get the index
                            for(int mbi = 0; mbi < mesh->getMeshBufferCount(); mbi++)
                            {
                                scene::SSkinMeshBuffer* mb = mesh->getMeshBuffers()[mbi];
                                if(mb == mesh_buffer)
                                {
                                    //std::cout << "Found Mesh Buffer for <" << element.obj_name << ", " << a_file.irr_joint_data[n].bone.name << ">" << std::endl;
                                    mesh_buffer_id = mbi;
                                    break;
                                }
                            }

                            if(mesh_buffer_id < 0)
                            {
                                std::cout << "NO BUFFER ID" << std::endl;
                                continue;
                            }

                            //loop through all the vertices in the buffer
                            //std::cout << "Loop through vertices: " << mesh_buffer->Vertices_Standard.size() << std::endl;

                            irr::s32 meshBuffer_figure_transform_calculated = idInList(figure_transformed_meshBuffers, mesh_buffer_id);

                            if(meshBuffer_figure_transform_calculated >= 0)
                                mesh_buffer_id = figure_transformed_meshBuffers[meshBuffer_figure_transform_calculated].id;

                            if(meshBuffer_figure_transform_calculated < 0)
                            {
                                old_mesh_buffer omb;
                                omb.id = mesh_buffer_id;
                                omb.buffer = mesh_buffer[0];
                                figure_transformed_meshBuffers.push_back(omb);
                            }

                            for(int vert_index = 0; vert_index < mesh_buffer->Vertices_Standard.size(); vert_index++)
                            {
                                video::S3DVertex vertex;
                                if(meshBuffer_figure_transform_calculated < 0)
                                {
                                    vertex = mesh_buffer->Vertices_Standard[vert_index];
                                    //std::cout << "use vertex[" << ((vert_index==39)*11) << "]: " << vertex.Pos.X << ", " << vertex.Pos.Y << ", " << vertex.Pos.Z << std::endl;
                                }
                                else
                                {
                                    vertex = figure_transformed_meshBuffers[meshBuffer_figure_transform_calculated].buffer.Vertices_Standard[vert_index];
                                    //std::cout << "use transform[" << ((vert_index==39)*11) << "]: " << vertex.Pos.X << ", " << vertex.Pos.Y << ", " << vertex.Pos.Z << std::endl;
                                }
                                an8::an8_irr_joint_data* joint_data = &a_file.irr_joint_data[n];

                                scene::ISkinnedMesh::SWeight* vert_weight = mesh->addWeight(joint_data->joint);

                                vert_weight->buffer_id = mesh_buffer_id;
                                vert_weight->vertex_id = vert_index;

                                vert_weight->strength = an8_calculate_figure_transform(&vertex, a_file.irr_joint_data[n], (vert_index==39)*11);

                                if(meshBuffer_figure_transform_calculated < 0)
                                    mesh_buffer->Vertices_Standard[vert_index] = vertex;
                            }

                            std::cout << std::endl << std::endl;
                        }
                    }
                    cout << std::endl << std::endl;
                }

                an8_addKeys(&a_file, mesh, scene_index, i);


                //setAN8Track(mesh_scene.elements[i]);
                break;

            case ELEMENT_TYPE_OBJECT:

                //std::cout << "obj_name = " << element.obj_name << std::endl;

                break;
        }
    }



    mesh->finalize();

    //irr::scene::IMeshManipulator *meshManip = device->getSceneManager()->getMeshManipulator();
    //meshManip->flipSurfaces(mesh);

    return mesh;
}



bool drawJoints_Init = false;


void drawJointCircles(IrrlichtDevice *device, scene::ICameraSceneNode* camera, irr::scene::IAnimatedMeshSceneNode* node, float radius) {
    if (!node || !device) return;

    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();

    scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

    // Get the absolute transformation of the node
    irr::core::matrix4 transform = node->getAbsoluteTransformation();

    // Retrieve the skeleton (the array of joints)
    //const irr::core::array<irr::scene::IBoneSceneNode*>& joints = node->getJointList();

    // Set the color of the circle
    irr::video::SColor color[5];
    color[0] = irr::video::SColor(255, 0, 0, 0); // Red color with full opacity
    color[1] = irr::video::SColor(255, 255, 0, 0);
    color[2] = irr::video::SColor(255, 0, 255, 0);
    color[3] = irr::video::SColor(255, 0, 0, 255);
    color[4] = irr::video::SColor(255, 255, 255, 255);

    // Iterate through all the joints
    for (irr::u32 i = 0; i < node->getJointCount(); ++i) {
        irr::scene::IBoneSceneNode* joint = node->getJointNode(i);

        // Get the position of the joint
        irr::core::vector3df jointPos = joint->getAbsolutePosition();
        //jointPos = joint->getPosition();

        // Transform the 3D position to 2D screen coordinates
        irr::core::vector2di screenPos = collMan->getScreenCoordinatesFrom3DPosition(jointPos, camera);

        // Draw the circle on the screen
        driver->draw2DPolygon(screenPos, radius, color[i], 16);

        if(!drawJoints_Init)
        {
            std::cout << "Joint Position[" << i << "] = " << jointPos.X << ", " << jointPos.Y << ", " << jointPos.Z << std::endl;
        }
    }

    drawJoints_Init = true;
}


enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};

int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=EDT_OPENGL;

	// create device
	IrrlichtDevice *device =
		createDevice(driverType, core::dimension2d<u32>(640, 480), 16, false);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	//    device->getFileSystem()->addFileArchive("./assets/assets.zip");
	device->getFileSystem()->addFileArchive("../../media/map-20kdm2.pk3");

	scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode* q3node = 0;

	// The Quake mesh is pickable, but doesn't get highlighted.
	if (q3levelmesh)
		q3node = smgr->addOctreeSceneNode(q3levelmesh->getMesh(0), 0, IDFlag_IsPickable);

	/*
	So far so good, we've loaded the quake 3 level like in tutorial 2. Now,
	here comes something different: We create a triangle selector. A
	triangle selector is a class which can fetch the triangles from scene
	nodes for doing different things with them, for example collision
	detection. There are different triangle selectors, and all can be
	created with the ISceneManager. In this example, we create an
	OctreeTriangleSelector, which optimizes the triangle output a little
	bit by reducing it like an octree. This is very useful for huge meshes
	like quake 3 levels. After we created the triangle selector, we attach
	it to the q3node. This is not necessary, but in this way, we do not
	need to care for the selector, for example dropping it after we do not
	need it anymore.
	*/

	scene::ITriangleSelector* selector = 0;

	if (q3node)
	{
		q3node->setPosition(core::vector3df(-1350,-130,-1400));

		selector = smgr->createOctreeTriangleSelector(
				q3node->getMesh(), q3node, 128);
		q3node->setTriangleSelector(selector);
		// We're not done with this selector yet, so don't drop it.
	}


	scene::ICameraSceneNode* camera =
		smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
	camera->setPosition(core::vector3df(50,50,-60));
	camera->setTarget(core::vector3df(-70,30,-60));

	if (selector)
	{
		scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camera, core::vector3df(30,50,30),
			core::vector3df(0,-10,0), core::vector3df(0,30,0));
		selector->drop(); // As soon as we're done with the selector, drop it.
		camera->addAnimator(anim);
		anim->drop();  // And likewise, drop the animator when we're done referring to it.
	}

	// Now I create three animated characters which we can pick, a dynamic light for
	// lighting them, and a billboard for drawing where we found an intersection.

	// First, let's get rid of the mouse cursor.  We'll use a billboard to show
	// what we're looking at.
	device->getCursorControl()->setVisible(false);

	// Add the billboard.
	scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	bill->setMaterialTexture(0, driver->getTexture("../../media/particle.bmp"));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZBUFFER, false);
	bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
	bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it

	/* Add 3 animated hominids, which we can pick using a ray-triangle intersection.
	They all animate quite slowly, to make it easier to see that accurate triangle
	selection is being performed. */
	scene::IAnimatedMeshSceneNode* node = 0;

	video::SMaterial material;


	//an8::an8_project p = an8::loadAN8("assets/test4.an8");
	an8::an8_project p = an8::loadAN8("assets/knight_f4.an8");

	std::cout << "test load: " << p.scenes.size() << std::endl;
	//device->drop();
	//return 0;

	scene::IAnimatedMesh* test_mesh = loadAN8Scene(device, p, "scene01");
	std::cout << "test load complete" << std::endl;

	// Add an MD2 node, which uses vertex-based animation.
	node = smgr->addAnimatedMeshSceneNode(test_mesh,
						0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	//node->setPosition(core::vector3df(90,-15,-140)); // Put its feet on the floor.
	//node->setScale(core::vector3df(0.5f, 0.5f, 0.5f)); // Make it appear realistically scaled
	//node->setMD2Animation(scene::EMAT_POINT);
	//node->setAnimationSpeed(20.f);
	node->setCurrentFrame(0);
	node->setAnimationSpeed(12);
	//node->setFrameLoop(0, 51);
	node->setLoopMode(true);
	//node->setPosition( node->getPosition() + core::vector3df(0,-160,0));
	std::cout << "node: " << node->getMesh()->getFrameCount() << std::endl;
	material.setTexture(0, driver->getTexture("assets/knight_f_texture.bmp"));
	material.Lighting = false;
	//material.NormalizeNormals = true;
	node->getMaterial(0) = material;

	// Now create a triangle selector for it.  The selector will know that it
	// is associated with an animated node, and will update itself as necessary.
	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop(); // We're done with this selector, so drop it now.




	material.setTexture(0, 0);
	material.Lighting = false;

	// Add a light, so that the unselected nodes aren't completely dark.
	scene::ILightSceneNode * light = smgr->addLightSceneNode(0, core::vector3df(-60,100,400),
		video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
	light->setID(ID_IsNotPickable); // Make it an invalid target for selection.

	// Remember which scene node is highlighted
	scene::ISceneNode* highlightedSceneNode = 0;
	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
	int lastFPS = -1;

	// draw the selection triangle only as wireframe
	material.Wireframe=true;

	//if(0)
	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, 0);
		smgr->drawAll();

		// Unlight any currently highlighted scene node
		if (highlightedSceneNode)
		{
			//highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
			highlightedSceneNode = 0;
		}

		// All intersections in this example are done with a ray cast out from the camera to
		// a distance of 1000.  You can easily modify this to check (e.g.) a bullet
		// trajectory or a sword's position, or create a ray from a mouse click position using
		// ISceneCollisionManager::getRayFromScreenCoordinates()
		core::line3d<f32> ray;
		ray.start = camera->getPosition();
		ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

		// Tracks the current intersection point with the level or a mesh
		core::vector3df intersection;
		// Used to show with triangle has been hit
		core::triangle3df hitTriangle;

		// This call is all you need to perform ray/triangle collision on every scene node
		// that has a triangle selector, including the Quake level mesh.  It finds the nearest
		// collision point/triangle, and returns the scene node containing that point.
		// Irrlicht provides other types of selection, including ray/triangle selector,
		// ray/box and ellipse/triangle selector, plus associated helpers.
		// See the methods of ISceneCollisionManager
		scene::ISceneNode * selectedSceneNode =
			collMan->getSceneNodeAndCollisionPointFromRay(
					ray,
					intersection, // This will be the position of the collision
					hitTriangle, // This will be the triangle hit in the collision
					IDFlag_IsPickable, // This ensures that only nodes that we have
							// set up to be pickable are considered
					0); // Check the entire scene (this is actually the implicit default)

		// If the ray hit anything, move the billboard to the collision position
		// and draw the triangle that was hit.
		if(selectedSceneNode)
		{
			bill->setPosition(intersection);

			// We need to reset the transform before doing our own rendering.
			driver->setTransform(video::ETS_WORLD, core::matrix4());
			driver->setMaterial(material);
			driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));

			// We can check the flags for the scene node that was hit to see if it should be
			// highlighted. The animated nodes can be highlighted, but not the Quake level mesh
			if((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
			{
				highlightedSceneNode = selectedSceneNode;

				// Highlighting in this case means turning lighting OFF for this node,
				// which means that it will be drawn with full brightness.
				//highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
			}
		}

		// We're all done drawing, so end the scene.

		drawJointCircles(device, camera, node, 10);

		driver->endScene();

		//break;

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw str = L"Collision detection example - Irrlicht Engine [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}

/*
**/


