/****************************************************************************
 * NCSA Mosaic for the X Window System                                      *
 * Copyright (C) 1993                                                       *
 * National Center for Supercomputing Applications                          *
 * Software Development Group                                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * The NCSA software Mosaic, both binary and source, is copyrighted,        *
 * but available without fee for education, academic research and           *
 * non-commercial purposes.  The software is copyrighted in the name of     *
 * the University of Illinois, and ownership of the software remains with   *
 * the University of Illinois.  Users may distribute the binary and         *
 * source code to third parties provided that the copyright notice and      *
 * this statement appears on all copies and that no charge is made for      *
 * such copies.  Any entity wishing to integrate all or part of the         *
 * source code into a product for commercial use or resale, should          *
 * contact the University of Illinois, c/o NCSA, to negotiate an            *
 * appropriate license for such commercial use.                             *
 *                                                                          *
 * THE UNIVERSITY OF ILLINOIS MAKES NO REPRESENTATIONS ABOUT THE            *
 * SUITABILITY OF THE SOFTWARE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS"     *
 * WITHOUT EXPRESS OR IMPLIED WARRANTY.  THE UNIVERSITY OF ILLINOIS SHALL   *
 * NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY THE USER OF THIS SOFTWARE.     *
 * The software may have been developed under agreements between the        *
 * University of Illinois and the Federal Government which entitle the      *
 * Government to certain rights.                                            *
 *                                                                          *
 * By copying this program, you, the user, agree to abide by the            *
 * copyright conditions and understandings with respect to any software     *
 * which is marked with a copyright notice.                                 *
 *                                                                          *
 * If you have problems or comments about NCSA Mosaic, please feel free     *
 * to mail them to marca@ncsa.uiuc.edu.                                     *
 ****************************************************************************/

#ifndef __XMX_H__
#define __XMX_H__

/* --------------------------- SYSTEM INCLUDES ---------------------------- */

/* Generic X11/Xt/Xm includes. */
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <Xm/Xm.h>

#if 0

#ifdef __sgi
/* Fast malloc. */
#include <malloc.h>
/* For GLXconfig type. */
#include <X11/Xirisw/GlxMDraw.h>
#endif

#ifdef _IBMR2
/* nothing that I know of */
#endif /* _IBMR2 */

#endif /* if 0 */

/* --------------------------- INTERNAL DEFINES --------------------------- */

/* Maximum number of resource args usable --- should be large
   since applications also can add resources. */
#define XmxMaxNumArgs 30

/* ------------------------------ VARIABLES ------------------------------- */

/* These three will also be used from application code. */
extern int Xmx_n;
extern Arg Xmx_wargs[];
extern Widget Xmx_w;

/* This probably won't be needed to be accessed
   directly by an application; if not, it should be
   moved to XmxP.h. */
extern int Xmx_uniqid;

/* ------------------------------ CONSTANTS ------------------------------- */

/* These probably shouldn't be necessary, since we use
   Gadgets whenever possible. */
#define XmxWidget 0
#define XmxGadget 1

/* XmxFrame types. */
#define XmxShadowIn        0
#define XmxShadowOut       1
#define XmxShadowEtchedIn  2
#define XmxShadowEtchedOut 3

/* Currently unused. */
#define XmxLeft   XmALIGNMENT_BEGINNING
#define XmxCenter XmALIGNMENT_CENTER
#define XmxRight  XmALIGNMENT_END

/* Null values for some arguments. */
#define XmxNotDisplayed -99999
#define XmxNoPosition   -99999
#define XmxNoOffset     -99999

/* States for togglebuttons. */
#define XmxUnset  0
#define XmxNotSet XmxUnset
#define XmxSet    1

/* Sensitivity states. */
#define XmxUnsensitive 0
#define XmxNotSensitive XmxUnsensitive
#define XmxSensitive 1

/* Types of togglebuttons. */
#define XmxOneOfMany 0
#define XmxNOfMany   1

/* Constraint possibilities. */
#define XmxCwidget XmATTACH_WIDGET
#define XmxCform   XmATTACH_FORM
#define XmxCnone   XmATTACH_NONE

/* ---------------------------- MENU TYPEDEFS ----------------------------- */

/* Struct used by app when loading option menu.  The 'set_state'
   parameter is used to indicate menu history for the option menu;
   the entry with 'XmxSet' is used. */
typedef struct _XmxOptionMenuStruct
{
  String namestr;
  int data;
  int set_state;
} XmxOptionMenuStruct;

/* Toggle menu and option menu accept same struct. */
typedef XmxOptionMenuStruct XmxToggleMenuStruct;

/* Menubar uses a recursive struct. */
typedef struct _XmxMenubarStruct
{
  String namestr;
  char mnemonic;
  void (*func)();
  int data;
  struct _XmxMenubarStruct *sub_menu;
} XmxMenubarStruct;

/* --------------------------- RECORD TYPEDEFS ---------------------------- */

/* These typedefs, while public, are not guaranteed to remain static
   and should not be actually used by an application. */

/* A single entry in a menu (menubar, toggle menu, or option menu),
   tagged by the integer token used as callback_data. */
typedef struct _XmxMenuEntry
{
  Widget w;
  int token;
  struct _XmxMenuEntry *next;
} XmxMenuEntry;

/* A menu (menubar, toggle menu, or option menu). */
typedef struct _XmxMenuRecord
{
  Widget base;
  XmxMenuEntry *first_entry;
} XmxMenuRecord;

/* -------------------------------- MACROS -------------------------------- */

/* Callback definitions and prototypes. */
#define XmxCallback(name)						      \
  void name (Widget w, XtPointer client_data, XtPointer call_data)
#define XmxCallbackPrototype(name)                                            \
  extern void name (Widget, XtPointer, XtPointer)

/* Event handler functions and prototypes. */
#define XmxEventHandler(name)						      \
  void name (Widget w, XtPointer client_data, XEvent *event, Boolean *cont)
#define XmxEventHandlerPrototype(name) 				              \
  extern void name (Widget, XtPointer, XEvent *, Boolean *)

/* Shortcut for XtAppInitialize --- of dubious value. */
#define XmxInit()							      \
  XtAppInitialize (&app_context, "XmxApplication", NULL, 0, &argc, argv,      \
                   NULL, Xmx_wargs, Xmx_n);

/* ------------------------------ PROTOTYPES ------------------------------ */

/* Xmx.c */
extern int XmxMakeNewUniqid (void);
extern void XmxSetUniqid (int);
extern void XmxZeroUniqid (void);
extern int XmxExtractUniqid (int);
extern int XmxExtractToken (int);

extern void XmxAddCallback (Widget, String, XtCallbackProc, int);
extern void XmxAddEventHandler (Widget, EventMask, XtEventHandler, int);
extern void XmxRemoveEventHandler (Widget, EventMask, XtEventHandler, int);

extern void XmxStartup (void);
extern void XmxSetArg (String, XtArgVal);
extern void XmxSetValues (Widget);
extern void XmxManageRemanage (Widget);
extern void XmxSetSensitive (Widget, int);

extern Widget XmxMakePushButton (Widget, String, XtCallbackProc, int);
extern Widget XmxMakeBlankButton (Widget, XtCallbackProc, int);
extern Widget XmxMakeCommand (Widget, String, XtCallbackProc, int);
extern Widget XmxMakeScrolledList (Widget, XtCallbackProc, int);
extern Widget XmxMakeDrawingArea (Widget, int, int);
extern Widget XmxMakeRadioBox (Widget);
extern Widget XmxMakeOptionBox (Widget);
extern Widget XmxMakeToggleButton (Widget, String, XtCallbackProc, int);
extern void XmxSetToggleButton (Widget button, int set_state);
extern Widget XmxMakeScale (Widget, XtCallbackProc, int, String, 
                            int, int, int, int);
extern void XmxAdjustScale (Widget, int);
extern Widget XmxMakeFrame (Widget, int);
extern Widget XmxMakeForm (Widget);
extern void XmxSetPositions (Widget, int, int, int, int);
extern void XmxSetOffsets (Widget, int, int, int, int);
extern void XmxSetConstraints (Widget, int, int, int, int, Widget, Widget, 
                               Widget, Widget);
extern Widget XmxMakeVerticalRowColumn (Widget);
extern Widget XmxMakeHorizontalRowColumn (Widget);
extern Widget XmxMakeNColumnRowColumn (Widget, int);
extern Widget XmxMakeVerticalBboard (Widget);
extern Widget XmxMakeVerticalBboardWithFont (Widget, String);
extern Widget XmxMakeHorizontalBboard (Widget);
#ifdef NONWORKING_CENTERING
extern Widget XmxMakeCenteringBboard (Widget, XtAppContext);
#endif
extern void XmxAdjustLabelText (Widget, String);
extern Widget XmxMakeLabel (Widget, String);
extern Widget XmxMakeNamedLabel (Widget, String, String);
extern Widget XmxMakeBlankLabel (Widget);
extern Widget XmxMakeErrorDialog (Widget, String, String);
extern Widget XmxMakeInfoDialog (Widget, String, String);
extern Widget XmxMakeQuestionDialog (Widget, String, String, XtCallbackProc, 
                                     int, int);
extern XmString XmxMakeXmstrFromFile (String);
extern XmString XmxMakeXmstrFromString (String);
extern Widget XmxMakeBboardDialog (Widget, String);
extern Widget XmxMakeFormDialog (Widget, String);
extern Widget XmxMakeFileSBDialog (Widget, String, String, XtCallbackProc, 
                                   int);
extern Widget XmxMakeHelpDialog (Widget, XmString, String);
extern Widget XmxMakeHelpTextDialog (Widget, String, String, Widget *);
extern void XmxAdjustHelpDialogText (Widget, XmString, String);
extern void XmxAdjustDialogTitle (Widget, String);
extern Widget XmxMakeHorizontalSeparator (Widget);
extern Widget XmxMakeHorizontalSpacer (Widget);
extern Widget XmxMakeHorizontalBoundary (Widget);
extern Widget XmxMakeScrolledText (Widget);
extern Widget XmxMakeText (Widget);
extern Widget XmxMakeTextField (Widget);
extern void XmxTextSetString (Widget, String);
extern String XmxTextGetString (Widget);
extern void XmxAddCallbackToText (Widget, XtCallbackProc, int);

#if 0

#ifdef __sgi
extern Widget XmxMakeDrawingVolume 
  (Widget, int, int, GLXconfig *, XtCallbackProc, XtCallbackProc,
   XtCallbackProc);
extern void XmxInstallColormaps (Widget, Widget);
extern void XmxInstallColormapsWithOverlay (Widget, Widget);
extern void XmxWinset (Widget);
#endif

#ifdef _IBMR2
extern Widget XmxMakeDrawingVolume 
  (Widget, int, int, XtCallbackProc, XtCallbackProc,
   XtCallbackProc);
extern void XmxInstallColormaps (Widget, Widget);
extern void XmxWinset (Widget);
#endif

#endif /* if 0 */

extern void XmxApplyBitmapToLabelWidget (Widget, String, unsigned int, 
                                         unsigned int);

extern Widget XmxMakeFormAndOneButton (Widget, XtCallbackProc, String, int);
extern Widget XmxMakeFormAndTwoButtons (Widget, XtCallbackProc, String, 
                                        String, int, int);
extern Widget XmxMakeFormAndTwoButtonsSqueezed (Widget, XtCallbackProc, String, 
                                                String, int, int);
extern Widget XmxMakeFormAndThreeButtons (Widget, XtCallbackProc, String, 
                                          String, String, int, int, int);
extern Widget XmxMakeFormAndThreeButtonsSqueezed (Widget, XtCallbackProc, String, 
                                                  String, String, int, int, int);
extern Widget XmxMakeFormAndFourButtons (Widget, XtCallbackProc, String, 
                                         String, String, String, int, int, int, int);

/* Xmx2.c */
extern void XmxRSetSensitive (XmxMenuRecord *, int, int);
extern void XmxRSetToggleState (XmxMenuRecord *, int, int);
extern void XmxRUnsetAllToggles (XmxMenuRecord *);
extern void XmxRSetOptionMenuHistory (XmxMenuRecord *, int);
extern void XmxRSetValues (XmxMenuRecord *, int);
extern Widget XmxRGetWidget (XmxMenuRecord *, int);

extern XmxMenuRecord *XmxRMakeOptionMenu (Widget, String, XtCallbackProc, 
                                          XmxOptionMenuStruct *);
extern XmxMenuRecord *XmxRMakeToggleMenu (Widget, int, XtCallbackProc, 
                                          XmxToggleMenuStruct *);
extern XmxMenuRecord *XmxRMakeMenubar (Widget, XmxMenubarStruct *);

#endif /* __XMX_H__ */
