inline u32 mxSystemWorld::GetRealTime() const
{
	// TODO: use platform-specific optimizations.
	// Use assembly.
	return m_timer->getRealTime();
}

inline u32 mxSystemWorld::GetTime() const
{
	return m_timer->getTime();
}

inline void mxSystemWorld::SetMouseCursorVisible( bool bVisible )
{
	CursorControl->setVisible( bVisible );
}
