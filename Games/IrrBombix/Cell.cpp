/*
 * Copyright 2011
 * Copyright 2009 Mathias Kraus <k.hias@gmx.de>
 * Copyright 2007-2008 Thomas Gallinari <tg8187@yahoo.fr>
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

#include "Cell.h"
#include "CellPos.h"
#include "BlockIrrlicht.h"
#include "TerrainIrrlicht.h"
#include "BombExplosionIrrlicht.h"
#include "BombIrrlicht.h"
#include "ElementIrrlicht.h"
#include "irrlicht.h"
#include <iostream>
#include "Bonus/BonusIrrlicht.h"

#include "PlayerIrrlicht.h"

 irr::f32 Cell::SIZE = 40.0;
 //qreal Cell::SIZE = 6.0;

Cell::Cell() :   m_block(0),m_terrain(0),m_Bonus(0)
{
	init();
}

Cell::~Cell()
{
}

void Cell::init(){
	m_players = new irr::core::array<PlayerIrrlicht*>;
	m_terrain=0;
	m_block=0;
	m_Bonus=0;
	m_bomb=0;
}

bool Cell::isWalkable()
{
	//std::cout << "Cell::iswalkable "<< this->getI()<<", "<< this->getJ()<<"\n";
	//std::cout << "Cell::iswalkable "<<m_terrain->isWalkable()<<"\n";
	if (getBlock()==0 && getBomb()==0){
		return m_terrain->isWalkable();
	}else{
		//return (m_terrain->isWalkable() && getBlock()->isWalkable());
		return false;
		//return true;
	}
	return false;
}

bool Cell::isExplodable()
{
	//std::cout << "Cell::isexplodable "<< this->getI()<<", "<< this->getJ()<<"\n";
	//std::cout << "Cell::isexplodable "<<m_terrain->isWalkable()<<"\n";
	if (getBlock()==0){
		return m_terrain->isWalkable();
	}else{
		//return (m_terrain->isWalkable() && getBlock()->isWalkable());

		return false;
	}
	return false;
}


void Cell::addPlayer(PlayerIrrlicht* p){
	m_players->push_back(p);
	/*
	std::cout << "Cell::players.size "<<m_players->size()<<"\n";
	for (int i = 0;i<m_players->size();i++){
		std::cout << "Cell::players "<<getI() << ", "<<getJ()<<" "<<m_players->operator [](i)->getPlayerName().c_str()<<" "<< m_players<<"\n";
	}
	*/
}

void Cell::removePlayer(PlayerIrrlicht* p){
	/*std::cout << "Cell::players remove "<<getI() << ", "<<getJ()<<" "<<p->getPlayerName().c_str()<<" "<< m_players<<"\n";
	for (int i = 0;i<m_players->size();i++){
			std::cout << "Cell::players "<<getI() << ", "<<getJ()<<" "<<m_players->operator [](i)->getPlayerName().c_str()<<" "<< m_players<<"\n";
		}
		*/
	if(m_players->size()>0){
		int pos = m_players->linear_search(p);
		if (pos>=0){
			m_players->erase(pos);
		};
	}
	return;
}


irr::core::array<PlayerIrrlicht*>* Cell::getPlayers(){
	return m_players;
}

BombIrrlicht* Cell::getBomb() {
	return m_bomb;
}

void Cell::setBomb(BombIrrlicht* p_bomb){
	//p_bomb->setI(this->getI());
	//p_bomb->setJ(this->getJ());
	m_bomb=p_bomb;

}

void Cell::removeBomb(){
	m_bomb=0;
}

BonusIrrlicht* Cell::getBonus() {
	return m_Bonus;
}

void Cell::setBonus(BonusIrrlicht* p_Bonus){
	p_Bonus->setI(this->getI());
	p_Bonus->setJ(this->getJ());
	m_Bonus=p_Bonus;
}

void Cell::removeBonus(){
	m_Bonus=0;
}

BlockIrrlicht* Cell::getBlock(){
	return m_block;
}

void Cell::setBlock(BlockIrrlicht* p_block){
	p_block->setI(this->getI());
	p_block->setJ(this->getJ());
	m_block=p_block;

}

void Cell::removeBlock(){
	m_block=0;
}


TerrainIrrlicht* Cell::getTerrain(){
	//std::cout <<"Cell " << m_terrain << " " <<m_terrain->getView();
	return m_terrain;
}

void Cell::setTerrain(TerrainIrrlicht* p_Terrain){
	m_terrain=p_Terrain;
	m_terrain->setI(this->getI());
	m_terrain->setJ(this->getJ());


}

void Cell::removeTerrain(){
	m_terrain=0;
}


BombExplosionIrrlicht* Cell::getExplosion(){
	return m_explosion;
}

void Cell::setExplosion(BombExplosionIrrlicht* p_Explosion){
	p_Explosion->setI(this->getI());
	p_Explosion->setJ(this->getJ());
	m_explosion=p_Explosion;

}

void Cell::removeExplosion(){
	m_explosion=0;
}
