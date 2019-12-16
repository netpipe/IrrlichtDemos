// ==========================================================
// FreeImage 3 .NET wrapper
// Original FreeImage 3 functions and .NET compatible derived functions
//
// Design and implementation by
// - Jean-Philippe Goerke (jpgoerke@users.sourceforge.net)
// - Carsten Klein (cklein05@users.sourceforge.net)
//
// Contributors:
// - David Boland (davidboland@vodafone.ie)
//
// Main reference : MSDN Knowlede Base
//
// This file is part of FreeImage 3
//
// COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
// THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
// OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
// CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
// THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
// SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
// PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
// THIS DISCLAIMER.
//
// Use at your own risk!
// ==========================================================

// ==========================================================
// CVS
// $Revision: 1.1 $
// $Date: 2007/11/28 15:33:39 $
// $Id: FREE_IMAGE_LOAD_FLAGS.cs,v 1.1 2007/11/28 15:33:39 cklein05 Exp $
// ==========================================================

namespace FreeImageAPI
{
	/// <summary>
	/// Flags used in load functions.
	/// </summary>
	[System.Flags]
	public enum FREE_IMAGE_LOAD_FLAGS
	{
		/// <summary>
		/// Default option for all types.
		/// </summary>
		DEFAULT = 0,
		/// <summary>
		/// Load the image as a 256 color image with ununsed palette entries, if it's 16 or 2 color.
		/// </summary>
		GIF_LOAD256 = 1,
		/// <summary>
		/// 'Play' the GIF to generate each frame (as 32bpp) instead of returning raw frame data when loading.
		/// </summary>
		GIF_PLAYBACK = 2,
		/// <summary>
		/// Convert to 32bpp and create an alpha channel from the AND-mask when loading.
		/// </summary>
		ICO_MAKEALPHA = 1,
		/// <summary>
		/// Load the file as fast as possible, sacrificing some quality.
		/// </summary>
		JPEG_FAST = 0x0001,
		/// <summary>
		/// Load the file with the best quality, sacrificing some speed.
		/// </summary>
		JPEG_ACCURATE = 0x0002,
		/// <summary>
		/// load separated CMYK "as is" (use | to combine with other load flags).
		/// </summary>
		JPEG_CMYK = 0x0004,
		/// <summary>
		/// Load the bitmap sized 768 x 512.
		/// </summary>
		PCD_BASE = 1,
		/// <summary>
		/// Load the bitmap sized 384 x 256.
		/// </summary>
		PCD_BASEDIV4 = 2,
		/// <summary>
		/// Load the bitmap sized 192 x 128.
		/// </summary>
		PCD_BASEDIV16 = 3,
		/// <summary>
		/// Avoid gamma correction.
		/// </summary>
		PNG_IGNOREGAMMA = 1,
		/// <summary>
		/// If set the loader converts RGB555 and ARGB8888 -> RGB888.
		/// </summary>
		TARGA_LOAD_RGB888 = 1,
		/// <summary>
		/// Reads tags for separated CMYK.
		/// </summary>
		TIFF_CMYK = 0x0001
	}
}