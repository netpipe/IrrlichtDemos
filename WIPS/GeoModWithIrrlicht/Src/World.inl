inline mxEnvironment * mxWorld::GetEnvironment()
{
	return Environment;
}

inline mxGamePlayer * mxWorld::GetLocalPlayer()
{
	return m_localPlayer;
}

inline mxConsole & mxWorld::GetConsole()
{
	return m_gameConsole;
}

inline u32 mxWorld::GetTime() const
{
	return m_uCurrentTime;
}

inline void mxWorld::AddAction( mxAction& rAction, ::irr::EKEY_CODE  key )
{
//	m_keyActions[ key ].Append( & rAction );
}
