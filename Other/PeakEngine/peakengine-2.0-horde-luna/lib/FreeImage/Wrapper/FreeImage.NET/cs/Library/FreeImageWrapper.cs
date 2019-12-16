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
// $Revision: 1.6 $
// $Date: 2008/06/17 13:50:59 $
// $Id: FreeImageWrapper.cs,v 1.6 2008/06/17 13:50:59 cklein05 Exp $
// ==========================================================

using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using FreeImageAPI.IO;
using FreeImageAPI.Metadata;

namespace FreeImageAPI
{
	/// <summary>
	/// Static class importing functions from the FreeImage library
	/// and providing additional functions.
	/// </summary>
	public static partial class FreeImage
	{
		#region Constants

		/// <summary>
		/// Array containing all 'FREE_IMAGE_MDMODEL's.
		/// </summary>
		public static readonly FREE_IMAGE_MDMODEL[] FREE_IMAGE_MDMODELS =
			(FREE_IMAGE_MDMODEL[])Enum.GetValues(typeof(FREE_IMAGE_MDMODEL));

		/// <summary>
		/// Saved instance for faster access.
		/// </summary>
		private static readonly ConstructorInfo PropertyItemConstructor =
			typeof(PropertyItem).GetConstructor(
			BindingFlags.NonPublic | BindingFlags.Instance, null, new Type[] { }, null);

		private const int DIB_RGB_COLORS = 0;
		private const int DIB_PAL_COLORS = 1;
		private const int CBM_INIT = 0x4;

		/// <summary>
		/// An uncompressed format.
		/// </summary>
		public const int BI_RGB = 0;

		/// <summary>
		/// A run-length encoded (RLE) format for bitmaps with 8 bpp. The compression format is a 2-byte
		/// format consisting of a count byte followed by a byte containing a color index.
		/// </summary>
		public const int BI_RLE8 = 1;

		/// <summary>
		/// An RLE format for bitmaps with 4 bpp. The compression format is a 2-byte format consisting
		/// of a count byte followed by two word-length color indexes.
		/// </summary>
		public const int BI_RLE4 = 2;

		/// <summary>
		/// Specifies that the bitmap is not compressed and that the color table consists of three
		/// <b>DWORD</b> color masks that specify the red, green, and blue components, respectively,
		/// of each pixel. This is valid when used with 16- and 32-bpp bitmaps.
		/// </summary>
		public const int BI_BITFIELDS = 3;

		/// <summary>
		/// <b>Windows 98/Me, Windows 2000/XP:</b> Indicates that the image is a JPEG image.
		/// </summary>
		public const int BI_JPEG = 4;

		/// <summary>
		/// <b>Windows 98/Me, Windows 2000/XP:</b> Indicates that the image is a PNG image.
		/// </summary>
		public const int BI_PNG = 5;

		#endregion

		#region Callback

		// Callback delegate
		private static OutputMessageFunction outputMessageFunction;
		// Handle to pin the functions address
		private static GCHandle outputMessageHandle;

		static FreeImage()
		{
			// Check if FreeImage.dll is present and cancel setting the callbackfuntion if not
			if (!IsAvailable())
			{
				return;
			}
			// Create a delegate (function pointer) to 'OnMessage'
			outputMessageFunction = new OutputMessageFunction(OnMessage);
			// Pin the object so the garbage collector does not move it around in memory
			outputMessageHandle = GCHandle.Alloc(outputMessageFunction, GCHandleType.Normal);
			// Set the callback
			SetOutputMessage(outputMessageFunction);
		}

		/// <summary>
		/// Internal callback
		/// </summary>
		private static void OnMessage(FREE_IMAGE_FORMAT fif, string message)
		{
			// Invoke the message
			if (Message != null)
			{
				Message.Invoke(fif, message);
			}
		}

		/// <summary>
		/// Internal errors in FreeImage generate a logstring that can be
		/// captured by this event.
		/// </summary>
		public static event OutputMessageFunction Message;

		#endregion

		#region General functions

		/// <summary>
		/// Returns the internal version of this FreeImage 3 .NET wrapper.
		/// </summary>
		/// <returns>The internal version of this FreeImage 3 .NET wrapper.</returns>
		public static string GetWrapperVersion()
		{
			return FREEIMAGE_MAJOR_VERSION + "." + FREEIMAGE_MINOR_VERSION + "." + FREEIMAGE_RELEASE_SERIAL;
		}

		/// <summary>
		/// Returns a value indicating if the FreeImage DLL is available or not.
		/// </summary>
		/// <returns>True, if the FreeImage DLL is available, false otherwise.</returns>
		public static bool IsAvailable()
		{
			try
			{
				// Call a static fast executing function
				GetVersion();
				// No exception thrown, the dll seems to be present
				return true;
			}
			catch (DllNotFoundException)
			{
				return false;
			}
			catch (EntryPointNotFoundException)
			{
				return false;
			}
		}

		#endregion

		#region Bitmap management functions

		/// <summary>
		/// Converts a FreeImage bitmap to a .NET <see cref="System.Drawing.Bitmap"/>.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns>The converted .NET <see cref="System.Drawing.Bitmap"/>.</returns>
		/// <remarks>Copying metadata has been disabled until a proper way
		/// of reading and storing metadata in a .NET bitmap is found.</remarks>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// The image type of <paramref name="dib"/> is not FIT_BITMAP.</exception>
		public static Bitmap GetBitmap(FIBITMAP dib)
		{
			return GetBitmap(dib, true);
		}

		/// <summary>
		/// Converts a FreeImage bitmap to a .NET <see cref="System.Drawing.Bitmap"/>.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="copyMetadata">When true existing metadata will be copied.</param>
		/// <returns>The converted .NET <see cref="System.Drawing.Bitmap"/>.</returns>
		/// <remarks>Copying metadata has been disabled until a proper way
		/// of reading and storing metadata in a .NET bitmap is found.</remarks>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// The image type of <paramref name="dib"/> is not FIT_BITMAP.</exception>
		internal static Bitmap GetBitmap(FIBITMAP dib, bool copyMetadata)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			if (GetImageType(dib) != FREE_IMAGE_TYPE.FIT_BITMAP)
			{
				throw new ArgumentException("Only bitmaps with type of FIT_BITMAP can be converted.");
			}

			PixelFormat format = GetPixelFormat(dib);

			if ((format == PixelFormat.Undefined) && (GetBPP(dib) == 16u))
			{
				throw new ArgumentException("Only 16bit 555 and 565 are supported.");
			}

			int height = (int)GetHeight(dib);
			int width = (int)GetWidth(dib);
			int pitch = (int)GetPitch(dib);

			Bitmap result = new Bitmap(width, height, format);
			BitmapData data;
			// Locking the complete bitmap in writeonly mode
			data = result.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly, format);
			// Writing the bitmap data directly into the new created .NET bitmap.
			ConvertToRawBits(data.Scan0, dib, pitch, GetBPP(dib),
				GetRedMask(dib), GetGreenMask(dib), GetBlueMask(dib), true);
			// Unlock the bitmap
			result.UnlockBits(data);
			// Apply the bitmaps resolution
			result.SetResolution(GetResolutionX(dib), GetResolutionY(dib));
			// Check whether the bitmap has a palette
			if (GetPalette(dib) != IntPtr.Zero)
			{
				// Get the bitmaps palette to apply changes
				ColorPalette palette = result.Palette;
				// Get the orgininal palette
				Color[] colorPalette = new Palette(dib).ColorData;
				// Copy each value
				if (palette.Entries.Length == colorPalette.Length)
				{
					for (int i = 0; i < colorPalette.Length; i++)
					{
						palette.Entries[i] = colorPalette[i];
					}
					// Set the bitmaps palette
					result.Palette = palette;
				}
			}
			// Copy metadata
			if (copyMetadata)
			{
				try
				{
					List<PropertyItem> list = new List<PropertyItem>();
					// Get a list of all types
					FITAG tag;
					FIMETADATA mData;
					foreach (FREE_IMAGE_MDMODEL model in FREE_IMAGE_MDMODELS)
					{
						// Get a unique search handle
						mData = FindFirstMetadata(model, dib, out tag);
						// Check if metadata exists for this type
						if (mData.IsNull) continue;
						do
						{
							PropertyItem propItem = CreatePropertyItem();
							propItem.Len = (int)GetTagLength(tag);
							propItem.Id = (int)GetTagID(tag);
							propItem.Type = (short)GetTagType(tag);
							byte[] buffer = new byte[propItem.Len];

							unsafe
							{
								byte* src = (byte*)GetTagValue(tag);
								fixed (byte* dst = buffer)
								{
									MoveMemory(dst, src, (uint)propItem.Len);
								}
							}

							propItem.Value = buffer;
							list.Add(propItem);
						}
						while (FindNextMetadata(mData, out tag));
						FindCloseMetadata(mData);
					}
					foreach (PropertyItem propItem in list)
					{
						result.SetPropertyItem(propItem);
					}
				}
				catch
				{
				}
			}
			return result;
		}

		/// <summary>
		/// Converts an .NET <see cref="System.Drawing.Bitmap"/> into a FreeImage bitmap.
		/// </summary>
		/// <param name="bitmap">The <see cref="System.Drawing.Bitmap"/> to convert.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <remarks>Copying metadata has been disabled until a proper way
		/// of reading and storing metadata in a .NET bitmap is found.</remarks>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="bitmap"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// The bitmaps pixelformat is invalid.</exception>
		public static FIBITMAP CreateFromBitmap(Bitmap bitmap)
		{
			return CreateFromBitmap(bitmap, false);
		}

		/// <summary>
		/// Converts an .NET <see cref="System.Drawing.Bitmap"/> into a FreeImage bitmap.
		/// </summary>
		/// <param name="bitmap">The <see cref="System.Drawing.Bitmap"/> to convert.</param>
		/// <param name="copyMetadata">When true existing metadata will be copied.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <remarks>Copying metadata has been disabled until a proper way
		/// of reading and storing metadata in a .NET bitmap is found.</remarks>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="bitmap"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// The bitmaps pixelformat is invalid.</exception>
		internal static FIBITMAP CreateFromBitmap(Bitmap bitmap, bool copyMetadata)
		{
			if (bitmap == null)
			{
				throw new ArgumentNullException("bitmap");
			}
			uint bpp, red_mask, green_mask, blue_mask;
			if (!GetFormatParameters(bitmap.PixelFormat, out bpp, out red_mask, out green_mask, out blue_mask))
			{
				throw new ArgumentException("The bitmaps pixelformat is invalid.");
			}
			// Locking the complete bitmap in readonly mode
			BitmapData data = bitmap.LockBits(
				new Rectangle(0, 0, bitmap.Width, bitmap.Height),
				ImageLockMode.ReadOnly, bitmap.PixelFormat);
			// Copying the bitmap data directly from the .NET bitmap
			FIBITMAP result =
				ConvertFromRawBits(
					data.Scan0,
					data.Width,
					data.Height,
					data.Stride,
					bpp,
					red_mask,
					green_mask,
					blue_mask,
					true);
			bitmap.UnlockBits(data);
			// Handle palette
			if (GetPalette(result) != IntPtr.Zero)
			{
				Palette palette = new Palette(result);
				if (palette.Length == bitmap.Palette.Entries.Length)
				{
					for (int i = 0; i < palette.Length; i++)
					{
						palette[i] = (RGBQUAD)bitmap.Palette.Entries[i];
					}
				}
			}
			// Handle meta data
			// Disabled
			//if (copyMetadata)
			//{
			//    foreach (PropertyItem propItem in bitmap.PropertyItems)
			//    {
			//        FITAG tag = CreateTag();
			//        SetTagLength(tag, (uint)propItem.Len);
			//        SetTagID(tag, (ushort)propItem.Id);
			//        SetTagType(tag, (FREE_IMAGE_MDTYPE)propItem.Type);
			//        SetTagValue(tag, propItem.Value);
			//        SetMetadata(FREE_IMAGE_MDMODEL.FIMD_EXIF_EXIF, result, "", tag);
			//    }
			//}
			return result;
		}

		/// <summary>
		/// Saves a .NET <see cref="System.Drawing.Bitmap"/> to a file.
		/// </summary>
		/// <param name="bitmap">The .NET <see cref="System.Drawing.Bitmap"/> to save.</param>
		/// <param name="filename">Name of the file to save to.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="bitmap"/> or <paramref name="filename"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// The bitmaps pixelformat is invalid.</exception>
		public static bool SaveBitmap(
			Bitmap bitmap,
			string filename)
		{
			return SaveBitmap(
				bitmap,
				filename,
				FREE_IMAGE_FORMAT.FIF_UNKNOWN,
				FREE_IMAGE_SAVE_FLAGS.DEFAULT);
		}

		/// <summary>
		/// Saves a .NET <see cref="System.Drawing.Bitmap"/> to a file.
		/// </summary>
		/// <param name="bitmap">The .NET <see cref="System.Drawing.Bitmap"/> to save.</param>
		/// <param name="filename">Name of the file to save to.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="bitmap"/> or <paramref name="filename"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// The bitmaps pixelformat is invalid.</exception>
		public static bool SaveBitmap(
			Bitmap bitmap,
			string filename,
			FREE_IMAGE_SAVE_FLAGS flags)
		{
			return SaveBitmap(
				bitmap,
				filename,
				FREE_IMAGE_FORMAT.FIF_UNKNOWN,
				flags);
		}

		/// <summary>
		/// Saves a .NET <see cref="System.Drawing.Bitmap"/> to a file.
		/// </summary>
		/// <param name="bitmap">The .NET <see cref="System.Drawing.Bitmap"/> to save.</param>
		/// <param name="filename">Name of the file to save to.</param>
		/// <param name="format">Format of the bitmap. If the format should be taken from the
		/// filename use <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="bitmap"/> or <paramref name="filename"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// The bitmaps pixelformat is invalid.</exception>
		public static bool SaveBitmap(
			Bitmap bitmap,
			string filename,
			FREE_IMAGE_FORMAT format,
			FREE_IMAGE_SAVE_FLAGS flags)
		{
			FIBITMAP dib = CreateFromBitmap(bitmap);
			bool result = SaveEx(dib, filename, format, flags);
			Unload(dib);
			return result;
		}

		/// <summary>
		/// Loads a FreeImage bitmap.
		/// The file will be loaded with default loading flags.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists.</exception>
		public static FIBITMAP LoadEx(string filename)
		{
			FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			return LoadEx(filename, FREE_IMAGE_LOAD_FLAGS.DEFAULT, ref format);
		}

		/// <summary>
		/// Loads a FreeImage bitmap.
		/// Load flags can be provided by the flags parameter.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists.</exception>
		public static FIBITMAP LoadEx(string filename, FREE_IMAGE_LOAD_FLAGS flags)
		{
			FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			return LoadEx(filename, flags, ref format);
		}

		/// <summary>
		/// Loads a FreeImage bitmap.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> the files
		/// real format is being analysed. If no plugin can read the file, format remains
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> and 0 is returned.
		/// The file will be loaded with default loading flags.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <param name="format">Format of the image. If the format is unknown use
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.
		/// In case a suitable format was found by LoadEx it will be returned in format.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists.</exception>
		public static FIBITMAP LoadEx(string filename, ref FREE_IMAGE_FORMAT format)
		{
			return LoadEx(filename, FREE_IMAGE_LOAD_FLAGS.DEFAULT, ref format);
		}

		/// <summary>
		/// Loads a FreeImage bitmap.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> the files
		/// real format is being analysed. If no plugin can read the file, format remains
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> and 0 is returned.
		/// Load flags can be provided by the flags parameter.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="format">Format of the image. If the format is unknown use
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.
		/// In case a suitable format was found by LoadEx it will be returned in format.
		/// </param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists.</exception>
		public static FIBITMAP LoadEx(string filename, FREE_IMAGE_LOAD_FLAGS flags, ref FREE_IMAGE_FORMAT format)
		{
			// check if file exists
			if (!File.Exists(filename))
			{
				throw new FileNotFoundException(filename + " could not be found.");
			}
			FIBITMAP dib = 0;
			if (format == FREE_IMAGE_FORMAT.FIF_UNKNOWN)
			{
				// query all plugins to see if one can read the file
				format = GetFileType(filename, 0);
			}
			// check if the plugin is capable of loading files
			if (FIFSupportsReading(format))
			{
				dib = Load(format, filename, flags);
			}
			return dib;
		}

		/// <summary>
		/// Loads a .NET <see cref="System.Drawing.Bitmap"/> from a file.
		/// </summary>
		/// <param name="filename">Name of the file to be loaded.</param>
		/// <param name="format">Format of the image. If the format should be taken from the
		/// filename use <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>The loaded .NET <see cref="System.Drawing.Bitmap"/>.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists.</exception>
		/// <exception cref="ArgumentException">
		/// The image type of the image is not <see cref="FREE_IMAGE_TYPE.FIT_BITMAP"/>.</exception>
		public static Bitmap LoadBitmap(string filename, FREE_IMAGE_LOAD_FLAGS flags, ref FREE_IMAGE_FORMAT format)
		{
			FIBITMAP dib = LoadEx(filename, flags, ref format);
			Bitmap result = GetBitmap(dib, true);
			Unload(dib);
			return result;
		}

		/// <summary>
		/// Deletes a previously loaded FreeImage bitmap from memory and resets the handle to 0.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public static void UnloadEx(ref FIBITMAP dib)
		{
			if (!dib.IsNull)
			{
				Unload(dib);
				dib = 0;
			}
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a file.
		/// The format is taken off the filename.
		/// If no suitable format was found false will be returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="filename">The complete name of the file to save to.
		/// The extension will be corrected if it is no valid extension for the
		/// selected format or if no extension was specified.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="filename"/> is null.</exception>
		public static bool SaveEx(
			FIBITMAP dib,
			string filename)
		{
			return SaveEx(
				ref dib,
				filename,
				FREE_IMAGE_FORMAT.FIF_UNKNOWN,
				FREE_IMAGE_SAVE_FLAGS.DEFAULT,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				false);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a file.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>
		/// the format is taken off the filename.
		/// If no suitable format was found false will be returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="filename">The complete name of the file to save to.
		/// The extension will be corrected if it is no valid extension for the
		/// selected format or if no extension was specified.</param>
		/// <param name="format">Format of the image. If the format should be taken from the
		/// filename use <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="filename"/> is null.</exception>
		public static bool SaveEx(
			FIBITMAP dib,
			string filename,
			FREE_IMAGE_FORMAT format)
		{
			return SaveEx(
				ref dib,
				filename,
				format,
				FREE_IMAGE_SAVE_FLAGS.DEFAULT,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				false);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a file.
		/// The format is taken off the filename.
		/// If no suitable format was found false will be returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="filename">The complete name of the file to save to.
		/// The extension will be corrected if it is no valid extension for the
		/// selected format or if no extension was specified.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.
		/// If the function failed and returned false, the bitmap was not unloaded.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="filename"/> is null.</exception>
		public static bool SaveEx(
			ref FIBITMAP dib,
			string filename,
			bool unloadSource)
		{
			return SaveEx(
				ref dib,
				filename,
				FREE_IMAGE_FORMAT.FIF_UNKNOWN,
				FREE_IMAGE_SAVE_FLAGS.DEFAULT,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				unloadSource);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a file.
		/// The format is taken off the filename.
		/// If no suitable format was found false will be returned.
		/// Save flags can be provided by the flags parameter.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="filename">The complete name of the file to save to.
		/// The extension will be corrected if it is no valid extension for the
		/// selected format or if no extension was specified</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="filename"/> is null.</exception>
		public static bool SaveEx(
			FIBITMAP dib,
			string filename,
			FREE_IMAGE_SAVE_FLAGS flags)
		{
			return SaveEx(
				ref dib,
				filename,
				FREE_IMAGE_FORMAT.FIF_UNKNOWN,
				flags,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				false);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a file.
		/// The format is taken off the filename.
		/// If no suitable format was found false will be returned.
		/// Save flags can be provided by the flags parameter.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="filename">The complete name of the file to save to.
		/// The extension will be corrected if it is no valid extension for the
		/// selected format or if no extension was specified.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.
		/// If the function failed and returned false, the bitmap was not unloaded.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="filename"/> is null.</exception>
		public static bool SaveEx(
			ref FIBITMAP dib,
			string filename,
			FREE_IMAGE_SAVE_FLAGS flags,
			bool unloadSource)
		{
			return SaveEx(
				ref dib,
				filename,
				FREE_IMAGE_FORMAT.FIF_UNKNOWN,
				flags,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				unloadSource);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a file.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>
		/// the format is taken off the filename.
		/// If no suitable format was found false will be returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="filename">The complete name of the file to save to.
		/// The extension will be corrected if it is no valid extension for the
		/// selected format or if no extension was specified.</param>
		/// <param name="format">Format of the image. If the format should be taken from the
		/// filename use <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.
		/// If the function failed and returned false, the bitmap was not unloaded.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="filename"/> is null.</exception>
		public static bool SaveEx(
			ref FIBITMAP dib,
			string filename,
			FREE_IMAGE_FORMAT format,
			bool unloadSource)
		{
			return SaveEx(
				ref dib,
				filename,
				format,
				FREE_IMAGE_SAVE_FLAGS.DEFAULT,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				unloadSource);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a file.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>
		/// the format is taken off the filename.
		/// If no suitable format was found false will be returned.
		/// Save flags can be provided by the flags parameter.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="filename">The complete name of the file to save to.
		/// The extension will be corrected if it is no valid extension for the
		/// selected format or if no extension was specified.</param>
		/// <param name="format">Format of the image. If the format should be taken from the
		/// filename use <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="filename"/> is null.</exception>
		public static bool SaveEx(
			FIBITMAP dib,
			string filename,
			FREE_IMAGE_FORMAT format,
			FREE_IMAGE_SAVE_FLAGS flags)
		{
			return SaveEx(
				ref dib,
				filename,
				format,
				flags,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				false);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a file.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>
		/// the format is taken off the filename.
		/// If no suitable format was found false will be returned.
		/// Save flags can be provided by the flags parameter.
		/// The bitmaps color depth can be set by 'colorDepth'.
		/// If set to <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_AUTO"/> a suitable color depth
		/// will be taken if available.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="filename">The complete name of the file to save to.
		/// The extension will be corrected if it is no valid extension for the
		/// selected format or if no extension was specified.</param>
		/// <param name="format">Format of the image. If the format should be taken from the
		/// filename use <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="colorDepth">The new color depth of the bitmap.
		/// Set to <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_AUTO"/> if Save should take the
		/// best suitable color depth.
		/// If a color depth is selected that the provided format cannot write an
		/// error-message will be thrown.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.
		/// If the function failed and returned false, the bitmap was not unloaded.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentException">
		/// A direct color conversion failed.</exception>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="filename"/> is null.</exception>
		public static bool SaveEx(
			ref FIBITMAP dib,
			string filename,
			FREE_IMAGE_FORMAT format,
			FREE_IMAGE_SAVE_FLAGS flags,
			FREE_IMAGE_COLOR_DEPTH colorDepth,
			bool unloadSource)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			if (filename == null)
			{
				throw new ArgumentNullException("filename");
			}
			bool result = false;
			// Gets format from filename if the format is unknown
			if (format == FREE_IMAGE_FORMAT.FIF_UNKNOWN)
			{
				format = GetFIFFromFilename(filename);
			}
			if (format != FREE_IMAGE_FORMAT.FIF_UNKNOWN)
			{
				// Checks writing support
				if (FIFSupportsWriting(format) && FIFSupportsExportType(format, GetImageType(dib)))
				{
					// Check valid filename and correct it if needed
					if (!IsFilenameValidForFIF(format, filename))
					{
						int index = filename.LastIndexOf('.');
						string extension = GetPrimaryExtensionFromFIF(format);

						if (index == -1)
						{
							// We have no '.' (dot) so just add the extension
							filename += "." + extension;
						}
						else
						{
							// Overwrite the old extension
							filename = filename.Substring(0, filename.LastIndexOf('.')) + extension;
						}
					}

					FIBITMAP dibToSave = PrepareBitmapColorDepth(dib, format, colorDepth);
					result = Save(format, dibToSave, filename, flags);

					// Always unload a temporary created bitmap.
					if (dibToSave != dib)
					{
						UnloadEx(ref dibToSave);
					}
					// On success unload the bitmap
					if (result && unloadSource)
					{
						UnloadEx(ref dib);
					}
				}
			}
			return result;
		}

		/// <summary>
		/// Loads a FreeImage bitmap.
		/// The stream must be set to the correct position before calling LoadFromStream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> is not capable of reading.</exception>
		public static FIBITMAP LoadFromStream(
			Stream stream)
		{
			FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			return LoadFromStream(stream, FREE_IMAGE_LOAD_FLAGS.DEFAULT, ref format);
		}

		/// <summary>
		/// Loads a FreeImage bitmap.
		/// The stream must be set to the correct position before calling LoadFromStream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> is not capable of reading.</exception>
		public static FIBITMAP LoadFromStream(
			Stream stream,
			FREE_IMAGE_LOAD_FLAGS flags)
		{
			FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			return LoadFromStream(stream, flags, ref format);
		}

		/// <summary>
		/// Loads a FreeImage bitmap.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> the
		/// bitmaps real format is being analysed.
		/// The stream must be set to the correct position before calling LoadFromStream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		/// <param name="format">Format of the image. If the format is unknown use
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.
		/// In case a suitable format was found by LoadFromStream it will be returned in format.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> is not capable of reading.</exception>
		public static FIBITMAP LoadFromStream(
			Stream stream,
			ref FREE_IMAGE_FORMAT format)
		{
			return LoadFromStream(stream, FREE_IMAGE_LOAD_FLAGS.DEFAULT, ref format);
		}

		/// <summary>
		/// Loads a FreeImage bitmap.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>
		/// the bitmaps real format is being analysed.
		/// The stream must be set to the correct position before calling LoadFromStream.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="format">Format of the image. If the format is unknown use
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.
		/// In case a suitable format was found by LoadFromStream it will be returned in format.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> is not capable of reading.</exception>
		public static FIBITMAP LoadFromStream(
			Stream stream,
			FREE_IMAGE_LOAD_FLAGS flags,
			ref FREE_IMAGE_FORMAT format)
		{
			if (stream == null)
			{
				throw new ArgumentNullException("stream");
			}
			if (!stream.CanRead)
			{
				throw new ArgumentException("stream is not capable of reading.");
			}
			// Wrap the source stream if it is unable to seek (which is required by FreeImage)
			stream = (stream.CanSeek) ? stream : new StreamWrapper(stream, true);
			// Save the streams position
			if (format == FREE_IMAGE_FORMAT.FIF_UNKNOWN)
			{
				long position = stream.Position;
				// Get the format of the bitmap
				format = GetFileTypeFromStream(stream);
				// Restore the streams position
				stream.Position = position;
			}
			if (!FIFSupportsReading(format))
			{
				return 0;
			}
			// Create a 'FreeImageIO' structure for calling 'LoadFromHandle'
			// using the internal structure 'FreeImageStreamIO'.
			FreeImageIO io = FreeImageStreamIO.io;
			using (fi_handle handle = new fi_handle(stream))
			{
				return LoadFromHandle(format, ref io, handle, flags);
			}
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a stream.
		/// The stream must be set to the correct position before calling SaveToStream.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="stream">The stream to write to.</param>
		/// <param name="format">Format of the image.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> cannot write.</exception>
		public static bool SaveToStream(
			FIBITMAP dib,
			Stream stream,
			FREE_IMAGE_FORMAT format)
		{
			return SaveToStream(
				ref dib,
				stream,
				format,
				FREE_IMAGE_SAVE_FLAGS.DEFAULT,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				false);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a stream.
		/// The stream must be set to the correct position before calling SaveToStream.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="stream">The stream to write to.</param>
		/// <param name="format">Format of the image.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> cannot write.</exception>
		public static bool SaveToStream(
			ref FIBITMAP dib,
			Stream stream,
			FREE_IMAGE_FORMAT format,
			bool unloadSource)
		{
			return SaveToStream(
				ref dib,
				stream,
				format,
				FREE_IMAGE_SAVE_FLAGS.DEFAULT,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				unloadSource);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a stream.
		/// The stream must be set to the correct position before calling SaveToStream.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="stream">The stream to write to.</param>
		/// <param name="format">Format of the image.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> cannot write.</exception>
		public static bool SaveToStream(
			FIBITMAP dib,
			Stream stream,
			FREE_IMAGE_FORMAT format,
			FREE_IMAGE_SAVE_FLAGS flags)
		{
			return SaveToStream(
				ref dib,
				stream,
				format,
				flags,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				false);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a stream.
		/// The stream must be set to the correct position before calling SaveToStream.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="stream">The stream to write to.</param>
		/// <param name="format">Format of the image.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> cannot write.</exception>
		public static bool SaveToStream(
			ref FIBITMAP dib,
			Stream stream,
			FREE_IMAGE_FORMAT format,
			FREE_IMAGE_SAVE_FLAGS flags,
			bool unloadSource)
		{
			return SaveToStream(
				ref dib, stream,
				format,
				flags,
				FREE_IMAGE_COLOR_DEPTH.FICD_AUTO,
				unloadSource);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a stream.
		/// The stream must be set to the correct position before calling SaveToStream.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="stream">The stream to write to.</param>
		/// <param name="format">Format of the image.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="colorDepth">The new color depth of the bitmap.
		/// Set to <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_AUTO"/> if SaveToStream should
		/// take the best suitable color depth.
		/// If a color depth is selected that the provided format cannot write an
		/// error-message will be thrown.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> cannot write.</exception>
		public static bool SaveToStream(
			FIBITMAP dib,
			Stream stream,
			FREE_IMAGE_FORMAT format,
			FREE_IMAGE_SAVE_FLAGS flags,
			FREE_IMAGE_COLOR_DEPTH colorDepth)
		{
			return SaveToStream(
				ref dib,
				stream,
				format,
				flags,
				colorDepth,
				false);
		}

		/// <summary>
		/// Saves a previously loaded FreeImage bitmap to a stream.
		/// The stream must be set to the correct position before calling SaveToStream.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="stream">The stream to write to.</param>
		/// <param name="format">Format of the image.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="colorDepth">The new color depth of the bitmap.
		/// Set to <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_AUTO"/> if SaveToStream should
		/// take the best suitable color depth.
		/// If a color depth is selected that the provided format cannot write an
		/// error-message will be thrown.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> cannot write.</exception>
		public static bool SaveToStream(
			ref FIBITMAP dib,
			Stream stream,
			FREE_IMAGE_FORMAT format,
			FREE_IMAGE_SAVE_FLAGS flags,
			FREE_IMAGE_COLOR_DEPTH colorDepth,
			bool unloadSource)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			if (stream == null)
			{
				throw new ArgumentNullException("stream");
			}
			if (!stream.CanWrite)
			{
				throw new ArgumentException("stream is not capable of writing.");
			}
			if ((!FIFSupportsWriting(format)) || (!FIFSupportsExportType(format, FREE_IMAGE_TYPE.FIT_BITMAP)))
			{
				return false;
			}

			FIBITMAP dibToSave = PrepareBitmapColorDepth(dib, format, colorDepth);
			bool result = false;

			try
			{
				// Create a 'FreeImageIO' structure for calling 'SaveToHandle'
				FreeImageIO io = FreeImageStreamIO.io;

				using (fi_handle handle = new fi_handle(stream))
				{
					result = SaveToHandle(format, dibToSave, ref io, handle, flags);
				}
			}
			catch
			{
				// Always unload a temporary created bitmap.
				if (dibToSave != dib)
				{
					UnloadEx(ref dibToSave);
				}
				// On success unload the bitmap
				if (result && unloadSource)
				{
					UnloadEx(ref dib);
				}
			}

			return result;
		}

		#endregion

		#region Plugin functions

		/// <summary>
		/// Checks if an extension is valid for a certain format.
		/// </summary>
		/// <param name="fif">The desired format.</param>
		/// <param name="extension">The desired extension.</param>
		/// <returns>True if the extension is valid for the given format, false otherwise.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="extension"/> is null.</exception>
		public static bool IsExtensionValidForFIF(FREE_IMAGE_FORMAT fif, string extension)
		{
			return IsExtensionValidForFIF(fif, extension, StringComparison.CurrentCulture);
		}

		/// <summary>
		/// Checks if an extension is valid for a certain format.
		/// </summary>
		/// <param name="fif">The desired format.</param>
		/// <param name="extension">The desired extension.</param>
		/// <param name="comparisonType">The string comparison type.</param>
		/// <returns>True if the extension is valid for the given format, false otherwise.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="extension"/> is null.</exception>
		public static bool IsExtensionValidForFIF(FREE_IMAGE_FORMAT fif, string extension, StringComparison comparisonType)
		{
			if (extension == null)
			{
				throw new ArgumentNullException("extension");
			}
			bool result = false;
			// Split up the string and compare each with the given extension
			string tempList = GetFIFExtensionList(fif);
			if (tempList != null)
			{
				string[] extensionList = tempList.Split(',');
				foreach (string ext in extensionList)
				{
					if (extension.Equals(ext, comparisonType))
					{
						result = true;
						break;
					}
				}
			}
			return result;
		}

		/// <summary>
		/// Checks if a filename is valid for a certain format.
		/// </summary>
		/// <param name="fif">The desired format.</param>
		/// <param name="filename">The desired filename.</param>
		/// <returns>True if the filename is valid for the given format, false otherwise.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="filename"/> is null.</exception>
		public static bool IsFilenameValidForFIF(FREE_IMAGE_FORMAT fif, string filename)
		{
			return IsFilenameValidForFIF(fif, filename, StringComparison.CurrentCulture);
		}

		/// <summary>
		/// Checks if a filename is valid for a certain format.
		/// </summary>
		/// <param name="fif">The desired format.</param>
		/// <param name="filename">The desired filename.</param>
		/// <param name="comparisonType">The string comparison type.</param>
		/// <returns>True if the filename is valid for the given format, false otherwise.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="filename"/> is null.</exception>
		public static bool IsFilenameValidForFIF(FREE_IMAGE_FORMAT fif, string filename, StringComparison comparisonType)
		{
			if (filename == null)
			{
				throw new ArgumentNullException("filename");
			}
			bool result = false;
			// Extract the filenames extension if it exists
			int position = filename.LastIndexOf('.');
			if (position >= 0)
			{
				result = IsExtensionValidForFIF(fif, filename.Substring(position + 1), comparisonType);
			}
			return result;
		}

		/// <summary>
		/// This function returns the primary (main or most commonly used?) extension of a certain
		/// image format (fif). This is done by returning the first of all possible extensions
		/// returned by GetFIFExtensionList().
		/// That assumes, that the plugin returns the extensions in ordered form.</summary>
		/// <param name="fif">The image format to obtain the primary extension for.</param>
		/// <returns>The primary extension of the specified image format.</returns>
		public static string GetPrimaryExtensionFromFIF(FREE_IMAGE_FORMAT fif)
		{
			string result = null;
			string extensions = GetFIFExtensionList(fif);
			if (extensions != null)
			{
				int position = extensions.IndexOf(',');
				if (position < 0)
				{
					result = extensions;
				}
				else
				{
					result = extensions.Substring(0, position);
				}
			}
			return result;
		}

		#endregion

		#region Multipage functions

		/// <summary>
		/// Loads a FreeImage multi-paged bitmap.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <returns>Handle to a FreeImage multi-paged bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists while opening.</exception>
		public static FIMULTIBITMAP OpenMultiBitmapEx(
			string filename)
		{
			FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			return OpenMultiBitmapEx(
				filename,
				ref format,
				FREE_IMAGE_LOAD_FLAGS.DEFAULT,
				false,
				false,
				false);
		}

		/// <summary>
		/// Loads a FreeImage multi-paged bitmap.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <param name="keep_cache_in_memory">When true performance is increased at the cost of memory.</param>
		/// <returns>Handle to a FreeImage multi-paged bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists while opening.</exception>
		public static FIMULTIBITMAP OpenMultiBitmapEx(
			string filename,
			bool keep_cache_in_memory)
		{
			FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			return OpenMultiBitmapEx(
				filename,
				ref format,
				FREE_IMAGE_LOAD_FLAGS.DEFAULT,
				false,
				false,
				keep_cache_in_memory);
		}

		/// <summary>
		/// Loads a FreeImage multi-paged bitmap.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <param name="read_only">When true the bitmap will be loaded read only.</param>
		/// <param name="keep_cache_in_memory">When true performance is increased at the cost of memory.</param>
		/// <returns>Handle to a FreeImage multi-paged bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists while opening.</exception>
		public static FIMULTIBITMAP OpenMultiBitmapEx(
			string filename,
			bool read_only,
			bool keep_cache_in_memory)
		{
			FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			return OpenMultiBitmapEx(
				filename,
				ref format,
				FREE_IMAGE_LOAD_FLAGS.DEFAULT,
				false,
				read_only,
				keep_cache_in_memory);
		}

		/// <summary>
		/// Loads a FreeImage multi-paged bitmap.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <param name="create_new">When true a new bitmap is created.</param>
		/// <param name="read_only">When true the bitmap will be loaded read only.</param>
		/// <param name="keep_cache_in_memory">When true performance is increased at the cost of memory.</param>
		/// <returns>Handle to a FreeImage multi-paged bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists while opening.</exception>
		public static FIMULTIBITMAP OpenMultiBitmapEx(
			string filename,
			bool create_new,
			bool read_only,
			bool keep_cache_in_memory)
		{
			FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			return OpenMultiBitmapEx(
				filename,
				ref format,
				FREE_IMAGE_LOAD_FLAGS.DEFAULT,
				create_new,
				read_only,
				keep_cache_in_memory);
		}

		/// <summary>
		/// Loads a FreeImage multi-paged bitmap.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> the files real
		/// format is being analysed. If no plugin can read the file, format remains
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> and 0 is returned.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <param name="format">Format of the image. If the format is unknown use
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.
		/// In case a suitable format was found by LoadEx it will be returned in format.</param>
		/// <param name="create_new">When true a new bitmap is created.</param>
		/// <param name="read_only">When true the bitmap will be loaded read only.</param>
		/// <param name="keep_cache_in_memory">When true performance is increased at the cost of memory.</param>
		/// <returns>Handle to a FreeImage multi-paged bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists while opening.</exception>
		public static FIMULTIBITMAP OpenMultiBitmapEx(
			string filename,
			ref FREE_IMAGE_FORMAT format,
			bool create_new,
			bool read_only,
			bool keep_cache_in_memory)
		{
			return OpenMultiBitmapEx(
				filename,
				ref format,
				FREE_IMAGE_LOAD_FLAGS.DEFAULT,
				create_new,
				read_only,
				keep_cache_in_memory);
		}

		/// <summary>
		/// Loads a FreeImage multi-paged bitmap.
		/// In case the loading format is <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> the files
		/// real format is being analysed. If no plugin can read the file, format remains
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/> and 0 is returned.
		/// Load flags can be provided by the flags parameter.
		/// </summary>
		/// <param name="filename">The complete name of the file to load.</param>
		/// <param name="format">Format of the image. If the format is unknown use 
		/// <see cref="FREE_IMAGE_FORMAT.FIF_UNKNOWN"/>.
		/// In case a suitable format was found by LoadEx it will be returned in format.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="create_new">When true a new bitmap is created.</param>
		/// <param name="read_only">When true the bitmap will be loaded read only.</param>
		/// <param name="keep_cache_in_memory">When true performance is increased at the cost of memory.</param>
		/// <returns>Handle to a FreeImage multi-paged bitmap.</returns>
		/// <exception cref="FileNotFoundException">
		/// <paramref name="filename"/> does not exists while opening.</exception>
		public static FIMULTIBITMAP OpenMultiBitmapEx(
			string filename,
			ref FREE_IMAGE_FORMAT format,
			FREE_IMAGE_LOAD_FLAGS flags,
			bool create_new,
			bool read_only,
			bool keep_cache_in_memory)
		{
			if (!File.Exists(filename) && !create_new)
			{
				throw new FileNotFoundException(filename + " could not be found.");
			}
			if (format == FREE_IMAGE_FORMAT.FIF_UNKNOWN)
			{
				// Check if a plugin can read the data
				format = GetFileType(filename, 0);
			}
			FIMULTIBITMAP dib = 0;
			if (FIFSupportsReading(format))
			{
				dib = OpenMultiBitmap(format, filename, create_new, read_only, keep_cache_in_memory, flags);
			}
			return dib;
		}

		/// <summary>
		/// Closes a previously opened multi-page bitmap and, when the bitmap was not opened read-only,
		/// applies any changes made to it.
		/// On success the handle will be reset to null.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage multi-paged bitmap.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		public static bool CloseMultiBitmapEx(ref FIMULTIBITMAP dib)
		{
			return CloseMultiBitmapEx(ref dib, FREE_IMAGE_SAVE_FLAGS.DEFAULT);
		}

		/// <summary>
		/// Closes a previously opened multi-page bitmap and, when the bitmap was not opened read-only,
		/// applies any changes made to it.
		/// On success the handle will be reset to null.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage multi-paged bitmap.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		public static bool CloseMultiBitmapEx(ref FIMULTIBITMAP dib, FREE_IMAGE_SAVE_FLAGS flags)
		{
			bool result = false;
			if (!dib.IsNull)
			{
				if (CloseMultiBitmap(dib, flags))
				{
					dib = 0;
					result = true;
				}
			}
			return result;
		}

		/// <summary>
		/// Retrieves the number of pages that are locked in a multi-paged bitmap.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage multi-paged bitmap.</param>
		/// <returns>Number of locked pages.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static int GetLockedPageCount(FIMULTIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			int result = 0;
			GetLockedPageNumbers(dib, null, ref result);
			return result;
		}

		/// <summary>
		/// Retrieves a list locked pages of a multi-paged bitmap.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage multi-paged bitmap.</param>
		/// <returns>List containing the indexes of the locked pages.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static int[] GetLockedPages(FIMULTIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			// Get the number of pages and create an array to save the information
			int count = 0;
			int[] result = null;
			// Get count
			if (GetLockedPageNumbers(dib, result, ref count))
			{
				result = new int[count];
				// Fill array
				if (!GetLockedPageNumbers(dib, result, ref count))
				{
					result = null;
				}
			}
			return result;
		}

		/// <summary>
		/// Loads a FreeImage multi-paged bitmap from a stream and returns the
		/// FreeImage memory stream used as temporary buffer.
		/// The bitmap can not be modified by calling
		/// <see cref="FreeImage.AppendPage(FIMULTIBITMAP,FIBITMAP)"/>,
		/// <see cref="FreeImage.InsertPage(FIMULTIBITMAP,Int32,FIBITMAP)"/>,
		/// <see cref="FreeImage.MovePage(FIMULTIBITMAP,Int32,Int32)"/> or
		/// <see cref="FreeImage.DeletePage(FIMULTIBITMAP,Int32)"/>.
		/// </summary>
		/// <param name="stream">The stream to read from.</param>
		/// <param name="format">Format of the image.</param>
		/// <param name="flags">Flags to enable or disable plugin-features.</param>
		/// <param name="memory">The temporary memory buffer used to load the bitmap.</param>
		/// <returns>Handle to a FreeImage multi-paged bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> can not read.</exception>
		public static FIMULTIBITMAP LoadMultiBitmapFromStream(
			Stream stream,
			FREE_IMAGE_FORMAT format,
			FREE_IMAGE_LOAD_FLAGS flags,
			out FIMEMORY memory)
		{
			if (stream == null)
			{
				throw new ArgumentNullException("stream");
			}
			if (!stream.CanRead)
			{
				throw new ArgumentException("stream");
			}
			const int blockSize = 1024;
			int bytesRead;
			byte[] buffer = new byte[blockSize];

			stream = stream.CanSeek ? stream : new StreamWrapper(stream, true);
			memory = OpenMemory(IntPtr.Zero, 0);

			do
			{
				bytesRead = stream.Read(buffer, 0, blockSize);
				WriteMemory(buffer, (uint)blockSize, (uint)1, memory);
			}
			while (bytesRead == blockSize);

			return LoadMultiBitmapFromMemory(format, memory, flags);
		}

		#endregion

		#region Filetype functions

		/// <summary>
		/// Orders FreeImage to analyze the bitmap signature.
		/// In case the stream is not seekable, the stream will have been used
		/// and must be recreated for loading.
		/// </summary>
		/// <param name="stream">Name of the stream to analyze.</param>
		/// <returns>Type of the bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="stream"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <paramref name="stream"/> can not read.</exception>
		public static FREE_IMAGE_FORMAT GetFileTypeFromStream(Stream stream)
		{
			if (stream == null)
			{
				throw new ArgumentNullException("stream");
			}
			if (!stream.CanRead)
			{
				throw new ArgumentException("stream is not capable of reading.");
			}
			// Wrap the stream if it cannot seek
			stream = (stream.CanSeek) ? stream : new StreamWrapper(stream, true);
			// Create a 'FreeImageIO' structure for the stream
			FreeImageIO io = FreeImageStreamIO.io;
			using (fi_handle handle = new fi_handle(stream))
			{
				return GetFileTypeFromHandle(ref io, handle, 0);
			}
		}

		#endregion

		#region Pixel access functions

		/// <summary>
		/// Retrieves an hBitmap for a FreeImage bitmap.
		/// Call FreeHbitmap(IntPtr) to free the handle.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="hdc">A reference device context.
		/// Use IntPtr.Zero if no reference is available.</param>
		/// <param name="unload">When true dib will be unloaded if the function succeeded.</param>
		/// <returns>The hBitmap for the FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static unsafe IntPtr GetHbitmap(FIBITMAP dib, IntPtr hdc, bool unload)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			IntPtr hBitmap = IntPtr.Zero;
			bool release = false;
			IntPtr ppvBits = IntPtr.Zero;
			// Check if we have destination
			if (release = (hdc == IntPtr.Zero))
			{
				// We don't so request dc
				hdc = GetDC(IntPtr.Zero);
			}
			if (hdc != IntPtr.Zero)
			{
				// Get pointer to the infoheader of the bitmap
				IntPtr info = GetInfo(dib);
				// Create a bitmap in the dc
				hBitmap = CreateDIBSection(hdc, info, DIB_RGB_COLORS, out ppvBits, IntPtr.Zero, 0);
				if (hBitmap != IntPtr.Zero && ppvBits != IntPtr.Zero)
				{
					// Copy the data into the dc
					CopyMemory(
						ppvBits,
						GetBits(dib),
						(GetHeight(dib) * GetPitch(dib)));
					// Success: we unload the bitmap
					if (unload)
					{
						Unload(dib);
					}
				}
				// We have to release the dc
				if (release)
				{
					ReleaseDC(IntPtr.Zero, hdc);
				}
			}
			return hBitmap;
		}

		/// <summary>
		/// Returns an HBITMAP created by the <c>CreateDIBitmap()</c> function which in turn
		/// has always the same color depth as the reference DC, which may be provided
		/// through <paramref name="hdc"/>. The desktop DC will be used,
		/// if <c>IntPtr.Zero</c> DC is specified.
		/// Call <see cref="FreeImage.FreeHbitmap(IntPtr)"/> to free the handle.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="hdc">Handle to a device context.</param>
		/// <param name="unload">When true the structure will be unloaded on success.
		/// If the function failed and returned false, the bitmap was not unloaded.</param>
		/// <returns>If the function succeeds, the return value is a handle to the
		/// compatible bitmap. If the function fails, the return value is <see cref="IntPtr.Zero"/>.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static IntPtr GetBitmapForDevice(FIBITMAP dib, IntPtr hdc, bool unload)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			IntPtr hbitmap = IntPtr.Zero;
			bool release = false;
			if (release = (hdc == IntPtr.Zero))
			{
				hdc = GetDC(IntPtr.Zero);
			}
			if (hdc != IntPtr.Zero)
			{
				hbitmap = CreateDIBitmap(
					hdc,
					GetInfoHeader(dib),
					CBM_INIT,
					GetBits(dib),
					GetInfo(dib),
					DIB_RGB_COLORS);
				if (unload)
				{
					Unload(dib);
				}
				if (release)
				{
					ReleaseDC(IntPtr.Zero, hdc);
				}
			}
			return hbitmap;
		}

		/// <summary>
		/// Creates a FreeImage DIB from a Device Context/Compatible Bitmap.
		/// </summary>
		/// <param name="hbitmap">Handle to the bitmap.</param>
		/// <param name="hdc">Handle to a device context.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="hbitmap"/> is null.</exception>
		public unsafe static FIBITMAP CreateFromHbitmap(IntPtr hbitmap, IntPtr hdc)
		{
			if (hbitmap == IntPtr.Zero)
			{
				throw new ArgumentNullException("hbitmap");
			}

			FIBITMAP dib = 0;
			BITMAP bm;
			uint colors;
			bool release;

			if (GetObject(hbitmap, sizeof(BITMAP), (IntPtr)(&bm)) != 0)
			{
				dib = Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel, 0, 0, 0);
				if (!dib.IsNull)
				{
					colors = GetColorsUsed(dib);
					if (release = (hdc == IntPtr.Zero))
					{
						hdc = GetDC(IntPtr.Zero);
					}
					if (GetDIBits(
							hdc,
							hbitmap,
							0,
							(uint)bm.bmHeight,
							GetBits(dib),
							GetInfo(dib),
							DIB_RGB_COLORS) != 0)
					{
						if (colors != 0)
						{
							BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*)GetInfo(dib);
							bmih[0].biClrImportant = bmih[0].biClrUsed = colors;
						}
					}
					else
					{
						UnloadEx(ref dib);
					}
					if (release)
					{
						ReleaseDC(IntPtr.Zero, hdc);
					}
				}
			}

			return dib;
		}

		/// <summary>
		/// Frees a bitmap handle.
		/// </summary>
		/// <param name="hbitmap">Handle to a bitmap.</param>
		/// <returns>True on success, false on failure.</returns>
		public static bool FreeHbitmap(IntPtr hbitmap)
		{
			return DeleteObject(hbitmap);
		}

		#endregion

		#region Bitmap information functions

		/// <summary>
		/// Retrieves a DIB's resolution in X-direction measured in 'dots per inch' (DPI) and not in
		/// 'dots per meter'.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns>The resolution in 'dots per inch'.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static uint GetResolutionX(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			return (uint)(0.5d + 0.0254d * GetDotsPerMeterX(dib));
		}

		/// <summary>
		/// Retrieves a DIB's resolution in Y-direction measured in 'dots per inch' (DPI) and not in
		/// 'dots per meter'.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns>The resolution in 'dots per inch'.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static uint GetResolutionY(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			return (uint)(0.5d + 0.0254d * GetDotsPerMeterY(dib));
		}

		/// <summary>
		/// Sets a DIB's resolution in X-direction measured in 'dots per inch' (DPI) and not in
		/// 'dots per meter'.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="res">The new resolution in 'dots per inch'.</param>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static void SetResolutionX(FIBITMAP dib, uint res)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			SetDotsPerMeterX(dib, (uint)((double)res / 0.0254d + 0.5d));
		}

		/// <summary>
		/// Sets a DIB's resolution in Y-direction measured in 'dots per inch' (DPI) and not in
		/// 'dots per meter'.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="res">The new resolution in 'dots per inch'.</param>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static void SetResolutionY(FIBITMAP dib, uint res)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			SetDotsPerMeterY(dib, (uint)((double)res / 0.0254d + 0.5d));
		}

		/// <summary>
		/// Returns whether the image is a greyscale image or not.
		/// The function scans all colors in the bitmaps palette for entries where
		/// red, green and blue are not all the same (not a grey color).
		/// Supports 1-, 4- and 8-bit bitmaps.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns>True if the image is a greyscale image, else false.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static unsafe bool IsGreyscaleImage(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			bool result = true;
			uint bpp = GetBPP(dib);
			switch (bpp)
			{
				case 1:
				case 4:
				case 8:
					RGBQUAD* palette = (RGBQUAD*)GetPalette(dib);
					uint paletteLength = GetColorsUsed(dib);
					for (int i = 0; i < paletteLength; i++)
					{
						if (palette[i].rgbRed != palette[i].rgbGreen ||
							palette[i].rgbRed != palette[i].rgbBlue)
						{
							result = false;
							break;
						}
					}
					break;
				default:
					result = false;
					break;
			}
			return result;
		}

		/// <summary>
		/// Returns a structure that represents the palette of a FreeImage bitmap.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns>A structure representing the bitmaps palette.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static Palette GetPaletteEx(FIBITMAP dib)
		{
			return new Palette(dib);
		}

		/// <summary>
		/// Returns the <see cref="BITMAPINFOHEADER"/> structure of a FreeImage bitmap.
		/// The structure is a copy, so changes will have no effect on
		/// the bitmap itself.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns><see cref="BITMAPINFOHEADER"/> structure of the bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static unsafe BITMAPINFOHEADER GetInfoHeaderEx(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			return *(BITMAPINFOHEADER*)GetInfoHeader(dib);
		}

		/// <summary>
		/// Returns the <see cref="BITMAPINFO"/> structure of a FreeImage bitmap.
		/// The structure is a copy, so changes will have no effect on
		/// the bitmap itself.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns><see cref="BITMAPINFO"/> structure of the bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static BITMAPINFO GetInfoEx(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			BITMAPINFO result = new BITMAPINFO();
			result.bmiHeader = GetInfoHeaderEx(dib);
			IntPtr ptr = GetPalette(dib);
			if (ptr == IntPtr.Zero)
			{
				result.bmiColors = new RGBQUAD[0];
			}
			else
			{
				result.bmiColors = new MemoryArray<RGBQUAD>(ptr, (int)result.bmiHeader.biClrUsed).Data;
			}
			return result;
		}

		/// <summary>
		/// Returns the pixelformat of the bitmap.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns><see cref="System.Drawing.Imaging.PixelFormat"/> of the bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static PixelFormat GetPixelFormat(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}

			PixelFormat result = PixelFormat.Undefined;

			if (GetImageType(dib) == FREE_IMAGE_TYPE.FIT_BITMAP)
			{
				switch (GetBPP(dib))
				{
					case 1:
						result = PixelFormat.Format1bppIndexed;
						break;
					case 4:
						result = PixelFormat.Format4bppIndexed;
						break;
					case 8:
						result = PixelFormat.Format8bppIndexed;
						break;
					case 16:
						if ((GetBlueMask(dib) == FI16_565_BLUE_MASK) &&
							(GetGreenMask(dib) == FI16_565_GREEN_MASK) &&
							(GetRedMask(dib) == FI16_565_RED_MASK))
						{
							result = PixelFormat.Format16bppRgb565;
						}
						if ((GetBlueMask(dib) == FI16_555_BLUE_MASK) &&
							(GetGreenMask(dib) == FI16_555_GREEN_MASK) &&
							(GetRedMask(dib) == FI16_555_RED_MASK))
						{
							result = PixelFormat.Format16bppRgb555;
						}
						break;
					case 24:
						result = PixelFormat.Format24bppRgb;
						break;
					case 32:
						result = PixelFormat.Format32bppArgb;
						break;
				}
			}
			return result;
		}

		/// <summary>
		/// Retrieves all parameters needed to create a new FreeImage bitmap from
		/// the format of a .NET <see cref="System.Drawing.Image"/>.
		/// </summary>
		/// <param name="format">The <see cref="System.Drawing.Imaging.PixelFormat"/>
		/// of the .NET <see cref="System.Drawing.Image"/>.</param>
		/// <param name="bpp">Returns the color depth for the new bitmap.</param>
		/// <param name="red_mask">Returns the red_mask for the new bitmap.</param>
		/// <param name="green_mask">Returns the green_mask for the new bitmap.</param>
		/// <param name="blue_mask">Returns the blue_mask for the new bitmap.</param>
		/// <returns>True in case <paramref name="format"/> is
		/// <see cref="FREE_IMAGE_TYPE.FIT_BITMAP"/>, else false.</returns>
		public static bool GetFormatParameters(
			PixelFormat format,
			out uint bpp,
			out uint red_mask,
			out uint green_mask,
			out uint blue_mask)
		{
			bool result = false;
			bpp = 0;
			red_mask = 0;
			green_mask = 0;
			blue_mask = 0;
			switch (format)
			{
				case PixelFormat.Format1bppIndexed:
					bpp = 1;
					result = true;
					break;
				case PixelFormat.Format4bppIndexed:
					bpp = 4;
					result = true;
					break;
				case PixelFormat.Format8bppIndexed:
					bpp = 8;
					result = true;
					break;
				case PixelFormat.Format16bppRgb565:
					bpp = 16;
					red_mask = FI16_565_RED_MASK;
					green_mask = FI16_565_GREEN_MASK;
					blue_mask = FI16_565_BLUE_MASK;
					result = true;
					break;
				case PixelFormat.Format16bppRgb555:
					bpp = 16;
					red_mask = FI16_555_RED_MASK;
					green_mask = FI16_555_GREEN_MASK;
					blue_mask = FI16_555_BLUE_MASK;
					result = true;
					break;
				case PixelFormat.Format24bppRgb:
					bpp = 24;
					red_mask = FI_RGBA_RED_MASK;
					green_mask = FI_RGBA_GREEN_MASK;
					blue_mask = FI_RGBA_BLUE_MASK;
					result = true;
					break;
				case PixelFormat.Format32bppRgb:
				case PixelFormat.Format32bppArgb:
					bpp = 32;
					red_mask = FI_RGBA_RED_MASK;
					green_mask = FI_RGBA_GREEN_MASK;
					blue_mask = FI_RGBA_BLUE_MASK;
					result = true;
					break;
			}
			return result;
		}

		/// <summary>
		/// Compares two FreeImage bitmaps.
		/// </summary>
		/// <param name="dib1">The first bitmap to compare.</param>
		/// <param name="dib2">The second bitmap to compare.</param>
		/// <param name="flags">Determines which components of the bitmaps will be compared.</param>
		/// <returns>True in case both bitmaps match the compare conditions, false otherwise.</returns>
		public static bool Compare(FIBITMAP dib1, FIBITMAP dib2, FREE_IMAGE_COMPARE_FLAGS flags)
		{
			// Check whether one bitmap is null
			if (dib1.IsNull ^ dib2.IsNull)
			{
				return false;
			}
			// Check whether both pointers are the same
			if (dib1 == dib2)
			{
				return true;
			}
			if (((flags & FREE_IMAGE_COMPARE_FLAGS.HEADER) > 0) && (!CompareHeader(dib1, dib2)))
			{
				return false;
			}
			if (((flags & FREE_IMAGE_COMPARE_FLAGS.PALETTE) > 0) && (!ComparePalette(dib1, dib2)))
			{
				return false;
			}
			if (((flags & FREE_IMAGE_COMPARE_FLAGS.DATA) > 0) && (!CompareData(dib1, dib2)))
			{
				return false;
			}
			if (((flags & FREE_IMAGE_COMPARE_FLAGS.METADATA) > 0) && (!CompareMetadata(dib1, dib2)))
			{
				return false;
			}
			return true;
		}

		private static unsafe bool CompareHeader(FIBITMAP dib1, FIBITMAP dib2)
		{
			IntPtr i1 = GetInfoHeader(dib1);
			IntPtr i2 = GetInfoHeader(dib2);
			return CompareMemory((void*)i1, (void*)i2, sizeof(BITMAPINFOHEADER));
		}

		private static unsafe bool ComparePalette(FIBITMAP dib1, FIBITMAP dib2)
		{
			IntPtr pal1 = GetPalette(dib1), pal2 = GetPalette(dib2);
			bool hasPalette1 = pal1 != IntPtr.Zero;
			bool hasPalette2 = pal2 != IntPtr.Zero;
			if (hasPalette1 ^ hasPalette2)
			{
				return false;
			}
			if (!hasPalette1)
			{
				return true;
			}
			uint colors = GetColorsUsed(dib1);
			if (colors != GetColorsUsed(dib2))
			{
				return false;
			}
			return CompareMemory((void*)pal1, (void*)pal2, sizeof(RGBQUAD) * colors);
		}

		private static unsafe bool CompareData(FIBITMAP dib1, FIBITMAP dib2)
		{
			uint width = GetWidth(dib1);
			if (width != GetWidth(dib2))
			{
				return false;
			}
			uint height = GetHeight(dib1);
			if (height != GetHeight(dib2))
			{
				return false;
			}
			uint bpp = GetBPP(dib1);
			if (bpp != GetBPP(dib2))
			{
				return false;
			}
			if (GetColorType(dib1) != GetColorType(dib2))
			{
				return false;
			}
			FREE_IMAGE_TYPE type = GetImageType(dib1);
			if (type != GetImageType(dib2))
			{
				return false;
			}

			byte* ptr1, ptr2;
			int fullBytes;
			int shift;
			uint line = GetLine(dib1);

			if (type == FREE_IMAGE_TYPE.FIT_BITMAP)
			{
				switch (bpp)
				{
					case 32:
						for (int i = 0; i < height; i++)
						{
							ptr1 = (byte*)GetScanLine(dib1, i);
							ptr2 = (byte*)GetScanLine(dib2, i);
							if (!CompareMemory(ptr1, ptr2, line))
							{
								return false;
							}
						}
						break;
					case 24:
						for (int i = 0; i < height; i++)
						{
							ptr1 = (byte*)GetScanLine(dib1, i);
							ptr2 = (byte*)GetScanLine(dib2, i);
							if (!CompareMemory(ptr1, ptr2, line))
							{
								return false;
							}
						}
						break;
					case 16:
						short* sPtr1, sPtr2;
						for (int i = 0; i < height; i++)
						{
							sPtr1 = (short*)GetScanLine(dib1, i);
							sPtr2 = (short*)GetScanLine(dib2, i);
							for (int x = 0; x < width; x++)
							{
								if ((sPtr1[x] << 1) != (sPtr2[x] << 1))
								{
									return false;
								}
							}
						}
						break;
					case 8:
						for (int i = 0; i < height; i++)
						{
							ptr1 = (byte*)GetScanLine(dib1, i);
							ptr2 = (byte*)GetScanLine(dib2, i);
							if (!CompareMemory(ptr1, ptr2, line))
							{
								return false;
							}
						}
						break;
					case 4:
						fullBytes = (int)width / 2;
						shift = (width % 2) == 0 ? 8 : 4;
						for (int i = 0; i < height; i++)
						{
							ptr1 = (byte*)GetScanLine(dib1, i);
							ptr2 = (byte*)GetScanLine(dib2, i);
							if (fullBytes != 0)
							{
								if (!CompareMemory(ptr1, ptr2, fullBytes))
								{
									return false;
								}
								ptr1 += fullBytes;
								ptr2 += fullBytes;
							}
							if (shift != 8)
							{
								if ((ptr1[0] >> shift) != (ptr2[0] >> shift))
								{
									return false;
								}
							}
						}
						break;
					case 1:
						fullBytes = (int)width / 8;
						shift = 8 - ((int)width % 8);
						for (int i = 0; i < height; i++)
						{
							ptr1 = (byte*)GetScanLine(dib1, i);
							ptr2 = (byte*)GetScanLine(dib2, i);
							if (fullBytes != 0)
							{
								if (!CompareMemory(ptr1, ptr2, fullBytes))
								{
									return false;
								}
								ptr1 += fullBytes;
								ptr2 += fullBytes;
							}
							if (shift != 8)
							{
								if ((ptr1[0] >> shift) != (ptr2[0] >> shift))
								{
									return false;
								}
							}
						}
						break;
					default:
						throw new NotSupportedException();
				}
			}
			else
			{
				for (int i = 0; i < height; i++)
				{
					ptr1 = (byte*)GetScanLine(dib1, i);
					ptr2 = (byte*)GetScanLine(dib2, i);
					if (!CompareMemory(ptr1, ptr2, line))
					{
						return false;
					}
				}
			}
			return true;
		}

		private static bool CompareMetadata(FIBITMAP dib1, FIBITMAP dib2)
		{
			MetadataTag tag1, tag2;

			foreach (FREE_IMAGE_MDMODEL metadataModel in FREE_IMAGE_MDMODELS)
			{
				if (GetMetadataCount(metadataModel, dib1) !=
					GetMetadataCount(metadataModel, dib2))
				{
					return false;
				}
				if (GetMetadataCount(metadataModel, dib1) == 0)
				{
					continue;
				}

				FIMETADATA mdHandle = FindFirstMetadata(metadataModel, dib1, out tag1);
				if (mdHandle.IsNull)
				{
					continue;
				}
				do
				{
					if ((!GetMetadata(metadataModel, dib2, tag1.Key, out tag2)) ||
						(tag1 != tag2))
					{
						FindCloseMetadata(mdHandle);
						return false;
					}
				}
				while (FindNextMetadata(mdHandle, out tag1));
				FindCloseMetadata(mdHandle);
			}

			return true;
		}

		/// <summary>
		/// Returns the FreeImage bitmap's transparency table.
		/// The array is empty in case the bitmap has no transparency table.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns>The FreeImage bitmap's transparency table.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static unsafe byte[] GetTransparencyTableEx(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			uint count = GetTransparencyCount(dib);
			byte[] result = new byte[count];
			byte* ptr = (byte*)GetTransparencyTable(dib);
			fixed (byte* dst = result)
			{
				MoveMemory(dst, ptr, count);
			}
			return result;
		}

		/// <summary>
		/// Set the FreeImage bitmap's transparency table. Only affects palletised bitmaps.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="table">The FreeImage bitmap's new transparency table.</param>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> or <paramref name="table"/> is null.</exception>
		public static void SetTransparencyTable(FIBITMAP dib, byte[] table)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			if (table == null)
			{
				throw new ArgumentNullException("table");
			}
			SetTransparencyTable_(dib, table, table.Length);
		}

		/// <summary>
		/// This function returns the number of unique colors actually used by the
		/// specified 1-, 4-, 8-, 16-, 24- or 32-bit image. This might be different from
		/// what function FreeImage_GetColorsUsed() returns, which actually returns the
		/// palette size for palletised images. Works for
		/// <see cref="FREE_IMAGE_TYPE.FIT_BITMAP"/> type images only.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns>Returns the number of unique colors used by the image specified or
		/// zero, if the image type cannot be handled.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static unsafe int GetUniqueColors(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}

			int result = 0;

			if (GetImageType(dib) == FREE_IMAGE_TYPE.FIT_BITMAP)
			{
				BitArray bitArray;
				int uniquePalEnts;
				int hashcode;
				byte[] lut;
				int width = (int)GetWidth(dib);
				int height = (int)GetHeight(dib);

				switch (GetBPP(dib))
				{
					case 1:

						result = 1;
						lut = CreateShrunkenPaletteLUT(dib, out uniquePalEnts);
						if (uniquePalEnts == 1)
						{
							break;
						}

						if ((*(byte*)GetScanLine(dib, 0) & 0x80) == 0)
						{
							for (int y = 0; y < height; y++)
							{
								byte* scanline = (byte*)GetScanLine(dib, y);
								int mask = 0x80;
								for (int x = 0; x < width; x++)
								{
									if ((scanline[x / 8] & mask) > 0)
									{
										return 2;
									}
									mask = (mask == 0x1) ? 0x80 : (mask >> 1);
								}
							}
						}
						else
						{
							for (int y = 0; y < height; y++)
							{
								byte* scanline = (byte*)GetScanLine(dib, y);
								int mask = 0x80;
								for (int x = 0; x < width; x++)
								{
									if ((scanline[x / 8] & mask) == 0)
									{
										return 2;
									}
									mask = (mask == 0x1) ? 0x80 : (mask >> 1);
								}
							}
						}
						break;

					case 4:

						bitArray = new BitArray(0x10);
						lut = CreateShrunkenPaletteLUT(dib, out uniquePalEnts);
						if (uniquePalEnts == 1)
						{
							result = 1;
							break;
						}

						for (int y = 0; (y < height) && (result < uniquePalEnts); y++)
						{
							byte* scanline = (byte*)GetScanLine(dib, y);
							bool top = true;
							for (int x = 0; (x < width) && (result < uniquePalEnts); x++)
							{
								if (top)
								{
									hashcode = lut[scanline[x / 2] >> 4];
								}
								else
								{
									hashcode = lut[scanline[x / 2] & 0xF];
								}
								top = !top;
								if (!bitArray[hashcode])
								{
									bitArray[hashcode] = true;
									result++;
								}
							}
						}
						break;

					case 8:

						bitArray = new BitArray(0x100);
						lut = CreateShrunkenPaletteLUT(dib, out uniquePalEnts);
						if (uniquePalEnts == 1)
						{
							result = 1;
							break;
						}

						for (int y = 0; (y < height) && (result < uniquePalEnts); y++)
						{
							byte* scanline = (byte*)GetScanLine(dib, y);
							for (int x = 0; (x < width) && (result < uniquePalEnts); x++)
							{
								hashcode = lut[scanline[x]];
								if (!bitArray[hashcode])
								{
									bitArray[hashcode] = true;
									result++;
								}
							}
						}
						break;

					case 16:

						bitArray = new BitArray(0x10000);

						for (int y = 0; y < height; y++)
						{
							short* scanline = (short*)GetScanLine(dib, y);
							for (int x = 0; x < width; x++, scanline++)
							{
								hashcode = *scanline;
								if (!bitArray[hashcode])
								{
									bitArray[hashcode] = true;
									result++;
								}
							}
						}
						break;

					case 24:

						bitArray = new BitArray(0x1000000);

						for (int y = 0; y < height; y++)
						{
							byte* scanline = (byte*)GetScanLine(dib, y);
							for (int x = 0; x < width; x++, scanline += 3)
							{
								hashcode = *((int*)scanline) & 0x00FFFFFF;
								if (!bitArray[hashcode])
								{
									bitArray[hashcode] = true;
									result++;
								}
							}
						}
						break;

					case 32:

						bitArray = new BitArray(0x1000000);

						for (int y = 0; y < height; y++)
						{
							int* scanline = (int*)GetScanLine(dib, y);
							for (int x = 0; x < width; x++, scanline++)
							{
								hashcode = *scanline & 0x00FFFFFF;
								if (!bitArray[hashcode])
								{
									bitArray[hashcode] = true;
									result++;
								}
							}
						}
						break;
				}
			}
			return result;
		}

		/// <summary>
		/// Verifies whether the FreeImage bitmap is 16bit 555.
		/// </summary>
		/// <param name="dib">The FreeImage bitmap to verify.</param>
		/// <returns><b>true</b> if the bitmap is RGB16-555; otherwise <b>false</b>.</returns>
		public static bool IsRGB555(FIBITMAP dib)
		{
			return ((GetRedMask(dib) == FI16_555_RED_MASK) &&
					(GetGreenMask(dib) == FI16_555_GREEN_MASK) &&
					(GetBlueMask(dib) == FI16_555_BLUE_MASK));
		}

		/// <summary>
		/// Verifies whether the FreeImage bitmap is 16bit 565.
		/// </summary>
		/// <param name="dib">The FreeImage bitmap to verify.</param>
		/// <returns><b>true</b> if the bitmap is RGB16-565; otherwise <b>false</b>.</returns>
		public static bool IsRGB565(FIBITMAP dib)
		{
			return ((GetRedMask(dib) == FI16_565_RED_MASK) &&
					(GetGreenMask(dib) == FI16_565_GREEN_MASK) &&
					(GetBlueMask(dib) == FI16_565_BLUE_MASK));
		}

		#endregion

		#region ICC profile functions

		/// <summary>
		/// Creates a new ICC-Profile for a FreeImage bitmap.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="data">The data of the new ICC-Profile.</param>
		/// <returns>The new ICC-Profile of the bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIICCPROFILE CreateICCProfileEx(FIBITMAP dib, byte[] data)
		{
			return new FIICCPROFILE(dib, data);
		}

		/// <summary>
		/// Creates a new ICC-Profile for a FreeImage bitmap.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="data">The data of the new ICC-Profile.</param>
		/// <param name="size">The number of bytes of <paramref name="data"/> to use.</param>
		/// <returns>The new ICC-Profile of the FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIICCPROFILE CreateICCProfileEx(FIBITMAP dib, byte[] data, int size)
		{
			return new FIICCPROFILE(dib, data, size);
		}

		#endregion

		#region Conversion functions

		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion)
		{
			return ConvertColorDepth(
				dib,
				conversion,
				128,
				FREE_IMAGE_DITHER.FID_FS,
				FREE_IMAGE_QUANTIZE.FIQ_WUQUANT,
				false);
		}

		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion,
			bool unloadSource)
		{
			return ConvertColorDepth(
				dib,
				conversion,
				128,
				FREE_IMAGE_DITHER.FID_FS,
				FREE_IMAGE_QUANTIZE.FIQ_WUQUANT,
				unloadSource);
		}

		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <param name="threshold">Threshold value when converting with
		/// <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_01_BPP_THRESHOLD"/>.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion,
			byte threshold)
		{
			return ConvertColorDepth(
				dib,
				conversion,
				threshold,
				FREE_IMAGE_DITHER.FID_FS,
				FREE_IMAGE_QUANTIZE.FIQ_WUQUANT,
				false);
		}

		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <param name="ditherMethod">Dither algorithm when converting 
		/// with <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_01_BPP_DITHER"/>.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion,
			FREE_IMAGE_DITHER ditherMethod)
		{
			return ConvertColorDepth(
				dib,
				conversion,
				128,
				ditherMethod,
				FREE_IMAGE_QUANTIZE.FIQ_WUQUANT,
				false);
		}


		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <param name="quantizationMethod">The quantization algorithm for conversion to 8-bit color depth.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion,
			FREE_IMAGE_QUANTIZE quantizationMethod)
		{
			return ConvertColorDepth(
				dib,
				conversion,
				128,
				FREE_IMAGE_DITHER.FID_FS,
				quantizationMethod,
				false);
		}

		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <param name="threshold">Threshold value when converting with
		/// <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_01_BPP_THRESHOLD"/>.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion,
			byte threshold,
			bool unloadSource)
		{
			return ConvertColorDepth(
				dib,
				conversion,
				threshold,
				FREE_IMAGE_DITHER.FID_FS,
				FREE_IMAGE_QUANTIZE.FIQ_WUQUANT,
				unloadSource);
		}

		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <param name="ditherMethod">Dither algorithm when converting with
		/// <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_01_BPP_DITHER"/>.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion,
			FREE_IMAGE_DITHER ditherMethod,
			bool unloadSource)
		{
			return ConvertColorDepth(
				dib,
				conversion,
				128,
				ditherMethod,
				FREE_IMAGE_QUANTIZE.FIQ_WUQUANT,
				unloadSource);
		}


		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <param name="quantizationMethod">The quantization algorithm for conversion to 8-bit color depth.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion,
			FREE_IMAGE_QUANTIZE quantizationMethod,
			bool unloadSource)
		{
			return ConvertColorDepth(
				dib,
				conversion,
				128,
				FREE_IMAGE_DITHER.FID_FS,
				quantizationMethod,
				unloadSource);
		}

		/// <summary>
		/// Converts a FreeImage bitmap from one color depth to another.
		/// If the conversion fails the original FreeImage bitmap is returned.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="conversion">The desired output format.</param>
		/// <param name="threshold">Threshold value when converting with
		/// <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_01_BPP_THRESHOLD"/>.</param>
		/// <param name="ditherMethod">Dither algorithm when converting with
		/// <see cref="FREE_IMAGE_COLOR_DEPTH.FICD_01_BPP_DITHER"/>.</param>
		/// <param name="quantizationMethod">The quantization algorithm for conversion to 8-bit color depth.</param>
		/// <param name="unloadSource">When true the structure will be unloaded on success.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		internal static FIBITMAP ConvertColorDepth(
			FIBITMAP dib,
			FREE_IMAGE_COLOR_DEPTH conversion,
			byte threshold,
			FREE_IMAGE_DITHER ditherMethod,
			FREE_IMAGE_QUANTIZE quantizationMethod,
			bool unloadSource)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}

			FIBITMAP result = 0;
			FIBITMAP dibTemp = 0;
			uint bpp = GetBPP(dib);
			bool reorderPalette = ((conversion & FREE_IMAGE_COLOR_DEPTH.FICD_REORDER_PALETTE) > 0);
			bool forceGreyscale = ((conversion & FREE_IMAGE_COLOR_DEPTH.FICD_FORCE_GREYSCALE) > 0);

			switch (conversion & (FREE_IMAGE_COLOR_DEPTH)0xFF)
			{
				case FREE_IMAGE_COLOR_DEPTH.FICD_01_BPP_THRESHOLD:

					if (bpp != 1)
					{
						result = Threshold(dib, threshold);
					}
					else
					{
						bool isGreyscale = IsGreyscaleImage(dib);
						if ((forceGreyscale && (!isGreyscale)) ||
						(reorderPalette && isGreyscale))
						{
							result = Threshold(dib, threshold);
						}
					}
					break;

				case FREE_IMAGE_COLOR_DEPTH.FICD_01_BPP_DITHER:

					if (bpp != 1)
					{
						result = Dither(dib, ditherMethod);
					}
					else
					{
						bool isGreyscale = IsGreyscaleImage(dib);
						if ((forceGreyscale && (!isGreyscale)) ||
						(reorderPalette && isGreyscale))
						{
							result = Dither(dib, ditherMethod);
						}
					}
					break;

				case FREE_IMAGE_COLOR_DEPTH.FICD_04_BPP:

					if (bpp != 4)
					{
						// Special case when 1bpp and FIC_PALETTE
						if (forceGreyscale && (bpp == 1) && (GetColorType(dib) == FREE_IMAGE_COLOR_TYPE.FIC_PALETTE))
						{
							dibTemp = ConvertToGreyscale(dib);
							result = ConvertTo4Bits(dibTemp);
							Unload(dibTemp);
						}
						// All other cases are converted directly
						else
						{
							result = ConvertTo4Bits(dib);
						}
					}
					else
					{
						bool isGreyscale = IsGreyscaleImage(dib);
						if ((forceGreyscale && (!isGreyscale)) ||
							(reorderPalette && isGreyscale))
						{
							dibTemp = ConvertToGreyscale(dib);
							result = ConvertTo4Bits(dibTemp);
							Unload(dibTemp);
						}
					}

					break;

				case FREE_IMAGE_COLOR_DEPTH.FICD_08_BPP:

					if (bpp != 8)
					{
						if (forceGreyscale)
						{
							result = ConvertToGreyscale(dib);
						}
						else
						{
							dibTemp = ConvertTo24Bits(dib);
							result = ColorQuantize(dibTemp, quantizationMethod);
							Unload(dibTemp);
						}
					}
					else
					{
						bool isGreyscale = IsGreyscaleImage(dib);
						if ((forceGreyscale && (!isGreyscale)) || (reorderPalette && isGreyscale))
						{
							result = ConvertToGreyscale(dib);
						}
					}
					break;

				case FREE_IMAGE_COLOR_DEPTH.FICD_16_BPP_555:

					if (forceGreyscale)
					{
						dibTemp = ConvertToGreyscale(dib);
						result = ConvertTo16Bits555(dibTemp);
						Unload(dibTemp);
					}
					else if (bpp != 16 || GetRedMask(dib) != FI16_555_RED_MASK || GetGreenMask(dib) != FI16_555_GREEN_MASK || GetBlueMask(dib) != FI16_555_BLUE_MASK)
					{
						result = ConvertTo16Bits555(dib);
					}
					break;

				case FREE_IMAGE_COLOR_DEPTH.FICD_16_BPP:

					if (forceGreyscale)
					{
						dibTemp = ConvertToGreyscale(dib);
						result = ConvertTo16Bits565(dibTemp);
						Unload(dibTemp);
					}
					else if (bpp != 16 || GetRedMask(dib) != FI16_565_RED_MASK || GetGreenMask(dib) != FI16_565_GREEN_MASK || GetBlueMask(dib) != FI16_565_BLUE_MASK)
					{
						result = ConvertTo16Bits565(dib);
					}
					break;

				case FREE_IMAGE_COLOR_DEPTH.FICD_24_BPP:

					if (forceGreyscale)
					{
						dibTemp = ConvertToGreyscale(dib);
						result = ConvertTo24Bits(dibTemp);
						Unload(dibTemp);
					}
					else if (bpp != 24)
					{
						result = ConvertTo24Bits(dib);
					}
					break;

				case FREE_IMAGE_COLOR_DEPTH.FICD_32_BPP:

					if (forceGreyscale)
					{
						dibTemp = ConvertToGreyscale(dib);
						result = ConvertTo32Bits(dibTemp);
						Unload(dibTemp);
					}
					else if (bpp != 32)
					{
						result = ConvertTo32Bits(dib);
					}
					break;
			}

			if (result.IsNull)
			{
				return dib;
			}
			if (unloadSource)
			{
				Unload(dib);
			}

			return result;
		}

		#endregion

		#region Metadata

		/// <summary>
		/// Copies metadata from one FreeImage bitmap to another.
		/// </summary>
		/// <param name="src">Source FreeImage bitmap containing the metadata.</param>
		/// <param name="dst">FreeImage bitmap to copy the metadata to.</param>
		/// <param name="flags">Flags to switch different copy modes.</param>
		/// <returns>Returns -1 on failure else the number of copied tags.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="src"/> or <paramref name="dst"/> is null.</exception>
		public static int CloneMetadataEx(FIBITMAP src, FIBITMAP dst, FREE_IMAGE_METADATA_COPY flags)
		{
			if (src.IsNull)
			{
				throw new ArgumentNullException("src");
			}
			if (dst.IsNull)
			{
				throw new ArgumentNullException("dst");
			}

			FITAG tag = 0, tag2 = 0;
			int copied = 0;

			// Clear all existing metadata
			if ((flags & FREE_IMAGE_METADATA_COPY.CLEAR_EXISTING) > 0)
			{
				foreach (FREE_IMAGE_MDMODEL model in FREE_IMAGE_MDMODELS)
				{
					if (!SetMetadata(model, dst, null, tag))
					{
						return -1;
					}
				}
			}

			bool keep = !((flags & FREE_IMAGE_METADATA_COPY.REPLACE_EXISTING) > 0);

			foreach (FREE_IMAGE_MDMODEL model in FREE_IMAGE_MDMODELS)
			{
				FIMETADATA mData = FindFirstMetadata(model, src, out tag);
				if (mData.IsNull) continue;
				do
				{
					string key = GetTagKey(tag);
					if (!(keep && GetMetadata(model, dst, key, out tag2)))
					{
						if (SetMetadata(model, dst, key, tag))
						{
							copied++;
						}
					}
				}
				while (FindNextMetadata(mData, out tag));
				FindCloseMetadata(mData);
			}

			return copied;
		}

		/// <summary>
		/// Returns the comment of a JPEG, PNG or GIF image.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <returns>Comment of the FreeImage bitmp, or null in case no comment exists.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static string GetImageComment(FIBITMAP dib)
		{
			string result = null;
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			FITAG tag;
			if (GetMetadata(FREE_IMAGE_MDMODEL.FIMD_COMMENTS, dib, "Comment", out tag))
			{
				MetadataTag metadataTag = new MetadataTag(tag, FREE_IMAGE_MDMODEL.FIMD_COMMENTS);
				result = metadataTag.Value as string;
			}
			return result;
		}

		/// <summary>
		/// Sets the comment of a JPEG, PNG or GIF image.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="comment">New comment of the FreeImage bitmap.
		/// Use null to remove the comment.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static bool SetImageComment(FIBITMAP dib, string comment)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			bool result;
			if (comment != null)
			{
				FITAG tag = CreateTag();
				MetadataTag metadataTag = new MetadataTag(tag, FREE_IMAGE_MDMODEL.FIMD_COMMENTS);
				metadataTag.Value = comment;
				result = SetMetadata(FREE_IMAGE_MDMODEL.FIMD_COMMENTS, dib, "Comment", tag);
				DeleteTag(tag);
			}
			else
			{
				result = SetMetadata(FREE_IMAGE_MDMODEL.FIMD_COMMENTS, dib, "Comment", 0);
			}
			return result;
		}

		/// <summary>
		/// Retrieve a metadata attached to a FreeImage bitmap.
		/// </summary>
		/// <param name="model">The metadata model to look for.</param>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="key">The metadata field name.</param>
		/// <param name="tag">A <see cref="MetadataTag"/> structure returned by the function.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static bool GetMetadata(
			FREE_IMAGE_MDMODEL model,
			FIBITMAP dib,
			string key,
			out MetadataTag tag)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}

			FITAG _tag;
			bool result;
			if (GetMetadata(model, dib, key, out _tag))
			{
				tag = new MetadataTag(_tag, model);
				result = true;
			}
			else
			{
				tag = null;
				result = false;
			}
			return result;
		}

		/// <summary>
		/// Attach a new metadata tag to a FreeImage bitmap.
		/// </summary>
		/// <param name="model">The metadata model used to store the tag.</param>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="key">The tag field name.</param>
		/// <param name="tag">The <see cref="MetadataTag"/> to be attached.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static bool SetMetadata(
			FREE_IMAGE_MDMODEL model,
			FIBITMAP dib,
			string key,
			MetadataTag tag)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			return SetMetadata(model, dib, key, tag.tag);
		}

		/// <summary>
		/// Provides information about the first instance of a tag that matches the metadata model.
		/// </summary>
		/// <param name="model">The model to match.</param>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="tag">Tag that matches the metadata model.</param>
		/// <returns>Unique search handle that can be used to call FindNextMetadata or FindCloseMetadata.
		/// Null if the metadata model does not exist.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static FIMETADATA FindFirstMetadata(
			FREE_IMAGE_MDMODEL model,
			FIBITMAP dib,
			out MetadataTag tag)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			FITAG _tag;
			FIMETADATA result = FindFirstMetadata(model, dib, out _tag);
			if (result.IsNull)
			{
				tag = null;
				return result;
			}
			tag = new MetadataTag(_tag, model);
			if (metaDataSearchHandler.ContainsKey(result))
			{
				metaDataSearchHandler[result] = model;
			}
			else
			{
				metaDataSearchHandler.Add(result, model);
			}
			return result;
		}

		/// <summary>
		/// Find the next tag, if any, that matches the metadata model argument in a previous call
		/// to FindFirstMetadata, and then alters the tag object contents accordingly.
		/// </summary>
		/// <param name="mdhandle">Unique search handle provided by FindFirstMetadata.</param>
		/// <param name="tag">Tag that matches the metadata model.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		public static bool FindNextMetadata(FIMETADATA mdhandle, out MetadataTag tag)
		{
			FITAG _tag;
			bool result;
			if (FindNextMetadata(mdhandle, out _tag))
			{
				tag = new MetadataTag(_tag, metaDataSearchHandler[mdhandle]);
				result = true;
			}
			else
			{
				tag = null;
				result = false;
			}
			return result;
		}

		/// <summary>
		/// Closes the specified metadata search handle and releases associated resources.
		/// </summary>
		/// <param name="mdhandle">The handle to close.</param>
		public static void FindCloseMetadata(FIMETADATA mdhandle)
		{
			if (metaDataSearchHandler.ContainsKey(mdhandle))
			{
				metaDataSearchHandler.Remove(mdhandle);
			}
			FindCloseMetadata_(mdhandle);
		}

		/// <summary>
		/// This dictionary links FIMETADATA handles and FREE_IMAGE_MDMODEL models.
		/// </summary>
		private static Dictionary<FIMETADATA, FREE_IMAGE_MDMODEL> metaDataSearchHandler
			= new Dictionary<FIMETADATA, FREE_IMAGE_MDMODEL>(1);

		#endregion

		#region Rotation and flipping

		/// <summary>
		/// Rotates a 4-bit color FreeImage bitmap.
		/// Allowed values for <paramref name="angle"/> are 90, 180 and 270.
		/// In case <paramref name="angle"/> is 0 or 360 a clone is returned.
		/// 0 is returned for other values or in case the rotation fails.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <param name="angle">The angle of rotation.</param>
		/// <returns>Handle to a FreeImage bitmap.</returns>
		/// <remarks>
		/// This function is kind of temporary due to FreeImage's lack of
		/// rotating 4-bit images. It's particularly used by <see cref="FreeImageBitmap"/>'s
		/// method RotateFlip. This function will be removed as soon as FreeImage
		/// supports rotating 4-bit images.
		/// </remarks>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		public static unsafe FIBITMAP Rotate4bit(FIBITMAP dib, double angle)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}

			FIBITMAP result = 0;
			int ang = (int)angle;

			if ((GetImageType(dib) == FREE_IMAGE_TYPE.FIT_BITMAP) &&
				(GetBPP(dib) == 4) &&
				((ang % 90) == 0))
			{
				int width, height, xOrg, yOrg;
				Scanline<FI4BIT>[] src, dst;
				width = (int)GetWidth(dib);
				height = (int)GetHeight(dib);
				byte index = 0;
				switch (ang)
				{
					case 90:
						result = Allocate(height, width, 4, 0, 0, 0);
						if (result.IsNull)
						{
							break;
						}
						CopyPalette(dib, result);
						src = Get04BitScanlines(dib);
						dst = Get04BitScanlines(result);
						for (int y = 0; y < width; y++)
						{
							yOrg = height - 1;
							for (int x = 0; x < height; x++, yOrg--)
							{
								index = src[yOrg][y];
								dst[y][x] = index;
							}
						}
						break;
					case 180:
						result = Allocate(width, height, 4, 0, 0, 0);
						if (result.IsNull)
						{
							break;
						}
						CopyPalette(dib, result);
						src = Get04BitScanlines(dib);
						dst = Get04BitScanlines(result);

						yOrg = height - 1;
						for (int y = 0; y < height; y++, yOrg--)
						{
							xOrg = width - 1;
							for (int x = 0; x < width; x++, xOrg--)
							{
								index = src[yOrg][xOrg];
								dst[y][x] = index;
							}
						}
						break;
					case 270:
						result = Allocate(height, width, 4, 0, 0, 0);
						if (result.IsNull)
						{
							break;
						}
						CopyPalette(dib, result);
						src = Get04BitScanlines(dib);
						dst = Get04BitScanlines(result);
						xOrg = width - 1;
						for (int y = 0; y < width; y++, xOrg--)
						{
							for (int x = 0; x < height; x++)
							{
								index = src[x][xOrg];
								dst[y][x] = index;
							}
						}
						break;
					case 0:
					case 360:
						result = Clone(dib);
						break;
				}
			}
			return result;
		}

		#endregion

		#region Wrapper functions

		/// <summary>
		/// Returns the next higher possible color depth.
		/// </summary>
		/// <param name="bpp">Color depth to increase.</param>
		/// <returns>The next higher color depth or 0 if there is no valid color depth.</returns>
		internal static int GetNextColorDepth(int bpp)
		{
			int result = 0;
			switch (bpp)
			{
				case 1:
					result = 4;
					break;
				case 4:
					result = 8;
					break;
				case 8:
					result = 16;
					break;
				case 16:
					result = 24;
					break;
				case 24:
					result = 32;
					break;
			}
			return result;
		}

		/// <summary>
		/// Returns the next lower possible color depth.
		/// </summary>
		/// <param name="bpp">Color depth to decrease.</param>
		/// <returns>The next lower color depth or 0 if there is no valid color depth.</returns>
		internal static int GetPrevousColorDepth(int bpp)
		{
			int result = 0;
			switch (bpp)
			{
				case 32:
					result = 24;
					break;
				case 24:
					result = 16;
					break;
				case 16:
					result = 8;
					break;
				case 8:
					result = 4;
					break;
				case 4:
					result = 1;
					break;
			}
			return result;
		}

		/// <summary>
		/// Reads a null-terminated c-string.
		/// </summary>
		/// <param name="ptr">Pointer to the first char of the string.</param>
		/// <returns>The converted string.</returns>
		internal static unsafe string PtrToStr(byte* ptr)
		{
			string result = null;
			if (ptr != null)
			{
				System.Text.StringBuilder sb = new System.Text.StringBuilder();

				while (*ptr != 0)
				{
					sb.Append((char)(*(ptr++)));
				}
				result = sb.ToString();
			}
			return result;
		}

		internal static unsafe byte[] CreateShrunkenPaletteLUT(FIBITMAP dib, out int uniqueColors)
		{
			byte[] result = null;
			uniqueColors = 0;

			if ((!dib.IsNull) && (GetImageType(dib) == FREE_IMAGE_TYPE.FIT_BITMAP) && (GetBPP(dib) <= 8))
			{
				int size = (int)GetColorsUsed(dib);
				List<RGBQUAD> newPalette = new List<RGBQUAD>(size);
				List<byte> lut = new List<byte>(size);
				RGBQUAD* palette = (RGBQUAD*)GetPalette(dib);
				RGBQUAD color;
				int index;

				for (int i = 0; i < size; i++)
				{
					color = palette[i];
					color.rgbReserved = 255; // ignore alpha

					index = newPalette.IndexOf(color);
					if (index < 0)
					{
						newPalette.Add(color);
						lut.Add((byte)(newPalette.Count - 1));
					}
					else
					{
						lut.Add((byte)index);
					}
				}
				result = lut.ToArray();
				uniqueColors = newPalette.Count;
			}
			return result;
		}

		internal static PropertyItem CreatePropertyItem()
		{
			PropertyItem result = null;
			try
			{
				result = (PropertyItem)PropertyItemConstructor.Invoke(null);
			}
			catch
			{
			}
			return result;
		}

		private static unsafe void CopyPalette(FIBITMAP src, FIBITMAP dst)
		{
			RGBQUAD* orgPal = (RGBQUAD*)GetPalette(src);
			RGBQUAD* newPal = (RGBQUAD*)GetPalette(dst);
			uint size = (uint)(sizeof(RGBQUAD) * GetColorsUsed(src));
			MoveMemory(newPal, orgPal, size);
		}

		private static unsafe Scanline<FI4BIT>[] Get04BitScanlines(FIBITMAP dib)
		{
			int height = (int)GetHeight(dib);
			Scanline<FI4BIT>[] array = new Scanline<FI4BIT>[height];
			for (int i = 0; i < height; i++)
			{
				array[i] = new Scanline<FI4BIT>(dib, i);
			}
			return array;
		}

		/// <summary>
		/// Changes a bitmaps color depth.
		/// Used by SaveEx and SaveToStream
		/// </summary>
		private static FIBITMAP PrepareBitmapColorDepth(FIBITMAP dibToSave, FREE_IMAGE_FORMAT format, FREE_IMAGE_COLOR_DEPTH colorDepth)
		{
			int bpp = (int)GetBPP(dibToSave);
			int targetBpp = (int)(colorDepth & FREE_IMAGE_COLOR_DEPTH.FICD_COLOR_MASK);

			if (colorDepth != FREE_IMAGE_COLOR_DEPTH.FICD_AUTO)
			{
				// A fix colordepth was chosen
				if (FIFSupportsExportBPP(format, targetBpp))
				{
					dibToSave = ConvertColorDepth(dibToSave, colorDepth, false);
				}
				else
				{
					throw new ArgumentException("FreeImage\n\nFreeImage Library plugin " +
						GetFormatFromFIF(format) + " is unable to write images with a color depth of " +
						targetBpp + " bpp.");
				}
			}
			else
			{
				// Auto selection was chosen
				if (!FIFSupportsExportBPP(format, bpp))
				{
					// The color depth is not supported
					int bppUpper = bpp;
					int bppLower = bpp;
					// Check from the bitmaps current color depth in both directions
					do
					{
						bppUpper = GetNextColorDepth(bppUpper);
						if (FIFSupportsExportBPP(format, bppUpper))
						{
							dibToSave = ConvertColorDepth(dibToSave, (FREE_IMAGE_COLOR_DEPTH)bppUpper, false);
							break;
						}
						bppLower = GetPrevousColorDepth(bppLower);
						if (FIFSupportsExportBPP(format, bppLower))
						{
							dibToSave = ConvertColorDepth(dibToSave, (FREE_IMAGE_COLOR_DEPTH)bppLower, false);
							break;
						}
					} while (!((bppLower == 0) && (bppUpper == 0)));
				}
			}
			return dibToSave;
		}

		/// <summary>
		/// Compares blocks of memory.
		/// </summary>
		/// <param name="buf1">Pointer to a block of memory to compare.</param>
		/// <param name="buf2">Pointer to a block of memory to compare.</param>
		/// <param name="length">Specifies the number of bytes to be compared.</param>
		/// <returns>If all bytes compare as equal, true is returned.</returns>
		public static unsafe bool CompareMemory(void* buf1, void* buf2, uint length)
		{
			return (length == RtlCompareMemory(buf1, buf2, length));
		}

		/// <summary>
		/// Compares blocks of memory.
		/// </summary>
		/// <param name="buf1">Pointer to a block of memory to compare.</param>
		/// <param name="buf2">Pointer to a block of memory to compare.</param>
		/// <param name="length">Specifies the number of bytes to be compared.</param>
		/// <returns>If all bytes compare as equal, true is returned.</returns>
		public static unsafe bool CompareMemory(void* buf1, void* buf2, long length)
		{
			return (length == RtlCompareMemory(buf1, buf2, checked((uint)length)));
		}

		/// <summary>
		/// Compares blocks of memory.
		/// </summary>
		/// <param name="buf1">Pointer to a block of memory to compare.</param>
		/// <param name="buf2">Pointer to a block of memory to compare.</param>
		/// <param name="length">Specifies the number of bytes to be compared.</param>
		/// <returns>If all bytes compare as equal, true is returned.</returns>
		public static unsafe bool CompareMemory(IntPtr buf1, IntPtr buf2, uint length)
		{
			return (length == RtlCompareMemory(buf1.ToPointer(), buf2.ToPointer(), length));
		}

		/// <summary>
		/// Compares blocks of memory.
		/// </summary>
		/// <param name="buf1">Pointer to a block of memory to compare.</param>
		/// <param name="buf2">Pointer to a block of memory to compare.</param>
		/// <param name="length">Specifies the number of bytes to be compared.</param>
		/// <returns>If all bytes compare as equal, true is returned.</returns>
		public static unsafe bool CompareMemory(IntPtr buf1, IntPtr buf2, long length)
		{
			return (length == RtlCompareMemory(buf1.ToPointer(), buf2.ToPointer(), checked((uint)length)));
		}

		/// <summary>
		/// Moves a block of memory from one location to another.
		/// </summary>
		/// <param name="dst">Pointer to the starting address of the move destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be moved.</param>
		/// <param name="size">Size of the block of memory to move, in bytes.</param>
		public static unsafe void MoveMemory(void* dst, void* src, long size)
		{
			MoveMemory(dst, src, checked((uint)size));
		}

		/// <summary>
		/// Moves a block of memory from one location to another.
		/// </summary>
		/// <param name="dst">Pointer to the starting address of the move destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be moved.</param>
		/// <param name="size">Size of the block of memory to move, in bytes.</param>
		public static unsafe void MoveMemory(IntPtr dst, IntPtr src, uint size)
		{
			MoveMemory(dst.ToPointer(), src.ToPointer(), size);
		}

		/// <summary>
		/// Moves a block of memory from one location to another.
		/// </summary>
		/// <param name="dst">Pointer to the starting address of the move destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be moved.</param>
		/// <param name="size">Size of the block of memory to move, in bytes.</param>
		public static unsafe void MoveMemory(IntPtr dst, IntPtr src, long size)
		{
			MoveMemory(dst.ToPointer(), src.ToPointer(), checked((uint)size));
		}

		/// <summary>
		/// Copies a block of memory from one location to another.
		/// </summary>
		/// <param name="dest">Pointer to the starting address of the copy destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be copied.</param>
		/// <param name="len">Size of the block of memory to copy, in bytes.</param>
		/// <remarks>
		/// <b>CopyMemory</b> runs faster than <see cref="MoveMemory(void*, void*, uint)"/>.
		/// However, if both blocks overlap the result is undefined.
		/// </remarks>
		public static unsafe void CopyMemory(byte* dest, byte* src, int len)
		{
			if (len >= 0x10)
			{
				do
				{
					*((int*)dest) = *((int*)src);
					*((int*)(dest + 4)) = *((int*)(src + 4));
					*((int*)(dest + 8)) = *((int*)(src + 8));
					*((int*)(dest + 12)) = *((int*)(src + 12));
					dest += 0x10;
					src += 0x10;
				}
				while ((len -= 0x10) >= 0x10);
			}
			if (len > 0)
			{
				if ((len & 8) != 0)
				{
					*((int*)dest) = *((int*)src);
					*((int*)(dest + 4)) = *((int*)(src + 4));
					dest += 8;
					src += 8;
				}
				if ((len & 4) != 0)
				{
					*((int*)dest) = *((int*)src);
					dest += 4;
					src += 4;
				}
				if ((len & 2) != 0)
				{
					*((short*)dest) = *((short*)src);
					dest += 2;
					src += 2;
				}
				if ((len & 1) != 0)
				{
					*dest = *src;
				}
			}
		}

		/// <summary>
		/// Copies a block of memory from one location to another.
		/// </summary>
		/// <param name="dest">Pointer to the starting address of the copy destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be copied.</param>
		/// <param name="size">Size of the block of memory to copy, in bytes.</param>
		/// <remarks>
		/// <b>CopyMemory</b> runs faster than <see cref="MoveMemory(void*, void*, long)"/>.
		/// However, if both blocks overlap the result is undefined.
		/// </remarks>
		public static unsafe void CopyMemory(void* dest, void* src, long size)
		{
			CopyMemory((byte*)dest, (byte*)src, checked((int)size));
		}

		/// <summary>
		/// Copies a block of memory from one location to another.
		/// </summary>
		/// <param name="dest">Pointer to the starting address of the copy destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be copied.</param>
		/// <param name="size">Size of the block of memory to copy, in bytes.</param>
		/// <remarks>
		/// <b>CopyMemory</b> runs faster than <see cref="MoveMemory(void*, void*, long)"/>.
		/// However, if both blocks overlap the result is undefined.
		/// </remarks>
		public static unsafe void CopyMemory(void* dest, void* src, int size)
		{
			CopyMemory((byte*)dest, (byte*)src, size);
		}

		/// <summary>
		/// Copies a block of memory from one location to another.
		/// </summary>
		/// <param name="dest">Pointer to the starting address of the copy destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be copied.</param>
		/// <param name="size">Size of the block of memory to copy, in bytes.</param>
		/// <remarks>
		/// <b>CopyMemory</b> runs faster than <see cref="MoveMemory(IntPtr, IntPtr, uint)"/>.
		/// However, if both blocks overlap the result is undefined.
		/// </remarks>
		public static unsafe void CopyMemory(IntPtr dest, IntPtr src, int size)
		{
			CopyMemory((byte*)dest, (byte*)src, size);
		}

		/// <summary>
		/// Copies a block of memory from one location to another.
		/// </summary>
		/// <param name="dest">Pointer to the starting address of the copy destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be copied.</param>
		/// <param name="size">Size of the block of memory to copy, in bytes.</param>
		/// <remarks>
		/// <b>CopyMemory</b> runs faster than <see cref="MoveMemory(IntPtr, IntPtr, long)"/>.
		/// However, if both blocks overlap the result is undefined.
		/// </remarks>
		public static unsafe void CopyMemory(IntPtr dest, IntPtr src, long size)
		{
			CopyMemory((byte*)dest, (byte*)src, checked((int)size));
		}

		internal static string ColorToString(Color color)
		{
			return string.Format(
				System.Globalization.CultureInfo.CurrentCulture,
				"{{Name={0}, ARGB=({1}, {2}, {3}, {4})}}",
				new object[] { color.Name, color.A, color.R, color.G, color.B });
		}

		#endregion

		#region Dll-Imports

		/// <summary>
		/// Retrieves a handle to a display device context (DC) for the client area of a specified window
		/// or for the entire screen. You can use the returned handle in subsequent GDI functions to draw in the DC.
		/// </summary>
		/// <param name="hWnd">Handle to the window whose DC is to be retrieved.
		/// If this value is IntPtr.Zero, GetDC retrieves the DC for the entire screen. </param>
		/// <returns>If the function succeeds, the return value is a handle to the DC for the specified window's client area.
		/// If the function fails, the return value is NULL.</returns>
		[DllImport("user32.dll")]
		private static extern IntPtr GetDC(IntPtr hWnd);

		/// <summary>
		/// Releases a device context (DC), freeing it for use by other applications.
		/// The effect of the ReleaseDC function depends on the type of DC. It frees only common and window DCs.
		/// It has no effect on class or private DCs.
		/// </summary>
		/// <param name="hWnd">Handle to the window whose DC is to be released.</param>
		/// <param name="hDC">Handle to the DC to be released.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		[DllImport("user32.dll")]
		private static extern bool ReleaseDC(IntPtr hWnd, IntPtr hDC);

		/// <summary>
		/// Creates a DIB that applications can write to directly.
		/// The function gives you a pointer to the location of the bitmap bit values.
		/// You can supply a handle to a file-mapping object that the function will use to create the bitmap,
		/// or you can let the system allocate the memory for the bitmap.
		/// </summary>
		/// <param name="hdc">Handle to a device context.</param>
		/// <param name="pbmi">Pointer to a BITMAPINFO structure that specifies various attributes of the DIB,
		/// including the bitmap dimensions and colors.</param>
		/// <param name="iUsage">Specifies the type of data contained in the bmiColors array member of the BITMAPINFO structure
		/// pointed to by pbmi (either logical palette indexes or literal RGB values).</param>
		/// <param name="ppvBits">Pointer to a variable that receives a pointer to the location of the DIB bit values.</param>
		/// <param name="hSection">Handle to a file-mapping object that the function will use to create the DIB.
		/// This parameter can be NULL.</param>
		/// <param name="dwOffset">Specifies the offset from the beginning of the file-mapping object referenced by hSection
		/// where storage for the bitmap bit values is to begin. This value is ignored if hSection is NULL.</param>
		/// <returns>If the function succeeds, the return value is a handle to the newly created DIB,
		/// and *ppvBits points to the bitmap bit values. If the function fails, the return value is NULL, and *ppvBits is NULL.</returns>
		[DllImport("gdi32.dll")]
		private static extern IntPtr CreateDIBSection(
			IntPtr hdc,
			[In] IntPtr pbmi,
			uint iUsage,
			out IntPtr ppvBits,
			IntPtr hSection,
			uint dwOffset);

		/// <summary>
		/// Deletes a logical pen, brush, font, bitmap, region, or palette, freeing all system resources associated with the object.
		/// After the object is deleted, the specified handle is no longer valid.
		/// </summary>
		/// <param name="hObject">Handle to a logical pen, brush, font, bitmap, region, or palette.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		[DllImport("gdi32.dll")]
		private static extern bool DeleteObject(IntPtr hObject);

		/// <summary>
		/// Creates a compatible bitmap (DDB) from a DIB and, optionally, sets the bitmap bits.
		/// </summary>
		/// <param name="hdc">Handle to a device context.</param>
		/// <param name="lpbmih">Pointer to a bitmap information header structure.</param>
		/// <param name="fdwInit">Specifies how the system initializes the bitmap bits - (use 4).</param>
		/// <param name="lpbInit">Pointer to an array of bytes containing the initial bitmap data.</param>
		/// <param name="lpbmi">Pointer to a BITMAPINFO structure that describes the dimensions
		/// and color format of the array pointed to by the lpbInit parameter.</param>
		/// <param name="fuUsage">Specifies whether the bmiColors member of the BITMAPINFO structure
		/// was initialized - (use 0).</param>
		/// <returns>Handle to a DIB or null on failure.</returns>
		[DllImport("gdi32.dll")]
		private static extern IntPtr CreateDIBitmap(
			IntPtr hdc,
			IntPtr lpbmih,
			uint fdwInit,
			IntPtr lpbInit,
			IntPtr lpbmi,
			uint fuUsage);

		/// <summary>
		/// Retrieves information for the specified graphics object.
		/// </summary>
		/// <param name="hgdiobj">Handle to the graphics object of interest.</param>
		/// <param name="cbBuffer">Specifies the number of bytes of information to
		/// be written to the buffer.</param>
		/// <param name="lpvObject">Pointer to a buffer that receives the information
		/// about the specified graphics object.</param>
		/// <returns>0 on failure.</returns>
		[DllImport("gdi32.dll")]
		private static extern int GetObject(IntPtr hgdiobj, int cbBuffer, IntPtr lpvObject);

		/// <summary>
		/// Retrieves the bits of the specified compatible bitmap and copies them into a buffer
		/// as a DIB using the specified format.
		/// </summary>
		/// <param name="hdc">Handle to the device context.</param>
		/// <param name="hbmp">Handle to the bitmap. This must be a compatible bitmap (DDB).</param>
		/// <param name="uStartScan">Specifies the first scan line to retrieve.</param>
		/// <param name="cScanLines">Specifies the number of scan lines to retrieve.</param>
		/// <param name="lpvBits">Pointer to a buffer to receive the bitmap data.</param>
		/// <param name="lpbmi">Pointer to a BITMAPINFO structure that specifies the desired
		/// format for the DIB data.</param>
		/// <param name="uUsage">Specifies the format of the bmiColors member of the
		/// BITMAPINFO structure - (use 0).</param>
		/// <returns>0 on failure.</returns>
		[DllImport("gdi32.dll")]
		private static extern unsafe int GetDIBits(
			IntPtr hdc,
			IntPtr hbmp,
			uint uStartScan,
			uint cScanLines,
			IntPtr lpvBits,
			IntPtr lpbmi,
			uint uUsage);

		/// <summary>
		/// Moves a block of memory from one location to another.
		/// </summary>
		/// <param name="dst">Pointer to the starting address of the move destination.</param>
		/// <param name="src">Pointer to the starting address of the block of memory to be moved.</param>
		/// <param name="size">Size of the block of memory to move, in bytes.</param>
		[DllImport("Kernel32.dll", EntryPoint = "RtlMoveMemory", SetLastError = false)]
		public static unsafe extern void MoveMemory(void* dst, void* src, uint size);

		/// <summary>
		/// The RtlCompareMemory routine compares blocks of memory
		/// and returns the number of bytes that are equivalent.
		/// </summary>
		/// <param name="buf1">Pointer to a block of memory to compare.</param>
		/// <param name="buf2">Pointer to a block of memory to compare.</param>
		/// <param name="count">Specifies the number of bytes to be compared.</param>
		/// <returns>RtlCompareMemory returns the number of bytes that compare as equal.
		/// If all bytes compare as equal, the input Length is returned.</returns>
		[DllImport("ntdll.dll", EntryPoint = "RtlCompareMemory", SetLastError = false)]
		internal static unsafe extern uint RtlCompareMemory(void* buf1, void* buf2, uint count);

		#endregion
	}
}