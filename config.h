/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h> 

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "terminus:size=10" };
static const char dmenufont[]       = "terminus:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel] =  { col_gray4, col_cyan,  col_cyan  },
};

/* False means using the scroll wheel on a window will not change focus */
static const Bool focusonwheelscroll = False;

/* tagging */
static const char *tags[] = { "W1", "D2", "M3", "F4", "5", "6", "M7", "E8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Tilda",		NULL,       NULL,       0,		True,		-1 },
	{ "Chromium",	NULL,       NULL,       1,     	False,		-1 },
	{ "Thunar",		NULL,       NULL,       1 << 3,	False,		-1 },
	{ "Thunderbird",NULL,       NULL,       1 << 7,	False,		-1 },
	{ "Geany",		NULL,       NULL,       1 << 1,	False,		-1 },
	{ "MATLAB R2016a - academic use",NULL,       NULL,       1 << 1,       False,       -1 },
	{ "Spotify",	NULL,       NULL,       1 << 6,	False,       -1 },
	{ "Audacious",	NULL,       NULL,       1 << 6,	False,       -1 },
	{ "QtCreator",	NULL,       NULL,       1 << 1,	False,       -1 },
	{ "TeXstudio",	NULL,       NULL,       1 << 1,	False,       -1 },
	{ "Franz",		NULL,       NULL,       1 << 2,	False,       -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      monocle },
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* custom functions declarations */
static void rotatewindows(const Arg *arg);
static void tag_view(const Arg *arg);

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag_view,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *browsercmd[]  = { "chromium", NULL };
static const char *mailcmd[]  = { "thunderbird", NULL };
static const char *messagecmd[]  = { "franz-bin", NULL };
static const char *filecmd[]  = { "thunar", NULL };
static const char *spotifycmd[]  = { "spotify", NULL };
static const char *calcmd[]  = { "gsimplecal_de", NULL };
static const char *taskcmd[]  = { "st", "-e", "htop", NULL };
static const char *runcmd[]  = { "gmrun", NULL };
static const char *lockcmd[]  = { "slock", NULL };
static const char *killcmd[]  = { "xkill", NULL };

static const char *sleepcmd[]  = { "sleep_custom", NULL };

static const char *mediastopcmd[]  = { "mediakeyscript", "stop", NULL };
static const char *mediaplaycmd[]  = { "mediakeyscript", "playpause", NULL };
static const char *medianextcmd[]  = { "mediakeyscript", "next", NULL };
static const char *mediaprevcmd[]  = { "mediakeyscript", "prev", NULL };
static const char *audiotoggle[]  = { "pacon_custom", "togglemute", NULL };
static const char *audioplus[]  = { "pacon_custom", "plus", NULL };
static const char *audiominus[]  = { "pacon_custom", "minus", NULL };
//static const char *setmonitor[]  = { "setmonitor", "rotate", NULL };

static const char *wallpaper[]  = { "changewallpaper", "random", NULL };

static const char *screenshotcmd[]  = { "tscreenshot", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	
	//starter
	{ Mod4Mask,             XK_w, spawn,          {.v = browsercmd } },
	{ Mod4Mask,             XK_f, spawn,          {.v = filecmd } },
	{ Mod4Mask,             XK_t, spawn,          {.v = termcmd } },
	{ Mod4Mask,             XK_m, spawn,          {.v = mailcmd } },
	{ Mod4Mask,             XK_c, spawn,          {.v = messagecmd } },
	{ Mod4Mask,             XK_s, spawn,          {.v = spotifycmd } },
	{ Mod4Mask,             XK_l, spawn,          {.v = lockcmd } },
	{ Mod4Mask,             XK_k, spawn,          {.v = killcmd } },
	{ Mod1Mask,             XK_F2, spawn,          {.v = runcmd } },
	{ Mod4Mask,             XK_r, spawn,          {.v = wallpaper } },
	{ ControlMask|Mod1Mask,             XK_Delete, spawn,          {.v = taskcmd } },
	{ Mod1Mask,             XK_F4,      killclient,     {0} },
	
	//direct XF86
	{ 0,             XF86XK_Sleep, spawn,          {.v = sleepcmd } },
	{ 0,             XF86XK_AudioStop, spawn,          {.v = mediastopcmd } },
	{ 0,             XF86XK_AudioPlay, spawn,          {.v = mediaplaycmd } },
	{ 0,             XF86XK_AudioNext, spawn,          {.v = medianextcmd } },
	{ 0,             XF86XK_AudioPrev, spawn,          {.v = mediaprevcmd } },
	{ 0,             XF86XK_AudioMute, spawn,          {.v = audiotoggle } },
	{ 0,             XF86XK_AudioRaiseVolume, spawn,          {.v = audioplus } },
	{ 0,             XF86XK_AudioLowerVolume, spawn,          {.v = audiominus } },
//	{ 0,             XF86XK_Display, spawn,          {.v = setmonitor } },	
	{ 0,             XK_Print, spawn,          {.v = screenshotcmd } },
	
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	//{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_Tab,    rotatewindows,  {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
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
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = calcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* custom functions */
void
rotatewindows(const Arg *arg) {
	Arg arg1;
	arg1.i = -1;
	focusstack(&arg1);
	if(selmon->lt[selmon->sellt] != &layouts[2])
		zoom(0);
}

void
tag_view(const Arg *arg)
{
	if (selmon->sel && arg->ui & TAGMASK) {
		selmon->sel->tags = arg->ui & TAGMASK;
		focus(NULL);
		arrange(selmon);
		view(arg);
	}
}
