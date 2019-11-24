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

#include "nvsgcommon.h"

#include <string>

namespace nvsg
{
// forward declaration
class NVSGException;
/** A NVSG error handler routine is of this type */
typedef void NVSGERRORHANDLER(const NVSGException&);

/** Global pointer to the currently used NVSG error handler routine. Use the 
  * nvsgSetErrorHandler function to assign a new error handler routine .
  */
extern NVSGERRORHANDLER * nvsgErrorHandler;
/** Replace the current NVSG error handler by newErrorHandler 
  *
  * Parameters
  * - newErrorHandler
  *     The replacement for the currently used NVSG error handler routine.
  *
  * Return Value
  *   A pointer to the formerly used error handler routine.
  */
NVSGERRORHANDLER * nvsgSetErrorHandler(NVSGERRORHANDLER * newErrorHandler); 

/** Exception base class. Derive from this class if you intend to provide more specific exceptions. */


class NVSGException
{
public:
  /** Constructs a NVSGExeption object
    *
    * Parameters
    * - critical
    *     Assigns whether this will be treated as a critical or noncritical exception.
    * - errMsg
    *     The related error message to be displayed with subsequent error handling.
    */
  NVSGException(bool critical, const std::string& errMsg) : m_critical(critical), m_errMsg(errMsg) {}
  /** Constructs a NVSGExeption object
    *
    * Parameters
    * - critical
    *     Assigns whether this will be treated as a critical or noncritical exception.
    * - errMsgID
    *     Identifies a string resource whithin a string table that holds the related
    *     error message to be displayed with subsequent error handling.
    * 
    * Note
    *   This is not implemented yet. Use the NVSGException(bool, const string&) variant
    *   instead.
    */
  NVSGException(bool critical, unsigned int errMsgID) {}
  virtual ~NVSGException() {}

  /** Tests whether this exception is critical to the execution of
    * subsequent code. For example, an error handler routine can query 
    * the critical status of an exception, and then decide to either 
    * throw the exception (if it is critical) or pop up a message box
    * and then resume execution.
    *
    * Return value
    *   \c true if this is a critical exception and execution of subsequent
    *   code is not possible; \c false if this exception is noncritical.
    */
  virtual bool isCritical() const;
  /** Receive the exception's error message.
    * 
    * Return value
    *   A reference to a const string object holding the exception's error message.
    */
  virtual const std::string& getErrorMessage() const;

private:
  bool m_critical; //!< holds the critical status
  std::string m_errMsg; //!< the exception's error message
};

// inline function implementation

inline bool NVSGException::isCritical() const 
{ 
  return m_critical; 
}

inline const std::string& NVSGException::getErrorMessage() const 
{ 
  return m_errMsg; 
}  

}
