// Copyright(c) Max Kolosov 2011 maxkolosov@inbox.ru
// http://vosolok2008.narod.ru
// BSD license

#define _CRT_SECURE_NO_WARNINGS


#include "named_colors_cairo.h"
#include "cairo.h"


#ifndef M_PI
#define M_PI 3.14159265f
#endif

struct gtag
{
	bool open;
	core::stringw name;
};

struct container_fill
{
	bool f;
	double r, g, b, a;
};

struct container_stroke
{
	bool f, set_linecap, set_linejoin;
	double r, g, b, a, width, miterlimit;
	cairo_line_cap_t linecap;
	cairo_line_join_t linejoin;
};

struct container_style
{
	bool set_linecap, set_linejoin;
	double r, g, b, a, width, miterlimit;
	cairo_fill_rule_t fill_rule;
	cairo_line_cap_t linecap;
	cairo_line_join_t linejoin;
	core::stringw name;
};


inline int __cdecl cmp_color_cairo(const void* p1, const void* p2)
{
	return wcscmp(((cairo_named_color*)p1)->name, ((cairo_named_color*)p2)->name);
}


class svg_cairo_image
{
public:
	svg_cairo_image(IVideoDriver* video_driver, IFileSystem* fs, const irr::io::path& file_name = "tiger.svg", bool content_unicode = true, double alpha_value = 0.0, video::ECOLOR_FORMAT color_format = ECF_A8R8G8B8, cairo_antialias_t antialias_type = CAIRO_ANTIALIAS_DEFAULT, double scale_x = 1.0, double scale_y = 1.0)
	{
		_scale_x_ = scale_x;
		_scale_y_ = scale_y;
		_alpha_value_ = alpha_value;
		_file_name_ = file_name;
		_color_format_ = color_format;
		_video_driver_ = video_driver;
		_use_alpha_ = use_alpha();
		_current_pattern_ = cairo_pattern_create_rgba(0, 0, 0, _alpha_value_);
		_cf_.r = _cf_.g = _cf_.b = _cf_.a = 0.0; _cf_.f = false;
		_cs_.r = _cs_.g = _cs_.b = _cs_.a = 0.0; _cs_.f = false; _cs_.width = 0.1;
		IXMLReader* xml_reader;
		if (content_unicode)
			xml_reader = fs->createXMLReader(_file_name_);
		else
			xml_reader = dynamic_cast<IXMLReader*>(fs->createXMLReaderUTF8(file_name));
		if (xml_reader)
		{
			bool flag_start_linear_gradient = false, defs_section = false, open_g = false;
			u32 w = 0, h = 0, gradients_count = 0;
			unsigned int attr_count = 0;
			core::array<core::stringw> list_attr;
			core::array<gradient> gradients;
			//core::array<gtag> gtags;
			stringw g_style, g_transform, current_node_name;//current read iteration doing name and data as same value in one variable, therefore we must save current name value in other variable
			while (xml_reader->read())
			{
				switch (xml_reader->getNodeType())
				{
				case io::EXN_ELEMENT:
					{
					wchar_t symbol_percent = L'%';
					const wchar_t* node_name = xml_reader->getNodeName();
					current_node_name = stringw(node_name);
					attr_count = xml_reader->getAttributeCount();
					if (attr_count)
					{
						if (_wcsnicmp(node_name, L"svg", 3) == 0)//Compare characters of two strings without regard to case
						{
							const wchar_t* viewBox = xml_reader->getAttributeValue(L"viewBox");
							if (viewBox)
							{
								core::stringw(viewBox).trim().split(list_attr, L" ");
								_width_ = _wtof(list_attr[2].c_str());
								_height_ = _wtof(list_attr[3].c_str());
							}
							const wchar_t* width = xml_reader->getAttributeValue(L"width");
							if (width)
							{
								if (wcschr(width, symbol_percent) != NULL)
								{
									if (list_attr.size() > 2)
										_width_ = _wtof(list_attr[2].c_str()) * _wtof(width) / 100;
								}
								else
									_width_ = xml_reader->getAttributeValueAsFloat(L"width");
							}
							const wchar_t* height = xml_reader->getAttributeValue(L"height");
							if (height)
							{
								if (wcschr(height, symbol_percent) != NULL)
								{
									if (list_attr.size() > 3)
										_height_ = _wtof(list_attr[3].c_str()) * _wtof(height) / 100;
								}
								else
									_height_ = xml_reader->getAttributeValueAsFloat(L"height");
							}
							list_attr.clear();
							w = (u32)(_width_ * _scale_x_);
							if (_width_ > (double)w)
								w++;
							h = (u32)(_height_ * _scale_y_);
							if (_height_ > (double)h)
								h++;
							//const dimension2d<u32> image_size = dimension2d<u32>(w, h);
							//_image_ = _video_driver_->createImage(_color_format_, image_size);
							//cairo_format_t cairo_format_color = color_format_to_cairo(_color_format_);
							//_surface_ = cairo_image_surface_create_for_data((unsigned char*)_image_->lock(), cairo_format_color, (int)image_size.Width, (int)image_size.Height, cairo_format_stride_for_width(cairo_format_color, (int)image_size.Width));
							_surface_ = cairo_image_surface_create(color_format_to_cairo(_color_format_), (int)w, (int)h);
							_cr_ = cairo_create(_surface_);
							cairo_set_antialias(_cr_, antialias_type);
							cairo_scale(_cr_, _scale_x_, _scale_y_);
							cairo_pattern_set_filter(cairo_get_source(_cr_), CAIRO_FILTER_NEAREST);
							cairo_pattern_set_extend(cairo_get_source(_cr_), CAIRO_EXTEND_REPEAT);
							// SETUP WHITE BACKGRAUND COLOR, DEFAULT IS BLACK
							cairo_set_source_rgb(_cr_, 1.0, 1.0, 1.0);
							if (_use_alpha_)
							{
								//cairo_set_source_rgba(_cr_, 1.0, 1.0, 1.0, _alpha_value_);
								cairo_paint_with_alpha(_cr_, _alpha_value_);
							}
							else
							{
								//cairo_set_source_rgb(_cr_, 1.0, 1.0, 1.0);
								cairo_paint(_cr_);
							}
							parse_fill_rule(xml_reader->getAttributeValueSafe(L"clip-rule"));
							///////////////////TEST/////////////////////////
							//if (_use_alpha_)
							//	cairo_set_source_rgba(_cr_, 0.8, 0.0, 0.0, _alpha_value_);
							//else
							//	cairo_set_source_rgb(_cr_, 0.8, 0.0, 0.0);
							//cairo_select_font_face(_cr_, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
							//cairo_set_font_size(_cr_, 20.0);
							//cairo_move_to(_cr_, 0, 20);
							//cairo_show_text(_cr_, "1 TEST Text");
							///////////////////TEST/////////////////////////
						}
						else if (_wcsnicmp(node_name, L"defs", 4) == 0)
							defs_section = true;
						else if (_wcsnicmp(node_name, L"linearGradient", 14) == 0)
						{
							gradient gr(xml_reader->getAttributeValue(L"id"), xml_reader->getAttributeValueSafe(L"xlink:href"));
							//flag_start_linear_gradient = true;
							gradients_count++;
							gradients.reallocate(gradients_count);
							gradients.insert(gr);
						}
						else if (wcslen(node_name) == 1)
						{
							if (node_name[0] == L'g')
							{
								//gtag g;
								//g.open = true;
								//g.name = core::stringw(xml_reader->getAttributeValue(L"id"));
								//gtags.push_back(g);
								//if (open_g)
								//	cairo_pop_group_to_source(_cr_);
								//	cairo_restore(_cr_);
								//wprintf(L"=== G tag OPEN %s\n", xml_reader->getAttributeValue(L"id"));
								//else
								//	printf("=== G tag CLOSE = %d\n", open_g);
								cairo_save(_cr_);
								//cairo_push_group() calls cairo_save()
								//cairo_push_group(_cr_);
								parse_transform(xml_reader->getAttributeValue(L"transform"));
								//if (xml_reader->getAttributeValue(L"transform"))
								//	g_transform = stringw(xml_reader->getAttributeValue(L"transform"));
								//parse_style(xml_reader->getAttributeValue(L"style"));
								g_style = stringw(xml_reader->getAttributeValue(L"style"));
								//parse_attributes(xml_reader);
								//if (xml_reader->isEmptyElement())
								//	cairo_restore(_cr_);
								open_g = true;
							}
						}
						else if (_wcsnicmp(node_name, L"path", 4) == 0)
						{
							const wchar_t* d = xml_reader->getAttributeValue(L"d");
							if (d)
							{
								cairo_save(_cr_);
								parse_transform(xml_reader->getAttributeValue(L"transform"));
								if (g_transform.size())
									parse_transform(g_transform.c_str());
								parse_style(xml_reader->getAttributeValue(L"style"));
								if (g_style.size())
									parse_style(g_style.c_str());
								parse_attributes(xml_reader);
								parse_path(d);
								//double min_x, min_y, w1, h1;
								//cairo_path_extents(_cr_, &min_x, &min_y, &w1, &h1);
								//printf("== %g, %g, %g, %g\n", min_x, min_y, w1, h1);
								paint();
								cairo_restore(_cr_);
								//printf("===\n%s\n===\n", cairo_status_to_string(cairo_status(_cr_)));
							}
						}
						else if (_wcsnicmp(node_name, L"rect", 4) == 0)
						{
							double w = _wtof(xml_reader->getAttributeValue(L"width"));
							double h = _wtof(xml_reader->getAttributeValue(L"height"));
							if(w != 0.0 && h != 0.0)
							{
								double x = _wtof(xml_reader->getAttributeValue(L"x"));
								double y = _wtof(xml_reader->getAttributeValue(L"y"));
								if(w < 0.0) printf("ERROR rect %s: Invalid width: %f\n", xml_reader->getAttributeValue(L"id"), w);
								if(h < 0.0) printf("ERROR rect %s: Invalid height: %f\n", xml_reader->getAttributeValue(L"id"), h);
								cairo_save(_cr_);
								//cairo_new_sub_path(_cr_);
								cairo_rectangle(_cr_, x, y, w, h);
								//cairo_close_path(_cr_);
								parse_transform(xml_reader->getAttributeValue(L"transform"));
								parse_style(xml_reader->getAttributeValue(L"style"));
								parse_attributes(xml_reader);
								//cairo_clip(_cr_);
								paint();
								cairo_restore(_cr_);
							}
						}
						else if (wcslen(node_name) == 4)
						{
							if (_wcsnicmp(node_name, L"line", 4) == 0)
							{
								double x1 = _wtof(xml_reader->getAttributeValue(L"x1"));
								double y1 = _wtof(xml_reader->getAttributeValue(L"y1"));
								double x2 = _wtof(xml_reader->getAttributeValue(L"x2"));
								double y2 = _wtof(xml_reader->getAttributeValue(L"y2"));
								cairo_save(_cr_);
								cairo_move_to(_cr_, x1, y1);
								cairo_line_to(_cr_, x2, y2);
								parse_style(xml_reader->getAttributeValue(L"style"));
								parse_transform(xml_reader->getAttributeValue(L"transform"));
								parse_attributes(xml_reader);
								paint();
								cairo_restore(_cr_);
							}
						}
						else if (_wcsnicmp(node_name, L"polyline", 8) == 0)
						{
							const wchar_t* attr_value = xml_reader->getAttributeValue(L"points");
							if (attr_value)
							{
								u32 points_count = stringw(attr_value).trim().split(list_attr, L", ", 2);
								if (points_count > 3)
								{
									cairo_save(_cr_);
									cairo_move_to(_cr_, _wtof(list_attr[0].c_str()), _wtof(list_attr[1].c_str()));
									for (int i = 2; i < points_count; i += 2)
										cairo_line_to(_cr_, _wtof(list_attr[i].c_str()), _wtof(list_attr[i+1].c_str()));
									//cairo_close_path(_cr_);
									parse_style(xml_reader->getAttributeValue(L"style"));
									parse_transform(xml_reader->getAttributeValue(L"transform"));
									parse_attributes(xml_reader);
									paint();
									cairo_restore(_cr_);
								}
								list_attr.clear();
							}
						}
						else if (_wcsnicmp(node_name, L"polygon", 7) == 0)
						{
							const wchar_t* attr_value = xml_reader->getAttributeValue(L"points");
							if (attr_value)
							{
								u32 points_count = stringw(attr_value).trim().split(list_attr, L", ", 2);
								if (points_count > 3)
								{
									cairo_save(_cr_);
									cairo_move_to(_cr_, _wtof(list_attr[0].c_str()), _wtof(list_attr[1].c_str()));
									for (int i = 2; i < points_count; i += 2)
										cairo_line_to(_cr_, _wtof(list_attr[i].c_str()), _wtof(list_attr[i+1].c_str()));
									//cairo_close_path(_cr_);
									parse_style(xml_reader->getAttributeValue(L"style"));
									parse_transform(xml_reader->getAttributeValue(L"transform"));
									parse_attributes(xml_reader);
									paint();
									cairo_restore(_cr_);
								}
								list_attr.clear();
							}
						}
					}
					break;
					}
				case io::EXN_ELEMENT_END:
					{
					const wchar_t* node_name = xml_reader->getNodeName();
					if (wcslen(node_name) == 1)
					{
						if (node_name[0] == L'g')
						{
							//wprintf(L"--- end g transform = %s\n", g_transform.c_str());
							//wprintf(L"--- end g style = %s\n", g_style.c_str());
							//parse_transform(g_transform.c_str());
							//g_transform = L"";
							//parse_style(g_style.c_str());
							g_style = L"";
							//paint();
							//cairo_pop_group() calls cairo_restore()
							//_current_pattern_ = cairo_pop_group(_cr_);
							//cairo_pop_group_to_source(_cr_);
							cairo_restore(_cr_);
							//open_g = false;
							//wprintf(L"=== G tag CLOSE %s\n", xml_reader->getAttributeValue(L"id"));
							//close_gtag(gtags);
						}
					}
					else if (_wcsnicmp(node_name, L"defs", 4) == 0)
						defs_section = false;
					else if (_wcsnicmp(node_name, L"linearGradient", 14) == 0)
						flag_start_linear_gradient = false;
					break;
					}
				case io::EXN_TEXT:
					{
					const wchar_t* node_name = xml_reader->getNodeName();
					if (current_node_name.equalsn(L"style", 5))
					{
						current_node_name = "";
						css_parse_style_data(xml_reader->getNodeData());
					}
					break;
					}
				case io::EXN_COMMENT:
					{
					//wprintf(L"--- EXN_COMMENT id node = %s\n", xml_reader->getAttributeValue(L"id"));
					break;
					}
				case io::EXN_CDATA:
					{
					//wprintf(L"--- EXN_CDATA id node = %s\n", xml_reader->getAttributeValue(L"id"));
					break;
					}
				case io::EXN_UNKNOWN:
					{
					//wprintf(L"--- EXN_UNKNOWN id node = %s\n", xml_reader->getAttributeValue(L"id"));
					break;
					}
				default:
					{
					//printf("--- UNREGISTER NODE TYPE\n");
					break;
					}
				}
			}
			//cairo_restore(_cr_);
			//for (u32 i=0; i < gtags.size(); i++)
			//	wprintf(L"=== %d, %s\n", gtags[i].open, gtags[i].name.c_str());
			//gtags.clear();
			//for (int i = 0; i < gradients.size(); i++)
			//	wprintf(L"=== gradient id = %s, xlink:href = %s\n", gradients[i].id(), gradients[i].xlink_href());
			//cairo_surface_flush(_surface_);
			//cairo_surface_finish(_surface_); /* data will go out of scope */
			//cairo_surface_write_to_png(_surface_, (_file_name_.trim("svg") + "png").c_str());
			/////////////////////////////START TEST///////////////////////////////
			////cairo_surface_t* surface = cairo_image_surface_create(color_format_to_cairo(_color_format_), 400, 150);
			////cairo_t* cr = cairo_create(surface);
			////cairo_push_group(_cr_);
			////cairo_save(_cr_);
			////cairo_identity_matrix(_cr_);
			////cairo_set_line_width(_cr_, 1.0);
			//if (_use_alpha_)
			//	cairo_set_source_rgba(_cr_, 0.8, 0.0, 0.0, _alpha_value_);
			//else
			//	cairo_set_source_rgb(_cr_, 0.8, 0.0, 0.0);
			////cairo_text_extents_t extents;
			//cairo_select_font_face(_cr_, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
			//cairo_set_font_size(_cr_, 20.0);
			////cairo_text_extents(_cr_, "Cairo", &extents);
			////cairo_move_to(_cr_, 10 - (extents.width/2. + extents.x_bearing), 10 - (extents.height/2. + extents.y_bearing));
			//cairo_move_to(_cr_, 0, 40);
			//cairo_show_text(_cr_, "2 Test TEXT");
			////cairo_text_path(_cr_, "Cairo");
			////cairo_fill(_cr_);
			////cairo_restore(_cr_);
			////_current_pattern_ = cairo_pop_group(_cr_);
			////cairo_surface_write_to_png(_surface_, (_file_name_ + ".png").c_str());
			////cairo_surface_destroy(surface);
			/////////////////////////////END TEST///////////////////////////////
			//_image_ = get_image();
			//_image_->unlock();
		}
		else
			printf("!!! ERROR create svg_cairo_image from file = %s\n", _file_name_.c_str());
	}
	bool _close_gtag(core::array<gtag>& tags, const wchar_t* value)
	{
		bool result = false;
		u32 n = (value == NULL) ? 0 : wcslen(value);
		for (u32 i = 0; i < tags.size(); i++)
		{
			if (tags[i].name.equalsn(value, n))
			{
				tags[i].open = false;
				result = true;
				break;
			}
		}
		//wprintf(L"=== CLOSE = %d, %s\n", result, value);
		return result;
	}
	void close_gtag(core::array<gtag>& tags)
	{
		for (u32 i = tags.size()-1; i >= 0; i--)
		{
			if (tags[i].open)
			{
				tags[i].open = false;
				//wprintf(L"=== CLOSE %s\n", tags[i].name.c_str());
				break;
			}
		}
	}
	bool use_alpha()
	{
		if (_color_format_ == ECF_A1R5G5B5 || _color_format_ == ECF_A8R8G8B8 || _color_format_ == ECF_A16B16G16R16F || _color_format_ == ECF_A32B32G32R32F)
			return true;
		else
			return false;
	}
	cairo_format_t color_format_to_cairo(ECOLOR_FORMAT color_format)
	{
		switch(color_format)
		{
			case ECF_A1R5G5B5:
				return CAIRO_FORMAT_A1;
			case ECF_R5G6B5:
				return CAIRO_FORMAT_RGB16_565;
			case ECF_R8G8B8:
				return CAIRO_FORMAT_RGB24;
			case ECF_A8R8G8B8:
				return CAIRO_FORMAT_ARGB32;
			case ECF_R16F:
				//return CAIRO_FORMAT_A8;
			case ECF_G16R16F:
			case ECF_A16B16G16R16F:
			case ECF_R32F:
			case ECF_G32R32F:
			case ECF_A32B32G32R32F:
			case ECF_UNKNOWN:
				return CAIRO_FORMAT_INVALID;
			default:
				return CAIRO_FORMAT_INVALID;
		}
	}
	void parse_color_name(const wchar_t* value, double& r, double& g, double& b, double& a)
	{
		cairo_named_color c;
#ifdef _MSC_VER
		wcscpy_s(c.name, sizeof(c.name), value);
#else
		wcscpy(c.name, value);
#endif
		const void* p = bsearch(&c, cairo_named_colors, sizeof(cairo_named_colors) / sizeof(cairo_named_colors[0]), sizeof(cairo_named_colors[0]), cmp_color_cairo);
		if(p)
		{
			const cairo_named_color* pc = (const cairo_named_color*)p;
			r = pc->r; g = pc->g; b = pc->b; a = pc->a;
		}
		else
		{
			r = 1.0; g = 1.0; b = 1.0; a = 1.0;
		}
	}
	void parse_color(core::stringw s, double& r, double& g, double& b)
	{
		if(s.size() > 6)
		{
			r = wcstoul(s.subString(1,2).c_str(), 0, 16)/256.0;
			g = wcstoul(s.subString(3,2).c_str(), 0, 16)/256.0;
			b = wcstoul(s.subString(5,2).c_str(), 0, 16)/256.0;
		}
		else
		{
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
	}
	void parse_fill(core::stringw value)
	{
		if (value.equalsn(L"none", 4))
			_cf_.f = false;
		else if (value.equalsn(L"#", 1))
		{
			parse_color(value, _cf_.r, _cf_.g, _cf_.b);
			_cf_.f = true;
		}
		else if (value.equalsn(L"url", 3))
		{//must be replaced with valid code
			_cf_.r = 0.5;
			_cf_.g = 0.5;
			_cf_.b = 0.5;
			_cf_.f = true;
		}
		else
		{
			parse_color_name(value.c_str(), _cf_.r, _cf_.g, _cf_.b, _cf_.a);
			_cf_.f = true;
		}
	}
	void parse_fill_rule(const wchar_t* value)
	{
		if (_wcsnicmp(value, L"evenodd", 7) == 0)
			cairo_set_fill_rule(_cr_, CAIRO_FILL_RULE_EVEN_ODD);
		else// if (_wcsnicmp(value, L"winding", 7) == 0)
			cairo_set_fill_rule(_cr_, CAIRO_FILL_RULE_WINDING);
	}
	void parse_stroke(core::stringw value)
	{
		if (value.equalsn(L"none", 4))
			_cs_.f = false;
		else
		{
			parse_color(value, _cs_.r, _cs_.g, _cs_.b);
			_cs_.f = true;
		}
	}
	void parse_linecap(core::stringw value)
	{
		if (value.equalsn(L"butt", 4))
			_cs_.linecap = CAIRO_LINE_CAP_BUTT;
		else if (value.equalsn(L"square", 6))
			_cs_.linecap = CAIRO_LINE_CAP_SQUARE;
		else if (value.equalsn(L"round", 5))
			_cs_.linecap = CAIRO_LINE_CAP_ROUND;
		_cs_.set_linecap = true;
	}
	void parse_linejoin(core::stringw value)
	{
		if (value.equalsn(L"miter", 5))
			_cs_.linejoin = CAIRO_LINE_JOIN_MITER;
		else if (value.equalsn(L"round", 5))
			_cs_.linejoin = CAIRO_LINE_JOIN_ROUND;
		else if (value.equalsn(L"bevel", 5))
			_cs_.linejoin = CAIRO_LINE_JOIN_BEVEL;
		_cs_.set_linejoin = true;
	}
	void parse_attributes(IXMLReader* xml_reader)
	{
		const wchar_t* fill = xml_reader->getAttributeValue(L"fill");
		if (fill)
			parse_fill(stringw(fill));
		const wchar_t* fill_rule = xml_reader->getAttributeValue(L"fill-rule");
		if (fill_rule)
			parse_fill_rule(fill_rule);
		double fill_opacity = xml_reader->getAttributeValueAsFloat(L"fill-opacity");
		if (fill_opacity)
			_cf_.a = fill_opacity;
		const wchar_t* stroke = xml_reader->getAttributeValue(L"stroke");
		if (stroke)
			parse_stroke(stringw(stroke));
		double stroke_opacity = xml_reader->getAttributeValueAsFloat(L"stroke-opacity");
		if (stroke_opacity)
			_cs_.a = stroke_opacity;
		double stroke_width = xml_reader->getAttributeValueAsFloat(L"stroke-width");
		if (stroke_width)
			_cs_.width = stroke_width;
		double stroke_miterlimit = xml_reader->getAttributeValueAsFloat(L"stroke-miterlimit");
		if (stroke_miterlimit)
			_cs_.miterlimit = stroke_miterlimit;
		const wchar_t* linecap = xml_reader->getAttributeValue(L"stroke-linecap");
		if (linecap)
			parse_linecap(stringw(linecap));
		const wchar_t* linejoin = xml_reader->getAttributeValue(L"stroke-linejoin");
		if (linejoin)
			parse_linejoin(stringw(linejoin));
	}
	void parse_style(const wchar_t* value)
	{
		bool has_stroke = false;
		core::array<core::stringw> list_attr;
		core::stringw v(value);
		if (v.find(L"stroke:") != -1 && v.find(L"stroke:none") == -1)
			has_stroke = true;
		if (v.trim().split(list_attr, L";"))
		{
			for (int i = 0; i < list_attr.size(); i++)
			{
				if (list_attr[i].equalsn(L"fill:", 5))
					parse_fill(list_attr[i].trim("fill:"));
				else if (list_attr[i].equalsn(L"fill-rule:", 10))
					parse_fill_rule(list_attr[i].trim(L"fill-rule:").c_str());
				else if (list_attr[i].equalsn(L"fill-opacity:", 13))
					_cf_.a = _wtof(list_attr[i].trim(L"fill-opacity:").c_str());
				else if (list_attr[i].equalsn(L"stroke:", 7))
					parse_stroke(list_attr[i].trim("stroke:"));
				else if (list_attr[i].equalsn(L"stroke-opacity:", 15))
					_cs_.a = _wtof(list_attr[i].trim(L"stroke-opacity:").c_str());
				else if (list_attr[i].equalsn(L"stroke-width:", 13))
					_cs_.width = _wtof(list_attr[i].trim(L"stroke-width:").c_str());
				else if (list_attr[i].equalsn(L"stroke-miterlimit:", 18))
					_cs_.miterlimit = _wtof(list_attr[i].trim(L"stroke-miterlimit:").c_str());
				else if (list_attr[i].equalsn(L"stroke-linecap:", 15))
					parse_linecap(list_attr[i].trim(L"stroke-linecap:"));
				else if (list_attr[i].equalsn(L"stroke-linejoin:", 16))
					parse_linejoin(list_attr[i].trim(L"stroke-linejoin:"));
			}
			list_attr.clear();
		}
	}
	void css_parse_fill(core::stringw value, container_style& css_style)
	{
		if (!value.equalsn(L"none", 4))
			parse_color(value, css_style.r, css_style.g, css_style.b);
	}
	void css_parse_fill_rule(core::stringw value, container_style& css_style)
	{
		if (value.equalsn(L"evenodd", 7))
			css_style.fill_rule = CAIRO_FILL_RULE_EVEN_ODD;
			//cairo_set_fill_rule(_cr_, CAIRO_FILL_RULE_EVEN_ODD);
		else if (value.equalsn(L"winding", 7))
			css_style.fill_rule = CAIRO_FILL_RULE_WINDING;
			//cairo_set_fill_rule(_cr_, CAIRO_FILL_RULE_WINDING);
	}
	void css_parse_stroke(core::stringw value, container_style& css_style)
	{
		if (!value.equalsn(L"none", 4))
			parse_color(value, css_style.r, css_style.g, css_style.b);
	}
	void css_parse_linecap(core::stringw value, container_style& css_style)
	{
		if (value.equalsn(L"butt", 4))
			css_style.linecap = CAIRO_LINE_CAP_BUTT;
		else if (value.equalsn(L"square", 6))
			css_style.linecap = CAIRO_LINE_CAP_SQUARE;
		else if (value.equalsn(L"round", 5))
			css_style.linecap = CAIRO_LINE_CAP_ROUND;
		css_style.set_linecap = true;
	}
	void css_parse_linejoin(core::stringw value, container_style& css_style)
	{
		if (value.equalsn(L"miter", 5))
			css_style.linejoin = CAIRO_LINE_JOIN_MITER;
		else if (value.equalsn(L"round", 5))
			css_style.linejoin = CAIRO_LINE_JOIN_ROUND;
		else if (value.equalsn(L"bevel", 5))
			css_style.linejoin = CAIRO_LINE_JOIN_BEVEL;
		css_style.set_linejoin = true;
	}
	void css_parse_style(core::stringw value, container_style& css_style)
	{
		bool has_stroke = false;
		core::array<core::stringw> list_attr;
		if (value.find(L"stroke:") != -1 && value.find(L"stroke:none") == -1)
			has_stroke = true;
		if (value.trim().split(list_attr, L";"))
		{
			for (int i = 0; i < list_attr.size(); i++)
			{
				if (list_attr[i].equalsn(L"fill:", 5))
					css_parse_fill(list_attr[i].trim("fill:"), css_style);
				else if (list_attr[i].equalsn(L"fill-rule:", 10))
					css_parse_fill_rule(list_attr[i].trim(L"fill-rule:").c_str(), css_style);
				else if (list_attr[i].equalsn(L"fill-opacity:", 13))
					css_style.a = _wtof(list_attr[i].trim(L"fill-opacity:").c_str());
				else if (list_attr[i].equalsn(L"stroke:", 7))
					css_parse_stroke(list_attr[i].trim("stroke:"), css_style);
				else if (list_attr[i].equalsn(L"stroke-opacity:", 15))
					css_style.a = _wtof(list_attr[i].trim(L"stroke-opacity:").c_str());
				else if (list_attr[i].equalsn(L"stroke-width:", 13))
					css_style.width = _wtof(list_attr[i].trim(L"stroke-width:").c_str());
				else if (list_attr[i].equalsn(L"stroke-miterlimit:", 18))
					css_style.miterlimit = _wtof(list_attr[i].trim(L"stroke-miterlimit:").c_str());
				else if (list_attr[i].equalsn(L"stroke-linecap:", 15))
					css_parse_linecap(list_attr[i].trim(L"stroke-linecap:"), css_style);
				else if (list_attr[i].equalsn(L"stroke-linejoin:", 16))
					css_parse_linejoin(list_attr[i].trim(L"stroke-linejoin:"), css_style);
			}
			list_attr.clear();
		}
	}
	void css_parse_style_data(const wchar_t* data)
	{
		core::array<core::stringw> lines, args;
		if (stringw(data).split(lines, L"\n"))
		{
			for (u32 i = 0; i < lines.size(); i++)
			{
				if (lines[i].trim("} \t\n\r").split(args, L"{"))
				{
					//if (_css_style_.allocated_size() < i+1)
					//	_css_style_.reallocate(i+1);
					container_style cs;
					cs.name = args[0].trim(". ");
					css_parse_style(args[1].trim(), cs);
					_css_style_.push_back(cs);
					args.clear();
				}
			}
			lines.clear();
		}
	}
	void parse_transform(const wchar_t* value)
	{
		//wprintf(L"--- transform = %s\n", value);
		if (value)
		{
			core::array<core::stringw> list_attr;
			core::stringw(value).trim(core::stringw(")")).split(list_attr, L"(");
			if (list_attr.size())
			{
				if (list_attr[0].equals_substring_ignore_case(stringw("matrix")))
				{
					core::array<double> args = string_split_d(list_attr[1].c_str(), 6);
					cairo_matrix_t m = {args[0], args[1], args[2], args[3], args[4], args[5]};
					cairo_transform(_cr_, &m);
				}
				else if (list_attr[0].equals_substring_ignore_case(stringw("translate")))
				{
					core::array<double> args = string_split_d(list_attr[1].c_str(), 2);
					cairo_translate(_cr_, args[0], args[1]);
				}
				else if (_wcsnicmp(list_attr[0].c_str(), L"rotate", 6) == 0)
				{
					core::array<double> args = string_split_d(list_attr[1].c_str(), 3);
					if (args[1] == 0.0 && args[2] == 0.0)
						cairo_rotate(_cr_, M_PI / args[0]);
					else
					{
						cairo_translate(_cr_, -args[1], -args[2]);
						cairo_rotate(_cr_, M_PI / args[0]);
						cairo_translate(_cr_, args[1], args[2]);
					}
				}
				else if (_wcsnicmp(list_attr[0].c_str(), L"scale", 5) == 0)
				{
					core::array<double> args = string_split_d(list_attr[1].c_str(), 2);
					if (args[1] == 0.0)
						args[1] = args[0];
					cairo_scale(_cr_, args[0], args[1]);
				}
				else if (_wcsnicmp(list_attr[0].c_str(), L"skewX", 5) == 0)
				{
					cairo_matrix_t skew_x = {1.0, 0.0, _wtof(list_attr[1].c_str()), 1.0, 0.0, 0.0};
					cairo_transform(_cr_, &skew_x);
				}
				else if (_wcsnicmp(list_attr[0].c_str(), L"skewY", 5) == 0)
				{
					cairo_matrix_t skew_y = {1.0, _wtof(list_attr[1].c_str()), 0.0, 1.0, 0.0, 0.0};
					cairo_transform(_cr_, &skew_y);
				}
				list_attr.clear();
			}
		}
	}
	void path_arc(double rx, double ry, double x_axis_rotation, int large_arc_flag, int sweep_flag, double x, double y, bool rel = false)
	{// this function code from rsvg project file rsvg-path.c (Copyright (C) 2000 Eazel, Inc.)
		double f, sinf, cosf, x1, y1, x2, y2, x3, y3, x1_, y1_, cx_, cy_, cx, cy, gamma, theta1, delta_theta, t, th_half, k1, k2, k3, k4, k5, th0, th1;
		int i, n_segs;

		/* Start and end of path segment */
		cairo_get_current_point(_cr_, &x1, &y1);
		x2 = x;
		y2 = y;

		if(x1 == x2 && y1 == y2)
			return;

		/* X-axis */
		f = x_axis_rotation * M_PI / 180.0;
		sinf = sin(f);
		cosf = cos(f);
		if ((fabs(rx) < DBL_EPSILON) || (fabs(ry) < DBL_EPSILON))
		{
			if (rel)
				cairo_rel_line_to(_cr_, x, y);
			else
				cairo_line_to(_cr_, x, y);
			return;
		}
		if(rx < 0)rx = -rx;
		if(ry < 0)ry = -ry;
		k1 = (x1 - x2)/2;
		k2 = (y1 - y2)/2;
		x1_ = cosf * k1 + sinf * k2;
		y1_ = -sinf * k1 + cosf * k2;
		gamma = (x1_*x1_)/(rx*rx) + (y1_*y1_)/(ry*ry);
		if (gamma > 1)
		{
			rx *= sqrt(gamma);
			ry *= sqrt(gamma);
		}

		/* Compute the center */
		k1 = rx*rx*y1_*y1_ + ry*ry*x1_*x1_;
		if(k1 == 0)    
			return;
		k1 = sqrt(fabs((rx*rx*ry*ry)/k1 - 1));
		if(sweep_flag == large_arc_flag)
			k1 = -k1;
		cx_ = k1*rx*y1_/ry;
		cy_ = -k1*ry*x1_/rx;
		cx = cosf*cx_ - sinf*cy_ + (x1+x2)/2;
		cy = sinf*cx_ + cosf*cy_ + (y1+y2)/2;

		/* Compute start angle */
		k1 = (x1_ - cx_)/rx;
		k2 = (y1_ - cy_)/ry;
		k3 = (-x1_ - cx_)/rx;
		k4 = (-y1_ - cy_)/ry;
		k5 = sqrt(fabs(k1*k1 + k2*k2));
		if(k5 == 0)
			return;
		k5 = k1/k5;
		if(k5 < -1)
			k5 = -1;
		else if(k5 > 1)
			k5 = 1;
		theta1 = acos(k5);
		if(k2 < 0)
			theta1 = -theta1;

		/* Compute delta_theta */
		k5 = sqrt(fabs((k1*k1 + k2*k2)*(k3*k3 + k4*k4)));
		if(k5 == 0)
			return;
		k5 = (k1*k3 + k2*k4)/k5;
		if(k5 < -1)
			k5 = -1;
		else if(k5 > 1)
			k5 = 1;
		delta_theta = acos(k5);
		if(k1*k4 - k3*k2 < 0)
			delta_theta = -delta_theta;
		if(sweep_flag && delta_theta < 0)
			delta_theta += M_PI*2;
		else if(!sweep_flag && delta_theta > 0)
			delta_theta -= M_PI*2;
	   
		/* Now draw the arc */
		n_segs = ceil (fabs (delta_theta / (M_PI * 0.5 + 0.001)));
		for (i = 0; i < n_segs; i++)
		{
			th0 = theta1 + i * delta_theta / n_segs;
			th1 = theta1 + (i + 1) * delta_theta / n_segs;
			th_half = 0.5 * (th1 - th0);
			t = (8.0 / 3.0) * sin (th_half * 0.5) * sin (th_half * 0.5) / sin (th_half);
			x1 = rx*(cos (th0) - t * sin (th0));
			y1 = ry*(sin (th0) + t * cos (th0));
			x3 = rx*cos (th1);
			y3 = ry*sin (th1);
			x2 = x3 + rx*(t * sin (th1));
			y2 = y3 + ry*(-t * cos (th1));
			if (rel)
				cairo_rel_curve_to(_cr_, cx + cosf*x1 - sinf*y1, cy + sinf*x1 + cosf*y1, cx + cosf*x2 - sinf*y2, cy + sinf*x2 + cosf*y2, cx + cosf*x3 - sinf*y3, cy + sinf*x3 + cosf*y3);
			else
				cairo_curve_to(_cr_, cx + cosf*x1 - sinf*y1, cy + sinf*x1 + cosf*y1, cx + cosf*x2 - sinf*y2, cy + sinf*x2 + cosf*y2, cx + cosf*x3 - sinf*y3, cy + sinf*x3 + cosf*y3);
		}
	}
	bool parse_path(const wchar_t* d)
	{
		bool result = false;
		wchar_t cmd, prev_cmd = L'0';
		array<stringw> cmds, arg;
		double x, y, current_point_x, current_point_y, reflection_point_x, reflection_point_y;
		stringw(d).trim().split(cmds, L"MmZzLlHhVvCcSsQqTtAaFfPp", 24, true, true);
		u32 count = cmds.size();
		cairo_new_path(_cr_);
		//cairo_new_sub_path(_cr_);
		for (u32 i = 0; i < count; i++)
		{
			//wprintf(L"=== %s\n", cmds[i].c_str());
			cmd = cmds[i].c_str()[0];
			switch(cmd)
			{
				case L'M': case L'm':
					cmds[i].trim("Mm ").split(arg, L"-, ", 3, true, true);
					if (arg.size() > 1)
					{
						for (u32 a = 0; a < arg.size()/2; a++)
						{
							if (cmd == L'm')
								cairo_rel_move_to(_cr_, _wtof(arg[a*2].trim(", ").c_str()), _wtof(arg[a*2+1].trim(", ").c_str()));
							else
								cairo_move_to(_cr_, _wtof(arg[a*2].trim(", ").c_str()), _wtof(arg[a*2+1].trim(", ").c_str()));
						}
					}
					cairo_get_current_point(_cr_, &reflection_point_x, &reflection_point_y);
					break;

				case L'L': case L'l':
					cmds[i].trim("Ll ").split(arg, L"-, ", 3, true, true);
					if (arg.size() > 1)
					{
						for (u32 a = 0; a < arg.size()/2; a++)
						{
							if (cmd == L'l')
								cairo_rel_line_to(_cr_, _wtof(arg[a*2].trim(", ").c_str()), _wtof(arg[a*2+1].trim(", ").c_str()));
							else
								cairo_line_to(_cr_, _wtof(arg[a*2].trim(", ").c_str()), _wtof(arg[a*2+1].trim(", ").c_str()));
						}
					}
					cairo_get_current_point(_cr_, &reflection_point_x, &reflection_point_y);
					break;

				case L'V': case L'v':
					cairo_get_current_point(_cr_, &reflection_point_x, &reflection_point_y);
					reflection_point_y = _wtof(cmds[i].trim("Vv ").c_str());
					if (cmd == L'v')
						cairo_rel_line_to(_cr_, reflection_point_x, reflection_point_y);
					else
						cairo_line_to(_cr_, reflection_point_x, reflection_point_y);
					break;

				case L'H': case L'h':
					cairo_get_current_point(_cr_, &reflection_point_x, &reflection_point_y);
					reflection_point_x = _wtof(cmds[i].trim("Hh ").c_str());
					if (cmd == L'h')
						cairo_rel_line_to(_cr_, reflection_point_x, reflection_point_y);
					else
						cairo_line_to(_cr_, reflection_point_x, reflection_point_y);
					break;
	            
				case L'Q': case L'q'://quadratic Bezier curveto
					cmds[i].trim("Qq ").split(arg, L"-, ", 3, true, true);
					if (arg.size() > 3)
					{
						for (u32 a = 0; a < arg.size()/4; a++)
						{
							cairo_get_current_point(_cr_, &current_point_x, &current_point_y);
							reflection_point_x = _wtof(arg[a*4].trim(", ").c_str());
							reflection_point_y = _wtof(arg[a*4+1].trim(", ").c_str());
							x = _wtof(arg[a*4+2].trim(", ").c_str());
							y = _wtof(arg[a*4+3].trim(", ").c_str());
							if (cmd == L'q')
								cairo_rel_curve_to(_cr_, current_point_x + 2.0/3.0 * (reflection_point_x - current_point_x), current_point_y + 2.0/3.0 * (reflection_point_y - current_point_y), x + 2.0/3.0 * (reflection_point_x - x), y + 2.0/3.0 * (reflection_point_y - y), x, y);
							else
								cairo_curve_to(_cr_, current_point_x + 2.0/3.0 * (reflection_point_x - current_point_x), current_point_y + 2.0/3.0 * (reflection_point_y - current_point_y), x + 2.0/3.0 * (reflection_point_x - x), y + 2.0/3.0 * (reflection_point_y - y), x, y);
							//double x1 = (current_point_x + 2 * reflection_point_x) * (1.0 / 3.0);
							//double y1 = (current_point_y + 2 * reflection_point_y) * (1.0 / 3.0);
							//double x2 = (x + 2 * reflection_point_x) * (1.0 / 3.0);
							//double y2 = (y + 2 * reflection_point_y) * (1.0 / 3.0);
							//if (cmd == L'q')
							//	cairo_rel_curve_to(_cr_, x1, y1, x2, y2, x, y);
							//else
							//	cairo_curve_to(_cr_, x1, y1, x2, y2, x, y);
						}
					}
					break;

				case L'T': case L't'://smooth quadratic Bezier curveto
					cmds[i].trim("Tt ").split(arg, L"-, ", 3, true, true);
					if (arg.size() == 2)
					{
						cairo_get_current_point(_cr_, &current_point_x, &current_point_y);
						reflection_point_x = 2 * current_point_x - reflection_point_x;
						reflection_point_x = 2 * current_point_y - reflection_point_y;
						x = (current_point_x + 2 * reflection_point_x) * (1.0 / 3.0);
						y = (current_point_y + 2 * reflection_point_x) * (1.0 / 3.0);
						current_point_x = _wtof(arg[0].trim(", ").c_str());
						current_point_y = _wtof(arg[1].trim(", ").c_str());
						double x2 = (current_point_x + 2 * reflection_point_x) * (1.0 / 3.0);
						double y2 = (current_point_y + 2 * reflection_point_x) * (1.0 / 3.0);
						if (cmd == L't')
							cairo_rel_curve_to(_cr_, x, y, x2, y2, current_point_x, current_point_y);
						else
							cairo_curve_to(_cr_, x, y, x2, y2, current_point_x, current_point_y);
					}
					if (arg.size() > 3)
					{
						double x2, y2;
						for (u32 a = 0; a < arg.size()/4; a++)
						{
							cairo_get_current_point(_cr_, &current_point_x, &current_point_y);
							reflection_point_x = _wtof(arg[a*4].trim(", ").c_str());
							reflection_point_y = _wtof(arg[a*4+1].trim(", ").c_str());
							if (a * 4 - 2 == arg.size())
							{
								if (cmd == L't')
									cairo_rel_line_to(_cr_, reflection_point_x, reflection_point_y);
								else
									cairo_line_to(_cr_, reflection_point_x, reflection_point_y);
							}
							else
							{
								x = (current_point_x + 2 * reflection_point_x) * (1.0 / 3.0);
								y = (current_point_y + 2 * reflection_point_y) * (1.0 / 3.0);
								current_point_x = _wtof(arg[a*2+2].trim(", ").c_str());
								current_point_y = _wtof(arg[a*2+3].trim(", ").c_str());
								x2 = (current_point_x + 2 * reflection_point_x) * (1.0 / 3.0);
								y2 = (current_point_y + 2 * reflection_point_y) * (1.0 / 3.0);
								if (cmd == L't')
									cairo_rel_curve_to(_cr_, x, y, x2, y2, current_point_x, current_point_y);
								else
									cairo_curve_to(_cr_, x, y, x2, y2, current_point_x, current_point_y);
							}
						}
					}
					break;

				case L'C': case L'c'://cubic Bezier curveto
					cmds[i].trim("Cc ").split(arg, L"-, ", 3, true, true);
					if (arg.size() > 5)
					{
						if (cmd == L'c')
						{
							for (u32 a = 0; a < arg.size()/6; a++)
								cairo_rel_curve_to(_cr_, _wtof(arg[a*6].trim(", ").c_str()), _wtof(arg[a*6+1].trim(", ").c_str()), _wtof(arg[a*6+2].trim(", ").c_str()), _wtof(arg[a*6+3].trim(", ").c_str()), _wtof(arg[a*6+4].trim(", ").c_str()), _wtof(arg[a*6+5].trim(", ").c_str()));
						}
						else
						{
							for (u32 a = 0; a < arg.size()/6; a++)
								cairo_curve_to(_cr_, _wtof(arg[a*6].trim(", ").c_str()), _wtof(arg[a*6+1].trim(", ").c_str()), _wtof(arg[a*6+2].trim(", ").c_str()), _wtof(arg[a*6+3].trim(", ").c_str()), _wtof(arg[a*6+4].trim(", ").c_str()), _wtof(arg[a*6+5].trim(", ").c_str()));
						}
						reflection_point_x = _wtof(arg[arg.size()-4].trim(", ").c_str());
						reflection_point_y = _wtof(arg[arg.size()-3].trim(", ").c_str());
					}
					break;

				case L'S': case L's'://smooth cubic Bezier curveto
					cmds[i].trim("Ss ").split(arg, L"-, ", 3, true, true);
					if (arg.size() > 3)
					{
						for (u32 a = 0; a < arg.size()/4; a++)
						{
							cairo_get_current_point(_cr_, &current_point_x, &current_point_y);
							if (prev_cmd == L'C' || prev_cmd == L'c' || prev_cmd == L'S' || prev_cmd == L's')
							{
								current_point_x = current_point_x + (reflection_point_x - current_point_x) * 2.0 / 3.0;
								current_point_y = current_point_y + (reflection_point_y - current_point_y) * 2.0 / 3.0;
								//current_point_x = 2.0 * current_point_x - reflection_point_x;
								//current_point_y = 2.0 * current_point_y - reflection_point_y;
							}
							reflection_point_x = _wtof(arg[a*4].trim(", ").c_str());
							reflection_point_y = _wtof(arg[a*4+1].trim(", ").c_str());
							x = _wtof(arg[a*4+2].trim(", ").c_str());
							y = _wtof(arg[a*4+3].trim(", ").c_str());
							if (cmd == L's')
								cairo_rel_curve_to(_cr_, current_point_x, current_point_y, reflection_point_x, reflection_point_y, x, y);
							else
								cairo_curve_to(_cr_, current_point_x, current_point_y, reflection_point_x, reflection_point_y, x, y);
						}
					}
					break;

				case L'A': case L'a':
					cmds[i].trim("Aa ").split(arg, L"-, ", 3, true, true);
					//bool large_arc_flag, sweep_flag;
					//double x, y, rx, ry, x_axis_rotation;
					//double xc, yc, radius, angle1, angle2;
					if (arg.size() > 6)
					{
						if (cmd == L'a')
						{
							for (u32 a = 0; a < arg.size()/7; a++)
							//{
							//	rx = _wtof(arg[a*7].trim(", ").c_str());
							//	ry = _wtof(arg[a*7+1].trim(", ").c_str());
							//	x_axis_rotation = _wtof(arg[a*7+2].trim(", ").c_str());
							//	large_arc_flag = _wtoi(arg[a*7+3].trim(", ").c_str());
							//	sweep_flag = _wtoi(arg[a*7+4].trim(", ").c_str());
							//	x = _wtof(arg[a*7+5].trim(", ").c_str());
							//	y = _wtof(arg[a*7+6].trim(", ").c_str());
							//	if (sweep_flag)
							//		cairo_arc_negative(_cr_, xc, yc, radius, angle1, angle2);
							//	else
							//		cairo_arc(_cr_, xc, yc, radius, angle1, angle2);
							//}
								path_arc(_wtof(arg[a*7].trim(", ").c_str()), _wtof(arg[a*7+1].trim(", ").c_str()), _wtof(arg[a*7+2].trim(", ").c_str()), _wtoi(arg[a*7+3].trim(", ").c_str()), _wtoi(arg[a*7+4].trim(", ").c_str()), _wtof(arg[a*7+5].trim(", ").c_str()), _wtof(arg[a*7+6].trim(", ").c_str()), true);
						}
						else
						{
							//cairo_new_sub_path(_cr_);
							for (u32 a = 0; a < arg.size()/7; a++)
								path_arc(_wtof(arg[a*7].trim(", ").c_str()), _wtof(arg[a*7+1].trim(", ").c_str()), _wtof(arg[a*7+2].trim(", ").c_str()), _wtoi(arg[a*7+3].trim(", ").c_str()), _wtoi(arg[a*7+4].trim(", ").c_str()), _wtof(arg[a*7+5].trim(", ").c_str()), _wtof(arg[a*7+6].trim(", ").c_str()));
						}
					}
					break;

				case L'Z': case L'z':
					cairo_close_path(_cr_);
					result = true;
					break;

				default:
				{
					wprintf(L"parse_path: Invalid Command %c\n", cmd);
				}
			}
			prev_cmd = cmd;
			if (!arg.empty())
				arg.clear();
		}
		if (!cmds.empty())
			cmds.clear();
		return result;
	}
	void paint()
	{
		//cairo_push_group(_cr_);
		//cairo_save(_cr_);
		if (_cf_.f)
		{
			//printf("===fill %g, %g, %g, %g\n", _cf_.r, _cf_.g, _cf_.b, _cf_.a);
			if (_cf_.a)
			{
				cairo_set_source_rgba(_cr_, _cf_.r, _cf_.g, _cf_.b, _cf_.a);
				_cf_.a = 0.0;
			}
			else
				cairo_set_source_rgb(_cr_, _cf_.r, _cf_.g, _cf_.b);
			if (_cs_.f)
				cairo_fill_preserve(_cr_);
			else
				cairo_fill(_cr_);
		}
		if (_cs_.f)
		{
			//printf("===stroke %g, %g, %g, %g\n", _cs_.r, _cs_.g, _cs_.b, _cs_.a);
			if (_cs_.a)
			{
				cairo_set_source_rgba(_cr_, _cs_.r, _cs_.g, _cs_.b, _cs_.a);
				_cs_.a = 0.0;
			}
			else
				cairo_set_source_rgb(_cr_, _cs_.r, _cs_.g, _cs_.b);
			cairo_set_line_width(_cr_, _cs_.width);
			if (_cs_.miterlimit)
				cairo_set_miter_limit(_cr_, _cs_.miterlimit);
			if (_cs_.set_linecap)
			{
				cairo_set_line_cap(_cr_, _cs_.linecap);
				_cs_.set_linecap = false;
			}
			if (_cs_.set_linejoin)
			{
				cairo_set_line_join(_cr_, _cs_.linejoin);
				_cs_.set_linejoin = false;
			}
			cairo_stroke(_cr_);
			//cairo_stroke_preserve(_cr_);
		}
		//cairo_restore(_cr_);
		//cairo_pop_group_to_source(_cr_);
		_cf_.f = false;
		_cs_.f = false;
	}
	void scale(double scale_x = 1.0, double scale_y = 1.0)
	{
		_scale_x_ = scale_x;
		_scale_y_ = scale_y;
		if (_scale_x_ != 0.0 || _scale_y_ != 0.0)
		{
			cairo_format_t cairo_format_color = color_format_to_cairo(_color_format_);
			cairo_surface_t* surface = cairo_image_surface_create_for_data(cairo_image_surface_get_data(_surface_), cairo_format_color, _width_, _height_, cairo_format_stride_for_width(cairo_format_color, (int)_width_));
			cairo_scale(_cr_, _scale_x_, _scale_y_);
			cairo_set_source_surface(_cr_, surface, 1 , 1);
			cairo_pattern_set_filter(cairo_get_source(_cr_), CAIRO_FILTER_NEAREST);
			cairo_paint(_cr_);
			cairo_surface_finish(surface);
			cairo_surface_destroy(surface);
		}
	}
	IImage* get_image()
	{
		IImage* image = 0;
		u32 width = (u32)(_width_ * _scale_x_);
		if (_width_ > (double)width)
			width++;
		u32 height = (u32)(_height_ * _scale_y_);
		if (_height_ > (double)height)
			height++;
		if (_color_format_ == ECF_R8G8B8)
		{
			unsigned char* data = cairo_image_surface_get_data(_surface_);
			u32 ci = 0;
			image = _video_driver_->createImage(_color_format_, dimension2d<u32>(width, height));
			unsigned char* irr_data = (unsigned char*)image->lock();
			for (u32 i = 0; i < image->getImageDataSizeInBytes(); i += 3)
			{
				irr_data[i] = data[ci+2];//b
				irr_data[i+1] = data[ci+1];//g
				irr_data[i+2] = data[ci];//r
				ci += 4;//cairo rgb format is also 32 bit (like rgba)
			}
			image->unlock();
		}
		else if (_color_format_ == ECF_A8R8G8B8)
			image = _video_driver_->createImageFromData(_color_format_, dimension2d<u32>(width, height), cairo_image_surface_get_data(_surface_));
		return image;
	}
	ITexture* get_texture()
	{
		return _video_driver_->addTexture(_file_name_, get_image());
	}
	~svg_cairo_image()
	{
		_video_driver_ = 0;
		cairo_pattern_destroy(_current_pattern_);
		cairo_destroy(_cr_);
		cairo_surface_destroy(_surface_);
	}

private:
	bool _use_alpha_;
	double _alpha_value_, _width_, _height_, _scale_x_, _scale_y_;
	container_fill _cf_;
	container_stroke _cs_;
	array<container_style> _css_style_;
	video::ECOLOR_FORMAT _color_format_;
	irr::io::path _file_name_;
	IVideoDriver* _video_driver_;
	cairo_pattern_t* _current_pattern_;
	cairo_surface_t* _surface_;
	cairo_t* _cr_;//the cairo drawing context
};





#ifdef __cplusplus
extern "C" {
#endif

IRRLICHT_C_API svg_cairo_image* svg_cairo_image_ctor1(IVideoDriver* video_driver, IFileSystem* fs, const fschar_t* file_name = "tiger.svg", bool content_unicode = true, double alpha_value = 0.0, video::ECOLOR_FORMAT color_format = ECF_A8R8G8B8, cairo_antialias_t antialias_type = CAIRO_ANTIALIAS_DEFAULT, double scale_x = 1.0, double scale_y = 1.0)
{return new svg_cairo_image(video_driver, fs, irr::io::path(file_name), content_unicode, alpha_value, color_format, antialias_type, scale_x, scale_y);}
IRRLICHT_C_API void svg_cairo_image_scale(svg_cairo_image* pointer, double scale_x = 1.0, double scale_y = 1.0)
{pointer->scale(scale_x, scale_y);}
IRRLICHT_C_API IImage* svg_cairo_image_get_image(svg_cairo_image* pointer)
{return pointer->get_image();}
IRRLICHT_C_API ITexture* svg_cairo_image_get_texture(svg_cairo_image* pointer)
{return pointer->get_texture();}


IRRLICHT_C_API int tool_cairo_version(){return cairo_version();}
IRRLICHT_C_API const char* tool_cairo_version_string(){return cairo_version_string();}

#ifdef __cplusplus
}
#endif
