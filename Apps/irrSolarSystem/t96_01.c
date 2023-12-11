/* C:\Dokumente und Einstellungen\tobi\Eigene Dateien\Visual Studio 2005\Projects\irrSolarSystem\T96_01.f -- translated by f2c (version 20060506).
You must link the resulting object file with libf2c:
on Microsoft Windows system, link with libf2c.lib;
on Linux or Unix systems, link with .../path/to/libf2c.a -lm
or, if you install libf2c.a in a standard place, with -lf2c -lm
-- in that order, at the end of the command line, as in
cc *.o -lf2c -lm
Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Common Block Declarations */

union {
	struct {
		doublereal cpss, spss, dpsrr, rps, warp, d__, xs, zs, dxsx, dxsy, 
			dxsz, dzsx, dzsy, dzsz, dzetas, ddzetadx, ddzetady, ddzetadz, 
			zsww;
	} _1;
	struct {
		doublereal cpss, spss, dpsrr, xnext[3], xs, zswarped, dxsx, dxsy, 
			dxsz, dzsx, dzsywarped, dzsz, other[4], zs;
	} _2;
	struct {
		doublereal cpss, spss, dpsrr, xnext[3], xs, zs, dxsx, dxsy, dxsz, 
			other[3], dzetas, ddzetadx, ddzetady, ddzetadz, zsww;
	} _3;
	struct {
		doublereal first[3], rps, warp, d__, other[13];
	} _4;
} warp_;

#define warp_1 (warp_._1)
#define warp_2 (warp_._2)
#define warp_3 (warp_._3)
#define warp_4 (warp_._4)

struct coord11_1_ {
	doublereal xx1[12], yy1[12];
};
struct coord11_2_ {
	doublereal xx[12], yy[12];
};

#define coord11_1 (*(struct coord11_1_ *) &coord11_)
#define coord11_2 (*(struct coord11_2_ *) &coord11_)

struct rhdr_1_ {
	doublereal rh, dr;
};

#define rhdr_1 (*(struct rhdr_1_ *) &rhdr_)

struct loopdip1_1_ {
	doublereal tilt, xcentre[2], radius[2], dipx, dipy;
};

#define loopdip1_1 (*(struct loopdip1_1_ *) &loopdip1_)

struct coord21_1_ {
	doublereal xx2[14], yy2[14], zz2[14];
};
struct coord21_2_ {
	doublereal xx[14], yy[14], zz[14];
};

#define coord21_1 (*(struct coord21_1_ *) &coord21_)
#define coord21_2 (*(struct coord21_2_ *) &coord21_)

struct dx1_1_ {
	doublereal dx, scalein, scaleout;
};

#define dx1_1 (*(struct dx1_1_ *) &dx1_)

/* Initialized data */

struct {
	doublereal e_1[24];
} coord11_ = { -11., -7., -7., -3., -3., 1., 1., 1., 5., 5., 9., 9., 2., 
0., 4., 2., 6., 0., 4., 8., 2., 6., 0., 4. };

struct {
	doublereal e_1[2];
} rhdr_ = { 9., 4. };

struct {
	doublereal e_1[7];
} loopdip1_ = { 1.00891, 2.28397, -5.60831, 1.86106, 7.83281, 1.12541, 
.945719 };

struct {
	doublereal e_1[42];
} coord21_ = { -10., -7., -4., -4., 0., 4., 4., 7., 10., 0., 0., 0., 0., 
0., 3., 6., 3., 9., 6., 3., 9., 6., 3., 0., 0., 0., 0., 0., 20., 
20., 4., 20., 4., 4., 20., 20., 20., 2., 3., 4.5, 7., 10. };

struct {
	doublereal e_1[3];
} dx1_ = { -.16, .08, .4 };


/* Table of constant values */

static doublereal c_b3 = .14;
static integer c__0 = 0;
static integer c__1 = 1;
static doublereal c_b35 = 1.;
static doublereal c_b47 = .1666666667;
static integer c__5 = 5;

/* The FORTRAN source code T96_01.FOR is the last released version (June 22, 1996) */
/* of a new data-based model of the geomagnetospheric magnetic field with an ex- */
/* plicitly defined realistic magnetopause, large-scale Region 1 and 2 Birkeland */
/* current systems, and the IMF penetration  across the boundary. */

/* The file T96_01.FOR contains a set of 33 subroutines and functions. The first */
/* subroutine, named T96_01, is the primary one, accepting the input values of the */
/* solar wind pressure, Dst-index, By- and Bz-components of the interplanetary */
/* magnetic field, the geodipole tilt angle, and GSM position of the observation */
/* point (X,Y,Z). The subroutine returns GSM components of the external field */
/* (i.e., the total vector of B minus the Earth's contribution).   The remaining */
/* 32 subroutines are invoked by T96_01. */



/* The source code T96_01.FOR differs in several aspects from the previous version */
/* T95_06.FOR (released in November 1995). */

/* (1) The T96_01 does not include the AE-index as an input parameter. */

/* (2) The number of terms in the ring current, tail modes, and in the shielding */
/* fields was reduced to a necessary minimum, in order to increase the speed of the */
/* code. The tail field is now a sum of two modes; the first one is responsible */
/* for the near-Earth tail field and is similar to that in the previous version. */
/* The second mode provides the asymptotic field in the far magnetotail. */

/* (3) The way of representing the effects of the dipole tilt on the tail/ring */
/* current field was revised:  instead of a "shear" transformation (introduced in */
/* the T89 model), a radially dependent "space-warping" is used in this model, */
/* which decreases tilt-induced spurious currents. */

/* (4) The representation for the Region 2 Birkeland current field was completely */
/* revised:  in the present version, a smooth approximation was developed for */
/* the field inside the current layer. As a result, unphysical kinks in the Bz */
/* profile on the nightside were eliminated. */



/*             ******************************************* */
/*             | Users should be aware of the following. | */
/*             ******************************************* */


/*   (1) A simple linear dependence of the amplitudes of the field sources on the */
/* SQRT(Pdyn), Dst, and the IMF-related parameter EPS=SQRT(N)*V*Bt*sin(theta/2) */
/* was employed.  Hence, the best results should be expected near the most probable */
/* values of the input parameters,  corresponding to the regions in the Pdyn-Dst- */
/* ByIMF-BzIMF space with the highest density of the spacecraft measurements. For */
/* the same reason, caution is needed in modeling situations with unusually low or */
/* high values of these parameters: extrapolating the model too far beyond the */
/* range of reliable approximation can lead to unrealistic results.  As a rough */
/* estimate, the parameter values should remain within the intervals: */
/* Pdyn:  between 0.5 and 10 nPa, */
/* Dst:  between -100 and +20, */
/* ByIMF and BzIMF: between -10 and +10 nT. */

/*   (2) The only parameter which controls the size of the model magnetopause is */
/* the solar wind ram pressure Pdyn. No IMF dependence has been introduced so far */
/* in the magnetopause shape/size.  This is planned to be done in future versions */
/* of the model. */
/*      To facilitate using the model, we provide users with two supplementary */
/* FORTRAN subroutines, named LOCATE and CROSSING.  The first one finds the point */
/* on the model magnetopause which is closest to a given point of space, for any */
/* values of the solar wind density and velocity (or, optionally, solar wind */
/* pressure).  The second subroutine estimates the current value of the solar wind */
/* ram pressure, based on the observed GSM position of the magnetopause at any */
/* local time or latitude, sunward from XGSM=-60Re. */

/*   (3) In its present form, the subroutine T96_01 is compatible with new version */
/* (April 16, 1996) of the software package GEOPACK for coordinate transformation */
/* and line tracing, which replaces the old version and is available from the same */
/* WWW site. */

/*   (4) This is not a "final version":  the model is supposed to be further */
/* improved and upgraded in the future. In this regard, any kind of feedback from */
/* the users is very important for us and will be greatly appreciated. In */
/* particular, it is very important that any problems encountered in adapting and */
/* using the code be reported to us as soon as possible.  Please send your */
/* questions and comments to the address given in the end of this file. */

/*   (5) More details on the approach used in devising this model can be found in */
/* the following publications: */


/*       Tsyganenko, N.A. and M. Peredo, Analytical models of the magnetic field */
/*         of disk-shaped current sheets,  J.Geophys.Res., v.99, pp.199-205, 1994. */

/*       Tsyganenko, N.A., Modeling the Earth's magnetospheric magnetic field */
/*         confined within a realistic magnetopause, J.Geophys.Res., v.100, */
/*         pp.5599-5612, 1995. */

/*       Fairfield, D.H., N.A. Tsyganenko, A.V. Usmanov, and M.V. Malkov, A large */
/*         magnetosphere magnetic field database, J.Geophys.Res., v.99, */
/*         pp.11319-11326, 1994. */

/*       Tsyganenko, N.A. and D.P. Stern, Modeling the global magnetic field */
/*         the large-scale Birkeland current systems, J.Geophys.Res., v.101, */
/*         p.27187-27198, 1996. */

/*       Tsyganenko, N.A., Effects of the solar wind conditions on the global */
/*          magnetospheric configuration as deduced from data-based field */
/*          models, in:  Proc.of 3rd International Conference on Substorms */
/*          (ICS-3), Versailles, France, 12-17 May 1996, ESA SP-389, p.181-185, */
/*          1996. */

/*        (A PostScript file of the last paper, named versail.ps, can be ftp-ed */
/*        from anonymous ftp-area at:   www-spof.gsfc.nasa.gov;   /pub/kolya) */


/*    Please send your questions, comments, and requests to: */

/*    Nikolai Tsyganenko */

/*    E-address:        kolya@nssdca.gsfc.nasa.gov */

/*    Mail address:     Code 695,  NASA GSFC */
/*                      Greenbelt, MD 20771, */
/*                      U.S.A. */

/*     Phone: (301)-286-7925 */
/*     Fax:   (301)-286-1629 */





/* ---------------------------------------------------------------------- */

/* Subroutine */ int t96_01__(integer *iopt, real *parmod, real *ps, real *x, 
							  real *y, real *z__, real *bx, real *by, real *bz)
{
	/* Initialized data */

	static doublereal pdyn0 = 2.;
	static doublereal eps10 = 3630.7;
	static real a[9] = { 1.162f,22.344f,18.5f,2.602f,6.903f,5.287f,.579f,
		.4462f,.785f };
	static doublereal am0 = 70.;
	static doublereal s0 = 1.08;
	static doublereal x00 = 5.48;
	static doublereal dsig = .005;
	static doublereal delimfx = 20.;
	static doublereal delimfy = 10.;

	/* System generated locals */
	real r__1, r__2;
	doublereal d__1, d__2;

	/* Builtin functions */
	double sin(doublereal), sqrt(doublereal), atan2(doublereal, doublereal), 
		cos(doublereal), pow_dd(doublereal *, doublereal *), exp(
		doublereal);

	/* Local variables */
	static doublereal rimfampl;
	extern /* Subroutine */ int intercon_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *);
	static doublereal x0, am;
	extern /* Subroutine */ int birk1tot_02__(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *), birk2tot_02__(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *);
	static doublereal bt, ct, fx, fy, fz, st;
	static real qx, qy, qz;
	static doublereal ys, zs, xx, yy, zz, r1x, r1y, r1z, r2x, r2y, r2z, cfx, 
		aro, cfy, cfz, asq, eps;
	static real dst;
	static doublereal pps, sps, rho2, bxt2, axx0, byt2, bzt2, bxt3, byt3, 
		bzt3, depr, bxrc, byrc, bzrc, fint, fext;
	static real pdyn;
	static doublereal xmxm, sigma, theta;
	static real byimf, bzimf;
	static doublereal xappa, oimfx, oimfy, oimfz, rimfx, rimfy, rimfz, b1ampl,
		b2ampl, xappa3, tampl2, tampl3, factpd;
	extern /* Subroutine */ int dipole_(real *, real *, real *, real *, real *
		, real *, real *);
	static doublereal rcampl, reconn, rimfys, rimfzs, factimf, facteps;
	extern /* Subroutine */ int dipshld_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *), tailrc96_(doublereal *, doublereal *, doublereal *,
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *);


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

	/* Parameter adjustments */
	--parmod;

	/* Function Body */



	pdyn = parmod[1];
	dst = parmod[2];
	byimf = parmod[3];
	bzimf = parmod[4];

	sps = sin(*ps);
	pps = *ps;

	depr = dst * .8f - sqrt(pdyn) * 13.f;
	/*                                         depression, based on DST and Pdyn */
	/*                                             (usually, DEPR < 0 ) */

	/*  CALCULATE THE IMF-RELATED QUANTITIES: */

	/*  DEPR is an estimate of total near */
	/* Computing 2nd power */
	r__1 = byimf;
	/* Computing 2nd power */
	r__2 = bzimf;
	bt = sqrt(r__1 * r__1 + r__2 * r__2);
	if (byimf == 0.f && bzimf == 0.f) {
		theta = 0.f;
		goto L1;
	}

	theta = atan2(byimf, bzimf);
	if (theta <= 0.) {
		theta += 6.2831853f;
	}
L1:
	ct = cos(theta);
	st = sin(theta);
	eps = sqrt(pdyn) * 718.5f * bt * sin(theta / 2.f);

	facteps = eps / eps10 - 1.f;
	factpd = sqrt(pdyn / pdyn0) - 1.f;

	rcampl = -a[0] * depr;
	/*                  (positive and equal to abs.value of RC depression at origin) */

	/*   RCAMPL is the amplitude of the ring cur */
	tampl2 = a[1] + a[2] * factpd + a[3] * facteps;
	tampl3 = a[4] + a[5] * factpd;
	b1ampl = a[6] + a[7] * facteps;
	b2ampl = b1ampl * 20.f;
	/*                           IN THE REGION 2 SYSTEM IS 40% OF THAT IN REGION 1 */
	/* IT IS EQUIVALENT TO ASSUMING THAT THE TOTAL */
	reconn = a[8];

	d__1 = pdyn / pdyn0;
	xappa = pow_dd(&d__1, &c_b3);
	/* Computing 3rd power */
	d__1 = xappa;
	xappa3 = d__1 * (d__1 * d__1);
	ys = *y * ct - *z__ * st;
	zs = *z__ * ct + *y * st;

	/* Computing 2nd power */
	d__1 = ys / delimfy;
	factimf = exp(*x / delimfx - d__1 * d__1);

	/*  CALCULATE THE "IMF" COMPONENTS OUTSIDE THE LAYER  (HENCE BEGIN WITH "O") */

	oimfx = 0.f;
	oimfy = reconn * byimf * factimf;
	oimfz = reconn * bzimf * factimf;

	rimfampl = reconn * bt;

	pps = *ps;
	xx = *x * xappa;
	yy = *y * xappa;
	zz = *z__ * xappa;

	/*  SCALE AND CALCULATE THE MAGNETOPAUSE PARAMETERS FOR THE INTERPOLATION ACROSS */
	/*   THE BOUNDARY LAYER (THE COORDINATES XX,YY,ZZ  ARE ALREADY SCALED) */

	x0 = x00 / xappa;
	am = am0 / xappa;
	/* Computing 2nd power */
	r__1 = *y;
	/* Computing 2nd power */
	r__2 = *z__;
	rho2 = r__1 * r__1 + r__2 * r__2;
	/* Computing 2nd power */
	d__1 = am;
	asq = d__1 * d__1;
	xmxm = am + *x - x0;
	if (xmxm < 0.f) {
		xmxm = 0.f;
	}
	/* THE BOUNDARY IS A CYLINDER TAILWARD OF */
	/* Computing 2nd power */
	d__1 = xmxm;
	axx0 = d__1 * d__1;
	aro = asq + rho2;
	/* Computing 2nd power */
	d__1 = aro + axx0;
	sigma = sqrt((aro + axx0 + sqrt(d__1 * d__1 - asq * 4.f * axx0)) / (asq * 
		2.f));

	/*   NOW, THERE ARE THREE POSSIBLE CASES: */
	/*    (1) INSIDE THE MAGNETOSPHERE */
	/*    (2) IN THE BOUNDARY LAYER */
	/*    (3) OUTSIDE THE MAGNETOSPHERE AND B.LAYER */
	/*       FIRST OF ALL, CONSIDER THE CASES (1) AND (2): */

	if (sigma < s0 + dsig) {
		/*                                POTENTIAL "PENETRATED" INTERCONNECTION FIELD): */
		/*  CALCULATE THE T95_06 FIELD (WITH TH */
		dipshld_(&pps, &xx, &yy, &zz, &cfx, &cfy, &cfz);
		tailrc96_(&sps, &xx, &yy, &zz, &bxrc, &byrc, &bzrc, &bxt2, &byt2, &
			bzt2, &bxt3, &byt3, &bzt3);
		birk1tot_02__(&pps, &xx, &yy, &zz, &r1x, &r1y, &r1z);
		birk2tot_02__(&pps, &xx, &yy, &zz, &r2x, &r2y, &r2z);
		d__1 = ys * xappa;
		d__2 = zs * xappa;
		intercon_(&xx, &d__1, &d__2, &rimfx, &rimfys, &rimfzs);
		rimfy = rimfys * ct + rimfzs * st;
		rimfz = rimfzs * ct - rimfys * st;

		fx = cfx * xappa3 + rcampl * bxrc + tampl2 * bxt2 + tampl3 * bxt3 + 
			b1ampl * r1x + b2ampl * r2x + rimfampl * rimfx;
		fy = cfy * xappa3 + rcampl * byrc + tampl2 * byt2 + tampl3 * byt3 + 
			b1ampl * r1y + b2ampl * r2y + rimfampl * rimfy;
		fz = cfz * xappa3 + rcampl * bzrc + tampl2 * bzt2 + tampl3 * bzt3 + 
			b1ampl * r1z + b2ampl * r2z + rimfampl * rimfz;

		/*  NOW, LET US CHECK WHETHER WE HAVE THE CASE (1). IF YES - WE ARE DONE: */

		if (sigma < s0 - dsig) {
			*bx = fx;
			*by = fy;
			*bz = fz;
		} else {
			/*                                         THE INTERPOLATION REGION */
			/*  THIS IS THE MOST COMPLEX CASE: WE ARE I */
			fint = (1.f - (sigma - s0) / dsig) * .5f;
			fext = ((sigma - s0) / dsig + 1.f) * .5f;

			dipole_(ps, x, y, z__, &qx, &qy, &qz);
			*bx = (fx + qx) * fint + oimfx * fext - qx;
			*by = (fy + qy) * fint + oimfy * fext - qy;
			*bz = (fz + qz) * fint + oimfz * fext - qz;

		}
		/*                      POSSIBILITY IS NOW THE CASE (3): */
		/*   THE CASES (1) AND (2) ARE EXHAUSTED; THE ONLY REMAINI */
	} else {
		dipole_(ps, x, y, z__, &qx, &qy, &qz);
		*bx = oimfx - qx;
		*by = oimfy - qy;
		*bz = oimfz - qz;
	}

	return 0;
} /* t96_01__ */

/* ===================================================================== */
/* Subroutine */ int dipshld_(doublereal *ps, doublereal *x, doublereal *y, 
							  doublereal *z__, doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal a1[12] = { .24777,-27.003,-.46815,7.0637,-1.5918,
		-.090317,57.522,13.757,2.01,10.458,4.5798,2.1695 };
	static doublereal a2[12] = { -.65385,-18.061,-.40457,-5.0995,1.2846,
		.078231,39.592,13.291,1.997,10.062,4.514,2.1558 };

	/* Builtin functions */
	double cos(doublereal), sin(doublereal);

	/* Local variables */
	static doublereal fx, fy, hx, hy, hz, fz, cps, sps;
	extern /* Subroutine */ int cylhar1_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *), cylharm_(doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *);


	/*   CALCULATES GSM COMPONENTS OF THE EXTERNAL MAGNETIC FIELD DUE TO */
	/*    SHIELDING OF THE EARTH'S DIPOLE ONLY */


	cps = cos(*ps);
	sps = sin(*ps);
	cylharm_(a1, x, y, z__, &hx, &hy, &hz);
	cylhar1_(a2, x, y, z__, &fx, &fy, &fz);

	*bx = hx * cps + fx * sps;
	*by = hy * cps + fy * sps;
	*bz = hz * cps + fz * sps;
	return 0;
} /* dipshld_ */


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  THIS CODE YIELDS THE SHIELDING FIELD FOR THE PERPENDICULAR DIPOLE */

/* Subroutine */ int cylharm_(doublereal *a, doublereal *x, doublereal *y, 
							  doublereal *z__, doublereal *bx, doublereal *by, doublereal *bz)
{
	/* System generated locals */
	doublereal d__1, d__2;

	/* Builtin functions */
	double sqrt(doublereal), exp(doublereal);

	/* Local variables */
	static integer i__;
	static doublereal xj0, xj1;
	extern doublereal bes_(doublereal *, integer *);
	static doublereal rho, bphi, brho, xksi, xexp, si2co2, cosfi, dzeta, 
		sinfi, sinfi2;



	/*   ***  N.A. Tsyganenko ***  Sept. 14-18, 1993; revised March 16, 1994 *** */

	/*   An approximation for the Chapman-Ferraro field by a sum of 6 cylin- */
	/*   drical harmonics (see pp. 97-113 in the brown GSFC notebook #1) */

	/*      Description of parameters: */

	/*  A   - input vector containing model parameters; */
	/*  X,Y,Z   -  input GSM coordinates */
	/*  BX,BY,BZ - output GSM components of the shielding field */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*  The 6 linear parameters A(1)-A(6) are amplitudes of the cylindrical harmonic */
	/*       terms. */
	/*  The 6 nonlinear parameters A(7)-A(12) are the corresponding scale lengths */
	/*       for each term (see GSFC brown notebook). */

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



	/* Parameter adjustments */
	--a;

	/* Function Body */
	/* Computing 2nd power */
	d__1 = *y;
	/* Computing 2nd power */
	d__2 = *z__;
	rho = sqrt(d__1 * d__1 + d__2 * d__2);
	if (rho < 1e-8) {
		sinfi = 1.;
		cosfi = 0.;
		rho = 1e-8;
		goto L1;
	}

	sinfi = *z__ / rho;
	cosfi = *y / rho;
L1:
	/* Computing 2nd power */
	d__1 = sinfi;
	sinfi2 = d__1 * d__1;
	/* Computing 2nd power */
	d__1 = cosfi;
	si2co2 = sinfi2 - d__1 * d__1;

	*bx = 0.;
	*by = 0.;
	*bz = 0.;

	for (i__ = 1; i__ <= 3; ++i__) {
		dzeta = rho / a[i__ + 6];
		xj0 = bes_(&dzeta, &c__0);
		xj1 = bes_(&dzeta, &c__1);
		xexp = exp(*x / a[i__ + 6]);
		*bx -= a[i__] * xj1 * xexp * sinfi;
		*by += a[i__] * (xj1 * 2. / dzeta - xj0) * xexp * sinfi * cosfi;
		*bz += a[i__] * (xj1 / dzeta * si2co2 - xj0 * sinfi2) * xexp;
		/* L11: */
	}

	for (i__ = 4; i__ <= 6; ++i__) {
		dzeta = rho / a[i__ + 6];
		xksi = *x / a[i__ + 6];
		xj0 = bes_(&dzeta, &c__0);
		xj1 = bes_(&dzeta, &c__1);
		xexp = exp(xksi);
		/* Computing 2nd power */
		d__1 = dzeta;
		brho = (xksi * xj0 - (d__1 * d__1 + xksi - 1.) * xj1 / dzeta) * xexp *
			sinfi;
		bphi = (xj0 + xj1 / dzeta * (xksi - 1.)) * xexp * cosfi;
		*bx += a[i__] * (dzeta * xj0 + xksi * xj1) * xexp * sinfi;
		*by += a[i__] * (brho * cosfi - bphi * sinfi);
		*bz += a[i__] * (brho * sinfi + bphi * cosfi);
		/* L12: */
	}


	return 0;
} /* cylharm_ */


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*  THIS CODE YIELDS THE SHIELDING FIELD FOR THE PARALLEL DIPOLE */

/* Subroutine */ int cylhar1_(doublereal *a, doublereal *x, doublereal *y, 
							  doublereal *z__, doublereal *bx, doublereal *by, doublereal *bz)
{
	/* System generated locals */
	doublereal d__1, d__2;

	/* Builtin functions */
	double sqrt(doublereal), exp(doublereal);

	/* Local variables */
	static integer i__;
	static doublereal xj0, xj1;
	extern doublereal bes_(doublereal *, integer *);
	static doublereal rho, brho, xksi, xexp, cosfi, dzeta, sinfi;



	/*   ***  N.A. Tsyganenko ***  Sept. 14-18, 1993; revised March 16, 1994 *** */

	/*   An approximation of the Chapman-Ferraro field by a sum of 6 cylin- */
	/*   drical harmonics (see pages 97-113 in the brown GSFC notebook #1) */

	/*      Description of parameters: */

	/*  A   - input vector containing model parameters; */
	/*  X,Y,Z - input GSM coordinates, */
	/*  BX,BY,BZ - output GSM components of the shielding field */

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	/*      The 6 linear parameters A(1)-A(6) are amplitudes of the cylindrical */
	/*  harmonic terms. */
	/*      The 6 nonlinear parameters A(7)-A(12) are the corresponding scale */
	/*  lengths for each term (see GSFC brown notebook). */

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



	/* Parameter adjustments */
	--a;

	/* Function Body */
	/* Computing 2nd power */
	d__1 = *y;
	/* Computing 2nd power */
	d__2 = *z__;
	rho = sqrt(d__1 * d__1 + d__2 * d__2);
	if (rho < 1e-10) {
		sinfi = 1.;
		cosfi = 0.;
		goto L1;
	}

	sinfi = *z__ / rho;
	cosfi = *y / rho;

L1:
	*bx = 0.;
	*by = 0.;
	*bz = 0.;

	for (i__ = 1; i__ <= 3; ++i__) {
		dzeta = rho / a[i__ + 6];
		xksi = *x / a[i__ + 6];
		xj0 = bes_(&dzeta, &c__0);
		xj1 = bes_(&dzeta, &c__1);
		xexp = exp(xksi);
		brho = xj1 * xexp;
		*bx -= a[i__] * xj0 * xexp;
		*by += a[i__] * brho * cosfi;
		*bz += a[i__] * brho * sinfi;
		/* L11: */
	}

	for (i__ = 4; i__ <= 6; ++i__) {
		dzeta = rho / a[i__ + 6];
		xksi = *x / a[i__ + 6];
		xj0 = bes_(&dzeta, &c__0);
		xj1 = bes_(&dzeta, &c__1);
		xexp = exp(xksi);
		brho = (dzeta * xj0 + xksi * xj1) * xexp;
		*bx += a[i__] * (dzeta * xj1 - xj0 * (xksi + 1.)) * xexp;
		*by += a[i__] * brho * cosfi;
		*bz += a[i__] * brho * sinfi;
		/* L12: */
	}

	return 0;
} /* cylhar1_ */

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

doublereal bes_(doublereal *x, integer *k)
{
	/* System generated locals */
	doublereal ret_val;

	/* Local variables */
	static doublereal g;
	static integer n;
	static doublereal xjn, sum;
	extern doublereal bes0_(doublereal *), bes1_(doublereal *);
	static doublereal xjnm1, xjnp1;


	if (*k == 0) {
		ret_val = bes0_(x);
		return ret_val;
	}

	if (*k == 1) {
		ret_val = bes1_(x);
		return ret_val;
	}

	if (*x == 0.) {
		ret_val = 0.;
		return ret_val;
	}

	g = 2. / *x;
	if (*x <= (doublereal) (*k)) {
		goto L10;
	}

	n = 1;
	xjn = bes1_(x);
	xjnm1 = bes0_(x);

L1:
	xjnp1 = g * n * xjn - xjnm1;
	++n;
	if (n < *k) {
		goto L2;
	}
	ret_val = xjnp1;
	return ret_val;

L2:
	xjnm1 = xjn;
	xjn = xjnp1;
	goto L1;

L10:
	n = 24;
	xjn = 1.;
	xjnp1 = 0.;
	sum = 0.;

L3:
	if (n % 2 == 0) {
		sum += xjn;
	}
	xjnm1 = g * n * xjn - xjnp1;
	--n;

	xjnp1 = xjn;
	xjn = xjnm1;
	if (n == *k) {
		ret_val = xjn;
	}

	if (abs(xjn) > 1e5) {
		xjnp1 *= 1e-5;
		xjn *= 1e-5;
		sum *= 1e-5;
		if (n <= *k) {
			ret_val *= 1e-5;
		}
	}

	if (n == 0) {
		goto L4;
	}
	goto L3;

L4:
	sum = xjn + sum * 2.;
	ret_val /= sum;
	return ret_val;
} /* bes_ */

/* ------------------------------------------------------------------- */

doublereal bes0_(doublereal *x)
{
	/* System generated locals */
	doublereal ret_val, d__1;

	/* Builtin functions */
	double sqrt(doublereal), cos(doublereal);

	/* Local variables */
	static doublereal f0, t0, x32, xd3;



	if (abs(*x) < 3.) {
		/* Computing 2nd power */
		d__1 = *x / 3.;
		x32 = d__1 * d__1;
		ret_val = 1. - x32 * (2.2499997 - x32 * (1.2656208 - x32 * (.3163866 
			- x32 * (.0444479 - x32 * (.0039444 - x32 * 2.1e-4)))));
	} else {
		xd3 = 3. / *x;
		f0 = .79788456 - xd3 * (xd3 * (xd3 * (9.512e-5 - xd3 * (.00137237 - 
			xd3 * (7.2805e-4 - xd3 * 1.4476e-4))) + .0055274) + 7.7e-7);
		t0 = *x - .78539816 - xd3 * (xd3 * (3.954e-5 - xd3 * (.00262573 - xd3 
			* (xd3 * (2.9333e-4 - xd3 * 1.3558e-4) + 5.4125e-4))) + 
			.04166397);
		ret_val = f0 / sqrt(*x) * cos(t0);
	}
	return ret_val;
} /* bes0_ */


/* -------------------------------------------------------------------------- */

doublereal bes1_(doublereal *x)
{
	/* System generated locals */
	doublereal ret_val, d__1;

	/* Builtin functions */
	double sqrt(doublereal), cos(doublereal);

	/* Local variables */
	static doublereal f1, t1, x32, xd3, bes1xm1;



	if (abs(*x) < 3.) {
		/* Computing 2nd power */
		d__1 = *x / 3.;
		x32 = d__1 * d__1;
		bes1xm1 = .5 - x32 * (.56249985 - x32 * (.21093573 - x32 * (.03954289 
			- x32 * (.00443319 - x32 * (3.1761e-4 - x32 * 1.109e-5)))));
		ret_val = bes1xm1 * *x;
	} else {
		xd3 = 3. / *x;
		f1 = xd3 * (xd3 * (xd3 * (1.7105e-4 - xd3 * (.00249511 - xd3 * (
			.00113653 - xd3 * 2.0033e-4))) + .01659667) + 1.56e-6) + 
			.79788456;
		t1 = *x - 2.35619449 + xd3 * (xd3 * (5.65e-5 - xd3 * (.00637879 - xd3 
			* (xd3 * (7.9824e-4 - xd3 * 2.9166e-4) + 7.4348e-4))) + 
			.12499612);
		ret_val = f1 / sqrt(*x) * cos(t1);
	}
	return ret_val;
} /* bes1_ */

/* ------------------------------------------------------------ */

/* Subroutine */ int intercon_(doublereal *x, doublereal *y, doublereal *z__, 
							   doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal a[15] = { -8.411078731,5932254.951,-9073284.93,
		-11.68794634,6027598.824,-9218378.368,-6.508798398,-11824.42793,
		18015.66212,7.99754043,13.9669886,90.24475036,16.75728834,
		1015.645781,1553.493216 };
	static integer m = 0;

	/* System generated locals */
	doublereal d__1, d__2;

	/* Builtin functions */
	double cos(doublereal), sin(doublereal), sqrt(doublereal), exp(doublereal)
		;

	/* Local variables */
	static integer i__, k, l;
	static doublereal p[3], r__[3], hx, hy, rp[3], hz, rr[3], epr, cypi, czrk,
		sypi, sqpr, szrk;


	/*      Calculates the potential interconnection field inside the magnetosphere, */
	/*  corresponding to  DELTA_X = 20Re and DELTA_Y = 10Re (NB#3, p.90, 6/6/1996). */
	/*  The position (X,Y,Z) and field components BX,BY,BZ are given in the rotated */
	/*   coordinate system, in which the Z-axis is always directed along the BzIMF */
	/*   (i.e. rotated by the IMF clock angle Theta) */
	/*   It is also assumed that the IMF Bt=1, so that the components should be */
	/*     (i) multiplied by the actual Bt, and */
	/*     (ii) transformed to standard GSM coords by rotating back around X axis */
	/*              by the angle -Theta. */

	/*      Description of parameters: */

	/*     X,Y,Z -   GSM POSITION */
	/*      BX,BY,BZ - INTERCONNECTION FIELD COMPONENTS INSIDE THE MAGNETOSPHERE */
	/*        OF A STANDARD SIZE (TO TAKE INTO ACCOUNT EFFECTS OF PRESSURE CHANGES, */
	/*         APPLY THE SCALING TRANSFORMATION) */

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	/*     The 9 linear parameters are amplitudes of the "cartesian" harmonics */
	/*     The 6 nonlinear parameters are the scales Pi and Ri entering */
	/*    the arguments of exponents, sines, and cosines in the 9 "Cartesian" */
	/*       harmonics (3+3) */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */





	if (m != 0) {
		goto L111;
	}
	m = 1;

	p[0] = a[9];
	p[1] = a[10];
	p[2] = a[11];
	r__[0] = a[12];
	r__[1] = a[13];
	r__[2] = a[14];


	for (i__ = 1; i__ <= 3; ++i__) {
		rp[i__ - 1] = 1. / p[i__ - 1];
		/* L11: */
		rr[i__ - 1] = 1. / r__[i__ - 1];
	}

L111:

	l = 0;

	*bx = 0.f;
	*by = 0.f;
	*bz = 0.f;

	/*        "PERPENDICULAR" KIND OF SYMMETRY ONLY */

	for (i__ = 1; i__ <= 3; ++i__) {
		cypi = cos(*y * rp[i__ - 1]);
		sypi = sin(*y * rp[i__ - 1]);

		for (k = 1; k <= 3; ++k) {
			szrk = sin(*z__ * rr[k - 1]);
			czrk = cos(*z__ * rr[k - 1]);
			/* Computing 2nd power */
			d__1 = rp[i__ - 1];
			/* Computing 2nd power */
			d__2 = rr[k - 1];
			sqpr = sqrt(d__1 * d__1 + d__2 * d__2);
			epr = exp(*x * sqpr);

			hx = -sqpr * epr * cypi * szrk;
			hy = rp[i__ - 1] * epr * sypi * szrk;
			hz = -rr[k - 1] * epr * cypi * czrk;
			++l;

			*bx += a[l - 1] * hx;
			*by += a[l - 1] * hy;
			*bz += a[l - 1] * hz;
			/* L2: */
		}
	}

	return 0;
} /* intercon_ */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Subroutine */ int tailrc96_(doublereal *sps, doublereal *x, doublereal *y, 
							   doublereal *z__, doublereal *bxrc, doublereal *byrc, doublereal *bzrc,
							   doublereal *bxt2, doublereal *byt2, doublereal *bzt2, doublereal *
							   bxt3, doublereal *byt3, doublereal *bzt3)
{
	/* Initialized data */

	static doublereal arc[48] = { -3.087699646,3.516259114,18.81380577,
		-13.95772338,-5.497076303,.1712890838,2.392629189,-2.728020808,
		-14.79349936,11.08738083,4.388174084,.02492163197,.7030375685,
		-.7966023165,-3.835041334,2.642228681,-.2405352424,-.7297705678,
		-.3680255045,.1333685557,2.795140897,-1.078379954,.801402863,
		.1245825565,.6149982835,-.2207267314,-4.424578723,1.730471572,
		-1.716313926,-.2306302941,-.2450342688,.08617173961,1.54697858,
		-.6569391113,-.6537525353,.2079417515,12.75434981,11.37659788,
		636.4346279,1.752483754,3.604231143,12.83078674,7.412066636,
		9.434625736,676.7557193,1.701162737,3.580307144,14.64298662 };
	static doublereal atail2[48] = { .8747515218,-.9116821411,2.209365387,
		-2.159059518,-7.059828867,5.924671028,-1.916935691,1.996707344,
		-3.877101873,3.947666061,11.38715899,-8.343210833,1.194109867,
		-1.244316975,3.73895491,-4.406522465,-20.66884863,3.020952989,
		.2189908481,-.09942543549,-.927225562,.1555224669,.6994137909,
		-.08111721003,-.7565493881,.4686588792,4.266058082,-.3717470262,
		-3.920787807,.0229856987,.7039506341,-.5498352719,-6.675140817,
		.8279283559,-2.234773608,-1.622656137,5.187666221,6.802472048,
		39.13543412,2.784722096,6.979576616,25.7171676,4.495005873,
		8.068408272,93.47887103,4.158030104,9.313492566,57.18240483 };
	static doublereal atail3[48] = { -19091.95061,-3011.613928,20582.16203,
		4242.91843,-2377.091102,-1504.820043,19884.0465,2725.150544,
		-21389.04845,-3990.475093,2401.610097,1548.171792,-946.5493963,
		490.1528941,986.9156625,-489.326593,-67.99278499,8.71117571,
		-45.1573426,-10.761065,210.7927312,11.41764141,-178.0262808,
		.7558830028,339.3806753,9.904695974,69.50583193,-118.0271581,
		22.85935896,45.91014857,-425.6607164,15.47250738,118.2988915,
		65.58594397,-201.4478068,-14.5706294,19.6987797,20.3009568,
		86.4540742,22.50403727,23.41617329,48.48140573,24.61031329,
		123.5395974,223.5367692,39.50824342,65.83385762,266.2948657 };
	static doublereal rh = 9.;
	static doublereal dr = 4.;
	static doublereal g = 10.;
	static doublereal d0 = 2.;
	static doublereal deltady = 10.;

	/* System generated locals */
	doublereal d__1, d__2;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	extern /* Subroutine */ int taildisk_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *), 
		shlcar3x3_(doublereal *, doublereal *, doublereal *, doublereal *,
		doublereal *, doublereal *, doublereal *, doublereal *);
	static doublereal c__, r__, w, c1;
	extern /* Subroutine */ int ringcurr96_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *);
	static doublereal c11, c12, cs, hx, hy, hz, ws, wx, wy, wz, dr2, sq1, sq2,
		wfac, dddy, spsc1;
	extern /* Subroutine */ int tail87_(doublereal *, doublereal *, 
		doublereal *, doublereal *);


	/*  COMPUTES THE COMPONENTS OF THE FIELD OF THE MODEL RING CURRENT AND THREE */
	/*                   TAIL MODES WITH UNIT AMPLITUDES */
	/*      (FOR THE RING CURRENT, IT MEANS THE DISTURBANCE OF Bz=-1nT AT ORIGIN, */
	/*   AND FOR THE TAIL MODES IT MEANS MAXIMAL BX JUST ABOVE THE SHEET EQUAL 1 nT. */






	/*   TO ECONOMIZE THE CODE, WE FIRST CALCULATE COMMON VARIABLES, WHICH ARE */
	/*      THE SAME FOR ALL MODES, AND PUT THEM IN THE COMMON-BLOCK /WARP/ */

	dr2 = dr * dr;
	/* Computing 2nd power */
	d__1 = rh + 1.;
	c11 = sqrt(d__1 * d__1 + dr2);
	/* Computing 2nd power */
	d__1 = 1. - rh;
	c12 = sqrt(d__1 * d__1 + dr2);
	c1 = c11 - c12;
	spsc1 = *sps / c1;
	warp_1.rps = (c11 + c12) * .5f * *sps;
	/*                            TO GSM EQ.PLANE FOR THE 3RD (ASYMPTOTIC) TAIL MODE */

	/*  THIS IS THE SHIFT OF OF THE SHEET WITH */
	r__ = sqrt(*x * *x + *y * *y + *z__ * *z__);
	/* Computing 2nd power */
	d__1 = r__ + rh;
	sq1 = sqrt(d__1 * d__1 + dr2);
	/* Computing 2nd power */
	d__1 = r__ - rh;
	sq2 = sqrt(d__1 * d__1 + dr2);
	c__ = sq1 - sq2;
	cs = (r__ + rh) / sq1 - (r__ - rh) / sq2;
	warp_1.spss = spsc1 / r__ * c__;
	/* Computing 2nd power */
	d__1 = warp_1.spss;
	warp_1.cpss = sqrt(1. - d__1 * d__1);
	/* Computing 2nd power */
	d__1 = r__ * c1;
	/* Computing 2nd power */
	d__2 = c__ * *sps;
	warp_1.dpsrr = *sps / (r__ * r__) * (cs * r__ - c__) / sqrt(d__1 * d__1 - 
		d__2 * d__2);

	/* Computing 4th power */
	d__1 = *y, d__1 *= d__1;
	wfac = *y / (d__1 * d__1 + 1e4);
	/*   WARPING */
	/* Computing 3rd power */
	d__1 = *y;
	w = wfac * (d__1 * (d__1 * d__1));
	/* Computing 2nd power */
	d__1 = wfac;
	ws = *y * 4e4 * (d__1 * d__1);
	warp_1.warp = g * *sps * w;
	warp_1.xs = *x * warp_1.cpss - *z__ * warp_1.spss;
	warp_1.zsww = *z__ * warp_1.cpss + *x * warp_1.spss;
	/* "WW" MEANS "WITHOUT Y-Z WARPING" (IN X-Z O */
	warp_1.zs = warp_1.zsww + warp_1.warp;
	warp_1.dxsx = warp_1.cpss - *x * warp_1.zsww * warp_1.dpsrr;
	warp_1.dxsy = -(*y) * warp_1.zsww * warp_1.dpsrr;
	warp_1.dxsz = -warp_1.spss - *z__ * warp_1.zsww * warp_1.dpsrr;
	warp_1.dzsx = warp_1.spss + *x * warp_1.xs * warp_1.dpsrr;
	warp_1.dzsy = warp_1.xs * *y * warp_1.dpsrr + g * *sps * ws;
	/*  THE LAST TERM IS FOR THE Y-Z WARP */
	warp_1.dzsz = warp_1.cpss + warp_1.xs * *z__ * warp_1.dpsrr;
	/*      (TAIL MODES ONLY) */
	/* Computing 2nd power */
	d__1 = *y / 20.;
	warp_1.d__ = d0 + deltady * (d__1 * d__1);
	/*  SHEET HALF-THICKNESS FOR THE TAIL */
	dddy = deltady * *y * .005;
	/*                                         ALONG X, IN CONTRAST TO RING CURRENT) */

	/*  (THICKENS TO FLANKS, BUT NO VARIA */
	/* Computing 2nd power */
	d__1 = warp_1.zs;
	/* Computing 2nd power */
	d__2 = warp_1.d__;
	warp_1.dzetas = sqrt(d__1 * d__1 + d__2 * d__2);
	/*                                        OUT THE SHEET, AS THAT USED IN T89 */
	/*  THIS IS THE SAME SIMPLE WAY TO SPRE */
	warp_1.ddzetadx = warp_1.zs * warp_1.dzsx / warp_1.dzetas;
	warp_1.ddzetady = (warp_1.zs * warp_1.dzsy + warp_1.d__ * dddy) / 
		warp_1.dzetas;
	warp_1.ddzetadz = warp_1.zs * warp_1.dzsz / warp_1.dzetas;

	shlcar3x3_(arc, x, y, z__, sps, &wx, &wy, &wz);
	ringcurr96_(x, y, z__, &hx, &hy, &hz);
	*bxrc = wx + hx;
	*byrc = wy + hy;
	*bzrc = wz + hz;

	shlcar3x3_(atail2, x, y, z__, sps, &wx, &wy, &wz);
	taildisk_(x, y, z__, &hx, &hy, &hz);
	*bxt2 = wx + hx;
	*byt2 = wy + hy;
	*bzt2 = wz + hz;

	shlcar3x3_(atail3, x, y, z__, sps, &wx, &wy, &wz);
	tail87_(x, z__, &hx, &hz);
	*bxt3 = wx + hx;
	*byt3 = wy;
	*bzt3 = wz + hz;

	return 0;
} /* tailrc96_ */


/* ******************************************************************** */

/* Subroutine */ int ringcurr96_(doublereal *x, doublereal *y, doublereal *
								 z__, doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal d0 = 2.;
	static doublereal deltadx = 0.;
	static doublereal xd = 0.;
	static doublereal xldx = 4.;
	static doublereal f[2] = { 569.895366,-1603.386993 };
	static doublereal beta[2] = { 2.722188,3.766875 };

	/* System generated locals */
	doublereal d__1, d__2, d__3, d__4;

	/* Builtin functions */
	double sqrt(doublereal), d_sign(doublereal *, doublereal *);

	/* Local variables */
	static doublereal ds1drhos, ds2drhos, ddzetadx, ddzetady, ddzetadz, d__;
	static integer i__;
	static doublereal s1, s2, bi, as, fdx, xxd, fac1, fac2, dddx, s1ps2, rhos,
		s1ts2, dzsy, ds1dx, ds1dy, ds1dz, ds2dx, ds2dy, ds2dz, term1, 
		dasdx, dasdy, dasdz, dasds1, dasds2, ds1ddz, ds2ddz, dzetas, 
		s1ps2sq, drhosdx, drhosdy, drhosdz;


	/*       THIS SUBROUTINE COMPUTES THE COMPONENTS OF THE RING CURRENT FIELD, */
	/*        SIMILAR TO THAT DESCRIBED BY TSYGANENKO AND PEREDO (1994).  THE */
	/*          DIFFERENCE IS THAT NOW WE USE SPACEWARPING, AS DESCRIBED IN THE */
	/*           PAPER ON MODELING BIRKELAND CURRENTS (TSYGANENKO AND STERN, 1996), */
	/*            INSTEAD OF SHEARING IT IN THE SPIRIT OF THE T89 TAIL MODEL. */

	/*          IN  ADDITION, INSTEAD OF 7 TERMS FOR THE RING CURRENT MODEL, WE USE */
	/*             NOW ONLY 2 TERMS;  THIS SIMPLIFICATION ALSO GIVES RISE TO AN */
	/*                EASTWARD RING CURRENT LOCATED EARTHWARD FROM THE MAIN ONE, */
	/*                  IN LINE WITH WHAT IS ACTUALLY OBSERVED */

	/*             FOR DETAILS, SEE NB #3, PAGES 70-73 */


	/*  ZS HERE IS WITHOUT Y- */
	/*                                            COMPLETELY SYMMETRIC (DELTADX=0) */

	/*  ACHTUNG !!  THE RC IS NO */

	/*  THE ORIGINAL VALUES OF F(I) WERE MULTIPLIED BY BETA(I) (TO REDUCE THE */
	/*     NUMBER OF MULTIPLICATIONS BELOW)  AND BY THE FACTOR -0.43, NORMALIZING */
	/*      THE DISTURBANCE AT ORIGIN  TO  B=-1nT */

	dzsy = warp_2.xs * *y * warp_2.dpsrr;
	/*                         THIS IS WHY WE DO NOT USE  DZSY FROM THE COMMON-BLOCK */
	/* NO WARPING IN THE Y-Z PLANE (ALONG X ONLY) */
	xxd = *x - xd;
	/* Computing 2nd power */
	d__1 = xxd;
	/* Computing 2nd power */
	d__2 = xldx;
	fdx = (xxd / sqrt(d__1 * d__1 + d__2 * d__2) + 1.) * .5;
	/* Computing 2nd power */
	d__1 = xldx;
	/* Computing 2nd power */
	d__3 = xxd;
	/* Computing 2nd power */
	d__4 = xldx;
	/* Computing 3rd power */
	d__2 = sqrt(d__3 * d__3 + d__4 * d__4);
	dddx = deltadx * .5 * (d__1 * d__1) / (d__2 * (d__2 * d__2));
	d__ = d0 + deltadx * fdx;
	/* Computing 2nd power */
	d__1 = warp_2.zs;
	/* Computing 2nd power */
	d__2 = d__;
	dzetas = sqrt(d__1 * d__1 + d__2 * d__2);
	/*                                        OUT THE SHEET, AS THAT USED IN T89 */
	/*  THIS IS THE SAME SIMPLE WAY TO S */
	/* Computing 2nd power */
	d__1 = warp_2.xs;
	/* Computing 2nd power */
	d__2 = *y;
	rhos = sqrt(d__1 * d__1 + d__2 * d__2);
	ddzetadx = (warp_2.zs * warp_2.dzsx + d__ * dddx) / dzetas;
	ddzetady = warp_2.zs * dzsy / dzetas;
	ddzetadz = warp_2.zs * warp_2.dzsz / dzetas;
	if (rhos < 1e-5) {
		drhosdx = 0.;
		drhosdy = d_sign(&c_b35, y);
		drhosdz = 0.;
	} else {
		drhosdx = warp_2.xs * warp_2.dxsx / rhos;
		drhosdy = (warp_2.xs * warp_2.dxsy + *y) / rhos;
		drhosdz = warp_2.xs * warp_2.dxsz / rhos;
	}

	*bx = 0.;
	*by = 0.;
	*bz = 0.;

	for (i__ = 1; i__ <= 2; ++i__) {

		bi = beta[i__ - 1];

		/* Computing 2nd power */
		d__1 = dzetas + bi;
		/* Computing 2nd power */
		d__2 = rhos + bi;
		s1 = sqrt(d__1 * d__1 + d__2 * d__2);
		/* Computing 2nd power */
		d__1 = dzetas + bi;
		/* Computing 2nd power */
		d__2 = rhos - bi;
		s2 = sqrt(d__1 * d__1 + d__2 * d__2);
		ds1ddz = (dzetas + bi) / s1;
		ds2ddz = (dzetas + bi) / s2;
		ds1drhos = (rhos + bi) / s1;
		ds2drhos = (rhos - bi) / s2;

		ds1dx = ds1ddz * ddzetadx + ds1drhos * drhosdx;
		ds1dy = ds1ddz * ddzetady + ds1drhos * drhosdy;
		ds1dz = ds1ddz * ddzetadz + ds1drhos * drhosdz;

		ds2dx = ds2ddz * ddzetadx + ds2drhos * drhosdx;
		ds2dy = ds2ddz * ddzetady + ds2drhos * drhosdy;
		ds2dz = ds2ddz * ddzetadz + ds2drhos * drhosdz;

		s1ts2 = s1 * s2;
		s1ps2 = s1 + s2;
		/* Computing 2nd power */
		d__1 = s1ps2;
		s1ps2sq = d__1 * d__1;
		/* Computing 2nd power */
		d__1 = bi * 2.;
		fac1 = sqrt(s1ps2sq - d__1 * d__1);
		as = fac1 / (s1ts2 * s1ps2sq);
		term1 = 1. / (s1ts2 * s1ps2 * fac1);
		fac2 = as / s1ps2sq;
		dasds1 = term1 - fac2 / s1 * (s2 * s2 + s1 * (s1 * 3. + s2 * 4.));
		dasds2 = term1 - fac2 / s2 * (s1 * s1 + s2 * (s2 * 3. + s1 * 4.));

		dasdx = dasds1 * ds1dx + dasds2 * ds2dx;
		dasdy = dasds1 * ds1dy + dasds2 * ds2dy;
		dasdz = dasds1 * ds1dz + dasds2 * ds2dz;

		/* Computing 2nd power */
		d__1 = *y;
		*bx += f[i__ - 1] * ((as * 2. + *y * dasdy) * warp_2.spss - warp_2.xs 
			* dasdz + as * warp_2.dpsrr * (d__1 * d__1 * warp_2.cpss + *
			z__ * warp_2.zs));
		*by -= f[i__ - 1] * *y * (as * warp_2.dpsrr * warp_2.xs + dasdz * 
			warp_2.cpss + dasdx * warp_2.spss);
		/* L1: */
		/* Computing 2nd power */
		d__1 = *y;
		*bz += f[i__ - 1] * ((as * 2. + *y * dasdy) * warp_2.cpss + warp_2.xs 
			* dasdx - as * warp_2.dpsrr * (*x * warp_2.zs + d__1 * d__1 * 
			warp_2.spss));
	}

	return 0;
} /* ringcurr96_ */


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */

/* Subroutine */ int taildisk_(doublereal *x, doublereal *y, doublereal *z__, 
							   doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal xshift = 4.5;
	static doublereal f[4] = { -745796.7338,1176470.141,-444610.529,
		-57508.01028 };
	static doublereal beta[4] = { 7.925,8.085,8.47125,27.895 };

	/* System generated locals */
	doublereal d__1, d__2;

	/* Builtin functions */
	double sqrt(doublereal), d_sign(doublereal *, doublereal *);

	/* Local variables */
	static doublereal ds1drhos, ds2drhos;
	static integer i__;
	static doublereal s1, s2, bi, as, fac1, fac2, s1ps2, rhos, s1ts2, ds1dx, 
		ds1dy, ds1dz, ds2dx, ds2dy, ds2dz, term1, dasdx, dasdy, dasdz, 
		dasds1, dasds2, ds1ddz, ds2ddz, s1ps2sq, drhosdx, drhosdy, 
		drhosdz;



	/*       THIS SUBROUTINE COMPUTES THE COMPONENTS OF THE TAIL CURRENT FIELD, */
	/*        SIMILAR TO THAT DESCRIBED BY TSYGANENKO AND PEREDO (1994).  THE */
	/*          DIFFERENCE IS THAT NOW WE USE SPACEWARPING, AS DESCRIBED IN OUR */
	/*           PAPER ON MODELING BIRKELAND CURRENTS (TSYGANENKO AND STERN, 1996) */
	/*            INSTEAD OF SHEARING IT IN THE SPIRIT OF T89 TAIL MODEL. */

	/*          IN  ADDITION, INSTEAD OF 8 TERMS FOR THE TAIL CURRENT MODEL, WE USE */
	/*           NOW ONLY 4 TERMS */

	/*             FOR DETAILS, SEE NB #3, PAGES 74- */




	/*  here original F(I) are multiplied by BETA(I), to economize */
	/*    calculations */

	/* Computing 2nd power */
	d__1 = warp_3.xs - xshift;
	/* Computing 2nd power */
	d__2 = *y;
	rhos = sqrt(d__1 * d__1 + d__2 * d__2);
	if (rhos < 1e-5) {
		drhosdx = 0.;
		drhosdy = d_sign(&c_b35, y);
		drhosdz = 0.;
	} else {
		drhosdx = (warp_3.xs - xshift) * warp_3.dxsx / rhos;
		drhosdy = ((warp_3.xs - xshift) * warp_3.dxsy + *y) / rhos;
		drhosdz = (warp_3.xs - xshift) * warp_3.dxsz / rhos;
	}

	*bx = 0.;
	*by = 0.;
	*bz = 0.;

	for (i__ = 1; i__ <= 4; ++i__) {

		bi = beta[i__ - 1];

		/* Computing 2nd power */
		d__1 = warp_3.dzetas + bi;
		/* Computing 2nd power */
		d__2 = rhos + bi;
		s1 = sqrt(d__1 * d__1 + d__2 * d__2);
		/* Computing 2nd power */
		d__1 = warp_3.dzetas + bi;
		/* Computing 2nd power */
		d__2 = rhos - bi;
		s2 = sqrt(d__1 * d__1 + d__2 * d__2);
		ds1ddz = (warp_3.dzetas + bi) / s1;
		ds2ddz = (warp_3.dzetas + bi) / s2;
		ds1drhos = (rhos + bi) / s1;
		ds2drhos = (rhos - bi) / s2;

		ds1dx = ds1ddz * warp_3.ddzetadx + ds1drhos * drhosdx;
		ds1dy = ds1ddz * warp_3.ddzetady + ds1drhos * drhosdy;
		ds1dz = ds1ddz * warp_3.ddzetadz + ds1drhos * drhosdz;

		ds2dx = ds2ddz * warp_3.ddzetadx + ds2drhos * drhosdx;
		ds2dy = ds2ddz * warp_3.ddzetady + ds2drhos * drhosdy;
		ds2dz = ds2ddz * warp_3.ddzetadz + ds2drhos * drhosdz;

		s1ts2 = s1 * s2;
		s1ps2 = s1 + s2;
		/* Computing 2nd power */
		d__1 = s1ps2;
		s1ps2sq = d__1 * d__1;
		/* Computing 2nd power */
		d__1 = bi * 2.;
		fac1 = sqrt(s1ps2sq - d__1 * d__1);
		as = fac1 / (s1ts2 * s1ps2sq);
		term1 = 1. / (s1ts2 * s1ps2 * fac1);
		fac2 = as / s1ps2sq;
		dasds1 = term1 - fac2 / s1 * (s2 * s2 + s1 * (s1 * 3. + s2 * 4.));
		dasds2 = term1 - fac2 / s2 * (s1 * s1 + s2 * (s2 * 3. + s1 * 4.));

		dasdx = dasds1 * ds1dx + dasds2 * ds2dx;
		dasdy = dasds1 * ds1dy + dasds2 * ds2dy;
		dasdz = dasds1 * ds1dz + dasds2 * ds2dz;

		/* Computing 2nd power */
		d__1 = *y;
		*bx += f[i__ - 1] * ((as * 2. + *y * dasdy) * warp_3.spss - (
			warp_3.xs - xshift) * dasdz + as * warp_3.dpsrr * (d__1 * 
			d__1 * warp_3.cpss + *z__ * warp_3.zsww));

		*by -= f[i__ - 1] * *y * (as * warp_3.dpsrr * warp_3.xs + dasdz * 
			warp_3.cpss + dasdx * warp_3.spss);
		/* L1: */
		/* Computing 2nd power */
		d__1 = *y;
		*bz += f[i__ - 1] * ((as * 2. + *y * dasdy) * warp_3.cpss + (
			warp_3.xs - xshift) * dasdx - as * warp_3.dpsrr * (*x * 
			warp_3.zsww + d__1 * d__1 * warp_3.spss));
	}
	return 0;
} /* taildisk_ */

/* ------------------------------------------------------------------------- */

/* Subroutine */ int tail87_(doublereal *x, doublereal *z__, doublereal *bx, 
							 doublereal *bz)
{
	/* Initialized data */

	static doublereal dd = 3.;
	static doublereal hpi = 1.5707963;
	static doublereal rt = 40.;
	static doublereal xn = -10.;
	static doublereal x1 = -1.261;
	static doublereal x2 = -.663;
	static doublereal b0 = .391734;
	static doublereal b1 = 5.89715;
	static doublereal b2 = 24.6833;
	static doublereal xn21 = 76.37;
	static doublereal xnr = -.1071;
	static doublereal adln = .13238005;

	/* System generated locals */
	doublereal d__1;

	/* Builtin functions */
	double sqrt(doublereal), log(doublereal), atan(doublereal);

	/* Local variables */
	static doublereal b, f, d2, g1, g2, s0, s1, s2, b20, bm, bp, fm, fp, zm, 
		zp, zs, b2m, b2p, g1m, g2m, g1p, g2p, xa1, xa2, xc1, xc2, s0m, 
		s1m, s2m, s0p, s1p, s2p, xr2, aln, xc12, xc22, xna, xnx, xam1, 
		xam2, xap1, xap2, xln1, xln2, xnx2, xnam, xnap, xlnm1, xlnm2, 
		xlnp1, xlnp2;


	/*      'LONG' VERSION OF THE 1987 TAIL MAGNETIC FIELD MODEL */
	/*              (N.A.TSYGANENKO, PLANET. SPACE SCI., V.35, P.1347, 1987) */

	/*      D   IS THE Y-DEPENDENT SHEET HALF-THICKNESS (INCREASING TOWARDS FLANKS) */
	/*      RPS  IS THE TILT-DEPENDENT SHIFT OF THE SHEET IN THE Z-DIRECTION, */
	/*           CORRESPONDING TO THE ASYMPTOTIC HINGING DISTANCE, DEFINED IN THE */
	/*           MAIN SUBROUTINE (TAILRC96) FROM THE PARAMETERS RH AND DR OF THE */
	/*           T96-TYPE MODULE, AND */
	/*      WARP  IS THE BENDING OF THE SHEET FLANKS IN THE Z-DIRECTION, DIRECTED */
	/*           OPPOSITE TO RPS, AND INCREASING WITH DIPOLE TILT AND |Y| */


	/*                !!!   THESE ARE NEW VALUES OF  X1, X2, B0, B1, B2, */
	/*                       CORRESPONDING TO TSCALE=1, INSTEAD OF TSCALE=0.6 */

	/*  THE ABOVE QUANTITIES WERE DEFINED AS FOLLOWS:------------------------ */
	/*       HPI=PI/2 */
	/*       RT=40.      !  Z-POSITION OF UPPER AND LOWER ADDITIONAL SHEETS */
	/*       XN=-10.     !  INNER EDGE POSITION */

	/*       TSCALE=1  !  SCALING FACTOR, DEFINING THE RATE OF INCREASE OF THE */
	/*                       CURRENT DENSITY TAILWARDS */

	/*  ATTENTION !  NOW I HAVE CHANGED TSCALE TO:  TSCALE=1.0, INSTEAD OF 0.6 */
	/*                  OF THE PREVIOUS VERSION */

	/*       B0=0.391734 */
	/*       B1=5.89715 *TSCALE */
	/*       B2=24.6833 *TSCALE**2 */

	/*    HERE ORIGINAL VALUES OF THE MODE AMPLITUDES (P.77, NB#3) WERE NORMALIZED */
	/*      SO THAT ASYMPTOTIC  BX=1  AT X=-200RE */

	/*      X1=(4.589  -5.85) *TSCALE -(TSCALE-1.)*XN ! NONLINEAR PARAMETERS OF THE */
	/*                                                         CURRENT FUNCTION */
	/*      X2=(5.187  -5.85) *TSCALE -(TSCALE-1.)*XN */


	/*      XN21=(XN-X1)**2 */
	/*      XNR=1./(XN-X2) */
	/*      ADLN=-DLOG(XNR**2*XN21) */

	/* --------------------------------------------------------------- */

	zs = *z__ - warp_4.rps + warp_4.warp;
	zp = *z__ - rt;
	zm = *z__ + rt;

	xnx = xn - *x;
	/* Computing 2nd power */
	d__1 = xnx;
	xnx2 = d__1 * d__1;
	xc1 = *x - x1;
	xc2 = *x - x2;
	/* Computing 2nd power */
	d__1 = xc2;
	xc22 = d__1 * d__1;
	xr2 = xc2 * xnr;
	/* Computing 2nd power */
	d__1 = xc1;
	xc12 = d__1 * d__1;
	/* Computing 2nd power */
	d__1 = dd;
	d2 = d__1 * d__1;
	/*  SQUARE OF THE TOTAL HALFTHICKNESS (DD=3Re for this */
	/* Computing 2nd power */
	d__1 = zs;
	b20 = d__1 * d__1 + d2;
	/* Computing 2nd power */
	d__1 = zp;
	b2p = d__1 * d__1 + d2;
	/* Computing 2nd power */
	d__1 = zm;
	b2m = d__1 * d__1 + d2;
	b = sqrt(b20);
	bp = sqrt(b2p);
	bm = sqrt(b2m);
	xa1 = xc12 + b20;
	xap1 = xc12 + b2p;
	xam1 = xc12 + b2m;
	xa2 = 1.f / (xc22 + b20);
	xap2 = 1.f / (xc22 + b2p);
	xam2 = 1.f / (xc22 + b2m);
	xna = xnx2 + b20;
	xnap = xnx2 + b2p;
	xnam = xnx2 + b2m;
	f = b20 - xc22;
	fp = b2p - xc22;
	fm = b2m - xc22;
	xln1 = log(xn21 / xna);
	xlnp1 = log(xn21 / xnap);
	xlnm1 = log(xn21 / xnam);
	xln2 = xln1 + adln;
	xlnp2 = xlnp1 + adln;
	xlnm2 = xlnm1 + adln;
	aln = (xlnp1 + xlnm1 - xln1 * 2.f) * .25f;
	s0 = (atan(xnx / b) + hpi) / b;
	s0p = (atan(xnx / bp) + hpi) / bp;
	s0m = (atan(xnx / bm) + hpi) / bm;
	s1 = (xln1 * .5f + xc1 * s0) / xa1;
	s1p = (xlnp1 * .5f + xc1 * s0p) / xap1;
	s1m = (xlnm1 * .5f + xc1 * s0m) / xam1;
	s2 = (xc2 * xa2 * xln2 - xnr - f * xa2 * s0) * xa2;
	s2p = (xc2 * xap2 * xlnp2 - xnr - fp * xap2 * s0p) * xap2;
	s2m = (xc2 * xam2 * xlnm2 - xnr - fm * xam2 * s0m) * xam2;
	g1 = (b20 * s0 - xc1 * .5f * xln1) / xa1;
	g1p = (b2p * s0p - xc1 * .5f * xlnp1) / xap1;
	g1m = (b2m * s0m - xc1 * .5f * xlnm1) / xam1;
	g2 = ((f * .5f * xln2 + s0 * 2.f * b20 * xc2) * xa2 + xr2) * xa2;
	g2p = ((fp * .5f * xlnp2 + s0p * 2.f * b2p * xc2) * xap2 + xr2) * xap2;
	g2m = ((fm * .5f * xlnm2 + s0m * 2.f * b2m * xc2) * xam2 + xr2) * xam2;
	*bx = b0 * (zs * s0 - (zp * s0p + zm * s0m) * .5f) + b1 * (zs * s1 - (zp *
		s1p + zm * s1m) * .5f) + b2 * (zs * s2 - (zp * s2p + zm * s2m) * 
		.5f);
	*bz = b0 * aln + b1 * (g1 - (g1p + g1m) * .5f) + b2 * (g2 - (g2p + g2m) * 
		.5f);

	/*    CALCULATION OF THE MAGNETOTAIL CURRENT CONTRIBUTION IS FINISHED */

	return 0;
} /* tail87_ */

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */

/* THIS CODE RETURNS THE SHIELDING FIELD REPRESENTED BY  2x3x3=18 "CARTESIAN" */
/*    HARMONICS */

/* Subroutine */ int shlcar3x3_(doublereal *a, doublereal *x, doublereal *y, 
								doublereal *z__, doublereal *sps, doublereal *hx, doublereal *hy, 
								doublereal *hz)
{
	/* System generated locals */
	doublereal d__1, d__2;

	/* Builtin functions */
	double sqrt(doublereal), cos(doublereal), sin(doublereal), exp(doublereal)
		;

	/* Local variables */
	static integer i__, k, l, m, n;
	static doublereal p, q, r__, s, dx, dy, dz, cps, epr, eqs, s3ps, cypi, 
		cyqi, czrk, czsk, sypi, syqi, sqpr, sqqs, szrk, szsk;


	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*  The 36 coefficients enter in pairs in the amplitudes of the "cartesian" */
	/*    harmonics (A(1)-A(36). */
	/*  The 12 nonlinear parameters (A(37)-A(48) are the scales Pi,Ri,Qi,and Si */
	/*   entering the arguments of exponents, sines, and cosines in each of the */
	/*   18 "Cartesian" harmonics */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



	/* Parameter adjustments */
	--a;

	/* Function Body */
	/* Computing 2nd power */
	d__1 = *sps;
	cps = sqrt(1. - d__1 * d__1);
	/* Computing 2nd power */
	d__1 = cps;
	s3ps = d__1 * d__1 * 4. - 1.;

	/*  THIS IS SIN(3*PS)/SIN(PS) */
	*hx = 0.;
	*hy = 0.;
	*hz = 0.;
	l = 0;

	for (m = 1; m <= 2; ++m) {
		/*                           AND M=2 IS FOR THE SECOND SUM ("PARALL." SYMMETRY) */
		/*    M=1 IS FOR THE 1ST SUM ("PERP." SYMMETRY) */
		for (i__ = 1; i__ <= 3; ++i__) {
			p = a[i__ + 36];
			q = a[i__ + 42];
			cypi = cos(*y / p);
			cyqi = cos(*y / q);
			sypi = sin(*y / p);
			syqi = sin(*y / q);

			for (k = 1; k <= 3; ++k) {
				r__ = a[k + 39];
				s = a[k + 45];
				szrk = sin(*z__ / r__);
				czsk = cos(*z__ / s);
				czrk = cos(*z__ / r__);
				szsk = sin(*z__ / s);
				/* Computing 2nd power */
				d__1 = p;
				/* Computing 2nd power */
				d__2 = r__;
				sqpr = sqrt(1. / (d__1 * d__1) + 1. / (d__2 * d__2));
				/* Computing 2nd power */
				d__1 = q;
				/* Computing 2nd power */
				d__2 = s;
				sqqs = sqrt(1. / (d__1 * d__1) + 1. / (d__2 * d__2));
				epr = exp(*x * sqpr);
				eqs = exp(*x * sqqs);

				for (n = 1; n <= 2; ++n) {
					/*                                  AND N=2 IS FOR THE SECOND ONE */

					/* N=1 IS FOR THE FIRST PART OF EACH COEFF */
					++l;
					if (m == 1) {
						if (n == 1) {
							dx = -sqpr * epr * cypi * szrk;
							dy = epr / p * sypi * szrk;
							dz = -epr / r__ * cypi * czrk;
							*hx += a[l] * dx;
							*hy += a[l] * dy;
							*hz += a[l] * dz;
						} else {
							dx *= cps;
							dy *= cps;
							dz *= cps;
							*hx += a[l] * dx;
							*hy += a[l] * dy;
							*hz += a[l] * dz;
						}
					} else {
						if (n == 1) {
							dx = -(*sps) * sqqs * eqs * cyqi * czsk;
							dy = *sps * eqs / q * syqi * czsk;
							dz = *sps * eqs / s * cyqi * szsk;
							*hx += a[l] * dx;
							*hy += a[l] * dy;
							*hz += a[l] * dz;
						} else {
							dx *= s3ps;
							dy *= s3ps;
							dz *= s3ps;
							*hx += a[l] * dx;
							*hy += a[l] * dy;
							*hz += a[l] * dz;
						}
					}

					/* L4: */
				}
				/* L3: */
			}
			/* L2: */
		}
		/* L1: */
	}

	return 0;
} /* shlcar3x3_ */


/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

/* Subroutine */ int birk1tot_02__(doublereal *ps, doublereal *x, doublereal *
								   y, doublereal *z__, doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal c1[26] = { -9.11582e-4,-.00376654,-.00727423,-.00270084,
		-.00123899,-.00154387,-.0034004,-.0191858,-.0518979,.0635061,
		.44068,-.39657,.00561238,.00160938,-.00451229,-.0025181,
		-.00151599,-.00133665,-9.62089e-4,-.0272085,-.0524319,.0717024,
		.523439,-.405015,-89.5587,23.2806 };
	static doublereal c2[79] = { 6.04133,.305415,.00606066,1.28379e-4,
		-1.79406e-5,1.41714,-27.2586,-4.28833,-1.30675,35.5607,8.95792,
		9.61617e-4,-8.01477e-4,-7.82795e-4,-1.65242,-16.5242,-5.33798,
		4.24878e-4,3.31787e-4,-7.04305e-4,8.44342e-4,9.53682e-5,
		8.86271e-4,25.112,20.9299,5.14569,-44.167,-51.0672,-1.87725,
		20.2998,48.7505,-2.97415,3.35184,-54.2921,-.838712,-10.5123,
		70.7594,-4.94104,1.06166e-4,4.65791e-4,-1.93719e-4,10.8439,
		-29.7968,8.08068,4.63507e-4,-2.24475e-5,1.77035e-4,-3.17581e-4,
		-2.64487e-4,1.02075e-4,7.7139,10.1915,-4.99797,-23.1114,-29.2043,
		12.2928,10.9542,33.6671,-9.3851,1.74615e-4,-7.89777e-7,6.86047e-4,
		4.60104e-5,-.00345216,.00221871,.0110078,-.00661373,.00249201,
		.0343978,-1.93145e-6,4.93963e-6,-5.35748e-5,1.91833e-5,
		-1.00496e-4,-2.10103e-4,-.00232195,.00315335,-.013432,-.0263222 };
	static doublereal xltday = 78.;
	static doublereal xltnght = 70.;
	static doublereal dtet0 = .034906;

	/* System generated locals */
	doublereal d__1, d__2, d__3;

	/* Builtin functions */
	double sin(doublereal), sqrt(doublereal), atan2(doublereal, doublereal), 
		pow_dd(doublereal *, doublereal *), asin(doublereal), cos(
		doublereal);

	/* Local variables */
	extern /* Subroutine */ int diploop1_(doublereal *, doublereal *), 
		birk1shld_(doublereal *, doublereal *, doublereal *, doublereal *,
		doublereal *, doublereal *, doublereal *);
	static doublereal c__, f;
	static integer i__;
	static doublereal q, r__, d1[78]	/* was [3][26] */, d2[237]	/* 
																	was [3][79] */, r2, r3, x1, y1, z1, y2, x2, z2, t01, t02, ds, xi[
																		4], ss, dr2, bx1, by1, bz1, bx2, by2, bz2;
																		static integer loc;
																		static doublereal pas, tas, xas, bsx, zas, bsy, bsz, sqm, sqp, sps, sqr, 
																			tet0, xas1, zas1, xas2, zas2, frac, dtet, rmrh, stas, rprh, 
																			ct01as, ct02as, cpsas, st01as, st02as, spsas, tetr1n, tetr1s, 
																			dtetdn, tnoonn, tnoons;
																		extern /* Subroutine */ int condip1_(doublereal *, doublereal *);


																		/*  THIS IS THE SECOND VERSION OF THE ANALYTICAL MODEL OF THE REGION 1 FIELD */
																		/*   BASED ON A SEPARATE REPRESENTATION OF THE POTENTIAL FIELD IN THE INNER AND */
																		/*   OUTER SPACE, MAPPED BY MEANS OF A SPHERO-DIPOLAR COORDINATE SYSTEM (NB #3, */
																		/*   P.91).   THE DIFFERENCE FROM THE FIRST ONE IS THAT INSTEAD OF OCTAGONAL */
																		/*   CURRENT LOOPS, CIRCULAR ONES ARE USED IN THIS VERSION FOR APPROXIMATING THE */
																		/*   FIELD IN THE OUTER REGION, WHICH IS FASTER. */







																		/*                                TRANSITION SCALE LENGTH, DEFINING THE */
																		/*                                CURVATURE  OF THE WARPING (SEE P.89, NB #2) */

																		/*  RH IS THE "HINGING DISTANCE" AND DR IS */
																		/*                                             AT NOON AND AT MIDNIGHT */
																		/*  THESE ARE LATITUDES OF THE R */
																		/*                                  R-1 OVAL (THE INTERPOLATION REGION BETWEEN */
																		/*                                    THE HIGH-LAT. AND THE PLASMA SHEET) */

																		/*   THIS IS THE LATITUDINAL HALF-THICKNESS */
																		tnoonn = (90. - xltday) * .01745329;
																		tnoons = 3.141592654 - tnoonn;
																		/*                                          THE NORTHERN AND SOUTHERN R-1 OVALS */
																		/*                                          ARE SYMMETRIC IN THE SM-COORDINATES */
																		/* HERE WE ASSUME THAT THE POSITI */
																		dtetdn = (xltday - xltnght) * .01745329;
																		/* Computing 2nd power */
																		d__1 = rhdr_1.dr;
																		dr2 = d__1 * d__1;

																		sps = sin(*ps);
																		/* Computing 2nd power */
																		d__1 = *x;
																		/* Computing 2nd power */
																		d__2 = *y;
																		/* Computing 2nd power */
																		d__3 = *z__;
																		r2 = d__1 * d__1 + d__2 * d__2 + d__3 * d__3;
																		r__ = sqrt(r2);
																		r3 = r__ * r2;

																		rmrh = r__ - rhdr_1.rh;
																		rprh = r__ + rhdr_1.rh;
																		/* Computing 2nd power */
																		d__1 = rmrh;
																		sqm = sqrt(d__1 * d__1 + dr2);
																		/* Computing 2nd power */
																		d__1 = rprh;
																		sqp = sqrt(d__1 * d__1 + dr2);
																		c__ = sqp - sqm;
																		/* Computing 2nd power */
																		d__1 = rhdr_1.rh + 1.;
																		/* Computing 2nd power */
																		d__2 = rhdr_1.rh - 1.;
																		q = sqrt(d__1 * d__1 + dr2) - sqrt(d__2 * d__2 + dr2);
																		spsas = sps / r__ * c__ / q;
																		/* Computing 2nd power */
																		d__1 = spsas;
																		cpsas = sqrt(1. - d__1 * d__1);
																		xas = *x * cpsas - *z__ * spsas;
																		zas = *x * spsas + *z__ * cpsas;
																		if (xas != 0. || *y != 0.) {
																			pas = atan2(*y, xas);
																		} else {
																			pas = 0.;
																		}

																		/* Computing 2nd power */
																		d__1 = xas;
																		/* Computing 2nd power */
																		d__2 = *y;
																		tas = atan2(sqrt(d__1 * d__1 + d__2 * d__2), zas);
																		stas = sin(tas);
																		/* Computing 6th power */
																		d__2 = stas, d__2 *= d__2;
																		d__1 = d__2 * (d__2 * d__2) * (1. - r3) + r3;
																		f = stas / pow_dd(&d__1, &c_b47);

																		tet0 = asin(f);
																		if (tas > 1.5707963) {
																			tet0 = 3.141592654 - tet0;
																		}
																		/* Computing 2nd power */
																		d__1 = sin(pas * .5);
																		dtet = dtetdn * (d__1 * d__1);
																		tetr1n = tnoonn + dtet;
																		tetr1s = tnoons - dtet;

																		/* NOW LET'S DEFINE WHICH OF THE FOUR REGIONS (HIGH-LAT., NORTHERN PSBL, */
																		/*   PLASMA SHEET, SOUTHERN PSBL) DOES THE POINT (X,Y,Z) BELONG TO: */

																		if (tet0 < tetr1n - dtet0 || tet0 > tetr1s + dtet0) {
																			loc = 1;
																		}
																		/* HIGH-L */
																		if (tet0 > tetr1n + dtet0 && tet0 < tetr1s - dtet0) {
																			loc = 2;
																		}
																		/* PL.SHE */
																		if (tet0 >= tetr1n - dtet0 && tet0 <= tetr1n + dtet0) {
																			loc = 3;
																		}
																		/* NORTH */
																		if (tet0 >= tetr1s - dtet0 && tet0 <= tetr1s + dtet0) {
																			loc = 4;
																		}

																		/* SOUTH */
																		if (loc == 1) {

																			/*      print *, '  LOC=1 (HIGH-LAT)'    !  (test printout; disabled now) */
																			/* IN THE HIGH-LAT. REGION USE THE SUBROUTINE */
																			xi[0] = *x;
																			xi[1] = *y;
																			xi[2] = *z__;
																			xi[3] = *ps;
																			diploop1_(xi, d1);
																			*bx = 0.;
																			*by = 0.;
																			*bz = 0.;
																			for (i__ = 1; i__ <= 26; ++i__) {
																				*bx += c1[i__ - 1] * d1[i__ * 3 - 3];
																				*by += c1[i__ - 1] * d1[i__ * 3 - 2];
																				/* L1: */
																				*bz += c1[i__ - 1] * d1[i__ * 3 - 1];
																			}
																		}

																		/*  END OF THE CAS */
																		if (loc == 2) {
																			/*           print *, '  LOC=2 (PLASMA SHEET)'  !  (test printout; disabled now) */

																			xi[0] = *x;
																			xi[1] = *y;
																			xi[2] = *z__;
																			xi[3] = *ps;
																			condip1_(xi, d2);
																			*bx = 0.;
																			*by = 0.;
																			*bz = 0.;
																			for (i__ = 1; i__ <= 79; ++i__) {
																				*bx += c2[i__ - 1] * d2[i__ * 3 - 3];
																				*by += c2[i__ - 1] * d2[i__ * 3 - 2];
																				/* L2: */
																				*bz += c2[i__ - 1] * d2[i__ * 3 - 1];
																			}
																		}

																		/*   END OF THE CA */
																		if (loc == 3) {
																			/*       print *, '  LOC=3 (north PSBL)'  !  (test printout; disabled now) */

																			t01 = tetr1n - dtet0;
																			t02 = tetr1n + dtet0;
																			sqr = sqrt(r__);
																			/* Computing 6th power */
																			d__2 = sin(t01), d__2 *= d__2;
																			d__1 = r3 + 1. / (d__2 * (d__2 * d__2)) - 1.;
																			st01as = sqr / pow_dd(&d__1, &c_b47);
																			/* Computing 6th power */
																			d__2 = sin(t02), d__2 *= d__2;
																			d__1 = r3 + 1. / (d__2 * (d__2 * d__2)) - 1.;
																			st02as = sqr / pow_dd(&d__1, &c_b47);
																			/* Computing 2nd power */
																			d__1 = st01as;
																			ct01as = sqrt(1. - d__1 * d__1);
																			/* Computing 2nd power */
																			d__1 = st02as;
																			ct02as = sqrt(1. - d__1 * d__1);
																			xas1 = r__ * st01as * cos(pas);
																			y1 = r__ * st01as * sin(pas);
																			zas1 = r__ * ct01as;
																			x1 = xas1 * cpsas + zas1 * spsas;
																			z1 = -xas1 * spsas + zas1 * cpsas;
																			/*                                                      BOUNDARY POINT */
																			/* X1,Y1,Z1 ARE COORDS OF THE NORTHERN */
																			xi[0] = x1;
																			xi[1] = y1;
																			xi[2] = z1;
																			xi[3] = *ps;
																			diploop1_(xi, d1);
																			bx1 = 0.;
																			by1 = 0.;
																			bz1 = 0.;
																			for (i__ = 1; i__ <= 26; ++i__) {
																				bx1 += c1[i__ - 1] * d1[i__ * 3 - 3];
																				/*   BX1,BY1,BZ1  ARE FIELD COMPONENT */
																				by1 += c1[i__ - 1] * d1[i__ * 3 - 2];
																				/*  IN THE NORTHERN BOUNDARY POINT */
																				/* L11: */
																				bz1 += c1[i__ - 1] * d1[i__ * 3 - 1];
																			}


																			xas2 = r__ * st02as * cos(pas);
																			y2 = r__ * st02as * sin(pas);
																			zas2 = r__ * ct02as;
																			x2 = xas2 * cpsas + zas2 * spsas;
																			z2 = -xas2 * spsas + zas2 * cpsas;
																			/*                                        BOUNDARY POINT */
																			/* X2,Y2,Z2 ARE COORDS OF THE SOUTHERN */
																			xi[0] = x2;
																			xi[1] = y2;
																			xi[2] = z2;
																			xi[3] = *ps;
																			condip1_(xi, d2);
																			bx2 = 0.;
																			by2 = 0.;
																			bz2 = 0.;
																			for (i__ = 1; i__ <= 79; ++i__) {
																				bx2 += c2[i__ - 1] * d2[i__ * 3 - 3];
																				/*  BX2,BY2,BZ2  ARE FIELD COMPONENTS */
																				by2 += c2[i__ - 1] * d2[i__ * 3 - 2];
																				/*  IN THE SOUTHERN BOUNDARY POINT */
																				/* L12: */
																				bz2 += c2[i__ - 1] * d2[i__ * 3 - 1];
																			}

																			/*  NOW INTERPOLATE: */

																			/* Computing 2nd power */
																			d__1 = x2 - x1;
																			/* Computing 2nd power */
																			d__2 = y2 - y1;
																			/* Computing 2nd power */
																			d__3 = z2 - z1;
																			ss = sqrt(d__1 * d__1 + d__2 * d__2 + d__3 * d__3);
																			/* Computing 2nd power */
																			d__1 = *x - x1;
																			/* Computing 2nd power */
																			d__2 = *y - y1;
																			/* Computing 2nd power */
																			d__3 = *z__ - z1;
																			ds = sqrt(d__1 * d__1 + d__2 * d__2 + d__3 * d__3);
																			frac = ds / ss;
																			*bx = bx1 * (1. - frac) + bx2 * frac;
																			*by = by1 * (1. - frac) + by2 * frac;
																			*bz = bz1 * (1. - frac) + bz2 * frac;

																		}

																		/* END OF THE */
																		if (loc == 4) {
																			/*       print *, '  LOC=4 (south PSBL)'  !  (test printout; disabled now) */

																			t01 = tetr1s - dtet0;
																			t02 = tetr1s + dtet0;
																			sqr = sqrt(r__);
																			/* Computing 6th power */
																			d__2 = sin(t01), d__2 *= d__2;
																			d__1 = r3 + 1. / (d__2 * (d__2 * d__2)) - 1.;
																			st01as = sqr / pow_dd(&d__1, &c_b47);
																			/* Computing 6th power */
																			d__2 = sin(t02), d__2 *= d__2;
																			d__1 = r3 + 1. / (d__2 * (d__2 * d__2)) - 1.;
																			st02as = sqr / pow_dd(&d__1, &c_b47);
																			/* Computing 2nd power */
																			d__1 = st01as;
																			ct01as = -sqrt(1. - d__1 * d__1);
																			/* Computing 2nd power */
																			d__1 = st02as;
																			ct02as = -sqrt(1. - d__1 * d__1);
																			xas1 = r__ * st01as * cos(pas);
																			y1 = r__ * st01as * sin(pas);
																			zas1 = r__ * ct01as;
																			x1 = xas1 * cpsas + zas1 * spsas;
																			z1 = -xas1 * spsas + zas1 * cpsas;
																			/*                                               BOUNDARY POINT */
																			/* X1,Y1,Z1 ARE COORDS OF THE NORTHERN */
																			xi[0] = x1;
																			xi[1] = y1;
																			xi[2] = z1;
																			xi[3] = *ps;
																			condip1_(xi, d2);
																			bx1 = 0.;
																			by1 = 0.;
																			bz1 = 0.;
																			for (i__ = 1; i__ <= 79; ++i__) {
																				bx1 += c2[i__ - 1] * d2[i__ * 3 - 3];
																				/*  BX1,BY1,BZ1  ARE FIELD COMPONENTS */
																				by1 += c2[i__ - 1] * d2[i__ * 3 - 2];
																				/*  IN THE NORTHERN BOUNDARY POINT */
																				/* L21: */
																				bz1 += c2[i__ - 1] * d2[i__ * 3 - 1];
																			}


																			xas2 = r__ * st02as * cos(pas);
																			y2 = r__ * st02as * sin(pas);
																			zas2 = r__ * ct02as;
																			x2 = xas2 * cpsas + zas2 * spsas;
																			z2 = -xas2 * spsas + zas2 * cpsas;
																			/*                                          BOUNDARY POINT */
																			/* X2,Y2,Z2 ARE COORDS OF THE SOUTHERN */
																			xi[0] = x2;
																			xi[1] = y2;
																			xi[2] = z2;
																			xi[3] = *ps;
																			diploop1_(xi, d1);
																			bx2 = 0.;
																			by2 = 0.;
																			bz2 = 0.;
																			for (i__ = 1; i__ <= 26; ++i__) {
																				bx2 += c1[i__ - 1] * d1[i__ * 3 - 3];
																				/*  BX2,BY2,BZ2  ARE FIELD COMPONENTS */
																				by2 += c1[i__ - 1] * d1[i__ * 3 - 2];
																				/*     IN THE SOUTHERN BOUNDARY POINT */
																				/* L22: */
																				bz2 += c1[i__ - 1] * d1[i__ * 3 - 1];
																			}

																			/*  NOW INTERPOLATE: */

																			/* Computing 2nd power */
																			d__1 = x2 - x1;
																			/* Computing 2nd power */
																			d__2 = y2 - y1;
																			/* Computing 2nd power */
																			d__3 = z2 - z1;
																			ss = sqrt(d__1 * d__1 + d__2 * d__2 + d__3 * d__3);
																			/* Computing 2nd power */
																			d__1 = *x - x1;
																			/* Computing 2nd power */
																			d__2 = *y - y1;
																			/* Computing 2nd power */
																			d__3 = *z__ - z1;
																			ds = sqrt(d__1 * d__1 + d__2 * d__2 + d__3 * d__3);
																			frac = ds / ss;
																			*bx = bx1 * (1. - frac) + bx2 * frac;
																			*by = by1 * (1. - frac) + by2 * frac;
																			*bz = bz1 * (1. - frac) + bz2 * frac;

																		}

																		/*   NOW, LET US ADD THE SHIELDING FIELD */

																		/* END OF THE CASE 4 */
																		birk1shld_(ps, x, y, z__, &bsx, &bsy, &bsz);
																		*bx += bsx;
																		*by += bsy;
																		*bz += bsz;
																		return 0;
} /* birk1tot_02__ */


/* ------------------------------------------------------------------------------ */


/* Subroutine */ int diploop1_(doublereal *xi, doublereal *d__)
{
	/* System generated locals */
	doublereal d__1, d__2, d__3;

	/* Builtin functions */
	double sin(doublereal), sqrt(doublereal);

	/* Local variables */
	static doublereal c__;
	static integer i__;
	static doublereal q, r__, x, y, z__, r2, bx, xd, yd, zd, by, bz, ps, dr2, 
		sqm, sqp, sps, bx1x, by1x, bz1x, bx1y, by1y, bz1y, bx1z, by1z, 
		bz1z, bx2x, by2x, bz2x, bx2y, by2y, bz2y, bx2z, by2z, bz2z, rmrh, 
		rprh, xoct1, yoct1, zoct1, xoct2, yoct2, zoct2, cpsas, spsas, 
		bxoct1, byoct1, bzoct1;
	extern /* Subroutine */ int circle_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *), dipxyz_(doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *), crosslp_(doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *);



	/*      Calculates dependent model variables and their deriva- */
	/*  tives for given independent variables and model parame- */
	/*  ters.  Specifies model functions with free parameters which */
	/*  must be determined by means of least squares fits (RMS */
	/*  minimization procedure). */

	/*      Description of parameters: */

	/*  XI  - input vector containing independent variables; */
	/*  D   - output double precision vector containing */
	/*        calculated values for derivatives of dependent */
	/*        variables with respect to LINEAR model parameters; */

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	/*  The  26 coefficients are moments (Z- and X-components) of 12 dipoles placed */
	/*    inside the  R1-shell,  PLUS amplitudes of two octagonal double loops. */
	/*     The dipoles with nonzero  Yi appear in pairs with equal moments. */
	/*                  (see the notebook #2, pp.102-103, for details) */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



	/* Parameter adjustments */
	d__ -= 4;
	--xi;

	/* Function Body */
	x = xi[1];
	y = xi[2];
	z__ = xi[3];
	ps = xi[4];
	sps = sin(ps);

	for (i__ = 1; i__ <= 12; ++i__) {
		/* Computing 2nd power */
		d__1 = coord11_2.xx[i__ - 1] * loopdip1_1.dipx;
		/* Computing 2nd power */
		d__2 = coord11_2.yy[i__ - 1] * loopdip1_1.dipy;
		r2 = d__1 * d__1 + d__2 * d__2;
		r__ = sqrt(r2);
		rmrh = r__ - rhdr_1.rh;
		rprh = r__ + rhdr_1.rh;
		/* Computing 2nd power */
		d__1 = rhdr_1.dr;
		dr2 = d__1 * d__1;
		/* Computing 2nd power */
		d__1 = rmrh;
		sqm = sqrt(d__1 * d__1 + dr2);
		/* Computing 2nd power */
		d__1 = rprh;
		sqp = sqrt(d__1 * d__1 + dr2);
		c__ = sqp - sqm;
		/* Computing 2nd power */
		d__1 = rhdr_1.rh + 1.;
		/* Computing 2nd power */
		d__2 = rhdr_1.rh - 1.;
		q = sqrt(d__1 * d__1 + dr2) - sqrt(d__2 * d__2 + dr2);
		spsas = sps / r__ * c__ / q;
		/* Computing 2nd power */
		d__1 = spsas;
		cpsas = sqrt(1. - d__1 * d__1);
		xd = coord11_2.xx[i__ - 1] * loopdip1_1.dipx * cpsas;
		yd = coord11_2.yy[i__ - 1] * loopdip1_1.dipy;
		zd = -(coord11_2.xx[i__ - 1] * loopdip1_1.dipx) * spsas;
		d__1 = x - xd;
		d__2 = y - yd;
		d__3 = z__ - zd;
		dipxyz_(&d__1, &d__2, &d__3, &bx1x, &by1x, &bz1x, &bx1y, &by1y, &bz1y,
			&bx1z, &by1z, &bz1z);
		if (abs(yd) > 1e-10) {
			d__1 = x - xd;
			d__2 = y + yd;
			d__3 = z__ - zd;
			dipxyz_(&d__1, &d__2, &d__3, &bx2x, &by2x, &bz2x, &bx2y, &by2y, &
				bz2y, &bx2z, &by2z, &bz2z);
		} else {
			bx2x = 0.;
			by2x = 0.;
			bz2x = 0.;

			bx2z = 0.;
			by2z = 0.;
			bz2z = 0.;
		}

		d__[i__ * 3 + 1] = bx1z + bx2z;
		d__[i__ * 3 + 2] = by1z + by2z;
		d__[i__ * 3 + 3] = bz1z + bz2z;
		d__[(i__ + 12) * 3 + 1] = (bx1x + bx2x) * sps;
		d__[(i__ + 12) * 3 + 2] = (by1x + by2x) * sps;
		d__[(i__ + 12) * 3 + 3] = (bz1x + bz2x) * sps;
		/* L1: */
	}

	/* Computing 2nd power */
	d__1 = loopdip1_1.xcentre[0] + loopdip1_1.radius[0];
	r2 = d__1 * d__1;
	r__ = sqrt(r2);
	rmrh = r__ - rhdr_1.rh;
	rprh = r__ + rhdr_1.rh;
	/* Computing 2nd power */
	d__1 = rhdr_1.dr;
	dr2 = d__1 * d__1;
	/* Computing 2nd power */
	d__1 = rmrh;
	sqm = sqrt(d__1 * d__1 + dr2);
	/* Computing 2nd power */
	d__1 = rprh;
	sqp = sqrt(d__1 * d__1 + dr2);
	c__ = sqp - sqm;
	/* Computing 2nd power */
	d__1 = rhdr_1.rh + 1.;
	/* Computing 2nd power */
	d__2 = rhdr_1.rh - 1.;
	q = sqrt(d__1 * d__1 + dr2) - sqrt(d__2 * d__2 + dr2);
	spsas = sps / r__ * c__ / q;
	/* Computing 2nd power */
	d__1 = spsas;
	cpsas = sqrt(1. - d__1 * d__1);
	xoct1 = x * cpsas - z__ * spsas;
	yoct1 = y;
	zoct1 = x * spsas + z__ * cpsas;

	crosslp_(&xoct1, &yoct1, &zoct1, &bxoct1, &byoct1, &bzoct1, 
		loopdip1_1.xcentre, loopdip1_1.radius, &loopdip1_1.tilt);
	d__[76] = bxoct1 * cpsas + bzoct1 * spsas;
	d__[77] = byoct1;
	d__[78] = -bxoct1 * spsas + bzoct1 * cpsas;

	/* Computing 2nd power */
	d__1 = loopdip1_1.radius[1] - loopdip1_1.xcentre[1];
	r2 = d__1 * d__1;
	r__ = sqrt(r2);
	rmrh = r__ - rhdr_1.rh;
	rprh = r__ + rhdr_1.rh;
	/* Computing 2nd power */
	d__1 = rhdr_1.dr;
	dr2 = d__1 * d__1;
	/* Computing 2nd power */
	d__1 = rmrh;
	sqm = sqrt(d__1 * d__1 + dr2);
	/* Computing 2nd power */
	d__1 = rprh;
	sqp = sqrt(d__1 * d__1 + dr2);
	c__ = sqp - sqm;
	/* Computing 2nd power */
	d__1 = rhdr_1.rh + 1.;
	/* Computing 2nd power */
	d__2 = rhdr_1.rh - 1.;
	q = sqrt(d__1 * d__1 + dr2) - sqrt(d__2 * d__2 + dr2);
	spsas = sps / r__ * c__ / q;
	/* Computing 2nd power */
	d__1 = spsas;
	cpsas = sqrt(1. - d__1 * d__1);
	xoct2 = x * cpsas - z__ * spsas - loopdip1_1.xcentre[1];
	yoct2 = y;
	zoct2 = x * spsas + z__ * cpsas;
	circle_(&xoct2, &yoct2, &zoct2, &loopdip1_1.radius[1], &bx, &by, &bz);
	d__[79] = bx * cpsas + bz * spsas;
	d__[80] = by;
	d__[81] = -bx * spsas + bz * cpsas;

	return 0;
} /* diploop1_ */

/* ------------------------------------------------------------------------- */

/* Subroutine */ int circle_(doublereal *x, doublereal *y, doublereal *z__, 
							 doublereal *rl, doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal pi = 3.141592654;

	/* System generated locals */
	doublereal d__1;

	/* Builtin functions */
	double sqrt(doublereal), log(doublereal);

	/* Local variables */
	static doublereal e, k, r2, dl, r12, r22, r32, xk2, rho, rho2, xk2s, brho;


	/*  RETURNS COMPONENTS OF THE FIELD FROM A CIRCULAR CURRENT LOOP OF RADIUS RL */
	/*  USES THE SECOND (MORE ACCURATE) APPROXIMATION GIVEN IN ABRAMOWITZ AND STEGUN */

	rho2 = *x * *x + *y * *y;
	rho = sqrt(rho2);
	/* Computing 2nd power */
	d__1 = rho + *rl;
	r22 = *z__ * *z__ + d__1 * d__1;
	r2 = sqrt(r22);
	r12 = r22 - rho * 4. * *rl;
	r32 = (r12 + r22) * .5;
	xk2 = 1. - r12 / r22;
	xk2s = 1. - xk2;
	dl = log(1. / xk2s);
	k = xk2s * (xk2s * (xk2s * (xk2s * .01451196212f + .03742563713f) + 
		.03590092383f) + .09666344259) + 1.38629436112 + dl * (xk2s * (
		xk2s * (xk2s * (xk2s * .00441787012 + .03328355346) + 
		.06880248576) + .12498593597) + .5);
	e = xk2s * (xk2s * (xk2s * (xk2s * .01736506451 + .04757383546) + 
		.0626060122) + .44325141463) + 1. + dl * xk2s * (xk2s * (xk2s * (
		xk2s * .00526449639 + .04069697526) + .09200180037) + .2499836831)
		;
	if (rho > 1e-6) {
		brho = *z__ / (rho2 * r2) * (r32 / r12 * e - k);
		/*  THIS IS NOT EXACTLY THE B-R */
	} else {
		/*   PONENT - NOTE THE ADDITION */
		brho = pi * *rl / r2 * (*rl - rho) / r12 * *z__ / (r32 - rho2);
		/*      DIVISION BY R */
	}
	*bx = brho * *x;
	*by = brho * *y;
	*bz = (k - e * (r32 - *rl * 2. * *rl) / r12) / r2;
	return 0;
} /* circle_ */

/* ------------------------------------------------------------- */

/* Subroutine */ int crosslp_(doublereal *x, doublereal *y, doublereal *z__, 
							  doublereal *bx, doublereal *by, doublereal *bz, doublereal *xc, 
							  doublereal *rl, doublereal *al)
{
	/* System generated locals */
	doublereal d__1;

	/* Builtin functions */
	double cos(doublereal), sin(doublereal);

	/* Local variables */
	static doublereal y1, z1, y2, z2, bx1, by1, bz1, bx2, by2, bz2, cal, sal;
	extern /* Subroutine */ int circle_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *);


	/*   RETURNS FIELD COMPONENTS OF A PAIR OF LOOPS WITH A COMMON CENTER AND */
	/*    DIAMETER,  COINCIDING WITH THE X AXIS. THE LOOPS ARE INCLINED TO THE */
	/*    EQUATORIAL PLANE BY THE ANGLE AL (RADIANS) AND SHIFTED IN THE POSITIVE */
	/*     X-DIRECTION BY THE DISTANCE  XC. */


	cal = cos(*al);
	sal = sin(*al);

	y1 = *y * cal - *z__ * sal;
	z1 = *y * sal + *z__ * cal;
	y2 = *y * cal + *z__ * sal;
	z2 = -(*y) * sal + *z__ * cal;
	d__1 = *x - *xc;
	circle_(&d__1, &y1, &z1, rl, &bx1, &by1, &bz1);
	d__1 = *x - *xc;
	circle_(&d__1, &y2, &z2, rl, &bx2, &by2, &bz2);
	*bx = bx1 + bx2;
	*by = (by1 + by2) * cal + (bz1 - bz2) * sal;
	*bz = -(by1 - by2) * sal + (bz1 + bz2) * cal;

	return 0;
} /* crosslp_ */

/* ******************************************************************* */
/* Subroutine */ int dipxyz_(doublereal *x, doublereal *y, doublereal *z__, 
							 doublereal *bxx, doublereal *byx, doublereal *bzx, doublereal *bxy, 
							 doublereal *byy, doublereal *bzy, doublereal *bxz, doublereal *byz, 
							 doublereal *bzz)
{
	/* System generated locals */
	doublereal d__1;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	static doublereal r2, x2, y2, z2, xmr5, xmr53;


	/*       RETURNS THE FIELD COMPONENTS PRODUCED BY THREE DIPOLES, EACH */
	/*        HAVING M=Me AND ORIENTED PARALLEL TO X,Y, and Z AXIS, RESP. */


	/* Computing 2nd power */
	d__1 = *x;
	x2 = d__1 * d__1;
	/* Computing 2nd power */
	d__1 = *y;
	y2 = d__1 * d__1;
	/* Computing 2nd power */
	d__1 = *z__;
	z2 = d__1 * d__1;
	r2 = x2 + y2 + z2;
	xmr5 = 30574. / (r2 * r2 * sqrt(r2));
	xmr53 = xmr5 * 3.;
	*bxx = xmr5 * (x2 * 3. - r2);
	*byx = xmr53 * *x * *y;
	*bzx = xmr53 * *x * *z__;

	*bxy = *byx;
	*byy = xmr5 * (y2 * 3. - r2);
	*bzy = xmr53 * *y * *z__;

	*bxz = *bzx;
	*byz = *bzy;
	*bzz = xmr5 * (z2 * 3. - r2);

	return 0;
} /* dipxyz_ */


/* ------------------------------------------------------------------------------ */
/* Subroutine */ int condip1_(doublereal *xi, doublereal *d__)
{
	/* System generated locals */
	integer i__1, i__2;
	doublereal d__1, d__2, d__3;

	/* Builtin functions */
	double sin(doublereal), cos(doublereal), sqrt(doublereal), pow_di(
		doublereal *, integer *);

	/* Local variables */
	static doublereal c__;
	static integer i__, m;
	static doublereal r__, s, x, y, z__, r2, bf, cf[5], ch, bt, sf[5], sh, by,
		xd, yd, zd, ro, ps;
	static integer ix, iy, iz;
	static doublereal ro2, cnh, cps, tnh, sps, xsm, zsm, bx1x, by1x, bz1x, 
		bx1y, by1y, bz1y, bx1z, by1z, bz1z, bx2x, by2x, bz2x, bx2y, by2y, 
		bz2y, bx2z, by2z, bz2z, bx3x, by3x, bz3x, bx3y, by3y, bz3y, bx3z, 
		by3z, bz3z, bx4x, by4x, bz4x, bx4y, by4y, bz4y, bx4z, by4z, bz4z, 
		bxsm, bzsm;
	extern /* Subroutine */ int dipxyz_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *);


	/*      Calculates dependent model variables and their derivatives for given */
	/*  independent variables and model parameters.  Specifies model functions with */
	/*  free parameters which must be determined by means of least squares fits */
	/*  (RMS minimization procedure). */

	/*      Description of parameters: */

	/*  XI  - input vector containing independent variables; */
	/*  D   - output double precision vector containing */
	/*        calculated values for derivatives of dependent */
	/*        variables with respect to LINEAR model parameters; */

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	/*  The  79 coefficients are (1) 5 amplitudes of the conical harmonics, plus */
	/*                           (2) (9x3+5x2)x2=74 components of the dipole moments */
	/*              (see the notebook #2, pp.113-..., for details) */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */




	/* Parameter adjustments */
	d__ -= 4;
	--xi;

	/* Function Body */
	x = xi[1];
	y = xi[2];
	z__ = xi[3];
	ps = xi[4];
	sps = sin(ps);
	cps = cos(ps);

	xsm = x * cps - z__ * sps - dx1_1.dx;
	zsm = z__ * cps + x * sps;
	/* Computing 2nd power */
	d__1 = xsm;
	/* Computing 2nd power */
	d__2 = y;
	ro2 = d__1 * d__1 + d__2 * d__2;
	ro = sqrt(ro2);

	cf[0] = xsm / ro;
	sf[0] = y / ro;

	/* Computing 2nd power */
	d__1 = cf[0];
	/* Computing 2nd power */
	d__2 = sf[0];
	cf[1] = d__1 * d__1 - d__2 * d__2;
	sf[1] = sf[0] * 2.f * cf[0];
	cf[2] = cf[1] * cf[0] - sf[1] * sf[0];
	sf[2] = sf[1] * cf[0] + cf[1] * sf[0];
	cf[3] = cf[2] * cf[0] - sf[2] * sf[0];
	sf[3] = sf[2] * cf[0] + cf[2] * sf[0];
	cf[4] = cf[3] * cf[0] - sf[3] * sf[0];
	sf[4] = sf[3] * cf[0] + cf[3] * sf[0];

	/* Computing 2nd power */
	d__1 = zsm;
	r2 = ro2 + d__1 * d__1;
	r__ = sqrt(r2);
	c__ = zsm / r__;
	s = ro / r__;
	ch = sqrt((c__ + 1.) * .5);
	sh = sqrt((1. - c__) * .5);
	tnh = sh / ch;
	cnh = 1. / tnh;

	for (m = 1; m <= 5; ++m) {
		bt = m * cf[m - 1] / (r__ * s) * (pow_di(&tnh, &m) + pow_di(&cnh, &m))
			;
		i__1 = m - 1;
		/* Computing 2nd power */
		d__1 = ch;
		i__2 = m - 1;
		/* Computing 2nd power */
		d__2 = sh;
		bf = m * -.5 * sf[m - 1] / r__ * (pow_di(&tnh, &i__1) / (d__1 * d__1) 
			- pow_di(&cnh, &i__2) / (d__2 * d__2));
		bxsm = bt * c__ * cf[0] - bf * sf[0];
		by = bt * c__ * sf[0] + bf * cf[0];
		bzsm = -bt * s;

		d__[m * 3 + 1] = bxsm * cps + bzsm * sps;
		d__[m * 3 + 2] = by;
		/* L1: */
		d__[m * 3 + 3] = -bxsm * sps + bzsm * cps;
	}

	xsm = x * cps - z__ * sps;
	zsm = z__ * cps + x * sps;

	for (i__ = 1; i__ <= 9; ++i__) {

		if (i__ == 3 || i__ == 5 || i__ == 6) {
			xd = coord21_2.xx[i__ - 1] * dx1_1.scalein;
			yd = coord21_2.yy[i__ - 1] * dx1_1.scalein;
		} else {
			xd = coord21_2.xx[i__ - 1] * dx1_1.scaleout;
			yd = coord21_2.yy[i__ - 1] * dx1_1.scaleout;
		}

		zd = coord21_2.zz[i__ - 1];

		d__1 = xsm - xd;
		d__2 = y - yd;
		d__3 = zsm - zd;
		dipxyz_(&d__1, &d__2, &d__3, &bx1x, &by1x, &bz1x, &bx1y, &by1y, &bz1y,
			&bx1z, &by1z, &bz1z);
		d__1 = xsm - xd;
		d__2 = y + yd;
		d__3 = zsm - zd;
		dipxyz_(&d__1, &d__2, &d__3, &bx2x, &by2x, &bz2x, &bx2y, &by2y, &bz2y,
			&bx2z, &by2z, &bz2z);
		d__1 = xsm - xd;
		d__2 = y - yd;
		d__3 = zsm + zd;
		dipxyz_(&d__1, &d__2, &d__3, &bx3x, &by3x, &bz3x, &bx3y, &by3y, &bz3y,
			&bx3z, &by3z, &bz3z);
		d__1 = xsm - xd;
		d__2 = y + yd;
		d__3 = zsm + zd;
		dipxyz_(&d__1, &d__2, &d__3, &bx4x, &by4x, &bz4x, &bx4y, &by4y, &bz4y,
			&bx4z, &by4z, &bz4z);

		ix = i__ * 3 + 3;
		iy = ix + 1;
		iz = iy + 1;

		d__[ix * 3 + 1] = (bx1x + bx2x - bx3x - bx4x) * cps + (bz1x + bz2x - 
			bz3x - bz4x) * sps;
		d__[ix * 3 + 2] = by1x + by2x - by3x - by4x;
		d__[ix * 3 + 3] = (bz1x + bz2x - bz3x - bz4x) * cps - (bx1x + bx2x - 
			bx3x - bx4x) * sps;

		d__[iy * 3 + 1] = (bx1y - bx2y - bx3y + bx4y) * cps + (bz1y - bz2y - 
			bz3y + bz4y) * sps;
		d__[iy * 3 + 2] = by1y - by2y - by3y + by4y;
		d__[iy * 3 + 3] = (bz1y - bz2y - bz3y + bz4y) * cps - (bx1y - bx2y - 
			bx3y + bx4y) * sps;

		d__[iz * 3 + 1] = (bx1z + bx2z + bx3z + bx4z) * cps + (bz1z + bz2z + 
			bz3z + bz4z) * sps;
		d__[iz * 3 + 2] = by1z + by2z + by3z + by4z;
		d__[iz * 3 + 3] = (bz1z + bz2z + bz3z + bz4z) * cps - (bx1z + bx2z + 
			bx3z + bx4z) * sps;

		ix += 27;
		iy += 27;
		iz += 27;

		d__[ix * 3 + 1] = sps * ((bx1x + bx2x + bx3x + bx4x) * cps + (bz1x + 
			bz2x + bz3x + bz4x) * sps);
		d__[ix * 3 + 2] = sps * (by1x + by2x + by3x + by4x);
		d__[ix * 3 + 3] = sps * ((bz1x + bz2x + bz3x + bz4x) * cps - (bx1x + 
			bx2x + bx3x + bx4x) * sps);

		d__[iy * 3 + 1] = sps * ((bx1y - bx2y + bx3y - bx4y) * cps + (bz1y - 
			bz2y + bz3y - bz4y) * sps);
		d__[iy * 3 + 2] = sps * (by1y - by2y + by3y - by4y);
		d__[iy * 3 + 3] = sps * ((bz1y - bz2y + bz3y - bz4y) * cps - (bx1y - 
			bx2y + bx3y - bx4y) * sps);

		d__[iz * 3 + 1] = sps * ((bx1z + bx2z - bx3z - bx4z) * cps + (bz1z + 
			bz2z - bz3z - bz4z) * sps);
		d__[iz * 3 + 2] = sps * (by1z + by2z - by3z - by4z);
		d__[iz * 3 + 3] = sps * ((bz1z + bz2z - bz3z - bz4z) * cps - (bx1z + 
			bx2z - bx3z - bx4z) * sps);
		/* L2: */
	}

	for (i__ = 1; i__ <= 5; ++i__) {
		zd = coord21_2.zz[i__ + 8];
		d__1 = zsm - zd;
		dipxyz_(&xsm, &y, &d__1, &bx1x, &by1x, &bz1x, &bx1y, &by1y, &bz1y, &
			bx1z, &by1z, &bz1z);
		d__1 = zsm + zd;
		dipxyz_(&xsm, &y, &d__1, &bx2x, &by2x, &bz2x, &bx2y, &by2y, &bz2y, &
			bx2z, &by2z, &bz2z);
		ix = (i__ << 1) + 58;
		iz = ix + 1;
		d__[ix * 3 + 1] = (bx1x - bx2x) * cps + (bz1x - bz2x) * sps;
		d__[ix * 3 + 2] = by1x - by2x;
		d__[ix * 3 + 3] = (bz1x - bz2x) * cps - (bx1x - bx2x) * sps;

		d__[iz * 3 + 1] = (bx1z + bx2z) * cps + (bz1z + bz2z) * sps;
		d__[iz * 3 + 2] = by1z + by2z;
		d__[iz * 3 + 3] = (bz1z + bz2z) * cps - (bx1z + bx2z) * sps;

		ix += 10;
		iz += 10;
		d__[ix * 3 + 1] = sps * ((bx1x + bx2x) * cps + (bz1x + bz2x) * sps);
		d__[ix * 3 + 2] = sps * (by1x + by2x);
		d__[ix * 3 + 3] = sps * ((bz1x + bz2x) * cps - (bx1x + bx2x) * sps);

		d__[iz * 3 + 1] = sps * ((bx1z - bx2z) * cps + (bz1z - bz2z) * sps);
		d__[iz * 3 + 2] = sps * (by1z - by2z);
		/* L3: */
		d__[iz * 3 + 3] = sps * ((bz1z - bz2z) * cps - (bx1z - bx2z) * sps);
	}

	return 0;
} /* condip1_ */


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */

/* Subroutine */ int birk1shld_(doublereal *ps, doublereal *x, doublereal *y, 
								doublereal *z__, doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static struct {
		doublereal e_1[80];
	} equiv_3 = { 1.174198045, -1.463820502, 4.840161537, -3.674506864, 
		82.18368896, -94.94071588, -4122.331796, 4670.278676, 
		-21.54975037, 26.72661293, -72.81365728, 44.09887902, 
		40.08073706, -51.2356351, 1955.348537, -1940.97155, 
		794.0496433, -982.2441344, 1889.837171, -558.9779727, 
		-1260.543238, 1260.063802, -293.5942373, 344.7250789, 
		-773.7002492, 957.0094135, -1824.143669, 520.7994379, 
		1192.484774, -1192.184565, 89.15537624, -98.52042999, 
		-.08168777675, .04255969908, .3155237661, -.3841755213, 
		2.494553332, -.06571440817, -2.76566131, .4331001908, 
		.1099181537, -.0615412698, -.325864926, .6698439193, 
		-5.542735524, .1604203535, 5.854456934, -.8323632049, 
		3.732608869, -3.130002153, 107.0972607, -32.28483411, 
		-115.2389298, 54.4506436, -.582685332, -3.582482231, 
		-4.046544561, 3.311978102, -104.0839563, 30.26401293, 
		97.29109008, -50.62370872, -296.3734955, 127.7872523, 
		5.303648988, 10.40368955, 69.65230348, 466.5099509, 
		1.645049286, 3.82583819, 11.66675599, 558.9781177, 
		1.826531343, 2.066018073, 25.40971369, 990.2795225, 
		2.319489258, 4.555148484, 9.691185703, 591.8280358 };


	/* System generated locals */
	doublereal d__1, d__2;

	/* Builtin functions */
	double cos(doublereal), sin(doublereal), sqrt(doublereal), exp(doublereal)
		;

	/* Local variables */
#define a ((doublereal *)&equiv_3)
	static integer i__, k, l, m, n;
#define p1 ((doublereal *)&equiv_3 + 64)
#define q1 ((doublereal *)&equiv_3 + 72)
#define r1 ((doublereal *)&equiv_3 + 68)
#define s1 ((doublereal *)&equiv_3 + 76)
	static doublereal hx, hy, rp[4], rr[4], rq[4], rs[4], hz, cps, epr, eqs, 
		sps, s3ps, cypi, cyqi, czrk, czsk, sypi, syqi, sqpr, sqqs, szrk, 
		szsk;


	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	/*  The 64 linear parameters are amplitudes of the "box" harmonics. */
	/* The 16 nonlinear parameters are the scales Pi, and Qk entering the arguments */
	/*  of sines/cosines and exponents in each of  32 cartesian harmonics */
	/*  N.A. Tsyganenko, Spring 1994, adjusted for the Birkeland field Aug.22, 1995 */
	/*    Revised  June 12, 1996. */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */





	*bx = 0.;
	*by = 0.;
	*bz = 0.;
	cps = cos(*ps);
	sps = sin(*ps);
	/* Computing 2nd power */
	d__1 = cps;
	s3ps = d__1 * d__1 * 4. - 1.;

	for (i__ = 1; i__ <= 4; ++i__) {
		rp[i__ - 1] = 1. / p1[i__ - 1];
		rr[i__ - 1] = 1. / r1[i__ - 1];
		rq[i__ - 1] = 1. / q1[i__ - 1];
		/* L11: */
		rs[i__ - 1] = 1. / s1[i__ - 1];
	}

	l = 0;

	for (m = 1; m <= 2; ++m) {
		/*                           AND M=2 IS FOR THE SECOND SUM ("PARALL." SYMMETRY) */
		/*    M=1 IS FOR THE 1ST SUM ("PERP." SYMMETRY) */
		for (i__ = 1; i__ <= 4; ++i__) {
			cypi = cos(*y * rp[i__ - 1]);
			cyqi = cos(*y * rq[i__ - 1]);
			sypi = sin(*y * rp[i__ - 1]);
			syqi = sin(*y * rq[i__ - 1]);

			for (k = 1; k <= 4; ++k) {
				szrk = sin(*z__ * rr[k - 1]);
				czsk = cos(*z__ * rs[k - 1]);
				czrk = cos(*z__ * rr[k - 1]);
				szsk = sin(*z__ * rs[k - 1]);
				/* Computing 2nd power */
				d__1 = rp[i__ - 1];
				/* Computing 2nd power */
				d__2 = rr[k - 1];
				sqpr = sqrt(d__1 * d__1 + d__2 * d__2);
				/* Computing 2nd power */
				d__1 = rq[i__ - 1];
				/* Computing 2nd power */
				d__2 = rs[k - 1];
				sqqs = sqrt(d__1 * d__1 + d__2 * d__2);
				epr = exp(*x * sqpr);
				eqs = exp(*x * sqqs);

				for (n = 1; n <= 2; ++n) {
					/*                                  AND N=2 IS FOR THE SECOND ONE */
					/* N=1 IS FOR THE FIRST PART OF EACH COEF */
					if (m == 1) {
						if (n == 1) {
							hx = -sqpr * epr * cypi * szrk;
							hy = rp[i__ - 1] * epr * sypi * szrk;
							hz = -rr[k - 1] * epr * cypi * czrk;
						} else {
							hx *= cps;
							hy *= cps;
							hz *= cps;
						}
					} else {
						if (n == 1) {
							hx = -sps * sqqs * eqs * cyqi * czsk;
							hy = sps * rq[i__ - 1] * eqs * syqi * czsk;
							hz = sps * rs[k - 1] * eqs * cyqi * szsk;
						} else {
							hx *= s3ps;
							hy *= s3ps;
							hz *= s3ps;
						}
					}
					++l;

					*bx += a[l - 1] * hx;
					*by += a[l - 1] * hy;
					/* L4: */
					*bz += a[l - 1] * hz;
				}
				/* L3: */
			}
			/* L2: */
		}
		/* L1: */
	}

	return 0;
} /* birk1shld_ */

#undef s1
#undef r1
#undef q1
#undef p1
#undef a



/* ########################################################################## */

/* Subroutine */ int birk2tot_02__(doublereal *ps, doublereal *x, doublereal *
								   y, doublereal *z__, doublereal *bx, doublereal *by, doublereal *bz)
{
	static doublereal hx, hy, hz, wx, wy, wz;
	extern /* Subroutine */ int r2_birk__(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *), birk2shl_(doublereal *, doublereal *, doublereal *,
		doublereal *, doublereal *, doublereal *, doublereal *);



	birk2shl_(x, y, z__, ps, &wx, &wy, &wz);
	r2_birk__(x, y, z__, ps, &hx, &hy, &hz);
	*bx = wx + hx;
	*by = wy + hy;
	*bz = wz + hz;
	return 0;
} /* birk2tot_02__ */


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */

/* THIS CODE IS FOR THE FIELD FROM  2x2x2=8 "CARTESIAN" HARMONICS */

/* Subroutine */ int birk2shl_(doublereal *x, doublereal *y, doublereal *z__, 
							   doublereal *ps, doublereal *hx, doublereal *hy, doublereal *hz)
{
	/* Initialized data */

	static struct {
		doublereal e_1[24];
	} equiv_3 = { -111.6371348, 124.5402702, 110.3735178, -122.0095905, 
		111.9448247, -129.1957743, -110.7586562, 126.5649012, 
		-.7865034384, -.2483462721, .8026023894, .2531397188, 
		10.72890902, .8483902118, -10.96884315, -.8583297219, 
		13.85650567, 14.905545, 10.21914434, 10.09021632, 6.34038246, 
		14.40432686, 12.71023437, 12.83966657 };


	/* System generated locals */
	doublereal d__1, d__2;

	/* Builtin functions */
	double cos(doublereal), sin(doublereal), sqrt(doublereal), exp(doublereal)
		;

	/* Local variables */
#define a ((doublereal *)&equiv_3)
	static integer i__, k, l, m, n;
#define p ((doublereal *)&equiv_3 + 16)
#define q ((doublereal *)&equiv_3 + 20)
#define r__ ((doublereal *)&equiv_3 + 18)
#define s ((doublereal *)&equiv_3 + 22)
	static doublereal dx, dy, dz, cps, epr, eqs, sps, s3ps, cypi, cyqi, czrk, 
		czsk, sypi, syqi, sqpr, sqqs, szrk, szsk;


	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*    The model parameters are provided to this module via common-block /A/. */
	/*  The 16 linear parameters enter in pairs in the amplitudes of the */
	/*       "cartesian" harmonics. */
	/*    The 8 nonlinear parameters are the scales Pi,Ri,Qi,and Si entering the */
	/*  arguments of exponents, sines, and cosines in each of the 8 "Cartesian" */
	/*   harmonics */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */




	cps = cos(*ps);
	sps = sin(*ps);
	/* Computing 2nd power */
	d__1 = cps;
	s3ps = d__1 * d__1 * 4. - 1.;

	/*  THIS IS SIN(3*PS)/SIN(PS) */
	*hx = 0.;
	*hy = 0.;
	*hz = 0.;
	l = 0;

	for (m = 1; m <= 2; ++m) {
		/*                           AND M=2 IS FOR THE SECOND SUM ("PARALL." SYMMETRY) */
		/*    M=1 IS FOR THE 1ST SUM ("PERP." SYMMETRY) */
		for (i__ = 1; i__ <= 2; ++i__) {
			cypi = cos(*y / p[i__ - 1]);
			cyqi = cos(*y / q[i__ - 1]);
			sypi = sin(*y / p[i__ - 1]);
			syqi = sin(*y / q[i__ - 1]);

			for (k = 1; k <= 2; ++k) {
				szrk = sin(*z__ / r__[k - 1]);
				czsk = cos(*z__ / s[k - 1]);
				czrk = cos(*z__ / r__[k - 1]);
				szsk = sin(*z__ / s[k - 1]);
				/* Computing 2nd power */
				d__1 = p[i__ - 1];
				/* Computing 2nd power */
				d__2 = r__[k - 1];
				sqpr = sqrt(1. / (d__1 * d__1) + 1. / (d__2 * d__2));
				/* Computing 2nd power */
				d__1 = q[i__ - 1];
				/* Computing 2nd power */
				d__2 = s[k - 1];
				sqqs = sqrt(1. / (d__1 * d__1) + 1. / (d__2 * d__2));
				epr = exp(*x * sqpr);
				eqs = exp(*x * sqqs);

				for (n = 1; n <= 2; ++n) {
					/*                                  AND N=2 IS FOR THE SECOND ONE */

					/* N=1 IS FOR THE FIRST PART OF EACH COEFF */
					++l;
					if (m == 1) {
						if (n == 1) {
							dx = -sqpr * epr * cypi * szrk;
							dy = epr / p[i__ - 1] * sypi * szrk;
							dz = -epr / r__[k - 1] * cypi * czrk;
							*hx += a[l - 1] * dx;
							*hy += a[l - 1] * dy;
							*hz += a[l - 1] * dz;
						} else {
							dx *= cps;
							dy *= cps;
							dz *= cps;
							*hx += a[l - 1] * dx;
							*hy += a[l - 1] * dy;
							*hz += a[l - 1] * dz;
						}
					} else {
						if (n == 1) {
							dx = -sps * sqqs * eqs * cyqi * czsk;
							dy = sps * eqs / q[i__ - 1] * syqi * czsk;
							dz = sps * eqs / s[k - 1] * cyqi * szsk;
							*hx += a[l - 1] * dx;
							*hy += a[l - 1] * dy;
							*hz += a[l - 1] * dz;
						} else {
							dx *= s3ps;
							dy *= s3ps;
							dz *= s3ps;
							*hx += a[l - 1] * dx;
							*hy += a[l - 1] * dy;
							*hz += a[l - 1] * dz;
						}
					}

					/* L4: */
				}
				/* L3: */
			}
			/* L2: */
		}
		/* L1: */
	}

	return 0;
} /* birk2shl_ */

#undef s
#undef r__
#undef q
#undef p
#undef a


/* ******************************************************************** */

/* Subroutine */ int r2_birk__(doublereal *x, doublereal *y, doublereal *z__, 
							   doublereal *ps, doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal delarg = .03;
	static doublereal delarg1 = .015;
	static doublereal psi = 10.;

	/* System generated locals */
	doublereal d__1;

	/* Builtin functions */
	double cos(doublereal), sin(doublereal);

	/* Local variables */
	static doublereal f1, f2, by1, by2, cps, sps, xks, xsm, zsm, bxsm;
	extern doublereal tksi_(doublereal *, doublereal *, doublereal *);
	static doublereal bzsm;
	extern doublereal xksi_(doublereal *, doublereal *, doublereal *);
	static doublereal bxsm1, bxsm2, bzsm1, bzsm2;
	extern /* Subroutine */ int r2sheet_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *), r2inner_(
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *), r2outer_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *);


	/*  RETURNS THE MODEL FIELD FOR THE REGION 2 BIRKELAND CURRENT/PARTIAL RC */
	/*    (WITHOUT SHIELDING FIELD) */


	if ((d__1 = psi - *ps, abs(d__1)) > 1e-10) {
		psi = *ps;
		cps = cos(*ps);
		sps = sin(*ps);
	}

	xsm = *x * cps - *z__ * sps;
	zsm = *z__ * cps + *x * sps;

	xks = xksi_(&xsm, y, &zsm);
	if (xks < -(delarg + delarg1)) {
		r2outer_(&xsm, y, &zsm, &bxsm, by, &bzsm);
		bxsm = -bxsm * .02f;
		/*  ALL COMPONENTS ARE MULTIPLIED BY THE */
		*by = -(*by) * .02f;
		/*  FACTOR -0.02, IN ORDER TO NORMALIZE THE */
		bzsm = -bzsm * .02f;
		/*  FIELD (SO THAT Bz=-1 nT at X=-5.3 RE, Y */
	}
	if (xks >= -(delarg + delarg1) && xks < -delarg + delarg1) {
		r2outer_(&xsm, y, &zsm, &bxsm1, &by1, &bzsm1);
		r2sheet_(&xsm, y, &zsm, &bxsm2, &by2, &bzsm2);
		d__1 = -delarg;
		f2 = tksi_(&xks, &d__1, &delarg1) * -.02f;
		f1 = -.02f - f2;
		bxsm = bxsm1 * f1 + bxsm2 * f2;
		*by = by1 * f1 + by2 * f2;
		bzsm = bzsm1 * f1 + bzsm2 * f2;
	}
	if (xks >= -delarg + delarg1 && xks < delarg - delarg1) {
		r2sheet_(&xsm, y, &zsm, &bxsm, by, &bzsm);
		bxsm = -bxsm * .02f;
		*by = -(*by) * .02f;
		bzsm = -bzsm * .02f;
	}
	if (xks >= delarg - delarg1 && xks < delarg + delarg1) {
		r2inner_(&xsm, y, &zsm, &bxsm1, &by1, &bzsm1);
		r2sheet_(&xsm, y, &zsm, &bxsm2, &by2, &bzsm2);
		f1 = tksi_(&xks, &delarg, &delarg1) * -.02f;
		f2 = -.02f - f1;
		bxsm = bxsm1 * f1 + bxsm2 * f2;
		*by = by1 * f1 + by2 * f2;
		bzsm = bzsm1 * f1 + bzsm2 * f2;
	}
	if (xks >= delarg + delarg1) {
		r2inner_(&xsm, y, &zsm, &bxsm, by, &bzsm);
		bxsm = -bxsm * .02f;
		*by = -(*by) * .02f;
		bzsm = -bzsm * .02f;
	}

	*bx = bxsm * cps + bzsm * sps;
	*bz = bzsm * cps - bxsm * sps;

	return 0;
} /* r2_birk__ */


/* **************************************************************** */

/* Subroutine */ int r2inner_(doublereal *x, doublereal *y, doublereal *z__, 
							  doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal pl1 = 154.185;
	static doublereal pl2 = -2.12446;
	static doublereal pl3 = .0601735;
	static doublereal pl4 = -.00153954;
	static doublereal pl5 = 3.55077e-5;
	static doublereal pl6 = 29.9996;
	static doublereal pl7 = 262.886;
	static doublereal pl8 = 99.9132;
	static doublereal pn1 = -8.1902;
	static doublereal pn2 = 6.5239;
	static doublereal pn3 = 5.504;
	static doublereal pn4 = 7.7815;
	static doublereal pn5 = .8573;
	static doublereal pn6 = 3.0986;
	static doublereal pn7 = .0774;
	static doublereal pn8 = -.038;

	/* System generated locals */
	doublereal d__1;

	/* Local variables */
	extern /* Subroutine */ int dipdistr_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, integer *)
		;
	static doublereal cbx[5], cby[5], cbz[5], dbx6, dby6, dbx8, dby8, dbz8, 
		dbz6, dbx7, dby7, dbz7;
	extern /* Subroutine */ int loops4_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *), bconic_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, integer *)
		;





	bconic_(x, y, z__, cbx, cby, cbz, &c__5);

	/*   NOW INTRODUCE  ONE  4-LOOP SYSTEM: */

	loops4_(x, y, z__, &dbx8, &dby8, &dbz8, &pn1, &pn2, &pn3, &pn4, &pn5, &
		pn6);

	d__1 = *x - pn7;
	dipdistr_(&d__1, y, z__, &dbx6, &dby6, &dbz6, &c__0);
	d__1 = *x - pn8;
	dipdistr_(&d__1, y, z__, &dbx7, &dby7, &dbz7, &c__1);
	/*                           NOW COMPUTE THE FIELD COMPONENTS: */
	*bx = pl1 * cbx[0] + pl2 * cbx[1] + pl3 * cbx[2] + pl4 * cbx[3] + pl5 * 
		cbx[4] + pl6 * dbx6 + pl7 * dbx7 + pl8 * dbx8;
	*by = pl1 * cby[0] + pl2 * cby[1] + pl3 * cby[2] + pl4 * cby[3] + pl5 * 
		cby[4] + pl6 * dby6 + pl7 * dby7 + pl8 * dby8;
	*bz = pl1 * cbz[0] + pl2 * cbz[1] + pl3 * cbz[2] + pl4 * cbz[3] + pl5 * 
		cbz[4] + pl6 * dbz6 + pl7 * dbz7 + pl8 * dbz8;

	return 0;
} /* r2inner_ */

/* ----------------------------------------------------------------------- */
/* Subroutine */ int bconic_(doublereal *x, doublereal *y, doublereal *z__, 
							 doublereal *cbx, doublereal *cby, doublereal *cbz, integer *nmax)
{
	/* System generated locals */
	integer i__1;
	doublereal d__1, d__2;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	static doublereal c__;
	static integer m;
	static doublereal r__, s, r2, bf, cf, ch, bt, sf, sh, ro, ro2, cfm, cnh, 
		sfm, tnh, cfm1, sfm1, cnhm, tnhm, cnhm1, tnhm1;


	/*   "CONICAL" HARMONICS */


	/* Parameter adjustments */
	--cbz;
	--cby;
	--cbx;

	/* Function Body */
	/* Computing 2nd power */
	d__1 = *x;
	/* Computing 2nd power */
	d__2 = *y;
	ro2 = d__1 * d__1 + d__2 * d__2;
	ro = sqrt(ro2);

	cf = *x / ro;
	sf = *y / ro;
	cfm1 = 1.;
	sfm1 = 0.;

	/* Computing 2nd power */
	d__1 = *z__;
	r2 = ro2 + d__1 * d__1;
	r__ = sqrt(r2);
	c__ = *z__ / r__;
	s = ro / r__;
	ch = sqrt((c__ + 1.) * .5);
	sh = sqrt((1. - c__) * .5);
	tnhm1 = 1.;
	cnhm1 = 1.;
	tnh = sh / ch;
	cnh = 1. / tnh;

	i__1 = *nmax;
	for (m = 1; m <= i__1; ++m) {
		cfm = cfm1 * cf - sfm1 * sf;
		sfm = cfm1 * sf + sfm1 * cf;
		cfm1 = cfm;
		sfm1 = sfm;
		tnhm = tnhm1 * tnh;
		cnhm = cnhm1 * cnh;
		bt = m * cfm / (r__ * s) * (tnhm + cnhm);
		/* Computing 2nd power */
		d__1 = ch;
		/* Computing 2nd power */
		d__2 = sh;
		bf = m * -.5 * sfm / r__ * (tnhm1 / (d__1 * d__1) - cnhm1 / (d__2 * 
			d__2));
		tnhm1 = tnhm;
		cnhm1 = cnhm;
		cbx[m] = bt * c__ * cf - bf * sf;
		cby[m] = bt * c__ * sf + bf * cf;
		/* L1: */
		cbz[m] = -bt * s;
	}

	return 0;
} /* bconic_ */

/* ------------------------------------------------------------------- */

/* Subroutine */ int dipdistr_(doublereal *x, doublereal *y, doublereal *z__, 
							   doublereal *bx, doublereal *by, doublereal *bz, integer *mode)
{
	/* System generated locals */
	doublereal d__1;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	static doublereal r2, r3, x2, rho2;


	/*   RETURNS FIELD COMPONENTS FROM A LINEAR DISTRIBUTION OF DIPOLAR SOURCES */
	/*     ON THE Z-AXIS.  THE PARAMETER MODE DEFINES HOW THE DIPOLE STRENGTH */
	/*     VARIES ALONG THE Z-AXIS:  MODE=0 IS FOR A STEP-FUNCTION (Mx=const > 0 */
	/*         FOR Z > 0, AND Mx=-const < 0 FOR Z < 0) */
	/*      WHILE MODE=1 IS FOR A LINEAR VARIATION OF THE DIPOLE MOMENT DENSITY */
	/*       SEE NB#3, PAGE 53 FOR DETAILS. */


	/* INPUT: X,Y,Z OF A POINT OF SPACE, AND MODE */

	x2 = *x * *x;
	rho2 = x2 + *y * *y;
	r2 = rho2 + *z__ * *z__;
	r3 = r2 * sqrt(r2);
	if (*mode == 0) {
		/* Computing 2nd power */
		d__1 = rho2;
		*bx = *z__ / (d__1 * d__1) * (r2 * (*y * *y - x2) - rho2 * x2) / r3;
		/* Computing 2nd power */
		d__1 = rho2;
		*by = -(*x) * *y * *z__ / (d__1 * d__1) * (r2 * 2. + rho2) / r3;
		*bz = *x / r3;
	} else {
		/* Computing 2nd power */
		d__1 = rho2;
		*bx = *z__ / (d__1 * d__1) * (*y * *y - x2);
		/* Computing 2nd power */
		d__1 = rho2;
		*by = *x * -2. * *y * *z__ / (d__1 * d__1);
		*bz = *x / rho2;
	}
	return 0;
} /* dipdistr_ */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* Subroutine */ int r2outer_(doublereal *x, doublereal *y, doublereal *z__, 
							  doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal pl1 = -34.105;
	static doublereal pl2 = -2.00019;
	static doublereal pl3 = 628.639;
	static doublereal pl4 = 73.4847;
	static doublereal pl5 = 12.5162;
	static doublereal pn1 = .55;
	static doublereal pn2 = .694;
	static doublereal pn3 = .0031;
	static doublereal pn4 = 1.55;
	static doublereal pn5 = 2.8;
	static doublereal pn6 = .1375;
	static doublereal pn7 = -.7;
	static doublereal pn8 = .2;
	static doublereal pn9 = .9625;
	static doublereal pn10 = -2.994;
	static doublereal pn11 = 2.925;
	static doublereal pn12 = -1.775;
	static doublereal pn13 = 4.3;
	static doublereal pn14 = -.275;
	static doublereal pn15 = 2.7;
	static doublereal pn16 = .4312;
	static doublereal pn17 = 1.55;

	/* System generated locals */
	doublereal d__1;

	/* Local variables */
	static doublereal dbx1, dby1, dbz1, dbx2, dby2, dbz2, dbx3, dby3, dbz3, 
		dbx4, dby4, dbz4, dbx5, dby5, dbz5;
	extern /* Subroutine */ int loops4_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *), circle_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *), crosslp_(doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *, doublereal *);




	/*    THREE PAIRS OF CROSSED LOOPS: */

	crosslp_(x, y, z__, &dbx1, &dby1, &dbz1, &pn1, &pn2, &pn3);
	crosslp_(x, y, z__, &dbx2, &dby2, &dbz2, &pn4, &pn5, &pn6);
	crosslp_(x, y, z__, &dbx3, &dby3, &dbz3, &pn7, &pn8, &pn9);

	/*    NOW AN EQUATORIAL LOOP ON THE NIGHTSIDE */

	d__1 = *x - pn10;
	circle_(&d__1, y, z__, &pn11, &dbx4, &dby4, &dbz4);

	/*   NOW A 4-LOOP SYSTEM ON THE NIGHTSIDE */

	loops4_(x, y, z__, &dbx5, &dby5, &dbz5, &pn12, &pn13, &pn14, &pn15, &pn16,
		&pn17);
	/* --------------------------------------------------------------------- */
	/*                           NOW COMPUTE THE FIELD COMPONENTS: */
	*bx = pl1 * dbx1 + pl2 * dbx2 + pl3 * dbx3 + pl4 * dbx4 + pl5 * dbx5;
	*by = pl1 * dby1 + pl2 * dby2 + pl3 * dby3 + pl4 * dby4 + pl5 * dby5;
	*bz = pl1 * dbz1 + pl2 * dbz2 + pl3 * dbz3 + pl4 * dbz4 + pl5 * dbz5;
	return 0;
} /* r2outer_ */


/* -------------------------------------------------------------------- */

/* Subroutine */ int loops4_(doublereal *x, doublereal *y, doublereal *z__, 
							 doublereal *bx, doublereal *by, doublereal *bz, doublereal *xc, 
							 doublereal *yc, doublereal *zc, doublereal *r__, doublereal *theta, 
							 doublereal *phi)
{
	/* Builtin functions */
	double cos(doublereal), sin(doublereal);

	/* Local variables */
	static doublereal cp, ct, sp, st, xs, zs, bx1, by1, bz1, bz2, bx2, by2, 
		bz3, bx3, by3, bz4, bx4, by4, bxs, bys, xss, yss, zss, bxss, bzss;
	extern /* Subroutine */ int circle_(doublereal *, doublereal *, 
		doublereal *, doublereal *, doublereal *, doublereal *, 
		doublereal *);


	/*   RETURNS FIELD COMPONENTS FROM A SYSTEM OF 4 CURRENT LOOPS, POSITIONED */
	/*     SYMMETRICALLY WITH RESPECT TO NOON-MIDNIGHT MERIDIAN AND EQUATORIAL */
	/*      PLANES. */
	/*  INPUT: X,Y,Z OF A POINT OF SPACE */
	/*        XC,YC,ZC (YC > 0 AND ZC > 0) - POSITION OF THE CENTER OF THE */
	/*                                         1ST-QUADRANT LOOP */
	/*        R - LOOP RADIUS (THE SAME FOR ALL FOUR) */
	/*        THETA, PHI  -  SPECIFY THE ORIENTATION OF THE NORMAL OF THE 1ST LOOP */
	/*      ----------------------------------------------------------- */

	ct = cos(*theta);
	st = sin(*theta);
	cp = cos(*phi);
	sp = sin(*phi);
	/* ------------------------------------1ST QUADRANT: */
	xs = (*x - *xc) * cp + (*y - *yc) * sp;
	yss = (*y - *yc) * cp - (*x - *xc) * sp;
	zs = *z__ - *zc;
	xss = xs * ct - zs * st;
	zss = zs * ct + xs * st;
	circle_(&xss, &yss, &zss, r__, &bxss, &bys, &bzss);
	bxs = bxss * ct + bzss * st;
	bz1 = bzss * ct - bxss * st;
	bx1 = bxs * cp - bys * sp;
	by1 = bxs * sp + bys * cp;
	/* -------------------------------------2nd QUADRANT: */
	xs = (*x - *xc) * cp - (*y + *yc) * sp;
	yss = (*y + *yc) * cp + (*x - *xc) * sp;
	zs = *z__ - *zc;
	xss = xs * ct - zs * st;
	zss = zs * ct + xs * st;
	circle_(&xss, &yss, &zss, r__, &bxss, &bys, &bzss);
	bxs = bxss * ct + bzss * st;
	bz2 = bzss * ct - bxss * st;
	bx2 = bxs * cp + bys * sp;
	by2 = -bxs * sp + bys * cp;
	/* -------------------------------------3RD QUADRANT: */
	xs = -(*x - *xc) * cp + (*y + *yc) * sp;
	yss = -(*y + *yc) * cp - (*x - *xc) * sp;
	zs = *z__ + *zc;
	xss = xs * ct - zs * st;
	zss = zs * ct + xs * st;
	circle_(&xss, &yss, &zss, r__, &bxss, &bys, &bzss);
	bxs = bxss * ct + bzss * st;
	bz3 = bzss * ct - bxss * st;
	bx3 = -bxs * cp - bys * sp;
	by3 = bxs * sp - bys * cp;
	/* -------------------------------------4TH QUADRANT: */
	xs = -(*x - *xc) * cp - (*y - *yc) * sp;
	yss = -(*y - *yc) * cp + (*x - *xc) * sp;
	zs = *z__ + *zc;
	xss = xs * ct - zs * st;
	zss = zs * ct + xs * st;
	circle_(&xss, &yss, &zss, r__, &bxss, &bys, &bzss);
	bxs = bxss * ct + bzss * st;
	bz4 = bzss * ct - bxss * st;
	bx4 = -bxs * cp + bys * sp;
	by4 = -bxs * sp - bys * cp;
	*bx = bx1 + bx2 + bx3 + bx4;
	*by = by1 + by2 + by3 + by4;
	*bz = bz1 + bz2 + bz3 + bz4;
	return 0;
} /* loops4_ */


/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

/* Subroutine */ int r2sheet_(doublereal *x, doublereal *y, doublereal *z__, 
							  doublereal *bx, doublereal *by, doublereal *bz)
{
	/* Initialized data */

	static doublereal c59 = 3.19466;
	static doublereal c60 = 5.89142;
	static doublereal c61 = -8.63227;
	static doublereal c62 = -2.36418;
	static doublereal c63 = -1.027;
	static doublereal c64 = -2.31515;
	static doublereal c65 = 1035.38;
	static doublereal c66 = 2040.66;
	static doublereal c67 = -131.881;
	static doublereal c68 = -744.533;
	static doublereal c69 = -3274.93;
	static doublereal c70 = -4845.61;
	static doublereal c71 = 482.438;
	static doublereal c72 = 1567.43;
	static doublereal c73 = 1354.02;
	static doublereal c74 = 2040.47;
	static doublereal c75 = -151.653;
	static doublereal c76 = -845.012;
	static doublereal c77 = -111.723;
	static doublereal c78 = -265.343;
	static doublereal c79 = -26.1171;
	static doublereal c80 = 216.632;
	static doublereal pnonx1 = -19.0969;
	static doublereal pnonx2 = -9.28828;
	static doublereal pnonx3 = -.129687;
	static doublereal pnonx4 = 5.58594;
	static doublereal pnonx5 = 22.5055;
	static doublereal pnonx6 = .048375;
	static doublereal pnonx7 = .0396953;
	static doublereal pnonx8 = .0579023;
	static doublereal pnony1 = -13.675;
	static doublereal pnony2 = -6.70625;
	static doublereal pnony3 = 2.31875;
	static doublereal pnony4 = 11.4062;
	static doublereal pnony5 = 20.4562;
	static doublereal pnony6 = .047875;
	static doublereal pnony7 = .036375;
	static doublereal pnony8 = .05675;
	static doublereal pnonz1 = -16.7125;
	static doublereal pnonz2 = -16.4625;
	static doublereal pnonz3 = -.1625;
	static doublereal pnonz4 = 5.1;
	static doublereal pnonz5 = 23.7125;
	static doublereal pnonz6 = .0355625;
	static doublereal pnonz7 = .031875;
	static doublereal pnonz8 = .053875;
	static doublereal a1 = 8.0719;
	static doublereal a2 = -7.39582;
	static doublereal a3 = -7.62341;
	static doublereal a4 = .684671;
	static doublereal a5 = -13.5672;
	static doublereal a6 = 11.6681;
	static doublereal a7 = 13.1154;
	static doublereal a8 = -.890217;
	static doublereal a9 = 7.78726;
	static doublereal a10 = -5.38346;
	static doublereal a11 = -8.08738;
	static doublereal a12 = .609385;
	static doublereal a13 = -2.7041;
	static doublereal a14 = 3.53741;
	static doublereal a15 = 3.15549;
	static doublereal a16 = -1.11069;
	static doublereal a17 = -8.47555;
	static doublereal a18 = .278122;
	static doublereal a19 = 2.73514;
	static doublereal a20 = 4.55625;
	static doublereal a21 = 13.1134;
	static doublereal a22 = 1.15848;
	static doublereal a23 = -3.52648;
	static doublereal a24 = -8.24698;
	static doublereal a25 = -6.8571;
	static doublereal a26 = -2.81369;
	static doublereal a27 = 2.03795;
	static doublereal a28 = 4.64383;
	static doublereal a29 = 2.49309;
	static doublereal a30 = -1.22041;
	static doublereal a31 = -1.67432;
	static doublereal a32 = -.422526;
	static doublereal a33 = -5.39796;
	static doublereal a34 = 7.10326;
	static doublereal a35 = 5.5373;
	static doublereal a36 = -13.1918;
	static doublereal a37 = 4.67853;
	static doublereal a38 = -7.60329;
	static doublereal a39 = -2.53066;
	static doublereal a40 = 7.76338;
	static doublereal a41 = 5.60165;
	static doublereal a42 = 5.34816;
	static doublereal a43 = -4.56441;
	static doublereal a44 = 7.05976;
	static doublereal a45 = -2.62723;
	static doublereal a46 = -.529078;
	static doublereal a47 = 1.42019;
	static doublereal a48 = -2.93919;
	static doublereal a49 = 55.6338;
	static doublereal a50 = -1.55181;
	static doublereal a51 = 39.8311;
	static doublereal a52 = -80.6561;
	static doublereal a53 = -46.9655;
	static doublereal a54 = 32.8925;
	static doublereal a55 = -6.32296;
	static doublereal a56 = 19.7841;
	static doublereal a57 = 124.731;
	static doublereal a58 = 10.4347;
	static doublereal a59 = -30.7581;
	static doublereal a60 = 102.68;
	static doublereal a61 = -47.4037;
	static doublereal a62 = -3.31278;
	static doublereal a63 = 9.37141;
	static doublereal a64 = -50.0268;
	static doublereal a65 = -533.319;
	static doublereal a66 = 110.426;
	static doublereal a67 = 1000.2;
	static doublereal a68 = -1051.4;
	static doublereal a69 = 1619.48;
	static doublereal a70 = 589.855;
	static doublereal a71 = -1462.73;
	static doublereal a72 = 1087.1;
	static doublereal a73 = -1994.73;
	static doublereal a74 = -1654.12;
	static doublereal a75 = 1263.33;
	static doublereal a76 = -260.21;
	static doublereal a77 = 1424.84;
	static doublereal a78 = 1255.71;
	static doublereal a79 = -956.733;
	static doublereal a80 = 219.946;
	static doublereal b1 = -9.08427;
	static doublereal b2 = 10.6777;
	static doublereal b3 = 10.3288;
	static doublereal b4 = -.969987;
	static doublereal b5 = 6.45257;
	static doublereal b6 = -8.42508;
	static doublereal b7 = -7.97464;
	static doublereal b8 = 1.41996;
	static doublereal b9 = -1.9249;
	static doublereal b10 = 3.93575;
	static doublereal b11 = 2.83283;
	static doublereal b12 = -1.48621;
	static doublereal b13 = .244033;
	static doublereal b14 = -.757941;
	static doublereal b15 = -.386557;
	static doublereal b16 = .344566;
	static doublereal b17 = 9.56674;
	static doublereal b18 = -2.5365;
	static doublereal b19 = -3.32916;
	static doublereal b20 = -5.86712;
	static doublereal b21 = -6.19625;
	static doublereal b22 = 1.83879;
	static doublereal b23 = 2.52772;
	static doublereal b24 = 4.34417;
	static doublereal b25 = 1.87268;
	static doublereal b26 = -2.13213;
	static doublereal b27 = -1.69134;
	static doublereal b28 = -.176379;
	static doublereal b29 = -.261359;
	static doublereal b30 = .566419;
	static doublereal b31 = .3138;
	static doublereal b32 = -.134699;
	static doublereal b33 = -3.83086;
	static doublereal b34 = -8.4154;
	static doublereal b35 = 4.77005;
	static doublereal b36 = -9.31479;
	static doublereal b37 = 37.5715;
	static doublereal b38 = 19.3992;
	static doublereal b39 = -17.9582;
	static doublereal b40 = 36.4604;
	static doublereal b41 = -14.9993;
	static doublereal b42 = -3.1442;
	static doublereal b43 = 6.17409;
	static doublereal b44 = -15.5519;
	static doublereal b45 = 2.28621;
	static doublereal b46 = -.00891549;
	static doublereal b47 = -.462912;
	static doublereal b48 = 2.47314;
	static doublereal b49 = 41.7555;
	static doublereal b50 = 208.614;
	static doublereal b51 = -45.7861;
	static doublereal b52 = -77.8687;
	static doublereal b53 = 239.357;
	static doublereal b54 = -67.9226;
	static doublereal b55 = 66.8743;
	static doublereal b56 = 238.534;
	static doublereal b57 = -112.136;
	static doublereal b58 = 16.2069;
	static doublereal b59 = -40.4706;
	static doublereal b60 = -134.328;
	static doublereal b61 = 21.56;
	static doublereal b62 = -.201725;
	static doublereal b63 = 2.21;
	static doublereal b64 = 32.5855;
	static doublereal b65 = -108.217;
	static doublereal b66 = -1005.98;
	static doublereal b67 = 585.753;
	static doublereal b68 = 323.668;
	static doublereal b69 = -817.056;
	static doublereal b70 = 235.75;
	static doublereal b71 = -560.965;
	static doublereal b72 = -576.892;
	static doublereal b73 = 684.193;
	static doublereal b74 = 85.0275;
	static doublereal b75 = 168.394;
	static doublereal b76 = 477.776;
	static doublereal b77 = -289.253;
	static doublereal b78 = -123.216;
	static doublereal b79 = 75.6501;
	static doublereal b80 = -178.605;
	static doublereal c1 = 1167.61;
	static doublereal c2 = -917.782;
	static doublereal c3 = -1253.2;
	static doublereal c4 = -274.128;
	static doublereal c5 = -1538.75;
	static doublereal c6 = 1257.62;
	static doublereal c7 = 1745.07;
	static doublereal c8 = 113.479;
	static doublereal c9 = 393.326;
	static doublereal c10 = -426.858;
	static doublereal c11 = -641.1;
	static doublereal c12 = 190.833;
	static doublereal c13 = -29.9435;
	static doublereal c14 = -1.04881;
	static doublereal c15 = 117.125;
	static doublereal c16 = -25.7663;
	static doublereal c17 = -1168.16;
	static doublereal c18 = 910.247;
	static doublereal c19 = 1239.31;
	static doublereal c20 = 289.515;
	static doublereal c21 = 1540.56;
	static doublereal c22 = -1248.29;
	static doublereal c23 = -1727.61;
	static doublereal c24 = -131.785;
	static doublereal c25 = -394.577;
	static doublereal c26 = 426.163;
	static doublereal c27 = 637.422;
	static doublereal c28 = -187.965;
	static doublereal c29 = 30.0348;
	static doublereal c30 = .221898;
	static doublereal c31 = -116.68;
	static doublereal c32 = 26.0291;
	static doublereal c33 = 12.6804;
	static doublereal c34 = 4.84091;
	static doublereal c35 = 1.18166;
	static doublereal c36 = -2.75946;
	static doublereal c37 = -17.9822;
	static doublereal c38 = -6.80357;
	static doublereal c39 = -1.47134;
	static doublereal c40 = 3.02266;
	static doublereal c41 = 4.79648;
	static doublereal c42 = .665255;
	static doublereal c43 = -.256229;
	static doublereal c44 = -.0857282;
	static doublereal c45 = -.588997;
	static doublereal c46 = .0634812;
	static doublereal c47 = .164303;
	static doublereal c48 = -.15285;
	static doublereal c49 = 22.2524;
	static doublereal c50 = -22.4376;
	static doublereal c51 = -3.85595;
	static doublereal c52 = 6.07625;
	static doublereal c53 = -105.959;
	static doublereal c54 = -41.6698;
	static doublereal c55 = .378615;
	static doublereal c56 = 1.55958;
	static doublereal c57 = 44.3981;
	static doublereal c58 = 18.8521;

	/* System generated locals */
	doublereal d__1, d__2, d__3, d__4, d__5;

	/* Builtin functions */
	double sqrt(doublereal);

	/* Local variables */
	static doublereal r__, s1, s2, s3, s4, s5, xks, t1x, t2x, t3x, t1y, t2y, 
		t3y, t1z, t2z, t3z, rho2, rho, c1p, s1p, s2p, c2p, s3p, c3p, s4p, 
		ct, st;
	extern doublereal fexp_(doublereal *, doublereal *), xksi_(doublereal *, 
		doublereal *, doublereal *), fexp1_(doublereal *, doublereal *);









	/* ------------------------------------------------------------------ */

	xks = xksi_(x, y, z__);
	/*  variation across the current sheet */
	/* Computing 2nd power */
	d__1 = xks;
	/* Computing 2nd power */
	d__2 = pnonx6;
	t1x = xks / sqrt(d__1 * d__1 + d__2 * d__2);
	/* Computing 3rd power */
	d__1 = pnonx7;
	/* Computing 2nd power */
	d__3 = xks;
	/* Computing 2nd power */
	d__4 = pnonx7;
	/* Computing 3rd power */
	d__2 = sqrt(d__3 * d__3 + d__4 * d__4);
	t2x = d__1 * (d__1 * d__1) / (d__2 * (d__2 * d__2));
	/* Computing 2nd power */
	d__2 = xks;
	/* Computing 2nd power */
	d__3 = pnonx8;
	/* Computing 5th power */
	d__1 = sqrt(d__2 * d__2 + d__3 * d__3), d__4 = d__1, d__1 *= d__1;
	/* Computing 4th power */
	d__5 = pnonx8, d__5 *= d__5;
	t3x = xks / (d__4 * (d__1 * d__1)) * 3.493856 * (d__5 * d__5);

	/* Computing 2nd power */
	d__1 = xks;
	/* Computing 2nd power */
	d__2 = pnony6;
	t1y = xks / sqrt(d__1 * d__1 + d__2 * d__2);
	/* Computing 3rd power */
	d__1 = pnony7;
	/* Computing 2nd power */
	d__3 = xks;
	/* Computing 2nd power */
	d__4 = pnony7;
	/* Computing 3rd power */
	d__2 = sqrt(d__3 * d__3 + d__4 * d__4);
	t2y = d__1 * (d__1 * d__1) / (d__2 * (d__2 * d__2));
	/* Computing 2nd power */
	d__2 = xks;
	/* Computing 2nd power */
	d__3 = pnony8;
	/* Computing 5th power */
	d__1 = sqrt(d__2 * d__2 + d__3 * d__3), d__4 = d__1, d__1 *= d__1;
	/* Computing 4th power */
	d__5 = pnony8, d__5 *= d__5;
	t3y = xks / (d__4 * (d__1 * d__1)) * 3.493856 * (d__5 * d__5);

	/* Computing 2nd power */
	d__1 = xks;
	/* Computing 2nd power */
	d__2 = pnonz6;
	t1z = xks / sqrt(d__1 * d__1 + d__2 * d__2);
	/* Computing 3rd power */
	d__1 = pnonz7;
	/* Computing 2nd power */
	d__3 = xks;
	/* Computing 2nd power */
	d__4 = pnonz7;
	/* Computing 3rd power */
	d__2 = sqrt(d__3 * d__3 + d__4 * d__4);
	t2z = d__1 * (d__1 * d__1) / (d__2 * (d__2 * d__2));
	/* Computing 2nd power */
	d__2 = xks;
	/* Computing 2nd power */
	d__3 = pnonz8;
	/* Computing 5th power */
	d__1 = sqrt(d__2 * d__2 + d__3 * d__3), d__4 = d__1, d__1 *= d__1;
	/* Computing 4th power */
	d__5 = pnonz8, d__5 *= d__5;
	t3z = xks / (d__4 * (d__1 * d__1)) * 3.493856 * (d__5 * d__5);

	rho2 = *x * *x + *y * *y;
	r__ = sqrt(rho2 + *z__ * *z__);
	rho = sqrt(rho2);

	c1p = *x / rho;
	s1p = *y / rho;
	s2p = s1p * 2. * c1p;
	c2p = c1p * c1p - s1p * s1p;
	s3p = s2p * c1p + c2p * s1p;
	c3p = c2p * c1p - s2p * s1p;
	s4p = s3p * c1p + c3p * s1p;
	ct = *z__ / r__;
	st = rho / r__;

	s1 = fexp_(&ct, &pnonx1);
	s2 = fexp_(&ct, &pnonx2);
	s3 = fexp_(&ct, &pnonx3);
	s4 = fexp_(&ct, &pnonx4);
	s5 = fexp_(&ct, &pnonx5);

	/*                   NOW COMPUTE THE GSM FIELD COMPONENTS: */


	*bx = s1 * (a1 + a2 * t1x + a3 * t2x + a4 * t3x + c1p * (a5 + a6 * t1x + 
		a7 * t2x + a8 * t3x) + c2p * (a9 + a10 * t1x + a11 * t2x + a12 * 
		t3x) + c3p * (a13 + a14 * t1x + a15 * t2x + a16 * t3x)) + s2 * (
		a17 + a18 * t1x + a19 * t2x + a20 * t3x + c1p * (a21 + a22 * t1x 
		+ a23 * t2x + a24 * t3x) + c2p * (a25 + a26 * t1x + a27 * t2x + 
		a28 * t3x) + c3p * (a29 + a30 * t1x + a31 * t2x + a32 * t3x)) + 
		s3 * (a33 + a34 * t1x + a35 * t2x + a36 * t3x + c1p * (a37 + a38 *
		t1x + a39 * t2x + a40 * t3x) + c2p * (a41 + a42 * t1x + a43 * 
		t2x + a44 * t3x) + c3p * (a45 + a46 * t1x + a47 * t2x + a48 * t3x)
		) + s4 * (a49 + a50 * t1x + a51 * t2x + a52 * t3x + c1p * (a53 + 
		a54 * t1x + a55 * t2x + a56 * t3x) + c2p * (a57 + a58 * t1x + a59 
		* t2x + a60 * t3x) + c3p * (a61 + a62 * t1x + a63 * t2x + a64 * 
		t3x)) + s5 * (a65 + a66 * t1x + a67 * t2x + a68 * t3x + c1p * (
		a69 + a70 * t1x + a71 * t2x + a72 * t3x) + c2p * (a73 + a74 * t1x 
		+ a75 * t2x + a76 * t3x) + c3p * (a77 + a78 * t1x + a79 * t2x + 
		a80 * t3x));


	s1 = fexp_(&ct, &pnony1);
	s2 = fexp_(&ct, &pnony2);
	s3 = fexp_(&ct, &pnony3);
	s4 = fexp_(&ct, &pnony4);
	s5 = fexp_(&ct, &pnony5);

	*by = s1 * (s1p * (b1 + b2 * t1y + b3 * t2y + b4 * t3y) + s2p * (b5 + b6 *
		t1y + b7 * t2y + b8 * t3y) + s3p * (b9 + b10 * t1y + b11 * t2y + 
		b12 * t3y) + s4p * (b13 + b14 * t1y + b15 * t2y + b16 * t3y)) + 
		s2 * (s1p * (b17 + b18 * t1y + b19 * t2y + b20 * t3y) + s2p * (
		b21 + b22 * t1y + b23 * t2y + b24 * t3y) + s3p * (b25 + b26 * t1y 
		+ b27 * t2y + b28 * t3y) + s4p * (b29 + b30 * t1y + b31 * t2y + 
		b32 * t3y)) + s3 * (s1p * (b33 + b34 * t1y + b35 * t2y + b36 * 
		t3y) + s2p * (b37 + b38 * t1y + b39 * t2y + b40 * t3y) + s3p * (
		b41 + b42 * t1y + b43 * t2y + b44 * t3y) + s4p * (b45 + b46 * t1y 
		+ b47 * t2y + b48 * t3y)) + s4 * (s1p * (b49 + b50 * t1y + b51 * 
		t2y + b52 * t3y) + s2p * (b53 + b54 * t1y + b55 * t2y + b56 * t3y)
		+ s3p * (b57 + b58 * t1y + b59 * t2y + b60 * t3y) + s4p * (b61 + 
		b62 * t1y + b63 * t2y + b64 * t3y)) + s5 * (s1p * (b65 + b66 * 
		t1y + b67 * t2y + b68 * t3y) + s2p * (b69 + b70 * t1y + b71 * t2y 
		+ b72 * t3y) + s3p * (b73 + b74 * t1y + b75 * t2y + b76 * t3y) + 
		s4p * (b77 + b78 * t1y + b79 * t2y + b80 * t3y));

	s1 = fexp1_(&ct, &pnonz1);
	s2 = fexp1_(&ct, &pnonz2);
	s3 = fexp1_(&ct, &pnonz3);
	s4 = fexp1_(&ct, &pnonz4);
	s5 = fexp1_(&ct, &pnonz5);

	*bz = s1 * (c1 + c2 * t1z + c3 * t2z + c4 * t3z + c1p * (c5 + c6 * t1z + 
		c7 * t2z + c8 * t3z) + c2p * (c9 + c10 * t1z + c11 * t2z + c12 * 
		t3z) + c3p * (c13 + c14 * t1z + c15 * t2z + c16 * t3z)) + s2 * (
		c17 + c18 * t1z + c19 * t2z + c20 * t3z + c1p * (c21 + c22 * t1z 
		+ c23 * t2z + c24 * t3z) + c2p * (c25 + c26 * t1z + c27 * t2z + 
		c28 * t3z) + c3p * (c29 + c30 * t1z + c31 * t2z + c32 * t3z)) + 
		s3 * (c33 + c34 * t1z + c35 * t2z + c36 * t3z + c1p * (c37 + c38 *
		t1z + c39 * t2z + c40 * t3z) + c2p * (c41 + c42 * t1z + c43 * 
		t2z + c44 * t3z) + c3p * (c45 + c46 * t1z + c47 * t2z + c48 * t3z)
		) + s4 * (c49 + c50 * t1z + c51 * t2z + c52 * t3z + c1p * (c53 + 
		c54 * t1z + c55 * t2z + c56 * t3z) + c2p * (c57 + c58 * t1z + c59 
		* t2z + c60 * t3z) + c3p * (c61 + c62 * t1z + c63 * t2z + c64 * 
		t3z)) + s5 * (c65 + c66 * t1z + c67 * t2z + c68 * t3z + c1p * (
		c69 + c70 * t1z + c71 * t2z + c72 * t3z) + c2p * (c73 + c74 * t1z 
		+ c75 * t2z + c76 * t3z) + c3p * (c77 + c78 * t1z + c79 * t2z + 
		c80 * t3z));

	return 0;
} /* r2sheet_ */


/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
doublereal xksi_(doublereal *x, doublereal *y, doublereal *z__)
{
	/* Initialized data */

	static doublereal a11a12 = .305662;
	static doublereal a21a22 = -.383593;
	static doublereal a41a42 = .2677733;
	static doublereal a51a52 = -.097656;
	static doublereal a61a62 = -.636034;
	static doublereal b11b12 = -.359862;
	static doublereal b21b22 = .424706;
	static doublereal c61c62 = -.126366;
	static doublereal c71c72 = .292578;
	static doublereal r0 = 1.21563;
	static doublereal dr = 7.50937;
	static doublereal tnoon = .3665191;
	static doublereal dteta = .09599309;

	/* System generated locals */
	doublereal ret_val, d__1, d__2;

	/* Builtin functions */
	double sqrt(doublereal), sin(doublereal);

	/* Local variables */
	static doublereal f, g, h__, r__, g2, r2, r3, r4, x2, y2, z2, pr, xr, yr, 
		zr, xy, dr2, fgh, phi, xyz, fgh32, alpha, theta, fchsg2, sqfchsg2;



	/*   A11 - C72, R0, and DR below  ARE STRETCH PARAMETERS (P.26-27, NB# 3), */

	/*                                         latitudes 69 and 63.5 degs, resp. */
	/* Correspond to noon and mi */
	dr2 = dr * dr;

	x2 = *x * *x;
	y2 = *y * *y;
	z2 = *z__ * *z__;
	xy = *x * *y;
	xyz = xy * *z__;
	r2 = x2 + y2 + z2;
	r__ = sqrt(r2);
	r3 = r2 * r__;
	r4 = r2 * r2;
	xr = *x / r__;
	yr = *y / r__;
	zr = *z__ / r__;

	if (r__ < r0) {
		pr = 0.;
	} else {
		/* Computing 2nd power */
		d__1 = r__ - r0;
		pr = sqrt(d__1 * d__1 + dr2) - dr;
	}

	f = *x + pr * (a11a12 + a21a22 * xr + a41a42 * xr * xr + a51a52 * yr * yr 
		+ a61a62 * zr * zr);
	g = *y + pr * (b11b12 * yr + b21b22 * xr * yr);
	h__ = *z__ + pr * (c61c62 * zr + c71c72 * xr * zr);
	g2 = g * g;

	/* Computing 2nd power */
	d__1 = f;
	/* Computing 2nd power */
	d__2 = h__;
	fgh = d__1 * d__1 + g2 + d__2 * d__2;
	/* Computing 3rd power */
	d__1 = sqrt(fgh);
	fgh32 = d__1 * (d__1 * d__1);
	/* Computing 2nd power */
	d__1 = f;
	fchsg2 = d__1 * d__1 + g2;
	if (fchsg2 < 1e-5) {
		ret_val = -1.;
		/*  THIS IS JUST FOR ELIMINATING PROBLE */
		return ret_val;
		/*  ON THE Z-AXIS */
	}
	sqfchsg2 = sqrt(fchsg2);
	alpha = fchsg2 / fgh32;
	theta = tnoon + dteta * .5 * (1. - f / sqfchsg2);
	/* Computing 2nd power */
	d__1 = sin(theta);
	phi = d__1 * d__1;

	ret_val = alpha - phi;

	return ret_val;
} /* xksi_ */


/* -------------------------------------------------------------------- */

doublereal fexp_(doublereal *s, doublereal *a)
{
	/* Initialized data */

	static doublereal e = 2.718281828459;

	/* System generated locals */
	doublereal ret_val;

	/* Builtin functions */
	double sqrt(doublereal), exp(doublereal);

	if (*a < 0.) {
		ret_val = sqrt(-2. * *a * e) * *s * exp(*a * *s * *s);
	}
	if (*a >= 0.) {
		ret_val = *s * exp(*a * (*s * *s - 1.));
	}
	return ret_val;
} /* fexp_ */


/* ----------------------------------------------------------------------- */
doublereal fexp1_(doublereal *s, doublereal *a)
{
	/* System generated locals */
	doublereal ret_val;

	/* Builtin functions */
	double exp(doublereal);

	if (*a <= 0.) {
		ret_val = exp(*a * *s * *s);
	}
	if (*a > 0.) {
		ret_val = exp(*a * (*s * *s - 1.));
	}
	return ret_val;
} /* fexp1_ */


/* ************************************************************************ */

doublereal tksi_(doublereal *xksi, doublereal *xks0, doublereal *dxksi)
{
	/* Initialized data */

	static integer m = 0;

	/* System generated locals */
	doublereal ret_val, d__1;

	/* Local variables */
	static doublereal br3, tdz3, tksii;


	if (m == 0) {
		/* Computing 3rd power */
		d__1 = *dxksi;
		tdz3 = d__1 * (d__1 * d__1) * 2.f;
		m = 1;
	}

	if (*xksi - *xks0 < -(*dxksi)) {
		tksii = 0.f;
	}
	if (*xksi - *xks0 >= *dxksi) {
		tksii = 1.f;
	}

	if (*xksi >= *xks0 - *dxksi && *xksi < *xks0) {
		/* Computing 3rd power */
		d__1 = *xksi - *xks0 + *dxksi;
		br3 = d__1 * (d__1 * d__1);
		tksii = br3 * 1.5f / (tdz3 + br3);
	}

	if (*xksi >= *xks0 && *xksi < *xks0 + *dxksi) {
		/* Computing 3rd power */
		d__1 = *xksi - *xks0 - *dxksi;
		br3 = d__1 * (d__1 * d__1);
		tksii = br3 * 1.5f / (tdz3 - br3) + 1.f;
	}
	ret_val = tksii;
	return ret_val;
} /* tksi_ */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Subroutine */ int dipole_(real *ps, real *x, real *y, real *z__, real *bx, 
							 real *by, real *bz)
{
	/* Initialized data */

	static integer m = 0;
	static real psi = 5.f;

	/* System generated locals */
	real r__1, r__2;

	/* Builtin functions */
	double sin(doublereal), cos(doublereal), sqrt(doublereal);

	/* Local variables */
	static real p, q, t, u, v, cps, sps;


	/*  CALCULATES GSM COMPONENTS OF GEODIPOLE FIELD WITH THE DIPOLE MOMENT */
	/*  CORRESPONDING TO THE EPOCH OF 1980. */
	/* ------------INPUT PARAMETERS: */
	/*   PS - GEODIPOLE TILT ANGLE IN RADIANS, X,Y,Z - GSM COORDINATES IN RE */
	/* ------------OUTPUT PARAMETERS: */
	/*   BX,BY,BZ - FIELD COMPONENTS IN GSM SYSTEM, IN NANOTESLA. */


	/*                   AUTHOR: NIKOLAI A. TSYGANENKO */
	/*                           INSTITUTE OF PHYSICS */
	/*                           ST.-PETERSBURG STATE UNIVERSITY */
	/*                           STARY PETERGOF 198904 */
	/*                           ST.-PETERSBURG */
	/*                           RUSSIA */


	if (m == 1 && (r__1 = *ps - psi, dabs(r__1)) < 1e-5f) {
		goto L1;
	}
	sps = sin(*ps);
	cps = cos(*ps);
	psi = *ps;
	m = 1;
L1:
	/* Computing 2nd power */
	r__1 = *x;
	p = r__1 * r__1;
	/* Computing 2nd power */
	r__1 = *z__;
	u = r__1 * r__1;
	v = *z__ * 3.f * *x;
	/* Computing 2nd power */
	r__1 = *y;
	t = r__1 * r__1;
	/* Computing 5th power */
	r__1 = sqrt(p + t + u), r__2 = r__1, r__1 *= r__1;
	q = 30574.f / (r__2 * (r__1 * r__1));
	*bx = q * ((t + u - p * 2.f) * sps - v * cps);
	*by = *y * -3.f * q * (*x * sps + *z__ * cps);
	*bz = q * ((p + t - u * 2.f) * cps - v * sps);
	return 0;
} /* dipole_ */

