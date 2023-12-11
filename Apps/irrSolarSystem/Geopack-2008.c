/* Geopack-2008.f -- translated by f2c (version 20060506).
You must link the resulting object file with libf2c:
on Microsoft Windows system, link with libf2c.lib;
on Linux or Unix systems, link with .../path/to/libf2c.a -lm
or, if you install libf2c.a in a standard place, with -lf2c -lm
-- in that order, at the end of the command line, as in
cc *.o -lf2c -lm
Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

http://www.netlib.org/f2c/libf2c.zip
*/
#pragma comment(lib,"vcf2c.lib")
#include "f2c.h"

/* Common Block Declarations */

struct {
	real g[105], h__[105], rec[105];
} geopack2_;

#define geopack2_1 geopack2_

union {
	struct {
		real aa[10], sps, cps, bb[22];
	} _1;
	struct {
		real st0, ct0, sl0, cl0, ctcl, stcl, ctsl, stsl, sfi, cfi, sps, cps,
			ds3, cgst, sgst, psi, a11, a21, a31, a12, a22, a32, a13, a23,
			a33, e11, e21, e31, e12, e22, e32, e13, e23, e33;
	} _2;
	struct {
		real aaa[25], e11, e21, e31, e12, e22, e32, e13, e23, e33;
	} _3;
	struct {
		real st0, ct0, sl0, cl0, ctcl, stcl, ctsl, stsl, ab[26];
	} _4;
	struct {
		real a[13], cgst, sgst, b[19];
	} _5;
	struct {
		real a[8], sfi, cfi, b[24];
	} _6;
	struct {
		real a[10], sps, cps, b[22];
	} _7;
	struct {
		real aa[16], a11, a21, a31, a12, a22, a32, a13, a23, a33, b[9];
	} _8;
	struct {
		real a[12], ds3, bb[2], psi, cc[18];
	} _9;
	struct {
		real a[12], ds3, b[21];
	} _10;
	struct {
		real aa[12], dd, bb[21];
	} _11;
} geopack1_;

#define geopack1_1 (geopack1_._1)
#define geopack1_2 (geopack1_._2)
#define geopack1_3 (geopack1_._3)
#define geopack1_4 (geopack1_._4)
#define geopack1_5 (geopack1_._5)
#define geopack1_6 (geopack1_._6)
#define geopack1_7 (geopack1_._7)
#define geopack1_8 (geopack1_._8)
#define geopack1_9 (geopack1_._9)
#define geopack1_10 (geopack1_._10)
#define geopack1_11 (geopack1_._11)

/* Table of constant values */

static integer c_n1 = -1;
static integer c__1 = 1;
static doublereal c_b22 = 360.;
static integer c__9 = 9;
static doublereal c_b91 = -.15151515;
static real c_b92 = -1.f;
static doublereal c_b98 = .14;

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */


/*          ########################################################################## */
/*          #                                                                        # */
/*          #                             GEOPACK-2008                               # */
/*          #                     (MAIN SET OF FORTRAN CODES)                        # */
/*          #                                                                        # */
/*          #             version of 04/21/2008 with a minor correction              # */
/*          #                                                                        # */
/*          ########################################################################## */


/*  This collection of subroutines is a result of several upgrades of the original package */
/*  written by N. A. Tsyganenko in 1978-1979. */

/*  PREFATORY NOTE TO THE VERSION OF FEBRUARY 8, 2008: */

/*  To avoid inappropriate use of obsolete subroutines from earlier versions, a suffix 08 was */
/*  added to the name of each subroutine in this release. */

/*  A possibility has been added in this version to calculate vector components in the */
/*  "Geocentric Solar Wind" (GSW) coordinate system, which, to our knowledge, was first */
/*  introduced by Hones et al., Planet. Space Sci., v.34, p.889, 1986 (aka GSWM, see Appendix, */
/*  Tsyganenko et al., JGRA, v.103(A4), p.6827, 1998). The GSW system is analogous to the */
/*  standard GSM, except that its X-axis is antiparallel to the currently observed solar wind */
/*  flow vector, rather than aligned with the Earth-Sun line. The orientation of axes in the */
/*  GSW system can be uniquely defined by specifying three components (VGSEX,VGSEY,VGSEZ) of */
/*  the solar wind velocity, and in the case of a strictly radial anti-sunward flow (VGSEY= */
/*  VGSEZ=0) the GSW system becomes identical to the standard GSM, which fact was used here */
/*  to minimize the number of subroutines in the package. To that end, instead of the special */
/*  case of the GSM coordinates, this version uses a more general GSW system, and three more */
/*  input parameters are added in the subroutine RECALC_08, the observed values (VGSEX,VGSEY, */
/*  VGSEZ) of the solar wind velocity. Invoking RECALC_08 with VGSEY=VGSEZ=0 restores the */
/*  standard (sunward) orientation of the X axis, which allows one to easily convert vectors */
/*  between GSW and GSM, as well as to/from other standard and commonly used systems. For more */
/*  details, see the documentation file GEOPACK-2008.DOC. */

/*  Another modification allows users to have more control over the procedure of field line */
/*  mapping using the subroutine TRACE_08. To that end, three new input parameters were added */
/*  in that subroutine, allowing one to set (i) an upper limit, DSMAX, on the automatically */
/*  adjusted step size, (ii) a permissible step error, ERR, and (iii) maximal length, LMAX, */
/*  of arrays where field line point coordinates are stored. Minor changes were also made in */
/*  the tracing subroutine, to make it more compact and easier for understanding, and to */
/*  prevent the algorithm from making uncontrollable large number of multiple loops in some */
/*  cases with plasmoid-like field structures. */

/*  One more subroutine, named GEODGEO_08, was added to the package, allowing one to convert */
/*  geodetic coordinates of a point in space (altitude above the Earth's WGS84 ellipsoid and */
/*  geodetic latitude) to geocentric radial distance and colatitude, and vice versa. */

/*  For a complete list of modifications made earlier in previous versions, see the */
/*  documentation file GEOPACK-2008.DOC. */

/* ---------------------------------------------------------------------------------- */

/* Subroutine */ int igrf_gsw_08__(real *xgsw, real *ygsw, real *zgsw, real *
								   hxgsw, real *hygsw, real *hzgsw)
{
	/* System generated locals */
	integer i__1, i__2;
	real r__1, r__2;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	extern /* Subroutine */ int geogsw_08__(real *, real *, real *, real *,
		real *, real *, integer *);
	static real a[14], b[14], c__, d__, e;
	static integer k, m, n;
	static real p, q, r__, s, w, x, y, z__, d2, p2, bf, cf, bi, he, an, hh,
		dp, br, bt, sf;
	static integer mm, nm, mn;
	static real pm, pp, qq, xk, bbf, bbr, bbt, rho, rho2;
	static integer irp3;
	static real xgeo, ygeo, zgeo, hxgeo, hygeo, hzgeo;


	/*  CALCULATES COMPONENTS OF THE MAIN (INTERNAL) GEOMAGNETIC FIELD IN THE GEOCENTRIC SOLAR-WIND */
	/*  (GSW) COORDINATE SYSTEM, USING IAGA INTERNATIONAL GEOMAGNETIC REFERENCE MODEL COEFFICIENTS */
	/*  (e.g., http://www.ngdc.noaa.gov/IAGA/vmod/igrf.html, revised 22 March, 2005) */

	/*  THE GSW SYSTEM IS ESSENTIALLY SIMILAR TO THE STANDARD GSM (THE TWO SYSTEMS BECOME IDENTICAL */
	/*  TO EACH OTHER IN THE CASE OF STRICTLY ANTI-SUNWARD SOLAR WIND FLOW). FOR A DETAILED */
	/*  DEFINITION, SEE INTRODUCTORY COMMENTS FOR THE SUBROUTINE GSWGSE_08 . */

	/*  BEFORE THE FIRST CALL OF THIS SUBROUTINE, OR, IF THE DATE/TIME (IYEAR,IDAY,IHOUR,MIN,ISEC), */
	/*  OR THE SOLAR WIND VELOCITY COMPONENTS (VGSEX,VGSEY,VGSEZ) HAVE CHANGED, THE MODEL COEFFICIENTS */
	/*  AND GEO-GSW ROTATION MATRIX ELEMENTS SHOULD BE UPDATED BY CALLING THE SUBROUTINE RECALC_08. */

	/* -----INPUT PARAMETERS: */

	/*     XGSW,YGSW,ZGSW - CARTESIAN GEOCENTRIC SOLAR-WIND COORDINATES (IN UNITS RE=6371.2 KM) */

	/* -----OUTPUT PARAMETERS: */

	/*     HXGSW,HYGSW,HZGSW - CARTESIAN GEOCENTRIC SOLAR-WIND COMPONENTS OF THE MAIN GEOMAGNETIC */
	/*                           FIELD IN NANOTESLA */

	/*     LAST MODIFICATION:  FEB 07, 2008. */
	/*     THIS VERSION OF THE CODE ACCEPTS DATES FROM 1965 THROUGH 2010. */

	/*     AUTHOR: N. A. TSYGANENKO */


	geogsw_08__(&xgeo, &ygeo, &zgeo, xgsw, ygsw, zgsw, &c_n1);
	/* Computing 2nd power */
	r__1 = xgeo;
	/* Computing 2nd power */
	r__2 = ygeo;
	rho2 = r__1 * r__1 + r__2 * r__2;
	/* Computing 2nd power */
	r__1 = zgeo;
	r__ = sqrt(rho2 + r__1 * r__1);
	c__ = zgeo / r__;
	rho = sqrt(rho2);
	s = rho / r__;
	if (s < 1e-5f) {
		cf = 1.f;
		sf = 0.f;
	} else {
		cf = xgeo / rho;
		sf = ygeo / rho;
	}

	pp = 1.f / r__;
	p = pp;

	/*  IN THIS VERSION, THE OPTIMAL VALUE OF THE PARAMETER NM (MAXIMAL ORDER OF THE SPHERICAL */
	/*    HARMONIC EXPANSION) IS NOT USER-PRESCRIBED, BUT CALCULATED INSIDE THE SUBROUTINE, BASED */
	/*      ON THE VALUE OF THE RADIAL DISTANCE R: */

	irp3 = r__ + 2;
	nm = 30 / irp3 + 3;
	if (nm > 13) {
		nm = 13;
	}
	k = nm + 1;
	i__1 = k;
	for (n = 1; n <= i__1; ++n) {
		p *= pp;
		a[n - 1] = p;
		/* L150: */
		b[n - 1] = p * n;
	}
	p = 1.f;
	d__ = 0.f;
	bbr = 0.f;
	bbt = 0.f;
	bbf = 0.f;
	i__1 = k;
	for (m = 1; m <= i__1; ++m) {
		if (m == 1) {
			goto L160;
		}
		mm = m - 1;
		w = x;
		x = w * cf + y * sf;
		y = y * cf - w * sf;
		goto L170;
L160:
		x = 0.f;
		y = 1.f;
L170:
		q = p;
		z__ = d__;
		bi = 0.f;
		p2 = 0.f;
		d2 = 0.f;
		i__2 = k;
		for (n = m; n <= i__2; ++n) {
			an = a[n - 1];
			mn = n * (n - 1) / 2 + m;
			e = geopack2_1.g[mn - 1];
			hh = geopack2_1.h__[mn - 1];
			w = e * y + hh * x;
			bbr += b[n - 1] * w * q;
			bbt -= an * w * z__;
			if (m == 1) {
				goto L180;
			}
			qq = q;
			if (s < 1e-5f) {
				qq = z__;
			}
			bi += an * (e * x - hh * y) * qq;
L180:
			xk = geopack2_1.rec[mn - 1];
			dp = c__ * z__ - s * q - xk * d2;
			pm = c__ * q - xk * p2;
			d2 = z__;
			p2 = q;
			z__ = dp;
			/* L190: */
			q = pm;
		}
		d__ = s * d__ + c__ * p;
		p = s * p;
		if (m == 1) {
			goto L200;
		}
		bi *= mm;
		bbf += bi;
L200:
		;
	}

	br = bbr;
	bt = bbt;
	if (s < 1e-5f) {
		goto L210;
	}
	bf = bbf / s;
	goto L211;
L210:
	if (c__ < 0.f) {
		bbf = -bbf;
	}
	bf = bbf;
L211:
	he = br * s + bt * c__;
	hxgeo = he * cf - bf * sf;
	hygeo = he * sf + bf * cf;
	hzgeo = br * c__ - bt * s;

	geogsw_08__(&hxgeo, &hygeo, &hzgeo, hxgsw, hygsw, hzgsw, &c__1);

	return 0;
} /* igrf_gsw_08__ */


/* ========================================================================================== */


/* Subroutine */ int igrf_geo_08__(real *r__, real *theta, real *phi, real *
								   br, real *btheta, real *bphi)
{
	/* System generated locals */
	integer i__1, i__2;

	/* Builtin functions */
	double cos(doublereal), sin(doublereal);

	/* Local variables */
	static real a[14], b[14], c__, d__, e;
	static integer k, m, n;
	static real p, q, s, w, x, y, z__, d2, p2, cf, bi, an, hh, dp, sf;
	static integer mm, nm, mn;
	static real pm, pp, qq, xk, bbf, bbr, bbt;
	static integer irp3;


	/*  CALCULATES COMPONENTS OF THE MAIN (INTERNAL) GEOMAGNETIC FIELD IN THE SPHERICAL GEOGRAPHIC */
	/*  (GEOCENTRIC) COORDINATE SYSTEM, USING IAGA INTERNATIONAL GEOMAGNETIC REFERENCE MODEL */
	/*  COEFFICIENTS  (e.g., http://www.ngdc.noaa.gov/IAGA/vmod/igrf.html, revised: 22 March, 2005) */

	/*  BEFORE THE FIRST CALL OF THIS SUBROUTINE, OR IF THE DATE (IYEAR AND IDAY) WAS CHANGED, */
	/*  THE MODEL COEFFICIENTS SHOULD BE UPDATED BY CALLING THE SUBROUTINE RECALC_08 */

	/* -----INPUT PARAMETERS: */

	/*   R, THETA, PHI - SPHERICAL GEOGRAPHIC (GEOCENTRIC) COORDINATES: */
	/*   RADIAL DISTANCE R IN UNITS RE=6371.2 KM, COLATITUDE THETA AND LONGITUDE PHI IN RADIANS */

	/* -----OUTPUT PARAMETERS: */

	/*     BR, BTHETA, BPHI - SPHERICAL COMPONENTS OF THE MAIN GEOMAGNETIC FIELD IN NANOTESLA */
	/*      (POSITIVE BR OUTWARD, BTHETA SOUTHWARD, BPHI EASTWARD) */

	/*     LAST MODIFICATION:  MAY 4, 2005. */
	/*     THIS VERSION OF THE  CODE ACCEPTS DATES FROM 1965 THROUGH 2010. */

	/*     AUTHOR: N. A. TSYGANENKO */


	c__ = cos(*theta);
	s = sin(*theta);
	cf = cos(*phi);
	sf = sin(*phi);

	pp = 1.f / *r__;
	p = pp;

	/*  IN THIS NEW VERSION, THE OPTIMAL VALUE OF THE PARAMETER NM (MAXIMAL ORDER OF THE SPHERICAL */
	/*    HARMONIC EXPANSION) IS NOT USER-PRESCRIBED, BUT CALCULATED INSIDE THE SUBROUTINE, BASED */
	/*      ON THE VALUE OF THE RADIAL DISTANCE R: */

	irp3 = *r__ + 2;
	nm = 30 / irp3 + 3;
	if (nm > 13) {
		nm = 13;
	}
	k = nm + 1;
	i__1 = k;
	for (n = 1; n <= i__1; ++n) {
		p *= pp;
		a[n - 1] = p;
		/* L150: */
		b[n - 1] = p * n;
	}
	p = 1.f;
	d__ = 0.f;
	bbr = 0.f;
	bbt = 0.f;
	bbf = 0.f;
	i__1 = k;
	for (m = 1; m <= i__1; ++m) {
		if (m == 1) {
			goto L160;
		}
		mm = m - 1;
		w = x;
		x = w * cf + y * sf;
		y = y * cf - w * sf;
		goto L170;
L160:
		x = 0.f;
		y = 1.f;
L170:
		q = p;
		z__ = d__;
		bi = 0.f;
		p2 = 0.f;
		d2 = 0.f;
		i__2 = k;
		for (n = m; n <= i__2; ++n) {
			an = a[n - 1];
			mn = n * (n - 1) / 2 + m;
			e = geopack2_1.g[mn - 1];
			hh = geopack2_1.h__[mn - 1];
			w = e * y + hh * x;
			bbr += b[n - 1] * w * q;
			bbt -= an * w * z__;
			if (m == 1) {
				goto L180;
			}
			qq = q;
			if (s < 1e-5f) {
				qq = z__;
			}
			bi += an * (e * x - hh * y) * qq;
L180:
			xk = geopack2_1.rec[mn - 1];
			dp = c__ * z__ - s * q - xk * d2;
			pm = c__ * q - xk * p2;
			d2 = z__;
			p2 = q;
			z__ = dp;
			/* L190: */
			q = pm;
		}
		d__ = s * d__ + c__ * p;
		p = s * p;
		if (m == 1) {
			goto L200;
		}
		bi *= mm;
		bbf += bi;
L200:
		;
	}

	*br = bbr;
	*btheta = bbt;
	if (s < 1e-5f) {
		goto L210;
	}
	*bphi = bbf / s;
	return 0;
L210:
	if (c__ < 0.f) {
		bbf = -bbf;
	}
	*bphi = bbf;
	return 0;
} /* igrf_geo_08__ */


/* ========================================================================================== */

/* Subroutine */ int dip_08__(real *xgsw, real *ygsw, real *zgsw, real *bxgsw,
							  real *bygsw, real *bzgsw)
{
	/* System generated locals */
	real r__1, r__2, r__3;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	static real p, q, t, u, v, dipmom;


	/*  CALCULATES GSW (GEOCENTRIC SOLAR-WIND) COMPONENTS OF GEODIPOLE FIELD WITH THE DIPOLE MOMENT */
	/*  CORRESPONDING TO THE EPOCH, SPECIFIED BY CALLING SUBROUTINE RECALC_08 (SHOULD BE */
	/*  INVOKED BEFORE THE FIRST USE OF THIS ONE, OR IF THE DATE/TIME, AND/OR THE OBSERVED */
	/*  SOLAR WIND DIRECTION, HAVE CHANGED. */

	/*  THE GSW COORDINATE SYSTEM IS ESSENTIALLY SIMILAR TO THE STANDARD GSM (THE TWO SYSTEMS BECOME */
	/*  IDENTICAL TO EACH OTHER IN THE CASE OF STRICTLY RADIAL ANTI-SUNWARD SOLAR WIND FLOW). ITS */
	/*  DETAILED DEFINITION IS GIVEN IN INTRODUCTORY COMMENTS FOR THE SUBROUTINE GSWGSE_08 . */
	/* --INPUT PARAMETERS: XGSW,YGSW,ZGSW - GSW COORDINATES IN RE (1 RE = 6371.2 km) */

	/* --OUTPUT PARAMETERS: BXGSW,BYGSW,BZGSW - FIELD COMPONENTS IN GSW SYSTEM, IN NANOTESLA. */

	/*  LAST MODIFICATION: JAN 28, 2008. */

	/*  AUTHOR: N. A. TSYGANENKO */


	/* Computing 2nd power */
	r__1 = geopack2_1.g[1];
	/* Computing 2nd power */
	r__2 = geopack2_1.g[2];
	/* Computing 2nd power */
	r__3 = geopack2_1.h__[2];
	dipmom = sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3);

	/* Computing 2nd power */
	r__1 = *xgsw;
	p = r__1 * r__1;
	/* Computing 2nd power */
	r__1 = *zgsw;
	u = r__1 * r__1;
	v = *zgsw * 3.f * *xgsw;
	/* Computing 2nd power */
	r__1 = *ygsw;
	t = r__1 * r__1;
	/* Computing 5th power */
	r__1 = sqrt(p + t + u), r__2 = r__1, r__1 *= r__1;
	q = dipmom / (r__2 * (r__1 * r__1));
	*bxgsw = q * ((t + u - p * 2.f) * geopack1_1.sps - v * geopack1_1.cps);
	*bygsw = *ygsw * -3.f * q * (*xgsw * geopack1_1.sps + *zgsw *
		geopack1_1.cps);
	*bzgsw = q * ((p + t - u * 2.f) * geopack1_1.cps - v * geopack1_1.sps);
	return 0;
} /* dip_08__ */

/* ******************************************************************* */

/* Subroutine */ int sun_08__(integer *iyear, integer *iday, integer *ihour,
							  integer *min__, integer *isec, real *gst, real *slong, real *srasn,
							  real *sdec)
{
	/* Initialized data */

	static real rad = 57.295779513f;

	/* System generated locals */
	real r__1;
	doublereal d__1;

	/* Builtin functions */
	double d_mod(doublereal *, doublereal *), sin(doublereal), sqrt(
		doublereal), atan(doublereal), cos(doublereal), atan2(doublereal,
		doublereal);

	/* Local variables */
	static real g, t;
	static doublereal dj;
	static real sc, vl, sob, slp;
	static doublereal fday;
	static real cosd, sind, obliq;


	/*  CALCULATES FOUR QUANTITIES NECESSARY FOR COORDINATE TRANSFORMATIONS */
	/*  WHICH DEPEND ON SUN POSITION (AND, HENCE, ON UNIVERSAL TIME AND SEASON) */

	/* -------  INPUT PARAMETERS: */
	/*  IYR,IDAY,IHOUR,MIN,ISEC -  YEAR, DAY, AND UNIVERSAL TIME IN HOURS, MINUTES, */
	/*    AND SECONDS  (IDAY=1 CORRESPONDS TO JANUARY 1). */

	/* -------  OUTPUT PARAMETERS: */
	/*  GST - GREENWICH MEAN SIDEREAL TIME, SLONG - LONGITUDE ALONG ECLIPTIC */
	/*  SRASN - RIGHT ASCENSION,  SDEC - DECLINATION  OF THE SUN (RADIANS) */
	/*  ORIGINAL VERSION OF THIS SUBROUTINE HAS BEEN COMPILED FROM: */
	/*  RUSSELL, C.T., COSMIC ELECTRODYNAMICS, 1971, V.2, PP.184-196. */

	/*  LAST MODIFICATION:  MARCH 31, 2003 (ONLY SOME NOTATION CHANGES) */

	/*     ORIGINAL VERSION WRITTEN BY:    Gilbert D. Mead */


	if (*iyear < 1901 || *iyear > 2099) {
		return 0;
	}
	fday = (doublereal) (*ihour * 3600 + *min__ * 60 + *isec) / 86400.;
	dj = (*iyear - 1900) * 365 + (*iyear - 1901) / 4 + *iday - .5 + fday;
	t = dj / 36525.f;
	d__1 = dj * .9856473354f + 279.696678f;
	vl = d_mod(&d__1, &c_b22);
	d__1 = dj * .9856473354f + 279.690983f + fday * 360.f + 180.f;
	*gst = d_mod(&d__1, &c_b22) / rad;
	d__1 = dj * .985600267f + 358.475845f;
	g = d_mod(&d__1, &c_b22) / rad;
	*slong = (vl + (1.91946f - t * .004789f) * sin(g) + sin(g * 2.f) *
		.020094f) / rad;
	if (*slong > 6.2831853f) {
		*slong += -6.2831853f;
	}
	if (*slong < 0.f) {
		*slong += 6.2831853f;
	}
	obliq = (23.45229f - t * .0130125f) / rad;
	sob = sin(obliq);
	slp = *slong - 9.924e-5f;

	/*   THE LAST CONSTANT IS A CORRECTION FOR THE ANGULAR ABERRATION DUE TO */
	/*   EARTH'S ORBITAL MOTION */

	sind = sob * sin(slp);
	/* Computing 2nd power */
	r__1 = sind;
	cosd = sqrt(1.f - r__1 * r__1);
	sc = sind / cosd;
	*sdec = atan(sc);
	*srasn = 3.141592654f - atan2(cos(obliq) / sob * sc, -cos(slp) / cosd);
	return 0;
} /* sun_08__ */


/* ================================================================================ */

/* Subroutine */ int sphcar_08__(real *r__, real *theta, real *phi, real *x,
								 real *y, real *z__, integer *j)
{
	/* System generated locals */
	real r__1, r__2;

	/* Builtin functions */
	double sqrt(doublereal), atan2(doublereal, doublereal), sin(doublereal),
		cos(doublereal);

	/* Local variables */
	static real sq;


	/*   CONVERTS SPHERICAL COORDS INTO CARTESIAN ONES AND VICE VERSA */
	/*    (THETA AND PHI IN RADIANS). */

	/*                  J>0            J<0 */
	/* -----INPUT:   J,R,THETA,PHI     J,X,Y,Z */
	/* ----OUTPUT:      X,Y,Z        R,THETA,PHI */

	/*  NOTE: AT THE POLES (X=0 AND Y=0) WE ASSUME PHI=0 WHEN CONVERTING */
	/*        FROM CARTESIAN TO SPHERICAL COORDS (I.E., FOR J<0) */

	/*   LAST MOFIFICATION:  APRIL 1, 2003 (ONLY SOME NOTATION CHANGES AND MORE */
	/*                         COMMENTS ADDED) */

	/*   AUTHOR:  N. A. TSYGANENKO */

	if (*j > 0) {
		goto L3;
	}
	/* Computing 2nd power */
	r__1 = *x;
	/* Computing 2nd power */
	r__2 = *y;
	sq = r__1 * r__1 + r__2 * r__2;
	/* Computing 2nd power */
	r__1 = *z__;
	*r__ = sqrt(sq + r__1 * r__1);
	if (sq != 0.f) {
		goto L2;
	}
	*phi = 0.f;
	if (*z__ < 0.f) {
		goto L1;
	}
	*theta = 0.f;
	return 0;
L1:
	*theta = 3.141592654f;
	return 0;
L2:
	sq = sqrt(sq);
	*phi = atan2(*y, *x);
	*theta = atan2(sq, *z__);
	if (*phi < 0.f) {
		*phi += 6.28318531f;
	}
	return 0;
L3:
	sq = *r__ * sin(*theta);
	*x = sq * cos(*phi);
	*y = sq * sin(*phi);
	*z__ = *r__ * cos(*theta);
	return 0;
} /* sphcar_08__ */


/* =========================================================================== */

/* Subroutine */ int bspcar_08__(real *theta, real *phi, real *br, real *
								 btheta, real *bphi, real *bx, real *by, real *bz)
{
	/* Builtin functions */
	double sin(doublereal), cos(doublereal);

	/* Local variables */
	static real c__, s, be, cf, sf;


	/*   CALCULATES CARTESIAN FIELD COMPONENTS FROM LOCAL SPHERICAL ONES */

	/* -----INPUT:   THETA,PHI - SPHERICAL ANGLES OF THE POINT IN RADIANS */
	/*              BR,BTHETA,BPHI -  LOCAL SPHERICAL COMPONENTS OF THE FIELD */
	/* -----OUTPUT:  BX,BY,BZ - CARTESIAN COMPONENTS OF THE FIELD */

	/*   LAST MOFIFICATION:  APRIL 1, 2003 (ONLY SOME NOTATION CHANGES) */

	/*   WRITTEN BY:  N. A. TSYGANENKO */

	s = sin(*theta);
	c__ = cos(*theta);
	sf = sin(*phi);
	cf = cos(*phi);
	be = *br * s + *btheta * c__;
	*bx = be * cf - *bphi * sf;
	*by = be * sf + *bphi * cf;
	*bz = *br * c__ - *btheta * s;
	return 0;
} /* bspcar_08__ */


/* ============================================================================== */

/* Subroutine */ int bcarsp_08__(real *x, real *y, real *z__, real *bx, real *
								 by, real *bz, real *br, real *btheta, real *bphi)
{
	/* System generated locals */
	real r__1, r__2;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	static real r__, ct, st, rho, rho2, cphi, sphi;


	/* ALCULATES LOCAL SPHERICAL FIELD COMPONENTS FROM THOSE IN CARTESIAN SYSTEM */

	/* -----INPUT:   X,Y,Z  - CARTESIAN COMPONENTS OF THE POSITION VECTOR */
	/*              BX,BY,BZ - CARTESIAN COMPONENTS OF THE FIELD VECTOR */
	/* -----OUTPUT:  BR,BTHETA,BPHI - LOCAL SPHERICAL COMPONENTS OF THE FIELD VECTOR */

	/*  NOTE: AT THE POLES (THETA=0 OR THETA=PI) WE ASSUME PHI=0, */
	/*        AND HENCE BTHETA=BX, BPHI=BY */

	/*   WRITTEN AND ADDED TO THIS PACKAGE:  APRIL 1, 2003, */
	/*   AUTHOR:   N. A. TSYGANENKO */

	/* Computing 2nd power */
	r__1 = *x;
	/* Computing 2nd power */
	r__2 = *y;
	rho2 = r__1 * r__1 + r__2 * r__2;
	/* Computing 2nd power */
	r__1 = *z__;
	r__ = sqrt(rho2 + r__1 * r__1);
	rho = sqrt(rho2);
	if (rho != 0.f) {
		cphi = *x / rho;
		sphi = *y / rho;
	} else {
		cphi = 1.f;
		sphi = 0.f;
	}
	ct = *z__ / r__;
	st = rho / r__;
	*br = (*x * *bx + *y * *by + *z__ * *bz) / r__;
	*btheta = (*bx * cphi + *by * sphi) * ct - *bz * st;
	*bphi = *by * cphi - *bx * sphi;
	return 0;
} /* bcarsp_08__ */


/* ===================================================================================== */
/* Subroutine */ int recalc_08__(integer *iyear, integer *iday, integer *ihour,
								 integer *min__, integer *isec,
								 real *vgsex, real *vgsey, real *vgsez)
{
	/* Initialized data */

	static integer isw = 0;
	static real g65[105] = { 0.f,-30334.f,-2119.f,-1662.f,2997.f,1594.f,
		1297.f,-2038.f,1292.f,856.f,957.f,804.f,479.f,-390.f,252.f,-219.f,
		358.f,254.f,-31.f,-157.f,-62.f,45.f,61.f,8.f,-228.f,4.f,1.f,
		-111.f,75.f,-57.f,4.f,13.f,-26.f,-6.f,13.f,1.f,13.f,5.f,-4.f,
		-14.f,0.f,8.f,-1.f,11.f,4.f,8.f,10.f,2.f,-13.f,10.f,-1.f,-1.f,5.f,
		1.f,-2.f,-2.f,-3.f,2.f,-5.f,-2.f,4.f,4.f,0.f,2.f,2.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f };
	static real h65[105] = { 0.f,0.f,5776.f,0.f,-2016.f,114.f,0.f,-404.f,
		240.f,-165.f,0.f,148.f,-269.f,13.f,-269.f,0.f,19.f,128.f,-126.f,
		-97.f,81.f,0.f,-11.f,100.f,68.f,-32.f,-8.f,-7.f,0.f,-61.f,-27.f,
		-2.f,6.f,26.f,-23.f,-12.f,0.f,7.f,-12.f,9.f,-16.f,4.f,24.f,-3.f,
		-17.f,0.f,-22.f,15.f,7.f,-4.f,-5.f,10.f,10.f,-4.f,1.f,0.f,2.f,1.f,
		2.f,6.f,-4.f,0.f,-2.f,3.f,0.f,-6.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };
	static real g70[105] = { 0.f,-30220.f,-2068.f,-1781.f,3e3f,1611.f,1287.f,
		-2091.f,1278.f,838.f,952.f,800.f,461.f,-395.f,234.f,-216.f,359.f,
		262.f,-42.f,-160.f,-56.f,43.f,64.f,15.f,-212.f,2.f,3.f,-112.f,
		72.f,-57.f,1.f,14.f,-22.f,-2.f,13.f,-2.f,14.f,6.f,-2.f,-13.f,-3.f,
		5.f,0.f,11.f,3.f,8.f,10.f,2.f,-12.f,10.f,-1.f,0.f,3.f,1.f,-1.f,
		-3.f,-3.f,2.f,-5.f,-1.f,6.f,4.f,1.f,0.f,3.f,-1.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f };
	static real h70[105] = { 0.f,0.f,5737.f,0.f,-2047.f,25.f,0.f,-366.f,251.f,
		-196.f,0.f,167.f,-266.f,26.f,-279.f,0.f,26.f,139.f,-139.f,-91.f,
		83.f,0.f,-12.f,100.f,72.f,-37.f,-6.f,1.f,0.f,-70.f,-27.f,-4.f,8.f,
		23.f,-23.f,-11.f,0.f,7.f,-15.f,6.f,-17.f,6.f,21.f,-6.f,-16.f,0.f,
		-21.f,16.f,6.f,-4.f,-5.f,10.f,11.f,-2.f,1.f,0.f,1.f,1.f,3.f,4.f,
		-4.f,0.f,-1.f,3.f,1.f,-4.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };
	static real g75[105] = { 0.f,-30100.f,-2013.f,-1902.f,3010.f,1632.f,
		1276.f,-2144.f,1260.f,830.f,946.f,791.f,438.f,-405.f,216.f,-218.f,
		356.f,264.f,-59.f,-159.f,-49.f,45.f,66.f,28.f,-198.f,1.f,6.f,
		-111.f,71.f,-56.f,1.f,16.f,-14.f,0.f,12.f,-5.f,14.f,6.f,-1.f,
		-12.f,-8.f,4.f,0.f,10.f,1.f,7.f,10.f,2.f,-12.f,10.f,-1.f,-1.f,4.f,
		1.f,-2.f,-3.f,-3.f,2.f,-5.f,-2.f,5.f,4.f,1.f,0.f,3.f,-1.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f };
	static real h75[105] = { 0.f,0.f,5675.f,0.f,-2067.f,-68.f,0.f,-333.f,
		262.f,-223.f,0.f,191.f,-265.f,39.f,-288.f,0.f,31.f,148.f,-152.f,
		-83.f,88.f,0.f,-13.f,99.f,75.f,-41.f,-4.f,11.f,0.f,-77.f,-26.f,
		-5.f,10.f,22.f,-23.f,-12.f,0.f,6.f,-16.f,4.f,-19.f,6.f,18.f,-10.f,
		-17.f,0.f,-21.f,16.f,7.f,-4.f,-5.f,10.f,11.f,-3.f,1.f,0.f,1.f,1.f,
		3.f,4.f,-4.f,-1.f,-1.f,3.f,1.f,-5.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };
	static real g80[105] = { 0.f,-29992.f,-1956.f,-1997.f,3027.f,1663.f,
		1281.f,-2180.f,1251.f,833.f,938.f,782.f,398.f,-419.f,199.f,-218.f,
		357.f,261.f,-74.f,-162.f,-48.f,48.f,66.f,42.f,-192.f,4.f,14.f,
		-108.f,72.f,-59.f,2.f,21.f,-12.f,1.f,11.f,-2.f,18.f,6.f,0.f,-11.f,
		-7.f,4.f,3.f,6.f,-1.f,5.f,10.f,1.f,-12.f,9.f,-3.f,-1.f,7.f,2.f,
		-5.f,-4.f,-4.f,2.f,-5.f,-2.f,5.f,3.f,1.f,2.f,3.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f };
	static real h80[105] = { 0.f,0.f,5604.f,0.f,-2129.f,-200.f,0.f,-336.f,
		271.f,-252.f,0.f,212.f,-257.f,53.f,-297.f,0.f,46.f,150.f,-151.f,
		-78.f,92.f,0.f,-15.f,93.f,71.f,-43.f,-2.f,17.f,0.f,-82.f,-27.f,
		-5.f,16.f,18.f,-23.f,-10.f,0.f,7.f,-18.f,4.f,-22.f,9.f,16.f,-13.f,
		-15.f,0.f,-21.f,16.f,9.f,-5.f,-6.f,9.f,10.f,-6.f,2.f,0.f,1.f,0.f,
		3.f,6.f,-4.f,0.f,-1.f,4.f,0.f,-6.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };
	static real g85[105] = { 0.f,-29873.f,-1905.f,-2072.f,3044.f,1687.f,
		1296.f,-2208.f,1247.f,829.f,936.f,780.f,361.f,-424.f,170.f,-214.f,
		355.f,253.f,-93.f,-164.f,-46.f,53.f,65.f,51.f,-185.f,4.f,16.f,
		-102.f,74.f,-62.f,3.f,24.f,-6.f,4.f,10.f,0.f,21.f,6.f,0.f,-11.f,
		-9.f,4.f,4.f,4.f,-4.f,5.f,10.f,1.f,-12.f,9.f,-3.f,-1.f,7.f,1.f,
		-5.f,-4.f,-4.f,3.f,-5.f,-2.f,5.f,3.f,1.f,2.f,3.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f };
	static real h85[105] = { 0.f,0.f,5500.f,0.f,-2197.f,-306.f,0.f,-310.f,
		284.f,-297.f,0.f,232.f,-249.f,69.f,-297.f,0.f,47.f,150.f,-154.f,
		-75.f,95.f,0.f,-16.f,88.f,69.f,-48.f,-1.f,21.f,0.f,-83.f,-27.f,
		-2.f,20.f,17.f,-23.f,-7.f,0.f,8.f,-19.f,5.f,-23.f,11.f,14.f,-15.f,
		-11.f,0.f,-21.f,15.f,9.f,-6.f,-6.f,9.f,9.f,-7.f,2.f,0.f,1.f,0.f,
		3.f,6.f,-4.f,0.f,-1.f,4.f,0.f,-6.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };
	static real g90[105] = { 0.f,-29775.f,-1848.f,-2131.f,3059.f,1686.f,
		1314.f,-2239.f,1248.f,802.f,939.f,780.f,325.f,-423.f,141.f,-214.f,
		353.f,245.f,-109.f,-165.f,-36.f,61.f,65.f,59.f,-178.f,3.f,18.f,
		-96.f,77.f,-64.f,2.f,26.f,-1.f,5.f,9.f,0.f,23.f,5.f,-1.f,-10.f,
		-12.f,3.f,4.f,2.f,-6.f,4.f,9.f,1.f,-12.f,9.f,-4.f,-2.f,7.f,1.f,
		-6.f,-3.f,-4.f,2.f,-5.f,-2.f,4.f,3.f,1.f,3.f,3.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f };
	static real h90[105] = { 0.f,0.f,5406.f,0.f,-2279.f,-373.f,0.f,-284.f,
		293.f,-352.f,0.f,247.f,-240.f,84.f,-299.f,0.f,46.f,154.f,-153.f,
		-69.f,97.f,0.f,-16.f,82.f,69.f,-52.f,1.f,24.f,0.f,-80.f,-26.f,0.f,
		21.f,17.f,-23.f,-4.f,0.f,10.f,-19.f,6.f,-22.f,12.f,12.f,-16.f,
		-10.f,0.f,-20.f,15.f,11.f,-7.f,-7.f,9.f,8.f,-7.f,2.f,0.f,2.f,1.f,
		3.f,6.f,-4.f,0.f,-2.f,3.f,-1.f,-6.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };
	static real g95[105] = { 0.f,-29692.f,-1784.f,-2200.f,3070.f,1681.f,
		1335.f,-2267.f,1249.f,759.f,940.f,780.f,290.f,-418.f,122.f,-214.f,
		352.f,235.f,-118.f,-166.f,-17.f,68.f,67.f,68.f,-170.f,-1.f,19.f,
		-93.f,77.f,-72.f,1.f,28.f,5.f,4.f,8.f,-2.f,25.f,6.f,-6.f,-9.f,
		-14.f,9.f,6.f,-5.f,-7.f,4.f,9.f,3.f,-10.f,8.f,-8.f,-1.f,10.f,-2.f,
		-8.f,-3.f,-6.f,2.f,-4.f,-1.f,4.f,2.f,2.f,5.f,1.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f };
	static real h95[105] = { 0.f,0.f,5306.f,0.f,-2366.f,-413.f,0.f,-262.f,
		302.f,-427.f,0.f,262.f,-236.f,97.f,-306.f,0.f,46.f,165.f,-143.f,
		-55.f,107.f,0.f,-17.f,72.f,67.f,-58.f,1.f,36.f,0.f,-69.f,-25.f,
		4.f,24.f,17.f,-24.f,-6.f,0.f,11.f,-21.f,8.f,-23.f,15.f,11.f,-16.f,
		-4.f,0.f,-20.f,15.f,12.f,-6.f,-8.f,8.f,5.f,-8.f,3.f,0.f,1.f,0.f,
		4.f,5.f,-5.f,-1.f,-2.f,1.f,-2.f,-7.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };
	static real g00[105] = { 0.f,-29619.4f,-1728.2f,-2267.7f,3068.4f,1670.9f,
		1339.6f,-2288.f,1252.1f,714.5f,932.3f,786.8f,250.f,-403.f,111.3f,
		-218.8f,351.4f,222.3f,-130.4f,-168.6f,-12.9f,72.3f,68.2f,74.2f,
		-160.9f,-5.9f,16.9f,-90.4f,79.f,-74.f,0.f,33.3f,9.1f,6.9f,7.3f,
		-1.2f,24.4f,6.6f,-9.2f,-7.9f,-16.6f,9.1f,7.f,-7.9f,-7.f,5.f,9.4f,
		3.f,-8.4f,6.3f,-8.9f,-1.5f,9.3f,-4.3f,-8.2f,-2.6f,-6.f,1.7f,-3.1f,
		-.5f,3.7f,1.f,2.f,4.2f,.3f,-1.1f,2.7f,-1.7f,-1.9f,1.5f,-.1f,.1f,
		-.7f,.7f,1.7f,.1f,1.2f,4.f,-2.2f,-.3f,.2f,.9f,-.2f,.9f,-.5f,.3f,
		-.3f,-.4f,-.1f,-.2f,-.4f,-.2f,-.9f,.3f,.1f,-.4f,1.3f,-.4f,.7f,
		-.4f,.3f,-.1f,.4f,0.f,.1f };
	static real h00[105] = { 0.f,0.f,5186.1f,0.f,-2481.6f,-458.f,0.f,-227.6f,
		293.4f,-491.1f,0.f,272.6f,-231.9f,119.8f,-303.8f,0.f,43.8f,171.9f,
		-133.1f,-39.3f,106.3f,0.f,-17.4f,63.7f,65.1f,-61.2f,.7f,43.8f,0.f,
		-64.6f,-24.2f,6.2f,24.f,14.8f,-25.4f,-5.8f,0.f,11.9f,-21.5f,8.5f,
		-21.5f,15.5f,8.9f,-14.9f,-2.1f,0.f,-19.7f,13.4f,12.5f,-6.2f,-8.4f,
		8.4f,3.8f,-8.2f,4.8f,0.f,1.7f,0.f,4.f,4.9f,-5.9f,-1.2f,-2.9f,.2f,
		-2.2f,-7.4f,0.f,.1f,1.3f,-.9f,-2.6f,.9f,-.7f,-2.8f,-.9f,-1.2f,
		-1.9f,-.9f,0.f,-.4f,.3f,2.5f,-2.6f,.7f,.3f,0.f,0.f,.3f,-.9f,-.4f,
		.8f,0.f,-.9f,.2f,1.8f,-.4f,-1.f,-.1f,.7f,.3f,.6f,.3f,-.2f,-.5f,
		-.9f };
	static real g05[105] = { 0.f,-29556.8f,-1671.8f,-2340.5f,3047.f,1656.9f,
		1335.7f,-2305.3f,1246.8f,674.4f,919.8f,798.2f,211.5f,-379.5f,
		100.2f,-227.6f,354.4f,208.8f,-136.6f,-168.3f,-14.1f,72.9f,69.6f,
		76.6f,-151.1f,-15.f,14.7f,-86.4f,79.8f,-74.4f,-1.4f,38.6f,12.3f,
		9.4f,5.5f,2.f,24.8f,7.7f,-11.4f,-6.8f,-18.f,10.f,9.4f,-11.4f,-5.f,
		5.6f,9.8f,3.6f,-7.f,5.f,-10.8f,-1.3f,8.7f,-6.7f,-9.2f,-2.2f,-6.3f,
		1.6f,-2.5f,-.1f,3.f,.3f,2.1f,3.9f,-.1f,-2.2f,2.9f,-1.6f,-1.7f,
		1.5f,-.2f,.2f,-.7f,.5f,1.8f,.1f,1.f,4.1f,-2.2f,-.3f,.3f,.9f,-.4f,
		1.f,-.4f,.5f,-.3f,-.4f,0.f,-.4f,0.f,-.2f,-.9f,.3f,.3f,-.4f,1.2f,
		-.4f,.7f,-.3f,.4f,-.1f,.4f,-.1f,-.3f };
	static real h05[105] = { 0.f,0.f,5080.f,0.f,-2594.9f,-516.7f,0.f,-200.4f,
		269.3f,-524.5f,0.f,281.4f,-225.8f,145.7f,-304.7f,0.f,42.7f,179.8f,
		-123.f,-19.5f,103.6f,0.f,-20.2f,54.7f,63.7f,-63.4f,0.f,50.3f,0.f,
		-61.4f,-22.5f,6.9f,25.4f,10.9f,-26.4f,-4.8f,0.f,11.2f,-21.f,9.7f,
		-19.8f,16.1f,7.7f,-12.8f,-.1f,0.f,-20.1f,12.9f,12.7f,-6.7f,-8.1f,
		8.1f,2.9f,-7.9f,5.9f,0.f,2.4f,.2f,4.4f,4.7f,-6.5f,-1.f,-3.4f,-.9f,
		-2.3f,-8.f,0.f,.3f,1.4f,-.7f,-2.4f,.9f,-.6f,-2.7f,-1.f,-1.5f,-2.f,
		-1.4f,0.f,-.5f,.3f,2.3f,-2.7f,.6f,.4f,0.f,0.f,.3f,-.8f,-.4f,1.f,
		0.f,-.7f,.3f,1.7f,-.5f,-1.f,0.f,.7f,.2f,.6f,.4f,-.2f,-.5f,-1.f };
	static real dg05[45] = { 0.f,8.8f,10.8f,-15.f,-6.9f,-1.f,-.3f,-3.1f,-.9f,
		-6.8f,-2.5f,2.8f,-7.1f,5.9f,-3.2f,-2.6f,.4f,-3.f,-1.2f,.2f,-.6f,
		-.8f,.2f,-.2f,2.1f,-2.1f,-.4f,1.3f,-.4f,0.f,-.2f,1.1f,.6f,.4f,
		-.5f,.9f,-.2f,.2f,-.2f,.2f,-.2f,.2f,.5f,-.7f,.5f };
	static real dh05[45] = { 0.f,0.f,-21.3f,0.f,-23.3f,-14.f,0.f,5.4f,-6.5f,
		-2.f,0.f,2.f,1.8f,5.6f,0.f,0.f,.1f,1.8f,2.f,4.5f,-1.f,0.f,-.4f,
		-1.9f,-.4f,-.4f,-.2f,.9f,0.f,.8f,.4f,.1f,.2f,-.9f,-.3f,.3f,0.f,
		-.2f,.2f,.2f,.4f,.2f,-.3f,.5f,.4f };

	/* Format strings */
	//static char fmt_10[] = " ";
	static char fmt_10[] = "(//1x,\002**** RECALC_08 WARNS: YEAR IS OUT OF I"
		"NTERVAL 1965-2010: IYEAR=\002,i4,/,6x,\002CALCULATIONS WILL BE D"
		"ONE FOR IYEAR=\002,i4,/)";

	/* System generated locals */
	integer i__1;
	real r__1, r__2, r__3;

	/* Builtin functions */
	integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen),
		e_wsle(void), s_wsfe(cilist *), do_fio(integer *, char *, ftnlen),
		e_wsfe(void);
	double sqrt(doublereal), cos(doublereal), sin(doublereal), asin(
		doublereal);

	/* Local variables */
	static integer m, n;
	static real p, s, t, v, y, f1, f2;
	static integer n2;
	static real s1, s2, s3, x1, x2, x3, y1, y2, y3, z1, z2, z3, aa;
	static integer iy, mn;
	static real dt, g10, g11, h11, sq, dj, dx1, dy1, dz1, dz2, dz3, dy2, dy3,
		dx2, dx3;
	static integer mnn;
	static real gst, sqq, sqr, dip1, dip2, dip3, sdec, obliq;
	extern /* Subroutine */ int sun_08__(integer *, integer *, integer *,
		integer *, integer *, real *, real *, real *, real *);
	static real slong, srasn, exmagx, exmagy, exmagz, eymagx, eymagy;

	/* Fortran I/O blocks */
	static cilist io___134 = { 0, 6, 0, 0, 0 };
	static cilist io___135 = { 0, 6, 0, 0, 0 };
	static cilist io___136 = { 0, 6, 0, 0, 0 };
	static cilist io___137 = { 0, 6, 0, 0, 0 };
	static cilist io___139 = { 0, 6, 0, fmt_10, 0 };
	static cilist io___140 = { 0, 6, 0, fmt_10, 0 };



	/*  1. PREPARES ELEMENTS OF ROTATION MATRICES FOR TRANSFORMATIONS OF VECTORS BETWEEN */
	/*     SEVERAL COORDINATE SYSTEMS, MOST FREQUENTLY USED IN SPACE PHYSICS. */

	/*  2. PREPARES COEFFICIENTS USED IN THE CALCULATION OF THE MAIN GEOMAGNETIC FIELD */
	/*      (IGRF MODEL) */

	/*  THIS SUBROUTINE SHOULD BE INVOKED BEFORE USING THE FOLLOWING SUBROUTINES: */
	/*  IGRF_GEO_08, IGRF_GSW_08, DIP_08, GEOMAG_08, GEOGSW_08, MAGSW_08, SMGSW_08, GSWGSE_08, */
	/*  GEIGEO_08, TRACE_08, STEP_08, RHAND_08. */

	/*  THERE IS NO NEED TO REPEATEDLY INVOKE RECALC_08, IF MULTIPLE CALCULATIONS ARE MADE */
	/*    FOR THE SAME DATE/TIME AND SOLAR WIND FLOW DIRECTION. */

	/* -----INPUT PARAMETERS: */

	/*     IYEAR   -  YEAR NUMBER (FOUR DIGITS) */
	/*     IDAY  -  DAY OF YEAR (DAY 1 = JAN 1) */
	/*     IHOUR -  HOUR OF DAY (00 TO 23) */
	/*     MIN   -  MINUTE OF HOUR (00 TO 59) */
	/*     ISEC  -  SECONDS OF MINUTE (00 TO 59) */
	/*     VGSEX,VGSEY,VGSEZ - GSE (GEOCENTRIC SOLAR-ECLIPTIC) COMPONENTS OF THE OBSERVED */
	/*                              SOLAR WIND FLOW VELOCITY (IN KM/S) */

	/*  IMPORTANT: IF ONLY QUESTIONABLE INFORMATION (OR NO INFORMATION AT ALL) IS AVAILABLE */
	/*             ON THE SOLAR WIND SPEED, OR, IF THE STANDARD GSM AND/OR SM COORDINATES ARE */
	/*             INTENDED TO BE USED, THEN SET VGSEX=-400.0 AND VGSEY=VGSEZ=0. IN THIS CASE, */
	/*             THE GSW COORDINATE SYSTEM BECOMES IDENTICAL TO THE STANDARD GSM. */

	/*             IF ONLY SCALAR SPEED V OF THE SOLAR WIND IS KNOWN, THEN SETTING */
	/*             VGSEX=-V, VGSEY=29.78, VGSEZ=0.0 WILL TAKE INTO ACCOUNT THE ~4 degs */
	/*             ABERRATION OF THE MAGNETOSPHERE DUE TO EARTH'S ORBITAL MOTION */

	/*             IF ALL THREE GSE COMPONENTS OF THE SOLAR WIND VELOCITY ARE AVAILABLE, */
	/*             PLEASE NOTE THAT IN SOME SOLAR WIND DATABASES THE ABERRATION EFFECT */
	/*             HAS ALREADY BEEN TAKEN INTO ACCOUNT BY SUBTRACTING 29.78 KM/S FROM VYGSE; */
	/*             IN THAT CASE, THE UNABERRATED (OBSERVED) VYGSE VALUES SHOULD BE RESTORED */
	/*             BY ADDING BACK THE 29.78 KM/S CORRECTION. WHETHER OR NOT TO DO THAT, MUST */
	/*             BE EITHER VERIFIED WITH THE DATA ORIGINATOR OR DETERMINED BY AVERAGING */
	/*             VGSEY OVER A SUFFICIENTLY LONG TIME INTERVAL. */

	/* -----OUTPUT PARAMETERS:  NONE (ALL OUTPUT QUANTITIES ARE PLACED */
	/*                         INTO THE COMMON BLOCKS /GEOPACK1/ AND /GEOPACK2/) */

	/*    OTHER SUBROUTINES CALLED BY THIS ONE: SUN_08 */

	/*    AUTHOR:  N.A. TSYGANENKO */
	/*    DATE:    DEC.1, 1991 */

	/*    REVISION OF NOVEMBER 15, 2007: ADDED THE POSSIBILITY TO TAKE INTO ACCOUNT THE OBSERVED */
	/*     DEFLECTION OF THE SOLAR WIND FLOW FROM STRICTLY RADIAL DIRECTION. TO THAT END, THREE */
	/*     GSE COMPONENTS OF THE SOLAR WIND VELOCITY WERE ADDED TO THE INPUT PARAMETERS. */

	/*    CORRECTION OF MAY 9, 2006:  INTERPOLATION OF THE COEFFICIENTS (BETWEEN */
	/*     LABELS 50 AND 105) IS NOW MADE THROUGH THE LAST ELEMENT OF THE ARRAYS */
	/*     G(105)  AND H(105) (PREVIOUSLY MADE ONLY THROUGH N=66, WHICH IN SOME */
	/*     CASES CAUSED RUNTIME ERRORS) */

	/*    REVISION OF MAY 3, 2005: */
	/*     The table of IGRF coefficients was extended to include those for the epoch 2005 */
	/*       the maximal order of spherical harmonics was also increased up to 13 */
	/*         (for details, see http://www.ngdc.noaa.gov/IAGA/vmod/igrf.html) */

	/*    REVISION OF APRIL 3, 2003: */
	/*    The code now includes preparation of the model coefficients for the subroutines */
	/*    IGRF_08 and GEOMAG_08. This eliminates the need for the SAVE statements, used */
	/*    in the old versions, making the codes easier and more compiler-independent. */



	/*  THE COMMON BLOCK /GEOPACK1/ CONTAINS ELEMENTS OF THE ROTATION MATRICES AND OTHER */
	/*   PARAMETERS RELATED TO THE COORDINATE TRANSFORMATIONS PERFORMED BY THIS PACKAGE */


	/*  THE COMMON BLOCK /GEOPACK2/ CONTAINS COEFFICIENTS OF THE IGRF FIELD MODEL, CALCULATED */
	/*    FOR A GIVEN YEAR AND DAY FROM THEIR STANDARD EPOCH VALUES. THE ARRAY REC CONTAINS */
	/*    COEFFICIENTS USED IN THE RECURSION RELATIONS FOR LEGENDRE ASSOCIATE POLYNOMIALS. */


	if (*vgsey == 0.f && *vgsez == 0.f && isw != 1) {
		/*      PRINT *, '' */
		s_wsle(&io___134);
		do_lio(&c__9, &c__1, " RECALC_08: RADIAL SOLAR WIND --> GSW SYSTEM I"
			"DENTICAL HERE", (ftnlen)59);
		e_wsle();
		s_wsle(&io___135);
		do_lio(&c__9, &c__1, " TO STANDARD GSM (I.E., XGSW AXIS COINCIDES WI"
			"TH EARTH-SUN LINE)", (ftnlen)64);
		e_wsle();
		/*      PRINT *, '' */
		isw = 1;
	}
	if (*vgsey != 0.f || *vgsez != 0.f && isw != 2) {
		/*      PRINT *, '' */
		s_wsle(&io___136);
		do_lio(&c__9, &c__1, " WARNING: NON-RADIAL SOLAR WIND FLOW SPECIFIED"
			" IN RECALC_08;", (ftnlen)60);
		e_wsle();
		s_wsle(&io___137);
		do_lio(&c__9, &c__1, " HENCE XGSW AXIS IS ASSUMED ORIENTED ANTIPARAL"
			"LEL TO V_SW VECTOR", (ftnlen)64);
		e_wsle();
		/*      PRINT *, '' */
		isw = 2;
	}

	iy = *iyear;

	/*  WE ARE RESTRICTED BY THE INTERVAL 1965-2010, FOR WHICH THE IGRF COEFFICIENTS */
	/*    ARE KNOWN; IF IYEAR IS OUTSIDE THIS INTERVAL, THEN THE SUBROUTINE USES THE */
	/*      NEAREST LIMITING VALUE AND PRINTS A WARNING: */

	if (iy < 1965) {
		iy = 1965;
		s_wsfe(&io___139);
		do_fio(&c__1, (char *)&(*iyear), (ftnlen)sizeof(integer));
		do_fio(&c__1, (char *)&iy, (ftnlen)sizeof(integer));
		e_wsfe();
	}
	if (iy > 2010) {
		iy = 2010;
		s_wsfe(&io___140);
		do_fio(&c__1, (char *)&(*iyear), (ftnlen)sizeof(integer));
		do_fio(&c__1, (char *)&iy, (ftnlen)sizeof(integer));
		e_wsfe();
	}

	/*  CALCULATE THE ARRAY REC, CONTAINING COEFFICIENTS FOR THE RECURSION RELATIONS, */
	/*  USED IN THE IGRF SUBROUTINE FOR CALCULATING THE ASSOCIATE LEGENDRE POLYNOMIALS */
	/*  AND THEIR DERIVATIVES: */

	for (n = 1; n <= 14; ++n) {
		n2 = (n << 1) - 1;
		n2 *= n2 - 2;
		i__1 = n;
		for (m = 1; m <= i__1; ++m) {
			mn = n * (n - 1) / 2 + m;
			/* L20: */
			geopack2_1.rec[mn - 1] = (real) ((n - m) * (n + m - 2)) / (real)
				n2;
		}
	}

	if (iy < 1970) {
		goto L50;
	}
	/* INTERPOLATE BETWEEN 1965 - 1970 */
	if (iy < 1975) {
		goto L60;
	}
	/* INTERPOLATE BETWEEN 1970 - 1975 */
	if (iy < 1980) {
		goto L70;
	}
	/* INTERPOLATE BETWEEN 1975 - 1980 */
	if (iy < 1985) {
		goto L80;
	}
	/* INTERPOLATE BETWEEN 1980 - 1985 */
	if (iy < 1990) {
		goto L90;
	}
	/* INTERPOLATE BETWEEN 1985 - 1990 */
	if (iy < 1995) {
		goto L100;
	}
	/* INTERPOLATE BETWEEN 1990 - 1995 */
	if (iy < 2000) {
		goto L110;
	}
	/* INTERPOLATE BETWEEN 1995 - 2000 */
	if (iy < 2005) {
		goto L120;
	}

	/*       EXTRAPOLATE BEYOND 2005: */

	/* INTERPOLATE BETWEEN 2000 - 2005 */
	dt = (real) iy + (real) (*iday - 1) / 365.25f - 2005.f;
	for (n = 1; n <= 105; ++n) {
		geopack2_1.g[n - 1] = g05[n - 1];
		geopack2_1.h__[n - 1] = h05[n - 1];
		if (n > 45) {
			goto L40;
		}
		geopack2_1.g[n - 1] += dg05[n - 1] * dt;
		geopack2_1.h__[n - 1] += dh05[n - 1] * dt;
L40:
		;
	}
	goto L300;

	/*       INTERPOLATE BETWEEEN 1965 - 1970: */

L50:
	f2 = ((real) iy + (real) (*iday - 1) / 365.25f - 1965) / 5.f;
	f1 = 1.f - f2;
	for (n = 1; n <= 105; ++n) {
		geopack2_1.g[n - 1] = g65[n - 1] * f1 + g70[n - 1] * f2;
		/* L55: */
		geopack2_1.h__[n - 1] = h65[n - 1] * f1 + h70[n - 1] * f2;
	}
	goto L300;

	/*       INTERPOLATE BETWEEN 1970 - 1975: */

L60:
	f2 = ((real) iy + (real) (*iday - 1) / 365.25f - 1970) / 5.f;
	f1 = 1.f - f2;
	for (n = 1; n <= 105; ++n) {
		geopack2_1.g[n - 1] = g70[n - 1] * f1 + g75[n - 1] * f2;
		/* L65: */
		geopack2_1.h__[n - 1] = h70[n - 1] * f1 + h75[n - 1] * f2;
	}
	goto L300;

	/*       INTERPOLATE BETWEEN 1975 - 1980: */

L70:
	f2 = ((real) iy + (real) (*iday - 1) / 365.25f - 1975) / 5.f;
	f1 = 1.f - f2;
	for (n = 1; n <= 105; ++n) {
		geopack2_1.g[n - 1] = g75[n - 1] * f1 + g80[n - 1] * f2;
		/* L75: */
		geopack2_1.h__[n - 1] = h75[n - 1] * f1 + h80[n - 1] * f2;
	}
	goto L300;

	/*       INTERPOLATE BETWEEN 1980 - 1985: */

L80:
	f2 = ((real) iy + (real) (*iday - 1) / 365.25f - 1980) / 5.f;
	f1 = 1.f - f2;
	for (n = 1; n <= 105; ++n) {
		geopack2_1.g[n - 1] = g80[n - 1] * f1 + g85[n - 1] * f2;
		/* L85: */
		geopack2_1.h__[n - 1] = h80[n - 1] * f1 + h85[n - 1] * f2;
	}
	goto L300;

	/*       INTERPOLATE BETWEEN 1985 - 1990: */

L90:
	f2 = ((real) iy + (real) (*iday - 1) / 365.25f - 1985) / 5.f;
	f1 = 1.f - f2;
	for (n = 1; n <= 105; ++n) {
		geopack2_1.g[n - 1] = g85[n - 1] * f1 + g90[n - 1] * f2;
		/* L95: */
		geopack2_1.h__[n - 1] = h85[n - 1] * f1 + h90[n - 1] * f2;
	}
	goto L300;

	/*       INTERPOLATE BETWEEN 1990 - 1995: */

L100:
	f2 = ((real) iy + (real) (*iday - 1) / 365.25f - 1990) / 5.f;
	f1 = 1.f - f2;
	for (n = 1; n <= 105; ++n) {
		geopack2_1.g[n - 1] = g90[n - 1] * f1 + g95[n - 1] * f2;
		/* L105: */
		geopack2_1.h__[n - 1] = h90[n - 1] * f1 + h95[n - 1] * f2;
	}
	goto L300;

	/*       INTERPOLATE BETWEEN 1995 - 2000: */

L110:
	f2 = ((real) iy + (real) (*iday - 1) / 365.25f - 1995) / 5.f;
	f1 = 1.f - f2;
	for (n = 1; n <= 105; ++n) {
		/*  THE 2000 COEFFICIENTS (G00) GO THROUGH THE ORD */
		geopack2_1.g[n - 1] = g95[n - 1] * f1 + g00[n - 1] * f2;
		/* L115: */
		geopack2_1.h__[n - 1] = h95[n - 1] * f1 + h00[n - 1] * f2;
	}
	goto L300;

	/*       INTERPOLATE BETWEEN 2000 - 2005: */

L120:
	f2 = ((real) iy + (real) (*iday - 1) / 365.25f - 2000) / 5.f;
	f1 = 1.f - f2;
	for (n = 1; n <= 105; ++n) {
		geopack2_1.g[n - 1] = g00[n - 1] * f1 + g05[n - 1] * f2;
		/* L125: */
		geopack2_1.h__[n - 1] = h00[n - 1] * f1 + h05[n - 1] * f2;
	}
	goto L300;

	/*   COEFFICIENTS FOR A GIVEN YEAR HAVE BEEN CALCULATED; NOW MULTIPLY */
	/*   THEM BY SCHMIDT NORMALIZATION FACTORS: */

L300:
	s = 1.f;
	for (n = 2; n <= 14; ++n) {
		mn = n * (n - 1) / 2 + 1;
		s = s * (real) ((n << 1) - 3) / (real) (n - 1);
		geopack2_1.g[mn - 1] *= s;
		geopack2_1.h__[mn - 1] *= s;
		p = s;
		i__1 = n;
		for (m = 2; m <= i__1; ++m) {
			aa = 1.f;
			if (m == 2) {
				aa = 2.f;
			}
			p *= sqrt(aa * (real) (n - m + 1) / (real) (n + m - 2));
			mnn = mn + m - 1;
			geopack2_1.g[mnn - 1] *= p;
			/* L130: */
			geopack2_1.h__[mnn - 1] *= p;
		}
	}
	g10 = -geopack2_1.g[1];
	g11 = geopack2_1.g[2];
	h11 = geopack2_1.h__[2];

	/*  NOW CALCULATE GEO COMPONENTS OF THE UNIT VECTOR EzMAG, PARALLEL TO GEODIPOLE AXIS: */
	/*   SIN(TETA0)*COS(LAMBDA0), SIN(TETA0)*SIN(LAMBDA0), AND COS(TETA0) */
	/*         ST0 * CL0                ST0 * SL0                CT0 */

	/* Computing 2nd power */
	r__1 = g11;
	/* Computing 2nd power */
	r__2 = h11;
	sq = r__1 * r__1 + r__2 * r__2;
	sqq = sqrt(sq);
	/* Computing 2nd power */
	r__1 = g10;
	sqr = sqrt(r__1 * r__1 + sq);
	geopack1_2.sl0 = -h11 / sqq;
	geopack1_2.cl0 = -g11 / sqq;
	geopack1_2.st0 = sqq / sqr;
	geopack1_2.ct0 = g10 / sqr;
	geopack1_2.stcl = geopack1_2.st0 * geopack1_2.cl0;
	geopack1_2.stsl = geopack1_2.st0 * geopack1_2.sl0;
	geopack1_2.ctsl = geopack1_2.ct0 * geopack1_2.sl0;
	geopack1_2.ctcl = geopack1_2.ct0 * geopack1_2.cl0;

	/*  NOW CALCULATE GEI COMPONENTS (S1,S2,S3) OF THE UNIT VECTOR S = EX_GSE */
	/*    POINTING FROM THE EARTH'S CENTER TO SUN */

	sun_08__(&iy, iday, ihour, min__, isec, &gst, &slong, &srasn, &sdec);

	s1 = cos(srasn) * cos(sdec);
	s2 = sin(srasn) * cos(sdec);
	s3 = sin(sdec);

	/*  NOW CALCULATE GEI COMPONENTS (DZ1,DZ2,DZ3) OF THE UNIT VECTOR EZGSE */
	/*  POINTING NORTHWARD AND ORTHOGONAL TO THE ECLIPTIC PLANE, AS */
	/*  (0,-SIN(OBLIQ),COS(OBLIQ)). FOR THE EPOCH 1978, OBLIQ = 23.44214 DEGS. */
	/*  HERE WE USE A MORE ACCURATE TIME-DEPENDENT VALUE, DETERMINED AS: */

	dj = (real) ((iy - 1900) * 365 + (iy - 1901) / 4 + *iday) - .5f + (real) (
		*ihour * 3600 + *min__ * 60 + *isec) / 86400.f;
	t = dj / 36525.f;
	obliq = (23.45229f - t * .0130125f) / 57.2957795f;
	dz1 = 0.f;
	dz2 = -sin(obliq);
	dz3 = cos(obliq);

	/*  NOW WE OBTAIN GEI COMPONENTS OF THE UNIT VECTOR EYGSE=(DY1,DY2,DY3), */
	/*  COMPLETING THE RIGHT-HANDED SYSTEM. THEY CAN BE FOUND FROM THE VECTOR */
	/*  PRODUCT EZGSE x EXGSE = (DZ1,DZ2,DZ3) x (S1,S2,S3): */

	dy1 = dz2 * s3 - dz3 * s2;
	dy2 = dz3 * s1 - dz1 * s3;
	dy3 = dz1 * s2 - dz2 * s1;

	/*  NOW LET'S CALCULATE GEI COMPONENTS OF THE UNIT VECTOR X = EXGSW, DIRECTED ANTIPARALLEL */
	/*  TO THE OBSERVED SOLAR WIND FLOW. FIRST, CALCULATE ITS COMPONENTS IN GSE: */

	/* Computing 2nd power */
	r__1 = *vgsex;
	/* Computing 2nd power */
	r__2 = *vgsey;
	/* Computing 2nd power */
	r__3 = *vgsez;
	v = sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3);
	dx1 = -(*vgsex) / v;
	dx2 = -(*vgsey) / v;
	dx3 = -(*vgsez) / v;

	/*  THEN IN GEI: */

	x1 = dx1 * s1 + dx2 * dy1 + dx3 * dz1;
	x2 = dx1 * s2 + dx2 * dy2 + dx3 * dz2;
	x3 = dx1 * s3 + dx2 * dy3 + dx3 * dz3;

	/*  NOW CALCULATE GEI COMPONENTS (DIP1,DIP2,DIP3) OF THE UNIT VECTOR DIP = EZ_SM = EZ_MAG, */
	/*   ALIGNED WITH THE GEODIPOLE AND POINTING NORTHWARD FROM ECLIPTIC PLANE: */

	geopack1_2.cgst = cos(gst);
	geopack1_2.sgst = sin(gst);

	dip1 = geopack1_2.stcl * geopack1_2.cgst - geopack1_2.stsl *
		geopack1_2.sgst;
	dip2 = geopack1_2.stcl * geopack1_2.sgst + geopack1_2.stsl *
		geopack1_2.cgst;
	dip3 = geopack1_2.ct0;

	/*  THIS ALLOWS US TO CALCULATE GEI COMPONENTS OF THE UNIT VECTOR Y = EYGSW */
	/*   BY TAKING THE VECTOR PRODUCT DIP x X AND NORMALIZING IT TO UNIT LENGTH: */

	y1 = dip2 * x3 - dip3 * x2;
	y2 = dip3 * x1 - dip1 * x3;
	y3 = dip1 * x2 - dip2 * x1;
	y = sqrt(y1 * y1 + y2 * y2 + y3 * y3);
	y1 /= y;
	y2 /= y;
	y3 /= y;

	/*   AND GEI COMPONENTS OF THE UNIT VECTOR Z = EZGSW = EXGSW x EYGSW = X x Y: */

	z1 = x2 * y3 - x3 * y2;
	z2 = x3 * y1 - x1 * y3;
	z3 = x1 * y2 - x2 * y1;

	/*   ELEMENTS OF THE MATRIX GSE TO GSW ARE THE SCALAR PRODUCTS: */

	/*  E11=(EXGSE,EXGSW)  E12=(EXGSE,EYGSW)  E13=(EXGSE,EZGSW) */
	/*  E21=(EYGSE,EXGSW)  E22=(EYGSE,EYGSW)  E23=(EYGSE,EZGSW) */
	/*  E31=(EZGSE,EXGSW)  E32=(EZGSE,EYGSW)  E33=(EZGSE,EZGSW) */

	geopack1_2.e11 = s1 * x1 + s2 * x2 + s3 * x3;
	geopack1_2.e12 = s1 * y1 + s2 * y2 + s3 * y3;
	geopack1_2.e13 = s1 * z1 + s2 * z2 + s3 * z3;
	geopack1_2.e21 = dy1 * x1 + dy2 * x2 + dy3 * x3;
	geopack1_2.e22 = dy1 * y1 + dy2 * y2 + dy3 * y3;
	geopack1_2.e23 = dy1 * z1 + dy2 * z2 + dy3 * z3;
	geopack1_2.e31 = dz1 * x1 + dz2 * x2 + dz3 * x3;
	geopack1_2.e32 = dz1 * y1 + dz2 * y2 + dz3 * y3;
	geopack1_2.e33 = dz1 * z1 + dz2 * z2 + dz3 * z3;

	/*   GEODIPOLE TILT ANGLE IN THE GSW SYSTEM: PSI=ARCSIN(DIP,EXGSW) */

	geopack1_2.sps = dip1 * x1 + dip2 * x2 + dip3 * x3;
	/* Computing 2nd power */
	r__1 = geopack1_2.sps;
	geopack1_2.cps = sqrt(1.f - r__1 * r__1);
	geopack1_2.psi = asin(geopack1_2.sps);

	/*   ELEMENTS OF THE MATRIX GEO TO GSW ARE THE SCALAR PRODUCTS: */

	/*   A11=(EXGEO,EXGSW), A12=(EYGEO,EXGSW), A13=(EZGEO,EXGSW), */
	/*   A21=(EXGEO,EYGSW), A22=(EYGEO,EYGSW), A23=(EZGEO,EYGSW), */
	/*   A31=(EXGEO,EZGSW), A32=(EYGEO,EZGSW), A33=(EZGEO,EZGSW), */

	/*   ALL THE UNIT VECTORS IN BRACKETS ARE ALREADY DEFINED IN GEI: */

	/*  EXGEO=(CGST,SGST,0), EYGEO=(-SGST,CGST,0), EZGEO=(0,0,1) */
	/*  EXGSW=(X1,X2,X3),  EYGSW=(Y1,Y2,Y3),   EZGSW=(Z1,Z2,Z3) */
	/*                                                           AND  THEREFORE: */

	geopack1_2.a11 = x1 * geopack1_2.cgst + x2 * geopack1_2.sgst;
	geopack1_2.a12 = -x1 * geopack1_2.sgst + x2 * geopack1_2.cgst;
	geopack1_2.a13 = x3;
	geopack1_2.a21 = y1 * geopack1_2.cgst + y2 * geopack1_2.sgst;
	geopack1_2.a22 = -y1 * geopack1_2.sgst + y2 * geopack1_2.cgst;
	geopack1_2.a23 = y3;
	geopack1_2.a31 = z1 * geopack1_2.cgst + z2 * geopack1_2.sgst;
	geopack1_2.a32 = -z1 * geopack1_2.sgst + z2 * geopack1_2.cgst;
	geopack1_2.a33 = z3;

	/*  NOW CALCULATE ELEMENTS OF THE MATRIX MAG TO SM (ONE ROTATION ABOUT THE GEODIPOLE AXIS); */
	/*   THEY ARE FOUND AS THE SCALAR PRODUCTS: CFI=GM22=(EYSM,EYMAG)=(EYGSW,EYMAG), */
	/*                                          SFI=GM23=(EYSM,EXMAG)=(EYGSW,EXMAG), */
	/*    DERIVED AS FOLLOWS: */

	/* IN GEO, THE VECTORS EXMAG AND EYMAG HAVE THE COMPONENTS (CT0*CL0,CT0*SL0,-ST0) */
	/*  AND (-SL0,CL0,0), RESPECTIVELY.    HENCE, IN GEI THEIR COMPONENTS ARE: */
	/*  EXMAG:    CT0*CL0*COS(GST)-CT0*SL0*SIN(GST) */
	/*            CT0*CL0*SIN(GST)+CT0*SL0*COS(GST) */
	/*            -ST0 */
	/*  EYMAG:    -SL0*COS(GST)-CL0*SIN(GST) */
	/*            -SL0*SIN(GST)+CL0*COS(GST) */
	/*             0 */
	/*  NOW, NOTE THAT GEI COMPONENTS OF EYSM=EYGSW WERE FOUND ABOVE AS Y1, Y2, AND Y3, */
	/*  AND WE ONLY HAVE TO COMBINE THESE QUANTITIES INTO SCALAR PRODUCTS: */

	exmagx = geopack1_2.ct0 * (geopack1_2.cl0 * geopack1_2.cgst -
		geopack1_2.sl0 * geopack1_2.sgst);
	exmagy = geopack1_2.ct0 * (geopack1_2.cl0 * geopack1_2.sgst +
		geopack1_2.sl0 * geopack1_2.cgst);
	exmagz = -geopack1_2.st0;
	eymagx = -(geopack1_2.sl0 * geopack1_2.cgst + geopack1_2.cl0 *
		geopack1_2.sgst);
	eymagy = -(geopack1_2.sl0 * geopack1_2.sgst - geopack1_2.cl0 *
		geopack1_2.cgst);
	geopack1_2.cfi = y1 * eymagx + y2 * eymagy;
	geopack1_2.sfi = y1 * exmagx + y2 * exmagy + y3 * exmagz;

	return 0;
} /* recalc_08__ */


/* ================================================================================== */
/* Subroutine */ int gswgse_08__(real *xgsw, real *ygsw, real *zgsw, real *
								 xgse, real *ygse, real *zgse, integer *j)
{

	/*  THIS SUBROUTINE TRANSFORMS COMPONENTS OF ANY VECTOR BETWEEN THE STANDARD GSE */
	/*  COORDINATE SYSTEM AND THE GEOCENTRIC SOLAR-WIND (GSW, aka GSWM), DEFINED AS FOLLOWS */
	/*  (HONES ET AL., PLANET.SPACE SCI., V.34, P.889, 1986; TSYGANENKO ET AL., JGRA, */
	/*  V.103(A4), P.6827, 1998): */

	/*  IN THE GSW SYSTEM, X AXIS IS ANTIPARALLEL TO THE OBSERVED DIRECTION OF THE SOLAR WIND FLOW. */
	/*  TWO OTHER AXES, Y AND Z, ARE DEFINED IN THE SAME WAY AS FOR THE STANDARD GSM, THAT IS, */
	/*  Z AXIS ORTHOGONAL TO X AXIS, POINTS NORTHWARD, AND LIES IN THE PLANE DEFINED BY THE X- */
	/*  AND GEODIPOLE AXIS. THE Y AXIS COMPLETES THE RIGHT-HANDED SYSTEM. */

	/*  THE GSW SYSTEM BECOMES IDENTICAL TO THE STANDARD GSM IN THE CASE OF */
	/*   A STRICTLY RADIAL SOLAR WIND FLOW. */

	/*  AUTHOR:  N. A. TSYGANENKO */
	/*  ADDED TO 2008 VERSION OF GEOPACK: JAN 27, 2008. */

	/*                    J>0                       J<0 */
	/* -----INPUT:   J,XGSW,YGSW,ZGSW          J,XGSE,YGSE,ZGSE */
	/* -----OUTPUT:    XGSE,YGSE,ZGSE            XGSW,YGSW,ZGSW */

	/*  IMPORTANT THINGS TO REMEMBER: */

	/*   (1) BEFORE CALLING GSWGSE_08, BE SURE TO INVOKE SUBROUTINE RECALC_08, IN ORDER */
	/*       TO DEFINE ALL NECESSARY ELEMENTS OF TRANSFORMATION MATRICES */

	/*   (2) IN THE ABSENCE OF INFORMATION ON THE SOLAR WIND DIRECTION, E.G., WITH ONLY SCALAR */
	/*       SPEED V KNOWN, THIS SUBROUTINE CAN BE USED TO CONVERT VECTORS TO ABERRATED */
	/*       COORDINATE SYSTEM, TAKING INTO ACCOUNT EARTH'S ORBITAL SPEED OF 29 KM/S. */
	/*       TO DO THAT, SPECIFY THE LAST 3 PARAMETERS IN RECALC_08 AS FOLLOWS: */
	/*       VGSEX=-V, VGSEY=29.0, VGSEZ=0.0. */

	/*       IT SHOULD ALSO BE KEPT IN MIND THAT IN SOME SOLAR WIND DATABASES THE ABERRATION */
	/*       EFFECT HAS ALREADY BEEN TAKEN INTO ACCOUNT BY SUBTRACTING 29 KM/S FROM VYGSE; */
	/*       IN THAT CASE, THE ORIGINAL VYGSE VALUES SHOULD BE RESTORED BY ADDING BACK THE */
	/*       29 KM/S CORRECTION. WHETHER OR NOT TO DO THAT, MUST BE VERIFIED WITH THE DATA */
	/*       ORIGINATOR (OR CAN BE DETERMINED BY CALCULATING THE AVERAGE VGSEY OVER */
	/*       A SUFFICIENTLY LONG TIME INTERVAL) */

	/*   (3) IF NO INFORMATION IS AVAILABLE ON THE SOLAR WIND SPEED, THEN SET VGSEX=-400.0 */
	/*       AND  VGSEY=VGSEZ=0. IN THAT CASE, THE GSW COORDINATE SYSTEM BECOMES */
	/*       IDENTICAL TO THE STANDARD ONE. */


	/*  DIRECT TRANSFORMATION: */

	if (*j > 0) {
		*xgse = *xgsw * geopack1_3.e11 + *ygsw * geopack1_3.e12 + *zgsw *
			geopack1_3.e13;
		*ygse = *xgsw * geopack1_3.e21 + *ygsw * geopack1_3.e22 + *zgsw *
			geopack1_3.e23;
		*zgse = *xgsw * geopack1_3.e31 + *ygsw * geopack1_3.e32 + *zgsw *
			geopack1_3.e33;
	}

	/*   INVERSE TRANSFORMATION: CARRIED OUT USING THE TRANSPOSED MATRIX: */

	if (*j < 0) {
		*xgsw = *xgse * geopack1_3.e11 + *ygse * geopack1_3.e21 + *zgse *
			geopack1_3.e31;
		*ygsw = *xgse * geopack1_3.e12 + *ygse * geopack1_3.e22 + *zgse *
			geopack1_3.e32;
		*zgsw = *xgse * geopack1_3.e13 + *ygse * geopack1_3.e23 + *zgse *
			geopack1_3.e33;
	}

	return 0;
} /* gswgse_08__ */


/* ======================================================================================== */

/* Subroutine */ int geomag_08__(real *xgeo, real *ygeo, real *zgeo, real *
								 xmag, real *ymag, real *zmag, integer *j)
{

	/*    CONVERTS GEOGRAPHIC (GEO) TO DIPOLE (MAG) COORDINATES OR VICE VERSA. */

	/*                    J>0                       J<0 */
	/* -----INPUT:  J,XGEO,YGEO,ZGEO           J,XMAG,YMAG,ZMAG */
	/* -----OUTPUT:    XMAG,YMAG,ZMAG           XGEO,YGEO,ZGEO */

	/*  ATTENTION:  SUBROUTINE  RECALC_08  MUST BE INVOKED BEFORE GEOMAG_08 IN TWO CASES: */
	/*     /A/  BEFORE THE FIRST TRANSFORMATION OF COORDINATES */
	/*     /B/  IF THE VALUES OF IYEAR AND/OR IDAY HAVE BEEN CHANGED */

	/*  NO INFORMATION IS REQUIRED HERE ON THE SOLAR WIND VELOCITY, SO ONE */
	/*  CAN SET VGSEX=-400.0, VGSEY=0.0, VGSEZ=0.0 IN RECALC_08. */

	/*   LAST MOFIFICATION:  JAN 28, 2008 */

	/*   AUTHOR:  N. A. TSYGANENKO */

	if (*j > 0) {
		*xmag = *xgeo * geopack1_4.ctcl + *ygeo * geopack1_4.ctsl - *zgeo *
			geopack1_4.st0;
		*ymag = *ygeo * geopack1_4.cl0 - *xgeo * geopack1_4.sl0;
		*zmag = *xgeo * geopack1_4.stcl + *ygeo * geopack1_4.stsl + *zgeo *
			geopack1_4.ct0;
	} else {
		*xgeo = *xmag * geopack1_4.ctcl - *ymag * geopack1_4.sl0 + *zmag *
			geopack1_4.stcl;
		*ygeo = *xmag * geopack1_4.ctsl + *ymag * geopack1_4.cl0 + *zmag *
			geopack1_4.stsl;
		*zgeo = *zmag * geopack1_4.ct0 - *xmag * geopack1_4.st0;
	}
	return 0;
} /* geomag_08__ */


/* ========================================================================================= */

/* Subroutine */ int geigeo_08__(real *xgei, real *ygei, real *zgei, real *
								 xgeo, real *ygeo, real *zgeo, integer *j)
{

	/*   CONVERTS EQUATORIAL INERTIAL (GEI) TO GEOGRAPHICAL (GEO) COORDS */
	/*   OR VICE VERSA. */
	/*                    J>0                J<0 */
	/* ----INPUT:  J,XGEI,YGEI,ZGEI    J,XGEO,YGEO,ZGEO */
	/* ----OUTPUT:   XGEO,YGEO,ZGEO      XGEI,YGEI,ZGEI */

	/*  ATTENTION:  SUBROUTINE  RECALC_08  MUST BE INVOKED BEFORE GEIGEO_08 IN TWO CASES: */
	/*     /A/  BEFORE THE FIRST TRANSFORMATION OF COORDINATES */
	/*     /B/  IF THE CURRENT VALUES OF IYEAR,IDAY,IHOUR,MIN,ISEC HAVE BEEN CHANGED */

	/*  NO INFORMATION IS REQUIRED HERE ON THE SOLAR WIND VELOCITY, SO ONE */
	/*  CAN SET VGSEX=-400.0, VGSEY=0.0, VGSEZ=0.0 IN RECALC_08. */

	/*     LAST MODIFICATION:  JAN 28, 2008 */
	/*     AUTHOR:  N. A. TSYGANENKO */


	if (*j > 0) {
		*xgeo = *xgei * geopack1_5.cgst + *ygei * geopack1_5.sgst;
		*ygeo = *ygei * geopack1_5.cgst - *xgei * geopack1_5.sgst;
		*zgeo = *zgei;
	} else {
		*xgei = *xgeo * geopack1_5.cgst - *ygeo * geopack1_5.sgst;
		*ygei = *ygeo * geopack1_5.cgst + *xgeo * geopack1_5.sgst;
		*zgei = *zgeo;
	}
	return 0;
} /* geigeo_08__ */


/* ======================================================================================= */

/* Subroutine */ int magsm_08__(real *xmag, real *ymag, real *zmag, real *xsm,
								real *ysm, real *zsm, integer *j)
{

	/*  CONVERTS DIPOLE (MAG) TO SOLAR MAGNETIC (SM) COORDINATES OR VICE VERSA */

	/*                    J>0              J<0 */
	/* ----INPUT: J,XMAG,YMAG,ZMAG     J,XSM,YSM,ZSM */
	/* ----OUTPUT:    XSM,YSM,ZSM       XMAG,YMAG,ZMAG */

	/*  ATTENTION:  SUBROUTINE  RECALC_08  MUST BE INVOKED BEFORE MAGSM_08 IN THREE CASES: */
	/*     /A/  BEFORE THE FIRST TRANSFORMATION OF COORDINATES, OR */
	/*     /B/  IF THE VALUES OF IYEAR,IDAY,IHOUR,MIN,ISEC HAVE CHANGED, AND/OR */
	/*     /C/  IF THE VALUES OF COMPONENTS OF THE SOLAR WIND FLOW VELOCITY HAVE CHANGED */

	/*    IMPORTANT NOTE: */

	/*        A NON-STANDARD DEFINITION IS IMPLIED HERE FOR THE SOLAR MAGNETIC COORDINATE */
	/*        SYSTEM:  IT IS ASSUMED THAT THE XSM AXIS LIES IN THE PLANE DEFINED BY THE */
	/*        GEODIPOLE AXIS AND THE OBSERVED VECTOR OF THE SOLAR WIND FLOW (RATHER THAN */
	/*        THE EARTH-SUN LINE).  IN ORDER TO CONVERT MAG COORDINATES TO AND FROM THE */
	/*        STANDARD SM COORDINATES, INVOKE RECALC_08 WITH VGSEX=-400.0, VGSEY=0.0, VGSEZ=0.0 */

	/*     LAST MODIFICATION:  FEB 07, 2008 */

	/*     AUTHOR:  N. A. TSYGANENKO */


	if (*j > 0) {
		*xsm = *xmag * geopack1_6.cfi - *ymag * geopack1_6.sfi;
		*ysm = *xmag * geopack1_6.sfi + *ymag * geopack1_6.cfi;
		*zsm = *zmag;
	} else {
		*xmag = *xsm * geopack1_6.cfi + *ysm * geopack1_6.sfi;
		*ymag = *ysm * geopack1_6.cfi - *xsm * geopack1_6.sfi;
		*zmag = *zsm;
	}
	return 0;
} /* magsm_08__ */


/* ===================================================================================== */

/* Subroutine */ int smgsw_08__(real *xsm, real *ysm, real *zsm, real *xgsw,
								real *ygsw, real *zgsw, integer *j)
{

	/*  CONVERTS SOLAR MAGNETIC (SM) TO GEOCENTRIC SOLAR-WIND (GSW) COORDINATES OR VICE VERSA. */

	/*                  J>0                 J<0 */
	/* -----INPUT: J,XSM,YSM,ZSM        J,XGSW,YGSW,ZGSW */
	/* ----OUTPUT:  XGSW,YGSW,ZGSW       XSM,YSM,ZSM */

	/*  ATTENTION:  SUBROUTINE RECALC_08 MUST BE INVOKED BEFORE SMGSW_08 IN THREE CASES: */
	/*     /A/  BEFORE THE FIRST TRANSFORMATION OF COORDINATES */
	/*     /B/  IF THE VALUES OF IYEAR,IDAY,IHOUR,MIN,ISEC HAVE BEEN CHANGED */
	/*     /C/  IF THE VALUES OF COMPONENTS OF THE SOLAR WIND FLOW VELOCITY HAVE CHANGED */

	/*    IMPORTANT NOTE: */

	/*        A NON-STANDARD DEFINITION IS IMPLIED HERE FOR THE SOLAR MAGNETIC (SM) COORDINATE */
	/*        SYSTEM:  IT IS ASSUMED THAT THE XSM AXIS LIES IN THE PLANE DEFINED BY THE */
	/*        GEODIPOLE AXIS AND THE OBSERVED VECTOR OF THE SOLAR WIND FLOW (RATHER THAN */
	/*        THE EARTH-SUN LINE).  IN ORDER TO CONVERT MAG COORDINATES TO AND FROM THE */
	/*        STANDARD SM COORDINATES, INVOKE RECALC_08 WITH VGSEX=-400.0, VGSEY=0.0, VGSEZ=0.0 */

	/*     LAST MODIFICATION:  FEB 07, 2008 */

	/*     AUTHOR:  N. A. TSYGANENKO */

	if (*j > 0) {
		*xgsw = *xsm * geopack1_7.cps + *zsm * geopack1_7.sps;
		*ygsw = *ysm;
		*zgsw = *zsm * geopack1_7.cps - *xsm * geopack1_7.sps;
	} else {
		*xsm = *xgsw * geopack1_7.cps - *zgsw * geopack1_7.sps;
		*ysm = *ygsw;
		*zsm = *xgsw * geopack1_7.sps + *zgsw * geopack1_7.cps;
	}
	return 0;
} /* smgsw_08__ */


/* ========================================================================================== */

/* Subroutine */ int geogsw_08__(real *xgeo, real *ygeo, real *zgeo, real *
								 xgsw, real *ygsw, real *zgsw, integer *j)
{

	/* CONVERTS GEOGRAPHIC (GEO) TO GEOCENTRIC SOLAR-WIND (GSW) COORDINATES OR VICE VERSA. */

	/*                   J>0                   J<0 */
	/* ----- INPUT:  J,XGEO,YGEO,ZGEO    J,XGSW,YGSW,ZGSW */
	/* ---- OUTPUT:    XGSW,YGSW,ZGSW      XGEO,YGEO,ZGEO */

	/*  ATTENTION:  SUBROUTINE  RECALC_08  MUST BE INVOKED BEFORE GEOGSW_08 IN THREE CASES: */
	/*     /A/  BEFORE THE FIRST TRANSFORMATION OF COORDINATES, OR */
	/*     /B/  IF THE VALUES OF IYEAR,IDAY,IHOUR,MIN,ISEC  HAVE CHANGED, AND/OR */
	/*     /C/  IF THE VALUES OF COMPONENTS OF THE SOLAR WIND FLOW VELOCITY HAVE CHANGED */

	/*  NOTE: THIS SUBROUTINE CONVERTS GEO VECTORS TO AND FROM THE SOLAR-WIND GSW COORDINATE */
	/*        SYSTEM, TAKING INTO ACCOUNT POSSIBLE DEFLECTIONS OF THE SOLAR WIND DIRECTION FROM */
	/*        STRICTLY RADIAL.  BEFORE CONVERTING TO/FROM STANDARD GSM COORDINATES, INVOKE RECALC_08 */
	/*        WITH VGSEX=-400.0 and VGSEY=0.0, VGSEZ=0.0 */

	/*     LAST MODIFICATION:  FEB 07, 2008 */

	/*     AUTHOR:  N. A. TSYGANENKO */


	if (*j > 0) {
		*xgsw = geopack1_8.a11 * *xgeo + geopack1_8.a12 * *ygeo +
			geopack1_8.a13 * *zgeo;
		*ygsw = geopack1_8.a21 * *xgeo + geopack1_8.a22 * *ygeo +
			geopack1_8.a23 * *zgeo;
		*zgsw = geopack1_8.a31 * *xgeo + geopack1_8.a32 * *ygeo +
			geopack1_8.a33 * *zgeo;
	} else {
		*xgeo = geopack1_8.a11 * *xgsw + geopack1_8.a21 * *ygsw +
			geopack1_8.a31 * *zgsw;
		*ygeo = geopack1_8.a12 * *xgsw + geopack1_8.a22 * *ygsw +
			geopack1_8.a32 * *zgsw;
		*zgeo = geopack1_8.a13 * *xgsw + geopack1_8.a23 * *ygsw +
			geopack1_8.a33 * *zgsw;
	}
	return 0;
} /* geogsw_08__ */


/* ===================================================================================== */

/* Subroutine */ int geodgeo_08__(real *h__, real *xmu, real *r__, real *
								  theta, integer *j)
{
	/* Initialized data */

	static real r_eq__ = 6378.137f;
	static real beta = .00673949674228f;
	static real tol = 1e-6f;

	/* System generated locals */
	real r__1, r__2, r__3;

	/* Builtin functions */
	double cos(doublereal), sin(doublereal), sqrt(doublereal), acos(
		doublereal), asin(doublereal);

	/* Local variables */
	static integer n;
	static real x, z__, sp, rr, rs, den, arg, phi, phi1, dphi, xmus, coslam,
		sinlam, cosxmu, sinxmu, cosfims;


	/*  THIS SUBROUTINE (1) CONVERTS VERTICAL LOCAL HEIGHT (ALTITUDE) H AND GEODETIC */
	/*  LATITUDE XMU INTO GEOCENTRIC COORDINATES R AND THETA (GEOCENTRIC RADIAL */
	/*  DISTANCE AND COLATITUDE, RESPECTIVELY; ALSO KNOWN AS ECEF COORDINATES), */
	/*  AS WELL AS (2) PERFORMS THE INVERSE TRANSFORMATION FROM {R,THETA} TO {H,XMU}. */

	/*  THE SUBROUTINE USES WORLD GEODETIC SYSTEM WGS84 PARAMETERS FOR THE EARTH'S */
	/*  ELLIPSOID. THE ANGULAR QUANTITIES (GEO COLATITUDE THETA AND GEODETIC LATITUDE */
	/*  XMU) ARE IN RADIANS, AND THE DISTANCES (GEOCENTRIC RADIUS R AND ALTITUDE H */
	/*  ABOVE THE EARTH'S ELLIPSOID) ARE IN KILOMETERS. */

	/*  IF J>0, THE TRANSFORMATION IS MADE FROM GEODETIC TO GEOCENTRIC COORDINATES */
	/*   USING SIMPLE DIRECT EQUATIONS. */
	/*  IF J<0, THE INVERSE TRANSFORMATION FROM GEOCENTRIC TO GEODETIC COORDINATES */
	/*   IS MADE BY MEANS OF A FAST ITERATIVE ALGORITHM. */

	/* ------------------------------------------------------------------------------- */
	/*                   J>0                     |            J<0 */
	/* -------------------------------------------|----------------------------------- */
	/* --INPUT:   J        H          XMU         |    J         R          THETA */
	/*         flag  altitude (km)  geodetic     |   flag   geocentric    spherical */
	/*                              latitude     |         distance (km) colatitude */
	/*                              (radians)    |                        (radians) */
	/* -------------------------------------------|----------------------------------- */
	/*                                           | */
	/* ----OUTPUT:         R           THETA      |          H              XMU */
	/*                geocentric    spherical    |      altitude (km)    geodetic */
	/*                distance (km) colatitude   |                       latitude */
	/*                              (radians)    |                       (radians) */
	/* ------------------------------------------------------------------------------- */

	/*   AUTHOR:  N. A. TSYGANENKO */
	/*   DATE:    DEC 5, 2007 */


	/*  R_EQ is the semi-major axis of the Earth's ellipsoid, and BETA is its */
	/*  second eccentricity squared */


	/*   Direct transformation (GEOD=>GEO): */

	if (*j > 0) {
		cosxmu = cos(*xmu);
		sinxmu = sin(*xmu);
		/* Computing 2nd power */
		r__1 = cosxmu;
		/* Computing 2nd power */
		r__2 = sinxmu / (beta + 1.f);
		den = sqrt(r__1 * r__1 + r__2 * r__2);
		coslam = cosxmu / den;
		sinlam = sinxmu / (den * (beta + 1.f));
		/* Computing 2nd power */
		r__1 = sinlam;
		rs = r_eq__ / sqrt(beta * (r__1 * r__1) + 1.f);
		x = rs * coslam + *h__ * cosxmu;
		z__ = rs * sinlam + *h__ * sinxmu;
		/* Computing 2nd power */
		r__1 = x;
		/* Computing 2nd power */
		r__2 = z__;
		*r__ = sqrt(r__1 * r__1 + r__2 * r__2);
		*theta = acos(z__ / *r__);
	}

	/*   Inverse transformation (GEO=>GEOD): */

	if (*j < 0) {
		n = 0;
		phi = 1.570796327f - *theta;
		phi1 = phi;
L1:
		sp = sin(phi1);
		/* Computing 2nd power */
		r__1 = sp;
		arg = sp * (beta + 1.) / sqrt(beta * (beta + 2.f) * (r__1 * r__1) +
			1.f);
		xmus = asin(arg);
		/* Computing 2nd power */
		r__1 = sin(phi1);
		rs = r_eq__ / sqrt(beta * (r__1 * r__1) + 1.f);
		cosfims = cos(phi1 - xmus);
		/* Computing 2nd power */
		r__1 = rs * cosfims;
		/* Computing 2nd power */
		r__2 = *r__;
		/* Computing 2nd power */
		r__3 = rs;
		*h__ = sqrt(r__1 * r__1 + r__2 * r__2 - r__3 * r__3) - rs * cosfims;
		z__ = rs * sin(phi1) + *h__ * sin(xmus);
		x = rs * cos(phi1) + *h__ * cos(xmus);
		/* Computing 2nd power */
		r__1 = x;
		/* Computing 2nd power */
		r__2 = z__;
		rr = sqrt(r__1 * r__1 + r__2 * r__2);
		dphi = asin(z__ / rr) - phi;
		phi1 -= dphi;
		++n;
		if (dabs(dphi) > tol && n < 100) {
			goto L1;
		}
		*xmu = xmus;
	}
	return 0;
} /* geodgeo_08__ */


/* ===================================================================================== */

/* Subroutine */ int rhand_08__(real *x, real *y, real *z__, real *r1, real *
								r2, real *r3, integer *iopt, real *parmod, S_fp exname, S_fp inname__)
{
	/* System generated locals */
	real r__1, r__2, r__3;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	static real b, bx, by, bz, bxgsw, bygsw, bzgsw, hxgsw, hygsw, hzgsw;


	/*  CALCULATES THE COMPONENTS OF THE RIGHT HAND SIDE VECTOR IN THE GEOMAGNETIC FIELD */
	/*    LINE EQUATION  (a subsidiary subroutine for the subroutine STEP_08) */

	/*     LAST MODIFICATION:  FEB 07, 2008 */

	/*     AUTHOR:  N. A. TSYGANENKO */


	/*     EXNAME AND INNAME ARE NAMES OF SUBROUTINES FOR THE EXTERNAL AND INTERNAL */
	/*     PARTS OF THE TOTAL FIELD, E.G., T96_01 AND IGRF_GSW_08 */

	/* Parameter adjustments */
	--parmod;

	/* Function Body */
	(*exname)(iopt, &parmod[1], &geopack1_9.psi, x, y, z__, &bxgsw, &bygsw, &
		bzgsw);
	(*inname__)(x, y, z__, &hxgsw, &hygsw, &hzgsw);
	bx = bxgsw + hxgsw;
	by = bygsw + hygsw;
	bz = bzgsw + hzgsw;
	/* Computing 2nd power */
	r__1 = bx;
	/* Computing 2nd power */
	r__2 = by;
	/* Computing 2nd power */
	r__3 = bz;
	b = geopack1_9.ds3 / sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3);
	*r1 = bx * b;
	*r2 = by * b;
	*r3 = bz * b;
	return 0;
} /* rhand_08__ */


/* =================================================================================== */

/* Subroutine */ int step_08__(real *x, real *y, real *z__, real *ds, real *
							   dsmax, real *errin, integer *iopt, real *parmod, S_fp exname, S_fp
							   inname__)
{
	/* System generated locals */
	real r__1, r__2, r__3;

	/* Builtin functions */
	double r_sign(real *, real *);

	/* Local variables */
	static real r11, r12, r13, r21, r22, r23, r31, r32, r33, r41, r42, r43,
		r51, r52, r53, errcur;
	extern /* Subroutine */ int rhand_08__(real *, real *, real *, real *,
		real *, real *, integer *, real *, S_fp, S_fp);


	/*   RE-CALCULATES THE INPUT VALUES {X,Y,Z} (IN GSW COORDINATES) FOR ANY POINT ON A FIELD LINE, */
	/*     BY MAKING A STEP ALONG THAT LINE USING RUNGE-KUTTA-MERSON ALGORITHM (G.N. Lance, Numerical */
	/*      methods for high-speed computers, Iliffe & Sons, London 1960.) */
	/*   DS IS A PRESCRIBED VALUE OF THE CURRENT STEP SIZE, DSMAX IS ITS UPPER LIMIT. */
	/*   ERRIN IS A PERMISSIBLE ERROR (ITS OPTIMAL VALUE SPECIFIED IN THE S/R TRACE_08) */
	/*     IF THE ACTUAL ERROR (ERRCUR) AT THE CURRENT STEP IS LARGER THAN ERRIN, THE STEP IS REJECTED, */
	/*       AND THE CALCULATION IS REPEATED ANEW WITH HALVED STEPSIZE DS. */
	/*     IF ERRCUR IS SMALLER THAN ERRIN, THE STEP IS ACCEPTED, AND THE CURRENT VALUE OF DS IS RETAINED */
	/*       FOR THE NEXT STEP. */
	/*     IF ERRCUR IS SMALLER THAN 0.04*ERRIN, THE STEP IS ACCEPTED, AND THE VALUE OF DS FOR THE NEXT STEP */
	/*       IS INCREASED BY THE FACTOR 1.5, BUT NOT LARGER THAN DSMAX. */
	/*   IOPT IS A FLAG, RESERVED FOR SPECIFYNG A VERSION OF THE EXTERNAL FIELD MODEL EXNAME. */
	/*   ARRAY PARMOD(10) CONTAINS INPUT PARAMETERS FOR THE MODEL EXNAME. */
	/*   EXNAME IS THE NAME OF THE SUBROUTINE FOR THE EXTERNAL FIELD MODEL. */
	/*   INNAME IS THE NAME OF THE SUBROUTINE FOR THE INTERNAL FIELD MODEL (EITHER DIP_08 OR IGRF_GSW_08) */

	/*   ALL THE ABOVE PARAMETERS ARE INPUT ONES; OUTPUT IS THE RECALCULATED VALUES OF X,Y,Z */

	/*     LAST MODIFICATION:  APR 21, 2008 */

	/*     AUTHOR:  N. A. TSYGANENKO */

	/* Parameter adjustments */
	--parmod;

	/* Function Body */
L1:
	geopack1_10.ds3 = -(*ds) / 3.f;
	rhand_08__(x, y, z__, &r11, &r12, &r13, iopt, &parmod[1], (S_fp)exname, (
		S_fp)inname__);
	r__1 = *x + r11;
	r__2 = *y + r12;
	r__3 = *z__ + r13;
	rhand_08__(&r__1, &r__2, &r__3, &r21, &r22, &r23, iopt, &parmod[1], (S_fp)
		exname, (S_fp)inname__);
	r__1 = *x + (r11 + r21) * .5f;
	r__2 = *y + (r12 + r22) * .5f;
	r__3 = *z__ + (r13 + r23) * .5f;
	rhand_08__(&r__1, &r__2, &r__3, &r31, &r32, &r33, iopt, &parmod[1], (S_fp)
		exname, (S_fp)inname__);
	r__1 = *x + (r11 + r31 * 3.f) * .375f;
	r__2 = *y + (r12 + r32 * 3.f) * .375f;
	r__3 = *z__ + (r13 + r33 * 3.f) * .375f;
	rhand_08__(&r__1, &r__2, &r__3, &r41, &r42, &r43, iopt, &parmod[1], (S_fp)
		exname, (S_fp)inname__);
	r__1 = *x + (r11 - r31 * 3.f + r41 * 4.f) * 1.5f;
	r__2 = *y + (r12 - r32 * 3.f + r42 * 4.f) * 1.5f;
	r__3 = *z__ + (r13 - r33 * 3.f + r43 * 4.f) * 1.5f;
	rhand_08__(&r__1, &r__2, &r__3, &r51, &r52, &r53, iopt, &parmod[1], (S_fp)
		exname, (S_fp)inname__);
	errcur = (r__1 = r11 - r31 * 4.5f + r41 * 4.f - r51 * .5f, dabs(r__1)) + (
		r__2 = r12 - r32 * 4.5f + r42 * 4.f - r52 * .5f, dabs(r__2)) + (
		r__3 = r13 - r33 * 4.5f + r43 * 4.f - r53 * .5f, dabs(r__3));

	/*  READY FOR MAKING THE STEP, BUT CHECK THE ACCURACY; IF INSUFFICIENT, */
	/*   REPEAT THE STEP WITH HALVED STEPSIZE: */

	if (errcur > *errin) {
		*ds *= .5f;
		goto L1;
	}

	/*  ACCURACY IS ACCEPTABLE, BUT CHECK IF THE STEPSIZE IS NOT TOO LARGE; */
	/*    OTHERWISE REPEAT THE STEP WITH DS=DSMAX */

	if (dabs(*ds) > *dsmax) {
		*ds = r_sign(dsmax, ds);
		goto L1;
	}

	/*  MAKING THE STEP: */

	/* L2: */
	*x += (r11 + r41 * 4.f + r51) * .5f;
	*y += (r12 + r42 * 4.f + r52) * .5f;
	*z__ += (r13 + r43 * 4.f + r53) * .5f;

	/*  IF THE ACTUAL ERROR IS TOO SMALL (LESS THAN 4% OF ERRIN) AND DS SMALLER */
	/*   THAN DSMAX/1.5, THEN WE INCREASE THE STEPSIZE FOR THE NEXT STEP BY 50% */

	if (errcur < *errin * .04f && *ds < *dsmax / 1.5f) {
		*ds *= 1.5f;
	}
	return 0;
} /* step_08__ */


/* ============================================================================== */

/* Subroutine */ int trace_08__(real *xi, real *yi, real *zi, real *dir, real
								*dsmax, real *err, real *rlim, real *r0, integer *iopt, real *parmod,
								S_fp exname, S_fp inname__, real *xf, real *yf, real *zf, real *xx,
								real *yy, real *zz, integer *l, integer *lmax)
{
	/* Format strings */
	static char fmt_10[] = "(//,1x,\002**** COMPUTATIONS IN THE SUBROUTINE T"
		"RACE_08 ARE\002,\002 TERMINATED: THE NUMBER OF POINTS EXCEEDED L"
		"MAX ****\002//)";

	/* System generated locals */
	real r__1, r__2, r__3;

	/* Builtin functions */
	double sqrt(doublereal);
	integer s_wsfe(cilist *), e_wsfe(void);

	/* Local variables */
	static real r__, x, y, z__, r1, r2, r3, ad, fc, al, dr, ds, rr, xr, yr,
		zr, drp, ryz;
	static integer nrev;
	extern /* Subroutine */ int step_08__(real *, real *, real *, real *,
		real *, real *, integer *, real *, S_fp, S_fp), rhand_08__(real *,
		real *, real *, real *, real *, real *, integer *, real *, S_fp,
		S_fp);

	/* Fortran I/O blocks */
	static cilist io___261 = { 0, 6, 0, fmt_10, 0 };



	/*  TRACES A FIELD LINE FROM AN ARBITRARY POINT OF SPACE TO THE EARTH'S */
	/*  SURFACE OR TO A MODEL LIMITING BOUNDARY. */

	/*  THIS SUBROUTINE ALLOWS TWO OPTIONS: */

	/*  (1) IF INNAME=IGRF_GSW_08, THEN THE IGRF MODEL WILL BE USED FOR CALCULATING */
	/*      CONTRIBUTION FROM EARTH'S INTERNAL SOURCES. IN THIS CASE, SUBROUTINE */
	/*      RECALC_08 MUST BE CALLED BEFORE USING TRACE_08, WITH PROPERLY SPECIFIED DATE, */
	/*      UNIVERSAL TIME, AND SOLAR WIND VELOCITY COMPONENTS, TO CALCULATE IN ADVANCE */
	/*      ALL QUANTITIES NEEDED FOR THE MAIN FIELD MODEL AND FOR TRANSFORMATIONS */
	/*      BETWEEN INVOLVED COORDINATE SYSTEMS. */

	/*  (2) IF INNAME=DIP_08, THEN A PURE DIPOLE FIELD WILL BE USED INSTEAD OF THE IGRF MODEL. */
	/*      IN THIS CASE, THE SUBROUTINE RECALC_08 MUST ALSO BE CALLED BEFORE TRACE_08. */
	/*      HERE ONE CAN CHOOSE EITHER TO */
	/*      (a) CALCULATE DIPOLE TILT ANGLE BASED ON DATE, TIME, AND SOLAR WIND DIRECTION, */
	/*   OR (b) EXPLICITLY SPECIFY THAT ANGLE, WITHOUT ANY REFERENCE TO DATE/UT/SOLAR WIND. */
	/*      IN THE LAST CASE (b), THE SINE (SPS) AND COSINE (CPS) OF THE DIPOLE TILT */
	/*      ANGLE MUST BE SPECIFIED IN ADVANCE (BUT AFTER HAVING CALLED RECALC_08) AND FORWARDED */
	/*      IN THE COMMON BLOCK /GEOPACK1/ (IN ITS 11th AND 12th ELEMENTS, RESPECTIVELY). */
	/*      IN THIS CASE THE ROLE OF THE SUBROUTINE RECALC_08 IS REDUCED TO ONLY CALCULATING */
	/*      THE COMPONENTS OF THE EARTH'S DIPOLE MOMENT. */

	/* ------------- INPUT PARAMETERS: */

	/*   XI,YI,ZI - GSW COORDS OF THE FIELD LINE STARTING POINT (IN EARTH RADII, 1 RE = 6371.2 km), */

	/*   DIR - SIGN OF THE TRACING DIRECTION: IF DIR=1.0 THEN THE TRACING IS MADE ANTIPARALLEL */
	/*     TO THE TOTAL FIELD VECTOR (E.G., FROM NORTHERN TO SOUTHERN CONJUGATE POINT); */
	/*     IF DIR=-1.0 THEN THE TRACING PROCEEDS IN THE OPPOSITE DIRECTION, THAT IS, PARALLEL TO */
	/*     THE TOTAL FIELD VECTOR. */

	/*   DSMAX - UPPER LIMIT ON THE STEPSIZE (SETS A DESIRED MAXIMAL SPACING BETWEEN */
	/*                 THE FIELD LINE POINTS) */

	/*   ERR - PERMISSIBLE STEP ERROR. A REASONABLE ESTIMATE PROVIDING A SUFFICIENT ACCURACY FOR MOST */
	/*         APPLICATIONS IS ERR=0.0001. SMALLER/LARGER VALUES WILL RESULT IN LARGER/SMALLER NUMBER */
	/*         OF STEPS AND, HENCE, OF OUTPUT FIELD LINE POINTS. NOTE THAT USING MUCH SMALLER VALUES */
	/*         OF ERR MAY REQUIRE USING A DOUBLE PRECISION VERSION OF THE ENTIRE PACKAGE. */

	/*   R0 -  RADIUS OF A SPHERE (IN RE), DEFINING THE INNER BOUNDARY OF THE TRACING REGION */
	/*         (USUALLY, EARTH'S SURFACE OR THE IONOSPHERE, WHERE R0~1.0) */
	/*         IF THE FIELD LINE REACHES THAT SPHERE FROM OUTSIDE, ITS INBOUND TRACING IS */
	/*         TERMINATED AND THE CROSSING POINT COORDINATES XF,YF,ZF  ARE CALCULATED. */

	/*   RLIM - RADIUS OF A SPHERE (IN RE), DEFINING THE OUTER BOUNDARY OF THE TRACING REGION; */
	/*         IF THE FIELD LINE REACHES THAT BOUNDARY FROM INSIDE, ITS OUTBOUND TRACING IS */
	/*         TERMINATED AND THE CROSSING POINT COORDINATES XF,YF,ZF ARE CALCULATED. */

	/*   IOPT - A MODEL INDEX; CAN BE USED FOR SPECIFYING A VERSION OF THE EXTERNAL FIELD */
	/*       MODEL (E.G., A NUMBER OF THE KP-INDEX INTERVAL). ALTERNATIVELY, ONE CAN USE THE ARRAY */
	/*       PARMOD FOR THAT PURPOSE (SEE BELOW); IN THAT CASE IOPT IS JUST A DUMMY PARAMETER. */

	/*   PARMOD -  A 10-ELEMENT ARRAY CONTAINING INPUT PARAMETERS NEEDED FOR A UNIQUE */
	/*      SPECIFICATION OF THE EXTERNAL FIELD MODEL. THE CONCRETE MEANING OF THE COMPONENTS */
	/*      OF PARMOD DEPENDS ON A SPECIFIC VERSION OF THAT MODEL. */

	/*   EXNAME - NAME OF A SUBROUTINE PROVIDING COMPONENTS OF THE EXTERNAL MAGNETIC FIELD */
	/*    (E.G., T89, OR T96_01, ETC.). */
	/*   INNAME - NAME OF A SUBROUTINE PROVIDING COMPONENTS OF THE INTERNAL MAGNETIC FIELD */
	/*    (EITHER DIP_08 OR IGRF_GSW_08). */

	/*   LMAX - MAXIMAL LENGTH OF THE ARRAYS XX,YY,ZZ, IN WHICH COORDINATES OF THE FIELD */
	/*          LINE POINTS ARE STORED. LMAX SHOULD BE SET EQUAL TO THE ACTUAL LENGTH OF */
	/*          THE ARRAYS, DEFINED IN THE MAIN PROGRAM AS ACTUAL ARGUMENTS OF THIS SUBROUTINE. */

	/* -------------- OUTPUT PARAMETERS: */

	/*   XF,YF,ZF - GSW COORDINATES OF THE ENDPOINT OF THE TRACED FIELD LINE. */
	/*   XX,YY,ZZ - ARRAYS OF LENGTH LMAX, CONTAINING COORDINATES OF THE FIELD LINE POINTS. */
	/*   L - ACTUAL NUMBER OF FIELD LINE POINTS, GENERATED BY THIS SUBROUTINE. */

	/* ---------------------------------------------------------- */

	/*     LAST MODIFICATION:  JAN 30, 2008. */

	/*     AUTHOR:  N. A. TSYGANENKO */


	/* Parameter adjustments */
	--parmod;
	--zz;
	--yy;
	--xx;

	/* Function Body */
	*l = 0;
	nrev = 0;
	geopack1_11.dd = *dir;

	/*  INITIALIZE THE STEP SIZE AND STARTING PONT: */

	ds = *dir * .5f;
	x = *xi;
	y = *yi;
	z__ = *zi;

	/*  here we call RHAND_08 just to find out the sign of the radial component of the field */
	/*   vector, and to determine the initial direction of the tracing (i.e., either away */
	/*   or towards Earth): */

	rhand_08__(&x, &y, &z__, &r1, &r2, &r3, iopt, &parmod[1], (S_fp)exname, (
		S_fp)inname__);
	ad = .01f;
	if (x * r1 + y * r2 + z__ * r3 < 0.f) {
		ad = -.01f;
	}

	/*     |AD|=0.01 and its sign follows the rule: */
	/* (1) if DIR=1 (tracing antiparallel to B vector) then the sign of AD is the same as of Br */
	/* (2) if DIR=-1 (tracing parallel to B vector) then the sign of AD is opposite to that of Br */
	/*     AD is defined in order to initialize the value of RR (radial distance at previous step): */
	/* Computing 2nd power */
	r__1 = x;
	/* Computing 2nd power */
	r__2 = y;
	/* Computing 2nd power */
	r__3 = z__;
	rr = sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3) + ad;

L1:
	++(*l);
	if (*l > *lmax) {
		goto L7;
	}
	xx[*l] = x;
	yy[*l] = y;
	zz[*l] = z__;
	/* Computing 2nd power */
	r__1 = y;
	/* Computing 2nd power */
	r__2 = z__;
	ryz = r__1 * r__1 + r__2 * r__2;
	/* Computing 2nd power */
	r__1 = x;
	r2 = r__1 * r__1 + ryz;
	r__ = sqrt(r2);

	/*  check if the line hit the outer tracing boundary; if yes, then terminate */
	/*   the tracing (label 8). The outer boundary is assumed reached, when the line */
	/*   crosses any of the 3 surfaces: (1) a sphere R=RLIM, (2) a cylinder of radius 40Re, */
	/*   coaxial with the XGSW axis, (3) the plane X=20Re: */
	if (r__ > *rlim || ryz > 1600.f || x > 20.f) {
		goto L8;
	}

	/*  check whether or not the inner tracing boundary was crossed from outside, */
	/*  if yes, then calculate the footpoint position by interpolation (go to label 6): */

	if (r__ < *r0 && rr > r__) {
		goto L6;
	}
	/*  check if we are moving outward, or R is still larger than 3Re; if yes, proceed further: */

	if (r__ >= rr || r__ >= 3.f) {
		goto L4;
	}

	/*  now we entered inside the sphere R=3: to avoid too large steps (and hence */
	/*  inaccurate interpolated position of the footpoint), enforce the progressively */
	/*  smaller stepsize values as we approach the inner boundary R=R0: */

	fc = .2f;
	if (r__ - *r0 < .05f) {
		fc = .05f;
	}
	al = fc * (r__ - *r0 + .2f);
	ds = *dir * al;

L4:
	xr = x;
	yr = y;
	zr = z__;

	drp = r__ - rr;
	rr = r__;

	step_08__(&x, &y, &z__, &ds, dsmax, err, iopt, &parmod[1], (S_fp)exname, (
		S_fp)inname__);

	/*  check the total number NREV of changes in the tracing radial direction; (NREV.GT.2) means */
	/*   that the line started making multiple loops, in which case we stop the process: */

	/* Computing 2nd power */
	r__1 = x;
	/* Computing 2nd power */
	r__2 = y;
	/* Computing 2nd power */
	r__3 = z__;
	r__ = sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3);
	dr = r__ - rr;
	if (drp * dr < 0.f) {
		++nrev;
	}
	if (nrev > 2) {
		goto L8;
	}

	goto L1;

	/*  find the footpoint position by interpolating between the current and previous */
	/*   field line points: */

L6:
	r1 = (*r0 - r__) / (rr - r__);
	x -= (x - xr) * r1;
	y -= (y - yr) * r1;
	z__ -= (z__ - zr) * r1;
	goto L8;
L7:
	s_wsfe(&io___261);
	e_wsfe();
	*l = *lmax;
L8:
	*xf = x;
	*yf = y;
	*zf = z__;
	return 0;
} /* trace_08__ */


/* ==================================================================================== */

/* Subroutine */ int shuetal_mgnp_08__(real *xn_pd__, real *vel, real *bzimf,
									   real *xgsw, real *ygsw, real *zgsw, real *xmgnp, real *ymgnp, real *
									   zmgnp, real *dist, integer *id)
{
	/* System generated locals */
	real r__1, r__2, r__3;
	doublereal d__1, d__2;

	/* Builtin functions */
	double atan2(doublereal, doublereal), tanh(doublereal), pow_dd(doublereal
		*, doublereal *), log(doublereal), sqrt(doublereal), sin(
		doublereal), cos(doublereal);
	integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen),
		e_wsle(void);

	/* Local variables */
	static real f, p, r__, t;
	extern /* Subroutine */ int t96_mgnp_08__(real *, real *, real *, real *,
		real *, real *, real *, real *, real *, integer *);
	static real r0, dr, ct, dt, ds, rm, st;
	static integer id96;
	static real phi, rho;
	static integer nit;
	static real rho2, xmt96, ymt96, zmt96, gradf, alpha, gradf_r__, gradf_t__;

	/* Fortran I/O blocks */
	static cilist io___284 = { 0, 6, 0, 0, 0 };



	/*  FOR ANY POINT OF SPACE WITH COORDINATES (XGSW,YGSW,ZGSW) AND SPECIFIED CONDITIONS */
	/*  IN THE INCOMING SOLAR WIND, THIS SUBROUTINE: */

	/* (1) DETERMINES IF THE POINT (XGSW,YGSW,ZGSW) LIES INSIDE OR OUTSIDE THE */
	/*      MODEL MAGNETOPAUSE OF SHUE ET AL. (JGR-A, V.103, P. 17691, 1998). */

	/* (2) CALCULATES THE GSW POSITION OF A POINT {XMGNP,YMGNP,ZMGNP}, LYING AT THE MODEL */
	/*      MAGNETOPAUSE AND ASYMPTOTICALLY TENDING TO THE NEAREST BOUNDARY POINT WITH */
	/*      RESPECT TO THE OBSERVATION POINT {XGSW,YGSW,ZGSW}, AS IT APPROACHES THE MAGNETO- */
	/*      PAUSE. */

	/*  INPUT: XN_PD - EITHER SOLAR WIND PROTON NUMBER DENSITY (PER C.C.) (IF VEL>0) */
	/*                    OR THE SOLAR WIND RAM PRESSURE IN NANOPASCALS   (IF VEL<0) */
	/*         BZIMF - IMF BZ IN NANOTESLAS */

	/*         VEL - EITHER SOLAR WIND VELOCITY (KM/SEC) */
	/*                  OR ANY NEGATIVE NUMBER, WHICH INDICATES THAT XN_PD STANDS */
	/*                     FOR THE SOLAR WIND PRESSURE, RATHER THAN FOR THE DENSITY */

	/*         XGSW,YGSW,ZGSW - GSW POSITION OF THE OBSERVATION POINT IN EARTH RADII */

	/*  OUTPUT: XMGNP,YMGNP,ZMGNP - GSW POSITION OF THE BOUNDARY POINT */
	/*          DIST - DISTANCE (IN RE) BETWEEN THE OBSERVATION POINT (XGSW,YGSW,ZGSW) */
	/*                 AND THE MODEL NAGNETOPAUSE */
	/*          ID -  POSITION FLAG:  ID=+1 (-1) MEANS THAT THE OBSERVATION POINT */
	/*          LIES INSIDE (OUTSIDE) OF THE MODEL MAGNETOPAUSE, RESPECTIVELY. */

	/*  OTHER SUBROUTINES USED: T96_MGNP_08 */

	/*          AUTHOR:  N.A. TSYGANENKO, */
	/*          DATE:    APRIL 4, 2003. */

	if (*vel < 0.f) {
		p = *xn_pd__;
	} else {
		/* Computing 2nd power */
		r__1 = *vel;
		p = *xn_pd__ * 1.94e-6f * (r__1 * r__1);
		/* P IS THE SOLAR WIND DYNAMIC PRESSURE ( */
	}

	/*  DEFINE THE ANGLE PHI, MEASURED DUSKWARD FROM THE NOON-MIDNIGHT MERIDIAN PLANE; */
	/*  IF THE OBSERVATION POINT LIES ON THE X AXIS, THE ANGLE PHI CANNOT BE UNIQUELY */
	/*  DEFINED, AND WE SET IT AT ZERO: */

	if (*ygsw != 0.f || *zgsw != 0.f) {
		phi = atan2(*ygsw, *zgsw);
	} else {
		phi = 0.f;
	}

	/*  FIRST, FIND OUT IF THE OBSERVATION POINT LIES INSIDE THE SHUE ET AL BDRY */
	/*  AND SET THE VALUE OF THE ID FLAG: */

	*id = -1;
	d__1 = (doublereal) p;
	r0 = (tanh((*bzimf + 8.14f) * .184f) * 1.29f + 10.22f) * pow_dd(&d__1, &
		c_b91);
	alpha = (.58f - *bzimf * .007f) * (log(p) * .024f + 1.f);
	/* Computing 2nd power */
	r__1 = *xgsw;
	/* Computing 2nd power */
	r__2 = *ygsw;
	/* Computing 2nd power */
	r__3 = *zgsw;
	r__ = sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3);
	d__1 = (doublereal) (2.f / (*xgsw / r__ + 1.f));
	d__2 = (doublereal) alpha;
	rm = r0 * pow_dd(&d__1, &d__2);
	if (r__ <= rm) {
		*id = 1;
	}

	/*  NOW, FIND THE CORRESPONDING T96 MAGNETOPAUSE POSITION, TO BE USED AS */
	/*  A STARTING APPROXIMATION IN THE SEARCH OF A CORRESPONDING SHUE ET AL. */
	/*  BOUNDARY POINT: */

	t96_mgnp_08__(&p, &c_b92, xgsw, ygsw, zgsw, &xmt96, &ymt96, &zmt96, dist,
		&id96);

	/* Computing 2nd power */
	r__1 = ymt96;
	/* Computing 2nd power */
	r__2 = zmt96;
	rho2 = r__1 * r__1 + r__2 * r__2;
	/* Computing 2nd power */
	r__1 = xmt96;
	r__ = sqrt(rho2 + r__1 * r__1);
	st = sqrt(rho2) / r__;
	ct = xmt96 / r__;

	/*  NOW, USE NEWTON'S ITERATIVE METHOD TO FIND THE NEAREST POINT AT THE */
	/*   SHUE ET AL.'S BOUNDARY: */

	nit = 0;
L1:
	t = atan2(st, ct);
	d__1 = (doublereal) (2.f / (ct + 1.f));
	d__2 = (doublereal) alpha;
	rm = r0 * pow_dd(&d__1, &d__2);
	f = r__ - rm;
	gradf_r__ = 1.f;
	gradf_t__ = -alpha / r__ * rm * st / (ct + 1.f);
	/* Computing 2nd power */
	r__1 = gradf_r__;
	/* Computing 2nd power */
	r__2 = gradf_t__;
	gradf = sqrt(r__1 * r__1 + r__2 * r__2);
	/* Computing 2nd power */
	r__1 = gradf;
	dr = -f / (r__1 * r__1);
	dt = dr / r__ * gradf_t__;
	r__ += dr;
	t += dt;
	st = sin(t);
	ct = cos(t);
	/* Computing 2nd power */
	r__1 = dr;
	/* Computing 2nd power */
	r__2 = r__ * dt;
	ds = sqrt(r__1 * r__1 + r__2 * r__2);
	++nit;
	if (nit > 1000) {
		s_wsle(&io___284);
		do_lio(&c__9, &c__1, " BOUNDARY POINT COULD NOT BE FOUND; ITERATIONS"
			" DO NOT CONVERGE", (ftnlen)62);
		e_wsle();
	}
	if (ds > 1e-4f) {
		goto L1;
	}
	*xmgnp = r__ * cos(t);
	rho = r__ * sin(t);
	*ymgnp = rho * sin(phi);
	*zmgnp = rho * cos(phi);
	/* Computing 2nd power */
	r__1 = *xgsw - *xmgnp;
	/* Computing 2nd power */
	r__2 = *ygsw - *ymgnp;
	/* Computing 2nd power */
	r__3 = *zgsw - *zmgnp;
	*dist = sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3);
	return 0;
} /* shuetal_mgnp_08__ */


/* ======================================================================================= */

/* Subroutine */ int t96_mgnp_08__(real *xn_pd__, real *vel, real *xgsw, real
								   *ygsw, real *zgsw, real *xmgnp, real *ymgnp, real *zmgnp, real *dist,
								   integer *id)
{
	/* System generated locals */
	real r__1, r__2, r__3;
	doublereal d__1;

	/* Builtin functions */
	double pow_dd(doublereal *, doublereal *), atan2(doublereal, doublereal),
		sqrt(doublereal), sin(doublereal), cos(doublereal);

	/* Local variables */
	static real a, a0, s0, x0, s00, pd, x00, xm, sq1, sq2, arg, phi, rat, rho,
		tau, rat16, xksi, xdzt, sigma, rhomgnp;


	/*  FOR ANY POINT OF SPACE WITH GIVEN COORDINATES (XGSW,YGSW,ZGSW), THIS SUBROUTINE DEFINES */
	/*  THE POSITION OF A POINT (XMGNP,YMGNP,ZMGNP) AT THE T96 MODEL MAGNETOPAUSE WITH THE */
	/*  SAME VALUE OF THE ELLIPSOIDAL TAU-COORDINATE, AND THE DISTANCE BETWEEN THEM.  THIS IS */
	/*  NOT THE SHORTEST DISTANCE D_MIN TO THE BOUNDARY, BUT DIST ASYMPTOTICALLY TENDS TO D_MIN, */
	/*  AS THE OBSERVATION POINT GETS CLOSER TO THE MAGNETOPAUSE. */

	/*  INPUT: XN_PD - EITHER SOLAR WIND PROTON NUMBER DENSITY (PER C.C.) (IF VEL>0) */
	/*                    OR THE SOLAR WIND RAM PRESSURE IN NANOPASCALS   (IF VEL<0) */
	/*         VEL - EITHER SOLAR WIND VELOCITY (KM/SEC) */
	/*                  OR ANY NEGATIVE NUMBER, WHICH INDICATES THAT XN_PD STANDS */
	/*                     FOR THE SOLAR WIND PRESSURE, RATHER THAN FOR THE DENSITY */

	/*         XGSW,YGSW,ZGSW - COORDINATES OF THE OBSERVATION POINT IN EARTH RADII */

	/*  OUTPUT: XMGNP,YMGNP,ZMGNP - GSW POSITION OF THE BOUNDARY POINT, HAVING THE SAME */
	/*          VALUE OF TAU-COORDINATE AS THE OBSERVATION POINT (XGSW,YGSW,ZGSW) */
	/*          DIST -  THE DISTANCE BETWEEN THE TWO POINTS, IN RE, */
	/*          ID -    POSITION FLAG; ID=+1 (-1) MEANS THAT THE POINT (XGSW,YGSW,ZGSW) */
	/*          LIES INSIDE (OUTSIDE) THE MODEL MAGNETOPAUSE, RESPECTIVELY. */

	/*  THE PRESSURE-DEPENDENT MAGNETOPAUSE IS THAT USED IN THE T96_01 MODEL */
	/*  (TSYGANENKO, JGR, V.100, P.5599, 1995; ESA SP-389, P.181, OCT. 1996) */

	/*   AUTHOR:  N.A. TSYGANENKO */
	/*   DATE:    AUG.1, 1995, REVISED APRIL 3, 2003. */


	/*  DEFINE SOLAR WIND DYNAMIC PRESSURE (NANOPASCALS, ASSUMING 4% OF ALPHA-PARTICLES), */
	/*   IF NOT EXPLICITLY SPECIFIED IN THE INPUT: */
	if (*vel < 0.f) {
		pd = *xn_pd__;
	} else {
		/* Computing 2nd power */
		r__1 = *vel;
		pd = *xn_pd__ * 1.94e-6f * (r__1 * r__1);

	}

	/*  RATIO OF PD TO THE AVERAGE PRESSURE, ASSUMED EQUAL TO 2 nPa: */
	rat = pd / 2.f;
	d__1 = (doublereal) rat;
	rat16 = pow_dd(&d__1, &c_b98);
	/* (THE POWER INDEX 0.14 IN THE SCALING FACTOR IS THE BEST-FIT VALUE OBTAINED FROM DATA */
	/*    AND USED IN THE T96_01 VERSION) */

	/*  VALUES OF THE MAGNETOPAUSE PARAMETERS FOR  PD = 2 nPa: */

	a0 = 70.f;
	s00 = 1.08f;
	x00 = 5.48f;

	/*   VALUES OF THE MAGNETOPAUSE PARAMETERS, SCALED BY THE ACTUAL PRESSURE: */

	a = a0 / rat16;
	s0 = s00;
	x0 = x00 / rat16;
	xm = x0 - a;

	/*  (XM IS THE X-COORDINATE OF THE "SEAM" BETWEEN THE ELLIPSOID AND THE CYLINDER) */

	/*     (FOR DETAILS OF THE ELLIPSOIDAL COORDINATES, SEE THE PAPER: */
	/*      N.A.TSYGANENKO, SOLUTION OF CHAPMAN-FERRARO PROBLEM FOR AN */
	/*      ELLIPSOIDAL MAGNETOPAUSE, PLANET.SPACE SCI., V.37, P.1037, 1989). */

	if (*ygsw != 0.f || *zgsw != 0.f) {
		phi = atan2(*ygsw, *zgsw);
	} else {
		phi = 0.f;
	}

	/* Computing 2nd power */
	r__1 = *ygsw;
	/* Computing 2nd power */
	r__2 = *zgsw;
	rho = sqrt(r__1 * r__1 + r__2 * r__2);

	if (*xgsw < xm) {
		*xmgnp = *xgsw;
		/* Computing 2nd power */
		r__1 = s0;
		rhomgnp = a * sqrt(r__1 * r__1 - 1);
		*ymgnp = rhomgnp * sin(phi);
		*zmgnp = rhomgnp * cos(phi);
		/* Computing 2nd power */
		r__1 = *xgsw - *xmgnp;
		/* Computing 2nd power */
		r__2 = *ygsw - *ymgnp;
		/* Computing 2nd power */
		r__3 = *zgsw - *zmgnp;
		*dist = sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3);
		if (rhomgnp > rho) {
			*id = 1;
		}
		if (rhomgnp <= rho) {
			*id = -1;
		}
		return 0;
	}

	xksi = (*xgsw - x0) / a + 1.f;
	xdzt = rho / a;
	/* Computing 2nd power */
	r__1 = xksi + 1.f;
	/* Computing 2nd power */
	r__2 = xdzt;
	sq1 = sqrt(r__1 * r__1 + r__2 * r__2);
	/* Computing 2nd power */
	r__1 = 1.f - xksi;
	/* Computing 2nd power */
	r__2 = xdzt;
	sq2 = sqrt(r__1 * r__1 + r__2 * r__2);
	sigma = (sq1 + sq2) * .5f;
	tau = (sq1 - sq2) * .5f;

	/*  NOW CALCULATE (X,Y,Z) FOR THE CLOSEST POINT AT THE MAGNETOPAUSE */

	*xmgnp = x0 - a * (1.f - s0 * tau);
	/* Computing 2nd power */
	r__1 = s0;
	/* Computing 2nd power */
	r__2 = tau;
	arg = (r__1 * r__1 - 1.f) * (1.f - r__2 * r__2);
	if (arg < 0.f) {
		arg = 0.f;
	}
	rhomgnp = a * sqrt(arg);
	*ymgnp = rhomgnp * sin(phi);
	*zmgnp = rhomgnp * cos(phi);

	/*  NOW CALCULATE THE DISTANCE BETWEEN THE POINTS {XGSW,YGSW,ZGSW} AND {XMGNP,YMGNP,ZMGNP}: */
	/*   (IN GENERAL, THIS IS NOT THE SHORTEST DISTANCE D_MIN, BUT DIST ASYMPTOTICALLY TENDS */
	/*    TO D_MIN, AS WE ARE GETTING CLOSER TO THE MAGNETOPAUSE): */

	/* Computing 2nd power */
	r__1 = *xgsw - *xmgnp;
	/* Computing 2nd power */
	r__2 = *ygsw - *ymgnp;
	/* Computing 2nd power */
	r__3 = *zgsw - *zmgnp;
	*dist = sqrt(r__1 * r__1 + r__2 * r__2 + r__3 * r__3);

	if (sigma > s0) {
		*id = -1;
	}
	/*  ID=-1 MEANS THAT THE POINT LIES OUTSID */
	if (sigma <= s0) {
		*id = 1;
	}
	/*                                           THE MAGNETOSPHERE */
	/*  ID=+1 MEANS THAT THE POINT LIES INSIDE */
	return 0;
} /* t96_mgnp_08__ */

