#ifndef __COSMIC_GAMEPOINTCLOUDOBJECT_H_INCLUDED__
#define __COSMIC_GAMEPOINTCLOUDOBJECT_H_INCLUDED__


#include "GameGlobals.h"


class GamePointcloudObject : public GameObject
{
public:
	scene::IMeshSceneNode* pPointcloudNode;

	GamePointcloudObject();
	virtual ~GamePointcloudObject();

	void Load(io::IXMLReader *xml);

private:
	void BuildPointcloud(
		float radius_from, float radius_to, float height_from, float height_to,
		float bright_from, float bright_to, int pointcount,
		int volume_every_at, core::stringc volume_textures, int volume_texturescount,
		int bb_every_at, core::stringc bb_textures, int bb_texturescount);
};


#endif
