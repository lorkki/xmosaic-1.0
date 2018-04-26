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

/* We look for a file called ~/.mosaicusr.  If it exists, we read it
   and look for this format:

   ncsa-mosaic-usr-format-1
   instrument yes                    [or instrument no]

   If it doesn't exist, we put up the initial splash screen
   and ask the user what to do, what to do. */

#define MO_USR_FILE_NAME ".mosaicusr"
#define NCSA_USR_FORMAT_COOKIE_ONE "ncsa-mosaic-usr-format-1"

#define YAK_YAK_YAK \
"Welcome to NCSA Mosaic for the X Window System.\n\n\
NCSA Mosaic is part of a research program being conducted by the\n\
National Center for Supercomputing Applications at the University of Illinois\n\
at Urbana-Champaign.  We are studying how information is discovered, retrieved,\n\
manipulated, published, and shared on wide-area networks like the Internet.\n\n\
As part of this research, we would like to ask you to allow us to instrument\n\
your use of NCSA Mosaic.  If you agree to participate, each time you use Mosaic\n\
a log of the activities you undertake will be sent via email to NCSA, where it\n\
will be added to a large and growing pool of such information and used as the\n\
basis for our research.\n\n\
All such information will be held in the strictest confidence and never released\n\
to anyone but the principle investigators of the research program (the lead\n\
investigator is Joseph Hardin, hardin@ncsa.uiuc.edu).  Further, you will have the\n\
option of turning off Mosaic's built-in instrumentation at any time, temporarily\n\
or permanently.  We ask you to participate in this study as doing so will allow us\n\
both to fulfill our research goals and to build more and better software for you\n\
to use.\n\n\
Please choose either 'Participate' or 'Don't Participate' below.\n\
We thank you for your support.\n"

extern int defer_initial_window;
extern Widget toplevel;

static Widget dialog_win;

/* ------------------------ mo_post_startup_dialog ------------------------ */

static XmxCallback (dialog_win_cb)
{
  FILE *fp;
  char *home = getenv ("HOME"), *filename;

  switch (XmxExtractToken ((int)client_data))
    {
    case 0:
      /* participate */
      can_instrument = 1;
      break;
    case 1:
      /* don't participate */
      can_instrument = 0;
      break;
    }

  if (!home)
    home = "/tmp";
  
  filename = (char *)malloc (strlen (home) + 
                             strlen (MO_USR_FILE_NAME) + 8);
  sprintf (filename, "%s/%s\0", home, MO_USR_FILE_NAME);
  fp = fopen (filename, "w");
  if (!fp)
    goto never_mind;
  
  fprintf (fp, "%s\n", NCSA_USR_FORMAT_COOKIE_ONE);
  fprintf (fp, "instrument %s\n", can_instrument ? "yes" : "no");
  
  fclose (fp);
  
 never_mind:
  free (filename);
  XtUnmanageChild (dialog_win);
  mo_open_initial_window ();
}

static void mo_post_startup_dialog (void)
{
  Widget dialog_frame, dialog_form;
  Widget dialog_sep, buttons_form;
  Widget label, logo;

  dialog_win = XmxMakeFormDialog
    (toplevel, "NCSA Mosaic: Startup Questionnaire");
  dialog_frame = XmxMakeFrame (dialog_win, XmxShadowOut);

  /* Constraints for dialog_win. */
  XmxSetConstraints 
    (dialog_frame, XmATTACH_FORM, XmATTACH_FORM, 
     XmATTACH_FORM, XmATTACH_FORM, NULL, NULL, NULL, NULL);
      
  dialog_form = XmxMakeForm (dialog_frame);

  label = XmxMakeLabel (dialog_form, YAK_YAK_YAK);
  
  dialog_sep = XmxMakeHorizontalSeparator (dialog_form);
  
  buttons_form = XmxMakeFormAndTwoButtons
    (dialog_form, dialog_win_cb, "Participate", "Don't Participate", 0, 1);
  
  XmxSetOffsets (label, 10, 10, 10, 10);
  XmxSetConstraints
    (label, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_FORM, XmATTACH_FORM,
     NULL, NULL, NULL, NULL);
  XmxSetConstraints
    (dialog_sep, XmATTACH_WIDGET, XmATTACH_WIDGET, 
     XmATTACH_FORM, XmATTACH_FORM,
     label, buttons_form, NULL, NULL);
  XmxSetConstraints
    (buttons_form, XmATTACH_NONE, XmATTACH_FORM, XmATTACH_FORM, XmATTACH_FORM,
     NULL, NULL, NULL, NULL);
  
  XtManageChild (dialog_win);
  
  return;
}

/* ------------------------------ mo_startup ------------------------------ */

mo_status mo_startup (void)
{
  FILE *fp;
  char *home = getenv ("HOME"), *filename;

  if (!home)
    home = "/tmp";

  filename = (char *)malloc (strlen (home) + strlen (MO_USR_FILE_NAME) + 8);
  sprintf (filename, "%s/%s\0", home, MO_USR_FILE_NAME);
  fp = fopen (filename, "r");
  if (!fp)
    goto new_user;

  /* File exists; read it. */
  {
    char *line = (char *)malloc (MO_LINE_LENGTH * sizeof (char));

    /* Check for starting line. */
    line = fgets (line, MO_LINE_LENGTH, fp);
    if (strncmp (line, NCSA_USR_FORMAT_COOKIE_ONE,
                 strlen (NCSA_USR_FORMAT_COOKIE_ONE)))
      {
        fclose (fp);
        goto new_user;
      }

    /* Check for instrument permission line. */
    line = fgets (line, MO_LINE_LENGTH, fp);
    if (strncmp (line, "instrument ", strlen ("instrument ")))
      {
        fclose (fp);
        goto new_user;
      }
    if (!strncmp (line, "instrument yes", strlen ("instrument yes")))
      can_instrument = 1;
    else
      can_instrument = 0;
  }
  
  fclose (fp);
  free (filename);
  defer_initial_window = 0;
  return mo_succeed;
  
 new_user:
  free (filename);
  defer_initial_window = 1;
  mo_post_startup_dialog ();
  
  return;
}
