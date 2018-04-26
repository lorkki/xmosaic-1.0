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

#ifdef GRPAN

/* -------------------------- Group Annotations --------------------------- */

/* If Rdata.annotation_server is non-NULL, then every time we
   retrieve a document from any remote system we ask the annotation
   server (aserver for short) about any annotations corresonding to
   that document.

   The annotation server either returns NULL (none exist) or a block
   of HTML-format text suitable for appending at the end of a document
   that contain anchors that link to the annotations themselves.

   The external interface to this module is as follows:

   char *mo_fetch_grpan_links (char *url)
     Return an HTML-format hyperlink table to be appended to
     the document text, or NULL if no annotations exist.
     If a non-NULL result is returned, the caller must free it.
     
   mo_status mo_new_grpan (char *url, char *title, char *author,
                           char *text)
     Add a new annotation for document named by 'url' to the
     list of annotations for that document on the annotation server.
     Return mo_succeed if successful, else mo_fail.

   All functions herein behave properly if Rdata.annotation_server
   is NULL; therefore, they can be freely called as desired.

   */

char *mo_fetch_grpan_links (char *url)
{
  if (!Rdata.annotation_server)
    return NULL;
  else
    return NULL;
}

mo_status mo_new_grpan (char *url, char *title, char *author,
                        char *text)
{
  if (!Rdata.annotation_server)
    return mo_fail;
  else
    return mo_fail;
}

#endif /* GRPAN */
