
#ifndef bionukg_window_h
#define bionukg_window_h
#pragma once
#include <Windows.h>
#include <wingdi.h>
#include <iostream>
#include <vector>
#include <thread>

#include "./bionukg_inputsdx.h"
#include "./bionukg_graphics_.h"
#include "./bionukg_phys_eng.h"
enum wMsg : UINT
{
#ifndef NOWINMESSAGES
    WMSG_NULL = WM_NULL,
    WMSG_CREATE = WM_CREATE,
    WMSG_DESTROY = WM_DESTROY,
    WMSG_MOVE = WM_MOVE,
    WMSG_SIZE = WM_SIZE,
    WMSG_ACTIVATE = WM_ACTIVATE,
    WMSG_SETFOCUS = WM_SETFOCUS,
    WMSG_KILLFOCUS = WM_KILLFOCUS,
    WMSG_ENABLE = WM_ENABLE,
    WMSG_SETREDRAW = WM_SETREDRAW,
    WMSG_SETTEXT = WM_SETTEXT,
    WMSG_GETTEXT = WM_GETTEXT,
    WMSG_GETTEXTLENGTH = WM_GETTEXTLENGTH,
    WMSG_PAINT = WM_PAINT,
    WMSG_CLOSE = WM_CLOSE,
#ifndef _WIN32_WCE
    WMSG_QUERYENDSESSION = WM_QUERYENDSESSION,
    WMSG_QUERYOPEN = WM_QUERYOPEN,
    WMSG_ENDSESSION = WM_ENDSESSION,
#endif
    WMSG_QUIT = WM_QUIT,
    WMSG_ERASEBKGND = WM_ERASEBKGND,
    WMSG_SYSCOLORCHANGE = WM_SYSCOLORCHANGE,
    WMSG_SHOWWINDOW = WM_SHOWWINDOW,
    WMSG_WININICHANGE = WM_WININICHANGE,
#if (WINVER >= 0x0400)
    WMSG_SETTINGCHANGE = WM_SETTINGCHANGE,
#endif /* WINVER >= 0x0400 */
    WMSG_DEVMODECHANGE = WM_DEVMODECHANGE,
    WMSG_ACTIVATEAPP = WM_ACTIVATEAPP,
    WMSG_FONTCHANGE = WM_FONTCHANGE,
    WMSG_TIMECHANGE = WM_TIMECHANGE,
    WMSG_CANCELMODE = WM_CANCELMODE,
    WMSG_SETCURSOR = WM_SETCURSOR,
    WMSG_MOUSEACTIVATE = WM_MOUSEACTIVATE,
    WMSG_CHILDACTIVATE = WM_CHILDACTIVATE,
    WMSG_QUEUESYNC = WM_QUEUESYNC,
    WMSG_GETMINMAXINFO = WM_GETMINMAXINFO,
    WMSG_PAINTICON = WM_PAINTICON,
    WMSG_ICONERASEBKGND = WM_ICONERASEBKGND,
    WMSG_NEXTDLGCTL = WM_NEXTDLGCTL,
    WMSG_SPOOLERSTATUS = WM_SPOOLERSTATUS,
    WMSG_DRAWITEM = WM_DRAWITEM,
    WMSG_MEASUREITEM = WM_MEASUREITEM,
    WMSG_DELETEITEM = WM_DELETEITEM,
    WMSG_VKEYTOITEM = WM_VKEYTOITEM,
    WMSG_CHARTOITEM = WM_CHARTOITEM,
    WMSG_SETFONT = WM_SETFONT,
    WMSG_GETFONT = WM_GETFONT,
    WMSG_SETHOTKEY = WM_SETHOTKEY,
    WMSG_GETHOTKEY = WM_GETHOTKEY,
    WMSG_QUERYDRAGICON = WM_QUERYDRAGICON,
    WMSG_COMPAREITEM = WM_COMPAREITEM,
#if (WINVER >= 0x0500)
#ifndef _WIN32_WCE
    WMSG_GETOBJECT = WM_GETOBJECT,
#endif
#endif /* WINVER >= 0x0500 */
    WMSG_COMPACTING = WM_COMPACTING,
    WMSG_COMMNOTIFY = WM_COMMNOTIFY,
    WMSG_WINDOWPOSCHANGING = WM_WINDOWPOSCHANGING,
    WMSG_WINDOWPOSCHANGED = WM_WINDOWPOSCHANGED,
    WMSG_POWER = WM_POWER,
    WMSG_COPYDATA = WM_COPYDATA,
    WMSG_CANCELJOURNAL = WM_CANCELJOURNAL,
#if (WINVER >= 0x0400)
    WMSG_NOTIFY = WM_NOTIFY,
    WMSG_INPUTLANGCHANGEREQUEST = WM_INPUTLANGCHANGEREQUEST,
    WMSG_INPUTLANGCHANGE = WM_INPUTLANGCHANGE,
    WMSG_TCARD = WM_TCARD,
    WMSG_HELP = WM_HELP,
    WMSG_USERCHANGED = WM_USERCHANGED,
    WMSG_NOTIFYFORMAT = WM_NOTIFYFORMAT,
    WMSG_CONTEXTMENU = WM_CONTEXTMENU,
    WMSG_STYLECHANGING = WM_STYLECHANGING,
    WMSG_STYLECHANGED = WM_STYLECHANGED,
    WMSG_DISPLAYCHANGE = WM_DISPLAYCHANGE,
    WMSG_GETICON = WM_GETICON,
    WMSG_SETICON = WM_SETICON,
#endif /* WINVER >= 0x0400 */
    WMSG_NCCREATE = WM_NCCREATE,
    WMSG_NCDESTROY = WM_NCDESTROY,
    WMSG_NCCALCSIZE = WM_NCCALCSIZE,
    WMSG_NCHITTEST = WM_NCHITTEST,
    WMSG_NCPAINT = WM_NCPAINT,
    WMSG_NCACTIVATE = WM_NCACTIVATE,
    WMSG_GETDLGCODE = WM_GETDLGCODE,
#ifndef _WIN32_WCE
    WMSG_SYNCPAINT = WM_SYNCPAINT,
#endif
    WMSG_NCMOUSEMOVE = WM_NCMOUSEMOVE,
    WMSG_NCLBUTTONDOWN = WM_NCLBUTTONDOWN,
    WMSG_NCLBUTTONUP = WM_NCLBUTTONUP,
    WMSG_NCLBUTTONDBLCLK = WM_NCLBUTTONDBLCLK,
    WMSG_NCRBUTTONDOWN = WM_NCRBUTTONDOWN,
    WMSG_NCRBUTTONUP = WM_NCRBUTTONUP,
    WMSG_NCRBUTTONDBLCLK = WM_NCRBUTTONDBLCLK,
    WMSG_NCMBUTTONDOWN = WM_NCMBUTTONDOWN,
    WMSG_NCMBUTTONUP = WM_NCMBUTTONUP,
    WMSG_NCMBUTTONDBLCLK = WM_NCMBUTTONDBLCLK,
#if (_WIN32_WINNT >= 0x0500)
    WMSG_NCXBUTTONDOWN = WM_NCXBUTTONDOWN,
    WMSG_NCXBUTTONUP = WM_NCXBUTTONUP,
    WMSG_NCXBUTTONDBLCLK = WM_NCXBUTTONDBLCLK,
#endif /* _WIN32_WINNT >= 0x0500 */
#if (_WIN32_WINNT >= 0x0501)
    WMSG_INPUT_DEVICE_CHANGE = WM_INPUT_DEVICE_CHANGE,
#endif /* _WIN32_WINNT >= 0x0501 */
#if (_WIN32_WINNT >= 0x0501)
    WMSG_INPUT = WM_INPUT,
#endif /* _WIN32_WINNT >= 0x0501 */
    WMSG_KEYFIRST = WM_KEYFIRST,
    WMSG_KEYDOWN = WM_KEYDOWN,
    WMSG_KEYUP = WM_KEYUP,
    WMSG_CHAR = WM_CHAR,
    WMSG_DEADCHAR = WM_DEADCHAR,
    WMSG_SYSKEYDOWN = WM_SYSKEYDOWN,
    WMSG_SYSKEYUP = WM_SYSKEYUP,
    WMSG_SYSCHAR = WM_SYSCHAR,
    WMSG_SYSDEADCHAR = WM_SYSDEADCHAR,
#if (_WIN32_WINNT >= 0x0501)
    WMSG_UNICHAR = WM_UNICHAR,
    WMSG_KEYLAST = WM_KEYLAST,
#else
    WMSG_KEYLAST = WM_KEYLAST,
#endif /* _WIN32_WINNT >= 0x0501 */
#if (WINVER >= 0x0400)
    WMSG_IME_STARTCOMPOSITION = WM_IME_STARTCOMPOSITION,
    WMSG_IME_ENDCOMPOSITION = WM_IME_ENDCOMPOSITION,
    WMSG_IME_COMPOSITION = WM_IME_COMPOSITION,
    WMSG_IME_KEYLAST = WM_IME_KEYLAST,
#endif /* WINVER >= 0x0400 */
    WMSG_INITDIALOG = WM_INITDIALOG,
    WMSG_COMMAND = WM_COMMAND,
    WMSG_SYSCOMMAND = WM_SYSCOMMAND,
    WMSG_TIMER = WM_TIMER,
    WMSG_HSCROLL = WM_HSCROLL,
    WMSG_VSCROLL = WM_VSCROLL,
    WMSG_INITMENU = WM_INITMENU,
    WMSG_INITMENUPOPUP = WM_INITMENUPOPUP,
#if (WINVER >= 0x0601)
    WMSG_GESTURE = WM_GESTURE,
    WMSG_GESTURENOTIFY = WM_GESTURENOTIFY,
#endif /* WINVER >= 0x0601 */
    WMSG_MENUSELECT = WM_MENUSELECT,
    WMSG_MENUCHAR = WM_MENUCHAR,
    WMSG_ENTERIDLE = WM_ENTERIDLE,
#if (WINVER >= 0x0500)
#ifndef _WIN32_WCE
    WMSG_MENURBUTTONUP = WM_MENURBUTTONUP,
    WMSG_MENUDRAG = WM_MENUDRAG,
    WMSG_MENUGETOBJECT = WM_MENUGETOBJECT,
    WMSG_UNINITMENUPOPUP = WM_UNINITMENUPOPUP,
    WMSG_MENUCOMMAND = WM_MENUCOMMAND,
#ifndef _WIN32_WCE
#if (_WIN32_WINNT >= 0x0500)
    WMSG_CHANGEUISTATE = WM_CHANGEUISTATE,
    WMSG_UPDATEUISTATE = WM_UPDATEUISTATE,
    WMSG_QUERYUISTATE = WM_QUERYUISTATE,
#endif /* _WIN32_WINNT >= 0x0500 */
#endif
#endif
#endif /* WINVER >= 0x0500 */
    WMSG_CTLCOLORMSGBOX = WM_CTLCOLORMSGBOX,
    WMSG_CTLCOLOREDIT = WM_CTLCOLOREDIT,
    WMSG_CTLCOLORLISTBOX = WM_CTLCOLORLISTBOX,
    WMSG_CTLCOLORBTN = WM_CTLCOLORBTN,
    WMSG_CTLCOLORDLG = WM_CTLCOLORDLG,
    WMSG_CTLCOLORSCROLLBAR = WM_CTLCOLORSCROLLBAR,
    WMSG_CTLCOLORSTATIC = WM_CTLCOLORSTATIC,
    WMSG_MOUSEFIRST = WM_MOUSEFIRST,
    WMSG_MOUSEMOVE = WM_MOUSEMOVE,
    WMSG_LBUTTONDOWN = WM_LBUTTONDOWN,
    WMSG_LBUTTONUP = WM_LBUTTONUP,
    WMSG_LBUTTONDBLCLK = WM_LBUTTONDBLCLK,
    WMSG_RBUTTONDOWN = WM_RBUTTONDOWN,
    WMSG_RBUTTONUP = WM_RBUTTONUP,
    WMSG_RBUTTONDBLCLK = WM_RBUTTONDBLCLK,
    WMSG_MBUTTONDOWN = WM_MBUTTONDOWN,
    WMSG_MBUTTONUP = WM_MBUTTONUP,
    WMSG_MBUTTONDBLCLK = WM_MBUTTONDBLCLK,
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
    WMSG_MOUSEWHEEL = WM_MOUSEWHEEL,
#endif
#if (_WIN32_WINNT >= 0x0500)
    WMSG_XBUTTONDOWN = WM_XBUTTONDOWN,
    WMSG_XBUTTONUP = WM_XBUTTONUP,
    WMSG_XBUTTONDBLCLK = WM_XBUTTONDBLCLK,
#endif
#if (_WIN32_WINNT >= 0x0600)
    WMSG_MOUSEHWHEEL = WM_MOUSEHWHEEL,
#endif
#if (_WIN32_WINNT >= 0x0600)
    WMSG_MOUSELAST = WM_MOUSELAST,
#elif (_WIN32_WINNT >= 0x0500)
    WMSG_MOUSELAST = WM_MOUSELAST,
#elif (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
    WMSG_MOUSELAST = WM_MOUSELAST,
#else
    WMSG_MOUSELAST = WM_MOUSELAST,
#endif /* (_WIN32_WINNT >= 0x0600) */
    WMSG_PARENTNOTIFY = WM_PARENTNOTIFY,
    WMSG_ENTERMENULOOP = WM_ENTERMENULOOP,
    WMSG_EXITMENULOOP = WM_EXITMENULOOP,
#if (WINVER >= 0x0400)
    WMSG_NEXTMENU = WM_NEXTMENU,
    WMSG_SIZING = WM_SIZING,
    WMSG_CAPTURECHANGED = WM_CAPTURECHANGED,
    WMSG_MOVING = WM_MOVING,
#endif /* WINVER >= 0x0400 */
#if (WINVER >= 0x0400)
    WMSG_POWERBROADCAST = WM_POWERBROADCAST,
#endif /* WINVER >= 0x0400 */
#if (WINVER >= 0x0400)
    WMSG_DEVICECHANGE = WM_DEVICECHANGE,
#endif /* WINVER >= 0x0400 */
    WMSG_MDICREATE = WM_MDICREATE,
    WMSG_MDIDESTROY = WM_MDIDESTROY,
    WMSG_MDIACTIVATE = WM_MDIACTIVATE,
    WMSG_MDIRESTORE = WM_MDIRESTORE,
    WMSG_MDINEXT = WM_MDINEXT,
    WMSG_MDIMAXIMIZE = WM_MDIMAXIMIZE,
    WMSG_MDITILE = WM_MDITILE,
    WMSG_MDICASCADE = WM_MDICASCADE,
    WMSG_MDIICONARRANGE = WM_MDIICONARRANGE,
    WMSG_MDIGETACTIVE = WM_MDIGETACTIVE,
    WMSG_MDISETMENU = WM_MDISETMENU,
    WMSG_ENTERSIZEMOVE = WM_ENTERSIZEMOVE,
    WMSG_EXITSIZEMOVE = WM_EXITSIZEMOVE,
    WMSG_DROPFILES = WM_DROPFILES,
    WMSG_MDIREFRESHMENU = WM_MDIREFRESHMENU,
#if (WINVER >= 0x0602)
    WMSG_POINTERDEVICECHANGE = WM_POINTERDEVICECHANGE,
    WMSG_POINTERDEVICEINRANGE = WM_POINTERDEVICEINRANGE,
    WMSG_POINTERDEVICEOUTOFRANGE = WM_POINTERDEVICEOUTOFRANGE,
#endif /* WINVER >= 0x0602 */
#if (WINVER >= 0x0601)
    WMSG_TOUCH = WM_TOUCH,
#endif /* WINVER >= 0x0601 */
#if (WINVER >= 0x0602)
    WMSG_NCPOINTERUPDATE = WM_NCPOINTERUPDATE,
    WMSG_NCPOINTERDOWN = WM_NCPOINTERDOWN,
    WMSG_NCPOINTERUP = WM_NCPOINTERUP,
    WMSG_POINTERUPDATE = WM_POINTERUPDATE,
    WMSG_POINTERDOWN = WM_POINTERDOWN,
    WMSG_POINTERUP = WM_POINTERUP,
    WMSG_POINTERENTER = WM_POINTERENTER,
    WMSG_POINTERLEAVE = WM_POINTERLEAVE,
    WMSG_POINTERACTIVATE = WM_POINTERACTIVATE,
    WMSG_POINTERCAPTURECHANGED = WM_POINTERCAPTURECHANGED,
    WMSG_TOUCHHITTESTING = WM_TOUCHHITTESTING,
    WMSG_POINTERWHEEL = WM_POINTERWHEEL,
    WMSG_POINTERHWHEEL = WM_POINTERHWHEEL,
    WMSG_POINTERROUTEDTO = WM_POINTERROUTEDTO,
    WMSG_POINTERROUTEDAWAY = WM_POINTERROUTEDAWAY,
    WMSG_POINTERROUTEDRELEASED = WM_POINTERROUTEDRELEASED,
#endif /* WINVER >= 0x0602 */
#if (WINVER >= 0x0400)
    WMSG_IME_SETCONTEXT = WM_IME_SETCONTEXT,
    WMSG_IME_NOTIFY = WM_IME_NOTIFY,
    WMSG_IME_CONTROL = WM_IME_CONTROL,
    WMSG_IME_COMPOSITIONFULL = WM_IME_COMPOSITIONFULL,
    WMSG_IME_SELECT = WM_IME_SELECT,
    WMSG_IME_CHAR = WM_IME_CHAR,
#endif /* WINVER >= 0x0400 */
#if (WINVER >= 0x0500)
    WMSG_IME_REQUEST = WM_IME_REQUEST,
#endif /* WINVER >= 0x0500 */
#if (WINVER >= 0x0400)
    WMSG_IME_KEYDOWN = WM_IME_KEYDOWN,
    WMSG_IME_KEYUP = WM_IME_KEYUP,
#endif /* WINVER >= 0x0400 */
#if ((_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500))
    WMSG_MOUSEHOVER = WM_MOUSEHOVER,
    WMSG_MOUSELEAVE = WM_MOUSELEAVE,
#endif
#if (WINVER >= 0x0500)
    WMSG_NCMOUSEHOVER = WM_NCMOUSEHOVER,
    WMSG_NCMOUSELEAVE = WM_NCMOUSELEAVE,
#endif /* WINVER >= 0x0500 */
#if (_WIN32_WINNT >= 0x0501)
    WMSG_WTSSESSION_CHANGE = WM_WTSSESSION_CHANGE,
    WMSG_TABLET_FIRST = WM_TABLET_FIRST,
    WMSG_TABLET_LAST = WM_TABLET_LAST,
#endif /* _WIN32_WINNT >= 0x0501 */
#if (WINVER >= 0x0601)
    WMSG_DPICHANGED = WM_DPICHANGED,
#endif /* WINVER >= 0x0601 */
#if (WINVER >= 0x0605)
    WMSG_DPICHANGED_BEFOREPARENT = WM_DPICHANGED_BEFOREPARENT,
    WMSG_DPICHANGED_AFTERPARENT = WM_DPICHANGED_AFTERPARENT,
    WMSG_GETDPISCALEDSIZE = WM_GETDPISCALEDSIZE,
#endif /* WINVER >= 0x0605 */
    WMSG_CUT = WM_CUT,
    WMSG_COPY = WM_COPY,
    WMSG_PASTE = WM_PASTE,
    WMSG_CLEAR = WM_CLEAR,
    WMSG_UNDO = WM_UNDO,
    WMSG_RENDERFORMAT = WM_RENDERFORMAT,
    WMSG_RENDERALLFORMATS = WM_RENDERALLFORMATS,
    WMSG_DESTROYCLIPBOARD = WM_DESTROYCLIPBOARD,
    WMSG_DRAWCLIPBOARD = WM_DRAWCLIPBOARD,
    WMSG_PAINTCLIPBOARD = WM_PAINTCLIPBOARD,
    WMSG_VSCROLLCLIPBOARD = WM_VSCROLLCLIPBOARD,
    WMSG_SIZECLIPBOARD = WM_SIZECLIPBOARD,
    WMSG_ASKCBFORMATNAME = WM_ASKCBFORMATNAME,
    WMSG_CHANGECBCHAIN = WM_CHANGECBCHAIN,
    WMSG_HSCROLLCLIPBOARD = WM_HSCROLLCLIPBOARD,
    WMSG_QUERYNEWPALETTE = WM_QUERYNEWPALETTE,
    WMSG_PALETTEISCHANGING = WM_PALETTEISCHANGING,
    WMSG_PALETTECHANGED = WM_PALETTECHANGED,
    WMSG_HOTKEY = WM_HOTKEY,
#if (WINVER >= 0x0400)
    WMSG_PRINT = WM_PRINT,
    WMSG_PRINTCLIENT = WM_PRINTCLIENT,
#endif /* WINVER >= 0x0400 */
#if (_WIN32_WINNT >= 0x0500)
    WMSG_APPCOMMAND = WM_APPCOMMAND,
#endif /* _WIN32_WINNT >= 0x0500 */
#if (_WIN32_WINNT >= 0x0501)
    WMSG_THEMECHANGED = WM_THEMECHANGED,
#endif /* _WIN32_WINNT >= 0x0501 */
#if (_WIN32_WINNT >= 0x0501)
    WMSG_CLIPBOARDUPDATE = WM_CLIPBOARDUPDATE,
#endif /* _WIN32_WINNT >= 0x0501 */
#if (_WIN32_WINNT >= 0x0600)
    WMSG_DWMCOMPOSITIONCHANGED = WM_DWMCOMPOSITIONCHANGED,
    WMSG_DWMNCRENDERINGCHANGED = WM_DWMNCRENDERINGCHANGED,
    WMSG_DWMCOLORIZATIONCOLORCHANGED = WM_DWMCOLORIZATIONCOLORCHANGED,
    WMSG_DWMWINDOWMAXIMIZEDCHANGE = WM_DWMWINDOWMAXIMIZEDCHANGE,
#endif /* _WIN32_WINNT >= 0x0600 */
#if (_WIN32_WINNT >= 0x0601)
    WMSG_DWMSENDICONICTHUMBNAIL = WM_DWMSENDICONICTHUMBNAIL,
    WMSG_DWMSENDICONICLIVEPREVIEWBITMAP = WM_DWMSENDICONICLIVEPREVIEWBITMAP,
#endif /* _WIN32_WINNT >= 0x0601 */
#if (WINVER >= 0x0600)
    WMSG_GETTITLEBARINFOEX = WM_GETTITLEBARINFOEX,
#endif /* WINVER >= 0x0600 */
#if (WINVER >= 0x0400)
    WMSG_HANDHELDFIRST = WM_HANDHELDFIRST,
    WMSG_HANDHELDLAST = WM_HANDHELDLAST,
    WMSG_AFXFIRST = WM_AFXFIRST,
    WMSG_AFXLAST = WM_AFXLAST,
#endif /* WINVER >= 0x0400 */
    WMSG_PENWINFIRST = WM_PENWINFIRST,
    WMSG_PENWINLAST = WM_PENWINLAST,
#if (WINVER >= 0x0400)
    WMSG_APP = WM_APP,
#endif /* WINVER >= 0x0400 */
    WMSG_USER = WM_USER,
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#ifdef UNICODE
#else
#endif // !UNICODE
#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#endif /* !NOWINMESSAGES */

};

LPCSTR wMsg2str(wMsg wmsg)
{
    static char buf[32] = {};

    switch (wmsg)
    {
    case WMSG_NULL:
        return "WMSG_NULL";
        break;
    case WMSG_CREATE:
        return "WMSG_CREATE";
        break;
    case WMSG_DESTROY:
        return "WMSG_DESTROY";
        break;
    case WMSG_MOVE:
        return "WMSG_MOVE";
        break;
    case WMSG_SIZE:
        return "WMSG_SIZE";
        break;
    case WMSG_ACTIVATE:
        return "WMSG_ACTIVATE";
        break;
    case WMSG_SETFOCUS:
        return "WMSG_SETFOCUS";
        break;
    case WMSG_KILLFOCUS:
        return "WMSG_KILLFOCUS";
        break;
    case WMSG_ENABLE:
        return "WMSG_ENABLE";
        break;
    case WMSG_SETREDRAW:
        return "WMSG_SETREDRAW";
        break;
    case WMSG_SETTEXT:
        return "WMSG_SETTEXT";
        break;
    case WMSG_GETTEXT:
        return "WMSG_GETTEXT";
        break;
    case WMSG_GETTEXTLENGTH:
        return "WMSG_GETTEXTLENGTH";
        break;
    case WMSG_PAINT:
        return "WMSG_PAINT";
        break;
    case WMSG_CLOSE:
        return "WMSG_CLOSE";
        break;
    case WMSG_QUERYENDSESSION:
        return "WMSG_QUERYENDSESSION";
        break;
    case WMSG_QUERYOPEN:
        return "WMSG_QUERYOPEN";
        break;
    case WMSG_ENDSESSION:
        return "WMSG_ENDSESSION";
        break;
    case WMSG_QUIT:
        return "WMSG_QUIT";
        break;
    case WMSG_ERASEBKGND:
        return "WMSG_ERASEBKGND";
        break;
    case WMSG_SYSCOLORCHANGE:
        return "WMSG_SYSCOLORCHANGE";
        break;
    case WMSG_SHOWWINDOW:
        return "WMSG_SHOWWINDOW";
        break;
    case WMSG_WININICHANGE:
        return "WMSG_WININICHANGE";
        break;

    case WMSG_DEVMODECHANGE:
        return "WMSG_DEVMODECHANGE";
        break;
    case WMSG_ACTIVATEAPP:
        return "WMSG_ACTIVATEAPP";
        break;
    case WMSG_FONTCHANGE:
        return "WMSG_FONTCHANGE";
        break;
    case WMSG_TIMECHANGE:
        return "WMSG_TIMECHANGE";
        break;
    case WMSG_CANCELMODE:
        return "WMSG_CANCELMODE";
        break;
    case WMSG_SETCURSOR:
        return "WMSG_SETCURSOR";
        break;
    case WMSG_MOUSEACTIVATE:
        return "WMSG_MOUSEACTIVATE";
        break;
    case WMSG_CHILDACTIVATE:
        return "WMSG_CHILDACTIVATE";
        break;
    case WMSG_QUEUESYNC:
        return "WMSG_QUEUESYNC";
        break;
    case WMSG_GETMINMAXINFO:
        return "WMSG_GETMINMAXINFO";
        break;
    case WMSG_PAINTICON:
        return "WMSG_PAINTICON";
        break;
    case WMSG_ICONERASEBKGND:
        return "WMSG_ICONERASEBKGND";
        break;
    case WMSG_NEXTDLGCTL:
        return "WMSG_NEXTDLGCTL";
        break;
    case WMSG_SPOOLERSTATUS:
        return "WMSG_SPOOLERSTATUS";
        break;
    case WMSG_DRAWITEM:
        return "WMSG_DRAWITEM";
        break;
    case WMSG_MEASUREITEM:
        return "WMSG_MEASUREITEM";
        break;
    case WMSG_DELETEITEM:
        return "WMSG_DELETEITEM";
        break;
    case WMSG_VKEYTOITEM:
        return "WMSG_VKEYTOITEM";
        break;
    case WMSG_CHARTOITEM:
        return "WMSG_CHARTOITEM";
        break;
    case WMSG_SETFONT:
        return "WMSG_SETFONT";
        break;
    case WMSG_GETFONT:
        return "WMSG_GETFONT";
        break;
    case WMSG_SETHOTKEY:
        return "WMSG_SETHOTKEY";
        break;
    case WMSG_GETHOTKEY:
        return "WMSG_GETHOTKEY";
        break;
    case WMSG_QUERYDRAGICON:
        return "WMSG_QUERYDRAGICON";
        break;
    case WMSG_COMPAREITEM:
        return "WMSG_COMPAREITEM";
        break;
    case WMSG_GETOBJECT:
        return "WMSG_GETOBJECT";
        break;
    case WMSG_COMPACTING:
        return "WMSG_COMPACTING";
        break;
    case WMSG_COMMNOTIFY:
        return "WMSG_COMMNOTIFY";
        break;
    case WMSG_WINDOWPOSCHANGING:
        return "WMSG_WINDOWPOSCHANGING";
        break;
    case WMSG_WINDOWPOSCHANGED:
        return "WMSG_WINDOWPOSCHANGED";
        break;
    case WMSG_POWER:
        return "WMSG_POWER";
        break;
    case WMSG_COPYDATA:
        return "WMSG_COPYDATA";
        break;
    case WMSG_CANCELJOURNAL:
        return "WMSG_CANCELJOURNAL";
        break;
    case WMSG_NOTIFY:
        return "WMSG_NOTIFY";
        break;
    case WMSG_INPUTLANGCHANGEREQUEST:
        return "WMSG_INPUTLANGCHANGEREQUEST";
        break;
    case WMSG_INPUTLANGCHANGE:
        return "WMSG_INPUTLANGCHANGE";
        break;
    case WMSG_TCARD:
        return "WMSG_TCARD";
        break;
    case WMSG_HELP:
        return "WMSG_HELP";
        break;
    case WMSG_USERCHANGED:
        return "WMSG_USERCHANGED";
        break;
    case WMSG_NOTIFYFORMAT:
        return "WMSG_NOTIFYFORMAT";
        break;
    case WMSG_CONTEXTMENU:
        return "WMSG_CONTEXTMENU";
        break;
    case WMSG_STYLECHANGING:
        return "WMSG_STYLECHANGING";
        break;
    case WMSG_STYLECHANGED:
        return "WMSG_STYLECHANGED";
        break;
    case WMSG_DISPLAYCHANGE:
        return "WMSG_DISPLAYCHANGE";
        break;
    case WMSG_GETICON:
        return "WMSG_GETICON";
        break;
    case WMSG_SETICON:
        return "WMSG_SETICON";
        break;
    case WMSG_NCCREATE:
        return "WMSG_NCCREATE";
        break;
    case WMSG_NCDESTROY:
        return "WMSG_NCDESTROY";
        break;
    case WMSG_NCCALCSIZE:
        return "WMSG_NCCALCSIZE";
        break;
    case WMSG_NCHITTEST:
        return "WMSG_NCHITTEST";
        break;
    case WMSG_NCPAINT:
        return "WMSG_NCPAINT";
        break;
    case WMSG_NCACTIVATE:
        return "WMSG_NCACTIVATE";
        break;
    case WMSG_GETDLGCODE:
        return "WMSG_GETDLGCODE";
        break;
    case WMSG_SYNCPAINT:
        return "WMSG_SYNCPAINT";
        break;
    case WMSG_NCMOUSEMOVE:
        return "WMSG_NCMOUSEMOVE";
        break;
    case WMSG_NCLBUTTONDOWN:
        return "WMSG_NCLBUTTONDOWN";
        break;
    case WMSG_NCLBUTTONUP:
        return "WMSG_NCLBUTTONUP";
        break;
    case WMSG_NCLBUTTONDBLCLK:
        return "WMSG_NCLBUTTONDBLCLK";
        break;
    case WMSG_NCRBUTTONDOWN:
        return "WMSG_NCRBUTTONDOWN";
        break;
    case WMSG_NCRBUTTONUP:
        return "WMSG_NCRBUTTONUP";
        break;
    case WMSG_NCRBUTTONDBLCLK:
        return "WMSG_NCRBUTTONDBLCLK";
        break;
    case WMSG_NCMBUTTONDOWN:
        return "WMSG_NCMBUTTONDOWN";
        break;
    case WMSG_NCMBUTTONUP:
        return "WMSG_NCMBUTTONUP";
        break;
    case WMSG_NCMBUTTONDBLCLK:
        return "WMSG_NCMBUTTONDBLCLK";
        break;
    case WMSG_NCXBUTTONDOWN:
        return "WMSG_NCXBUTTONDOWN";
        break;
    case WMSG_NCXBUTTONUP:
        return "WMSG_NCXBUTTONUP";
        break;
    case WMSG_NCXBUTTONDBLCLK:
        return "WMSG_NCXBUTTONDBLCLK";
        break;
    case WMSG_INPUT_DEVICE_CHANGE:
        return "WMSG_INPUT_DEVICE_CHANGE";
        break;
    case WMSG_INPUT:
        return "WMSG_INPUT";
        break;
    case WMSG_KEYFIRST:
        return "WMSG_KEYFIRST";
        break;

    case WMSG_KEYUP:
        return "WMSG_KEYUP";
        break;
    case WMSG_CHAR:
        return "WMSG_CHAR";
        break;
    case WMSG_DEADCHAR:
        return "WMSG_DEADCHAR";
        break;
    case WMSG_SYSKEYDOWN:
        return "WMSG_SYSKEYDOWN";
        break;
    case WMSG_SYSKEYUP:
        return "WMSG_SYSKEYUP";
        break;
    case WMSG_SYSCHAR:
        return "WMSG_SYSCHAR";
        break;
    case WMSG_SYSDEADCHAR:
        return "WMSG_SYSDEADCHAR";
        break;
    case WMSG_UNICHAR:
        return "WMSG_UNICHAR";
        break;

    case WMSG_IME_STARTCOMPOSITION:
        return "WMSG_IME_STARTCOMPOSITION";
        break;
    case WMSG_IME_ENDCOMPOSITION:
        return "WMSG_IME_ENDCOMPOSITION";
        break;
    case WMSG_IME_COMPOSITION:
        return "WMSG_IME_COMPOSITION";
        break;

    case WMSG_INITDIALOG:
        return "WMSG_INITDIALOG";
        break;
    case WMSG_COMMAND:
        return "WMSG_COMMAND";
        break;
    case WMSG_SYSCOMMAND:
        return "WMSG_SYSCOMMAND";
        break;
    case WMSG_TIMER:
        return "WMSG_TIMER";
        break;
    case WMSG_HSCROLL:
        return "WMSG_HSCROLL";
        break;
    case WMSG_VSCROLL:
        return "WMSG_VSCROLL";
        break;
    case WMSG_INITMENU:
        return "WMSG_INITMENU";
        break;
    case WMSG_INITMENUPOPUP:
        return "WMSG_INITMENUPOPUP";
        break;
    case WMSG_GESTURE:
        return "WMSG_GESTURE";
        break;
    case WMSG_GESTURENOTIFY:
        return "WMSG_GESTURENOTIFY";
        break;
    case WMSG_MENUSELECT:
        return "WMSG_MENUSELECT";
        break;
    case WMSG_MENUCHAR:
        return "WMSG_MENUCHAR";
        break;
    case WMSG_ENTERIDLE:
        return "WMSG_ENTERIDLE";
        break;
    case WMSG_MENURBUTTONUP:
        return "WMSG_MENURBUTTONUP";
        break;
    case WMSG_MENUDRAG:
        return "WMSG_MENUDRAG";
        break;
    case WMSG_MENUGETOBJECT:
        return "WMSG_MENUGETOBJECT";
        break;
    case WMSG_UNINITMENUPOPUP:
        return "WMSG_UNINITMENUPOPUP";
        break;
    case WMSG_MENUCOMMAND:
        return "WMSG_MENUCOMMAND";
        break;
    case WMSG_CHANGEUISTATE:
        return "WMSG_CHANGEUISTATE";
        break;
    case WMSG_UPDATEUISTATE:
        return "WMSG_UPDATEUISTATE";
        break;
    case WMSG_QUERYUISTATE:
        return "WMSG_QUERYUISTATE";
        break;
    case WMSG_CTLCOLORMSGBOX:
        return "WMSG_CTLCOLORMSGBOX";
        break;
    case WMSG_CTLCOLOREDIT:
        return "WMSG_CTLCOLOREDIT";
        break;
    case WMSG_CTLCOLORLISTBOX:
        return "WMSG_CTLCOLORLISTBOX";
        break;
    case WMSG_CTLCOLORBTN:
        return "WMSG_CTLCOLORBTN";
        break;
    case WMSG_CTLCOLORDLG:
        return "WMSG_CTLCOLORDLG";
        break;
    case WMSG_CTLCOLORSCROLLBAR:
        return "WMSG_CTLCOLORSCROLLBAR";
        break;
    case WMSG_CTLCOLORSTATIC:
        return "WMSG_CTLCOLORSTATIC";
        break;
    case WMSG_MOUSEFIRST:
        return "WMSG_MOUSEFIRST";
        break;

    case WMSG_LBUTTONDOWN:
        return "WMSG_LBUTTONDOWN";
        break;
    case WMSG_LBUTTONUP:
        return "WMSG_LBUTTONUP";
        break;
    case WMSG_LBUTTONDBLCLK:
        return "WMSG_LBUTTONDBLCLK";
        break;
    case WMSG_RBUTTONDOWN:
        return "WMSG_RBUTTONDOWN";
        break;
    case WMSG_RBUTTONUP:
        return "WMSG_RBUTTONUP";
        break;
    case WMSG_RBUTTONDBLCLK:
        return "WMSG_RBUTTONDBLCLK";
        break;
    case WMSG_MBUTTONDOWN:
        return "WMSG_MBUTTONDOWN";
        break;
    case WMSG_MBUTTONUP:
        return "WMSG_MBUTTONUP";
        break;
    case WMSG_MBUTTONDBLCLK:
        return "WMSG_MBUTTONDBLCLK";
        break;
    case WMSG_MOUSEWHEEL:
        return "WMSG_MOUSEWHEEL";
        break;
    case WMSG_XBUTTONDOWN:
        return "WMSG_XBUTTONDOWN";
        break;
    case WMSG_XBUTTONUP:
        return "WMSG_XBUTTONUP";
        break;
    case WMSG_XBUTTONDBLCLK:
        return "WMSG_XBUTTONDBLCLK";
        break;
    case WMSG_MOUSEHWHEEL:
        return "WMSG_MOUSEHWHEEL";
        break;

    case WMSG_PARENTNOTIFY:
        return "WMSG_PARENTNOTIFY";
        break;
    case WMSG_ENTERMENULOOP:
        return "WMSG_ENTERMENULOOP";
        break;
    case WMSG_EXITMENULOOP:
        return "WMSG_EXITMENULOOP";
        break;
    case WMSG_NEXTMENU:
        return "WMSG_NEXTMENU";
        break;
    case WMSG_SIZING:
        return "WMSG_SIZING";
        break;
    case WMSG_CAPTURECHANGED:
        return "WMSG_CAPTURECHANGED";
        break;
    case WMSG_MOVING:
        return "WMSG_MOVING";
        break;
    case WMSG_POWERBROADCAST:
        return "WMSG_POWERBROADCAST";
        break;
    case WMSG_DEVICECHANGE:
        return "WMSG_DEVICECHANGE";
        break;
    case WMSG_MDICREATE:
        return "WMSG_MDICREATE";
        break;
    case WMSG_MDIDESTROY:
        return "WMSG_MDIDESTROY";
        break;
    case WMSG_MDIACTIVATE:
        return "WMSG_MDIACTIVATE";
        break;
    case WMSG_MDIRESTORE:
        return "WMSG_MDIRESTORE";
        break;
    case WMSG_MDINEXT:
        return "WMSG_MDINEXT";
        break;
    case WMSG_MDIMAXIMIZE:
        return "WMSG_MDIMAXIMIZE";
        break;
    case WMSG_MDITILE:
        return "WMSG_MDITILE";
        break;
    case WMSG_MDICASCADE:
        return "WMSG_MDICASCADE";
        break;
    case WMSG_MDIICONARRANGE:
        return "WMSG_MDIICONARRANGE";
        break;
    case WMSG_MDIGETACTIVE:
        return "WMSG_MDIGETACTIVE";
        break;
    case WMSG_MDISETMENU:
        return "WMSG_MDISETMENU";
        break;
    case WMSG_ENTERSIZEMOVE:
        return "WMSG_ENTERSIZEMOVE";
        break;
    case WMSG_EXITSIZEMOVE:
        return "WMSG_EXITSIZEMOVE";
        break;
    case WMSG_DROPFILES:
        return "WMSG_DROPFILES";
        break;
    case WMSG_MDIREFRESHMENU:
        return "WMSG_MDIREFRESHMENU";
        break;
    case WMSG_POINTERDEVICECHANGE:
        return "WMSG_POINTERDEVICECHANGE";
        break;
    case WMSG_POINTERDEVICEINRANGE:
        return "WMSG_POINTERDEVICEINRANGE";
        break;
    case WMSG_POINTERDEVICEOUTOFRANGE:
        return "WMSG_POINTERDEVICEOUTOFRANGE";
        break;
    case WMSG_TOUCH:
        return "WMSG_TOUCH";
        break;
    case WMSG_NCPOINTERUPDATE:
        return "WMSG_NCPOINTERUPDATE";
        break;
    case WMSG_NCPOINTERDOWN:
        return "WMSG_NCPOINTERDOWN";
        break;
    case WMSG_NCPOINTERUP:
        return "WMSG_NCPOINTERUP";
        break;
    case WMSG_POINTERUPDATE:
        return "WMSG_POINTERUPDATE";
        break;
    case WMSG_POINTERDOWN:
        return "WMSG_POINTERDOWN";
        break;
    case WMSG_POINTERUP:
        return "WMSG_POINTERUP";
        break;
    case WMSG_POINTERENTER:
        return "WMSG_POINTERENTER";
        break;
    case WMSG_POINTERLEAVE:
        return "WMSG_POINTERLEAVE";
        break;
    case WMSG_POINTERACTIVATE:
        return "WMSG_POINTERACTIVATE";
        break;
    case WMSG_POINTERCAPTURECHANGED:
        return "WMSG_POINTERCAPTURECHANGED";
        break;
    case WMSG_TOUCHHITTESTING:
        return "WMSG_TOUCHHITTESTING";
        break;
    case WMSG_POINTERWHEEL:
        return "WMSG_POINTERWHEEL";
        break;
    case WMSG_POINTERHWHEEL:
        return "WMSG_POINTERHWHEEL";
        break;
    case WMSG_POINTERROUTEDTO:
        return "WMSG_POINTERROUTEDTO";
        break;
    case WMSG_POINTERROUTEDAWAY:
        return "WMSG_POINTERROUTEDAWAY";
        break;
    case WMSG_POINTERROUTEDRELEASED:
        return "WMSG_POINTERROUTEDRELEASED";
        break;
    case WMSG_IME_SETCONTEXT:
        return "WMSG_IME_SETCONTEXT";
        break;
    case WMSG_IME_NOTIFY:
        return "WMSG_IME_NOTIFY";
        break;
    case WMSG_IME_CONTROL:
        return "WMSG_IME_CONTROL";
        break;
    case WMSG_IME_COMPOSITIONFULL:
        return "WMSG_IME_COMPOSITIONFULL";
        break;
    case WMSG_IME_SELECT:
        return "WMSG_IME_SELECT";
        break;
    case WMSG_IME_CHAR:
        return "WMSG_IME_CHAR";
        break;
    case WMSG_IME_REQUEST:
        return "WMSG_IME_REQUEST";
        break;
    case WMSG_IME_KEYDOWN:
        return "WMSG_IME_KEYDOWN";
        break;
    case WMSG_IME_KEYUP:
        return "WMSG_IME_KEYUP";
        break;
    case WMSG_MOUSEHOVER:
        return "WMSG_MOUSEHOVER";
        break;
    case WMSG_MOUSELEAVE:
        return "WMSG_MOUSELEAVE";
        break;
    case WMSG_NCMOUSEHOVER:
        return "WMSG_NCMOUSEHOVER";
        break;
    case WMSG_NCMOUSELEAVE:
        return "WMSG_NCMOUSELEAVE";
        break;
    case WMSG_WTSSESSION_CHANGE:
        return "WMSG_WTSSESSION_CHANGE";
        break;
    case WMSG_TABLET_FIRST:
        return "WMSG_TABLET_FIRST";
        break;
    case WMSG_TABLET_LAST:
        return "WMSG_TABLET_LAST";
        break;
    case WMSG_DPICHANGED:
        return "WMSG_DPICHANGED";
        break;
    case WMSG_DPICHANGED_BEFOREPARENT:
        return "WMSG_DPICHANGED_BEFOREPARENT";
        break;
    case WMSG_DPICHANGED_AFTERPARENT:
        return "WMSG_DPICHANGED_AFTERPARENT";
        break;
    case WMSG_GETDPISCALEDSIZE:
        return "WMSG_GETDPISCALEDSIZE";
        break;
    case WMSG_CUT:
        return "WMSG_CUT";
        break;
    case WMSG_COPY:
        return "WMSG_COPY";
        break;
    case WMSG_PASTE:
        return "WMSG_PASTE";
        break;
    case WMSG_CLEAR:
        return "WMSG_CLEAR";
        break;
    case WMSG_UNDO:
        return "WMSG_UNDO";
        break;
    case WMSG_RENDERFORMAT:
        return "WMSG_RENDERFORMAT";
        break;
    case WMSG_RENDERALLFORMATS:
        return "WMSG_RENDERALLFORMATS";
        break;
    case WMSG_DESTROYCLIPBOARD:
        return "WMSG_DESTROYCLIPBOARD";
        break;
    case WMSG_DRAWCLIPBOARD:
        return "WMSG_DRAWCLIPBOARD";
        break;
    case WMSG_PAINTCLIPBOARD:
        return "WMSG_PAINTCLIPBOARD";
        break;
    case WMSG_VSCROLLCLIPBOARD:
        return "WMSG_VSCROLLCLIPBOARD";
        break;
    case WMSG_SIZECLIPBOARD:
        return "WMSG_SIZECLIPBOARD";
        break;
    case WMSG_ASKCBFORMATNAME:
        return "WMSG_ASKCBFORMATNAME";
        break;
    case WMSG_CHANGECBCHAIN:
        return "WMSG_CHANGECBCHAIN";
        break;
    case WMSG_HSCROLLCLIPBOARD:
        return "WMSG_HSCROLLCLIPBOARD";
        break;
    case WMSG_QUERYNEWPALETTE:
        return "WMSG_QUERYNEWPALETTE";
        break;
    case WMSG_PALETTEISCHANGING:
        return "WMSG_PALETTEISCHANGING";
        break;
    case WMSG_PALETTECHANGED:
        return "WMSG_PALETTECHANGED";
        break;
    case WMSG_HOTKEY:
        return "WMSG_HOTKEY";
        break;
    case WMSG_PRINT:
        return "WMSG_PRINT";
        break;
    case WMSG_PRINTCLIENT:
        return "WMSG_PRINTCLIENT";
        break;
    case WMSG_APPCOMMAND:
        return "WMSG_APPCOMMAND";
        break;
    case WMSG_THEMECHANGED:
        return "WMSG_THEMECHANGED";
        break;
    case WMSG_CLIPBOARDUPDATE:
        return "WMSG_CLIPBOARDUPDATE";
        break;
    case WMSG_DWMCOMPOSITIONCHANGED:
        return "WMSG_DWMCOMPOSITIONCHANGED";
        break;
    case WMSG_DWMNCRENDERINGCHANGED:
        return "WMSG_DWMNCRENDERINGCHANGED";
        break;
    case WMSG_DWMCOLORIZATIONCOLORCHANGED:
        return "WMSG_DWMCOLORIZATIONCOLORCHANGED";
        break;
    case WMSG_DWMWINDOWMAXIMIZEDCHANGE:
        return "WMSG_DWMWINDOWMAXIMIZEDCHANGE";
        break;
    case WMSG_DWMSENDICONICTHUMBNAIL:
        return "WMSG_DWMSENDICONICTHUMBNAIL";
        break;
    case WMSG_DWMSENDICONICLIVEPREVIEWBITMAP:
        return "WMSG_DWMSENDICONICLIVEPREVIEWBITMAP";
        break;
    case WMSG_GETTITLEBARINFOEX:
        return "WMSG_GETTITLEBARINFOEX";
        break;
    case WMSG_HANDHELDFIRST:
        return "WMSG_HANDHELDFIRST";
        break;
    case WMSG_HANDHELDLAST:
        return "WMSG_HANDHELDLAST";
        break;
    case WMSG_AFXFIRST:
        return "WMSG_AFXFIRST";
        break;
    case WMSG_AFXLAST:
        return "WMSG_AFXLAST";
        break;
    case WMSG_PENWINFIRST:
        return "WMSG_PENWINFIRST";
        break;
    case WMSG_PENWINLAST:
        return "WMSG_PENWINLAST";
        break;
    case WMSG_APP:
        return "WMSG_APP";
        break;
    case WMSG_USER:
        return "WMSG_USER";
        break;
    default:
        sprintf_s(buf, "Unknown message 0x%x", wmsg);
        return buf;
        break;
    }
}

void change_resolution(int width, int height)
{
    // Get the current screen resolution
    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
    {
        std::cout << "Current screen resolution: " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << std::endl;
    }

    // Change the screen resolution
    dm.dmPelsWidth = width;
    dm.dmPelsHeight = height;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    LONG result = ChangeDisplaySettings(&dm, 0);
    if (result == DISP_CHANGE_SUCCESSFUL)
    {
        std::cout << "Screen resolution changed successfully" << std::endl;
    }
    else
    {
        std::cerr << "Failed to change screen resolution" << std::endl;
    }
}

void invisible_cursor()
{

    BYTE color = 255;
    // Create a monochrome mask for the cursor
    HBITMAP mask = CreateBitmap(1, 1, 1, 1, &color);

    // Create an icon from the mask
    ICONINFO ii = {0};
    ii.fIcon = FALSE;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = mask;
    ii.hbmColor = mask;
    HCURSOR cursor = CreateIconIndirect(&ii);

    // Set the cursor
    SetCursor(cursor);

    // Clean up
    DeleteObject(mask);
}

void nullcursor()
{
    // Replace these values with the actual data of your bitmap
    int width = 1;
    int height = 1;

    BYTE bitmapData[4] = {0, 0, 0, 255};

    // Create a monochrome mask for the cursor
    HBITMAP mask = CreateBitmap(width, height, 1, 1, &bitmapData[3]);

    // Create a color bitmap for the cursor
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    HDC hdc = GetDC(NULL);
    HBITMAP color = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, bitmapData, &bmi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hdc);

    // Create an icon from the bitmaps
    ICONINFO ii = {0};
    ii.fIcon = FALSE;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = mask;
    ii.hbmColor = color;
    HCURSOR cursor = CreateIconIndirect(&ii);

    // Set the cursor
    SetCursor(cursor);

    // Clean up
    DeleteObject(mask);
    DeleteObject(color);
}

HICON CreateCursorFromBMP(bmp *pic, DWORD x_offset = 0, DWORD y_offset = 0)
{
    int wid = pic->width;
    int hei = pic->height;
    HBITMAP hbmMask, hbmColor;
    if (pic->bitdepth == 32)
    {
        hbmMask = CreateBitmap(wid, hei, 1, 1, NULL);
        hbmColor = CreateBitmap(wid, hei, 1, 32, pic->data);
    }
    else if (pic->bitdepth == 24)
    {
        hbmMask = CreateBitmap(wid, hei, 1, 1, NULL);
        hbmColor = CreateBitmap(wid, hei, 1, 24, pic->data);
    }
    else
    {
        return NULL;
    }
    ICONINFO ii;
    ii.fIcon = FALSE; // Cursor
    ii.xHotspot = x_offset;
    ii.yHotspot = y_offset;
    ii.hbmMask = hbmMask;
    ii.hbmColor = hbmColor;
    HCURSOR hCursor = CreateIconIndirect(&ii);
    DeleteObject(hbmMask);
    DeleteObject(hbmColor);
    return hCursor;
}

HICON CreateIconFromBMP(bmp *pic)
{
    int wid = pic->width;
    int hei = pic->height;
    HBITMAP hbmMask, hbmColor;
    if (pic->bitdepth == 32)
    {
        hbmMask = CreateBitmap(wid, hei, 1, 1, NULL);
        hbmColor = CreateBitmap(wid, hei, 1, 32, pic->data);
    }
    else if (pic->bitdepth == 24)
    {
        hbmMask = CreateBitmap(wid, hei, 1, 1, NULL);
        hbmColor = CreateBitmap(wid, hei, 1, 24, pic->data);
    }
    else
    {
        return NULL;
    }
    ICONINFO ii;
    ii.fIcon = TRUE; // NOT Cursor
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = hbmMask;
    ii.hbmColor = hbmColor;
    HICON hIcon = CreateIconIndirect(&ii);
    DeleteObject(hbmMask);
    DeleteObject(hbmColor);
    return hIcon;
}

void set_cursor_as(uint32_t *picdata, int pic_wid, int pic_hei, DWORD x_offset = 0, DWORD y_offset = 0)
{
    // Create a bitmap mask for the cursor
    HBITMAP hbmMask = CreateBitmap(pic_wid, pic_hei, 1, 1, NULL);

    // Create a bitmap for the cursor
    HBITMAP hbmColor = CreateBitmap(pic_wid, pic_hei, 1, 32, picdata);

    // Create an icon information structure
    ICONINFO ii;
    ii.fIcon = FALSE; // Cursor
    ii.xHotspot = x_offset;
    ii.yHotspot = y_offset;
    ii.hbmMask = hbmMask;
    ii.hbmColor = hbmColor;

    // Create the cursor from the icon information
    HCURSOR hCursor = CreateIconIndirect(&ii);

    // Set the cursor
    HCURSOR hCret = SetCursor(hCursor);

    // Clean up
    DeleteObject(hbmMask);
    DeleteObject(hbmColor);
    static HCURSOR last_hCursor = NULL;
    if (last_hCursor != NULL)
    {
        DestroyIcon(last_hCursor);
    }
    last_hCursor = hCursor;
}

void gray_map(uint32_t *buf)
{
    for (uint32_t y = 0; y < 256; y++)
    {
        for (uint32_t x = 0; x < 256; x++)
        {
            buf[y * 256 + x] = (y << 24) | (x << 16) | (x << 8) | x;
        }
    }
}

void gray_map_cursor()
{
    std::vector<uint32_t> buf(256 * 256);
    gray_map(buf.data());
    set_cursor_as(buf.data(), 256, 256);
}

void set_window_on_top(HWND hwnd)
{
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

static outputs ou;

class onceobj
{
public:
    onceobj(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        gray_map_cursor();
        // set_window_on_top(hwnd);
    }

    void rep(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {

        static auto force = []() -> vector_<2, double>
        {
            LONG dx, dy;
            ou.get_mouse_move(&dx, &dy);
            return vector_<2, double>(dx, dy);
        };

        static class psys_
        {
        public:
            Particle_system<2, double> ps;

            psys_()
            {
                ps.add_particle(Particle_2df64(vector_<2, double>(128, 128), 1));
                auto box = box_II<2, double>(vector_<2, double>(0, 0), vector_<2, double>(256, 256));
                ps.field_set(0, box);
                auto cbk = callback_<2, double>(force);
                ps.field_set(0, cbk);
            }

            vector_<2, double> getposition(double dt = 0.1)
            {
                this->ps.run(dt);
                return this->ps.get_particle(0).get_position();
            }
        } psys;

        static LONG mx0 = 0, my0 = 0, mx = 0, my = 0;
        static timer t;
        if (0)
        {
            auto msg = (MSG *)lParam;
            mx = msg->pt.x;
            my = msg->pt.y;
        }
        ou.refresh();
        ou.get_mouse(&mx0, &my0);
        // mx++;
        // my += (mx >> 8) & 1;
        mx = mx0; //&0xff;
        my = my0; // &0xff;

        static bmp pic(bmphd(256, 256, 32));
        pic.foreach_pixel_write([](uint32_t x, uint32_t y) -> uint32_t
                                { return (y << 24) | ((mx) << 16) | ((my) << 8) | x; });
        auto xy = psys.getposition(0.01 * t.ms_form_last());
        pic.draw_solid_circle(xy.x(), xy.y(), 5);
        set_cursor_as((uint32_t *)pic.data, 256, 256, mx, my);
        if (ou.is_down(BACKQUOTE_GRAVEACCENT_KC))
        {
            set_window_on_top(hwnd);
        }
    }
};

class msgCnt
{
public:
    constexpr static const auto max = 0xffff;
    uint32_t *msgcntp = new uint32_t[max]{};
    uint32_t (&msgcnt)[max] = *(decltype(&msgcnt))msgcntp;
    constexpr static const auto orderedmax = 0xff;

    struct cntmap
    {
        wMsg msg;
        uint32_t count;
    };

    cntmap *orderedcntp = new cntmap[orderedmax]{};
    cntmap (&orderedcnt)[orderedmax] = *(decltype(&orderedcnt))orderedcntp;

    uint32_t get(UINT uMsg)
    {
        return uMsg >= max ? 0 : msgcnt[uMsg];
    }
    uint32_t get(wMsg uMsg)
    {
        return uMsg >= max ? 0 : msgcnt[uMsg];
    }

    uint32_t count(uint32_t uMsg)
    {
        return uMsg >= max ? 0 : ++msgcnt[uMsg];
    }
    uint32_t count(wMsg uMsg)
    {
        return uMsg >= max ? 0 : ++msgcnt[uMsg];
    }

    auto &get_max()
    {
        memset(orderedcnt, 0, sizeof(orderedcnt));
        // get the max count into orderedcnt
        for (uint32_t i = 0, idx = 0; i < max; i++)
        {
            if (msgcnt[i] > 0)
            {
                orderedcnt[idx].msg = (wMsg)i;
                orderedcnt[idx].count = msgcnt[i];
                idx++;
                if (idx >= orderedmax)
                    break;
            }
        }
        return orderedcnt;
    }

    ~msgCnt()
    {
        delete[] msgcntp;
        delete[] orderedcntp;
    }
};

LRESULT CALLBACK minWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static msgCnt cnt;
    ;
    auto c = cnt.count(uMsg);
    if (c > 5 && c < 10)
    {
        printf("msg:%d, count:%d\n", uMsg, c);
    }

    static onceobj once(hwnd, uMsg, wParam, lParam);
    // for (;;)
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_NULL:
        once.rep(hwnd, uMsg, wParam, lParam);
        return 0;
    default:
        once.rep(hwnd, uMsg, wParam, lParam);
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void full_window(WNDPROC wproc = minWindowProc)
{

    WNDCLASS wc = {0};
    wc.lpfnWndProc = wproc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"FullScreenWindowClass";
    RegisterClass(&wc);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    HWND hwnd = CreateWindow(L"FullScreenWindowClass", L"Full Screen Window", WS_POPUP, 0, 0, w, h, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(hwnd, SW_SHOW);

    for (;;)
    {
        ou.refresh();
        CallWindowProc(wproc, hwnd, 0, 0, 0);
        Sleep(15);
    }
}

class windowclass
{
public:
    WNDCLASSW wc = {0};
    ATOM atom;
    typedef LRESULT(CALLBACK *WNDPROC_MSG)(HWND, wMsg, WPARAM, LPARAM);
    windowclass(
        WNDPROC wproc = minWindowProc,
        HMODULE instance = GetModuleHandle(NULL),
        LPCWSTR classname = L"wcname")
    {
        wc.lpfnWndProc = (WNDPROC)wproc;
        wc.hInstance = instance;
        wc.lpszClassName = classname;

        // set the icon
        bmp icbmp = bmp(bmphd(64, 64, 32));

        auto iconfn = [](double x, double y)
        {
            uint32_t clr = bionukg_icon(-x, -y);

            if (clr == 0xffffffff)
            {
                clr = 0x00ffffff;
            }

            return clr;
        };

        icbmp.foreach_pixel_write(iconfn);
        icbmp.presave();
        icbmp.show_on_v2(GetConsoleWindow());
        wc.hIcon = CreateIconFromBMP(&icbmp);
        atom = RegisterClassW(&wc);
    }
    windowclass(
        WNDPROC_MSG wproc,
        HMODULE instance = GetModuleHandle(NULL),
        LPCWSTR classname = L"wcname")
    {
        wc.lpfnWndProc = (WNDPROC)wproc;
        wc.hInstance = instance;
        wc.lpszClassName = classname;
        atom = RegisterClassW(&wc);
    }
    ~windowclass()
    {
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        DestroyIcon(wc.hIcon);
    }
};

class window
{
    HWND hwnd;
    WNDPROC wproc;

public:
    typedef struct tagMSG
    {
        HWND hwnd;
        wMsg message;
        WPARAM wParam;
        LPARAM lParam;
        DWORD time;
        POINT pt;
#ifdef _MAC
        DWORD lPrivate;
#endif
    } MSG_;
    MSG_ msg_ = {};
    MSG &msg = *(MSG *)&msg_;

    enum windowstyle
    {
        POPUP_STYLE = 0x80000000L,
        CHILD_STYLE = 0x40000000L,
        MINIMIZE_STYLE = 0x20000000L,
        VISIBLE_STYLE = 0x10000000L,
        DISABLED_STYLE = 0x08000000L,
        CLIPSIBLINGS_STYLE = 0x04000000L,
        CLIPCHILDREN_STYLE = 0x02000000L,
        MAXIMIZE_STYLE = 0x01000000L,
        CAPTION_STYLE = 0x00C00000L,
        BORDER_STYLE = 0x00800000L,
        DLGFRAME_STYLE = 0x00400000L,
        VSCROLL_STYLE = 0x00200000L,
        HSCROLL_STYLE = 0x00100000L,
        SYSMENU_STYLE = 0x00080000L,
        THICKFRAME_STYLE = 0x00040000L,
        GROUP_STYLE = 0x00020000L,
        TABSTOP_STYLE = 0x00010000L,
        MINIMIZEBOX_STYLE = 0x00020000L,
        MAXIMIZEBOX_STYLE = 0x00010000L,
        /*
        TILED_STYLE = OVERLAPPED,
        ICONIC_STYLE = MINIMIZE,
        SIZEBOX_STYLE = THICKFRAME,
        OVERLAPPEDWINDOW_STYLE = (OVERLAPPED | CAPTION | SYSMENU |
            THICKFRAME | MINIMIZEBOX | MAXIMIZEBOX),
        TILEDWINDOW_STYLE = OVERLAPPEDWINDOW,
        POPUPWINDOW_STYLE = (POPUP | BORDER | SYSMENU),
        CHILDWINDOW_STYLE = (CHILD)
        */
        NULL_STYLE = 0x00000000L
    };

    struct windowinfo
    {
        DWORD style = POPUP_STYLE;
        DWORD exstyle = 0;
        int x = 0;
        int y = 0;
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);
        HWND parent = 0;
        HMENU menu = 0;
        LPVOID param = 0;
        windowclass wc;
        LPCWSTR windowname = L"window_default_name";
        windowinfo(WNDPROC wproc = minWindowProc) : wc(wproc) {}
    };
    window(windowinfo info = windowinfo())
    {
        hwnd = CreateWindowExW(
            info.exstyle,
            info.wc.wc.lpszClassName,
            info.windowname,
            info.style,
            info.x, info.y,          // the left and top position of the window
            info.width, info.height, // the width and height of the window
            info.parent,             // the parent of the window
            info.menu,               // the menu of the window
            info.wc.wc.hInstance,    // the instance of the module associated with the window
            info.param);
        wproc = info.wc.wc.lpfnWndProc;
        ShowWindow(hwnd, SW_SHOW);
    }
    window(HWND hwnd_)
    {
        hwnd = hwnd_;
        wproc = minWindowProc;
    }
    auto GetMessage_()
    {
        return ::GetMessageW(&msg, hwnd, 0, 0);
    }
    auto PeekMessage_()
    {
        return ::PeekMessageW(&msg, hwnd, 0, 0, PM_REMOVE);
    }

    auto TranslateMessage_()
    {
        return ::TranslateMessage(&msg);
    };
    auto DispatchMessage_()
    {
        return ::DispatchMessageW(&msg);
    };

    HWND hwnd_()
    {
        return hwnd;
    }

    void display_message()
    {
        printf("message:%16s,wParam:%8d,lParam:%8d,pt:%4d,%4d\n",
               wMsg2str(msg_.message), msg_.wParam, msg_.lParam, msg_.pt.x, msg_.pt.y);
    }
    template <typename LPARAM_t = LPARAM>
    auto Call(wMsg Msg = wMsg::WMSG_NULL, WPARAM wParam = 0, LPARAM_t lParam = 0)
    {
        return CallWindowProcW(wproc, hwnd, Msg, wParam, (LPARAM)lParam);
    }
};

#endif