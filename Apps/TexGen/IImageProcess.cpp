#include "StdAfx.h"
#include "IImageProcess.h"

namespace TexGen
{

IImageProcess::IImageProcess( video::IVideoDriver* driver, u32 ID )
: m_Driver(driver)
, m_Processed(false)
, m_ID(ID)
, m_TimeStamp(0)
, m_OutputSize( 0, 0 )
{
	// Grab the driver
	if (m_Driver)
		m_Driver->grab();
}

IImageProcess::~IImageProcess(void)
{
	// Drop the output images
	for (u32 i = 0; i < m_OutputImages.size(); i++)
		m_OutputImages[i]->drop();
	m_OutputImages.clear();

	// Release the driver
	if (m_Driver)
		m_Driver->drop();
}

/// Add an input into this process
/// \param inputIdx Which input this process is
/// \param sourceProce The actual source process
/// \param sourceIdx Which of the outputs in the source process we want
void IImageProcess::setInput( u32 inputIdx, IImageProcess* sourceProc, u32 sourceIdx )
{
	if (!sourceProc)
		return;

	if (inputIdx < getNumInputs())
	{
		m_Inputs[inputIdx].Proc = sourceProc;
		m_Inputs[inputIdx].SourceIdx = sourceIdx;
	}
}

/// Remove an input into the process
/// \param inputIdx The input to remove
void IImageProcess::removeInput( u32 inputIdx )
{
	if (inputIdx < getNumInputs())
	{
		m_Inputs[inputIdx].Proc = NULL;
		m_Inputs[inputIdx].SourceIdx = 0;
	}
	// Also clear the output images as they are no longer valid
	for (u32 i = 0; i < getNumOutputs(); i++)
	{
		if (m_OutputImages[i])
		{
			m_OutputImages[i]->drop();
			m_OutputImages[i] = NULL;
		}
	}

	// No longer processed either
	m_Processed = false;

}

void IImageProcess::setOutputSize( u32 width, u32 height )
{
	if ((width != m_OutputSize.Width) || (height != m_OutputSize.Height))
	{
		m_Processed = false;
		m_OutputSize = core::dimension2d<u32>( width, height );

		// Create the output images
		for (u32 i = 0; i < getNumOutputs(); i++)
		{
			// Create the image
			video::IImage* img = m_Driver->createImage( video::ECF_A8R8G8B8, m_OutputSize );
			// Check if we need to drop the old image
			if ( (m_OutputImages.size() > i) )
			{
				if (m_OutputImages[i])
					m_OutputImages[i]->drop();
				// Set the new image
				m_OutputImages[i] = img;
			}
			else
			{
				// We need to add it to the array
				m_OutputImages.push_back( img );
			}
		}
	}

	// Loop through all inputs and set their input sizes
	for (u32 i = 0; i < getNumInputs(); i++)
	{
		if (m_Inputs[i].Proc)
			m_Inputs[i].Proc->setOutputSize( width, height );
	};
}

video::IImage* IImageProcess::getOutputImage( u32 idx )
{
	if (idx < getNumOutputs())
		return m_OutputImages[idx];

	return NULL;
}

void IImageProcess::Refresh( bool force )
{
	// Check to see if we need refresh the output image or not
	if ( (!isProcessed()) || (force) )
		RunProcess();
}

u32 IImageProcess::getID( void )
{
	return m_ID;
}

const c8* IImageProcess::getName( void ) const
{
	return PT_String[ getType() ];
}

bool IImageProcess::isProcessed( void )
{
	// We are only processed if all our child processes have also been processed
	// Loop through all inputs and check
	for (u32 i = 0; i < getNumInputs(); i++)
	{
		if (m_Inputs[i].Proc)
		{
			if ((!m_Inputs[i].Proc->isProcessed()) || (m_TimeStamp < m_Inputs[i].Proc->getTimeStamp()))
			{
				m_Processed = false;
				return false;
			}
		}
	}
	return m_Processed;
}

time_t IImageProcess::getTimeStamp( void )
{
	return m_TimeStamp;
}

void IImageProcess::serializeAttributes( io::IAttributes *out, io::SAttributeReadWriteOptions *options) const
{
	out->addInt( "(id)", m_ID );
	out->addString( "(name)", getName() );
}

void IImageProcess::deserializeAttributes( io::IAttributes *in, io::SAttributeReadWriteOptions *options)
{
	//m_ID = in->getAttributeAsInt( "ID" );
	m_Processed = false;
}

}