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

/* Support for whining to Mosaic authors. */

#include "mosaic.h"

static char *yap_msg =
  "Type your message in the editing area below and then press\n\
the Send button at the bottom of the window.  Your message\n\
will be mailed to the developers of NCSA Mosaic.\n\n\
Thanks in advance for your comments and feedback.";

/* ----------------------- mo_post_whine_window ------------------------ */

static XmxCallback (whine_win_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));
  char *msg, *fnam, *cmd;
  FILE *fp;

  switch (XmxExtractToken ((int)client_data))
    {
    case 0:
      XtUnmanageChild (win->whine_win);

      msg = XmxTextGetString (win->whine_text);
      if (!msg)
        return;
      if (msg[0] == '\0')
        return;

      if (can_instrument && win->instrument_usage)
        mo_inst_register_action (mo_inst_whine, 
                                 win->current_node->url);
      
      fnam = (char *)malloc (L_tmpnam + 16);
      tmpnam (fnam);

      fp = fopen (fnam, "w");
      if (!fp)
        goto oops;

      fputs (msg, fp);
      fclose (fp);

      cmd = (char *)malloc ((64 + strlen (Rdata.mail_command) + 
                             strlen (MO_DEVELOPER_ADDRESS) + 
                             strlen (fnam) + 24) * sizeof (char));
      sprintf (cmd, "%s -s \"NCSA Mosaic %s on %s.\" %s < %s", 
               Rdata.mail_command,
               MO_VERSION_STRING, MO_MACHINE_TYPE, MO_DEVELOPER_ADDRESS, fnam);
      if ((system (cmd)) != 0)
        {
          XmxMakeErrorDialog 
            (win->base,
             "Unable to mail document;\ncheck the value of the X resource\nmailCommand.", 
             "Mail Error");
          XtManageChild (Xmx_w);
        }
      free (cmd);
      
    oops:
      free (msg);

      cmd = (char *)malloc ((strlen (fnam) + 32) * sizeof (char));
      sprintf (cmd, "/bin/rm -f %s &", fnam);
      system (cmd);

      free (fnam);
      free (cmd);
      
      break;
    case 1:
      XtUnmanageChild (win->whine_win);
      /* Do nothing. */
      break;
    case 2:
      mo_open_another_window
        (win, 
         "http://hoohoo.ncsa.uiuc.edu:80/mosaic-docs/help-on-whining.html",
         NULL, NULL);
      break;
    }

  return;
}

mo_status mo_post_whine_win (mo_window *win)
{
  if (!win->whine_win)
    {
      Widget dialog_frame;
      Widget dialog_sep, buttons_form;
      Widget whine_form, yap_label;
      
      /* Create it for the first time. */
      XmxSetUniqid (win->id);
      win->whine_win = XmxMakeFormDialog 
        (win->base, "NCSA Mosaic: Mail Developers");
      dialog_frame = XmxMakeFrame (win->whine_win, XmxShadowOut);

      /* Constraints for base. */
      XmxSetConstraints 
        (dialog_frame, XmATTACH_FORM, XmATTACH_FORM, 
         XmATTACH_FORM, XmATTACH_FORM, NULL, NULL, NULL, NULL);
      
      /* Main form. */
      whine_form = XmxMakeForm (dialog_frame);
      
      yap_label = XmxMakeLabel (whine_form, yap_msg);

      XmxSetArg (XmNscrolledWindowMarginWidth, 10);
      XmxSetArg (XmNscrolledWindowMarginHeight, 10);
      XmxSetArg (XmNcursorPositionVisible, True);
      XmxSetArg (XmNeditable, True);
      XmxSetArg (XmNeditMode, XmMULTI_LINE_EDIT);
      XmxSetArg (XmNrows, 15);
      XmxSetArg (XmNcolumns, 80);
      /* XmxSetArg (XmNwordWrap, True); */
      /* XmxSetArg (XmNscrollHorizontal, False); */
      win->whine_text = XmxMakeScrolledText (whine_form);
      
      dialog_sep = XmxMakeHorizontalSeparator (whine_form);
      
      buttons_form = XmxMakeFormAndThreeButtonsSqueezed
        (whine_form, whine_win_cb, "Send", "Dismiss", "Help...", 0, 1, 2);

      /* Constraints for whine_form. */
      XmxSetOffsets (yap_label, 8, 0, 0, 0);
      XmxSetConstraints
        (yap_label, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_FORM, XmATTACH_FORM,
         NULL, NULL, NULL, NULL);

      XmxSetOffsets (XtParent (win->whine_text), 3, 0, 3, 3);
      XmxSetConstraints
        (XtParent (win->whine_text), XmATTACH_WIDGET, XmATTACH_WIDGET, 
         XmATTACH_FORM, XmATTACH_FORM,
         yap_label, dialog_sep, NULL, NULL);

      XmxSetArg (XmNtopOffset, 10);
      XmxSetConstraints 
        (dialog_sep, XmATTACH_NONE, XmATTACH_WIDGET, XmATTACH_FORM, 
         XmATTACH_FORM,
         NULL, buttons_form, NULL, NULL);
      XmxSetConstraints 
        (buttons_form, XmATTACH_NONE, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_FORM,
         NULL, NULL, NULL, NULL);
    }
  
  XtManageChild (win->whine_win);
  XmxTextSetString (win->whine_text, "");
  
  return mo_succeed;
}

