#ifndef ZUMAHD_HQC_KEY_H
#define ZUMAHD_HQC_KEY_H

/* SDL scancode wrapper */
typedef enum HQC_Key
{
    HQC_KEY_UNKNOWN = 0,

    HQC_KEY_A = 4,
    HQC_KEY_B = 5,
    HQC_KEY_C = 6,
    HQC_KEY_D = 7,
    HQC_KEY_E = 8,
    HQC_KEY_F = 9,
    HQC_KEY_G = 10,
    HQC_KEY_H = 11,
    HQC_KEY_I = 12,
    HQC_KEY_J = 13,
    HQC_KEY_K = 14,
    HQC_KEY_L = 15,
    HQC_KEY_M = 16,
    HQC_KEY_N = 17,
    HQC_KEY_O = 18,
    HQC_KEY_P = 19,
    HQC_KEY_Q = 20,
    HQC_KEY_R = 21,
    HQC_KEY_S = 22,
    HQC_KEY_T = 23,
    HQC_KEY_U = 24,
    HQC_KEY_V = 25,
    HQC_KEY_W = 26,
    HQC_KEY_X = 27,
    HQC_KEY_Y = 28,
    HQC_KEY_Z = 29,

    HQC_KEY_1 = 30,
    HQC_KEY_2 = 31,
    HQC_KEY_3 = 32,
    HQC_KEY_4 = 33,
    HQC_KEY_5 = 34,
    HQC_KEY_6 = 35,
    HQC_KEY_7 = 36,
    HQC_KEY_8 = 37,
    HQC_KEY_9 = 38,
    HQC_KEY_0 = 39,

    HQC_KEY_RETURN = 40,
    HQC_KEY_ESCAPE = 41,
    HQC_KEY_BACKSPACE = 42,
    HQC_KEY_TAB = 43,
    HQC_KEY_SPACE = 44,

    HQC_KEY_MINUS = 45,
    HQC_KEY_EQUALS = 46,
    HQC_KEY_LEFTBRACKET = 47,
    HQC_KEY_RIGHTBRACKET = 48,
    HQC_KEY_BACKSLASH = 49,
    HQC_KEY_NONUSHASH = 50,
    HQC_KEY_SEMICOLON = 51,
    HQC_KEY_APOSTROPHE = 52,
    HQC_KEY_GRAVE = 53,
    HQC_KEY_COMMA = 54,
    HQC_KEY_PERIOD = 55,
    HQC_KEY_SLASH = 56,

    HQC_KEY_CAPSLOCK = 57,

    HQC_KEY_F1 = 58,
    HQC_KEY_F2 = 59,
    HQC_KEY_F3 = 60,
    HQC_KEY_F4 = 61,
    HQC_KEY_F5 = 62,
    HQC_KEY_F6 = 63,
    HQC_KEY_F7 = 64,
    HQC_KEY_F8 = 65,
    HQC_KEY_F9 = 66,
    HQC_KEY_F10 = 67,
    HQC_KEY_F11 = 68,
    HQC_KEY_F12 = 69,

    HQC_KEY_PRINTSCREEN = 70,
    HQC_KEY_SCROLLLOCK = 71,
    HQC_KEY_PAUSE = 72,
    HQC_KEY_INSERT = 73,
    HQC_KEY_HOME = 74,
    HQC_KEY_PAGEUP = 75,
    HQC_KEY_DELETE = 76,
    HQC_KEY_END = 77,
    HQC_KEY_PAGEDOWN = 78,
    HQC_KEY_RIGHT = 79,
    HQC_KEY_LEFT = 80,
    HQC_KEY_DOWN = 81,
    HQC_KEY_UP = 82,

    HQC_KEY_NUMLOCKCLEAR = 83,

    HQC_KEY_KP_DIVIDE = 84,
    HQC_KEY_KP_MULTIPLY = 85,
    HQC_KEY_KP_MINUS = 86,
    HQC_KEY_KP_PLUS = 87,
    HQC_KEY_KP_ENTER = 88,
    HQC_KEY_KP_1 = 89,
    HQC_KEY_KP_2 = 90,
    HQC_KEY_KP_3 = 91,
    HQC_KEY_KP_4 = 92,
    HQC_KEY_KP_5 = 93,
    HQC_KEY_KP_6 = 94,
    HQC_KEY_KP_7 = 95,
    HQC_KEY_KP_8 = 96,
    HQC_KEY_KP_9 = 97,
    HQC_KEY_KP_0 = 98,
    HQC_KEY_KP_PERIOD = 99,

    HQC_KEY_NONUSBACKSLASH = 100,
    HQC_KEY_APPLICATION = 101,
    HQC_KEY_KP_EQUALS = 103,
    HQC_KEY_F13 = 104,
    HQC_KEY_F14 = 105,
    HQC_KEY_F15 = 106,
    HQC_KEY_F16 = 107,
    HQC_KEY_F17 = 108,
    HQC_KEY_F18 = 109,
    HQC_KEY_F19 = 110,
    HQC_KEY_F20 = 111,
    HQC_KEY_F21 = 112,
    HQC_KEY_F22 = 113,
    HQC_KEY_F23 = 114,
    HQC_KEY_F24 = 115,
    HQC_KEY_EXECUTE = 116,
    HQC_KEY_HELP = 117,
    HQC_KEY_MENU = 118,
    HQC_KEY_SELECT = 119,
    HQC_KEY_STOP = 120,
    HQC_KEY_AGAIN = 121,
    HQC_KEY_UNDO = 122,
    HQC_KEY_CUT = 123,
    HQC_KEY_COPY = 124,
    HQC_KEY_PASTE = 125,
    HQC_KEY_FIND = 126,
    HQC_KEY_MUTE = 127,
    HQC_KEY_VOLUMEUP = 128,
    HQC_KEY_VOLUMEDOWN = 129,

    HQC_KEY_KP_COMMA = 133,
    HQC_KEY_KP_EQUALSAS400 = 134,

    HQC_KEY_INTERNATIONAL1 = 135,
    HQC_KEY_INTERNATIONAL2 = 136,
    HQC_KEY_INTERNATIONAL3 = 137,
    HQC_KEY_INTERNATIONAL4 = 138,
    HQC_KEY_INTERNATIONAL5 = 139,
    HQC_KEY_INTERNATIONAL6 = 140,
    HQC_KEY_INTERNATIONAL7 = 141,
    HQC_KEY_INTERNATIONAL8 = 142,
    HQC_KEY_INTERNATIONAL9 = 143,
    HQC_KEY_LANG1 = 144, /**< Hangul/English toggle */
    HQC_KEY_LANG2 = 145, /**< Hanja conversion */
    HQC_KEY_LANG3 = 146, /**< Katakana */
    HQC_KEY_LANG4 = 147, /**< Hiragana */
    HQC_KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
    HQC_KEY_LANG6 = 149, /**< reserved */
    HQC_KEY_LANG7 = 150, /**< reserved */
    HQC_KEY_LANG8 = 151, /**< reserved */
    HQC_KEY_LANG9 = 152, /**< reserved */

    HQC_KEY_ALTERASE = 153,
    HQC_KEY_SYSREQ = 154,
    HQC_KEY_CANCEL = 155,
    HQC_KEY_CLEAR = 156,
    HQC_KEY_PRIOR = 157,
    HQC_KEY_RETURN2 = 158,
    HQC_KEY_SEPARATOR = 159,
    HQC_KEY_OUT = 160,
    HQC_KEY_OPER = 161,
    HQC_KEY_CLEARAGAIN = 162,
    HQC_KEY_CRSEL = 163,
    HQC_KEY_EXSEL = 164,

    HQC_KEY_KP_00 = 176,
    HQC_KEY_KP_000 = 177,
    HQC_KEY_THOUSANDSSEPARATOR = 178,
    HQC_KEY_DECIMALSEPARATOR = 179,
    HQC_KEY_CURRENCYUNIT = 180,
    HQC_KEY_CURRENCYSUBUNIT = 181,
    HQC_KEY_KP_LEFTPAREN = 182,
    HQC_KEY_KP_RIGHTPAREN = 183,
    HQC_KEY_KP_LEFTBRACE = 184,
    HQC_KEY_KP_RIGHTBRACE = 185,
    HQC_KEY_KP_TAB = 186,
    HQC_KEY_KP_BACKSPACE = 187,
    HQC_KEY_KP_A = 188,
    HQC_KEY_KP_B = 189,
    HQC_KEY_KP_C = 190,
    HQC_KEY_KP_D = 191,
    HQC_KEY_KP_E = 192,
    HQC_KEY_KP_F = 193,
    HQC_KEY_KP_XOR = 194,
    HQC_KEY_KP_POWER = 195,
    HQC_KEY_KP_PERCENT = 196,
    HQC_KEY_KP_LESS = 197,
    HQC_KEY_KP_GREATER = 198,
    HQC_KEY_KP_AMPERSAND = 199,
    HQC_KEY_KP_DBLAMPERSAND = 200,
    HQC_KEY_KP_VERTICALBAR = 201,
    HQC_KEY_KP_DBLVERTICALBAR = 202,
    HQC_KEY_KP_COLON = 203,
    HQC_KEY_KP_HASH = 204,
    HQC_KEY_KP_SPACE = 205,
    HQC_KEY_KP_AT = 206,
    HQC_KEY_KP_EXCLAM = 207,
    HQC_KEY_KP_MEMSTORE = 208,
    HQC_KEY_KP_MEMRECALL = 209,
    HQC_KEY_KP_MEMCLEAR = 210,
    HQC_KEY_KP_MEMADD = 211,
    HQC_KEY_KP_MEMSUBTRACT = 212,
    HQC_KEY_KP_MEMMULTIPLY = 213,
    HQC_KEY_KP_MEMDIVIDE = 214,
    HQC_KEY_KP_PLUSMINUS = 215,
    HQC_KEY_KP_CLEAR = 216,
    HQC_KEY_KP_CLEARENTRY = 217,
    HQC_KEY_KP_BINARY = 218,
    HQC_KEY_KP_OCTAL = 219,
    HQC_KEY_KP_DECIMAL = 220,
    HQC_KEY_KP_HEXADECIMAL = 221,

    HQC_KEY_LCTRL = 224,
    HQC_KEY_LSHIFT = 225,
    HQC_KEY_LALT = 226, /**< alt, option */
    HQC_KEY_LGUI = 227, /**< windows, command (apple), meta */
    HQC_KEY_RCTRL = 228,
    HQC_KEY_RSHIFT = 229,
    HQC_KEY_RALT = 230, /**< alt gr, option */
    HQC_KEY_RGUI = 231, /**< windows, command (apple), meta */

    HQC_KEY_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *  See https://usb.org/sites/default/files/hut1_2.pdf
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    HQC_KEY_AUDIONEXT = 258,
    HQC_KEY_AUDIOPREV = 259,
    HQC_KEY_AUDIOSTOP = 260,
    HQC_KEY_AUDIOPLAY = 261,
    HQC_KEY_AUDIOMUTE = 262,
    HQC_KEY_MEDIASELECT = 263,
    HQC_KEY_WWW = 264,             /**< AL Internet Browser */
    HQC_KEY_MAIL = 265,
    HQC_KEY_CALCULATOR = 266,      /**< AL Calculator */
    HQC_KEY_COMPUTER = 267,
    HQC_KEY_AC_SEARCH = 268,       /**< AC Search */
    HQC_KEY_AC_HOME = 269,         /**< AC Home */
    HQC_KEY_AC_BACK = 270,         /**< AC Back */
    HQC_KEY_AC_FORWARD = 271,      /**< AC Forward */
    HQC_KEY_AC_STOP = 272,         /**< AC Stop */
    HQC_KEY_AC_REFRESH = 273,      /**< AC Refresh */
    HQC_KEY_AC_BOOKMARKS = 274,    /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    HQC_KEY_BRIGHTNESSDOWN = 275,
    HQC_KEY_BRIGHTNESSUP = 276,
    HQC_KEY_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    HQC_KEY_KBDILLUMTOGGLE = 278,
    HQC_KEY_KBDILLUMDOWN = 279,
    HQC_KEY_KBDILLUMUP = 280,
    HQC_KEY_EJECT = 281,
    HQC_KEY_SLEEP = 282,           /**< SC System Sleep */

    HQC_KEY_APP1 = 283,
    HQC_KEY_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    HQC_KEY_AUDIOREWIND = 285,
    HQC_KEY_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    HQC_KEY_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    HQC_KEY_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    HQC_KEY_CALL = 289, /**< Used for accepting phone calls. */
    HQC_KEY_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    HQC_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
} HQC_Key;

#endif //ZUMAHD_HQC_KEY_H
