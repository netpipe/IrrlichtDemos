// Copyright(c) Max Kolosov 2011 maxkolosov@inbox.ru
// http://vosolok2008.narod.ru
// BSD license

#define _CRT_SECURE_NO_WARNINGS

#include "agg_scanline_p.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"
#include "..\..\agg-2.5\examples\svg_viewer\agg_svg_parser.h"
#include "named_colors_agg.h"

using namespace agg;

inline int __cdecl cmp_color_agg(const void* p1, const void* p2)
{
	return wcscmp(((agg_named_color*)p1)->name, ((agg_named_color*)p2)->name);
}

class svg_agg_image
{
public:
	svg_agg_image(IVideoDriver* video_driver, IFileSystem* fs, const irr::io::path& file_name = "tiger.svg", bool content_unicode = true, u32 alpha_value = 128, video::ECOLOR_FORMAT color_format = ECF_A8R8G8B8, int stride = 4)
	{
		_scale_ = 1.0;
		_alpha_value_ = alpha_value;
		_file_name_ = file_name;
		_color_format_ = color_format;
		_stride_ = stride;
		_video_driver_ = video_driver;
		IXMLReader* xml_reader;
		if (content_unicode)
			xml_reader = fs->createXMLReader(_file_name_);
		else
			xml_reader = dynamic_cast<IXMLReader*>(fs->createXMLReaderUTF8(file_name));
		if (xml_reader)
		{
			//bool flag_start_linear_gradient = false;
			u32 gradients_count = 0;
			unsigned int attr_count = 0;
			core::array<core::stringw> list_attr;
			//core::array<gradient> gradients;
			//_path_renderer_.remove_all();
			while (xml_reader->read())
			{
				switch (xml_reader->getNodeType())
				{
				case io::EXN_ELEMENT:
					{
					const wchar_t* node_name = xml_reader->getNodeName();
					attr_count = xml_reader->getAttributeCount();
					if (attr_count)
					{
						if (_wcsnicmp(node_name, L"svg", 3) == 0)//Compare characters of two strings without regard to case
						{
							wchar_t symbol_percent = L'%';
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
									_width_ = (double)xml_reader->getAttributeValueAsFloat(L"width");
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
									_height_ = (double)xml_reader->getAttributeValueAsFloat(L"height");
							}
							list_attr.clear();
						}
						//else if (_wcsnicmp(node_name, L"linearGradient", 14) == 0)
						//{
						//	gradient gr(xml_reader->getAttributeValue(L"id"), xml_reader->getAttributeValueSafe(L"xlink:href"));
						//	flag_start_linear_gradient = true;
						//	gradients_count++;
						//	gradients.reallocate(gradients_count);
						//	gradients.insert(gr);
						//}
						else if (wcslen(node_name) == 1)
						{
							if (node_name[0] == L'g')
							{
								_path_renderer_.push_attr();
								parse_style(xml_reader->getAttributeValue(L"style"));
								parse_transform(xml_reader->getAttributeValue(L"transform"));
							}
						}
						else if (_wcsnicmp(node_name, L"path", 4) == 0)
						{
							const wchar_t* d = xml_reader->getAttributeValue(L"d");
							if (d)
							{
								_path_renderer_.begin_path();
								parse_style(xml_reader->getAttributeValue(L"style"));
								parse_attributes(xml_reader);
								parse_transform(xml_reader->getAttributeValue(L"transform"));
								parse_path(d);
								_path_renderer_.end_path();
							}
							//if (d)
							//{
							//	size_t converted_chars;
							//	size_t len_string = wcslen(d) + 1;
							//	char* attr_value = (char*)calloc(len_string, sizeof(char));
							//	errno_t err = wcstombs_s(&converted_chars, attr_value, len_string, d, _TRUNCATE);
							//	if (attr_value && err == 0)
							//	{
							//		_path_renderer_.begin_path();
							//		parse_style(xml_reader->getAttributeValue(L"style"));
							//		parse_transform(xml_reader->getAttributeValue(L"transform"));
							//		agg::svg::path_tokenizer tokenizer;
							//		tokenizer.set_path_str(attr_value);
							//		_path_renderer_.parse_path(tokenizer);
							//		_path_renderer_.end_path();
							//	} 
							//	free(attr_value);
							//}
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
								_path_renderer_.begin_path();
								parse_style(xml_reader->getAttributeValue(L"style"));
								parse_attributes(xml_reader);
								parse_transform(xml_reader->getAttributeValue(L"transform"));
								_path_renderer_.move_to(x, y);
								_path_renderer_.line_to(x + w, y);
								_path_renderer_.line_to(x + w, y + h);
								_path_renderer_.line_to(x, y + h);
								_path_renderer_.close_subpath();
								_path_renderer_.end_path();
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
								_path_renderer_.begin_path();
								parse_style(xml_reader->getAttributeValue(L"style"));
								parse_attributes(xml_reader);
								parse_transform(xml_reader->getAttributeValue(L"transform"));
								_path_renderer_.move_to(x1, y1);
								_path_renderer_.line_to(x2, y2);
								_path_renderer_.end_path();
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
									_path_renderer_.begin_path();
									parse_style(xml_reader->getAttributeValue(L"style"));
									parse_attributes(xml_reader);
									parse_transform(xml_reader->getAttributeValue(L"transform"));
									_path_renderer_.move_to(_wtof(list_attr[0].c_str()), _wtof(list_attr[1].c_str()));
									for (int i = 2; i < points_count; i += 2)
										_path_renderer_.line_to(_wtof(list_attr[i].c_str()), _wtof(list_attr[i+1].c_str()));
									_path_renderer_.end_path();
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
									_path_renderer_.begin_path();
									parse_style(xml_reader->getAttributeValue(L"style"));
									parse_attributes(xml_reader);
									parse_transform(xml_reader->getAttributeValue(L"transform"));
									_path_renderer_.move_to(_wtof(list_attr[0].c_str()), _wtof(list_attr[1].c_str()));
									for (int i = 2; i < points_count; i += 2)
										_path_renderer_.line_to(_wtof(list_attr[i].c_str()), _wtof(list_attr[i+1].c_str()));
									_path_renderer_.close_subpath();
									_path_renderer_.end_path();
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
							_path_renderer_.pop_attr();
					}
					//else if (_wcsnicmp(node_name, L"linearGradient", 14) == 0)
					//	flag_start_linear_gradient = false;
					break;
					}
				case io::EXN_TEXT:
					{
					//wprintf(L"--- EXN_TEXT id node = %s\n", xml_reader->getAttributeValue(L"id"));
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
					printf("--- UNREGISTER NODE TYPE\n");
					break;
					}
				}
			}
			//for (int i = 0; i < gradients.size(); i++)
			//	wprintf(L"=== gradient id = %s, xlink:href = %s\n", gradients[i].id(), gradients[i].xlink_href());
			//_path_renderer_.arrange_orientations();
			_path_renderer_.bounding_rect(&_min_x_, &_min_y_, &_width_, &_height_);
		}
		else
			printf("!!! ERROR create svg_agg_image from file = %s\n", _file_name_.c_str());
	}
	agg::rgba8 parse_color_name(const wchar_t* value)
	{
		agg_named_color c;
#ifdef _MSC_VER
		wcscpy_s(c.name, sizeof(c.name), value);
#else
		wcscpy(c.name, value);
#endif
		const void* p = bsearch(&c, agg_named_colors, sizeof(agg_named_colors) / sizeof(agg_named_colors[0]), sizeof(agg_named_colors[0]), cmp_color_agg);
		if(p)
		{
			const agg_named_color* pc = (const agg_named_color*)p;
			return agg::rgba8(pc->r, pc->g, pc->b, pc->a);
		}
		else
			return agg::rgba8(255, 255, 255, 255);
	}
	agg::rgba8 parse_color(const wchar_t* value)
	{
		wchar_t *endptr = 0;
		if(wcslen(value) > 2)
			return agg::rgb8_packed(static_cast<unsigned int>(wcstoul(value, &endptr, 16)));
		//{
		//	unsigned int v = static_cast<unsigned int>(wcstoul(value, &endptr, 16));
		//	return agg::rgba8((v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF, _alpha_value_);
		//}
		else
			return agg::rgba8(255, 255, 255, 255);
	}
	agg::rgba8 _parse_color(const wchar_t* value)
	{
		unsigned c = 0;
		if(wcslen(value))
		{
			swscanf_s(value, L"%x", &c);
			return agg::rgb8_packed(c);
		}
		else
			return agg::rgba8(255, 255, 255, 255);
	}
	void parse_fill(core::stringw value)
	{
		if (value.equalsn(L"none", 4))
			_path_renderer_.fill_none();
		else if (value.equalsn(L"#", 1))
		{
			value[0] = L'x';
			_path_renderer_.fill(parse_color((core::stringw(L"0") + value).c_str()));
		}
		else if (value.equalsn(L":#", 2))
		{
			value[0] = L'0';
			value[1] = L'x';
			_path_renderer_.fill(parse_color(value.c_str()));
		}
		else
			_path_renderer_.fill(parse_color_name(value.trim().c_str()));
	}
	void parse_fill_rule(const wchar_t* value)
	{
		if (_wcsnicmp(value, L"evenodd", 7) == 0)
			_path_renderer_.even_odd(true);
		else
			_path_renderer_.even_odd(false);
	}
	void parse_stroke(core::stringw value)
	{
		if (value.equalsn(L"none", 4))
			_path_renderer_.stroke_none();
		else if (value.equalsn(L"#", 1))
		{
			value[0] = L'x';
			_path_renderer_.stroke(parse_color((core::stringw(L"0") + value).c_str()));
		}
		else if (value.equalsn(L":#", 2))
		{
			value[0] = L'0';
			value[1] = L'x';
			_path_renderer_.stroke(parse_color(value.c_str()));
		}
		else
			_path_renderer_.stroke(parse_color_name(value.trim().c_str()));
	}
	void parse_linecap(core::stringw value)
	{
		if (value.equalsn(L"butt", 4))
			_path_renderer_.line_cap(butt_cap);
		else if (value.equalsn(L"square", 6))
			_path_renderer_.line_cap(square_cap);
		else if (value.equalsn(L"round", 5))
			_path_renderer_.line_cap(round_cap);
	}
	void parse_linejoin(core::stringw value)
	{
		if (value.equalsn(L"miter", 5))
			_path_renderer_.line_join(miter_join);
		else if (value.equalsn(L"round", 5))
			_path_renderer_.line_join(round_join);
		else if (value.equalsn(L"bevel", 5))
			_path_renderer_.line_join(bevel_join);
	}
	void parse_attributes(IXMLReader* xml_reader)
	{
		const wchar_t* fill = xml_reader->getAttributeValue(L"fill");
		if (fill)
			parse_fill(core::stringw(fill));
		const wchar_t* fill_rule = xml_reader->getAttributeValue(L"fill-rule");
		if (fill_rule)
			parse_fill_rule(fill_rule);
		double fill_opacity = xml_reader->getAttributeValueAsFloat(L"fill-opacity");
		if (fill_opacity)
			_path_renderer_.fill_opacity(fill_opacity);
		const wchar_t* stroke = xml_reader->getAttributeValue(L"stroke");
		if (stroke)
			parse_stroke(core::stringw(stroke));
		double stroke_opacity = xml_reader->getAttributeValueAsFloat(L"stroke-opacity");
		if (stroke_opacity)
			_path_renderer_.stroke_opacity(stroke_opacity);
		double stroke_width = xml_reader->getAttributeValueAsFloat(L"stroke-width");
		if (stroke_width)
			_path_renderer_.stroke_width(stroke_width);
		double stroke_miterlimit = xml_reader->getAttributeValueAsFloat(L"stroke-miterlimit");
		if (stroke_miterlimit)
			_path_renderer_.miter_limit(stroke_miterlimit);
		const wchar_t* linecap = xml_reader->getAttributeValue(L"stroke-linecap");
		if (linecap)
			parse_linecap(stringw(linecap));
		const wchar_t* linejoin = xml_reader->getAttributeValue(L"stroke-linejoin");
		if (linejoin)
			parse_linejoin(stringw(linejoin));
	}
	void parse_style(const wchar_t* value)
	{
		core::array<core::stringw> list_attr;
		core::stringw(value).trim().split(list_attr, L";");
		for (int i = 0; i < list_attr.size(); i++)
		{
			if (list_attr[i].equalsn(L"fill:", 5))
			{
				if (list_attr[i].equalsn(L"fill:#", 6))
					parse_fill(list_attr[i].trim("fill"));
				else
					parse_fill(list_attr[i].trim("fill:"));
			}
			else if (list_attr[i].equalsn(L"fill-opacity:", 13))
				_path_renderer_.fill_opacity(_wtof(list_attr[i].trim("fill-opacity:").c_str()));
			else if (list_attr[i].equalsn(L"fill-rule:", 10))
				parse_fill_rule(list_attr[i].trim(L"fill-rule:").c_str());
			else if (list_attr[i].equalsn(L"stroke:", 7))
			{
				if (list_attr[i].equalsn(L"stroke:#", 8))
					parse_stroke(list_attr[i].trim("stroke"));
				else
					parse_stroke(list_attr[i].trim("stroke:"));
			}
			else if (list_attr[i].equalsn(L"stroke-width:", 13))
				_path_renderer_.stroke_width(_wtof(list_attr[i].trim("stroke-width:").c_str()));
			else if (list_attr[i].equalsn(L"stroke-linecap:", 15))
				parse_linecap(list_attr[i].trim("stroke-linecap:"));
			else if (list_attr[i].equalsn(L"stroke-linejoin:", 16))
				parse_linejoin(list_attr[i].trim("stroke-linejoin:"));
			else if (list_attr[i].equalsn(L"stroke-miterlimit:", 18))
				_path_renderer_.miter_limit(_wtof(list_attr[i].trim("stroke-miterlimit:").c_str()));
			else if (list_attr[i].equalsn(L"stroke-opacity:", 15))
				_path_renderer_.stroke_opacity(_wtof(list_attr[i].trim("stroke-opacity:").c_str()));
		}
		list_attr.clear();
	}
	void parse_transform(const wchar_t* value)
	{
		core::array<core::stringw> list_attr;
		core::stringw(value).trim(core::stringw(")")).split(list_attr, L"(");
		if (list_attr.size())
		{
			if (list_attr[0].equals_substring_ignore_case(stringw("matrix")))
			{
				core::array<double> args = string_split_d(list_attr[1].c_str(), 6);
				_path_renderer_.transform().premultiply(trans_affine(args[0], args[1], args[2], args[3], args[4], args[5]));
			}
			else if (list_attr[0].equals_substring_ignore_case(stringw("translate")))
			{
				core::array<double> args = string_split_d(list_attr[1].c_str(), 2);
				_path_renderer_.transform().premultiply(trans_affine_translation(args[0], args[1]));
			}
			else if (_wcsnicmp(list_attr[0].c_str(), L"rotate", 6) == 0)
			{
				core::array<double> args = string_split_d(list_attr[1].c_str(), 3);
				if (args[1] == 0.0 && args[2] == 0.0)
					_path_renderer_.transform().premultiply(trans_affine_rotation(deg2rad(args[0])));
				else
				{
					trans_affine t = trans_affine_translation(-args[1], -args[2]);
					t *= trans_affine_rotation(deg2rad(args[0]));
					t *= trans_affine_translation(args[1], args[2]);
					_path_renderer_.transform().premultiply(t);
				}
			}
			else if (_wcsnicmp(list_attr[0].c_str(), L"scale", 5) == 0)
			{
				core::array<double> args = string_split_d(list_attr[1].c_str(), 2);
				if (args[1] == 0.0)
					args[1] = args[0];
				_path_renderer_.transform().premultiply(trans_affine_scaling(args[0], args[1]));
			}
			else if (_wcsnicmp(list_attr[0].c_str(), L"skewX", 5) == 0)
				_path_renderer_.transform().premultiply(trans_affine_skewing(deg2rad(_wtof(list_attr[1].c_str())), 0.0));
			else if (_wcsnicmp(list_attr[0].c_str(), L"skewY", 5) == 0)
				_path_renderer_.transform().premultiply(trans_affine_skewing(0.0, deg2rad(_wtof(list_attr[1].c_str()))));
			list_attr.clear();
		}
	}
	void parse_path(const wchar_t* d)
	{
		if (!d)
			return;
		array<stringw> cmds, arg;
		stringw(d).trim().split(cmds, L"MmZzLlHhVvCcSsQqTtAaFfPp", 24, true, true);
		u32 count = cmds.size();
		for (u32 i = 0; i < count; i++)
		{
			const wchar_t cmd = cmds[i].c_str()[0];
			switch(cmd)
			{
				case L'M': case L'm':
					cmds[i].trim("Mm ").split(arg, L"-, ", 3, true, true);
					for (u32 a = 0; a < arg.size()/2; a++)
						_path_renderer_.move_to(_wtof(arg[a*2].trim(", ").c_str()), _wtof(arg[a*2+1].trim(", ").c_str()), cmd == L'm');
					//wprintf(L"=== cmd = %c, %d", cmd, cmd == L'm');
					//for (u32 a = 0; a < arg.size(); a++)
					//	printf(", %g", _wtof(arg[a].trim(", ").c_str()));
					//printf("\n");
					arg.clear();
					break;

				case L'L': case L'l':
					cmds[i].trim("Ll ").split(arg, L"-, ", 3, true, true);
					for (u32 a = 0; a < arg.size()/2; a++)
						_path_renderer_.line_to(_wtof(arg[a*2].trim(", ").c_str()), _wtof(arg[a*2+1].trim(", ").c_str()), cmd == L'l');
					arg.clear();
					break;

				case L'V': case L'v':
					_path_renderer_.vline_to(_wtof(cmds[i].trim("Vv ").c_str()), cmd == L'v');
					break;

				case L'H': case L'h':
					_path_renderer_.hline_to(_wtof(cmds[i].trim("Hh ").c_str()), cmd == L'h');
					break;
	            
				case L'Q': case L'q':
					cmds[i].trim("Qq ").split(arg, L"-, ", 3, true, true);
					for (u32 a = 0; a < arg.size()/4; a++)
						_path_renderer_.curve3(_wtof(arg[a*4].trim(", ").c_str()), _wtof(arg[a*4+1].trim(", ").c_str()), _wtof(arg[a*4+2].trim(", ").c_str()), _wtof(arg[a*4+3].trim(", ").c_str()), cmd == L'q');
					arg.clear();
					break;

				case L'T': case L't':
					cmds[i].trim("Tt ").split(arg, L"-, ", 3, true, true);
					for (u32 a = 0; a < arg.size()/2; a++)
						_path_renderer_.curve3(_wtof(arg[a*2].trim(", ").c_str()), _wtof(arg[a*2+1].trim(", ").c_str()), cmd == L't');
					arg.clear();
					break;

				case L'C': case L'c':
					cmds[i].trim("Cc ").split(arg, L"-, ", 3, true, true);
					for (u32 a = 0; a < arg.size()/6; a++)
						_path_renderer_.curve4(_wtof(arg[a*6].trim(", ").c_str()), _wtof(arg[a*6+1].trim(", ").c_str()), _wtof(arg[a*6+2].trim(", ").c_str()), _wtof(arg[a*6+3].trim(", ").c_str()), _wtof(arg[a*6+4].trim(", ").c_str()), _wtof(arg[a*6+5].trim(", ").c_str()), cmd == L'c');
					arg.clear();
					break;

				case L'S': case L's':
					cmds[i].trim("Ss ").split(arg, L"-, ", 3, true, true);
					for (u32 a = 0; a < arg.size()/4; a++)
						_path_renderer_.curve4(_wtof(arg[a*4].trim(", ").c_str()), _wtof(arg[a*4+1].trim(", ").c_str()), _wtof(arg[a*4+2].trim(", ").c_str()), _wtof(arg[a*4+3].trim(", ").c_str()), cmd == L's');
					arg.clear();
					break;

				case L'A': case L'a':
					//thanks for this code Dov Grobgeld (dov.grobgeldr@gmail.com  http://github.com/dov/agg-svg)
					cmds[i].trim("Aa ").split(arg, L"-, ", 3, true, true);
					for (u32 a = 0; a < arg.size()/7; a++)
						_path_renderer_.arc_to(_wtof(arg[a*7].trim(", ").c_str()), _wtof(arg[a*7+1].trim(", ").c_str()), _wtof(arg[a*7+2].trim(", ").c_str()), bool(_wtoi(arg[a*7+3].trim(", ").c_str())>0), bool(_wtoi(arg[a*7+4].trim(", ").c_str())>0), _wtof(arg[a*7+5].trim(", ").c_str()), _wtof(arg[a*7+6].trim(", ").c_str()), cmd == L'a');
					arg.clear();
					break;

				case L'Z': case L'z':
					_path_renderer_.close_subpath();
					break;

				default:
				{
					wprintf(L"parse_path: Invalid Command %c\n", cmd);
				}
			}
		}
		cmds.clear();
	}
	void scale(double scale_value = 1.0)
	{
		_scale_ = scale_value;
		if (_scale_ < 0.0)
			_scale_ = 1.0;
		_width_ *= _scale_;
		_height_ *= _scale_;
		if (_scale_ > 0.0)
			_trans_affine_ *= agg::trans_affine_scaling(_scale_);
	}
	IImage* render_old()
	{
		u32 w = (u32)_width_;
		if (_width_ > (double)w)
			w++;
		u32 h = (u32)_height_;
		if (_height_ > (double)h)
			h++;
		const dimension2d<u32>& image_size = dimension2d<u32>(w, h);
		IImage* image = _video_driver_->createImage(_color_format_, image_size);
		row_accessor<int8u> rbuf((int8u*)image->lock(), (unsigned)image_size.Width, (unsigned)image_size.Height, (int)image_size.Width * _stride_);

		pixfmt_alpha_blend_rgba<blender_bgra32, row_accessor<int8u>, pixel32_type> pixf(rbuf);
		agg::renderer_base< pixfmt_alpha_blend_rgba<blender_bgra32, row_accessor<int8u>, pixel32_type> > renb(pixf);
		agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt_alpha_blend_rgba<blender_bgra32, row_accessor<int8u>, pixel32_type> > > ren(renb);

		renb.clear(rgba8(255, 255, 255, _alpha_value_));
		agg::rasterizer_scanline_aa<> ras;
		agg::scanline32_p8 sl;
		agg::render_scanlines(ras, sl, ren);
		_path_renderer_.render(ras, sl, ren, _trans_affine_, renb.clip_box(), _alpha_value_/255.0);
		image->unlock();
		return image;
	}
	IImage* render()
	{
		u32 w = (u32)_width_;
		if (_width_ > (double)w)
			w++;
		u32 h = (u32)_height_;
		if (_height_ > (double)h)
			h++;
		unsigned char* data = new unsigned char[w*h*_stride_];
		row_accessor<int8u> rbuf(data, (unsigned)w, (unsigned)h, (int)w * _stride_);

		pixfmt_alpha_blend_rgba<blender_rgba32, row_accessor<int8u>, pixel32_type> pixf(rbuf);
		agg::renderer_base< pixfmt_alpha_blend_rgba<blender_rgba32, row_accessor<int8u>, pixel32_type> > renb(pixf);
		agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt_alpha_blend_rgba<blender_rgba32, row_accessor<int8u>, pixel32_type> > > ren(renb);

		renb.clear(rgba8(255, 255, 255, _alpha_value_));
		agg::rasterizer_scanline_aa<> ras;
		agg::scanline32_p8 sl;
		//agg::render_scanlines(ras, sl, ren);
		_path_renderer_.render(ras, sl, ren, _trans_affine_, renb.clip_box(), _alpha_value_/255.0);
		//_path_renderer_.render(ras, sl, ren, _trans_affine_, renb.clip_box(), 1.0);
		return create_image_from_data(w, h, data);
	}
	ITexture* get_texture()
	{
		return _video_driver_->addTexture(_file_name_, render());
	}
	~svg_agg_image()
	{
		_video_driver_ = 0;
	}
	IImage* create_image_from_data(u32 width, u32 height, unsigned char* data)
	{
		IImage* _image_;
		if (_color_format_ == ECF_R8G8B8)
		{
			u32 ai = 0;
			_image_ = _video_driver_->createImage(_color_format_, dimension2d<u32>(width, height));
			unsigned char* irr_data = (unsigned char*)_image_->lock();
			for (u32 i = 0; i < _image_->getImageDataSizeInBytes(); i += 3)
			{
				irr_data[i] = data[ai];//r
				irr_data[i+1] = data[ai+1];//g
				irr_data[i+2] = data[ai+2];//b
				ai += 4;//agg rgba format is 32 bit
			}
			_image_->unlock();
		}
		else if (_color_format_ == ECF_A8R8G8B8)
			//_image_ = _video_driver_->createImageFromData(_color_format_, dimension2d<u32>(width, height), data);
		{
			_image_ = _video_driver_->createImage(_color_format_, dimension2d<u32>(width, height));
			unsigned char* irr_data = (unsigned char*)_image_->lock();
			for (u32 i = 0; i < _image_->getImageDataSizeInBytes(); i += 4)
			{
				irr_data[i] = data[i+2];//b
				irr_data[i+1] = data[i+1];//g
				irr_data[i+2] = data[i];//r
				irr_data[i+3] = data[i+3];//a
				//irr_data[i+3] = _alpha_value_;//a
			}
			_image_->unlock();
		}
		delete data;
		return _image_;
	}

private:
	int _stride_;
	u32 _alpha_value_;
	double _min_x_;
	double _min_y_;
	double _width_;
	double _height_;
	double _scale_;
	video::ECOLOR_FORMAT _color_format_;
	agg::trans_affine _trans_affine_;
	agg::svg::path_renderer _path_renderer_;
	irr::io::path _file_name_;
	IVideoDriver* _video_driver_;
};



#ifdef __cplusplus
extern "C" {
#endif

IRRLICHT_C_API svg_agg_image* svg_agg_image_ctor1(IVideoDriver* video_driver, IFileSystem* fs, const fschar_t* file_name = "tiger.svg", bool content_unicode = true, u32 alpha_value = 128, video::ECOLOR_FORMAT color_format = ECF_A8R8G8B8, int stride = 4)
{return new svg_agg_image(video_driver, fs, irr::io::path(file_name), content_unicode, alpha_value, color_format, stride);}
IRRLICHT_C_API void svg_agg_image_scale(svg_agg_image* pointer, double scale_value = 1.0)
{pointer->scale(scale_value);}
IRRLICHT_C_API IImage* svg_agg_image_render(svg_agg_image* pointer)
{return pointer->render();}
IRRLICHT_C_API ITexture* svg_agg_image_get_texture(svg_agg_image* pointer)
{return pointer->get_texture();}

#ifdef __cplusplus
}
#endif
