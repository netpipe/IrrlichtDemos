using System;

namespace FreeImageAPI
{
	/// <summary>
	/// Class handling non-bitmap related functions.
	/// </summary>
	public static class FreeImageEngine
	{
		/// <summary>
		/// Gets a value indicating if the FreeImage DLL is available or not.
		/// </summary>
		public static bool IsAvailable
		{
			get
			{
				return FreeImage.IsAvailable();
			}
		}

		/// <summary>
		/// Internal errors in FreeImage generate a logstring that can be
		/// captured by this event.
		/// </summary>
		public static event OutputMessageFunction Message
		{
			add
			{
				FreeImage.Message += value;
			}
			remove
			{
				FreeImage.Message -= value;
			}
		}

		/// <summary>
		/// Gets a string containing the current version of the library.
		/// </summary>
		public static string Version
		{
			get
			{
				return FreeImage.GetVersion();
			}
		}

		/// <summary>
		/// Gets a string containing a standard copyright message.
		/// </summary>
		public static string CopyrightMessage
		{
			get
			{
				return FreeImage.GetCopyrightMessage();
			}
		}

		/// <summary>
		/// Gets whether the platform is using Little Endian.
		/// </summary>
		public static bool IsLittleEndian
		{
			get
			{
				return FreeImage.IsLittleEndian();
			}
		}
	}
}