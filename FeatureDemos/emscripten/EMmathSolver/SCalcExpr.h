#ifndef SCALCEXPR_H
#define SCALCEXPR_H

#include <irrlicht.h>
using namespace irr;
using namespace core;

//! Class to get an access to your variables
/*!
To calculate with variables, you have to provide an access to them. This is done with a derived object of this (abstract) class.

You have to implement this functionality:
\li \p VarHandler::VarLink		- this is a link to a particular variable
\li \p VarHandler::decodeVar()		- function, that takes the variable name as programmed in the expression and returns the link to the variable as type VarHandler::VarLink
\li \p VarHandler::getGameVar()		- function, that takes the link to the variable (type VarHandler::VarLink) and returns the (float) value of the variable

While reading and parsing the expression, a variable may be found. Note, that in my case, all variables start with `$`, `#` or `*`. You'll have to modify \ref SCalcExpr::tryToReadOperand ,
when you want to change this. For a found variable, the function `decodeVar()` is called to get the link to the variable.
\code
	VarHandler::VarLink *VarHandler::decodeVar(stringc varName);
\endcode
This function should return NULL, when the string isn't a known variable and no link could be created. When there was a link returned, it's saved into the post fix expression.

When calculating the expression, for each variable link in the post fix stack, the static function `getGameVar()` is called, to get the variables value.
\code
	bool VarHandler::getGameVar(VarHandler::VarLink &link, void *local, f32 &res);
\endcode
The `local` parameter is the same pointer, that is programmed in the call of \ref SCalcExpr::calcExpr and \ref SCalcExpr::calcExpression. You can give the expression resolver some access to local
variables.

\note When the variable handler creates objects of type VarLink with `new`, it should delete them, when the expression isn't calculated anymore or you get a memory leak.

*/
class VarHandler
{
    public:
        //! Class to get an access to your variables
        /*!
            This class represents a link to a particular variable. A pointer to such an object is saved in the post fix expression.

            When the expression is calculated, this link is used to get the value of the variable.

            You should derive an own class of VarLink, that stores all relevant data.
        */
        class VarLink
        {
            public:
                //! Returns the name of the variable as programmed in the expression (for error messages)
                virtual stringc getName()=0;
        };

        //! Returns the link to the variable (or NULL)
        /*!
            \param var	name of the variable access as programmed in the expression
            \return pointer to the link to the variable, or NULL, when an error occurred
        */
        virtual VarLink *decodeVar(stringc var) = 0;
        //! Get's the value of a variable
        /*!
            \param link	pointer to the link to the variable
            \param local    pointer to some local variables (same as given in \ref SCalcExpr::calcExpr and \ref SCalcExpr::calcExpression)
            \param res  Output: the value of the variable
            \return true, when no error occurred and the variable's value is put into res
        */
        virtual bool getGameVar(VarLink *link, void *local, f32 &res) = 0;
};

//! Class for reading, parsing and calculating an expression
/*!
With this class, you can evaluate an mathematical expression like `3.4*PI/sin(4.6+min(4,2)^1.7)`. This can be useful for scripting...

The class only works on float (f32) values. (no vectors, no double, no matrices)

The expression
--------------

Possible operators for the expression are: + - * / ^

Possible operands are:
\li (float) numbers (no EXP-expressions)
\li variables (see \ref VarHandler "here")
\li mathematical functions

Example: 3.4*PI/sin(4.6+min($MyVar,2)^1.7)

Here is `$MyVar` a variable and `PI`, `sin` and `min` are functions.

Using this class
----------------

First, you have to read and parse the expression. Use the function \ref readExpr or \ref readExpression for this.
The expression is converted to post fix notation (which is much easier to calculate) and save in intern structures.
You should do this in a loading process, when time isn't critical.

After that, you can calculate the expression with functions \ref calcExpr and \ref calcExpression.
The focus is set on fast calculation (no string handling here). You can calculate a read expression multiple times. (This makes sense, when you have
variables in the expression, that may be changed.)

There may be errors while parsing (invalid expression) or while calculating (arithmetic error). You can get an error message with \ref getLastError and
the error position with \ref getLastErrorPos.

Example 1:

\code{.cpp}
// Create a new object
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
\endcode


Example 2:

\code{.cpp}
// The expression
stringc expr("3.4*PI/sin(4.6+min($MyVar,2)^1.7)");

// Create a new object. Note, that varHandler (object of type VarHandler) have to provide access to your variables.
SCalcExpr *calc = new SCalcExpr(varHandler);

// try to parse the string
if (!calc->readExpression(expr))
{
	stringc part = expr.subString(0,calc->getLastErrorPos());
	printf("Error while parsing expression!\n%s\nError occurred at '%s'.\nOriginal expression was '%s'.\n", calc->getLastError.c_str(), part.c_str(), expr.c_str());
	return(0);
}

f32 f;
// Calculate the result. localVars is a pointer to some local variables. The variable handler gets this pointer.
if (!calc->calcExpression(localVars, f))
{
	stringc part = orgExpr.subString(0,calc->getLastErrorPos());
	console->errorLog("Error while calculating expression!\n%s\nError occurred at %s.\nOriginal expression was %s.\n", calc->getLastError.c_str(), part.c_str(), orgExpr.c_str());
	return(0);
}

// we don't need this object anymore
delete calc;

printf("Result = %0.3f\n", f);
return(0);
\endcode

Extend this class
-----------------

Of course you can also extend this class. See \ref buildInFunctions "here" how to add new mathematical functions. See \ref calculate "here" how to add new operations.

*/
class SCalcExpr
{
	public:
		//! Constructor, very simple
        /*! \param varHandl	access to the global variable handler (can be NULL, when you don't have variables)
            See \ref VarHandler how to write your own variable handler.
        */
		SCalcExpr(VarHandler *varHandl=NULL) : varHandler(varHandl), valid(false) {};
		static stringc getExpr(const char **str);
		bool readExpr(stringc expr);
		bool readExpression(stringc expr);
		bool calcExpr(void *local, f32 &result);
		bool calcExpression(void *local, f32 &result);

		void printPostFixExpr();
		//! returns the last error message
		stringc getLastError() {return(errorStr);};
		//! returns the position in the expression, where the last error occurred
		u32 getLastErrorPos() {return(errorPos);};
	protected:
	private:
		//! Enumeration of items, that can be found in an expression
		/*! All items, that may be found in the expression are listed here. Some types only occur while reading the expression.
		*/
		enum types
		{
			Operator,		//!< Operator in the expression, character is in .op
			Variable,		//!< Variable in the expression, variable identifier is in .var
			Number,			//!< Number in the expression, the value is in .nr
			Negation,		//!< Negation in the expression, just a flag
			Bracket,		//!< Opening or closing bracket in the expression, only for reading
			Comma,			//!< Comma in the expression (separator of function parameters), only for reading
			Function		//!< Function in the expression, the id of this function is in .id
		};

		//! Enumeration of all function, that are build in
		/*! You may extend this enumeration with own functions. See \ref buildInFunctions for details.
		*/
		enum fct_types
		{
			Fct_Abs = 0,
			Fct_Max,
			Fct_Min,
			Fct_Sinus,
			Fct_Cosinus,
			Fct_Tangens,
			Fct_Random,
			Fct_SqRoot,
			Fct_Round,
			Fct_Clamp,
			Fct_Pi,
			NumberOfFct_C
		};

		//! Type of data of a single expression item
		/*! Variables of this type are used for reading, parsing and calculating the expression.
		*/
		struct elm_t
		{
			types type;				//!< Type of the item, possible \ref types
			char op;				//!< Operator (as character) (when type==operator)
			f32 nr;					//!< Value of a number (when type==number)
			fct_types id;			//!< the id of a function (when type==function)
			VarHandler::VarLink *var;			//!< the link to a variable, you have to change this! (when type==variable)
			u32 orgPos;				//!< the position in the expression, where this item can be found (for error diagnosis)
            //! Simple constructor with initialization
			elm_t() : type(Operator), op(' '), nr(0.0f), id(Fct_Sinus), var(NULL), orgPos(0) {};
            //! Constructor with initialization of a type and it's position
			elm_t(types t, u32 pos) : type(t), op(' '), nr(0.0f), id(Fct_Sinus), var(NULL), orgPos(pos) {};
            //! Constructor with initialization of a type (operator), the operator and it's position
			elm_t(types t, char o, u32 pos) : type(t), op(o), nr(0.0f), id(Fct_Sinus), var(NULL), orgPos(pos) {};
		};

		//! Type of data for reading the expression
		/*! When reading and parsing the expression, it is converted to post fix notation. So, it can be calculated later much easier.
			For this process (especially resolving brackets), we need a stack.
		*/
		struct stackElm_t
		{
			char op;				//!< Operator (as character) when isFct==false or Id of function when isFct==true
			u32 pos;				//!< the position in the expression, where this item can be found (for error diagnosis)
			bool isFct;				//!< is this item a function?
            //! Simple constructor with initialization
			stackElm_t() : op(' '), pos(0), isFct(false) {};
            //! Constructor with initialization of an operator
			stackElm_t(char Op, u32 Pos, bool IsFct=false) : op(Op), pos(Pos), isFct(IsFct) {};
		};

		//! Structure for the data of one build in function
		/*! Some functions are build in and can occur in the expression. Functions may take parameters. There must be an
			assignment of 'function name - function id - number of parameters'. This assignment is defined in the array \ref buildInFunctions.
		*/
		struct fct_data_t
		{
			stringc name;		//!< name of built in function as programmed in the expression
			fct_types id;		//!< id of built in function (saved in the post fix expression)
			u32 npar;		    //!< number of parameters that are required for this function
		};

		//! Structure of internal data for reading the expression
		/*! When reading and parsing the expression, some data have to be shared in several functions. This data is put into a
			variable of this type.
		*/
		struct readData_t
		{
			bool wantOperand;				//!< do we expect an operand now? (or an operation?)
			bool negOperand;				//!< have the next operand to be negated?
			u32 negPos;						//!< when the next operand have to be negated, the position of the '-' is saved here
			array<u32> parToRead;			//!< this is a stack of the expected number of parameters of functions in the expression
            //! Simple constructor with initialization
			readData_t() {init();};
            //! Initialization of local data
			void init() {wantOperand=true; negOperand=false; negPos=0; parToRead.clear();};
		};

        VarHandler *varHandler;    //!< saved access to the global variable handler
		bool valid;				//!< was the expression successful parsed? (and can now be calculated?)
		stringc orgExpr;		//!< here we save the original expression (for diagnosis)
		array<elm_t> postfixExpr;					//!< the expression in post fix notation
		static fct_data_t buildInFunctions[];		//!< Declaration of all build in functions
		stringc errorStr;		//!< error message of last occurred error
		u32 errorPos;			//!< position in the expression, where the last error occurred
		readData_t data;		//!< Structure of internal data for reading the expression
		array<stackElm_t> stck;	//!< Stack for parsing the expression
		array<f32> fstck;		//!< Stack for calculating the expression

		bool topStack(stackElm_t &res);
		bool popStack(stackElm_t &res);
		void pushStack(stackElm_t s);
		void skipSpaces(stringc &s, u32 &pos);
		s32 getPrecedence(char op1, char op2);

		bool readOperand(stringc &expr, u32 &pos);
		bool readOperator(stringc &expr, u32 &pos);
		bool tryToReadOperand(stringc &expr, u32 &pos, elm_t &op);
		bool tryToReadOperator(stringc &expr, u32 &pos, elm_t &op);
		void putInOperator(elm_t op);

		bool resolveFunction(fct_types id, f32 &res);
		bool getParameter(f32 &p, fct_types id);
		bool calculate(f32 a, char op, f32 b, f32 &res);

		static stringc getFunctionName(fct_types id);
		static bool equalsMultiple(f32 toTest, f32 value);

        static bool isDigit(const char ch);
        static bool isLetter(const char ch);
		static void skipSpaces(const char **p);
        static void getFloatString(f32 f, char *buf, u32 bufSize, char prec);
        static stringc getFloatString(f32 f, s32 prec=1);
	};


/*! \mainpage Classes to calculate a mathematical expression
With these classes, you can evaluate an mathematical expression like `3.4*PI/sin(4.6+min(4,2)^1.7)`. This can be useful for scripting...

The classes only works on float (f32) values. (no vectors, no double, no matrices, sorry...)

\section sec_expression The expression

Possible operators for the expression are: + - * / ^

Possible operands are:
\li (float) numbers (no EXP-expressions)
\li variables (see \ref VarHandler "here")
\li mathematical functions

Example: 3.4*PI/sin(4.6+min($MyVar,2)^1.7)

Here is `$MyVar` a variable and `PI`, `sin` and `min` are functions.

\section sec_usage Using this class

First of all you have to create an object of class \ref SCalcExpr . When you have variables in your expressions, you have to provide
an access to them. This access is implemented by an object of class \ref VarHandler . See there for details. Of course you can also create
an object class \ref SCalcExpr without a variable handler, but then you can't have variables in your expressions.

After that, you have to read and parse the expression. Use the function \ref SCalcExpr::readExpr or \ref SCalcExpr::readExpression for this.
The expression is converted to post fix notation (which is much easier to calculate) and saved in intern structures.
You should do this in a loading process, when time isn't critical.

Finally, you can calculate the expression with functions \ref SCalcExpr::calcExpr and \ref SCalcExpr::calcExpression.
The focus is set on fast calculation (no string handling here). You can calculate a read expression multiple times. (This makes sense, when you have
variables in the expression, that may be changed.)

There may be errors while parsing (invalid expression) or while calculating (arithmetic error). You can get an error message with \ref SCalcExpr::getLastError and
the error position with \ref SCalcExpr::getLastErrorPos.

Example 1:

\code
// Create a new object
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
\endcode


Example 2:

\code
// The expression
stringc expr("3.4*PI/sin(4.6+min($MyVar,2)^1.7)");

// Create a new object. Note, that varHandler (object of type VarHandler) have to provide access to your variables.
SCalcExpr *calc = new SCalcExpr(varHandler);

// try to parse the string
if (!calc->readExpression(expr))
{
	stringc part = expr.subString(0,calc->getLastErrorPos());
	printf("Error while parsing expression!\n%s\nError occurred at '%s'.\nOriginal expression was '%s'.\n", calc->getLastError.c_str(), part.c_str(), expr.c_str());
	return(0);
}

f32 f;
// Calculate the result. localVars is a pointer to some local variables. The variable handler gets this pointer.
if (!calc->calcExpression(localVars, f))
{
	stringc part = orgExpr.subString(0,calc->getLastErrorPos());
	console->errorLog("Error while calculating expression!\n%s\nError occurred at %s.\nOriginal expression was %s.\n", calc->getLastError.c_str(), part.c_str(), orgExpr.c_str());
	return(0);
}

// we don't need this object anymore
delete calc;

printf("Result = %0.3f\n", f);
return(0);
\endcode

\section sec_extend Extend this class

Of course you can also extend this class. See \ref SCalcExpr::buildInFunctions "here" how to add new mathematical functions. See \ref SCalcExpr::calculate "here" how to add new operations.

*/

#endif // SCALCEXPR_H