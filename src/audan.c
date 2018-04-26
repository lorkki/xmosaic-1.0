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

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <pwd.h>

#ifdef HAVE_AUDIO_ANNOTATIONS

/* Check and make sure that the recording command we named
   in the resource is actually there and executable. */
mo_status mo_audio_capable (void)
{
  char *filename = Rdata.record_command_location;
  struct stat buf;
  int r;

  r = stat (filename, &buf); 
  if (r != -1 && buf.st_mode & S_IXOTH)
    return mo_succeed;
  else
    return mo_fail;
}

static XmxCallback (start_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));

  /* Take ourselves out of circulation. */
  XmxSetSensitive (win->audio_start_button, XmxNotSensitive);

  /* Come up with a new tmpnam. */
  win->record_fnam = (char *)malloc (L_tmpnam + 16);
  tmpnam (win->record_fnam);

  /* Fork off the recording process. */
  if ((win->record_pid = fork ()) < 0)
    {
      /* Uh oh, no process. */
      XmxMakeErrorDialog (win->audio_annotate_win, 
                          "Unable to start audio recording process.", 
                          "Audio Annotation Error");
      XtManageChild (Xmx_w);
    }
  else if (win->record_pid == 0)
    {
      /* We're in the child. */
      execl (Rdata.record_command_location, Rdata.record_command,
             win->record_fnam, (char *)0);
    }

  /* Let the user stop the record process. */
  XmxSetSensitive (win->audio_stop_button, XmxSensitive);
}

static XmxCallback (stop_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));

  /* Take ourselves out of circulation. */
  XmxSetSensitive (win->audio_stop_button, XmxNotSensitive);

  /* Stop the record process.  This works for both SGI recordaiff
     and Sun record, apparently. */
  kill (win->record_pid, SIGINT);
  
  /* Wait until the process is dead. */
  waitpid (win->record_pid, NULL, 0);

  /* No more process. */
  win->record_pid = 0;

  /* Let the user make another recording. */
  XmxSetSensitive (win->audio_start_button, XmxSensitive);
}

static XmxCallback (buttons_cb)
{
  mo_window *win = mo_fetch_window_by_id (XmxExtractUniqid ((int)client_data));

  switch (XmxExtractToken ((int)client_data))
    {
    case 0:
      if (win->record_pid)
        {
          /* Stop the record process. */
          kill (win->record_pid, SIGINT);
          
          /* Wait until the process is dead. */
          waitpid (win->record_pid, NULL, 0);
        }
      if (win->record_fnam)
        {
          /* Do the right thing. */
          int pan_id = mo_next_pan_id ();
          char *default_directory = Rdata.private_annotation_directory;
          char filename[500], *cmd, namestr[200], textstr[500], titlestr[200],
            *author;
          struct passwd *pw = getpwuid (getuid ());
          extern char *machine;
          
          if (Rdata.default_author_name)
            author = Rdata.default_author_name;
          else
            author = pw->pw_gecos;

#ifdef __sgi
          sprintf (filename, "%s/%s/%s%d.aiff\0", getenv ("HOME"), 
                   default_directory,
                   "PAN-", pan_id);
#else /* sun or hp */
          sprintf (filename, "%s/%s/%s%d.au\0", getenv ("HOME"), 
                   default_directory,
                   "PAN-", pan_id);
#endif

          cmd = (char *)malloc 
            ((strlen (win->record_fnam) + strlen (filename) + 32)
             * sizeof (char));
          sprintf (cmd, "/bin/mv %s %s", win->record_fnam, filename);
          if ((system (cmd)) != 0)
            {
              XmxMakeErrorDialog 
                (win->base, "Unable to complete audio annotation.", 
                 "Audio Annotation Error");
              XtManageChild (Xmx_w);
            }
          free (cmd);

          sprintf (titlestr, "Audio Annotation by %s\0", author);
          sprintf (namestr, "%s (%s@%s)\0", author, pw->pw_name, machine);
          sprintf (textstr, 
                   "This is an audio annotation. <P>\n\nTo hear the annotation, go <A HREF=\"file:%s\">here</A>. <P>\n", filename);

          mo_new_pan (win->current_node->url, titlestr, namestr,
                      textstr);

          /* Inefficient, but safe. */
          mo_write_pan_list ();

          if (can_instrument && win->instrument_usage)
            mo_inst_register_action 
              (mo_inst_audio_annotate, win->current_node->url);

          /* Snarf current scrollbar values to make sure we
             jump back to current place in do_window_text. */
          mo_snarf_scrollbar_values (win);
          mo_do_window_node (win, win->current_node);
        }
        
      win->record_pid = 0;
      win->record_fnam = 0;

      XtUnmanageChild (win->audio_annotate_win);
      break;
    case 1:
      if (win->record_pid)
        {
          /* Stop the record process. */
          kill (win->record_pid, SIGINT);
          
          /* Wait until the process is dead. */
          waitpid (win->record_pid, NULL, 0);
          
          win->record_pid = 0;
          win->record_fnam = 0;
        }
      XtUnmanageChild (win->audio_annotate_win);
      break;
    case 2:
      mo_open_another_window
        (win,
         "http://hoohoo.ncsa.uiuc.edu:80/mosaic-docs/help-on-audio-annotate.html",
         NULL, NULL);
      break;
    }
  
  return;
}


/* Post a dialog window with the following options:

   Start recording.
   Stop recording.

   Commit.
   Dismiss.
   Help.
*/
mo_status mo_post_audio_annotate_win (mo_window *win)
{
  Widget dialog_frame;
  Widget dialog_sep, buttons_form;
  Widget audio_annotate_form, yap_label;

  if (!win->audio_annotate_win)
    {
      /* Create it for the first time. */
      XmxSetUniqid (win->id);
      XmxSetArg (XmNresizePolicy, XmRESIZE_GROW);
      win->audio_annotate_win = XmxMakeFormDialog 
        (win->base, "NCSA Mosaic: Audio Annotate Window");
      dialog_frame = XmxMakeFrame (win->audio_annotate_win, XmxShadowOut);
      
      /* Constraints for base. */
      XmxSetConstraints 
        (dialog_frame, XmATTACH_FORM, XmATTACH_FORM, 
         XmATTACH_FORM, XmATTACH_FORM, NULL, NULL, NULL, NULL);
      
      /* Main form. */
      XmxSetArg (XmNfractionBase, 2);
      audio_annotate_form = XmxMakeForm (dialog_frame);
      
      yap_label = XmxMakeLabel 
        (audio_annotate_form,
         "Press Start to start recording; Stop to stop recording.\n\
Repeat until you're satisfied with the annotation.\n\
Then either Commit or Dismiss the annotation.");
      
      win->audio_start_button = XmxMakePushButton (audio_annotate_form,
                                                   "Start Recording",
                                                   start_cb,
                                                   0);
      win->audio_stop_button = XmxMakePushButton (audio_annotate_form,
                                                  "Stop Recording",
                                                  stop_cb,
                                                  0);
      
      dialog_sep = XmxMakeHorizontalSeparator (audio_annotate_form);
      
      buttons_form = XmxMakeFormAndThreeButtons
        (audio_annotate_form, buttons_cb, "Commit", "Dismiss", "Help...", 
         0, 1, 2);
      
      /* Constraints for audio_annotate_form. */
      XmxSetOffsets (yap_label, 10, 10, 10, 10);
      XmxSetConstraints
        (yap_label, XmATTACH_FORM, XmATTACH_NONE, XmATTACH_FORM, XmATTACH_FORM,
         NULL, NULL, NULL, NULL);
      XmxSetOffsets (win->audio_start_button, 10, 10, 10, 10);
      XmxSetPositions (win->audio_start_button, 
                       XmxNoPosition, XmxNoPosition, 0, 1);
      XmxSetConstraints 
        (win->audio_start_button, XmATTACH_WIDGET, XmATTACH_NONE,
         XmATTACH_POSITION, XmATTACH_POSITION,
         yap_label, NULL, NULL, NULL);
      XmxSetOffsets (win->audio_stop_button, 10, 10, 10, 10);
      XmxSetPositions (win->audio_stop_button, 
                       XmxNoPosition, XmxNoPosition, 1, 2);
      XmxSetConstraints 
        (win->audio_stop_button, XmATTACH_WIDGET, XmATTACH_NONE,
         XmATTACH_POSITION, XmATTACH_POSITION,
         yap_label, NULL, NULL, NULL);
      
      XmxSetArg (XmNtopOffset, 10);
      XmxSetConstraints 
        (dialog_sep, XmATTACH_WIDGET, XmATTACH_NONE, XmATTACH_FORM, 
         XmATTACH_FORM,
         win->audio_start_button, NULL, NULL, NULL);
      XmxSetConstraints 
        (buttons_form, XmATTACH_WIDGET, XmATTACH_NONE, XmATTACH_FORM, 
         XmATTACH_FORM,
         dialog_sep, NULL, NULL, NULL);
    }

  XtManageChild (win->audio_annotate_win);
  XmxSetSensitive (win->audio_stop_button, XmxNotSensitive);
  XmxSetSensitive (win->audio_start_button, XmxSensitive);
  
  return;
}


#endif
