// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Copyright (c) 2021 HRA! (t.hara)
//	
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
// --------------------------------------------------------------------

// --------------------------------------------------------------------
//	keymap
//		keymap[ USBキーコード ] = { y_code, x_code }
//			y_code:
//				MSXキーマトリクスの行番号 0～15。
//				-1 にすると、そのキーは無視される。
//			x_code: 
//				MSXキーマトリクスのビット番号 0～7。
//
//		keymap[ USB key code ] = { y_code, x_code }
//			y_code:
//				MSX key matrix line number 0 to 15.
//				If set to -1, the key will be ignored.
//			x_code: 
//				Bit numbers 0 to 7 of the MSX key matrix.
//
static const KEYMAP_T keymap_2nd[] = {
	{ -1, 0 },		//	HID_KEY_NONE               0x00
	{ -1, 0 },		//	                           0x01
	{ -1, 0 },		//	                           0x02
	{ -1, 0 },		//	                           0x03
	{  2, 6 },		//	HID_KEY_A                  0x04
	{  2, 7 },		//	HID_KEY_B                  0x05
	{  3, 0 },		//	HID_KEY_C                  0x06
	{  3, 1 },		//	HID_KEY_D                  0x07
	{  3, 2 },		//	HID_KEY_E                  0x08
	{  3, 3 },		//	HID_KEY_F                  0x09
	{  3, 4 },		//	HID_KEY_G                  0x0A
	{  3, 5 },		//	HID_KEY_H                  0x0B
	{  3, 6 },		//	HID_KEY_I                  0x0C
	{  3, 7 },		//	HID_KEY_J                  0x0D
	{  4, 0 },		//	HID_KEY_K                  0x0E
	{  4, 1 },		//	HID_KEY_L                  0x0F
	{  4, 2 },		//	HID_KEY_M                  0x10
	{  4, 3 },		//	HID_KEY_N                  0x11
	{  4, 4 },		//	HID_KEY_O                  0x12
	{  4, 5 },		//	HID_KEY_P                  0x13
	{  4, 6 },		//	HID_KEY_Q                  0x14
	{  4, 7 },		//	HID_KEY_R                  0x15
	{  5, 0 },		//	HID_KEY_S                  0x16
	{  5, 1 },		//	HID_KEY_T                  0x17
	{  5, 2 },		//	HID_KEY_U                  0x18
	{  5, 3 },		//	HID_KEY_V                  0x19
	{  5, 4 },		//	HID_KEY_W                  0x1A
	{  5, 5 },		//	HID_KEY_X                  0x1B
	{  5, 6 },		//	HID_KEY_Y                  0x1C
	{  5, 7 },		//	HID_KEY_Z                  0x1D
	{  0, 1 },		//	HID_KEY_1                  0x1E
	{  0, 2 },		//	HID_KEY_2                  0x1F
	{  0, 3 },		//	HID_KEY_3                  0x20
	{  0, 4 },		//	HID_KEY_4                  0x21
	{  0, 5 },		//	HID_KEY_5                  0x22
	{  0, 6 },		//	HID_KEY_6                  0x23
	{  0, 7 },		//	HID_KEY_7                  0x24
	{  1, 0 },		//	HID_KEY_8                  0x25
	{  1, 1 },		//	HID_KEY_9                  0x26
	{  0, 0 },		//	HID_KEY_0                  0x27
	{  7, 7 },		//	HID_KEY_RETURN             0x28
	{  7, 2 },		//	HID_KEY_ESCAPE             0x29
	{  7, 5 },		//	HID_KEY_BACKSPACE          0x2A
	{  7, 3 },		//	HID_KEY_TAB                0x2B
	{  8, 0 },		//	HID_KEY_SPACE              0x2C
	{  1, 2 },		//	HID_KEY_MINUS              0x2D
	{  1, 3 },		//	HID_KEY_EQUAL              0x2E
	{  1, 5 },		//	HID_KEY_BRACKET_LEFT       0x2F
	{  1, 6 },		//	HID_KEY_BRACKET_RIGHT      0x30
	{  1, 4 },		//	HID_KEY_BACKSLASH          0x31
	{  2, 1 },		//	HID_KEY_EUROPE_1           0x32
	{  1, 7 },		//	HID_KEY_SEMICOLON          0x33
	{  2, 0 },		//	HID_KEY_APOSTROPHE         0x34
	{  7, 2 },		//	全/半                      0x35
	{  2, 2 },		//	HID_KEY_COMMA              0x36
	{  2, 3 },		//	HID_KEY_PERIOD             0x37
	{  2, 4 },		//	HID_KEY_SLASH              0x38
	{  6, 3 },		//	HID_KEY_CAPS_LOCK          0x39
	{  6, 5 },		//	HID_KEY_F1                 0x3A
	{  6, 6 },		//	HID_KEY_F2                 0x3B
	{  6, 7 },		//	HID_KEY_F3                 0x3C
	{  7, 0 },		//	HID_KEY_F4                 0x3D
	{  7, 1 },		//	HID_KEY_F5                 0x3E
	{  7, 4 },		//	HID_KEY_F6                 0x3F  STOP
	{  8, 1 },		//	HID_KEY_F7                 0x40  CLS/HOME
	{  7, 6 },		//	HID_KEY_F8                 0x41  SELECT
	{  8, 2 },		//	HID_KEY_F9                 0x42  INS
	{  8, 3 },		//	HID_KEY_F10                0x43  DEL
	{ -1, 0 },		//	HID_KEY_F11                0x44
	{ -1, 0 },		//	HID_KEY_F12                0x45
	{ -1, 0 },		//	HID_KEY_PRINT_SCREEN       0x46
	{ -1, 0 },		//	HID_KEY_SCROLL_LOCK        0x47
	{ 12, 0 },		//	HID_KEY_PAUSE              0x48  PAUSE
	{  8, 2 },		//	HID_KEY_INSERT             0x49
	{  8, 1 },		//	HID_KEY_HOME               0x4A
	{ -1, 0 },		//	HID_KEY_PAGE_UP            0x4B
	{  8, 3 },		//	HID_KEY_DELETE             0x4C
	{  7, 4 },		//	HID_KEY_END                0x4D  STOP
	{ -1, 0 },		//	HID_KEY_PAGE_DOWN          0x4E
	{  8, 7 },		//	HID_KEY_ARROW_RIGHT        0x4F
	{  8, 4 },		//	HID_KEY_ARROW_LEFT         0x50
	{  8, 6 },		//	HID_KEY_ARROW_DOWN         0x51
	{  8, 5 },		//	HID_KEY_ARROW_UP           0x52
	{ -1, 0 },		//	HID_KEY_NUM_LOCK           0x53
	{  9, 2 },		//	HID_KEY_KEYPAD_DIVIDE      0x54
	{  9, 0 },		//	HID_KEY_KEYPAD_MULTIPLY    0x55
	{ 10, 5 },		//	HID_KEY_KEYPAD_SUBTRACT    0x56
	{  9, 1 },		//	HID_KEY_KEYPAD_ADD         0x57
	{  7, 7 },		//	HID_KEY_KEYPAD_ENTER       0x58
	{  9, 4 },		//	HID_KEY_KEYPAD_1           0x59
	{  9, 5 },		//	HID_KEY_KEYPAD_2           0x5A
	{  9, 6 },		//	HID_KEY_KEYPAD_3           0x5B
	{  9, 7 },		//	HID_KEY_KEYPAD_4           0x5C
	{ 10, 0 },		//	HID_KEY_KEYPAD_5           0x5D
	{ 10, 1 },		//	HID_KEY_KEYPAD_6           0x5E
	{ 10, 2 },		//	HID_KEY_KEYPAD_7           0x5F
	{ 10, 3 },		//	HID_KEY_KEYPAD_8           0x60
	{ 10, 4 },		//	HID_KEY_KEYPAD_9           0x61
	{  9, 4 },		//	HID_KEY_KEYPAD_0           0x62
	{ 10, 7 },		//	HID_KEY_KEYPAD_DECIMAL     0x63
	{ -1, 0 },		//	HID_KEY_EUROPE_2           0x64
	{  6, 2 },		//	HID_KEY_APPLICATION        0x65  GRAPH
	{ -1, 0 },		//	HID_KEY_POWER              0x66
	{ -1, 0 },		//	HID_KEY_KEYPAD_EQUAL       0x67
	{ -1, 0 },		//	HID_KEY_F13                0x68
	{ -1, 0 },		//	HID_KEY_F14                0x69
	{ -1, 0 },		//	HID_KEY_F15                0x6A
	{ -1, 0 },		//	                           0x6B
	{ -1, 0 },		//	                           0x6C
	{ -1, 0 },		//	                           0x6D
	{ -1, 0 },		//	                           0x6E
	{ -1, 0 },		//	                           0x6F
	{ -1, 0 },		//	                           0x70
	{ -1, 0 },		//	                           0x71
	{ -1, 0 },		//	                           0x72
	{ -1, 0 },		//	                           0x73
	{ -1, 0 },		//	                           0x74
	{ -1, 0 },		//	                           0x75
	{ -1, 0 },		//	                           0x76
	{ -1, 0 },		//	                           0x77
	{ -1, 0 },		//	                           0x78
	{ -1, 0 },		//	                           0x79
	{ -1, 0 },		//	                           0x7A
	{ -1, 0 },		//	                           0x7B
	{ -1, 0 },		//	                           0x7C
	{ -1, 0 },		//	                           0x7D
	{ -1, 0 },		//	                           0x7E
	{ -1, 0 },		//	                           0x7F
	{ -1, 0 },		//	                           0x80
	{ -1, 0 },		//	                           0x81
	{ -1, 0 },		//	                           0x82
	{ -1, 0 },		//	                           0x83
	{ -1, 0 },		//	                           0x84
	{ -1, 0 },		//	                           0x85
	{ -1, 0 },		//	                           0x86
	{  2, 5 },		//	                           0x87  _
	{  6, 4 },		//	かな (Kana)                0x88  かな
	{  1, 4 },		//	                           0x89  ￥  
	{ 11, 1 },		//	変換                       0x8A  実行
	{ 11, 3 },		//	無変換                     0x8B  取消
	{ -1, 0 },		//	                           0x8C
	{ -1, 0 },		//	                           0x8D
	{ -1, 0 },		//	                           0x8E
	{ -1, 0 },		//	                           0x8F
	{ -1, 0 },		//	                           0x90
	{ -1, 0 },		//	                           0x91
	{ -1, 0 },		//	                           0x92
	{ -1, 0 },		//	                           0x93
	{ -1, 0 },		//	                           0x94
	{ -1, 0 },		//	                           0x95
	{ -1, 0 },		//	                           0x96
	{ -1, 0 },		//	                           0x97
	{ -1, 0 },		//	                           0x98
	{ -1, 0 },		//	                           0x99
	{ -1, 0 },		//	                           0x9A
	{ -1, 0 },		//	                           0x9B
	{ -1, 0 },		//	                           0x9C
	{ -1, 0 },		//	                           0x9D
	{ -1, 0 },		//	                           0x9E
	{ -1, 0 },		//	                           0x9F
	{ -1, 0 },		//	                           0xA0
	{ -1, 0 },		//	                           0xA1
	{ -1, 0 },		//	                           0xA2
	{ -1, 0 },		//	                           0xA3
	{ -1, 0 },		//	                           0xA4
	{ -1, 0 },		//	                           0xA5
	{ -1, 0 },		//	                           0xA6
	{ -1, 0 },		//	                           0xA7
	{ -1, 0 },		//	                           0xA8
	{ -1, 0 },		//	                           0xA9
	{ -1, 0 },		//	                           0xAA
	{ -1, 0 },		//	                           0xAB
	{ -1, 0 },		//	                           0xAC
	{ -1, 0 },		//	                           0xAD
	{ -1, 0 },		//	                           0xAE
	{ -1, 0 },		//	                           0xAF
	{ -1, 0 },		//	                           0xB0
	{ -1, 0 },		//	                           0xB1
	{ -1, 0 },		//	                           0xB2
	{ -1, 0 },		//	                           0xB3
	{ -1, 0 },		//	                           0xB4
	{ -1, 0 },		//	                           0xB5
	{ -1, 0 },		//	                           0xB6
	{ -1, 0 },		//	                           0xB7
	{ -1, 0 },		//	                           0xB8
	{ -1, 0 },		//	                           0xB9
	{ -1, 0 },		//	                           0xBA
	{ -1, 0 },		//	                           0xBB
	{ -1, 0 },		//	                           0xBC
	{ -1, 0 },		//	                           0xBD
	{ -1, 0 },		//	                           0xBE
	{ -1, 0 },		//	                           0xBF
	{ -1, 0 },		//	                           0xC0
	{ -1, 0 },		//	                           0xC1
	{ -1, 0 },		//	                           0xC2
	{ -1, 0 },		//	                           0xC3
	{ -1, 0 },		//	                           0xC4
	{ -1, 0 },		//	                           0xC5
	{ -1, 0 },		//	                           0xC6
	{ -1, 0 },		//	                           0xC7
	{ -1, 0 },		//	                           0xC8
	{ -1, 0 },		//	                           0xC9
	{ -1, 0 },		//	                           0xCA
	{ -1, 0 },		//	                           0xCB
	{ -1, 0 },		//	                           0xCC
	{ -1, 0 },		//	                           0xCD
	{ -1, 0 },		//	                           0xCE
	{ -1, 0 },		//	                           0xCF
	{ -1, 0 },		//	                           0xD0
	{ -1, 0 },		//	                           0xD1
	{ -1, 0 },		//	                           0xD2
	{ -1, 0 },		//	                           0xD3
	{ -1, 0 },		//	                           0xD4
	{ -1, 0 },		//	                           0xD5
	{ -1, 0 },		//	                           0xD6
	{ -1, 0 },		//	                           0xD7
	{ -1, 0 },		//	                           0xD8
	{ -1, 0 },		//	                           0xD9
	{ -1, 0 },		//	                           0xDA
	{ -1, 0 },		//	                           0xDB
	{ -1, 0 },		//	                           0xDC
	{ -1, 0 },		//	                           0xDD
	{ -1, 0 },		//	                           0xDE
	{ -1, 0 },		//	                           0xDF
	{  6, 1 },		//	HID_KEY_CONTROL_LEFT       0xE0
	{  6, 0 },		//	HID_KEY_SHIFT_LEFT         0xE1
	{  6, 2 },		//	HID_KEY_ALT_LEFT           0xE2
	{ -1, 0 },		//	HID_KEY_GUI_LEFT           0xE3
	{  6, 1 },		//	HID_KEY_CONTROL_RIGHT      0xE4
	{  6, 0 },		//	HID_KEY_SHIFT_RIGHT        0xE5
	{  6, 2 },		//	HID_KEY_ALT_RIGHT          0xE6
	{ -1, 0 },		//	HID_KEY_GUI_RIGHT          0xE7
	{ -1, 0 },		//	                           0xE8
	{ -1, 0 },		//	                           0xE9
	{ -1, 0 },		//	                           0xEA
	{ -1, 0 },		//	                           0xEB
	{ -1, 0 },		//	                           0xEC
	{ -1, 0 },		//	                           0xED
	{ -1, 0 },		//	                           0xEE
	{ -1, 0 },		//	                           0xEF
	{ -1, 0 },		//	                           0xF0
	{ -1, 0 },		//	                           0xF1
	{ -1, 0 },		//	                           0xF2
	{ -1, 0 },		//	                           0xF3
	{ -1, 0 },		//	                           0xF4
	{ -1, 0 },		//	                           0xF5
	{ -1, 0 },		//	                           0xF6
	{ -1, 0 },		//	                           0xF7
	{ -1, 0 },		//	                           0xF8
	{ -1, 0 },		//	                           0xF9
	{ -1, 0 },		//	                           0xFA
	{ -1, 0 },		//	                           0xFB
	{ -1, 0 },		//	                           0xFC
	{ -1, 0 },		//	                           0xFD
	{ -1, 0 },		//	                           0xFE
	{ -1, 0 },		//	                           0xFF
	{  6, 1 },		//	HID_KEY_CONTROL_LEFT       modifier 0
	{  6, 0 },		//	HID_KEY_SHIFT_LEFT         modifier 1
	{  6, 2 },		//	HID_KEY_ALT_LEFT           modifier 2
	{ -1, 0 },		//	HID_KEY_GUI_LEFT           modifier 3
	{  6, 1 },		//	HID_KEY_CONTROL_RIGHT      modifier 4
	{  6, 0 },		//	HID_KEY_SHIFT_RIGHT        modifier 5
	{  6, 2 },		//	HID_KEY_ALT_RIGHT          modifier 6
	{ -1, 0 },		//	HID_KEY_GUI_RIGHT          modifier 7
};
