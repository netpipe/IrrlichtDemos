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
// $Revision: 1.3 $
// $Date: 2008/06/17 13:48:47 $
// $Id: MetadataModels.cs,v 1.3 2008/06/17 13:48:47 cklein05 Exp $
// ==========================================================

namespace FreeImageAPI.Metadata
{
	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_ANIMATION"/>.
	/// </summary>
	public sealed class MDM_ANIMATION : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_ANIMATION(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_ANIMATION; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_COMMENTS"/>.
	/// </summary>
	public sealed class MDM_COMMENTS : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_COMMENTS(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_COMMENTS; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_CUSTOM"/>.
	/// </summary>
	public sealed class MDM_CUSTOM : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_CUSTOM(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_CUSTOM; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_EXIF_EXIF"/>.
	/// </summary>
	public sealed class MDM_EXIF_EXIF : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_EXIF_EXIF(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_EXIF_EXIF; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_EXIF_GPS"/>.
	/// </summary>
	public sealed class MDM_EXIF_GPS : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_EXIF_GPS(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_EXIF_GPS; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_EXIF_INTEROP"/>.
	/// </summary>
	public sealed class MDM_INTEROP : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_INTEROP(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_EXIF_INTEROP; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_EXIF_MAIN"/>.
	/// </summary>
	public class MDM_MAIN : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_MAIN(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_EXIF_MAIN; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_EXIF_MAKERNOTE"/>.
	/// </summary>
	public sealed class MDM_MAKERNOTE : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_MAKERNOTE(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_EXIF_MAKERNOTE; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_GEOTIFF"/>.
	/// </summary>
	public sealed class MDM_GEOTIFF : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_GEOTIFF(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_GEOTIFF; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_IPTC"/>.
	/// </summary>
	public sealed class MDM_IPTC : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_IPTC(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_IPTC; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_NODATA"/>.
	/// </summary>
	public sealed class MDM_NODATA : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_NODATA(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_NODATA; }
		}
	}

	/// <summary>
	/// Represents a collection of all tags contained in the metadata model <see cref="FREE_IMAGE_MDMODEL.FIMD_XMP"/>.
	/// </summary>
	public sealed class MDM_XMP : MetadataModel
	{
		/// <summary>
		/// Initializes a new instance of this class.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public MDM_XMP(FIBITMAP dib) : base(dib) { }

		/// <summary>
		/// Retrieves the datamodel that this instance represents.
		/// </summary>
		public override FREE_IMAGE_MDMODEL Model
		{
			get { return FREE_IMAGE_MDMODEL.FIMD_XMP; }
		}
	}
}