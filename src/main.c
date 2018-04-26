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
#include <signal.h>

void mo_exit (void)
{
  mo_write_default_hotlist ();
  if (Rdata.use_global_history)
    mo_write_global_history ();
  mo_write_pan_list ();

  if (can_instrument && Rdata.instrument_usage)
    mo_inst_dump_and_flush_data ();

  exit (0);
}

/* Extern to mo_do_gui. */
#ifdef _HPUX_SOURCE
int Interrupt (void)
#else /* not HP */
void Interrupt (void)
#endif
{
  /* Apparently this doesn't work on the Sun. */
  signal (SIGINT, Interrupt);
#ifndef VMS
  signal (SIGUSR1, Interrupt);
#endif
  mo_recover_from_interrupt ();
}

static void RealFatal (void)
{
  signal (SIGBUS, 0);
  signal (SIGSEGV, 0);
  signal (SIGILL, 0);
  abort ();
}

#ifdef _HPUX_SOURCE
static int FatalProblem(int sig, int code, struct sigcontext *scp,
                        char *addr)
#else
static void FatalProblem(int sig, int code, struct sigcontext *scp,
                         char *addr)
#endif
{
  fprintf (stderr, "\nCongratulations, you have found a bug in\n");
  fprintf (stderr, "NCSA Mosaic %s on %s.\n\n", MO_VERSION_STRING, 
           MO_MACHINE_TYPE);
  fprintf (stderr, "If a core file was generated in your directory,\n");
  fprintf (stderr, "please run 'dbx xmosaic' and then type:\n");
  fprintf (stderr, "  dbx> where\n");
  fprintf (stderr, "and mail the results, and a description of what you were doing at the time,\n");
  fprintf (stderr, "to %s.  We thank you for your support.\n\n", 
           MO_DEVELOPER_ADDRESS);
  fprintf (stderr, "...exiting NCSA Mosaic now.\n\n");

  RealFatal ();
}

main (int argc, char **argv)
{
  signal (SIGBUS, FatalProblem);
  signal (SIGSEGV, FatalProblem);
  signal (SIGILL, FatalProblem);

  /* SIGINT and SIGUSR1 signals set in mo_do_gui now. */

  mo_do_gui (argc, argv);
}
