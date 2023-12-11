#ifndef AN8_H_INCLUDED
#define AN8_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <irrlicht/irrlicht.h>
#include <cmath>

#ifdef IRRLICHT_SDK_VERSION
#define AN8_IRRLICHT
#endif // IRRLICHT_SDK_VERSION

//#define AN8_DEBUG 1


namespace an8
{

std::string ltrim(std::string l_string)
{
    if(l_string.find_first_not_of(" ") != std::string::npos)
        return l_string.substr(l_string.find_first_not_of(" "));
    return "";
}

std::string rtrim(std::string src)
{
    if(src.length()==0)
        return "";

    unsigned long i = 0;
    for(i = src.length()-1; ; i--)
    {
        if(src.substr(i,1).compare(" ") != 0 || i == 0)
            break;
    }
    return src.substr(0,i+1);
}

std::string trim(std::string t_string)
{
    return ltrim(rtrim(t_string));
}

std::string str_replace(std::string src, std::string tgt, std::string rpc)
{
    if(tgt.length()==0)
        return src;
    unsigned long found_inc = rpc.length() > 0 ? rpc.length() : 1;
    size_t found = 0;
    found = src.find(tgt);
    while( found != std::string::npos && found < src.length())
    {
        src = src.substr(0,found) + rpc + src.substr(found + tgt.length());
        found = src.find(tgt,found+found_inc);
    }
    return src;
}

struct an8_header
{
    std::string version;
    std::string build;
};

struct an8_description
{
    std::string text;
};

struct an8_grid
{
    int auto_grid;
    float model_grid_spacing;
    float scene_edit_grid_spacing;
    float ground_grid_size;
};

struct an8_environment
{
    an8_grid grid;
    int32_t framerate;
    int32_t limitplayback;
};

struct an8_texture
{
    std::string name;
    int32_t invert; //I am not actually sure what this is
    int32_t cubemap; //not sure what this is
    std::string file[6]; //will only use file[0] unless cubemap is non-zero
};

struct an8_color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#define AN8_BLEND_MODE_DECAL 0
#define AN8_BLEND_MODE_DARKEN 1
#define AN8_BLEND_MODE_LIGHTEN 2

#define AN8_ALPHA_MODE_NONE 0
#define AN8_ALPHA_MODE_LAYER 1
#define AN8_ALPHA_MODE_FINAL 2

struct an8_texture_param
{
    uint8_t blend_mode;
    uint8_t alpha_mode;
    int32_t percent;
};

struct an8_surface_property
{
    an8_color color;
    float factor;
    std::string texturename;
    an8_texture_param textureparams;

};

struct an8_map
{
    std::string texturename;
    std::string kind;
    an8_texture_param textureparams;
};

struct an8_surface
{
    an8_surface_property ambient;
    an8_surface_property diffuse;
    an8_surface_property specular;
    an8_surface_property emissive;
    uint8_t alpha;
    float brilliance;
    float phongsize;
    std::vector<an8_map> surface_map;
    bool lockambdiff;
};

struct an8_material
{
    std::string name;
    an8_surface surface;
    an8_surface backsurface;
};

struct an8_point2i
{
    int32_t x;
    int32_t y;
};

struct an8_point2f
{
    double x;
    double y;
};

struct an8_point3f
{
    double x;
    double y;
    double z;
};

struct an8_point4f
{
    double x;
    double y;
    double z;
    double w;
};

struct an8_uv
{
    double u;
    double v;
};

struct an8_edge
{
    uint32_t point_index1;
    uint32_t point_index2;
    int32_t sharpness;
};

struct an8_pointdata
{
    int32_t point_index;
    int32_t normal_index; // index in normals
    int32_t texture_index; // texture index
};

#define AN8_FACE_SF_SHOW_BACK   1
#define AN8_FACE_SF_HAS_NORMALS 2
#define AN8_FACE_SF_HAS_TEXTURE 4

struct an8_facedata
{
    int32_t num_points;
    uint8_t flags; //bitmask
    uint32_t material_index;
    int32_t flat_normal_index; //index in normal array or -1 if no value is stored in an8 file
    std::vector<an8_pointdata> point_data;
};

struct an8_base
{
    an8_point3f origin;
    an8_point4f orientation;
};

struct an8_mesh
{
    std::string name;

    an8_base base;
    an8_base pivot;
    an8_material material; //used for new faces added to mesh; only name is used
    float smoothangle;
    std::vector<std::string> materialname;
    std::vector<an8_point3f> points;
    std::vector<an8_point3f> normals;
    std::vector<an8_edge> edges; //mostly unused
    std::vector<an8_uv> texcoords;
    std::vector<an8_facedata> faces;

    //irrlicht loader specific crap
    #ifdef AN8_IRRLICHT
    void* meshBuffer;
    #endif // AN8_IRRLICHT
};

struct an8_name
{
    std::string name;
};

struct an8_weightedBy
{
    std::string bone_name;
};

struct an8_boneWeight
{
    uint32_t bone_index; //This is the index in the weightedBy vector in namedobject
    double weight; //normalized
};

struct an8_vertexWeight
{
    std::vector<an8_boneWeight> boneWeight;
};

struct an8_meshWeights
{
    std::string name;
    std::vector<an8_vertexWeight> vertexWeight;
};

struct an8_namedobject
{
    std::string name;
    an8_name name_block;
    an8_base base;
    an8_base pivot;
    an8_material material;
    std::vector<an8_weightedBy> weightedBy;
    std::vector<an8_meshWeights> meshWeights;

    //irrlicht loader specific stuff
    #ifdef AN8_IRRLICHT
    std::vector<int> meshList_index;

    // Note: Each named object will only have one object but
    //       an object can have multiple meshes
    std::vector<irr::scene::SSkinMeshBuffer*> meshBufferList;
    std::string base_bone; //base transformations will happen from this bone
    #endif // AN8_IRRLICHT
};

#define AN8_COMPONENT_TYPE_MESH         0
#define AN8_COMPONENT_TYPE_SPHERE       1
#define AN8_COMPONENT_TYPE_CYLINDER     2
#define AN8_COMPONENT_TYPE_CUBE         3
#define AN8_COMPONENT_TYPE_SUBDIVISION  4
#define AN8_COMPONENT_TYPE_PATH         5
#define AN8_COMPONENT_TYPE_TEXTCOM      6
#define AN8_COMPONENT_TYPE_MODIFIER     7
#define AN8_COMPONENT_TYPE_IMAGE        8
#define AN8_COMPONENT_TYPE_NAMEDOBJECT  9
#define AN8_COMPONENT_TYPE_GROUP        10

struct an8_component
{
    uint8_t type;
    an8_mesh mesh;
    //sphere
    //cylinder
    //cube
    //subdivision
    //path
    //textcom
    //modifier
    //image
    an8_namedobject named_object;
    //group

    an8_base base;
    an8_base pivot;
};

struct an8_object
{
    std::string name;
    std::vector<an8_material> material;
    std::vector<an8_component> component;
};

struct an8_dof
{
    std::string axis; //X, Y, or Z
    double min_angle;
    double default_angle;
    double max_angle;
    bool locked;
    bool unlimited;
};

struct an8_influence
{
    double center0;
    double inRadius0;
    double outRadius0;
    double center1;
    double inRadius1;
    double outRadius1;
};

struct an8_bone
{
    std::string name;
    double length;
    double diameter; // seems to be optional
    an8_point4f orientation;
    bool locked; //not important, just here for completion
    std::vector<an8_dof> dof;
    an8_influence influence;
    std::vector<an8_component> component;
    std::vector<an8_bone> bone;

    //irrlicht stuff
    #ifdef AN8_IRRLICHT
    irr::core::vector3df pos;
    irr::core::quaternion rot;
    #endif // AN8_IRRLICHT
};

struct an8_figure
{
    std::string name;
    std::vector<an8_material> material;
    an8_bone bone;
};

struct an8_floatkey
{
    uint32_t frame_index;
    double value;
    std::string mod_string;
};

struct an8_pointkey
{
    uint32_t frame_index;
    an8_point3f value;
    an8_point3f forward_vector;
    an8_point3f reverse_vector;
    std::string mod_string;
};

struct an8_qkey
{
    uint32_t frame_index;
    an8_point4f value;
    std::string mod_string;
};

struct an8_booleankey
{
    uint32_t frame_index;
    bool value;
    std::string mod_string;
};

#define TRACK_TYPE_NONE         0
#define TRACK_TYPE_FLOAT        1
#define TRACK_TYPE_POINT        2
#define TRACK_TYPE_QUATERNION   3
#define TRACK_TYPE_BOOLEAN      4

struct an8_track
{
    int track_type;
    std::vector<an8_floatkey> floatkey;
    std::vector<an8_pointkey> pointkey;
    std::vector<an8_qkey> qkey;
    std::vector<an8_booleankey> booleankey;
};

struct an8_jointangle
{
    std::string bone_name;
    std::string axis;
    an8_track track;
};

struct an8_sequence
{
    std::string name;
    std::string figure_name;
    uint32_t num_frames;
    std::vector<an8_jointangle> jointangle;
};

struct an8_panorama
{
    double left_longitude;
    double right_longitude;
    double bottom_latitude;
    double top_latitude;
};

#define IMAGE_TYPE_NORMAL   0
#define IMAGE_TYPE_PANORAMA 1

struct an8_image
{
    std::string filename;
    uint32_t image_type;;
    an8_panorama panorama;
};

struct an8_fog
{
    an8_color color;
    double fog_start;
    double fog_end;
    uint32_t fog_percent;
    bool radial;
};


struct an8_controller
{
    std::string name;
    an8_track track;
};

struct an8_namedsequence
{
    std::string name;
    uint32_t frame_num;
};

#define ELEMENT_TYPE_CAMERA 1
#define ELEMENT_TYPE_FIGURE 2
#define ELEMENT_TYPE_OBJECT 3
#define ELEMENT_TYPE_LIGHT  4
#define ELEMENT_TYPE_NULL   0

#define ORIENT_RELATIVE_TO_FACESPATH    1
#define ORIENT_RELATIVE_TO_FACESTARGET  2
#define ORIENT_RELATIVE_TO_ORIENTTARGET 3

#define LIGHT_TYPE_INFINITE 1
#define LIGHT_TYPE_LOCAL    2
#define LIGHT_TYPE_SPOT     3

struct an8_element
{
    uint32_t element_type;
    std::string name;
    an8_point3f loc;
    an8_point4f orientation;
    bool roll;
    uint32_t orient_relative_to;
    std::string bound_target; //if it has the facestarget or orienttarget chunk
    double scale;
    bool locked;

    double fov; // camera

    an8_color color; // light
    uint32_t light_type;
    double in_radius;
    double out_radius;
    double in_angle;
    double out_angle;
    bool cast_shadow;
    bool raytrace_shadow;
    bool soft_edges;
    double soft_size;
    uint32_t min_samples;
    uint32_t max_samples;
    bool monte_carlo;

    std::string obj_name; // object
    bool visible;
    bool receive_shadow;

    std::vector<an8_namedsequence> named_sequence; // figure

    std::vector<an8_controller> controller;
    std::vector<an8_element> element; //child elements
};

struct an8_scene
{
    std::string name;
    uint32_t num_frames;
    bool groundgrid;
    double shadowbias;
    an8_color background;
    an8_image image;
    an8_fog fog;
    double znear;
    double zfar;
    std::vector<an8_element> elements;
};


#ifdef AN8_IRRLICHT
struct an8_irr_joint_data
{
    std::vector<an8_namedobject> namedobject;
    an8_bone bone;
    irr::scene::ISkinnedMesh::SJoint *joint;

    //irrlicht specific stuff
    int32_t namedobject_index; //index of named object used during an8_calculate_weight()
    //vector<irr::u32> meshBufferIndex; //This is a list of meshes weighted by this bone
    irr::scene::ISkinnedMesh::SJoint *parent_joint;
    double parent_length;
    irr::core::matrix4 GlobalMatrix;

};
#endif // AN8_IRRLICHT


struct an8_project
{
    an8_header header;
    an8_description description;
    an8_environment environment;
    std::vector<an8_texture> textures;
    std::vector<an8_material> materials;
    std::vector<an8_object> objects;

    std::vector<an8_figure> figures;
    std::vector<an8_sequence> sequences;
    std::vector<an8_scene> scenes;

    //The following are not part of anim8or structs but just used for storing temp data when loading irrlicht mesh
    #ifdef AN8_IRRLICHT
    std::vector<an8_irr_joint_data> irr_joint_data;
    an8::an8_figure node_figure; // I am going to store the figure the current bones are from here
    #endif // AN8_IRRLICHT
};

struct an8_file_block
{
    int type;
    int start_index = -1;
    int end_index = -1;
    std::string name;
    std::string obj_name;
    std::string value;
    std::string value2;
    std::string value3;
    an8_file_block* parent;
    std::vector<an8_file_block> block;
};


void getHeader(an8_project* project, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("header")==0)
        {
            for(int j = 0; j < c_block->block.size(); j++)
            {
                if(c_block->block[j].name.compare("version")==0)
                    project->header.version = c_block->block[j].obj_name;
                else if(c_block->block[j].name.compare("build")==0)
                    project->header.build = c_block->block[j].obj_name;

                #ifdef AN8_DEBUG
                cout << "header attribute[" << c_block->block[j].name << "] = " << c_block->block[j].obj_name << endl;
                #endif
            }
            break;
        }
    }

}


void getTextureParam(an8_texture_param* t_param, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("blendmode")==0)
        {
            if(c_block->value.compare("decal")==0)
                t_param->blend_mode = AN8_BLEND_MODE_DECAL;
            else if(c_block->value.compare("darken")==0)
                t_param->blend_mode = AN8_BLEND_MODE_DARKEN;
            else if(c_block->value.compare("lighten")==0)
                t_param->blend_mode = AN8_BLEND_MODE_LIGHTEN;
        }
        else if(c_block->name.compare("alphamode")==0)
        {
            if(c_block->value.compare("none")==0)
                t_param->alpha_mode = AN8_ALPHA_MODE_NONE;
            else if(c_block->value.compare("layer")==0)
                t_param->alpha_mode = AN8_ALPHA_MODE_LAYER;
            else if(c_block->value.compare("final")==0)
                t_param->alpha_mode = AN8_ALPHA_MODE_FINAL;
        }
        else if(c_block->name.compare("percent")==0)
        {
            t_param->percent = atoi(c_block->value.c_str());
        }
    }

}


void getSurfaceProperty(an8_surface_property* surf_property, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("rgb")==0)
        {
            std::string c_value = c_block->value + " ";
            std::string c_str = "";
            uint8_t c[3];
            int ci = 0;
            for(int j = 0; j < c_value.length(); j++)
            {
                if(c_value.substr(j,1).compare(" ")==0)
                {
                    c[ci] = atoi(c_value.c_str());
                    ci++;

                    if(ci >= 3)
                        break;
                }

            }

            an8_color color;
            color.r = c[0];
            color.g = c[1];
            color.b = c[2];
            //cout << "color = " << (int)c[0] << ", " << (int)c[1] << ", " << (int)c[2] << endl;
            surf_property->color = color;
        }
        else if(c_block->name.compare("factor")==0)
        {
            surf_property->factor = atof(c_block->value.c_str());
        }
        else if(c_block->name.compare("texturename")==0)
        {
            surf_property->texturename = c_block->obj_name;
        }
        else if(c_block->name.compare("textureparams")==0)
        {
            an8_texture_param t_param;

            getTextureParam(&t_param, c_block);

            surf_property->textureparams = t_param;
        }
    }

}

void getMap(an8_map* surface_map, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("kind")==0)
        {
            surface_map->kind = c_block->obj_name;
        }
        else if(c_block->name.compare("texturename")==0)
        {
            surface_map->texturename = c_block->obj_name;
        }
        else if(c_block->name.compare("textureparams")==0)
        {
            an8_texture_param t_param;
            getTextureParam(&t_param, c_block);
            surface_map->textureparams = t_param;
        }
    }

}

void getSurface(an8_material* mat, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("surface")==0 || c_block->name.compare("backsurface")==0)
        {
            an8_surface surface;

            for(int j = 0; j < c_block->block.size(); j++)
            {
                if(c_block->block[j].name.compare("ambient")==0)
                {
                    an8_surface_property surf_property;
                    getSurfaceProperty(&surf_property, &c_block->block[j]);
                    surface.ambient = surf_property;
                }
                else if(c_block->block[j].name.compare("diffuse")==0)
                {
                    an8_surface_property surf_property;
                    getSurfaceProperty(&surf_property, &c_block->block[j]);
                    surface.diffuse = surf_property;
                }
                else if(c_block->block[j].name.compare("specular")==0)
                {
                    an8_surface_property surf_property;
                    getSurfaceProperty(&surf_property, &c_block->block[j]);
                    surface.specular = surf_property;
                }
                else if(c_block->block[j].name.compare("emissive")==0)
                {
                    an8_surface_property surf_property;
                    getSurfaceProperty(&surf_property, &c_block->block[j]);
                    surface.emissive = surf_property;
                }
                else if(c_block->block[j].name.compare("alpha")==0)
                {
                    surface.alpha = (uint8_t)atoi(c_block->block[i].value.c_str());
                }
                else if(c_block->block[j].name.compare("brilliance")==0)
                {
                    surface.brilliance = atof(c_block->block[i].value.c_str());
                }
                else if(c_block->block[j].name.compare("phongsize")==0)
                {
                    surface.phongsize = atof(c_block->block[i].value.c_str());
                }
                else if(c_block->block[j].name.compare("map")==0)
                {
                    an8_map surface_map;
                    getMap(&surface_map, &c_block->block[j]);
                    surface.surface_map.push_back(surface_map);
                }
            }

            if(c_block->name.compare("backsurface")==0)
                mat->backsurface = surface;
            else
                mat->surface = surface;
        }
    }

}

void getMaterial(an8_object* obj, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("material")==0)
        {
            an8_material mat;
            mat.name = c_block->obj_name;
            getSurface(&mat, c_block);
            obj->material.push_back(mat);
        }
    }

}

void getMaterial(an8_figure* figure, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("material")==0)
        {
            an8_material mat;
            mat.name = c_block->obj_name;
            getSurface(&mat, c_block);
            figure->material.push_back(mat);
        }
    }

}

void getBase(an8_base* base, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;

    base->origin.x = 0;
    base->origin.y = 0;
    base->origin.z = 0;
    base->orientation.x = 0;
    base->orientation.y = 0;
    base->orientation.z = 0;
    base->orientation.w = 0;

    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("origin")==0)
        {
            std::string token = "";
            double v_point[3];
            v_point[0] = 0;
            v_point[1] = 0;
            v_point[2] = 0;
            int vi = 0;
            std::string c_value = c_block->value + " ";

            for(int n = 0; n < c_value.length(); n++)
            {
                std::string c = c_value.substr(n, 1);

                if(c.compare("(")==0 || c.compare(")")==0 )
                    continue;
                else if(c.compare(" ")==0)
                {
                    v_point[vi] = atof(token.c_str());
                    vi++;
                    token = "";

                    if(vi >= 3)
                        break;
                }
                else
                    token += c;
            }

            base->origin.x = v_point[0];
            base->origin.y = v_point[1];
            base->origin.z = v_point[2];
        }
        else if(c_block->name.compare("orientation")==0)
        {
            std::string token = "";
            double v_point[4];
            v_point[0] = 0;
            v_point[1] = 0;
            v_point[2] = 0;
            v_point[3] = 0;
            int vi = 0;
            std::string c_value = c_block->value + " ";

            for(int n = 0; n < c_value.length(); n++)
            {
                std::string c = c_value.substr(n, 1);

                if(c.compare("(")==0 || c.compare(")")==0 )
                    continue;
                else if(c.compare(" ")==0)
                {
                    v_point[vi] = atof(token.c_str());
                    vi++;
                    token = "";

                    if(vi >= 4)
                        break;
                }
                else
                    token += c;
            }

            base->orientation.x = v_point[0];
            base->orientation.y = v_point[1];
            base->orientation.z = v_point[2];
            base->orientation.w = v_point[3];
        }
    }

}


void getMaterialList(std::vector<std::string>* mat_list, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("materialname")==0)
        {
            #ifdef AN8_DEBUG
            //cout << "material list add -> " << c_block->obj_name << endl;
            #endif // AN8_DEBUG
            mat_list->push_back(c_block->obj_name);
        }
    }

}

void getPoints3f(std::vector<an8_point3f>* points, an8_file_block* block)
{
    double v[3];
    int vi = 0;
    std::string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        std::string c = block->value.substr(i,1);

        if(c.compare("(")==0)
        {
            in_scope = true;
            arg = "";
            vi = 0;
            v[0] = 0;
            v[1] = 0;
            v[2] = 0;
        }
        else if(c.compare(")")==0)
        {
            in_scope = false;

            if(vi < 3)
            {
                v[2] = atof(arg.c_str());
            }

            an8_point3f p;
            p.x = v[0];
            p.y = v[1];
            p.z = v[2];
            points->push_back(p);
        }
        else if(c.compare(" ")==0 && in_scope)
        {
            v[vi] = atof(arg.c_str());
            arg = "";
            vi++;
        }
        else
            arg += c;
    }

}

void getPoints4f(std::vector<an8_point4f>* points, an8_file_block* block)
{
    double v[4];
    int vi = 0;
    std::string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        std::string c = block->value.substr(i,1);

        if(c.compare("(")==0)
        {
            in_scope = true;
            arg = "";
            vi = 0;
            v[0] = 0;
            v[1] = 0;
            v[2] = 0;
            v[3] = 0;
        }
        else if(c.compare(")")==0)
        {
            in_scope = false;

            if(vi < 4)
            {
                v[3] = atof(arg.c_str());
            }

            an8_point4f p;
            p.x = v[0];
            p.y = v[1];
            p.z = v[2];
            p.w = v[3];
            points->push_back(p);
        }
        else if(c.compare(" ")==0 && in_scope)
        {
            v[vi] = atof(arg.c_str());
            arg = "";
            vi++;
        }
        else
            arg += c;
    }

}

void getEdges(std::vector<an8_edge>* edges, an8_file_block* block)
{
    uint32_t v[3];
    int vi = 0;
    std::string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        std::string c = block->value.substr(i,1);

        if(c.compare("(")==0)
        {
            in_scope = true;
            arg = "";
            vi = 0;
            v[0] = 0;
            v[1] = 0;
            v[2] = -1; //default edge sharpness

        }
        else if(c.compare(")")==0)
        {
            in_scope = false;

            v[vi] = atoi(arg.c_str());

            an8_edge e;
            e.point_index1 = v[0];
            e.point_index2 = v[1];
            e.sharpness = v[2];
            edges->push_back(e);

            arg = "";
        }
        else if(c.compare(" ")==0 && in_scope)
        {
            v[vi] = atoi(arg.c_str());
            arg = "";
            vi++;
        }
        else
            arg += c;

    }

}

void getUV(std::vector<an8_uv>* texcoords, an8_file_block* block)
{
    double v[2];
    int vi = 0;
    std::string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        std::string c = block->value.substr(i,1);

        if(c.compare("(")==0)
        {
            in_scope = true;
            arg = "";
            vi = 0;
            v[0] = 0;
            v[1] = 0;
        }
        else if(c.compare(")")==0)
        {
            in_scope = false;

            if(vi < 2)
            {
                v[1] = atof(arg.c_str());
            }

            an8_uv t;
            t.u = v[0];
            t.v = v[1];
            texcoords->push_back(t);
        }
        else if(c.compare(" ")==0 && in_scope)
        {
            v[vi] = atof(arg.c_str());
            arg = "";
            vi++;
        }
        else
            arg += c;
    }

}

void getPoints2i(std::vector<an8_point2i>* points, an8_file_block* block)
{
    int32_t v[2];
    int vi = 0;
    std::string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        std::string c = block->value.substr(i,1);

        if(c.compare("(")==0)
        {
            in_scope = true;
            arg = "";
            vi = 0;
            v[0] = 0;
            v[1] = 0;
        }
        else if(c.compare(")")==0)
        {
            in_scope = false;

            if(vi < 2)
            {
                v[1] = atoi(arg.c_str());
            }

            an8_point2i p;
            p.x = v[0];
            p.y = v[1];
            points->push_back(p);
        }
        else if(c.compare(" ")==0 && in_scope)
        {
            v[vi] = atoi(arg.c_str());
            arg = "";
            vi++;
        }
        else
            arg += c;
    }

}

void getFaces(std::vector<an8_facedata>* faces, an8_file_block* block)
{
    int v[4];

    int f_data[4];
    std::vector<an8_pointdata> point_data;

    int vi = 0;
    int fi = 0;
    std::string arg = "";

    bool in_pdata = false;
    bool in_scope = false;

    bool normal_flag = false;
    bool texture_flag = false;

    bool flag_set = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        std::string c = block->value.substr(i,1);

        if(c.compare("(")==0)
        {
            if(in_pdata)
            {
                in_scope = true;
                arg = "";
                vi = 0;
                v[0] = 0;
                v[1] = 0;
                v[2] = 0;
            }
            else
                in_pdata = true;
        }
        else if(c.compare(")")==0)
        {
            if(in_scope)
            {
                in_scope = false;

                //f_data[1] (flag bitmask) should have been set by now
                if(!flag_set)
                {
                    normal_flag = (f_data[1] >> 1);
                    texture_flag = (f_data[1] >> 2);
                    flag_set = true;
                }

                an8_pointdata p;

                if(i > 0)
                {
                    if(block->value.substr(i-1,1).compare(" ")!=0) //if the last character was not space then we need to read the last arg
                    {
                        v[vi] = atoi(arg.c_str());
                        vi++;
                    }

                }

                //an8_pointdata p;
                p.point_index = v[0];

                if(vi < 3)
                {
                    if(normal_flag)
                    {
                        p.normal_index = v[1];
                        p.texture_index = -1;
                    }
                    else
                    {
                        p.texture_index = v[1];
                        p.normal_index = -1;
                    }
                }
                else
                {
                    p.normal_index = v[1];
                    p.texture_index = v[2];
                }

                point_data.push_back(p);
            }
            else
            {
                in_pdata = false;
                fi = 0;
                vi = 0;

                an8_facedata face;
                face.num_points = f_data[0];
                face.flags = f_data[1];
                face.material_index = f_data[2];
                face.flat_normal_index = f_data[3];

                for(int fp = 0; fp < point_data.size(); fp++)
                    face.point_data.push_back(point_data[fp]);


                #ifdef AN8_DEBUG
                /*
                if(faces->size() < 5)
                {
                    cout << "Face [" << faces->size() << "] = " <<
                            (int)face.num_points << ", " <<
                            (int)face.flags << ", " <<
                            (int)face.material_index << ", " <<
                            (int)face.flat_normal_index << ", ( ";

                    for(int fp = 0; fp < face.point_data.size(); fp++)
                        cout << "(" << (int)face.point_data[fp].point_index << " " <<
                                       (int)face.point_data[fp].mt_index << ") ";

                    cout << ")" << endl;
                }
                */
                #endif // AN8_DEBUG

                faces->push_back(face);

                point_data.clear();
                arg = "";
            }
        }
        else if(c.compare(" ")==0 && trim(arg).compare("")!=0)
        {
            if(in_pdata && in_scope)
            {
                v[vi] = atoi(arg.c_str());
                arg = "";
                vi++;
            }
            else if(!in_pdata)
            {
                if(fi >= 4)
                    continue;

                f_data[fi] = atoi(arg.c_str());
                arg = "";
                fi++;
            }
        }
        else
            arg += c;
    }

}

void getMesh(an8_mesh* mesh, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("name")==0)
        {
            mesh->name = c_block->obj_name;
        }
        else if(c_block->name.compare("base")==0)
        {
            an8_base base;
            getBase(&base, c_block);
            mesh->base = base;
        }
        else if(c_block->name.compare("pivot")==0)
        {
            an8_base base;
            getBase(&base, c_block);
            mesh->pivot = base;
        }
        else if(c_block->name.compare("material")==0)
        {
            an8_material mat;
            mat.name = c_block->obj_name;
            getSurface(&mat, c_block);
            mesh->material = mat;
        }
        else if(c_block->name.compare("smoothangle")==0)
        {
            mesh->smoothangle = atof(c_block->value.c_str());
        }
        else if(c_block->name.compare("materiallist")==0)
        {
            getMaterialList(&mesh->materialname, c_block);
        }
        else if(c_block->name.compare("points")==0)
        {
            getPoints3f(&mesh->points, c_block);
        }
        else if(c_block->name.compare("normals")==0)
        {
            getPoints3f(&mesh->normals, c_block);
        }
        else if(c_block->name.compare("edges")==0)
        {
            getEdges(&mesh->edges, c_block);
        }
        else if(c_block->name.compare("texcoords")==0)
        {
            getUV(&mesh->texcoords, c_block);
        }
        else if(c_block->name.compare("faces")==0)
        {
            getFaces(&mesh->faces, c_block);
        }
    }

}

void getWeights(an8_meshWeights* meshWeight, an8_file_block* block)
{
    std::string value = block->value;
    meshWeight->name = block->obj_name;

    //cout << "Name: " << meshWeight->name << endl;
    //cout << "GET Weights: " << value << endl << endl;

    int p_scope = 0;
    std::string vertex_value = "";

    for(int i = 0; i < value.length(); i++)
    {
        std::string c = value.substr(i,1);

        if(c.compare("(")==0)
        {
            p_scope++;
        }
        else if(c.compare(")")==0)
        {
            p_scope--;
        }
        else if(p_scope > 0)
        {
            vertex_value += c;
        }

        if(p_scope==0 && vertex_value.length() > 0)
        {
            vertex_value = str_replace(vertex_value, "(", " ");
            vertex_value = str_replace(vertex_value, ")", " ");
            vertex_value += " ";

            int numVertices = 0;
            an8_boneWeight bone_weight;
            an8_vertexWeight vert_weight;

            int arg_num = 0;
            std::string arg = "";

            for(int n = 0; n < vertex_value.length(); n++)
            {
                c = vertex_value.substr(n, 1);

                if(c.compare(" ") == 0)
                {
                    if(arg.length()==0)
                        continue;

                    if(arg_num==0)
                        numVertices = atoi(arg.c_str());
                    else if( (arg_num % 2) > 0)
                        bone_weight.bone_index = atoi(arg.c_str());
                    else
                    {
                        bone_weight.weight = atof(arg.c_str());
                        vert_weight.boneWeight.push_back(bone_weight);
                    }

                    arg = "";
                    arg_num++;
                }
                else
                    arg += c;

            }

            int n_index = meshWeight->vertexWeight.size();
            meshWeight->vertexWeight.push_back(vert_weight);

            /*
            if(meshWeight->name.compare("mesh01x")==0)
            {
                cout << "---------Vertex: " << n_index << " --> " << vertex_value << endl << endl;
                for(int n = 0; n < meshWeight->vertexWeight[n_index].boneWeight.size(); n++)
                {
                    cout << "bone: " << meshWeight->vertexWeight[n_index].boneWeight[n].bone_index << endl;
                    cout << "weight: " << meshWeight->vertexWeight[n_index].boneWeight[n].weight << endl << endl;
                }
            }
            */
            vertex_value = "";
        }
    }

}

void getNamedObject(an8_namedobject* named_object, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;

    named_object->name = block->obj_name;

    named_object->material.name = "";

    named_object->base.origin.x = 0;
    named_object->base.origin.y = 0;
    named_object->base.origin.z = 0;
    named_object->base.orientation.x = 0;
    named_object->base.orientation.y = 0;
    named_object->base.orientation.z = 0;
    named_object->base.orientation.w = 0;

    named_object->pivot.origin.x = 0;
    named_object->pivot.origin.y = 0;
    named_object->pivot.origin.z = 0;
    named_object->pivot.orientation.x = 0;
    named_object->pivot.orientation.y = 0;
    named_object->pivot.orientation.z = 0;
    named_object->pivot.orientation.w = 0;

    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("name")==0)
        {
            named_object->name_block.name = c_block->obj_name;
        }
        else if(c_block->name.compare("base")==0)
        {
            an8_base base;
            getBase(&base, c_block);
            named_object->base = base;
        }
        else if(c_block->name.compare("pivot")==0)
        {
            an8_base base;
            getBase(&base, c_block);
            named_object->pivot = base;
        }
        else if(c_block->name.compare("material")==0)
        {
            an8_material mat;
            mat.name = c_block->obj_name;
            getSurface(&mat, c_block);
            named_object->material = mat;
        }
        else if(c_block->name.compare("weightedby")==0)
        {
            an8_weightedBy wb;
            wb.bone_name = c_block->obj_name;
            named_object->weightedBy.push_back(wb);
        }
        else if(c_block->name.compare("weights")==0)
        {
            an8_meshWeights weights;
            getWeights(&weights, c_block);
        }
    }

}

void getComponent(std::vector<an8_component>* component_list, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("mesh")==0)
        {
            an8_mesh mesh;
            mesh.base.origin.x = 0;
            mesh.base.origin.y = 0;
            mesh.base.origin.z = 0;
            mesh.base.orientation.x = 0;
            mesh.base.orientation.y = 0;
            mesh.base.orientation.z = 0;
            mesh.base.orientation.w = 0;

            getMesh(&mesh, c_block);

            an8_component component;
            component.mesh = mesh;
            component.type = AN8_COMPONENT_TYPE_MESH;

            component_list->push_back(component);
        }
        else if(c_block->name.compare("namedobject")==0)
        {
            an8_namedobject named_object;

            getNamedObject(&named_object, c_block);

            #ifdef AN8_DEBUG
            cout << "--NAMED OBJECT--" << endl;
            cout << "Obj Name: " << named_object.name << endl;
            cout << "Name Block: " << named_object.name_block.name << endl;
            cout << "Base: " << named_object.base.origin.x << ", "
                              << named_object.base.origin.y << ", "
                              << named_object.base.origin.z << endl;
            cout << "Pivot: " << named_object.pivot.origin.x << ", "
                              << named_object.pivot.origin.y << ", "
                              << named_object.pivot.origin.z << endl;
            cout << "Material: " << named_object.material.name << endl;

            for(int wb_index = 0; wb_index < named_object.weightedBy.size(); wb_index++)
            {
                cout << "Weighted By: " << named_object.weightedBy[wb_index].bone_name << endl;
            }

            cout << endl << endl;
            #endif // AN8_DEBUG

            an8_component component;
            component.named_object = named_object;
            component.type = AN8_COMPONENT_TYPE_NAMEDOBJECT;

            component_list->push_back(component);


        }
    }

}

void getObject(an8_project* project, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("object")==0)
        {
            an8_object obj;
            obj.name = c_block->obj_name;
            getMaterial(&obj, c_block);
            getComponent(&obj.component, c_block);
            project->objects.push_back(obj);
        }
    }

}

void getDOF(std::vector<an8_dof>* dof, an8_file_block* block)
{
    int vi = 0;
    std::string arg = "";


    an8_dof tmp_dof;
    tmp_dof.axis = block->obj_name;
    tmp_dof.default_angle = 0;
    tmp_dof.locked = false;
    tmp_dof.max_angle = 0;
    tmp_dof.min_angle = 0;
    tmp_dof.unlimited = false;

    std::string value2 = block->value2 + " ";

    for(int i = 0; i < value2.length(); i++)
    {
        std::string c = value2.substr(i,1);

        if(c.compare(" ")==0 && arg.compare("") != 0)
        {
            if(arg.compare("unlimited")==0)
                tmp_dof.unlimited = true;
            else if(arg.compare("locked")==0)
                tmp_dof.locked = true;
            else
            {
                switch(vi)
                {
                    case 0:
                        tmp_dof.min_angle = atof(arg.c_str());
                        break;
                    case 1:
                        tmp_dof.default_angle = atof(arg.c_str());
                        break;
                    case 2:
                        tmp_dof.max_angle = atof(arg.c_str());
                        break;
                }

                vi++;
            }
            arg = "";
        }
        else if(c.compare(" ") != 0)
            arg += c;
    }

    dof->push_back(tmp_dof);

}

void getInfluence(an8_influence* inf, an8_file_block* block)
{
    double v[6];
    int vi = 0;
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    v[3] = 0;
    v[4] = 0;
    v[5] = 0;
    std::string arg = "";
    std::string b_value = trim(block->value) + " ;";

    for(int i = 0; i < b_value.length(); i++)
    {
        std::string c = b_value.substr(i,1);

        if(c.compare(";")==0)
        {
            inf->center0 = v[0];
            inf->inRadius0 = v[1];
            inf->outRadius0 = v[2];
            inf->center1 = v[3];
            inf->inRadius1 = v[4];
            inf->outRadius1 = v[5];
            break;
        }
        else if(c.compare(" ")==0)
        {
            v[vi] = atof(arg.c_str());
            arg = "";
            vi++;
        }
        else
            arg += c;
    }

}

void getBone(an8_bone* bone, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;

    bone->name = block->obj_name;

    #ifdef AN8_DEBUG
    cout << endl << "BONE [" << bone->name << "]" << endl;
    #endif // AN8_DEBUG

    an8_base base;
    getBase(&base, block);
    bone->orientation = base.orientation;

    getComponent(&bone->component, block);

    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("bone")==0)
        {
            an8_bone sub_bone;
            getBone(&sub_bone, c_block);
            bone->bone.push_back(sub_bone);
        }
        else if(c_block->name.compare("length")==0)
        {
            bone->length = atof(c_block->value.c_str());
            #ifdef AN8_DEBUG
            cout << "length: " << bone->length << endl;
            #endif // AN8_DEBUG
        }
        else if(c_block->name.compare("diameter")==0)
        {
            bone->diameter = atof(c_block->obj_name.c_str());
        }
        else if(c_block->name.compare("locked")==0)
        {
            bone->locked = true;
        }
        else if(c_block->name.compare("dof")==0)
        {
            getDOF(&bone->dof, c_block);

            an8_dof dof = bone->dof[bone->dof.size()-1];
            #ifdef AN8_DEBUG
            cout << "-->DOF: " << dof.axis << " "
                 << dof.min_angle << " "
                 << dof.default_angle << " "
                 << dof.max_angle << " "
                 << (dof.locked ? "true" : "false" ) << " "
                 << (dof.unlimited ? "true" : "false") << endl;
            #endif // AN8_DEBUG
        }
        else if(c_block->name.compare("influence")==0)
        {
            getInfluence(&bone->influence, c_block);

            an8_influence inf = bone->influence;
            #ifdef AN8_DEBUG
            cout << "-->INFLUENCE = " << inf.center0 << ", "
                                      << inf.inRadius0 << ", "
                                      << inf.outRadius0 << ", "
                                      << inf.center1 << ", "
                                      << inf.inRadius1 << ", "
                                      << inf.outRadius1 << endl;
            #endif // AN8_DEBUG
        }
    }

}

void getFigure(an8_project* project, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("figure")==0)
        {
            #ifdef AN8_DEBUG
            //cout << "figure_name = " << c_block->obj_name << endl;
            #endif // AN8_DEBUG

            an8_figure figure;
            figure.name = c_block->obj_name;
            getMaterial(&figure, c_block);

            an8_bone root_bone;
            an8_file_block* c2_block;

            for(int i = 0; i < c_block->block.size(); i++)
            {
                c2_block = &c_block->block[i];
                if(c2_block->name.compare("bone")==0)
                {
                    getBone(&root_bone, c2_block);
                    break;
                }
            }

            figure.bone = root_bone;

            project->figures.push_back(figure);

        }

    }

}

void getFloatKey(an8_floatkey* key, an8_file_block* block)
{

    an8_file_block* c_block;

    std::string value = trim(block->value) + " ";

    int key_index = 0;

    std::string c_arg = "";

    int m_start = block->value3.find_first_of("\"") + 1;
    int m_end = block->value3.substr(m_start).find_first_of("\"")+1;

    if(m_start != std::string::npos && m_end != std::string::npos)
        key->mod_string = block->value3.substr(m_start, m_end-m_start);
    else
        key->mod_string = "";

    for(int i = 0; i < value.length(); i++)
    {
        std::string c = value.substr(i,1);

        if(c.compare(" ")==0)
        {
            switch(key_index)
            {
                case 0:
                    key->frame_index = atoi(c_arg.c_str());
                    break;
                case 1:
                    key->value = atof(c_arg.c_str());
                    break;
            }
            c_arg = "";
            key_index++;
        }
        else
            c_arg += c;
    }

}

void getPointKey(an8_pointkey* key, an8_file_block* block)
{

    an8_file_block* c_block;

    std::string value = trim(block->value) + " ";

    int key_index = 0;

    std::string c_arg = "";

    int m_start = block->value3.find_first_of("\"") + 1;
    int m_end = block->value3.substr(m_start).find_first_of("\"")+1;

    if(m_start != std::string::npos && m_end != std::string::npos)
        key->mod_string = block->value3.substr(m_start, m_end-m_start);
    else
        key->mod_string = "";

    std::vector<an8_point3f> points;
    getPoints3f(&points, block);

    if(points.size() >= 3)
    {
        key->value = points[0];
        key->forward_vector = points[1];
        key->reverse_vector = points[2];
    }


    for(int i = 0; i < value.length(); i++)
    {
        std::string c = value.substr(i,1);

        if(c.compare(" ")==0)
        {
            key->frame_index = atoi(c_arg.c_str());
            break;
        }
        else
            c_arg += c;
    }

}

void getQKey(an8_qkey* key, an8_file_block* block)
{

    an8_file_block* c_block;

    std::string value = trim(block->value) + " ";

    int key_index = 0;

    std::string c_arg = "";

    int m_start = block->value3.find_first_of("\"") + 1;
    int m_end = block->value3.substr(m_start).find_first_of("\"")+1;

    if(m_start != std::string::npos && m_end != std::string::npos)
        key->mod_string = block->value3.substr(m_start, m_end-m_start);
    else
        key->mod_string = "";

    std::vector<an8_point4f> points;
    getPoints4f(&points, block);

    if(points.size() >= 1)
    {
        key->value = points[0];
    }


    for(int i = 0; i < value.length(); i++)
    {
        std::string c = value.substr(i,1);

        if(c.compare(" ")==0)
        {
            key->frame_index = atoi(c_arg.c_str());
            break;
        }
        else
            c_arg += c;
    }

}

void getBooleanKey(an8_booleankey* key, an8_file_block* block)
{

    an8_file_block* c_block;


    std::string value = trim(block->value) + " ";
    //cout << "DBG BOOLEAN: " << value << endl;

    int key_index = 0;

    std::string c_arg = "";

    int m_start = block->value3.find_first_of("\"") + 1;
    int m_end = block->value3.substr(m_start).find_first_of("\"")+1;

    if(m_start != std::string::npos && m_end != std::string::npos)
        key->mod_string = block->value3.substr(m_start, m_end-m_start);
    else
        key->mod_string = "";

    for(int i = 0; i < value.length(); i++)
    {
        std::string c = value.substr(i,1);

        if(c.compare(" ")==0)
        {
            switch(key_index)
            {
                case 0:
                    key->frame_index = atoi(c_arg.c_str());
                    break;
                case 1:
                    //cout << "debug bool = " << c_arg << endl;
                    key->value = atoi(c_arg.c_str()) == 0 ? false : true;
                    break;
            }
            c_arg = "";
            key_index++;
        }
        else
            c_arg += c;
    }

}

void getTrack(an8_track* track, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;

    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("floatkey")==0)
        {
            //cout << "FLOAT_KEY" << endl;
            an8_floatkey key;
            getFloatKey(&key, c_block);
            //cout << "key frame = " << key.frame_index << endl;
            //cout << "key value = " << key.value << endl;
            //cout << "key mod = " << key.mod_string << endl;

            track->track_type = TRACK_TYPE_FLOAT;
            track->floatkey.push_back(key);
            //cout << "Float Key = " << c_block->value << endl;
            //named_object->name_block.name = c_block->obj_name;
        }
        else if(c_block->name.compare("pointkey")==0)
        {
            //cout << "POINT_KEY" << endl;
            an8_pointkey key;
            getPointKey(&key, c_block);
            //cout << "key frame = " << key.frame_index << endl;
            //cout << "key value = " << key.value.x << ", " << key.value.y << ", " << key.value.z << endl;
            //cout << "key forward = " << key.forward_vector.x << ", " << key.forward_vector.y << ", " << key.forward_vector.z << endl;
            //cout << "key reverse = " << key.reverse_vector.x << ", " << key.reverse_vector.y << ", " << key.reverse_vector.z << endl;
            //cout << "key mod = " << key.mod_string << endl;

            track->track_type = TRACK_TYPE_POINT;
            track->pointkey.push_back(key);
            //cout << "Float Key = " << c_block->value << endl;
            //named_object->name_block.name = c_block->obj_name;
        }
        else if(c_block->name.compare("qkey")==0)
        {
            //cout << "Q_KEY" << endl;
            an8_qkey key;
            getQKey(&key, c_block);
            //cout << "key frame = " << key.frame_index << endl;
            //cout << "key value = " << key.value.x << ", " << key.value.y << ", " << key.value.z << endl;
            //cout << "key mod = " << key.mod_string << endl;

            track->track_type = TRACK_TYPE_QUATERNION;
            track->qkey.push_back(key);
            //cout << "Float Key = " << c_block->value << endl;
            //named_object->name_block.name = c_block->obj_name;
        }
        else if(c_block->name.compare("booleankey")==0)
        {
            //cout << "BOOLEAN_KEY" << endl;
            an8_booleankey key;
            getBooleanKey(&key, c_block);
            //cout << "key frame = " << key.frame_index << endl;
            //cout << "key value = " << (key.value ? "true" : "false") << endl;
            //cout << "key mod = " << key.mod_string << endl;

            track->track_type = TRACK_TYPE_BOOLEAN;
            track->booleankey.push_back(key);
            //cout << "Float Key = " << c_block->value << endl;
            //named_object->name_block.name = c_block->obj_name;
        }
    }

}

void getJointAngle(an8_jointangle* jointangle, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;

    std::string ja_val3 = block->value3;

    int bn_start = ja_val3.find_first_of("\"") + 1;
    int bn_end = ja_val3.substr(bn_start).find_first_of("\"")+1;

    int axis_start = bn_end + ja_val3.substr(bn_end+1).find_first_of("\"")+2;
    int axis_end = axis_start + ja_val3.substr(axis_start).find_first_of("\"");

    jointangle->bone_name = ja_val3.substr(bn_start, bn_end-bn_start);
    jointangle->axis = ja_val3.substr(axis_start, axis_end-axis_start);

    //cout << "VAL3 = " << ja_val3 << endl;
    //cout << "bone name =[" << jointangle->bone_name << "]" << endl;
    //cout << "axis =[" << jointangle->axis << "]" << endl;


    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("track")==0)
        {
            //cout << "GET TRACK" << endl;
            getTrack(&jointangle->track, c_block);

            //named_object->name_block.name = c_block->obj_name;
        }
    }

}

void getSequence(an8_project* project, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("sequence")==0)
        {
            an8_sequence sequence;
            sequence.name = c_block->obj_name;

            #ifdef AN8_DEBUG
            //cout << "sequence_name = " << sequence.name << endl;
            #endif // AN8_DEBUG

            an8_file_block* c2_block;

            for(int n = 0; n < c_block->block.size(); n++)
            {
                c2_block = &c_block->block[n];
                if(c2_block->name.compare("figure")==0)
                {
                    sequence.figure_name = c2_block->obj_name;
                    //cout << "figure_name = " << sequence.figure_name << endl;

                }
                else if(c2_block->name.compare("frames")==0)
                {
                    sequence.num_frames = atoi(c2_block->value.c_str());
                    //cout << "frame_count = " << sequence.num_frames << endl;

                }
                else if(c2_block->name.compare("jointangle")==0)
                {
                    an8_jointangle jointangle;

                    getJointAngle(&jointangle, c2_block);

                    //jointangle.bone_name = c2_block->value3;
                    //cout << "jointangle_val3 = " << jointangle.bone_name << endl;


                    sequence.jointangle.push_back(jointangle);
                }
                else
                {
                    //cout << "data obj (" << c2_block->name << ")" << endl;
                }
            }

            project->sequences.push_back(sequence);

        }

    }

}

void getController(an8_controller* controller, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;

    controller->name = block->obj_name;

    //cout << "Controller Name: " << controller->name << endl;


    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("track")==0)
        {
            //cout << "GET TRACK" << endl;
            getTrack(&controller->track, c_block);

            //named_object->name_block.name = c_block->obj_name;
        }
    }

}

void getNamedSequence(an8_namedsequence* namedsequence, an8_file_block* block)
{

    an8_file_block* c_block;


    std::string value = trim(block->value) + " ";
    int key_index = 0;

    std::string c_arg = "";

    namedsequence->name = block->obj_name;

    //cout << "Named Sequence: " << namedsequence->name << endl;

    for(int i = 0; i < value.length(); i++)
    {
        std::string c = value.substr(i,1);

        if(c.compare(" ")==0)
        {
            namedsequence->frame_num = atoi(c_arg.c_str());
            //cout << "NS Frame Num: " << namedsequence->frame_num << endl;

            c_arg = "";
            key_index++;
        }
        else
            c_arg += c;
    }

}

void getElement(an8_element* element, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;

    std::string element_val3 = block->value3;

    int name_start = element_val3.find_first_of("\"") + 1;
    int name_end = element_val3.substr(name_start).find_first_of("\"")+1;

    element->name = element_val3.substr(name_start, name_end-name_start);

    if(block->name.compare("figureelement") == 0)
        element->element_type = ELEMENT_TYPE_FIGURE;
    else if(block->name.compare("objectelement") == 0)
        element->element_type = ELEMENT_TYPE_OBJECT;
    else if(block->name.compare("camera") == 0)
        element->element_type = ELEMENT_TYPE_CAMERA;
    else if(block->name.compare("light") == 0)
        element->element_type = ELEMENT_TYPE_LIGHT;
    else
        element->element_type = ELEMENT_TYPE_NULL;

    element->obj_name = "";
    switch(element->element_type)
    {
        case ELEMENT_TYPE_FIGURE:
        case ELEMENT_TYPE_OBJECT:
            int obj_start = name_end + element_val3.substr(name_end+1).find_first_of("\"")+2;
            int obj_end = obj_start + element_val3.substr(obj_start).find_first_of("\"");
            element->obj_name = element_val3.substr(obj_start, obj_end-obj_start);

            //cout << "DEBUG: " << element->name << ", " << element->obj_name << endl;
            break;
    }

    element->bound_target = "";
    element->cast_shadow = false;
    element->fov = 0;
    element->in_angle = 0;
    element->in_radius = 0;
    element->locked = false;
    element->max_samples = 0;
    element->min_samples = 0;
    element->monte_carlo = false;
    element->out_angle = 0;
    element->out_radius = 0;
    element->raytrace_shadow = false;
    element->receive_shadow = false;
    element->roll = false;
    element->scale = 1;
    element->soft_edges = false;
    element->soft_size = 0;
    element->visible = true;

    //cout << "VAL3 = " << element_val3 << endl;
    //cout << "element_name =[" << element->name << "]" << endl;
    //cout << "element_obj_name =[" << element->obj_name << "]" << endl;
    //cout << "element type = " << element->element_type << endl;


    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];

        if(c_block->name.compare("loc")==0)
        {
            std::vector<an8_point3f> points;
            getPoints3f(&points, c_block);

            if(points.size() > 0)
                element->loc = points[0];
            else
                std::cout << "Error getting element location" << std::endl;

            //cout << "GET Location: " << points[0].x << ", " << points[0].y << ", " << points[0].z << endl;
        }
        else if(c_block->name.compare("orientation")==0)
        {
            std::vector<an8_point4f> points;
            getPoints4f(&points, c_block);

            if(points.size() > 0)
                element->orientation = points[0];
            else
                std::cout << "Error getting element orientation" << std::endl;

            //cout << "GET Orientation: " << points[0].x << ", " << points[0].y << ", " << points[0].z << ", " << points[0].w << endl;

        }
        else if(c_block->name.compare("roll")==0)
        {
            element->roll = (atoi(c_block->value.c_str())==0) ? false : true;
            //cout << "GET roll: " << (element->roll ? "true" : "false") << endl;
        }
        else if(c_block->name.compare("facespath")==0)
        {
            element->orient_relative_to = ORIENT_RELATIVE_TO_FACESPATH;
            //cout << "GET relative orient: " << c_block->name << endl;
        }
        else if(c_block->name.compare("facestarget")==0)
        {
            element->orient_relative_to = ORIENT_RELATIVE_TO_FACESTARGET;
            //cout << "GET relative orient: " << c_block->name << endl;
        }
        else if(c_block->name.compare("orienttarget")==0)
        {
            element->orient_relative_to = ORIENT_RELATIVE_TO_ORIENTTARGET;
            //cout << "GET relative orient: " << c_block->name << endl;
        }
        else if(c_block->name.compare("boundtarget")==0)
        {
            element->bound_target = c_block->obj_name;
            //cout << "GET Bound Target: " << element->bound_target << endl;
        }
        else if(c_block->name.compare("scale")==0)
        {
            element->scale = atof(c_block->value.c_str());
            //cout << "GET scale: " << element->scale << endl;
        }
        else if(c_block->name.compare("locked")==0)
        {
            element->locked = atoi(c_block->value.c_str()) == 0 ? false : true;
            //cout << "GET locked: " << element->locked << endl;
        }
        else if(c_block->name.compare("controller")==0)
        {
            an8_controller controller;
            getController(&controller, c_block);
            element->controller.push_back(controller);
        }
        else if(c_block->name.compare("camera")==0 ||
                        c_block->name.compare("figureelement")==0 ||
                        c_block->name.compare("objectelement")==0 ||
                        c_block->name.compare("light")==0 ||
                        c_block->name.compare("null")==0)
        {
            //cout << "GET CHILD" << endl;
            an8_element child_element;
            getElement(&child_element, c_block);
            element->element.push_back(child_element);
        }
        else if(c_block->name.compare("namedsequence")==0)
        {
            an8_namedsequence namedsequence;
            getNamedSequence(&namedsequence, c_block);
            element->named_sequence.push_back(namedsequence);
        }
        else if(c_block->name.compare("fov")==0)
        {
            element->fov = atof(c_block->value.c_str());
            //cout << "FOV: " << element->fov << endl;
        }
        else if(c_block->name.compare("color")==0)
        {
            std::vector<an8_point3f> point;
            getPoints3f(&point, c_block);
            if(point.size() > 0)
            {
                element->color.r = (uint8_t)point[0].x;
                element->color.g = (uint8_t)point[0].y;
                element->color.b = (uint8_t)point[0].z;
            }
            //cout << "GET COLOR: " << (uint32_t)element->color.r << ", " << (uint32_t)element->color.g << ", " << (uint32_t)element->color.b << endl;
        }
        else if(c_block->name.compare("infinite")==0)
        {
            element->light_type = LIGHT_TYPE_INFINITE;
        }
        else if(c_block->name.compare("local")==0)
        {
            element->light_type = LIGHT_TYPE_LOCAL;
        }
        else if(c_block->name.compare("spotlight")==0)
        {
            element->light_type = LIGHT_TYPE_SPOT;
        }
        else if(c_block->name.compare("inradius")==0)
        {
            element->in_radius = atof(c_block->value.c_str());
        }
        else if(c_block->name.compare("outradius")==0)
        {
            element->out_radius = atof(c_block->value.c_str());
        }
        else if(c_block->name.compare("inangle")==0)
        {
            element->in_angle = atof(c_block->value.c_str());
        }
        else if(c_block->name.compare("outangle")==0)
        {
            element->out_angle = atof(c_block->value.c_str());
        }
        else if(c_block->name.compare("castshadow")==0)
        {
            element->cast_shadow = true;
        }
        else if(c_block->name.compare("raytraceshadow")==0)
        {
            element->raytrace_shadow = true;
        }
        else if(c_block->name.compare("soft")==0)
        {
            element->soft_edges = true;
        }
        else if(c_block->name.compare("softsize")==0)
        {
            element->soft_size = atof(c_block->value.c_str());
        }
        else if(c_block->name.compare("minsamples")==0)
        {
            element->min_samples = atoi(c_block->value.c_str());
        }
        else if(c_block->name.compare("maxsamples")==0)
        {
            element->max_samples = atoi(c_block->value.c_str());
        }
        else if(c_block->name.compare("montecarlo")==0)
        {
            element->monte_carlo = true;
        }
        else if(c_block->name.compare("visibility")==0)
        {
            element->visible = atoi(c_block->value.c_str()) == 0 ? false : true;
        }
        else if(c_block->name.compare("castshadow")==0)
        {
            element->cast_shadow = true;
        }
        else if(c_block->name.compare("receiveshadow")==0)
        {
            element->receive_shadow = true;
        }
        else
        {
            //cout << "element obj(" << c_block->name << ")" << endl;
        }
    }

    //cout << "DEBUG2: " << element->name << ", " << element->obj_name << endl;

}

void getScene(an8_project* project, an8_file_block* block)
{
    if(block->block.size() == 0)
        return;

    an8_file_block* c_block;
    for(int i = 0; i < block->block.size(); i++)
    {
        c_block = &block->block[i];
        if(c_block->name.compare("scene")==0)
        {
            an8_scene scene;
            scene.name = c_block->obj_name;

            #ifdef AN8_DEBUG
            //cout << "scene_name = " << scene.name << endl;
            #endif // AN8_DEBUG

            an8_file_block* c2_block;

            for(int n = 0; n < c_block->block.size(); n++)
            {
                c2_block = &c_block->block[n];
                if(c2_block->name.compare("frames")==0)
                {
                    scene.num_frames = atoi(c2_block->value.c_str());
                    //cout << "frame_count = " << scene.num_frames << endl;

                }
                else if(c2_block->name.compare("groundgrid")==0)
                {
                    scene.groundgrid = (atoi(c2_block->value.c_str()) == 0) ? false : true;
                    //cout << "groundgrid = " << (scene.groundgrid ? "true" : "false") << endl;
                }
                else if(c2_block->name.compare("shadowbias")==0)
                {
                    scene.shadowbias = atof(c2_block->value.c_str());
                    //cout << "shadowbias = " << scene.shadowbias << endl;
                }
                else if(c2_block->name.compare("background")==0)
                {
                    int c_index = 0;
                    std::string c_val = "";
                    std::string block_value = c2_block->value + " ";
                    for(int bkg_i = 0; bkg_i < block_value.length(); bkg_i++)
                    {
                        std::string c = block_value.substr(bkg_i, 1);
                        if(c.compare(" ")==0)
                        {
                            switch(c_index)
                            {
                                case 0: scene.background.r = atoi(c_val.c_str()); break;
                                case 1: scene.background.g = atoi(c_val.c_str()); break;
                                case 2: scene.background.b = atoi(c_val.c_str()); break;
                            }
                            c_index++;
                            c_val = "";
                        }
                        else
                            c_val += c;
                    }
                    //cout << "bkg = " << (uint32_t)scene.background.r << ", " << (uint32_t)scene.background.g << ", " << (uint32_t)scene.background.b << endl;
                }
                else if(c2_block->name.compare("image")==0)
                {
                    scene.image.filename = str_replace(str_replace(c2_block->value3, "\"", ""), "\\\\", "\\");
                    if(c2_block->block.size()>0)
                    {
                        if(c2_block->block[0].name.compare("panorama")==0)
                        {
                            scene.image.image_type = IMAGE_TYPE_PANORAMA;
                            an8_file_block* panorama_block = & c2_block->block[0];
                            // need to come back here eventually
                        }
                    }

                    //cout << "image = " << scene.image.filename << endl;
                }
                else if(c2_block->name.compare("camera")==0 ||
                        c2_block->name.compare("figureelement")==0 ||
                        c2_block->name.compare("objectelement")==0 ||
                        c2_block->name.compare("light")==0 ||
                        c2_block->name.compare("null")==0)
                {
                    an8_element element;
                    getElement(&element, c2_block);
                    scene.elements.push_back(element);
                }
                else
                {
                    //cout << "data obj (" << c2_block->name << ")" << endl;
                }
            }

            project->scenes.push_back(scene);

        }

    }

}

an8_project loadAN8(std::string an8_project_file)
{
    an8_project project;
    int an8_file_scope = 0;
    std::string an8_file_content;

    // Read File
    std::fstream f;
    f.open(an8_project_file.c_str(), std::fstream::in);

    std::string f_line = "";

    while( !f.eof() )
    {
        getline(f, f_line);
        an8_file_content += f_line + "\n";
    }

    f.close();
    // End Read File


    int curly_scope = 0;
    std::string token;
    an8_file_block block;
    block.name = "AN8_PROJECT";
    block.value = "AN8_PROJECT";
    block.parent = NULL;
    an8_file_block* c_block = &block;

    std::vector<int> block_index;

    an8_header header;

    std::stack<std::string> value2_stack;
    std::string value2 = "";
    std::string value3 = "";

    for(int i = 0; i < an8_file_content.length(); i++)
    {
        std::string c = an8_file_content.substr(i,1);
        std::string start_comment = an8_file_content.substr(i,2);

        if(c.compare("\n")==0 || c.compare("\r")==0)
            continue;
        else if(c.compare("\"")==0)
        {
            std::string value = an8_file_content.substr(i+1);
            int close_str = value.find_first_of("\"");
            value = value.substr(0, close_str);
            c_block->obj_name = value;
            i += close_str+1;
            c_block->value3 += "\"" + value + "\" ";
            continue;
        }
        else if(start_comment.compare("/*")==0)
        {
            bool end_comment = false;
            for(; i < an8_file_content.length(); i++)
            {
                if(end_comment)
                    break;

                if(an8_file_content.substr(i,2).compare("*/")==0)
                    end_comment = true;
            }
            continue;
        }
        else if(c.compare("{")==0)
        {
            value2_stack.push(value2);
            value2 = "";

            if(c_block->value.compare("")==0)
                c_block->value = token;

            an8_file_block b;
            b.name = trim(token);
            b.start_index = i+1;
            b.parent = c_block;

            c_block->block.push_back(b);

            int new_index = c_block->block.size()-1;
            block_index.push_back(new_index);

            c_block = &c_block->block[new_index];

            token = "";
            curly_scope++;
        }
        else if(c.compare("}")==0)
        {
            c_block->value = trim(token);

            block_index.pop_back();

            c_block = &block;

            for(int bi = 0; bi < block_index.size(); bi++)
                c_block = &c_block->block[block_index[bi]];


            token = "";
            curly_scope--;


            if(value2_stack.size()>0)
            {
                c_block->value2 = value2_stack.top();
                value2_stack.pop();
            }
            else
                c_block->value2 = value2;

            value2 = "";
        }
        else
        {
            token += c;
            value2 += c;
        }

    }

    getHeader(&project, &block);
    getObject(&project, &block);
    getFigure(&project, &block);
    getSequence(&project, &block);
    getScene(&project, &block);
    return project;

}


}



//irrlicht loader
#ifdef AN8_IRRLICHT
namespace an8
{
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



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
void getAN8MeshList(an8::an8_project* p, std::vector<an8::an8_component>* componentList, std::vector<an8::an8_mesh>* meshList, an8::an8_irr_joint_data* joint_data)
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


irr::f32 an8_calculate_figure_transform(video::S3DVertex* vertex, an8::an8_irr_joint_data* joint_data, int debug_vertex_count)
{
    if(!joint_data)
        return 0;

    an8::an8_namedobject* namedobject = &joint_data->namedobject[joint_data->namedobject_index];

    //previous node position is px, py, pz
	//px = parent_x : py = parent_y : pz = parent_z
	scene::ISkinnedMesh::SJoint* pjoint = joint_data->joint;

	if( (!pjoint) || (!joint_data->parent_joint) )
    {
        return 0;
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

	transform_matrix.transformVect(vertex->Pos);



	//----------calculate weights----------------------------------


	//transform the vertex to its namedobject orientation
	// NOTE: In this orientation, the joint is pointing straight up so its much easier to check where the vertex is in the influence area
	core::matrix4 joint_transform_matrix = joint_data->GlobalMatrix;

	core::matrix4 jm;

    joint_transform_matrix.getInverse(jm);

	namedobject_matrix.transformVect(v_out);

	v_out = v_out - joint_data->bone.pos;
	jm.transformVect(v_out);

	core::vector3df parent_translate_vector(0, joint_data->parent_length, 0);


    //Anim8or weight capsule (NOTE: I call it a capsule but one end can have a larger radius than the other)
    irr::f32 center0 = joint_data->bone.influence.center0;
	irr::f32 inRadius0 = joint_data->bone.influence.inRadius0;
	irr::f32 outRadius0 = joint_data->bone.influence.outRadius0;

	irr::f32 center1 = joint_data->bone.influence.center1;
	irr::f32 inRadius1 = joint_data->bone.influence.inRadius1;
	irr::f32 outRadius1 = joint_data->bone.influence.outRadius1;


	core::vector3df tvec(0,joint_data->bone.length,0);

	core::vector3df c0_vec(0, tvec.Y * center0, 0);
	core::vector3df c1_vec(0, tvec.Y * center1, 0);

	double dist_from_center = 0;

	double weight = 0;

	if( v_out.Y < c0_vec.Y ) // Then 'Point is below bottom part of cone area
    {
        dist_from_center = v_out.getDistanceFrom( c0_vec );

		if(dist_from_center > outRadius0)
        {
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

		if(dist_from_center > outRadius1) // Then
        {
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

		if(dist_from_center > outRadius2) // Then
        {
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


    if(joint_data->namedobject[joint_data->namedobject_index].base_bone.compare(joint_data->bone.name)==0)
        weight = 0.001;

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


    //look for the joint with the bone associated with the controller parameter
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

        }
    }
    return NULL;
}

//This struct will be used in an8_addKeys to hold each key since all of them have to be added to the frame at the same time
struct an8_rotation_key
{
    int32_t frame;
    std::string axis;
    std::string bone_name;
    irr::scene::ISkinnedMesh::SJoint* joint;
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


    //store all the rotation keys so that all the keys for a frame for a given bone can be added at the same time
    std::vector< an8_rotation_key > bone_key;


    for(int controller_index = 0; controller_index < element->controller.size(); controller_index++ )
    {
        an8::an8_controller* controller = &element->controller[controller_index];
        an8::an8_track* track = &controller->track;

        if(track->track_type == TRACK_TYPE_FLOAT && controller->name.substr(0,5).compare("joint")==0)
        {
            std::string c_axis = "N";
            joint_data = getJointData(p, scene_index, element_index, controller_index, &c_axis);

            if(joint_data == NULL)
                continue;

            core::matrix4 rm = joint_data->joint->LocalMatrix;
            core::vector3df v;
            v = rm.getRotationDegrees();


            for(int key_index = 0; key_index < track->floatkey.size(); key_index++)
            {
                a_fkey = track->floatkey[key_index];

                an8_rotation_key key;
                key.bone_name = joint_data->bone.name;
                key.frame = a_fkey.frame_index;
                key.joint = joint_data->joint;

                if(c_axis.compare("X")==0)
                {
                    key.axis = "X";
                    key.angle = a_fkey.value;
                }
                else if(c_axis.compare("Y")==0)
                {
                    key.axis = "Y";
                    key.angle = a_fkey.value;
                }
                else if(c_axis.compare("Z")==0)
                {
                    key.axis = "Z";
                    key.angle = a_fkey.value;
                }

                bone_key.push_back(key);

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

            for(int key_index = 0; key_index < track->pointkey.size(); key_index++)
            {
                a_pkey = track->pointkey[key_index];
                irr::scene::ISkinnedMesh::SPositionKey* pkey = AnimatedMesh->addPositionKey(root_joint);

                pkey->frame = (irr::f32)a_pkey.frame_index;
                pkey->position.set( a_pkey.value.x, a_pkey.value.y, a_pkey.value.z );
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
            }
        }

    }

    //loop through bone_key vector
    core::quaternion rx, ry, rz;
    bool xset = false;
    bool yset = false;
    bool zset = false;

    for(int i = 0; i < bone_key.size(); i++)
    {

        xset = false;
        yset = false;
        zset = false;

        if(bone_key[i].frame < 0)
            continue;

        int32_t frame = bone_key[i].frame;

        irr::scene::ISkinnedMesh::SRotationKey* rkey = AnimatedMesh->addRotationKey(bone_key[i].joint);
        rkey->frame = bone_key[i].frame;

        for(int j = i; j < bone_key.size(); j++)
        {
            if(bone_key[j].frame == frame &&
               bone_key[j].bone_name.compare(bone_key[i].bone_name)==0)
            {
                if(bone_key[j].axis.compare("X")==0)
                {
                    rx.fromAngleAxis( core::degToRad(bone_key[j].angle), core::vector3df(1,0,0) );
                    xset = true;
                }
                else if(bone_key[j].axis.compare("Y")==0)
                {
                    ry.fromAngleAxis( core::degToRad(bone_key[j].angle), core::vector3df(0,1,0) );
                    yset = true;
                }
                else if(bone_key[j].axis.compare("Z")==0)
                {
                    rz.fromAngleAxis( core::degToRad(bone_key[j].angle), core::vector3df(0,0,1) );
                    zset = true;
                }
                bone_key[j].frame = -1; //prevent this key from being applied again
            }

            if(xset && yset && zset)
                break;
        }

        rkey->rotation = rx*ry*rz;

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

    core::array<video::S3DVertex> vertices;
    vertices.clear();

    video::S3DVertex v;

    //-----Transformations applied to mesh in Object view in Anim8or-------
    core::matrix4 transform_matrix;
    transform_matrix.setTranslation( core::vector3df(mesh.base.origin.x,
                                                     mesh.base.origin.y,
                                                     mesh.base.origin.z));

    core::vector3df rot;
    core::quaternion quat( mesh.base.orientation.x, mesh.base.orientation.y, mesh.base.orientation.z, mesh.base.orientation.w );
    quat.toEuler(rot);

    transform_matrix.setRotationRadians(rot);

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
            }

            //The X coordinate needs to be negated to line up with the joint
            v.Pos.set(-point.x, point.y, point.z);

            //Transform the vertex to the object view space in anim8or
            transform_matrix.transformVect(v.Pos);

            //NOTE: Negating the normal X to match the vertex
            v.Normal.set(-normal.x, normal.y, normal.z);

            //I do 1 minus the texture v because the way anim8or expects the texture buffer to be aligned is just weird
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

        }

    }

    if(indices.size() > 0)
    {
        for(int i = 0; i < vertices.size(); i++)
            meshBuffer->Vertices_Standard.push_back(vertices[i]);

        for(int i = 0; i < indices.size(); i++)
            meshBuffer->Indices.push_back(indices[i]);

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

core::matrix4 calculate_bone_matrix(scene::ISkinnedMesh::SJoint* inJoint, an8::an8_bone* parent, bool should_translate=true)
{
    core::vector3df tvec( 0, parent->length, 0);
    core::quaternion q(parent->orientation.x,
                       parent->orientation.y,
                       -parent->orientation.z,
                       parent->orientation.w);



    core::vector3df old_pos = parent->pos;
    core::vector3df t = tvec;
    core::matrix4 m = inJoint->GlobalMatrix;
    core::vector3df old_rotation = m.getRotationDegrees();

    core::vector3df angle;
    q.toEuler(angle);

    angle = vectorRadiansToDegrees(angle);


    m = rotateAxesXYZToEuler(old_rotation, angle, true);

    core::matrix4 tm;
    tm.setTranslation(old_pos);

    if(should_translate)
        m = tm * m;

    return m;

}


//get the bone data for a figure
bool getAN8BoneNode(an8::an8_project* p, scene::ISkinnedMesh* AnimatedMesh, scene::ISkinnedMesh::SJoint* inJoint, an8::an8_bone bone, an8::an8_bone parent_bone, an8::an8_irr_joint_data* parent_joint_data)
{
	scene::ISkinnedMesh::SJoint *joint = AnimatedMesh->addJoint(inJoint);
	joint->Name = bone.name.c_str();

	//The root bone won't have a position despite whats in the *.an8 file
	joint->LocalMatrix.makeIdentity();
	joint->GlobalMatrix.makeIdentity();

	core::vector3df position(0,0,0);

	bone.rot = core::quaternion(0,0,0,0);

	core::matrix4 tm;

	if(inJoint)
    {
        joint->GlobalMatrix = calculate_bone_matrix(inJoint, &parent_bone, true);
        position.Y = parent_bone.length;

        tm = calculate_bone_matrix(inJoint, &parent_bone, false);
    }

    core::quaternion orient = core::quaternion(-bone.orientation.x,
                                               -bone.orientation.y,
                                               bone.orientation.z, //z needs to be reversed
                                               -bone.orientation.w);


    bone.rot = core::quaternion(-bone.orientation.x,
                                -bone.orientation.y,
                                bone.orientation.z, //z needs to be reversed
                                -bone.orientation.w);

	joint->GlobalMatrix.transformVect(position);
	bone.pos = position;
	bone.pos.Z = -bone.pos.Z;

	position = core::vector3df(0, parent_bone.length, 0);
	tm.transformVect(position);

    joint->Animatedrotation = core::quaternion(0,0,0,-1);
    joint->Animatedposition = position;


    //Build LocalMatrix:
	core::matrix4 positionMatrix;
	positionMatrix.setTranslation( joint->Animatedposition );

	core::matrix4 rotationMatrix;
	joint->Animatedrotation.getMatrix_transposed(rotationMatrix);

	joint->LocalMatrix = positionMatrix * rotationMatrix;



    //Get Meshes  NOTE: a bone can have multiple meshes
    std::vector<an8::an8_mesh> meshList;

    an8::an8_irr_joint_data joint_data;
    joint_data.parent_joint = inJoint;
    joint_data.joint = joint;
    joint_data.bone = bone;
    joint_data.parent_length = parent_bone.length;


    joint_data.GlobalMatrix = calculate_bone_matrix(joint, &bone, false);

    getAN8MeshList(p, &bone.component, &meshList, &joint_data);

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

    //Add Child Bones

    for(int i = 0; i < bone.bone.size(); i++)
    {
        getAN8BoneNode(p, AnimatedMesh, joint, bone.bone[i], bone, &joint_data);
    }

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



    //Make magic happen
    for(int i = 0; i < mesh_scene.elements.size(); i++)
    {
        element = mesh_scene.elements[i];

        switch(mesh_scene.elements[i].element_type)
        {
            case ELEMENT_TYPE_FIGURE:

                if(!getAN8Figure(&a_file, element.obj_name, &figure))
                    continue;

                a_file.node_figure = figure;

                figure.bone.length = 0;

                getAN8BoneNode(&a_file, mesh, (scene::ISkinnedMesh::SJoint*)0, figure.bone, figure.bone, (an8::an8_irr_joint_data*)0);

                for(int n = 0; n < a_file.irr_joint_data.size(); n++)
                {

                    for(int n_obj_index = 0; n_obj_index < a_file.irr_joint_data[n].namedobject.size(); n_obj_index++)
                    {
                        an8::an8_namedobject* n_obj = &a_file.irr_joint_data[n].namedobject[n_obj_index];

                        a_file.irr_joint_data[n].namedobject_index = n_obj_index;

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
                                }
                                else
                                {
                                    vertex = figure_transformed_meshBuffers[meshBuffer_figure_transform_calculated].buffer.Vertices_Standard[vert_index];
                                }
                                an8::an8_irr_joint_data* joint_data = &a_file.irr_joint_data[n];

                                scene::ISkinnedMesh::SWeight* vert_weight = mesh->addWeight(joint_data->joint);

                                vert_weight->buffer_id = mesh_buffer_id;
                                vert_weight->vertex_id = vert_index;

                                vert_weight->strength = an8_calculate_figure_transform(&vertex, &a_file.irr_joint_data[n], (vert_index==39)*11);

                                if(meshBuffer_figure_transform_calculated < 0)
                                    mesh_buffer->Vertices_Standard[vert_index] = vertex;
                            }

                        }
                    }
                }

                an8_addKeys(&a_file, mesh, scene_index, i);

                break;

            case ELEMENT_TYPE_OBJECT:

                //std::cout << "obj_name = " << element.obj_name << std::endl;

                //an8::an8_figure obj_figure;
                //obj_figure.name = an8::str_replace(element.obj_name, " ", "") + "_figure";

                //an8::an8_bone obj_root_bone;
                //obj_root_bone.orientation.x = 0;
                //obj_root_bone.orientation.y = 0;
                //obj_root_bone.orientation.z = 0;
                //obj_root_bone.orientation.w = 0;

                //obj_figure.bone = obj_root_bone;

                //a_file.node_figure = figure;

                //figure.bone.length = 0;

                //getAN8BoneNode(&a_file, mesh, (scene::ISkinnedMesh::SJoint*)0, figure.bone, figure.bone, (an8::an8_irr_joint_data*)0);

                break;
        }
    }



    mesh->finalize();


    //irr::scene::IMeshManipulator *meshManip = device->getSceneManager()->getMeshManipulator();
    //meshManip->flipSurfaces(mesh);

    return mesh;
}

}
#endif // AN8_IRRLICHT


#endif // AN8_H_INCLUDED
