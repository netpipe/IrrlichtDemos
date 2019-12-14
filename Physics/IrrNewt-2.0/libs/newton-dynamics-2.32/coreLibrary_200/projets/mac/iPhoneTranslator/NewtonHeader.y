%{
	#include <stdafx.h>
	#include "main.h"


	#define YYDEBUG 1
	#define YYERROR_VERBOSE
	#define YYSTYPE LexicalData

	static	int tab;
	void GenerateTabs (char* tabs)
	{
		tabs[0] = 0;
		for (int i = 0; i < tab; i ++) {
			strcat (tabs, "\t");   
		}
	}

	extern char yytext[];
	//extern int column;
	int yyerror(char *s)
	{
	//	fflush(stdout);
	//	printf("\n%*s\n%*s\n", column, "^", column, s);
		return 0;
	}
%}


%token INT
%token CHAR
%token VOID
%token CONST
%token SHORT
%token UNSIGNED
%token FLOAT
%token DOUBLE
%token STRUCT
%token UNION
%token TYPEDEF

%token IFDEF
%token IFNDEF
%token ELSE
%token ENDIF
%token DEFINE
%token INCLUDE
%token NUMERIC_CONSTANT
%token LITERAL_IDENTIFIER
%token QUOTED_CONSTANT


%left '='
%left '+' '-'
%left '*' '/'
%left NEG

%start NewtonHeader

%%

NewtonHeader 
	: IFNDEF LITERAL_IDENTIFIER CDeclareAndClassDeclare	 ENDIF
	;

	
CDeclareAndClassDeclare	
	: StatementList IfDefStatement
	;
	
StatementList
	: Statement
	| StatementList Statement	
	;

Statement
	: HeaderInfo
		{
//			PrintHeader ($1.m_literal);
		}	
	| DefineConstant  	  
		{
			PrintDefine ($1.m_literal);
		}	
	| TypeDefStructures	  
		{
			PrintStrutureDefine ($1.m_literal);
		}	
	| StructuresDeclaration			  
		{
			PrintDataStrutures ($1.m_literal);
		}	
	| FunctionCallBackDeclaration		
		{
			PrintFunctionCallbacks ($1.m_literal);		
		}	  
	| ClassMethod 
		{
			PrintClassMethod ($1.m_literal);
		}		
	| IfDefStatement	
	| Include
	;

HeaderInfo
	:DEFINE LITERAL_IDENTIFIER

DefineConstant 
	: DEFINE literalIndentifer NUMERIC_CONSTANT 
	  {
		sprintf ($$.m_literal, "#define %s\t%s\n", $2.m_literal, $3.m_literal);
	  }
	| DEFINE LITERAL_IDENTIFIER QUOTED_CONSTANT 
	;	
	
Include
	: INCLUDE QUOTED_CONSTANT 
	| INCLUDE '<' LITERAL_IDENTIFIER '>'
	;	

IfDefStatement
	: IfDef LITERAL_IDENTIFIER EmptyDefine ENDIF
	| IfDef LITERAL_IDENTIFIER IfDefStatement ENDIF
	| IfDef LITERAL_IDENTIFIER EmptyDefine ELSE IfDefStatement ENDIF
	| IfDef LITERAL_IDENTIFIER EmptyDefine ELSE EmptyDefine ENDIF
	| IfDef LITERAL_IDENTIFIER LITERAL_IDENTIFIER QUOTED_CONSTANT '{' ENDIF
	| IfDef LITERAL_IDENTIFIER '}' ENDIF
	;

EmptyDefine
	: DEFINE LITERAL_IDENTIFIER 
	| DEFINE FLOAT FLOAT
	| DEFINE FLOAT DOUBLE
	| DEFINE LITERAL_IDENTIFIER LITERAL_IDENTIFIER '(' LITERAL_IDENTIFIER ')' 
	;

IfDef
	: IFDEF
	| IFNDEF
	;	


TypeDefStructures
	: TYPEDEF STRUCT literalIndentifer '{' '}' literalIndentifer ';'	 
	  {
		sprintf ($$.m_literal, "typedef struct i%s{} i%s;\n", $3.m_literal, $6.m_literal);
	  }
	;


literalIndentifer
	: LITERAL_IDENTIFIER
		{
			$$ = $1;
		}
	;
	

StructuresDeclaration
	: StructuresDeclarationInit DataInstanceList StructuresDeclarationEnd
		{
			sprintf ($$.m_literal, "%s%s%s\n", $1.m_literal, $2.m_literal, $3.m_literal);
		}
	;
	
StructuresDeclarationInit
	: STRUCT LITERAL_IDENTIFIER '{' 
		{
			char tabs[256];
			GenerateTabs (tabs);
			sprintf ($$.m_literal, "%sstruct i%s {\n", tabs, $2.m_literal);
			tab ++;
		}
	| UNION '{'
		{
			char tabs[256];
			GenerateTabs (tabs);
			sprintf ($$.m_literal, "%sunion {\n", tabs);
			tab ++;
		}
	;		  	
	
DataInstanceList
	: DataInstance
		{
			$$ = $1;
		}
	| DataInstanceList DataInstance
		{
			sprintf ($$.m_literal, "%s%s", $1.m_literal, $2.m_literal);
		}
	;

DataInstance	
	: DataType literalIndentifer ';'
		{
			char tabs[256];
			GenerateTabs (tabs);
			sprintf ($$.m_literal, "%s%s %s;\n", tabs, $1.m_literal, $2.m_literal);
		}
	| DataType literalIndentifer '[' NUMERIC_CONSTANT ']' ';'  
		{
			char tabs[256];
			GenerateTabs (tabs);
			sprintf ($$.m_literal, "%s%s %s[%s];\n", tabs, $1.m_literal, $2.m_literal, $4.m_literal);
		}
	| DataType literalIndentifer '[' NUMERIC_CONSTANT ']' '[' NUMERIC_CONSTANT ']' ';'  
		{		
			char tabs[256];
			GenerateTabs (tabs);
			sprintf ($$.m_literal, "%s%s %s[%s][%s];\n", tabs, $1.m_literal, $2.m_literal, $4.m_literal, $7.m_literal);		
		}
	| StructuresDeclaration
		{
			sprintf ($$.m_literal, "%s", $1.m_literal);
		}
	;
	
DataType
	: DataInstace
		{
			$$ = $1;
		}
	| DataInstace '*'
		{
			sprintf ($$.m_literal, "%s*", $1.m_literal);
		}
	| DataInstace '*' CONST
		{
			//sprintf ($$.m_literal, "%s*", $1.m_literal);
			sprintf ($$.m_literal, "%s* const", $1.m_literal);
		}
	| DataInstace '*' '*'
		{
			sprintf ($$.m_literal, "%s**", $1.m_literal);
		}
		
	| DataInstace '*' '*' CONST
		{
			sprintf ($$.m_literal, "%s** const", $1.m_literal);
			//sprintf ($$.m_literal, "%s**", $1.m_literal);
		}
		
	;	
	
DataInstace
	: INT 
		{
			sprintf ($$.m_literal, "int");						
		}
	| CHAR 
		{
			sprintf ($$.m_literal, "char");			
		}
	| VOID 
		{
			sprintf ($$.m_literal, "void");
		}
	| FLOAT
		{
			sprintf ($$.m_literal, "float");
		}
	| UNSIGNED
		{
			sprintf ($$.m_literal, "unsigned");
		}
	| SHORT	INT		 
		{
			sprintf ($$.m_literal, "short int");
		}
	| UNSIGNED SHORT			 
		{
			sprintf ($$.m_literal, "unsigned short");
		}
	| CONST VOID			 		
		{
			sprintf ($$.m_literal, "const void");
		}
	| CONST INT
		{
			sprintf ($$.m_literal, "const int");
		}
	| CONST FLOAT
		{
			sprintf ($$.m_literal, "const float");
		}
	| literalIndentifer
		{
			sprintf ($$.m_literal, "struct i%s", $1.m_literal);
		}
	| CONST literalIndentifer
		{
			sprintf ($$.m_literal, "%s struct i%s", $1.m_literal, $2.m_literal);
		}
	;	
	
	
StructuresDeclarationEnd	
	: '}' ';'	
		{
			char tabs[256];
			tab --;
			GenerateTabs (tabs);
			sprintf ($$.m_literal, "%s};\n", tabs);
		}
	;	
	


FunctionCallBackDeclaration
	: FunctionCallBackDeclarationName '(' FunctionCallBackDeclarationParamsList ')' ';'
	  {
		sprintf ($$.m_literal, "%s (%s);\n", $1.m_literal, $3.m_literal);
	  }
	| FunctionCallBackDeclarationName '(' ')' ';'
	  {
		sprintf ($$.m_literal, "%s ();\n", $1.m_literal);
	  }
	;

FunctionCallBackDeclarationName
	: TYPEDEF DataType '(' '*' literalIndentifer ')' 
	  {
		SaveCallback ($5.m_literal);
		sprintf ($$.m_literal, "typedef %s (*i%s)", $2.m_literal, $5.m_literal);
	  }
	;

FunctionCallBackDeclarationParamsList
	: FunctionCallBackDeclarationParam
		{
			$$ = $1;
		}
	| FunctionCallBackDeclarationParamsList FunctionCallBackDeclarationParam	
		{
			sprintf ($$.m_literal, "%s%s", $1.m_literal, $2.m_literal);
		}
	;

FunctionCallBackDeclarationParam	
	: DataType literalIndentifer
	  {
		sprintf ($$.m_literal, "%s %s", $1.m_literal, $2.m_literal);
	  }
	| DataType literalIndentifer ','
	  {
		sprintf ($$.m_literal, "%s %s, ", $1.m_literal, $2.m_literal);
	  }
	;


ClassMethodName
	:literalIndentifer
		{
			if (!strncmp ($1.m_literal, "Newton", 6)) {
				sprintf ($$.m_literal, "%s", $1.m_literal + 6);
			} else {
				$$ = $1;
			}	
		}
	;

ClassMethod
	: literalIndentifer DataType ClassMethodName '(' ')' ';'
		{
			const char* callback = IsCallback ($2.m_literal);
			if (callback) {
				sprintf ($$.m_literal, "-(i%s) %s;\n", callback, $3.m_literal);
			} else {
				sprintf ($$.m_literal, "-(%s) %s;\n", $2.m_literal, $3.m_literal);
			}
		}
	| literalIndentifer DataType ClassMethodName '(' functionArgumentList ')' ';'
		{
			const char* callback = IsCallback ($2.m_literal);
			if (callback) {
				sprintf ($$.m_literal, "-(i%s) %s%s;\n", callback, $3.m_literal, $5.m_literal);
			} else {
				sprintf ($$.m_literal, "-(%s) %s%s;\n", $2.m_literal, $3.m_literal, $5.m_literal);
			}
		}
	;	

functionArgumentList
	: functionArgument
		{
			$$ = $1;
		}
	| functionArgumentList functionArgument
		{
			sprintf ($$.m_literal, "%s%s", $1.m_literal, $2.m_literal);
		}
	;

functionArgument
	: DataType literalIndentifer
	 {
		const char* callback = IsCallback ($1.m_literal);
		if (callback) {
			sprintf ($$.m_literal, ": (i%s) %s", callback, $2.m_literal);
		} else {
			sprintf ($$.m_literal, ": (%s) %s", $1.m_literal, $2.m_literal);
		}
	 }	
	| DataType literalIndentifer ','	
	 {
		const char* callback = IsCallback ($1.m_literal);
		if (callback) {
			sprintf ($$.m_literal, ": (i%s) %s", callback, $2.m_literal);
		} else {
			sprintf ($$.m_literal, ": (%s) %s", $1.m_literal, $2.m_literal);
		}
	 }	
	| DataType literalIndentifer '['']'
	 {
		sprintf ($$.m_literal, ": (%s*) %s", $1.m_literal, $2.m_literal);
	 }	
	| DataType literalIndentifer '['']' ','
	 {
		sprintf ($$.m_literal, ": (%s*) %s", $1.m_literal, $2.m_literal);
	 }	
	;

%%



