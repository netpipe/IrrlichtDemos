/*
(c) 2012 Nicolaus Anderson
*/

#include "irrTree.h"

#pragma once


bool irrTreeNode::matches( irrTreeElement * match )
{
	return element->matches(match);
}

bool irrTreeNode::getNodeWithID( s32 id, irrTreeNode * node )
{
	// First, search self
	if ( ID == id )
	{
		*node = *this;
		return true;
	}

	// Second search just the childrem
	for ( s32 c = 0; c < (s32)children.size(); c++ )
	{
		if ( ((irrTreeNode *)children[c])->ID == id )
		{
			node = (irrTreeNode *)children[c];
			return true;
		}
	}

	// Now have the children search their children
	for ( s32 c = 0; c < (s32)children.size(); c++ )
	{
		if ( ((irrTreeNode *)children[c])->getNodeWithID( id, node ) )
			return true;
	}

	return false;
}

bool irrTreeNode::getAnyMatching( irrTreeElement * match, irrTreeNode * node )
{
	// First, search self
	if ( matches(match)  )
	{
		*node = *this;
		return true;
	}

	// Now have the children search their children
	for ( s32 c = 0; c < (s32)children.size(); c++ )
	{
		if ( ((irrTreeNode *)children[c])->getAnyMatching( match, node ) )
			return true;
	}

	return false;
}

s32 irrTreeNode::getChildIndexByMatch( irrTreeElement * match )
{
	for ( s32 c = 0; c < (s32)children.size(); c++ )
	{
		if ( ((irrTreeNode *)children[c])->matches( match ) )
		{
			return c;
		}
	}

	return -1;
}

bool irrTreeNode::getChildByMatch( irrTreeElement * match, irrTreeNode * node )
{
	for ( s32 c = 0; c < (s32)children.size(); c++ )
	{
		if ( ((irrTreeNode *)children[c])->matches( match ) )
		{
			node = (irrTreeNode *)children[c];
			return true;
		}
	}

	return false;
}