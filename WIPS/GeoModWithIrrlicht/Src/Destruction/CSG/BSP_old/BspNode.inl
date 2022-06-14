
inline bool Node::IsLeaf() const
{ return Type != Internal; }

inline bool Node::IsInternal() const
{ return Type == Internal; }

inline bool Node::IsSolid() const
{ return Type == InCell; }

inline bool Node::IsSpace() const
{ return Type == OutCell; }


inline void Node::SetFront( Node* child )
{ FrontChild = child; }

inline void Node::SetBack( Node* child )
{ BackChild = child; }

inline Node * Node::GetFront()
{ return FrontChild; }

inline Node * Node::GetBack()
{ return BackChild; }

inline const Node * Node::GetFront() const
{ return FrontChild; }

inline const Node * Node::GetBack() const
{ return BackChild; }


inline const mxPlane & Node::GetPlane() const
{	return Plane;	}

inline const mxBounds & Node::GetBounds() const
{	return Bounds;	}
