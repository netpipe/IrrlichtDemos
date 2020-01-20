
{
	NOTE("Testing utils::str");
	
	
	// Trim functions
	
	ASSERT( str::rtrim("") == "" );
	ASSERT( str::rtrim("lala") == "lala" );
	ASSERT( str::rtrim("lala \r\n\t") == "lala" );
	ASSERT( str::rtrim("la la \r\n\t") == "la la" );
	ASSERT( str::rtrim("  \r\n\t    lala") == "  \r\n\t    lala" );
	ASSERT( str::rtrim("1.01000", "0") == "1.01" );
	
	ASSERT( str::ltrim("  \r\n\t    lala") == "lala" );
	ASSERT( str::ltrim("la la") == "la la" );
	ASSERT( str::ltrim(" a") == "a" );
	ASSERT( str::ltrim("  \r\n\t") == "" );
	ASSERT( str::ltrim("") == "" );
	
	ASSERT( str::trim("  \r\n\t    lala  \r\n\t    ") == "lala" );
	ASSERT( str::trim("  \r\n\t    la\r\nla  \r\n\t    ") == "la\r\nla" );
	ASSERT( str::trim("lala") == "lala" );
	ASSERT( str::trim("") == "" );
	
	
	// Explode
	{
		std::vector<core::stringc> results = str::explode("", "");
		ASSERT( results.size() == 0 );
	}
	{
		std::vector<core::stringc> results = str::explode(" ", "");
		ASSERT( results.size() == 0 );
	}
	{
		std::vector<core::stringc> results = str::explode("", "a b c");
		ASSERT( results.size() == 1 );
		ASSERT( results[0] == "a b c" );
	}
	{
		std::vector<core::stringc> results = str::explode("la", "a b c");
		ASSERT( results.size() == 1 );
		ASSERT( results[0] == "a b c" );
	}
	{
		std::vector<core::stringc> results = str::explode(" ", "a b c");
		ASSERT( results.size() == 3 );
		ASSERT( results[0] == "a" );
		ASSERT( results[1] == "b" );
		ASSERT( results[2] == "c" );
	}
	{
		std::vector<core::stringc> results = str::explode(" ", "   a   b   c  ");
		ASSERT( results.size() == 3 );
		ASSERT( results[0] == "a" );
		ASSERT( results[1] == "b" );
		ASSERT( results[2] == "c" );
	}
	{
		std::vector<core::stringc> results = str::explode("123", "123123a123123b123c123123");
		ASSERT( results.size() == 3 );
		ASSERT( results[0] == "a" );
		ASSERT( results[1] == "b" );
		ASSERT( results[2] == "c" );
	}
	{
		std::vector<core::stringc> results = str::explode("=", "something=99");
		ASSERT( results.size() == 2 );
		ASSERT( results[0] == "something" );
		ASSERT( results[1] == "99" );
	}
	
	
	// Explode by random charlist
	{
		std::vector<core::stringc> results = str::explode_chars("", "");
		ASSERT( results.size() == 0 );
	}
	{
		std::vector<core::stringc> results = str::explode_chars(" ", "");
		ASSERT( results.size() == 0 );
	}
	{
		std::vector<core::stringc> results = str::explode_chars("", "a b c");
		ASSERT( results.size() == 1 );
		ASSERT( results[0] == "a b c" );
	}
	{
		std::vector<core::stringc> results = str::explode_chars("123", "a b c");
		ASSERT( results.size() == 1 );
		ASSERT( results[0] == "a b c" );
	}
	{
		std::vector<core::stringc> results = str::explode_chars(" -,", "a,,,b--c-d,e f g");
		ASSERT( results.size() == 7 );
		ASSERT( results[0] == "a" );
		ASSERT( results[1] == "b" );
		ASSERT( results[2] == "c" );
		ASSERT( results[3] == "d" );
		ASSERT( results[4] == "e" );
		ASSERT( results[5] == "f" );
		ASSERT( results[6] == "g" );
	}
	{
		std::vector<core::stringc> results = str::explode_chars(" -,", "   a,,,b--c-d,e f g,");
		ASSERT( results.size() == 7 );
		ASSERT( results[0] == "a" );
		ASSERT( results[1] == "b" );
		ASSERT( results[2] == "c" );
		ASSERT( results[3] == "d" );
		ASSERT( results[4] == "e" );
		ASSERT( results[5] == "f" );
		ASSERT( results[6] == "g" );
	}
	{
		std::vector<core::stringc> results = str::explode_chars("\r\n", "\n\rone\r\ntwo\r\r\n\nthree");
		ASSERT( results.size() == 3 );
		ASSERT( results[0] == "one" );
		ASSERT( results[1] == "two" );
		ASSERT( results[2] == "three" );
	}
	{
		std::vector<core::stringc> results = str::explode_chars("\r\n", "one\r\ntwo\r\r\n\nthree\r\n");
		ASSERT( results.size() == 3 );
		ASSERT( results[0] == "one" );
		ASSERT( results[1] == "two" );
		ASSERT( results[2] == "three" );
	}
	{
		std::vector<core::stringc> results = str::explode_chars("\r\n", "\na");
		ASSERT( results.size() == 1 );
		ASSERT( results[0] == "a" );
	}
	{
		std::vector<core::stringc> results = str::explode_chars("\r\n", "a\n");
		ASSERT( results.size() == 1 );
		ASSERT( results[0] == "a" );
	}
	
	
	// Conversion to functions
	
	ASSERT( str::to((bool)true) == "1" );
	ASSERT( str::to((bool)false) == "0" );
	
	ASSERT( str::to((u32)111) == "111" );
	ASSERT( str::to((u32)123.456) == "123" );
	
	ASSERT( str::to((s32)111) == "111" );
	ASSERT( str::to((s32)-111) == "-111" );
	
	ASSERT( str::to((f32)111.f) == "111.0" );
	ASSERT( str::to((f32)111) == "111.0" );
	ASSERT( str::to((f32)-123.456) == "-123.456001" );
	
	
	// Conversion from functions
	
	ASSERT( str::from_bool("true") == true );
	ASSERT( str::from_bool("trueaaa") == true );
	ASSERT( str::from_bool("tRuEaaa") == true );
	ASSERT( str::from_bool("false") == false );
	ASSERT( str::from_bool("sdjghskjdgkjsdhg") == false );
	ASSERT( str::from_bool("") == false );
	
	ASSERT( str::from_u32("0 lala") == (u32)0 );
	ASSERT( str::from_u32("99") == (u32)99 );
	ASSERT( str::from_u32("-99") == (u32)-99 );
	ASSERT( str::from_u32("1.2") == (u32)1 );
	ASSERT( str::from_u32("") == (u32)0 );
	ASSERT( str::from_u32("lala") == (u32)0 );
	
	ASSERT( str::from_s32("0 lala") == (s32)0 );
	ASSERT( str::from_s32("-99 lala") == (s32)-99 );
	ASSERT( str::from_s32("99.1") == (s32)99 );
	ASSERT( str::from_s32("") == (s32)0 );
	ASSERT( str::from_s32("lala") == (s32)0 );
	
	ASSERT( str::from_f32("123") == (f32)123.f );
	ASSERT( str::from_f32("123.456") == (f32)123.456f );
	ASSERT( str::from_f32("-123.456") == (f32)-123.456f );
}

