#define VOS_NT_WINDOWS32    0x00040004L
#define VFT_APP             0x00000001L

// this will set your .exe icon
A ICON MOVEABLE PURE LOADONCALL DISCARDABLE "game.ico"

// include version information in .exe, modify these values to match your needs
1 VERSIONINFO
FILEVERSION 0,1,1,1
PRODUCTVERSION 0,1,1,1
FILETYPE VFT_APP
{
  BLOCK "StringFileInfo"
	 {
		 BLOCK "040904E4"
		 {
			 VALUE "CompanyName", "write version info here"
			 VALUE "FileVersion", "write version info here"
			 VALUE "FileDescription", "write version info here"
			 VALUE "InternalName", "write version info here"
			 VALUE "LegalCopyright", "write version info here"
			 VALUE "LegalTrademarks", "write version info here"
			 VALUE "OriginalFilename", "write version info here"
			 VALUE "ProductName", "write version info here"
			 VALUE "ProductVersion", "write version info here"
		 }
	 }
  BLOCK "VarFileInfo"
	 {
		 VALUE "Translation", 0x0409, 1252 // language codes
	 }
}
