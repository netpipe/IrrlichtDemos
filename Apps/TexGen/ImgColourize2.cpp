#include "StdAfx.h"
#include "ImgColourize2.h"

namespace TexGen
{

CImgColourize2::CImgColourize2( video::IVideoDriver *driver, u32 ID )
: IImageProcess( driver, ID )
, m_Sorted( false )
{
	// Setup the arrays for the inputs and outputs
	for (u32 i = 0; i < getNumInputs(); i++)
		m_Inputs.push_back( SInputLink() );

	// Setup the array for the output images, currently NULL pointers as we don't
	// know what size the output images will be
	// TODO: We can do this in the setOutputSize function.
	for ( u32 i = 0; i < getNumOutputs(); i++ )
		m_OutputImages.push_back( NULL );

	// Add some basic colour points
	addColourPoint( video::SColor( 255, 84, 84, 84 ), 0.2f );
	addColourPoint( video::SColor( 255, 73, 47, 37 ), 0.5f );
	addColourPoint( video::SColor( 255, 114, 114, 114 ), 0.95f );
}

CImgColourize2::~CImgColourize2(void)
{
	// Delete the inputs and outputs
	m_Inputs.clear();
	
	for (u32 i = 0; i < m_OutputImages.size(); i++)
		m_OutputImages[i]->drop();
	m_OutputImages.clear();
}

/// Get the process type
PROCESS_TYPES CImgColourize2::getType( void ) const
{
	return PT_COLOURIZE_ADVANCED;
}

/// Colourise the input image
/// \param width The width of the final output image
/// \param height The height of the final output image
void CImgColourize2::RunProcess( )
{
	
	// Check we have an input to work with
	if (!m_Inputs[0].Proc)
		return;

	// Check to see if we have atleast 2 colour points
	if (m_Colours.size() < 2)
		return;

	// Refresh our input process if needed
	m_Inputs[0].Proc->Refresh();

	// Get our input image
	video::IImage* input = m_Inputs[0].Proc->getOutputImage(m_Inputs[0].SourceIdx);
	// Check if valid
	if (!input)
		return;

	// Pixel we are working on
	video::SColor pix;
	SColourPoint* C1;
	SColourPoint* C2;

	core::list<SColourPoint*>::Iterator point;
	core::list<SColourPoint*>::Iterator nextp;

	// If out colour list hasn't been sorted then we need to sort it
	if (!m_Sorted)
		SortColours();

	// Loop through each pixel on the image
	for ( u32 v = 0; v < m_OutputSize.Width; v++ )
	{
		for ( u32 u = 0; u < m_OutputSize.Height; u++ )
		{
			// Get the pixel color
			pix = input->getPixel( v, u );
			// Get the green channel and convert to a 0.0-1.0 float value
			f32 d = (f32)(pix.getGreen() / 255.0f);

			// Get the first point
			point = m_SortedColours.begin();
			
			// Assume C1 and C2 = the first point
			C1 = (*point);
			C2 = (*point);

			// Loop until we find the correct two points
			while (point != m_SortedColours.end())
			{
				if ( d > (*point)->D )
				{
					C1 = (*point);
					if ( point == m_SortedColours.getLast() )
						C2 = C1;
					else
						C2 = (*(point + 1));
				}
				// Next point
				point++;
			}
			// Racalculate d as a point between C1.D and C2.D
			f32 D = ((d - C1->D) / (C2->D - C1->D));
			
				
			// Interpolate to the target colour
			video::SColor res = C2->Colour.getInterpolated( C1->Colour, D );

			// Set the value
			m_OutputImages[0]->setPixel( v, u, res );
		}
	}

	m_Processed = true;
	m_TimeStamp = time(NULL);

	//std::cout << "Rebuilt Colourized. TimeStamp now = " << m_TimeStamp << std::endl;
}

/// Get the number of input processes to this image process
/// \return The number of input processes
u32 CImgColourize2::getNumInputs( void ) const
{
	return 1;	// 1 input to this process
}

/// Get the number of output processes to this image process
/// \return The number of out processes
u32 CImgColourize2::getNumOutputs( void ) const
{
	return 1;	// 1 output
}

/// Save the attributes of this process
void CImgColourize2::serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options) const
{
	IImageProcess::serializeAttributes( out, options );
	// Save out our specific settings
	
	// Save the number of colours
	out->addInt( "PointCount", m_Colours.size() );
	// Loop through all the colour points
	for (u32 i = 0; i < m_Colours.size(); i++ )
	{
		// Save the colour
		core::stringc colour("Colour");
		colour += i;
		out->addColor( colour.c_str(), m_Colours[i]->Colour );
		// Save the delta
		core::stringc delta("Delta");
		delta += i;
		out->addFloat( delta.c_str(), m_Colours[i]->D );
	}
	
}
// Load the attributes of this process
void CImgColourize2::deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options)
{
	IImageProcess::deserializeAttributes( in, options );
	// Read in our specific settings

	// Clear the colour point array
	for (u32 i = 0; i < m_Colours.size(); i++ )
	{
		delete m_Colours[i];
	}
	m_Colours.clear();
	
	// Get the number of colours
	s32 count = in->getAttributeAsInt( "PointCount" );
	// Loop through and read all the colour points
	for ( s32 i = 0; i < count; i++ )
	{
		// Read the colour
		core::stringc colour("Colour");
		colour += i;
		video::SColor col = in->getAttributeAsColor( colour.c_str() );
		// Read the delta
		core::stringc delta("Delta");
		delta += i;
		f32 d = in->getAttributeAsFloat( delta.c_str() );
		// Add the colour
		addColourPoint( col, d );
	}
}

u32 CImgColourize2::addColourPoint( video::SColor colour,  f32 delta )
{
	SColourPoint* point = new SColourPoint;
	point->Colour = colour;
	point->D = delta;

	// Add the point to the colour array
	m_Colours.push_back( point );

	// No longer sorted
	m_Sorted = false;
	m_Processed = false;

	return m_Colours.size();
}

u32 CImgColourize2::getNumColourPoints( void )
{
	return m_Colours.size();
}

void CImgColourize2::removeColourPoint( u32 idx )
{
	// Delete a colour point
	// Check
	if (idx >= m_Colours.size())
		return;
	
	delete m_Colours[idx];	// Delete the point from memory
	m_Colours.erase( idx );	// Erase it from the array

	// No longer sorted
	m_Sorted = false;
	m_Processed = false;
}

video::SColor CImgColourize2::getColourPointColour( u32 idx )
{
	// Get the colour of a colour point
	// Check
	if (idx >= m_Colours.size())
		return video::SColor();

	return m_Colours[idx]->Colour;

}

void CImgColourize2::setColourPointColour( u32 idx, video::SColor colour )
{
	// Set the colour of a colour point
	// Check
	if (idx >= m_Colours.size())
		return;

	m_Colours[idx]->Colour = colour;

	// No longer sorted
	m_Sorted = false;
	m_Processed = false;
	
}

f32 CImgColourize2::getColourPointDelta( u32 idx )
{
	// Get the delta of a colour point
	// Check
	if (idx >= m_Colours.size())
		return 0;

	return m_Colours[idx]->D;
}

void CImgColourize2::setColourPointDelta( u32 idx,  f32 delta)
{
	// Set the colour of a colour point
	// Check
	if (idx >= m_Colours.size())
		return;

	m_Colours[idx]->D = delta;

	// No longer sorted
	m_Sorted = false;
	m_Processed = false;
}

void CImgColourize2::SortColours( void )
{
	
	core::list<SColourPoint*>::Iterator itor;

	// Clear the currently sorted list
	m_SortedColours.clear();

	// Check to see if we have colours to sort
	if (m_Colours.empty())
	{
		// Nothing to sort
		m_Sorted = true;
		return;		
	}

	// Loop through all elements in the colour array
	for ( u32 i = 0; i < m_Colours.size(); i++ )
	{
		// If this is the first point in the sorted list then just add it
		if ( m_SortedColours.empty() )
		{
			// Empty sorted list so just add the first point
			m_SortedColours.push_front( m_Colours[i] );
		}
		else
		{
			// find a point where the delta of the current point is < than the next but > than the last
			itor = m_SortedColours.begin();
			bool inserted = false;

			while (!inserted)
			{
				if ( m_Colours[i]->D < (*itor)->D )
				{
					// Insert new point before the current point
					m_SortedColours.insert_before( itor, m_Colours[i] );
					inserted = true;
				}
				// If we're at the end and we haven't inserted the point then add it to the end
				if ( (itor == m_SortedColours.getLast()) && (!inserted) )
				{
					m_SortedColours.push_back( m_Colours[i] );
					inserted = true;
				}
				else
				{
					itor++;	// Next point
				}
			}
		}
	}
	// Now we're sorted
	m_Sorted = true;
}

}	// namespace texgen