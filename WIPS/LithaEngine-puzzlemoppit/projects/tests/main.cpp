
#include "Litha.h"

// A very basic test system, simply using ASSERT.
// So as soon as one assertion fails, the tests will stop.
// Results are logged to "tests.log"

int main(int argc, const char **argv)
{
	utils::log::setfile("tests.log");
	NOTE("RUNNING TESTS...");
	
#include "test_str.h"
#include "test_Variant.h"
	
	NOTE("All tests passed!");
	
	return 0;
}

