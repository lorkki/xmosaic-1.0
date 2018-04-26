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

#include "mosaic.h"
#include <pwd.h>
#include <sys/types.h>
#include <Xm/ToggleB.h>
#include <Xm/TextF.h>
#include "annotate.xbm"

typedef enum
{
  mo_annotation_public = 0, mo_annotation_workgroup, mo_annotation_private
} mo_pubpri_token;

typedef enum
{
  mo_new_annotation = 0, mo_edit_annotation
} mo_amode;

/* ---------------------- mo_is_editable_annotation ----------------------- */

mo_status mo_is_editable_annotation (char *text)
{
  /* For now, it can only be a pan. */
  return mo_is_editable_pan (text);
}

/* ---------------------- mo_fetch_annotation_links ----------------------- */

char *mo_fetch_annotation_links (char *url, int on_top)
{
  char *pans = mo_fetch_pan_links (url, on_top);

#ifdef GRPANS
  char *grpans = mo_fetch_grpan_links (url);

  if (!pans && !grpans)
    return NULL;
  else if (pans && !grpans)
    return pans;
  else if (!pans && grpans)
    return grpans;
  else /* both exist */
    {
      char *ret = (char *)malloc (strlen (pans) + strlen (grpans) + 8);
      strcpy (ret, grpans);
      strcat (ret, pans);
      free (grpans);
      free (pans);
      return ret;
    }
#else /* not GRPANS */
  return pans;
#endif
}

/* ------------------------- set_annotation_mode -------------------------- */

static void set_annotation_mode (mo_window *win, int mode)
{
  win->annotation_mode = mode;

  XmxSetSensitive 
    (win->delete_button, 
     mode == mo_edit_annotation ? XmxSensitive : XmxNotSensitive);
  return;
}

/* ----------------------- mo_post_annotate_window ------------------------ */

static XmxCallback (annotate_win_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));
  char *msg, *fnam, *cmd;
  FILE *fp;

  switch (XmxExtractToken ((int)client_data))
    {
    case 0:
#ifdef GRPAN
      if (win->pubpri == mo_annotation_public ||
          win->pubpri == mo_annotation_workgroup)
        {
          XmxMakeErrorDialog 
            (win->annotate_win, 
             "Sorry, public annotations aren't working yet.", 
             "Just Can't Do It");
          XtManageChild (Xmx_w);
        }
      else
#endif
        {
          char *txt = XmxTextGetString (win->annotate_text);
          /* Make sure text exists before actually annotating. */
          /* In the case of editing, no text should result in
             a delete. */
          if (txt && *txt)
            {
              if (win->annotation_mode == mo_new_annotation)
                {
                  mo_new_pan 
                    (win->current_node->url, 
                     XmxTextGetString (win->annotate_title), 
                     XmxTextGetString (win->annotate_author), 
                     txt);
                  if (can_instrument && win->instrument_usage)
                    mo_inst_register_action 
                      (mo_inst_annotate, win->current_node->url);
                  /* Inefficient, but safe. */
                  mo_write_pan_list ();
                  /* Snarf current scrollbar values to make sure we
                     jump back to current place in do_window_text. */
                  mo_snarf_scrollbar_values (win);
                  mo_do_window_node (win, win->current_node);
                }
              else /* edit annotation */
                {
                  mo_modify_pan (win->editing_id,
                                 XmxTextGetString (win->annotate_title), 
                                 XmxTextGetString (win->annotate_author), 
                                 txt);
                  if (can_instrument && win->instrument_usage)
                    mo_inst_register_action 
                      (mo_inst_edit_annotation, win->current_node->url);
                  /* Inefficient, but safe. */
                  mo_write_pan_list ();
                  mo_reload_window_text (win);
                }
            }
          XtUnmanageChild (win->annotate_win);
        }
      break;
    case 1:
      XtUnmanageChild (win->annotate_win);
      /* Do nothing. */
      break;
    case 2:
      mo_open_another_window
        (win,
         "http://hoohoo.ncsa.uiuc.edu:80/mosaic-docs/help-on-annotate-win.html",
         NULL, NULL);
      break;
    }

  return;
}

#ifdef GRPAN
static XmxCallback (passwd_toggle_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));
  int on = XmToggleButtonGetState (win->passwd_toggle);
  
  if (on)
    {
      XmxSetSensitive (win->passwd_label, XmxSensitive);
      XmxSetSensitive (win->annotate_passwd, XmxSensitive);
    }
  else
    {
      XmxSetSensitive (win->passwd_label, XmxNotSensitive);
      XmxSetSensitive (win->annotate_passwd, XmxNotSensitive);
      XmxTextSetString (win->annotate_passwd, "\0");
    }

  return;
}
#endif

/* ---------------------- clear slate & include file ---------------------- */

static void do_slate (mo_window *win)
{
  struct passwd *pw = getpwuid (getuid ());
  char namestr[200], *author;
  extern char *machine;
  
  if (Rdata.default_author_name)
    author = Rdata.default_author_name;
  else
    author = pw->pw_gecos;

  sprintf (namestr, "%s (%s@%s)\0", author, pw->pw_name, machine);
  XmxTextSetString (win->annotate_author, namestr);

  sprintf (namestr, "Annotation by %s\0", author);
  XmxTextSetString (win->annotate_title, namestr);

#ifdef GRPAN
  XmxTextSetString (win->annotate_passwd, "\0");
#endif
  XmxTextSetString (win->annotate_text, "\0");

  return;
}

static XmxCallback (include_fsb_cb)
{
  char *fname;
  FILE *fp;
  char *line = (char *)malloc (MO_LINE_LENGTH * sizeof (char));

  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));

  XtUnmanageChild (win->include_fsb);
  fname = (char *)malloc (128 * sizeof (char));
  
  XmStringGetLtoR (((XmFileSelectionBoxCallbackStruct *)call_data)->value,
                   XmSTRING_DEFAULT_CHARSET,
                   &fname);

  fp = fopen (fname, "r");
  if (!fp)
    {
      XmxMakeErrorDialog (win->annotate_win, 
                          "Sorry, cannot open that file.", 
                          "Just Can't Do It");
      XtManageChild (Xmx_w);
      return;
    }
  
  while (1)
    {
      line = fgets (line, MO_LINE_LENGTH, fp);
      if (!line)
        goto done;
      if (!(*line))
        goto done;
      
      XmTextInsert (win->annotate_text,
                    XmTextGetInsertionPosition (win->annotate_text),
                    line);
    }

 done:
  return;
}

static XmxCallback (include_button_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));
  if (!win->include_fsb)
    {
      win->include_fsb = XmxMakeFileSBDialog
        (win->annotate_win,
         "NCSA Mosaic: Include File In Annotation",
         "Name of file to include:",
         include_fsb_cb, 0);
    }
  else
    {
      XmFileSelectionDoSearch (win->include_fsb, NULL);
    }
  
  XmxManageRemanage (win->include_fsb);

  return;
}

static XmxCallback (clear_button_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));

  do_slate (win);

  return;
}

/* ------------------------- mo_delete_annotation ------------------------- */

mo_status mo_delete_annotation (mo_window *win, int id)
{
  /* Delete the annotation currently being viewed. */
  extern char *home_document;

  mo_delete_pan (id);
  if (can_instrument && win->instrument_usage)
    mo_inst_register_action 
      (mo_inst_delete_annotation, win->current_node->url);
  
  /* Now that we've deleted the annotation, take care of
     business.  Damn, but this shouldn't have to happen here. */
  if (win->current_node->previous)
    {
      mo_node *prev = win->current_node->previous;
      
      mo_kill_node (win, win->current_node);
      prev->next = NULL;
      win->current_node = prev;
      /* Set the text. */
      mo_reload_window_text (win);
    }      
  else
    {
      /* No previous node; this only happens if someone's dumb enough
         to have his/her home document be a personal annotation which
         he/she then deletes. */
      mo_kill_node (win, win->current_node);
      win->current_node = NULL;
      win->history = NULL;

      mo_load_window_text (win, home_document, NULL);
    }
  
  return mo_succeed;
}

static XmxCallback (delete_button_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));

  XtUnmanageChild (win->annotate_win);
  mo_delete_annotation (win, win->editing_id);

  return;
}

/* ---------------------------- public/private ---------------------------- */

#ifdef GRPAN
static XmxOptionMenuStruct pubpri_opts[] =
{
  { "Personal Annotation", mo_annotation_private, XmxSet },
  { "Workgroup Annotation", mo_annotation_workgroup, XmxNotSet },
  { "Public Annotation",   mo_annotation_public, XmxNotSet },
  { NULL },
};
#endif

#ifdef GRPAN
static void do_privacy (mo_window *win)
{
  /* If pubpri = mo_annotation_public, unfreeze the appropriate
     GUI pieces.  Else, freeze 'em. */
  int on = XmToggleButtonGetState (win->passwd_toggle);

  if (win->pubpri == mo_annotation_public ||
      win->pubpri == mo_annotation_workgroup)
    {
      /* Match current state of on. */
      if (on)
        {
          XmxSetSensitive (win->passwd_label, XmxSensitive);
          XmxSetSensitive (win->annotate_passwd, XmxSensitive);
        }
      XmxSetSensitive (win->passwd_toggle, XmxSensitive);
      XtVaSetValues (win->passwd_toggle, XmNfillOnSelect, True, NULL);
    }
  else
    {
      /* We're private.  No password, nohow. */
      XmxSetSensitive (win->passwd_label, XmxNotSensitive);
      XmxSetSensitive (win->annotate_passwd, XmxNotSensitive);
      XmxTextSetString (win->annotate_passwd, "\0");
      XmxSetSensitive (win->passwd_toggle, XmxNotSensitive);
      XtVaSetValues (win->passwd_toggle, XmNfillOnSelect, False, NULL);
    }

  return;
}
#endif

#ifdef GRPAN
static int done_warning = 0;
static XmxCallback (pubpri_opts_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));
  
  win->pubpri = XmxExtractToken ((int)client_data);

  do_privacy (win);
  if ((win->pubpri == mo_annotation_public ||
       win->pubpri == mo_annotation_workgroup)
      && !done_warning)
    {
      done_warning = 1;
      XmxMakeErrorDialog 
        (win->annotate_win, 
         "Please keep in mind that public annotations\njust don't work yet.", 
         "Won't Be Able To Do The Job");
      XtManageChild (Xmx_w);
    }

  return;
}
#endif

/* OK, here's how we're gonna work this.

   There will still only be one annotate window per document view window.
   The annotate window will either be in new_annotation or edit_annotation mode.
   This mode will be stored in win->annotation_mode.
   Only routines in this file will touch that mode.

   The mode will be set upon entry to mo_post_annotate_win.

   If mo_post_annotate_win is told it's supposed to edit,
   then it installs title, author, text, etc. as it sees fit.

   If, while in edit mode, 'Clear Slate' gets hit, 
   mode flips to mo_edit_annotation regardless -- everything's wiped out.
   */

static void make_annotate_win (mo_window *win)
{
  Widget dialog_frame;
  Widget dialog_sep, buttons_form;
  Widget annotate_form, yap_label;
  Widget logo;
  Widget author_label, title_label, text_label;
  Widget include_button, clear_button;
  Widget sep;
  Widget passwd_frame, passwd_f;
  
  /* Create it for the first time. */
  XmxSetUniqid (win->id);
  XmxSetArg (XmNresizePolicy, XmRESIZE_GROW);
  win->annotate_win = XmxMakeFormDialog 
    (win->base, "NCSA Mosaic: Annotate Window");
  dialog_frame = XmxMakeFrame (win->annotate_win, XmxShadowOut);
  
  /* Constraints for base. */
  XmxSetConstraints 
    (dialog_frame, XmATTACH_FORM, XmATTACH_FORM, 
     XmATTACH_FORM, XmATTACH_FORM, NULL, NULL, NULL, NULL);
  
  /* Main form. */
  annotate_form = XmxMakeForm (dialog_frame);
  
  author_label = XmxMakeLabel (annotate_form, "Annotation Author:");
  win->annotate_author = XmxMakeTextField (annotate_form);
  
  title_label = XmxMakeLabel (annotate_form, "Annotation Title:");
  win->annotate_title = XmxMakeTextField (annotate_form);

  /* Let's just not do any of this shit if we don't have
     group annotations yet. */
#ifdef GRPAN  
  win->pubpri_menu = XmxRMakeOptionMenu
    (annotate_form, NULL, pubpri_opts_cb, pubpri_opts);
  XmxRSetSensitive (win->pubpri_menu, mo_annotation_public, XmxNotSensitive);
#ifndef GRPAN
  XmxRSetSensitive (win->pubpri_menu, mo_annotation_workgroup, XmxNotSensitive);
#endif
  win->pubpri = mo_annotation_private;
  
  /* --- Frame for the password fields. --- */
  XmxSetArg (XmNmarginWidth, 4);
  XmxSetArg (XmNmarginHeight, 4);
  XmxSetArg (XmNadjustLast, True);
  passwd_frame = XmxMakeFrame (annotate_form, XmxShadowEtchedOut);
  passwd_f = XmxMakeForm (passwd_frame);
  {
    win->passwd_toggle = XmxMakeToggleButton 
      (passwd_f, "Password Protection", passwd_toggle_cb, 0);
    XmxSetToggleButton (win->passwd_toggle, XmxSet);
    
    win->passwd_label = XmxMakeLabel (passwd_f, "Password:");
    XmxSetArg (XmNcolumns, 20);
    win->annotate_passwd = XmxMakeTextField (passwd_f);

    XmxSetOffsets (win->passwd_toggle, 1, 0, 2, 10);
    XmxSetConstraints 
      (win->passwd_toggle, XmATTACH_FORM, XmATTACH_FORM, 
       XmATTACH_FORM, XmATTACH_NONE, NULL, NULL, NULL, NULL);
    XmxSetOffsets (win->passwd_label, 1, 0, 15, 10);
    XmxSetConstraints
      (win->passwd_label, XmATTACH_FORM, XmATTACH_FORM,
       XmATTACH_WIDGET, XmATTACH_NONE, NULL, NULL, win->passwd_toggle, NULL);
    XmxSetOffsets (win->annotate_passwd, 1, 0, 6, 6);
    XmxSetConstraints
      (win->annotate_passwd, XmATTACH_FORM, XmATTACH_FORM,
       XmATTACH_WIDGET, XmATTACH_FORM, NULL, NULL, win->passwd_label, NULL);
  }
#endif /* GRPAN */
  
  logo = XmxMakeNamedLabel (annotate_form, NULL, "logo");
  XmxApplyBitmapToLabelWidget
    (logo, annotate_bits, annotate_width, annotate_height);
  
  sep = XmxMakeHorizontalSeparator (annotate_form);
  
  text_label = XmxMakeLabel 
    (annotate_form, "Enter the annotation text:");

  clear_button = XmxMakePushButton
    (annotate_form, "Clean Slate", clear_button_cb, 0);
  include_button = XmxMakePushButton
    (annotate_form, "Include File...", include_button_cb, 0);
  win->delete_button = XmxMakePushButton
    (annotate_form, "Delete", delete_button_cb, 0);
  
  XmxSetArg (XmNscrolledWindowMarginWidth, 10);
  XmxSetArg (XmNscrolledWindowMarginHeight, 8);
  XmxSetArg (XmNcursorPositionVisible, True);
  XmxSetArg (XmNeditable, True);
  XmxSetArg (XmNeditMode, XmMULTI_LINE_EDIT);
  XmxSetArg (XmNrows, 15);
  XmxSetArg (XmNcolumns, 80);
  win->annotate_text = XmxMakeScrolledText (annotate_form);
  
  dialog_sep = XmxMakeHorizontalSeparator (annotate_form);
  
  buttons_form = XmxMakeFormAndThreeButtonsSqueezed
    (annotate_form, annotate_win_cb, "Commit", "Dismiss", "Help...", 
     0, 1, 2);
  
  /* Constraints for annotate_form. */
  /* author_label: top form, bottom nothing,
     left form, right nothing. */
  XmxSetOffsets (author_label, 14, 10, 10, 10);
  XmxSetConstraints
    (author_label, XmATTACH_FORM, XmATTACH_NONE,
     XmATTACH_FORM, XmATTACH_NONE, NULL, NULL, NULL, NULL);
  /* annotate_author: top form, bottom nothing,
     left author_label, right logo. */
  XmxSetOffsets (win->annotate_author, 10, 10, 10, 10);
  XmxSetConstraints
    (win->annotate_author, XmATTACH_FORM, XmATTACH_NONE,
     XmATTACH_WIDGET, XmATTACH_WIDGET,
     NULL, NULL, author_label, logo);
  /* title_label: top annotate_author, bottom nothing,
     left form, right nothing */
  XmxSetOffsets (title_label, 14, 10, 10, 10);
  XmxSetConstraints
    (title_label, XmATTACH_WIDGET, XmATTACH_NONE,
     XmATTACH_FORM, XmATTACH_NONE, 
     win->annotate_author, NULL, NULL, NULL);
  /* annotate_title: top annotate_author, bottom nothing,
     left title_label, right logo. */
  XmxSetOffsets (win->annotate_title, 10, 10, 10, 10);
  XmxSetConstraints
    (win->annotate_title, XmATTACH_WIDGET, XmATTACH_NONE,
     XmATTACH_WIDGET, XmATTACH_WIDGET,
     win->annotate_author, NULL, title_label, logo);
#ifdef GRPAN
  XmxSetOffsets (win->pubpri_menu->base, 19, 10, 0, 10);
  XmxSetConstraints
    (win->pubpri_menu->base, XmATTACH_WIDGET, XmATTACH_NONE,
     XmATTACH_FORM, XmATTACH_NONE, 
     win->annotate_title, NULL, NULL, NULL);
  /* passwd_frame: top annotate_title, bottom nothing,
     left pubpri_menu, right form */
  XmxSetOffsets (passwd_frame, 10, 10, 10, 10);
  XmxSetConstraints
    (passwd_frame, XmATTACH_WIDGET, XmATTACH_NONE,
     XmATTACH_WIDGET, XmATTACH_FORM,
     win->annotate_title, NULL, win->pubpri_menu->base, NULL);
  XmxSetOffsets (sep, 10, 0, 0, 0);
  XmxSetConstraints
    (sep, XmATTACH_WIDGET, XmATTACH_NONE, XmATTACH_FORM,
     XmATTACH_FORM, passwd_frame, NULL, NULL, NULL);
#else /* not GRPAN */
  XmxSetOffsets (sep, 10, 0, 0, 0);
  XmxSetConstraints
    (sep, XmATTACH_WIDGET, XmATTACH_NONE, XmATTACH_FORM,
     XmATTACH_FORM, win->annotate_title, NULL, NULL, NULL);
#endif
    
  /* text_label: top sep, bottom nothing,
     left form, right nothing */
  XmxSetOffsets (text_label, 12, 0, 10, 10);
  XmxSetConstraints
    (text_label, XmATTACH_WIDGET, XmATTACH_NONE,
     XmATTACH_FORM, XmATTACH_NONE,
     sep, NULL, NULL, NULL);
  
  /* Buttons */
  XmxSetOffsets (clear_button, 8, 0, 0, 3);
  XmxSetConstraints
    (clear_button, XmATTACH_WIDGET, XmATTACH_NONE,
     XmATTACH_NONE, XmATTACH_WIDGET,
     sep, NULL, NULL, include_button);
  XmxSetOffsets (include_button, 8, 0, 0, 3);
  XmxSetConstraints
    (include_button, XmATTACH_WIDGET, XmATTACH_NONE,
     XmATTACH_NONE, XmATTACH_WIDGET,
     sep, NULL, NULL, win->delete_button);
  XmxSetOffsets (win->delete_button, 8, 0, 10, 10);
  XmxSetConstraints
    (win->delete_button, XmATTACH_WIDGET, XmATTACH_NONE,
     XmATTACH_NONE, XmATTACH_FORM,
     sep, NULL, NULL, NULL);
  
  /* logo: top form, bottom nothing,
     left nothing, right form. */
  XmxSetOffsets (logo, 5, 10, 10, 5);
  XmxSetConstraints
    (logo, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_NONE, XmATTACH_FORM,
     NULL, NULL, NULL, NULL);
  
  XmxSetOffsets (XtParent (win->annotate_text), 0, 2, 0, 0);
  XmxSetConstraints
    (XtParent (win->annotate_text), XmATTACH_WIDGET, XmATTACH_WIDGET, 
     XmATTACH_FORM, XmATTACH_FORM,
     text_label, dialog_sep, NULL, NULL);
  
  XmxSetArg (XmNtopOffset, 10);
  XmxSetConstraints 
    (dialog_sep, XmATTACH_NONE, XmATTACH_WIDGET, XmATTACH_FORM, 
     XmATTACH_FORM,
     NULL, buttons_form, NULL, NULL);
  XmxSetConstraints 
    (buttons_form, XmATTACH_NONE, XmATTACH_FORM, XmATTACH_FORM, 
     XmATTACH_FORM,
     NULL, NULL, NULL, NULL);

  XtRealizeWidget (win->annotate_win);
  do_slate (win);
#ifdef GRPAN
  do_privacy (win);
#endif
  win->include_fsb = 0;
  
  return;
}


/* For editing, we should pass in:
     (x) a flag to indicate that we're editing
     (a) the URL we're annotation
     (b) the ID of the current annotation
     (c) the starting Title
     (d) the starting Author
     (e) the starting Text.
     (f) the original filename (probably not needed). */
/* If editflag is high, then:
     * window title should change (to Edit Annotation)
     * 'Annotate' should change to 'Commit'.
   Right now this doesn't happen. */
mo_status mo_post_annotate_win 
  (mo_window *win, int edit_flag, int id, char *title, char *author, 
   char *text, char *fname)
{
  /* First thing we do is create the window if it doesn't exist yet. */
  if (!win->annotate_win)
    make_annotate_win (win);
  
  /* If we're in edit mode, then do the right thing. */
  if (edit_flag)
    {
      set_annotation_mode (win, mo_edit_annotation);
      win->editing_id = id;

      XmxTextSetString (win->annotate_author, author);
      XmxTextSetString (win->annotate_title, title);
#ifdef GRPAN
      XmxTextSetString (win->annotate_passwd, "\0");
#endif
      XmxTextSetString (win->annotate_text, text);
      XmTextSetTopCharacter (win->annotate_text, 0);
    }
  else
    {
      set_annotation_mode (win, mo_new_annotation);
    }

  /* Finally, we manage. */
  XtManageChild (win->annotate_win);
  
  return mo_succeed;
}

