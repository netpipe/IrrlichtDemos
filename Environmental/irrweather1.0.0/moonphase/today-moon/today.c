/*
 *			T O D A Y
 *
 * time of day
 *
 * Define UNIX for "native" Unix
 */

/*)BUILD	$(PROGRAM)	= today
		$(FILES)	= { today datetx timetx nbrtxt moontx }
		$(TKBOPTIONS)	= {
			TASK	= ...TOD
		}
*/

#ifdef	DOCUMENTATION

title	today	Date and Time in English
index		Date and Time in English

synopsis

	today [-] [x] | [date]

description

	Today prints the date, time, and phase of the moon in English.
	The following options are available:
	.lm +8
	.s.i -8;- or x	Read date strings from the standard input file.
	.s.i -8;date	Print information for the indicated date.
	.s.lm -8
	Date and time information is given in ISO numeric notation.  For
	example, November 6, 1980 would be represented as "801106".  If
	a time is needed, it would be appended to the date, using 24-hour
	notation: "801106110402" would be a time which is exact to the
	second.  To specify the century, the two-digit century number
	may be preceeded by '+' as in "+18801106".
	.s
	Non-numeric separators between the various fields are permitted:
	"+1776.07.04-11:15:21".  Note that the full two digit entry must be
	given.
	.s
	If no parameter is given, today outputs the current date and time.

diagnostics

	.lm +8
	.s.i -8;Bad parameters or date out of range in ...
	.s
	An input date or time is incorrect.
	.lm -8

author

	Martin Minow

bugs

	The algorithm is only valid for the Gregorian calender.

#endif

//#define	APRIL_FOOLS
#undef	APRIL_FOOLS

int	__narg	=	1;		/* No prompt if no args		*/
#define LINEWIDTH       72              /* Width of line                */

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#undef NULL

#define	NULL		0
#define	EOS		0
#define	FALSE		0
#define	TRUE		1

int day_month[] = {			/* Needed for dotexttime()      */
	0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
int     ccpos;                          /* Current line position        */
char    lastbyte;                       /* Memory for output()          */
char    line[100];                      /* Data line for input function */
char    *valptr;                        /* Needed for number converter  */
char    wordbuffer[LINEWIDTH];          /* Buffer for output function   */
char    *wordptr = wordbuffer;          /* Free byte in wordbuffer      */
char	linebuffer[LINEWIDTH+2];	/* Output text buffer		*/
char	*lineptr = linebuffer;		/* Free byte in linebuffer	*/
int     polish;                         /* Funny mode flag              */
int	sunrise;			/* Sunrise print flag		*/

extern  char    *datetxt();             /* Date getter                  */
extern  char    *timetxt();             /* Time of day getter           */
extern  char    *moontxt();             /* Phase of the moon getter     */

main(argc, argv)
int     argc;
char    *argv[];
/*
 * Driver for time routines.  Usage:
 *
 *      today                   Prints current time of day in readable form,
 *                              followed by a cookie.
 *
 *      today {+cc}yymmddhhmmss Prints indicated time of day.
 *                              Note, hh, mm, ss may be omitted.
 *                              For example:
 *                                today 401106     = Nov. 6, 1940
 *                                today +19401106  = Nov. 6, 1940
 *                                today 4011061015 = Nov. 6, 1940 10:15 AM
 *
 *      today -                 Data is read from the standard input and
 *      today x                 output as needed.  The format of each
 *                              line is identical to the command format.
 *				("today x" is needed for vms.)
 */

{
  ccpos = 0;			/* New line now                 */
  wordptr = wordbuffer;		/* Nothing buffered             */
  lineptr = linebuffer;		/* Nothing in output buffer too	*/
  polish = 0;			/* Normal mode			*/
  if (argc > 1 && tolower(argv[1][0]) == 'p') {
    polish = 1;
    argc--;
    argv++;
  }
  sunrise = 0;
  if (argc > 1 && tolower(argv[1][0]) == 's') {
    sunrise = 1;
    argc--;
    argv++;
  }
  
  if (argc == 1) sunrise = 1;
  
  if (argc == 2 && ((argv[1][0] == '-') || (argv[1][0] | 040) == 'x')) {
///    while (!getline()) {	/* Read and print times */
///      dotexttime(line);
///    }
    return;
  }
  else if (argc > 1) {
    if (dotexttime(argv[1]) == 0)
      return;
  }
  /*
   * Here if no parameters or an error in the parameter field.
   */
  dotime();			/* Print the time.              */
  output("\n");           	/* Space before cookie          */

#ifdef	UNIX
  execl(COOKIEPROGRAM, "cookie", 0);
#endif
}

dotime()
/*
 * Print the time of day for Unix or VMS native mode.
 */
{
  long    tvec;                   /* Buffer for time function     */
  struct  tm *localtime();	/* Unix time decompile function */
  struct  tm *p;			/* Local pointer to time of day */
  int     year;
  int     month;
  
  time(&tvec);                     /* Get the time of day          */
  p = localtime(&tvec);           /* Make it more understandable  */
  year = p->tm_year + 1900;
  month = p->tm_mon + 1;
  
#ifdef	APRIL_FOOLS
  if (month == 4 && p->tm_mday == 1)
    polish = !polish;
#endif

  process(year, month, p->tm_mday, p->tm_hour,
	  p->tm_min, p->tm_sec, p->tm_isdst);
}

dotexttime(text)
char    *text;                          /* Time text                    */
/*
 * Create the time values and print them, return 1 on error.
 */

{
  int     epoch;                  /* Which century                */
  int     year;
  int     month;
  int     day;
  int     hour;
  int     minute;
  int     second;
  int     leapyear;
  
  valptr = text;                          /* Setup for getval()   */
  while (*valptr == ' ') valptr++;        /* Leading blanks skip  */
  if (*valptr != '+')
    epoch = 1900;                   /* Default for now      */
  else {
    valptr++;
    if ((epoch = getval(-1, 00, 99)) < 0) goto bad;
    epoch *= 100;		/* Make it a real epoch */
  }
  
  if ((year = getval(-1, 00, 99)) < 0) goto bad;
  year += epoch;
  leapyear = ((year%4) == 0) && (((year%400) == 0) || (year%100 != 0));
  if ((month = getval(-1, 1, 12)) < 0) goto bad;
  if ((day = getval(-1, 1,
		    (month == 2 && leapyear) ? 29 : day_month[month])) < 0)
    goto bad;
  if ((hour = getval(-2, 0, 23)) == -1) goto bad;
  if ((minute = getval(-2, 0, 59)) == -1) goto bad;
  if ((second = getval(-2, 0, 59)) == -1) goto bad;
  process(year, month, day, hour, minute, second, 0);
  return(0);				/* Normal exit		*/
  
 bad:    output("Bad parameters or date out of range in \"");
  output(text);
  output("\" after scanning \"");
  *valptr = '\0';
  output(text);
  output("\".\n");
  return(1);				/* Error exit		*/
}

static	char    outline[500];		/* Output buffer                */

process(year, month, day, hour, minute, second, daylight)
int     year;                           /* Year		1900 = 1900	*/
int     month;                          /* Month	January = 1	*/
int     day;                            /* Day		1 = 1		*/
int	hour;				/* Hour		0 .. 23		*/
int	minute;				/* Minute	0 .. 59		*/
int	second;				/* Second	0 .. 59		*/
int	daylight;			/* Daylight savings time if 1	*/
/*
 * Output the information.  Note that the parameters are within range.
 */
{
  
  output("Today is ");
  datetxt(outline, year, month, day);
  output(outline);
  output(".  ");
  timetxt(outline, hour, minute, second,
	  (polish) ? 0101010 : daylight);
  output(outline);
  if (sunrise) {
    int sunrh, sunrm, sunsh, sunsm;
    sun(&sunrh, &sunrm, &sunsh, &sunsm);
    output("\nSunrise is at ");
    timetxt(outline, sunrh, sunrm, -2, -1);
    output(outline);
    output("\nSunset is at ");
    timetxt(outline, sunsh, sunsm, -2, -1);
    output(outline);
    output(".\n");
  }

  moontxt(outline);	/* replaced by smarter version */
  output(outline);
  output(".\n");
  
}


output(text)
char    *text;                                  /* What to print        */
/*
 * Output routine.  Text is output using put() so that lines are
 * not more than LINEWIDTH bytes long.  Current position is in global ccpos.
 * (put is equivalent to putchar() except that it is locally buffered.)
 */
{
  register char	*in;		/* Current pos. in scan */
  register char	c;		/* Current character    */
  register char	*wp;		/* Word pointer		*/

  in = text;
  while (c = *in++) {
    switch (c) {
    case '\n':			/* Force new line       */
    case ' ':			/* or a space seen      */
      if ((wordptr-wordbuffer) + ccpos >= LINEWIDTH) {
	put('\n');		/* Current word         */
	ccpos = 0;		/* won't fit, dump it.  */
      }
      if (wordptr > wordbuffer) {
	if (ccpos) {		/* Leading space needed */
	  put(' ');
	  ccpos++;
	}
	for (wp = wordbuffer; wp < wordptr;) {
	  put(*wp++);
	}
	ccpos += (wordptr - wordbuffer);
	wordptr = wordbuffer;	/* Empty buffer	*/
      }
      if (c == '\n') {
	put('\n');		/* Print a newline	*/
	ccpos = 0;		/* and reset the cursor	*/
      }
      break;
			
    default:
      *wordptr++ = c;		/* Save piece of word   */
    }
  }
}

put(c)
register char	c;
/*
 * Actual output routine
 */
{
  if (c == '\n' || (lineptr - linebuffer) >= LINEWIDTH) {
    *lineptr = EOS;
    puts(linebuffer);
    lineptr = linebuffer;
    if (c == '\n')
      return;
  }
  *lineptr++ = c;
} 

/*
 * Read text to global line[].  Return 1 on end of file, zero on ok.

getline() {
  char *t;
#ifdef NJC
  char *p;

  // slightly safer handling of gets
  // '\n' line is not an empty line
  // EOF should be an empty line
  if(fgets(line, sizeof(line), stdin) == NULL) {
    return(1);
  }

  if((p = strchr(line, '\n')) != NULL) {
    *p = '\0';
  }

  return(0);
  // was
  // return(fgets(line, sizeof(line), stdin) == NULL);
#else  // old and unsafe
  return (gets(line) == NULL);
#endif
}
 */
getval(flag, low, high)
int     flag;
int     low;
int     high;
/*
 * Global valptr points to a 2-digit positive decimal integer.
 * Skip over leading non-numbers and return the value.
 * Return flag if text[0] == '\0'. Return -1 if the text is bad,
 * or if the value is out of the low:high range.
 */
{
  register int value;
  register int i;
  register int temp;
  
  if (*valptr == '\0') return(flag);        /* Default?             */
  while (*valptr && (*valptr < '0' || *valptr > '9')) *valptr++;
  /* The above allows for 78.04.22 format */
  for (value = i = 0; i < 2; i++) {
    temp = *valptr++ - '0';
    if (temp < 0 || temp > 9) return(-1);
    value = (value*10) + temp;
  }
  return((value >= low && value <= high) ? value : -1);
}

