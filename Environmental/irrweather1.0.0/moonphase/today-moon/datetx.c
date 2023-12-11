/*
 *              Convert Date to Readable Format.
 *
 * Synopsis:
 *
 *      char    *datetxt(buffer, year, month, day);
 *      char    *buffer;        -- Output string goes here
 *      int     year;           -- Year,        1979 = 1979
 *      int     month;          -- Month,       January = 1
 *      int     day;            -- Day,         1 = 1
 *
 * The readable date will be written into the outpub buffer, terminated by
 * a null byte.  datetxt returns a pointer to the null byte.
 *
 * External routines called:
 *
 *      nbrtxt          (Number to ascii conversion)
 *      copyst          (String copy routine)
 */

extern  char    *nbrtxt();
extern  char    *copyst();
extern  char    *datetxt();

static char *daynames[] = {
	"Sunday",                       /* Sunday is day zero           */
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",
};

static char *monthnames[] = {
	"?Nomember?",                   /* Illegal month                */
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December",
};

char *datetxt(buffer, year, month, day)
char    *buffer;                        /* Output goes here             */
int     year;                           /* Year, 1979 = 1979            */
int     month;                          /* Month of year, Jan = 1       */
int     day;                            /* Day in the month 1 = 1       */
/*
 * Output the date in readable format:
 *      Tuesday, the third of October
 */
{
	register char   *op;                    /* Output pointer       */

	op = buffer;                            /* Setup output pointer */
	op = copyst(op, daynames[dayofweek(year, month, day)]);
	op = copyst(op, ", the ");
	op = nbrtxt(op, day, 1);
	op = copyst(op, " day of ");
	op = copyst(op, monthnames[(month < 0 || month > 12) ? 0 : month]);
	op = copyst(op, ", ");
	if (year < 1000 || year >= 2000)
		return(nbrtxt(op, year, 0));
	else {
		op = nbrtxt(op, year/100, 0);
		op = copyst(op, " ");
		if ((year = year % 100) == 0)
			return(copyst(op, "hundred"));
		else
			return(nbrtxt(op, year, 0));
	}
}

dayofweek(year, month, day)
int     year;                                   /* Year, 1978 = 1978    */
int     month;                                  /* Month, January = 1   */
int     day;                                    /* Day of month, 1 = 1  */
/*
 * Return the day of the week on which this date falls: Sunday = 0.
 * Note, this routine is valid only for the Gregorian calender.
 */
{
	register int yearfactor;

	yearfactor = year + (month - 14)/12;
	return (( (13 * (month + 10 - (month + 10)/13*12) - 1)/5
		+ day + 77 + 5 * (yearfactor % 100)/4
		+ yearfactor / 400
		- yearfactor / 100 * 2) % 7);
}
