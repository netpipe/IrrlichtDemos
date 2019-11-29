/*

Copyright (c) 2011, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/
#include <sstream>
#include <fstream>
#include "Shader.h"
#include "Phoenix.h"

using namespace phoenix;

/* Destructor */
Shader::~Shader(){
	if( vertex_shader ) glDeleteShader( vertex_shader );
	if( fragment_shader ) glDeleteShader( fragment_shader );
	if( shader_program ) glDeleteProgram( shader_program );
}

/* Acivates a shader */
void Shader::activate(){
	if( !GLEW_VERSION_2_0 ) return;
	if( shader_program ){
		glUseProgram(shader_program);
	} else {
		glUseProgram( 0 );
	}
};

/*! Activates FFP
*/
void Shader::deactivate(){
	if( !GLEW_VERSION_2_0 ) return;
	glUseProgram( 0 );
}

/*! Find the location of a uniform in the shader. If not there, returns -1.
*/
int Shader::getUniform( const std::string &uniform_name ) {
    return glGetUniformLocation( shader_program , uniform_name.c_str() );
}

void Shader::setUniform( const std::string &uniform_name, const int value ) {
    glUniform1i( getUniform(uniform_name) , value );
}

void Shader::setUniform( const std::string &uniform_name, const float value ) {
    glUniform1f( getUniform(uniform_name) , value );
}

void Shader::setUniform( const std::string &uniform_name, const Vector2d &value ) {
    glUniform2f( getUniform(uniform_name) , value.getX(), value.getY() );
}

void Shader::setUniform( const std::string &uniform_name, const Color &value ) {
    glUniform4f( getUniform(uniform_name) , (GLfloat)value.getRed()/255.0f, (GLfloat)value.getGreen()/255.0f, (GLfloat)value.getBlue()/255.0f, (GLfloat)value.getAlpha()/255.0f );
}

void Shader::setUniform( const std::string &uniform_name, const TexturePtr value ) {
    glUniform1i( getUniform(uniform_name) , value->getTextureId() );
}
   

/*! Loads shader files, compiles them and links them
*/
bool Shader::load( const std::string& vertex_file, const std::string& fragment_file ){
	vertex_shader = compile( GL_VERTEX_SHADER, getFileContents( vertex_file ) );
	fragment_shader = compile( GL_FRAGMENT_SHADER, getFileContents( fragment_file ) );

	if( !vertex_shader || ! fragment_shader ) return false;

	shader_program = link();

	if( !shader_program ) return false;

	return true;
}

/*! Loads shaders from a string, compiles, and links them */
bool Shader::loadFromString( const std::string& vertex_source, const std::string& fragment_source ){
	vertex_shader = compile( GL_VERTEX_SHADER, vertex_source );
	fragment_shader = compile( GL_FRAGMENT_SHADER, fragment_source );

	if( !vertex_shader || ! fragment_shader ) return false;

	shader_program = link();

	if( !shader_program ) return false;

	return true;
}

/*! Compiles a specific shader, usually not called directly
*/
GLuint Shader::compile( GLenum _type, const std::string& _source ){
	if( !GLEW_VERSION_2_0 ) return 0;

	// create a shader object
	GLuint shader = glCreateShader(_type);

	// compile it
	const GLchar* src = _source.c_str();
	const GLint size = _source.size(); 
	glShaderSource(shader, 1, &src, &size);
	glCompileShader(shader);

	//check it
	GLint shader_ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok) {
		addError("Failed to compile shader object:");
		show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

/*! Links the shader program
*/
GLuint Shader::link(){
	if( !GLEW_VERSION_2_0 ) return 0;
	GLint program_ok;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok) {
		addError("Failed to link shader program:");
		show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);

		shader_program = 0;
		return 0;
	} else {
		shader_program = program;
		return program;
	}
}


/* Adds a string to the error log */
void Shader::addError( const std::string& _error ){
	errors += _error + std::string("\n");
}

/*! Retrieves errors from OpenGL */
void Shader::show_info_log(
	GLuint object,
	PFNGLGETSHADERIVPROC glGet__iv,
	PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
	GLint log_length;
	char *log;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = (char *)malloc(log_length);
	glGet__InfoLog(object, log_length, NULL, log);
	addError(log);
	free(log);
}

/* Reads and entire file and returns it as a string */
const std::string Shader::getFileContents( const std::string& file_name ){
	std::ifstream file;
	file.open( file_name.c_str(), std::ios::in );

	if( !file.is_open() || !file.good() ) return std::string("");

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}