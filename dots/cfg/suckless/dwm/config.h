/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

#define SESSION_FILE "/tmp/dwm-session"

#define ICONSIZE 20    /* icon size */
#define ICONSPACING 10 /* space between icon and title */
/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int gappx = 15;   /* gaps between windows */
static const unsigned int snap = 16;    /* snap pixel */
static const int swallowfloating =
    1; /* 1 means swallow floating windows by default */
static const unsigned int systraypinning =
    0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
          X */
static const unsigned int systrayonleft =
    0; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const unsigned int systrayiconsize = 12; /* systray icon size in px */
static const int systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor*/
static const int showsystray = 1; /* 0 means no systray */
static const int showbar = 1;     /* 0 means no bar */
static const int topbar = 1;      /* 0 means bottom bar */
static const char buttonbar[] = " ";
static const int user_bh =
    8; /* 2 is the default spacing around the bar's font */
static const char *fonts[] = {"Fantasque Sans M Nerd Font:size=14"};
static const char dmenufont[] = "Fantasque Sans M Nerd Font:size=14";
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#005577";
static char selbgcolor[] = "#005577";
static char termcol0[] = "#000000";  /* black   */
static char termcol1[] = "#ff0000";  /* red     */
static char termcol2[] = "#33ff00";  /* green   */
static char termcol3[] = "#ff0099";  /* yellow  */
static char termcol4[] = "#0066ff";  /* blue    */
static char termcol5[] = "#cc00ff";  /* magenta */
static char termcol6[] = "#00ffff";  /* cyan    */
static char termcol7[] = "#d0d0d0";  /* white   */
static char termcol8[] = "#808080";  /* black   */
static char termcol9[] = "#ff0000";  /* red     */
static char termcol10[] = "#33ff00"; /* green   */
static char termcol11[] = "#ff0099"; /* yellow  */
static char termcol12[] = "#0066ff"; /* blue    */
static char termcol13[] = "#cc00ff"; /* magenta */
static char termcol14[] = "#00ffff"; /* cyan    */
static char termcol15[] = "#ffffff"; /* white   */
static char accent[] = "#00ff00";
static char *termcolor[] = {
    termcol0,  termcol1,  termcol2,  termcol3,  termcol4,  termcol5,
    termcol6,  termcol7,  termcol8,  termcol9,  termcol10, termcol11,
    termcol12, termcol13, termcol14, termcol15,
};
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
    [SchemeHid] = {selfgcolor, normbgcolor, selbordercolor},
    [SchemeButton] = {selfgcolor, normbgcolor, selbordercolor},
};
static const int horizpadbar = 5; /* horizontal padding for statusbar */
static const int vertpadbar = 5;  /* vertical padding for statusbar */

/* tagging */
static const char *tags[] = {"cmd", "www", "dev", "chat", "med"};
static const char *alttags[] = {"[cmd]", "[www]", "[dev]", "[chat]", "[med]"};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;

const char *spterm[] = {"wezterm", "start",  "--always-new-process",
                        "--class", "spterm", NULL};

static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm", spterm},
};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
    /* class     instance  title           tags mask  isfloating  isterminal
       noswallow  monitor */
    {"Gimp", NULL, NULL, 0, 1, 0, 0, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, 0, -1, -1},
    {"org.wezfurlong.wezterm", NULL, NULL, 0, 0, 1, 0, -1},
    {"st-256color", NULL, NULL, 0, 0, 1, 0, -1},
    {NULL, "spterm", NULL, SPTAG(0), 1, -1, 0, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1}, /* xev */
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-nb", normbgcolor, "-nf",
                                 normfgcolor, "-sb", selbgcolor,  "-sf",
                                 selfgcolor,  NULL};
static const char *termcmd[] = {"wezterm", NULL};

static const char *upvol[] = {"changevolume", "up", NULL};
static const char *downvol[] = {"changevolume", "down", NULL};
static const char *mutevol[] = {"changevolume", "mute", NULL};

static const char *light_up[] = {"changebrightness", "up", NULL};
static const char *light_down[] = {"changebrightness", "down", NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_v, togglescratch, {.ui = 0}},
    {MODKEY, XK_j, focusstackvis, {.i = +1}},
    {MODKEY, XK_k, focusstackvis, {.i = -1}},
    {MODKEY | ShiftMask, XK_j, focusstackhid, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, focusstackhid, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY, XK_s, show, {0}},
    {MODKEY | ShiftMask, XK_s, showall, {0}},
    {MODKEY | ShiftMask, XK_h, hide, {0}},
    {MODKEY, XK_minus, setgaps, {.i = -1}},
    {MODKEY, XK_equal, setgaps, {.i = +1}},
    {MODKEY | ShiftMask, XK_equal, setgaps, {.i = 0}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY | ControlMask | ShiftMask, XK_q, quit, {1}},
    {MODKEY | ShiftMask, XK_q, quit, {0}},
    {MODKEY, XK_F5, xrdb, {.v = NULL}},
    {0, XF86XK_AudioLowerVolume, spawn, {.v = downvol}},
    {0, XF86XK_AudioMute, spawn, {.v = mutevol}},
    {0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol}},
    {0, XF86XK_MonBrightnessUp, spawn, {.v = light_up}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = light_down}},
    {0, XK_Print, spawn,
     SHCMD("maim -s -u | xclip -selection clipboard -t image/png")},
    {ShiftMask, XK_Print, spawn,
     SHCMD("maim -u | xclip -selection clipboard -t image/png")},

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkButton, 0, Button1, spawn, {.v = dmenucmd}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button1, togglewin, {0}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
