#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
// static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */
static const char *fonts[]          = { "InputMono:size=12" };
static const char dmenufont[]       = "InputMono:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#70c0ba";
static const char col_blue[]        = "#005577";
static const char col_red[]         = "#C02519";
static const char col_maroon[]      = "#990000";
static const char col_black[]       = "#000000";
static const unsigned int baralpha    = 0x28;
static const unsigned int borderalpha = 0xa;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_cyan,  col_black, col_black },
};

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

static const char term[]        = "alacritty";
static const char webbrowser[]  = "librewolf";
static const char filemanager[] = "nautilus";

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
// static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const char *tagDefaults[] = { term, webbrowser, "onlyoffice-desktopeditors", filemanager, "spotify", "lutris", "krita", "reaper", "discord" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title          tags mask isfloating  isterminal  noswallow  monitor */
	{ "Gimp"      , NULL , NULL           , 0      , 1 , 0 , 0 , -1 } ,
	{ "DesktopEditors", NULL , NULL       , 1 << 2 , 0 , 0 , 0 , -1 } ,
	{ "Spotify"   , NULL , NULL           , 1 << 4 , 0 , 0 , 0 , -1 } ,
	{ "eog"       , NULL , NULL           , 1 << 4 , 0 , 0 , 0 , -1 } ,
	{ "vlc"       , NULL , NULL           , 1 << 4 , 0 , 0 , 0 , -1 } ,
	{ "Lutris"    , NULL , NULL           , 1 << 5 , 0 , 0 , 0 , -1 } ,
	{ "krita"     , NULL , NULL           , 1 << 6 , 0 , 0 , 0 , -1 } ,
	{ "REAPER"    , NULL , NULL           , 1 << 7 , 0 , 0 , 0 , -1 } ,
	{ "Discord"   , NULL , NULL           , 1 << 8 , 0 , 0 , 0 , -1 } ,
	{ "Caprine"   , NULL , NULL           , 1 << 8 , 0 , 0 , 0 , -1 } ,
	{ "Alacritty" , NULL , NULL           , 0      , 0 , 1 , 0 , -1 } ,
	{ "cool-retro-term" , NULL , NULL     , 0      , 0 , 1 , 0 , -1 } ,
	{ NULL        , NULL , "Event Tester" , 0      , 0 , 0 , 1 , -1 } , /* xev */
	{ NULL        , NULL , "Untitled"     , 0      , 0 , 0 , 1 , -1 } , /* Markdown Preview */
	{ NULL        , NULL , "iZotope Product Portal", 0 , 0 , 0 , 1 , -1 } ,
	{ "Peek"      , NULL , NULL           , 0      , 1 , 0 , 0 , -1 } ,
    { "krita"     , NULL , "Krita - Edit Text", 0, 1, 0, 1, -1 } ,
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask // Set the Super key
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

// Client commands
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run", "-b", "-m", "-4", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray2, "-sf", col_gray4, NULL };
static const char *rofiappmenu[]    = { "rofi", "-monitor", "-4", "-matching", "regex", "-show", "drun", "-modi", "drun", NULL };
static const char *termcmd[]        = { term, NULL };
static const char *browsercmd[]     = { webbrowser, NULL };
static const char *filemanagercmd[] = { filemanager, NULL };
static const char *screencapcmd[]   = { "flameshot", "gui", NULL };
static const char *gifcapcmd[]       = { "peek", NULL };
static const char *xkillcmd[]       = { "xkill", NULL };

// Brightness and display
static const char *autorandr[]  = { "AutoRandR", NULL };

// Audio commands
static const char *togglemuteaudio[]    = { "amixer", "-D", "pulse", "set", "Master", "1+", "toggle", NULL};
static const char *togglemutemic[]      = { "amixer", "set", "Capture", "toggle", NULL };
static const char *pulseAudioSettings[] = { "pavucontrol" };
static const char *nextsong[]           = { "playerctl", "--player=spotify", "next", NULL };
static const char *previoussong[]       = { "playerctl", "--player=spotify", "previous", NULL };
static const char *togglepausemusic[]   = { "playerctl", "--player=spotify", "play-pause", NULL };

static Key keys[] = {
	/* modifier          key         function       argument */

	// Applications
	{ MODKEY,            XK_d,       spawn,         {.v = dmenucmd } },
	{ MODKEY,            XK_e,       spawn,         {.v = rofiappmenu } },
	{ MODKEY,            XK_Return,  spawn,         {.v = termcmd } },
	{ MODKEY|ShiftMask,  XK_x,       spawnDefault,  {0} },
	{ MODKEY,            XK_x,       spawn,         {.v = xkillcmd} },
	{ MODKEY|ShiftMask,  XK_e,       spawn,         {.v = filemanagercmd } },
	{ MODKEY|ShiftMask,  XK_s,       spawn,         {.v = browsercmd } },
	{ MODKEY|ShiftMask,  XK_r,       spawn,         SHCMD("researchPapers") },
	{ 0,                 XK_Print,   spawn,         {.v = screencapcmd } },
	{ MODKEY,            XK_Print,   spawn,         {.v = gifcapcmd } },

	// Brightness
	{ 0,  XF86XK_MonBrightnessUp,    spawn,  SHCMD("light -A 5; pkill -RTMIN+11 dwmblocks") },
	{ 0,  XF86XK_MonBrightnessDown,  spawn,  SHCMD("light -U 5; pkill -RTMIN+11 dwmblocks") },

	// Audio
	{ 0,       XF86XK_AudioRaiseVolume,  spawn,  SHCMD("amixer sset Master 5%+; pkill -RTMIN+12 dwmblocks") },
	{ 0,       XF86XK_AudioLowerVolume,  spawn,  SHCMD("amixer sset Master 5%-; pkill -RTMIN+12 dwmblocks") },
	{ 0,       XF86XK_AudioMute,         spawn,  {.v = togglemuteaudio } },
	{ MODKEY,  XK_i,                     spawn,  {.v = pulseAudioSettings } },
	{ 0,       XF86XK_AudioNext,         spawn,  {.v = nextsong } },
	{ 0,       XF86XK_AudioPrev,         spawn,  {.v = previoussong } },
	{ 0,       XF86XK_AudioPlay,         spawn,  {.v = togglepausemusic } },
	{ 0,       XF86XK_AudioMicMute,      spawn,  {.v = togglemutemic } },

	// Display
	{ MODKEY,  XK_p,  spawn,  {.v = autorandr } }, // Mod+p is actually the display key on laptops

	// Main dwm stuff
	{ MODKEY,            XK_b,       togglebar,       {0} },
	{ MODKEY,            XK_j,       focusstack,      {.i = +1 } },
	{ MODKEY,            XK_k,       focusstack,      {.i = -1 } },
	{ MODKEY|ShiftMask,  XK_h,       incnmaster,      {.i = +1 } },
	{ MODKEY|ShiftMask,  XK_l,       incnmaster,      {.i = -1 } },
	{ MODKEY,            XK_h,       setmfact,        {.f = -0.05} },
	{ MODKEY,            XK_l,       setmfact,        {.f = +0.05} },
	{ MODKEY|ShiftMask,  XK_Return,  zoom,            {0} },
	{ MODKEY,            XK_Tab,     view,            {0} },
	{ MODKEY,            XK_q,       killclient,      {0} },
	{ MODKEY,            XK_c,       togglefloating,  {0} },
	{ MODKEY,            XK_f,       togglefullscr,   {0} },
	{ MODKEY,            XK_u,       focusurgent,     {0} },
	{ MODKEY,            XK_0,       view,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,  XK_0,       tag,             {.ui = ~0 } },

	// Layouts
	{ MODKEY,            XK_t,      setlayout,  {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,  XK_f,      setlayout,  {.v = &layouts[1]} },
	{ MODKEY,            XK_m,      setlayout,  {.v = &layouts[2]} },
	{ MODKEY,            XK_space,  setlayout,  {0} },

	// Monitor
	{ MODKEY,           XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,           XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask, XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon,         {.i = +1 } },

	// Tags
	TAGKEYS(            XK_1,                      0)
	TAGKEYS(            XK_2,                      1)
	TAGKEYS(            XK_3,                      2)
	TAGKEYS(            XK_4,                      3)
	TAGKEYS(            XK_5,                      4)
	TAGKEYS(            XK_6,                      5)
	TAGKEYS(            XK_7,                      6)
	TAGKEYS(            XK_8,                      7)
	TAGKEYS(            XK_9,                      8)
	{ MODKEY|ShiftMask, XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,    0,       Button1,  setlayout,       {0} },
	{ ClkLtSymbol,    0,       Button3,  setlayout,       {.v = &layouts[2]} },
	{ ClkWinTitle,    0,       Button2,  zoom,            {0} },
	{ ClkStatusText,  0,       Button2,  spawn,           {.v = termcmd } },
	{ ClkClientWin,   MODKEY,  Button1,  movemouse,       {0} },
	{ ClkClientWin,   MODKEY,  Button2,  togglefloating,  {0} },
	{ ClkClientWin,   MODKEY,  Button3,  resizemouse,     {0} },
	{ ClkTagBar,      0,       Button1,  view,            {0} },
	{ ClkTagBar,      0,       Button3,  toggleview,      {0} },
	{ ClkTagBar,      MODKEY,  Button1,  tag,             {0} },
	{ ClkTagBar,      MODKEY,  Button3,  toggletag,       {0} },
};

