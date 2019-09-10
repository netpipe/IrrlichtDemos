ITexture* test_vectors(IVideoDriver* driver, video::ECOLOR_FORMAT image_format = ECF_R8G8B8, const dimension2d<u32>& image_size = dimension2d<u32>(640, 480), char* texture_name = "texture_01", u32 alpha_value = 0)
{
   IImage* image = driver->createImage(image_format, image_size);

   agg::rendering_buffer rbuf;
   rbuf.attach((unsigned char*)image->lock(), image_size.Width, image_size.Height, image_size.Width*4);

   // Pixel format and basic primitives renderer
   agg::pixfmt_bgra32 pixf(rbuf);
   agg::renderer_base<agg::pixfmt_bgra32> renb(pixf);

   renb.clear(agg::rgba8(255, 255, 255, alpha_value));

   // Scanline renderer for solid filling.
   agg::renderer_scanline_aa_solid<agg::renderer_base<agg::pixfmt_bgra32> > ren(renb);

   // Rasterizer & scanline
   agg::rasterizer_scanline_aa<> ras;
   agg::scanline_p8 sl;

   // Polygon (triangle)
   ras.move_to_d(20.7, 34.15);
   ras.line_to_d(398.23, 123.43);
   ras.line_to_d(165.45, 401.87);

   // Setting the attrribute (color) & Rendering
   ren.color(agg::rgba8(80, 90, 60));
   agg::render_scanlines(ras, sl, ren);

   image->unlock();
   return driver->addTexture(texture_name, image);
}
