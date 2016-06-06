#include "GameGlobals.h"


GameMapDescription::GameMapDescription()
{
	// nothing
}

GameMapDescription::~GameMapDescription()
{
	// nothing
}


void GameMapDescription::Load(core::stringc fn)
{
	GameLog("Map description %s loading started...", fn.c_str());
	io::IXMLReader *xml = Game.FS->createXMLReader(fn.c_str());

	for (int i=0; i < GAMEMAPDESCRIPTION_MAXDESCCOUNT; i++)
	{
		Items[i].keycount = 0;
		for (int k=0; k < GAMEMAPDESCRIPTION_MAXKEYCOUNT; k++) Items[i].key[k] = "";
		Items[i].title = "";
		Items[i].desc = "";
	}

	ItemsCount = 0;

	while( xml && xml->read() )
	{
		core::stringc node = xml->getNodeName();

		if (node == L"item")
		{
			switch (xml->getNodeType())
			{
				case io::EXN_ELEMENT_END:
					ItemsCount++;
					break;

				case io::EXN_ELEMENT:
					Items[ItemsCount].keycount = 0;
					while(xml && xml->read())
					{
						core::stringc node2 = xml->getNodeName();

						if (xml->getNodeType() == io::EXN_ELEMENT_END &&
							node == L"item")
						{
							break;
						}

						if (xml->getNodeType() == io::EXN_TEXT)
						{
							Items[ItemsCount].desc = xml->getNodeData();
						}

						if (node2 == L"title")
						{
							Items[ItemsCount].title =
								xml->getAttributeValue(L"value");
						}

						if (node2 == L"key")
						{
							Items[ItemsCount].key[Items[ItemsCount].keycount++] =
								xml->getAttributeValue(L"value");
						}

						assert( Items[ItemsCount].keycount < GAMEMAPDESCRIPTION_MAXKEYCOUNT );
					}
					break;
			}
		}
	}

	xml->drop();
	GameLog("Map description loaded.");
}


void GameMapDescription::GetDescription(core::stringc key,
	core::stringc &title, core::stringc &desc)
{
	for (int i=0; i < ItemsCount; i++)
	{
		for (int k=0; k < Items[i].keycount; k++)
		{
			if (key == Items[i].key[k])
			{
				title = Items[i].title;
				desc = Items[i].desc;
				return;
			}
		}
	}

	title = "";
	desc = "";

	return;
}
