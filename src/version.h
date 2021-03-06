/*  version.h
 *  Dec 04 (PJW)
 *  
 *  Marker file for holding information about overall
 *  revisions to the program.
 *
 * $Id: version.h 49 2018-06-16 17:15:39Z wilcoxen $
 *
 * $Log: version.h,v $
 * Revision 2.14  2007/10/07 13:17:36  wilcoxen
 * added some new units to the msgproc back end.
 *
 * Revision 2.13  2005/11/26 05:12:48  wilcoxen
 * improved output of /doc and /syntax.
 *
 * Revision 2.12  2005/08/16 15:01:52  wilcoxen
 * drop date because it is UTC and may be confusing.
 *
 * Revision 2.11  2005/08/16 13:49:27  wilcoxen
 * increment for changes in msgproc
 *
 * Revision 2.10  2005/08/11 22:32:12  wilcoxen
 * support html output
 *
 * Revision 2.9  2005/08/11 17:43:14  wilcoxen
 * track version numbers for language modules
 *
 * Revision 2.8  2005/06/27 00:57:47  wilcoxen
 * Better error message when build_domain fails because of late discovery of
 * undeclared variables (why not found earlier?).
 * Convert a bunch of errors to use error_front() and error_more().
 * Tidy up the display of equations in the info file.
 * Support /first and /last flags.
 * Omit equations when their time domain is incompatible with the target language.
 * Allow {} for equation attributes.
 * Stifle equations that are incompatible with the target language.
 * Added check_identifiers to enforce domain rules.
 *
 * Revision 2.7  2005/01/06 19:07:32  wilcoxen
 * handle time sets and allow first and last as explicit
 * subsets referring to the first and last periods.  other
 * subsets are generated automatically if needed.
 *
 * Revision 2.6  2004/12/22 17:50:15  wilcoxen
 * vast changes to clean up the split between the front
 * end and the back end.  preliminary support for tablo.
 *
 * Revision 2.5  2004/12/17 16:04:57  wilcoxen
 * improve diagnostics generated by syntax errors.
 *
 */

#ifndef VERSION_H
#define VERSION_H

char *rcs = "$Revision: 49 $";

#endif /* VERSION_H */
