/*
layer 1 maps are all prebuilt flat maps
other layers are added when you break a map you will either break up or down if a map exists in that direction do not add a map
Vertices added from break go on the new map that is made or reposition the ones on existing map.
Vertices added have heights based off first selection's height, first selection must be a used vertex
*/
/***************************************************
***************MAP BUILDINGS CLASS******************
***************************************************/
class mapBuildings
{
};
/***************************************************
***************MAP FOLIAGE CLASS********************
***************************************************/
class mapFoliage
{
};
/***************************************************
***************MAP BUFFER CLASS*********************
***************************************************/
class mapBufferClass
{
public:
	//holds the buffer
	ITerrainSceneNode* terrain;
	vector<int> removedIndices;
	void addBuilding();
	void addFoliage();
	void setxyz(int xIn, int yIn, int zIn);
	void removeIndices();
	void getXYZ(int &xO, int &yO, int &zO);
	void setChanged(bool change);
	bool getChanged();
	mapBufferClass();
	vector<mapBuildings> buildings;
	vector<mapFoliage> foliage;
	bool isPlayerOn, isAccessible;
	//map built here
	vector3df buildPosition;
private:
	//identifies map
	int x,y,z;
	bool changed;

};
void mapBufferClass::setChanged(bool change)
{
	changed=change;
}
bool mapBufferClass::getChanged()
{
	return changed;
}
void mapBufferClass::getXYZ(int &xO, int &yO, int &zO)
{
	xO=x;
	yO=y;
	zO=z;
}
void mapBufferClass::removeIndices()
{
	//remove indices in selection area
}

void mapBufferClass::setxyz(int xIn, int yIn, int zIn)
{
	x=xIn;
	y=yIn;
	z=zIn;
}
mapBufferClass::mapBufferClass()
{
	changed=false;
	isAccessible=false;
	isPlayerOn=false;
}
/***************************************************
***************MAP DATA CLASS***********************
***************************************************/
class mapData
{
public:
	void stretch();
	void getNearestVertex(line3df line, S3DVertex &vertexOut);
	void getNearestVertex(line3df line, int &vertexOut, int &bufferOut);
	void selectVertex(line3df line);
	void useTool(double deltaT);
	void changeTool(int newTool);
	void updateSelectionBox();
	void loadMap(int xIn, int yIn, int zIn, vector3df buildPositionIn, int bufferIndex);
	void unloadMap(int xIn, int yIn, int zIn);
	void saveMap(int xIn, int yIn, int zIn);
	void updateMap();
	void buildMap();
	bool saveMapBool;
	void setNewColor(int r, int g, int b);
	void setMap(int mapx, int mapy, int mapz);
	void saveMaps();
	mapData();
private:
	bool selectionBuilt, mapBuilt;
	int length, toolId, mapX, mapY, mapZ;
	S3DVertex selectionA, selectionB;
	vector<mapBufferClass> mapBuffers;
	int virtualGrid[5][3][5];
	ISceneNode* selectionNode;
	CMeshBuffer<S3DVertex>* selectionBuffer;
	SMesh* selectionMesh;
	SColor setColor;
};
void mapData::saveMaps()
{
	for(int x=0;x<5;x++)
	{
		for(int y=0;y<3;y++)
		{
			for(int z=0;z<5;z++)
			{
				if(y==1||(z>0&&z<4&&x>0&&x<4))
				{
					if(mapBuffers[virtualGrid[x][y][z]].getChanged())
					{
						int xO=0,yO=0,zO=0;
						mapBuffers[virtualGrid[x][y][z]].getXYZ(xO,yO,zO);
						saveMap(xO,yO,zO);
					}
				}
			}
		}
	}
}
void mapData::setMap(int mapx, int mapy, int mapz)
{
	 mapX=mapx;  
	 mapY=mapy;  
	 mapZ=mapz;
}
mapData::mapData()
{
	selectionBuilt=false;
	mapBuilt=false;
	saveMapBool=false;
}

void mapData::setNewColor(int r, int g, int b)
{
	//setColor
	setColor.setRed(r);
	setColor.setGreen(g);
	setColor.setBlue(b);
}

void mapData::buildMap()
{
	if(mapEditor)
	{
		saveMapBool=true;
	}
	setColor.setRed(0);
	setColor.setGreen(0);
	setColor.setBlue(0);
	int q=0;
	for(int x=0;x<5;x++)
	{
		for(int y=0;y<3;y++)
		{
			for(int z=0;z<5;z++)
			{
				if(y==1||(z>0&&z<4&&x>0&&x<4))
				{
					virtualGrid[x][y][z]=q;
					loadMap(mapX+(x-2), mapY+(y-1),mapZ+(z-2),vector3df(mapMaxX*(x-2),y-1,mapMaxZ*(z-2)),q);
					q++;
				}else
				{
					virtualGrid[x][y][z]=-1;
				}
			}
		}
	}
	mapBuilt=true;
}

void mapData::updateMap()
{
	if(mapBuilt)
	{
		//will update maps
		//see if player changed maps
		//emptyNode is the player scene node
		//if player is not inside current maps bounding box
		int x=0,y=0,z=0;

		line3d<f32> testLine;
		testLine.start=Player.playerModel->getPosition();
		testLine.end=testLine.start;
		testLine.end.Y-=500;
		if(!mapBuffers[virtualGrid[2][1][2]].terrain->getBoundingBox().isPointInside(Player.playerModel->getPosition())&&!mapBuffers[virtualGrid[2][1][2]].terrain->getBoundingBox().intersectsWithLine(testLine))
		{
			int newx=-2,newy=-2,newz=-2;
			vector<int> notUsedBuffers(0);
			int newVirtualGrid[5][3][5];
			//determine which way they went using the virtual grid
			for(x=-1;x<2;x++)
			{
				for(y=-1;y<2;y++)
				{
					for(z=-1;z<2;z++)
					{
						//if point is inside map[center+x,y,z]
						//issue may occur if map is inside another maps bounding box, may not load proper maps
						if(mapBuffers[virtualGrid[2+x][1+y][2+z]].terrain->getBoundingBox().isPointInside(Player.playerModel->getPosition()))
						{
							//get translation of the grid
							newx=x;
							newy=y;
							newz=z;
						}
					}
				}
			}
			if(newx==-2&&newy==-2&&newz==-2)
			{
				//failsafe, check if flying over nearby bounding boxes
				
				for(x=-1;x<2;x++)
				{
					for(z=-1;z<2;z++)
					{
						//if point is inside map[center+x,y,z]
						//issue may occur if map is inside another maps bounding box, may not load proper maps
						if(mapBuffers[virtualGrid[2+x][1][2+z]].terrain->getBoundingBox().intersectsWithLine(testLine))
						{
							//get translation of the grid
							newx=x;
							newy=0;
							newz=z;
						}
					}
				}
			}
			
			if(newx!=-2&&newy!=-2&&newz!=-2)
			{
				if(!mapBuffers[virtualGrid[2+newx][1+newy][2+newz]].isAccessible&&!mapEditor)
				{
					//push player back onto accessible map
					//restrict movement in certain direction
				}else
				{
					printf("changing map %i :: direction %i, %i, %i\n",mapBuffers.size(), newx, newy, newz);
					//translate the grid
					for(x=0;x<5;x++)
					{
						for(y=0;y<3;y++)
						{
							for(z=0;z<5;z++)
							{
								if(y+newy<3&&y+newy>-1&&x+newx<5&&x+newx>-1&&z+newz<5&&z+newz>-1)
								{
									//translate the map to new locations
									newVirtualGrid[x][y][z]=virtualGrid[x+newx][y+newy][z+newz];
								}else{
									//move empty slot into the spot
									newVirtualGrid[x][y][z]=-1;
								}
							}
						}
					}

					//"unload" maps that are not supposed to be loaded
					for(x=0;x<5;x++)
					{
						for(y=0;y<3;y++)
						{
							for(z=0;z<5;z++)
							{
								//if map is not supposed to be loaded and isn't already unloaded
								if(y!=1&&((x<1||x>3)||(z<1||z>3))&&newVirtualGrid[x][y][z]!=-1)
								{
									//unload it
									newVirtualGrid[x][y][z]=-1;
								}
							}
						}
					}

					//go through virtual grid and if any buffers are missing from the new one add to remove list
					for(x=0;x<5;x++)
					{
						for(y=0;y<3;y++)
						{
							for(z=0;z<5;z++)
							{
								bool found=false;
								//go through newVirtualGrid
								for(int xq=0;xq<5;xq++)
								{
									for(int yq=0;yq<3;yq++)
									{
										for(int zq=0;zq<5;zq++)
										{
											//if virtualGrid is found in new one say it is found
											if(virtualGrid[x][y][z]==newVirtualGrid[xq][yq][zq])
											{
												found=true;
											}
										}
									}
								}
								//if not found add to remove list
								if(!found)
								{
									notUsedBuffers.push_back(virtualGrid[x][y][z]);	
								}
							}
						}
					}
					//load new maps using the grid and replacing buffers that are flagged
					for(x=0;x<5;x++)
					{
						for(y=0;y<3;y++)
						{
							for(z=0;z<5;z++)
							{
								
								//if map needs to be loaded and is in load area
								if(newVirtualGrid[x][y][z]==-1&&!(y!=1&&((x<1||x>3)||(z<1||z>3))))
								{
									//find a buffer you can use
									int bufferToChange=-1;
									for(int w=0;w<notUsedBuffers.size();w++)
									{
										if(notUsedBuffers[w]!=-1)
										{
											//save buffer we can use and remove from list, end loop
											bufferToChange=notUsedBuffers[w];
											notUsedBuffers[w]=-1;
											w=mapBuffers.size();
										}
									}
									if(bufferToChange!=-1)
									{
										//load new map on the buffer and store it in virtual grid
										if(saveMapBool)
										{
											if(mapBuffers[bufferToChange].getChanged())
											{
												int xO=0,yO=0,zO=0;
												mapBuffers[bufferToChange].getXYZ(xO,yO,zO);
												saveMap(xO,yO,zO);
											}
										}
										newVirtualGrid[x][y][z]=bufferToChange;
										loadMap(mapX+x-2, mapY+y-1, mapZ+z-2,mapBuffers[newVirtualGrid[2][1][2]].buildPosition+vector3df((x-2)*mapMaxX,newy,(z-2)*mapMaxZ),bufferToChange);
									}
								}
								printf("%i | ",newVirtualGrid[x][y][z]);
							}
							printf("\n");
						}
						printf("\n");
					}
					//update virtualGrid
					for(int x=0;x<5;x++)
					{
						for(int y=0;y<3;y++)
						{
							for(int z=0;z<5;z++)
							{
								virtualGrid[x][y][z]=newVirtualGrid[x][y][z];
							}
						}
					}
					//mapMesh->recalculateBoundingBox();
				}
			}
		}
	}
}

void mapData::saveMap(int xIn, int yIn, int zIn)
{
	
	printf("saving a map\n");
	//get file string
	stringc mapFileString;
	char buffer[20];
	sprintf(buffer, "maps/%ix%ix%i.rofh", xIn, yIn, zIn);
	mapFileString=buffer;
	//open file
	int mapNumber=-1, xO=0,yO=0,zO=0;
	for(int x=0;x<mapBuffers.size();x++)
	{
		mapBuffers[x].getXYZ(xO,yO,zO);
		if(xO==xIn&&yO==yIn&&zO==zIn)
		{
			mapNumber=x;
			x=mapBuffers.size();
		}
	}
	ofstream fileOut(mapFileString.c_str());
	if(fileOut.is_open()&&mapNumber>-1)
	{
		//save vertices
		int vertexCount=mapBuffers[mapNumber].terrain->getRenderBuffer()->getVertexCount();
		S3DVertex2TCoords* Vertices=reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[mapNumber].terrain->getRenderBuffer()->getVertices());
		fileOut << vertexCount << ' ';
		//save vertex color
		for(int x=0;x<vertexCount;x++)
		{
			S3DVertex2TCoords* vertex=Vertices+x;
			fileOut << vertex->Pos.Y << ' ';
			fileOut << vertex->Color.getRed() << ' ';
			fileOut << vertex->Color.getGreen() << ' ';
			fileOut << vertex->Color.getBlue() << ' ';
		}
		//save removed indices
		fileOut<< mapBuffers[mapNumber].removedIndices.size()<<' ';
		for(int x=0;x<mapBuffers[mapNumber].removedIndices.size();x++)
		{
			fileOut<< mapBuffers[mapNumber].removedIndices[x]<<' ';
		}
		//save buildings
		fileOut<<mapBuffers[mapNumber].buildings.size()<<' ';
		for(int x=0;x<mapBuffers[mapNumber].buildings.size();x++)
		{
			//fileOut<< mapBuffers[mapNumber].buildings[x];
		}
		//save foliage
		fileOut<<mapBuffers[mapNumber].foliage.size()<<' ';
		for(int x=0;x<mapBuffers[mapNumber].foliage.size();x++)
		{
			//fileOut<< mapBuffers[mapNumber].foliage[x];
		}
		fileOut.close();
	}
	printf("DONE SAVING\n");
}

void mapData::loadMap(int xIn, int yIn, int zIn, irr::core::vector3df buildPositionIn, int bufferIndex)
{
	//get file string
	stringc mapFileString;
	stringc heightFileString;
	char buffer[50];
	sprintf(buffer, "maps/%ix%ix%i.rofh", xIn, yIn, zIn);
	mapFileString=buffer;
	sprintf(buffer, "maps/%ix%ix%i.png", xIn, yIn, zIn);
	heightFileString=buffer;
	if(bufferIndex<0||bufferIndex>=mapBuffers.size())
	{
		//make buffer
		mapBufferClass newBuffer;
		//set x,y,z vars in buffer
		newBuffer.setxyz(xIn, yIn, zIn);
		newBuffer.buildPosition=buildPositionIn;
		
		//open file
		ifstream fileIn(mapFileString.c_str());
		if(fileIn.is_open())
		{
			printf("LOADING");
			newBuffer.isAccessible=true;
			//load in map info
			//load in vertex y positions
			buildPositionIn.Y=0;
			newBuffer.terrain=smgr->addTerrainSceneNode("maps/default.png",0,-1,buildPositionIn);
			newBuffer.terrain->setMaterialFlag(EMF_LIGHTING,false);
			newBuffer.terrain->setMaterialTexture(0,driver->getTexture("textures/dirt_x_43.bmp"));
			//load vertex info
			int vertexCount=0;
			fileIn >> vertexCount;
			S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(newBuffer.terrain->getRenderBuffer()->getVertices());
			for(int x=0;x<vertexCount;x++)
			{	
				S3DVertex2TCoords* vertex=vertices+x;
				int r=100,g=0,b=0;
				double y=0;
				fileIn>>y;
				fileIn>>r;
				fileIn>>g;
				fileIn>>b;
				vertex->Pos.Y=y;
				vertex->Color.setRed(r);
				vertex->Color.setGreen(g);
				vertex->Color.setBlue(b);
			}
			//unload removed indices
			int numIndices=0;
			fileIn>>numIndices;
			for(int x=0;x<numIndices;x++)
			{
				int a=0,b=0,c=0, lastFound=0;
				fileIn>>a;
				fileIn>>b;
				fileIn>>c;
				bool removed=false;
				/*while(!removed)
				{
					lastFound=newBuffer.mapBuffer->Indices.binary_search(a,lastFound, newBuffer.mapBuffer->Indices.size()-1);
					if(newBuffer.mapBuffer->Indices[lastFound+1]==b&&newBuffer.mapBuffer->Indices[lastFound+2]==c)
					{
						//remove
						newBuffer.mapBuffer->Indices[lastFound]=0;
						newBuffer.mapBuffer->Indices[lastFound+1]=0;
						newBuffer.mapBuffer->Indices[lastFound+2]=0;
						removed=true;
					}else
					if(lastFound==-1)
					{
						removed=true;
					}
				}*/
			}
			//load buildings
			int numBuildings=0;
			fileIn>>numBuildings;
			for(int x=0; x<numBuildings;x++)
			{
			}
			//load foliage
			int numFoliage=0;
			fileIn>>numFoliage;
			for(int x=0; x<numFoliage;x++)
			{
			}
			mapBuffers.push_back(newBuffer);
			fileIn.close();
			}else
			{
			//load default map
			//reposition
				printf("DEFAULTING");
				buildPositionIn.Y*=500;
				newBuffer.isAccessible=false;
				newBuffer.terrain=smgr->addTerrainSceneNode("maps/default.png",0,-1,buildPositionIn);
				newBuffer.terrain->setMaterialFlag(EMF_LIGHTING,false);
				newBuffer.terrain->setMaterialTexture(0,driver->getTexture("textures/dirt_x_43.bmp"));
				//newBuffer.terrain->setDebugDataVisible(EDS_BBOX);
				mapBuffers.push_back(newBuffer);
		}
	}else
	{
			//change buffer
		//set x,y,z vars in buffer
		mapBuffers[bufferIndex].setxyz(xIn, yIn, zIn);
		vector3df oldBuildPosition=mapBuffers[bufferIndex].buildPosition;
		mapBuffers[bufferIndex].buildPosition=buildPositionIn;
		
		//open file
		ifstream fileIn(mapFileString.c_str());
		if(fileIn.is_open())
		{
			//load in map info
			//load in vertex y positions
			buildPositionIn.Y=0;
			mapBuffers[bufferIndex].isAccessible=true;
			mapBuffers[bufferIndex].terrain->loadHeightMap(device->getFileSystem()->createAndOpenFile(heightFileString.c_str()));
			mapBuffers[bufferIndex].terrain->setPosition(buildPositionIn);
			//load vertex info
			int vertexCount=0;
			fileIn >> vertexCount;
			S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[bufferIndex].terrain->getRenderBuffer()->getVertices());
			for(int x=0;x<vertexCount;x++)
			{	
				S3DVertex2TCoords* vertex=vertices+x;
				int y=0,r=0,g=0,b=0;
				fileIn>>y;
				fileIn>>r;
				fileIn>>g;
				fileIn>>b;
				vertex->Pos.Y=y;
				vertex->Color.setRed(r);
				vertex->Color.setGreen(g);
				vertex->Color.setBlue(b);
			}
			//unload removed indices
			int numIndices=0;
			fileIn>>numIndices;
			for(int x=0;x<numIndices;x++)
			{
				int a=0,b=0,c=0, lastFound=0;
				fileIn>>a;
				fileIn>>b;
				fileIn>>c;
				bool removed=false;
				/*while(!removed)
				{
					lastFound=mapBuffers[bufferIndex].mapBuffer->Indices.binary_search(a,lastFound, mapBuffers[bufferIndex].mapBuffer->Indices.size()-1);
					if(mapBuffers[bufferIndex].mapBuffer->Indices[lastFound+1]==b&&mapBuffers[bufferIndex].mapBuffer->Indices[lastFound+2]==c)
					{
						//remove
						mapBuffers[bufferIndex].mapBuffer->Indices[lastFound]=0;
						mapBuffers[bufferIndex].mapBuffer->Indices[lastFound+1]=0;
						mapBuffers[bufferIndex].mapBuffer->Indices[lastFound+2]=0;
						removed=true;
					}else
					if(lastFound==-1)
					{
						removed=true;
					}
				}*/
			}
			//load buildings
			int numBuildings=0;
			fileIn>>numBuildings;
			for(int x=0; x<numBuildings;x++)
			{
			}
			//load foliage
			int numFoliage=0;
			fileIn>>numFoliage;
			for(int x=0; x<numFoliage;x++)
			{
			}
			fileIn.close();
		}
		else
		{
			//load default map
			//reposition
			buildPositionIn.Y*=500;
			mapBuffers[bufferIndex].isAccessible=false;
			mapBuffers[bufferIndex].terrain->loadHeightMap(device->getFileSystem()->createAndOpenFile("maps/default.png"));
			mapBuffers[bufferIndex].terrain->setPosition(buildPositionIn);
		}
	}
}

void mapData::updateSelectionBox()
{
	if(selectionA.TCoords.X!=-1)
	{
		selectionNode->setVisible(true);
		vector3df vectorCorner;
		ICursorControl* cursor=device->getCursorControl();
		vectorCorner=smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition()).getClosestPoint(selectionA.Pos);
		//make a transparent square
		SColor color(100,10,100,10);
		selectionBuffer->Vertices.push_back(S3DVertex(selectionA.Pos+vector3df(0,.1,0),vector3df(0,1,0),color,vector2df(0,0)));
		selectionBuffer->Vertices.push_back(S3DVertex(vectorCorner+vector3df(0,.1,0),vector3df(0,1,0),color,vector2df(1,1)));
		selectionMesh->addMeshBuffer(selectionBuffer);
		selectionNode=smgr->addMeshSceneNode(selectionMesh,0,-1,selectionA.Pos);
		selectionBuilt=true;
	}
}

void mapData::changeTool(int newTool)
{
	selectionA.TCoords.X=-1;
	selectionB.TCoords.X=-1;
	length=-1;
	if(selectionBuilt)
	{
		selectionNode->~ISceneNode();
	}
	selectionBuilt=false;
	toolId=newTool;
}

void mapData::useTool(double deltaT)
{
	
	switch(toolId)
	{
	case 1:
		{
			ITimer* temptimer = device->getTimer();
			//change Height (Spherical)
			ICursorControl* cursor=device->getCursorControl();
			int vertexOut=-1, bufferOut=-1;
			vector<int> possibleBuffers(0);
			temptimer->tick();
			printf("start FULL: %i\n", temptimer->getTime());
			line3d<f32> line=smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition());
			getNearestVertex(line,vertexOut, bufferOut);
			S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[bufferOut].terrain->getRenderBuffer()->getVertices());
			S3DVertex2TCoords* vertex=vertices+vertexOut;
			S3DVertex replicaVertex=*vertex;
			if(vertexOut!=-1&&bufferOut!=-1)
			{
				//make a fake cube and see if any other maps are in it to make a list of maps to possibly edit
				if(selectionBuilt)
				{
					selectionNode->~ISceneNode();
				}
				selectionNode=smgr->addSphereSceneNode(sphereRadius,16,0,-1,vertex->Pos);
				//2 delay good as it'll get imo
				for(int x=0;x<mapBuffers.size();x++)
				{
					//mapBuffers[x].mapBuffer->recalculateBoundingBox();
					if(mapBuffers[x].terrain->getBoundingBox().intersectsWithBox(selectionNode->getTransformedBoundingBox()))
					{
						possibleBuffers.push_back(x);
					}
				}
				//edit main buffer
				//buffers
				//16 delay could be better with some good thought
				for(int x=0;x<possibleBuffers.size();x++)
				{
					if(x<mapBuffers.size())
					{
						S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[possibleBuffers[x]].terrain->getRenderBuffer()->getVertices());
						for(int z=0;z<mapBuffers[possibleBuffers[x]].terrain->getRenderBuffer()->getVertexCount();z++)
						{
							S3DVertex2TCoords* vertex=vertices+z;
							double distance=vertex->Pos.getDistanceFromSQ(replicaVertex.Pos);
							if(distance<sphereRadius*sphereRadius)
							{
								mapBuffers[possibleBuffers[x]].setChanged(true);
								vertex->Pos.Y+=(abs(distance-(sphereRadius*sphereRadius))/(sphereRadius*sphereRadius))*heightRate/100*deltaT;
							}
						}
					}
				}
				selectionNode->~ISceneNode();
				selectionBuilt=false;
				temptimer->tick();
				printf("end FULL: %i\n", temptimer->getTime());
			}else
			{
				printf(":(\n");
			}
			break;
		}
	case 2:
		{
			//remove vertices
			mouseButtonState[0]=0;
			ICursorControl* cursor=device->getCursorControl();
			if(selectionA.TCoords.X==-1)
			{
				getNearestVertex(smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition()),selectionA);
			}else if(selectionB.TCoords.X==-1)
			{
				getNearestVertex(smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition()),selectionB);
				//stretch();
			}
			break;
		}
	case 3:
		{
			//change color
			
			/*int vertexOut=-1, bufferOut=-1;
			ICursorControl* cursor=device->getCursorControl();
			line3d<f32> line=smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition());
			getNearestVertex(line,vertexOut, bufferOut);
			if(vertexOut!=-1&&bufferOut!=-1)
			{
				S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[bufferOut].terrain->getRenderBuffer()->getVertices());
				S3DVertex2TCoords* vertex=vertices+vertexOut;
				vertex->Color=setColor;
			}*/

			ITimer* temptimer = device->getTimer();
			//change Height (Spherical)
			ICursorControl* cursor=device->getCursorControl();
			int vertexOut=-1, bufferOut=-1;
			vector<int> possibleBuffers(0);
			temptimer->tick();
			printf("start FULL: %i\n", temptimer->getTime());
			line3d<f32> line=smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition());
			getNearestVertex(line,vertexOut, bufferOut);
			S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[bufferOut].terrain->getRenderBuffer()->getVertices());
			S3DVertex2TCoords* vertex=vertices+vertexOut;
			S3DVertex replicaVertex=*vertex;
			if(vertexOut!=-1&&bufferOut!=-1)
			{
				//make a fake cube and see if any other maps are in it to make a list of maps to possibly edit
				if(selectionBuilt)
				{
					selectionNode->~ISceneNode();
				}
				selectionNode=smgr->addSphereSceneNode(sphereRadius,16,0,-1,vertex->Pos);
				//2 delay good as it'll get imo
				for(int x=0;x<mapBuffers.size();x++)
				{
					//mapBuffers[x].mapBuffer->recalculateBoundingBox();
					if(mapBuffers[x].terrain->getBoundingBox().intersectsWithBox(selectionNode->getTransformedBoundingBox()))
					{
						possibleBuffers.push_back(x);
					}
				}
				//edit main buffer
				//buffers
				//16 delay could be better with some good thought
				for(int x=0;x<possibleBuffers.size();x++)
				{
					if(x<mapBuffers.size())
					{
						S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[possibleBuffers[x]].terrain->getRenderBuffer()->getVertices());
						for(int z=0;z<mapBuffers[possibleBuffers[x]].terrain->getRenderBuffer()->getVertexCount();z++)
						{
							S3DVertex2TCoords* vertex=vertices+z;
							double distance=vertex->Pos.getDistanceFromSQ(replicaVertex.Pos);
							if(distance<sphereRadius*sphereRadius)
							{
								mapBuffers[possibleBuffers[x]].setChanged(true);
								vertex->Color=setColor;
							}
						}
					}
				}
				selectionNode->~ISceneNode();
				selectionBuilt=false;
				temptimer->tick();
				printf("end FULL: %i\n", temptimer->getTime());
			}
			break;
		}
		case 4:
		{
			ITimer* temptimer = device->getTimer();
			//change Height (Spherical)
			ICursorControl* cursor=device->getCursorControl();
			int vertexOut=-1, bufferOut=-1;
			vector<int> possibleBuffers(0);
			temptimer->tick();
			printf("start FULL: %i\n", temptimer->getTime());
			line3d<f32> line=smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition());
			getNearestVertex(line,vertexOut, bufferOut);
			S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[bufferOut].terrain->getRenderBuffer()->getVertices());
			S3DVertex2TCoords* vertex=vertices+vertexOut;
			S3DVertex replicaVertex=*vertex;
			if(vertexOut!=-1&&bufferOut!=-1)
			{
				//make a fake cube and see if any other maps are in it to make a list of maps to possibly edit
				if(selectionBuilt)
				{
					selectionNode->~ISceneNode();
				}
				selectionNode=smgr->addSphereSceneNode(sphereRadius,16,0,-1,vertex->Pos);
				//2 delay good as it'll get imo
				for(int x=0;x<mapBuffers.size();x++)
				{
					//mapBuffers[x].mapBuffer->recalculateBoundingBox();
					if(mapBuffers[x].terrain->getBoundingBox().intersectsWithBox(selectionNode->getTransformedBoundingBox()))
					{
						possibleBuffers.push_back(x);
					}
				}
				//edit main buffer
				//buffers
				//16 delay could be better with some good thought
				for(int x=0;x<possibleBuffers.size();x++)
				{
					if(x<mapBuffers.size())
					{
						S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[possibleBuffers[x]].terrain->getRenderBuffer()->getVertices());
						for(int z=0;z<mapBuffers[possibleBuffers[x]].terrain->getRenderBuffer()->getVertexCount();z++)
						{
							S3DVertex2TCoords* vertex=vertices+z;
							double distance=vertex->Pos.getDistanceFromSQ(replicaVertex.Pos);
							if(distance<sphereRadius*sphereRadius)
							{
								mapBuffers[possibleBuffers[x]].setChanged(true);
								vertex->Pos.Y-=(abs(distance-(sphereRadius*sphereRadius))/(sphereRadius*sphereRadius))*heightRate/100*deltaT;
							}
						}
					}
				}
				selectionNode->~ISceneNode();
				selectionBuilt=false;
				temptimer->tick();
				printf("end FULL: %i\n", temptimer->getTime());
			}else
			{
				printf(":(\n");
			}
			break;
		}
			case 5:
		{
			//change color + random
			
			/*int vertexOut=-1, bufferOut=-1;
			ICursorControl* cursor=device->getCursorControl();
			line3d<f32> line=smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition());
			getNearestVertex(line,vertexOut, bufferOut);
			if(vertexOut!=-1&&bufferOut!=-1)
			{
				S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[bufferOut].terrain->getRenderBuffer()->getVertices());
				S3DVertex2TCoords* vertex=vertices+vertexOut;
				vertex->Color=setColor;
			}*/

			ITimer* temptimer = device->getTimer();
			//change Height (Spherical)
			ICursorControl* cursor=device->getCursorControl();
			int vertexOut=-1, bufferOut=-1;
			vector<int> possibleBuffers(0);
			temptimer->tick();
			printf("start FULL: %i\n", temptimer->getTime());
			line3d<f32> line=smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor->getPosition());
			getNearestVertex(line,vertexOut, bufferOut);
			S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[bufferOut].terrain->getRenderBuffer()->getVertices());
			S3DVertex2TCoords* vertex=vertices+vertexOut;
			S3DVertex replicaVertex=*vertex;
			if(vertexOut!=-1&&bufferOut!=-1)
			{
				//make a fake cube and see if any other maps are in it to make a list of maps to possibly edit
				if(selectionBuilt)
				{
					selectionNode->~ISceneNode();
				}
				selectionNode=smgr->addSphereSceneNode(sphereRadius,16,0,-1,vertex->Pos);
				//2 delay good as it'll get imo
				for(int x=0;x<mapBuffers.size();x++)
				{
					//mapBuffers[x].mapBuffer->recalculateBoundingBox();
					if(mapBuffers[x].terrain->getBoundingBox().intersectsWithBox(selectionNode->getTransformedBoundingBox()))
					{
						possibleBuffers.push_back(x);
					}
				}
				//edit main buffer
				//buffers
				//16 delay could be better with some good thought
				for(int x=0;x<possibleBuffers.size();x++)
				{
					if(x<mapBuffers.size())
					{
						S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[possibleBuffers[x]].terrain->getRenderBuffer()->getVertices());
						for(int z=0;z<mapBuffers[possibleBuffers[x]].terrain->getRenderBuffer()->getVertexCount();z++)
						{
							S3DVertex2TCoords* vertex=vertices+z;
							double distance=vertex->Pos.getDistanceFromSQ(replicaVertex.Pos);
							if(distance<sphereRadius*sphereRadius)
							{
								mapBuffers[possibleBuffers[x]].setChanged(true);
								SColor newColor=setColor;
								newColor.setBlue(newColor.getBlue()+((rand()%10)-5));
								newColor.setRed(newColor.getRed()+((rand()%10)-5));
								newColor.setGreen(newColor.getGreen()+((rand()%10)-5));
								vertex->Color=newColor;
							}
						}
					}
				}
				selectionNode->~ISceneNode();
				selectionBuilt=false;
				temptimer->tick();
				printf("end FULL: %i\n", temptimer->getTime());
			}
			break;
		}
	default:
		//do nothing
		break;
	}
}

void mapData::getNearestVertex(irr::core::line3df line, S3DVertex &vertexOut)
{
	double  distance=100000;
	//recalc all bounding boxes just in case
	/*for(int q=0;q<mapBuffers.size();q++)
	{
		mapBuffers[q].mapBuffer->recalculateBoundingBox();
	}
	//go through each buffer
	for(int q=0;q<mapBuffers.size();q++)
	{
		//3ms delay
		//get buffers whose bounding boxes intersect line since they may be affected
		if(mapBuffers[q].mapBuffer->getBoundingBox().intersectsWithLine(line))
		{
			//3ms delay
			//go through all the vertices
			for(int x=0;x<mapBuffers[q].mapBuffer->Vertices.size();x++)
			{
				//get closest point to the vertex then get its distance
				double newDistance=line.getClosestPoint(mapBuffers[q].mapBuffer->Vertices[x].Pos).getDistanceFrom(mapBuffers[q].mapBuffer->Vertices[x].Pos);
				//if it is closer than last distance save its buffer and vertex info
				if(newDistance<distance)
				{
					distance=newDistance;
					vertexOut=mapBuffers[q].mapBuffer->Vertices[x];
				}
			}
		}
	}*/
}
void mapData::getNearestVertex(irr::core::line3df line, int &vertexOut, int &bufferOut)
{
	double  distance=100000;
	int camDist[5]={0,0,0,0,0};
	int camBuff[5]={0,0,0,0,0};
	//recalc all bounding boxes just in case
	//go through each buffer
	for(int q=0;q<mapBuffers.size();q++)
	{
		//3ms delay
		//get buffers whose bounding boxes intersect line since they may be affected
		if(mapBuffers[q].terrain->getBoundingBox().intersectsWithLine(line))
		{
			//3ms delay
			//go through all the vertices
			int vertexCount=mapBuffers[q].terrain->getRenderBuffer()->getVertexCount();
			S3DVertex2TCoords* Vertices=reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[q].terrain->getRenderBuffer()->getVertices());
			for(int x=0;x<vertexCount;x++)
			{
				S3DVertex2TCoords* vertex=Vertices+x;
				//get closest point to the vertex then get its distance
				double newDistance=line.getClosestPoint(vertex->Pos).getDistanceFrom(vertex->Pos);
				//if it is closer than last distance save its buffer and vertex info
				if(newDistance<distance)
				{
					camDist[5]=camDist[4];
					camDist[4]=camDist[3];
					camDist[3]=camDist[2];
					camDist[2]=camDist[1];
					camDist[1]=camDist[0];
					camDist[0]=x;
					camBuff[5]=camBuff[4];
					camBuff[4]=camBuff[3];
					camBuff[3]=camBuff[2];
					camBuff[2]=camBuff[1];
					camBuff[1]=camBuff[0];
					camBuff[0]=q;
					distance=newDistance;
					vertexOut=x;
					bufferOut=q;
				}
			}
		}
	}
	//go through 5 closest ones to see which is closest to cam
	double camDistance=100000;
	for(int x=0;x<3;x++)
	{
		S3DVertex2TCoords* Vertices=reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[camBuff[x]].terrain->getRenderBuffer()->getVertices());
		S3DVertex2TCoords* vertex=Vertices+camDist[x];
		if(line.start.getDistanceFrom(vertex->Pos)<camDistance)
		{
			vertexOut=camDist[x];
			bufferOut=camBuff[x];
		}
	}
}

void mapData::selectVertex(irr::core::line3df line)
{
	if(selectionBuilt)
	{
		selectionNode->~ISceneNode();
	}
	S3DVertex theVertex;
	getNearestVertex(line, theVertex);
	selectionBuilt=false;
	int vertexOut=-1,bufferOut=-1;
	getNearestVertex(line,vertexOut, bufferOut);
	if(vertexOut!=-1&&bufferOut!=-1)
	{
		S3DVertex2TCoords* vertices = reinterpret_cast<S3DVertex2TCoords *>(mapBuffers[bufferOut].terrain->getRenderBuffer()->getVertices());
		S3DVertex2TCoords* vertex=vertices+vertexOut;
		if(toolId==1||toolId==4)
		{
			selectionNode=smgr->addSphereSceneNode(sphereRadius,16,0,-1,vertex->Pos);
			selectionNode->setMaterialFlag(video::EMF_LIGHTING, false);
			selectionNode->setMaterialFlag(EMF_WIREFRAME,true);
			selectionBuilt=true;
		}
		else
		{
			selectionNode= smgr->addCubeSceneNode(.1,0,-1,vertex->Pos);
			selectionBuilt=true;
		}
	}
}