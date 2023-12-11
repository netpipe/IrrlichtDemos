
{
	// Could probably do with some more variant tests, but this will do for now.
	
	NOTE("Testing Variant");
	
	
	// Test variant copying
	
	{
		Variant a = Variant((bool)1);
		ASSERT( a.GetType() == Variant::EVT_BOOL );
		ASSERT( a.To<bool>() == 1 );
	}
	{
		Variant a = Variant((u32)99);
		ASSERT( a.GetType() == Variant::EVT_U32 );
		ASSERT( a.To<u32>() == 99 );
	}
	{
		Variant a = Variant((s32)99);
		ASSERT( a.GetType() == Variant::EVT_S32 );
		ASSERT( a.To<s32>() == 99 );
	}
	{
		Variant a = Variant((f32)99.99f);
		ASSERT( a.GetType() == Variant::EVT_F32 );
		ASSERT( core::equals(a.To<f32>(), 99.99f) );
	}
	{
		Variant a = Variant("lala");
		ASSERT( a.GetType() == Variant::EVT_STRING );
		ASSERT( a.To<core::stringc>() == "lala" );
	}
	
	
	// Test Variant casting
	
	// bool cast to all other types
	{
		Variant v = (bool)true;
		ASSERT( v.GetType() == Variant::EVT_BOOL );
		ASSERT( v.To<bool>() == true );
		ASSERT( v.To<u32>() == 1 );
		ASSERT( v.To<s32>() == 1 );
		ASSERT( core::equals(v.To<f32>(), 1.f) );
		ASSERT( v.To<core::stringc>() == "1" );
	}
	// u32 cast to all other types
	{
		Variant v = (u32)99;
		ASSERT( v.GetType() == Variant::EVT_U32 );
		ASSERT( v.To<bool>() == true );
		ASSERT( v.To<u32>() == 99 );
		ASSERT( v.To<s32>() == 99 );
		ASSERT( core::equals(v.To<f32>(), 99.f) );
		ASSERT( v.To<core::stringc>() == "99" );
	}
	// s32 cast to all other types
	{
		Variant v = (s32)99;
		ASSERT( v.GetType() == Variant::EVT_S32 );
		ASSERT( v.To<bool>() == true );
		ASSERT( v.To<u32>() == 99 );
		ASSERT( v.To<s32>() == 99 );
		ASSERT( core::equals(v.To<f32>(), 99.f) );
		ASSERT( v.To<core::stringc>() == "99" );
	}
	// f32 cast to all other types
	{
		Variant v = (f32)99.f;
		ASSERT( v.GetType() == Variant::EVT_F32 );
		ASSERT( v.To<bool>() == true );
		ASSERT( v.To<u32>() == 99 );
		ASSERT( v.To<s32>() == 99 );
		ASSERT( core::equals(v.To<f32>(), 99.f) );
		ASSERT( v.To<core::stringc>() == "99.0" );
	}
	// string cast to all other types (1)
	{
		Variant v = "lala";
		ASSERT( v.GetType() == Variant::EVT_STRING );
		ASSERT( v.To<bool>() == false );
		ASSERT( v.To<u32>() == 0 );
		ASSERT( v.To<s32>() == 0 );
		ASSERT( core::equals(v.To<f32>(), 0.f) );
		ASSERT( v.To<core::stringc>() == "lala" );
	}
	// string cast to all other types (2)
	{
		Variant v = "99.0";
		ASSERT( v.GetType() == Variant::EVT_STRING );
		ASSERT( v.To<bool>() == true);
		ASSERT( v.To<u32>() == 99 );
		ASSERT( v.To<s32>() == 99 );
		ASSERT( core::equals(v.To<f32>(), 99.f) );
		ASSERT( v.To<core::stringc>() == "99.0" );
	}
	
	
	// Test Variant comparisons
	
	// bool compared to all other types
	{
		Variant v = (bool)true;
		ASSERT( v == (bool)true );
		ASSERT( v == (u32)1 );
		ASSERT( v == (s32)1 );
		ASSERT( v == (f32)1.f );
		ASSERT( v == "1" );
	}
	// u32 compared to all other types
	{
		Variant v = (u32)99;
		ASSERT( v == (bool)true );
		ASSERT( v == (u32)99 );
		ASSERT( v == (s32)99 );
		ASSERT( v == (f32)99.f );
		ASSERT( v == "99" );
	}
	// s32 compared to all other types
	{
		Variant v = (s32)99;
		ASSERT( v == (bool)true );
		ASSERT( v == (u32)99 );
		ASSERT( v == (s32)99 );
		ASSERT( v == (f32)99.f );
		ASSERT( v == "99" );
	}
	// f32 compared to all other types
	{
		Variant v = (f32)99.123;
		ASSERT( v == (bool)true );
		ASSERT( v == (u32)99 );
		ASSERT( v == (s32)99 );
		ASSERT( v == (f32)99.123f );
		ASSERT( v == "99.123001" );
	}
	// string compared to all other types (1)
	{
		Variant v = "lala";
		ASSERT( v == (bool)false );
		ASSERT( v == (u32)0 );
		ASSERT( v == (s32)0 );
		ASSERT( v == (f32)0.f );
		ASSERT( v == "lala" );
	}
	// string compared to all other types (2)
	{
		Variant v = "99.123";
		ASSERT( v == (bool)true );
		ASSERT( v == (u32)99 );
		ASSERT( v == (s32)99 );
		ASSERT( v == (f32)99.123f );
		ASSERT( v == "99.123" );
	}
	
	
	// Test variant addition.
	
	// bool with others added
	{
		ASSERT( Variant((bool)false) + (bool)true == (bool)true );
		
		ASSERT( Variant((bool)false) + (u32)99 == (bool)true );
		ASSERT( Variant((bool)false) + (u32)0 == (bool)false );
		
		ASSERT( Variant((bool)false) + (s32)99 == (bool)true );
		
		ASSERT( Variant((bool)false) + (f32)99 == (bool)true );
		ASSERT( Variant((bool)false) + (f32)0 == (bool)false );
		
		ASSERT( Variant((bool)false) + (f64)99 == (bool)true );
		ASSERT( Variant((bool)false) + (f64)0 == (bool)false );
		
		ASSERT( Variant((bool)false) + "true" == (bool)true );
		ASSERT( Variant((bool)false) + "false" == (bool)false );
	}
	// u32 with others added
	{
		ASSERT( Variant((u32)99) + (bool)false == (u32)99 );
		ASSERT( Variant((u32)99) + (bool)true == (u32)100 );
		
		ASSERT( Variant((u32)99) + (u32)0 == (u32)99 );
		ASSERT( Variant((u32)99) + (u32)100 == (u32)199 );
		
		ASSERT( Variant((u32)99) + (s32)0 == (u32)99 );
		ASSERT( Variant((u32)99) + (s32)100 == (u32)199 );
		
		ASSERT( Variant((u32)99) + (f32)0 == (u32)99 );
		ASSERT( Variant((u32)99) + (f32)10.5 == (u32)109 );
		
		ASSERT( Variant((u32)99) + (f64)0 == (u32)99 );
		ASSERT( Variant((u32)99) + (f64)10.5 == (u32)109 );
		
		ASSERT( Variant((u32)99) + "0" == (u32)99 );
		ASSERT( Variant((u32)99) + "100" == (u32)199 );
	}
	// s32 with others added
	{
		ASSERT( Variant((s32)-99) + (bool)false == (s32)-99 );
		ASSERT( Variant((s32)-99) + (bool)true == (s32)-98 );
		
		ASSERT( Variant((s32)-99) + (u32)0 == (s32)-99 );
		ASSERT( Variant((s32)-99) + (u32)100 == (s32)1 );
		
		ASSERT( Variant((s32)-99) + (s32)0 == (s32)-99 );
		ASSERT( Variant((s32)-99) + (s32)100 == (s32)1 );
		
		ASSERT( Variant((s32)-99) + (f32)0 == (s32)-99 );
		ASSERT( Variant((s32)-99) + (f32)10.5 == (s32)-89 );
		
		ASSERT( Variant((s32)-99) + (f64)0 == (s32)-99 );
		ASSERT( Variant((s32)-99) + (f64)10.5 == (s32)-89 );
		
		ASSERT( Variant((s32)-99) + "0" == (s32)-99 );
		ASSERT( Variant((s32)-99) + "-100" == (s32)-199 );
	}
	// f32 with others added
	{
		ASSERT( Variant((f32)-99.5) + (bool)false == (f32)-99.5 );
		ASSERT( Variant((f32)-99.5) + (bool)true == (f32)-98.5 );
		
		ASSERT( Variant((f32)-99.5) + (u32)0 == (f32)-99.5 );
		ASSERT( Variant((f32)-99.5) + (u32)100 == (f32)0.5 );
		
		ASSERT( Variant((f32)-99.5) + (s32)0 == (f32)-99.5 );
		ASSERT( Variant((f32)-99.5) + (s32)-100 == (f32)-199.5 );
		
		ASSERT( Variant((f32)-99.5) + (f32)0 == (f32)-99.5 );
		ASSERT( Variant((f32)-99.5) + (f32)-100.5 == (f32)-200.f );
		
		ASSERT( Variant((f32)-99.5) + (f64)0 == (f32)-99.5 );
		ASSERT( Variant((f32)-99.5) + (f64)-100.5 == (f32)-200.f );
		
		ASSERT( Variant((f32)-99.5) + "0" == (f32)-99.5 );
		ASSERT( Variant((f32)-99.5) + "-100.5" == (f32)-200.f );
	}
	// f64 with others added
	{
		ASSERT( Variant((f64)-99.5) + (bool)false == (f64)-99.5 );
		ASSERT( Variant((f64)-99.5) + (bool)true == (f64)-98.5 );
		
		ASSERT( Variant((f64)-99.5) + (u32)0 == (f64)-99.5 );
		ASSERT( Variant((f64)-99.5) + (u32)100 == (f64)0.5 );
		
		ASSERT( Variant((f64)-99.5) + (s32)0 == (f64)-99.5 );
		ASSERT( Variant((f64)-99.5) + (s32)-100 == (f64)-199.5 );
		
		ASSERT( Variant((f64)-99.5) + (f32)0 == (f64)-99.5 );
		ASSERT( Variant((f64)-99.5) + (f32)-100.5 == (f64)-200.f );
		
		ASSERT( Variant((f64)-99.5) + (f64)0 == (f64)-99.5 );
		ASSERT( Variant((f64)-99.5) + (f64)-100.5 == (f64)-200.f );
		
		ASSERT( Variant((f64)-99.5) + "0" == (f64)-99.5 );
		ASSERT( Variant((f64)-99.5) + "-100.5" == (f64)-200.f );
	}
	// string with others added
	{
		ASSERT( Variant("lala") + (bool)false == "lala0" );
		ASSERT( Variant("lala") + (bool)true == "lala1" );
		
		ASSERT( Variant("lala") + (u32)0 == "lala0" );
		ASSERT( Variant("lala") + (u32)99 == "lala99" );
		
		ASSERT( Variant("lala") + (s32)0 == "lala0" );
		ASSERT( Variant("lala") + (s32)-99 == "lala-99" );
		
		ASSERT( Variant("lala") + (f32)0 == "lala0.0" );
		
		ASSERT( Variant("lala") + (f32)-100.5 == "lala-100.5" );
		
		ASSERT( Variant("lala") + (f64)0 == "lala0.0" );
		ASSERT( Variant("lala") + (f64)-100.5 == "lala-100.5" );
		
		ASSERT( Variant("lala") + "!?" == "lala!?" );
		ASSERT( Variant("lala") + core::stringc("!?") == "lala!?" );
	}
	
	
	
	// Some random other tests...
	
	
	// Test conversion from all types to string
	
	ASSERT( Variant((bool)0).To<core::stringc>() == "0" );
	ASSERT( Variant((bool)1).To<core::stringc>() == "1" );
	ASSERT( Variant((u32)99).To<core::stringc>() == "99" );
	ASSERT( Variant((s32)99).To<core::stringc>() == "99" );
	ASSERT( Variant((s32)-99).To<core::stringc>() == "-99" );
	ASSERT( Variant((f32)99.f).To<core::stringc>() == "99.0" );
	ASSERT( Variant((f32)99.123f).To<core::stringc>() == "99.123001" );
	ASSERT( Variant((f32)-99.123f).To<core::stringc>() == "-99.123001" );
	ASSERT( Variant("abcdefg").To<core::stringc>() == "abcdefg" );
	ASSERT( Variant(core::stringc("abcdefg")).To<core::stringc>() == "abcdefg" );
	
	
	// Test conversion from string to all types
	
	ASSERT( Variant("true").To<bool>() == true );
	ASSERT( Variant("tRuE").To<bool>() == true );
	ASSERT( Variant("trueaaaa").To<bool>() == true );
	ASSERT( Variant("atrue").To<bool>() == false );
	ASSERT( Variant("false").To<bool>() == false );
	ASSERT( Variant("sdfhsdkjg").To<bool>() == false );
	ASSERT( Variant("").To<bool>() == false );
	
	ASSERT( Variant("0....!?").To<u32>() == 0 );
	ASSERT( Variant("99").To<u32>() == 99 );
	
	ASSERT( Variant("-99 lala").To<s32>() == -99 );
	ASSERT( Variant("99").To<s32>() == 99 );
	
	ASSERT( Variant("0.1").To<f32>() == 0.1f );
	ASSERT( Variant("123.456    ").To<f32>() == 123.456f );
	ASSERT( Variant("-123.456").To<f32>() == -123.456f );
	
	ASSERT( Variant("lala").To<core::stringc>() == "lala" );
	ASSERT( Variant("").To<core::stringc>() == "" );
}
