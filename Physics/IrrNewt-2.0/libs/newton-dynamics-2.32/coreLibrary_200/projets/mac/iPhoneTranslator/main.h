// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


extern FILE* Newtonin;
struct LexicalData
{
	char m_literal[1024 * 4];
};

int main(int argc, char* argv[]);

int Newtonlex ();
int Newtonparse ();
void SaveCallback (const char* name);
const char* IsCallback (const char* name);

void PrintHeader (const char* name);
void PrintDefine (const char* name);
void PrintStrutureDefine (const char* name);
void PrintDataStrutures (const char* name);
void PrintFunctionCallbacks (const char* name);
void PrintClassMethod (const char* name);


void XXXX ();

void CopyLable (char* const dst, const char* const lable);

