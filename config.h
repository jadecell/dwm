/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static unsigned int borderpx        = 2;        /* border pixel of windows */
static unsigned int snap            = 32;       /* snap pixel */
static unsigned int gappih          = 10;       /* horiz inner gap between windows */
static unsigned int gappiv          = 10;       /* vert inner gap between windows */
static unsigned int gappoh          = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov          = 10;       /* vert outer gap between windows and screen edge */
static int swallowfloating          = 0;        /* 1 means swallow floating windows by default */
static int smartgaps                = 0;        /* 1 means no outer gap when there is only one window */
static int showbar                  = 1;        /* 0 means no bar */
static int topbar                   = 1;        /* 0 means bottom bar */
static char *fonts[]                = { "Hack Nerd Font:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true"  };
static const char dmenufont[]       = "Hack Nerd Font:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#23D18B";
static char selbgcolor[]            = "#157F54";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;

const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spspot", "-g", "120x34", "-e", "spotify", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spspot",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ TERMCLASS,  NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,      "spspot",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
 	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", "#1e1e1e", "-nf", "#abb2bf", "-sb", "#303030", "-sf", selbordercolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		/* { "color0",		    STRING,	 &normbordercolor }, */
		/* { "color8",		    STRING,	 &selbordercolor }, */
		/* { "color0",		    STRING,	 &normbgcolor }, */
		/* { "color4",		    STRING,	 &normfgcolor }, */
		/* { "color0",		    STRING,	 &selfgcolor }, */
		/* { "color4",		    STRING,	 &selbgcolor }, */
		{ "borderpx",		    INTEGER, &borderpx },
		{ "snap",		        INTEGER, &snap },
		{ "showbar",		    INTEGER, &showbar },
		{ "topbar",		        INTEGER, &topbar },
		{ "nmaster",		    INTEGER, &nmaster },
		{ "resizehints",	    INTEGER, &resizehints },
		{ "mfact",		        FLOAT,	 &mfact },
		{ "gappih",		        INTEGER, &gappih },
		{ "gappiv",		        INTEGER, &gappiv },
		{ "gappoh",		        INTEGER, &gappoh },
		{ "gappov",		        INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		    INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	TAGKEYS(			            XK_1,		0)
	TAGKEYS(			            XK_2,		1)
	TAGKEYS(			            XK_3,		2)
	TAGKEYS(			            XK_4,		3)
	TAGKEYS(			            XK_5,		4)
	TAGKEYS(			            XK_6,		5)
	TAGKEYS(			            XK_7,		6)
	TAGKEYS(			            XK_8,		7)
	TAGKEYS(			            XK_9,		8)

	{ MODKEY|ShiftMask,             XK_Return,                  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_w,                       spawn,          SHCMD("$BROWSER") },
	{ Mod1Mask|ControlMask,         XK_e,                       spawn,          SHCMD("emacsclient -c -n -a emacs") },
	{ Mod1Mask|ControlMask,         XK_b,                       spawn,          SHCMD("emacsclient -c -n -a emacs --eval '(ibuffer)'") },
	{ Mod1Mask|ControlMask,         XK_d,                       spawn,          SHCMD("emacsclient -c -n -a emacs --eval '(dired nil)'") },
	{ Mod1Mask|ControlMask,         XK_n,                       spawn,          SHCMD("emacsclient -c -n -a emacs --eval '(elfeed)'") },
	{ Mod1Mask|ControlMask,         XK_s,                       spawn,          SHCMD("emacsclient -c -n -a emacs --eval '(eshell)'") },
	{ MODKEY|ShiftMask,             XK_s,                       spawn,          SHCMD("spotify") },
	{ MODKEY|ControlMask,           XK_u,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-unicode.sh") },
	{ MODKEY|ControlMask,           XK_p,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-power.sh") },
	{ MODKEY|ControlMask,           XK_t,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-timer.sh") },
	{ MODKEY|ControlMask,           XK_s,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-ssh.sh") },
	{ MODKEY|ControlMask|ShiftMask, XK_s,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-scrot.sh") },
	{ MODKEY|ControlMask,           XK_b,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-bookmarks.sh") },
	{ MODKEY|ControlMask|ShiftMask, XK_b,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-browse.sh") },
	{ MODKEY|ControlMask,           XK_y,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-sysmon.sh") },
	{ MODKEY|ControlMask,           XK_m,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-mount.sh") },
	{ MODKEY|ControlMask|ShiftMask, XK_m,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-umount.sh") },
	{ MODKEY|ControlMask,           XK_e,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-edit-configs.sh") },
	{ MODKEY|ControlMask,           XK_c,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-calc.sh") },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,          SHCMD("pamixer -d 2 ; pkill -RTMIN+3 dwmblocks") },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,          SHCMD("pamixer -i 2 ; pkill -RTMIN+3 dwmblocks") },
	{ 0,                            XF86XK_AudioMute,           spawn,          SHCMD("pamixer --toggle-mute ; pkill -RTMIN+3 dwmblocks") },
	{ 0,                            XF86XK_AudioPlay,           spawn,          SHCMD("playerctl -p spotify play-pause") },
	{ 0,                            XF86XK_AudioPrev,           spawn,          SHCMD("playerctl -p spotify previous") },
	{ 0,                            XF86XK_AudioNext,           spawn,          SHCMD("playerctl -p spotify next") },
	{ 0,                            XF86XK_AudioStop,           spawn,          SHCMD("playerctl -p spotify stop") },
	{ MODKEY|ShiftMask,             XK_q,                       spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-prompt.sh 'Quit DWM?' 'killall xinit'") },
	{ MODKEY,			            XK_0,		                view,		    {.ui = ~0 } },
	{ MODKEY|ShiftMask,		        XK_0,		                tag,		    {.ui = ~0 } },
	{ MODKEY,			            XK_Tab,		                view,		    {0} },
	{ MODKEY|ShiftMask,		        XK_c,		                killclient,	    {0} },
	{ MODKEY,			            XK_t,		                setlayout,	    {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		        XK_t,		                setlayout,	    {.v = &layouts[1]} }, /* bstack */
	/* { MODKEY,			        XK_u,		                setlayout,	    {.v = &layouts[4]} }, /1* deck *1/ */
	/* { MODKEY|ShiftMask,          XK_u,		                setlayout,	    {.v = &layouts[5]} }, /1* monocle *1/ */
	{ MODKEY,			            XK_i,		                setlayout,	    {.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		        XK_i,		                setlayout,	    {.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,                       XK_comma,                   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,         {.i = +1 } },
	{ MODKEY,			            XK_o,		                incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,		        XK_o,		                incnmaster,     {.i = -1 } },
	{ MODKEY,			            XK_backslash,	            view,		    {0} },
	{ MODKEY,			            XK_a,		                togglegaps,	    {0} },
	{ MODKEY|ShiftMask,		        XK_a,		                defaultgaps,	{0} },
	{ MODKEY,			            XK_s,		                togglesticky,	{0} },
	{ MODKEY,			            XK_space,		            togglefullscr,	{0} },
	{ MODKEY,		                XK_f,		                setlayout,	    {.v = &layouts[8]} },
	{ MODKEY,			            XK_g,		                shiftview,	    { .i = -1 } },
	{ MODKEY|ShiftMask,		        XK_g,		                shifttag,	    { .i = -1 } },
	{ MODKEY,			            XK_h,		                setmfact,	    {.f = -0.05} },
	{ MODKEY,			            XK_l,		                setmfact,      	{.f = +0.05} },
	{ MODKEY,			            XK_semicolon,	            shiftview,	    { .i = 1 } },
	{ MODKEY|ShiftMask,		        XK_semicolon,	            shifttag,	    { .i = 1 } },
	{ MODKEY,			            XK_apostrophe,	            togglescratch,	{.ui = 1} },
	{ MODKEY|ShiftMask|ControlMask, XK_Return,	                togglescratch,	{.ui = 0} },
	{ MODKEY,			            XK_z,		                incrgaps,	    {.i = +3 } },
	{ MODKEY,			            XK_x,		                incrgaps,	    {.i = -3 } },
	{ MODKEY,			            XK_b,		                togglebar,	    {0} },
	{ MODKEY,			            XK_Left,	                focusmon,	    {.i = -1 } },
	{ MODKEY|ShiftMask,		        XK_Left,	                tagmon,		    {.i = -1 } },
	{ MODKEY,			            XK_Right,	                focusmon,	    {.i = +1 } },
	{ MODKEY|ShiftMask,		        XK_Right,	                tagmon,		    {.i = +1 } },
	{ MODKEY,			            XK_Page_Up,	                shiftview,	    { .i = -1 } },
	{ MODKEY|ShiftMask,		        XK_Page_Up,	                shifttag,	    { .i = -1 } },
	{ MODKEY,			            XK_Page_Down,	            shiftview,	    { .i = +1 } },
	{ MODKEY|ShiftMask,		        XK_Page_Down,	            shifttag,	    { .i = +1 } },
	{ MODKEY|ControlMask,           XK_space,	                zoom,		    {0} },
	{ MODKEY|ShiftMask,		        XK_space,	                togglefloating,	{0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e $EDITOR ~/.local/repos/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		    MODKEY,		    Button4,	    incrgaps,	    {.i = +1} },
	{ ClkClientWin,		    MODKEY,		    Button5,	    incrgaps,	    {.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		    0,		        Button4,	    shiftview,	    {.i = -1} },
	{ ClkTagBar,		    0,		        Button5,	    shiftview,	    {.i = 1} },
	{ ClkRootWin,		    0,		        Button2,	    togglebar,	    {0} },
};
