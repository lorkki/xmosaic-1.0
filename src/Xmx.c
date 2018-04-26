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

#include "XmxP.h"

/* ---------------------------- FILE VARIABLES ---------------------------- */

/* Variables accessed through Xmx.h as extern. */
int    Xmx_n = 0;
Arg    Xmx_wargs[XmxMaxNumArgs];
Widget Xmx_w;
int    Xmx_uniqid = 0;

/* Counter for values returned from XmxMakeNewUniqid. */
static int Xmx_uniqid_counter = 0;

/* Flag for whether or not XmxSetUniqid has ever been called. */
static int Xmx_uniqid_has_been_set = 0;

/* --------------------------- UNIQID FUNCTIONS --------------------------- */

int
XmxMakeNewUniqid (void)
{
  Xmx_uniqid_counter++;

  return Xmx_uniqid_counter;
}

void
XmxSetUniqid (int uniqid)
{
  Xmx_uniqid = uniqid;
  Xmx_uniqid_has_been_set = 1;

  return;
}

void
XmxZeroUniqid (void)
{
  Xmx_uniqid = 0;
  /* Do NOT reset Xmx_uniqid_has_been_set. */

  return;
}

int
XmxExtractUniqid (int cd)
{
  /* Pull the high 16 bits, if uniqid has been set. */
  if (Xmx_uniqid_has_been_set)
    return (cd >> 16);
  else
    return 0;
}

int
XmxExtractToken (int cd)
{
  /* Pull the low 16 bits, if uniqid has been set. */
  if (Xmx_uniqid_has_been_set)
    return ((cd << 16) >> 16);
  else
    return cd;
}

/* This function should be called by every Xmx routine
   when registering a callback or event handler. */
/* This is PRIVATE but accessible to Xmx2.c also. */
int
_XmxMakeClientData (int token)
{
  if (Xmx_uniqid_has_been_set)
    return ((Xmx_uniqid << 16) | token);
  else
    return token;
}

/* -------------------------- INTERNAL CALLBACKS -------------------------- */

/* Internal routine to unmanage file selection box on Cancel. */
static
XmxCallback(_XmxCancelCallback)
{
  XtUnmanageChild (w);

  return;
}


/* --------------------------- CALLBACK SUPPORT --------------------------- */

/* args do nothing */
void XmxAddCallback (Widget w, String name, XtCallbackProc cb, int cb_data)
{
  XtAddCallback (w, name, cb, _XmxMakeClientData (cb_data));
  return;
}


/* ------------------------ EVENT HANDLER SUPPORT ------------------------- */

void
XmxAddEventHandler (Widget w, EventMask event_mask, XtEventHandler proc,
                    int client_data)
{
  XtAddEventHandler 
    (w, event_mask, False, proc, _XmxMakeClientData (client_data));

  return;
}

void
XmxRemoveEventHandler (Widget w, EventMask event_mask, XtEventHandler proc,
                       int client_data)
{
  XtRemoveEventHandler
    (w, event_mask, False, proc, _XmxMakeClientData (client_data));

  return;
}


/* ------------------- (nonworking) CENTERING ROUTINES -------------------- */

#ifdef NONWORKING_CENTERING

/* Adapted from Dan Heller's post in comp.windows.x.motif;
   assumes BulletinBoard with one (centered) child. */
static void
XmxCenteringResize 
  (CompositeWidget w, XConfigureEvent *event, String args[], int *num_args)
{
  WidgetList children;
  int width = event->width;
  int height = event->height;
  Dimension w_width, w_height; 
  
  /* get handle to BulletinBoard's children and marginal spacing */
  XtVaGetValues (w,
                 XmNchildren, &children,
                 NULL);
  
  XtVaGetValues (children[0],
                 XmNheight, &w_height,
                 XmNwidth, &w_width,
                 NULL);
  XtVaSetValues (children[0],
                 XmNx, width/2 - w_width/2,
                 XmNy, height/2 - w_height/2,
                 NULL);
                 
  return;
}

#endif /* NONWORKING_CENTERING */

/* -------------------------- UTILITY FUNCTIONS --------------------------- */

/* resets args */
void XmxStartup (void)
{
  Xmx_n = 0;
  return;
}

/* sets an arg */
void XmxSetArg (String arg, XtArgVal val)
{
  XtSetArg (Xmx_wargs[Xmx_n], arg, val);
  Xmx_n++;
  return;
}

void XmxSetValues (Widget w)
{
  XtSetValues (w, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return;
}

/* args do nothing */
void XmxManageRemanage (Widget w)
{
#if 0
  if (XtIsManaged (w))
    XtUnmanageChild (w);
  XtManageChild (w);
#endif

  if (XtIsManaged (w))
    XRaiseWindow (XtDisplay (w), XtWindow (w));
  else
    XtManageChild (w);

  return;
}

/* args do nothing */
void XmxSetSensitive (Widget w, int state)
{
  assert (state == XmxSensitive || state == XmxUnsensitive);
  XtSetSensitive (w, (state == XmxSensitive) ? True : False);

  return;
}

/* ------------------------------------------------------------------------ */
/* ---------------- WIDGET CREATION AND HANDLING ROUTINES ----------------- */
/* ------------------------------------------------------------------------ */

/* ----------------------------- PUSHBUTTONS ------------------------------ */

/* args work */
Widget XmxMakePushButton (Widget parent, String name, XtCallbackProc cb,
                          int cb_data)
{
  if (name)
    XmxSetArg (XmNlabelString,
	       XmStringCreateLtoR (name, XmSTRING_DEFAULT_CHARSET));
  Xmx_w = XtCreateManagedWidget ("pushbutton", xmPushButtonWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  XtAddCallback (Xmx_w, XmNactivateCallback, cb, _XmxMakeClientData (cb_data));

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeBlankButton (Widget parent, XtCallbackProc cb,
			   int cb_data)
{
  Xmx_w = XtCreateManagedWidget ("blankbutton", xmPushButtonWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  XtAddCallback (Xmx_w, XmNactivateCallback, cb, _XmxMakeClientData (cb_data));
  
  Xmx_n = 0;
  return Xmx_w;
}

/* ------------------------------- COMMAND -------------------------------- */

/* args work */
Widget XmxMakeCommand 
  (Widget parent, String prompt, XtCallbackProc cb, int cb_data)
{
  /* Probably not necessary. */
  /* XmxSetArg (XmNresizePolicy, XmRESIZE_ANY); */
  XmxSetArg (XmNpromptString, XmxMakeXmstrFromString (prompt));
  Xmx_w = XtCreateManagedWidget ("command", xmCommandWidgetClass, parent,
				 Xmx_wargs, Xmx_n);
  XtAddCallback 
    (Xmx_w, XmNcommandEnteredCallback, cb, _XmxMakeClientData (cb_data));

  Xmx_n = 0;
  return Xmx_w;
}

/* ---------------------------- SCROLLED LIST ----------------------------- */

/* args work */
Widget XmxMakeScrolledList
  (Widget parent, XtCallbackProc cb, int cb_data)
{
  Xmx_w = XmCreateScrolledList (parent, "scrolled_list", Xmx_wargs, Xmx_n);
  XtManageChild (Xmx_w);
  /* defaultAction gets triggered on double click and sends item
     along with it... */
  XmxAddCallback (Xmx_w, XmNdefaultActionCallback, cb,
                  _XmxMakeClientData (cb_data));
  
  Xmx_n = 0;
  return Xmx_w;
}

/* ----------------------------- DRAWING AREA ----------------------------- */

/* args work */
Widget XmxMakeDrawingArea (Widget parent, int width, int height)
{
  XmxSetArg (XmNwidth, width);
  XmxSetArg (XmNheight, height);
  Xmx_w = XtCreateManagedWidget ("drawingarea", xmDrawingAreaWidgetClass,
				 parent, Xmx_wargs, Xmx_n);

  Xmx_n = 0;
  return Xmx_w;
}

/* ------------------------ TOGGLE BUTTONS & BOXES ------------------------ */

/* args work */
Widget XmxMakeRadioBox (Widget parent)
{
  /* Could set XmxNspacing here to avoid having to play with
     margins for each togglebutton. */
  XmxSetArg (XmNspacing, 0);
  XmxSetArg (XmNentryClass, xmToggleButtonGadgetClass);
  Xmx_w = XmCreateRadioBox (parent, "radiobox", Xmx_wargs, Xmx_n);
  XtManageChild (Xmx_w);

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeOptionBox (Widget parent)
{
  XmxSetArg (XmNentryClass, xmToggleButtonGadgetClass);
  XmxSetArg (XmNisHomogeneous, True);
  Xmx_w = XtCreateManagedWidget ("optionbox", xmRowColumnWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeToggleButton (Widget parent, String name, XtCallbackProc cb,
                            int cb_data)
{
  XmxSetArg (XmNlabelString,
             XmStringCreateLtoR (name, XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNmarginHeight, 0);
  Xmx_w = XtCreateManagedWidget
    ("togglebutton", xmToggleButtonWidgetClass, parent, Xmx_wargs, Xmx_n);
  /* Used to be XmNarmCallback --- probably not right. */
  if (cb)
    XtAddCallback (Xmx_w, XmNvalueChangedCallback, cb, 
                   _XmxMakeClientData (cb_data));

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
void XmxSetToggleButton (Widget button, int set_state)
{
  assert (set_state == XmxSet || set_state == XmxUnset);
  XmToggleButtonSetState 
    (button, (set_state == XmxSet) ? True : False, False);

  Xmx_n = 0;
  return;
}

/* -------------------------------- SCALES -------------------------------- */

/* args ignored if label is non-NULL, otherwise args work */
Widget XmxMakeScale (Widget parent, XtCallbackProc cb, int cb_data,
                     String label, int min, int max, int start, int dec_adj)
{
  if (label)
    {
      Xmx_n = 0;
      XmxMakeLabel (parent, label);
    }

  XmxSetArg (XmNminimum, min);
  XmxSetArg (XmNmaximum, max);
  XmxSetArg (XmNvalue, start);
  XmxSetArg (XmNorientation, XmHORIZONTAL);
  XmxSetArg (XmNprocessingDirection, XmMAX_ON_RIGHT);
  if (dec_adj != XmxNotDisplayed)
    {
      XmxSetArg (XmNshowValue, True);
      XmxSetArg (XmNdecimalPoints, dec_adj);
    }
  Xmx_w = XtCreateManagedWidget ("scale", xmScaleWidgetClass, parent,
                                 Xmx_wargs, Xmx_n);

  XtAddCallback 
    (Xmx_w, XmNvalueChangedCallback, cb, _XmxMakeClientData (cb_data));
  XtAddCallback (Xmx_w, XmNdragCallback, cb, _XmxMakeClientData (cb_data));

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
void XmxAdjustScale (Widget scale, int val)
{
  XmxSetArg (XmNvalue, val);
  XtSetValues (scale, Xmx_wargs, Xmx_n);

  Xmx_n = 0;
  return;
}


/* args work */
Widget XmxMakeFrame (Widget parent, int shadow)
{
  assert (shadow == XmxShadowIn || shadow == XmxShadowOut || shadow == XmxShadowEtchedIn || shadow == XmxShadowEtchedOut);
  switch (shadow)
    {
    case XmxShadowIn:
      XmxSetArg (XmNshadowType, XmSHADOW_IN);  break;
    case XmxShadowOut:
      XmxSetArg (XmNshadowType, XmSHADOW_OUT);  break;
    case XmxShadowEtchedIn:
      XmxSetArg (XmNshadowType, XmSHADOW_ETCHED_IN);  break;
    case XmxShadowEtchedOut:
      XmxSetArg (XmNshadowType, XmSHADOW_ETCHED_OUT);  break;
    }
  Xmx_w = XtCreateManagedWidget ("frame", xmFrameWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);

  Xmx_n = 0;
  return Xmx_w;
}

/* -------------------------------- FORMS --------------------------------- */

/* args work */
Widget XmxMakeForm (Widget parent)
{
  Xmx_w = XtCreateManagedWidget ("form", xmFormWidgetClass, parent,
				 Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args sent to w */
void XmxSetPositions (Widget w, int top, int bottom, int left, int right)
{
  if (top != XmxNoPosition)
    {
      XmxSetArg (XmNtopAttachment, XmATTACH_POSITION);
      XmxSetArg (XmNtopPosition, top);
    }
  if (bottom != XmxNoPosition)
    {
      XmxSetArg (XmNbottomAttachment, XmATTACH_POSITION);
      XmxSetArg (XmNbottomPosition, bottom);
    }
  if (left != XmxNoPosition)
    {
      XmxSetArg (XmNleftAttachment, XmATTACH_POSITION);
      XmxSetArg (XmNleftPosition, left);
    }
  if (right != XmxNoPosition)
    {
      XmxSetArg (XmNrightAttachment, XmATTACH_POSITION);
      XmxSetArg (XmNrightPosition, right);
    }

  XmxSetValues (w);

  Xmx_n = 0;
  return;
}

/* args sent to w */
void XmxSetOffsets (Widget w, int top, int bottom, int left, int right)
{
  if (top != XmxNoOffset)
    XmxSetArg (XmNtopOffset, top);
  if (bottom != XmxNoOffset)
    XmxSetArg (XmNbottomOffset, bottom);
  if (left != XmxNoOffset)
    XmxSetArg (XmNleftOffset, left);
  if (right != XmxNoOffset)
    XmxSetArg (XmNrightOffset, right);
  
  XmxSetValues (w);

  Xmx_n = 0;
  return;
}

/* args sent to w */
void XmxSetConstraints (Widget w, 
			int top, int bottom, int left, int right,
			Widget topw, Widget botw, Widget lefw, Widget rigw)
{
  if (top != XmATTACH_NONE)
    {
      XmxSetArg (XmNtopAttachment, top);
      if (topw)
        XmxSetArg (XmNtopWidget, topw);
    }
  
  if (bottom != XmATTACH_NONE)
    {
      XmxSetArg (XmNbottomAttachment, bottom);
      if (botw)
        XmxSetArg (XmNbottomWidget, botw);
    }

  if (left != XmATTACH_NONE)
    {
      XmxSetArg (XmNleftAttachment, left);
      if (lefw)
        XmxSetArg (XmNleftWidget, lefw);
    }

  if (right != XmATTACH_NONE)
    {
      XmxSetArg (XmNrightAttachment, right);
      if (rigw)
        XmxSetArg (XmNrightWidget, rigw);
    }
  
  XmxSetValues (w);

  Xmx_n = 0;
  return;
}

/* ------------------------------ ROWCOLUMNS ------------------------------ */

/* args work */
Widget XmxMakeVerticalRowColumn (Widget parent)
{
  Xmx_w = XtCreateManagedWidget ("rowcolumn", xmRowColumnWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeHorizontalRowColumn (Widget parent)
{
  XmxSetArg (XmNorientation, XmHORIZONTAL);
  /* XmxSetArg (XmNmarginHeight, 0);
  XmxSetArg (XmNmarginWidth, 0); */
  Xmx_w = XtCreateManagedWidget ("rowcolumn", xmRowColumnWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeNColumnRowColumn (Widget parent, int ncolumns)
{
  XmxSetArg (XmNorientation, XmVERTICAL);
  XmxSetArg (XmNpacking, XmPACK_COLUMN);
  XmxSetArg (XmNnumColumns, ncolumns);
  Xmx_w = XtCreateManagedWidget ("rowcolumn", xmRowColumnWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* --------------------------- BULLETIN BOARDS ---------------------------- */

/* args work */
Widget XmxMakeVerticalBboard (Widget parent)
{
  Xmx_w = XtCreateManagedWidget ("bboard", xmBulletinBoardWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeVerticalBboardWithFont (Widget parent, String fontname)
{
  XFontStruct *_font;
  XmFontList _fontlist;

  _font = XLoadQueryFont (XtDisplay (parent), fontname);
  if (_font != (XFontStruct *)NULL)
    {
      _fontlist = XmFontListCreate (_font, XmSTRING_DEFAULT_CHARSET);
      XmxSetArg (XmNbuttonFontList, _fontlist);
      XmxSetArg (XmNlabelFontList, _fontlist);
      XmxSetArg (XmNtextFontList, _fontlist);
    }
  XmxSetArg (XmNmarginWidth, 0);
  XmxSetArg (XmNmarginHeight, 0);
  Xmx_w = XtCreateManagedWidget ("bboard", xmBulletinBoardWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeHorizontalBboard (Widget parent)
{
  XmxSetArg (XmNorientation, XmHORIZONTAL);
  Xmx_w = XtCreateManagedWidget ("bboard", xmBulletinBoardWidgetClass,
                              parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* -------------------- (nonworking) CENTERING BBOARD --------------------- */

#ifdef NONWORKING_CENTERING

/* args work */
Widget XmxMakeCenteringBboard (Widget parent, XtAppContext app)
{
  XtActionsRec rec;

  Xmx_w = XtCreateManagedWidget 
    ("bboard", xmBulletinBoardWidgetClass, parent, Xmx_wargs, Xmx_n);

  /* Does this have to happen more than once? */
  rec.string = "resize";
  rec.proc = XmxCenteringResize;
  XtAppAddActions (app, &rec, 1);

  /* This does, for sure... */
  XtOverrideTranslations 
    (Xmx_w, XtParseTranslationTable ("<Configure>: resize()"));
  
  Xmx_n = 0;
  return Xmx_w;
}

#endif /* NONWORKING_CENTERING */

/* -------------------------------- LABELS -------------------------------- */

/* args work */
void XmxAdjustLabelText (Widget label, String text)
{
  XmxSetArg (XmNlabelString,
	     XmStringCreateLtoR (text, XmSTRING_DEFAULT_CHARSET));
  XtSetValues (label, Xmx_wargs, Xmx_n);

  Xmx_n = 0;
  return;
}

/* args work */
Widget XmxMakeLabel (Widget parent, String name)
{
  XmxSetArg (XmNlabelString,
             XmStringCreateLtoR (name, XmSTRING_DEFAULT_CHARSET));
  Xmx_w = XtCreateManagedWidget ("label", xmLabelWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeNamedLabel (Widget parent, String name, String wname)
{
  XmxSetArg (XmNlabelString,
             XmStringCreateLtoR (name, XmSTRING_DEFAULT_CHARSET));
  Xmx_w = XtCreateManagedWidget (wname, xmLabelWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeBlankLabel (Widget parent)
{
  Xmx_w = XtCreateManagedWidget ("label", xmLabelWidgetClass,
                                 parent, Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* ------------------------------- DIALOGS -------------------------------- */

/* args work */
Widget XmxMakeErrorDialog (Widget parent, String name, String title)
{
  XmxSetArg (XmNmessageString,
             XmStringCreateLtoR (name, XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNdialogTitle,
             XmStringCreateLtoR (title, XmSTRING_DEFAULT_CHARSET));

  Xmx_w = XmCreateErrorDialog (parent, "error", Xmx_wargs, Xmx_n);
  XtUnmanageChild (XmMessageBoxGetChild (Xmx_w, XmDIALOG_CANCEL_BUTTON));
  XtUnmanageChild (XmMessageBoxGetChild (Xmx_w, XmDIALOG_HELP_BUTTON));

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeInfoDialog (Widget parent, String name, String title)
{
  XmxSetArg (XmNmessageString,
             XmStringCreateLtoR (name, XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNdialogTitle,
             XmStringCreateLtoR (title, XmSTRING_DEFAULT_CHARSET));

  Xmx_w = XmCreateInformationDialog (parent, "infozoid", Xmx_wargs, Xmx_n);
  XtUnmanageChild (XmMessageBoxGetChild (Xmx_w, XmDIALOG_CANCEL_BUTTON));
  XtUnmanageChild (XmMessageBoxGetChild (Xmx_w, XmDIALOG_HELP_BUTTON));

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeQuestionDialog (Widget parent, String question, String title,
			      XtCallbackProc cb, int yes_token, int no_token)
{
  XmxSetArg (XmNmessageString,
             XmStringCreateLtoR (question, XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNdialogTitle,
             XmStringCreateLtoR (title, XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNokLabelString,
	     XmStringCreateLtoR ("Yes", XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNcancelLabelString,
	     XmStringCreateLtoR ("No", XmSTRING_DEFAULT_CHARSET));
  
  Xmx_w = XmCreateQuestionDialog (parent, "question", Xmx_wargs, Xmx_n);
  XtUnmanageChild (XmMessageBoxGetChild (Xmx_w, XmDIALOG_HELP_BUTTON));

  XtAddCallback (Xmx_w, XmNcancelCallback, cb, _XmxMakeClientData (no_token));
  XtAddCallback (Xmx_w, XmNokCallback, cb, _XmxMakeClientData (yes_token));

  Xmx_n = 0;
  return Xmx_w;
}

/* ----------------------------- STRING UTILS ----------------------------- */

/* args do nothing */
XmString XmxMakeXmstrFromFile (String filename)
{
  FILE *_f;
  char _mstr[81];
  XmString _xmstr;

  _f = fopen (filename, "r");
  assert (_f != (FILE *)NULL);

  _xmstr = (XmString)NULL;
  while (!feof (_f))
    {
      if (fgets (_mstr, 80, _f) == (char *)NULL)
        break;
      _mstr[strlen (_mstr)-1] = '\0';
      if (_xmstr != (XmString)NULL)
        _xmstr = XmStringConcat (_xmstr, XmStringSeparatorCreate ());
      /* Used to be XmStringCreate; changed to standard call. */
      _xmstr = XmStringConcat
        (_xmstr, XmStringCreateLtoR (_mstr, XmSTRING_DEFAULT_CHARSET));
    }

  fclose (_f);
  return _xmstr;
}

/* args do nothing */
XmString XmxMakeXmstrFromString (String mstr)
{
  XmString _xmstr;

  _xmstr = XmStringCreateLtoR (mstr, XmSTRING_DEFAULT_CHARSET);
  return _xmstr;
}

/* args work */
Widget XmxMakeBboardDialog (Widget parent, String title)
{
  XmxSetArg (XmNdialogTitle,
             XmStringCreateLtoR (title, XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNautoUnmanage, False);
  XmxSetArg (XmNmarginWidth, 0);
  XmxSetArg (XmNmarginHeight, 0);

  Xmx_w = XmCreateBulletinBoardDialog (parent, "bbdialog", Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeFormDialog (Widget parent, String title)
{
  XmxSetArg (XmNdialogTitle,
             XmStringCreateLtoR (title, XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNautoUnmanage, False);
  /* XmxSetArg (XmNdefaultPosition, False); */

  Xmx_w = XmCreateFormDialog (parent, "formdialog", Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeFileSBDialog (Widget parent, String title, String selection_txt,
                            XtCallbackProc cb, int cb_data)
{
  Widget _selection_label;

  XmxSetArg (XmNdialogTitle,
             XmStringCreateLtoR (title, XmSTRING_DEFAULT_CHARSET));
  /* Can't set width of box with XmNwidth here... why not? */

  /* Create the FileSelectionBox with OK and Cancel buttons. */
  Xmx_w = XmCreateFileSelectionDialog (parent, "fsb", Xmx_wargs, Xmx_n);
  XtUnmanageChild (XmFileSelectionBoxGetChild (Xmx_w, XmDIALOG_HELP_BUTTON));
  XtAddCallback (Xmx_w, XmNokCallback, cb, _XmxMakeClientData (cb_data));
  XtAddCallback (Xmx_w, XmNcancelCallback, _XmxCancelCallback, 0);

  /* Set selection label to specified selection_txt. */
  Xmx_n = 0;
  _selection_label = 
    XmFileSelectionBoxGetChild (Xmx_w, XmDIALOG_SELECTION_LABEL);
  XmxSetArg (XmNlabelString, XmStringCreateLtoR (selection_txt,
                                                 XmSTRING_DEFAULT_CHARSET));
  XtSetValues (_selection_label, Xmx_wargs, Xmx_n);

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeHelpDialog (Widget parent, XmString xmstr, String title)
{
  XmxSetArg (XmNmessageString, xmstr);
  XmxSetArg (XmNdialogTitle,
             XmStringCreateLtoR (title, XmSTRING_DEFAULT_CHARSET));

  Xmx_w = XmCreateMessageDialog (parent, "helpdialog", Xmx_wargs, Xmx_n);
  XtUnmanageChild (XmMessageBoxGetChild (Xmx_w, XmDIALOG_CANCEL_BUTTON));
  XtUnmanageChild (XmMessageBoxGetChild (Xmx_w, XmDIALOG_HELP_BUTTON));

  Xmx_n = 0;
  return Xmx_w;
}

/* Boy, this is a hack. */
static
XmxCallback(_XmxHelpTextCancelCallback)
{
  /* This is highly dependent on the button being four layers
     below the dialog shell... what a ridiculous hack. */
  XtUnmanageChild (XtParent (XtParent (XtParent (XtParent (w)))));

  return;
}

/* args work */
Widget XmxMakeHelpTextDialog 
  (Widget parent, String str, String title, Widget *text_w)
{
  Widget _box, _outer_frame, _form;
  Widget _scr_text, _sep, _buttons_form;

  /* Create the dialog box. */
  _box = XmxMakeFormDialog (parent, title);

  /* Make it 3D. */
  _outer_frame = XmxMakeFrame (_box, XmxShadowOut);
  XmxSetConstraints
    (_outer_frame, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_FORM,
     NULL, NULL, NULL, NULL);

  /* Put form inside that, then frame for text window. */
  _form = XmxMakeForm (_outer_frame);

  /* Make multiline non-editable text window, with scrollbars. */
  XmxSetArg (XmNscrolledWindowMarginWidth, 10);
  XmxSetArg (XmNscrolledWindowMarginHeight, 10);
  XmxSetArg (XmNcursorPositionVisible, False);
  XmxSetArg (XmNeditable, False);
  XmxSetArg (XmNeditMode, XmMULTI_LINE_EDIT);
  XmxSetArg (XmNrows, 20);
  XmxSetArg (XmNcolumns, 60);
  XmxSetArg (XmNwordWrap, True);
  XmxSetArg (XmNscrollHorizontal, False);
  _scr_text = XmxMakeScrolledText (_form);
  XmTextSetString (_scr_text, str);

  /* Separate the text window/frame and the OK button. */
  XmxSetArg (XmNtopOffset, 10);
  _sep = XmxMakeHorizontalSeparator (_form);

  /* Make an OK button. */
  _buttons_form = XmxMakeFormAndOneButton 
    (_form, _XmxHelpTextCancelCallback, "OK", 0);

  /* Constraints for _form. */
  XmxSetConstraints 
    (XtParent (_scr_text), XmATTACH_FORM, XmATTACH_WIDGET, XmATTACH_FORM, 
     XmATTACH_FORM, NULL, _sep, NULL, NULL);
  XmxSetConstraints 
    (_sep, XmATTACH_NONE, XmATTACH_WIDGET, XmATTACH_FORM, XmATTACH_FORM,
     NULL, _buttons_form, NULL, NULL);
  XmxSetConstraints 
    (_buttons_form, XmATTACH_NONE, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_FORM,
     NULL, NULL, NULL, NULL);

  /* Return _scr_text in text_w argument. */
  *text_w = _scr_text;
  Xmx_w = _box;
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
void XmxAdjustHelpDialogText (Widget dialog, XmString message,
                              String title)
{
  XmxSetArg (XmNdialogTitle,
             XmStringCreateLtoR (title, XmSTRING_DEFAULT_CHARSET));
  XmxSetArg (XmNmessageString, message);
  XtSetValues (dialog, Xmx_wargs, Xmx_n);

  Xmx_n = 0;
  return;
}

/* args work */
void XmxAdjustDialogTitle (Widget dialog, String title)
{
  XmxSetArg (XmNdialogTitle, XmStringCreateSimple (title));
  XmxSetValues (dialog);

  Xmx_n = 0;
  return;
}

/* ------------------------------ SEPARATORS ------------------------------ */

/* args work */
Widget XmxMakeHorizontalSeparator (Widget parent)
{
  Xmx_w = XmCreateSeparatorGadget (parent, "separator", Xmx_wargs, Xmx_n);
  XtManageChild (Xmx_w);

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeHorizontalSpacer (Widget parent)
{
  XmxSetArg (XmNlabelString,
             XmStringCreateLtoR (" ", XmSTRING_DEFAULT_CHARSET));
  Xmx_w = XtCreateManagedWidget ("label", xmLabelGadgetClass, parent, 
                                 Xmx_wargs, Xmx_n);
  Xmx_n = 0;
  return Xmx_w;
}

/* args do nothing */
Widget XmxMakeHorizontalBoundary (Widget parent)
{
  /* To avoid confusion, nullify preloaded resources first. */
  Xmx_n = 0;
  XmxMakeHorizontalSpacer (parent);
  XmxMakeHorizontalSeparator (parent);
  XmxMakeHorizontalSpacer (parent);

  Xmx_n = 0;
  return Xmx_w;
}

/* ------------------------- TEXT & SCROLLED TEXT ------------------------- */

/* args work */
Widget XmxMakeScrolledText (Widget parent)
{
  Xmx_w = XmCreateScrolledText (parent, "scrolledtext",
				Xmx_wargs, Xmx_n);
  XtManageChild (Xmx_w);

  /* Remember this returns the Text Widget, NOT the ScrolledWindow Widget, 
     which is what needs to be tied into a form.  Use XtParent to get the
     actual ScrolledWindow. */
  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeText (Widget parent)
{
  Xmx_w = XmCreateText (parent, "text", Xmx_wargs, Xmx_n);
  XtManageChild (Xmx_w);

  Xmx_n = 0;
  return Xmx_w;
}

/* args work */
Widget XmxMakeTextField (Widget parent)
{
  Xmx_w = XmCreateTextField (parent, "textfield", Xmx_wargs, Xmx_n);
  XtManageChild (Xmx_w);

  Xmx_n = 0;
  return Xmx_w;
}

/* args do nothing */
void XmxTextSetString (Widget text, String str)
{
  XmTextSetString (text, str);
  XmTextShowPosition (text, 0);
  return;
}

/* args do nothing */
String XmxTextGetString (Widget text)
{
  return XmTextGetString (text);
}

/* args do nothing */
void XmxAddCallbackToText (Widget text, XtCallbackProc cb, int cb_data)
{
  XtAddCallback (text, XmNactivateCallback, cb, _XmxMakeClientData (cb_data));
  return;
}


#if 0

/* --------------------------- DRAWING VOLUMES ---------------------------- */

#ifdef __sgi
/* args work */
Widget
XmxMakeDrawingVolume 
  (Widget parent, int width, int height,
   GLXconfig *glxConfig,
   XtCallbackProc redraw_cb, XtCallbackProc resize_cb,
   XtCallbackProc ginit_cb)
{
  XmxSetArg (XmNwidth, width);
  XmxSetArg (XmNheight, height);
  XmxSetArg (GlxNglxConfig, glxConfig);

  Xmx_w = GlxCreateMDraw (parent, "drawingvolume", Xmx_wargs, Xmx_n);
  XtManageChild (Xmx_w);

  XtAddCallback (Xmx_w, GlxNexposeCallback, redraw_cb, _XmxMakeClientData (0));
  XtAddCallback (Xmx_w, GlxNresizeCallback, resize_cb, _XmxMakeClientData (0));
  XtAddCallback (Xmx_w, GlxNginitCallback, ginit_cb, _XmxMakeClientData (0));

  Xmx_n = 0;
  return Xmx_w;
}

void XmxInstallColormaps (Widget toplevel, Widget glw)
{
  Window windows[2];

  windows[0] = XtWindow (glw);
  windows[1] = XtWindow (toplevel);
  XSetWMColormapWindows (XtDisplay (toplevel), XtWindow (toplevel),
			 windows, 2);
  return;
}

void XmxInstallColormapsWithOverlay (Widget toplevel, Widget glw)
{
  Window windows[5];
  Window overlay, popup, underlay;
  Arg args[5];
  register int i=0;
  
  i=0;
  XtSetArg(args[i], GlxNoverlayWindow, &overlay); i++;
  XtSetArg(args[i], GlxNpopupWindow, &popup); i++;
  XtSetArg(args[i], GlxNunderlayWindow, &underlay); i++;
  XtGetValues(glw, args, i);
  i = 0;
  if (overlay)
    {
      windows[i] = overlay;
      i++;
    }
  if (popup)
    {
      windows[i] = popup;
      i++;
    }
  if (underlay)
    {
      windows[i] = underlay;
      i++;
    }
  windows[i] = XtWindow(glw); i++;
  windows[i] = XtWindow(toplevel); i++;
  XSetWMColormapWindows(XtDisplay(toplevel), XtWindow(toplevel), windows, i);
  
  return;
}

void XmxWinset (Widget w)
{
  GLXwinset (XtDisplay (w), XtWindow (w));
  return;
}
#endif /* __sgi */


#ifdef _IBMR2
/* args work */
Widget
XmxMakeDrawingVolume
  (Widget parent, int width, int height,
   XtCallbackProc redraw_cb, XtCallbackProc resize_cb,
   XtCallbackProc ginit_cb)
{
  XmxSetArg (XmNwidth, width);
  XmxSetArg (XmNheight, height);

#if 0
  fprintf (stderr, "[Xmx] Going to XtCreateWidget\n");
#endif
  Xmx_w = XtCreateWidget
    ("drawingvolume", glibWidgetClass, parent, Xmx_wargs, Xmx_n);
#if 0
  fprintf (stderr, "[Xmx] Done with XtCreateWidget, going to ManageChild\n");
#endif
  XtManageChild (Xmx_w);
#if 0
  fprintf (stderr, "[Xmx] Done with ManageChild\n");
#endif

  XtAddCallback (Xmx_w, XglNexposeCallback, redraw_cb, _XmxMakeClientData (0));
  XtAddCallback (Xmx_w, XglNresizeCallback, resize_cb, _XmxMakeClientData (0));
  XtAddCallback (Xmx_w, XglNgconfigCallback, ginit_cb, _XmxMakeClientData (0));

#if 0
  fprintf (stderr, "[Xmx] Leaving MakeDrawingVolume\n");
#endif
  Xmx_n = 0;
  return Xmx_w;
}

void XmxInstallColormaps (Widget toplevel, Widget glw)
{
  return;
}

void XmxWinset (Widget w)
{
  GlWinsetWidget (w);
  return;
}
#endif /* _IBMR2 */

#endif /* if 0 */

/* ----------------------------- BITMAP UTILS ----------------------------- */

/* args ignored and reset */
void XmxApplyBitmapToLabelWidget
  (Widget label, String data, unsigned int width, unsigned int height)
{
  Display *_disp;
  Pixel _fg, _bg;
  Pixmap _pix;

  _disp = XtDisplay (label);

  Xmx_n = 0;
  XmxSetArg (XmNforeground, &_fg);
  XmxSetArg (XmNbackground, &_bg);
  XtGetValues (label, Xmx_wargs, Xmx_n);
  Xmx_n = 0;

  _pix = XCreatePixmapFromBitmapData
    (_disp, DefaultRootWindow (_disp), data, width, height, _fg, _bg,
     DefaultDepthOfScreen (DefaultScreenOfDisplay (_disp)));
  XmxSetArg (XmNlabelPixmap, _pix);
  XmxSetArg (XmNlabelType, XmPIXMAP);
  XmxSetValues (label);

  Xmx_n = 0;
  return;
}

/* ------------------------ DIALOG CONTROL BUTTONS ------------------------ */

/* args apply to form */
Widget XmxMakeFormAndOneButton (Widget parent, XtCallbackProc cb, 
                                String name1, int cb_data1)
{
  Widget _form, _button1;

  XmxSetArg (XmNverticalSpacing, 8);
  XmxSetArg (XmNfractionBase, 3);
  _form = XmxMakeForm (parent);

  _button1 = XmxMakePushButton (_form, name1, cb, cb_data1);

  XmxSetConstraints 
    (_button1, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetPositions (_button1, XmxNoPosition, XmxNoPosition, 1, 2);
  XmxSetOffsets (_button1, XmxNoOffset, XmxNoOffset, 8, 8);
  
  Xmx_n = 0;
  Xmx_w = _form;
  return Xmx_w;
}

/* args apply to form */
Widget XmxMakeFormAndTwoButtons 
  (Widget parent, XtCallbackProc cb,
   String name1, String name2,
   int cb_data1, int cb_data2)
{
  Widget _form, _button1, _button2;

  XmxSetArg (XmNverticalSpacing, 8);
  XmxSetArg (XmNfractionBase, 2);
  _form = XmxMakeForm (parent);

  _button1 = XmxMakePushButton (_form, name1, cb, cb_data1);
  _button2 = XmxMakePushButton (_form, name2, cb, cb_data2);

  XmxSetConstraints 
    (_button1, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button2, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetPositions (_button1, XmxNoPosition, XmxNoPosition, 0, 1);
  XmxSetPositions (_button2, XmxNoPosition, XmxNoPosition, 1, 2);
  XmxSetOffsets (_button1, XmxNoOffset, XmxNoOffset, 8, 4);
  XmxSetOffsets (_button2, XmxNoOffset, XmxNoOffset, 4, 8);
  
  Xmx_n = 0;
  Xmx_w = _form;
  return Xmx_w;
}

/* args apply to form */
Widget XmxMakeFormAndTwoButtonsSqueezed
  (Widget parent, XtCallbackProc cb,
   String name1, String name2,
   int cb_data1, int cb_data2)
{
  Widget _form, _button1, _button2;

  XmxSetArg (XmNverticalSpacing, 8);
  XmxSetArg (XmNfractionBase, 5);
  _form = XmxMakeForm (parent);

  _button1 = XmxMakePushButton (_form, name1, cb, cb_data1);
  _button2 = XmxMakePushButton (_form, name2, cb, cb_data2);

  XmxSetConstraints 
    (_button1, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button2, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetPositions (_button1, XmxNoPosition, XmxNoPosition, 1, 2);
  XmxSetPositions (_button2, XmxNoPosition, XmxNoPosition, 3, 4);
  XmxSetOffsets (_button1, XmxNoOffset, XmxNoOffset, 8, 4);
  XmxSetOffsets (_button2, XmxNoOffset, XmxNoOffset, 4, 8);
  
  Xmx_n = 0;
  Xmx_w = _form;
  return Xmx_w;
}

/* args apply to form */
Widget XmxMakeFormAndThreeButtonsSqueezed
  (Widget parent, XtCallbackProc cb,
   String name1, String name2, String name3,
   int cb_data1, int cb_data2, int cb_data3)
{
  Widget _form, _button1, _button2, _button3;

  XmxSetArg (XmNverticalSpacing, 8);
  XmxSetArg (XmNfractionBase, 7);
  _form = XmxMakeForm (parent);

  _button1 = XmxMakePushButton (_form, name1, cb, cb_data1);
  _button2 = XmxMakePushButton (_form, name2, cb, cb_data2);
  _button3 = XmxMakePushButton (_form, name3, cb, cb_data3);

  XmxSetConstraints 
    (_button1, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button2, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button3, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetPositions (_button1, XmxNoPosition, XmxNoPosition, 1, 2);
  XmxSetPositions (_button2, XmxNoPosition, XmxNoPosition, 3, 4);
  XmxSetPositions (_button3, XmxNoPosition, XmxNoPosition, 5, 6);
  XmxSetOffsets (_button1, XmxNoOffset, XmxNoOffset, 8, 4);
  XmxSetOffsets (_button2, XmxNoOffset, XmxNoOffset, 4, 4);
  XmxSetOffsets (_button3, XmxNoOffset, XmxNoOffset, 4, 8);
  
  Xmx_n = 0;
  Xmx_w = _form;
  return Xmx_w;
}

/* args apply to form */
Widget XmxMakeFormAndThreeButtons
  (Widget parent, XtCallbackProc cb,
   String name1, String name2, String name3,
   int cb_data1, int cb_data2, int cb_data3)
{
  Widget _form, _button1, _button2, _button3;

  XmxSetArg (XmNverticalSpacing, 8);
  XmxSetArg (XmNfractionBase, 3);
  _form = XmxMakeForm (parent);

  _button1 = XmxMakePushButton (_form, name1, cb, cb_data1);
  _button2 = XmxMakePushButton (_form, name2, cb, cb_data2);
  _button3 = XmxMakePushButton (_form, name3, cb, cb_data3);

  XmxSetConstraints 
    (_button1, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button2, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button3, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetPositions (_button1, XmxNoPosition, XmxNoPosition, 0, 1);
  XmxSetPositions (_button2, XmxNoPosition, XmxNoPosition, 1, 2);
  XmxSetPositions (_button3, XmxNoPosition, XmxNoPosition, 2, 3);
  XmxSetOffsets (_button1, XmxNoOffset, XmxNoOffset, 8, 4);
  XmxSetOffsets (_button2, XmxNoOffset, XmxNoOffset, 4, 4);
  XmxSetOffsets (_button3, XmxNoOffset, XmxNoOffset, 4, 8);
  
  Xmx_n = 0;
  Xmx_w = _form;
  return Xmx_w;
}

/* args apply to form */
Widget XmxMakeFormAndFourButtons 
  (Widget parent, XtCallbackProc cb,
   String name1, String name2, String name3, String name4,
   int cb_data1, int cb_data2, int cb_data3, int cb_data4)
{
  Widget _form, _button1, _button2, _button3, _button4;

  XmxSetArg (XmNverticalSpacing, 8);
  XmxSetArg (XmNfractionBase, 4);
  _form = XmxMakeForm (parent);

  _button1 = XmxMakePushButton (_form, name1, cb, cb_data1);
  _button2 = XmxMakePushButton (_form, name2, cb, cb_data2);
  _button3 = XmxMakePushButton (_form, name3, cb, cb_data3);
  _button4 = XmxMakePushButton (_form, name4, cb, cb_data4);

  XmxSetConstraints 
    (_button1, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button2, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button3, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints 
    (_button4, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE,
     NULL, NULL, NULL, NULL);
  XmxSetPositions (_button1, XmxNoPosition, XmxNoPosition, 0, 1);
  XmxSetPositions (_button2, XmxNoPosition, XmxNoPosition, 1, 2);
  XmxSetPositions (_button3, XmxNoPosition, XmxNoPosition, 2, 3);
  XmxSetPositions (_button4, XmxNoPosition, XmxNoPosition, 3, 4);
  XmxSetOffsets (_button1, XmxNoOffset, XmxNoOffset, 8, 4);
  XmxSetOffsets (_button2, XmxNoOffset, XmxNoOffset, 4, 4);
  XmxSetOffsets (_button3, XmxNoOffset, XmxNoOffset, 4, 4);
  XmxSetOffsets (_button4, XmxNoOffset, XmxNoOffset, 4, 8);
  
  Xmx_n = 0;
  Xmx_w = _form;
  return Xmx_w;
}

