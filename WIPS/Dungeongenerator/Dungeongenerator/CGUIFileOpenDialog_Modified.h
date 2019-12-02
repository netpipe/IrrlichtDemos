// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// modified by Maximilian Honig

// zur Verhinderung doppelter Definition
#ifndef FILEOPENDIALOG_MODIFED_H
#define FILEOPENDIALOG_MODIFED_H

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIFileOpenDialog.h"
#include "IGUIButton.h"
#include "IGUIListBox.h"
#include "IGUIEditBox.h"
#include "IFileSystem.h"

namespace irr
{
namespace gui
{

	class CGUIFileOpenDialog_Modified : public IGUIFileOpenDialog
	{
	public:

		//! constructor
		CGUIFileOpenDialog_Modified(const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id);

		//! destructor
		virtual ~CGUIFileOpenDialog_Modified();

		//! returns the filename of the selected file. Returns NULL, if no file was selected.
		virtual const wchar_t* getFileName() const;

		//! Returns the directory of the selected file. Returns NULL, if no directory was selected.
		virtual const io::path& getDirectoryName();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();


				virtual const io::path& getFileNameP() const{return;};

		//! Returns the directory of the selected file. Empty if no directory was selected.
		virtual const io::path& getDirectoryName() const {return;};

		//! Returns the directory of the selected file converted to wide characters. Returns NULL if no directory was selected.
		virtual const wchar_t* getDirectoryNameW() const {return;};

	protected:

		//! fills the listbox with files.
		void fillListBox();

		//! sends the event that the file has been selected.
		void sendSelectedEvent( EGUI_EVENT_TYPE type );

		//! sends the event that the file choose process has been canceld
		void sendCancelEvent();

		core::position2d<s32> DragStart;
		core::stringw FileName;
		io::path FileDirectory;

		IGUIButton* CloseButton;
		IGUIButton* OKButton;
		IGUIButton* CancelButton;
		IGUIListBox* FileBox;
		IGUIEditBox* FileNameText;
		IGUIElement* EventParent;
		io::IFileSystem* FileSystem;
		io::IFileList* FileList;
		bool Dragging;
	};


} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif // __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__

