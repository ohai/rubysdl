/* rubysdl.c -- SDL(Simple Directmedia Layer)
 */

#include "rubysdl.h"

static VALUE sdl_init(VALUE obj,VALUE flags)
{
  Uint32 flag;

  flag= NUM2INT(flags);
  flag = flag & ~(SDL_INIT_TIMER & SDL_INIT_THREAD);
  return INT2FIX(SDL_Init(flag)); 
}

static VALUE sdl_quit(VALUE obj)
{
  SDL_Quit();
  return Qnil;
}

static void defineConst()
{
  rb_define_const(gSDLModule,"INIT_TIMER",INT2NUM(SDL_INIT_TIMER));
  rb_define_const(gSDLModule,"INIT_AUDIO",INT2NUM(SDL_INIT_AUDIO));
  rb_define_const(gSDLModule,"INIT_VIDEO",INT2NUM(SDL_INIT_VIDEO));
  rb_define_const(gSDLModule,"INIT_CDROM",INT2NUM(SDL_INIT_CDROM));
  rb_define_const(gSDLModule,"INIT_JOYSTICK",INT2NUM(SDL_INIT_JOYSTICK));
  rb_define_const(gSDLModule,"INIT_NOPARACHUTE",INT2NUM(SDL_INIT_NOPARACHUTE));
  rb_define_const(gSDLModule,"INIT_EVENTTHREAD",INT2NUM(SDL_INIT_EVENTTHREAD));
  rb_define_const(gSDLModule,"INIT_EVERYTHING",INT2NUM(SDL_INIT_EVERYTHING));
}


static void defineConstForKey()
{
  rb_define_const(gKeyClass,"UNKNOWN",INT2NUM(SDLK_UNKNOWN));
  rb_define_const(gKeyClass,"FIRST",INT2NUM(SDLK_FIRST));
  rb_define_const(gKeyClass,"BACKSPACE",INT2NUM(SDLK_BACKSPACE));
  rb_define_const(gKeyClass,"TAB",INT2NUM(SDLK_TAB));
  rb_define_const(gKeyClass,"CLEAR",INT2NUM(SDLK_CLEAR));
  rb_define_const(gKeyClass,"RETURN",INT2NUM(SDLK_RETURN));
  rb_define_const(gKeyClass,"PAUSE",INT2NUM(SDLK_PAUSE));
  rb_define_const(gKeyClass,"ESCAPE",INT2NUM(SDLK_ESCAPE));
  rb_define_const(gKeyClass,"SPACE",INT2NUM(SDLK_SPACE));
  rb_define_const(gKeyClass,"EXCLAIM",INT2NUM(SDLK_EXCLAIM));
  rb_define_const(gKeyClass,"QUOTEDBL",INT2NUM(SDLK_QUOTEDBL));
  rb_define_const(gKeyClass,"HASH",INT2NUM(SDLK_HASH));
  rb_define_const(gKeyClass,"DOLLAR",INT2NUM(SDLK_DOLLAR));
  rb_define_const(gKeyClass,"AMPERSAND",INT2NUM(SDLK_AMPERSAND));
  rb_define_const(gKeyClass,"QUOTE",INT2NUM(SDLK_QUOTE));
  rb_define_const(gKeyClass,"LEFTPAREN",INT2NUM(SDLK_LEFTPAREN));
  rb_define_const(gKeyClass,"RIGHTPAREN",INT2NUM(SDLK_RIGHTPAREN));
  rb_define_const(gKeyClass,"ASTERISK",INT2NUM(SDLK_ASTERISK));
  rb_define_const(gKeyClass,"PLUS",INT2NUM(SDLK_PLUS));
  rb_define_const(gKeyClass,"COMMA",INT2NUM(SDLK_COMMA));
  rb_define_const(gKeyClass,"MINUS",INT2NUM(SDLK_MINUS));
  rb_define_const(gKeyClass,"PERIOD",INT2NUM(SDLK_PERIOD));
  rb_define_const(gKeyClass,"SLASH",INT2NUM(SDLK_SLASH));
  rb_define_const(gKeyClass,"K0",INT2NUM(SDLK_0));
  rb_define_const(gKeyClass,"K1",INT2NUM(SDLK_1));
  rb_define_const(gKeyClass,"K2",INT2NUM(SDLK_2));
  rb_define_const(gKeyClass,"K3",INT2NUM(SDLK_3));
  rb_define_const(gKeyClass,"K4",INT2NUM(SDLK_4));
  rb_define_const(gKeyClass,"K5",INT2NUM(SDLK_5));
  rb_define_const(gKeyClass,"K6",INT2NUM(SDLK_6));
  rb_define_const(gKeyClass,"K7",INT2NUM(SDLK_7));
  rb_define_const(gKeyClass,"K8",INT2NUM(SDLK_8));
  rb_define_const(gKeyClass,"K9",INT2NUM(SDLK_9));
  rb_define_const(gKeyClass,"COLON",INT2NUM(SDLK_COLON));
  rb_define_const(gKeyClass,"SEMICOLON",INT2NUM(SDLK_SEMICOLON));
  rb_define_const(gKeyClass,"LESS",INT2NUM(SDLK_LESS));
  rb_define_const(gKeyClass,"EQUALS",INT2NUM(SDLK_EQUALS));
  rb_define_const(gKeyClass,"GREATER",INT2NUM(SDLK_GREATER));
  rb_define_const(gKeyClass,"QUESTION",INT2NUM(SDLK_QUESTION));
  rb_define_const(gKeyClass,"AT",INT2NUM(SDLK_AT));
  /* 
     Skip uppercase letters
     */
  rb_define_const(gKeyClass,"LEFTBRACKET",INT2NUM(SDLK_LEFTBRACKET));
  rb_define_const(gKeyClass,"BACKSLASH",INT2NUM(SDLK_BACKSLASH));
  rb_define_const(gKeyClass,"RIGHTBRACKET",INT2NUM(SDLK_RIGHTBRACKET));
  rb_define_const(gKeyClass,"CARET",INT2NUM(SDLK_CARET));
  rb_define_const(gKeyClass,"UNDERSCORE",INT2NUM(SDLK_UNDERSCORE));
  rb_define_const(gKeyClass,"BACKQUOTE",INT2NUM(SDLK_BACKQUOTE));
  rb_define_const(gKeyClass,"A",INT2NUM(SDLK_a));
  rb_define_const(gKeyClass,"B",INT2NUM(SDLK_b));
  rb_define_const(gKeyClass,"C",INT2NUM(SDLK_c));
  rb_define_const(gKeyClass,"D",INT2NUM(SDLK_d));
  rb_define_const(gKeyClass,"E",INT2NUM(SDLK_e));
  rb_define_const(gKeyClass,"F",INT2NUM(SDLK_f));
  rb_define_const(gKeyClass,"G",INT2NUM(SDLK_g));
  rb_define_const(gKeyClass,"H",INT2NUM(SDLK_h));
  rb_define_const(gKeyClass,"I",INT2NUM(SDLK_i));
  rb_define_const(gKeyClass,"J",INT2NUM(SDLK_j));
  rb_define_const(gKeyClass,"K",INT2NUM(SDLK_k));
  rb_define_const(gKeyClass,"L",INT2NUM(SDLK_l));
  rb_define_const(gKeyClass,"M",INT2NUM(SDLK_m));
  rb_define_const(gKeyClass,"N",INT2NUM(SDLK_n));
  rb_define_const(gKeyClass,"O",INT2NUM(SDLK_o));
  rb_define_const(gKeyClass,"P",INT2NUM(SDLK_p));
  rb_define_const(gKeyClass,"Q",INT2NUM(SDLK_q));
  rb_define_const(gKeyClass,"R",INT2NUM(SDLK_r));
  rb_define_const(gKeyClass,"S",INT2NUM(SDLK_s));
  rb_define_const(gKeyClass,"T",INT2NUM(SDLK_t));
  rb_define_const(gKeyClass,"U",INT2NUM(SDLK_u));
  rb_define_const(gKeyClass,"V",INT2NUM(SDLK_v));
  rb_define_const(gKeyClass,"W",INT2NUM(SDLK_w));
  rb_define_const(gKeyClass,"X",INT2NUM(SDLK_x));
  rb_define_const(gKeyClass,"Y",INT2NUM(SDLK_y));
  rb_define_const(gKeyClass,"Z",INT2NUM(SDLK_z));
  rb_define_const(gKeyClass,"DELETE",INT2NUM(SDLK_DELETE));
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
  rb_define_const(gKeyClass,"WORLD_0",INT2NUM(SDLK_WORLD_0));
  rb_define_const(gKeyClass,"WORLD_1",INT2NUM(SDLK_WORLD_1));
  rb_define_const(gKeyClass,"WORLD_2",INT2NUM(SDLK_WORLD_2));
  rb_define_const(gKeyClass,"WORLD_3",INT2NUM(SDLK_WORLD_3));
  rb_define_const(gKeyClass,"WORLD_4",INT2NUM(SDLK_WORLD_4));
  rb_define_const(gKeyClass,"WORLD_5",INT2NUM(SDLK_WORLD_5));
  rb_define_const(gKeyClass,"WORLD_6",INT2NUM(SDLK_WORLD_6));
  rb_define_const(gKeyClass,"WORLD_7",INT2NUM(SDLK_WORLD_7));
  rb_define_const(gKeyClass,"WORLD_8",INT2NUM(SDLK_WORLD_8));
  rb_define_const(gKeyClass,"WORLD_9",INT2NUM(SDLK_WORLD_9));
  rb_define_const(gKeyClass,"WORLD_10",INT2NUM(SDLK_WORLD_10));
  rb_define_const(gKeyClass,"WORLD_11",INT2NUM(SDLK_WORLD_11));
  rb_define_const(gKeyClass,"WORLD_12",INT2NUM(SDLK_WORLD_12));
  rb_define_const(gKeyClass,"WORLD_13",INT2NUM(SDLK_WORLD_13));
  rb_define_const(gKeyClass,"WORLD_14",INT2NUM(SDLK_WORLD_14));
  rb_define_const(gKeyClass,"WORLD_15",INT2NUM(SDLK_WORLD_15));
  rb_define_const(gKeyClass,"WORLD_16",INT2NUM(SDLK_WORLD_16));
  rb_define_const(gKeyClass,"WORLD_17",INT2NUM(SDLK_WORLD_17));
  rb_define_const(gKeyClass,"WORLD_18",INT2NUM(SDLK_WORLD_18));
  rb_define_const(gKeyClass,"WORLD_19",INT2NUM(SDLK_WORLD_19));
  rb_define_const(gKeyClass,"WORLD_20",INT2NUM(SDLK_WORLD_20));
  rb_define_const(gKeyClass,"WORLD_21",INT2NUM(SDLK_WORLD_21));
  rb_define_const(gKeyClass,"WORLD_22",INT2NUM(SDLK_WORLD_22));
  rb_define_const(gKeyClass,"WORLD_23",INT2NUM(SDLK_WORLD_23));
  rb_define_const(gKeyClass,"WORLD_24",INT2NUM(SDLK_WORLD_24));
  rb_define_const(gKeyClass,"WORLD_25",INT2NUM(SDLK_WORLD_25));
  rb_define_const(gKeyClass,"WORLD_26",INT2NUM(SDLK_WORLD_26));
  rb_define_const(gKeyClass,"WORLD_27",INT2NUM(SDLK_WORLD_27));
  rb_define_const(gKeyClass,"WORLD_28",INT2NUM(SDLK_WORLD_28));
  rb_define_const(gKeyClass,"WORLD_29",INT2NUM(SDLK_WORLD_29));
  rb_define_const(gKeyClass,"WORLD_30",INT2NUM(SDLK_WORLD_30));
  rb_define_const(gKeyClass,"WORLD_31",INT2NUM(SDLK_WORLD_31));
  rb_define_const(gKeyClass,"WORLD_32",INT2NUM(SDLK_WORLD_32));
  rb_define_const(gKeyClass,"WORLD_33",INT2NUM(SDLK_WORLD_33));
  rb_define_const(gKeyClass,"WORLD_34",INT2NUM(SDLK_WORLD_34));
  rb_define_const(gKeyClass,"WORLD_35",INT2NUM(SDLK_WORLD_35));
  rb_define_const(gKeyClass,"WORLD_36",INT2NUM(SDLK_WORLD_36));
  rb_define_const(gKeyClass,"WORLD_37",INT2NUM(SDLK_WORLD_37));
  rb_define_const(gKeyClass,"WORLD_38",INT2NUM(SDLK_WORLD_38));
  rb_define_const(gKeyClass,"WORLD_39",INT2NUM(SDLK_WORLD_39));
  rb_define_const(gKeyClass,"WORLD_40",INT2NUM(SDLK_WORLD_40));
  rb_define_const(gKeyClass,"WORLD_41",INT2NUM(SDLK_WORLD_41));
  rb_define_const(gKeyClass,"WORLD_42",INT2NUM(SDLK_WORLD_42));
  rb_define_const(gKeyClass,"WORLD_43",INT2NUM(SDLK_WORLD_43));
  rb_define_const(gKeyClass,"WORLD_44",INT2NUM(SDLK_WORLD_44));
  rb_define_const(gKeyClass,"WORLD_45",INT2NUM(SDLK_WORLD_45));
  rb_define_const(gKeyClass,"WORLD_46",INT2NUM(SDLK_WORLD_46));
  rb_define_const(gKeyClass,"WORLD_47",INT2NUM(SDLK_WORLD_47));
  rb_define_const(gKeyClass,"WORLD_48",INT2NUM(SDLK_WORLD_48));
  rb_define_const(gKeyClass,"WORLD_49",INT2NUM(SDLK_WORLD_49));
  rb_define_const(gKeyClass,"WORLD_50",INT2NUM(SDLK_WORLD_50));
  rb_define_const(gKeyClass,"WORLD_51",INT2NUM(SDLK_WORLD_51));
  rb_define_const(gKeyClass,"WORLD_52",INT2NUM(SDLK_WORLD_52));
  rb_define_const(gKeyClass,"WORLD_53",INT2NUM(SDLK_WORLD_53));
  rb_define_const(gKeyClass,"WORLD_54",INT2NUM(SDLK_WORLD_54));
  rb_define_const(gKeyClass,"WORLD_55",INT2NUM(SDLK_WORLD_55));
  rb_define_const(gKeyClass,"WORLD_56",INT2NUM(SDLK_WORLD_56));
  rb_define_const(gKeyClass,"WORLD_57",INT2NUM(SDLK_WORLD_57));
  rb_define_const(gKeyClass,"WORLD_58",INT2NUM(SDLK_WORLD_58));
  rb_define_const(gKeyClass,"WORLD_59",INT2NUM(SDLK_WORLD_59));
  rb_define_const(gKeyClass,"WORLD_60",INT2NUM(SDLK_WORLD_60));
  rb_define_const(gKeyClass,"WORLD_61",INT2NUM(SDLK_WORLD_61));
  rb_define_const(gKeyClass,"WORLD_62",INT2NUM(SDLK_WORLD_62));
  rb_define_const(gKeyClass,"WORLD_63",INT2NUM(SDLK_WORLD_63));
  rb_define_const(gKeyClass,"WORLD_64",INT2NUM(SDLK_WORLD_64));
  rb_define_const(gKeyClass,"WORLD_65",INT2NUM(SDLK_WORLD_65));
  rb_define_const(gKeyClass,"WORLD_66",INT2NUM(SDLK_WORLD_66));
  rb_define_const(gKeyClass,"WORLD_67",INT2NUM(SDLK_WORLD_67));
  rb_define_const(gKeyClass,"WORLD_68",INT2NUM(SDLK_WORLD_68));
  rb_define_const(gKeyClass,"WORLD_69",INT2NUM(SDLK_WORLD_69));
  rb_define_const(gKeyClass,"WORLD_70",INT2NUM(SDLK_WORLD_70));
  rb_define_const(gKeyClass,"WORLD_71",INT2NUM(SDLK_WORLD_71));
  rb_define_const(gKeyClass,"WORLD_72",INT2NUM(SDLK_WORLD_72));
  rb_define_const(gKeyClass,"WORLD_73",INT2NUM(SDLK_WORLD_73));
  rb_define_const(gKeyClass,"WORLD_74",INT2NUM(SDLK_WORLD_74));
  rb_define_const(gKeyClass,"WORLD_75",INT2NUM(SDLK_WORLD_75));
  rb_define_const(gKeyClass,"WORLD_76",INT2NUM(SDLK_WORLD_76));
  rb_define_const(gKeyClass,"WORLD_77",INT2NUM(SDLK_WORLD_77));
  rb_define_const(gKeyClass,"WORLD_78",INT2NUM(SDLK_WORLD_78));
  rb_define_const(gKeyClass,"WORLD_79",INT2NUM(SDLK_WORLD_79));
  rb_define_const(gKeyClass,"WORLD_80",INT2NUM(SDLK_WORLD_80));
  rb_define_const(gKeyClass,"WORLD_81",INT2NUM(SDLK_WORLD_81));
  rb_define_const(gKeyClass,"WORLD_82",INT2NUM(SDLK_WORLD_82));
  rb_define_const(gKeyClass,"WORLD_83",INT2NUM(SDLK_WORLD_83));
  rb_define_const(gKeyClass,"WORLD_84",INT2NUM(SDLK_WORLD_84));
  rb_define_const(gKeyClass,"WORLD_85",INT2NUM(SDLK_WORLD_85));
  rb_define_const(gKeyClass,"WORLD_86",INT2NUM(SDLK_WORLD_86));
  rb_define_const(gKeyClass,"WORLD_87",INT2NUM(SDLK_WORLD_87));
  rb_define_const(gKeyClass,"WORLD_88",INT2NUM(SDLK_WORLD_88));
  rb_define_const(gKeyClass,"WORLD_89",INT2NUM(SDLK_WORLD_89));
  rb_define_const(gKeyClass,"WORLD_90",INT2NUM(SDLK_WORLD_90));
  rb_define_const(gKeyClass,"WORLD_91",INT2NUM(SDLK_WORLD_91));
  rb_define_const(gKeyClass,"WORLD_92",INT2NUM(SDLK_WORLD_92));
  rb_define_const(gKeyClass,"WORLD_93",INT2NUM(SDLK_WORLD_93));
  rb_define_const(gKeyClass,"WORLD_94",INT2NUM(SDLK_WORLD_94));
  rb_define_const(gKeyClass,"WORLD_95",INT2NUM(SDLK_WORLD_95));


  /* Numeric keypad */
  rb_define_const(gKeyClass,"KP0",INT2NUM(SDLK_KP0));
  rb_define_const(gKeyClass,"KP1",INT2NUM(SDLK_KP1));
  rb_define_const(gKeyClass,"KP2",INT2NUM(SDLK_KP2));
  rb_define_const(gKeyClass,"KP3",INT2NUM(SDLK_KP3));
  rb_define_const(gKeyClass,"KP4",INT2NUM(SDLK_KP4));
  rb_define_const(gKeyClass,"KP5",INT2NUM(SDLK_KP5));
  rb_define_const(gKeyClass,"KP6",INT2NUM(SDLK_KP6));
  rb_define_const(gKeyClass,"KP7",INT2NUM(SDLK_KP7));
  rb_define_const(gKeyClass,"KP8",INT2NUM(SDLK_KP8));
  rb_define_const(gKeyClass,"KP9",INT2NUM(SDLK_KP9));
  rb_define_const(gKeyClass,"KP_PERIOD",INT2NUM(SDLK_KP_PERIOD));
  rb_define_const(gKeyClass,"KP_DIVIDE",INT2NUM(SDLK_KP_DIVIDE));
  rb_define_const(gKeyClass,"KP_MULTIPLY",INT2NUM(SDLK_KP_MULTIPLY));
  rb_define_const(gKeyClass,"KP_MINUS",INT2NUM(SDLK_KP_MINUS));
  rb_define_const(gKeyClass,"KP_PLUS",INT2NUM(SDLK_KP_PLUS));
  rb_define_const(gKeyClass,"KP_ENTER",INT2NUM(SDLK_KP_ENTER));
  rb_define_const(gKeyClass,"KP_EQUALS",INT2NUM(SDLK_KP_EQUALS));
  
  /* Arrows + Home/End pad */
  rb_define_const(gKeyClass,"UP",INT2NUM(SDLK_UP));
  rb_define_const(gKeyClass,"DOWN",INT2NUM(SDLK_DOWN));
  rb_define_const(gKeyClass,"RIGHT",INT2NUM(SDLK_RIGHT));
  rb_define_const(gKeyClass,"LEFT",INT2NUM(SDLK_LEFT));
  rb_define_const(gKeyClass,"INSERT",INT2NUM(SDLK_INSERT));
  rb_define_const(gKeyClass,"HOME",INT2NUM(SDLK_HOME));
  rb_define_const(gKeyClass,"END",INT2NUM(SDLK_END));
  rb_define_const(gKeyClass,"PAGEUP",INT2NUM(SDLK_PAGEUP));
  rb_define_const(gKeyClass,"PAGEDOWN",INT2NUM(SDLK_PAGEDOWN));

  /* Function keys */
  rb_define_const(gKeyClass,"F1",INT2NUM(SDLK_F1));
  rb_define_const(gKeyClass,"F2",INT2NUM(SDLK_F2));
  rb_define_const(gKeyClass,"F3",INT2NUM(SDLK_F3));
  rb_define_const(gKeyClass,"F4",INT2NUM(SDLK_F4));
  rb_define_const(gKeyClass,"F5",INT2NUM(SDLK_F5));
  rb_define_const(gKeyClass,"F6",INT2NUM(SDLK_F6));
  rb_define_const(gKeyClass,"F7",INT2NUM(SDLK_F7));
  rb_define_const(gKeyClass,"F8",INT2NUM(SDLK_F8));
  rb_define_const(gKeyClass,"F9",INT2NUM(SDLK_F9));
  rb_define_const(gKeyClass,"F10",INT2NUM(SDLK_F10));
  rb_define_const(gKeyClass,"F11",INT2NUM(SDLK_F11));
  rb_define_const(gKeyClass,"F12",INT2NUM(SDLK_F12));
  rb_define_const(gKeyClass,"F13",INT2NUM(SDLK_F13));
  rb_define_const(gKeyClass,"F14",INT2NUM(SDLK_F14));
  rb_define_const(gKeyClass,"F15",INT2NUM(SDLK_F15));
  
  /* Key state modifier keys */
  rb_define_const(gKeyClass,"NUMLOCK",INT2NUM(SDLK_NUMLOCK));
  rb_define_const(gKeyClass,"CAPSLOCK",INT2NUM(SDLK_CAPSLOCK));
  rb_define_const(gKeyClass,"SCROLLOCK",INT2NUM(SDLK_SCROLLOCK));
  rb_define_const(gKeyClass,"RSHIFT",INT2NUM(SDLK_RSHIFT));
  rb_define_const(gKeyClass,"LSHIFT",INT2NUM(SDLK_LSHIFT));
  rb_define_const(gKeyClass,"RCTRL",INT2NUM(SDLK_RCTRL));
  rb_define_const(gKeyClass,"LCTRL",INT2NUM(SDLK_LCTRL));
  rb_define_const(gKeyClass,"RALT",INT2NUM(SDLK_RALT));
  rb_define_const(gKeyClass,"LALT",INT2NUM(SDLK_LALT));
  rb_define_const(gKeyClass,"RMETA",INT2NUM(SDLK_RMETA));
  rb_define_const(gKeyClass,"LMETA",INT2NUM(SDLK_LMETA));
  rb_define_const(gKeyClass,"LSUPER",INT2NUM(SDLK_LSUPER));
  rb_define_const(gKeyClass,"RSUPER",INT2NUM(SDLK_RSUPER));
  rb_define_const(gKeyClass,"MODE",INT2NUM(SDLK_MODE));
  
  /* Miscellaneous function keys */
  rb_define_const(gKeyClass,"HELP",INT2NUM(SDLK_HELP));
  rb_define_const(gKeyClass,"PRINT",INT2NUM(SDLK_PRINT));
  rb_define_const(gKeyClass,"SYSREQ",INT2NUM(SDLK_SYSREQ));
  rb_define_const(gKeyClass,"BREAK",INT2NUM(SDLK_BREAK));
  rb_define_const(gKeyClass,"MENU",INT2NUM(SDLK_MENU));
  rb_define_const(gKeyClass,"POWER",INT2NUM(SDLK_POWER));
  rb_define_const(gKeyClass,"EURO",INT2NUM(SDLK_EURO));

  /* Add any other keys here */
  
  rb_define_const(gKeyClass,"LAST",INT2NUM(SDLK_LAST));
}

void Init_rubysdl()
{
  gSDLModule = rb_define_module("SDL");
  rb_define_module_function(gSDLModule,"init",sdl_init,1);
  rb_define_module_function(gSDLModule,"quit",sdl_quit,0);

  init_video();
  init_event();
  gKeyClass = rb_define_class_under(gSDLModule,"Key",rb_cObject);
  
  defineConst();
  defineConstForKey();
}
