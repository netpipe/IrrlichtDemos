using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;

namespace FreeImageAPI
{
	/// <summary>
	/// Provides methods for working with the standard bitmap palette.
	/// </summary>
	public sealed class Palette : MemoryArray<RGBQUAD>
	{
		/// <summary>
		/// Initializes a new instance for the given FreeImage bitmap.
		/// </summary>
		/// <param name="dib">Handle to a FreeImage bitmap.</param>
		public Palette(FIBITMAP dib)
			: base(FreeImage.GetPalette(dib), (int)FreeImage.GetColorsUsed(dib))
		{
			if (dib.IsNull)
			{
				throw new ArgumentNullException();
			}
			if (FreeImage.GetImageType(dib) != FREE_IMAGE_TYPE.FIT_BITMAP)
			{
				throw new ArgumentException("dib");
			}
			if (FreeImage.GetBPP(dib) > 8u)
			{
				throw new ArgumentException("dib");
			}
		}

		/// <summary>
		/// Gets or sets the palette through an array of <see cref="RGBQUAD"/>.
		/// </summary>
		public RGBQUAD[] AsArray
		{
			get
			{
				return Data;
			}
			set
			{
				Data = value;
			}
		}

		/// <summary>
		/// Get an array of <see cref="System.Drawing.Color"/> that the block of memory represents.
		/// This property is used for internal palette operations.
		/// </summary>
		internal unsafe Color[] ColorData
		{
			get
			{
				Color[] data = new Color[length];
				for (int i = 0; i < length; i++)
				{
					data[i] = Color.FromArgb((int)(((uint*)baseAddress)[i] | 0xFF000000));
				}
				return data;
			}
		}

		/// <summary>
		/// Returns the palette as an array of <see cref="RGBQUAD"/>.
		/// </summary>
		/// <returns>The palette as an array of <see cref="RGBQUAD"/>.</returns>
		public RGBQUAD[] ToArray()
		{
			return Data;
		}

		/// <summary>
		/// Creates a linear palette based on the provided <paramref name="color"/>.
		/// </summary>
		/// <param name="color">The <see cref="System.Drawing.Color"/> used to colorize the palette.</param>
		/// <remarks>
		/// Only call this method on linear palettes.
		/// </remarks>
		public void Colorize(Color color)
		{
			Colorize(color, 0.5d);
		}

		/// <summary>
		/// Creates a linear palette based on the provided <paramref name="color"/>.
		/// </summary>
		/// <param name="color">The <see cref="System.Drawing.Color"/> used to colorize the palette.</param>
		/// <param name="splitSize">The position of the color within the new palette.
		/// 0 &lt; <paramref name="splitSize"/> &lt; 1.</param>
		/// <remarks>
		/// Only call this method on linear palettes.
		/// </remarks>
		public void Colorize(Color color, double splitSize)
		{
			Colorize(color, (int)(length * splitSize));
		}

		/// <summary>
		/// Creates a linear palette based on the provided <paramref name="color"/>.
		/// </summary>
		/// <param name="color">The <see cref="System.Drawing.Color"/> used to colorize the palette.</param>
		/// <param name="splitSize">The position of the color within the new palette.
		/// 0 &lt; <paramref name="splitSize"/> &lt; <see cref="MemoryArray&lt;T&gt;.Length"/>.</param>
		/// <remarks>
		/// Only call this method on linear palettes.
		/// </remarks>
		public void Colorize(Color color, int splitSize)
		{
			if (splitSize < 1 || splitSize >= length)
			{
				throw new ArgumentOutOfRangeException("splitSize");
			}

			RGBQUAD[] pal = new RGBQUAD[length];

			double red = color.R;
			double green = color.G;
			double blue = color.B;

			int i = 0;
			double r, g, b;

			r = red / splitSize;
			g = green / splitSize;
			b = blue / splitSize;

			for (; i <= splitSize; i++)
			{
				pal[i].rgbRed = (byte)(i * r);
				pal[i].rgbGreen = (byte)(i * g);
				pal[i].rgbBlue = (byte)(i * b);
			}

			r = (255 - red) / (length - splitSize);
			g = (255 - green) / (length - splitSize);
			b = (255 - blue) / (length - splitSize);

			for (; i < length; i++)
			{
				pal[i].rgbRed = (byte)(red + ((i - splitSize) * r));
				pal[i].rgbGreen = (byte)(green + ((i - splitSize) * g));
				pal[i].rgbBlue = (byte)(blue + ((i - splitSize) * b));
			}

			Data = pal;
		}

		/// <summary>
		/// Saves this <see cref="Palette"/> to the specified file.
		/// </summary>
		/// <param name="filename">
		/// A string that contains the name of the file to which to save this <see cref="Palette"/>.
		/// </param>
		public void Save(string filename)
		{
			using (Stream stream = new FileStream(filename, FileMode.Create, FileAccess.Write))
			{
				Save(stream);
			}
		}

		/// <summary>
		/// Saves this <see cref="Palette"/> to the specified stream.
		/// </summary>
		/// <param name="stream">
		/// The <see cref="Stream"/> where the image will be saved.
		/// </param>
		public void Save(Stream stream)
		{
			Save(new BinaryWriter(stream));
		}

		/// <summary>
		/// Saves this <see cref="Palette"/> using the specified writer.
		/// </summary>
		/// <param name="writer">
		/// The <see cref="BinaryWriter"/> used to save the image.
		/// </param>
		public void Save(BinaryWriter writer)
		{
			writer.Write(ToByteArray());
		}

		/// <summary>
		/// Loads a palette from the specified file.
		/// </summary>
		/// <param name="filename">The name of the palette file.</param>
		public void Load(string filename)
		{
			using (Stream stream = new FileStream(filename, FileMode.Open, FileAccess.Read))
			{
				Load(stream);
			}
		}

		/// <summary>
		/// Loads a palette from the specified stream.
		/// </summary>
		/// <param name="stream">The stream to load the palette from.</param>
		public void Load(Stream stream)
		{
			Load(new BinaryReader(stream));
		}

		/// <summary>
		/// Loads a palette from the reader.
		/// </summary>
		/// <param name="reader">The reader to load the palette from.</param>
		public unsafe void Load(BinaryReader reader)
		{
			int size = length * sizeof(RGBQUAD);
			byte[] data = reader.ReadBytes(size);
			fixed(byte* src = data)
			{
				CopyMemory(baseAddress, src, data.Length);
			}
		}
	}
}