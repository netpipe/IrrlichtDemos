/*-----------------------------------------------------------------//
// April 10, 2005                                                  //
//                                Copyright (C) 2005  Justin Walsh //
//                                                                 //
//  This code is Released Under the GNU GPL                        //
//       http://www.gnu.org/copyleft/gpl.html                      //
//                                                                 //
//  Shout out to GDNet+ member: Ranger_One                         //
//-----------------------------------------------------------------*/

#include "ObjLoader.h"

int i;
/*-----------------------------------------------------------------//
//                      ObjModel Class                             //
//                                                                 */
	ObjModel::ObjModel()  {
		//Set everything to zero and empty...
		ObjModel::NumNormal = 0;
		ObjModel::NumTexCoord = 0;
		ObjModel::NumTriangle = 0;
		ObjModel::NumVertex = 0;
		ObjModel::NormalArray = NULL;
		ObjModel::TexCoordArray = NULL;
		ObjModel::TriangleArray = NULL;
		ObjModel::VertexArray = NULL;
	}

	ObjModel::~ObjModel()  {
		//free up our data
		delete[] NormalArray;
		delete[] TexCoordArray;
		delete[] TriangleArray;
		delete[] VertexArray;
	}

	ObjModel::ObjModel(const ObjModel &copy)  {
		//make room for the new data
		NormalArray = new ObjNormal[copy.NumNormal];
		TexCoordArray = new ObjTexCoord[copy.NumTexCoord];
		TriangleArray = new ObjTriangle[copy.NumTriangle];
		VertexArray = new ObjVertex[copy.NumVertex];
		NumNormal = copy.NumNormal;
		NumTexCoord = copy.NumTexCoord;
		NumTriangle = copy.NumTriangle;
		NumVertex = copy.NumVertex;
	

		//Grab data from the copy to the current class
		for(i = 0; i < NumNormal; i++)
			NormalArray[i] = copy.NormalArray[i];

		for(i = 0; i < NumTexCoord; i++)
			TexCoordArray[i] = copy.TexCoordArray[i];

		for(i = 0; i < NumTriangle; i++)
			TriangleArray[i] = copy.TriangleArray[i];

		for(i = 0; i < NumVertex; i++)
			VertexArray[i] = copy.VertexArray[i];

		//good to go...
	}	

	ObjModel& ObjModel::operator=(const ObjModel &right)  {
		//free current data if we have it
		delete [] NormalArray;
		delete [] TexCoordArray;
		delete [] TriangleArray;
		delete [] VertexArray;

		//make room for the new data
		NormalArray = new ObjNormal[right.NumNormal];
		TexCoordArray = new ObjTexCoord[right.NumTexCoord];
		TriangleArray = new ObjTriangle[right.NumTriangle];
		VertexArray = new ObjVertex[right.NumVertex];
		NumNormal = right.NumNormal;
		NumTexCoord = right.NumTexCoord;
		NumTriangle = right.NumTriangle;
		NumVertex = right.NumVertex;
	

		//Grab data from the copy to the current class
		for(i = 0; i < NumNormal; i++)
			NormalArray[i] = right.NormalArray[i];

		for(i = 0; i < NumTexCoord; i++)
			TexCoordArray[i] = right.TexCoordArray[i];

		for(i = 0; i < NumTriangle; i++)
			TriangleArray[i] = right.TriangleArray[i];

		for(i = 0; i < NumVertex; i++)
			VertexArray[i] = right.VertexArray[i];

		//good to go...
		return *this;
	}
/*                                                                 //
//-----------------------------------------------------------------*/



/*-----------------------------------------------------------------//
//                       ObjLoader Class                           //
//                                                                 */

	ObjLoader::ObjLoader()  {
		fileName = NULL;
		theObj = NULL;
	}

	ObjLoader::~ObjLoader()  {
		FreeObj();
	}

	void ObjLoader::FreeObj(void)  {
		if(fileName != NULL) delete fileName;
		if(theObj != NULL)   delete theObj;
	}

	ObjModel ObjLoader::ReturnObj(void)  {
		ObjModel ret(*theObj);
		return ret;
	}

	ObjLoader::ObjLoader(string file)  {
		fileName = new string(file);
		theObj = new ObjModel();
		ReadData();
	}

	void ObjLoader::LoadObj(string file)  {
		FreeObj();
		fileName = new string(file);
		theObj = new ObjModel();
		ReadData();
	}

	void ObjLoader::ReadData(void)  {
		ifstream input(fileName->c_str());
		string buffer;

		//make sure we got the file opened up ok...
		if( !input.is_open() )
			return;

		//Setup our ObjModel arrays...
		//read one line at a time of the file...
		while( !input.eof() )  {
			getline(input, buffer);

			if(buffer.substr(0,2) == "vn")
				theObj->NumNormal++;
			else if(buffer.substr(0,2) == "vt")
				theObj->NumTexCoord++;
			else if(buffer.substr(0,1) == "v")
				theObj->NumVertex++;
			else if(buffer.substr(0,1) == "f")
				theObj->NumTriangle++;

		}

		//Make the arrays the right size...
		theObj->NormalArray =   new ObjNormal[theObj->NumNormal];
		theObj->TexCoordArray = new ObjTexCoord[theObj->NumTexCoord];
		theObj->TriangleArray = new ObjTriangle[theObj->NumTriangle];
		theObj->VertexArray =   new ObjVertex[theObj->NumVertex];

		//close the file...
		input.close();

		//reopen it...
		input.open(fileName->c_str());

		input.clear();

		//make sure we got the file opened up ok...
		if( !input.is_open() )
			return;

		//hear are the counters...
		int nC, vC, tC, fC;
		nC = vC = tC = fC = 0;

		//get the hard data, and load up our arrays...
		//read one line at a time of the file...
		while( !input.eof() )  {
			getline(input, buffer);
			istringstream line(buffer);
			string temp;
			string f1, f2, f3;
			
			if(buffer.substr(0,2) == "vn")  {
				line >> temp >> f1 >> f2 >> f3;
				theObj->NormalArray[nC].X = atof(f1.c_str());
				theObj->NormalArray[nC].Y = atof(f2.c_str());
				theObj->NormalArray[nC].Z = atof(f3.c_str());
                //sscanf(buffer.c_str(), "vn %f %f %f", theObj->NormalArray[nC].X, 
				//					   theObj->NormalArray[nC].Y, theObj->NormalArray[nC].Z);
				nC++;
			}				
			else if(buffer.substr(0,2) == "vt")  {
				line >> temp >> f1 >> f2;
				theObj->TexCoordArray[tC].U = atof(f1.c_str());
				theObj->TexCoordArray[tC].V = atof(f2.c_str());
				//sscanf(buffer.c_str(), "vt %f %f", theObj->TexCoordArray[tC].U, 
				//					   theObj->TexCoordArray[tC].V);
				tC++;
			}				
			else if(buffer.substr(0,1) == "v")  {
				line >> temp >> f1 >> f2 >> f3;
				theObj->VertexArray[vC].X = atof(f1.c_str());
				theObj->VertexArray[vC].Y = atof(f2.c_str());
				theObj->VertexArray[vC].Z = atof(f3.c_str());
				//sscanf(buffer.c_str(), "v %f %f %f", theObj->VertexArray[vC].X, 
				//					   theObj->VertexArray[vC].Y, theObj->VertexArray[vC].Z);
				vC++;
			}
			else if(buffer.substr(0,1) == "f")  {
				line >> temp >> f1 >> f2 >> f3;
				
				int sPos = 0;
				int ePos = sPos;
				string temp;
				ePos = f1.find_first_of("/");
				//we have a line with the format of "f %d/%d/%d %d/%d/%d %d/%d/%d"
				if(ePos != string::npos)  {
					temp = f1.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].Vertex[0] = atoi(temp.c_str()) - 1;

					sPos = ePos+1;
					ePos = f1.find("/", sPos);
					temp = f1.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].TexCoord[0] = atoi(temp.c_str()) - 1;

					sPos = ePos+1;
					ePos = f1.length();
					temp = f1.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].Normal[0] = atoi(temp.c_str()) - 1;
				}

				sPos = 0;
				ePos = f2.find_first_of("/");
				//we have a line with the format of "f %d/%d/%d %d/%d/%d %d/%d/%d"
				if(ePos != string::npos)  {
					temp = f2.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].Vertex[1] = atoi(temp.c_str()) - 1;

					sPos = ePos + 1;
					ePos = f2.find("/", sPos+1);
					temp = f2.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].TexCoord[1] = atoi(temp.c_str()) - 1;

					sPos = ePos + 1;
					ePos = f2.length();
					temp = f2.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].Normal[1] = atoi(temp.c_str()) - 1;
				}

				sPos = 0;
				ePos = f3.find_first_of("/");
				//we have a line with the format of "f %d/%d/%d %d/%d/%d %d/%d/%d"
				if(ePos != string::npos)  {
					temp = f3.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].Vertex[2] = atoi(temp.c_str()) - 1;

					sPos = ePos + 1;
					ePos = f3.find("/", sPos+1);
					temp = f3.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].TexCoord[2] = atoi(temp.c_str()) - 1;

					sPos = ePos + 1;
					ePos = f3.length();
					temp = f3.substr(sPos, ePos - sPos);
					theObj->TriangleArray[fC].Normal[2] = atoi(temp.c_str()) - 1;
				}
				//sscanf(buffer.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                //          theObj->TriangleArray[fC].Vertex[0], theObj->TriangleArray[fC].TexCoord[0], theObj->TriangleArray[fC].Normal[0],
				//     	  theObj->TriangleArray[fC].Vertex[1], theObj->TriangleArray[fC].TexCoord[1], theObj->TriangleArray[fC].Normal[1],
				//		  theObj->TriangleArray[fC].Vertex[2], theObj->TriangleArray[fC].TexCoord[2], theObj->TriangleArray[fC].Normal[2]);

				fC++;
			}
		}
		//all should be good
	}


/*                                                                 //
//-----------------------------------------------------------------*/

