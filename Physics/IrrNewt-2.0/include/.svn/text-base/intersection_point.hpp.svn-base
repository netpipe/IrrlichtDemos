// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_LIB_INTERSECTION_POINT_HPP_16516816ASFAVAEV51651651ASAVASDFASD
#define IRRNEWT_LIB_INTERSECTION_POINT_HPP_16516816ASFAVAEV51651651ASAVASDFASD
#pragma once

#include "dll_exports.hpp"

namespace irr{
	namespace newton{

		class IBody;

		//!\brief This class is used to store ray cast result info.
		//!
		//!When you shot a line 
		//!and you will check for 
		//!collision, if IrrNewt check a collision store collision informations 
		//!(not only the point) about the intersection trought this struct
		struct SIntersectionPoint {

			SIntersectionPoint():body(NULL) {}

			//copy contructor
			SIntersectionPoint(const SIntersectionPoint& p):point(p.point),normals(p.normals),
				body(p.body),parametric_value(p.parametric_value),face_id(p.face_id) {}

			//!the output point in 3 dimension space
			irr::core::vector3df point;

			//!the normal of the intersection point
			irr::core::vector3df normals;

			//!the body with the line collide
			irr::newton::IBody* body;

			//!the parametric value
			float parametric_value;

			//!The identifier of the face of the body wich the line collide with
			int face_id;
		};
	}
}

#endif
