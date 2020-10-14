CMenu m_NewMenu;
	m_NewMenu.LoadMenuW(IDR_MDIPainterTYPE);
	ASSERT(m_NewMenu);
	SetMenu(NULL);
	::DestroyMenu(m_hMenuDefault);
	SetMenu(&m_NewMenu);
	m_hMenuDefault = m_NewMenu.GetSafeHmenu();
