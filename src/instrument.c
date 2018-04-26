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
#include <time.h>

/* Initially we just keep a linked list, in order of time,
   of all documents accessed. */

typedef struct entry
{
  int action;
  char *url;
  time_t entry_time;
  struct entry *next;
} entry;

static entry *list = NULL;
static entry *list_end = NULL;

mo_status mo_inst_register_action (int action, char *url)
{
  entry *l;
  time_t t = time (NULL);

  l = (entry *)malloc (sizeof (entry));
  l->action = action;
  l->url = url;
  l->entry_time = t;
  l->next = NULL;

  if (!list)
    {
      list = l;
      list_end = l;
    }
  else
    {
      list_end->next = l;
      list_end = l;
    }

  return mo_succeed;
}

mo_status mo_inst_dump_and_flush_data (void)
{
  entry *l, *foo;
  char *fnam, *cmd;
  FILE *fp;
  char *actions[10];
  
  actions[0] = "view"; /* done */
  actions[1] = "annotate"; /* done */
  actions[2] = "edit_annotation"; /* done */
  actions[3] = "delete_annotation"; /* done */
  actions[4] = "whine"; /* done */
  actions[5] = "print"; /* done */
  actions[6] = "mail"; /* done */
  actions[7] = "save"; /* done */
  actions[8] = "dtm_broadcast"; /* done */
  actions[9] = "audio_annotate"; /* done */

  fnam = (char *)malloc (L_tmpnam + 16);
  tmpnam (fnam);
  
  fp = fopen (fnam, "w");
  if (!fp)
    goto oops;
  fprintf (fp, "Mosaic Instrumentation Statistics Format v1.0\n");
  fprintf (fp, "Mosaic for X version %s on %s.\n", MO_VERSION_STRING,
           MO_MACHINE_TYPE);
  l = list;
  while (l != NULL)
    {
      fprintf (fp, "%s %s %s\0", actions[l->action], l->url, ctime (&(l->entry_time)));
      foo = l;
      l = l->next;
      free (foo);
    }
  fclose (fp);

  cmd = (char *)malloc ((64 + strlen (Rdata.mail_command) + 
                         strlen (MO_INSTRUMENTATION_ADDRESS) +
                         strlen (fnam) + 24) * sizeof (char));
  sprintf (cmd, "%s -s \"NCSA Mosaic Instrumentation: v%s on %s.\" %s < %s", 
           Rdata.mail_command,
           MO_VERSION_STRING, MO_MACHINE_TYPE, MO_INSTRUMENTATION_ADDRESS, 
           fnam);
  if ((system (cmd)) != 0)
    {
      fprintf (stderr, "Note: While closing itself down, Mosaic was unable to\n");
      fprintf (stderr, "      mail its instrumentation log to NCSA.  Please\n");
      fprintf (stderr, "      check the value of the X resource mailCommand\n");
      fprintf (stderr, "      and make sure it references a valid mail program.\n");
      fprintf (stderr, "      If you need help, please contact us at\n");
      fprintf (stderr, "      'mosaic-x@ncsa.uiuc.edu'.  Thank you.\n");
    }
  free (cmd);
  
  cmd = (char *)malloc ((strlen (fnam) + 32) * sizeof (char));
  sprintf (cmd, "/bin/rm -f %s &", fnam);
  system (cmd);
  free (fnam);
  free (cmd);
  
 oops:
  return mo_succeed;
}
