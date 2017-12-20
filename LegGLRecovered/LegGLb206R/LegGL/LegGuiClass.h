#include "LegEngine.h"
#include "OpenALSound.h"
extern LegFrameworkClass p_lfc;
extern LegSoundscape p_soundapi;
extern void DebugLog(char* Message);
extern void CreateSettingsCfg();

#define FIRST 32
#define LAST 126

static GLubyte bitmapFont[][1+13] = {
{
32,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
33,
0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18,
0x18, 0x18, 0x18, 0x18, 0x18, 0x18
},
{
34,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x36, 0x36, 0x36, 0x36
},
{
35,
0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66,
0x66, 0xff, 0x66, 0x66, 0x00, 0x00
},
{
36,
0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f,
0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18
},
{
37,
0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30,
0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70
},
{
38,
0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70,
0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38
},
{
39,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e
},
{
40,
0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30,
0x30, 0x30, 0x30, 0x30, 0x18, 0x0c
},
{
41,
0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c,
0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30
},
{
42,
0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c,
0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00
},
{
43,
0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff,
0xff, 0x18, 0x18, 0x18, 0x00, 0x00
},
{
44,
0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
45,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
0xff, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
46,
0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
47,
0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18,
0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03
},
{
48,
0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3,
0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c
},
{
49,
0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x78, 0x38, 0x18
},
{
50,
0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30,
0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e
},
{
51,
0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07,
0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e
},
{
52,
0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c
},
{
53,
0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07,
0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff
},
{
54,
0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7,
0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e
},
{
55,
0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18,
0x0c, 0x06, 0x03, 0x03, 0x03, 0xff
},
{
56,
0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7,
0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e
},
{
57,
0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03,
0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e
},
{
58,
0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00,
0x38, 0x38, 0x00, 0x00, 0x00, 0x00
},
{
59,
0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00,
0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00
},
{
60,
0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60,
0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06
},
{
61,
0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
0xff, 0xff, 0x00, 0x00, 0x00, 0x00
},
{
62,
0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06,
0x03, 0x06, 0x0c, 0x18, 0x30, 0x60
},
{
63,
0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18,
0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e
},
{
64,
0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3,
0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00
},
{
65,
0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff,
0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18
},
{
66,
0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7,
0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe
},
{
67,
0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0,
0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e
},
{
68,
0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3,
0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc
},
{
69,
0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0,
0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff
},
{
70,
0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff
},
{
71,
0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf,
0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e
},
{
72,
0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3
},
{
73,
0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x18, 0x18, 0x7e
},
{
74,
0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06,
0x06, 0x06, 0x06, 0x06, 0x06, 0x06
},
{
75,
0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0,
0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3
},
{
76,
0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0,
0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0
},
{
77,
0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3
},
{
78,
0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf,
0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3
},
{
79,
0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3,
0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e
},
{
80,
0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe
},
{
81,
0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3,
0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c
},
{
82,
0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0,
0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe
},
{
83,
0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07,
0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e
},
{
84,
0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x18, 0x18, 0xff
},
{
85,
0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3,
0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3
},
{
86,
0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66,
0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3
},
{
87,
0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb,
0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3
},
{
88,
0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c,
0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3
},
{
89,
0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18,
0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3
},
{
90,
0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30,
0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff
},
{
91,
0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30,
0x30, 0x30, 0x30, 0x30, 0x30, 0x3c
},
{
92,
0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c,
0x18, 0x18, 0x30, 0x30, 0x60, 0x60
},
{
93,
0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c,
0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c
},
{
94,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18
},
{
95,
0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
96,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x18, 0x38, 0x30, 0x70
},
{
97,
0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03,
0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00
},
{
98,
0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3,
0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0
},
{
99,
0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0,
0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00
},
{
100,
0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3,
0x7f, 0x03, 0x03, 0x03, 0x03, 0x03
},
{
101,
0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3,
0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00
},
{
102,
0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30,
0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e
},
{
103,
0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3,
0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00
},
{
104,
0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0
},
{
105,
0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x00, 0x00, 0x18, 0x00
},
{
106,
0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00
},
{
107,
0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8,
0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0
},
{
108,
0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x18, 0x18, 0x78
},
{
109,
0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,
0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00
},
{
110,
0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00
},
{
111,
0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6,
0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00
},
{
112,
0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3,
0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00
},
{
113,
0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3,
0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00
},
{
114,
0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00
},
{
115,
0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0,
0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00
},
{
116,
0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30,
0x30, 0xfc, 0x30, 0x30, 0x30, 0x00
},
{
117,
0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6,
0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00
},
{
118,
0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66,
0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00
},
{
119,
0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3,
0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00
},
{
120,
0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c,
0x66, 0xc3, 0x00, 0x00, 0x00, 0x00
},
{
121,
0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66,
0x66, 0xc3, 0x00, 0x00, 0x00, 0x00
},
{
122,
0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c,
0x06, 0xff, 0x00, 0x00, 0x00, 0x00
},
{
123,
0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38,
0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f
},
{
124,
0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0x18, 0x18, 0x18
},
{
125,
0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c,
0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0
},
{
126,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00
}
};

// Gl Functions

int fontBitmapString(char *str)
{
int len,i;
GLint swapbytes, lsbfirst, rowlength;
GLint skiprows, skippixels, alignment;

len=(int)strlen(str);
glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

for(i=0;i < len;i++)
{
char ch = str[i];

if(ch==0) 
break;
if(ch < FIRST)
continue;
if(ch > LAST)
continue;
ch -= FIRST;
glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, &bitmapFont[ch][1]);
}

glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
return(1);
}


void GLFogSetLinear(void)
{
	if(p_lfc.MenuSelection==0 && p_lfc.GameState==4) glEnable (GL_FOG);
	//else glDisable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);

	// For linear fog only
	glFogf (GL_FOG_START, 16500.f);
	glFogf (GL_FOG_END, 17000.f);

	glFogf(GL_FOG_DENSITY, 0.0f);

	glHint(GL_FOG_HINT, GL_NICEST);
}

void GLFogSetEXP2(void)
{
	//if(p_lfc.MenuSelection==0 && p_lfc.GameState==4) glEnable (GL_FOG);
	//else glDisable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);

	float FogDistance = 0.0f;
	glFogf (GL_FOG_START, FogDistance);
	glFogf (GL_FOG_END, FogDistance+10.f);

	float DaytimeValue = 0.7f;
	static float Daytime[4] = { DaytimeValue, DaytimeValue, DaytimeValue, 0.1f };
	glFogfv(GL_FOG_COLOR, Daytime);

	glFogf(GL_FOG_DENSITY, 0.000025f);

	glHint(GL_FOG_HINT, GL_NICEST);
}




void glPrint(int x, int y, char *string, GLfloat red, GLfloat green, GLfloat blue)
{
char buffer[256];
va_list args;
GLfloat color[4], position[4];
GLboolean texturing;

va_start(args, string);
vsprintf(buffer, string, args);
va_end(args);

glDisable(GL_TEXTURE_2D);

glPushAttrib(GL_ALL_ATTRIB_BITS);
glGetFloatv(GL_CURRENT_COLOR, color);
glGetBooleanv(GL_TEXTURE_2D, &texturing);
glGetFloatv(GL_CURRENT_RASTER_POSITION, position);

glColor3f(red,green,blue);
glRasterPos2i(x, y);

fontBitmapString(buffer);

glPopAttrib();
glColor4fv(color);

glEnable(GL_TEXTURE_2D);
}

void glDrawTexturedBoxf(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLint ID)
{
glBindTexture (GL_TEXTURE_2D, 5);
glEnable(GL_TEXTURE_2D);
glBegin (GL_QUADS);
glTexCoord2f (0.0, 0.0);
glVertex3f (0.0, 0.0, 0.0);
glTexCoord2f (1.0, 0.0);
glVertex3f (10.0, 0.0, 0.0);
glTexCoord2f (1.0, 1.0);
glVertex3f (10.0, 10.0, 0.0);
glTexCoord2f (0.0, 1.0);
glVertex3f (0.0, 10.0, 0.0);
glEnd();
glDisable(GL_TEXTURE_2D);

/*
glBegin(GL_QUADS);
glVertex3f(x,y,z);
glVertex3f(x+width,y,z);
glVertex3f(x+width,y+height,z);
glVertex3f(x,y+height,z);
glEnd();
*/
} 

void glDrawBoxf(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
glBegin(GL_QUADS);
glVertex3f(x,y,z);
glVertex3f(x+width,y,z);
glVertex3f(x+width,y+height,z);
glVertex3f(x,y+height,z);
glEnd();
} 

void glInit2DMode(void)
{
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
glOrtho(0, p_lfc.gl_Resolutionw, p_lfc.gl_Resolutionh, 0, -1, 10.0f);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glDisable(GL_CULL_FACE);
glClear(GL_DEPTH_BUFFER_BIT);
}
void glEnd2DMode(void)
{
glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
}
bool LegGUI_LoadCustomTexture(char *filename, int ID) 
{
    int i, j=0; //Index variables
    FILE *l_file; //File pointer
    unsigned char *l_texture; //The pointer to the memory zone in which we will load the texture
     
    // windows.h gives us these types to work with the Bitmap files
    BITMAPFILEHEADER fileheader; 
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;


    if( (l_file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading
    
    fread(&fileheader, sizeof(fileheader), 1, l_file); // Read the fileheader
    
    fseek(l_file, sizeof(fileheader), SEEK_SET); // Jump the fileheader
    fread(&infoheader, sizeof(infoheader), 1, l_file); // and read the infoheader

    // Now we need to allocate the memory for our image (width * height * color deep)
    l_texture = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    // And fill it with zeros
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);
 
    // At this point we can read every pixel of the image
    for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++)
    {            
            // We load an RGB value from the file
            fread(&rgb, sizeof(rgb), 1, l_file); 

            // And store it
            l_texture[j+0] = rgb.rgbtRed; // Red component
            l_texture[j+1] = rgb.rgbtGreen; // Green component
            l_texture[j+2] = rgb.rgbtBlue; // Blue component
            l_texture[j+3] = 255; // Alpha value
            j += 4; // Go to the next position
    }

    fclose(l_file); // Closes the file stream
     
    glBindTexture(GL_TEXTURE_2D, ID); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function
	if(p_lfc.cvarAF) glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,16);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // We don't combine the color with the original surface color, use only the texture map.

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    free(l_texture); // Free the memory we used to load the texture

    return ID; // Returns the current texture OpenGL ID
}

void RestartApp(int Towindow)
{
	p_lfc.cvarStartup = Towindow;
	CreateSettingsCfg();
	char szFileName[MAX_PATH]="";
	GetModuleFileName(NULL,szFileName,MAX_PATH);
	ShellExecute(GetDesktopWindow(), "OPEN", szFileName, NULL, NULL, SW_SHOWDEFAULT);
	exit(0);
}

// Menu rendering
void LegGUI_DrawMenu( float x, float y )
{
//Draw The Background
glColor4f(0.0f,0.0f,0.0f,0.25f); // Black See Through Background
glDrawBoxf(x,x,-1.0f,x,y); // X,Y ( 200,200 ) Width-200 Height-300

//Draw The TitleBar
glColor4f(1.0f,1.0f,0.0f,1.0f); // Black Completely Opaque
glDrawBoxf(x,x,-1.0f,x,y-286.0f); // X,Y ( 200,200 ) Width-200 Height-14

glColor4f(1.0f,1.0f,0.0f,1.0f); // Black Completely Opaque
float Selection = p_lfc.MenuSelection*12; 
glDrawBoxf(x,x+Selection,-1.0f,x,y-286.0f); 

//Draw The Title
glPrint(x+1.0f,y-87.0f,"Menu",1.0f,1.0f,1.0f); 


//Set Menu Entrys
char whEntry[30],lmEntry[30],wiEntry[30];
{
sprintf(whEntry," Resume game ->");
}
{
sprintf(lmEntry," Options -> ");
}

{
sprintf(wiEntry," Exit to main -> ");
}

//Display Menu Entrys
glPrint(201.0f,226.0f,whEntry,1.0f,1.0f,1.0f);
glPrint(201.0f,238.0f,lmEntry,1.0f,1.0f,1.0f);
glPrint(201.0f,250.0f,wiEntry,1.0f,1.0f,1.0f);
	
	/*
	char temp[100];
	sprintf(temp, "%f %f %f;", p_lfc.gCamAnglessidevec.x, p_lfc.gCamAnglessidevec.y, p_lfc.gCamAnglessidevec.z);
	strcpy(p_lfc.gCamAnglesside, temp);
	sprintf(temp, "%f %f %f;", p_lfc.gCamAnglesupvec.x, p_lfc.gCamAnglesupvec.y, p_lfc.gCamAnglesupvec.z);
	strcpy(p_lfc.gCamAnglesup, temp);
	sprintf(temp, "%f %f %f;", p_lfc.gCamAnglesdirvec.x, p_lfc.gCamAnglesdirvec.y, p_lfc.gCamAnglesdirvec.z);
	strcpy(p_lfc.gCamAnglesdir, temp);
	glPrint(100.0f,100.0f,p_lfc.gCamAnglesside,1.0f,1.0f,1.0f);
	glPrint(100.0f,110.0f,p_lfc.gCamAnglesup,1.0f,1.0f,1.0f);
	glPrint(100.0f,120.0f,p_lfc.gCamAnglesdir,1.0f,1.0f,1.0f);
	*/
	char temp[100];
	sprintf(temp, "%f %f %f;", p_lfc.gCamPosition.x, p_lfc.gCamPosition.y, p_lfc.gCamPosition.z);
	glPrint(100.0f,130.0f,temp,0.8f,1.0f,1.0f);
	sprintf(temp, "%f %f %f;", p_lfc.gCamAngle.x, p_lfc.gCamAngle.y, p_lfc.gCamAngle.z);
	glPrint(100.0f,100.0f,temp,0.8f,1.0f,1.0f);
	
} 

void LegGUI_SelectionHandler(WPARAM wParam)
{
	int Max = p_lfc.MaxSelectionsinMenu;
	//int Max = 4;

	// Function is only called upon a button press
	if(wParam == VK_DOWN) p_lfc.MenuSelection++;
	if(wParam == VK_UP) p_lfc.MenuSelection--;
	if(p_lfc.MenuSelection<0 || p_lfc.MenuSelection>Max) p_lfc.MenuSelection = 0;

	else if (p_lfc.GameState==7) 
	{
		switch (p_lfc.controlIndex)
		{
		case 1:
			p_lfc.controlForward = (char)wParam;
			p_lfc.GameState = 6;
		break;
		case 2:
			p_lfc.controlBack = (char)wParam;
			p_lfc.GameState = 6;
		break;
		case 3:
			p_lfc.controlStrafeleft = (char)wParam;
			p_lfc.GameState = 6;
		break;
		case 4:
			p_lfc.controlStraferight = (char)wParam;
			p_lfc.GameState = 6;
		break;

		default:
			p_lfc.GameState = 6;
		break;
		}
	}


	if (wParam == VK_RETURN)
	{
		if (p_lfc.GameState == 0) // Main menu
		{
			if (p_lfc.MenuSelection == 1) { p_lfc.GameState = 1; p_lfc.GenerateNewIsland = true; }
			if (p_lfc.MenuSelection == 2)
			{
				if (p_lfc.NoSaveFiles()) p_lfc.DisplaySaveGameGLError = true;
				else p_lfc.GameState = 2;
			}
			if (p_lfc.MenuSelection == 3) p_lfc.GameState = 3;
			if (p_lfc.MenuSelection == 4) DestroyWindow(p_lfc.Window);
		}
		else if (p_lfc.GameState == 1) // 3d1 + ingame menu
		{
			p_lfc.DisplaySaveGameGLError = false;

			if (p_lfc.ShowIngameMenuCvar)
			{
				if (p_lfc.MenuSelection == 1) p_lfc.ShowIngameMenuCvar = false;
				if (p_lfc.MenuSelection == 2) p_lfc.GameState = 3;
				if (p_lfc.MenuSelection == 3) p_lfc.GameState = 0;
			}
		}
		else if (p_lfc.GameState == 2) // Load menu
		{
		}
		else if (p_lfc.GameState == 3) // Options
		{
			if (p_lfc.MenuSelection == 1) p_lfc.GameState = 4; // Video
			if (p_lfc.MenuSelection == 2) p_lfc.GameState = 5; // Audio
			if (p_lfc.MenuSelection == 3) p_lfc.GameState = 6; // Controls
			if (p_lfc.MenuSelection == 4) p_lfc.GameState = 0; // Main menu
		}
		else if (p_lfc.GameState == 4) // Video options
		{
			if (p_lfc.MenuSelection == 0) { p_lfc.cvarFog++; if (p_lfc.cvarFog > 1) p_lfc.cvarFog = 0; }
			if (p_lfc.MenuSelection == 1) { p_lfc.cvarAF = !p_lfc.cvarAF; }
			if (p_lfc.MenuSelection == 2) { p_lfc.cvarDensity++; if (p_lfc.cvarDensity > 2) p_lfc.cvarDensity = 0; }
			if (p_lfc.MenuSelection == 3) { p_lfc.cvarFullscreen = !p_lfc.cvarFullscreen; RestartApp(3); }
			if (p_lfc.MenuSelection == 4)
			{
				p_lfc.ResetVideoOptions = true;

				// Game state reset to 3 in LegEngine
				//RestartApp(3); 
			}
		}
		else if (p_lfc.GameState == 5) // Audio options
		{
			if (p_lfc.MenuSelection == 0)
			{
				p_soundapi.Gain += 0.1f;
				if (p_soundapi.Gain > 1.01f) p_soundapi.Gain = 0.0f;
				p_lfc.cvarGain = p_soundapi.Gain*10.0f;
				p_soundapi.playMusic("Sounds/a.wav", 1.0f);
			}
			if (p_lfc.MenuSelection == 1) { p_lfc.cvarMainMenu++; if (p_lfc.cvarMainMenu > 1) p_lfc.cvarMainMenu = 0; }
			if (p_lfc.MenuSelection == 2) { p_lfc.GameState = 3; CreateSettingsCfg(); }
		}
		else if (p_lfc.GameState == 6) // Control options
		{
			if (p_lfc.MenuSelection == 0)
			{
				p_lfc.GameState = 3; CreateSettingsCfg();
			}
			if (p_lfc.MenuSelection == 1)
			{
				p_lfc.GameState = 7; p_lfc.controlIndex = 1;
			}
			if (p_lfc.MenuSelection == 2)
			{
				p_lfc.GameState = 7; p_lfc.controlIndex = 2;
			}
			if (p_lfc.MenuSelection == 3)
			{
				p_lfc.GameState = 7; p_lfc.controlIndex = 3;
			}
			if (p_lfc.MenuSelection == 4)
			{
				p_lfc.GameState = 7; p_lfc.controlIndex = 4;
			}
		}
	}
}

void LegGUI_DrawMainMenu( float x, float y )
{
//Draw The Background
glEnable(GL_BLEND); 
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glColor4f(0.0f,0.0f,1.0f,0.1f); 
glDrawBoxf(x,y-12.0f,-1.0f,200,100); 
glDisable(GL_BLEND); 
//Draw The TitleBar
glColor4f(1.0f,1.0f,0.0f,1.0f); // Black Completely Opaque
float Selection = p_lfc.MenuSelection*12; 
glDrawBoxf(x,y+Selection-14.0f,-1.0f,200,14); 

//Draw The Title
glPrint(x+1.0f,y-1.0f,"Main Menu",1.0f,1.0f,1.0f); 


//Set Menu Entrys
char whEntry[30],lmEntry[30],wiEntry[30], liEntry[30];

{
sprintf(whEntry," New Game -> ");
}

{
sprintf(lmEntry," Resume -> ");
}

{
sprintf(wiEntry," Options -> ");
}

{
sprintf(liEntry, " Exit -> ");
}

//Display Menu Entrys
glPrint(x+1.0f,y+12.0f,whEntry,1.0f,1.0f,1.0f);
glPrint(x+1.0f,y+24.0f,lmEntry,1.0f,1.0f,1.0f);
glPrint(x+1.0f,y+36.0f,wiEntry,1.0f,1.0f,1.0f);
glPrint(x+1.0f,y+48.0f,liEntry,1.0f,1.0f,1.0f);
} 

DWORD InitCVARValues(void)
{
	//while(true)
	{
	p_lfc.cvarFog = gCVARS.GetInt("gl_fogtype");
	p_lfc.cvarDensity = gCVARS.GetInt("gl_terraindetail");
	if(gCVARS.GetInt("gl_Fullscreen")) p_lfc.cvarFullscreen = true;
	else p_lfc.cvarFullscreen = false;

	// Shaders
	p_lfc.cvarShaders = gCVARS.GetInt("glsl_Shaders");

	// Terrain Detail
	if(p_lfc.cvarDensity==0)
	{
		p_lfc.Density = 1;
	}
	else if (p_lfc.cvarDensity==2)
	{
		p_lfc.Density = 3;
	}
	else
	{
		p_lfc.Density = 2;
	}
	p_lfc.fDensity = (float)p_lfc.Density/10.0f;
	//Sleep(5000);

	// For restarting to a certain area
	if(gCVARS.GetInt("leg_Startup")) 
	{
	p_lfc.GameState = gCVARS.GetInt("leg_Startup");
	p_lfc.cvarStartup = 0;
	CreateSettingsCfg();
	}

	p_soundapi.Gain = (float)gCVARS.GetInt("sound_Volume")/10.0f;
	p_lfc.cvarGain = (float)gCVARS.GetInt("sound_Volume");
	p_lfc.cvarMainMenu = gCVARS.GetInt("sound_MenuMusic");


	p_lfc.cvarAF = gCVARS.GetInt("gl_AF");

	// +32 of (int)cvar = capitol version of char
	p_lfc.controlForward = gCVARS.GetInt("controls_Forward");
	p_lfc.controlBack = gCVARS.GetInt("controls_Back");
	p_lfc.controlStrafeleft = gCVARS.GetInt("controls_Strafeleft");
	p_lfc.controlStraferight = gCVARS.GetInt("controls_Straferight");

	// Mousespeed
	p_lfc.controlMousePitch = gCVARS.GetInt("controls_MousePitchSpeed")*-100000;
	p_lfc.controlMouseYaw = gCVARS.GetInt("controls_MouseYawSpeed")*-100000;
	}
	return true;
}


void LegGUI_DrawOptionsMenu(int Type)
{
if(Type==0) // Options Menu
{
//Draw The Background
int x = 300; 
int y = 300;
glEnable(GL_BLEND); 
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glColor4f(0.0f,0.0f,1.0f,0.1f); 
glDrawBoxf(x,y-12.0f,-1.0f,200,100); 

//Draw The TitleBar
glColor4f(1.0f,1.0f,0.0f,1.0f); // Black Completely Opaque
float Selection = p_lfc.MenuSelection*12; 
glDrawBoxf(x,y+Selection-14.0f,-1.0f,200,14); 
glDisable(GL_BLEND); 
//Draw The Title
glPrint(x+1.0f,y-1.0f,"Options",1.0f,1.0f,1.0f); 


//Set Menu Entrys
char whEntry[30],lmEntry[30],wiEntry[30], liEntry[30];

{
sprintf(whEntry," Video Options -> ");
}

{
sprintf(lmEntry," Audio Options -> ");
}

{
sprintf(wiEntry," Controls * ");
}

{
sprintf(liEntry, " Return to main menu -> ");
}

//Display Menu Entrys
glPrint(x+1.0f,y+12.0f,whEntry,1.0f,1.0f,1.0f);
glPrint(x+1.0f,y+24.0f,lmEntry,1.0f,1.0f,1.0f);
glPrint(x+1.0f,y+36.0f,wiEntry,1.0f,1.0f,1.0f);
glPrint(x+1.0f,y+48.0f,liEntry,1.0f,1.0f,1.0f);
}

if(Type==1)  // Video Options
	{
	int x = 200; 
	int y = 212;
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f,0.0f,1.0f,0.1f); 
	glDrawBoxf(x,y-12.0f,-1.0f,p_lfc.gl_Resolutionw-400,p_lfc.gl_Resolutionh-300); 

	//Draw The selection bar
	glColor4f(1.0f,1.0f,0.0f,1.0f); 
	float Selection = p_lfc.MenuSelection*12; 
	glDrawBoxf(x,y+Selection-14.0f,-1.0f,200,14); 
	glDisable(GL_BLEND); 


	// Fog
	glPrint(x+1.0f,y-2.0f," Fog Detail ",1.0f,1.0f,1.0f);
	if(p_lfc.cvarFog==1) 
	{
		glPrint(x+201.0f,y-2.0f," Performance ",1.0f,1.0f,1.0f);
		GLFogSetLinear();
	}
	else
	{
		glPrint(x+201.0f,y-2.0f," Quality ",1.0f,1.0f,1.0f);
		GLFogSetEXP2();
	}

	// GL Lighting, not implemented yet
	glPrint(x+1.0f,y+10.0f," Anisotropic Filter ",1.0f,1.0f,1.0f);
	if(p_lfc.cvarAF)
	glPrint(x+201.0f,y+10.0f," On ",1.0f,1.0f,1.0f);
	else glPrint(x+201.0f,y+10.0f," Off ",1.0f,1.0f,1.0f);

	// 3D Detail
	glPrint(x+1.0f,y+22.0f," Terrain detail ",1.0f,1.0f,1.0f);
	if(p_lfc.cvarDensity==0)
	{
		glPrint(x+201.0f,y+22.0f," Low ",1.0f,1.0f,1.0f);
		p_lfc.Density = 1;
		// ResetTerrainParams
	}
	else if (p_lfc.cvarDensity==2)
	{
		glPrint(x+201.0f,y+22.0f," High ",1.0f,1.0f,1.0f);
		p_lfc.Density = 4;
		// ResetTerrainParams
	}
	else
	{
		glPrint(x+201.0f,y+22.0f," Normal ",1.0f,1.0f,1.0f);
		p_lfc.Density = 2;
		// ResetTerrainParams
	}

	glPrint(x+1.0f,y+34.0f," Fullscreen ",1.0f,1.0f,1.0f);
	if(p_lfc.cvarFullscreen)
	glPrint(x+201.0f,y+34.0f," Yes ",1.0f,1.0f,1.0f);
	else 
	glPrint(x+201.0f,y+34.0f," Windowed ",1.0f,1.0f,1.0f);

	glPrint(x+1.0f,y+46.0f,"Ok",1.0f,1.0f,1.0f);
}

if(Type==2) // Audio Options
{
	p_lfc.MaxSelectionsinMenu = 2;
	int x = 200; 
	int y = 212;
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f,0.0f,1.0f,0.1f); 
	glDrawBoxf(x,y-12.0f,-1.0f,p_lfc.gl_Resolutionw-400,p_lfc.gl_Resolutionh-400); 

	//Draw The selection bar
	glColor4f(1.0f,1.0f,0.0f,1.0f); 
	float Selection = p_lfc.MenuSelection*12; 
	glPrint(x+1.0f,y-2.0f,"Volume",1.0f,1.0f,1.0f);
	glDrawBoxf(x,y+Selection-14.0f,-1.0f,200,14); 
	glDisable(GL_BLEND); 

		//char temp[100];
		//sprintf(temp, "%f", p_soundapi.Gain);
		//glPrint(100,100,temp,1.0f,.7f,.7f);
		glDrawBoxf(x+201.f,y-10.0f,.5f,20*(p_soundapi.Gain*10.0f),10); 
		glColor4f(0.0f,0.0f,0.1f,1.0f); 
		glDrawBoxf(x+201.f,y-10.0f,.5f,1,10); 
		glDrawBoxf(x+401.f,y-10.0f,.5f,1,10);

	glPrint(x+1.0f,y+10.0f,"Menu Music",1.0f,1.0f,1.0f);
	if(p_lfc.cvarMainMenu) glPrint(x+201.0f,y+12.0f,"On",1.0f,1.0f,1.0f);
	else glPrint(x+201.0f,y+12.0f,"Off",1.0f,1.0f,1.0f);

	glPrint(x+1.0f,y+22.0f,"Ok",1.0f,1.0f,1.0f);
}

if(Type==3) // Control options
{
	
	int x = 200; 
	int y = 212;
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f,0.0f,1.0f,0.1f); 
	glDrawBoxf(x,y-12.0f,-1.0f,p_lfc.gl_Resolutionw-400,p_lfc.gl_Resolutionh-400); 

	//Draw The selection bar
	glColor4f(1.0f,1.0f,0.0f,1.0f); 
	float Selection = p_lfc.MenuSelection*12; 
	glPrint(x+1.0f,y-2.0f,"Apply settings",1.0f,1.0f,1.0f);
	glDrawBoxf(x,y+Selection-14.0f,-1.0f,200,14); 
	glDisable(GL_BLEND); 

	char temp[10];
	// Walk forward
	glPrint(x+1.0f,y+12.0f," Forward ",1.0f,1.0f,1.0f);
	sprintf(temp, " %c ", (char)p_lfc.controlForward);
	glPrint(x+201.0f,y+12.0f,temp,1.0f,1.0f,1.0f);

	// Walk backward
	glPrint(x+1.0f,y+24.0f," Backward ",1.0f,1.0f,1.0f);
	sprintf(temp, " %c ", (char)p_lfc.controlBack);
	glPrint(x+201.0f,y+24.0f,temp,1.0f,1.0f,1.0f);

	// Strafe left
	glPrint(x+1.0f,y+36.0f," Strafe left ",1.0f,1.0f,1.0f);
	sprintf(temp, " %c ", (char)p_lfc.controlStrafeleft);
	glPrint(x+201.0f,y+36.0f,temp,1.0f,1.0f,1.0f);

	// Strafe right
	glPrint(x+1.0f,y+48.0f," Strafe right ",1.0f,1.0f,1.0f);
	sprintf(temp, " %c ", (char)p_lfc.controlStraferight);
	glPrint(x+201.0f,y+48.0f,temp,1.0f,1.0f,1.0f);
}

}

void RenderControlsText1()
{
	char temp[10];
	sprintf(temp, " %i ", p_lfc.controlForward);
	glPrint(100.0f,120.0f,temp,1.0f,0.5f,0.5f);
	glPrint(100.0f,100.0f," PRESS ANY KEY ",1.0f,0.5f,0.5f);
}


void DebugGLPrint(char* fmt, ...)
{
	glPrint(50.0f,50.0f, fmt, 1.0, 0.5f, 0.5f);
}

void glDrawMouseCursor()
{
	int ZOOM = -10.0f;
   //Will be transparent
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   
   //Draw the cursor at the current mouse pos
   glBegin(GL_TRIANGLES);
     glColor4f(0.75,0.75,0.75,0.75);
     glVertex3f(p_lfc.MousePosition[0],p_lfc.MousePosition[1],ZOOM);
     glVertex3f(p_lfc.MousePosition[0],p_lfc.MousePosition[1]-0.5f,ZOOM);
     glVertex3f(p_lfc.MousePosition[0]+0.5f,p_lfc.MousePosition[1],ZOOM);
   glEnd();
   
   glDisable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);
}
