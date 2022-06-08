/*
(c) 2012 Nicolaus Anderson
*/

#include <irrlicht.h>

using namespace irr;

#pragma once

class irrTreeElement
{
public:
	virtual bool matches( irrTreeElement * other )
	{
		return false;
	}

	bool operator== ( irrTreeElement * other )
	{
		return matches( other );
	}
};


class irrTreeNode
{
public:
	core::array<irrTreeNode *> children;
	s32 ID; // ID of this node
	irrTreeElement * element; // what this node is carrying
	irrTreeNode * parent; // parent node of this node

	irrTreeNode( s32 id=0, irrTreeNode * parent_node=0 )
	{
		ID = id;
		element = new irrTreeElement();
		this->parent = parent_node;
	}

	~irrTreeNode()
	{
		children.clear();
	}

	void setElem( irrTreeElement * new_element )
	{
		element = new_element;
	}

	void operator= ( irrTreeElement * new_element )
	{
		setElem( new_element );
	}

	irrTreeElement * getElem()
	{
		return element;
	}

	irrTreeElement& getElemCopy()
	{
		return *element;
	}

	void addNode( s32 id=0 )
	{
		children.push_back( new irrTreeNode( id, this ) );
	}

	//! Size of tree
	/* Returns the size of the entire tree, including all nodes. */
	s32 Size()
	{
		s32 size = children.size();

		for ( s32 i=0 ; i < (s32)children.size(); i++ )
			size += getChildByIndex(i)->Size();

		return size;
	}

		//! Get child by index
	/* Returns only one of this node's children based on index. */
	irrTreeNode * getChildByIndex( s32 child )
	{
		return children[child];
	}

	irrTreeNode& operator[] ( s32 index )
	{
		return *(children[index]);
	}

	irrTreeNode * get( s32 index )
	{
		return getChildByIndex( index );
	}

	irrTreeNode * getParent()
	{
		return this->parent;
	}

	bool matches( irrTreeElement * match );	

	bool getNodeWithID( s32 id, irrTreeNode * node );

	bool getAnyMatching( irrTreeElement * match, irrTreeNode * node );

	s32 getChildIndexByMatch( irrTreeElement * match );

	bool getChildByMatch( irrTreeElement * match, irrTreeNode * node );
};