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

#include "nvsg/nvsgapi.h"  // storage-class defines
#include <string>
#include <list>

namespace nvutil 
{
  //! Class that holds the problem information
  /** This class holds the problem information that is logged in the
    * ProblemLog class.*/
  class ProblemInfo 
  {
  public:
    //! Constructor to construct a valid problem info.
    ProblemInfo( const char * problem  //!< Catchword, like the name of a missing extension
               , bool isCritical       //!< Yes means critical problem
               , const char * missingFeatures //!< Longer description on what feature(s) will be missing
               ) 
      : m_problem(problem), m_isCritical(isCritical), m_missingFeatures(missingFeatures) {}
    ~ProblemInfo() {}

    std::string m_problem;          //!< catchword
    bool        m_isCritical;       //!< severity
    std::string m_missingFeatures;  //!< explanation
  };

  class LogListCompare
  {
  public:
    LogListCompare() {}
    ~LogListCompare() {}

    bool operator()( const ProblemInfo & infoA, const ProblemInfo & infoB ) const;
  };
  
  //!< Structure of the problem list
  typedef std::list<ProblemInfo> ProblemLogList;

  //!< This class logs problems
  class ProblemLog
  {
  public:
    ProblemLog() {}
    ~ProblemLog() {}

    //!< Add a new problem to the problems log
    void addProblem( const ProblemInfo & info  //!< Valid Problem information
                   );

    //!< Get the problem log
    /** Use the returned log to analyse problems and missing features, ...
      * \return Problem log list, if there is no problem the log list does not contain entries.*/
    const ProblemLogList & getLogList();
    
  private:
    ProblemLogList m_log;
  };

  //////////////////////////////////////////////////////////////////////////
  
  inline void ProblemLog::addProblem(const ProblemInfo & info)
  {
    m_log.push_back(info);
  }

  inline const ProblemLogList & ProblemLog::getLogList()
  { 
    m_log.sort(LogListCompare());
    return m_log; 
  }

  inline bool LogListCompare::operator()( const ProblemInfo & infoA, const ProblemInfo & infoB ) const
  {
      return(infoA.m_problem < infoB.m_problem);
  }
}