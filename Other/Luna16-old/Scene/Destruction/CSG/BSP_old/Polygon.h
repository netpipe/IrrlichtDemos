
inline u32 Polygon::NumVertices() const {
	return m_NumVertices;
}

inline void Polygon::AddVertex( const BspVertex& v )
{	Assert( NumVertices() <= SPLIT_THRESHOLD );
	Vertices[ m_NumVertices ] = v;
	++m_NumVertices;
}

inline void Polygon::SetVertex( u32 index, const mxVertex& renderVertex )
{	Assert( index >= 0 && index < m_NumVertices );
	Vertices[ index ] = renderVertex;
}

inline const BspVertex & Polygon::GetVertex( u32 index ) const
{	Assert( index >= 0 && index < m_NumVertices );
	return Vertices[ index ];
}

inline const Vec3D & Polygon::GetPos( u32 index ) const
{	Assert( index >= 0 && index < m_NumVertices );
	return Vertices[ index ].Pos;
}


inline Polygon * Polygon::GetNext()
{ return m_pNext; }

inline const Polygon * Polygon::GetNext() const
{ return m_pNext; }

inline void Polygon::SetNext( Polygon* face )
{ m_pNext = face; }


inline const mxPlane & Polygon::GetPlane() const
{ return Plane; }
