/*-----------------------------------------------------------------//
// April 10, 2005                                                  //
//                                Copyright (C) 2005  Justin Walsh //
//                                                                 //
//  This code is Released Under the GNU GPL                        //
//       http://www.gnu.org/copyleft/gpl.html                      //
//                                                                 //
//  Shout out to GDNet+ member: Ranger_One                         //
//-----------------------------------------------------------------*/

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <fstream>
#include <string>
#include <sstream>

using namespace std;

/*-----------------------------------------------------------------//
//Here we have our data types our loader class will need and use...//
//                                                                 */
	struct ObjVertex{
		 float X, Y, Z;
	};

	struct ObjNormal{
		 float X, Y, Z;
	};

	struct ObjTexCoord{
		 float U, V;
	};

	struct ObjTriangle{
		int Vertex[3];
		int Normal[3];
		int TexCoord[3];
	};

	class ObjModel {
		public:
			ObjModel();
			~ObjModel();

			ObjModel(const ObjModel& copy);
			ObjModel& operator=(const ObjModel& right);
			

			int NumVertex, NumNormal, NumTexCoord, NumTriangle;

			ObjVertex *VertexArray;
			ObjNormal *NormalArray;
			ObjTexCoord *TexCoordArray;

			ObjTriangle *TriangleArray;
	};
/*                                                                 //
//-----------------------------------------------------------------*/



/*-----------------------------------------------------------------//
//     The meat of the sandwitch, the class to load .obj files     //
//                                                                 */
	class ObjLoader  {
		public:
			ObjLoader();
			~ObjLoader();

			ObjLoader(string file);
			void LoadObj(string file);
			void FreeObj(void);
			ObjModel ReturnObj(void);

		protected:
			string *fileName;
			ObjModel *theObj;
			
			void ReadData(void);
	};
/*                                                                 //
//-----------------------------------------------------------------*/
#endif