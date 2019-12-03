#include "desteer/Util.hpp"
#include <iostream>
#include <tut/tut.hpp>
namespace tut
{
	struct util_world_to_local_data{};
	typedef test_group<util_world_to_local_data> util_world_to_local_tg;
	typedef util_world_to_local_tg::object object;

	util_world_to_local_tg util_world_to_local_test_suite("World to Local Vector Transformations");

	template<>
	template<>
	void object::test<1>()
	{
	    set_test_name("with heading on postive z-axis");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(0,0,0);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(1,0,0);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(1,0,0);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<1,0,0> => <0,0,1>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}

	template<>
	template<>
	void object::test<2>()
	{
	    set_test_name("with heading on negative z-axis");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,-1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(0,0,0);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(1,0,0);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(1,0,0);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<1,0,0> => <0,0,1>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}

	template<>
	template<>
	void object::test<3>()
	{
	    set_test_name("with heading on postive z-axis with position offset (0,0,5)");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(0,0,5);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(0,0,1);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(0,0,-4);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<0,0,1> @ <0,0,5> => <0,0,-4>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}


	template<>
	template<>
	void object::test<4>()
	{
	    set_test_name("with heading on postive z-axis with position offset");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(0,0,-5);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(0,0,1);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(0,0,6);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<0,0,1> @ <0,0,-5> => <0,0,6>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}

	template<>
	template<>
	void object::test<5>()
	{
	    set_test_name("with heading on postive z-axis with position offset");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(1,0,1);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(0,0,1);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(-1,0,0);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<0,0,1> @ <1,0,1> => <-1,0,0>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}

	template<>
	template<>
	void object::test<6>()
	{
	    set_test_name("with heading on postive z-axis with position offset");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(1,0,-1);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(1,0,1);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(0,0,2);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<0,0,1> @ <1,0,-1> => <0,0,2>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}
}

namespace tut
{
	struct util_local_to_world_data{};
	typedef test_group<util_local_to_world_data> util_local_to_world_tg;
	typedef util_local_to_world_tg::object local_to_world_test;

	util_local_to_world_tg util_local_to_world_test_suite("Local to World Vector Transformations");

	template<>
	template<>
	void local_to_world_test::test<1>()
	{
	    set_test_name("with heading on postive z-axis");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(0,0,0);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(1,0,0);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(1,0,0);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<1,0,0> => <0,0,1>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}

	template<>
	template<>
	void local_to_world_test::test<2>()
	{
	    set_test_name("with heading on negative z-axis");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,-1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(0,0,0);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(1,0,0);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(1,0,0);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<1,0,0> => <0,0,1>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}

	template<>
	template<>
	void local_to_world_test::test<3>()
	{
	    set_test_name("with heading on postive z-axis with position offset (0,0,5)");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(0,0,5);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(0,0,1);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(0,0,-4);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<0,0,1> @ <0,0,5> => <0,0,-4>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}


	template<>
	template<>
	void local_to_world_test::test<4>()
	{
	    set_test_name("with heading on postive z-axis with position offset");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(0,0,-5);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(0,0,1);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(0,0,6);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<0,0,1> @ <0,0,-5> => <0,0,6>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}

	template<>
	template<>
	void local_to_world_test::test<5>()
	{
	    set_test_name("with heading on postive z-axis with position offset");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(1,0,1);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(0,0,1);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(-1,0,0);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<0,0,1> @ <1,0,1> => <-1,0,0>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}

	template<>
	template<>
	void local_to_world_test::test<6>()
	{
	    set_test_name("with heading on postive z-axis with position offset");

        //setup a local coords system with heading along positive z-axis
        irr::core::vector3df forward = irr::core::vector3df(0,0,1);
        irr::core::vector3df side = irr::core::vector3df(1,0,0);

        //position of local coords in world coords
        irr::core::vector3df pos = irr::core::vector3df(1,0,-1);

        //input for transformation
        irr::core::vector3df worldVec = irr::core::vector3df(1,0,1);

        //correct output for transformation
        irr::core::vector3df localVec = irr::core::vector3df(0,0,2);

        irr::core::vector3df transformedVec = VectorTransformer::VectorTransformWorldToLocal(worldVec,forward,side,pos);

        //ensure_equals("<0,0,1> @ <1,0,-1> => <0,0,2>", transformedVec ,localVec);
        ensure(transformedVec == localVec);
	}
}

