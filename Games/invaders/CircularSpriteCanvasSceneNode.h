#ifndef __CIRCULAR_SPRITE_NODE_H_INCLUDED__
#define __CIRCULAR_SPRITE_NODE_H_INCLUDED__

#include "ISceneNode.h"
#include "ITexture.h"

namespace irr
{
namespace scene
{

	using namespace core;
	using namespace video;

	class CircularSpriteCanvasSceneNode : public ISceneNode
	{
	public:

		CircularSpriteCanvasSceneNode(ISceneManager *smgr, f32 width, f32 height) :
					ISceneNode(smgr->getRootSceneNode(), smgr, -1),
					Width(width), Height(height)
		{
			setSize(width,height);

			inds[0] = 0;
			inds[1] = 1;
			inds[2] = 2;
			inds[3] = 1;
			inds[4] = 3;
			inds[5] = 2;
			mat.Lighting = false;
			mat.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		}

		~CircularSpriteCanvasSceneNode()
		{

		}

		void setSize(f32 width, f32 height)
		{
			Width = width;
			Height = height;

			// height = radius
			Box.reset(-height,-height,0);
			Box.addInternalPoint( vector3df(height,height, 0));
		}

		virtual void render()
		{
			// we do manual updates, no need for this
		}

		void explosion( position2df p )
		{
			vector3df pos(p.X, p.Y, 0);
			pos.Y -= Height/3.0f;
			pos.rotateXYBy((pos.X/Width) * 360.0f, vector3df(pos.X,Height,0));
			pos.X -= p.X;
			pos.Y -= Height;

			IParticleSystemSceneNode *ps = SceneManager->addParticleSystemSceneNode(false, this, -1, pos);
			ps->setParticlesAreGlobal(false);
			ps->setParticleSize(dimension2d<f32>(10,10));
			ps->getMaterial(0).Lighting = false;
			IParticleEmitter *em = ps->createBoxEmitter(aabbox3df(-10,-10,-5,10,10,5),vector3df(0,0,-0.3),100,300, video::SColor(0,0,100,0), video::SColor(0,0,255,0),300, 500, 360);
			ps->setEmitter(em);
			em->drop();
			ISceneNodeAnimator *anm =  SceneManager->createDeleteAnimator(500);
			ps->addAnimator(anm);
			anm->drop();

		}

		void drawSprite(video::ITexture *tex, s32 cellsx, s32 cellsy, s32 spritex, s32 spritey,
						rect<f32> pos, bool flipx=false, bool flipy=false)
		{
			if (!tex)
				return;

			pos.UpperLeftCorner.Y -= Height/3.0f;
			pos.LowerRightCorner.Y -= Height/3.0f;

			video::IVideoDriver *drv = SceneManager->getVideoDriver();

			// set texture
			mat.setTexture(0, tex);

			// set texture coordinates
			f32 wx = 1.f / (f32)cellsx,
				wy = 1.f / (f32)cellsy;

			f32 px = 1.f / tex->getSize().Width,
				py = 1.f / tex->getSize().Height;

			vector2df	s = vector2df(wx*(f32)spritex+px, wy*(f32)spritey+py), // start
						e = vector2df(wx*((f32)spritex+1)-px, wy*((f32)spritey+1)-py );	// end

			if (flipx)
			{
				f32 t = s.X;
				s.X = e.X;
				e.X = t;
			}
			if (flipy)
			{
				f32 t = s.Y;
				s.Y = e.Y;
				e.Y = t;
			}

			verts[0].TCoords = s;
			verts[1].TCoords = vector2df(e.X, s.Y);
			verts[2].TCoords = vector2df(s.X, e.Y);
			verts[3].TCoords = e;

			// set vertices

			// take x and y position and rotate them around z
			vector3df	p1(pos.UpperLeftCorner.X, pos.UpperLeftCorner.Y, 0),
						p2(pos.UpperLeftCorner.X + pos.getWidth(), pos.UpperLeftCorner.Y, 0),
						p3(pos.UpperLeftCorner.X, pos.UpperLeftCorner.Y + pos.getHeight(), 0),
						p4(pos.UpperLeftCorner.X + pos.getWidth(), pos.UpperLeftCorner.Y + pos.getHeight(), 0);

			p1.rotateXYBy((p1.X/Width) * 360.0f, vector3df(p1.X,Height,0));
			p2.rotateXYBy((p2.X/Width) * 360.0f, vector3df(p2.X,Height,0));
			p3.rotateXYBy((p3.X/Width) * 360.0f, vector3df(p3.X,Height,0));
			p4.rotateXYBy((p4.X/Width) * 360.0f, vector3df(p4.X,Height,0));

			p1.X -= pos.UpperLeftCorner.X; p1.Y -= Height;
			p2.X -= pos.LowerRightCorner.X; p2.Y -= Height;
			p3.X -= pos.UpperLeftCorner.X; p3.Y -= Height;
			p4.X -= pos.LowerRightCorner.X; p4.Y -= Height;

			verts[0].Pos = p1;
			verts[1].Pos = p2;
			verts[2].Pos = p3;
			verts[3].Pos = p4;

			drv->setTransform(video::ETS_WORLD, AbsoluteTransformation);
			drv->setMaterial(mat);
			drv->drawIndexedTriangleList(verts, 4, inds, 2);

			// debug
			//drv->draw3DBox(Box);

		}

		virtual const core::aabbox3d<f32>& getBoundingBox() const
		{
			return Box;
		}

	private:
		f32 Width, Height;
		core::aabbox3d<f32> Box;
		SMaterial mat;
		S3DVertex verts[4];
		u16 inds[6];

	};

} // scene
} // irr

#endif
