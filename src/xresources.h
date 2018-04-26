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

/* This document should be included in whatever source document
   sets up the Intrinsics.  It is in a separate file so it doesn't
   clutter up that file.  So sue me. */

#ifndef __MOSAIC_XRESOURCES_H__

/* ----------------------------- X Resources ------------------------------ */
#define offset(x) XtOffset (AppDataPtr, x)

static XtResource resources[] = {
  /* Whether a new window should be created for each document, by default. */
  { "windowPerDocument", "WindowPerDocument", XtRBoolean, sizeof (Boolean), 
      offset (window_per_document), XtRString, "False" },
  /* Whether there is a per-window setting for the window-per-document
     feature or not (by default there isn't). */
  { "globalWindowPerDocument", "GlobalWindowPerDocument", XtRBoolean, 
      sizeof (Boolean), 
      offset (global_window_per_document), XtRString, "True" },
  /* Whether the pointer will be warped to the keyword entry area when
     accessing a searchable document.  Since Mosaic is point-to-focus,
     this automatically gives input focus for searchable documents. */
  { "warpPointerForIndex", "WarpPointerForIndex", XtRBoolean, sizeof (Boolean),
      offset (warp_pointer_for_index), XtRString, "False" },
  /* Whether Mosaic reads and writes global history from 
     ~/.mosaic-global-history
     and thus provides persistent history tracking. */
  { "useGlobalHistory", "UseGlobalHistory", XtRBoolean, sizeof (Boolean),
      offset (use_global_history), XtRString, "True" },
  /* Whether the user interface goes to great lengths not to change its
     configuration on a geometry change. */
  { "staticInterface", "StaticInterface", XtRBoolean, sizeof (Boolean),
      offset (static_interface), XtRString, "False" },
  /* Whether titles will be displayed wherever URL\'s are normally
     displayed. */
  { "displayURLsNotTitles", "DisplayURLsNotTitles", XtRBoolean, 
      sizeof (Boolean),
      offset (display_urls_not_titles), XtRString, "False" },
  /* Default width for a Document View window.  This will change as windows
     are cloned. */
  { "defaultWidth", "DefaultWidth", XtRInt, sizeof (int),
      offset (default_width), XtRString, "620" },
  /* Default height for a Document View window. */
  { "defaultHeight", "DefaultHeight", XtRInt, sizeof (int),
      offset (default_height), XtRString, "680" },
  /* Startup document. */
  { "homeDocument", "HomeDocument", XtRString, sizeof (char *),
      offset (home_document), XtRString, 
      "http://hoohoo.ncsa.uiuc.edu:80/mosaic-docs/mosaic-home.html" },
  { "confirmExit", "ConfirmExit", XtRBoolean, sizeof (Boolean),
      offset (confirm_exit), XtRString, "True" },
  /* Expected to accept -s flag. */
  { "mailCommand", "MailCommand", XtRString, sizeof (char *),
      offset (mail_command), XtRString, "Mail" },
  { "printCommand", "PrintCommand", XtRString, sizeof (char *),
      offset (print_command), XtRString, "lpr" },
  { "xtermCommand", "XtermCommand", XtRString, sizeof (char *),
      offset (xterm_command), XtRString, 
#ifdef _AIX
      "aixterm -v",
#else /* not _AIX */
      "xterm",
#endif /* not _AIX */
    },
  { "globalHistoryFile", "GlobalHistoryFile", XtRString, 
      sizeof (char *),
      offset (global_history_file), XtRString, ".mosaic-global-history" },
  { "defaultHotlistFile", "DefaultHotlistFile", XtRString, 
      sizeof (char *),
      offset (default_hotlist_file), XtRString, ".mosaic-hotlist-default" },
  { "personalAnnotationDirectory", "PersonalAnnotationDirectory", XtRString, 
      sizeof (char *),
      offset (private_annotation_directory), XtRString, 
      ".mosaic-personal-annotations" },
  /* Whether selections should be fancy, by default. */
  { "fancySelections", "FancySelections", XtRBoolean,
      sizeof (Boolean), offset (default_fancy_selections), 
      XtRString, "False" },
  /* --- New in 0.9 --- */
#ifdef GRPAN
  { "annotationServer", "AnnotationServer", XtRString, sizeof (char *),
      offset (annotation_server), XtRString, NULL },
#endif
  { "defaultAuthorName", "DefaultAuthorName", XtRString, sizeof (char *),
      offset (default_author_name), XtRString, NULL },
  { "annotationsOnTop", "AnnotationsOnTop", XtRBoolean, sizeof (Boolean),
      offset (annotations_on_top), XtRString, "False" },

  /* Most of the viewers used here by default are publicly available
     at large anonymous FTP sites.  Please don't mail me asking how to
     find/compile/install/use them; either have your local guru do it
     or learn to do it yourself.  Some are SGI- or NCSA-specific; I don't
     know what people on other systems can do. */
  { "gifViewerCommand", "GifViewerCommand", XtRString, sizeof (char *), 
      offset (gif_viewer_command), XtRString, "xv" },
  { "jpegViewerCommand", "JpegViewerCommand", XtRString, sizeof (char *),
      offset (jpeg_viewer_command), XtRString, "xv" },
  { "tiffViewerCommand", "TiffViewerCommand", XtRString, sizeof (char *),
      offset (tiff_viewer_command), XtRString, "xv" },
  { "audioPlayerCommand", "AudioPlayerCommand", XtRString, sizeof (char *),
      offset (audio_player_command), XtRString, "showaudio" },
  /* This is SGI specific. */
  { "aiffPlayerCommand", "AiffPlayerCommand", XtRString, sizeof (char *),
      offset (aiff_player_command), XtRString, "sfplay" },
  { "dviViewerCommand", "DviViewerCommand", XtRString, sizeof (char *),
      offset (dvi_viewer_command), XtRString, "xdvi" },
  { "mpegViewerCommand", "MpegViewerCommand", XtRString, sizeof (char *),
      offset (mpeg_viewer_command), XtRString, "mpeg_play" },
  { "mimeViewerCommand", "MimeViewerCommand", XtRString, sizeof (char *),
      offset (mime_viewer_command), XtRString, "xterm -e metamail" },
  { "xwdViewerCommand", "XwdViewerCommand", XtRString, sizeof (char *),
      offset (xwd_viewer_command), XtRString, "xwud -in" },
  /* Don't even ask what this is; it's a local hack... */
  { "sgimovieViewerCommand", "SgimovieViewerCommand", 
      XtRString, sizeof (char *),
      offset (sgimovie_viewer_command), XtRString, "sgimovieplay" },
  /* Don't even ask what this is; it's a local hack... */
  { "evlmovieViewerCommand", "EvlmovieViewerCommand", 
      XtRString, sizeof (char *),
      offset (evlmovie_viewer_command), XtRString, "evlmovie" },
  /* This is SGI-specific. */
  { "rgbViewerCommand", "RgbViewerCommand",
      XtRString, sizeof (char *),
      offset (rgb_viewer_command), XtRString, "ipaste" },
  { "postscriptViewerCommand", "PostscriptViewerCommand", XtRString, 
      sizeof (char *), 
      offset (postscript_viewer_command), XtRString, "ghostview" },

  /* --- new in 0.10 --- */
  { "colorsPerInlinedImage", "ColorsPerInlinedImage", XtRInt, sizeof (int),
      offset (colors_per_inlined_image), XtRString, "50" },
  /* Note that this is always overridden by the user's choice, stored
     in ~/.mosaicusr, the first time the program is fired up. */
  /* In 0.10, there is no usage instrumentation yet, so this resource
     is irrelevant. */
  { "instrumentUsage", "InstrumentUsage", XtRBoolean, sizeof (Boolean),
      offset (instrument_usage), XtRString, "False" },
  { "trackVisitedAnchors", "TrackVisitedAnchors", XtRBoolean, sizeof (Boolean),
      offset (track_visited_anchors), XtRString, "True" },

  { "uncompressCommand", "UncompressCommand", XtRString, sizeof (char *), 
      offset (uncompress_command), XtRString, "uncompress" },
  { "gunzipCommand", "GunzipCommand", XtRString, 
      sizeof (char *), offset (gunzip_command), XtRString, "gunzip" },

  { "binaryTransferMode", "BinaryTransferMode", XtRBoolean, sizeof (Boolean),
      offset (binary_transfer_mode), XtRString, "False" },

  { "recordCommandLocation", "RecordCommandLocation", XtRString, 
      sizeof (char *), offset (record_command_location), XtRString,
#ifdef __hpux
      "/usr/audio/bin/srecorder",
#else
#if defined(__sgi)
      "/usr/sbin/recordaiff",
#elif defined (sun)
      "/usr/demo/SOUND/record",
#else
      "/bin/true",
#endif /* if */
#endif /* ifdef */
    },
  { "recordCommand", "RecordCommand", XtRString, sizeof (char *),
      offset (record_command), XtRString,
#ifdef __hpux
      "srecorder -au",
#else
#if defined(__sgi)
      "recordaiff -n 1 -s 8 -r 8000",
#elif defined (sun)
      "record",
#else
      "true",
#endif /* if */
#endif /* ifdef */
    },

  /* --- new in 0.11 --- */
  { "caveViewerCommand", "CaveViewerCommand", XtRString, sizeof (char *),
      offset (cave_viewer_command), XtRString, "caveviewer" },

  /* --- new in 1.0 --- */
  { "gethostbynameIsEvil", "GethostbynameIsEvil", XtRBoolean, sizeof (Boolean),
      offset (gethostbyname_is_evil), XtRString, "False" },
  { "autoPlaceWindows", "AutoPlaceWindows", XtRBoolean, sizeof (Boolean),
      offset (auto_place_windows), XtRString, "True" },
  { "initialWindowIconic", "InitialWindowIconic", XtRBoolean, sizeof (Boolean),
      offset (initial_window_iconic), XtRString, "False" },
  
#ifdef __sgi
  { "debuggingMalloc", "DebuggingMalloc", XtRBoolean, sizeof (Boolean),
      offset (debugging_malloc), XtRString, "False" },
#endif
};

#undef offset

static XrmOptionDescRec options[] = {
  {"-fn",     "*fontList",            XrmoptionSepArg, NULL},
  {"-ft",     "*XmText*fontList",     XrmoptionSepArg, NULL},
  {"-fm",     "*menubar*fontList",    XrmoptionSepArg, NULL},
  {"-home",   "*homeDocument",        XrmoptionSepArg, NULL},
  {"-ngh",    "*useGlobalHistory",    XrmoptionNoArg,  "False"},
  {"-ninst",  "*instrumentUsage",     XrmoptionNoArg,  "False"},
  /* Let Xt strip out -mono from stuff it considers interesting. */
  {"-mono",   "*nothingUseful",       XrmoptionNoArg,  "True"},
  {"-color",  "*nothingUseful",       XrmoptionNoArg,  "True"},
  {"-ghbnie", "*gethostbynameIsEvil", XrmoptionNoArg,  "True"},
  {"-iconic", "*initialWindowIconic", XrmoptionNoArg,  "True"},
  {"-i",      "*initialWindowIconic", XrmoptionNoArg,  "True"},

#ifdef __sgi
  {"-dm",     "*debuggingMalloc",     XrmoptionNoArg,  "True"},
#endif
};

static String color_resources[] = {
  "*XmLabel*fontList:   		-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*XmLabelGadget*fontList:	-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*XmScale*fontList:   		-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*XmBulletinBoard*labelFontList:	-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*optionmenu.XmLabelGadget*fontList:	-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*XmPushButton*fontList:		-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmPushButtonGadget*fontList:	-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmToggleButton*fontList:	-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmToggleButtonGadget*fontList:	-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*optionmenu*fontList:		-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmIconGadget*fontList:		-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmBulletinBoard*buttonFontList: -*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*menubar*fontList:   		-*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmPushButton*fontList:  -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmLabelGadget*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmPushButtonGadget*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmCascadeButton*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmCascadeButtonGadget*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmToggleButton*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmToggleButtonGadget*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*pulldownmenu*fontList:	-*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmList*fontList:	-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmText.fontList:      -*-lucidatypewriter-medium-r-normal-*-14-*-iso8859-1",
  "*XmTextField.fontList: -*-lucidatypewriter-medium-r-normal-*-14-*-iso8859-1",
  
  "*optionmenu*marginHeight: 	0",
  "*optionmenu*marginTop: 		5",
  "*optionmenu*marginBottom: 	5",
  "*optionmenu*marginWidth: 	5",
  "*pulldownmenu*XmPushButton*marginHeight:	1",
  "*pulldownmenu*XmPushButton*marginWidth:	1",
  "*pulldownmenu*XmPushButton*marginLeft:	3",
  "*pulldownmenu*XmPushButton*marginRight:	3",
  "*XmList*listMarginWidth:        3",
  "*menubar*marginHeight: 		1",
  "*menubar.marginHeight: 		0",
  "*menubar*marginLeft:  		1",
  "*menubar.spacing:  		7",
  "*XmMenuShell*marginLeft:  	3",
  "*XmMenuShell*marginRight:  	4",
  "*XmMenuShell*XmToggleButtonGadget*spacing: 	 2",
  "*XmMenuShell*XmToggleButtonGadget*marginHeight:  0",
  "*XmMenuShell*XmToggleButtonGadget*indicatorSize: 12",
  "*XmMenuShell*XmLabelGadget*marginHeight: 4",
  "*XmToggleButtonGadget*spacing: 	4",
  "*XmToggleButton*spacing: 	4",
  "*XmScrolledWindow*spacing: 	0",
  "*XmScrollBar*width: 		        18",
  "*XmScrollBar*height: 		18",
  "*XmScale*scaleHeight: 		20",
  "*XmText*marginHeight:		4",
  "*fsb*XmText*width:                   420",
  "*fillOnSelect:			True",
  "*visibleWhenOff:		        True",
  "*XmText*highlightThickness:		0",
  "*XmTextField*highlightThickness:	0",
  "*XmPushButton*highlightThickness:	0",
  "*XmScrollBar*highlightThickness:     0",
  "*highlightThickness:	                0",
  "*geometry:                           +400+200",
  "*keyboardFocusPolicy:                pointer",
  
  "*TitleFont: -adobe-times-bold-r-normal-*-24-*-*-*-*-*-iso8859-1",
  "*Font: -adobe-times-medium-r-normal-*-17-*-*-*-*-*-iso8859-1",
  "*ItalicFont: -adobe-times-medium-i-normal-*-17-*-*-*-*-*-iso8859-1",
  "*BoldFont: -adobe-times-bold-r-normal-*-17-*-*-*-*-*-iso8859-1",
  "*FixedFont: -adobe-courier-medium-r-normal-*-17-*-*-*-*-*-iso8859-1",
  "*Header1Font: -adobe-times-bold-r-normal-*-24-*-*-*-*-*-iso8859-1",
  "*Header2Font: -adobe-times-bold-r-normal-*-18-*-*-*-*-*-iso8859-1",
  "*Header3Font: -adobe-times-bold-r-normal-*-17-*-*-*-*-*-iso8859-1",
  "*Header4Font: -adobe-times-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*Header5Font: -adobe-times-bold-r-normal-*-12-*-*-*-*-*-iso8859-1",
  "*Header6Font: -adobe-times-bold-r-normal-*-10-*-*-*-*-*-iso8859-1",
  "*AddressFont: -adobe-times-medium-i-normal-*-17-*-*-*-*-*-iso8859-1",
  "*PlainFont: -adobe-courier-medium-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*ListingFont: -adobe-courier-medium-r-normal-*-12-*-*-*-*-*-iso8859-1",

  /* New for 0.13. */
  "*AnchorUnderlines:                   1",
  "*VisitedAnchorUnderlines:            1",
  "*DashedVisitedAnchorUnderlines:      True",

#ifdef __sgi
  "*Foreground:			 	#000000000000",
  "*XmScrollBar*Foreground:             #afafafafafaf",
  "*Background:                         #afafafafafaf",
  "*XmList*Background:     		#afafafafafaf",
  "*XmText*Background:	 	        #afafafafafaf",
  "*TroughColor:                        #545454545454",
  "*XmSelectionBox*Background:	 	#afafafafafaf",
  "*XmMessageBox*Background:	 	#afafafafafaf",
  "*XmLabel*Foreground:                 #1d1d15155b5b",
  "*XmToggleButton*Foreground:          #1d1d15155b5b",
  "*XmPushButton*Foreground:            #5b5b00000000",
  "*XmTextField*Background: 		#8c8c8c8c8c8c",
  "*SelectColor:			#ffffffff0000",
  "*HighlightColor:		 	#afafafafafaf",

  "*TopShadowColor:                     #dfdfdfdfdfdf",
  "*XmList*TopShadowColor:              #dfdfdfdfdfdf",
  "*XmText*TopShadowColor:              #dfdfdfdfdfdf",
  "*XmSelectionBox*TopShadowColor:      #dfdfdfdfdfdf",
  "*XmMessageBox*TopShadowColor:        #dfdfdfdfdfdf",
  
  "*visitedAnchorColor:                 #272705055b5b",
  "*anchorColor:                        #00000000b0b0",
  "*activeAnchorFG:                     #ffff00000000",
  "*activeAnchorBG:                     #afafafafafaf",
#else /* not sgi */
  "*Foreground:			 	#000000000000",
  "*XmScrollBar*Foreground:             #bfbfbfbfbfbf",
  "*XmLabel*Foreground:                 #1d1d15155b5b",
  "*XmToggleButton*Foreground:          #1d1d15155b5b",
  "*XmPushButton*Foreground:            #5b5b00000000",

  "*Background:                         #bfbfbfbfbfbf",

  "*XmList*Background:     		#bfbfbfbfbfbf",
  "*XmText*Background:	 	        #bfbfbfbfbfbf",
  "*XmSelectionBox*Background:	 	#bfbfbfbfbfbf",
  "*XmMessageBox*Background:	 	#bfbfbfbfbfbf",
  "*XmTextField*Background: 		#9c9c9c9c9c9c",

  "*TopShadowColor:                     #e7e7e7e7e7e7",
  "*XmList*TopShadowColor:              #e7e7e7e7e7e7",
  "*XmText*TopShadowColor:              #e7e7e7e7e7e7",
  "*XmSelectionBox*TopShadowColor:      #e7e7e7e7e7e7",
  "*XmMessageBox*TopShadowColor:        #e7e7e7e7e7e7",
  
  "*TroughColor:                        #646464646464",
  "*SelectColor:			#ffffffff0000",
  "*HighlightColor:		 	#bfbfbfbfbfbf",

  /* Remember to update this in the app-defaults file. */
  "*visitedAnchorColor:                 #3f3f0f0f7b7b",
  "*anchorColor:                        #00000000b0b0",
  "*activeAnchorFG:                     #ffff00000000",
  "*activeAnchorBG:                     #bfbfbfbfbfbf",
#endif
  
  NULL,
};

static String mono_resources[] = {
  "*XmLabel*fontList:   		-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*XmLabelGadget*fontList:	-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*XmScale*fontList:   		-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*XmBulletinBoard*labelFontList:	-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*optionmenu.XmLabelGadget*fontList:	-*-helvetica-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*XmPushButton*fontList:		-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmPushButtonGadget*fontList:	-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmToggleButton*fontList:	-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmToggleButtonGadget*fontList:	-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*optionmenu*fontList:		-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmIconGadget*fontList:		-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmBulletinBoard*buttonFontList: -*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*menubar*fontList:   		-*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmPushButton*fontList:  -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmLabelGadget*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmPushButtonGadget*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmCascadeButton*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmCascadeButtonGadget*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmToggleButton*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmMenuShell*XmToggleButtonGadget*fontList: -*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*pulldownmenu*fontList:	-*-helvetica-bold-o-normal-*-14-*-iso8859-1",
  "*XmList*fontList:	-*-helvetica-medium-r-normal-*-14-*-iso8859-1",
  "*XmText.fontList:      -*-lucidatypewriter-medium-r-normal-*-14-*-iso8859-1",
  "*XmTextField.fontList: -*-lucidatypewriter-medium-r-normal-*-14-*-iso8859-1",

  "*optionmenu*marginHeight: 	0",
  "*optionmenu*marginTop: 		5",
  "*optionmenu*marginBottom: 	5",
  "*optionmenu*marginWidth: 	5",
  "*pulldownmenu*XmPushButton*marginHeight:	1",
  "*pulldownmenu*XmPushButton*marginWidth:	1",
  "*pulldownmenu*XmPushButton*marginLeft:	3",
  "*pulldownmenu*XmPushButton*marginRight:	3",
  "*XmList*listMarginWidth:        3",
  "*menubar*marginHeight: 		1",
  "*menubar.marginHeight: 		0",
  "*menubar*marginLeft:  		1",
  "*menubar.spacing:  		7",
  "*XmMenuShell*marginLeft:  	3",
  "*XmMenuShell*marginRight:  	4",
  "*XmMenuShell*XmToggleButtonGadget*spacing: 	 2",
  "*XmMenuShell*XmToggleButtonGadget*marginHeight:  0",
  "*XmMenuShell*XmToggleButtonGadget*indicatorSize: 12",
  "*XmMenuShell*XmLabelGadget*marginHeight: 4",
  "*XmToggleButtonGadget*spacing: 	4",
  "*XmToggleButton*spacing: 	4",
  "*XmScrolledWindow*spacing: 	0",
  "*XmScrollBar*width: 		        18",
  "*XmScrollBar*height: 		18",
  "*XmScale*scaleHeight: 		20",
  "*XmText*marginHeight:		4",
  "*fsb*XmText*width:                   420",
  "*fillOnSelect:			True",
  "*visibleWhenOff:		        True",
  "*XmText*highlightThickness:		0",
  "*XmTextField*highlightThickness:	0",
  "*XmPushButton*highlightThickness:	0",
  "*XmScrollBar*highlightThickness:     0",
  "*highlightThickness:	                0",
  "*geometry:                           +400+200",
  "*keyboardFocusPolicy:                pointer",

  "*TitleFont: -adobe-times-bold-r-normal-*-24-*-*-*-*-*-iso8859-1",
  "*Font: -adobe-times-medium-r-normal-*-17-*-*-*-*-*-iso8859-1",
  "*ItalicFont: -adobe-times-medium-i-normal-*-17-*-*-*-*-*-iso8859-1",
  "*BoldFont: -adobe-times-bold-r-normal-*-17-*-*-*-*-*-iso8859-1",
  "*FixedFont: -adobe-courier-medium-r-normal-*-17-*-*-*-*-*-iso8859-1",
  "*Header1Font: -adobe-times-bold-r-normal-*-24-*-*-*-*-*-iso8859-1",
  "*Header2Font: -adobe-times-bold-r-normal-*-18-*-*-*-*-*-iso8859-1",
  "*Header3Font: -adobe-times-bold-r-normal-*-17-*-*-*-*-*-iso8859-1",
  "*Header4Font: -adobe-times-bold-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*Header5Font: -adobe-times-bold-r-normal-*-12-*-*-*-*-*-iso8859-1",
  "*Header6Font: -adobe-times-bold-r-normal-*-10-*-*-*-*-*-iso8859-1",
  "*AddressFont: -adobe-times-medium-i-normal-*-17-*-*-*-*-*-iso8859-1",
  "*PlainFont: -adobe-courier-medium-r-normal-*-14-*-*-*-*-*-iso8859-1",
  "*ListingFont: -adobe-courier-medium-r-normal-*-12-*-*-*-*-*-iso8859-1",

  "*Foreground:                         black",
  "*Background:                         white",
  "*TopShadowColor:                     black",
  "*BottomShadowColor:                  black",
  "*anchorColor:                        black",
  "*visitedAnchorColor:                 black",
  "*activeAnchorFG:                     black",
  "*activeAnchorBG:                     white",
  "*TroughColor:                        black",
  "*SelectColor:                        black",
  "*AnchorUnderlines:                   1",
  "*VisitedAnchorUnderlines:            1",
  "*DashedVisitedAnchorUnderlines:      True",
  
  NULL,
};

#define __MOSAIC_XRESOURCES_H__
#endif /* __MOSAIC_XRESOURCES_H__ */
