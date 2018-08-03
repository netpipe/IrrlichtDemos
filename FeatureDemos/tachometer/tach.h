#ifndef TACH_H
#define TACH_H

#include "irrlicht.h"
#include <IGUIElement.h>

using namespace irr;
using namespace core;


class Tach: public gui::IGUIElement
{

	public:
	
	Tach
	(
		const core::rect<s32>& rect,
		gui::IGUIEnvironment* env,
		gui::IGUIElement* parent
	);
	
	virtual ~Tach();
	
	static core::rect<s32> getRect
	(
		IrrlichtDevice* device,
		f32 fx, f32 fy, f32 fs
	);
		
	void setReading(f32 reading);
	f32 getNeedleRot(f32 reading);
	void setNeedleRot(f32 deg);
	void setTexture(video::ITexture* texture);
	void setupMeshBuffer(scene::SMeshBuffer& buf);
	virtual void draw();


	private:
	scene::SMeshBuffer TachMeshBuffer;
	core::matrix4 Matrix;
	
	video::SMaterial TachMaterial;
	video::SMaterial NeedleMaterial;

	video::ITexture* TachTexture;
	video::ITexture* NeedleTexture;
}; 

#endif /* TACH_H */
