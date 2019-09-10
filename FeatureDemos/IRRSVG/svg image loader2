class agg_svg_loader : public irr::video::IImageLoader
{
public:
   agg_svg_loader(IVideoDriver* driver)
   {
      video_driver = driver;
   }
   ~agg_svg_loader()
   {
      video_driver = 0;
   }
   virtual bool isALoadableFileExtension(const io::path& filename) const
   {
      return core::hasFileExtension ( filename, "svg" );
   }
   virtual bool isALoadableFileFormat(irr::io::IReadFile* file) const
   {
      return (false);
   }
   virtual irr::video::IImage* loadImage(irr::io::IReadFile* file) const
   {
      agg::svg::path_renderer m_path;
      agg::svg::parser p(m_path);
      p.parse(file->getFileName().c_str());
      double m_min_x = 0.0;
      double m_min_y = 0.0;
      double m_max_x = 0.0;
      double m_max_y = 0.0;
      m_path.bounding_rect(&m_min_x, &m_min_y, &m_max_x, &m_max_y);
      const dimension2d<u32>& image_size = dimension2d<u32>((irr::u32)m_max_x, (irr::u32)m_max_y);
      IImage* image = video_driver->createImage(ECF_A8R8G8B8, image_size);
      typedef row_accessor<irr::u32> rendering_buffer_u32;
      rendering_buffer_u32 rbuf((irr::u32*)image->lock(), image_size.Width, image_size.Height, image_size.Width);
      agg_pixel_type pixf(rbuf);
      agg::renderer_base<agg_pixel_type> renb(pixf);
      agg::renderer_scanline_aa_solid<agg::renderer_base<agg_pixel_type>> ren(renb);
      renb.clear(agg::rgba8(255, 255, 255, 0));
      agg::rasterizer_scanline_aa<> ras;
      agg::scanline_p8 sl;
      agg::trans_affine mtx;
      agg::render_scanlines(ras, sl, ren);
      m_path.render(ras, sl, ren, mtx, renb.clip_box(), 1.0);
      image->unlock();
      return image;
   }
   protected:
      IVideoDriver* video_driver;
};

video_driver->addExternalImageLoader(new agg_svg_loader(video_driver));
ITexture* tex = video_driver->getTexture("test.svg");
