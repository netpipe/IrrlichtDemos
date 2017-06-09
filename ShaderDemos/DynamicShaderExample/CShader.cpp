/**************************************************************
file: CShader.h
author: Stephan Gödecker (Masterhawk studios)
version: rev. 5

description: this file was created to achieve a dynamic
management for GLSL shaders with irrlicht. After implementing
this code-snippet the user is able to dynamically load GLSL-
shader in his irrlicht application. The shader just have to
be additionally defined by a special xml-file. You can get
further information at:

http://www.masterhawk.dyndns.org/wordpress?p=267

***************************************************************/

#include "CShader.h"

CShader::CShader(IrrlichtDevice* device, const c8* shader_xml)
{

	dev=device;

	f_old_cycle_time = dev->getTimer()->getRealTime();

	irr::core::stringc vsFileName = "./shaders/"; //filename for vertex shader
	const c8* vsEntry;
	irr::core::stringc psFileName = "./shaders/"; //filename for pixel/fragment shader
	const c8* psEntry;


	//Handling xml-definition file
	doc = TiXmlDocument(shader_xml);
	bool loadOkay = doc.LoadFile();

	if(loadOkay)
	{
		printf("(XML-PARSER) XML successfully loaded\n");
		TiXmlNode* root = doc.FirstChild("irrshader");
		TiXmlNode* config = root->FirstChild("config");

		if(!config)
		{
			printf("(XML-PARSER) ERROR: Expected node not found!!!\n");
			return;
		}

		TiXmlNode* vert_node = config->FirstChild("vertex-shader-file");

		TiXmlAttribute* attr = ((TiXmlElement*)vert_node)->FirstAttribute();
		if(!attr)
		{
			printf("(XML-PARSER) ERROR: No Attribute found!\n");
			return;
		}

		vsFileName += irr::core::stringc(attr->Value());
		attr = attr->Next();
		vsEntry = attr->Value();

		attr = 0;

		TiXmlNode* frag_node = config->FirstChild("fragment-shader-file");

		attr = ((TiXmlElement*)frag_node)->FirstAttribute();
		if(!attr)
		{
			printf("(XML-PARSER) ERROR: No Attribute found!\n");
			return;
		}

		psFileName += irr::core::stringc(attr->Value());
		attr = attr->Next();
		psEntry = attr->Value();

	}
	else
	{
		printf("(XML PARSER) ERROR: Failed to load xml: %s\n", shader_xml);
		return;
	}

	driver = device->getVideoDriver();

	//Missing xml-handling

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	p_material = 0;

	if(gpu)
	{
		p_material = gpu->addHighLevelShaderMaterialFromFiles(
			vsFileName, "vertexMain", video::EVST_VS_4_1,
			psFileName, "pixelMain", video::EPST_PS_4_1,
			this, video::EMT_SOLID);
	}


	TiXmlNode* root = doc.FirstChild("irrshader");

	TiXmlNode* variables = root->FirstChild("variables");

	TiXmlNode* uniform = variables->FirstChild("uniform");

	s32 uniform_count = ((TiXmlElement*)uniform)->FirstAttribute()->IntValue();

	TiXmlNode* node = uniform->FirstChild("variable");

	for(s32 i=0; i<uniform_count; i++)
	{
		//create new uniform variable
		SShaderVariable* shader_var = new SShaderVariable();
		shader_var->predefinition = ESPV_NONE;

		TiXmlAttribute* attr = ((TiXmlElement*)node)->FirstAttribute();
		irr::core::stringc type( attr->Value() );

		if(type.equals_ignore_case("float"))
			shader_var->type = ESVT_FLOAT;
		else if(type.equals_ignore_case("vec2"))
			shader_var->type = ESVT_VEC2;
		else if(type.equals_ignore_case("vec3"))
			shader_var->type = ESVT_VEC3;
		else if(type.equals_ignore_case("vec4"))
			shader_var->type = ESVT_VEC4;
		else if(type.equals_ignore_case("mat4"))
			shader_var->type = ESVT_MAT4;
		else if(type.equals_ignore_case("sampler2d"))
			shader_var->type = ESVT_SAMPLER2D;
		else if(type.equals_ignore_case("predefined"))
			shader_var->type = ESVT_PREDEFINED;

		attr = attr->Next();
		shader_var->name = core::stringc( attr->Value() );

		attr = attr->Next();
		shader_var->b_frag_var = (attr->IntValue() != 0) ? true : false;

		switch(shader_var->type)
		{
		case ESVT_SAMPLER2D:
		case ESVT_FLOAT:
		{
			float* vars = new float[1];

			TiXmlNode* value = node->FirstChild("value");

			vars[0] = ((TiXmlElement*)value)->FirstAttribute()->DoubleValue();
			shader_var->value = vars;

		}break;

		/*case ESVT_SAMPLER2D:
		{
			int tex = 0;

			TiXmlNode* value = node->FirstChild("value");

			tex = ((TiXmlElement*)value)->FirstAttribute()->IntValue();

			float* vars = new float[1];
			vars = (float*)(&tex);
			shader_var->value = vars;
		}break;*/

		case ESVT_VEC2:
		{
			float* vars = new float[2];

			TiXmlNode* value = node->FirstChild("value");

			for(s32 j=0; j<2; j++)
			{
				vars[j] = ((TiXmlElement*)value)->FirstAttribute()->DoubleValue();
				value = value->NextSibling();
			}
			shader_var->value = vars;
		}break;

		case ESVT_VEC3:
		{
			float* vars = new float[3];

			TiXmlNode* value = node->FirstChild("value");

			for(s32 j=0; j<3; j++)
			{
				vars[j] = ((TiXmlElement*)value)->FirstAttribute()->DoubleValue();
				value = value->NextSibling();
			}
			shader_var->value = vars;
		}break;

		case ESVT_VEC4:
		{
			float* vars = new float[4];

			TiXmlNode* value = node->FirstChild("value");

			for(s32 j=0; j<4; j++)
			{
				vars[j] = ((TiXmlElement*)value)->FirstAttribute()->DoubleValue();
				value = value->NextSibling();
			}
			shader_var->value = vars;
		}break;

		case ESVT_MAT4:
			break;

		case ESVT_PREDEFINED:
		{
			TiXmlNode* value = node->FirstChild("value");

			attr = ((TiXmlElement*)value)->FirstAttribute();
			if(!attr) printf("(XML PARSER) ERROR: Missing Attribute\n");

			core::stringc str_val(attr->Value());

			shader_var->value = 0;

			if(str_val.equals_ignore_case("predefined_eye_position"))
				shader_var->predefinition = ESPV_EYEPOSITION;
			else if(str_val.equals_ignore_case("predefined_mat4_view_proj"))
			{
				shader_var->predefinition = ESPV_MAT4_VIEW_PROJ;
			}
			else if(str_val.equals_ignore_case("predefined_mat4_view"))
			{
				shader_var->predefinition = ESPV_MAT4_VIEW;
			}
			else if(str_val.equals_ignore_case("predefined_float_time0x"))
			{
				shader_var->predefinition = ESPV_FLOAT_TIME0X;
				shader_var->value = new float[1];
				shader_var->value[0] = 0.f;
			}
			else if(str_val.equals_ignore_case("predefined_rtt_view"))
			{
				shader_var->predefinition = ESPV_NONE;
				shader_var->type = ESVT_SAMPLER2D;

				attr = attr->Next();
				if(!attr) printf("(XML PARSER) ERROR: Missing Attribute\n");

				float* vars = new float[1];

				vars[0] = attr->DoubleValue();

				shader_var->value = vars;

				//registering for RTT_VIEW
				S_RTT_Info* rtt_info = new S_RTT_Info();
				rtt_info->type = ERT_VIEW;
				rtt_info->tex_channel = (int)vars[0];
				rtt_infos.push_back(rtt_info);

				rtt_info = 0;

			}
			else
				printf("(XML PARSER) ERROR: Unknown predefinition: %s\n",str_val.c_str());


			break;
		}

		default:
///			printf("(XML PARSER) ERROR: No type for uniform shader variable specified: %s\n",shader_var->name);
			break;
		}//end switch

		uniform_variables.push_back(shader_var);
		node = node->NextSibling("variable");
	}
	printf("(XML PARSER) Initialized %i uniform variables\n",uniform_variables.size());
	for(s32 i=0; i<uniform_variables.size(); i++)
	{
		printf("(XML PARSER) VAR%i: %s VALUE: ",i,uniform_variables[i]->name.c_str());
		if(uniform_variables[i]->type == ESVT_PREDEFINED)
		{
			printf("PREDEFINED\n");
			continue;
		}
		s32 element_count = (uniform_variables[i]->type != ESVT_SAMPLER2D) ? (s32)uniform_variables[i]->type : 1;
		for(s32 j=0; j<element_count; j++)
			//if(uniform_variables[i]->type != ESVT_SAMPLER2D)
				printf("%f ",(uniform_variables[i]->value[j]));
			//else
				//printf("%i",*uniform_variables[i]->value);
		printf("\n\n");
	}
}

void CShader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
	for(s32 i=0; i<uniform_variables.size(); i++)
	{
		int element_count = 0;
		switch(uniform_variables[i]->type)
		{
		case ESVT_SAMPLER2D:
			element_count = 1;
			break;
		case ESVT_PREDEFINED:
		{
			switch(uniform_variables[i]->predefinition)
			{
				case ESPV_EYEPOSITION:
				{
					if(uniform_variables[i]->value)
					{
						delete[] uniform_variables[i]->value;
						uniform_variables[i]->value = 0;
					}

					uniform_variables[i]->value = new float[3];
					core::vector3df cam_pos = dev->getSceneManager()->getActiveCamera()->getAbsolutePosition();
					uniform_variables[i]->value[0] = cam_pos.X;
					uniform_variables[i]->value[1] = cam_pos.Y;
					uniform_variables[i]->value[2] = cam_pos.Z;

					element_count = 3;
				}
				break;

				case ESPV_MAT4_VIEW:
				{
					core::matrix4 mat_view =  dev->getVideoDriver()->getTransform(video::ETS_VIEW);

					uniform_variables[i]->value = mat_view.pointer();

					element_count = 16;
				}
				break;

				case ESPV_MAT4_VIEW_PROJ:
				{
					core::matrix4 mat_view_proj =  dev->getVideoDriver()->getTransform(video::ETS_PROJECTION);

					mat_view_proj *= dev->getVideoDriver()->getTransform(video::ETS_VIEW);

					uniform_variables[i]->value = mat_view_proj.pointer();

					element_count = 16;

					/*printf("VIEW_PROJECTION_MATRIX\n");
					for(int k=0; k<16; k++)
					{
						printf("%f ",uniform_variables[i]->value[k]);
						if(k==3 || k==7 || k==11 || k==15) printf("\n");
					}*/
				}
				break;

				case ESPV_FLOAT_TIME0X:
				{

					f32 f_time_diff = dev->getTimer()->getRealTime() - f_old_cycle_time;

					uniform_variables[i]->value[0] += f_time_diff / 1000;

					s32 i_tmp = uniform_variables[i]->value[0] / 120;

					uniform_variables[i]->value[0] = uniform_variables[i]->value[0] - (i_tmp*120);

					f_old_cycle_time = dev->getTimer()->getRealTime();

					element_count = 1;

					//printf("FLOAT_TIME0X = %f\n",uniform_variables[i]->value[0]);
				}
				break;
			}
		}
		default:
			element_count = (s32)uniform_variables[i]->type;
		}
		if(uniform_variables[i]->b_frag_var)
			if(uniform_variables[i]->type != ESVT_SAMPLER2D)
				services->setPixelShaderConstant(uniform_variables[i]->name.c_str(),uniform_variables[i]->value,element_count);
			else
			{
				int tex = *uniform_variables[i]->value;
				services->setPixelShaderConstant(uniform_variables[i]->name.c_str(),(float*)(&tex),element_count);
			}

		else
			if(uniform_variables[i]->type != ESVT_SAMPLER2D)
				services->setVertexShaderConstant(uniform_variables[i]->name.c_str(),uniform_variables[i]->value,element_count);
			else
			{
				int tex = *uniform_variables[i]->value;
				services->setVertexShaderConstant(uniform_variables[i]->name.c_str(),(int*)(&tex),element_count);
			}

	}

}

s32 CShader::getMaterial()
{
	return p_material;
}

S_RTT_Info* CShader::getRTTInfo(E_RTT_TEXTURE rtt)
{
	for(s32 i=0; i<rtt_infos.size(); i++)
	{
		if(rtt == rtt_infos[i]->type)
			return rtt_infos[i];
	}
	return 0;
}

CShader::~CShader()
{
	for(s32 i=0; i<uniform_variables.size(); i++)
	{
		if(uniform_variables[i]->predefinition != ESPV_MAT4_VIEW_PROJ && uniform_variables[i]->predefinition != ESPV_MAT4_VIEW)
			delete[] uniform_variables[i]->value;

		delete uniform_variables[i];
		uniform_variables[i] = 0;
	}
	uniform_variables.clear();
}
