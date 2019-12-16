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
// $Revision: 1.2 $
// $Date: 2008/06/16 15:17:37 $
// $Id: FIMETADATA.cs,v 1.2 2008/06/16 15:17:37 cklein05 Exp $
// ==========================================================

using System;
using System.Runtime.InteropServices;

namespace FreeImageAPI
{
	/// <summary>
	/// The <b>FIMETADATA</b> structure is an unique search handle for metadata search operations.
	/// </summary>
	/// <remarks>
	/// The <b>FIMETADATA</b> structure is usually returned by the
	/// <see cref="FreeImageAPI.FreeImage.FindFirstMetadata(FREE_IMAGE_MDMODEL, FIBITMAP, out FITAG)"/>
	/// function and then used on subsequent calls to
	/// <see cref="FreeImageAPI.FreeImage.FindNextMetadata(FIMETADATA, out FITAG)"/>.
	/// When the <b>FIMETADATA</b> handle is no longer used, it needs to be freed by the
	/// <see cref="FreeImageAPI.FreeImage.FindCloseMetadata(FIMETADATA)"/> function.
	/// </remarks>
	[Serializable, StructLayout(LayoutKind.Sequential)]
	public struct FIMETADATA : IComparable, IComparable<FIMETADATA>, IEquatable<FIMETADATA>
	{
		private IntPtr data;

		/// <summary>
		/// Initializes a new instance of the <see cref="FIMETADATA"/> structure to the value indicated by
		/// a specified pointer to a native <see cref="FIMETADATA"/> structure.
		/// </summary>
		/// <param name="ptr">A pointer to a native <see cref="FIMETADATA"/> structure.</param>
		public FIMETADATA(int ptr)
		{
			data = new IntPtr(ptr);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="FIMETADATA"/> structure to the value indicated by
		/// a specified pointer to a native <see cref="FIMETADATA"/> structure.
		/// </summary>
		/// <param name="ptr">A pointer to a native <see cref="FIMETADATA"/> structure.</param>
		public FIMETADATA(IntPtr ptr)
		{
			data = ptr;
		}

		/// <summary>
		/// Tests whether two specified <see cref="FIMETADATA"/> structures are equivalent.
		/// </summary>
		/// <param name="left">The <see cref="FIMETADATA"/> that is to the left of the equality operator.</param>
		/// <param name="right">The <see cref="FIMETADATA"/> that is to the right of the equality operator.</param>
		/// <returns>
		/// <b>true</b> if the two <see cref="FIMETADATA"/> structures are equal; otherwise, <b>false</b>.
		/// </returns>
		public static bool operator ==(FIMETADATA left, FIMETADATA right)
		{
			return (left.data == right.data);
		}

		/// <summary>
		/// Tests whether two specified <see cref="FIMETADATA"/> structures are different.
		/// </summary>
		/// <param name="left">The <see cref="FIMETADATA"/> that is to the left of the inequality operator.</param>
		/// <param name="right">The <see cref="FIMETADATA"/> that is to the right of the inequality operator.</param>
		/// <returns>
		/// <b>true</b> if the two <see cref="FIMETADATA"/> structures are different; otherwise, <b>false</b>.
		/// </returns>
		public static bool operator !=(FIMETADATA left, FIMETADATA right)
		{
			return (left.data != right.data);
		}

		/// <summary>
		/// Converts the pointer specified in <paramref name="ptr"/> to a <see cref="FIMETADATA"/> structure.
		/// </summary>
		/// <param name="ptr">A 32-bit value to be converted into a <see cref="FIMETADATA"/> structure.</param>
		/// <returns>A <see cref="FIMETADATA"/> structure initialized with the specified pointer.</returns>
		public static implicit operator FIMETADATA(int ptr)
		{
			return new FIMETADATA(ptr);
		}

		/// <summary>
		/// Converts the <see cref="FIMETADATA"/> structure specified in <paramref name="handle"/> to a 32-bit value.
		/// </summary>
		/// <param name="handle">A <see cref="FIMETADATA"/> structure to be converted into a 32-bit value.</param>
		/// <returns>A 32-bit value initialized with the pointer of the <see cref="FIMETADATA"/> structure.</returns>
		public static implicit operator int(FIMETADATA handle)
		{
			return handle.data.ToInt32();
		}

		/// <summary>
		/// Converts the pointer specified in <paramref name="ptr"/> to a <see cref="FIMETADATA"/> structure.
		/// </summary>
		/// <param name="ptr">A 32-bit value to be converted into a <see cref="FIMETADATA"/> structure.</param>
		/// <returns>A <see cref="FIMETADATA"/> structure initialized with the specified pointer.</returns>
		public static implicit operator FIMETADATA(IntPtr ptr)
		{
			return new FIMETADATA(ptr);
		}

		/// <summary>
		/// Converts the <see cref="FIMETADATA"/> structure specified in <paramref name="handle"/> to an IntPtr.
		/// </summary>
		/// <param name="handle">A <see cref="FIMETADATA"/> structure to be converted into an IntPtr.</param>
		/// <returns>An IntPtr initialized with the pointer of the <see cref="FIMETADATA"/> structure.</returns>
		public static implicit operator IntPtr(FIMETADATA handle)
		{
			return handle.data;
		}

		/// <summary>
		/// Gets whether the pointer is a null pointer or not.
		/// </summary>
		/// <value><b>true</b> if this <see cref="FIMETADATA"/> is a null pointer;
		/// otherwise, <b>false</b>.</value>		
		public bool IsNull
		{
			get
			{
				return (data == IntPtr.Zero);
			}
		}

		/// <summary>
		/// Converts the numeric value of the <see cref="FIMETADATA"/> object
		/// to its equivalent string representation.
		/// </summary>
		/// <returns>The string representation of the value of this instance.</returns>
		public override string ToString()
		{
			return data.ToString();
		}

		/// <summary>
		/// Returns a hash code for this <see cref="FIMETADATA"/> structure.
		/// </summary>
		/// <returns>An integer value that specifies the hash code for this <see cref="FIMETADATA"/>.</returns>
		public override int GetHashCode()
		{
			return data.GetHashCode();
		}

		/// <summary>
		/// Determines whether the specified <see cref="Object"/> is equal to the current <see cref="Object"/>.
		/// </summary>
		/// <param name="obj">The <see cref="Object"/> to compare with the current <see cref="Object"/>.</param>
		/// <returns><b>true</b> if the specified <see cref="Object"/> is equal to the current <see cref="Object"/>; otherwise, <b>false</b>.</returns>
		public override bool Equals(object obj)
		{
			return ((obj is FIMETADATA) && (this == ((FIMETADATA)obj)));
		}

		/// <summary>
		/// Indicates whether the current object is equal to another object of the same type.
		/// </summary>
		/// <param name="other">An object to compare with this object.</param>
		/// <returns><b>true</b> if the current object is equal to the other parameter; otherwise, <b>false</b>.</returns>
		public bool Equals(FIMETADATA other)
		{
			return (this == other);
		}

		/// <summary>
		/// Compares this instance with a specified <see cref="Object"/>.
		/// </summary>
		/// <param name="obj">An object to compare with this instance.</param>
		/// <returns>A 32-bit signed integer indicating the lexical relationship between the two comparands.</returns>
		/// <exception cref="ArgumentException"><paramref name="obj"/> is not a <see cref="FIMETADATA"/>.</exception>
		public int CompareTo(object obj)
		{
			if (obj == null)
			{
				return 1;
			}
			if (!(obj is FIMETADATA))
			{
				throw new ArgumentException();
			}
			return CompareTo((FIMETADATA)obj);
		}

		/// <summary>
		/// Compares this instance with a specified <see cref="FIMETADATA"/> object.
		/// </summary>
		/// <param name="other">A <see cref="FIMETADATA"/> to compare.</param>
		/// <returns>A signed number indicating the relative values of this instance
		/// and <paramref name="other"/>.</returns>
		public int CompareTo(FIMETADATA other)
		{
			return this.data.ToInt64().CompareTo(other.data.ToInt64());
		}
	}
}