#include "segment.h"

Segment::Segment(u32 n1, u32 n2)
{
	if (n1 <= n2)
	{
		Node1 = n1;
		Node2 = n2;
	}
	else
	{
		Node1 = n2;
		Node2 = n1;
	}
	
}

Segment::~Segment()
{
}


void Segment::operator=(const Segment& other)
{
	setNode1(other.getNode1());
	setNode2(other.getNode2());
}


bool Segment::operator==(const Segment& other) const
{
	return
	(
		((getNode1() == other.getNode1()) && (getNode2() == other.getNode2()))
		|| ((getNode1() == other.getNode2()) && (getNode2() == other.getNode1()))
	);
	
}


bool Segment::isSameAs(const Segment& other) const
{
	return (*this == other);
}


bool Segment::operator<(const Segment& other) const
{
	return
	(
		(this->getNode1() + this->getNode2()) < (other.getNode1() + other.getNode2())
	);
}



void Segment::print() const
{
	printf("Segment: node1=%u, node2=%u\n", Node1, Node2);
}



