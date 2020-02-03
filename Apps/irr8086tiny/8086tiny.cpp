// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// This work is licensed under the MIT License. See included LICENSE.TXT.
#define IRRLICHT

#ifdef IRRLICHT
#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice * device;
   IVideoDriver *driver;
   ISceneManager* smgr ;

IGUIEnvironment* env;
IGUIStaticText* joy_error;
#endif

#include <time.h>
#include <sys/timeb.h>
#include <memory.h>

#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef USE_TMT
#include "tmt.h"
#endif

#ifndef NO_GRAPHICS
#include "SDL.h"
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


////http://irrlicht.sourceforge.net/forum//viewtopic.php?f=1&t=30750&p=174159&hilit=sdl#p174159


//
//SDL_Joystick *joystick1 = NULL;


#ifdef IRRLICHT
struct SAppContext
{
	IrrlichtDevice *device;
	s32				counter;
	IGUIListBox*	listbox;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR
};


void setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin)
{
	for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
	{
		video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(alpha);
		skin->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
}


class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext & context) : Context(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

			/*
			If a scrollbar changed its scroll position, and it is
			'our' scrollbar (the one with id GUI_ID_TRANSPARENCY_SCROLL_BAR),
			then we change the transparency of all gui elements. This is an
			easy task: There is a skin object, in which all color
			settings are stored. We simply go through all colors
			stored in the skin and change their alpha value.
			*/
			case EGET_SCROLL_BAR_CHANGED:
				if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
				{
					s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					setSkinTransparency(pos, env->getSkin());
				}
				break;

			/*
			If a button was clicked, it could be one of 'our'
			three buttons. If it is the first, we shut down the engine.
			If it is the second, we create a little window with some
			text on it. We also add a string to the list box to log
			what happened. And if it is the third button, we create
			a file open dialog, and add also this as string to the list box.
			That's all for the event receiver.
			*/
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:
					Context.device->closeDevice();
					return true;

				case GUI_ID_NEW_WINDOW_BUTTON:
					{
					Context.listbox->addItem(L"Window created");
					Context.counter += 30;
					if (Context.counter > 200)
						Context.counter = 0;

					IGUIWindow* window = env->addWindow(
						rect<s32>(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter),
						false, // modal?
						L"Test window");

					env->addStaticText(L"Please close me",
						rect<s32>(35,35,140,50),
						true, // border?
						false, // wordwrap?
						window);
					}
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
					Context.listbox->addItem(L"File open");
					// There are some options for the file open dialog
					// We set the title, make it a modal window, and make sure
					// that the working directory is restored after the dialog
					// is finished.
					env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
					return true;

				default:
					return false;
				}
				break;

			case EGET_FILE_SELECTED:
				{
					// show the event and the selected model filename from the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(L"EGET_FILE_SELECTED");
					Context.listbox->addItem(dialog->getFileName());
				}
				break;

			case EGET_DIRECTORY_SELECTED:
				{
					// show the event and the selected directory name from the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(L"EGET_DIRECTORY_SELECTED");
//					Context.listbox->addItem(dialog->getDirectoryNameW());
				}
				break;

			default:
				break;
			}
		}

		return false;
	}

private:
	SAppContext & Context;
};

#endif


// For using standard library functions for file io
#include <stdio.h>

// Emulator system constants
#define IO_PORT_COUNT 0x10000
#define RAM_SIZE 0x10FFF0
#define REGS_BASE 0xF0000
#define VIDEO_RAM_SIZE 0x10000

// Graphics/timer/keyboard update delays (explained later)
#ifndef GRAPHICS_UPDATE_DELAY
#define GRAPHICS_UPDATE_DELAY 360000
#endif
#define KEYBOARD_TIMER_UPDATE_DELAY 20000

// 16-bit register decodes
#define REG_AX 0
#define REG_CX 1
#define REG_DX 2
#define REG_BX 3
#define REG_SP 4
#define REG_BP 5
#define REG_SI 6
#define REG_DI 7

#define REG_ES 8
#define REG_CS 9
#define REG_SS 10
#define REG_DS 11

#define REG_ZERO 12
#define REG_SCRATCH 13

// 8-bit register decodes
#define REG_AL 0
#define REG_AH 1
#define REG_CL 2
#define REG_CH 3
#define REG_DL 4
#define REG_DH 5
#define REG_BL 6
#define REG_BH 7

// FLAGS register decodes
#define FLAG_CF 40
#define FLAG_PF 41
#define FLAG_AF 42
#define FLAG_ZF 43
#define FLAG_SF 44
#define FLAG_TF 45
#define FLAG_IF 46
#define FLAG_DF 47
#define FLAG_OF 48

// Lookup tables in the BIOS binary
#define TABLE_XLAT_OPCODE 8
#define TABLE_XLAT_SUBFUNCTION 9
#define TABLE_STD_FLAGS 10
#define TABLE_PARITY_FLAG 11
#define TABLE_BASE_INST_SIZE 12
#define TABLE_I_W_SIZE 13
#define TABLE_I_MOD_SIZE 14
#define TABLE_COND_JUMP_DECODE_A 15
#define TABLE_COND_JUMP_DECODE_B 16
#define TABLE_COND_JUMP_DECODE_C 17
#define TABLE_COND_JUMP_DECODE_D 18
#define TABLE_FLAGS_BITFIELDS 19

// Bitfields for TABLE_STD_FLAGS values
#define FLAGS_UPDATE_SZP 1
#define FLAGS_UPDATE_AO_ARITH 2
#define FLAGS_UPDATE_OC_LOGIC 4

// Default values of GRAPHICS_* used to create keyboard listening window in text mode when vterm does not use sdl rendering
#define GRAPHICS_X_DEFAULT 50
#define GRAPHICS_Y_DEFAULT 50

// Emscripten port related
#define EMSCRIPTEN_MAIN_LOOP_FRAMERATE 1000
#ifdef __EMSCRIPTEN__
#define EMSCRIPTEN_INSTRUCTIONS_PER_FRAME 10000
#else
#define EMSCRIPTEN_INSTRUCTIONS_PER_FRAME 1
#endif
// Use hd.img file
//#define EMSCRIPTEN_USE_HD
#define EMSCRIPTEN_BIOS_FILE "bios"
#define EMSCRIPTEN_FD_FILE "fd.img"
//#define EMSCRIPTEN_FD_FILE "ros.img"
//#define EMSCRIPTEN_HD_FILE "hd.img"

// Virtual terminal related
#ifdef USE_TMT
#ifdef VTERM_SMALL_CONSOLE
#define VTERM_BLANK_LINES 1
#define VTERM_LINES 9
#define VTERM_COLS 80
#else
#define VTERM_BLANK_LINES 20
#define VTERM_LINES 45
#define VTERM_COLS 80
#endif
#endif

// SDL_PumpEvents and display refreshing can iether be done in every main loop iteration or can be done after a set number of instructions
#ifdef __EMSCRIPTEN__
#define PUMP_EVENTS_EVERY_FRAME
#define REFRESH_DISPLAY_EVERY_FRAME
#endif


// Helper macros

// Decode mod, r_m and reg fields in instruction
#define DECODE_RM_REG scratch2_uint = 4 * !i_mod, \
					  op_to_addr = rm_addr = i_mod < 3 ? SEGREG(seg_override_en ? seg_override : bios_table_lookup[scratch2_uint + 3][i_rm], bios_table_lookup[scratch2_uint][i_rm], regs16[bios_table_lookup[scratch2_uint + 1][i_rm]] + bios_table_lookup[scratch2_uint + 2][i_rm] * i_data1+) : GET_REG_ADDR(i_rm), \
					  op_from_addr = GET_REG_ADDR(i_reg), \
					  i_d && (scratch_uint = op_from_addr, op_from_addr = rm_addr, op_to_addr = scratch_uint)

// Return memory-mapped register location (offset into mem array) for register #reg_id
#define GET_REG_ADDR(reg_id) (REGS_BASE + (i_w ? 2 * reg_id : 2 * reg_id + reg_id / 4 & 7))

// Returns number of top bit in operand (i.e. 8 for 8-bit operands, 16 for 16-bit operands)
#define TOP_BIT 8*(i_w + 1)

// Opcode execution unit helpers
#define OPCODE ;break; case
#define OPCODE_CHAIN ; case

// [I]MUL/[I]DIV/DAA/DAS/ADC/SBB helpers
#define MUL_MACRO(op_data_type,out_regs) (set_opcode(0x10), \
										  out_regs[i_w + 1] = (op_result = CAST(op_data_type)mem[rm_addr] * (op_data_type)*out_regs) >> 16, \
										  regs16[REG_AX] = op_result, \
										  set_OF(set_CF(op_result - (op_data_type)op_result)))
#define DIV_MACRO(out_data_type,in_data_type,out_regs) (scratch_int = CAST(out_data_type)mem[rm_addr]) && !(scratch2_uint = (in_data_type)(scratch_uint = (out_regs[i_w+1] << 16) + regs16[REG_AX]) / scratch_int, scratch2_uint - (out_data_type)scratch2_uint) ? out_regs[i_w+1] = scratch_uint - scratch_int * (*out_regs = scratch2_uint) : pc_interrupt(0)
#define DAA_DAS(op1,op2,mask,min) set_AF((((scratch2_uint = regs8[REG_AL]) & 0x0F) > 9) || regs8[FLAG_AF]) && (op_result = regs8[REG_AL] op1 6, set_CF(regs8[FLAG_CF] || (regs8[REG_AL] op2 scratch2_uint))), \
								  set_CF((((mask & 1 ? scratch2_uint : regs8[REG_AL]) & mask) > min) || regs8[FLAG_CF]) && (op_result = regs8[REG_AL] op1 0x60)
#define ADC_SBB_MACRO(a) OP(a##= regs8[FLAG_CF] +), \
						 set_CF(regs8[FLAG_CF] && (op_result == op_dest) || (a op_result < a(int)op_dest)), \
						 set_AF_OF_arith()

// Execute arithmetic/logic operations in emulator memory/registers
#define R_M_OP(dest,op,src) (i_w ? op_dest = CAST(unsigned short)dest, op_result = CAST(unsigned short)dest op (op_source = CAST(unsigned short)src) \
								 : (op_dest = dest, op_result = dest op (op_source = CAST(unsigned char)src)))
#define MEM_OP(dest,op,src) R_M_OP(mem[dest],op,mem[src])
#define OP(op) MEM_OP(op_to_addr,op,op_from_addr)

// Increment or decrement a register #reg_id (usually SI or DI), depending on direction flag and operand size (given by i_w)
#define INDEX_INC(reg_id) (regs16[reg_id] -= (2 * regs8[FLAG_DF] - 1)*(i_w + 1))

// Helpers for stack operations
#define R_M_PUSH(a) (i_w = 1, R_M_OP(mem[SEGREG(REG_SS, REG_SP, --)], =, a))
#define R_M_POP(a) (i_w = 1, regs16[REG_SP] += 2, R_M_OP(a, =, mem[SEGREG(REG_SS, REG_SP, -2+)]))

// Convert segment:offset to linear address in emulator memory space
#define SEGREG(reg_seg,reg_ofs,op) 16 * regs16[reg_seg] + (unsigned short)(op regs16[reg_ofs])

// Returns sign bit of an 8-bit or 16-bit operand
#define SIGN_OF(a) (1 & (i_w ? CAST(short)a : a) >> (TOP_BIT - 1))

// Reinterpretation cast
#define CAST(a) *(a*)&

// Keyboard driver for console. This may need changing for UNIX/non-UNIX platforms
#ifdef _WIN32
#define KEYBOARD_DRIVER kbhit() && (mem[0x4A6] = getch(), pc_interrupt(7))
#else
#define KEYBOARD_DRIVER read(0, mem + 0x4A6, 1) && (int8_asap = (mem[0x4A6] == 0x1B), pc_interrupt(7))
#endif

// Global variable definitions
unsigned char mem[RAM_SIZE], io_ports[IO_PORT_COUNT], *opcode_stream, *regs8, i_rm, i_w, i_reg, i_mod, i_mod_size, i_d, i_reg4bit, raw_opcode_id, xlat_opcode_id, extra, rep_mode, seg_override_en, rep_override_en, trap_flag, int8_asap, scratch_uchar, io_hi_lo, *vid_mem_base, spkr_en, bios_table_lookup[20][256];
unsigned short *regs16, reg_ip, seg_override, file_index, wave_counter;
unsigned int op_source, op_dest, rm_addr, op_to_addr, op_from_addr, i_data0, i_data1, i_data2, scratch_uint, scratch2_uint, scratch3_uint, inst_counter, set_flags_type, GRAPHICS_X, GRAPHICS_Y, pixel_colors[16], vmem_ctr;
int op_result, disk_size, scratch_int;
time_t clock_buf;
struct timeb ms_clock;
FILE* disk[3];

#ifndef NO_GRAPHICS
#ifndef NO_AUDIO
SDL_AudioSpec sdl_audio = {44100, AUDIO_U8, 1, 0, 128};
#endif
SDL_Surface *sdl_screen;
int is_display_init;
SDL_Event sdl_event;
SDL_PixelFormat* sdl_fmt;
unsigned short vid_addr_lookup[VIDEO_RAM_SIZE], cga_colors[4] = {0 /* Black */, 0x1F1F /* Cyan */, 0xE3E3 /* Magenta */, 0xFFFF /* White */};
#endif

// Virtual terminal via tmt
#ifdef USE_TMT
TMT* vterm;
int vterm_needs_draw;
#endif

// Should main loop continue
int cont_main_loop;

// Helper functions

// Set carry flag
char set_CF(int new_CF)
{
	return regs8[FLAG_CF] = !!new_CF;
}

// Set auxiliary flag
char set_AF(int new_AF)
{
	return regs8[FLAG_AF] = !!new_AF;
}

// Set overflow flag
char set_OF(int new_OF)
{
	return regs8[FLAG_OF] = !!new_OF;
}

// Set auxiliary and overflow flag after arithmetic operations
char set_AF_OF_arith()
{
	set_AF((op_source ^= op_dest ^ op_result) & 0x10);
	if (op_result == op_dest)
		return set_OF(0);
	else
		return set_OF(1 & (regs8[FLAG_CF] ^ op_source >> (TOP_BIT - 1)));
}

// Assemble and return emulated CPU FLAGS register in scratch_uint
void make_flags()
{
	scratch_uint = 0xF002; // 8086 has reserved and unused flags set to 1
	for (int i = 9; i--;)
		scratch_uint += regs8[FLAG_CF + i] << bios_table_lookup[TABLE_FLAGS_BITFIELDS][i];
}

// Set emulated CPU FLAGS register from regs8[FLAG_xx] values
void set_flags(int new_flags)
{
	for (int i = 9; i--;)
		regs8[FLAG_CF + i] = !!(1 << bios_table_lookup[TABLE_FLAGS_BITFIELDS][i] & new_flags);
}

// Convert raw opcode to translated opcode index. This condenses a large number of different encodings of similar
// instructions into a much smaller number of distinct functions, which we then execute
void set_opcode(unsigned char opcode)
{
	xlat_opcode_id = bios_table_lookup[TABLE_XLAT_OPCODE][raw_opcode_id = opcode];
	extra = bios_table_lookup[TABLE_XLAT_SUBFUNCTION][opcode];
	i_mod_size = bios_table_lookup[TABLE_I_MOD_SIZE][opcode];
	set_flags_type = bios_table_lookup[TABLE_STD_FLAGS][opcode];
}

// Execute INT #interrupt_num on the emulated machine
char pc_interrupt(unsigned char interrupt_num)
{
	set_opcode(0xCD); // Decode like INT

	make_flags();
	R_M_PUSH(scratch_uint);
	R_M_PUSH(regs16[REG_CS]);
	R_M_PUSH(reg_ip);
	MEM_OP(REGS_BASE + 2 * REG_CS, =, 4 * interrupt_num + 2);
	R_M_OP(reg_ip, =, mem[4 * interrupt_num]);

	return regs8[FLAG_TF] = regs8[FLAG_IF] = 0;
}

// AAA and AAS instructions - which_operation is +1 for AAA, and -1 for AAS
int AAA_AAS(char which_operation)
{
	return (regs16[REG_AX] += 262 * which_operation*set_AF(set_CF(((regs8[REG_AL] & 0x0F) > 9) || regs8[FLAG_AF])), regs8[REG_AL] &= 0x0F);
}

#ifndef NO_AUDIO
#ifndef NO_GRAPHICS
void audio_callback(void *data, unsigned char *stream, int len)
{
	for (int i = 0; i < len; i++)
		stream[i] = (spkr_en == 3) && CAST(unsigned short)mem[0x4AA] ? -((54 * wave_counter++ / CAST(unsigned short)mem[0x4AA]) & 1) : sdl_audio.silence;

	spkr_en = io_ports[0x61] & 3;
}
#endif
#endif

// Callback for tmt
#ifdef USE_TMT
void tmt_callback(tmt_msg_t m, TMT* vt, void const *a, void *p)
{
}
#endif

// Translates SDLKey to ASCII. Keysyms match ASCII only if SDL on the platform defines it that way.
#ifndef NO_GRAPHICS
unsigned int sdl_key_to_ascii(SDLKey sdl_key)
{
    switch(sdl_key)
    {
        case SDLK_BACKSPACE:        return 8;   break;
        case SDLK_TAB:              return 9;   break;
        case SDLK_CLEAR:		    return 12;  break;
        case SDLK_RETURN:           return 13;  break;
        case SDLK_PAUSE:            return 19;  break;
        case SDLK_ESCAPE:           return 27;  break;
        case SDLK_SPACE:		    return 32;  break;
        case SDLK_EXCLAIM:		    return 33;  break;
        case SDLK_QUOTEDBL:		    return 34;  break;
        case SDLK_HASH:             return 35;  break;
        case SDLK_DOLLAR:		    return 36;  break;
        case SDLK_AMPERSAND:		return 38;  break;
        case SDLK_QUOTE:		    return 39;  break;
        case SDLK_LEFTPAREN:        return 40;  break;
        case SDLK_RIGHTPAREN:       return 41;  break;
        case SDLK_ASTERISK:         return 42;  break;
        case SDLK_PLUS:             return 43;  break;
        case SDLK_COMMA:            return 44;  break;
        case SDLK_MINUS:            return 45;  break;
        case SDLK_PERIOD:           return 46;  break;
        case SDLK_SLASH:            return 47;  break;
        case SDLK_0:                return 48;  break;
        case SDLK_1:                return 49;  break;
        case SDLK_2:                return 50;  break;
        case SDLK_3:                return 51;  break;
        case SDLK_4:                return 52;  break;
        case SDLK_5:                return 53;  break;
        case SDLK_6:                return 54;  break;
        case SDLK_7:                return 55;  break;
        case SDLK_8:                return 56;  break;
        case SDLK_9:                return 57;  break;
        case SDLK_COLON:            return 58;  break;
        case SDLK_SEMICOLON:        return 59;  break;
        case SDLK_LESS:             return 60;  break;
        case SDLK_EQUALS:           return 61;  break;
        case SDLK_GREATER:          return 62;  break;
        case SDLK_QUESTION:         return 63;  break;
        case SDLK_AT:               return 64;  break;
        case SDLK_LEFTBRACKET:      return 91;  break;
        case SDLK_BACKSLASH:        return 92;  break;
        case SDLK_RIGHTBRACKET:     return 93;  break;
        case SDLK_CARET:            return 94;  break;
        case SDLK_UNDERSCORE:       return 95;  break;
        case SDLK_BACKQUOTE:        return 96;  break;
        case SDLK_a:                return 97;  break;
        case SDLK_b:                return 98;  break;
        case SDLK_c:                return 99;  break;
        case SDLK_d:                return 100; break;
        case SDLK_e:                return 101; break;
        case SDLK_f:                return 102; break;
        case SDLK_g:                return 103; break;
        case SDLK_h:                return 104; break;
        case SDLK_i:                return 105; break;
        case SDLK_j:                return 106; break;
        case SDLK_k:                return 107; break;
        case SDLK_l:                return 108; break;
        case SDLK_m:                return 109; break;
        case SDLK_n:                return 110; break;
        case SDLK_o:                return 111; break;
        case SDLK_p:                return 112; break;
        case SDLK_q:                return 113; break;
        case SDLK_r:                return 114; break;
        case SDLK_s:                return 115; break;
        case SDLK_t:                return 116; break;
        case SDLK_u:                return 117; break;
        case SDLK_v:                return 118; break;
        case SDLK_w:                return 119; break;
        case SDLK_x:                return 120; break;
        case SDLK_y:                return 121; break;
        case SDLK_z:                return 122; break;
        case SDLK_DELETE:           return 127; break;
        case SDLK_KP0:              return 256; break;
        case SDLK_KP1:		        return 257; break;
        case SDLK_KP2:		        return 258; break;
        case SDLK_KP3:		        return 259; break;
        case SDLK_KP4:		        return 260; break;
        case SDLK_KP5:		        return 261; break;
        case SDLK_KP6:		        return 262; break;
        case SDLK_KP7:		        return 263; break;
        case SDLK_KP8:		        return 264; break;
        case SDLK_KP9:		        return 265; break;
        case SDLK_KP_PERIOD:	    return 266; break;
        case SDLK_KP_DIVIDE:	    return 267; break;
        case SDLK_KP_MULTIPLY:	    return 268; break;
        case SDLK_KP_MINUS:		    return 269; break;
        case SDLK_KP_PLUS:		    return 270; break;
        case SDLK_KP_ENTER:		    return 271; break;
        case SDLK_KP_EQUALS:	    return 272; break;
        case SDLK_UP:			    return 273; break;
        case SDLK_DOWN:		        return 274; break;
        case SDLK_RIGHT:	        return 275; break;
        case SDLK_LEFT:		        return 276; break;
        case SDLK_INSERT:		    return 277; break;
        case SDLK_HOME:		        return 278; break;
        case SDLK_END:		        return 279; break;
        case SDLK_PAGEUP:		    return 280; break;
        case SDLK_PAGEDOWN:		    return 281; break;
        case SDLK_F1:			    return 282; break;
        case SDLK_F2:			    return 283; break;
        case SDLK_F3:			    return 284; break;
        case SDLK_F4:			    return 285; break;
        case SDLK_F5:			    return 286; break;
        case SDLK_F6:			    return 287; break;
        case SDLK_F7:			    return 288; break;
        case SDLK_F8:			    return 289; break;
        case SDLK_F9:			    return 290; break;
        case SDLK_F10:		        return 291; break;
        case SDLK_F11:		        return 292; break;
        case SDLK_F12:		        return 293; break;
        case SDLK_F13:		        return 294; break;
        case SDLK_F14:		        return 295; break;
        case SDLK_F15:		        return 296; break;
        case SDLK_NUMLOCK:		    return 300; break;
        case SDLK_CAPSLOCK:		    return 301; break;
        case SDLK_SCROLLOCK:	    return 302; break;
        case SDLK_RSHIFT:		    return 303; break;
        case SDLK_LSHIFT:		    return 304; break;
        case SDLK_RCTRL:	        return 305; break;
        case SDLK_LCTRL:	        return 306; break;
        case SDLK_RALT:		        return 307; break;
        case SDLK_LALT:		        return 308; break;
        default:                    return 0;   break;
    }
}
#endif

#ifndef NO_GRAPHICS
void set_video_mode()
{
    SDL_Init(SDL_INIT_VIDEO);
    sdl_screen = SDL_SetVideoMode(GRAPHICS_X, GRAPHICS_Y, 32, SDL_SWSURFACE);
    sdl_fmt = sdl_screen->format;
}
#endif

void init(int argc, char** argv)
{
	// Initialise SDL
#ifndef NO_GRAPHICS
    GRAPHICS_X = GRAPHICS_X_DEFAULT; GRAPHICS_Y = GRAPHICS_Y_DEFAULT; set_video_mode();
    is_display_init = 0;
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(500, 30);
    // Initialize audio
#ifndef NO_AUDIO
    SDL_InitSubSystem(SDL_INIT_AUDIO);
	sdl_audio.callback = audio_callback;
#ifdef _WIN32
	sdl_audio.samples = 512;
#endif
	SDL_OpenAudio(&sdl_audio, 0);
#endif
    // Initialize SDL_ttf for rendering vterm
#endif

    // Initialize TMT virtual terminal
#ifdef USE_TMT
    vterm = tmt_open(VTERM_LINES, VTERM_COLS, tmt_callback, NULL, NULL);
    vterm_needs_draw = 1;
#endif

	// regs16 and reg8 point to F000:0, the start of memory-mapped registers. CS is initialised to F000
	regs16 = (unsigned short *)(regs8 = mem + REGS_BASE);
	regs16[REG_CS] = 0xF000;

	// Trap flag off
	regs8[FLAG_TF] = 0;

	// Set DL equal to the boot device: 0 for the FD, or 0x80 for the HD. Normally, boot from the FD.
	// But, if the HD image file is prefixed with @, then boot from the HD
	regs8[REG_DL] = ((argc > 3) && (*argv[3] == '@')) ? argv[3]++, 0x80 : 0;

	// Open BIOS (file id disk[2]), floppy disk image (disk[1]), and hard disk image (disk[0]) if specified
	for (file_index = 3; file_index;)
        disk[--file_index] = (*++argv) ? fopen(*argv, "rb") : NULL;
        //disk[--file_index] = (*++argv) ? open(*argv, 32898) : 0;

	// Set CX:AX equal to the hard disk image size, if present
	CAST(unsigned)regs16[REG_AX] = *disk ? fseek(*disk, 0, SEEK_END) >> 9 : 0;

	// Load BIOS image into F000:0100, and set IP to 0100
	fread(regs8 + (reg_ip = 0x100), 1, 0xFF00, disk[2]);
    //read(disk[2], regs8 + (reg_ip = 0x100), 0xFF00);

	// Load instruction decoding helper table
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 256; j++)
			bios_table_lookup[i][j] = regs8[regs16[0x81 + i] + j];

    // Set main loop to continue
    cont_main_loop = 1;
}

void quit()
{
#ifdef USE_TMT
    tmt_close(vterm);
#endif
#ifndef NO_GRAPHICS
	SDL_Quit();
#endif
}


void main_loop()
{
    // Instruction execution loop. Loops for EMSCRIPTEN_INSTRUCTIONS_PER_FRAME. Terminates if CS:IP = 0:0
	for (int i = 0; i < EMSCRIPTEN_INSTRUCTIONS_PER_FRAME && cont_main_loop == 1; i++)
	{

        if(!((opcode_stream = mem + 16 * regs16[REG_CS] + reg_ip) && (opcode_stream != mem)))
        {
            // Quit logic
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
            quit();
#endif
            cont_main_loop = 0;
            break;
        }
		// Set up variables to prepare for decoding an opcode
		set_opcode(*opcode_stream);

		// Extract i_w and i_d fields from instruction
		i_w = (i_reg4bit = raw_opcode_id & 7) & 1;
		i_d = i_reg4bit / 2 & 1;

		// Extract instruction data fields
		i_data0 = CAST(short)opcode_stream[1];
		i_data1 = CAST(short)opcode_stream[2];
		i_data2 = CAST(short)opcode_stream[3];

		// seg_override_en and rep_override_en contain number of instructions to hold segment override and REP prefix respectively
		if (seg_override_en)
			seg_override_en--;
		if (rep_override_en)
			rep_override_en--;

		// i_mod_size > 0 indicates that opcode uses i_mod/i_rm/i_reg, so decode them
		if (i_mod_size)
		{
			i_mod = (i_data0 & 0xFF) >> 6;
			i_rm = i_data0 & 7;
			i_reg = i_data0 / 8 & 7;

			if ((!i_mod && i_rm == 6) || (i_mod == 2))
				i_data2 = CAST(short)opcode_stream[4];
			else if (i_mod != 1)
				i_data2 = i_data1;
			else // If i_mod is 1, operand is (usually) 8 bits rather than 16 bits
				i_data1 = (char)i_data1;

			DECODE_RM_REG;
		}

		// Instruction execution unit
		switch (xlat_opcode_id)
		{
			OPCODE_CHAIN 0: // Conditional jump (JAE, JNAE, etc.)
				// i_w is the invert flag, e.g. i_w == 1 means JNAE, whereas i_w == 0 means JAE
				scratch_uchar = raw_opcode_id / 2 & 7;
				reg_ip += (char)i_data0 * (i_w ^ (regs8[bios_table_lookup[TABLE_COND_JUMP_DECODE_A][scratch_uchar]] || regs8[bios_table_lookup[TABLE_COND_JUMP_DECODE_B][scratch_uchar]] || regs8[bios_table_lookup[TABLE_COND_JUMP_DECODE_C][scratch_uchar]] ^ regs8[bios_table_lookup[TABLE_COND_JUMP_DECODE_D][scratch_uchar]]))
			OPCODE 1: // MOV reg, imm
				i_w = !!(raw_opcode_id & 8);
				R_M_OP(mem[GET_REG_ADDR(i_reg4bit)], =, i_data0)
			OPCODE 3: // PUSH regs16
				R_M_PUSH(regs16[i_reg4bit])
			OPCODE 4: // POP regs16
				R_M_POP(regs16[i_reg4bit])
			OPCODE 2: // INC|DEC regs16
				i_w = 1;
				i_d = 0;
				i_reg = i_reg4bit;
				DECODE_RM_REG;
				i_reg = extra
			OPCODE_CHAIN 5: // INC|DEC|JMP|CALL|PUSH
				if (i_reg < 2) // INC|DEC
					MEM_OP(op_from_addr, += 1 - 2 * i_reg +, REGS_BASE + 2 * REG_ZERO),
					op_source = 1,
					set_AF_OF_arith(),
					set_OF(op_dest + 1 - i_reg == 1 << (TOP_BIT - 1)),
					(xlat_opcode_id == 5) && (set_opcode(0x10), 0); // Decode like ADC
				else if (i_reg != 6) // JMP|CALL
					i_reg - 3 || R_M_PUSH(regs16[REG_CS]), // CALL (far)
					i_reg & 2 && R_M_PUSH(reg_ip + 2 + i_mod*(i_mod != 3) + 2*(!i_mod && i_rm == 6)), // CALL (near or far)
					i_reg & 1 && (regs16[REG_CS] = CAST(short)mem[op_from_addr + 2]), // JMP|CALL (far)
					R_M_OP(reg_ip, =, mem[op_from_addr]),
					set_opcode(0x9A); // Decode like CALL
				else // PUSH
					R_M_PUSH(mem[rm_addr])
			OPCODE 6: // TEST r/m, imm16 / NOT|NEG|MUL|IMUL|DIV|IDIV reg
				op_to_addr = op_from_addr;

				switch (i_reg)
				{
					OPCODE_CHAIN 0: // TEST
						set_opcode(0x20); // Decode like AND
						reg_ip += i_w + 1;
						R_M_OP(mem[op_to_addr], &, i_data2)
					OPCODE 2: // NOT
						OP(=~)
					OPCODE 3: // NEG
						OP(=-);
						op_dest = 0;
						set_opcode(0x28); // Decode like SUB
						set_CF(op_result > op_dest)
					OPCODE 4: // MUL
						i_w ? MUL_MACRO(unsigned short, regs16) : MUL_MACRO(unsigned char, regs8)
					OPCODE 5: // IMUL
						i_w ? MUL_MACRO(short, regs16) : MUL_MACRO(char, regs8)
					OPCODE 6: // DIV
						i_w ? DIV_MACRO(unsigned short, unsigned, regs16) : DIV_MACRO(unsigned char, unsigned short, regs8)
					OPCODE 7: // IDIV
						i_w ? DIV_MACRO(short, int, regs16) : DIV_MACRO(char, short, regs8);
				}
			OPCODE 7: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP AL/AX, immed
				rm_addr = REGS_BASE;
				i_data2 = i_data0;
				i_mod = 3;
				i_reg = extra;
				reg_ip--;
			OPCODE_CHAIN 8: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP reg, immed
				op_to_addr = rm_addr;
				regs16[REG_SCRATCH] = (i_d |= !i_w) ? (char)i_data2 : i_data2;
				op_from_addr = REGS_BASE + 2 * REG_SCRATCH;
				reg_ip += !i_d + 1;
				set_opcode(0x08 * (extra = i_reg));
			OPCODE_CHAIN 9: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP|MOV reg, r/m
				switch (extra)
				{
					OPCODE_CHAIN 0: // ADD
						OP(+=),
						set_CF(op_result < op_dest)
					OPCODE 1: // OR
						OP(|=)
					OPCODE 2: // ADC
						ADC_SBB_MACRO(+)
					OPCODE 3: // SBB
						ADC_SBB_MACRO(-)
					OPCODE 4: // AND
						OP(&=)
					OPCODE 5: // SUB
						OP(-=),
						set_CF(op_result > op_dest)
					OPCODE 6: // XOR
						OP(^=)
					OPCODE 7: // CMP
						OP(-),
						set_CF(op_result > op_dest)
					OPCODE 8: // MOV
						OP(=);
				}
			OPCODE 10: // MOV sreg, r/m | POP r/m | LEA reg, r/m
				if (!i_w) // MOV
					i_w = 1,
					i_reg += 8,
					DECODE_RM_REG,
					OP(=);
				else if (!i_d) // LEA
					seg_override_en = 1,
					seg_override = REG_ZERO,
					DECODE_RM_REG,
					R_M_OP(mem[op_from_addr], =, rm_addr);
				else // POP
					R_M_POP(mem[rm_addr])
			OPCODE 11: // MOV AL/AX, [loc]
				i_mod = i_reg = 0;
				i_rm = 6;
				i_data1 = i_data0;
				DECODE_RM_REG;
				MEM_OP(op_from_addr, =, op_to_addr)
			OPCODE 12: // ROL|ROR|RCL|RCR|SHL|SHR|???|SAR reg/mem, 1/CL/imm (80186)
				scratch2_uint = SIGN_OF(mem[rm_addr]),
				scratch_uint = extra ? // xxx reg/mem, imm
					++reg_ip,
					(char)i_data1
				: // xxx reg/mem, CL
					i_d
						? 31 & regs8[REG_CL]
				: // xxx reg/mem, 1
					1;
				if (scratch_uint)
				{
					if (i_reg < 4) // Rotate operations
						scratch_uint %= i_reg / 2 + TOP_BIT,
						R_M_OP(scratch2_uint, =, mem[rm_addr]);
					if (i_reg & 1) // Rotate/shift right operations
						R_M_OP(mem[rm_addr], >>=, scratch_uint);
					else // Rotate/shift left operations
						R_M_OP(mem[rm_addr], <<=, scratch_uint);
					if (i_reg > 3) // Shift operations
						set_opcode(0x10); // Decode like ADC
					if (i_reg > 4) // SHR or SAR
						set_CF(op_dest >> (scratch_uint - 1) & 1);
				}

				switch (i_reg)
				{
					OPCODE_CHAIN 0: // ROL
						R_M_OP(mem[rm_addr], += , scratch2_uint >> (TOP_BIT - scratch_uint));
						set_OF(SIGN_OF(op_result) ^ set_CF(op_result & 1))
					OPCODE 1: // ROR
						scratch2_uint &= (1 << scratch_uint) - 1,
						R_M_OP(mem[rm_addr], += , scratch2_uint << (TOP_BIT - scratch_uint));
						set_OF(SIGN_OF(op_result * 2) ^ set_CF(SIGN_OF(op_result)))
					OPCODE 2: // RCL
						R_M_OP(mem[rm_addr], += (regs8[FLAG_CF] << (scratch_uint - 1)) + , scratch2_uint >> (1 + TOP_BIT - scratch_uint));
						set_OF(SIGN_OF(op_result) ^ set_CF(scratch2_uint & 1 << (TOP_BIT - scratch_uint)))
					OPCODE 3: // RCR
						R_M_OP(mem[rm_addr], += (regs8[FLAG_CF] << (TOP_BIT - scratch_uint)) + , scratch2_uint << (1 + TOP_BIT - scratch_uint));
						set_CF(scratch2_uint & 1 << (scratch_uint - 1));
						set_OF(SIGN_OF(op_result) ^ SIGN_OF(op_result * 2))
					OPCODE 4: // SHL
						set_OF(SIGN_OF(op_result) ^ set_CF(SIGN_OF(op_dest << (scratch_uint - 1))))
					OPCODE 5: // SHR
						set_OF(SIGN_OF(op_dest))
					OPCODE 7: // SAR
						scratch_uint < TOP_BIT || set_CF(scratch2_uint);
						set_OF(0);
						R_M_OP(mem[rm_addr], +=, scratch2_uint *= ~(((1 << TOP_BIT) - 1) >> scratch_uint));
				}
			OPCODE 13: // LOOPxx|JCZX
				scratch_uint = !!--regs16[REG_CX];

				switch(i_reg4bit)
				{
					OPCODE_CHAIN 0: // LOOPNZ
						scratch_uint &= !regs8[FLAG_ZF]
					OPCODE 1: // LOOPZ
						scratch_uint &= regs8[FLAG_ZF]
					OPCODE 3: // JCXXZ
						scratch_uint = !++regs16[REG_CX];
				}
				reg_ip += scratch_uint*(char)i_data0
			OPCODE 14: // JMP | CALL short/near
				reg_ip += 3 - i_d;
				if (!i_w)
				{
					if (i_d) // JMP far
						reg_ip = 0,
						regs16[REG_CS] = i_data2;
					else // CALL
						R_M_PUSH(reg_ip);
				}
				reg_ip += i_d && i_w ? (char)i_data0 : i_data0
			OPCODE 15: // TEST reg, r/m
				MEM_OP(op_from_addr, &, op_to_addr)
			OPCODE 16: // XCHG AX, regs16
				i_w = 1;
				op_to_addr = REGS_BASE;
				op_from_addr = GET_REG_ADDR(i_reg4bit);
			OPCODE_CHAIN 24: // NOP|XCHG reg, r/m
				if (op_to_addr != op_from_addr)
					OP(^=),
					MEM_OP(op_from_addr, ^=, op_to_addr),
					OP(^=)
			OPCODE 17: // MOVSx (extra=0)|STOSx (extra=1)|LODSx (extra=2)
				scratch2_uint = seg_override_en ? seg_override : REG_DS;

				for (scratch_uint = rep_override_en ? regs16[REG_CX] : 1; scratch_uint; scratch_uint--)
				{
					MEM_OP(extra < 2 ? SEGREG(REG_ES, REG_DI,) : REGS_BASE, =, extra & 1 ? REGS_BASE : SEGREG(scratch2_uint, REG_SI,)),
					extra & 1 || INDEX_INC(REG_SI),
					extra & 2 || INDEX_INC(REG_DI);
				}

				if (rep_override_en)
					regs16[REG_CX] = 0
			OPCODE 18: // CMPSx (extra=0)|SCASx (extra=1)
				scratch2_uint = seg_override_en ? seg_override : REG_DS;

				if ((scratch_uint = rep_override_en ? regs16[REG_CX] : 1))
				{
					for (; scratch_uint; rep_override_en || scratch_uint--)
					{
						MEM_OP(extra ? REGS_BASE : SEGREG(scratch2_uint, REG_SI,), -, SEGREG(REG_ES, REG_DI,)),
						extra || INDEX_INC(REG_SI),
						INDEX_INC(REG_DI), rep_override_en && !(--regs16[REG_CX] && (!op_result == rep_mode)) && (scratch_uint = 0);
					}

					set_flags_type = FLAGS_UPDATE_SZP | FLAGS_UPDATE_AO_ARITH; // Funge to set SZP/AO flags
					set_CF(op_result > op_dest);
				}
			OPCODE 19: // RET|RETF|IRET
				i_d = i_w;
				R_M_POP(reg_ip);
				if (extra) // IRET|RETF|RETF imm16
					R_M_POP(regs16[REG_CS]);
				if (extra & 2) // IRET
					set_flags(R_M_POP(scratch_uint));
				else if (!i_d) // RET|RETF imm16
					regs16[REG_SP] += i_data0
			OPCODE 20: // MOV r/m, immed
				R_M_OP(mem[op_from_addr], =, i_data2)
			OPCODE 21: // IN AL/AX, DX/imm8
				io_ports[0x20] = 0; // PIC EOI
				io_ports[0x42] = --io_ports[0x40]; // PIT channel 0/2 read placeholder
				io_ports[0x3DA] ^= 9; // CGA refresh
				scratch_uint = extra ? regs16[REG_DX] : (unsigned char)i_data0;
				scratch_uint == 0x60 && (io_ports[0x64] = 0); // Scancode read flag
				scratch_uint == 0x3D5 && (io_ports[0x3D4] >> 1 == 7) && (io_ports[0x3D5] = ((mem[0x49E]*80 + mem[0x49D] + CAST(short)mem[0x4AD]) & (io_ports[0x3D4] & 1 ? 0xFF : 0xFF00)) >> (io_ports[0x3D4] & 1 ? 0 : 8)); // CRT cursor position
				R_M_OP(regs8[REG_AL], =, io_ports[scratch_uint]);
			OPCODE 22: // OUT DX/imm8, AL/AX
				scratch_uint = extra ? regs16[REG_DX] : (unsigned char)i_data0;
				R_M_OP(io_ports[scratch_uint], =, regs8[REG_AL]);
				scratch_uint == 0x61 && (io_hi_lo = 0, spkr_en |= regs8[REG_AL] & 3); // Speaker control
				(scratch_uint == 0x40 || scratch_uint == 0x42) && (io_ports[0x43] & 6) && (mem[0x469 + scratch_uint - (io_hi_lo ^= 1)] = regs8[REG_AL]); // PIT rate programming
#ifndef NO_AUDIO
#ifndef NO_GRAPHICS
				scratch_uint == 0x43 && (io_hi_lo = 0, regs8[REG_AL] >> 6 == 2) && (SDL_PauseAudio((regs8[REG_AL] & 0xF7) != 0xB6), 0); // Speaker enable
#endif
#endif
				scratch_uint == 0x3D5 && (io_ports[0x3D4] >> 1 == 6) && (mem[0x4AD + !(io_ports[0x3D4] & 1)] = regs8[REG_AL]); // CRT video RAM start offset
				scratch_uint == 0x3D5 && (io_ports[0x3D4] >> 1 == 7) && (scratch2_uint = ((mem[0x49E]*80 + mem[0x49D] + CAST(short)mem[0x4AD]) & (io_ports[0x3D4] & 1 ? 0xFF00 : 0xFF)) + (regs8[REG_AL] << (io_ports[0x3D4] & 1 ? 0 : 8)) - CAST(short)mem[0x4AD], mem[0x49D] = scratch2_uint % 80, mem[0x49E] = scratch2_uint / 80); // CRT cursor position
				scratch_uint == 0x3B5 && io_ports[0x3B4] == 1 && (GRAPHICS_X = regs8[REG_AL] * 16); // Hercules resolution reprogramming. Defaults are set in the BIOS
				scratch_uint == 0x3B5 && io_ports[0x3B4] == 6 && (GRAPHICS_Y = regs8[REG_AL] * 4);
			OPCODE 23: // REPxx
				rep_override_en = 2;
				rep_mode = i_w;
				seg_override_en && seg_override_en++
			OPCODE 25: // PUSH reg
				R_M_PUSH(regs16[extra])
			OPCODE 26: // POP reg
				R_M_POP(regs16[extra])
			OPCODE 27: // xS: segment overrides
				seg_override_en = 2;
				seg_override = extra;
				rep_override_en && rep_override_en++
			OPCODE 28: // DAA/DAS
				i_w = 0;
				extra ? DAA_DAS(-=, >=, 0xFF, 0x99) : DAA_DAS(+=, <, 0xF0, 0x90) // extra = 0 for DAA, 1 for DAS
			OPCODE 29: // AAA/AAS
				op_result = AAA_AAS(extra - 1)
			OPCODE 30: // CBW
				regs8[REG_AH] = -SIGN_OF(regs8[REG_AL])
			OPCODE 31: // CWD
				regs16[REG_DX] = -SIGN_OF(regs16[REG_AX])
			OPCODE 32: // CALL FAR imm16:imm16
				R_M_PUSH(regs16[REG_CS]);
				R_M_PUSH(reg_ip + 5);
				regs16[REG_CS] = i_data2;
				reg_ip = i_data0
			OPCODE 33: // PUSHF
				make_flags();
				R_M_PUSH(scratch_uint)
			OPCODE 34: // POPF
				set_flags(R_M_POP(scratch_uint))
			OPCODE 35: // SAHF
				make_flags();
				set_flags((scratch_uint & 0xFF00) + regs8[REG_AH])
			OPCODE 36: // LAHF
				make_flags(),
				regs8[REG_AH] = scratch_uint
			OPCODE 37: // LES|LDS reg, r/m
				i_w = i_d = 1;
				DECODE_RM_REG;
				OP(=);
				MEM_OP(REGS_BASE + extra, =, rm_addr + 2)
			OPCODE 38: // INT 3
				++reg_ip;
				pc_interrupt(3)
			OPCODE 39: // INT imm8
				reg_ip += 2;
				pc_interrupt(i_data0)
			OPCODE 40: // INTO
				++reg_ip;
				regs8[FLAG_OF] && pc_interrupt(4)
			OPCODE 41: // AAM
				if (i_data0 &= 0xFF)
					regs8[REG_AH] = regs8[REG_AL] / i_data0,
					op_result = regs8[REG_AL] %= i_data0;
				else // Divide by zero
					pc_interrupt(0)
			OPCODE 42: // AAD
				i_w = 0;
				regs16[REG_AX] = op_result = 0xFF & regs8[REG_AL] + i_data0 * regs8[REG_AH]
			OPCODE 43: // SALC
				regs8[REG_AL] = -regs8[FLAG_CF]
			OPCODE 44: // XLAT
				regs8[REG_AL] = mem[SEGREG(seg_override_en ? seg_override : REG_DS, REG_BX, regs8[REG_AL] +)]
			OPCODE 45: // CMC
				regs8[FLAG_CF] ^= 1
			OPCODE 46: // CLC|STC|CLI|STI|CLD|STD
				regs8[extra / 2] = extra & 1
			OPCODE 47: // TEST AL/AX, immed
				R_M_OP(regs8[REG_AL], &, i_data0)
			OPCODE 48: // Emulator-specific 0F xx opcodes

				switch ((char)i_data0)
				{

					OPCODE_CHAIN 0: // PUTCHAR_AL
						//write(1, regs8, 1);
#ifdef USE_TMT
                        tmt_write(vterm, regs8, 1);
                        vterm_needs_draw = 1;
#else
                        putchar(*regs8);
                        fflush(stdout);
#endif
					OPCODE 1: // GET_RTC
						time(&clock_buf);
						ftime(&ms_clock);
						memcpy(mem + SEGREG(REG_ES, REG_BX,), localtime(&clock_buf), sizeof(struct tm));
						CAST(short)mem[SEGREG(REG_ES, REG_BX, 36+)] = ms_clock.millitm;
					OPCODE 2: // DISK_READ
					OPCODE_CHAIN 3: // DISK_WRITE
					    if(~fseek(disk[regs8[REG_DL]], CAST(unsigned)regs16[REG_BP] << 9, SEEK_SET))
                            if((char)i_data0 == 3)
                                regs8[REG_AL] = fwrite(mem + SEGREG(REG_ES, REG_BX,), 1, regs16[REG_AX], disk[regs8[REG_DL]]);
                            else
                                regs8[REG_AL] = fread(mem + SEGREG(REG_ES, REG_BX,), 1, regs16[REG_AX], disk[regs8[REG_DL]]);
						/*regs8[REG_AL] = ~fseek(disk[regs8[REG_DL]], CAST(unsigned)regs16[REG_BP] << 9, SEEK_SET)
							? ((char)i_data0 == 3 ? (int(*)())write : (int(*)())read)(disk[regs8[REG_DL]], mem + SEGREG(REG_ES, REG_BX,), regs16[REG_AX])
							: 0;*/

				}

		}

		// Increment instruction pointer by computed instruction length. Tables in the BIOS binary
		// help us here.
		reg_ip += (i_mod*(i_mod != 3) + 2*(!i_mod && i_rm == 6))*i_mod_size + bios_table_lookup[TABLE_BASE_INST_SIZE][raw_opcode_id] + bios_table_lookup[TABLE_I_W_SIZE][raw_opcode_id]*(i_w + 1);

		// If instruction needs to update SF, ZF and PF, set them as appropriate
		if (set_flags_type & FLAGS_UPDATE_SZP)
		{
			regs8[FLAG_SF] = SIGN_OF(op_result);
			regs8[FLAG_ZF] = !op_result;
			regs8[FLAG_PF] = bios_table_lookup[TABLE_PARITY_FLAG][(unsigned char)op_result];

			// If instruction is an arithmetic or logic operation, also set AF/OF/CF as appropriate.
			if (set_flags_type & FLAGS_UPDATE_AO_ARITH)
				set_AF_OF_arith();
			if (set_flags_type & FLAGS_UPDATE_OC_LOGIC)
				set_CF(0), set_OF(0);
		}

		// Poll timer/keyboard every KEYBOARD_TIMER_UPDATE_DELAY instructions
		if (!(++inst_counter % KEYBOARD_TIMER_UPDATE_DELAY))
			int8_asap = 1;

		// Application has set trap flag, so fire INT 1
		if (trap_flag)
			pc_interrupt(1);

		trap_flag = regs8[FLAG_TF];

		// If a timer tick is pending, interrupts are enabled, and no overrides/REP are active,
		// then process the tick and check for new keystrokes
		if (int8_asap && !seg_override_en && !rep_override_en && regs8[FLAG_IF] && !regs8[FLAG_TF])
        {
			pc_interrupt(0xA);
            int8_asap = 0;
#ifndef NO_GRAPHICS
            while(SDL_PollEvent(&sdl_event))
                if(sdl_event.type == SDL_KEYDOWN || sdl_event.type == SDL_KEYUP)
                {
                    scratch_uint = sdl_event.key.keysym.unicode;
                    scratch2_uint = sdl_event.key.keysym.mod;
                    scratch3_uint = sdl_key_to_ascii(sdl_event.key.keysym.sym);
                    CAST(short)mem[0x4A6] = 0x400 + 0x800*!!(scratch2_uint & KMOD_ALT) + 0x1000*!!(scratch2_uint & KMOD_SHIFT) + 0x2000*!!(scratch2_uint & KMOD_CTRL) + 0x4000*(sdl_event.type == SDL_KEYUP) + ((!(scratch_uint) || scratch_uint > 0x7F) ? scratch3_uint : scratch_uint);
                    pc_interrupt(7);
                }
#else
            KEYBOARD_DRIVER;
#endif
        }
	}
    // Draw virtual terminal
#ifdef USE_TMT
    if(vterm_needs_draw)
    {
        TMTSCREEN const *s = tmt_screen(vterm);
        for(size_t i = 0; i < VTERM_BLANK_LINES; i++)
            putchar('\n');
        for(size_t r = 0; r < s->nline; ++r)
        {
            for(size_t c = 0; c < s->ncol; ++c)
                if(!(s->lines[r]->chars[c].c & 0x80)) putchar(s->lines[r]->chars[c].c);
            putchar('\n');
#ifdef VTERM_USE_CR
            putchar('\r');
#endif
        }
        vterm_needs_draw = 0;
        tmt_clean(vterm);
    }
#endif

#ifndef NO_GRAPHICS
#ifndef REFRESH_DISPLAY_EVERY_FRAME
    // Update the video graphics display every GRAPHICS_UPDATE_DELAY instructions
    if (!(inst_counter % GRAPHICS_UPDATE_DELAY))
    {
#endif
        // Video card in graphics mode?
        if (io_ports[0x3B8] & 2)
        {
            // If we don't already have an SDL window open, set it up and compute color and video memory translation tables
            if (is_display_init != 1)
            {
                is_display_init = 1;
                for (int i = 0; i < 16; i++)
                    pixel_colors[i] = mem[0x4AC] ? // CGA?
                        cga_colors[(i & 12) >> 2] + (cga_colors[i & 3] << 16) // CGA -> RGB332
                        : 0xFF*(((i & 1) << 24) + ((i & 2) << 15) + ((i & 4) << 6) + ((i & 8) >> 3)); // Hercules -> RGB332

                for (int i = 0; i < GRAPHICS_X * GRAPHICS_Y / 4; i++)
                    vid_addr_lookup[i] = i / GRAPHICS_X * (GRAPHICS_X / 8) + (i / 2) % (GRAPHICS_X / 8) + 0x2000*(mem[0x4AC] ? (2 * i / GRAPHICS_X) % 2 : (4 * i / GRAPHICS_X) % 4);

                SDL_QuitSubSystem(SDL_INIT_VIDEO);
                set_video_mode();
            }

            // Refresh SDL display from emulated graphics card video RAM
            vid_mem_base = mem + 0xB0000 + 0x8000*(mem[0x4AC] ? 1 : io_ports[0x3B8] >> 7); // B800:0 for CGA/Hercules bank 2, B000:0 for Hercules bank 1
            if(SDL_MUSTLOCK(sdl_screen)) SDL_LockSurface(sdl_screen);
            for (int i = 0; i < GRAPHICS_X * GRAPHICS_Y / 4; i++)
            {
                unsigned int col_4_pack = pixel_colors[15 & (vid_mem_base[vid_addr_lookup[i]] >> 4*!(i & 1))];
                int x = (i % (GRAPHICS_X / 4)) * 4;
                int y = i / (GRAPHICS_X / 4);
                for(int j = 0; j < 4; ++j)
                {
                    *((uint32_t*)sdl_screen->pixels + x + j + (y * GRAPHICS_X)) = SDL_MapRGBA(sdl_fmt, col_4_pack & 0xE0, (col_4_pack & 0x1C) << 3, (col_4_pack & 0x02) << 6, 255);
                    col_4_pack >>= 8;
                }
            }
            if(SDL_MUSTLOCK(sdl_screen)) SDL_UnlockSurface(sdl_screen);
            SDL_Flip(sdl_screen);
                	SDL_SaveBMP( sdl_screen, "your_snapshot_name.bmp" );

        }
        else
        {
            if (is_display_init == 1) // Application has gone back to text mode, so return the SDL window to defaults
            {
                SDL_QuitSubSystem(SDL_INIT_VIDEO);
                GRAPHICS_X = GRAPHICS_X_DEFAULT; GRAPHICS_Y = GRAPHICS_Y_DEFAULT; set_video_mode();
            }
        }
        SDL_PumpEvents();
#ifndef REFRESH_DISPLAY_EVERY_FRAME
    }
#endif
#ifndef PUMP_EVENTS_EVERY_FRAME
    if (!(inst_counter % GRAPHICS_UPDATE_DELAY))
#endif
        SDL_PumpEvents();
#endif



}

void irrlichtloop(){
#ifdef IRRLICHT
//	env->addImage(driver->getTexture("your_snapshot_name.bmp"),position2d<int>(10,10));
//main_loop();
			device->run();

		//{check joysticks
//		if ( SDL_NumJoysticks() < 1 ) joy_error->setVisible(true); else joy_error->setVisible(false);
		//}

			//	if (device->isWindowActive())
			//{
			//	driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, SColor(0,200,200,200));
				driver->beginScene(true, true, video::SColor(0,220,220,220));

				env->drawAll();

				driver->endScene();
				device->sleep(20);
			//}
			#endif

		}

int main(int argc, char** argv)
{
    // Simulate command line args for emscripten
#ifdef __EMSCRIPTEN__
#ifdef EMSCRIPTEN_USE_HD
    char* arg[] = {"", EMSCRIPTEN_BIOS_FILE, EMSCRIPTEN_FD_FILE, EMSCRIPTEN_HD_FILE};
    argc = 4
#else
    char* arg[] = {"", EMSCRIPTEN_BIOS_FILE, EMSCRIPTEN_FD_FILE};
    argc = 3;
#endif
    argv = arg;
#endif
	char *argv1[]={"appname",EMSCRIPTEN_BIOS_FILE,EMSCRIPTEN_FD_FILE,"test"};//rectangular_BFS
	int argc1 = sizeof(argv1) / sizeof(char*) - 1;
//SDL_Event event;
    init(argc1, argv1);

    #ifdef IRRLICHT
	//device = createDevice(EDT_OPENGL, core::dimension2d<u32>(640, 480));

	device =
		createDevice( video::EDT_BURNINGSVIDEO, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	driver=device->getVideoDriver();
	 smgr = device->getSceneManager();
	if (device == 0)
		return 1; // could not create selected driver.
//		//
//		device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
//		device->setResizable(true);
	#endif





//{error text
//IGUIStaticText* joy_error = env->addStaticText(L"Please insert a joystick or gamepad...",rect<s32>(15,1,260,22), true);
//joy_error->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
//joy_error->move(position2d<int>(350,350) );
//}

//}

//{open joysticks

//Open joystick1
//SDL_JoystickEventState(SDL_ENABLE);
//joystick1 = SDL_JoystickOpen(0);


//	env->addImage(driver->getTexture(mediaPath + "irrlichtlogo2.png"),
//			position2d<int>(10,10));



#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, EMSCRIPTEN_MAIN_LOOP_FRAMERATE, 1);
#else
    while(cont_main_loop == 1) main_loop();

#ifdef IRRLICHT

	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	while(device->run())
	{
	main_loop();

	env->addImage(driver->getTexture("your_snapshot_name.bmp"),position2d<int>(10,10));
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene() call clears the screen with a color and
		the depth buffer, if desired. Then we let the Scene Manager and
		the GUI Environment draw their content. With the endScene()
		call everything is presented on the screen.
		*/
		driver->beginScene(true, true, video::SColor(0,220,220,220));
		//driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(255,100,101,140));

		smgr->drawAll();
	//	guienv->drawAll();

		driver->endScene();
	}

//
//while(1){
//irrlichtloop();
//};
device->drop();
#endif

#endif
#ifndef __EMSCRIPTEN__
    quit();
#endif
}
