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

#include <stdio.h>
#include <ctype.h>
#include "HTMLP.h"
#include "NoImage.xbm"


ImageInfo no_image;

unsigned long allocated_pixels[256];
int allocation_count = 0;


/*
 * Free all the colors in the default colormap that we have allocated so far.
 */
void
FreeColors(dsp, colormap)
	Display *dsp;
	Colormap colormap;
{
	if (allocation_count > 0)
	{
		XFreeColors(dsp, colormap, allocated_pixels, allocation_count,
				0L);
	}
	allocation_count = 0;
}


/*
 * Free up all the pixmaps allocated for this document.
 */
void
FreeImages(hw)
	HTMLWidget hw;
{
	int page;
	struct ele_rec *eptr;

	for (page = 1; page <= hw->html.page_cnt; page++)
	{
		eptr = hw->html.pages[(page - 1)].elist;

		while (eptr != NULL)
		{
			if ((eptr->type == E_IMAGE)&&(eptr->pic_data != NULL))
			{
				/*
				 * Don't free the no_image default image
				 */
				if ((eptr->pic_data->image != (Pixmap)NULL)&&
				    (eptr->pic_data->image != no_image.image))
				{
					XFreePixmap(XtDisplay(hw),
						eptr->pic_data->image);
					eptr->pic_data->image = (Pixmap)NULL;
				}
			}
			eptr = eptr->next;
		}
	}
}


/*
 * Find the closest color by allocating it, or picking an already allocated
 * color
 */
void
FindColor(dsp, colormap, colr)
	Display *dsp;
	Colormap colormap;
	XColor *colr;
{
	int i, match;
	double rd, gd, bd, dist, mindist;
	int cindx;
	XColor def_colrs[256];
	int NumCells;

	match = XAllocColor(dsp, colormap, colr);
	if (match == 0)
	{
		NumCells = DisplayCells(dsp, DefaultScreen(dsp));
		for (i=0; i<NumCells; i++)
		{
			def_colrs[i].pixel = i;
		}
		XQueryColors(dsp, colormap, def_colrs, NumCells);
		mindist = 65536.0 * 65536.0;
		cindx = colr->pixel;
		for (i=0; i<NumCells; i++)
		{
			rd = (def_colrs[i].red - colr->red) / 256.0;
			gd = (def_colrs[i].green - colr->green) / 256.0;
			bd = (def_colrs[i].blue - colr->blue) / 256.0;
			dist = (rd * rd * rd * rd) +
				(gd * gd * gd * gd) +
				(bd * bd * bd * bd);
			if (dist < mindist)
			{
				mindist = dist;
				cindx = def_colrs[i].pixel;
			}
		}
		colr->pixel = cindx;
		colr->red = def_colrs[cindx].red;
		colr->green = def_colrs[cindx].green;
		colr->blue = def_colrs[cindx].blue;
	}
	else
	{
		allocated_pixels[allocation_count] = colr->pixel;
		allocation_count++;
		if (allocation_count > 255)
		{
			allocation_count = 255;
		}
	}
}


/*
 * Make am image of appropriate depth for display from image data.
 */
XImage *
MakeImage(dsp, data, width, height, depth, img_info)
	Display *dsp;
	unsigned char *data;
	int width, height;
	int depth;
	ImageInfo *img_info;
{
	int linepad, shiftnum;
	int shiftstart, shiftstop, shiftinc;
	int bytesperline;
	int temp;
	int w, h;
	XImage *newimage;
	unsigned char *bit_data, *bitp, *datap;

	if ((depth != 1)&&(depth != 2)&&(depth != 4)&&(depth != 8)&&
		(depth != 24))
	{
		fprintf(stderr, "Don't know how to format image for display of depth %d\n", depth);
		return(NULL);
	}
    if (depth != 24)
    {
	if (BitmapBitOrder(dsp) == LSBFirst)
	{
		shiftstart = 0;
		shiftstop = 8;
		shiftinc = depth;
	}
	else
	{
		shiftstart = 8 - depth;
		shiftstop = -depth;
		shiftinc = -depth;
	}
	linepad = 8 - (width % 8);
	bit_data = (unsigned char *)malloc(((width + linepad) * height) + 1);
	bitp = bit_data;
	datap = data;
	*bitp = 0;
	shiftnum = shiftstart;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = *datap++ << shiftnum;
			*bitp = *bitp | temp;
			shiftnum = shiftnum + shiftinc;
			if (shiftnum == shiftstop)
			{
				shiftnum = shiftstart;
				bitp++;
				*bitp = 0;
			}
		}
		for (w=0; w<linepad; w++)
		{
			shiftnum = shiftnum + shiftinc;
			if (shiftnum == shiftstop)
			{
				shiftnum = shiftstart;
				bitp++;
				*bitp = 0;
			}
		}
	}
	bytesperline = (width + linepad) * depth / 8;
	newimage = XCreateImage(dsp, DefaultVisual(dsp, DefaultScreen(dsp)),
		depth, ZPixmap, 0, (char *)bit_data,
		(width + linepad), height, 8, bytesperline);
    }
    else
    {
	bit_data = (unsigned char *)malloc(width * height * 4);

	if (BitmapBitOrder(dsp) == MSBFirst)
	{
		bitp = bit_data;
		datap = data;
		for (w = (width * height); w > 0; w--)
		{
			*bitp++ = (unsigned char)0;
			*bitp++ = (unsigned char)
				((img_info->blues[(int)*datap]>>8) & 0xff);
			*bitp++ = (unsigned char)
				((img_info->greens[(int)*datap]>>8) & 0xff);
			*bitp++ = (unsigned char)
				((img_info->reds[(int)*datap]>>8) & 0xff);
			datap++;
		}
	}
	else
	{
		bitp = bit_data;
		datap = data;
		for (w = (width * height); w > 0; w--)
		{
			*bitp++ = (unsigned char)
				((img_info->reds[(int)*datap]>>8) & 0xff);
			*bitp++ = (unsigned char)
				((img_info->greens[(int)*datap]>>8) & 0xff);
			*bitp++ = (unsigned char)
				((img_info->blues[(int)*datap]>>8) & 0xff);
			*bitp++ = (unsigned char)0;
			datap++;
		}
	}

	newimage = XCreateImage(dsp, DefaultVisual(dsp, DefaultScreen(dsp)),
		depth, ZPixmap, 0, (char *)bit_data,
		width, height, 32, 0);
    }

	return(newimage);
}




Pixmap
NoImage(hw)
	HTMLWidget hw;
{
	if (no_image.image == (Pixmap)NULL)
	{
		no_image.image = XCreatePixmapFromBitmapData(XtDisplay(hw),
			XtWindow(hw), NoImage_bits,
			NoImage_width, NoImage_height,
/*
 * Without motif we use our own foreground resource instead of
 * using the manager's
 */
#ifdef MOTIF
                        hw->manager.foreground,
#else
                        hw->html.foreground,
#endif /* MOTIF */
			hw->core.background_pixel,
			DefaultDepthOfScreen(XtScreen(hw)));
	}
	return(no_image.image);
}


ImageInfo *
NoImageData(hw)
	HTMLWidget hw;
{
	no_image.width = NoImage_width;
	no_image.height = NoImage_height;
	no_image.num_colors = 0;
	no_image.reds = NULL;
	no_image.greens = NULL;
	no_image.blues = NULL;
	no_image.image_data = NULL;
	no_image.image = (Pixmap)NULL;

	return(&no_image);
}


Pixmap
InfoToImage(hw, img_info)
	HTMLWidget hw;
	ImageInfo *img_info;
{
	int i;
	Pixmap Img;
	XImage *tmpimage;
	XColor tmpcolr;
	int *Mapping;
	unsigned char *tmpdata;
	unsigned char *ptr;
	unsigned char *ptr2;
	int Vclass;
	XVisualInfo vinfo, *vptr;

	/* find the visual class. */
	vinfo.visualid = XVisualIDFromVisual(DefaultVisual(XtDisplay(hw),
		DefaultScreen(XtDisplay(hw))));
	vptr = XGetVisualInfo(XtDisplay(hw), VisualIDMask, &vinfo, &i);
	Vclass = vptr->class;
	XFree((char *)vptr);

	Mapping = (int *)malloc(img_info->num_colors * sizeof(int));

	for (i=0; i < img_info->num_colors; i++)
	{
		tmpcolr.red = img_info->reds[i];
		tmpcolr.green = img_info->greens[i];
		tmpcolr.blue = img_info->blues[i];
		tmpcolr.flags = DoRed|DoGreen|DoBlue;
		if (Vclass == TrueColor)
		{
			Mapping[i] = i;
		}
		else
		{
			FindColor(XtDisplay(hw),
				DefaultColormapOfScreen(XtScreen(hw)),
				&tmpcolr);
			Mapping[i] = tmpcolr.pixel;
		}
	}

	tmpdata = (unsigned char *)malloc(img_info->width * img_info->height);
	if (tmpdata == NULL)
	{
		tmpimage = NULL;
		Img = (Pixmap)NULL;
	}
	else
	{
		ptr = img_info->image_data;
		ptr2 = tmpdata;
		for (i=0; i < (img_info->width * img_info->height); i++)
		{
			*ptr2 = (unsigned char)Mapping[(int)*ptr];
			ptr++;
			ptr2++;
		}

		tmpimage = MakeImage(XtDisplay(hw), tmpdata,
			img_info->width, img_info->height,
			DefaultDepthOfScreen(XtScreen(hw)), img_info);

		Img = XCreatePixmap(XtDisplay(hw), XtWindow(hw),
			img_info->width, img_info->height,
			DefaultDepthOfScreen(XtScreen(hw)));
	}

	if ((tmpimage == NULL)||(Img == (Pixmap)NULL))
	{
		if (tmpimage != NULL)
		{
			XDestroyImage(tmpimage);
		}
		if (Img != (Pixmap)NULL)
		{
			XFreePixmap(XtDisplay(hw), Img);
		}
		img_info->width = NoImage_width;
		img_info->height = NoImage_height;
		Img = NoImage(hw);
	}
	else
	{
		XPutImage(XtDisplay(hw), Img, hw->html.drawGC, tmpimage, 0, 0,
			0, 0, img_info->width, img_info->height);
		XDestroyImage(tmpimage);
	}
	return(Img);
}

