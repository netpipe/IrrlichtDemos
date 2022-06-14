inline ::irr::video::E_DRIVER_TYPE  mxRenderWorld::GetDriverType() const
{
	return m_eDriverType;
}

inline bool mxRenderWorld::IsFullScreen() const
{
	return m_bFullScreen;
}

inline u32 mxRenderWorld::GetScreenWidth() const
{
	return m_uScreenWidth;
}

inline u32 mxRenderWorld::GetScreenHeight() const
{
	return m_uScreenHeight;
}

inline int32 mxRenderWorld::GetFPS() const
{
	return Driver->getFPS();
}
