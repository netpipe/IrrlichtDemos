/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "BMFontLoader.h"

using namespace phoenix;
using namespace std;
using namespace boost;

/* Loads a FNT file */
void BMFontLoader::load( const std::string& fnt_file ){
	//find the directory (For loading pages)
	std::string directory = fnt_file;
	for( size_t n = 0; (n = directory.find('/', n)) != string::npos; ) directory.replace(n, 1, "\\");
	size_t i = directory.rfind('\\');
	if( i != string::npos ){
		directory = directory.substr(0, i+1);
	} else {
		directory = "";
	}

	// read all of the contents
	std::ifstream in(fnt_file.c_str());
	if( !in ) return;

	std::stringstream buffer;
	buffer << in.rdbuf();

	std::string contents(buffer.str());

	//load it
	loadFromString( contents, directory );

}

/*
Parse a FNT file from a string
*/
void BMFontLoader::loadFromString( const std::string& _data, const std::string& _directory ){
	stringstream stream(_data);
	string line;
	string directory = _directory;

	/* Parse! */
	while( stream.good() ){
		getline( stream, line );
		
		//tokenize, parse.
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(" =");
		tokenizer tokens(line, sep);

		for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
			string token = *it;


			/* Common Properties */
			if( token == "common" ){

				//parse the info.
				for (++it; it != tokens.end(); ++it) {
					string name = *it; 
					if( name == "lineHeight" ){
						font->setLineHeight( (float)atoi( (*++it).c_str() ) );
					}
				}

				break;
			}

			/* Load Page Texture */
			if( token == "page" ){
				unsigned int id = 0;
				TexturePtr t;

				//parse the info.
				for (++it; it != tokens.end(); ++it) {
					string name = *it; 
					if( name == "file" ) {
						std::string filename;

						// gather the filename until we reach the end
						for( ++it; it != tokens.end(); ++it ){
							filename += std::string(" ") + *(it); 
						}
						
						boost::trim_if(filename, boost::is_any_of(" \""));
						t = system.loadTexture(directory + filename);
						break;
					}
					else if( name == "id" ){
						id = (unsigned int)atoi( (*++it).c_str() );
						break;
					}
				}

				font->setPage( id, t );
				font->setTexture( t );

				break;
			}

			/* Load character data */
			if( token == "char" ){
				int id = 0;
				BitmapFont::Character c;

				for (++it; it != tokens.end(); ++it) {
					string name = *it; 
					if( name == "id" ){
						id = atoi( (*++it).c_str() );
					}
					else if( name == "x" ){
						c.x = atoi( (*++it).c_str() );
					}
					else if( name == "y" ){
						c.y = atoi( (*++it).c_str() );
					}
					else if( name == "width" ){
						c.width = atoi( (*++it).c_str() );
					}
					else if( name == "height" ){
						c.height = atoi( (*++it).c_str() );
					}
					else if( name == "xoffset" ){
						c.xoffset = atoi( (*++it).c_str() );
					}
					else if( name == "yoffset" ){
						c.yoffset = atoi( (*++it).c_str() );
					}
					else if( name == "xadvance" ){
						c.xadvance = atoi( (*++it).c_str() );
					}
					else if( name == "page" ){
						c.page = (unsigned int)atoi( (*++it).c_str() );
					}
				}
				
				if( id != 0 ){
					font->setCharacterProperties(id,c);
				}
				
				break;
			}

			/* Load Kerning Pairs */
			if( token == "kerning" ){
				int first = 0;
				int second = 0;
				int amount = 0;

				for (++it; it != tokens.end(); ++it) {
					string name = *it; 
					if( name == "first" ){
						first = atoi( (*++it).c_str() );
					}
					if( name == "second" ){
						second = atoi( (*++it).c_str() );
					}
					if( name == "amount" ){
						amount = atoi( (*++it).c_str() );
					}
				}

				font->setKerning( first, second, amount );
				break;
			}
		}
	}
}