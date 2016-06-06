//!
//! MYMATERIAL
//! Irrlicht custom material that renders vertex alpha and alphachannel transparency
//! Its very incomplete and is hightly based on a material i get on forum for drawing gui elements
//! from xterminhate at http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=12387&highlight=settexturestagestate
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.

#ifndef __MyMaterial_H
#define __MyMaterial_H

//#include <IMaterialRenderer.h>
//#include <d3d9.h>

#pragma comment(lib, "Irrlicht.lib")
/*
using namespace irr;

class MyMaterialD3D9 : public video::IMaterialRenderer
{
public:
	//! Constructor
	MyMaterialD3D9( IDirect3DDevice9* d3ddev, video::IVideoDriver* driver );

protected:
	IDirect3DDevice9* pID3DDevice;
	video::IVideoDriver* Driver;
};

//! my material renderer
class MyMaterialD3D9_ALPHA : public MyMaterialD3D9
{
public:

	MyMaterialD3D9_ALPHA( IDirect3DDevice9* p, video::IVideoDriver* d );

  virtual void OnSetMaterial( video::SMaterial& material, const video::SMaterial& lastMaterial,
    bool resetAllRenderstates, video::IMaterialRendererServices* services );
};

*/
#endif /* MyMaterial_H */
