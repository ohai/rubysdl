#include "rubysdl.h"

static Uint8 *keyState;

void defineConstForKey()
{
  rb_define_const(cKey,"UNKNOWN",INT2NUM(SDLK_UNKNOWN));
  rb_define_const(cKey,"FIRST",INT2NUM(SDLK_FIRST));
  rb_define_const(cKey,"BACKSPACE",INT2NUM(SDLK_BACKSPACE));
  rb_define_const(cKey,"TAB",INT2NUM(SDLK_TAB));
  rb_define_const(cKey,"CLEAR",INT2NUM(SDLK_CLEAR));
  rb_define_const(cKey,"RETURN",INT2NUM(SDLK_RETURN));
  rb_define_const(cKey,"PAUSE",INT2NUM(SDLK_PAUSE));
  rb_define_const(cKey,"ESCAPE",INT2NUM(SDLK_ESCAPE));
  rb_define_const(cKey,"SPACE",INT2NUM(SDLK_SPACE));
  rb_define_const(cKey,"EXCLAIM",INT2NUM(SDLK_EXCLAIM));
  rb_define_const(cKey,"QUOTEDBL",INT2NUM(SDLK_QUOTEDBL));
  rb_define_const(cKey,"HASH",INT2NUM(SDLK_HASH));
  rb_define_const(cKey,"DOLLAR",INT2NUM(SDLK_DOLLAR));
  rb_define_const(cKey,"AMPERSAND",INT2NUM(SDLK_AMPERSAND));
  rb_define_const(cKey,"QUOTE",INT2NUM(SDLK_QUOTE));
  rb_define_const(cKey,"LEFTPAREN",INT2NUM(SDLK_LEFTPAREN));
  rb_define_const(cKey,"RIGHTPAREN",INT2NUM(SDLK_RIGHTPAREN));
  rb_define_const(cKey,"ASTERISK",INT2NUM(SDLK_ASTERISK));
  rb_define_const(cKey,"PLUS",INT2NUM(SDLK_PLUS));
  rb_define_const(cKey,"COMMA",INT2NUM(SDLK_COMMA));
  rb_define_const(cKey,"MINUS",INT2NUM(SDLK_MINUS));
  rb_define_const(cKey,"PERIOD",INT2NUM(SDLK_PERIOD));
  rb_define_const(cKey,"SLASH",INT2NUM(SDLK_SLASH));
  rb_define_const(cKey,"K0",INT2NUM(SDLK_0));
  rb_define_const(cKey,"K1",INT2NUM(SDLK_1));
  rb_define_const(cKey,"K2",INT2NUM(SDLK_2));
  rb_define_const(cKey,"K3",INT2NUM(SDLK_3));
  rb_define_const(cKey,"K4",INT2NUM(SDLK_4));
  rb_define_const(cKey,"K5",INT2NUM(SDLK_5));
  rb_define_const(cKey,"K6",INT2NUM(SDLK_6));
  rb_define_const(cKey,"K7",INT2NUM(SDLK_7));
  rb_define_const(cKey,"K8",INT2NUM(SDLK_8));
  rb_define_const(cKey,"K9",INT2NUM(SDLK_9));
  rb_define_const(cKey,"COLON",INT2NUM(SDLK_COLON));
  rb_define_const(cKey,"SEMICOLON",INT2NUM(SDLK_SEMICOLON));
  rb_define_const(cKey,"LESS",INT2NUM(SDLK_LESS));
  rb_define_const(cKey,"EQUALS",INT2NUM(SDLK_EQUALS));
  rb_define_const(cKey,"GREATER",INT2NUM(SDLK_GREATER));
  rb_define_const(cKey,"QUESTION",INT2NUM(SDLK_QUESTION));
  rb_define_const(cKey,"AT",INT2NUM(SDLK_AT));
  /* 
     Skip uppercase letters
     */
  rb_define_const(cKey,"LEFTBRACKET",INT2NUM(SDLK_LEFTBRACKET));
  rb_define_const(cKey,"BACKSLASH",INT2NUM(SDLK_BACKSLASH));
  rb_define_const(cKey,"RIGHTBRACKET",INT2NUM(SDLK_RIGHTBRACKET));
  rb_define_const(cKey,"CARET",INT2NUM(SDLK_CARET));
  rb_define_const(cKey,"UNDERSCORE",INT2NUM(SDLK_UNDERSCORE));
  rb_define_const(cKey,"BACKQUOTE",INT2NUM(SDLK_BACKQUOTE));
  rb_define_const(cKey,"A",INT2NUM(SDLK_a));
  rb_define_const(cKey,"B",INT2NUM(SDLK_b));
  rb_define_const(cKey,"C",INT2NUM(SDLK_c));
  rb_define_const(cKey,"D",INT2NUM(SDLK_d));
  rb_define_const(cKey,"E",INT2NUM(SDLK_e));
  rb_define_const(cKey,"F",INT2NUM(SDLK_f));
  rb_define_const(cKey,"G",INT2NUM(SDLK_g));
  rb_define_const(cKey,"H",INT2NUM(SDLK_h));
  rb_define_const(cKey,"I",INT2NUM(SDLK_i));
  rb_define_const(cKey,"J",INT2NUM(SDLK_j));
  rb_define_const(cKey,"K",INT2NUM(SDLK_k));
  rb_define_const(cKey,"L",INT2NUM(SDLK_l));
  rb_define_const(cKey,"M",INT2NUM(SDLK_m));
  rb_define_const(cKey,"N",INT2NUM(SDLK_n));
  rb_define_const(cKey,"O",INT2NUM(SDLK_o));
  rb_define_const(cKey,"P",INT2NUM(SDLK_p));
  rb_define_const(cKey,"Q",INT2NUM(SDLK_q));
  rb_define_const(cKey,"R",INT2NUM(SDLK_r));
  rb_define_const(cKey,"S",INT2NUM(SDLK_s));
  rb_define_const(cKey,"T",INT2NUM(SDLK_t));
  rb_define_const(cKey,"U",INT2NUM(SDLK_u));
  rb_define_const(cKey,"V",INT2NUM(SDLK_v));
  rb_define_const(cKey,"W",INT2NUM(SDLK_w));
  rb_define_const(cKey,"X",INT2NUM(SDLK_x));
  rb_define_const(cKey,"Y",INT2NUM(SDLK_y));
  rb_define_const(cKey,"Z",INT2NUM(SDLK_z));
  rb_define_const(cKey,"DELETE",INT2NUM(SDLK_DELETE));
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
  rb_define_const(cKey,"WORLD_0",INT2NUM(SDLK_WORLD_0));
  rb_define_const(cKey,"WORLD_1",INT2NUM(SDLK_WORLD_1));
  rb_define_const(cKey,"WORLD_2",INT2NUM(SDLK_WORLD_2));
  rb_define_const(cKey,"WORLD_3",INT2NUM(SDLK_WORLD_3));
  rb_define_const(cKey,"WORLD_4",INT2NUM(SDLK_WORLD_4));
  rb_define_const(cKey,"WORLD_5",INT2NUM(SDLK_WORLD_5));
  rb_define_const(cKey,"WORLD_6",INT2NUM(SDLK_WORLD_6));
  rb_define_const(cKey,"WORLD_7",INT2NUM(SDLK_WORLD_7));
  rb_define_const(cKey,"WORLD_8",INT2NUM(SDLK_WORLD_8));
  rb_define_const(cKey,"WORLD_9",INT2NUM(SDLK_WORLD_9));
  rb_define_const(cKey,"WORLD_10",INT2NUM(SDLK_WORLD_10));
  rb_define_const(cKey,"WORLD_11",INT2NUM(SDLK_WORLD_11));
  rb_define_const(cKey,"WORLD_12",INT2NUM(SDLK_WORLD_12));
  rb_define_const(cKey,"WORLD_13",INT2NUM(SDLK_WORLD_13));
  rb_define_const(cKey,"WORLD_14",INT2NUM(SDLK_WORLD_14));
  rb_define_const(cKey,"WORLD_15",INT2NUM(SDLK_WORLD_15));
  rb_define_const(cKey,"WORLD_16",INT2NUM(SDLK_WORLD_16));
  rb_define_const(cKey,"WORLD_17",INT2NUM(SDLK_WORLD_17));
  rb_define_const(cKey,"WORLD_18",INT2NUM(SDLK_WORLD_18));
  rb_define_const(cKey,"WORLD_19",INT2NUM(SDLK_WORLD_19));
  rb_define_const(cKey,"WORLD_20",INT2NUM(SDLK_WORLD_20));
  rb_define_const(cKey,"WORLD_21",INT2NUM(SDLK_WORLD_21));
  rb_define_const(cKey,"WORLD_22",INT2NUM(SDLK_WORLD_22));
  rb_define_const(cKey,"WORLD_23",INT2NUM(SDLK_WORLD_23));
  rb_define_const(cKey,"WORLD_24",INT2NUM(SDLK_WORLD_24));
  rb_define_const(cKey,"WORLD_25",INT2NUM(SDLK_WORLD_25));
  rb_define_const(cKey,"WORLD_26",INT2NUM(SDLK_WORLD_26));
  rb_define_const(cKey,"WORLD_27",INT2NUM(SDLK_WORLD_27));
  rb_define_const(cKey,"WORLD_28",INT2NUM(SDLK_WORLD_28));
  rb_define_const(cKey,"WORLD_29",INT2NUM(SDLK_WORLD_29));
  rb_define_const(cKey,"WORLD_30",INT2NUM(SDLK_WORLD_30));
  rb_define_const(cKey,"WORLD_31",INT2NUM(SDLK_WORLD_31));
  rb_define_const(cKey,"WORLD_32",INT2NUM(SDLK_WORLD_32));
  rb_define_const(cKey,"WORLD_33",INT2NUM(SDLK_WORLD_33));
  rb_define_const(cKey,"WORLD_34",INT2NUM(SDLK_WORLD_34));
  rb_define_const(cKey,"WORLD_35",INT2NUM(SDLK_WORLD_35));
  rb_define_const(cKey,"WORLD_36",INT2NUM(SDLK_WORLD_36));
  rb_define_const(cKey,"WORLD_37",INT2NUM(SDLK_WORLD_37));
  rb_define_const(cKey,"WORLD_38",INT2NUM(SDLK_WORLD_38));
  rb_define_const(cKey,"WORLD_39",INT2NUM(SDLK_WORLD_39));
  rb_define_const(cKey,"WORLD_40",INT2NUM(SDLK_WORLD_40));
  rb_define_const(cKey,"WORLD_41",INT2NUM(SDLK_WORLD_41));
  rb_define_const(cKey,"WORLD_42",INT2NUM(SDLK_WORLD_42));
  rb_define_const(cKey,"WORLD_43",INT2NUM(SDLK_WORLD_43));
  rb_define_const(cKey,"WORLD_44",INT2NUM(SDLK_WORLD_44));
  rb_define_const(cKey,"WORLD_45",INT2NUM(SDLK_WORLD_45));
  rb_define_const(cKey,"WORLD_46",INT2NUM(SDLK_WORLD_46));
  rb_define_const(cKey,"WORLD_47",INT2NUM(SDLK_WORLD_47));
  rb_define_const(cKey,"WORLD_48",INT2NUM(SDLK_WORLD_48));
  rb_define_const(cKey,"WORLD_49",INT2NUM(SDLK_WORLD_49));
  rb_define_const(cKey,"WORLD_50",INT2NUM(SDLK_WORLD_50));
  rb_define_const(cKey,"WORLD_51",INT2NUM(SDLK_WORLD_51));
  rb_define_const(cKey,"WORLD_52",INT2NUM(SDLK_WORLD_52));
  rb_define_const(cKey,"WORLD_53",INT2NUM(SDLK_WORLD_53));
  rb_define_const(cKey,"WORLD_54",INT2NUM(SDLK_WORLD_54));
  rb_define_const(cKey,"WORLD_55",INT2NUM(SDLK_WORLD_55));
  rb_define_const(cKey,"WORLD_56",INT2NUM(SDLK_WORLD_56));
  rb_define_const(cKey,"WORLD_57",INT2NUM(SDLK_WORLD_57));
  rb_define_const(cKey,"WORLD_58",INT2NUM(SDLK_WORLD_58));
  rb_define_const(cKey,"WORLD_59",INT2NUM(SDLK_WORLD_59));
  rb_define_const(cKey,"WORLD_60",INT2NUM(SDLK_WORLD_60));
  rb_define_const(cKey,"WORLD_61",INT2NUM(SDLK_WORLD_61));
  rb_define_const(cKey,"WORLD_62",INT2NUM(SDLK_WORLD_62));
  rb_define_const(cKey,"WORLD_63",INT2NUM(SDLK_WORLD_63));
  rb_define_const(cKey,"WORLD_64",INT2NUM(SDLK_WORLD_64));
  rb_define_const(cKey,"WORLD_65",INT2NUM(SDLK_WORLD_65));
  rb_define_const(cKey,"WORLD_66",INT2NUM(SDLK_WORLD_66));
  rb_define_const(cKey,"WORLD_67",INT2NUM(SDLK_WORLD_67));
  rb_define_const(cKey,"WORLD_68",INT2NUM(SDLK_WORLD_68));
  rb_define_const(cKey,"WORLD_69",INT2NUM(SDLK_WORLD_69));
  rb_define_const(cKey,"WORLD_70",INT2NUM(SDLK_WORLD_70));
  rb_define_const(cKey,"WORLD_71",INT2NUM(SDLK_WORLD_71));
  rb_define_const(cKey,"WORLD_72",INT2NUM(SDLK_WORLD_72));
  rb_define_const(cKey,"WORLD_73",INT2NUM(SDLK_WORLD_73));
  rb_define_const(cKey,"WORLD_74",INT2NUM(SDLK_WORLD_74));
  rb_define_const(cKey,"WORLD_75",INT2NUM(SDLK_WORLD_75));
  rb_define_const(cKey,"WORLD_76",INT2NUM(SDLK_WORLD_76));
  rb_define_const(cKey,"WORLD_77",INT2NUM(SDLK_WORLD_77));
  rb_define_const(cKey,"WORLD_78",INT2NUM(SDLK_WORLD_78));
  rb_define_const(cKey,"WORLD_79",INT2NUM(SDLK_WORLD_79));
  rb_define_const(cKey,"WORLD_80",INT2NUM(SDLK_WORLD_80));
  rb_define_const(cKey,"WORLD_81",INT2NUM(SDLK_WORLD_81));
  rb_define_const(cKey,"WORLD_82",INT2NUM(SDLK_WORLD_82));
  rb_define_const(cKey,"WORLD_83",INT2NUM(SDLK_WORLD_83));
  rb_define_const(cKey,"WORLD_84",INT2NUM(SDLK_WORLD_84));
  rb_define_const(cKey,"WORLD_85",INT2NUM(SDLK_WORLD_85));
  rb_define_const(cKey,"WORLD_86",INT2NUM(SDLK_WORLD_86));
  rb_define_const(cKey,"WORLD_87",INT2NUM(SDLK_WORLD_87));
  rb_define_const(cKey,"WORLD_88",INT2NUM(SDLK_WORLD_88));
  rb_define_const(cKey,"WORLD_89",INT2NUM(SDLK_WORLD_89));
  rb_define_const(cKey,"WORLD_90",INT2NUM(SDLK_WORLD_90));
  rb_define_const(cKey,"WORLD_91",INT2NUM(SDLK_WORLD_91));
  rb_define_const(cKey,"WORLD_92",INT2NUM(SDLK_WORLD_92));
  rb_define_const(cKey,"WORLD_93",INT2NUM(SDLK_WORLD_93));
  rb_define_const(cKey,"WORLD_94",INT2NUM(SDLK_WORLD_94));
  rb_define_const(cKey,"WORLD_95",INT2NUM(SDLK_WORLD_95));


  /* Numeric keypad */
  rb_define_const(cKey,"KP0",INT2NUM(SDLK_KP0));
  rb_define_const(cKey,"KP1",INT2NUM(SDLK_KP1));
  rb_define_const(cKey,"KP2",INT2NUM(SDLK_KP2));
  rb_define_const(cKey,"KP3",INT2NUM(SDLK_KP3));
  rb_define_const(cKey,"KP4",INT2NUM(SDLK_KP4));
  rb_define_const(cKey,"KP5",INT2NUM(SDLK_KP5));
  rb_define_const(cKey,"KP6",INT2NUM(SDLK_KP6));
  rb_define_const(cKey,"KP7",INT2NUM(SDLK_KP7));
  rb_define_const(cKey,"KP8",INT2NUM(SDLK_KP8));
  rb_define_const(cKey,"KP9",INT2NUM(SDLK_KP9));
  rb_define_const(cKey,"KP_PERIOD",INT2NUM(SDLK_KP_PERIOD));
  rb_define_const(cKey,"KP_DIVIDE",INT2NUM(SDLK_KP_DIVIDE));
  rb_define_const(cKey,"KP_MULTIPLY",INT2NUM(SDLK_KP_MULTIPLY));
  rb_define_const(cKey,"KP_MINUS",INT2NUM(SDLK_KP_MINUS));
  rb_define_const(cKey,"KP_PLUS",INT2NUM(SDLK_KP_PLUS));
  rb_define_const(cKey,"KP_ENTER",INT2NUM(SDLK_KP_ENTER));
  rb_define_const(cKey,"KP_EQUALS",INT2NUM(SDLK_KP_EQUALS));
  
  /* Arrows + Home/End pad */
  rb_define_const(cKey,"UP",INT2NUM(SDLK_UP));
  rb_define_const(cKey,"DOWN",INT2NUM(SDLK_DOWN));
  rb_define_const(cKey,"RIGHT",INT2NUM(SDLK_RIGHT));
  rb_define_const(cKey,"LEFT",INT2NUM(SDLK_LEFT));
  rb_define_const(cKey,"INSERT",INT2NUM(SDLK_INSERT));
  rb_define_const(cKey,"HOME",INT2NUM(SDLK_HOME));
  rb_define_const(cKey,"END",INT2NUM(SDLK_END));
  rb_define_const(cKey,"PAGEUP",INT2NUM(SDLK_PAGEUP));
  rb_define_const(cKey,"PAGEDOWN",INT2NUM(SDLK_PAGEDOWN));

  /* Function keys */
  rb_define_const(cKey,"F1",INT2NUM(SDLK_F1));
  rb_define_const(cKey,"F2",INT2NUM(SDLK_F2));
  rb_define_const(cKey,"F3",INT2NUM(SDLK_F3));
  rb_define_const(cKey,"F4",INT2NUM(SDLK_F4));
  rb_define_const(cKey,"F5",INT2NUM(SDLK_F5));
  rb_define_const(cKey,"F6",INT2NUM(SDLK_F6));
  rb_define_const(cKey,"F7",INT2NUM(SDLK_F7));
  rb_define_const(cKey,"F8",INT2NUM(SDLK_F8));
  rb_define_const(cKey,"F9",INT2NUM(SDLK_F9));
  rb_define_const(cKey,"F10",INT2NUM(SDLK_F10));
  rb_define_const(cKey,"F11",INT2NUM(SDLK_F11));
  rb_define_const(cKey,"F12",INT2NUM(SDLK_F12));
  rb_define_const(cKey,"F13",INT2NUM(SDLK_F13));
  rb_define_const(cKey,"F14",INT2NUM(SDLK_F14));
  rb_define_const(cKey,"F15",INT2NUM(SDLK_F15));
  
  /* Key state modifier keys */
  rb_define_const(cKey,"NUMLOCK",INT2NUM(SDLK_NUMLOCK));
  rb_define_const(cKey,"CAPSLOCK",INT2NUM(SDLK_CAPSLOCK));
  rb_define_const(cKey,"SCROLLOCK",INT2NUM(SDLK_SCROLLOCK));
  rb_define_const(cKey,"RSHIFT",INT2NUM(SDLK_RSHIFT));
  rb_define_const(cKey,"LSHIFT",INT2NUM(SDLK_LSHIFT));
  rb_define_const(cKey,"RCTRL",INT2NUM(SDLK_RCTRL));
  rb_define_const(cKey,"LCTRL",INT2NUM(SDLK_LCTRL));
  rb_define_const(cKey,"RALT",INT2NUM(SDLK_RALT));
  rb_define_const(cKey,"LALT",INT2NUM(SDLK_LALT));
  rb_define_const(cKey,"RMETA",INT2NUM(SDLK_RMETA));
  rb_define_const(cKey,"LMETA",INT2NUM(SDLK_LMETA));
  rb_define_const(cKey,"LSUPER",INT2NUM(SDLK_LSUPER));
  rb_define_const(cKey,"RSUPER",INT2NUM(SDLK_RSUPER));
  rb_define_const(cKey,"MODE",INT2NUM(SDLK_MODE));
  
  /* Miscellaneous function keys */
  rb_define_const(cKey,"HELP",INT2NUM(SDLK_HELP));
  rb_define_const(cKey,"PRINT",INT2NUM(SDLK_PRINT));
  rb_define_const(cKey,"SYSREQ",INT2NUM(SDLK_SYSREQ));
  rb_define_const(cKey,"BREAK",INT2NUM(SDLK_BREAK));
  rb_define_const(cKey,"MENU",INT2NUM(SDLK_MENU));
  rb_define_const(cKey,"POWER",INT2NUM(SDLK_POWER));
  rb_define_const(cKey,"EURO",INT2NUM(SDLK_EURO));

  /* Add any other keys here */
  
  rb_define_const(cKey,"LAST",INT2NUM(SDLK_LAST));

  /* key mods */
  rb_define_const(cKey,"MOD_NONE",UINT2NUM(KMOD_NONE));
  rb_define_const(cKey,"MOD_LSHIFT",UINT2NUM(KMOD_LSHIFT));
  rb_define_const(cKey,"MOD_RSHIFT",UINT2NUM(KMOD_RSHIFT));
  rb_define_const(cKey,"MOD_LCTRL",UINT2NUM(KMOD_LCTRL));
  rb_define_const(cKey,"MOD_RCTRL",UINT2NUM(KMOD_RCTRL));
  rb_define_const(cKey,"MOD_LALT",UINT2NUM(KMOD_LALT));
  rb_define_const(cKey,"MOD_RALT",UINT2NUM(KMOD_RALT));
  rb_define_const(cKey,"MOD_LMETA",UINT2NUM(KMOD_LMETA));
  rb_define_const(cKey,"MOD_RMETA",UINT2NUM(KMOD_RMETA));
  rb_define_const(cKey,"MOD_NUM",UINT2NUM(KMOD_NUM));
  rb_define_const(cKey,"MOD_CAPS",UINT2NUM(KMOD_CAPS));
  rb_define_const(cKey,"MOD_MODE",UINT2NUM(KMOD_MODE));
  rb_define_const(cKey,"MOD_RESERVED",UINT2NUM(KMOD_RESERVED));

  rb_define_const(cKey,"MOD_CTRL",UINT2NUM(KMOD_CTRL));
  rb_define_const(cKey,"MOD_SHIFT",UINT2NUM(KMOD_SHIFT));
  rb_define_const(cKey,"MOD_ALT",UINT2NUM(KMOD_ALT));
  rb_define_const(cKey,"MOD_META",UINT2NUM(KMOD_META));
}

/* under 2 function is for testing */
static VALUE sdl_keyScan(VALUE class)
{
  keyState=SDL_GetKeyState(NULL);
  return Qnil;
}
static VALUE sdl_keyState(VALUE class,VALUE keysym)
{
  return INT2NUM(keyState[NUM2INT(keysym)]);
}
void init_keyEvent()
{
  cKey = rb_define_class_under(mSDL,"Key",rb_cObject);
  rb_define_singleton_method(cKey,"scan",sdl_keyScan,0);
  rb_define_singleton_method(cKey,"state",sdl_keyState,1);
  defineConstForKey();
}
