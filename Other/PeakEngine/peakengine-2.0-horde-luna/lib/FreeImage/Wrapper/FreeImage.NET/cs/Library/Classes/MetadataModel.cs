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
// $Revision: 1.4 $
// $Date: 2008/06/17 13:48:47 $
// $Id: MetadataModel.cs,v 1.4 2008/06/17 13:48:47 cklein05 Exp $
// ==========================================================

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace FreeImageAPI.Metadata
{
	/// <summary>
	/// Base class that represents a collection of all tags contained in a metadata model.
	/// </summary>
	/// <remarks>
	/// The <b>MetedataModel</b> class is an abstract base class, which is inherited by
	/// several derived classes, one for each existing metadata model.
	/// </remarks> 
	public abstract class MetadataModel : IEnumerable
	{
		/// <summary>
		/// Handle to a FreeImage-bitmap.
		/// </summary>
		private readonly FIBITMAP dib;

		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="dib"/> is null.</exception>
		protected MetadataModel(FIBITMAP dib)
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException("dib");
			}
			this.dib = dib;
		}

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public abstract FREE_IMAGE_MDMODEL Model
		{
			get;
		}

		/// <summary>
		/// Adds new tag to the bitmap or updates its value in case it already exists.
		/// <see cref="FreeImageAPI.Metadata.MetadataTag.Key"/> will be used as key.
		/// </summary>
		/// <param name="tag">The tag to add or update.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="tag"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// The tags model differs from this instances model.</exception>
		public bool AddTag(MetadataTag tag)
		{
			if (tag == null)
			{
				throw new ArgumentNullException("tag");
			}
			if (tag.Model != Model)
			{
				throw new ArgumentException("tag.Model");
			}
			return tag.AddToImage(dib);
		}

		/// <summary>
		/// Adds a list of tags to the bitmap or updates their values in case they already exist.
		/// <see cref="FreeImageAPI.Metadata.MetadataTag.Key"/> will be used as key.
		/// </summary>
		/// <param name="list">A list of tags to add or update.</param>
		/// <returns>Returns the number of successfully added tags.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="list"/> is null.</exception>
		public int AddTag(IEnumerable<MetadataTag> list)
		{
			if (list == null)
			{
				throw new ArgumentNullException("list");
			}
			int count = 0;
			foreach (MetadataTag tag in list)
			{
				if (tag.Model == Model && tag.AddToImage(dib))
				{
					count++;
				}
			}
			return count;
		}

		/// <summary>
		/// Removes the specified tag from the bitmap.
		/// </summary>
		/// <param name="key">The key of the tag.</param>
		/// <returns>Returns true on success, false on failure.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="key"/> is null.</exception>
		public bool RemoveTag(string key)
		{
			if (key == null)
			{
				throw new ArgumentNullException("key");
			}
			return FreeImage.SetMetadata(Model, dib, key, 0);
		}

		/// <summary>
		/// Destroys the metadata model
		/// which will remove all tags of this model from the bitmap.
		/// </summary>
		/// <returns>Returns true on success, false on failure.</returns>
		public bool DestoryModel()
		{
			return FreeImage.SetMetadata(Model, dib, null, 0);
		}

		/// <summary>
		/// Returns the specified metadata tag.
		/// </summary>
		/// <param name="key">The key of the tag.</param>
		/// <returns>The metadata tag.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="key"/> is null.</exception>
		public MetadataTag GetTag(string key)
		{
			if (key == null)
			{
				throw new ArgumentNullException("key");
			}
			MetadataTag tag;
			return FreeImage.GetMetadata(Model, dib, key, out tag) ? tag : null;
		}

		/// <summary>
		/// Returns whether the specified tag exists.
		/// </summary>
		/// <param name="key">The key of the tag.</param>
		/// <returns>True in case the tag exists, else false.</returns>
		/// <exception cref="ArgumentNullException">
		/// <paramref name="key"/> is null.</exception>
		public bool TagExists(string key)
		{
			if (key == null)
			{
				throw new ArgumentNullException("key");
			}
			MetadataTag tag;
			return FreeImage.GetMetadata(Model, dib, key, out tag);
		}

		/// <summary>
		/// Returns a list of all metadata tags this instance represents.
		/// </summary>
		public List<MetadataTag> List
		{
			get
			{
				List<MetadataTag> list = new List<MetadataTag>((int)FreeImage.GetMetadataCount(Model, dib));
				MetadataTag tag;
				FIMETADATA mdHandle = FreeImage.FindFirstMetadata(Model, dib, out tag);
				if (!mdHandle.IsNull)
				{
					do
					{
						list.Add(tag);
					}
					while (FreeImage.FindNextMetadata(mdHandle, out tag));
					FreeImage.FindCloseMetadata(mdHandle);
				}
				return list;
			}
		}

		/// <summary>
		/// Returns the tag at the given index.
		/// </summary>
		/// <param name="index">Index of the tag to return.</param>
		/// <returns>The tag at the given index.</returns>
		protected MetadataTag GetTagFromIndex(int index)
		{
			if (index >= Count || index < 0)
			{
				throw new ArgumentOutOfRangeException("index");
			}
			MetadataTag tag;
			int count = 0;
			FIMETADATA mdHandle = FreeImage.FindFirstMetadata(Model, dib, out tag);
			if (!mdHandle.IsNull)
			{
				try
				{
					do
					{
						if (count++ == index)
						{
							break;
						}
					}
					while (FreeImage.FindNextMetadata(mdHandle, out tag));
				}
				finally
				{
					FreeImage.FindCloseMetadata(mdHandle);
				}				
			}
			return tag;
		}

		/// <summary>
		/// Returns the metadata tag at the given index. This operation is slow when accessing all tags.
		/// </summary>
		/// <param name="index">Index of the tag.</param>
		/// <returns>The metadata tag.</returns>
		/// <exception cref="ArgumentOutOfRangeException">
		/// <paramref name="index"/> is greater or equal <b>Count</b>
		/// or index is less than zero.</exception>
		public MetadataTag this[int index]
		{
			get
			{
				return GetTagFromIndex(index);
			}
		}

		/// <summary>
		/// Retrieves an object that can iterate through the individual MetadataTags in this MetadataModel.
		/// </summary>
		/// <returns>An <see cref="IEnumerator"/> for the
		/// <see cref="FreeImageAPI.Metadata.MetadataModel"/>.</returns>
		public IEnumerator GetEnumerator()
		{
			return List.GetEnumerator();
		}

		/// <summary>
		/// Returns the number of metadata tags this instance represents.
		/// </summary>
		public int Count
		{
			get { return (int)FreeImage.GetMetadataCount(Model, dib); }
		}

		/// <summary>
		/// Returns whether this model exists in the bitmaps metadata structure.
		/// </summary>
		public bool Exists
		{
			get
			{
				return Count > 0;
			}
		}

		/// <summary>
		/// Searches for a pattern in each metadata tag and returns the result as a list.
		/// </summary>
		/// <param name="searchPattern">The regular expression to use for the search.</param>
		/// <param name="flags">A bitfield that controls which fields should be searched in.</param>
		/// <returns>A list containing all found metadata tags.</returns>
		/// <exception cref="ArgumentNullException">
		/// <typeparamref name="searchPattern"/> is null.</exception>
		/// <exception cref="ArgumentException">
		/// <typeparamref name="searchPattern"/> is empty.</exception>
		public List<MetadataTag> RegexSearch(string searchPattern, MD_SEARCH_FLAGS flags)
		{
			if (searchPattern == null)
			{
				throw new ArgumentNullException("searchString");
			}
			if (searchPattern.Length == 0)
			{
				throw new ArgumentException("searchString is empty");
			}
			List<MetadataTag> result = new List<MetadataTag>(Count);
			Regex regex = new Regex(searchPattern);
			List<MetadataTag> list = List;
			foreach (MetadataTag tag in list)
			{
				if (((flags & MD_SEARCH_FLAGS.KEY) > 0) && regex.Match(tag.Key).Success)
				{
					result.Add(tag);
					continue;
				}
				if (((flags & MD_SEARCH_FLAGS.DESCRIPTION) > 0) && regex.Match(tag.Description).Success)
				{
					result.Add(tag);
					continue;
				}
				if (((flags & MD_SEARCH_FLAGS.TOSTRING) > 0) && regex.Match(tag.ToString()).Success)
				{
					result.Add(tag);
					continue;
				}
			}
			result.Capacity = result.Count;
			return result;
		}

		/// <summary>
		/// Converts the model of the MetadataModel object to its equivalent string representation.
		/// </summary>
		/// <returns>The string representation of the value of this instance.</returns>
		public override string ToString()
		{
			return Model.ToString();
		}
	}
}