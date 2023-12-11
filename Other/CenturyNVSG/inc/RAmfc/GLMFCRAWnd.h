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
#ifdef _WIN32
#include "nvui/GLWinRenderArea.h"

/** \file */

//  Don't need to document the API specifier
#if ! defined( DOXYGEN_IGNORE )
#ifdef RAMFC_EXPORTS
# define RAMFC_API __declspec(dllexport)
#else
# define RAMFC_API __declspec(dllimport)
#endif
#endif  //  DOXYGEN_IGNORE


namespace nvui
{
  //! OpenGL render area window. 
  /** This class adds MFC CWnd functionality to the MS Windows specific OpenGL RenderArea.
      Use one of CWnd's Create()/CreateEx() functions to create the window and then call init() to initialize
      the render area. */
  class RAMFC_API GLMFCRAWnd :  public CWnd, public GLWinRenderArea
  {
    public:
      /*! \brief virtual declared destructor.
       * \remarks The implementation does nothing. */
      virtual ~GLMFCRAWnd(void);
     
      /*! \brief Overrides RenderArea::init to perform a specialized initialization.
       * \param shareArea Specifies an optional RenderArea to share 'sharable' objects, 
       * like display lists, with. 
       * \return \c true if initialization completed successfully. \c false otherwise 
       * \remarks The function attaches the window inherited from the CWnd base class
       * to the GLWinRenderArea. After that it calls the GLWinRenderArea::init 
       * implementation to perform necessary OpenGL initialization. */
      virtual bool init(RenderArea * shareArea = NULL);

      afx_msg BOOL  OnEraseBkgnd( CDC* pDC ); //!< Do nothing in here to avoid flickering.
      afx_msg void  OnPaint( void );          //!< Do nothing other than get the CPaintDC and leave.
            
      /*! \brief Overrides RenderArea::setViewportSize.
       * \param x Specifies the new width in pixels
       * \param y Specifies the new height in pixels
       * \remarks The function calls the GLWinRenderArea::setViewportSize implementation 
       * and broadcasts the resulting new window size to the CWnd base class object. */
      virtual void setViewportSize(size_t x, size_t y);
      
      /*! \brief Overrides RenderArea::destroy.
       * \remarks The function calls the GLWinRenderArea::destroy implementation 
       * and after that destroys the window inherited from the CWnd base class. */
      virtual void destroy();
      
      /*! \brief Overrides RenderArea::triggerRedraw.
       * \remarks The function enqueues an update of the client area of the window 
       * inherited from the CWnd base class. */
      virtual void triggerRedraw();

    protected:
      /*! \brief Overrides CWnd::PreCreateWindow.
       * \remarks The function specifies some additional creation flags necessary for OpenGL 
       * enabled windows, and then calls the CWnd::PreCreateWindow implementation. */
      virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

      DECLARE_MESSAGE_MAP()
  };

} //  namespace nvui


#endif //_WIN32
