#include "../header/shader.h"

void shader::loadFile(const char* fn,std::string& str)
{
	std::ifstream in(fn);
	if(!in.is_open())
	{
		std::cout << "The file " << fn << " cannot be opened\n";
		return;
	}
	char tmp[300];
	while(!in.eof())
	{
		in.getline(tmp,300);
		str+=tmp;
		str+='\n';
	}
}

unsigned int shader::loadShader(std::string& source,unsigned int mode)
{
	unsigned int id;
	id=glCreateShader(mode);
	
	const char* csource=source.c_str();
	
	glShaderSource(id,1,&csource,NULL);
	glCompileShader(id);
	char error[1000];
	glGetShaderInfoLog(id,1000,NULL,error);
	std::cout << "Compile status: \n" << error << std::endl;
	return id;
}

shader::shader(const char* vss,const char* fss)
{
	std::cout<<std::endl<<" vertex SHader "<<vss<<std::endl<<" FragmentSHader "<<fss<<std::endl;
	std::string source;
	loadFile(vss,source);
	vs=loadShader(source,GL_VERTEX_SHADER);
	source="";
	loadFile(fss,source);
	fs=loadShader(source,GL_FRAGMENT_SHADER);
	
	program=glCreateProgram();
	glAttachShader(program,vs);
	glAttachShader(program,fs);
	
	glLinkProgram(program);
	glUseProgram(program);	
}

shader::~shader()
{
	glDetachShader(program,vs);
	glDetachShader(program,fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(program);
}

void shader::useShader()
{
	glUseProgram(program);
}

unsigned int shader::getProgramId()
{
	return program;
}


void shader::delShader()
{
	glUseProgram(0);
}
		
