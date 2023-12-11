//   ____    ___         __    ___      ___
//  /   __  /   \  |    |  \   |__  |  |__   |__|
//  \____/  \___/  |___ |__|   |    |  ___|  |  |
//
//
// Goldfish Interpretive Programming Language Interpreter (Goldfish Interpreter)
// source code
//
// version 8-2-2 source code
//
// created by: Nicolaus Anderson
// date: 3:08 PM, November 15, 2009

/*
    Goldfish Interpretive Programming Language Interpreter
    Copyright (C) 2010  Nicolaus Anderson

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version. If you wish to use this program as a subroutine in
	another program, you are allowed to use the GNU Library General
	Public License instead of the GNU General Public License.


    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

/////////////// LOGIC: ************
/*
The program devides up the task of processing text "code" into several functions
that are each simplistic enough to be used multiple times throughout the
program. These functions, especially the function called to read and store a
word from file, have to be simple enough to be called by multiple functions
and so therefore they must be compatible with respect to return data. That is,
what the function returns must be what the previous function needs from it.

Main holds a loop that repeats the task of searching for words until the end of
the file has been reached. Before that, Main calls a function that identifies
the file that gets a string holding the file name. This is stored in a universal
variable so that it can be used by other functions in the program.

The task for searching for words and their operations, of which is called by
main, simply calls a function to identify words and connected symbols. It then
uses if-statements to identify what functions have the correct operations to
work with the data from file.

If the word from file is a user object (e.g. function, variable, etc), it is
sent to another function that accesses a univeral variable, a vector, which is
the master list for all user objects.

If the word from file is a computer operation (e.g. if-statement, while-loop,
etc.), it is sent to a function that uses a switch to determine the operations
that the computer is to perform.

Once the program runs out of commands, it terminates itself.

See Goldfish Documentation for more details on how to use this program.

*/
//=============================&&&&&&&&&&&&&&&&&&&&&&&&&
// MAIN PROGRAM: ======-----------


// using standard libraries
#include <ios>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <vector>
#include <math.h>

using namespace::std;


// Functions from Foliage Library
float convert_str_to_flt( string );
string convert_float_to_str( float );
int convert_str_to_int( string );
unsigned int convert_str_to_Uint( string );
string convert_int_to_str( int );

//*********** PRELIMINARY ITEMS

//============= UNIVERSAL VARIABLES ++++++++++++++++++

// Debugging mode variable: indicates whether or not the programmer is debugging
bool debugging = false;

/* Create a variable that holds the user's program's "runtime stack" value.
This allows for variables and functions each be associated with a layer,
allowing them to be found more easily. */
int runtime_stack = 0; // ground level is zero
/* NOTE: in case a new file is opened that is to be read through, the actual
function (in this program) will temporarily store the runtime_stack value
until the new file has finished being read and is closed. */

/* Create a variable that stores the position to be read from by the
function readFromFile() */
int read_position;
	/* Create a read que that tells readFromFile() how far off the current
	position to set the get() pointer. This allows functions outside of
	readFromFile() to influence file reading. */

string FileName; // holds the name of the file currently being read by readFromFile()

// system characters
#define _MaxSystemChars 33 // the total number of system characters there are
char system_chars[_MaxSystemChars]; // all of the system characters in the language
/* The first array holds the system characters.
The second array holds the names of the functions associated with each of the system
characters. NOTE: The second array is declared AFTER the definition of strary. */

// system commands
#define _MaxSystemComads 15 // the total number of system commands there are
string system_comads[_MaxSystemComads]; // all of the system commands in the language

//============= STRUCTS +++++++++++++++++++++++++++++++

/* Create a struct for passing along information from file to file.
NOTE: This is not the struct that is used in the run-time stack vector! */
struct file_object
{
       string name; // this stores the name of the user object
       string word; /* this is for storing the value of the object, and it must
                    be converted using the atoi() function to be used in
                    operations of addition, subtraction, multiplication, etc. */
};

//-------------------------
/* Struct for operations using objects previously found in file. */
file_object _PREVIOUS;

//-------------------------
/* Create a struct for the system NEXT command. */
struct NEXT_elem
{
	int rp; // reading position
	string file; // file to read from
	bool opk; // override preventation key
} NEXT_blank;

//============= CLASSES +++++++++++++++++++++++++++++++
	/* Class strary

	This class is for nested strings (such as strings in arrays).
	It is specifically designed to allow the components or totality
	of a nested string (such as in an array) to be returned without
	complications in the algorithm. */
class strary
{
public:
		// functions handling assignment
	// function handling total replacement of the string
	void str_ttl_replace( string object );
	// function handling partial replacement
	void str_par_replace( char object , int position );
	// function handling piecewise replacement
	void str_pce_replace( string object , int beg , int end );

		// functions handling return
	// function handling total return
	string str_ttl_return();
	// function handling element return
	char str_par_return( int position );
	// function handling piece return
	string str_pce_return( int beg , int end );
	string str_pce_return( int beg, char stop ); // returns characters up to char stop

		// functions handling peculiar alteration
	// function handling string addition to the string
	void str_append_s( string object );
	// function handling string insertion into the string
	void str_insert_s( string object , int location );
	// function handling character addition to the string
	void str_append_c( char object );
	// function handling character insertion into the string
	//void str_insert_c( char object , int location );
	// function handling character overwriting
	void str_over_c( char object , unsigned int location );

		// other
	// function that returns the length of the string
	unsigned int str_len();

private:
	// the string
	string st;
};

// Functions of class strary ***********

	// functions handling assignment
// function handling total replacement of the string
void strary::str_ttl_replace( string object )
{ st = object; }

// function handling partial replacement
void strary::str_par_replace( char object , int position )
{ st[position] = object; }

// function handling piecewise replacement
void strary::str_pce_replace( string object , int beg , int end )
{
	for ( int c = 0; c < (end - beg); c++ )
	{ st[ c + beg ] = object[ c ]; }
}

	// functions handling return
// function handling total return
string strary::str_ttl_return()
{ return st; }

// function handling element return
char strary::str_par_return( int position )
{ return st[ position ]; }

// function handling piece return
		// function returns characters from beg to end
string strary::str_pce_return( int beg , int end )
{
	string ret;
	char d = ' ';

	for ( int c = beg; c <= end; c++ )
	{
		d = st[ c ];
		ret.push_back( d );
	}

	return ret;
}

		// function returns characters from beg to before stop
string strary::str_pce_return(int beg, char stop)
{
	string ret;
	char d = ' ';

	for ( int c = 0; c < 256; c++ )
	{
		d = st[ c ];
		
		if ( d == stop )
		{ break; }
		else {
			ret.push_back( d );
		}
	}

	return ret;
}

	// functions handling peculiar alteration
// function handling string addition to the string
void strary::str_append_s( string object )
{
	char d = ' ';

	for ( unsigned int c = 0; c < object.length() ; c++ )
	{
		d = object.at( c );
		st.push_back( d );
	}
}
// function handling string insertion into the string
void strary::str_insert_s( string object , int location )
{
	char d = ' ';
	int c2 = 0;

	for ( unsigned int c = 0; c < object.length() ; c++ )
	{
		//d = object.at( c );
		//st.insert( location + c2 , d );
		st.insert( location + c2, object );

		c2++;
	}
}

// function handling character addition to the string
void strary::str_append_c( char object )
{ st.push_back( object ); }

// function handling character insertion into the string
//void strary::str_insert_c( char object , int location )
//{
//	string s2;
//	s2.push_back( object );
//	st.insert( location, s2 );
//}

void strary::str_over_c( char object , unsigned int location )
{
	if ( st != "empty" )
	{
		string st2 = st; st.clear();
		unsigned int l = 0;
		for ( ; l < location; l++ )
		{ st.push_back( st2.at(l) ); }
		st.push_back( object );
		l++;
		for ( ; l < st2.length(); l++ )
		{ st.push_back( st2.at(l) ); }
	} else {
		st = "0";
		st[0] = object;
	}
}

	// extra
// function that returns the length of the string
unsigned int strary::str_len()
{ return st.length(); }




	/* Class variable
	
	This class allows variables to be stored in a vector by generating
	functions that allow for the extraction, manipulation, and assignment
	of a variable's value. */
class variable
{
public:
		// functions handling assignment
	// function that assigns a stack value to the variable
	void assign_level();
	void assign_level( int lev );

	// (QUASY PRIVATE SECTOR: VARIABLES)
	class internal_variables
	{
	public:
		// variable's runtime stack value (where it is in the runtime stack)
		int stack_val;
		// variable's name
		strary name;
		// variable's value
		strary value;
		// variable's lock (whether or not variable deletion is allowed
		bool lock;
		// whether the variable is a function or a class
			/* NOTE: functions and variables are both labeled "variable",
			which is the default value assigned to them upon their creation. */
		strary object_type;
	} internal_var;
};

variable blank_var; /* Multi-purpose; It is the template for all variables
					being added to the vector vars. It must be put at the
					beginning of a variable vector using the insert() command
					so that the push_back() command will function. */


// Functions of class variable **********

			// ASSIGNMENT
	// Assign a runtime stack value to the variable
void variable::assign_level()
{	internal_var.stack_val = runtime_stack;	}

void variable::assign_level( int lev )
{	internal_var.stack_val = lev;	}


//============= VECTORS ++++++++++++++++++++++++++++++

/* Create the file name run-time stack. This holds the name of the file
currently being read or in the que. */
vector<string> file_runtime_stack;
	// iterator
vector<string>::iterator _FILE_RUNTIME_STACK_iter;
unsigned int _FILE_RUNTIME_STACK_unsi = 0;

/* Create the included file run-time stack. This holds the name of the files
currently being used in the project. Inclusion is the user's choice. */
//vector<string> included;
	// iterator
//vector<string>::iterator _INCLUDED_iter;
//unsigned int _INCLUDED_unsi = 0;

	// vector holding user variables
vector<variable> vars;
		// iterator and unsigned int to extract characters from the vector
vector<variable>::iterator _vars_iter;
unsigned int _vars_UNSI = 0;

/* Create a vector holding the NEXT elements for the system NEXT command.
The last element holds the location of the object that would've come next
had a function (or reference to another file) not been called. */
vector<NEXT_elem> NEXT;
		// iterator
vector<NEXT_elem>::iterator _NEXT_iter;


//==============**************************************
// FUNCTION PROTOTYPES

void set_systemchars();
	/* Sets the system characters (these are the characters that have
	designated tasks. This setting-up is for the function that reads
	from the file the user wants "run". */

void set_systemcomads();
	/* Sets the system comands. Similar to set_systemchars. */

void getFileName();
/* This function gets the name of the file with the text "code" to be processed
by the "compiler". It stores the name as a string in a universal variable so
that all other functions can access it and use it for ifstream operations. */

string readFromFile();
/* This function extracts characters or words from a file. */

string readFileChars( int );
/* This function extracts a select number of characters from a file. */

file_object runPlainCode();
/* This function is called by main() to read from the file. It delegates tasks
to readFromFile() and OperationsControl() . */

	//--------- Functions associated with handling system characters

file_object Operations_SystemChars( file_object );
/* This function delegates tasks to sub functions according to the system
command found in file. */

bool precedence_char( string );
/* This function identifies if a given character is involved in the precedence chain
for data manipulators. The character must either be involved in combining numbers or
be a comparison operator (or part of one). */

string Quotations();
/* This function collects everything in the file between two quotation marks as
one whole string. */

string getTotal();
/* This function is for totalling up the values of variables and functions and
returning them to the assignment symbol function. */

string getTotal_condition();
/* Same as getTotal() except that it is for condition statements. */

	//---------- functions associated with handling system commands

file_object Operations_SystemComads( int );
/* This function handles operations associated with system commands. */

file_object system_cmd();
/* This function handles the operations associated with the commands
associated with the command "system".
Commands include "next" and "previous". */

file_object if_command();
/* This function handles the conditional running of code in a file. */

file_object memory_command();
/* This function handles the operations associated with the commands
associated with the command "memory".
Commands include "custom", "recall", "import", and "export". */

file_object file_command();
/* This function handles the operations associated with the commands
associated with the command "file".
Commands include "read". */

file_object math_command();
/* This function holds all of the built-in math functions.
Commands include "add", "sub", "mul", and "div". */

	string system_addition( string, string );
	string system_subtraction( string, string );
	string system_multiply( string, string );
	string system_divide( string, string );
	string system_power( string, string );

file_object in_command();
/* This function searches for a definition of a function or some user
object that is not a variable. */

file_object manip_command();
/* This function has operations that handle various manipulation tasks
on user objects. */

string if_define_found();
/* This function holds the operations associated with finding the system
command "define" without being in the context of another command (e.g. during
a file search process). */

file_object definer( string );
/* This function searches for the definition of a particular word.
The word must be user-defined in order for this function to be called. */


	//---------- functions associated with user objects

file_object Operations_UserObjects( file_object );
/* This function handles user objects (functions and variables) found in file. */

file_object recall_Operations_UserObjects( file_object );
/* This function recalls the function Operations_UserObjects() immediately
after a variable has been created. */

bool obtain_passed_objs();
/* This function finds and stores the user objects being passed to a function. */

bool findFunctionDef( string );
/* This function finds the beginning of a function definition and then sets the
reading position to the point immediately following the name of the function
at the beginning of the function definition. */

bool execute_user_function();
/* This function identifies the variables passed to a function, giving them
appropriate labels. Then it executes the code within the function definition.
When it is finished, it deletes the user variables at the end of the function
which are at the same runtime stack level as the function is. */

string count_brackets();
/* This function counts the brackets in a file in order to identify
where the end of a function, if-statement, loop, or system command is.
It returns only one of two words:
	"system" - everything went well
	"end_main" - the file has ended prematurely
*/

//******************************************
// MAIN FUNCTION *8*8*8*8*8*8*8*8*8*8*8*8*8*
int main()
{

	// VECTORS ****************
		// Set iterator positions to the beginning of the vectors
	_FILE_RUNTIME_STACK_iter = file_runtime_stack.begin();
	//_INCLUDED_iter = included.begin();

		// Add a word to the vectors to allow adding by push_back()
	 _FILE_RUNTIME_STACK_iter =
		 file_runtime_stack.insert( _FILE_RUNTIME_STACK_iter, "system" );
	 //_INCLUDED_iter = included.insert( _INCLUDED_iter, "system" );

		/* Add a blank slot to the vector to allow adding by push_back() */
	_vars_iter = vars.insert(vars.begin(), blank_var);
	vars[0].internal_var.name.str_ttl_replace( "system" );

	//*************************

    /* set the norms for the array systemchars[] */
    set_systemchars();
	
	/* set the norms for the array systemcomads[] */
	set_systemcomads();
    
    /* get the name of the file to be opened */
    getFileName();

	// more with vectors!! **************
		/* Add a blank slot to the vector to allow adding by push_back() */
	_NEXT_iter = NEXT.insert(NEXT.begin(), NEXT_blank);
	NEXT[0].file = FileName;
	//*************************
    
    /* Create a variable to receive the return value and make sure that the
    return does not terminate the program. Its data type must be the same as
    the return value, which is the same as the required return data that other
    functions call runPlainCode() to obtain. */
    file_object flag;
    
    do {
          // call the function to read from the file
          flag = runPlainCode();
    } while ( flag.name != "end_main" );

	// pause for debugging mode
	if ( debugging == true )
	{ cin >> flag.name; }
    
    return 0;
}

//=============================================
//=========== OTHER FUNCTIONS =================

// function for getting the name of the file to open
void getFileName()
{
	// LOGO
	cout << "Goldfish v-8-2-2\ncreated by: Nicolaus Anderson"
		<< "\nCopyright (C) 2010 Nicolaus Anderson\n\n";

    // prompt the user for input
	cout << "Please type in the name of the file to be openned:\n: ";
     
    /* allow for the user to enter the data into the global variable holding
    the name of the file to be openned */
    cin >> FileName;
    /* NOTE: the file name must include ".txt" for it to be useful in this
    program! */

	/* ask the user if they are debugging the program and want error messages */
	cout << "\nType \"1\" for debugging mode: ";
	cin >> debugging;

    /* Create a bunch of new lines so that the old logo and file name
	disappear. */
	// Temporary: Display a dashed line...
	cout << "\n-------------------------------------------------\n\n";

	/* At the file name to the stack. */
	file_runtime_stack.push_back( FileName );

	// set the unsigned int to the next point
	_FILE_RUNTIME_STACK_unsi = 1;
}

//======================================================
// REPEATEDLY-CALLED FUNCTIONS

//---------------------------------------------------------
// function for identifying words found in the file
file_object runPlainCode()
{
      /* Create the necessary variables. */
      file_object anObject; /* Holds the object from file */
      bool indicator = false; /* Indicates the truth or falsity of something
                              checked in a for-loop. */
	  int read_pos = 0;		/* Temporarily saves the reading position. */
	  int x = 0;			// for for-loops

	  //****************************
	  // RUN WHILE CHARACTER IS NOT A SPACE, "\n" , OR "end_main"
	  do
	  {
			/* Get a word from the file to read. */
			anObject.name = readFromFile();

			/* If end_main is found, return. */
			if ( anObject.name == "end_main" )
			{ return anObject; }

	  } while ( anObject.name == " " || anObject.name == "\n" );
	  //*****************************
      
      // Identify where to send the word
      if ( ( anObject.name[0] == '0' || anObject.name[0] == '1' ||
           anObject.name[0] == '2' || anObject.name[0] == '3' ||
           anObject.name[0] == '4' || anObject.name[0] == '5' ||
           anObject.name[0] == '6' || anObject.name[0] == '7' ||
           anObject.name[0] == '8' || anObject.name[0] == '9' )
		   || ( anObject.name[0] == '.' && anObject.name.length() > 1 ) )
		   /* NOTE: The string length is greater than 1 with a period
		   ONLY when it is a number. */
      {
           anObject.word = anObject.name; /* exchange values */

		   // Check to see if this is a number with a decimal place in it.
			// temporarily save the reading position in case this is not a float number
		   read_pos = read_position;
			// check the next character
		   anObject.name = readFileChars( 1 );
			// check to see if it is a decimal place
		   if ( anObject.name == "." )
		   {
			   // add on the decimal place
			   anObject.word.push_back( '.' );

			   // get the fractional part of the number
			   anObject.name = readFromFile();
			   
			   for ( unsigned int c = 0; c < anObject.name.length(); c++ )
			   { anObject.word.push_back( anObject.name.at(c) ); }
		   } else {
			   // restore the reading position
				read_position = read_pos;
		   }

           anObject.name = "empty"; /* indicate a non-string value */

		   // store the value in the previous
		   _PREVIOUS = anObject;

      } else {
             /* Distinguish between commands, system characters, and user
             objects. Call functions to see what is to be done with the
             objects. */
             
             /* Check for a system character, since those will be the most
             commonly used. */
             for ( x = 0; x < _MaxSystemChars; x++ )
             {
                 if ( system_chars[x] == anObject.name[0] )
                 {
					 indicator = true;
					 break; // end the for-loop
				 }
             }
             
             if (indicator == true )
             {
                    /* Go to the function that deals with system characters. */
                    anObject = Operations_SystemChars(anObject);
             } else {
                    
                    /* Check for a system command. */
                    for ( x = 0; x < _MaxSystemComads; x++ )
                    {
                        if ( system_comads[x] == anObject.name )
                        {
							indicator = true;
							break; // end the for-loop
						}
                    }
                    
                    if (indicator == true)
                    {
                           /* Go to the function that deals with system
                           commands. */
                           anObject = Operations_SystemComads(x);
                    } else {                           
                           /* Apparently the object from file is a user object,
                           so go to the function dealing with user objects. */
                           anObject = Operations_UserObjects(anObject);
                    }
             }
      }

	  return anObject;
}

//=========================================================
/* Below are functions of the Foliage Library which have been used for the
Goldfish Interpreter.
Copyright (C) 2010 Nicolaus Anderson */

//================================================================

// function that converts strings to floating point decimal numbers
float convert_str_to_flt( string word )
{
	// create the necessary variables
	float num = 0.0;	/* Stores the integer value of the "number" being
					viewed in the string. */
	float total = 0.00000000000000;	/* Store the current number. */
	bool deci_found = false; // Indicates if the decimal place has been found.
	int deci_place = 0; // Counts the numbers behind the decimal place.
	bool negative = false; // Indicates if the number is negative

		/* LOGIC: Extract each successive character from the string.
		Add each number to the floating number, multiplying the floating
		number by ten each time so that it grows. When the decimal place
		is found, account for it but do not store it in the floating point
		decimal number immediately. Instead, count the number of numbers
		that go by before the end of the word. Then divide the total floating
		point decimal number by the number of numbers that where added to the
		floating point decimal number after the decimal point had been found.
		This will account for the decimal place.
		*/

	unsigned int posit;
	for ( posit=0; posit < word.length() ; posit++ )
	{
		switch ( word[posit] ) // a switch will work for characters
		{
		case '1': num = 1.0; break;
		case '2': num = 2.0; break;
		case '3': num = 3.0; break;
		case '4': num = 4.0; break;
		case '5': num = 5.0; break;
		case '6': num = 6.0; break;
		case '7': num = 7.0; break;
		case '8': num = 8.0; break;
		case '9': num = 9.0; break;
		case '-':
			num = 0.0;
			total = total / 10; // cancel out the increment coming later

			if ( posit = 0 ) // this must occur at the beginning of the string
			negative = true;
		case '.':
			total = total / 10;
			deci_found = true;
			deci_place = deci_place - 1; // offset the latter addition
		case '0': num = 0.0;
		default: num = 0.0; break;
		}

		// count the numbers behind the decimal place
		if ( deci_found == true )
		{ deci_place++; }

		total = total*10 + num;
	}

	// account for the decimal place
	for ( ; deci_place > 0; deci_place = deci_place - 1 )
	{ total = total / 10; }

	// account for the number being negative
	if ( negative == true )
		total = total * (-1);

	return total;
}

//==========================================

// function that converts floating point decimal to strings
string convert_float_to_str( float number )
{
	// create the necessary variables
	string word; /* Holds the string to be returned. */
	float MAX_digit = 1; /* Power of ten to compare to the float number. */
	float num_digit = 1; /* Holds the number of digits in the float number.
					   This increases as digits increase, and then lowers
					   as numbers are inserted into the string. When it
					   reaches zero, the function stops examining the
					   number for digits. */
	int digit_number = 0; /* Holds the number from a certain digit to be
						  added to the string. This is used in the switch
						  to identify the character that corresponds with
						  the number from a digit in the float number. */
	char character = ' '; /* This holds the character to be stored in the
						  string to be returned. */

	/* Determine whether or not the number is negative or positive. */
	if ( number * (-1) > 0 )
	{
		// Reverse the sign.
		number = number * (-1);

		// Account for the negativity
		word.push_back( '-' );
	}

	/* Find the size of the float number. */
	if ( number >= 1 )
	{
		while ( number >= MAX_digit * 10 )
		{
			// increase MAX_digit by a power of ten
			MAX_digit = MAX_digit * 10;

			// increase the number of digits assumed in the float
			num_digit++;
		}

		/* Now that the number of digits is known, num_digit is used
		to find the number in each digit of the float number. */
		for ( int exam = 1; exam <= num_digit; exam++ )
		{
			/* Make sure to reset the digit number, since it is unknown and
			all values ( 0 - 9 ) must be checked. */
			digit_number = 0;

			if ( number < MAX_digit && number != 0 )
			{
				MAX_digit = MAX_digit/10;
			}

			while ( number >= MAX_digit )
			{
				/* Use MAX_digit to reduce the number's value to find out
				what the number is based on how many subtractions are
				required to reduce it to zero. */
				number = number - MAX_digit;

				// Account for the value of the number.
				digit_number++;
			}

			/* Identify the number */
			switch (digit_number)
			{
			case 1: /* The number is 1 */
				character = '1';
				break;
			case 2: /* The number is 2 */
				character = '2';
				break;
			case 3: /* The number is 3 */
				character = '3';
				break;
			case 4: /* The number is 4 */
				character = '4';
				break;
			case 5: /* The number is 5 */
				character = '5';
				break;
			case 6: /* The number is 6 */
				character = '6';
				break;
			case 7: /* The number is 7 */
				character = '7';
				break;
			case 8: /* The number is 8 */
				character = '8';
				break;
			case 9: /* The number is 9 */
				character = '9';
				break;
			default: /* The number is a zero */
				character = '0';
				break;
			}

			// add the character to the string
			word.push_back( character );
		}

	}

	/* Take action if there is a fraction less than 1 */
	if ( number > 0 )
	{
		// add a decimal point to the string
		word.push_back( '.' );

		// reset the maximum digit value
		MAX_digit = (float) 0.1;

		while ( number > 0.00000000000000 )
		{
			// reset the number in the digit
			digit_number = 0;

			// identify the number in that digit by subtraction
			while ( number >= MAX_digit )
			{
				// reduce the number
				number = number - MAX_digit;

				// account for the number
				digit_number++;
			}

			/* Identify the number */
			switch (digit_number)
			{
			case 1: /* The number is 1 */
				character = '1';
				break;
			case 2: /* The number is 2 */
				character = '2';
				break;
			case 3: /* The number is 3 */
				character = '3';
				break;
			case 4: /* The number is 4 */
				character = '4';
				break;
			case 5: /* The number is 5 */
				character = '5';
				break;
			case 6: /* The number is 6 */
				character = '6';
				break;
			case 7: /* The number is 7 */
				character = '7';
				break;
			case 8: /* The number is 8 */
				character = '8';
				break;
			case 9: /* The number is 9 */
				character = '9';
				break;
			default: /* The number is a zero */
				character = '0';
				break;
			}

			// add the character to the string
			word.push_back( character );

			// find out how small the digit now is
			if ( number < MAX_digit )
			{
				// decrease the MAX_digit by a power of ten
				MAX_digit = MAX_digit / 10;
			}
		}
	}

	// return the string
	return word;
}

//==========================================
// function that converts strings to integers

int convert_str_to_int( string word )
{
	// create the necessary variables
	int num = 0;	/* Stores the integer value of the "number" being
					viewed in the string. */
	int total = 0;	/* Store the current number. */

	// Identify the number:

			/* NOTE: To acount for numbers that go into the double digits, an
            algorithm views the first character, converts it to a number, and
            then multiplies it by ten if there is a second number. This
            continues for the next number and those following. */
			
    unsigned int posit;  // string character position
    for ( posit=0 ; posit < word.length(); posit++ )
    {
        total = total*10; // increase number by 10 if there is another digit

        // exchange the string value for the number value
		switch( word.at(posit) )
		{
		case '0':
			total = total / 10;
			num = 0;
			break;
		case '1': num = 1; break;
		case '2': num = 2; break;
		case '3': num = 3; break;
		case '4': num = 4; break;
		case '5': num = 5; break;
		case '6': num = 6; break;
		case '7': num = 7; break;
		case '8': num = 8; break;
		case '9': num = 9; break;
		}
	    
	    total = total + num; // add slots to number to get the true number
     }

	// return the number
	return total;
}

//==========================================
// function that converts strings to unsigned integers
unsigned int convert_str_to_Uint( string word )
{
	// create the necessary variables
	unsigned int num = 0;	/* Stores the integer value of the "number" being
					viewed in the string. */
	int total = 0;	/* Store the current number. */

	// Identify the number:

			/* NOTE: To acount for numbers that go into the double digits, an
            algorithm views the first character, converts it to a number, and
            then multiplies it by ten if there is a second number. This
            continues for the next number and those following. */
			
    unsigned int posit;  // string character position
    for ( posit=0 ; posit < word.length(); posit++ )
    {
        total = total*10; // increase number by 10 if there is another digit

        // exchange the string value for the number value
		switch( word.at(posit) )
		{
		case '0':
			total = total / 10;
			num = 0;
			break;
		case '1': num = 1; break;
		case '2': num = 2; break;
		case '3': num = 3; break;
		case '4': num = 4; break;
		case '5': num = 5; break;
		case '6': num = 6; break;
		case '7': num = 7; break;
		case '8': num = 8; break;
		case '9': num = 9; break;
		}
	    
	    total = total + num; // add slots to number to get the true number
     }

	// return the number
	return total;
}

//==========================================
// function that converts an integer to a string
string convert_int_to_str( int number )
{
	// create the necessary variables
	string word; /* Holds the string to be returned. */
	int MAX_digit = 1; /* Power of ten to compare to the integer. */
	int num_digit = 1; /* Holds the number of digits in the integer.
					   This increases as digits increase, and then lowers
					   as numbers are inserted into the string. When it
					   reaches zero, the function stops examining the
					   number for digits. */
	int digit_number = 0; /* Stores the value of the digit being examined. */
	char character = ' '; /* This holds the character to be stored in the
						  string to be returned. */

	/* Determine whether or not the number is negative or positive. */
	if ( number * (-1) > 0 )
	{
		// Reverse the sign.
		number = number * (-1);

		// Account for the negativity
		word.push_back( '-' );
	}

	/* Find the size of the integer. */
	if ( number >= 1 )
	{
		while ( number >= MAX_digit * 10 )
		{
			// increase MAX_digit by a power of ten
			MAX_digit = MAX_digit * 10;

			// increase the number of digits assumed in the integer
			num_digit++;
		}

		/* Now that the number of digits is known, num_digit is used
		to find the number in each digit of the integer. */
		for ( int exam = 1; exam <= num_digit; exam++ )
		{
			/* Make sure to reset the digit number, since it is unknown and
			all values ( 0 - 9 ) must be checked. */
			digit_number = 0;

			if ( number < MAX_digit && number != 0 )
			{
				MAX_digit = MAX_digit/10;
			}

			while ( number >= MAX_digit )
			{
				/* Use MAX_digit to reduce the number's value to find out
				what the number is based on how many subtractions are
				required to reduce it to zero. */
				number = number - MAX_digit;

				// Account for the value of the number.
				digit_number++;
			}

			/* Identify the number */
			switch (digit_number)
			{
			case 1: /* The number is 1 */
				character = '1';
				break;
			case 2: /* The number is 2 */
				character = '2';
				break;
			case 3: /* The number is 3 */
				character = '3';
				break;
			case 4: /* The number is 4 */
				character = '4';
				break;
			case 5: /* The number is 5 */
				character = '5';
				break;
			case 6: /* The number is 6 */
				character = '6';
				break;
			case 7: /* The number is 7 */
				character = '7';
				break;
			case 8: /* The number is 8 */
				character = '8';
				break;
			case 9: /* The number is 9 */
				character = '9';
				break;
			default: /* The number is a zero */
				character = '0';
				break;
			}

			// add the character to the string
			word.push_back( character );
		}

	}

	// return the string
	return word;
}
//========================================================
/* Below are all of the special functions used to obtain objects (tokens) from files
for the Goldfish Interpreter.
Copyright (C) 2010 Nicolaus Anderson */

//===================================================================

// function for reading the file for an object
string readFromFile()
{
	 /* Get the file name. Getting this allows the function
	 readFromFile() start reading another file at any point
	 in the previous file it was reading. */
	 FileName = file_runtime_stack.at( _FILE_RUNTIME_STACK_unsi );

      /* prepare and open a file for reading */
     ifstream theFile;
     theFile.open( FileName.c_str() );

	 /* Set the get() position to the latest location. */
	 theFile.seekg( read_position, ios::cur );
     
     /* read individual characters from the file */

	      // prep: create necessary variables
     char character = ' '; /* This stores the character to be placed in a
                           string and returned to the function that called this
                           function */
	 bool set = false;		/* Indicates whether or not the loop that collects
							characters from file (and puts them in a string) should
							be repeated. */
	 bool set_fail = false;	/* Indicates whether or not to reset the set variable so
							that the character-collecting loop can continue. */
	 int count = 0;			/* Used in for-loop to compare system characters with
							each new character extracted from file. */

      // initialize a string to return to the function that called this one
     string strng;
	 strng.clear();

	 //**********************************
	 // check to see if it is possible to get a character
	 character = theFile.get();

	 // do only if it is possible to extract characters
	 if ( theFile.good() )
	 {
		 /* Revert to the first character. This prevents the
		 interior do-while loop from messing up. */
         theFile.seekg(-1, ios::cur);
     
		// get characters from file
		do
		{
            // get a character from the file
			character = theFile.get();

			/* Get rid of any tabs. Turn them into spaces. */
			if (character == '	')
			{ character = ' '; }

			/* check to make sure that the character is not a system command
			character */
			for (count = 0; count < _MaxSystemChars; count++)
			{
			   /* If a character is a system character, plan to get rid of it. */
				if ( character == system_chars[count] )
			    { set = true; }
			}

			/* Terminate search for letters if the character obtained from
			file is a space. */
			if (character == ' ')
			{
				set = true; // indicate the loop should end
			}

			// Add the character if it is going to be the only one in the word.
			if ( set == false || strng.length() == 0 )
			{
				// Add the character to the word
				strng.push_back( character );

				/* Consider adding another character to the word ONLY if it
				is a number, since this might be a floating point decimal
				number. */
				if ( character == '.'
					&& ( theFile.peek() == '0' || theFile.peek() == '1'
					|| theFile.peek() == '2' || theFile.peek() == '3'
					|| theFile.peek() == '4' || theFile.peek() == '5'
					|| theFile.peek() == '6' || theFile.peek() == '7'
					|| theFile.peek() == '8' || theFile.peek() == '9' ) )
				{
					set = false;
				}
			} else {
				// Apparently, set was true, so a system character was found
				/* Only let a period be added to the word as long as there
				is no other period in the word already. */
				if ( character == '.'
					&& ( strng[0] == '0' || strng[0] == '1'
					|| strng[0] == '2' || strng[0] == '3'
					|| strng[0] == '4' || strng[0] == '5'
					|| strng[0] == '6' || strng[0] == '7'
					|| strng[0] == '8' || strng[0] == '9'
					) )
					/* NOTE: Only the first character in the word needs
					to be looked at, since the period will be added if it
					is going to be the first character in the word anyways.
					A number at the beginning of the word automatically
					indicates a floating decimal number.
					
					Minus signs are to be handled under the '-' data manipulator. */
				{
					// The system character is a period.
						// Check for a period already in the word
					set_fail = false; // assume there is no period in the word
						// start looking for a period in the word
					for (unsigned int w = 0; w < strng.length(); w++)
					{
						if (strng[w] == '.')
						{ set_fail = true; }
					}
					if ( set_fail == true )
					{
						/* If there is a period in the word already, allow
						the second period to be picked up next time. */
						theFile.seekg(-1, ios::cur);
					} else {
						// Add the period to the end of the word
						strng.push_back( character );

						/* only consider adding another character to the string
						if the character is a decimal place or a number */
						if ( theFile.peek() == '0' || theFile.peek() == '1'
							|| theFile.peek() == '2' || theFile.peek() == '3'
							|| theFile.peek() == '4' || theFile.peek() == '5'
							|| theFile.peek() == '6' || theFile.peek() == '7'
							|| theFile.peek() == '8' || theFile.peek() == '9' )
						{
							set = false;
						}
					}
				} else {
				/* The string length was not zero, or the character was
				not a period. */

					/* The system character was not added to the end of a word,
					so allow it to be extracted next time. */
					theFile.seekg(-1, ios::cur);
				}
			}

			/* Don't collect more characters if the file is at its end.
			Simply return what has been collected already. */
			if ( !theFile.good() && strng.length() > 0 )
			{
				/* Set the reading position to the point before the end. */
				theFile.seekg( -1, ios::cur );

				/* Store the reading position. */
				read_position = theFile.tellg();

				/* Return what has been acquired. */
				return strng;
			}

		} while ( set == false && theFile.good() );

		/* Store the reading position. */
		read_position = theFile.tellg();

		//************************************
		// WHAT TO DO IF AT THE END OF MAIN
		if ( !theFile.good() )
		{
			 /* assign "end_main" to the return string to indicate the
			 end of the file has been reached */
			 strng = "end_main";

			 // close the file
			 theFile.close();

			 // end the readFromFile() use here
			 return strng;
		} // endif WHAT TO DO IF AT THE END OF MAIN

		else // normal shut down
		{
			theFile.close();
		}

	 } //*********************************
		// endif - file fails to open
	 else {
		/* assign "end_main" to the return string to indicate the
		end of the file has been reached */
		strng = "end_main";

		// close the file
		theFile.close();
	 } //**********************************
     
     // return the string
     return strng;
}

//---------------------------------------------------------
// function for getting a select number of characters from file
string readFileChars( int obtain )
{
		 /* Get the file name. Getting this allows the function
	 readFromFile() start reading another file at any point
	 in the previous file it was reading. */
	 FileName = file_runtime_stack.at( _FILE_RUNTIME_STACK_unsi );

      /* prepare and open a file for reading */
     ifstream theFile;
     theFile.open( FileName.c_str() );

	 /* Set the get() position to the latest location. */
	 theFile.seekg( read_position, ios::cur );
     
     /* read individual characters from the file */

	      // prep: create necessary variables
     char character = ' '; /* This stores the character to be placed in a
                           string and returned to the function that called this
                           function */
	 int count = 0;			/* Used in while loop to compare the number of characters
							stored in the string with the number of characters
							desired in the string. */

      // initialize a string to return to the function that called this one
     string strng;


	 //**********************************
	 // check to see if it is possible to get a character
	 character = theFile.get();

	 // do only if it is possible to extract characters
	 if ( theFile.good() )
	 {
		 /* Revert to the first character. This prevents the
		 interior do-while loop from messing up. */
         theFile.seekg(-1, ios::cur);

		// get characters from file
		do
		{
            // get a character from the file
			character = theFile.get();

			// store the character in the string
			strng.push_back( character );

			// indicate another character has been put in the string
			count++;

		} while ( theFile.good() && count < obtain );

		/* Store the reading position. */
		read_position = theFile.tellg();

		//************************************
		// WHAT TO DO IF AT THE END OF MAIN
		if ( !theFile.good() )
		{
			 /* assign "end_main" to the return string to indicate the
			 end of the file has been reached */
			 strng = "end_main";

			 // close the file
			 theFile.close();

			 // end the readFromFile() use here
			 return strng;
		} // endif WHAT TO DO IF AT THE END OF MAIN

		else // normal shut down
		{
			theFile.close();
		}

	 } //*********************************
		// endif - file fails to open
	 else {
		/* assign "end_main" to the return string to indicate the
		end of the file has been reached */
		strng = "end_main";

		// close the file
		theFile.close();
	 } //**********************************
     
     // return the string
     return strng;
}
//===================================================
/* Below is the function, which skips over a section of text enclosed by brackets,
for the Goldfish Interpreter.
Copyright (C) 2010 Nicolaus Anderson */

//======================================================
// function that counts brackets in order to skip over a
// function, if-statement, loop, or system command

/* LOGIC:
This function frequently uses the runFromFile() function
and keeps track of the number of brackets open. When the
number of brackets open reaches zero, it returns "system"
to the calling function. If it fails to finish, it
returns "end_main" to the calling function. */
string count_brackets()
{
	// create the necessary variables
	int op_braks = 0;		/* This holds the number of brackets
						   that are open. When it reaches zero the
						   second time, this function returns the
						   keyword "system" to the function that
						   called it. */
	string object = "empty"; /* This holds the object from file. */

	// Search for the first bracket
	while ( object != "{" && object != "end_main" )
	{
		// get an object from file to examine it
		object = readFromFile();
	}

	if ( object == "end_main" )
	{
		// the end of the file has been found
		return object;
	} else {
		// The first bracket has been found, so account for that
		op_braks = 1;

		// Count brackets since the first one has been found
		while (op_braks > 0 && object != "end_main")
		{
			// get an object from file to examine it
			object = readFromFile();

			// determine if an open bracket has been found
			if ( object == "{" )
			{
				// account for the open bracket
				op_braks++;
			} else {
				// determine if a closing bracket has been found
				if ( object == "}" )
				{
					// account for the closing bracket
					op_braks = op_braks - 1;
				}
			}
		}

		/* Determine if "end_main" was the cause of the while-loop
		being terminated. */
		if ( object == "end_main" )
		{ return object; }
		else {
			object = "empty";
			return object;
		}
	}
}
//=======================================================
/* Below are all of the functions handling the system characters for the
Goldfish Interpreter.
Copyright (C) 2010 Nicolaus Anderson */

//===================================================================

/* Function that sets the system characters, meaning, the
characters that the program will recognize as establishing
the syntax and operations and will act as manipulators
of the interpreted language. */
#line 5000 "set_systemchars"
void set_systemchars()
{
	// establish the system characters in their proper array
     system_chars[0] = ' ';
     system_chars[1] = ';';
     system_chars[2] = ':';
     system_chars[3] = '(';
     system_chars[4] = ')';
     system_chars[5] = '\'';
     system_chars[6] = '\"';
     system_chars[7] = '\n';
     system_chars[8] = '+';
     system_chars[9] = '-';
     system_chars[10] = '*';
     system_chars[11] = '/';
     system_chars[12] = '=';
     system_chars[13] = '.';
     system_chars[14] = ',';
     system_chars[15] = '<';
     system_chars[16] = '>';
     system_chars[17] = '&';
     system_chars[18] = '|';
     system_chars[19] = '[';
     system_chars[20] = ']';
     system_chars[21] = '{';
     system_chars[22] = '}';
     system_chars[23] = '?';
     system_chars[24] = '~';
     system_chars[25] = '`';
     system_chars[26] = '!';
     system_chars[27] = '@';
     system_chars[28] = '#';
     system_chars[29] = '$';
     system_chars[30] = '%';
     system_chars[31] = '^';
	 system_chars[32] = '\\';
}

//=================================================

// function for dealing with system characters

/* Logic:
Force the computer to search for the definition of a system character in either
the current file being examined or in an included file.

Some system characters have built-in commands, such as "=" meaning assignment.
*/
#line 10000 "Operations_SystemChars"
file_object Operations_SystemChars( file_object _RECEIVED )
{
	// Create the necessary variables
	file_object package; // object to return
	file_object save; // general purpose
	int read_pos = 0; // temp read position holder

	// identify the system character
	switch ( _RECEIVED.name[0] )
	{
	case ' ': // in case space is read, ignore it
			// return an empty object
		package.name = " ";
		package.word = "empty";
		break;
	//===============================
	case ';': // semicolon reserved for statement ending
			// return an empty object
		package.name = ";";
		package.word = "empty";
			// clear the previous object
		_PREVIOUS.name = ";";
		break;
	//===============================

	case '(': // open parenthesis is reserved for running code

		do {
			// run objects from file
			package = runPlainCode();

			// in case the end of the file is reached
			if ( package.name == "end_main" )
			{
				// Error message
				cout << "\nERROR: End of file reached before closing parenthesis.";
				// crash
				return package;
			}
		} while ( package.name != ")" );

		// adjust the last thing found in file
		_PREVIOUS.name = package.name;
		package.word = _PREVIOUS.word;

		break;
	//------------------------------

	case ')': // close parenthesis is reserved for responding to open parenthesis
		// signature
		package.name = ")";
		// DO NOT interupt the value process!
		// save the last value found in file as the value to be returned
		package.word = _PREVIOUS.word;
		break;
	//===============================

	case '\"': // quotation marks are reserved for making strings

		// Request that the next text in the file be returned as a value.
		package.word = Quotations();
		package.name = "\"";

		// make sure the end of the file has not been reached
		if ( package.word == "end_main" )
		{
			// save the reading position
			read_pos = read_position;
			// check for end of main
			package.name = readFromFile();
			if ( package.name == "end_main" ) // throw error
			{ package.name = "end_main"; }
			else {
				package.name = "\"";
				read_position = read_pos;
			}
		}
		break;
	//===============================

	case '\n': // new line character acts similar to the space
		package.name.clear();
		package.word.clear();
		package.name = "\n";
		package.word = "\n";
		break;
	//==============================

	case '=': // "=" - assignment symbol / equality comparison symbol
		// save the previous object found in file
			// this is the name of the variable whose value is to be altered
		save = _PREVIOUS;

		// set the value of the PREVIOUS variable to zero
		_PREVIOUS.name = "=";
		_PREVIOUS.word = "0";

		// identify if this is acting as a comparison of equality
			// save the reading position in case necessary
		read_pos = read_position;
			// get the next character from file
		package.name = readFileChars( 1 );

		if ( package.name == "=" )
		{
				// treat as a comparison of equality. do not restore the read position

			// the first total is saved in save.word
			// get the next total
			package.word = getTotal_condition();

			if ( save.word == package.word )
			{
				package.word = "true";
			} else {
				package.word = "false";
			}

		} else {
				// treat this as an assignment to a variable

			// restore the reading position
			read_position = read_pos;

			// get the total to store in a variable
			package.word = getTotal();

			// search for the variable (in the vector vars) whose value is to be altered
			for ( _vars_UNSI = 0; _vars_UNSI < vars.size(); _vars_UNSI++ )
			{
				/* if the variable in the vector has the same name as the one
				being searched for */
				if ( vars[ _vars_UNSI ].internal_var.name.str_ttl_return() == save.name )
				{
					// check permissions
						/* the variable may be changed if it is a universal variable */
					if ( vars[_vars_UNSI].internal_var.stack_val == 0 )
					{
						vars[_vars_UNSI].internal_var.value.str_ttl_replace( package.word );
					} else {
						/* the variable must either be unlocked or be on the same
						runtime stack level. */
						if ( vars[_vars_UNSI].internal_var.lock == false
							|| vars[_vars_UNSI].internal_var.stack_val == runtime_stack )
						{
							// assign the total to the variable
							vars[_vars_UNSI].internal_var.value.str_ttl_replace( package.word );
						}
					}
				}
			}
		}
		
		break;
	//===============================

	case ',': // the comma is reserved for various purposes
		package.name = ",";
		package.word = "empty";
		break;
	//===============================

		// the curly brackets are reserved for outlining definitions of functions, etc.

	case '{': // { - opening curly bracket
		// save the reading position in case the brackets need to be skipped
		read_pos = read_position - 1; /* NOTE: the first { must be seen again for
									  count_brackets() to work properly. */

		// run code in a file, but handle when "break" and "return"
		do {
			// run objects from file
			package = runPlainCode();

			// in case the end of the file is reached
			if ( package.name == "end_main" )
			{
				// Error message
				cout << "\nERROR: End of file reached before closing parenthesis.";
				// crash
				return package;
			}

			// in case a "return" or "break" is reached
			if ( package.name == "return" || package.name == "break" )
			{
				// return to the starting reading position
				read_position = read_pos;

				// skip the do-while loop
				package.name = count_brackets();

				if ( package.name == "end_main" )
				{
					// error message
					if ( debugging == true )
						cout << "\nERROR: Missing } to end if-statement.";

					return package;
				}

				// signature
				package.name = "{}";
				// save the data
				package.word = _PREVIOUS.word;

				return package;
			}

		} while ( package.name != "}" );

		// adjust the last thing found in file
		package.name = "{}";
		package.word = _PREVIOUS.word;
		break;
	case '}': // } - closing curly bracket
		// signature
		package.name = "}";
		break;
	//===============================

	case '\\': // backslash indicates ignore next character
		// signature
		package.name = "\\";

		// set the return value to "\" and tack on the next character found in file
		/* NOTE: other parts of this program will not recognize their key characters
		if in the same string as "\" */
		package.word = "\\";

		// get the next character
		package.word.push_back( readFileChars( 1 )[0] );

		break;
	//===============================
		/* When a character is not found, but triggers this function, and thus, the
		user must have defined it. Search for that definition and return the result
		from that defintions operations. */

	default:
		// call the definer
		package = definer( _RECEIVED.name );
	}

	// return the default result of the operations in case there is none in the switch
	return package;
}

//=====================================================

#line 15000 "system_char_operations"
//--------------------------------------------
// Function that returns what is in quotations as a total
string Quotations()
{
	// create necessary variables
	string package;
	string item = " "; // object from file
	bool end = false; // indicates when to stop searching for the last quotation mark

	// search for the last quotation mark
		// get the first object from file (allows for push_back command)
	item = readFromFile();

	if ( item != "\"" )
	{
		// main search
		do {
			// add the object to the word to be returned if it is not the last mark
			if ( item != "\"" )
			{
				for( unsigned int c = 0; c < item.length(); c++ )
				{
					if ( item[c] == '\\' ) // indicates add the next character
					{
						// get the next character from file
						item = readFromFile();

						// add the next character if not at the end of the file
						if ( item != "end_main" )
						{
							if ( item == "n" )
							{ package.push_back( '\n' );
							} else {
								package.push_back( '\\' );
								package.push_back( item[c] );
							}
						}
						else {
							if ( debugging == true )
								cout << "\nERROR: Unfinished quotation.";

							package = "end_main";
							return package;
						}
					} else {
						// move the contents of item to the word to be returned
						package.push_back( item[c] );
					}
				}
			}

			// get an object from file
			item = readFromFile();

		} while ( item != "\"" );
	} else {
		package = "empty";
	}

	return package;
}

//--------------------------------------------
// Function indicating if a character is involved in data manipulator precedence chain

//bool precedence_char( string _REC )
//{
//	if (
//		_REC != ";" && _REC != "(" && _REC != ")"
//		&& _REC != "^" && _REC != "*" && _REC != "/" && _REC != "+"
//		&& _REC != "-" && _REC != ">" && _REC != "<" && _REC != "&"
//		&& _REC != "|"
//		)
//	return false;

//	else return true;
//}

//=====================================================

// Function that totals up a bunch of values from numbers, variables, and functions.
// Basically, it is the assignment symbol function, but it does little work.

string getTotal()
{
	// Create the necessary variables
	string package = "empty";	// String to be returned.
	file_object storage;		/* File object for storage. This allows for the
								value to be returned without end_main interupting
								the operations that use the value. */

	do {
		// continue to get values from file
			/* NOTE: Other functions are responsible for combining the values. */
		storage = runPlainCode();

		// make sure that the end of the file has not been reached
		if ( package == "end_main" )
		{
			/* set back the reading position so that the end of the file can
			be found later */
			read_position = read_position - 1;
			// return the found value
			return package;
		}

		// keep looking for values until the end of the expression, indicated by ;
	} while ( storage.name != ";" );

	// save the total, which will be left in _PREVIOUS
	package = _PREVIOUS.word;

	return package;
}

//-----------------------------
/* this is essentially the same function but it is used for comparison/contrast
in condition statmements */

string getTotal_condition()
{
	// Create the necessary variables
	string package = "empty";	// String to be returned.
	file_object storage;		/* File object for storage. This allows for the
								value to be returned without end_main interupting
								the operations that use the value. */

	do {
		// continue to get values from file
			/* NOTE: Other functions are responsible for combining the values. */
		storage = runPlainCode();

		// make sure that the end of the file has not been reached
		if ( package == "end_main" )
		{
			/* set back the reading position so that the end of the file can
			be found later */
			read_position = read_position - 1;
			// return the found value
			return package;
		}

		// keep looking for values until the end of the expression, indicated by ;
	} while ( storage.name != ")" && storage.name != "&&" && storage.name != "||" );

	// save the total, which will be in _PREVIOUS
	package = _PREVIOUS.word;

	return package;
}
//---------------------------
// Comment handling

string comments()
{
	// create the necessary variables
	string package;

	// for now, skip only "//"
	do {
		package = readFromFile();

		// in case the end of the file is reached
		if ( package == "end_main" )
			break;
	} while ( package != "\n" );

	// return (only error messages)
	return package;
}

//==================================================
/* Below are all of the functions handling the system commands for the
Goldfish Interpreter.
Copyright (C) 2010 Nicolaus Anderson */

//===================================================================
#line 20000 "system_comads"
/* Function that sets the system commands, meaning, the
commands that the program will associate with certain
operations and manipulators. */
void set_systemcomads()
{
	 system_comads[0] = "system";
	 system_comads[1] = "end_main";
     system_comads[2] = "if";
     system_comads[3] = "while";
     system_comads[4] = "memory"; // sub-commands: custom, import, export, etc.
     system_comads[5] = "print";
     system_comads[6] = "delete"; // deletes user objects, not files
     system_comads[7] = "input";
     system_comads[8] = "return";
	 system_comads[9] = "file";
						/* "file" includes "file scan current", meaning scanning
						the current file for characters or words. */
	 system_comads[10] = "break"; // ends a loop or switch
	 system_comads[11] = "define"; // for defining user objects
	 system_comads[12] = "math";
						/* "math" includes "math sin" (sine)???, "math cos" (cosine)???,
						"math abs" (absolute value)???, "math sqrt" (square root),
						"math base" (non-fractional part of a number)???,
						"math frac" (fractional part of a number)???,
						"math modulus" (modulus)???, etc. */
	 system_comads[13] = "in"; // identifies the location of a definition
	 system_comads[14] = "manip"; // random manipulation tasks on user objects
}

//======================================================

/* This function handles operations associated with system commands. */
file_object Operations_SystemComads( int cmd )
{
	// create the necessary variables
	file_object package;
	bool error_flag = false; // error flag (to be used in the switch)
	int read_pos = 0;	// temporarily stores the reading position

	/* Search for the command using the value of "command_num" to
	identify the actions associated with that command. */
	switch ( cmd )
	{
	case 0: /* system */
		// Identify the system command to be used by running the system command function
		package = system_cmd();
		break; //------------------------------------------

	case 2: /* if - conditional use of certain actions */
		package = if_command();

		// if the end of main is not reached, check to see if the "if" has been done
		if ( package.name != "end_main")
		{
			// if the "if" has not been done, look for else
			if ( package.word != "true" )
			{
				// save the reading position
				read_pos = read_position;

				// look for else, which MUST be the next word
				do {
					package.name = readFromFile();
				} while ( package.name == " " || package.name == "\n" );

				// only if else has been found are its contents to be activated
				if ( package.name == "else" )
				{
					// do the commands in the file
					package = runPlainCode();

					return package;
				} else {
					// restore the reading position
					read_position = read_pos;
				}
			} else {
				/* In case there is an "else" following the if-statement, jump
				over it. */

				// save the reading position, in case there is no else
				read_pos = read_position;

				// look for else, which MUST be the next word after closing curly bracket
				do {
					package.word = readFromFile();
				} while ( package.word == " " || package.word == "\n" );

				// only if else has been found are its contents to be skipped
				if ( package.word == "else" )
				{
					// look for curly brackets
						// save the reading position
					read_pos = read_position;
					
					do {
						package.name = readFromFile();
					} while ( package.name == " " || package.name == "\n" );

					// now that the object is found, restore the reading position
					read_position = read_pos;

					// if the curly bracket is found
					if ( package.name == "{" )
					{
						// skip the contents between the brackets
						package.word = count_brackets();
					} else {
						// skip to the next line change
						do {
							package.name = readFromFile();
						} while ( package.name != "\n" );
					}
				} else {
					// restore the reading position so that the next word can be run
					read_position = read_pos;
				}
			}
			package.name = "empty";
		}
		package.word = "empty";
		break; //--------------------------------------

	case 3: /* while - conditional continuous running */

		/* check for "do" to indicate whether or not to do everything without checking
		to see if it fulfills the conditions */
		read_pos = read_position;
		do {
			package.name = readFromFile();

			if ( package.name == "end_main" ) // end of file?
				return package;
		} while ( package.name == " " || package.name == "\n" );
		
		// is there a "do" command?
		if ( package.name == "do" )
		{
			// stores actual location before condition
			read_pos = read_position;

			// run the contents between the brackets
				// search for the open bracket
			do {
				package.name = readFromFile();

				if ( package.name == "end_main" )
				{
					// error message
					if ( debugging == true )
						cout << "\nERROR: Missing { to begin do-while-loop.";

					return package;
				}
			} while ( package.name != "{" );

				// the { is found, now run what is between the {}
			do {
				package = runPlainCode();

				if ( package.name == "end_main" )
				{
					// error message
					if ( debugging == true )
						cout << "\nERROR: Missing } to end do-while-loop.";

					return package;
				}

				// in case a "return" or "break" is reached
				if ( package.name == "return" || package.name == "break" )
				{
					// return to the starting reading position
					read_position = read_pos;

					// skip the do-while loop
					package.name = count_brackets();

					if ( package.name == "end_main" )
					{
						// error message
						if ( debugging == true )
							cout << "\nERROR: Missing } to end do-while-loop.";

						return package;
					}

					// set the package to the last value obtained before the break
					package = _PREVIOUS;

					return package;
				}

			} while ( package.name != "}" );
		}
		// restore the reading position
		read_position = read_pos;

		do { // repeat multiple times if necessary, since this begins the "while" aspect
			package = if_command();

			// does the process need to stop?
			if ( package.name == "break" || package.name == "return" )
			{
				// return the value
				if ( package.name == "return" ) // return directly to the calling function
					return package;
				else // just get out of the loop
					break;
			}

			// return to the beginning if the processing requires repetition
			if ( package.word == "true" && error_flag == false )
			{ read_position = read_pos; }

		// does it need to be repeated?
		} while ( package.word == "true" && error_flag == false );

		// if-statements return nothing except error flags
		if ( package.name != "end_main" && package.name != "break" )
		{
			package.name = "empty";
			package.word = "empty";
		}
		break; //-------------------------------------------

	case 4: /* memory - includes commands "custom", "recall", "import", and "export" */

		// go to the function handling this
		package = memory_command();
		break;

	case 5: /* print (display) - show objects and text in the command prompt window */

		/* continue to find and display objects until the end of the
		statement has been reached. */
		do {
			// get an object from file in its value form
			package = runPlainCode();

			// in case the end of the file is reached
			if ( package.name == "end_main" )
			{ return package; }

			// check for special characters before outputting a value
			if ( package.name != ";" && package.name != "end_main" )
			{
				// search the string for "\"
					// don't bother searching if the string is "empty"
				if ( package.word != "empty" )
				{
					for ( unsigned int c = 0; c < package.word.length(); c++ )
					{
						/* NOTE: More than one \n may exist in the string. */
						if ( package.word.at(c) == '\\' )
						{
							// examine the package contents
								// ONLY IF THE STRING IS NOT AT ITS END
							if ( c + 1 < package.word.length() )
							{
								if ( package.word.at(c+1) == 'n' )
									cout << endl;
								if ( package.word.at(c+1) == '\\' )
									cout << "\\";
								if ( package.word.at(c+1) == '\'' )
									cout << "\'";
								if ( package.word.at(c+1) == '\"' )
									cout << "\"";

								// do not let the next character be examined
								c++;
							} else { break; }
						}
						else {
							cout << package.word.at(c);
						}
					}
				}
			}

		} while ( package.name != ";" && package.name != "end_main" );

		// nothing is returned by "display"
		if ( package.name == "end_main" )
		{
			if ( debugging == true )
				cout << "\nERROR: Print function ending prematurely.";
		} else {
			package.name = "empty";
			package.word = "empty";
		}
		break;

	case 6: /* delete - remove a variable from the parallel vectors */
		// obtain the object's name
		/* NOTE: Functions and elements of classes are NOT to be deleted.
		NOTE: Locked variables are NOT tio be deleted. */

		do {
			package.name = readFromFile();
		} while ( package.name == " " || package.name == "\n" );

		/* Now that the name has been obtained, search for it in the
		user objects vector. NOTE: Most likely it will be one of the last
		objects in the vector. */

		_vars_iter = vars.end() - 1; // sets iterator to last element instead of after it
		for (  unsigned int loc = vars.size() - 1; loc > 0; loc = loc - 1 )
		{
			if ( package.name == vars[loc].internal_var.name.str_ttl_return() )
			{
				// delete the variable
				_vars_iter = vars.erase( _vars_iter );

				/* NOTE: Only one object needs to be deleted, so break here. */
				break;
			}

			// decrement the iterator
			--_vars_iter;
		}

		break;

	case 7: /* input - get the user's keyboard input */
		package.name = "input";
		cin >> package.word;
		_PREVIOUS = package;
		break;

	case 8: /* return - set the return value of a function */
		// obtain the value to be returned from the file, saving it in _PREVIOUS
		do {
			package = runPlainCode();

			if ( package.name == "end_main" ) // in case of file end
				return package;
		} while ( package.name == " " || package.name == "\n" );

		// save the final value to the previous
		_PREVIOUS = package;

		// return signature to the function to indicate operations end here
		package.name = "return";

		break;

	case 9: /* file -
			includes commands that open and close files and run the code in them */

		// go to the function handling this
		package = file_command();
		break;

	case 10: /* break - for ending loops and exiting switches */
		package.name = "break";
		break;

	case 11: /* define - for defining user objects, or rather, to indicate
			 that an object has been defined */

		// go to the function handling this
		package.name = if_define_found();
		break;

	case 12: /* math - do math operations including sine and cosine */
		package = math_command();
		break;

	case 13: /* in - search for a definition in another file */
		package = in_command();
		break;

	case 14: /* manip - edit and return particular parts of values */
		package = manip_command();
		break;

	default: /* end_main - stop running the file */
		package.name = "end_main";
		package.word = "empty";
		break;
	}

	// return the result
	return package;
}

//========================================================
// Functions handling system commands
#line 25000 "system_comads_operations-system"
//--------------------------------------------------------
/* Function handling the functions for the commands associated
with the command "system". */

/*
Syntax:

system [command]

where [command] is one of the following:

previous
sub
next

"previous" is a direct accessing/editting of the variable "_PREVIOUS",
which holds information about the previous object found in file.
Under "previous" are the commands (with syntax):

name
value
save_as "new name" "new value"

"name" returns the name of the object previously found in file as stored in _PREVIOUS.
"value" returns the value of the object previously found in file as stored in _PREVIOUS.
"save_as" overwrites the information in previous.


"sub" is a level setting for "next".
It alters how many "next" levels to go down. (see "next" for details)
Syntax:

sub "number to go down"


"next" accesses the next objects in file after the location of a defined object call.
When an object of type "define" is called, the current location in the file is saved.
Then the reading position is shifted to the location of the definition of the object.
Objects at the original location can be accessed through the "next" command via
the following sub commands:

"number to skip"
goto

"number to skip" is not a command but a number telling how many objects to skip.
That number of objects at the previous location will be skipped.
Operations can then end with a semicolon (or any random character) or
one of the sub commands can be called:

name
value
total

"name" returns the name of the "number to skip"th object from the old position.
"value" returns the value of that object.
"total" returns the result of an assignment operation starting from the "number to skip"th position.

Note that after any of these operations, the reading position will be returned
to the original current position.
This is not the case with "goto", which has the same syntax but without the sub commands.

Objects can be accessed even if in other file
(and such may be the case for definitions of objects in outside files).


The "goto" command is followed by the name of an object to skip up to.
Words are skipped until the object matching the given name is found.


The "sub" command can precede the "next" command.
If multiple "defines" have been called (as in, a defined object in a define is called),
sub indicates which "original position" to return to.
(For example, if one defined object is called in another,
sub 1 would allow for accessing the objects that a normal "next" command would if
it were in the definition that the defined object were found in.)
*/

file_object system_cmd()
{
	// create the necessary variables
	file_object package;
	int read_pos = 0; // temporary storage of the reading position
	string temp;
	unsigned int sub = 0;
	unsigned int u_num = 0;
	bool file_change = false; // indicates a change in file being viewed

	// identify the next word
	do {
		package.name = readFromFile();

		if ( package.name == "end_main" )
			return package;
	} while ( package.name == " " || package.name == "\n" );

	// PREVIOUS object information
	if ( package.name == "previous" )
	{
		if ( package.word == "name" )
		{ package.word = _PREVIOUS.name; }
		else {
			if ( package.word == "value" )
			{ package.word = _PREVIOUS.word; }
			else {
				// direct edit of _PREVIOUS
				if ( package.word == "save_as" )
				{
					/* This operation alters what is saved in _PREVIOUS
					Format:
					system previous save_as "new name" "new value"
					where _PREVIOUS.name = "new name"
					and _PREVIOUS.word = "new value"
					*/

					// identify the next word
					do {
						package = runPlainCode();

						if ( package.name == "end_main" )
							return package;
					} while ( package.name == " " || package.name == "\n" );

					// save the new name
					_PREVIOUS.name = package.word;

					// identify the next word
					do {
						package = runPlainCode();

						if ( package.name == "end_main" )
							return package;
					} while ( package.name == " " || package.name == "\n" );

					// save the new value
					_PREVIOUS.word = package.word;

					// return the new previous name
					package.name = _PREVIOUS.name;

				} else {
					// error
					if ( debugging == true ) // debug error message
						cout << "\nERROR: Incorrect \"system previous\" command.";
				}
			}
		}

		return package;
	}
	else { // else for "previous" command

	/* layers to go down if necessary (ex: if the object desired to be viewed by
	NEXT is outside two defines rather than one)
	*/
	if ( package.name == "sub" )
	{
		// identify the sub layer to view
		do {
			package = runPlainCode();

			if ( package.name == "end_main" )
				return package;
		} while ( package.name == " " || package.name == "\n" );

		// save the sub layer to be viewed
		sub = convert_str_to_Uint( package.word );

		// identify the next word
		do {
			package.name = readFromFile();

			if ( package.name == "end_main" )
				return package;
		} while ( package.name == " " || package.name == "\n" );
	}

	// NEXT OBJECT INFORMATION
	if ( package.name == "next" )
	{
		// identify the next word, which might be a number
		do {
			temp = readFromFile();

			if ( temp == "end_main" )
			{
				package.name = "end_main";
				return package;
			}
		} while ( temp == " " || temp == "\n" );

		// identify the next word, which could be "name", "value", or "total"
		do {
			package = runPlainCode();

			if ( package.name == "end_main" )
				return package;
		} while ( package.name == " " || package.name == "\n" );

		// if the user/programmer does not want to skip ahead to a specific character
		if ( temp != "goto" )
		{
			/* convert the number to an unsigned integer so it can be used in the
			for loop */
			u_num = convert_str_to_Uint( temp );
		}

		/* save the original reading position in the definition
		so that the rest of the operations in the definition can be done */
		read_pos = read_position;

		// set the new reading position
		read_position = NEXT[ NEXT.size()-1-sub ].rp;

		// if the file is not the same as the current
		if ( NEXT[ NEXT.size()-1-sub ].file != file_runtime_stack.at(
			file_runtime_stack.size()-1-sub ) )
		{
			// set the file name
			file_runtime_stack.push_back( NEXT[ NEXT.size()-1-sub ].file.c_str() );
			// indicate a new file is being viewed
			file_change = true;
		}

		// continuation of if the programmer doesn't want to skip to a certain object
		if ( temp != "goto" )
		{
			// use a for-loop to search for the next object or objects after that
			for ( unsigned int obj = 0; obj < u_num; obj++ )
			{
				// look for the next object
				do {
					temp = readFromFile();

					// in case the end of the file is reached
					if ( temp == "end_main" )
					{
						package.name = "end_main";
						package.word = "empty";
						return package;
					}
				} while ( temp == " " || temp == "\n" );
			}

			// save the query
			temp = package.word;

			if ( temp == "name" || temp == "value" )
			{
				if ( temp == "name" )
				{
					do {
						package.word = readFromFile();
					} while ( package.word == " " || package.word == "\n" );
					package.name = "NEXT name"; // signature
				}

				// obtain the value
				if ( temp == "value" )
				{
					// obtain the object, name and value
					package = runPlainCode();
					package.name = "NEXT value"; // signature
				}
			} else {

				// does the user want a total?
				if ( temp == "total" )
				{
					package.word = getTotal();
					package.name = "NEXT total";
				}
				else { package.name = temp; } // in case of end_main
			}

			// reset the old reading position and return to the proper file
			read_position = read_pos;
		}

		else { // the user/programmer wants to skip ahead to a certain object
			// "goto" command

			/* The command "goto" has been stored in "temp", indicating that the
			reading position is to be shifted to the point AFTER the last object
			to be scanned, which is given in "package.word". */

			// scan up to the point after the key object
			do {
				temp = readFromFile();
			} while ( temp != package.word );

			// save the reading position
			NEXT[ NEXT.size()-1-sub ].rp = read_position;

			// return to the original reading position
			read_position = read_pos;

			// wipe the package
			package.name = "empty";
			package.word = "empty";
		}

		// if a different file is being viewed, account for it
		if ( file_change == true )
		{
			// remove the new file from the stack
			file_runtime_stack.pop_back();
		}

	} // closing of the "next" command
	} // closing of else for "previous" command

	// return data
	return package;
}

//--------------------------------------------------------
#line 30000 "system_comads_operations-if"
/* Function handling the operations associated with the system command "if".
The command is a condition tester that determines whether the segment of code
between the next pair of curly brackets should be run. */
file_object if_command()
{
	// create the necessary variables
	file_object package;
	bool error_flag = false; // error flag (to be used in the switch)
	int read_pos = 0;	// temporarily stores the reading position

	// clear the package
	package.name = "empty";
	package.word = "empty";

	// save the starting location
	read_pos = read_position;

	// search for the open bracket
	while ( package.name != "(" && error_flag == false )
	{
		// Search the file
		package.name = readFromFile();

		// In case the end of the file is reached
		if ( package.name == "end_main" )
		{
			if ( debugging == true )
				cout << "\nERROR: if-statement incomplete.";

			return package;
		}

		/* Make sure the closing parenthesis has not been
		found. Otherwise, indicate an error. */
		if ( package.name == ")" )
		{
			// Send an error message
			if ( debugging == true )
				cout << "\nERROR: Missing opening parenthesis in if-statement.";

			// set the error flag
			error_flag = true;
		}
	}

	/* If the opening bracket is found, examine the condition. */
	if ( package.name == "(" && error_flag == false )
	{
		/* Search for the condition. It must be true or false.

		NOTE: The values of true and false must come from the
		value, since the programmer may use a single variable to
		indicate the condition.
			
		NOTE: The values of true and false must be searched for
		until a closing parenthesis is found. */
		while ( package.name != ")" )
		{

			do {
				// Search the file for variables and actions
				package = runPlainCode();

				/* If the closing parenthesis is found in this
				loop, determine if a "true" or "false" has been
				found. If neither has been found, return "true". */
				if ( package.name == ")" )
				{
					/* Is there NOT a "true" or "false" value returned
					from file? */
					if ( package.word != "true" && package.word != "false" )
					{
						// give the default return
						package.word = "true";
					}
				}

				// In case the end of the file is reached
				if ( package.name == "end_main" )
				{
					if ( debugging == true )
						cout << "ERROR: if-statement incomplete.";

					return package;
				}
			} while ( package.word != "true" && package.word != "false" );
		}

		/* Check to see whether or not to perform the actions
		in the if-statement. If not, count brackets. */
		if ( package.word == "true" )
		{
			// do commands in the file
			package = runPlainCode();

			// indicate the code was run
			package.word = "true";
		} else {
			// identify if the next characters are brackets. if so, skip them
				// save the reading position
			read_pos = read_position;

			do {
				package.name = readFromFile();

				// in case the end of the file is reached
				if ( package.name == "end_main" )
				{
					// error message
					if ( debugging == true )
						cout << "\nERROR: No definition to if-statement.";

					return package;
				}
			} while ( package.name == " " || package.name == "\n" );

			if ( package.name == "{" )
			{
				// set the reading position back one so that the bracket will be seen
				read_position = read_position - 1;
				// skip the brackets
				count_brackets();
			} else { // else just skip the next line
				// restore the reading position
				read_position = read_pos;
				// skip the next line
				do {
					package.name = readFromFile();

					// in case the end of the file is reached
					if ( package.name == "end_main" )
					{
						// error message
						if ( debugging == true )
							cout << "\nERROR: No definition to if-statement.";

						return package;
					}
				} while ( package.name != "\n" );
			}
			
			// empty the package
			package.name = "empty";
			package.word = "empty";

			return package;
		}
	} else {
		// error
		if ( debugging == true )
		{
			cout << "\nERROR:";
			if ( package.name != "(" )
				cout << " Missing open parenthesis for if-statement.";
			else
				cout << " if-statement declaration error.";
		}

		// if-statement error return
		package.name = "error";
		package.word = "empty";
	}

	return package;
}

//--------------------------------------------------------
#line 35000 "system_comads_operations-memory"
/* Function handling the functions/operations associated with the
command "memory". */

file_object memory_command()
{
	// create the necessary variables
	file_object package; // general purpose
	int read_pos = 0; // temp storage of reading position
	bool error = false; // for general purpose

	/* The command "memory" will always be followed by a sub-command that tells
	what specifically is to be done.
	These commands are:
	custom // create a custom object, e.g. variable or function
	recall // recall the main value of a custom object
	alter_type // not yet created: change the type of object an object is
	import // unlock an object (from a lower level of the runtime stack) to edit it
	export // lower the runtime stack value of an object by one

	The format is:
	memory [command]
	where [command] is some command followed by its own specific set of commands.
	
	Commands:
	custom [object type] "object_name" lock/unlocked
	This command creates a custom object with the name "object_name" of the
	type [object_type] with the lock setting either locked or unlocked.

	recall "object_name"
	This command returns the return value of an object.

	import "object_name"
	This command will make a user object editable on the current level of the runtime
	stack.

	export "object_name"
	This command decrements by one the runtime stack value of an object.

	alter_type "object_name" [new_type]
	This command changes the type of an object named "object_name" to another
	type called [new_type].
	*/

			// search for the keyword
		do {
			package.name = readFromFile();
		} while ( package.name == " " || package.name == "\n" );

			// identify the keyword
		if ( package.name == "custom" )
		{
			/* custom - custom define the names of variables, functions, or objects
			during runtime.
			Format:
			memory custom [object type] "variable_name" ["lock"/"unlocked"]
			*/

			// create a blank slot in the user objects vector
			vars.push_back( blank_var );

			// search for the type
			do {
				package.name = readFromFile();
			} while ( package.name == " " || package.name == "\n" );

			// save the type
			vars[vars.size() - 1].internal_var.object_type.str_ttl_replace( package.name );

			// search for the name
			do {
				package = runPlainCode();
			} while ( package.name == " " || package.name == "\n" );

			// save the name ( which will be in the returned value )
			vars[vars.size() - 1].internal_var.name.str_ttl_replace( package.word );

			// save the name and value in _PREVIOUS so that it can be assigned a value
			_PREVIOUS.name = package.word;
			_PREVIOUS.word = "empty";

			// save the reading position to check for a lock
			read_pos = read_position;

			// search for the lock
			do {
				package.name = readFromFile();
			} while ( package.name == " " || package.name == "\n" );

			// save the lock
			if ( package.word == "locked" )
			{ vars[vars.size() - 1].internal_var.lock = true; }
			else {
				// default lock assumes variable is to be used on this level of runtime stack
				vars[vars.size() - 1].internal_var.lock = false;

				// restore the reading position
				read_position = read_pos;
			}

			// signature
			package = _PREVIOUS;

		} else {
			if ( package.name == "recall" )
			{
				/* recall - make accessible an object whose name is a custom value,
				i.e. determined at runtime.
				Format:
				memory recall "object_name"
				*/

				// obtain the object's name (NOTE: it will be given as a value)
				do {
					package = runPlainCode();
				} while ( package.name == " " || package.name == "\n" );

				// the name of the user object is given in the value
				_PREVIOUS.name = _PREVIOUS.word;

				// search for the name in the user objects vector
				for ( unsigned int loc = vars.size() - 1; loc > 0; loc = loc - 1 )
				{
					if ( _PREVIOUS.word == vars[loc].internal_var.name.str_ttl_return() )
					{
						if ( vars[loc].internal_var.lock == false )
						{
							// identify the type
							if ( vars[loc].internal_var.object_type.str_ttl_return()
								!= "variable"
								&& vars[loc].internal_var.object_type.str_ttl_return()
								!= "function" )
							{
								// is it self-defined?
								if ( vars[loc].internal_var.object_type.str_ttl_return()
									== "define" )
								{
									// search for the object's definition
									package = definer( _PREVIOUS.name );
								} else {
									// use the definition it has been given
									package = definer(
										vars[loc].internal_var.object_type.str_ttl_return() );
								}
							} else { // assume this is a function or variable

								// if this is a function, find and run it
								if ( vars[loc].internal_var.object_type.str_ttl_return()
									== "function" )
								{
									// obtain the objects being passed to the function
									error = obtain_passed_objs();

									// find the function
									error = findFunctionDef( _PREVIOUS.name );

									// only if there is no error
									if ( error == false )
										error = execute_user_function();
								} else {
										// this is a variable
									// save the value to be returned
									package.word =
										vars[loc].internal_var.value.str_ttl_return();

									_PREVIOUS.word = package.word;
								}
							}

							// end the for-loop
							break;
						}
					}
				}

			} else {
				if ( package.name == "import" || package.name == "export" )
				{
					/* import - unlock a variable from a sub-level of the
					runtime stack for use.
					Format:
					memory import "object_name"
					*/

					/* export - save a variable for use on a lower level
					of the runtime stack.
					Format:
					memory export "object_name"
					*/

					// obtain the object's name
					do {
						package = runPlainCode();
					} while ( package.name == " " || package.name == "\n" );

					// search for the object in the user objects vector
					for ( unsigned int loc = vars.size() - 1; loc > 0; loc = loc - 1 )
					{
						// only change the first object found having this name
						if ( package.word
							== vars[loc].internal_var.name.str_ttl_return() )
						{
							if ( package.word == "import" )
							{
								// unlock the value
								vars[loc].internal_var.lock = false;
							} else {
								if ( package.word == "export" )
								{
									// decrement the runtime stack value of the object
									vars[loc].internal_var.stack_val -= 1;
								}
							}

							break; // exit for-loop
						}
					}
				} else {

					if ( package.name == "alter_type" )
					{
						/* alter_type - change the type of object a user object is.
						Format:
						memory alter_type "object_name" "new_type"
						*/

						// obtain the object's name
						do {
							package = runPlainCode();
						} while ( package.name == " " || package.name == "\n" );

						// search for the object in the user objects vector
						for ( unsigned int loc = vars.size() - 1; loc > 0; loc = loc - 1 )
						{
							// only unlock the first object found having this name
							if ( package.word
								== vars[loc].internal_var.name.str_ttl_return() )
							{
								// obtain the new type for the object
								do {
									package = runPlainCode();
								} while ( package.name == " " || package.name == "\n" );

								// change the object type
								vars[loc].internal_var.object_type.str_ttl_replace(
									package.word );

								break; // exit for-loop
							}
						}

					} else {
						// error
						if ( debugging == true )
							cout << "\nERROR: Improper keyword ( "
							<< package.name << " ) following \"memory\" command.";
					}
				}

				// set signature
				package.name = "empty";
				package.word = "empty";
			}
		}

	// return the data
	return package;
}

//--------------------------------------------------------
#line 40000 "system_comads_operations-file"
/* Function handling the functions/operations associated with the
command "file". */

file_object file_command()
{
	// for debug
	cout << "\nfile_command()"; // flag

	// create the necessary variables
	file_object package; // general purpose
	file_object save;
	int read_pos = 0; // temp storage for reading position

	/* The command "file" will always be followed by a sub-command that tells
	what specifically is to be done.
	These commands are:


	The format is:
	file [command]
	where [command] is some command followed by its own specific set of commands.
	
	Commands:
	read
		-> in
		-> at
		-> word
		-> char
	edit
		-> insert
		-> delete // imcomplete
	run

	ex:
	file read in "some_file.txt" at [reading position] word
	This operation will read a word out of a text file named "some_file.txt"
	starting at the reading position given by [read position].
	*/

		// search for the keyword
	do {
		package.name = readFromFile();

		if ( package.name == "end_main" ) // if at the end of the file
		{
			if ( debugging == true )
				cout << "\nERROR: Incomplete command: file.";

			return package;
		}
	} while ( package.name == " " || package.name == "\n" );

	// read command - includes sub commands "word" and "char" (character)
	if ( package.name == "read" )
	{
		// indicate that there is no other file to be checked nor location to start at
		save.name = "empty";
		save.word = "empty";

		// identify the name of the file to be read from (if not this one)
			// search for the keyword
		do {
			package.name = readFromFile();

			if ( package.name == "end_main" ) // if at the end of the file
			{
				if ( debugging == true )
					cout << "\nERROR: Incomplete command"
						<< " \"file read\".";

				return package;
			}
		} while ( package.name == " " || package.name == "\n" );

		if ( package.name == "in" )
		{
			// search for the file name to be searched in
			do {
				package = runPlainCode();

				if ( package.name == "end_main" ) // if at the end of the file
				{
					if ( debugging == true )
						cout << "\nERROR: Incomplete command"
							<< " \"file read\".";

					return package;
				}
			} while ( package.name == " " || package.name == "\n" );

			// save the name of the file to be read from
			save.name = package.word;

			// search for the next keyword
			do {
				package.name = readFromFile();

				if ( package.name == "end_main" ) // if at the end of the file
				{
					if ( debugging == true )
						cout << "\nERROR: Incomplete command"
							<< " \"file read\".";

					return package;
				}
			} while ( package.name == " " || package.name == "\n" );
		}

		// identify if there is a location in the file the user wants to start at
		if ( package.name == "at" )
		{
			// search for the keyword to look for in the file
			do {
				package = runPlainCode();

				if ( package.name == "end_main" ) // if at the end of the file
				{
					if ( debugging == true )
						cout << "\nERROR: Incomplete \"at\" in command"
							<< " \"file read\".";

					return package;
				}
			} while ( package.name == " " || package.name == "\n" );

			// save the keyword to look for in the file
			save.word = package.word;

			// search for the command keyword to look for in the file
			do {
				package = runPlainCode();

				if ( package.name == "end_main" ) // if at the end of the file
				{
					if ( debugging == true )
						cout << "\nERROR: Incomplete \"at\" in command"
							<< " \"file read\".";

					return package;
				}
			} while ( package.name == " " || package.name == "\n" );
		}

		// save the name of the file to be read from if other than this file
		if ( save.name != "empty" )
		{ file_runtime_stack.push_back( save.name ); }

		// save the current reading position
		read_pos = read_position;

		// alter the reading position if necessary. default = zero
		if ( save.word != "empty" )
		{ read_position = convert_str_to_int( save.word ); }
		else { read_position = 0; }

		// word reading
		if ( package.name == "word" )
		{
			// scan the file for a single word (NOTE: Spaces are considered words!!)
			package.word = readFromFile();
		} else {
			// character reading
			if ( package.name == "char" )
			{
				// scan the file for a single character
				package.word = readFileChars(1);
			} else {
				// error
				if ( debugging == true )
					cout << "\nERROR: No other command associated with command"
						<< " \"file read\".";
			}
		}

		// restore the reading position
		read_position = read_pos;

		// restore the original file
		if ( save.name != "empty" )
		{ file_runtime_stack.pop_back(); }

	} else {
		/* File editing commands!!!!! */

		if ( package.name == "edit" )
		{
			// search for the next keyword
			do {
				package.name = readFromFile();

				if ( package.name == "end_main" ) // if at the end of the file
				{
					if ( debugging == true )
						cout << "\nERROR: Incomplete command"
							<< " \"file edit\".";

					return package;
				}
			} while ( package.name == " " || package.name == "\n" );

					/* insert command */
			/* Format:
			file edit insert "object_name" at [read position] in "some_file.txt"
			*/
			if ( package.name == "insert" )
			{
				// search for the item to insert into the file
				do {
					package = runPlainCode();

					if ( package.name == "end_main" ) // if at the end of the file
					{
						if ( debugging == true )
							cout << "\nERROR: Incomplete \"insert\" in command"
								<< " \"file edit\".";

						return package;
					}
				} while ( package.name == " " || package.name == "\n" );

				do {
					package.word = readFromFile();

					if ( package.word == "end_main" ) // if at the end of the file
					{
						if ( debugging == true )
							cout << "\nERROR: Incomplete \"insert\" in command"
								<< " \"file edit\".";

						package.name = "end_main";
						return package;
					}
				} while ( package.word == " " && package.word == "\n" );

				// reading position must come first
				if ( package.word == "at" )
				{
					// obtain the reading position value
					do {
						package.word = readFromFile();
					} while ( package.word == " " && package.word == "\n" );

					// save the numeric value to search for
					read_pos = convert_str_to_int( package.word );

					// obtain the next token/object from file
					do {
						package.word = readFromFile();
					} while ( package.word == " " && package.word == "\n" );
				} else {
					read_pos = 0; // default is set to the beginning of the file
				}

				// is the file name given? else = error
				if ( package.word == "in" )
				{
					// obtain the file name
					do {
						save = runPlainCode();

						if ( save.name == "end_main" ) // if at the end of the file
						{
							if ( debugging == true )
								cout << "\nERROR: Incomplete \"in\" in command"
									<< " \"file edit\".";

							return save;
						}
					} while ( save.name == " " || save.name == "\n" );

					// create a pointer to a file
					ofstream aFile;

					// open the file (name is in save.word)
					aFile.open( save.word.c_str() );

					/* insert the object into the file
					(name in package.name) (location for insertion given by read_pos) */
						// seek the location
					aFile.seekp( read_pos, ios::beg );
						// insert the object
					aFile << package.name;

					// close the file
					aFile.close();

				} else {
					// error
					if ( debugging == true )
						cout << "\nERROR: Incomplete command \"file edit\".";
				}

			} else {
					/* delete command */
				//if ( package.name == "delete" )
				//{
					/* Delete may require the copying of en entire file, editing it
					in a character array, and saving over the previous file.
					To make deleting objects worth while, it is best to have a
					que-style delete that makes multiple deletions possible. */
					
					/* NOTE: The delete requests must be in successive order in how
					they come in the file. In other words, one section to be deleted
					must not be listed to be deleted before a section that comes
					before it in the file. */

					/*

					// obtain the file name
					do {
						package.name = readFromFile();

						if ( package.name == "end_main" ) // if at the end of the file
						{
							if ( debugging == true )
								cout << "\nERROR: Incomplete \"insert\" in command"
									<< " \"file edit\".";

							return package;
						}
					} while ( package.name == " " && package.name == "\n" );

					// create file openers
					ifstream loadFile;
					ofstream saveFile;
					// open the files
					loadFile.open( package.name.c_str() );
					saveFile.open( package.name.c_str() );

					*/

					// find out what is to be deleted. Stop at semicolon
					/* Format:
					delete "some_file.txt" [rpotd1 start] [rpotd1 end],
					[rpotd2 start] [rpotd2 end], [rpotd3 start] [rpotd3 end];

					where "rpotd" stands for "read position of object to delete"
					*/
					//do {
					// obtain the locations of the parts of the file to be deleted

					/* copy the desired contents into the desired file while skipping
					what the user does not want */
					//} while ();

					// close the file
					//loadFile.close();
					//saveFile.close();

				//} else {
					// error
					if ( debugging == true )
						cout << "\nERROR: Incomplete command \"file edit delete\".";
				//}
			}

			// return signature
			package.name = "empty";
			package.word = "empty";

		} else {
			/* File run command!!!!!
			Format:
			file run "some_file.txt"
			*/
			if ( package.name == "run" )
			{
				// obtain the name of the file to be run
				package = runPlainCode();

				/* add the file to the end of the file runtime stack so that it
				will be read by readFromFile() */
				file_runtime_stack.push_back( package.word.c_str() );

				// increment the runtime stack layer
				runtime_stack++;

				// run the contents of the file
				do {
					package = runPlainCode();
				} while ( package.name != "end_main" );

				// delete all of the old variables at the current runtime stack layer

					// sets iterator to last element instead of after it
				_vars_iter = vars.end() - 1;

				for (  unsigned int loc = vars.size() - 1; loc > 0; loc = loc - 1 )
				{
					if ( vars[loc].internal_var.stack_val >= runtime_stack )
					{
						// delete the variable
						_vars_iter = vars.erase( _vars_iter );
					}

					// decrement the iterator
					--_vars_iter;
				}

				/* now decrement the stack value since operations are going to return
				to the current file */
				runtime_stack = runtime_stack - 1;

				// return to the previous file for reading
				file_runtime_stack.pop_back();

			} else {
				// error
				if ( debugging == true )
				{
					cout << "\nERROR: Improper operation associated with command"
						<< " \"file\".";
				}
			}
		}
	}

	// return the data if any
	return package;
}

//--------------------------------------------------------
#line 45000 "system_comads_operations-math"
/* Function holding all of the built-in math commands. */

file_object math_command()
{
	// create the necessary variables
	file_object package; // general purpose
	file_object save; // general purpose
	file_object save2; // general purpose

	/* Identify whether this is math for strings or numeric math. */
	do {
		package.name = readFromFile();

		if ( package.name == "end_main" ) // end of file error
		{
			if ( debugging == true ) // debug error message
				cout << "\nERROR: Incomplete \"math\" command.";

			return package;
		}
	} while ( package.name == " " || package.name == "\n" );

	// for string math
	if ( package.name == "str" )
	{
		/* Current math for strings involves push_back and pop_back. */
			/* Identify the specific mathematical operation to be used. */
		do {
			package.name = readFromFile();

			if ( package.name == "end_main" ) // end of file error
			{
				if ( debugging == true ) // debug error message
					cout << "\nERROR: Incomplete command: \"math str\".";

				return package;
			}
		} while ( package.name == " " || package.name == "\n" );

		/* String addition */
		if ( package.name == "add" )
		{
			// obtain the strings to be combined
				// first string
			do {
				package = runPlainCode();

				if ( package.name == "end_main" ) // end of file error
				{
					if ( debugging == true ) // debug error message
						cout << "\nERROR: Incomplete command: \"math str add\".";

					return package;
				}
			} while ( package.name == " " || package.name == "\n" );

				// second string
			do {
				save = runPlainCode();

				if ( save.name == "end_main" ) // end of file error
				{
					if ( debugging == true ) // debug error message
						cout << "\nERROR: Incomplete command: \"math str add\".";

					return save;
				}
			} while ( save.name == " " || save.name == "\n" );

			// combine the strings
			for ( unsigned int c = 0; c < save.word.length(); c++ )
			{ package.word.push_back( save.word.at( c ) ); }

			// return the data
			package.name = "math_str_add";
			_PREVIOUS = package;
			return package;

		} else {
			/* String subtraction */
			if ( package.name == "sub" )
			{
				/* obtain the string a series of characters to be taken from
				the string */
					// string
				do {
					package = runPlainCode();

					if ( package.name == "end_main" ) // end of file error
					{
						if ( debugging == true ) // debug error message
							cout << "\nERROR: Incomplete command: \"math str add\".";

						return package;
					}
				} while ( package.name == " " || package.name == "\n" );

					// series of characters
				do {
					save = runPlainCode();

					if ( save.name == "end_main" ) // end of file error
					{
						if ( debugging == true ) // debug error message
							cout << "\nERROR: Incomplete command: \"math str add\".";

						return save;
					}
				} while ( save.name == " " || save.name == "\n" );

				// empty save.name for later use
				save.name = " ";

				// take out a certain sequence of characters
				for ( unsigned int s = package.word.length()-1; s>=0; s=s-1)
				{
					// is the sequence found? check by checking the next characters
					if ( package.word.at( s ) == save.word.at( save.word.length()-1 ) )
					{
						// assign what is in package.word to an unbiased spectator variable
						save.name = " ";
						save.name[0] = package.word.at(s-save.word.length()+1);

						for ( unsigned int u=s-save.word.length()+2; u<=s ; u++ )
						{ save.name.push_back( package.word.at(u) ); }

						// now check the validity of the match
						if ( save.name == save.word ) // match
						{
							// proceed to remove the select characters from the word
								// move the characters from the end down
							for ( unsigned int u=s-save.word.length()+1; u<=s; u++)
							{
								if ( u + save.word.length() >= package.word.length() ) // account for out of bounds
								{
									break;
								} else {
									// proceed normally
									package.word[ u ] = package.word.at( save.word.length() + u );
								}
							}

							/* clear the necessary var
							make sure the var is not entire empty:
							push_back must be used */
							save.name = " ";
							// save the first slot
							save.name[0] = package.word.at( 0 );

							// remove the excess slots
							for ( unsigned int c = 1; c<s; c++ )
							{ save.name.push_back( package.word.at( c ) ); }
							package.word = save.name;

							// break from the for-loop
							break;
						}
						// else, continue
					}
				}

				// return the data
				package.name = "math_str_sub";
				_PREVIOUS = package;
				return package;

			} else {
				/* Error */
				if ( debugging == true ) // debug error message
					cout << "\nERROR: Incomplete command: \"math str\".";

				package.name = "end_main";
				return package;
			}
		}
	} else {
		// numeric
		if ( package.name == "num" )
		{
			/* Identify the specific mathematical operation to be used. */
			do {
				package.name = readFromFile();

				if ( package.name == "end_main" ) // end of file error
				{
					if ( debugging == true ) // debug error message
						cout << "\nERROR: Incomplete command: \"math num\".";

					return package;
				}
			} while ( package.name == " " || package.name == "\n" );

			save = runPlainCode();
			save2 = runPlainCode();

			/* Numeric combinations of numbers */
			if ( package.name == "add" )
			{
				package.word = system_addition( save.word , save2.word );
			} else {
				if ( package.name == "sub" )
				{
					package.word = system_subtraction( save.word , save2.word );
				} else {
					if ( package.name == "mul" )
					{
						package.word = system_multiply( save.word , save2.word );
					} else {
						if ( package.name == "div" )
						{
							package.word = system_divide( save.word , save2.word );
						} else {
							if ( package.name == "total" )
							{
								package.word = getTotal();
							} else {
								/* Error */
								if ( debugging == true ) // debug error message
									cout << "\nERROR: Incomplete command: \"math num\".";

								package.name = "end_main";
								return package;
							}
						}
					}
				}
			}

		} else {
			// error
			if ( debugging == true ) // debug error message
				cout << "\nERROR: Improper operations associated with command"
				<< "\"math\".";
		}
	}

	// return the data
	return package;
}

//--------------------------------------------------------
/* Function handling the operations associated with the command "in". */

/* Following the format: in "file.txt" type myFunction
This tells the interpreter to read from the file "file.txt" to find
"myFunction", where "myFunction" is being defined as a function or as a
keyword. How: It stores the function name, gets the values being passed to
the function (if it is one) by calling "obtain_passed_objs()", changes the
file being looked at/processed, and sends the function name immediately to
"findFunctionDef()" (if type==function) or "define" (if type==define).
After either an error or function completion, the file name is immediately
set back to the original.

NOTE: There is no need for a "system include" command to be made if "in"
is being used. Thus, there is no need to create a vector of included files.
*/

/* Format:
in "some_file.txt" [object type] "object_name"
This opens a file named "some_file.txt" and searches for an object with the
name "object_name" of the type [object type].
NOTE: Functions will have to be followed by a pair of enclosing parenthesis.
These parethesis may hold the objects being passed to the function.

NOTE: This command does NOT search for variables since their values would
be entirely meaningless.
*/
#line 50000 "system_comads_operations-in"
file_object in_command()
{
	// create the necessary variables
	file_object package;
	file_object save;
	string type;		// stores the type of object to be searched for
	bool flag = false;	// indicates if there is an error or not
	int read_pos = 0;	// temporary stores the readin position

	// obtain the name of the file to be searched
	save = runPlainCode();
	// NOTE: The name will be saved as a value, thus it will be in package.word

	// obtain the type
	do {
		type = readFromFile();

		// in case the end of the file is reached
		if ( type == "end_main" )
		{
			// error message
			if ( debugging == true )
				cout << "\nERROR: in-command incoplete.";

			package.name = "end_main";
			return package;
		}
	} while ( type == " " || type == "\n" );

	// get the object's name
	do {
		package = runPlainCode();
	} while ( package.name == " "  || package.name == "\n" );

	// save the reading position so that it can be returned to when this file is
	read_pos = read_position;

	// is a function the object of the search?
	if ( type == "function" )
	{
		// obtain the variables being passed to the function
		flag = obtain_passed_objs();

	} // else, this is user-defined, not a function
	
	/* Save the reading position and the file name for the system NEXT command.
	Set the override preventation key on to insure that the file name is not
	erased. */
	NEXT.push_back( NEXT_blank );
	NEXT[ NEXT.size()-1 ].rp = read_position;
	NEXT[ NEXT.size()-1 ].file = file_runtime_stack.at( file_runtime_stack.size() - 1 );
	NEXT[ NEXT.size()-1 ].opk = true;
	/* NEXT.opk indicates to not override the file name nor to add another
	NEXT element, which is what would naturally be done. */

	// set the reading position to the beginning of any file
	read_position = 0;

	// add the name of the file to the end of the file runtime stack
	file_runtime_stack.push_back( save.word );

	// now search for the object
		// ... based on if it is a function
	if ( type == "function" )
	{
		flag = findFunctionDef( package.word );
		
		// if there is no error in finding the definition
		if ( flag == false )
		{ flag = execute_user_function(); }
	}
	else { // ... based on if it is a different object
		if ( type != "define" )
		{
			// custom user object
			package = definer( type );
		} else {
			package = definer( package.word );
		}
	}

	// stop searching that particular file
	file_runtime_stack.pop_back();

	// restore the old reading position from the calling file
	read_position = NEXT[ NEXT.size() - 1 ].rp;

	// delete the last NEXT element
	NEXT.pop_back();

	return package;
}

//--------------------------------------------------------
// random manipulation functions: the "manip" command

/* Tasks include:
Accessing particular parts of strings (ex: string[12] returns character at the
12th or 13th slot);
Removing a character at a particular slot position;
Altering the value of a particular part of a string (Assignment. ex: str[5]='f');
Making the name of an object the value and changing the return name to "empty"
(ex: for var1 = "f12",
package.name=_PREVIOUS.name="empty", package.word=_PREVIOUS.word="var1");
Incrementing/decrementing a character/digit (ex: char chr='a'; chr++; (chr=='b') ); ??
etc.
*/
#line 55000 "system_comads_operations-manip"
file_object manip_command()
{
	// create the necessary variables
	file_object package;
	string temp;
	int read_pos = 0; // temp storage of reading position

	// obtain the command
	do {
		package.name = readFromFile();

		// in case the end of the file is reached
		if ( package.name == "end_main" )
		{
			if ( debugging == true ) // error message
				cout << "\nERROR: command \"manip\" incoplete.";

			return package;
		}
	} while ( package.name == " " || package.name == "\n" );

	// identify the command

		// Accessing/altering a slot in an object value
	if ( package.name == "slot" )
	{
		// obtain the object name
		do {
			temp = readFromFile();

			// in case the end of the file is reached
			if ( temp == "end_main" )
			{
				if ( debugging == true ) // error message
					cout << "\nERROR: command \"manip slot\" incoplete.";

				package.name = "end_main";
				return package;
			}
		} while ( temp == " " || temp == "\n" );

		// obtain slot value
		do {
			package = runPlainCode();

			// in case the end of the file is reached
			if ( package.name == "end_main" )
			{
				if ( debugging == true ) // error message
					cout << "\nERROR: command \"manip slot\" incoplete.";

				return package;
			}
		} while ( package.name == " " || package.name == "\n" );

		// identify the next command: access or alter
			// save the reading position
		read_pos = read_position;
			// obtain the next object without processing it
		do {
			package.name = readFromFile();

			// in case the end of the file is reached
			if ( package.name == "end_main" )
			{
				if ( debugging == true ) // error message
					cout << "\nERROR: command \"manip slot\" incoplete.";

				return package;
			}
		} while ( package.name == " " || package.name == "\n" );

		// search for the object in the user objects vector
		for ( unsigned int loc = vars.size()-1; loc > 0; loc=loc-1 )
		{
			// object found
			if ( temp == vars[loc].internal_var.name.str_ttl_return() )
			{
				// alter
				if ( package.name == "=" )
				{
					if ( vars[loc].internal_var.lock == false ) // editable
					{
						// obtain the total
						temp = getTotal();

						// change the value
						vars[loc].internal_var.value.str_over_c(
							temp[0], convert_str_to_Uint( package.word )
							);
					}
				} else {
					/* since this is not an assignment, restore the reading
					position to read the next object */
					read_position = read_pos;
				}

				// return the value
				_PREVIOUS.name = vars[loc].internal_var.name.str_ttl_return();
				_PREVIOUS.word = vars[loc].internal_var.value.str_ttl_return();

				// return the data
				return _PREVIOUS;

				// end the for-loop since this action may be done only once
				break;
			}
		}
	} else {
		if ( debugging == true ) // error message
			cout << "\nERROR: Improper operations associated with command"
				<< "\"manip\".";
	}

	// return the data, if any
	return package;
}

//--------------------------------------------------------
/* Function handling operations associated with the command "define".
This function merely insures that the object following "define" is an
object that is classified as defined (that is, object_type==defined). */
#line 60000 "system_comads_operations-define"
string if_define_found()
{
	// create the necessary variables
	string package;
	unsigned int loc = 0; // for-loop counter
	bool found = false;

	// identify the object
	do {
		package = readFromFile();

		if ( package == "end_main" )
		{
			// error message
			if ( debugging == true )
				cout << "\nERROR: Nothing following define command.";

			return package;
		}
	} while ( package == " " || package == "\n" );

	// search for the object in the user objects vector
	for ( loc = 1; loc < vars.size(); loc++ )
	{
		// if the object is found
		if ( package == vars[loc].internal_var.name.str_ttl_return() )
		{
			// indicate the object has been found
			found = true;
			break;
		}
	}

	// if the object has been found, edit it
	if ( found == true )
	{
		vars[loc].internal_var.object_type.str_ttl_replace( "define" );
	}
	else {
		// if object does not exist, create it
		vars.push_back( blank_var );

		// add the name, type, and runtime stack value to the object
		vars[ vars.size() - 1 ].internal_var.name.str_ttl_replace( package );
		vars[ vars.size() - 1 ].internal_var.object_type.str_ttl_replace( "define" );
		vars[ vars.size() - 1 ].internal_var.stack_val = runtime_stack;
	}

	// now skip the definition
	package = count_brackets();

	// return a full string only to indicate if the end of the file has been found
	return package;
}


//========================================================
// built-in system functions

string system_power( string val1, string val2 )
{
	// create the necessary variables
	double v1 = 0.0;	// base. NOTE: 0 cannot be raised to a power
	double v2 = 0.0;	// exponent
	float vf = 0.00;	// final value; ignore truncation

	// convert the base value to numerical form
	v1 = (double) convert_str_to_flt( val1 );
		/* NOTE: It is not pointless to convert the exponent to numerical form
		unless the base is non-zero. */

	if ( v1 > 0.00000000001 )
	{
		// convert the second value
		v2 = (double) convert_str_to_flt( val2 );

		// perform an exponential operation
		vf = (float) pow( v1, v2 );
	}

	// return a string value
	return convert_float_to_str( vf );
}

string system_multiply( string val1, string val2 )
{
	// create the necessary variables
	float v1 = 0;
	float v2 = 0;

	// convert the values
	v1 = convert_str_to_flt( val1 );
	v2 = convert_str_to_flt( val2 );

	// perform multiplication of the values and return the result
	v1 = v1 * v2;
	return convert_float_to_str( v1 );
}

string system_divide( string val1, string val2 )
{
	// create the necessary variables
	float v1 = 0;
	float v2 = 0;

	// convert the values
	v1 = convert_str_to_flt( val1 );
	v2 = convert_str_to_flt( val2 );

	// perform division of the values and return the result
	v1 = v1 / v2;
	return convert_float_to_str( v1 );
}

string system_addition( string val1, string val2 )
{
	// create the necessary variables
	float v1 = 0;
	float v2 = 0;

	// convert the values
	v1 = convert_str_to_flt( val1 );
	v2 = convert_str_to_flt( val2 );

	// perform addition of the values and return the result
	v1 = v1 + v2;

	return convert_float_to_str( v1 );
}

string system_subtraction( string val1, string val2 )
{
	// create the necessary variables
	float v1 = 0;
	float v2 = 0;

	// convert the values
	v1 = convert_str_to_flt( val1 );
	v2 = convert_str_to_flt( val2 );

	// perform subtraction of the values and return the result
	v1 = v1 - v2;

	return convert_float_to_str( v1 );
}
//==========================================================
/* Below is/are the function(s) that search for the definitions of user-defined
objects in the Goldfish Interpretive Programming Language for the
Goldfish Interpreter.
Copyright (C) 2010 Nicolaus Anderson */

//==============================================================
#line 65000 "define"
// definer: the function that searches for the definition of a particular word
file_object definer( string key )
{
	// create the necessary variables
	file_object package; // normal
	string object;	// holds objects from file

	/* Create a new NEXT element and save the appropriate file name to it.
	Do NOT create a new NEXT element or save the file name if the override
	preventation key is on.*/
	if ( NEXT[ NEXT.size()-1 ].opk == false )
	{
		NEXT.push_back( NEXT_blank ); // new NEXT element
			// new file name
		NEXT[ NEXT.size()-1 ].file
			= file_runtime_stack.at( file_runtime_stack.size() - 1 );
	}
	/* Save the reading position and the file to be read from for the system NEXT
	command to use. */
	NEXT[ NEXT.size()-1 ].rp = read_position;

	//=========================== SEARCHES

	// search for the definition of the user's word in the current file
		// set the reading position to be at the beginning of the file
	read_position = 0;

		// search
	do {
		package.name = readFromFile();
			
		if ( package.name == "define" )
		{
			do {
				package.name = readFromFile();
			} while ( package.name == " " || package.name == "\n" );

			if ( package.name == key )
			{ package.name = "define key"; }
		}
	} while ( package.name != "define key" && package.name != "end_main" );

			// error
	if ( package.name == "end_main" && package.name != "define key" )
	{
		if (debugging == true)
			cout << "\nERROR: No definition for the object (" << key << ") found.";
	}

	//============================== RUN CONTENTS OF DEFINE

	// if define key is found
	if ( package.name == "define key" )
	{
		// run the operations
		package = runPlainCode();
	} else {
		// skip the brackets, which must be there
		package.name = count_brackets();
	}

	// restore the reading position to read the next object after the keyword
	read_position = NEXT[ NEXT.size()-1 ].rp;

	// remove the NEXT element if it is not locked
	if ( NEXT[ NEXT.size()-1 ].opk == false )
		NEXT.pop_back();

	/* do NOT return closing the closing bracket since that indicates normal
	definition end */
	if ( package.name == "{}" )
		package.name = "empty";

	return package;
}
//==========================================================
/* Below are all of the functions handling the user objects for the
Goldfish Interpreter.
Copyright (C) 2010 Nicolaus Anderson */

//======================================================
#line 70000 "user_objects"
/* This function handles user objects (functions and variables) found in file. */
file_object Operations_UserObjects( file_object _RECEIVED )
{
	// create the necessary variables
	file_object package;
	int read_posit = 0;		// temporarily stores the reading position
	bool error = false;		// indicates success of operations
	bool found = false;
	unsigned int loc = 0;	// used in the for-loop for finding the user object
	int count = 0;			// miscellaneous integer for counting purposes

	// Identify the type of object being viewed (user-created vs. internal)
		// search for the object in the vector vars
	for ( ; loc < vars.size(); loc++ )
	{
		/* The name of the variable must be the same as found in file
		the variable/function must be usable/unlocked. */
		if ( vars[loc].internal_var.name.str_ttl_return() == _RECEIVED.name
			&& vars[loc].internal_var.lock == false )
		{
			found = true;
			break;
		}
	}

	// does the object exist?
	if ( found == true )
	{

		// user-created == "defined" ; internal type == "variable"
	if ( vars[loc].internal_var.object_type.str_ttl_return() == "variable"
		|| vars[loc].internal_var.object_type.str_ttl_return() == "function" )
	{

		/* Identify the type of object being viewed (function or variable)
		by looking to see if parenthesis follow it. Yes -> function */
			// save the reading position
		read_posit = read_position;
			// peek at the next object
		do {
			package.name = readFromFile();

			// for errors
			if ( package.name == "end_main" )
				return package;
		} while ( package.name == " " );

		// is it an open parenthesis?
		if ( package.name == "(" ) // function
		{
		/* If this is a function, look at the variables in the function calling
		parenthesis. (NOTE: The function must first be found to NOT be the function
		definition, otherwise this is of no use.)
		A variable name preceded by & indicates the variable's address is to be
		saved as the value in the new variable. Without an &, only the value is
		saved in the new variable.
		A variable name preceded by * indicates the variable's address is to be
		pointed and edited directly. Multiple * indicate the direct editing of
		the variable pointed to by each successive variable. ex:
		
		var1 = 5;
		var2 = 1; // NOTE: var1 is in vector slot 1
		var3 = &var2; // address of var2 ( = 2 ) assigned to var3
		func(*var3) // value assigned to v_n changes value of var2
		... func( v_n )
		funct(**var3) // value assigned to v_m changes value of var1
		... funct( v_m )

		Save a slot in var for each variable, initially naming them numerically.
		execute_user_function() will change the temporary names when it finds
		out what the names are that the user wants. */

			/* Make sure that this is not a function definition.
			If it is, skip it. */
			do {
				package.name = readFromFile();
			} while ( package.name != ")" );
				// look for a {, which will immediately follow if this is a definition
			do {
				package.name = readFromFile();
			} while ( package.name == " " && package.name == "\n" );

			// restore the reading position
			read_position = read_posit;
				/* If this is a function definition:
				Restore the reading position to the point before the bracket
				so that count_brackets() can find it.
				
				If this is a function calling:
				Restore the reading position to the point before the parenthesis
				so that it can find the objects being passed to the function. */

			// is this a function definition?
			if ( package.name == "{" )
			{
				// skip
				package.name = count_brackets();
			} else {
					// this is a function calling

				// store the variables being passed to the function
				error = obtain_passed_objs();

				// find the function definition
				error = findFunctionDef( _RECEIVED.name );

				if ( error == false )	// the function definition was found
				{
					// execute the function
					error = execute_user_function();

					// indicate the function returning the value
					_PREVIOUS.name = _RECEIVED.name;

					// for data return
					package = _PREVIOUS;
				} else {
					// error message
					if ( debugging == true )
						cout << "\nERROR: Function definition non-existent.";

					// dump the variables put in the vector var for function use
					loc = vars.size() - 1;
					while ( vars[loc].internal_var.stack_val == runtime_stack + 1 )
					{
						vars.pop_back();
						loc = loc - 1;
					}

					// return to the original reading position
					read_position = read_posit;
				}
			}

		} else {	// variable or other user object

			/* Default value for a user object is a variable.
			However, during runtime, a variable can be changed to a defined object,
			that is, an object like a class that has a definition somewhere in file.

			NOTE: The runtime stack value and the object type must be stored with the
			user object, whether a variable or other.
			*/

			/* Upon identifying a non-function, search for it. If it does not exist,
			create it. */

			/* Restore the reading position only if not a function.
			(NOTE: This is only to take a step back to the end of the variable since
			the file was read from to look for the closing parenthesis.) */
			read_position = read_posit;

			// search for the value of the variable in the user objects vector
			for ( loc = 0; loc < vars.size(); loc++ )
			{
				if ( vars[loc].internal_var.name.str_ttl_return() == _RECEIVED.name )
				{
					// the object must be a variable since it is not a function
					package.word = vars[loc].internal_var.value.str_ttl_return();
					/* NOTE: Return the value of all user variables with that name. */
				}
			}

			// name the variable it's coming from
			package.name = _RECEIVED.name;
			_PREVIOUS = package;
			/* NOTE: Only the last value found in the user objects vector will
			be returned. */
		}
	} else {
		// search for the object in the user objects vector
		for ( loc = vars.size()-1; loc > 0; loc=loc-1 )
		{
			if ( vars[loc].internal_var.name.str_ttl_return() == _RECEIVED.name )
			{
				// check to see if the object is self-defined or is assigned to a definition
				if ( vars[loc].internal_var.object_type.str_ttl_return() == "define" )
				{
					// object is self-defined
					package = definer( _RECEIVED.name );
				} else {
					// assume it is defined by something else
					package = definer( vars[loc].internal_var.object_type.str_ttl_return());
				}

				break;
			}
		}
	}

	// object not found, so it must be created
	} else {
		// add a new slot
		vars.push_back( blank_var );

		// set the unsigned int iterator to the last slot
		_vars_UNSI = vars.size() - 1;

		// edit the blank slot, assigning it all the normal properties
		vars[ _vars_UNSI ].internal_var.stack_val = runtime_stack;
		vars[ _vars_UNSI ].internal_var.name.str_ttl_replace( _RECEIVED.name );
		vars[ _vars_UNSI ].internal_var.lock = false;
		vars[ _vars_UNSI ].internal_var.value.str_ttl_replace( "empty" );

		/* Identify the type of object being viewed (function or variable)
		by looking to see if parenthesis follow it. Yes -> function */
			// save the reading position
		read_posit = read_position;
			// peek at the next object
		do {
			package.name = readFromFile();

			// for errors
			if ( package.name == "end_main" )
				return package;
		} while ( package.name == " " && package.name == "\n" );

		// is it an open parenthesis?
		if ( package.name == "(" ) // function
		{
			vars[ _vars_UNSI ].internal_var.object_type.str_ttl_replace( "function" );
		} else {
			// assume this is a variable
			vars[ _vars_UNSI ].internal_var.object_type.str_ttl_replace( "variable" );
		}

		// restore the reading position
		read_position = read_posit;

		// recall this function ( Operations_UserObjects() )
		package = recall_Operations_UserObjects( _RECEIVED );
	}


	// in case of unsucess, usually occuring on the part of the function
	if ( error == true )
		package.name = "end_main";

	// return the result
	return package;
}


//=========----------------------------
#line 75000 "user_objects_associatives"
/* This function recalls the function Operations_UserObjects() immediately
after a new user object has been created. */

file_object recall_Operations_UserObjects( file_object _RECEIVED )
{
	return Operations_UserObjects( _RECEIVED );
}

//======================================================
/* This function finds and stores the user objects being passed to a function. */

bool obtain_passed_objs()
{
	// create the necessary variables
	file_object package;

	/* TEMPORARY IMPLEMENTATION:
	Until "=" and the variable handling part of Operations_UserObjects()
	are edited to account for pointers and address passing, use this: */

		// read for parenthesis first
	do { package.name = readFromFile(); } while ( package.name != "(" );

	/* Set a counter. This is to make sure that all variables
	being passed are labeled according to the order they are found
	in. */
	int count = 0;

	// read for objects and save them
	do {
		// read for the object name
		package.name = readFromFile();

		// make sure this is not a closing parenthesis
		if ( package.name != ")" )
		{
			/* CHANGE HERE: add a search for & , * , or multiple * before
			the name of the variable being passed. */

			// this must be an object name, thus, search for it
			for ( unsigned int loc = 0; loc < vars.size(); loc++ )
			{
				if ( vars[loc].internal_var.name.str_ttl_return()
						== package.name )
				{
					// indicate that another variable to be passed has been found
					count++;

					// create a new object in the user objects vector
					vars.push_back( blank_var );

					/* Since elements in a vector are counted starting at zero, the
					last element is at the size of the vector minus one. */

					// set the name of the object to be numerical
					vars[ vars.size() - 1 ].internal_var.name.str_ttl_replace( convert_int_to_str(count) );
					// edit the blank slot, assigning it all the normal properties
					vars[ vars.size() - 1 ].internal_var.stack_val = runtime_stack;
					vars[ vars.size() - 1 ].internal_var.lock = false;
					vars[ vars.size() - 1 ].internal_var.object_type.str_ttl_replace( "variable" );

					// CHANGE HERE: SAVE PRE-NAME SYMBOLS (EX: & AND *)

					/* Currently, the vector object's value is saved as the
					value of the new object about to be passed. */
					vars[ vars.size() - 1 ].internal_var.value.str_ttl_replace(
							vars[loc].internal_var.value.str_ttl_return()
							);
				}
			}
		}
	} while ( package.name != ")" );
	// stop searching for objects when function calling ends

	return false;
}

//======================================================
/* This function finds the beginning of a function definition and then sets the
reading position to the point immediately following the name of the function
at the beginning of the function definition. */

bool findFunctionDef( string funcName )
{
	// create the necessary variables
	string word;
	bool in = false;
	int read_pos = 0;		// temporarily holds the reading position when necessary
	bool error = false;		// indicates success of operations

	// start at the beginning of the file
	read_position = 0;

	// read from the file until the function name is found
	while ( in == false )
	{
		word = readFromFile();

		// read from the file until the function definition is found
		if ( word == funcName )
		{
			// hold the reading position
			read_pos = read_position;

			// check for parenthesis first
			word = readFromFile();

			// continue only if parenthesis are found
			if ( word == "(" )
			{
				/* Do not hold the reading position since function definitions
				are not to occur within parenthesis. */

				// look for the closing parenthesis
				while ( word != ")" && error == false )
				{
					word = readFromFile();
					// if the end of main is reached, end the loop
					if ( word == "end_main" )
						error = true;
				}

				/* If the closing parenthesis is found, look for the open
				curly bracket. */
				if ( word == ")" )
				{
					/* The open curly bracket must immediately follow the
					closing parenthesis (do not count spaces, tabs, and
					newlines). Otherwise, this is not a function definition. */
					do {
						word = readFromFile();
					}while ( word == " ");

					if ( word == "{" ) // function definition
					{
						read_position = read_pos;
						in = true;
					}
				}
			}
			else {
				read_position = read_pos;
			}
		}
	}

	return error;
}

//======================================================
bool execute_user_function()
{
	// create the necesary variables
	file_object package;	// generic use
	bool error = false;		// indicates success of operations
	unsigned int loc = 0;	// used to locate objects in user objects vector

	/* Lock all of the variables on lower runtime stack levels. (universal
	variables don't matter as far as locking is concerned)
	DO NOT LOCK FUNCTIONS!! */
	for ( loc = 0; loc < vars.size(); loc++ )
	{
		if ( vars[loc].internal_var.stack_val != 0 ) // excluding universal variables
		{
			// excluding functions
			if ( vars[loc].internal_var.object_type.str_ttl_return() != "function" )
			{
				// lock the object
				vars[loc].internal_var.lock = true;
			}
		}
	}

	/* Increment the runtime_stack value so that all new variables will be at
	that level. */
	runtime_stack++;

	// Unlock those variables on the current runtime stack level.
	loc = vars.size() - 1; // location of last object in vars vector
	while ( vars[loc].internal_var.stack_val == runtime_stack )
	{
		// unlock the objects on the current stack value
		vars[loc].internal_var.lock = false;
	}

	/* Go to the end of the vars vector to see if their are any values being
	passed to this function. If so, change their names from numbers to whatever
	names are given between the parenthesis following the name of the function
	as part of the function definiton. */
	loc = vars.size() - 1; // location of last object in vars vector
		/* Identify how many variables are being passed to this function.
		This number is revealed in the name of the last variable being passed,
		which is also the last variable in the function. */
	loc = convert_str_to_Uint( vars[loc].internal_var.name.str_ttl_return() );

		// find the open parenthesis in the function definition
	do {
		package.name = readFromFile();
	} while ( package.name != "(" );

	// rename those variables
	for ( unsigned int c = 0; c < loc; c++ )
	{
		/* Find the new name of the variable between the parenthesis in the function
		definition. */
		do {
			package.name = readFromFile();
		} while ( package.name == " " || package.name == "\n" );

		/* DO THIS IN FUNCTION CALLING:
		Either the name is found or a symbol, indicating the type of object
		this is supposed to be, such as a pointer. */

		/* The name of the variable is in package.name, and it must thus be
		applied to the variable in the vector. */
		vars[ vars.size() - c ].internal_var.name.str_ttl_replace( package.name );
	}

	/* Search for the opening bracket. Once the opening bracket is found,
	execute code until the closing bracket is found. */
	do {
		do { // first search for the closing parenthesis
			package.name = readFromFile();
		} while ( package.name != ")" && package.name != "end_main" );
	} while ( package.name != "{" && package.name != "end_main" );

	if ( package.name == "end_main" )
	{ error = true; }
	else
	{
		do {
			package = runPlainCode();
		} while ( package.name != "}" && package.name != "end_main" );
		
		if ( package.name == "end_main" )
			error = true;
	}


	/* Let the value returned be the previous value found in file.
	In other words, "return" stores the value after it in "_PREVIOUS.word".
	Furthermore, "return" returns a "}" to this function, ending operations. */

	// Delete all objects AT THE CURRENT RUNTIME STACK VALUE!!
	loc = vars.size() - 1;
	_vars_iter = vars.end() - 1; // sets iterator to the last element instead of after it
	for ( ; loc > 0 ; loc = loc - 1 )
	{
		if ( vars[loc].internal_var.stack_val == runtime_stack )
		{
			_vars_iter = vars.erase( _vars_iter ); // delete the object

			loc = loc - 1; // decrement counter to avoid "out of range" error
			_vars_iter = vars.end(); /* set to the end of the vector to insure
									 no variables are missed. */
		}

		--_vars_iter;
	}

	// Going down a stack value
	runtime_stack = runtime_stack - 1;

	// true = end_main ; false = success
	return error;
}
//===========================================================
