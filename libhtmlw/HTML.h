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

#ifndef HTML_H
#define HTML_H

#ifdef MOTIF
#include <Xm/Xm.h>
#else
#include <X11/Intrinsic.h>
#include <X11/Constraint.h>
#endif /* MOTIF */
#include <X11/StringDefs.h>



typedef int (*visitTestProc)();

typedef struct ele_ref_rec {
	int id, pos;
} ElementRef;

/*
 * Public functions
 */
#ifdef _NO_PROTO
extern char *HTMLGetText ();
extern char *HTMLGetTextAndSelection ();
extern char **HTMLGetHRefs ();
extern int HTMLPositionToId ();
extern int HTMLIdToPosition ();
extern int HTMLAnchorToPosition ();
extern void HTMLRetestAnchors ();
extern void HTMLClearSelection ();
extern void HTMLSetSelection ();
extern void HTMLSetText ();
extern int HTMLSearchText ();
#else
extern char *HTMLGetText (Widget w, int pretty);
extern char *HTMLGetTextAndSelection (Widget w, char **startp, char **endp,
					char **insertp);
extern char **HTMLGetHRefs (Widget w, int *num_hrefs);
extern int HTMLPositionToId(Widget w, int x, int y);
extern int HTMLIdToPosition(Widget w, int element_id, int *x, int *y);
extern int HTMLAnchorToPosition(Widget w, char *name, int *x, int *y);
extern void HTMLRetestAnchors(Widget w, visitTestProc testFunc);
extern void HTMLClearSelection (Widget w);
extern void HTMLSetSelection (Widget w, ElementRef *start, ElementRef *end);
extern void HTMLSetText (Widget w, char *text, char *header_text,
					char *footer_text);
extern int HTMLSearchText (Widget w, char *pattern,
	ElementRef *m_start, ElementRef *m_end, int backward, int caseless);
#endif /* _NO_PROTO */


/*
 * Public Structures
 */
typedef struct acall_rec {
	XEvent *event;
	int page;
	int element_id;
	char *text;
	char *href;
} WbAnchorCallbackData;


typedef struct image_rec {
	int width, height;
	int num_colors;
	int *reds;
	int *greens;
	int *blues;
	unsigned char *image_data;
	Pixmap image;
} ImageInfo;

typedef ImageInfo *(*resolveImageProc)();


/*
 * defines and structures used for the formatted element list
 */

#define E_TEXT		1
#define E_BULLET	2
#define E_LINEFEED	3
#define E_IMAGE		4

struct ele_rec {
	int type;
	ImageInfo *pic_data;
	XFontStruct *font;
	Boolean align_top;
	Boolean internal;
	Boolean selected;
	int indent_level;
	int start_pos, end_pos;
	int x, y;
	int y_offset;
	int line_number;
	int line_height;
	int ele_id;
	int underline_number;
	Boolean dashed_underline;
	unsigned long fg;
	unsigned long bg;
	char *anchorName;
	char *anchorHRef;
	char *edata;
	int edata_len;
	struct ele_rec *next;
	struct ele_rec *prev;
};

struct page_rec {
	int pnum;
	int pheight;
	int line_num;
	struct ele_rec *elist;
};

struct ref_rec {
	char *anchorHRef;
	struct ref_rec *next;
};


/*
 * defines and structures used for the HTML parser, and the
 * parsed object list.
 */

/* Mark types */
#define	M_UNKNOWN	-1
#define	M_NONE		0
#define	M_TITLE		1
#define	M_HEADER_1	2
#define	M_HEADER_2	3
#define	M_HEADER_3	4
#define	M_HEADER_4	5
#define	M_HEADER_5	6
#define	M_HEADER_6	7
#define	M_ANCHOR	8
#define	M_PARAGRAPH	9
#define	M_ADDRESS	10
#define	M_PLAIN_TEXT	11
#define	M_UNUM_LIST	12
#define	M_NUM_LIST	13
#define	M_LIST_ITEM	14
#define	M_DESC_LIST	15
#define	M_DESC_TITLE	16
#define	M_DESC_TEXT	17
#define	M_PREFORMAT	18
#define	M_PLAIN_FILE	19
#define M_LISTING_TEXT	20
#define M_INDEX		21
#define M_MENU		22
#define M_DIRECTORY	23
#define M_IMAGE		24
#define M_FIXED		25
#define M_BOLD		26
#define M_ITALIC	27
#define M_EMPHASIZED	28
#define M_STRONG	29
#define M_CODE		30
#define M_SAMPLE	31
#define M_KEYBOARD	32
#define M_VARIABLE	33

/* syntax of Mark types */
#define	MT_TITLE	"title"
#define	MT_HEADER_1	"h1"
#define	MT_HEADER_2	"h2"
#define	MT_HEADER_3	"h3"
#define	MT_HEADER_4	"h4"
#define	MT_HEADER_5	"h5"
#define	MT_HEADER_6	"h6"
#define	MT_ANCHOR	"a"
#define	MT_PARAGRAPH	"p"
#define	MT_ADDRESS	"address"
#define	MT_PLAIN_TEXT	"xmp"
#define	MT_UNUM_LIST	"ul"
#define	MT_NUM_LIST	"ol"
#define	MT_LIST_ITEM	"li"
#define	MT_DESC_LIST	"dl"
#define	MT_DESC_TITLE	"dt"
#define	MT_DESC_TEXT	"dd"
#define	MT_PREFORMAT	"pre"
#define	MT_PLAIN_FILE	"plaintext"
#define MT_LISTING_TEXT	"listing"
#define MT_INDEX	"isindex"
#define MT_MENU		"menu"
#define MT_DIRECTORY	"dir"
#define MT_IMAGE	"img"
#define MT_FIXED	"tt"
#define MT_BOLD		"b"
#define MT_ITALIC	"i"
#define MT_EMPHASIZED	"em"
#define MT_STRONG	"strong"
#define MT_CODE		"code"
#define MT_SAMPLE	"samp"
#define MT_KEYBOARD	"kbd"
#define MT_VARIABLE	"var"


/* anchor tags */
#define	AT_NAME		"name"
#define	AT_HREF		"href"


struct mark_up {
	int type;
	int is_end;
	char *start;
	char *text;
	char *end;
	struct mark_up *next;
};


/*
 * New resource names
 */
#define	WbNmarginWidth		"marginWidth"
#define	WbNmarginHeight		"marginHeight"
#define	WbNtext			"text"
#define	WbNheaderText		"headerText"
#define	WbNfooterText		"footerText"
#define	WbNtitleText		"titleText"
#define	WbNanchorUnderlines	"anchorUnderlines"
#define	WbNvisitedAnchorUnderlines	"visitedAnchorUnderlines"
#define	WbNdashedAnchorUnderlines	"dashedAnchorUnderlines"
#define	WbNdashedVisitedAnchorUnderlines	"dashedVisitedAnchorUnderlines"
#define	WbNanchorColor		"anchorColor"
#define	WbNvisitedAnchorColor	"visitedAnchorColor"
#define	WbNactiveAnchorFG	"activeAnchorFG"
#define	WbNactiveAnchorBG	"activeAnchorBG"
#define	WbNautoSize		"autoSize"
#define	WbNfancySelections	"fancySelections"
#define	WbNimageBorders		"imageBorders"
#define	WbNisIndex		"isIndex"
#define	WbNitalicFont		"italicFont"
#define	WbNboldFont		"boldFont"
#define	WbNfixedFont		"fixedFont"
#define	WbNheader1Font		"header1Font"
#define	WbNheader2Font		"header2Font"
#define	WbNheader3Font		"header3Font"
#define	WbNheader4Font		"header4Font"
#define	WbNheader5Font		"header5Font"
#define	WbNheader6Font		"header6Font"
#define	WbNaddressFont		"addressFont"
#define	WbNplainFont		"plainFont"
#define	WbNlistingFont		"listingFont"
#define	WbNanchorCallback	"anchorCallback"
#define	WbNdocumentPageCallback	"documentPageCallback"
#define	WbNpageHeight		"pageHeight"
#define	WbNcurrentPage		"currentPage"
#define	WbNpreviouslyVisitedTestFunction "previouslyVisitedTestFunction"
#define	WbNresolveImageFunction "resolveImageFunction"

/*
 * New resource classes
 */
#define	WbCMarginWidth		"MarginWidth"
#define	WbCMarginHeight		"MarginHeight"
#define	WbCText			"Text"
#define	WbCHeaderText		"HeaderText"
#define	WbCFooterText		"FooterText"
#define	WbCTitleText		"TitleText"
#define	WbCAnchorUnderlines	"AnchorUnderlines"
#define	WbCVisitedAnchorUnderlines	"VisitedAnchorUnderlines"
#define	WbCDashedAnchorUnderlines	"DashedAnchorUnderlines"
#define	WbCDashedVisitedAnchorUnderlines	"DashedVisitedAnchorUnderlines"
#define	WbCAnchorColor		"AnchorColor"
#define	WbCVisitedAnchorColor	"VisitedAnchorColor"
#define	WbCActiveAnchorFG	"ActiveAnchorFG"
#define	WbCActiveAnchorBG	"ActiveAnchorBG"
#define	WbCAutoSize		"AutoSize"
#define	WbCFancySelections	"FancySelections"
#define	WbCImageBorders		"ImageBorders"
#define	WbCIsIndex		"IsIndex"
#define	WbCItalicFont		"ItalicFont"
#define	WbCBoldFont		"BoldFont"
#define	WbCFixedFont		"FixedFont"
#define	WbCHeader1Font		"Header1Font"
#define	WbCHeader2Font		"Header2Font"
#define	WbCHeader3Font		"Header3Font"
#define	WbCHeader4Font		"Header4Font"
#define	WbCHeader5Font		"Header5Font"
#define	WbCHeader6Font		"Header6Font"
#define	WbCAddressFont		"AddressFont"
#define	WbCPlainFont		"PlainFont"
#define	WbCListingFont		"ListingFont"
#define	WbCPageHeight		"PageHeight"
#define	WbCCurrentPage		"CurrentPage"
#define	WbCPreviouslyVisitedTestFunction "PreviouslyVisitedTestFunction"
#define	WbCResolveImageFunction "ResolveImageFunction"



typedef struct _HTMLClassRec *HTMLWidgetClass;
typedef struct _HTMLRec      *HTMLWidget;

extern WidgetClass htmlWidgetClass;


#endif /* HTML_H */

