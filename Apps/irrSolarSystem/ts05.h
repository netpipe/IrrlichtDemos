/*  ASSEMBLED:  MARCH 25, 2004; */
/*  UPDATED:  AUGUST 2 & 31, DECEMBER 27, 2004. */
/*  LATEST MODIFICATIONS/BUGS REMOVED: */

/*  (1) MARCH 14, 2005:  79 -> 69  (LINE 94; might cause compilation problems with some Fortran compilers) */

/*  (2) JUNE 24, 2006:  REPLACED COEFFICIENTS IN */
/*       (i)   DATA statement in FUNCTION AP, */
/*       (ii)  DATA C_SY statement in SUBROUTINE FULL_RC, and */
/*       (iii) DATA A statement in SUBROUTINE T04_s. */
/*  This correction was needed due to a bug found in the symmetric ring current module. */
/*   Its impact can be significant (up to ~20 nT) only in the innermost magnetosphere (R<=2) */
/*      and only for strongly disturbed conditions; otherwise, the change in the model field */
/*      does not exceed a few percent. */

/* -------------------------------------------------------------------- */
/*   A DATA-BASED MODEL OF THE EXTERNAL (I.E., WITHOUT EARTH'S CONTRIBUTION) PART OF THE */
/*   MAGNETOSPHERIC MAGNETIC FIELD, CALIBRATED BY */
/*    (1) SOLAR WIND PRESSURE PDYN (NANOPASCALS), */
/*    (2) DST (NANOTESLA), */
/*    (3) BYIMF, */
/*    (4) BZIMF (NANOTESLA) */
/*    (5-10)   INDICES W1 - W6, CALCULATED AS TIME INTEGRALS FROM THE BEGINNING OF A STORM */
/*               SEE THE REFERENCE (3) BELOW, FOR A DETAILED DEFINITION OF THOSE VARIABLES */

/*   THE ABOVE 10 INPUT PARAMETERS SHOULD BE PLACED IN THE ELEMENTS */
/*   OF THE ARRAY PARMOD(10). */

/*   THE REST OF THE INPUT VARIABLES ARE: THE GEODIPOLE TILT ANGLE PS (RADIANS), */
/*        X,Y,Z -  GSM POSITION (RE) */

/*   IOPT IS A DUMMY INPUT PARAMETER, INCLUDED TO MAKE THIS SUBROUTINE */
/*   COMPATIBLE WITH THE TRACING SOFTWARE PACKAGE (GEOPACK). IN THIS MODEL, */
/*   THE PARAMETER IOPT DOES NOT AFFECT THE OUTPUT FIELD. */

/* ******************************************************************************************* */
/* ** ATTENTION:  THE MODEL IS BASED ON DATA TAKEN SUNWARD FROM X=-15Re, AND HENCE BECOMES   * */
/* **              INVALID AT LARGER TAILWARD DISTANCES !!!                                  * */
/* ******************************************************************************************* */

/*   OUTPUT:  GSM COMPONENTS OF THE EXTERNAL MAGNETIC FIELD (BX,BY,BZ, nanotesla) */
/*            COMPUTED AS A SUM OF CONTRIBUTIONS FROM PRINCIPAL FIELD SOURCES */

/*  (C) Copr. 2004, Nikolai A. Tsyganenko, USRA/Code 612.3, NASA GSFC */
/*      Greenbelt, MD 20771, USA */

/*                            REFERENCES: */

/*  (1)   N. A. Tsyganenko, A new data-based model of the near magnetosphere magnetic field: */
/*       1. Mathematical structure. */
/*       2. Parameterization and fitting to observations.  JGR v. 107(A8), 1176/1179, doi:10.1029/2001JA000219
/220, 2002. */

/*  (2)  N. A. Tsyganenko, H. J. Singer, J. C. Kasper, Storm-time distortion of the */
/*           inner magnetosphere: How severe can it get ?  JGR v. 108(A5), 1209, doi:10.1029/2002JA009808, 200
3. */
/*   (3)  N. A. Tsyganenko and M. I. Sitnov, Modeling the dynamics of the inner magnetosphere during */
/*         strong geomagnetic storms, J. Geophys. Res., v. 110 (A3), A03208, doi: 10.1029/2004JA010798, 2005. 
*/
/* ---------------------------------------------------------------------- */

extern "C" {void t04_s__(integer *iopt, real *parmod, real *ps, real *x, 
						 real *y, real *z__, real *bx, real *by, real *bz); }