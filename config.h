/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 2;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 5;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;     /* 0 means no systray */
static int showbar                       = 1;        /* 0 means no bar */
static int topbar                        = 1;        /* 0 means bottom bar */
static const int swallowfloating         = 0; /* 1 means swallow floating windows by default */
static const unsigned int gappih         = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv         = 10;       /* vert inner gap between windows */
static const unsigned int gappoh         = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps               = 0;        /* 1 means no outer gap when there is only one window */
static const char *fonts[]               = { "Fira Code Nerd Font:size=10", "JoyPixels:pixelsize=12:antialias=true:autohint=true" };
static const char dmenufont[]            = "Sauce Code Pro Nerd Font:size=10";
static char normbgcolor[]                = "#222222";
static char normbordercolor[]            = "#444444";
static char normfgcolor[]                = "#bbbbbb";
static char selfgcolor[]                 = "#eeeeee";
static char selbgcolor[]                 = "#157F54";
static char selbordercolor[]             = "#23D18B";

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

#define TERMINAL "alacritty"

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", "#242424", "-nf", "#ffffff", "-sb", "#303030", "-sf", "#ff8c00", NULL };
static const char *termcmd[]  = { "alacritty", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",             STRING,  &normbgcolor },
		{ "color8",             STRING,  &normbordercolor },
		{ "foreground",         STRING,  &normfgcolor },
		{ "color1",             STRING,  &selbgcolor },
		{ "color2",             STRING,  &selbordercolor },
		{ "foreground",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("librewolf") },
	{ MODKEY,                       XK_u,      spawn,          SHCMD(TERMINAL " -e paru -Syu") },
	{ Mod1Mask|ControlMask,          XK_e,      spawn,          SHCMD("emacsclient -c -n -a emacs") },
	{ Mod1Mask|ControlMask,          XK_b,      spawn,          SHCMD("emacsclient -c -n -a emacs --eval '(ibuffer)'") },
	{ Mod1Mask|ControlMask,          XK_d,      spawn,          SHCMD("emacsclient -c -n -a emacs --eval '(dired nil)'") },
	{ Mod1Mask|ControlMask,          XK_n,      spawn,          SHCMD("emacsclient -c -n -a emacs --eval '(elfeed)'") },
	{ Mod1Mask|ControlMask,          XK_s,      spawn,          SHCMD("emacsclient -c -n -a emacs --eval '(eshell)'") },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("spotify") },
	{ MODKEY|ControlMask,           XK_u,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-unicode.sh") },
	{ MODKEY|ControlMask,           XK_p,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-power.sh") },
	{ MODKEY|ControlMask,           XK_t,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-timer.sh") },
	{ MODKEY|ControlMask,           XK_s,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-ssh.sh") },
	{ MODKEY|ControlMask|ShiftMask, XK_s,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-scrot.sh") },
	{ MODKEY|ControlMask,           XK_y,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-sysmon.sh") },
	{ MODKEY|ControlMask,           XK_m,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-mount.sh") },
	{ MODKEY|ControlMask|ShiftMask, XK_m,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-umount.sh") },
	{ MODKEY|ControlMask,           XK_e,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-edit-configs.sh") },
	{ MODKEY|ControlMask,           XK_c,      spawn,          SHCMD("~/.config/dmenu/scripts/dmenu-calc.sh") },
	{ 0,                            XF86XK_AudioLowerVolume,      spawn,          SHCMD("pulsemixer --change-volume -2") },
	{ 0,                            XF86XK_AudioRaiseVolume,      spawn,          SHCMD("pulsemixer --change-volume +2 && pulsemixer --max-volume 100") },
	{ 0,                            XF86XK_AudioMicMute,          spawn,          SHCMD("pulsemixer --toggle-mute --id source-2") },
	{ 0,                            XF86XK_AudioMute,             spawn,          SHCMD("pulsemixer --toggle-mute") },
	{ 0,                            XF86XK_AudioPlay,             spawn,          SHCMD("playerctl -p spotify play-pause") },
	{ 0,                            XF86XK_AudioPrev,             spawn,          SHCMD("playerctl -p spotify previous") },
	{ 0,                            XF86XK_AudioNext,             spawn,          SHCMD("playerctl -p spotify next") },
	{ 0,                            XF86XK_AudioStop,             spawn,          SHCMD("playerctl -p spotify stop") },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  fullscreen,     {0} },
	{ MODKEY|ControlMask,           XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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
