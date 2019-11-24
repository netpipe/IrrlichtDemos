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
#include "nvui/DX9WinRenderArea.h"

/** \file */

#ifdef RAMFC_EXPORTS
# define RAMFC_API __declspec(dllexport)
#else
# define RAMFC_API __declspec(dllimport)
#endif


namespace nvui
{
  //! DirectX 9 render area window. 
  /** This class adds MFC CWnd functionality to the MS Windows specific DirectX 9 RenderArea.
      Use one of CWnd's Create()/CreateEx() functions to create the window and then call init() to initialize
      the render area. */
  class DX9MFCRAWnd :  public CWnd, public DX9WinRenderArea
  {
    public:
      //! Default constructor
      RAMFC_API DX9MFCRAWnd(void);
      //! Default destructor
      RAMFC_API virtual ~DX9MFCRAWnd(void);
     
      //! Create the render area.
      /** This method creates and initializes the render area.
        * \return true - everything went fine */
      RAMFC_API virtual bool init( RenderArea * shareArea = NULL  //!< Pointer to a render area with which to share display lists, textures, vertex programs, etc.
                                   );

      RAMFC_API afx_msg BOOL  OnEraseBkgnd( CDC* pDC ); //!< Do nothing in here to avoid flickering.
      RAMFC_API afx_msg void  OnPaint( void );          //!< Do nothing other than get the CPaintDC and leave.
            
      //! Set the size of the window.
      /** MFC window manager specific wrapper method.
        * \return true - everything went fine.*/
      RAMFC_API virtual void setViewportSize( size_t x    //!< width in pixels
                                            , size_t y    //!< height in pixels
                                            );
      
      //! Destroy this window.
      /** MFC window manager specific wrapper method.
        * \return true - everything went fine.*/
      RAMFC_API virtual void destroy();
      
      //! Trigger a redraw.
      /** MFC window manager specific wrapper method. */
      RAMFC_API virtual void triggerRedraw();

    protected:
      RAMFC_API virtual BOOL  PreCreateWindow( CREATESTRUCT& cs ); //!< DX9 specific base stuff.

      DECLARE_MESSAGE_MAP()
  };

} //  namespace nvui


#endif //_WIN32
