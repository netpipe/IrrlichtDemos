/*
 *              Convert Time to a Readable Format.
 *
 * Synopsis:
 *
 *      char    *timetxt(buffer, hour, minute, second, daylight);
 *      char    *buffer;        -- Where output goes
 *      int     hour;           -- Hour,        range is 0 to 24
 *      int     minute;         -- Minute,      range is -1 to 59
 *      int     second;         -- Seconds,     range is -1 to 59
 *      int     daylight;       -- Daylight savings time if non-zero.
 *
 * Note: if minute or second is less than zero, the value is not calculated.
 * This distinguishes between "unknown seconds" and "exactly no seconds."
 * If hour is less than zero, a null string is returned.
 * Timetxt converts the time to a null-trailed string.  It returns a pointer
 * to the first free byte (i.e. the null);
 *
 * The output follows the syntax of Robert J. Lurtsema, and includes:
 *
 *      In twenty-five seconds, the time will be ten minutes before noon.
 *
 *
 * External routines called:
 *
 *      nbrtxt          (Number to ascii conversion)
 *      copyst          (String copy routine)
 */

extern char     *nbrtxt();
extern char     *copyst();

char *timetxt(buffer, hour, minute, second, daylight)
char    *buffer;                        /* Output buffer                */
int     hour;                           /* Hours 00 - 23                */
int     minute;                         /* Minutes                      */
int     second;                         /* Seconds                      */
int     daylight;                       /* Non-zero if savings time     */
/*
 * Output time of day.
 */
{
	char            *op;            /* Output pointer               */
	register int    late;           /* after hour or afternoon      */
	register int	sec;		/* Seconds temp			*/
	char		*stuff();	/* Buffer stuffer		*/

	op = buffer;                    /* Setup buffer pointer         */
	if (hour < 0) {			/* If it's a dummy call,	*/
		*op = 0;		/* Return a null string		*/
		return(op);
	}
	if (daylight == 0101010) {      /* Secret flag                  */
		op = copyst(op, "The big hand is on the ");
		op = nbrtxt(op, (((minute + 2 + second/30)/5 + 11)%12)+1, 0);
		op = copyst(op," and the little hand is on the ");
		op = nbrtxt(op, ((hour + 11) % 12) + 1, 0);
		return(copyst(op, ".  "));
	}
	/*
	 * Check if the time is more than 30 minutes past the hour.
	 * If so, output the time before the next hour.
	 */
	if (minute < 0) second = (-2);  /* No minutes means no seconds  */
	else if ((late = (minute > 30 || (minute == 30 && second > 0)))) {
		if (second > 0) {       /* Before next hour             */
			second = 60 - second;
			minute += 1;    /* Diddle the minute, too       */
		}
		minute = 60 - minute;   /* Minutes before next hour     */
		hour += 1;              /* Frobozz next hour getter     */
	}
	/*
	 * Decisions, decisions:
	 *	Minutes	Seconds =>
	 *	  00	  00	Exactly Noon
	 *	  00	  01	One second after noon
	 *	  01	  00	Exactly one minute after noon
	 *	  30	  00	Exactly half past noon
	 *	  59	  00	Exactly one minute before noon
	 *	  59	  59	In one second, the time will be noon
	 */
	if (late > 0 && second > 0) {
		op = stuff(op, second, 1, "In ", " second");
		op = copyst(op, ", the time will be ");
		sec = -2;		/* We've done seconds already	*/
	}
	else if (daylight != -1) {
		op = copyst(op, "The time is ");
		sec = second;		/* Seconds still to be done	*/
	}
	if (sec == 0) {
		op = copyst(op, "exactly ");
		if (minute == 30)
			op = copyst(op, "half past ");
		else	op = stuff(op, minute, 1, " ", " minute");
	}
	else {				/* Non exact or missing seconds	*/
		op = stuff(op, minute, 0, " ",     " minute");
		if(second > 0)
	        op = stuff(op, sec, (sec > 0),  " and ", " second");
	}
	op = copyst(op, (minute < 0 || (minute == 0 && late)
			|| (second == 0
				&& ((minute == 0 && late == 0)
					|| minute == 30))) ? " "
		: (late) ? " before " : " after ");
	/*
	 * Hours are not quite so bad
	 */
	if (hour == 0 || hour == 24)
		op = copyst(op, "midnight");
	else if (hour == 12)
		op = copyst(op, "noon");
	else {
		if (late = (hour > 12))
			hour = hour - 12;
		op = nbrtxt(op, hour, 0);
		op = copyst(op, (late) ? " PM" : " AM");
	}
	if (daylight != -1) 
	    op = copyst(op, (daylight) ? TZ_Str1 : TZ_Str2); /* See Makefile */
	return(op);
}

//static char *
char *
stuff(buffer, value, flag, leading, trailing)
char    *buffer;                        /* Output goes here             */
int     value;                          /* The value to print if > 0    */
int     flag;                           /* flag is set to print leading */
char    *leading;                       /* preceeded by ...             */
char    *trailing;                      /* and followed by ...          */
/*
 * If value <= zero, output nothing. Else, output "leading" value "trailing".
 * Note: leading is output only if flag is set.
 * If value is not one, output an "s", too.
 */
{
	register char   *op;            /* Output pointer               */

	op = buffer;                    /* Setup buffer pointer         */
	if (value > 0) {
		if (flag)
			op = copyst(op, leading);
		op = nbrtxt(op, value, 0);
		op = copyst(op, trailing);
		if (value != 1)
			op = copyst(op, "s");
	}
	return(op);
}
