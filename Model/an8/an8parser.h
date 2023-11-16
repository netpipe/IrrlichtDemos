#ifndef IRRAN8_H_INCLUDED
#define IRRAN8_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <irrlicht/irrlicht.h>

//#define AN8_DEBUG 1

using namespace std;

namespace an8
{

string ltrim(string l_string)
{
    if(l_string.find_first_not_of(" ") != string::npos)
        return l_string.substr(l_string.find_first_not_of(" "));
    return "";
}

string rtrim(string src)
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

string trim(string t_string)
{
    return ltrim(rtrim(t_string));
}

string str_replace(string src, string tgt, string rpc)
{
    if(tgt.length()==0)
        return src;
    unsigned long found_inc = rpc.length() > 0 ? rpc.length() : 1;
    size_t found = 0;
    found = src.find(tgt);
    while( found != string::npos && found < src.length())
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
    string text;
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
    string name;
    int32_t invert; //I am not actually sure what this is
    int32_t cubemap; //not sure what this is
    string file[6]; //will only use file[0] unless cubemap is non-zero
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
    string texturename;
    an8_texture_param textureparams;

};

struct an8_map
{
    string texturename;
    string kind;
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
    string name;
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
    vector<an8_pointdata> point_data;
};

struct an8_base
{
    an8_point3f origin;
    an8_point4f orientation;
};

struct an8_mesh
{
    string name;

    an8_base base;
    an8_base pivot;
    an8_material material; //used for new faces added to mesh; only name is used
    float smoothangle;
    vector<string> materialname;
    vector<an8_point3f> points;
    vector<an8_point3f> normals;
    vector<an8_edge> edges; //mostly unused
    vector<an8_uv> texcoords;
    vector<an8_facedata> faces;

    //irrlicht loader specific crap
    void* meshBuffer;
};

struct an8_name
{
    string name;
};

struct an8_weightedBy
{
    string bone_name;
};

struct an8_boneWeight
{
    uint32_t bone_index; //This is the index in the weightedBy vector in namedobject
    double weight; //normalized
};

struct an8_vertexWeight
{
    vector<an8_boneWeight> boneWeight;
};

struct an8_meshWeights
{
    string name;
    vector<an8_vertexWeight> vertexWeight;
};

struct an8_namedobject
{
    string name;
    an8_name name_block;
    an8_base base;
    an8_base pivot;
    an8_material material;
    vector<an8_weightedBy> weightedBy;
    vector<an8_meshWeights> meshWeights;

    //irrlicht loader specific stuff
    vector<int> meshList_index;

    // Note: Each named object will only have one object but
    //       an object can have multiple meshes
    vector<irr::scene::SSkinMeshBuffer*> meshBufferList;
    std::string base_bone; //base transformations will happen from this bone
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
    string name;
    std::vector<an8_material> material;
    std::vector<an8_component> component;
};

struct an8_dof
{
    string axis; //X, Y, or Z
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
    string name;
    double length;
    double diameter; // seems to be optional
    an8_point4f orientation;
    bool locked; //not important, just here for completion
    std::vector<an8_dof> dof;
    an8_influence influence;
    std::vector<an8_component> component;
    std::vector<an8_bone> bone;

    //irrlicht stuff
    irr::core::vector3df pos;
    irr::core::quaternion rot;
};

struct an8_figure
{
    string name;
    std::vector<an8_material> material;
    an8_bone bone;
};

struct an8_floatkey
{
    uint32_t frame_index;
    double value;
    string mod_string;
};

struct an8_pointkey
{
    uint32_t frame_index;
    an8_point3f value;
    an8_point3f forward_vector;
    an8_point3f reverse_vector;
    string mod_string;
};

struct an8_qkey
{
    uint32_t frame_index;
    an8_point4f value;
    string mod_string;
};

struct an8_booleankey
{
    uint32_t frame_index;
    bool value;
    string mod_string;
};

#define TRACK_TYPE_NONE         0
#define TRACK_TYPE_FLOAT        1
#define TRACK_TYPE_POINT        2
#define TRACK_TYPE_QUATERNION   3
#define TRACK_TYPE_BOOLEAN      4

struct an8_track
{
    int track_type;
    vector<an8_floatkey> floatkey;
    vector<an8_pointkey> pointkey;
    vector<an8_qkey> qkey;
    vector<an8_booleankey> booleankey;
};

struct an8_jointangle
{
    string bone_name;
    string axis;
    an8_track track;
};

struct an8_sequence
{
    string name;
    string figure_name;
    uint32_t num_frames;
    vector<an8_jointangle> jointangle;
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
    string filename;
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
    string name;
    an8_track track;
};

struct an8_namedsequence
{
    string name;
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
    string name;
    an8_point3f loc;
    an8_point4f orientation;
    bool roll;
    uint32_t orient_relative_to;
    string bound_target; //if it has the facestarget or orienttarget chunk
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

    string obj_name; // object
    bool visible;
    bool receive_shadow;

    vector<an8_namedsequence> named_sequence; // figure

    vector<an8_controller> controller;
    vector<an8_element> element; //child elements
};

struct an8_scene
{
    string name;
    uint32_t num_frames;
    bool groundgrid;
    double shadowbias;
    an8_color background;
    an8_image image;
    an8_fog fog;
    double znear;
    double zfar;
    vector<an8_element> elements;
};


struct an8_irr_joint_data
{
    vector<an8_namedobject> namedobject;
    an8_bone bone;
    irr::scene::ISkinnedMesh::SJoint *joint;

    //irrlicht specific stuff
    int32_t namedobject_index; //index of named object used during an8_calculate_weight()
    //vector<irr::u32> meshBufferIndex; //This is a list of meshes weighted by this bone
    irr::scene::ISkinnedMesh::SJoint *parent_joint;
};

struct an8_project
{
    an8_header header;
    an8_description description;
    an8_environment environment;
    vector<an8_texture> textures;
    vector<an8_material> materials;
    vector<an8_object> objects;

    vector<an8_figure> figures;
    vector<an8_sequence> sequences;
    vector<an8_scene> scenes;

    //The following are not part of anim8or structs but just used for storing temp data when loading irrlicht mesh
    vector<an8_irr_joint_data> irr_joint_data;
    an8::an8_figure node_figure; // I am going to store the figure the current bones are from here
};

struct an8_file_block
{
    int type;
    int start_index = -1;
    int end_index = -1;
    string name;
    string obj_name;
    string value;
    string value2;
    string value3;
    an8_file_block* parent;
    vector<an8_file_block> block;
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
            string c_value = c_block->value + " ";
            string c_str = "";
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
            string token = "";
            double v_point[3];
            v_point[0] = 0;
            v_point[1] = 0;
            v_point[2] = 0;
            int vi = 0;
            string c_value = c_block->value + " ";

            for(int n = 0; n < c_value.length(); n++)
            {
                string c = c_value.substr(n, 1);

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
            string token = "";
            double v_point[4];
            v_point[0] = 0;
            v_point[1] = 0;
            v_point[2] = 0;
            v_point[3] = 0;
            int vi = 0;
            string c_value = c_block->value + " ";

            for(int n = 0; n < c_value.length(); n++)
            {
                string c = c_value.substr(n, 1);

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


void getMaterialList(vector<string>* mat_list, an8_file_block* block)
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

void getPoints3f(vector<an8_point3f>* points, an8_file_block* block)
{
    double v[3];
    int vi = 0;
    string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        string c = block->value.substr(i,1);

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

void getPoints4f(vector<an8_point4f>* points, an8_file_block* block)
{
    double v[4];
    int vi = 0;
    string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        string c = block->value.substr(i,1);

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

void getEdges(vector<an8_edge>* edges, an8_file_block* block)
{
    uint32_t v[3];
    int vi = 0;
    string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        string c = block->value.substr(i,1);

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

void getUV(vector<an8_uv>* texcoords, an8_file_block* block)
{
    double v[2];
    int vi = 0;
    string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        string c = block->value.substr(i,1);

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

void getPoints2i(vector<an8_point2i>* points, an8_file_block* block)
{
    int32_t v[2];
    int vi = 0;
    string arg = "";

    bool in_scope = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        string c = block->value.substr(i,1);

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

void getFaces(vector<an8_facedata>* faces, an8_file_block* block)
{
    int v[4];

    int f_data[4];
    vector<an8_pointdata> point_data;

    int vi = 0;
    int fi = 0;
    string arg = "";

    bool in_pdata = false;
    bool in_scope = false;

    bool normal_flag = false;
    bool texture_flag = false;

    bool flag_set = false;

    for(int i = 0; i < block->value.length(); i++)
    {
        string c = block->value.substr(i,1);

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
    string value = block->value;
    meshWeight->name = block->obj_name;

    //cout << "Name: " << meshWeight->name << endl;
    //cout << "GET Weights: " << value << endl << endl;

    int p_scope = 0;
    string vertex_value = "";

    for(int i = 0; i < value.length(); i++)
    {
        string c = value.substr(i,1);

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
            string arg = "";

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

void getComponent(vector<an8_component>* component_list, an8_file_block* block)
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

void getDOF(vector<an8_dof>* dof, an8_file_block* block)
{
    int vi = 0;
    string arg = "";


    an8_dof tmp_dof;
    tmp_dof.axis = block->obj_name;
    tmp_dof.default_angle = 0;
    tmp_dof.locked = false;
    tmp_dof.max_angle = 0;
    tmp_dof.min_angle = 0;
    tmp_dof.unlimited = false;

    string value2 = block->value2 + " ";

    for(int i = 0; i < value2.length(); i++)
    {
        string c = value2.substr(i,1);

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
    string arg = "";
    string b_value = trim(block->value) + " ;";

    for(int i = 0; i < b_value.length(); i++)
    {
        string c = b_value.substr(i,1);

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

    string value = trim(block->value) + " ";

    int key_index = 0;

    string c_arg = "";

    int m_start = block->value3.find_first_of("\"") + 1;
    int m_end = block->value3.substr(m_start).find_first_of("\"")+1;

    if(m_start != string::npos && m_end != string::npos)
        key->mod_string = block->value3.substr(m_start, m_end-m_start);
    else
        key->mod_string = "";

    for(int i = 0; i < value.length(); i++)
    {
        string c = value.substr(i,1);

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

    string value = trim(block->value) + " ";

    int key_index = 0;

    string c_arg = "";

    int m_start = block->value3.find_first_of("\"") + 1;
    int m_end = block->value3.substr(m_start).find_first_of("\"")+1;

    if(m_start != string::npos && m_end != string::npos)
        key->mod_string = block->value3.substr(m_start, m_end-m_start);
    else
        key->mod_string = "";

    vector<an8_point3f> points;
    getPoints3f(&points, block);

    if(points.size() >= 3)
    {
        key->value = points[0];
        key->forward_vector = points[1];
        key->reverse_vector = points[2];
    }


    for(int i = 0; i < value.length(); i++)
    {
        string c = value.substr(i,1);

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

    string value = trim(block->value) + " ";

    int key_index = 0;

    string c_arg = "";

    int m_start = block->value3.find_first_of("\"") + 1;
    int m_end = block->value3.substr(m_start).find_first_of("\"")+1;

    if(m_start != string::npos && m_end != string::npos)
        key->mod_string = block->value3.substr(m_start, m_end-m_start);
    else
        key->mod_string = "";

    vector<an8_point4f> points;
    getPoints4f(&points, block);

    if(points.size() >= 1)
    {
        key->value = points[0];
    }


    for(int i = 0; i < value.length(); i++)
    {
        string c = value.substr(i,1);

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


    string value = trim(block->value) + " ";
    //cout << "DBG BOOLEAN: " << value << endl;

    int key_index = 0;

    string c_arg = "";

    int m_start = block->value3.find_first_of("\"") + 1;
    int m_end = block->value3.substr(m_start).find_first_of("\"")+1;

    if(m_start != string::npos && m_end != string::npos)
        key->mod_string = block->value3.substr(m_start, m_end-m_start);
    else
        key->mod_string = "";

    for(int i = 0; i < value.length(); i++)
    {
        string c = value.substr(i,1);

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

    string ja_val3 = block->value3;

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


    string value = trim(block->value) + " ";
    int key_index = 0;

    string c_arg = "";

    namedsequence->name = block->obj_name;

    //cout << "Named Sequence: " << namedsequence->name << endl;

    for(int i = 0; i < value.length(); i++)
    {
        string c = value.substr(i,1);

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

    string element_val3 = block->value3;

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
            vector<an8_point3f> points;
            getPoints3f(&points, c_block);

            if(points.size() > 0)
                element->loc = points[0];
            else
                cout << "Error getting element location" << endl;

            //cout << "GET Location: " << points[0].x << ", " << points[0].y << ", " << points[0].z << endl;
        }
        else if(c_block->name.compare("orientation")==0)
        {
            vector<an8_point4f> points;
            getPoints4f(&points, c_block);

            if(points.size() > 0)
                element->orientation = points[0];
            else
                cout << "Error getting element orientation" << endl;

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
            vector<an8_point3f> point;
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
                    string c_val = "";
                    string block_value = c2_block->value + " ";
                    for(int bkg_i = 0; bkg_i < block_value.length(); bkg_i++)
                    {
                        string c = block_value.substr(bkg_i, 1);
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
    string an8_file_content;

    // Read File
    fstream f;
    f.open(an8_project_file.c_str(), fstream::in);

    string f_line = "";

    while( !f.eof() )
    {
        getline(f, f_line);
        an8_file_content += f_line + "\n";
    }

    f.close();
    // End Read File


    int curly_scope = 0;
    string token;
    an8_file_block block;
    block.name = "AN8_PROJECT";
    block.value = "AN8_PROJECT";
    block.parent = NULL;
    an8_file_block* c_block = &block;

    vector<int> block_index;

    an8_header header;

    stack<string> value2_stack;
    string value2 = "";
    string value3 = "";

    for(int i = 0; i < an8_file_content.length(); i++)
    {
        string c = an8_file_content.substr(i,1);
        string start_comment = an8_file_content.substr(i,2);

        if(c.compare("\n")==0 || c.compare("\r")==0)
            continue;
        else if(c.compare("\"")==0)
        {
            string value = an8_file_content.substr(i+1);
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
#endif // IRRAN8_H_INCLUDED
