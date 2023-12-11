/*     RELEASE DATE OF THIS VERSION:   JUNE 22, 1996. */
/* ---------------------------------------------------------------------- */

/*  WITH TWO CORRECTIONS, SUGGESTED BY T.SOTIRELIS' COMMENTS (APR.7, 1997) */

/*  (1) A "STRAY "  CLOSING PARENTHESIS WAS REMOVED IN THE S/R   R2_BIRK */
/*  (2) A 0/0 PROBLEM ON THE Z-AXIS WAS SIDESTEPPED (LINES 44-46 OF THE */
/*       DOUBLE PRECISION FUNCTION XKSI) */
/* -------------------------------------------------------------------- */
/* DATA-BASED MODEL CALIBRATED BY (1) SOLAR WIND PRESSURE PDYN (NANOPASCALS), */
/*           (2) DST (NANOTESLA),  (3) BYIMF, AND (4) BZIMF (NANOTESLA). */
/* THESE INPUT PARAMETERS SHOULD BE PLACED IN THE FIRST 4 ELEMENTS */
/* OF THE ARRAY PARMOD(10). */

/*   THE REST OF THE INPUT VARIABLES ARE: THE GEODIPOLE TILT ANGLE PS (RADIANS), */
/* AND   X,Y,Z -  GSM POSITION (RE) */

/*   IOPT  IS JUST A DUMMY INPUT PARAMETER, NECESSARY TO MAKE THIS SUBROUTINE */
/* COMPATIBLE WITH THE NEW RELEASE (APRIL 1996) OF THE TRACING SOFTWARE */
/* PACKAGE (GEOPACK). IOPT VALUE DOES NOT AFFECT THE OUTPUT FIELD. */


/* OUTPUT:  GSM COMPONENTS OF THE EXTERNAL MAGNETIC FIELD (BX,BY,BZ, nanotesla) */
/*            COMPUTED AS A SUM OF CONTRIBUTIONS FROM PRINCIPAL FIELD SOURCES */

/*  (C) Copr. 1995, 1996, Nikolai A. Tsyganenko, Raytheon STX, Code 695, NASA GSFC */
/*      Greenbelt, MD 20771, USA */

/*                            REFERENCES: */

/*               (1) N.A. TSYGANENKO AND D.P. STERN, A NEW-GENERATION GLOBAL */
/*           MAGNETOSPHERE FIELD MODEL  , BASED ON SPACECRAFT MAGNETOMETER DATA, */
/*           ISTP NEWSLETTER, V.6, NO.1, P.21, FEB.1996. */

/*              (2) N.A.TSYGANENKO,  MODELING THE EARTH'S MAGNETOSPHERIC */
/*           MAGNETIC FIELD CONFINED WITHIN A REALISTIC MAGNETOPAUSE, */
/*           J.GEOPHYS.RES., V.100, P. 5599, 1995. */

/*              (3) N.A. TSYGANENKO AND M.PEREDO, ANALYTICAL MODELS OF THE */
/*           MAGNETIC FIELD OF DISK-SHAPED CURRENT SHEETS, J.GEOPHYS.RES., */
/*           V.99, P. 199, 1994. */

/* ---------------------------------------------------------------------- */


extern "C" { void t96_01__(integer *iopt, real *parmod, real *ps, real *x, 
						   real *y, real *z__, real *bx, real *by, real *bz); }