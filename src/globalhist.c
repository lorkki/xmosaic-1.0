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

#include "libhtmlw/HTML.h" /* for ImageInfo */

/* ------------------------------------------------------------------------ */
/* ---------------------------- GLOBAL HISTORY ---------------------------- */
/* ------------------------------------------------------------------------ */

/* We save history list out to a file (~/.mosaic-global-history) and
   reload it on entry.

   Initially the history file format will look like this:

   ncsa-mosaic-history-format-1            [identifying string]
   Global                                  [title]
   url Fri Sep 13 00:00:00 1986            [first word is url;
                                            subsequent words are
                                            last-accessed date (GMT)]
   [1-line sequence for single document repeated as necessary]
   ...
*/

#define NCSA_HISTORY_FORMAT_COOKIE_ONE "ncsa-mosaic-history-format-1"

#define HASH_TABLE_SIZE 200

/* An entry in a hash bucket, containing a URL (in canonical,
   absolute form) and possibly cached info (right now, an ImageInfo
   struct for inlined images). */
typedef struct entry
{
  /* Canonical URL for this document. */
  char *url;
  /* Cached image info, if appropriate. */
  ImageInfo *imginfo;
  struct entry *next;
} entry;

/* A bucket in the hash table; contains a linked list of entries. */
typedef struct bucket
{
  entry *head;
  int count;
} bucket;

static bucket hash_table[HASH_TABLE_SIZE];

/* Given a URL, hash it and return the hash value, mod'd by the size
   of the hash table. */
static int hash_url (char *url)
{
  int len, i, val;

  if (!url)
    return 0;
  len = strlen (url);
  val = 0;
  for (i = 0; i < 10; i++)
    val += url[(i * val + 7) % len];

  return val % HASH_TABLE_SIZE;
}

/* Each bucket in the hash table maintains a count of the number of
   entries contained within; this function dumps that information
   out to stdout. */
static void dump_bucket_counts (void)
{
  int i;

  for (i = 0; i < HASH_TABLE_SIZE; i++)
    fprintf (stdout, "Bucket %03d, count %03d\n", i, hash_table[i].count);
  
  return;
}

/* Presumably url isn't already in the bucket; add it by
   creating a new entry and sticking it at the head of the bucket's
   linked list of entries. */
static void add_url_to_bucket (int buck, char *url)
{
  bucket *bkt = &(hash_table[buck]);
  entry *l;

  l = (entry *)malloc (sizeof (entry));
  l->url = strdup (url);
  l->imginfo = NULL;
  l->next = NULL;
  
  if (bkt->head == NULL)
    bkt->head = l;
  else
    {
      l->next = bkt->head;
      bkt->head = l;
    }

  bkt->count += 1;
}

/* This is the internal predicate that takes a URL, hashes it,
   does a search through the appropriate bucket, and either returns
   1 or 0 depending on whether we've been there. */
static int been_here_before (char *url)
{
  int hash = hash_url (url);
  entry *l;

  if (hash_table[hash].count)
    for (l = hash_table[hash].head; l != NULL; l = l->next)
      {
        if (!strcmp (l->url, url))
          return 1;
      }
  
  return 0;
}


/* 
 * Have we been here before?
 * See if the url is in the hash table yet.
 * If it's not, return mo_fail, else mo_succeed.
 *
 * This is the external interface; just call been_here_before().
 */
mo_status mo_been_here_before_huh_dad (char *url)
{
  if (been_here_before (url))
    return mo_succeed;
  else
    return mo_fail;
}

/* 
 * We're now here.  Take careful note of that fact.
 *
 * Add the current URL to the appropriate bucket if it's not
 * already there.
 */
mo_status mo_here_we_are_son (char *url)
{
  if (!been_here_before (url))
    add_url_to_bucket (hash_url (url), url);
  
  return mo_succeed;
}

/* ------------------------------------------------------------------------ */

/* Given a filename for the global history file, load the contents
   into the global hashtable. */
static void mo_read_global_history (char *filename)
{
  FILE *fp;
  char *line = (char *)malloc (MO_LINE_LENGTH * sizeof (char));
  
  fp = fopen (filename, "r");
  if (!fp)
    goto screwed_no_file;
  
  line = fgets (line, MO_LINE_LENGTH, fp);
  if (!line)
    goto screwed_with_file;
  
  /* See if it's our format. */
  if (strncmp (line, NCSA_HISTORY_FORMAT_COOKIE_ONE,
               strlen (NCSA_HISTORY_FORMAT_COOKIE_ONE)))
    goto screwed_with_file;

  /* Go fetch the name on the next line. */
  line = fgets (line, MO_LINE_LENGTH, fp);
  if (!line)
    goto screwed_with_file;
  
  /* Start grabbing url's. */
  while (1)
    {
      char *url;
      
      line = fgets (line, MO_LINE_LENGTH, fp);
      if (!line || !(*line))
        goto done;
      
      url = strdup (strtok (line, " "));
      /* We don't use the last-accessed date... yet. */
      /* lastdate = strdup (strtok (NULL, "\n")); */

      add_url_to_bucket (hash_url (url), url);
    }
  
 done:
  free (line);
  fclose (fp);
  return;

 screwed_with_file:
  free (line);
  fclose (fp);
  return;

 screwed_no_file:
  free (line);
  return;
}

/*
 * Called upon program initialization.
 * Sets up the global history from the init file.
 * Also coincidentally initializes history structs.
 * Returns mo_succeed.
 */
static char *cached_global_hist_fname = NULL;

mo_status mo_init_global_history (void)
{
  int i;

  /* Initialize the history structs. */
  for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
      hash_table[i].count = 0;
      hash_table[i].head = 0;
    }

  return mo_succeed;
}

mo_status mo_wipe_global_history (void)
{
  /* Memory leak! @@@ */
  mo_init_global_history ();

  return;
}

mo_status mo_setup_global_history (void)
{
  char *home = getenv ("HOME");
  char *default_filename = Rdata.global_history_file;
  char *filename;

  mo_init_global_history ();

  /* This shouldn't happen. */
  if (!home)
    home = "/tmp";
  
  filename = (char *)malloc 
    ((strlen (home) + strlen (default_filename) + 8) * sizeof (char));
  sprintf (filename, "%s/%s\0", home, default_filename);
  cached_global_hist_fname = filename;

  mo_read_global_history (filename);
  
  return mo_succeed;
}

mo_status mo_write_global_history (void)
{
  FILE *fp;
  int i;
  entry *l;
  time_t foo = time (NULL);
  char *ts = ctime (&foo);

  ts[strlen(ts)-1] = '\0';
 
  fp = fopen (cached_global_hist_fname, "w");
  if (!fp)
    return mo_fail;

  fprintf (fp, "%s\n%s\n", NCSA_HISTORY_FORMAT_COOKIE_ONE, "Global");
  
  for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
      for (l = hash_table[i].head; l != NULL; l = l->next)
        {
          fprintf (fp, "%s %s\n", l->url, ts);
        }
    }
  
  fclose (fp);
  
  return mo_succeed;
}

/* ------------------------- image caching stuff -------------------------- */

void *mo_fetch_cached_image_info (char *url)
{
  int hash = hash_url (url);
  entry *l;

  if (hash_table[hash].count)
    for (l = hash_table[hash].head; l != NULL; l = l->next)
      {
        if (!strcmp (l->url, url))
          {
            return (ImageInfo *)(l->imginfo);
          }
      }
  
  /* If we couldn't find the entry at all, obviously we didn't
     cache the image. */
  return NULL;
}

mo_status mo_cache_image_info (char *url, void *info)
{
  int hash = hash_url (url);
  entry *l;

  /* First, register ourselves if we're not already registered. */
  /* Call external interface since it checks to prevent redundancy. */
  mo_here_we_are_son (url);

  /* Then, find the right entry. */
  if (hash_table[hash].count)
    for (l = hash_table[hash].head; l != NULL; l = l->next)
      {
        if (!strcmp (l->url, url))
          goto found;
      }
  
  return mo_fail;

 found:
  l->imginfo = (ImageInfo *)info;
  return mo_succeed;
}
