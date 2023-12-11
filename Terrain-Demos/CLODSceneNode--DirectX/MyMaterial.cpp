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


#include "MyMaterial.h"

/*


void CD3D9Driver::draw2DImage(video::ITexture* texture, const core::rect<s32>& destRect,
                   const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect,
                   video::SColor* colors, bool useAlphaChannelOfTexture)
{
   if(!texture) return;

    if (useAlphaChannelOfTexture)
    {
        if ( !colors )
            setRenderStates2DMode(false, true, true);
        else
            setRenderStates2DMode(true, true, true);
    }
    else
    {
        if ( !colors )
            setRenderStates2DMode(false, true, false);
        else
            setRenderStates2DMode(true, true, false);
    }

   setTexture(0, texture);

   core::rect<s32> trgRect=destRect;
   core::rect<s32> srcRect=sourceRect;

   const core::dimension2d<s32> ss = texture->getOriginalSize();
   float ssw=1.0f/ss.Width;
   float ssh=1.0f/ss.Height;

   core::rect<f32> tcoords;
   tcoords.UpperLeftCorner.X = (((f32)srcRect.UpperLeftCorner.X)+0.5f) * ssw ;
   tcoords.UpperLeftCorner.Y = (((f32)srcRect.UpperLeftCorner.Y)+0.5f) * ssh;
   tcoords.LowerRightCorner.X = (((f32)srcRect.UpperLeftCorner.X +0.5f + (f32)srcRect.getWidth())) * ssw;
   tcoords.LowerRightCorner.Y = (((f32)srcRect.UpperLeftCorner.Y +0.5f + (f32)srcRect.getHeight())) * ssh;

   core::dimension2d<s32> renderTargetSize = getCurrentRenderTargetSize();

   s32 xPlus = -(renderTargetSize.Width>>1);
   f32 xFact = 1.0f / (renderTargetSize.Width>>1);

   s32 yPlus = renderTargetSize.Height-(renderTargetSize.Height>>1);
   f32 yFact = 1.0f / (renderTargetSize.Height>>1);

   core::rect<float> npos;
   npos.UpperLeftCorner.X = (f32)(trgRect.UpperLeftCorner.X+xPlus+0.5f) * xFact;
   npos.UpperLeftCorner.Y = (f32)(yPlus-trgRect.UpperLeftCorner.Y+0.5f) * yFact;
   npos.LowerRightCorner.X = (f32)(trgRect.LowerRightCorner.X+xPlus+0.5f) * xFact;
   npos.LowerRightCorner.Y = (f32)(yPlus-trgRect.LowerRightCorner.Y+0.5f) * yFact;

   // changed !!!
   SColor TempColors[4];
   if(colors==NULL)
   {
      for(int i=0;i<4;i++) TempColors[i]=SColor(255,255,255,255);
     colors = &TempColors[0];
   }

   S3DVertex vtx[4]; // clock wise
   vtx[0] = S3DVertex(npos.UpperLeftCorner.X, npos.UpperLeftCorner.Y , 0.0f, 0.0f, 0.0f, 0.0f, colors[0], tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
   vtx[1] = S3DVertex(npos.LowerRightCorner.X, npos.UpperLeftCorner.Y , 0.0f, 0.0f, 0.0f, 0.0f, colors[3], tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
   vtx[2] = S3DVertex(npos.LowerRightCorner.X, npos.LowerRightCorner.Y, 0.0f, 0.0f, 0.0f, 0.0f, colors[2], tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
   vtx[3] = S3DVertex(npos.UpperLeftCorner.X, npos.LowerRightCorner.Y, 0.0f, 0.0f, 0.0f, 0.0f, colors[1], tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);

   s16 indices[6] = {0,1,2,0,2,3};

   setVertexShader(EVT_STANDARD);

   pID3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0],
      D3DFMT_INDEX16,&vtx[0],   sizeof(S3DVertex));


   //if(bTempColors) delete [] colors;
}


//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
void CD3D9Driver::draw2DImage(video::ITexture* texture, const core::position2d<s32>& pos,
                         const core::rect<s32>& sourceRect,
                         const core::rect<s32>* clipRect, SColor color,
                         bool useAlphaChannelOfTexture)
{
   if (!texture)
      return;

   if (texture)
   {
      if (texture->getDriverType() != EDT_DIRECT3D9)
      {
         os::Printer::log("Fatal Error: Tried to copy from a surface not owned by this driver.", ELL_ERROR);
         return;
      }
   }

   if (sourceRect.UpperLeftCorner.X >= sourceRect.LowerRightCorner.X ||
      sourceRect.UpperLeftCorner.Y >= sourceRect.LowerRightCorner.Y)
      return;

   core::position2d<s32> targetPos = pos;
   core::position2d<s32> sourcePos = sourceRect.UpperLeftCorner;
   core::dimension2d<s32> sourceSize(sourceRect.getWidth(), sourceRect.getHeight());

   core::dimension2d<s32> renderTargetSize = getCurrentRenderTargetSize();
   const core::dimension2d<s32> targetSurfaceSize = renderTargetSize;

   if (clipRect)
   {
      if (targetPos.X < clipRect->UpperLeftCorner.X)
      {
         sourceSize.Width += targetPos.X - clipRect->UpperLeftCorner.X;
         if (sourceSize.Width <= 0)
            return;

         sourcePos.X -= targetPos.X - clipRect->UpperLeftCorner.X;
         targetPos.X = clipRect->UpperLeftCorner.X;
      }

      if (targetPos.X + sourceSize.Width > clipRect->LowerRightCorner.X)
      {
         sourceSize.Width -= (targetPos.X + sourceSize.Width) - clipRect->LowerRightCorner.X;
         if (sourceSize.Width <= 0)
            return;
      }

      if (targetPos.Y < clipRect->UpperLeftCorner.Y)
      {
         sourceSize.Height += targetPos.Y - clipRect->UpperLeftCorner.Y;
         if (sourceSize.Height <= 0)
            return;

         sourcePos.Y -= targetPos.Y - clipRect->UpperLeftCorner.Y;
         targetPos.Y = clipRect->UpperLeftCorner.Y;
      }

      if (targetPos.Y + sourceSize.Height > clipRect->LowerRightCorner.Y)
      {
         sourceSize.Height -= (targetPos.Y + sourceSize.Height) - clipRect->LowerRightCorner.Y;
         if (sourceSize.Height <= 0)
            return;
      }
   }

   // clip these coordinates

   if (targetPos.X<0)
   {
      sourceSize.Width += targetPos.X;
      if (sourceSize.Width <= 0)
         return;

      sourcePos.X -= targetPos.X;
      targetPos.X = 0;
   }

   if (targetPos.X + sourceSize.Width > targetSurfaceSize.Width)
   {
      sourceSize.Width -= (targetPos.X + sourceSize.Width) - targetSurfaceSize.Width;
      if (sourceSize.Width <= 0)
         return;
   }

   if (targetPos.Y<0)
   {
      sourceSize.Height += targetPos.Y;
      if (sourceSize.Height <= 0)
         return;

      sourcePos.Y -= targetPos.Y;
      targetPos.Y = 0;
   }

   if (targetPos.Y + sourceSize.Height > targetSurfaceSize.Height)
   {
      sourceSize.Height -= (targetPos.Y + sourceSize.Height) - targetSurfaceSize.Height;
      if (sourceSize.Height <= 0)
         return;
   }

   // ok, we've clipped everything.
   // now draw it.

    if (useAlphaChannelOfTexture)
    {
        if ( color == SColor(255,255,255,255) )
            setRenderStates2DMode(false, true, true);
        else
            setRenderStates2DMode(true, true, true);
    }
    else
    {
        if ( color == SColor(255,255,255,255) )
            setRenderStates2DMode(false, true, false);
        else
            setRenderStates2DMode(true, true, false);
    }

   setTexture(0, texture);

   core::rect<s32> poss(targetPos, sourceSize);

   s32 xPlus = -(renderTargetSize.Width>>1);
   f32 xFact = 1.0f / (renderTargetSize.Width>>1);

   s32 yPlus = renderTargetSize.Height-(renderTargetSize.Height>>1);
   f32 yFact = 1.0f / (renderTargetSize.Height>>1);

   const core::dimension2d<s32> sourceSurfaceSize = texture->getOriginalSize();
   core::rect<f32> tcoords;
   tcoords.UpperLeftCorner.X = (((f32)sourcePos.X)+0.5f) / texture->getOriginalSize().Width ;
   tcoords.UpperLeftCorner.Y = (((f32)sourcePos.Y)+0.5f) / texture->getOriginalSize().Height;
   tcoords.LowerRightCorner.X = (((f32)sourcePos.X +0.5f + (f32)sourceSize.Width)) / texture->getOriginalSize().Width;
   tcoords.LowerRightCorner.Y = (((f32)sourcePos.Y +0.5f + (f32)sourceSize.Height)) / texture->getOriginalSize().Height;

    S3DVertex vtx[4];
   vtx[0] = S3DVertex((f32)(poss.UpperLeftCorner.X+xPlus) * xFact, (f32)(yPlus-poss.UpperLeftCorner.Y ) * yFact , 0.0f, 0.0f, 0.0f, 0.0f, color, tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
   vtx[1] = S3DVertex((f32)(poss.LowerRightCorner.X+xPlus) * xFact, (f32)(yPlus- poss.UpperLeftCorner.Y) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
   vtx[2] = S3DVertex((f32)(poss.LowerRightCorner.X+xPlus) * xFact, (f32)(yPlus-poss.LowerRightCorner.Y) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
   vtx[3] = S3DVertex((f32)(poss.UpperLeftCorner.X+xPlus) * xFact, (f32)(yPlus-poss.LowerRightCorner.Y) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);

   s16 indices[6] = {0,1,2,0,2,3};

   setVertexShader(EVT_STANDARD);

   pID3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0],
                              D3DFMT_INDEX16,&vtx[0],   sizeof(S3DVertex));
}


Part two :

Code:
//! sets the needed renderstates
void CD3D9Driver::setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel)
{
   if (!pID3DDevice)
      return;

   if (CurrentRenderMode != ERM_2D || Transformation3DChanged)
   {
      core::matrix4 mat;
      pID3DDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)((void*)&mat));
      pID3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)((void*)&mat));
      pID3DDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)((void*)&mat));

      pID3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
      //pID3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
      pID3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
      pID3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
      pID3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
      pID3DDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

      pID3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
      pID3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );

      pID3DDevice->SetRenderState( D3DRS_STENCILENABLE, FALSE );

      pID3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);

      pID3DDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
      pID3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0);
      pID3DDevice->SetTransform( D3DTS_TEXTURE0, &UnitMatrixD3D9 );

      Transformation3DChanged = false;

      // unset last 3d material
      if (CurrentRenderMode == ERM_3D &&
         Material.MaterialType >= 0 && Material.MaterialType < (s32)MaterialRenderers.size())
         MaterialRenderers[Material.MaterialType]->OnUnsetMaterial();
   }

   if (texture)
   {
      pID3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
      pID3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
      pID3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

      if (alphaChannel)
      {
         if (alpha) /// ADDED IF
         {
                pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
                pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
                pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

                pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE ); /// CHANGED
                pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
                pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ); /// ADDED

                pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                pID3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                pID3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
         }
         else
         {
                pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
                pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
                pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

                pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1 );
                pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

                pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                pID3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                pID3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
         }
      }
      else
      {
         if (alpha)
         {
            pID3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
            pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
            pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            pID3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            pID3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
         }
         else
         {
            pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
            pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
            pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
            pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
            pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
         }
      }

   }
   else
   {
      if (alpha)
      {
         pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
         pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
         pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
         pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1);
         pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
         pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
         pID3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
         pID3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
      }
      else
      {
         pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
         pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
         pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
         pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
         pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
      }
   }

   CurrentRenderMode = ERM_2D;
}


*/

