/*
**	<potm.c> --	Print out the phase of the moon ...
**
**	programmer:	John Dilley	(mordred:jad)
**
**	creation date:	Sat Feb  9 14:27
**
**
**	modification history
**
*/

#include <stdio.h>

static	char	potm[64];

main()
{
    moontxt(potm);
    printf("Phase-of-the-Moon:%s\n", potm+11);
}
