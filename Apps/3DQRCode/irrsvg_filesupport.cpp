//#define SVG2
#ifdef SVG2
#include "agg_scanline_p.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"
#include "agg_svg_parser.h"
#include <irrlicht.h>

using namespace agg;
using namespace svg;

typedef row_accessor<irr::u32> rendering_buffer_u32;
typedef pixfmt_alpha_blend_rgba<blender_bgra32, rendering_buffer_u32, pixel32_type> agg_pixel_type;

agg::svg::path_renderer* agg_svg_path(char* file_name = "tiger.svg")
{
   agg::svg::path_renderer* m_path = new path_renderer();
   agg::svg::parser p(*m_path);
   p.parse(file_name);
   return m_path;
}

agg_svg_ITexture(agg::svg::path_renderer* m_path, IVideoDriver* driver, fschar_t* texture_name = "", double scale_value = 1.0, double rotate_value = 0.0, double expand_value = 0.0, video::ECOLOR_FORMAT color_format = ECF_A8R8G8B8, u32 alpha_value = 0, int stride_value = 1)
{
   double m_min_x = 0.0;
   double m_min_y = 0.0;
   double m_max_x = 0.0;
   double m_max_y = 0.0;
   //m_path->arrange_orientations();
   m_path->bounding_rect(&m_min_x, &m_min_y, &m_max_x, &m_max_y);
   m_max_x *= scale_value;
   m_max_y *= scale_value;
   const dimension2d<u32>& image_size = dimension2d<u32>((irr::u32)m_max_x, (irr::u32)m_max_y);
   IImage* image = driver->createImage(color_format, image_size);
   rendering_buffer_u32 rbuf((irr::u32*)image->lock(), image_size.Width, image_size.Height, image_size.Width*stride_value);
   //row_accessor<agg::int8u> rbuf((irr::u32*)image->lock(), image_size.Width, image_size.Height, image_size.Width*stride_value);
   agg_pixel_type pixf(rbuf);
   agg::renderer_base<agg_pixel_type> renb(pixf);
   agg::renderer_scanline_aa_solid<agg::renderer_base<agg_pixel_type>> ren(renb);
   renb.clear(agg::rgba8(255, 255, 255, alpha_value));
   agg::rasterizer_scanline_aa<> ras;
   agg::scanline_p8 sl;
   agg::trans_affine mtx;
   mtx *= agg::trans_affine_scaling(scale_value);
   mtx *= agg::trans_affine_rotation(agg::deg2rad(rotate_value));
   agg::render_scanlines(ras, sl, ren);
   m_path->expand(expand_value);
   m_path->render(ras, sl, ren, mtx, renb.clip_box(), 1.0);
   image->unlock();
   ITexture* texture = driver->addTexture(texture_name, image);
   image->drop();
   if (image)
      delete image;
   return texture;
}
#endif
