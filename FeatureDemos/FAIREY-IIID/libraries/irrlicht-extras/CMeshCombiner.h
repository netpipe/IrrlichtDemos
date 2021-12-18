#ifndef CMESHCOMBINER_H
#define CMESHCOMBINER_H
#include <irrlicht.h>


// Used to pack textures
class CRectPacker {
public:
	struct SPacked {
		irr::core::rect<irr::u32> pos;
		int id;
	};
	struct SRect {
		irr::core::rect<irr::u32> pos;
		int children[2];
		bool packed;
		int id;
        bool operator<(const SRect & other) const {
			return pos.getArea() > other.pos.getArea();
        }
	};

	void pack(const irr::core::array<irr::core::rect<irr::u32>> &rects, irr::core::array<irr::core::array<SPacked>> &packed, irr::u32 sizeOfPack);
	void clear();
	void fill(int pack);
	void split(int pack, int rect);
	bool fits(SRect rect1, SRect rect2);
	void addPackToArray(int pack, irr::core::array<SPacked> &addArray) const;

	bool isRectValid(int i) const;
	bool isPackValid(int i) const;

	int mPackSize;
	int mNumPacked;
	irr::core::array<SRect> mRects;
	irr::core::array<SRect> mPacks;
	irr::core::array<int> mRoots;
};

static int globalPackedTextureCount = 0;

enum TEXTURE_PADDING_TECHNIQUE {
	ETPT_EXPAND = 0,
	ETPT_TILE = 1
};

class CMeshCombiner {
	irr::f32 mSizingTolerance;
	irr::u32 mTexturePadding;
	TEXTURE_PADDING_TECHNIQUE mTexturePaddingTechnique;
public:
	CMeshCombiner();
	CMeshCombiner(irr::f32 sizingTolerance =0.8, irr::u32 texturePadding = 0, TEXTURE_PADDING_TECHNIQUE technique = ETPT_EXPAND);
	irr::core::dimension2du findOptimalPackingArea(irr::core::array<irr::core::rect<irr::u32>> rectangles);
	irr::video::ITexture* packTextures(irr::video::IVideoDriver* driver, irr::core::array<irr::video::ITexture*> textures, irr::core::array<irr::core::position2di> &outPositions);
	irr::scene::IMesh* combineMeshes(irr::video::IVideoDriver* driver, irr::core::array<irr::scene::IMesh*> meshes, bool useHardwareMappingHint = true);
	irr::scene::IMesh* combineMeshes(irr::video::IVideoDriver* driver, irr::core::array<irr::scene::IMeshSceneNode*> nodes, bool clearNodesAfterUse = true, bool useHardwareMappingHint = true);

	void setSizingTolerance(irr::f32 tolerance);
	void setTexturePadding(irr::u32 texturePadding);
	void setTexturePaddingTechnique(TEXTURE_PADDING_TECHNIQUE technique);

	irr::f32 getSizingTolerance();
	irr::u32 getTexturePadding();
	TEXTURE_PADDING_TECHNIQUE getTexturePaddingTechnique();
};

#endif