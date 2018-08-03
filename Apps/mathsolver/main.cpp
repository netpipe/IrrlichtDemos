//This can be useful for scripting...
//The classes only works on float (f32) values. (no vectors, no double, no matrices, sorry...)

//Author tom_gamer


//`3.4*PI/sin(4.6+min(4,2)^1.7)`

// may have broke the rand * p1 function - tecan


#include "SCalcExpr.h"


main(){

SCalcExpr *calc = new SCalcExpr();

// try to parse the string
if (!calc->readExpr("min(clamp(PI*2.5*round(3.9)/PI,1,2), 3)"))
{
        // readExpr() will show a detailed error message
        printf("Invalid expression!\n");
        return(0);
}

f32 f;
// Calculate the result
if (!calc->calcExpr(NULL, f))
{
        // calcExpr() will show a detailed error message
        printf("can't calculate expression\n");
        return(0);
}

// we don't need this object anymore
delete calc;

printf("Result = %0.3f\n", f);
return(0);
}
