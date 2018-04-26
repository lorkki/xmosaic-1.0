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

#ifndef HTMLP_H
#define HTMLP_H

#ifdef MOTIF
#include <Xm/XmP.h>
# ifdef MOTIF1_2
#  include <Xm/ManagerP.h>
# endif /* MOTIF1_2 */
#else
#include <X11/IntrinsicP.h>
#include <X11/ConstrainP.h>
#endif /* MOTIF */

#include "HTML.h"
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>


/*  New fields for the HTML widget class */
typedef struct _HTMLClassPart
{
	int none;	/* no extra HTML class stuff */
} HTMLClassPart;


typedef struct _HTMLClassRec
{
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
#ifdef MOTIF
	XmManagerClassPart	manager_class;
#endif /* MOTIF */
	HTMLClassPart		html_class;
} HTMLClassRec;


extern HTMLClassRec htmlClassRec;


/* New fields for the HTML widget */
typedef struct _HTMLPart
{
	/* Resources */
	Dimension		margin_width;
	Dimension		margin_height;
	Dimension		page_height;

	XtCallbackList		anchor_callback;
	XtCallbackList		document_page_callback;

	char			*title;
	char			*raw_text;
	char			*header_text;
	char			*footer_text;
/*
 * Without motif we have to define our own forground resource
 * instead of using the manager's
 */
#ifndef MOTIF
	Pixel			foreground;
#endif
	Pixel			anchor_fg;
	Pixel			visitedAnchor_fg;
	Pixel			activeAnchor_fg;
	Pixel			activeAnchor_bg;
	int			num_anchor_underlines;
	int			num_visitedAnchor_underlines;
	Boolean			dashed_anchor_lines;
	Boolean			dashed_visitedAnchor_lines;
	Boolean			fancy_selections;
	Boolean			border_images;
	Boolean			auto_size;
	Boolean			is_index;
	int			current_page;

	XFontStruct		*font;
	XFontStruct		*italic_font;
	XFontStruct		*bold_font;
	XFontStruct		*fixed_font;
	XFontStruct		*header1_font;
	XFontStruct		*header2_font;
	XFontStruct		*header3_font;
	XFontStruct		*header4_font;
	XFontStruct		*header5_font;
	XFontStruct		*header6_font;
	XFontStruct		*address_font;
	XFontStruct		*plain_font;
	XFontStruct		*listing_font;

        XtPointer		previously_visited_test;
        XtPointer		resolveImage;

	/* PRIVATE */
	Dimension		max_pre_width;
	Dimension		format_width;
	Dimension		format_height;
	Boolean			reformat;
	struct ele_rec		*formatted_elements;
	int			line_count;
	struct ele_rec		**line_array;
	struct ele_rec		*select_start;
	struct ele_rec		*select_end;
	int			sel_start_pos;
	int			sel_end_pos;
	struct ele_rec		*new_start;
	struct ele_rec		*new_end;
	int			new_start_pos;
	int			new_end_pos;
	struct ele_rec		*active_anchor;
	GC			drawGC;
	int			press_x;
	int			press_y;
	Time			but_press_time;
	Time			selection_time;
	struct mark_up		*html_objects;
	struct mark_up		*html_header_objects;
	struct mark_up		*html_footer_objects;
	struct page_rec		*pages;
	int			page_cnt;
	struct ref_rec		*my_visited_hrefs;
} HTMLPart;


typedef struct _HTMLRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
#ifdef MOTIF
	XmManagerPart		manager;
#endif /* MOTIF */
	HTMLPart		html;
} HTMLRec;

#endif /* HTMLP_H */
