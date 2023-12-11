#include "SCalcExpr.h"
#include <assert.h>
//! Declaration of all build in functions
/*! You may extend this array with further functions.

	\li \p name	this is the string that is in the expression
        \li \p id		this is the id of the function (that now string compare is required, when the expression is calculated)
	\li \p npar	set here the number of parameters the are required by the function

	The id has to be added to the enumeration \ref fct_types.

	You have to add a case in the function \ref resolveFunction.
*/
SCalcExpr::fct_data_t SCalcExpr::buildInFunctions[SCalcExpr::NumberOfFct_C] = {
		{.name="abs", .id=Fct_Abs, .npar=1},
		{.name="max", .id=Fct_Max, .npar=2},
		{.name="min", .id=Fct_Min, .npar=2},
		{.name="sin", .id=Fct_Sinus, .npar=1},
		{.name="cos", .id=Fct_Cosinus, .npar=1},
		{.name="tan", .id=Fct_Tangens, .npar=1},
		{.name="rand", .id=Fct_Random, .npar=1},
		{.name="sqrt", .id=Fct_SqRoot, .npar=1},
		{.name="round", .id=Fct_Round, .npar=1},
		{.name="clamp", .id=Fct_Clamp, .npar=3},
		{.name="PI", .id=Fct_Pi, .npar=0}
	};

//! Skips spaces in the expression string
/*! Empty characters and control characters are skipped.

	\param s	The expression string.
	\param pos	The current parsing position. This index is changed.
*/
void SCalcExpr::skipSpaces(stringc &s, u32 &pos)
{
	while ((s[pos] > '\0') && (s[pos] <= ' '))
		pos++;
}

//! Reads an expression from a string
/*! This static function can be used to extract an (bracketed) expression from a string. The given pointer is moved to the end of the expression.

	Actually, this function simply counts the opening and closing brackets.

	\param str	Input: pointer of the first opening bracket, Output: points on the last closing bracket
	\return 	The string of the expression.
*/
stringc SCalcExpr::getExpr(const char **str)
{
	skipSpaces(str);

	const char *start = *str;
	u32 brCnt = 0;
	while (**str)
	{
		if (**str == '(')
			brCnt++;
		else
		if (**str == ')')
		{
			if (brCnt == 0)
				break;
			brCnt--;
		}
		(*str)++;
	}
	if (start == *str)
		return("");
	return(stringc(start, *str-start));
}

//! Reads and parses an expression from a string with error message
/*! This function reads an expression, parses it and saves it to an internal representation. A successful parsed expression can then be calculated with \ref calcExpr or \ref calcExpression.

	When an error occurred, this function creates an error messages and prints it to the console.

	\param expr	the expression to be parsed
	\return 	true, when the expression could be parsed (and can now be calculated)
*/
bool SCalcExpr::readExpr(stringc expr)
{
	if (!readExpression(expr))
	{
		stringc part = orgExpr.subString(0,errorPos);
		printf("Error while parsing expression!\n%s\nError occurred at '%s'.\nOriginal expression was '%s'.\n",
							errorStr.c_str(), part.c_str(), orgExpr.c_str());
		return(false);
	}
	return(true);
}

//! Reads and parses an expression from a string
/*! This function reads an expression, parses it and saves it to an internal representation (post fix notation). A successful parsed expression can then be
    calculated with \ref calcExpr or \ref calcExpression.

	When an error occurred, this function saves the error position in \ref errorPos and the error message in \ref errorStr. You can get these values with
	\ref getLastError and \ref getLastErrorPos.

	\param expr	the expression to be parsed
	\return 	true, when the expression could be parsed (and can now be calculated)
*/
bool SCalcExpr::readExpression(stringc expr)
{
	orgExpr = expr;
	postfixExpr.clear();
	stck.clear();

	u32 pos = 0;
	u32 len = expr.size();
	data.init();
	while (pos < len)
	{
		if (data.wantOperand)
		{
			if (!readOperand(expr, pos))
				return(false);
		}
		else
		{
			if (!readOperator(expr, pos))
				return(false);
		}
		skipSpaces(expr, pos);
	}

	if (data.wantOperand)
	{
		if (!data.parToRead.empty())
			errorStr = "Function parameter expected, but expression ended.";
		else
			errorStr = "Expression ended with operator.";
		errorPos = 1000;
		return(false);
	}

	while (!stck.empty())
	{
		stackElm_t e;
		popStack(e);
		postfixExpr.push_back(elm_t(Operator, e.op, e.pos));
	}

	valid = true;
	return(true);
}

//! Reads a single operand
/*! This function reads an operand.

	When an error occurred, this function saves the error position in \ref errorPos and the error message in \ref errorStr and returns false.

	Some internal data is put into the private variable \ref data.

	\param expr	the expression to be parsed
	\param pos	The current parsing position. This index is changed.
	\return 	true, when an operand could be read
*/
bool SCalcExpr::readOperand(stringc &expr, u32 &pos)
{
	elm_t op;

	if (!tryToReadOperand(expr, pos, op))
	{
		errorStr = "Invalid operand.";
		errorPos = pos;
		return(false);
	}
	switch (op.type)
	{
		case Bracket:
			pushStack(stackElm_t('(', pos));
			break;
		case Negation:
			if (!data.negOperand)
				data.negPos = pos;
			data.negOperand = !data.negOperand;
			break;
		case Function:
			if (buildInFunctions[op.id].npar == 0)
			{
				postfixExpr.push_back(op);
				data.wantOperand = false;
			}
			else
			{
				pushStack(stackElm_t((char)op.id, pos, true));
				data.parToRead.push_back(buildInFunctions[op.id].npar);
			}
			break;
		default:
			if (data.negOperand)
			{
				postfixExpr.push_back(elm_t(Negation, data.negPos));
				data.negOperand = false;
			}
			op.orgPos = pos;
			postfixExpr.push_back(op);
			data.wantOperand = false;
	}
	return(true);
}

//! Reads a single operator
/*! This function reads an operator.

	When an error occurred, this function saves the error position in \ref errorPos and the error message in \ref errorStr and returns false.

	Some internal data is put into the private variable \ref data.

	\param expr	the expression to be parsed
	\param pos	The current parsing position. This index is changed.
	\return 	true, when an operator could be read
*/
bool SCalcExpr::readOperator(stringc &expr, u32 &pos)
{
	elm_t op;

	if (!tryToReadOperator(expr, pos, op))
	{
		errorStr = "Invalid operator.";
		errorPos = pos;
		return(false);
	}
	switch (op.type)
	{
		case Operator:
			op.orgPos = pos;
			putInOperator(op);
			data.wantOperand = true;
			break;
		case Comma:
			/* This is the separator of function parameters. So, a function have to be on the stack. */
			if (stck.empty())
			{
				errorPos = pos;
				return(false);
			}
			while (!stck.empty())
			{
				stackElm_t ch, ch2;
				if (!popStack(ch))
				{
					errorPos = pos;
					return(false);
				}
				if (ch.op == '(')
				{
					if (!stck.empty())
					{
						if (!topStack(ch2))
						{
							errorPos = pos;
							return(false);
						}
						if (ch2.isFct)
						{
							if (data.parToRead.empty() || (data.parToRead[data.parToRead.size()-1] == 0))
							{
								errorPos = pos;
								return(false);
							}
							/* One parameter read. Let's decrease then exprected number of parameters. */
							data.parToRead[data.parToRead.size()-1]--;
							if (data.parToRead[data.parToRead.size()-1] < 1)
							{
								errorStr = "Too many parameters for function.";
								errorPos = pos;
								return(false);
							}
							/* We push the opening bracket on the stack again, so it is the limit on the stack for the next parameter also. */
							pushStack(ch);
							data.wantOperand = true;
						}
					}
					break;
				}
				postfixExpr.push_back(elm_t(Operator, ch.op, ch.pos));
			}
			break;
		case Bracket:
			/* This can be a simple closing bracket or the end of a parameter list of a function. */
			while (!stck.empty())
			{
				stackElm_t ch;
				if (!popStack(ch))
				{
					errorPos = pos;
					return(false);
				}
				if (ch.op == '(')
				{
					if (!stck.empty())
					{
						/* Is a function on top of the stack? */
						if (!topStack(ch))
						{
							errorPos = pos;
							return(false);
						}
						if (ch.isFct)
						{
							/* Because there is a bracket, is can't be a parameter-less function. So, we expected some parameters. */
							if (data.parToRead.empty())
							{
								errorPos = pos;
								return(false);
							}
							/* Are all expected parameters read? */
							if (data.parToRead[data.parToRead.size()-1] > 1)
							{
								errorStr = stringc(data.parToRead[data.parToRead.size()-1]-1) +
									stringc(" more function parameters for '") + getFunctionName((fct_types)ch.op) + ("' expected.");
								errorPos = pos;
								return(false);
							}
							data.parToRead.erase(data.parToRead.size()-1);
							elm_t e(Function, ch.pos);
							e.id = (fct_types)ch.op;
							postfixExpr.push_back(e);
							popStack(ch);
						}
					}
					break;
				}
				postfixExpr.push_back(elm_t(Operator, ch.op, ch.pos));
			}
			break;
		default:
			errorStr = "Unknown operator.";
			errorPos = pos;
			return(false);
	}
	return(true);
}

//! Try to read a single operand
/*! This function reads an operand. Typical operands are numbers, variables and functions.

	Here may also be an opening bracket and a minus valid.

	Note, that my variables always start with '$', '#' or '*'.

	\param expr	the expression to be parsed
	\param pos	The current parsing position. This index is changed.
	\param op	Output: The data of the read operand.
	\return 	true, when an operand could be read
*/
bool SCalcExpr::tryToReadOperand(stringc &expr, u32 &pos, elm_t &op)
{
	skipSpaces(expr, pos);

	char ch = expr[pos];
	if (ch == '-')
	{
		op.type = Negation;
		pos++;
		return(true);
	}
	if (ch == '(')
	{
		op.type = Bracket;
		pos++;
		return(true);
	}
	if (isDigit(ch))
	{
		/* read float number */
		u32 start = pos;
		while (isDigit(expr[pos]) || (expr[pos] == '.'))
			pos++;
		op.type = Number;
		if (sscanf(expr.subString(start, pos-start).c_str(), "%f", &op.nr) != 1)
		{
			pos = start;
			return(false);
		}
		return(true);
	}
	if ((ch == '$') || (ch == '#') || (ch == '*'))
	{
		/* read var name */
		u32 start = pos;
		pos++;
		/* my variables may consist of characters, digits and '_' */
		while (isLetter(expr[pos]) || isDigit(expr[pos]) || (expr[pos] == '_'))
			pos++;
		if (pos-start <= 1)
		{
			pos = start;
			return(false);
		}
		op.type = Variable;

        if (!varHandler)
        {
			pos = start;
			return(false);
        }
		/* Here I check for the variable. Does it exist? And I get a link to it and save this link in op.var. */
		op.var = varHandler->decodeVar(expr.subString(start, pos-start));
		if (!op.var)
		{
			pos = start;
			return(false);
		}
		return(true);
	}
	if (isLetter(ch))
	{
		/* read function */
		u32 start = pos;
		pos++;
		while (isLetter(expr[pos]))
			pos++;
		if (pos-start <= 1)
		{
			pos = start;
			return(false);
		}
		stringc fct = expr.subString(start, pos-start);
		for (u32 i=0; i<NumberOfFct_C; i++)
			if (fct == buildInFunctions[i].name)
			{
				op.type = Function;
				op.id = buildInFunctions[i].id;
				return(true);
			}
		pos = start;
	}
	return(false);
}

//! Try to read a single operator
/*! This function reads an operator. Operators are '+', '-', '*', '/', '^'.

	Here may also be a closing bracket and a comma valid.

	\param expr	the expression to be parsed
	\param pos	The current parsing position. This index is changed.
	\param op	Output: The data of the read operator.
	\return 	true, when an operator could be read
*/
bool SCalcExpr::tryToReadOperator(stringc &expr, u32 &pos, elm_t &op)
{
	skipSpaces(expr, pos);

	char ch = expr[pos];
	if ((ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '^'))
	{
		op.op = ch;
		op.type = Operator;
		pos++;
		return(true);
	}
	if (ch == ')')
	{
		op.type = Bracket;
		pos++;
		return(true);
	}
	if (ch == ',')
	{
		op.type = Comma;
		pos++;
		return(true);
	}
	return(false);
}

//! Process a read operator
/*! When there are other operators on the \ref stck "stack", we have to check the precedence. So, the parts of the stack may be put
	to the \ref postfixExpr "post fix notation" and/or the read operator may be put on the stack also also to the \ref postfixExpr "post fix notation".

	\param op	the read operator
*/
void SCalcExpr::putInOperator(elm_t op)
{
	while (true)
	{
		if (stck.empty())
		{
			pushStack(stackElm_t(op.op, op.orgPos));
			return;
		}
		stackElm_t e;
		topStack(e);
		if (getPrecedence(e.op, op.op) >= 0)
		{
			popStack(e);
			postfixExpr.push_back(elm_t(Operator, e.op, e.pos));
		}
		else
		{
			pushStack(stackElm_t(op.op, op.orgPos));
			return;
		}
	}
}

//! Returns the precedence of two operators
/*! Some special logic is here for brackets.

	\param op1	the operator 1
	\param op2	the operator 2
	\return		-1 or +1
*/
s32 SCalcExpr::getPrecedence(char op1, char op2)
{
	if (op1 == '(')
		return(-1);
	if (op2 == '(')
		return(1);
	if (op1 == '^')
		return(1);
	if (op2 == '^')
		return(-1);
	if ((op1 == '+') || (op1 == '-'))
	{
		if ((op2 == '+') || (op2 == '-'))
			return(0);
		return(-1);
	}
	else
	{
		if ((op2 == '+') || (op2 == '-'))
			return(1);
		return(0);
	}
}

//! Returns the item from the top of the stack
/*! The stack itself is not changed. When the stack is empty, an error message is generated.

	\param res	Output: the top element of the stack (when there is one)
	\return		true, when the stack was not empty and res is set
*/
bool SCalcExpr::topStack(stackElm_t &res)
{
	if (stck.empty())
	{
		errorStr = "Tried to read empty stack.";
		return(false);
	}
	u32 idx = stck.size() - 1;
	res = stck[idx];
	return(true);
}

//! Returns the item from the top of the stack and removes it from the stack
/*! The returned element is removed from the stack. When the stack is empty, an error message is generated.

	\param res	Output: the top element of the stack (when there is one)
	\return		true, when the stack was not empty and res is set
*/
bool SCalcExpr::popStack(stackElm_t &res)
{
	if (stck.empty())
	{
		errorStr = "Tried to read empty stack.";
		return(false);
	}
	u32 idx = stck.size() - 1;
	res = stck[idx];
	stck.erase(idx);
	return(true);
}

//! Puts an item on the top of the stack
/*! The size of the stack is only limited by available memory.

	\param s	the element to be put on the stack
*/
void SCalcExpr::pushStack(stackElm_t s)
{
	stck.push_back(s);
}

//! Prints the parsed expression in post fix notation on the console
void SCalcExpr::printPostFixExpr()
{
	for (u32 i=0; i<postfixExpr.size(); i++)
	{
		switch (postfixExpr[i].type)
		{
			case Operator:
				printf(" (%u) op %c\n", postfixExpr[i].orgPos, postfixExpr[i].op);
				break;
			case Number:
				printf(" (%u) nr %0.3f\n", postfixExpr[i].orgPos, postfixExpr[i].nr);
				break;
			case Variable:
				printf(" (%u) var %s\n", postfixExpr[i].orgPos, postfixExpr[i].var->getName().c_str());
				break;
			case Negation:
				printf(" (%u) neg\n", postfixExpr[i].orgPos);
				break;
			case Function:
				printf(" (%u) function %s\n", postfixExpr[i].orgPos, getFunctionName(postfixExpr[i].id).c_str());
				break;
			case Bracket:
				printf(" (%u) ERROR!\n", postfixExpr[i].orgPos);
				break;
			case Comma:
				printf(" Error! Comma should not be in the post fix expression!\n");
				break;
		}
	}
}

//! Returns the function name (string) from a function id
/*! This function is required for error messaged.

	\param id	id of a function
	\return		name of this function as string (or 'ERROR!' at invalid id)
*/
stringc SCalcExpr::getFunctionName(fct_types id)
{
	for (u32 i=0; i<NumberOfFct_C; i++)
		if (id == buildInFunctions[i].id)
			return(buildInFunctions[i].name);
	return("ERROR!");
}

//! Calculates a parsed expression
/*! When an expression have been successful read and parsed (by \ref readExpr or \ref readExpression), it can be calculated by this function.
	This function is optimized on speed (no string handling).

	You have to provide an interface to your variable handler.

	When an error occurred, this function creates an error messages and prints it to the console.

	There is no exception handling for arithmetic exceptions implemented. But there are some check to prevent them (and generate error messages).

	\param local	interface to your variable handler
	\param result	Output: the result of the expression
	\return			true, when the expression could be calculated
*/
bool SCalcExpr::calcExpr(void *local, f32 &result)
{
	if (!calcExpression(local, result))
	{
		stringc part = orgExpr.subString(0,errorPos);
		printf("Error while calculating expression!\n%s\nError occurred at %s.\nOriginal expression was %s.\n",
							errorStr.c_str(), part.c_str(), orgExpr.c_str());
		return(false);
	}
	return(true);
}

#define CHECK_NEGATION      if (negNextVal) { f = -f; negNextVal = false; }

//! Calculates a parsed expression
/*! When an expression have been successful read and parsed (by \ref readExpr or \ref readExpression), it can be calculated by this function.
	This function is optimized on speed (no string handling).

	You have to provide an interface to your variable handler.

	When an error occurred, this function saves the error position in \ref errorPos and the error message in \ref errorStr. You can get these values
	with \ref getLastError and \ref getLastErrorPos.

	There is no exception handling for arithmetic exceptions implemented. But there are some check to prevent them (and generate error messages).

	\param local	interface to your variable handler
	\param result	Output: the result of the expression
	\return			true, when the expression could be calculated
*/
bool SCalcExpr::calcExpression(void *local, f32 &result)
{
	f32 f = 0.0f;
	if (!valid)
	{
		errorStr = "In calculation: tried to calculate a not successful parsed expression.";
		errorPos = 0;
		return(false);
	}
	fstck.clear();
	u32 s;
	bool negNextVal = false;
	for (u32 idx=0; idx < postfixExpr.size(); idx++)
	{
		switch (postfixExpr[idx].type)
		{
			case Operator:
				s = fstck.size();
				if (s < 2)
				{
					errorStr = "In calculation: invalid stack.";
					errorPos = postfixExpr[idx].orgPos;
					return(false);
				}
				if (!calculate(fstck[s-2], postfixExpr[idx].op, fstck[s-1], f))
				{
					errorPos = postfixExpr[idx].orgPos;
					return(false);
				}
				fstck.erase(s-2, 2);
				fstck.push_back(f);
				break;
			case Number:
				f = postfixExpr[idx].nr;
				CHECK_NEGATION;
				fstck.push_back(f);
				break;
			case Variable:
			    if (!varHandler)
                {
					errorStr = "In calculation: No variable handler defined.";
					errorPos = postfixExpr[idx].orgPos;
					return(false);
                }
			    if (!postfixExpr[idx].var)
                {
					errorStr = "In calculation: variable wasn't decoded right.";
					errorPos = postfixExpr[idx].orgPos;
					return(false);
                }
                if (!varHandler->getGameVar(postfixExpr[idx].var, local, f))
                {
					errorStr = stringc("In calculation: Can't get value of variable '") + postfixExpr[idx].var->getName() + stringc("'.");
					errorPos = postfixExpr[idx].orgPos;
					return(false);
                }
				CHECK_NEGATION;
				fstck.push_back(f);
				break;
			case Function:
				if (!resolveFunction(postfixExpr[idx].id, f))
				{
					errorPos = postfixExpr[idx].orgPos;
					return(false);
				}
				CHECK_NEGATION;
				fstck.push_back(f);
				break;
			case Negation:
				negNextVal = true;
				break;
			case Bracket:
			case Comma:
				break;
		}
	}
	if (fstck.size() != 1)
	{
		errorStr = "In calculation: invalid stack at end.";
		errorPos = 1000;
		return(false);
	}
	result = fstck[0];
	return(true);
}

//! Returns a function parameter from the stack
/*! While calculating the expression, some values have to be put on the \ref fstck "stack". Functions take their parameter from there.

	When the stack is empty, an error message is generated.

	\param p		Output: the value from the \ref fstck "stack"
	\param id		Id of the function, that reads a parameter from the stack (only for the error message)
	\return			true, when the stack was not empty
*/
bool SCalcExpr::getParameter(f32 &p, fct_types id)
{
	u32 s = fstck.size();
	if (s == 0)
	{
		errorStr = stringc("Not enough parameters for function ") + getFunctionName(id) + stringc(" in expression.");
		return(false);
	}
	p = fstck[s-1];
	fstck.erase(s-1);
	return(true);
}

//! Check for multiples
/*! Has the equation 'toTest*x=value' a integer solution for 'x'?

	When the stack is empty, an error message is generated.

	\param toTest		this may be a multiple of value
	\param value		this may be a factor of toTest
	\return				true, when toTest is a multiple of value
*/
bool SCalcExpr::equalsMultiple(f32 toTest, f32 value)
{
	f32 f = round_(toTest/value);
	return(equals(f*value, toTest));
}

//! Returns the result of a build in function
/*! The function have to be calculated. When you want additional functions, you have to add them to the enumeration \ref fct_types,
    add an entry to the array \ref buildInFunctions and	implement the calculation here.

	I tried to prevent arithmetic exceptions and generate error messages.

	The parameters of the function are put to the \ref fstck "stack" and can be read with \ref getParameter.

	\param id		the id of the function to be calculated
	\param res		Output: the result of the function
	\return			true, when no error occurred
*/
//float randf(int min_exp, int max_exp, char sign_flag) {
//    assert(min_exp <= max_exp);
//
//    int min_exp_mod = min_exp + 126;
//
//    int sign_mod = sign_flag + 1;
//    int frac_mod = (1 << 23);
//
//    int s = rand() % sign_mod;  // note x % 1 = 0
//    int e = (rand() % max_exp) + min_exp_mod;
//    int f = rand() % frac_mod;
//
//    int tmp = (s << 31) | (e << 23) | f;
//
//    float r = (float)*((float*)(&tmp));
//
//    /** uncomment if you want to see the structure of the float. */
////    printf("%x, %x, %x, %x, %f\n", (s << 31), (e << 23), f, tmp, r);
//
//    return r;
//}
//float randf()
//{
//    // returns a random value in the range [0.0-1.0)
//
//    // start with a bit pattern equating to 1.0
//    uint32_t pattern = 0x3f800000;
//
//    // get 23 bits of random integer
//    uint32_t random23 = 0x7fffff & (rand() << 8 ^ rand());
//
//    // replace the mantissa, resulting in a number [1.0-2.0)
//    pattern |= random23;
//
//    // convert from int to float without undefined behavior
//    assert(sizeof(float) == sizeof(uint32_t));
//    char buffer[sizeof(float)];
//    memcpy(buffer, &pattern, sizeof(float));
//    float f;
//    memcpy(&f, buffer, sizeof(float));
//
//    return f - 1.0;
//}

bool SCalcExpr::resolveFunction(fct_types id, f32 &res)
{
	f32 p1, p2, p3, t;
	switch (id)
	{
		case Fct_Abs:
			if (!getParameter(p1, id))
				return(false);
			res = abs_(p1);
			return(true);
		case Fct_Min:
			if (!getParameter(p1, id) || !getParameter(p2, id))
				return(false);
			res = min_(p1, p2);
			return(true);
		case Fct_Max:
			if (!getParameter(p1, id) || !getParameter(p2, id))
				return(false);
			res = max_(p1, p2);
			return(true);
		case Fct_Sinus:
			if (!getParameter(p1, id))
				return(false);
			res = sin(DEGTORAD * p1);
			return(true);
		case Fct_Cosinus:
			if (!getParameter(p1, id))
				return(false);
			res = cos(DEGTORAD * p1);
			return(true);
		case Fct_Tangens:
			if (!getParameter(p1, id))
				return(false);
			t = (p1 - 90.0f);
			if (equalsMultiple(t, 180.0f))
			{
				errorStr = stringc("Arithmetic exception: tan(") + getFloatString(p1) + stringc(") is not defined.");
				return(false);
			}
			res = tan(DEGTORAD * p1);
			return(true);
		case Fct_Random:
			if (!getParameter(p1, id))
				return(false);
			//res = randf * p1;
            res = rand() * p1;
			return(true);
		case Fct_SqRoot:
			if (!getParameter(p1, id))
				return(false);
			if (p1 < 0.0f)
			{
				errorStr = stringc("Arithmetic exception: sqrt(") + getFloatString(p1) + stringc(") is not allowed.");
				return(false);
			}
			res = squareroot(p1);
			return(true);
		case Fct_Round:
			if (!getParameter(p1, id))
				return(false);
			res = round_(p1);
			return(true);
		case Fct_Clamp:
			if (!getParameter(p1, id) || !getParameter(p2, id) || !getParameter(p3, id))
				return(false);
			res = min_(max_(p1, p2), p3);
			return(true);
		case Fct_Pi:
			res = PI;
			return(true);
		case NumberOfFct_C:
			errorStr = "Invalid function in expression.";
			return(false);
	}
	errorStr = "Function in expression not implemented.";
	return(false);
}

//! Returns the result of an operation
/*! The operation have to be calculated. Each operation takes two operands.

	When you want additional operations, you have to add them in the function \ref tryToReadOperator, add them in
	\ref getPrecedence and implement the calculation here.

	I tried to prevent arithmetic exceptions and generate error messages.

	\param a		operand 1
	\param op		operator
	\param b		operand 2
	\param res		Output: the result of the operation
	\return			true, when no error occurred
*/
bool SCalcExpr::calculate(f32 a, char op, f32 b, f32 &res)
{
	switch (op)
	{
		case '+': res = a+b; return(true);
		case '-': res = a-b; return(true);
		case '*': res = a*b; return(true);
		case '/':
			if (iszero(b))
			{
				errorStr = stringc("Arithmetic exception: division by zero is not allowed.");
				return(false);
			}
			res = a/b;
			return(true);
		case '^':
			if ((a < 0.0f) && !equals(b, round_(b)))
			{
				errorStr = stringc("Arithmetic exception: ") + getFloatString(a,3) + stringc(" raised to the power ") +
					getFloatString(b,3) + stringc(" is not allowed (must be 'negativeValue^integerValue').");
				return(false);
			}
			if (iszero(a) && iszero(b))
			{
				errorStr = stringc("Arithmetic exception: 0 raised to the power 0 is not allowed.");
				return(false);
			}
			if (iszero(a) && (b < 0.0f))
			{
				errorStr = stringc("Arithmetic exception: 0 raised to the power ") + getFloatString(b,3) +
					stringc(" is not allowed (must be '0^positiveValue').");
				return(false);
			}
			res = pow(a,b);
			return(true);
		default:
			errorStr = stringc("Invalid operator '") + stringc(op) + stringc("' in expression.");
			return(false);
	}
}

//! Is the character a digit?
/*! \param ch	the character to check.
	\return		true, when ch in '0' to '9'.
*/
bool SCalcExpr::isDigit(const char ch)
{
	return((ch >= '0')&&(ch <= '9'));
}

//! Is the character a letter?
/*! \param ch	the character to check.
	\return		true, when ch in 'a' to 'z' or in 'A' to 'Z'.
*/
bool SCalcExpr::isLetter(const char ch)
{
	return(((ch >= 'a')&&(ch <= 'z')) || ((ch >= 'A')&&(ch <= 'Z')));
}

//! Moves the string pointer forward until the next non-empty character.
/*! New line chars are also skipped.

	\param p	pointer to the string buffer.
*/
void SCalcExpr::skipSpaces(const char **p)
{
    while ((**p == ' ') || (**p == '\t') || (**p == '\n') || (**p == '\r'))
        (*p)++;
}

//! Creates a string of a float value
/*! The float will be converted to 'xxxx.xxx'. Tailing '0' will be removed. When the number is an integer, the period is also removed.

	\param f	The float value, that should be converted to a string.
	\param buf	a pointer to a string buffer, where the float string should be saved.
	\param bufSize	the size of the string buffer buf.
	\param prec	maximum number of digits after the period '.'
*/
void SCalcExpr::getFloatString(f32 f, char *buf, u32 bufSize, char prec)
{
    char tmp[] = "%0.3f";
    tmp[3] = prec;
    s32 l = snprintf(buf, bufSize, tmp, f)-1;
    while (buf[l] == '0')
    {
        buf[l] = 0;
        l--;
    }
    if (buf[l] == '.')
        buf[l] = 0;
}

//! Creates a string of a float value
/*! The float will be converted to 'xxxx.xxx'. Tailing '0' will be removed. When the number is an integer, the period is also removed.

	\param f	The float value, that should be converted to a string.
	\param prec	maximum number of digits after the period '.'
	\return		The float string.
*/
stringc SCalcExpr::getFloatString(f32 f, s32 prec)
{
    char tmp[50];
    getFloatString(f, tmp, 50, '0'+clamp(prec, 0, 9));
    return(tmp);
}
