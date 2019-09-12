/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const char * fonts[]            = { "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*" };
static const char normbordercolor[] = "#484848";
static const char normbgcolor[]     = "#000000";
static const char normfgcolor[]     = "#C40000";
static const char selbordercolor[]  = "#C40000";
static const char selbgcolor[]      = "#C40000";
static const char selfgcolor[]      = "#FFFFFF";
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const _Bool showbar           = 1;     /* 0 means no bar */
static const _Bool topbar            = 1;     /* 0 means bottom bar */
static const char col_gray1[]       = "#222222";                                                     
static const char col_gray2[]       = "#444444";                                         
static const char col_gray3[]       = "#bbbbbb";                                                
static const char col_gray4[]       = "#eeeeee";                                                                                              
static const char col_cyan[]        = "#005577";                                                                                            
static const char *colors[][3]      = {                                                                                                     
    /*               fg         bg         border   */    
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },    
    [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },    
};    

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
//	{ "Gimp",     NULL,       NULL,       0,            1,        -1 },
//	{ "Firefox",  NULL,       NULL,       1 << 8,       0,       -1 },
	NULL
};

/* layout(s) */
static const float mfact      = .5f; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const _Bool resizehints = 0; /* 1 means respect size hints in tiled resizals, 0 to avoid annoying gaps */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
# define DMENU_OPTIONS "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor
static const char *dmenucmd[] = { "dmenu_run", DMENU_OPTIONS, NULL };
static const char *termcmd[]  = { "xterm", "-fg", "red", "-bg", "black", NULL};

static Key keys[] = {
	/* modifier                     key                        function        argument */
	{ 0,                            XK_Print,                  spawn,          {.v = (const char * []) {"scrot",  "-q",  "100", NULL} } },
	{ 0,                            XF86XK_AudioRaiseVolume,   spawn,          {.v = (const char * []) {"pamixer", "-i", "5", NULL} } },
	{ 0,                            XF86XK_AudioLowerVolume,   spawn,          {.v = (const char * []) {"pamixer", "-d", "5", NULL} } },
	{ 0,                            XF86XK_AudioMute,          spawn,          {.v = (const char * []) {"pamixer", "-t", NULL} } },
	{ 0,                            XF86XK_AudioMicMute,       spawn,          {.v = (const char * []) {"pamixer", "-t", NULL} } },
	{ 0,                            XF86XK_AudioNext,          spawn,          {.v = (const char * []) {"cmus-remote", "-n", NULL} } },
	{ 0,                            XF86XK_AudioPrev,          spawn,          {.v = (const char * []) {"cmus-remote", "-r", NULL} } },
	{ 0,                            XF86XK_AudioPlay,          spawn,          {.v = (const char * []) {"cmus-remote", "-u", NULL} } },
	{ 0,                            XF86XK_AudioStop,          spawn,          {.v = (const char * []) {"cmus-remote", "-s", NULL} } },
	{ 0,                            XF86XK_MonBrightnessUp,    spawn,          {.v = (const char * []) {"xbacklight", "-inc", "10", NULL} } },
	{ 0,                            XF86XK_MonBrightnessDown,  spawn,          {.v = (const char * []) {"xbacklight", "-dec", "10", NULL} } },
	{ MODKEY,                       XK_Delete,                 spawn,          {.v = (const char * []) {"slock", NULL}} },
	{ MODKEY,                       XK_c,                      spawn,          {.v = (const char * []) {"clipmenu", "-i", DMENU_OPTIONS, NULL} } },
	{ MODKEY,                       XK_d,                      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                 spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,                      togglebar,      {0} },
	{ MODKEY,                       XK_Left,                   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Right,                  focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Tab,                    focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,                    focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Up,                     incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_Down,                   incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,                 zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_q,                      killclient,     {0} },
	{ MODKEY,                       XK_t,                      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                  togglefloating, {0} },
	{ MODKEY,                       XK_comma,                  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                 tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                                      0)
	TAGKEYS(                        XK_2,                                      1)
	TAGKEYS(                        XK_3,                                      2)
	TAGKEYS(                        XK_4,                                      3)
	TAGKEYS(                        XK_5,                                      4)
	TAGKEYS(                        XK_6,                                      5)
	TAGKEYS(                        XK_7,                                      6)
	TAGKEYS(                        XK_8,                                      7)
	TAGKEYS(                        XK_9,                                      8)
	{ MODKEY,                       XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                      tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_e,                      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

