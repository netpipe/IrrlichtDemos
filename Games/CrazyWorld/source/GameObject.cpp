#include "GameObject.h"
#include <math.h>

GameObject::GameObject(IrrlichtDevice* device, std::string name, core::vector3df pos, core::vector3df dir)
{
	moda = "../res/mod/";
	texa = "../res/tex/";
	this->device = device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	this->positionVec = pos;
	this->directionVec = dir;
	this->moda.append(name);
	pNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh(this->moda.append(".obj").c_str()));
	this->texa.append(name);
	pNode->setMaterialTexture(0,driver->getTexture(this->texa.append(".jpg").c_str()));
	pNode->setMaterialFlag(video::EMF_LIGHTING, false);
	pNode->setScale(core::vector3df(1,1,1));
	pNode->setPosition(positionVec);
	//pNode->setDebugDataVisible(scene::EDS_BBOX);
	next = new GameObject();
	this->setID(sin(rand()*123.456));
	this->SetDelFlag(false);

}

GameObject::GameObject()
{
}

GameObject::~GameObject(void)
{
	if(pNode != 0)
		pNode->remove();
}

void GameObject::TickAI()
{
}

void GameObject::SetSpd(double s)
{
	this->move_spd = s;
}

double GameObject::GetSpd()
{
	return move_spd;
}

void GameObject::CheckCollide(GameObject *)
{
}

void GameObject::UpdateSelf()
{
	this->pNode->setPosition(positionVec);
	this->pNode->setRotation(this->directionVec.getHorizontalAngle());
}

void GameObject::change_state(int state)
{
	this->states = state;
}

int GameObject::get_state()
{
	return states;
}

void GameObject::SetType(std::string a)
{
	this->type = a;
}

std::string GameObject::GetType()
{
	return this->type;
}
void GameObject::setID(double id)
{
	uid = id;
}

double GameObject::getID()
{
	return uid;
}

void GameObject::SetOwner(std::string o)
{
	owner = o;
}

std::string GameObject::GetOwner()
{
	return owner;
}

void GameObject::SetDelFlag(bool f)
{
	del_flag = f;
}
	
bool GameObject::GetDelFlag()
{
	return del_flag;
}

int GameObject::GetCurrHP()
{
	return c_hp;
}

void GameObject::SetCurrHP(int a)
{
	this->c_hp = a;
}

int GameObject::GetTotalHP()
{
	return t_hp;
}

void GameObject::SetTotalHP(int a)
{
	this->t_hp = a;
}