/*********************************************************************
Copyright (c) 2020-2021 TheMrCerebro
http://themrcerebro.com

irrTiled - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#ifndef IRRTILED_H_INCLUDED
#define IRRTILED_H_INCLUDED

#include <irrlicht.h>
#include <string>
#define __STDC_WANT_LIB_EXT1__ 1
#define __STDC_LIB_EXT1__ 1
#include <stdio.h>
#include <stddef.h>
#include <locale.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

enum SHAPE {
	RECT,
	ELLIPSE,
	POINT,
	POLYGON
};

struct PROPERTY {
	bool boolValue;
	s32 intValue;
	s32 objectValue;
	f32 floatValue;
	stringc name;
	stringc type;
	stringc fileValue;
	stringc stringValue;
	SColor colorValue;
};

struct IMAGE {
	s32 imageWidth, imageHeight;
	stringc source;
	ITexture* tex;
};

struct FRAME {
	s32 tileid;
	s32 duration;
};

struct OBJECT {
	s32 id;
	s32 gid;
	s32 x, y;
	s32 rotation;
	s32 width, height;
	array<PROPERTY> properties;
	SHAPE shape;
	array<vector2di> points;
};

struct OBJECTGROUP {
	bool visible;
	s32 id;
	stringc name;
	SColor tintColor;
	array<OBJECT> object;
	array<PROPERTY> properties;
};

struct TILE {
	s32 id;
	array<FRAME> animation;
	array<OBJECTGROUP> objectGroup;
};

struct TILESET {
	s32 firstgid;
	s32 tileWidth, tileHeight;
	s32 tileCount;
	s32 columns;
	stringc source;
	stringc name;
	IMAGE img;
	array<recti> subRects;
	array<TILE> tile;
};

struct LAYER {
	bool visible;
	s32 id;
	s32 width, height;
	stringc name;
	stringc encoding;
	SColor tintColor;
	array<s32> data;
	array<PROPERTY> properties;
};

struct GROUP {
	s32 id;
	stringc name;
	array<PROPERTY> properties;
};

struct IMAGELAYER {
	s32 id;
	stringc name;
	bool visible;
	SColor tintColor;
};

//
class irrTiled
{
public:

	// Constructor
	irrTiled(const c8* filename, IrrlichtDevice* device)
	{
		io::IXMLReader* xml = device->getFileSystem()->createXMLReader(filename);

		// Extrae el nombre del directorio donde esta el archivo tmx
		for (s32 i = 0; i < stringc(filename).findFirst('/')+1; ++i)
			directory += stringc(filename)[i];

		dev = device;

		if (!xml)
			printf("File not exist!!!\n");

		while (xml->read())
		{
			if (xml->getNodeType() == io::EXN_ELEMENT)
			{
				// Read Maps information
				if (stringc(xml->getNodeName()) == "map")
				{
					//read in the key
					version = xml->getAttributeValue(L"version");
					tiledVersion = xml->getAttributeValue(L"tiledversion");
					orientation = xml->getAttributeValue(L"orientation");
					renderOrder = xml->getAttributeValue(L"renderorder");
					width = xml->getAttributeValueAsInt(L"width");
					height = xml->getAttributeValueAsInt(L"height");
					tileWidth = xml->getAttributeValueAsInt(L"tilewidth");
					tileHeight = xml->getAttributeValueAsInt(L"tileheight");
					infinite = xml->getAttributeValueAsInt(L"infinite");
					nextLayerID = xml->getAttributeValueAsInt(L"nextlayerid");
					nextObjectID = xml->getAttributeValueAsInt(L"nextobjectid");
					backgroundColor = HEX2RGB(xml->getAttributeValue(L"backgroundcolor"));

					#if _DEBUG
					printf("MAP[*]\n");
					printf("- Version: %s\n", version.c_str());
					printf("- Tiled Version: %s\n", tiledVersion.c_str());
					printf("- Orientation: %s\n", orientation.c_str());
					printf("- Render Order: %s\n", renderOrder.c_str());
					printf("- Width: %i\n", width);
					printf("- Height: %i\n", height);
					printf("- Tile Width: %i\n", tileWidth);
					printf("- Tile Height: %i\n", tileHeight);
					printf("- Infinite: %i\n", infinite);
					printf("- Next Layer ID: %i\n", nextLayerID);
					printf("- Next Object ID: %i\n", nextObjectID);
					printf("- Background Color: R/%i, G/%i, B/%i\n\n",
						backgroundColor.getRed(),
						backgroundColor.getGreen(),
						backgroundColor.getBlue());
					#endif
				}

				// Read all Properties of map
				if (stringc(xml->getNodeName()) == "properties")
					properties = readProperties(xml);

				// Read Tileset information
				if (stringc(xml->getNodeName()) == "tileset")
				{
					TILESET ts;

					ts.firstgid = xml->getAttributeValueAsInt(L"firstgid");

					// If the tileset uses a ".TSX" file, a external tileset
					ts.source = xml->getAttributeValue(L"source");
					if (!ts.source.empty())
					{
						io::IXMLReader* tsx = device->getFileSystem()->createXMLReader(directory+ts.source);
						while (tsx->read())
						{
							if (tsx->getNodeType() == io::EXN_ELEMENT)
							{
								// Read Tileset information from .tsx file
								if (stringc(tsx->getNodeName()) == "tileset")
								{
									ts.name = tsx->getAttributeValue(L"name");
									ts.tileWidth = tsx->getAttributeValueAsInt(L"tilewidth");
									ts.tileHeight = tsx->getAttributeValueAsInt(L"tileheight");
									ts.tileCount = tsx->getAttributeValueAsInt(L"tilecount");
									ts.columns = tsx->getAttributeValueAsInt(L"columns");

									while (tsx->read())
									{
										if (tsx->getNodeType() == io::EXN_ELEMENT)
										{
											// Read Image information
											if (stringc(tsx->getNodeName()) == "image")
												ts.img = readImages(tsx);

											// Read Tile information
											if (stringc(tsx->getNodeName()) == "tile")
											{
												TILE tld;
												tld.id = tsx->getAttributeValueAsInt(L"id");

												#if _DEBUG
												printf("TILE [*]\n");
												printf("- ID: %i\n\n", tld.id);
												#endif

												while (tsx->read())
												{
													if (tsx->getNodeType() == io::EXN_ELEMENT)
													{
														// Read Object Groups information
														if (stringc(tsx->getNodeName()) == "objectgroup")
															tld.objectGroup.push_back(readObjectGroups(tsx));

														// Read Animations information
														if (stringc(tsx->getNodeName()) == "animation")
														{
															#if _DEBUG
															printf("ANIMATION [*]\n\n");
															#endif

															while (tsx->read())
															{
																if (tsx->getNodeType() == io::EXN_ELEMENT)
																{
																	// Read Frames information
																	if (stringc(tsx->getNodeName()) == "frame")
																	{
																		FRAME frm;

																		frm.tileid = tsx->getAttributeValueAsInt(L"tileid");
																		frm.duration = tsx->getAttributeValueAsInt(L"duration");

																		#if _DEBUG
																		printf("FRAME [*]\n");
																		printf("- TileID: %i\n", frm.tileid);
																		printf("- Duration: %i\n\n", frm.duration);
																		#endif

																		tld.animation.push_back(frm);
																	}
																}
																else
																	break;
															}
														}
													}
													else
														break;
												}

												ts.tile.push_back(tld);
											}
										}
										else
											break;
									}
								}
							}
						}
						tsx->drop();
					}
					else
					{
						ts.name = xml->getAttributeValue(L"name");
						ts.tileWidth = xml->getAttributeValueAsInt(L"tilewidth");
						ts.tileHeight = xml->getAttributeValueAsInt(L"tileheight");
						ts.tileCount = xml->getAttributeValueAsInt(L"tilecount");
						ts.columns = xml->getAttributeValueAsInt(L"columns");

						while (xml->read())
						{
							// Read Image information
							if (xml->getNodeType() == io::EXN_ELEMENT)
							{
								if (stringc(xml->getNodeName()) == "image")
									ts.img = readImages(xml);
							}
							else
								break;
						}
					}

					#if _DEBUG
					printf("TILESET[*]\n");
					printf("- First GID: %i\n", ts.firstgid);
					printf("- Source: %s\n", ts.source.c_str());
					printf("- Name: %s\n", ts.name.c_str());
					printf("- Tile Width: %i\n", ts.tileWidth);
					printf("- Tile Height: %i\n", ts.tileHeight);
					printf("- Tile Count: %i\n", ts.tileCount);
					printf("- Columns: %i\n\n", ts.columns);
					#endif

					// tiles/subrects are counted from 0, left to right, top to bottom
					for (s32 y = 0; y < ts.tileCount/ts.columns; ++y)
					{
						for (s32 x = 0; x < ts.columns; ++x)
							ts.subRects.push_back(recti(vector2di(x * ts.tileWidth, y * ts.tileHeight), dimension2du(ts.tileWidth, ts.tileHeight)));
					}

					tileset.push_back(ts);
				}

				// Read Layer information
				if (stringc(xml->getNodeName()) == "layer")
					layer.push_back(readLayers(xml));

				// Read Object Groups information
				if (stringc(xml->getNodeName()) == "objectgroup")
					objectGroup.push_back(readObjectGroups(xml));

				// Read Image Layer information
				if (stringc(xml->getNodeName()) == "imagelayer")
					imageLayer.push_back(readImageLayers(xml));

				// Read Group information
				if (stringc(xml->getNodeName()) == "group")
					group.push_back(readGroups(xml));
			}
		}

		xml->drop();
	}

	// Clear all :-(
	~irrTiled()
	{
		properties.clear();
		tileset.clear();
		layer.clear();
		objectGroup.clear();
		group.clear();
		imageLayer.clear();
	}

	// Variables
	stringc version;
	stringc tiledVersion;
	stringc orientation;
	stringc renderOrder;
	s32 width=0, height=0;
	s32 tileWidth=0, tileHeight=0;
	s32 infinite=0;
	s32 nextLayerID=0;
	s32 nextObjectID=0;
	SColor backgroundColor;
	array<PROPERTY> properties;
	array<TILESET> tileset;
	array<LAYER> layer;
	array<OBJECTGROUP> objectGroup;
	array<GROUP> group;
	array<IMAGELAYER> imageLayer;

private:

	// Convert from hex to rgb
	SColor HEX2RGB(stringc value)
	{
		SColor color(255, 255, 255, 255);
		stringc col = value.remove("#");
		if (!col.empty())
		{
			color = std::stoul(col.c_str(), nullptr, 16);
			color.setAlpha(255);
		}
		return color;
	}

	// Read Image Layer information
	IMAGELAYER readImageLayers(io::IXMLReader* xml)
	{
		IMAGELAYER iml;

		iml.id = xml->getAttributeValueAsInt(L"id");
		iml.name = xml->getAttributeValue(L"name");
		iml.visible = xml->getAttributeValueAsInt(L"visible") ? true : false;
		iml.tintColor = HEX2RGB(xml->getAttributeValue(L"tintcolor"));

		#if _DEBUG
		printf("IMAGE LAYER[*]\n");
		printf("- ID: %i\n", iml.id);
		printf("- Name: %s\n", iml.name.c_str());
		printf("- Visible: %s\n", iml.visible ? "True" : "False");
		printf("- Tint Color: R/%i, G/%i, B/%i\n\n",
			iml.tintColor.getRed(),
			iml.tintColor.getGreen(),
			iml.tintColor.getBlue());
		#endif

		return iml;
	}

	// Read Group information
	GROUP readGroups(io::IXMLReader* xml)
	{
		GROUP grp;

		grp.id = xml->getAttributeValueAsInt(L"id");
		grp.name = xml->getAttributeValue(L"name");

		#if _DEBUG
		printf("GROUP[*]\n");
		printf("- ID: %i\n", grp.id);
		printf("- Name: %s\n", grp.name.c_str());
		#endif

		if (!xml->isEmptyElement())
		{
			while (xml->read())
			{
				if (xml->getNodeType() == io::EXN_ELEMENT)
				{
					// Read all Properties of map
					if (stringc(xml->getNodeName()) == "properties")
						grp.properties = readProperties(xml);

					// Read Layer information
					if (stringc(xml->getNodeName()) == "layer")
						layer.push_back(readLayers(xml));

					// Read Object Groups information
					if (stringc(xml->getNodeName()) == "objectgroup")
						objectGroup.push_back(readObjectGroups(xml));

					// Read Image Layer information
					if (stringc(xml->getNodeName()) == "imagelayer")
						imageLayer.push_back(readImageLayers(xml));

					// Read Groups information
					if (stringc(xml->getNodeName()) == "group")
						group.push_back(readGroups(xml));
				}
			}
		}

		return grp;
	}

	// Read Layer information
	LAYER readLayers(io::IXMLReader* xml)
	{
		LAYER lyr;

		lyr.id = xml->getAttributeValueAsInt(L"id");
		lyr.name = xml->getAttributeValue(L"name");
		lyr.width = xml->getAttributeValueAsInt(L"width");
		lyr.height = xml->getAttributeValueAsInt(L"height");
		lyr.visible = xml->getAttributeValueAsInt(L"visible") ? true : false;
		lyr.tintColor = HEX2RGB(xml->getAttributeValue(L"tintcolor"));

		#if _DEBUG
		printf("LAYER[*]\n");
		printf("- ID: %i\n", lyr.id);
		printf("- Name: %s\n", lyr.name.c_str());
		printf("- Width: %i\n", lyr.width);
		printf("- Height: %i\n", lyr.height);
		printf("- Visible: %s\n", lyr.visible ? "True" : "False");
		printf("- Tint Color: R/%i, G/%i, B/%i\n\n",
			lyr.tintColor.getRed(),
			lyr.tintColor.getGreen(),
			lyr.tintColor.getBlue());
		#endif

		while (xml->read())
		{
			if (xml->getNodeType() == io::EXN_ELEMENT)
			{
				// Read Data
				if (stringc(xml->getNodeName()) == "data")
				{
					lyr.encoding = xml->getAttributeValue(L"encoding");

					// Read XML data (deprecated)
					if(lyr.encoding.empty())
					{
						while (xml->read())
						{
							if (xml->getNodeType() == io::EXN_ELEMENT)
							{
								if (stringc(xml->getNodeName()) == "tile")
									lyr.data.push_back(xml->getAttributeValueAsInt(L"gid"));
							}
							else
								break;
						}
					}

					#if _DEBUG
					printf("DATA[*]\n");
					printf("- Encoding: %s\n", lyr.encoding.c_str());
					#endif
				}

				// Read all Properties (if exist)
				if (stringc(xml->getNodeName()) == "properties")
					lyr.properties = readProperties(xml);
			}
			else if (xml->getNodeType() == io::EXN_TEXT)
			{
				stringc data = xml->getNodeName();

				#if _DEBUG
				printf("- Data: %s\n", data.c_str());
				#endif

				// Read CSV data
				if (lyr.encoding == "csv")
				{
					const c8* ptr = data.c_str();
					while (true)
					{
						c8* end;
						u32 res = strtoul(ptr, &end, 10);
						if (end == ptr) break;
						ptr = end;
						lyr.data.push_back(res);
						if (*ptr == ',') ++ptr;
					}
				}
			}
			else
				break;
		}

		return lyr;
	}

	// Read image information
	IMAGE readImages(io::IXMLReader* xml)
	{
		IMAGE img;

		img.source = xml->getAttributeValue(L"source");
		img.tex = dev->getVideoDriver()->getTexture(directory+img.source);
		img.imageWidth = xml->getAttributeValueAsInt(L"width");
		img.imageHeight = xml->getAttributeValueAsInt(L"height");

		// Make color transparent
		SColor col = HEX2RGB(xml->getAttributeValue(L"trans"));
		dev->getVideoDriver()->makeColorKeyTexture(img.tex, col);

		#if _DEBUG
		printf("IMAGE[*]\n");
		printf("- source: %s\n", img.source.c_str());
		printf("- Width: %i\n", img.imageWidth);
		printf("- Height: %i\n", img.imageHeight);
		printf("- Tansparent: R/%i, G/%i, B/%i\n\n",col.getRed(),col.getGreen(),col.getBlue());
		#endif

		return img;
	}

	// Read all properties
	array<PROPERTY> readProperties(io::IXMLReader* xml)
	{
		array<PROPERTY> pro;
		while (xml->read())
		{
			if (xml->getNodeType() == io::EXN_ELEMENT)
			{
				if (stringc(xml->getNodeName()) == "property")
				{
					PROPERTY pry;

					pry.name = xml->getAttributeValue(L"name");
					pry.type = xml->getAttributeValue(L"type");

					#if _DEBUG
					printf("PROPERTY[*]\n");
					printf("Name: %s\n", pry.name.c_str());
					printf("Type: %s\n", pry.type.c_str());
					#endif

					if (pry.type == "bool")
					{
						pry.boolValue = xml->getAttributeValueAsInt(L"value") ? false : true;
						#if _DEBUG
						printf("Value: %s\n", pry.boolValue ? "False" : "True");
						#endif
					}
					else if (pry.type == "float")
					{
						pry.floatValue = xml->getAttributeValueAsFloat(L"value");
						#if _DEBUG
						printf("Value: %.3f\n", pry.floatValue);
						#endif
					}
					else if (pry.type == "int")
					{
						pry.intValue = xml->getAttributeValueAsInt(L"value");
						#if _DEBUG
						printf("Value: %i\n", pry.intValue);
						#endif
					}
					else if (pry.type == "object")
					{
						pry.objectValue = xml->getAttributeValueAsInt(L"value");
						#if _DEBUG
						printf("Value: %i\n", pry.objectValue);
						#endif
					}
					else if (pry.type == "file")
					{
						pry.fileValue = xml->getAttributeValue(L"value");
						#if _DEBUG
						printf("Value: %s\n", pry.fileValue.c_str());
						#endif
					}
					else if (pry.type == "color")
					{
						pry.colorValue = HEX2RGB(xml->getAttributeValue(L"value"));
						#if _DEBUG
						printf("Value: R/%i, G/%i, B/%i\n",pry.colorValue.getRed(),pry.colorValue.getGreen(),pry.colorValue.getBlue());
						#endif
					}
					else
					{
						pry.stringValue = xml->getAttributeValue(L"value");
						#if _DEBUG
						printf("Value: %s\n", pry.stringValue.c_str());
						#endif
					}

					printf("\n");
					pro.push_back(pry);
				}
			}
			else
				break;
		}
		return pro;
	}

	// Read Object Groups information
	OBJECTGROUP readObjectGroups(io::IXMLReader* xml)
	{
		OBJECTGROUP og;

		og.id = xml->getAttributeValueAsInt(L"id");
		og.name = xml->getAttributeValue(L"name");
		og.visible = xml->getAttributeValueAsInt(L"visible") ? true : false;
		og.tintColor = HEX2RGB(xml->getAttributeValue(L"tintcolor"));

		#if _DEBUG
		printf("OBJECT GROUP[*]\n");
		printf("- ID: %i\n", og.id);
		printf("- Name: %s\n", og.name.c_str());
		printf("- Visible: %s\n", og.visible ? "True" : "False");
		printf("- Tint Color: R/%i, G/%i, B/%i\n\n",
			og.tintColor.getRed(),
			og.tintColor.getGreen(),
			og.tintColor.getBlue());
		#endif

		// Read Objects information
		if (!xml->isEmptyElement())
		{
			while (xml->read())
			{
				if (xml->getNodeType() == io::EXN_ELEMENT)
				{
					// Read all Properties
					if (stringc(xml->getNodeName()) == "properties")
						og.properties = readProperties(xml);

					// Read all objects
					if (stringc(xml->getNodeName()) == "object")
					{
						OBJECT obj;

						obj.id = xml->getAttributeValueAsInt(L"id");
						obj.gid = xml->getAttributeValueAsInt(L"gid");
						obj.x = xml->getAttributeValueAsInt(L"x");
						obj.y = xml->getAttributeValueAsInt(L"y");
						obj.width = xml->getAttributeValueAsInt(L"width");
						obj.height = xml->getAttributeValueAsInt(L"height");
						obj.rotation = xml->getAttributeValueAsInt(L"rotation");
						obj.shape = SHAPE::RECT;

						#if _DEBUG
						printf("OBJECT[*]\n");
						printf("- ID: %i\n", obj.id);
						printf("- X: %i\n", obj.x);
						printf("- Y: %i\n", obj.y);
						printf("- Rotation: %i\n", obj.rotation);
						printf("- Width: %i\n", obj.width);
						printf("- Height: %i\n\n", obj.height);
						#endif

						// Read all Properties (if exist)
						if (!xml->isEmptyElement())
						{
							while (xml->read())
							{
								if (xml->getNodeType() == io::EXN_ELEMENT)
								{
									if (stringc(xml->getNodeName()) == "properties")
										obj.properties = readProperties(xml);

									if (stringc(xml->getNodeName()) == "ellipse")
										obj.shape = SHAPE::ELLIPSE;

									if (stringc(xml->getNodeName()) == "point")
										obj.shape = SHAPE::POINT;

									if (stringc(xml->getNodeName()) == "polygon")
									{
										obj.shape = SHAPE::POLYGON;

										array<stringc> ac;
										stringc(xml->getAttributeValue(L"points")).split(ac, " ", 4);

										for (u32 x = 0; x < ac.size(); ++x)
										{
											vector2di pos;
											sscanf(ac[x].c_str(), "%i,%i", &pos.X, &pos.Y);
											obj.points.push_back(pos);
											#if _DEBUG
											printf("Point: %i,%i\n", pos.X, pos.Y);
											#endif
										}
									}
								}
								else
									break;
							}
						}

						og.object.push_back(obj);
					}
				}
				else
					break;
			}
		}

		return og;
	}

private:

	stringc directory;
	irr::IrrlichtDevice* dev;

};

#endif // IRRTILED_H_INCLUDED
