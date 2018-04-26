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
#include "libhtmlw/HTML.h"
#include "libwww/HTParse.h"

/* Defined in gui.c */
extern char *cached_url;

/* Image resolution function. */
static ImageInfo *Resolve (Widget w, char *src)
{
  int i, cnt;
  unsigned char *data;
  unsigned char *ptr;
  int width, height;
  int Used[256];
  XColor colrs[256];
  ImageInfo *img_data;
  char *txt;
  int foo;
  int len;
  char *fnam;
  int rc;
  extern int Vclass;

  if (src == NULL)
    return(NULL);
  
  /* Here should go hooks for built-in images. */

  /* OK, src is the URL we have to go hunt down.
     First, we go get it. */
  /* Call HTParse!  We can use cached_url here, since
     we set it in do_window_text. */
  src = HTParse(src, cached_url,
                PARSE_ACCESS | PARSE_HOST | PARSE_PATH |
                PARSE_PUNCTUATION);

  /* Go see if we already have the image info hanging around. */
  img_data = mo_fetch_cached_image_info (src);
  if (img_data)
    return (ImageInfo *)img_data;
  
  /* We have to load the image. */
  fnam = (char *)malloc (L_tmpnam + 16);
  tmpnam (fnam);
  
  rc = mo_pull_er_over_virgin (src, fnam);
  if (!rc)
    return NULL;

  data = ReadBitmap(fnam, &width, &height, colrs);

  /* Now delete the file. */
  {
    char *cmd = (char *)malloc ((strlen (fnam) + 32) * sizeof (char));
    sprintf (cmd, "/bin/rm -f %s &", fnam);
    system (cmd);
    free (cmd);
  }

  if (data == NULL)
    {
      return(NULL);
    }
  
  img_data = (ImageInfo *)malloc(sizeof(ImageInfo));
  img_data->width = width;
  img_data->height = height;
  for (i=0; i < 256; i++)
    {
      Used[i] = 0;
    }
  cnt = 1;
  ptr = data;
  for (i=0; i < (width * height); i++)
    {
      if (Used[(int)*ptr] == 0)
        {
          Used[(int)*ptr] = cnt;
          cnt++;
        }
      ptr++;
    }
  cnt--;

  /*
   * If the image has too many colors, apply a median cut algorithm to
   * reduce the color usage, and then reprocess it.
   * Don't cut colors for direct mapped visuals like TrueColor.
   */
  if ((cnt > Rdata.colors_per_inlined_image)&&(Vclass != TrueColor))
    {
      MedianCut(data, &width, &height, colrs, cnt, 
                Rdata.colors_per_inlined_image);

      for (i=0; i < 256; i++)
	{
	  Used[i] = 0;
	}
      cnt = 1;
      ptr = data;
      for (i=0; i < (width * height); i++)
	{
	  if (Used[(int)*ptr] == 0)
	    {
	      Used[(int)*ptr] = cnt;
	      cnt++;
	    }
	  ptr++;
	}
      cnt--;
    }

  img_data->num_colors = cnt;
  img_data->reds = (int *)malloc(sizeof(int) * cnt);
  img_data->greens = (int *)malloc(sizeof(int) * cnt);
  img_data->blues = (int *)malloc(sizeof(int) * cnt);
  
  for (i=0; i < 256; i++)
    {
      int indx;
      
      if (Used[i] != 0)
        {
          indx = Used[i] - 1;
          img_data->reds[indx] = colrs[i].red;
          img_data->greens[indx] = colrs[i].green;
          img_data->blues[indx] = colrs[i].blue;
        }
    }
  ptr = data;
  for (i=0; i < (width * height); i++)
    {
      *ptr = (unsigned char)(Used[(int)*ptr] - 1);
      ptr++;
    }
  img_data->image_data = data;
  img_data->image = NULL;

  mo_cache_image_info (src, (void *)img_data);
  
  return(img_data);
}

mo_status mo_register_image_resolution_function (mo_window *win)
{
  XmxSetArg (WbNresolveImageFunction, (long)Resolve);
  XmxSetValues (win->view);
}
