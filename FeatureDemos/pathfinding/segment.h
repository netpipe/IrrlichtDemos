#ifndef _SEGMENT_H_
#define _SEGMENT_H_

#include <iostream>
#include <irrlicht.h>

using namespace irr;


class Segment
{
	public:
	Segment(u32 n1, u32 n2);
	~Segment();
	
	void operator=(const Segment& other);
	bool operator==(const Segment& other) const;
	bool isSameAs(const Segment& other) const;
	bool operator<(const Segment& other) const;
	
	bool isValid() const { return (Node1 != Node2); }
	
	void setNode1(u32 n1) { Node1 = n1; }
	void setNode2(u32 n2) { Node2 = n2; }
	
	u32 getNode1() const { return Node1; }
	u32 getNode2() const { return Node2; }
	
	void print() const;
	



private:
	u32 Node1;
	u32 Node2;

};

#endif // _SEGMENT_H_

