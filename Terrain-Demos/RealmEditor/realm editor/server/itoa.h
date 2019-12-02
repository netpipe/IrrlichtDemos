/**
	
 * C++ version char* style "itoa":
	takes an int and turns it into a character array
 */	
char* itoa( int value, char* result, int base ) {
	
	// check that the base if valid
	
	if (base < 2 || base > 16) { *result = 0; return result; }
	

	
	char* out = result;
	
	int quotient = value;
	

	
	do {
	
		*out = "0123456789abcdef"[ std::abs( quotient % base ) ];
	
		++out;
	
		quotient /= base;
	
	} while ( quotient );
	

	
	// Only apply negative sign for base 10
	
	if ( value < 0 && base == 10) *out++ = '-';
	

	
	std::reverse( result, out );
	
	*out = 0;
	
	return result;
	
}
