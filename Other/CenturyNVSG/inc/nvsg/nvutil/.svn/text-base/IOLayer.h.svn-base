// Copyright NVIDIA Corporation 2002-2005
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once
/** \file */

#include <string>

#include "nvsgcommon.h"

namespace nvutil
{
  //! IO namespace
  /** The IO namespace has been introduced as an additional layer to maintain platform independency for
    * input/output related functionality.
    */
  namespace IO
  {
    //! Pop up an error message
    /** This function pops up a dialog showing the error message \a msg. The dialog box will show \ a caption as caption.
      * The boolean parameter \a resume forces a visual feedback for whether subsequent code will be resumed or not.
      *
      * \note This function is only implemented for win32 and win64 systems by now.
      */
    NVSG_API void errorMessage( const std::string& caption //!< The dialog's caption string
                              , const std::string& msg     //!< The error message string.
                              , bool resume           //!< \c true if subsequent code will be resumed, false otherwise.
                              );
  }
}
