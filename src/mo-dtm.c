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

#ifdef HAVE_DTM
/* Interface between Mosaic and DTM lives here.
   Right now all we want to do is sent text to a Collage session... */

#include "net.h"

NetPort *NetInternalCreateOutPort (char *, int);

mo_status mo_dtm_send_text (mo_window *win, char *port, char *url, char *text)
{
  Text *t = (Text *)malloc (sizeof (Text));
  char *title = (char *)malloc (strlen (url) + 16);
  NetPort *inport, *outport;
  int rv;

  sprintf (title, "Mosaic: %s\0", url);
  t->title = title;
  t->id = strdup ("mosaic");
  t->selLeft = t->selRight = t->insertPt = 0;
  t->numReplace = t->dim = strlen (text);
  t->replaceAll = TRUE;
  t->textString = strdup (text);

  NetInit ();
  inport = NetCreateInPort (":0");
  outport = NetInternalCreateOutPort (port, FALSE);

  rv = NetSendText (outport, t, FALSE, "NewText");

  return mo_succeed;
}

#endif /* HAVE_DTM */
