//! ----------------------------------------------------------------
//! renders big image of any size
//! ----------------------------------------------------------------

#include "renderToImage.h"

#include <irrlicht.h>

namespace irr
{

bool renderToImage( IrrlichtDevice* device, video::IImage* dst, s32 nSamples, const video::SColor& clearColor, bool renderGUI)
{
	// print call params
    printf(	"renderToImage( nSamples=%i, clearColor(%i,%i,%i,%i)\n",
			nSamples,
			clearColor.getAlpha(),
			clearColor.getRed(),
			clearColor.getGreen(),
			clearColor.getBlue()	);

	// abort
    if (!device)
	{
		printf("No Irrlicht-Device found.\n");
        return false;
	}

	// abort
    if (!dst)
	{
		printf("No destination image found.\n");
        return false;
	}

	// local pointers
	video::IVideoDriver* driver = device->getVideoDriver();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	scene::ISceneManager* smgr = device->getSceneManager();
	scene::ICameraSceneNode* camera = smgr->getActiveCamera();

	// abort
	if (!camera)
	{
		printf("No active camera found.\n");
		return false;
	}

	//! decompose camera's projection-matrix in d3d semantics

	core::matrix4 projMat = camera->getProjectionMatrix();

	bool IsOrthogonal = true;

	if ( core::equals( projMat(3,3), 0.f ) )
		IsOrthogonal = false;

	if ( !core::equals( projMat(2,3), 0.f ) )
		IsOrthogonal = false;

    f32 Left;
    f32 Right;
    f32 Bottom;
    f32 Top;
    f32 Near;
    f32 Far;

	if (IsOrthogonal)
	{
		Near = -projMat(3,2)/projMat(2,2);
		Far = -(projMat(3,2)-1.f)/projMat(2,2);
		Left = -(projMat(3,0)+1.f)/projMat(0,0);
		Right = -(projMat(3,0)-1.f)/projMat(0,0);
		Top = -(projMat(3,1)-1.f)/projMat(1,1);
		Bottom = -(projMat(3,1)+1.f)/projMat(1,1);
	}
	else
	{
		Near = -projMat(3,2)/projMat(2,2);
		Far = -projMat(3,2)/(projMat(2,2)-1.f);
		Left = -Near*(projMat(2,0)+1.f)/projMat(0,0);
		Right = -Near*(projMat(2,0)-1.f)/projMat(0,0);
		Top = -Near*(projMat(2,1)-1.f)/projMat(1,1);
		Bottom = -Near*(projMat(2,1)+1.f)/projMat(1,1);
	}
	Near = camera->getNearValue();	// take the values directly from Camera
	Far = camera->getFarValue();	// due to f32 rounding errors

	//! ----------------------------------------------------------------------
	//! calculate Border / get max thickness used by all registered materials
	//! ----------------------------------------------------------------------

	s32 Border = 0;
	f32 thickness = 0.0f;

    const core::list<scene::ISceneNode*>& children = smgr->getRootSceneNode()->getChildren();
    core::list<scene::ISceneNode*>::ConstIterator it = children.begin();

    while (it != children.end())
	{
		scene::ISceneNode* node = (*it);
		if (node)
		{
			u32 matCount = node->getMaterialCount();
			for (u32 m = 0; m < matCount; ++m)
			{
				if (thickness < node->getMaterial(m).Thickness)
					thickness = node->getMaterial(m).Thickness;
			}
		}
		it++;
	}

	Border = core::ceil32( thickness );

	//! ------------------------------------------------------------------------------------------
	//! choose render-mode: offscreen or onscreen depending on AA, nSamples and RTT capabilities
	//! ------------------------------------------------------------------------------------------

	//! @var nSamples - AntiAliasing depth > 1 make bigger rtts or downscale render-result by factor nSamples while having filter-methods enabled
	//! the framebuffer will always be downscaled by fast software bilinear filtering using colorkey/transparency to blend better into final image
	//! hardware-scaling: bilinear, trilinear or anisotropic texture-filtering available with irrlicht-engine
	//! software scaling: nearest, bilinear, bicubic downscaling available:
	//!  				  bilinear has best compromise for downscaling ( there is much more information-loss due to resize than interpolation)
	//! default: 1 (<2) - no AA/downscaling done

	//! @var clearColor - argb32 color, the final image will filled with
	//! before writing into it and be used as colorkey for AA/software bilinear downscaling operation

	//! @var DoOffscreen - default=false, onscreen (framebuffer)
	//! false -	onscreen-rendering using framebuffer/display.
	//! 		doublebuffer is possible, but will not prevent the showing of each rendered tile-texture
	//! true - 	offscreen-rendering using render-target-textures

	bool DoOffscreen = false;

	//! ----------------------------------------------------------------
	//! collect video-driver infos
	//! ----------------------------------------------------------------

	const core::dimension2du ScreenSize = driver->getScreenSize();
	const video::ECOLOR_FORMAT ScreenFormat = driver->getColorFormat();
	const u32 ScreenBits = video::IImage::getBitsPerPixelFromFormat( ScreenFormat );
    const io::IAttributes& info = driver->getDriverAttributes();
    const core::dimension2du MaxRTTSize = driver->getMaxTextureSize();
    const u32 MaxAA = info.getAttributeAsInt( "AntiAlias" );
    const u32 MaxAF = info.getAttributeAsInt( "MaxAnisotropy" );
    const bool HasNPOT = driver->queryFeature( video::EVDF_TEXTURE_NPOT );
    const bool HasNSQR = driver->queryFeature( video::EVDF_TEXTURE_NSQUARE );
	const bool HasRTT = driver->queryFeature( video::EVDF_RENDER_TO_TARGET);
	const bool HasMTT = driver->queryFeature( video::EVDF_MULTITEXTURE);
	const bool HasMRT = driver->queryFeature( video::EVDF_MULTIPLE_RENDER_TARGETS);
	const bool HasATC = driver->queryFeature( video::EVDF_ALPHA_TO_COVERAGE);
	const bool HasBTF = driver->queryFeature( video::EVDF_BILINEAR_FILTER);
	const bool HasCMK = driver->queryFeature( video::EVDF_COLOR_MASK);
	const bool HasMMP = driver->queryFeature( video::EVDF_MIP_MAP);
	const bool HasMMA = driver->queryFeature( video::EVDF_MIP_MAP_AUTO_UPDATE);
	const bool HasOCC = driver->queryFeature( video::EVDF_OCCLUSION_QUERY);
	const bool HasPOF = driver->queryFeature( video::EVDF_POLYGON_OFFSET);

	printf("ScreenSize = %i x %i x %i\n", ScreenSize.Width, ScreenSize.Height, ScreenBits);
	printf("MaxRTTSize = %i x %i\n", MaxRTTSize.Width, MaxRTTSize.Height);
	printf("MaxAA = %i\n", MaxAA);
	printf("MaxAF = %i\n", MaxAF);
	printf("HasNPOT = %s\n", HasNPOT?"true":"false");
	printf("HasNSQR = %s\n", HasNSQR?"true":"false");
	printf("HasRTT = %s\n", HasRTT?"true":"false");
	printf("HasMTT = %s\n", HasMTT?"true":"false");
	printf("HasMRT = %s\n", HasMRT?"true":"false");
	printf("HasATC = %s\n", HasATC?"true":"false");
	printf("HasBTF = %s\n", HasBTF?"true":"false");
	printf("HasCMK = %s\n", HasCMK?"true":"false");
	printf("HasMMP = %s\n", HasMMP?"true":"false");
	printf("HasMMA = %s\n", HasMMA?"true":"false");
	printf("HasOCC = %s\n", HasOCC?"true":"false");
	printf("HasPOF = %s\n", HasPOF?"true":"false");

    s32 RTTWidth = ScreenSize.Width;				// equals the actual rtt size before down scaling factor nSamples AA and writing to final Image
    s32 RTTHeight = ScreenSize.Height;				// the render-target is always a texture (offscreen) or the framebuffer (onscreen)
    s32 RTTWidthNB = RTTWidth - 2*Border;			// the border equals the overlapping during rendering due to material-thickness > 0
    s32 RTTHeightNB = RTTHeight - 2*Border;			// the area that gets written to unique places, while the border regions can overlapp ( same pos, but writing/adding different image-content )

	// so the position increment for the final writing is always RTTWidthNB or RTTHeightNB

    const s32 ImageWidth = (s32)dst->getDimension().Width;		// width of the final image
    const s32 ImageHeight = (s32)dst->getDimension().Height;	// height of the final image
    const video::ECOLOR_FORMAT ImageFormat = dst->getColorFormat();			// color-format of the final image, i.e. video::ECF_A8R8G8B8, ECF_R8G8B8, ECF_R16 for heightmaps ( bilinear, anisotropic )
	const s32 CountX = core::ceil32( (f32)ImageWidth / (f32)RTTWidthNB );
	const s32 CountY = core::ceil32( (f32)ImageHeight / (f32)RTTHeightNB );

	//! ------------------------------------------------------------------------------------------
	//! start render loop
	//! ------------------------------------------------------------------------------------------

    s32 IndexX = 0;
    s32 IndexY = 0;
	s32 dstX = -Border;
	s32 dstY = -Border;

	for ( IndexY = 0; IndexY < CountY; ++IndexY)
	{
		// reset row
		dstX = -Border;

		for ( IndexX = 0; IndexX < CountX; ++IndexX)
		{
			const f32 fLeft = Left + (Right - Left) * (f32)( IndexX * RTTWidthNB - Border )/(f32)ImageWidth;
			const f32 fRight = fLeft + (Right - Left) * (f32)RTTWidth / (f32)ImageWidth;
			const f32 fTop = Top - (Top - Bottom) * (f32)( IndexY * RTTHeightNB - Border )/(f32)ImageHeight;
			const f32 fBottom = fTop - (Top - Bottom) * (f32)RTTHeight / (f32)ImageHeight;
			const f32 fNear = Near;
			const f32 fFar = Far;

			//! ------------------------------------------------------------------------------------------
			//! build and set current projection-matrix
			//! ------------------------------------------------------------------------------------------

			core::matrix4 tmpProj = core::IdentityMatrix;
			if (IsOrthogonal)
			{
				tmpProj(0,0) = 2.f / (fRight - fLeft);
				tmpProj(1,1) = 2.f / (fTop - fBottom);
				tmpProj(2,2) = 1.f / (fFar - fNear);
				tmpProj(3,3) = 1.f;
				tmpProj(3,0) = (fLeft + fRight) / (fLeft - fRight);
				tmpProj(3,1) = (fTop + fBottom) / (fBottom - fTop);
				tmpProj(3,2) = fNear / (fNear - fFar);
			}
			else
			{
				tmpProj(0,0) = 2.f*fNear / (fRight - fLeft);
				tmpProj(1,1) = 2.f*fNear / (fTop - fBottom);
				tmpProj(2,0) = (fLeft + fRight) / (fLeft - fRight);
				tmpProj(2,1) = (fTop + fBottom) / (fBottom - fTop);
				tmpProj(2,2) = fFar / (fFar - fNear);
				tmpProj(2,3) = 1.f;
				tmpProj(3,2) = fNear*fFar / (fNear - fFar);
				tmpProj(3,3) = 0.f;
			}
			camera->setProjectionMatrix(tmpProj);

			//! ------------------------------------------------------------------------------------------
			//! now render all to current render-target
			//! ------------------------------------------------------------------------------------------

			driver->beginScene( true, true, clearColor ); // video::SColor(255,200,200,255)

				smgr->drawAll();
				if (renderGUI)
					guienv->drawAll();

			driver->endScene();

			//! ------------------------------------------------------------------------------------------
			//! write current render-target to final image
			//! ------------------------------------------------------------------------------------------

			video::IImage* src = driver->createScreenShot( ImageFormat, video::ERT_FRAME_BUFFER);
			if (src)
			{
				s32 x2 = src->getDimension().Width;
				s32 y2 = src->getDimension().Height;

				blitImageToImage( dst, core::position2di(dstX, dstY), src, core::recti(Border, Border, x2-Border, y2-Border) );

				src->drop();
			}
			dstX += RTTWidthNB;
		}
		dstY += RTTHeightNB;
	}

	//! ------------------------------------------------------------------------------------------
	//! reset camera's projection-matrix
	//! ------------------------------------------------------------------------------------------

	camera->setProjectionMatrix( projMat, IsOrthogonal);
    return true;
}


bool blitImageToImage(
	video::IImage* dst, core::position2di dstPos,
	video::IImage* src, const core::recti& srcRect)
{
	printf("blitImageToImage(x=%i,y=%i,rect(%i,%i,%i,%i))\n", dstPos.X, dstPos.Y,
			srcRect.UpperLeftCorner.X, srcRect.UpperLeftCorner.Y,srcRect.LowerRightCorner.X, srcRect.LowerRightCorner.Y);

	if (!dst)
	{
		printf("Destination Image does not exist.\n");
		return false;
	}

	if (!src)
	{
		printf("Source Image does not exist.\n");
		return false;
	}

	const core::dimension2du& dstSize = dst->getDimension();
	const core::dimension2du& srcSize = src->getDimension();

	if (dstPos.X >= (s32)dstSize.Width)	// ! if you forget cast to s32 it always fails when X < 0
		return false;

	if (dstPos.Y >= (s32)dstSize.Height) // ! if you forget cast to s32 it always fails when Y < 0
		return false;

	u32 dstPitch = dst->getPitch();
	u32 dstBPP = dst->getBytesPerPixel();
	u8* dstPtr = (u8*)dst->lock();

	u32 srcPitch = src->getPitch();
	u32 srcBPP = src->getBytesPerPixel();
	u8* srcPtr = (u8*)src->lock();

	printf("dst: %i, %i, pos=%i, %i, bpp=%i, pitch=%i\n", dstSize.Width, dstSize.Height, dstPos.X, dstPos.Y, dstBPP, dstPitch);
	printf("src: %i, %i, bpp=%i, pitch=%i\n", srcSize.Width, srcSize.Height, srcBPP, srcPitch);

	// inc dst pointer to start-pos;
	// dstPtr += (u32)dstPos.X*dstBPP + (u32)dstPos.Y*dstPitch;

	// slow and save, can be much optimized, but not now
	for (u32 y=0; y<(u32)srcRect.getHeight(); ++y)
	{
		for (u32 x=0; x<(u32)srcRect.getWidth(); ++x)
		{
			s32 sx = srcRect.UpperLeftCorner.X + x;
			s32 sy = srcRect.UpperLeftCorner.Y + y;
			if (sx>=0 && sy>=0)
			{
				video::SColor color = src->getPixel( (u32)sx, (u32)sy);
				s32 dx = dstPos.X + (s32)x;
				s32 dy = dstPos.Y + (s32)y;
				if (dx>=0 && dy>=0)
					dst->setPixel( (u32)dx, (u32)dy, color);
			}
		}
	}

	src->unlock();
	dst->unlock();
	return true;
}

} // end namespace irr
