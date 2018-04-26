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
#include "HTML.h"

/*
 * Code to manage a linked list of parsed HTML objects generated
 * from a raw text file.
 * Also code to manage a linked list of formatted elements that
 * make up a page of a formatted document.
 */


/*
 * Free up the passed linked list of parsed elements, freeing
 * all memory associates with each element.
 */
void
FreeObjList(List)
	struct mark_up *List;
{
	struct mark_up *current;
	struct mark_up *mptr;

	current = List;
	while (current != NULL)
	{
		mptr = current;
		current = current->next;
		mptr->next = NULL;
		if (mptr->start != NULL)
		{
			free((char *)mptr->start);
		}
		if (mptr->text != NULL)
		{
			free((char *)mptr->text);
		}
		if (mptr->end != NULL)
		{
			free((char *)mptr->end);
		}
		free((char *)mptr);
	}
}


/*
 * Add an object to the parsed object list.
 * return a pointer to the current (end) position in the list.
 * If the object is a normal text object containing nothing but
 * white space, throw it out, unless we have been told to keep
 * white space.
 */
struct mark_up *
AddObj(listp, current, mark, keep_wsp)
	struct mark_up **listp;
	struct mark_up *current;
	struct mark_up *mark;
	int keep_wsp;
{
	if (mark == NULL)
	{
		return(current);
	}

	/*
	 * Throw out normal text blocks that are only white space,
	 * unless keep_wsp is set.
	 */
	if ((mark->type == M_NONE)&&(!keep_wsp))
	{
		char *ptr;

		ptr = mark->text;
		if (ptr == NULL)
		{
			free((char *)mark);
			return(current);
		}

/*
 * No longer throw out whitespace, it is important to keep
 * white space between tags.
		while ((*ptr == ' ')||(*ptr == '\t')||(*ptr == '\n'))
		{
			ptr++;
		}
 *
 */

		if (*ptr == '\0')
		{
			free(mark->text);
			free((char *)mark);
			return(current);
		}
	}

	/*
	 * Add object to either the head of the list for a new list,
	 * or at the end after the current pointer.
	 */
	if (*listp == NULL)
	{
		*listp = mark;
		current = *listp;
	}
	else
	{
		current->next = mark;
		current = current->next;
	}

	current->next = NULL;

	return(current);
}


#ifdef DEBUG

/*
 * Convert type number to a printed string for debug
 */
void
PrintType(type)
	int type;
{
	switch(type)
	{
		case M_NONE:
			printf("M_NONE");
			break;
		case M_TITLE:
			printf("M_TITLE");
			break;
		case M_FIXED:
			printf("M_FIXED");
			break;
		case M_BOLD:
			printf("M_BOLD");
			break;
		case M_ITALIC:
			printf("M_ITALIC");
			break;
		case M_EMPHASIZED:
			printf("M_EMPHASIZED");
			break;
		case M_STRONG:
			printf("M_STRONG");
			break;
		case M_COD:
			printf("M_COD");
			break;
		case M_SAMPLE:
			printf("M_SAMPLE");
			break;
		case M_KEYBOARD:
			printf("M_KEYBOARD");
			break;
		case M_VARIABLE:
			printf("M_VARIABLE");
			break;
		case M_HEADER_1:
			printf("M_HEADER_1");
			break;
		case M_HEADER_2:
			printf("M_HEADER_2");
			break;
		case M_HEADER_3:
			printf("M_HEADER_3");
			break;
		case M_HEADER_4:
			printf("M_HEADER_4");
			break;
		case M_HEADER_5:
			printf("M_HEADER_5");
			break;
		case M_HEADER_6:
			printf("M_HEADER_6");
			break;
		case M_ANCHOR:
			printf("M_ANCHOR");
			break;
		case M_PARAGRAPH:
			printf("M_PARAGRAPH");
			break;
		case M_ADDRESS:
			printf("M_ADDRESS");
			break;
		case M_PLAIN_TEXT:
			printf("M_PLAIN_TEXT");
			break;
		case M_LISTING_TEXT:
			printf("M_LISTING_TEXT");
			break;
		case M_UNUM_LIST:
			printf("M_UNUM_LIST");
			break;
		case M_NUM_LIST:
			printf("M_NUM_LIST");
			break;
		case M_MENU:
			printf("M_MENU");
			break;
		case M_DIRECTORY:
			printf("M_DIRECTORY");
			break;
		case M_LIST_ITEM:
			printf("M_LIST_ITEM");
			break;
		case M_DESC_LIST:
			printf("M_DESC_LIST");
			break;
		case M_DESC_TITLE:
			printf("M_DESC_TITLE");
			break;
		case M_DESC_TEXT:
			printf("M_DESC_TEXT");
			break;
		case M_IMAGE:
			printf("M_IMAGE");
			break;
		case M_INDEX:
			printf("M_INDEX");
			break;
		default:
			printf("UNKNOWN %d", type);
			break;
	}
}


/*
 * Print the contents of a parsed object list, for debug
 */
void
PrintList(list)
	struct mark_up *list;
{
	struct mark_up *mptr;

	mptr = list;
	while (mptr != NULL)
	{
		PrintType(mptr->type);
		if (mptr->is_end)
		{
			printf(" END");
		}
		else
		{
			printf(" START");
		}
		if (mptr->text != NULL)
		{
			printf("\n{\n\t");
			printf("%s", mptr->text);
			printf("}\n");
		}
		else
		{
			printf("\n");
		}
		mptr = mptr->next;
	}
}

#endif /* DEBUG */


/*
 * Used to find the longest line (in characters) in a collection
 * of text blocks.  cnt is the running count of characters, and
 * txt is the pointer to the current text block.  Since we are
 * finding line widths, a newline resets the width count.
 */
char *
MaxTextWidth(txt, cnt)
	char *txt;
	int *cnt;
{
	char *start;
	char *end;
	int width;

	if (txt == NULL)
	{
		return(NULL);
	}

	width = *cnt;
	start = txt;

	/*
	 * If this blocks starts with a newline, reset the width
	 * count, and skip the newline.
	 */
	if (*start == '\n')
	{
		width = 0;
		start++;
	}

	end = start;

	/*
	 * count characters, stoping either at a newline, or at the
	 * end of this text block.  Expand tabs.
	 */
	while ((*end != '\0')&&(*end != '\n'))
	{
		if (*end == '\t')
		{
			width = ((width / 8) + 1) * 8;
		}
		else
		{
			width++;
		}
		end++;
	}

	*cnt = width;
	return(end);
}


/*
 * Free up the passed linked list of formatted elements, freeing
 * all memory associates with each element.
 */
void
FreeLineList(list)
	struct ele_rec *list;
{
	struct ele_rec *current;
	struct ele_rec *eptr;

	current = list;
	while (current != NULL)
	{
		eptr = current;
		current = current->next;
		eptr->next = NULL;
		if (eptr->edata != NULL)
		{
			free((char *)eptr->edata);
		}
		if (eptr->anchorHRef != NULL)
		{
			free((char *)eptr->anchorHRef);
		}
		free((char *)eptr);
	}
}


/*
 * Add an element to the linked list of formatted elements.
 * return a pointer to the current (end) position in the list.
 */
struct ele_rec *
AddEle(elistp, current, eptr)
	struct ele_rec **elistp;
	struct ele_rec *current;
	struct ele_rec *eptr;
{
	if (eptr == NULL)
	{
		return(current);
	}

	/*
	 * Add object to either the head of the list for a new list,
	 * or at the end after the current pointer.
	 */
	if (*elistp == NULL)
	{
		*elistp = eptr;
		(*elistp)->next = NULL;
		(*elistp)->prev = NULL;
		current = *elistp;
	}
	else
	{
		current->next = eptr;
		eptr->prev = current;
		current = current->next;
		current->next = NULL;
	}
	return(current);
}


/*
 * Contruct and return an array of pointers into the element list that
 * indexes the elements by line number.
 * Note, lines containing only while space will have NULL pointers
 * into the element list.
 */
struct ele_rec **
MakeLineList(elist, max_line)
	struct ele_rec *elist;
	int max_line;
{
	int i;
	struct ele_rec *eptr;
	struct ele_rec **ll;

	/*
	 * malloc index array
	 */
	ll = (struct ele_rec **)malloc(sizeof(struct ele_rec *) * max_line);
	if (ll == NULL)
	{
		fprintf(stderr, "cannot allocate space for line list\n");
		exit(1);
	}

	/*
	 * zero the index array
	 */
	for (i=0; i<max_line; i++)
	{
		ll[i] = NULL;
	}

	/*
	 * fill in pointers to beginning of the lines
	 */
	eptr = elist;
	while (eptr != NULL)
	{
		if (eptr->line_number > max_line)
		{
			break;
		}

		if (ll[eptr->line_number - 1] == NULL)
		{
			ll[eptr->line_number - 1] = eptr;
		}

		eptr = eptr->next;
	}
	return(ll);
}


/*
 * Passed in 2 element pointers, and element positions.
 * Function should return 1 if if start occurs before end.
 * Otherwise return 0.
 */
int
ElementLessThan(start, end, start_pos, end_pos)
	struct ele_rec *start;
	struct ele_rec *end;
	int start_pos, end_pos;
{
	struct ele_rec *current;

	/*
	 * Deal with start or end being NULL
	 */
	if ((start == NULL)&&(end == NULL))
	{
		return(0);
	}
	else if ((start == NULL)&&(end != NULL))
	{
		return(1);
	}
	else if ((start != NULL)&&(end == NULL))
	{
		return(0);
	}

	/*
	 * Deal with easy identical case
	 */
	if (start == end)
	{
		if (start_pos < end_pos)
		{
			return(1);
		}
		else
		{
			return(0);
		}
	}

	/*
	 * We know element Ids are always equal or increasing within a
	 * list.
	 */
	if (start->ele_id < end->ele_id)
	{
		return(1);
	}
	else if (start->ele_id == end->ele_id)
	{
		current = start;
		while (current != NULL)
		{
			if (current->ele_id != start->ele_id)
			{
				break;
			}
			else if (current == end)
			{
				break;
			}
			current = current->next;
		}
		if (current == end)
		{
			return(1);
		}
		else
		{
			return(0);
		}
	}
	else
	{
		return(0);
	}
}


/*
 * Passed in 2 element pointers, and element positions.
 * Function should return 1 if they need to be swapped in order for then
 * to proceed left to right and top to bottom in the text.
 * Otherwise return 0.
 */
int
SwapElements(start, end, start_pos, end_pos)
	struct ele_rec *start;
	struct ele_rec *end;
	int start_pos, end_pos;
{
	struct ele_rec *current;

	/*
	 * Deal with start or end being NULL
	 */
	if ((start == NULL)&&(end == NULL))
	{
		return(0);
	}
	else if ((start == NULL)&&(end != NULL))
	{
		return(1);
	}
	else if ((start != NULL)&&(end == NULL))
	{
		return(0);
	}

	/*
	 * Deal with easy identical case
	 */
	if (start == end)
	{
		if (start_pos > end_pos)
		{
			return(1);
		}
		else
		{
			return(0);
		}
	}

	/*
	 * We know element Ids are always equal or increasing within a
	 * list.
	 */
	if (start->ele_id < end->ele_id)
	{
		return(0);
	}
	else if (start->ele_id == end->ele_id)
	{
		current = start;
		while (current != NULL)
		{
			if (current->ele_id != start->ele_id)
			{
				break;
			}
			else if (current == end)
			{
				break;
			}
			current = current->next;
		}
		if (current == end)
		{
			return(0);
		}
		else
		{
			return(1);
		}
	}
	else
	{
		return(1);
	}
}


/*
 * Free up the allocated list of internal hrefs.
 */
void
FreeHRefs(list)
	struct ref_rec *list;
{
	struct ref_rec *hptr;
	struct ref_rec *tptr;

	hptr = list;
	while (hptr != NULL)
	{
		tptr = hptr;
		hptr = hptr->next;
		if (tptr->anchorHRef != NULL)
		{
			free((char *)tptr->anchorHRef);
		}
		free((char *)tptr);
	}
}


/*
 * Find an element in the linked list of Internal HREFS.
 * return a pointer to the element, or NULL if not found.
 */
struct ref_rec *
FindHRef(list, href)
	struct ref_rec *list;
	char *href;
{
	struct ref_rec *hptr;

	if (href == NULL)
	{
		return(NULL);
	}

	hptr = list;
	while (hptr != NULL)
	{
		if (strcmp(hptr->anchorHRef, href) == 0)
		{
			break;
		}
		hptr = hptr->next;
	}
	return(hptr);
}


/*
 * Add an element to the linked list of Internal HREFS we
 * have visited before.
 * return a pointer to the head of the new list.
 */
struct ref_rec *
AddHRef(list, href)
	struct ref_rec *list;
	char *href;
{
	struct ref_rec *hptr;

	if (href == NULL)
	{
		return(list);
	}

	hptr = FindHRef(list, href);

	if (hptr == NULL)
	{
		hptr = (struct ref_rec *)malloc(sizeof(struct ref_rec));
		if (hptr == NULL)
		{
			fprintf(stderr, "cannot extend internal href list\n");
			return(list);
		}
		hptr->anchorHRef = (char *)malloc(strlen(href) + 1);
		if (hptr->anchorHRef == NULL)
		{
			free((char *)hptr);
			fprintf(stderr, "cannot extend internal href list\n");
			return(list);
		}
		strcpy(hptr->anchorHRef, href);
		hptr->next = list;
		list = hptr;
	}

	return(list);
}

