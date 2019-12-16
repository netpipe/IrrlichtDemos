/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKGRAPHICS_SCENE_TERRAINSCENENODE_HPP_
#define _PEAKGRAPHICS_SCENE_TERRAINSCENENODE_HPP_

#include "SceneNode.hpp"

#include <string>

namespace peak
{
	class TerrainSceneNode : public SceneNode
	{
		public:
			TerrainSceneNode(std::string name, std::string heightmap,
				std::string colormap, std::string detailmap, Vector3F scale,
				Graphics *graphics);
			~TerrainSceneNode();

			virtual bool load();
			virtual bool isLoaded();
		private:
			std::string name;
			std::string heightmap;
			std::string colormap;
			std::string detailmap;
			Vector3F modelscale;
	};
}

#endif
