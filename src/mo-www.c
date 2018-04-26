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
#include <ctype.h>

/* Grumble grumble... */
#ifdef __sgi
#define __STDC__
#endif

#include "libwww/HTUtils.h"
#include "libwww/HTString.h"
#include "libwww/tcp.h"
#include "libwww/WWW.h"
#include "libwww/HTTCP.h"
#include "libwww/HTAnchor.h"
#include "libwww/HTParse.h"
#include "libwww/HTAccess.h"
#include "libwww/HTHistory.h"
#include "libwww/HTML.h"
#include "libwww/HText.h"
#include "libwww/HTList.h"
#include "libwww/HTStyle.h"

#define MO_BUFFER_SIZE 8192

/* Bare minimum. */
struct _HText {
  HTParentAnchor *node_anchor;
  char *expandedAddress;
  char *simpleAddress;

  /* This is what we should parse and display; it is *not*
     safe to free. */
  char *htmlSrc;
  /* This is what we should free. */
  char *htmlSrcHead;
  int srcalloc;    /* amount of space allocated */
  int srclen;      /* amount of space used */
};

HText* HTMainText = 0;                  /* Equivalent of main window */
HTParentAnchor* HTMainAnchor = 0;       /* Anchor for HTMainText */
HTStyleSheet *styleSheet;               /* Default or overridden */

int bypass_format_handling = 0;         /* hook to switch off www-lib value-add */
int force_dump_to_file = 0;             /* hook to force dumping binary data
                                           straight to file named by... */
char *force_dump_filename = 0;          /* this filename. */

extern Widget toplevel;

static char *hack_htmlsrc (void)
{
  /* Keep pointer to real head of htmlSrc memory block. */
  HTMainText->htmlSrcHead = HTMainText->htmlSrc;
  
  /* OK, bigtime hack -- again -- sigh, I really gotta stop doing this.
     First, we make sure that <plaintext> isn't repeated, or improperly
     applied to the result of a WAIS query (which has an immediate <title>
     tag). */
  if (HTMainText->htmlSrc && HTMainText->srclen > 30)
    {
      if (!strncmp (HTMainText->htmlSrc, "<plaintext>", 11) ||
          !strncmp (HTMainText->htmlSrc, "<PLAINTEXT>", 11))
        {
          if (!strncmp (HTMainText->htmlSrc + 11, "<plaintext>", 11) ||
              !strncmp (HTMainText->htmlSrc + 11, "<PLAINTEXT>", 11))
            {
              HTMainText->htmlSrc += 11;
            }
          else if (!strncmp (HTMainText->htmlSrc + 11, "\n<plaintext>", 12) ||
                   !strncmp (HTMainText->htmlSrc + 11, "\n<PLAINTEXT>", 12))
            {
              HTMainText->htmlSrc += 12;
            }
          else if (!strncmp (HTMainText->htmlSrc + 11, "\n<title>", 8) ||
                   !strncmp (HTMainText->htmlSrc + 11, "\n<TITLE>", 8))
            {
              HTMainText->htmlSrc += 12;
            }
          /* Catch CERN hytelnet gateway -- HTTP2, WHERE ARE YOU???? */
          else if (!strncmp (HTMainText->htmlSrc + 11, "\n<HEAD>", 7))
            {
              HTMainText->htmlSrc += 12;
            }
        }
    }
  return HTMainText->htmlSrc;
}

/* Given a url, a flag to indicate if this is the first url
   to be loaded in this context, and a pointer to string that
   should be set to represent the head of the allocated text,
   this returns the head of the displayable text. */
static char *doit (char *url, int first, char **texthead)
{
  char *msg;

  /* Eric -- here's where you should probably hook in. */
#ifdef HAVE_DMF
  {
    char *tag;
    
    tag = HTParse(url, "", PARSE_ACCESS);
    if ((tag != NULL)&&(strcmp(tag, "dmfdb") == 0))
      {
        char *addr;
        char *path;
        char *text;
        
        addr = HTParse(url, "", PARSE_HOST);
        path = HTParse(url, "", PARSE_PATH);
        
        text = DMFAnchor(tag, addr, path);
        if (text != NULL)
          {
            *texthead = text;
            return(text);
          }
      }
    else if ((tag != NULL)&&(strcmp(tag, "dmffield") == 0))
      {
        char *addr;
        char *path;
        char *text;
        
        addr = HTParse(url, "", PARSE_HOST);
        path = HTParse(url, "", PARSE_PATH);
        
        text = DMFField(tag, addr, path);
        if (text != NULL)
          {
            *texthead = text;
            return(text);
          }
      }
    else if ((tag != NULL)&&(strcmp(tag, "dmfquery") == 0))
      {
        char *addr;
        char *path;
        char *text;
        
        addr = HTParse(url, "", PARSE_HOST);
        path = HTParse(url, "", PARSE_PATH);
        
        text = DMFQuery(tag, addr, path);
        if (text != NULL)
          {
            *texthead = text;
            return(text);
          }
      }
    else if ((tag != NULL)&&(strcmp(tag, "dmfinquire") == 0))
      {
        char *addr;
        char *path;
        char *text;
        
        addr = HTParse(url, "", PARSE_HOST);
        path = HTParse(url, "", PARSE_PATH);
        
        text = DMFInquire(tag, addr, path);
        if (text != NULL)
          {
            *texthead = text;
            return(text);
          }
      }
    else if ((tag != NULL)&&(strcmp(tag, "dmfitem") == 0))
      {
        char *addr;
        char *path;
        char *text;
        
        addr = HTParse(url, "", PARSE_HOST);
        path = HTParse(url, "", PARSE_PATH);
        
        text = DMFItem(tag, addr, path);
        if (text != NULL)
          {
            *texthead = text;
            return(text);
          }
        else
          {
            *texthead = NULL;
            return(NULL);
          }
      }
  }
#endif /* HAVE_DMF */

  if (first)
    {
      if (HTLoadAbsolute (url, 0))
        {
          char *txt = hack_htmlsrc ();
          *texthead = HTMainText->htmlSrcHead;
          return txt;
        }
    }
  else
    {
      if (HTLoadRelative (url))
        {
          char *txt = hack_htmlsrc ();
          *texthead = HTMainText->htmlSrcHead;
          return txt;
        }
    }

  msg = (char *)malloc ((strlen (url) + 200) * sizeof (char));
  sprintf (msg, "<H1>ERROR</H1> Requested document (URL %s) could not be accessed.<p>The information server either is not accessible or is refusing to serve the document to you.<p>", url);
  *texthead = msg;
  return msg;
}

char *mo_pull_er_over (char *url, char **texthead)
{
  return doit (url, 0, texthead);
}

char *mo_pull_er_over_first (char *url, char **texthead)
{
  return doit (url, 1, texthead);
}

/* Don't let the WWW library play games with formats. */
/* May want to strip off leading <plaintext>'s here also. */
mo_status mo_pull_er_over_virgin (char *url, char *fnam)
{
  /* This was a mistake, but it's still in here at the moment. */
  bypass_format_handling = 1;
  
  /* Force dump to file. */
  force_dump_to_file = 1;
  force_dump_filename = fnam;

  if (HTLoadRelative (url))
    {
      bypass_format_handling = 0;
      force_dump_to_file = 0;
      return mo_succeed;
    }
  else
    {
      bypass_format_handling = 0;
      force_dump_to_file = 0;
      return mo_fail;
    }  
}

HText *HText_new (HTParentAnchor *anchor)
{
  HText *htObj = (HText *)malloc (sizeof (HText));

  htObj->expandedAddress = NULL;
  htObj->simpleAddress = NULL;
  htObj->htmlSrc = NULL;
  htObj->htmlSrcHead = NULL;
  htObj->srcalloc = 0;
  htObj->srclen = 0;

  htObj->node_anchor = anchor;
  
  HTMainText = htObj;
  HTMainAnchor = anchor;

  return htObj;
}

void HText_free (HText *self)
{
  if (self->htmlSrcHead)
    free (self->htmlSrcHead);
  free (self);
  return;
}

void HText_beginAppend (HText *text)
{
  HTMainText = text;
  return;
}

void HText_endAppend (HText *text)
{
  HText_appendCharacter (text, '\0');
  HTMainText = text;
  return;
}

void HText_setStyle (HText *text, HTStyle *style)
{
  return;
}

static void new_chunk (HText *text)
{
  if (text->srcalloc == 0)
    {
      text->htmlSrc = (char *)malloc (MO_BUFFER_SIZE);
      text->htmlSrc[0] = '\0';
    }
  else
    {
      text->htmlSrc = (char *)realloc
        (text->htmlSrc, text->srcalloc + MO_BUFFER_SIZE);
    }

  text->srcalloc += MO_BUFFER_SIZE;

  return;
}

void HText_appendCharacter (HText *text, char ch)
{
  if (text->srcalloc < text->srclen + 1 + 16)
    new_chunk (text);
  
  text->htmlSrc[text->srclen++] = ch;

  return;
}

void HText_appendText (HText *text, const char *str)
{
  int len;

  if (!str)
    return;

  len = strlen (str);

  if (text->srcalloc < text->srclen + len + 16)
    new_chunk (text);

  bcopy (str, (text->htmlSrc + text->srclen), len);

  text->srclen += len;
  text->htmlSrc[text->srclen] = '\0';

  return;
}

void HText_appendParagraph (HText *text)
{
  /* Boy, talk about a misnamed function. */
  char *str = " <p> \n";

  HText_appendText (text, str);

  return;
}

void HText_beginAnchor (HText *text, HTChildAnchor *anc)
{
  return;
}

void HText_endAnchor (HText * text)
{
  return;
}

void HText_dump (HText *me)
{
  return;
}

HTParentAnchor *HText_nodeAnchor (HText *me)
{
  return me->node_anchor;
}

char *HText_getText (HText *me)
{
  return me->htmlSrc;
}

BOOL HText_select (HText *text)
{
  return;
}

BOOL HText_selectAnchor (HText *text, HTChildAnchor *anchor)
{
  return;
}

/* Grumble grumble... */
#if defined(ultrix) || defined(VMS) || defined(NeXT) || defined(M4310) || defined(vax)
char *strdup (char *str)
{
  char *dup;

  dup = (char *)malloc (strlen (str) + 1);
  dup = strcpy (dup, str);

  return dup;
}
#endif

/* Feedback from the library. */
void application_user_feedback (char *str)
{
  XmxMakeInfoDialog (toplevel, str, "NCSA Mosaic: Application Feedback");
  XmxManageRemanage (Xmx_w);
}
