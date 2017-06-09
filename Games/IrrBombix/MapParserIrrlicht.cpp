/*
 * Copyright 2011
 * Copyright 2009 Mathias Kraus <k.hias@gmx.de>
 * Copyright 2007-2008 Thomas Gallinari <tg8187@yahoo.fr>
 * Copyright 2007-2008 Pierre-Benoît Besse <besse.pb@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tinyxml/tinyxml.h"
//#include <tinyxml.h>
#include <iostream>
#include <stdlib.h>
#include "MapParserIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "WallIrrlicht.h"
#include "GroundIrrlicht.h"
#include "BlockIrrlicht.h"

#include "Bonus/BonusBombIrrlicht.h"
#include "Bonus/BonusfireIrrlicht.h"
#include "Bonus/BonusspeedIrrlicht.h"
#include "Bonus/BonusslowIrrlicht.h"
#include "Bonus/BonushyperactiveIrrlicht.h"
#include "Bonus/BonusrestrainIrrlicht.h"
#include "Bonus/BonusmirrorIrrlicht.h"
#include "Bonus/BonusscattyIrrlicht.h"
#include "Bonus/BonuskickIrrlicht.h"
#include "Bonus/BonusthrowIrrlicht.h"

#include "States/StateOptionsboard.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MapparserIrrlicht::MapparserIrrlicht() {
	valid = false;
	mcounterRows = 0;
	arena = new ArenaIrrlicht();
	srand( time(NULL));

}
;
MapparserIrrlicht::~MapparserIrrlicht() {
}
;

bool MapparserIrrlicht::handleElement(TiXmlElement* node) {

	if (valid == false) {
		std::string str1("Arena");
		// if (str1.compare(node->Value())==0) {
		if (str1 == node->Value()) {
			int nbRows = 0;
			int nbColumns = 0;
			// Initialize the number of rows and columns

			TiXmlAttribute* pAttrib = node->FirstAttribute();

			while (pAttrib) {
				str1 = "rowCount";
				if (pAttrib->Name() == str1) {
					if (pAttrib->QueryIntValue(&nbRows) == TIXML_SUCCESS) {
						printf(" nbRows=%d ", nbRows);
					} else {
						printf(" clould not parse to int %s ", pAttrib->Name());
					};
				}
				str1 = "colCount";
				if (pAttrib->Name() == str1) {
					if (pAttrib->QueryIntValue(&nbColumns) == TIXML_SUCCESS) {
						printf(" nbColumns=%d ", nbColumns);
					} else {
						printf(" clould not parse to int %s ", pAttrib->Name());
					};
				}

				pAttrib = pAttrib->Next();
			}

			// Create the Arena matrix
			//m_arena->init(nbRows, nbColumns);

			arena->init(nbRows, nbColumns);

			mcounterRows = nbRows - 1;

			// initialize random generator
			//qsrand(QDateTime::currentDateTime().toTime_t());
			valid = true;
			return true;
		} else {
			printf("no valid Arena file\n");
			return false;
		}

	} else {
		std::string viewtext("view");
		ArenaItemIrrlicht* view =
				dynamic_cast<ArenaItemIrrlicht*> (arena->getView());

		if (node->Value() == viewtext) {
			TiXmlAttribute* pAttrib = node->FirstAttribute();
			while (pAttrib) {
				std::string str1 = "meshname";
				if (pAttrib->Name() == str1) {
					view->meshname = pAttrib->Value();
					std::cout << "Mapparser " << view->meshname << "\n\n";
				}
				str1 = "ipx";
				if (pAttrib->Name() == str1) {
					std::stringstream sstr(pAttrib->Value());
					sstr >> view->ipx;
				}
				str1 = "ipy";
				if (pAttrib->Name() == str1) {
					std::stringstream sstr(pAttrib->Value());
					sstr >> view->ipy;
				}
				str1 = "ipz";
				if (pAttrib->Name() == str1) {
					std::stringstream sstr(pAttrib->Value());
					sstr >> view->ipz;
				}
				str1 = "irx";
				if (pAttrib->Name() == str1) {
					std::stringstream sstr(pAttrib->Value());
					sstr >> view->irx;
				}
				str1 = "iry";
				if (pAttrib->Name() == str1) {
					std::stringstream sstr(pAttrib->Value());
					sstr >> view->iry;
				}
				str1 = "irz";
				if (pAttrib->Name() == str1) {
					std::stringstream sstr(pAttrib->Value());
					sstr >> view->irz;
				}
				str1 = "modellfieldsize";
				if (pAttrib->Name() == str1) {
					std::stringstream sstr(pAttrib->Value());
					sstr >> view->modellfieldsize;
				}

				pAttrib = pAttrib->Next();
			}

		}

		std::stringstream ss1;
		ss1.str(StateOptionsboard::stateoptionsboard->getValue("Rock Density"));
		int rockdensity;
		ss1 >> rockdensity;
		rockdensity = (10 - rockdensity) * 10;

		std::stringstream ss2;
		ss2.str(StateOptionsboard::stateoptionsboard->getValue("Bonus Density"));
		int Bonusdensity;
		ss2 >> Bonusdensity;
		Bonusdensity = (10 - Bonusdensity) * 10;

		std::stringstream ss3;
		ss3.str(StateOptionsboard::stateoptionsboard->getValue("Bad Bonus"));
		int Bonusitems = 13;
		if (ss3.str() == "off"){
			Bonusitems = 13 - 5;
		}

		std::string rowtext("Row");

		if (node->Value() == rowtext) {
			TiXmlNode* txtNode = node->FirstChild();

			//char* rowtext;
			if (txtNode->Type() == TiXmlNode::TINYXML_TEXT) {
				//rowtext = new std::string((char*)txtNode->ToText());
				rowtext = txtNode->Value();
				//rowtext = (char*) txtNode->ToText();
			}

			for (int i = 0; i < rowtext.size(); ++i) {
				//for (int i = 0; i < strlen(rowtext); ++i) {
				int tmp = static_cast<int> (rowtext[i]);
				//std::cout <<"mapparser rowtext "<< rowtext[i] << " \n";
				switch (tmp) {
				case ' ':
					//m_arena->setCellType(m_counterRows,i,Cell::HOLE);

					break;
				case '=':
					arena->getCell(i, mcounterRows)->setTerrain(
							new WallIrrlicht);
					//std::cout <<"mapparser is wall walkable "<<arena->getCell(i, mcounterRows)->isWalkable()<<"\n";
					//std::cout <<"mapparser is wall walkable mr,i "<< mcounterRows<<","<<i<<" \n";
					//std::cout <<"mapparser is wall walkable "<<arena->getCell(i, mcounterRows)<<"\n";
					//m_arena->setCellType(m_counterRows,i,Cell::WALL);
					break;
				case '_':
					arena->getCell(i, mcounterRows)->setTerrain(
							new GroundIrrlicht);
					//arena->getCell(i, mcounterRows)->setBonus(new BonushyperactiveIrrlicht);
					//m_arena->setCellType(m_counterRows,i,Cell::GROUND);
					break;
				case '+':
					arena->getCell(i, mcounterRows)->setBlock(new BlockIrrlicht);
					//m_arena->setCellType(m_counterRows,i,Cell::BLOCK);
					break;
				case 'x':

					if ((rand() % 100) > rockdensity) {
						arena->getCell(i, mcounterRows)->setBlock(
								new BlockIrrlicht);
						if ((rand() % 100) > Bonusdensity) {

							int doitem = rand() % (Bonusitems);
							/*std::cout << "MapParserIrrlicht doitem " << doitem
									<< "on " << i << ", " << mcounterRows
									<< "\n";
									*/
							switch (doitem) {

							case 0:
							case 1:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonusBombIrrlicht);
								break;
							case 2:
							case 3:
							case 4:

								arena->getCell(i, mcounterRows)->setBonus(
										new BonusfireIrrlicht);
								break;
							case 5:
							case 6:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonusspeedIrrlicht);
								break;
							case 7:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonuskickIrrlicht);
								break;
							case 8:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonusthrowIrrlicht);
								break;
							case 9:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonusslowIrrlicht);
								break;
							case 10:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonushyperactiveIrrlicht);
								break;
							case 11:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonusrestrainIrrlicht);
								break;
							case 12:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonusmirrorIrrlicht);
								break;
							case 13:
								arena->getCell(i, mcounterRows)->setBonus(
										new BonusscattyIrrlicht);
								break;
							default:

								// nothing
								break;
							}
						}
						//std::cout << "MapParserIrrlicht Bonus i,j "<<arena->getCell(i,mcounterRows)->getBonus()->getI()<< " "<<arena->getCell(i,mcounterRows)->getBonus()->getJ()<<"\n";
					}
					;

					arena->getCell(i, mcounterRows)->setTerrain(
							new GroundIrrlicht);
					break;
				case 'o':
					//m_arena->setCellType(m_counterRows,i,Cell::BOMBMORTAR);
					arena->getCell(i, mcounterRows)->setTerrain(
							new GroundIrrlicht);
					break;
				case '-':
					//m_arena->setCellType(m_counterRows,i,Cell::ICE);
					arena->getCell(i, mcounterRows)->setTerrain(
							new GroundIrrlicht);
					break;
				case 'u':
					//m_arena->setCellType(m_counterRows,i,Cell::ARROWUP);
					break;
				case 'r':
					//m_arena->setCellType(m_counterRows,i,Cell::ARROWRIGHT);
					break;
				case 'd':
					//m_arena->setCellType(m_counterRows,i,Cell::ARROWDOWN);
					break;
				case 'l':
					//m_arena->setCellType(m_counterRows,i,Cell::ARROWLEFT);
					break;
				case 'p':
					arena->getCell(i, mcounterRows)->setTerrain(
							new GroundIrrlicht);
					std::cout << "mapparser is startposion walkable "
							<< arena->getCell(i, mcounterRows)->isWalkable()
							<< "\n";
					arena->addPlayerPosition(i, mcounterRows);
					//m_arena->setCellType(m_counterRows,i,Cell::GROUND);
					//m_arena->addPlayerPosition(QPointF(i+0.5, m_counterRows+0.5));
					break;
				default:
					arena->getCell(i, mcounterRows)->setTerrain(
							new GroundIrrlicht);
					//m_arena->setCellType(m_counterRows,i,Cell::GROUND);
				}
			}

			mcounterRows--;

		}
		return true;
	}
}

void MapparserIrrlicht::parseNode(TiXmlNode* pParent, unsigned int indent = 0) {
	if (!pParent)
		return;

	static bool valid = false;
	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	//printf( "%s", getIndent(indent));
	int num;

	switch (t) {
	case TiXmlNode::TINYXML_ELEMENT: {
		//printf("Element [%s]\n", pParent->Value());
		handleElement(pParent->ToElement());
		break;
	}
	default:
		break;
	}
	//printf( "\n" );
	for (pChild = pParent->FirstChild(); pChild != 0; pChild
			= pChild->NextSibling()) {
		parseNode(pChild, indent + 1);
	}
}

bool MapparserIrrlicht::parse(const char* pFilename) {

	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay) {
		printf("\n%s:\n", pFilename);
		parseNode(&doc, 0);
	} else {
		printf("Failed to load file \"%s\"\n", pFilename);
	}

	return true;
}

ArenaIrrlicht* MapparserIrrlicht::getArena() const {
	return arena;
}
