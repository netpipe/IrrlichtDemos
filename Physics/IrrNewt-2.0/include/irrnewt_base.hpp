// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_BASE_HPP_181236ASDGDAS
#define IRRNEWT_BASE_HPP_181236ASDGDAS
#pragma once

#include "dll_exports.hpp"
#include "structures.hpp"

namespace irr{
	namespace newton{

		class IWorld;

		namespace Hidden {
			void DestroyAll();
			void ClearObjsList();
		}

		//!This is a base abstract class. All IrrNewt classes is derived from this class
		class IRRNEWT_API IrrNewt_Base{

		public:
			IrrNewt_Base();

		protected:

			virtual void reserved_destroy() = 0;

			//return true if 'delete this' should be called to delete the node,
			//return false won't call 'delete' on this object to delete it but only reserved_destroy()
			virtual bool call_delete() {return true;}

			SClassType class_type;

			friend void irr::newton::Hidden::DestroyAll();
			friend void irr::newton::Hidden::ClearObjsList();
			friend class irr::newton::IWorld;

		};
	}
}

#endif
