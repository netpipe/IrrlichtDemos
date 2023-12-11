// Copyright (C) 2008-2012 Benjamin Hampe
// This file uses parts of the "Irrlicht Engine" and PortMidi Libraries.
// For conditions of distribution and use, see copyright notice in common.h

#include "createRainbowMesh.h"

#include <irrlicht.h>
#include <../source/Irrlicht/CImage.h>

namespace irr{
namespace video{

// Der Algorithmus von Bruton für Wellenlängen im sichtbaren Licht, λ ∈ [380, 780] nm
SColor colorFromWavelength(	f64 lambda, f64 gamma)
{
    // Der Algorithmus von Dan Bruton [1] zur Berechnung von rgb-Werten aus Wellenlängen λ ∈ [380, 780]
    // er teilt das sichtbare Spektrum zunächst in Intervalle auf,
    // innerhalb derer jeweils nur einer der rgb-Parameter linear verändert wird.
    // Es folgt eine Adjustierung zur Dämpfung der Intensität an den Sichtbarkeitsgrenzen
    // und schließlich eine γ-Korrektur.
    // Sei also λ eine sichtbare Wellenlänge, ausgedrückt in der Einheit Nanometer (nm):
    // λ ∈ [380, 780]. Weiter sei γ > 0 eine feste Zahl (γ = 0.8 bei Bruton).
    // Wir setzen zunächst:

    // setze λ ∈ [380, 780]
    if (lambda < 380.0) lambda = 380.0;
    if (lambda > 780.0) lambda = 780.0;

    // Fallunterscheidung
    f64 r,g,b;

    if ((lambda >= 380.0) && (lambda < 440.0))
    {
        r = (440.0-lambda)/(440.0-380.0);
        g = 0.0;
        b = 1.0;
    }
    else if ((lambda >= 440.0) && (lambda < 490.0))
    {
        r = 0.0;
        g = (lambda-440.0)/(490.0-440.0);
        b = 1.0;
    }
    else if ((lambda >= 490.0) && (lambda < 510.0))
    {
        r = 0.0;
        g = 1.0;
        b = (510.0-lambda)/(510.0-490.0);
    }
    else if ((lambda >= 510.0) && (lambda < 580.0))
    {
        r = (lambda-510.0)/(580.0-510.0);
        g = 1.0;
        b = 0.0;
    }
    else if ((lambda >= 580.0) && (lambda < 645.0))
    {
        r = 1.0;
        g = (645.0-lambda)/(645.0-580.0);
        b = 0.0;
    }
    else // if ((lambda >= 645.0) && (lambda <= 780.0))
    {
        r = 1.0;
        g = 0.0;
        b = 0.0;
    }

    // Reduzierung der Intensität an den Rändern
    double f;
    if ((lambda >= 380.0) && (lambda < 420.0))
    {
        f = 0.3 + 0.7*(lambda-380.0)/(420.0-380.0);
    }
    else if ((lambda >= 420.0) && (lambda <= 700.0))
    {
        f = 1.0;
    }
    else // if ((lambda > 700.0) && (lambda <= 780.0))
    {
        f = 0.3 + 0.7*(780.0-lambda)/(780.0-700.0);
    }

    // eigentliche Korrektur
    if (f!=1.0)
    {
        r *= f;
        g *= f;
        b *= f;
    }

    // Gamma Korrektur
    if (gamma!=1.0)
    {
        r = pow(r, gamma);
        g = pow(g, gamma);
        b = pow(b, gamma);
    }

    // Clamping to [0,255]
    r *= 255.0;
    g *= 255.0;
    b *= 255.0;

    // hoffentlicht optimiert der compiler dies mit mmx und sse befehlen
    if (r<0.0) r=0.0;
    if (g<0.0) g=0.0;
    if (b<0.0) b=0.0;

    if (r>255.0) r=255.0;
    if (g>255.0) g=255.0;
    if (b>255.0) b=255.0;

    // return ARGB 32bit color (Alpha = 'opaque' = 'nicht transparent' = '255')
    return SColor(255,(u32)r, (u32)g, (u32)b );
}

SColor createRainbowColor( u32 i, u32 count, f64 gamma)
{
    //! division by zero would lead to runtime error !!!
    if (count < 1) return SColor(255,255,255,255);
	//! just a simple wrapper function, that linear inperpolates
    return colorFromWavelength(380.0 + 400.0*(f64)i/(f64)count, gamma);
}

IImage* createRainbowImage( u32 w, u32 h, u8 transparency, f64 gamma)
{
	// create image
	core::dimension2du size(w,h);

	IImage* img = (IImage*)new CImage(ECF_A8R8G8B8, size);

	if (!img) return 0;

	size = img->getDimension();

	// loop pixels per col
	for (u32 y=0; y<size.Height; y++)
	{
		// calculate rainbow-color
		SColor c = createRainbowColor( y, size.Height, gamma);

		// set transparency
		c.setAlpha(transparency);

		// write one row with same color
		for (u32 x=0; x<size.Width; x++)
		{
			// write rainbow-color
			img->setPixel(x,y,c,false);
		}
	}

	return img;
}

ITexture* createRainbowTexture( IVideoDriver* driver, u32 w, u32 h, u8 transparency, f64 gamma)
{
	if (!driver)
	{
		return 0;
	}

	IImage* img = createRainbowImage(w,h,transparency, gamma);
	if (!img)
		return 0;

	ITexture* tex = 0;

	//! save states
	bool b0 = driver->getTextureCreationFlag(ETCF_ALLOW_NON_POWER_2);
	bool b1 = driver->getTextureCreationFlag(ETCF_ALWAYS_16_BIT);
	bool b2 = driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT);
	bool b3 = driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	bool b4 = driver->getTextureCreationFlag(ETCF_FORCE_32_BIT_DO_NOT_USE);
	bool b5 = driver->getTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL);
	bool b6 = driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY);
	bool b7 = driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED);

	//! my states
	driver->setTextureCreationFlag(ETCF_ALLOW_NON_POWER_2,true);
	driver->setTextureCreationFlag(ETCF_ALWAYS_16_BIT,false);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT,true);
	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS,true);
	driver->setTextureCreationFlag(ETCF_FORCE_32_BIT_DO_NOT_USE,false);
	driver->setTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL,false);
	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY,true);
	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED,false);

	//! my RainbowTexture
	tex = driver->addTexture("rainbow", img, 0);

	//! restore states
	driver->setTextureCreationFlag(ETCF_ALLOW_NON_POWER_2,b0);
	driver->setTextureCreationFlag(ETCF_ALWAYS_16_BIT,b1);
	driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT,b2);
	driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS,b3);
	driver->setTextureCreationFlag(ETCF_FORCE_32_BIT_DO_NOT_USE,b4);
	driver->setTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL,b5);
	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY,b6);
	driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED,b7);

	img->drop(); // delete rainbow-image

	return tex;
} // END FUNCTION
} // END NAMESPACE VIDEO

namespace scene{
SMesh* createArcMesh( // XY-plane, normale is -Z
		f32 outerRadius,
		f32 innerRadius,
		f32 zAngleStart,
		f32 zAngleEnd,
		u32 segments,
		const core::vector3df& pos)
{
	SMesh* mesh = new SMesh();
	if (!mesh) return 0;

	SMeshBuffer* buffer = new SMeshBuffer();
	if (!buffer) return mesh;

	//! outer-cylinder material
	buffer->Material.Lighting = false;
	buffer->Material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	buffer->Material.Wireframe = false;
	buffer->Material.FogEnable = false;
	buffer->Material.BackfaceCulling = true;
	buffer->Vertices.clear();
//	buffer->Vertices.reallocate();
//	buffer->Vertices.set_used(0);
	buffer->Indices.clear();
//	buffer->Indices.reallocate();
//	buffer->Indices.set_used(0);

	//! Constants
	const f32 phi = 2.0f*core::PI * (zAngleEnd-zAngleStart) / (360.0f * (f32)segments);
	const f32 phiStart = 2.0f*core::PI * (zAngleStart-90.0f) / 360.0f;
	const f32 z = pos.Z;

	//! Locals
	f32 x0,y0,x1,y1,x2,y2,x3,y3,sinPhi,cosPhi;

	//! Tables
	f32* sinTable = new f32[segments+1];
	f32* cosTable = new f32[segments+1];
	for (u32 i=0; i<segments+1; ++i)
	{
		const f32 angle = (f32)i*phi + phiStart;
		sinTable[i] = sinf(angle);
		cosTable[i] = cosf(angle);
	}

	//! build MeshBuffer (each segment consists of 2 triangles, important for texturing, each segment contains one complete texture)
	for (u32 i=0; i<segments; ++i)
	{
		x0 = pos.X - innerRadius * sinTable[i+1];
		y0 = pos.Y + innerRadius * cosTable[i+1];
		x1 = pos.X - outerRadius * sinTable[i+1];
		y1 = pos.Y + outerRadius * cosTable[i+1];
		x2 = pos.X - outerRadius * sinTable[i];
		y2 = pos.Y + outerRadius * cosTable[i];
		x3 = pos.X - innerRadius * sinTable[i];
		y3 = pos.Y + innerRadius * cosTable[i];

		buffer->Vertices.push_back(video::S3DVertex(x0,y0,z,0,0,-1,0xFFFFFFFF,0,1)); // A
		buffer->Vertices.push_back(video::S3DVertex(x1,y1,z,0,0,-1,0xFFFFFFFF,0,0)); // B
		buffer->Vertices.push_back(video::S3DVertex(x2,y2,z,0,0,-1,0xFFFFFFFF,1,0)); // C
		buffer->Vertices.push_back(video::S3DVertex(x3,y3,z,0,0,-1,0xFFFFFFFF,1,1)); // D
		buffer->Indices.push_back(4*i+0); // A
		buffer->Indices.push_back(4*i+1); // B
		buffer->Indices.push_back(4*i+2); // C
		buffer->Indices.push_back(4*i+0); // A
		buffer->Indices.push_back(4*i+2); // C
		buffer->Indices.push_back(4*i+3); // D
	}

	delete [] sinTable;
	delete [] cosTable;

	//! BBox
	buffer->recalculateBoundingBox();

	//! to mesh
	mesh->addMeshBuffer(buffer);
	mesh->BoundingBox.addInternalBox(buffer->getBoundingBox());
	buffer->drop();

	return mesh;
} // END FUNCTION


} // END NAMESPACE SCENE
} // END NAMESPACE IRR
