/*-----------------------------------------------------------------//
// April 10, 2005                                                  //
//                                Copyright (C) 2005  Justin Walsh //
//                                                                 //
//  This code is Released Under the GNU GPL                        //
//       http://www.gnu.org/copyleft/gpl.html                      //
//                                                                 //
//  Shout out to GDNet+ member: Ranger_One                         //
//-----------------------------------------------------------------*/

#include <iostream>
#include "ObjLoader.h"
using namespace std;
int main(void)  {
int i;
	ObjModel data;
	ObjLoader LoaderClass;

	LoaderClass.LoadObj("customCube.obj");
	data = LoaderClass.ReturnObj();

	cout << "Object Model has: " << data.NumTriangle << " faces! \n";
	for(i = 0; i < data.NumVertex; i++)  {
		cout << data.VertexArray[i].X 
			 << " "
			 << data.VertexArray[i].Y 
			 << " " 
			 << data.VertexArray[i].Z 
			 << "\n";
	}
	for(i = 0; i < data.NumTriangle; i++)  {
		cout << "Indacies into faces:\n";
		cout << data.TriangleArray[i].Vertex[0] 
			 << " "
			 << data.TriangleArray[i].Vertex[1]
			 << " " 
			 << data.TriangleArray[i].Vertex[2]
			 << "\n";
	}

	cout << "Vertex from triangle info]:\n";
	cout << data.VertexArray[data.TriangleArray[1].Vertex[0]].X
		 << " "
		 << data.VertexArray[data.TriangleArray[1].Vertex[0]].Y
		 << " "
		 << data.VertexArray[data.TriangleArray[1].Vertex[0]].Z
		 << "\n"
		 << data.VertexArray[data.TriangleArray[1].Vertex[1]].X
		 << " "
		 << data.VertexArray[data.TriangleArray[1].Vertex[1]].Y
		 << " "
		 << data.VertexArray[data.TriangleArray[1].Vertex[1]].Z
		 << "\n"
		 << data.VertexArray[data.TriangleArray[1].Vertex[2]].X
		 << " "
		 << data.VertexArray[data.TriangleArray[1].Vertex[2]].Y
		 << " "
		 << data.VertexArray[data.TriangleArray[1].Vertex[2]].Z
		 << "\n";


	system("PAUSE");
	return 0;
}


