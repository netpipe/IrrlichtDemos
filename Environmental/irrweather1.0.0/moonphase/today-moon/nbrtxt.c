/*
 *              Integer to Readable ASCII Conversion Routine.
 *
 * Synopsis:
 *
 *      char *nbrtxt(buffer, value, ordinal)
 *      char    *buffer;        -- The output buffer
 *      int     value;          -- The number to output
 *      int     ordinal;        -- Non-zero for ordinal number
 *
 *
 * The value is converted to a readable number and put in the output
 * buffer (null-terminated).  A pointer to the first free location
 * in the buffer (i.e., the null) is returned.  The ordinal
 * flag distinguishes between cardinal and ordinal numbers:
 *
 *      nbrtxt(buffer, 1, 0) = "one"
 *      nbrtxt(buffer, 1, 1) = "first"
 *
 * The longest output string is:
 *
 *      Twenty-seven thousand, three hundred and seventy-seventh.
 *
 *
 *
 *              Copy a String
 *
 * Synopsis
 *
 *      char *copyst(out, in)
 *      char    *out;           -- The output string
 *      char    *in;            -- The input string
 *
 * The input string is copied into the output string.  Copyst returns
 * a pointer to the null trailer.
 *
 */

extern char     *nbrtxt();
extern char     *copyst();

static char *cardinal[] = {
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
	"ten",
	"eleven",
	"twelve",
	"thirteen",
	"fourteen",
	"fifteen",
	"sixteen",
	"seventeen",
	"eighteen",
	"nineteen"
};

static char *ordinal[] = {
	"zeroth",
	"first",
	"second",
	"third",
	"fourth",
	"fifth",
	"sixth",
	"seventh",
	"eighth",
	"ninth",
	"tenth",
	"eleventh",
	"twelfth"
};

static char *twenties[] = {
	"twen",
	"thir",
	"for",
	"fif",
	"six",
	"seven",
	"eigh",
	"nine"
};

char *nbrtxt(buffer, datum, ordflag)
char    *buffer;                        /* Output string buffer         */
int     datum;                          /* what to translate            */
int     ordflag;                        /* 0 if cardinal, 1 if ordinal  */
/*
 * Translate a number to a readable text string, punctuation and all.
 * If ordflag is non-zero, ordinal numbers ("first, second") will
 * be generated, rather than cardinal ("one, two").
 * Note: nbrtxt() is recursive.
 */
{

	register int value;
	register char   *op;

	op = buffer;
	value = datum;
	if (value < 0) {
		op = copyst(op, "minus ");
		value = (-value);
		if (value < 0) {                /* Hack -32768          */
			op = copyst(op, twenties[1]);
			value = 2768;
		}
	}
	if (value >= 1000) {
		op = nbrtxt(op, value/1000, 0);
		op = copyst(op, " thousand");
		value = value % 1000;
		if (value == 0) goto exit;
		op = copyst(op, (value >= 100) ? ", " : " and ");
	}
	if (value >= 100) {
		op = copyst(op, cardinal[value/100]);
		op = copyst(op, " hundred");
		value = value % 100;
		if (value == 0) goto exit;
		op = copyst(op, " ");
	}
	if (value >= 20) {
		if (value == 90 && ordflag)
			return(copyst(op, "nintieth"));
		op = copyst(op, twenties[(value-20) / 10]);
		value = value % 10;
		if (value == 0) {
			return(copyst(op, (ordflag) ? "tieth" : "ty"));
		}
		op = copyst(op, "ty-");
	}
	if (value <= 12) {
		return(copyst(op,
			(ordflag) ? ordinal[value] : cardinal[value]));
	}
	op = copyst(op, cardinal[value]);       /* fourteen, fourteenth */
	/*
	 * Here on 100, 14000, etc.
	 */
exit:   if (ordflag) op = copyst(op, "th");
	return(op);
}

char *
copyst(buffer, string)
char    *buffer;
char    *string;
/*
 * Copy a string into buffer.  Return the free pointer.
 */
{
	register char   *ip;
	register char   *op;

	ip = string;
	op = buffer;

	while ((*op = *ip++)) op++;
	return (op);
}
