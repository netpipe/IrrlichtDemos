/** FileSystem Extensions

2012 by BenjaminHampe@gmx.de

//! This is just my current state, far from beeing complete and only tested on Win7-32bit
//! so dont tell me its not working :-)
//! i just want to give some hints about my approach and ask for advices

//! Main creates a big LogFile, later it shall compare needed time between the variations

//! remember to only use '/' style paths, for windows and etc.. they will be converted automaticly
//! remember i dont know anything about MacOS, WinCE and XBox, Irrlicht Virtual FS logic

//! question: will irrlicht ever be able to mount/open *.iso and [password protected] *.rar, *.7z archives?

*/

#include <irrlicht.h>

//! new define, will the functions use Windows Shell Operations, if _IRR_WINDOWS_API_ is defined
//#define _IRR_USE_WINDOWS_SHELL_FOR_FILE_OPERATIONS_

//! new define, will the functions use system() commands?
#define _IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_

#if defined (_IRR_WINDOWS_API_)
	#if !defined ( _WIN32_WCE )
        #include <windows.h>
		#include <direct.h> // for _chdir
		#include <io.h> // for _access
		#include <tchar.h>
	#endif
#else
	#if (defined(_IRR_POSIX_API_) || defined(_IRR_OSX_PLATFORM_))
		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h>
		#include <limits.h>
		#include <sys/types.h>
		#include <dirent.h>
		#include <sys/stat.h>
		#include <unistd.h>
		#include <errno.h>
		#include <sys/statfs.h> // for getDriveList()

		//! different filesystem type-values
        //#include <linux/affs_fs.h>
        //#include <linux/efs_fs.h>
        //#include <linux/ext_fs.h>
        //#include <linux/ext2_fs.h>
        //#include <linux/hpfs_fs.h>
        //#include <linux/iso_fs.h>
        //#include <linux/minix_fs.h>
        //#include <linux/msdos_fs.h>
        //#include <linux/ncp_fs.h>
        //#include <linux/nfs_fs.h>
        //#include <linux/proc_fs.h>
        //#include <linux/smb_fs.h>
        //#include <linux/sysv_fs.h>
        //#include <linux/ufs_fs.h>
        //#include <linux/xfs_fs.h>
        //#include <linux/xia_fs.h>

        #define AFFS_SUPER_MAGIC 0xADFF
        #define EFS_SUPER_MAGIC 0x00414A53
        #define EXT_SUPER_MAGIC 0x137D
        #define EXT2_OLD_SUPER_MAGIC 0xEF51
        #define EXT2_SUPER_MAGIC 0xEF53
        #define HPFS_SUPER_MAGIC 0xF995E849
        #define ISOFS_SUPER_MAGIC 0x9660
        #define MINIX_SUPER_MAGIC 0x137F /* orig. minix */
        #define MINIX_SUPER_MAGIC2 0x138F /* 30 char minix */
        #define MINIX2_SUPER_MAGIC 0x2468 /* minix V2 */
        #define MINIX2_SUPER_MAGIC2 0x2478 /* minix V2, 30 char names */
        #define MSDOS_SUPER_MAGIC 0x4d44
        #define NCP_SUPER_MAGIC 0x564c
        #define NFS_SUPER_MAGIC 0x6969
        #define PROC_SUPER_MAGIC 0x9fa0
        #define SMB_SUPER_MAGIC 0x517B
        #define XENIX_SUPER_MAGIC 0x012FF7B4
        #define SYSV4_SUPER_MAGIC 0x012FF7B5
        #define SYSV2_SUPER_MAGIC 0x012FF7B6
        #define COH_SUPER_MAGIC 0x012FF7B7
        #define UFS_MAGIC 0x00011954
        #define XFS_SUPER_MAGIC 0x58465342
        #define _XIAFS_SUPER_MAGIC 0x012FD16D


	#endif
#endif

//! use shell or system(), system() has priority and will #undef shell (advapi32)
#if defined(_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
	#undef _IRR_USE_WINDOWS_SHELL_FOR_FILE_OPERATIONS_
#endif

#if defined(_IRR_WINDOWS_API_) || defined(_MSC_VER)
	#if defined(_IRR_USE_WINDOWS_SHELL_FOR_FILE_OPERATIONS_)
        #pragma comment(lib, "advapi32.lib") //! for shell operations
	#endif
#endif

//! is not defined in irrlicht-1.7.3, so i added it here
#ifndef _IRR_TEXT
#if defined(_IRR_WCHAR_FILESYSTEM)
	typedef wchar_t fschar_t;
	#define _IRR_TEXT(X) L##X
#else
	typedef char fschar_t;
	#define _IRR_TEXT(X) X
#endif
#endif

#include <iostream>
#include <fstream>

using namespace irr;
using namespace io;

//! emulate IFIleSystem class, so we dont need to change functions
//! BIG-BUT: Windows already defined a enum member FileSystemType,
//! in a really creepy way! See "winnt.h" typedef enum _CM_SERVICE_NODE_TYPE
#undef FILESYSTEM_NATIVE
#define FILESYSTEM_NATIVE 0
u32 FSType = FILESYSTEM_NATIVE; // later FSType has to be replaced with FileSystemType

//! for getDriveList()
enum E_LOGICAL_DRIVE_TYPE
{
	ELDT_UNKNOWN = 0,
	ELDT_NOT_EXIST,
	ELDT_REMOVABLE,
	ELDT_FIXED,
	ELDT_REMOTE,
	ELDT_CDROM,
	ELDT_RAMDISK
};

//! new names
const irr::c8* const E_LOGICAL_DRIVE_TYPE_NAMES[] = {
	"ELDT_UNKNOWN",
	"ELDT_NOT_EXIST",
	"ELDT_REMOVABLE",
	"ELDT_FIXED",
	"ELDT_REMOTE",
	"ELDT_CDROM",
	"ELDT_RAMDISK"
};

//! used by getDriveAvailSpace() and getDriveTotalSpace()
#ifndef u64
#define u64 unsigned long long int
#endif

//! holding information about available physical drives
struct SDriveInfo
{
	io::path Name;
	E_LOGICAL_DRIVE_TYPE Type;
	u64 AvailSpace;
	u64 TotalSpace;
	u32 OptimalBlockSize;
};

core::array<SDriveInfo> DriveList;

/************************************************************************************

	FUNCTION PROTOTYPES/DECLARATIONS

	These are the functions, the forum's thread talks about

************************************************************************************/

bool 	createTestFile(const path& filename);							//! ok for windows

core::stringc getErrorText();											//! should be ok, not tested on Linux

path 	getFileDir(const path& filename);								//! copied and rewritten from svn
void 	getDriveList();													//! working only for windows, else does nothing
u32 	getDriveCount();												//! ok
path 	getDriveName( u32 i );											//! ok
u64 	getDriveAvailSpace( u32 i );									//! ok
u64 	getDriveTotalSpace( u32 i );									//! ok
u32 	getDriveOptimalBlockSize( u32 i );								//! ok
E_LOGICAL_DRIVE_TYPE getDriveType( u32 i );								//! ok
path 	getExeName();													//! ok
path 	getExePath();													//! ok
bool 	existFile(const path& filename);								//! ok
bool 	existDirectory(const path& filename);							//! ok
bool 	touchFile(const path& filename, s32 mode = 0777);				//! not touched yet
s32 	createDirectoryOnce( const path& filename, s32 mode = 0777);	//! should not be called by user, hope i can make it
bool 	createDirectory(const path& filename, s32 mode = 0777);			//! windows system() variant is working, also the linux native variant
bool 	removeFile(const path& filename);								//! in progress
bool 	removeDirectory(const path& filename);							//! in progress
bool 	copyFile(const path& dstName, const path& srcName);				//! in progress
bool 	copyDirectory(const path& dstName, const path& srcName);		//! in progress
bool 	moveFile(const path& dstName, const path& srcName);				//! in progress
bool 	moveDirectory(const path& dstName, const path& srcName);		//! in progress

u64		getFileSize(const path& filename);								//! new and not working
u64		getDirectorySize(const path& filename);							//! new and not working

/************************************************************************************

	MAIN PROGRAM

	THIS PROGRAM TESTS ALL IMPORTANT FILESYSTEM COMMANDS AVAILABLE

************************************************************************************/

s32 main(s32 argc, c8** argv)
{
	/*******************************************************
		CREATE LOGFILE
	*******************************************************/

#if defined(_IRR_WINDOWS_API_)
    path testName = "Windows_Test";
#elif defined(_IRR_POSIX_API_)
    path testName = "Posix_Test";
#else
    path testName = "otherOS_Test";
#endif

    path fileName = testName + ".log";
    std::ofstream fout( fileName.c_str(), std::ios::out);
    if (!fout.is_open())
    {
        std::cout << "ERROR::could not open LogFile "<<fileName.c_str()<<std::endl;
        ::exit(-1);
    }

	/*******************************************************
		SAVE POINTERS TO STANDARD STREAMS
	*******************************************************/
	std::streambuf* stdCout = std::cout.rdbuf();
	std::streambuf* stdCerr = std::cerr.rdbuf();
	std::streambuf* stdClog = std::clog.rdbuf();

	/*******************************************************
		REDIRECT STANDARD STREAMS TO LOGFILE
	*******************************************************/

	std::cout.rdbuf( fout.rdbuf() );
	std::cerr.rdbuf( fout.rdbuf() );
	std::clog.rdbuf( fout.rdbuf() );

	/*******************************************************
		WRITE TEST CONDITIONS
	*******************************************************/
	fout<<"Defined Preprocessor Switches for your System::"<<std::endl<<std::endl;
#if defined(_IRR_USE_WINDOWS_SHELL_FOR_FILE_OPERATIONS_)
    fout<<" _IRR_USE_WINDOWS_SHELL_FOR_FILE_OPERATIONS_"<<std::endl;
#endif
#if defined(_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
    fout<<" _IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_"<<std::endl;
	if (!system(NULL))
    {
		fout<<"No System-CommandLine-Interpreter found! Abort Program..."<<std::endl;
		fout.close();
    	::exit(1);
    }
#endif
#if defined(_IRR_WINDOWS_API_)
    fout<<" _IRR_WINDOWS_API_"<<std::endl;
#endif
#if defined(_IRR_WCHAR_FILESYSTEM)
    fout<<" _IRR_WCHAR_FILESYSTEM"<<std::endl;
#endif
#if defined(_IRR_POSIX_API_)
    fout<<" _IRR_POSIX_API_"<<std::endl;
    #if defined(__USE_LARGEFILE64)
    fout<<" __USE_LARGEFILE64"<<std::endl;
    #endif
#endif
#if defined(_MSC_VER)
    fout<<" _MSC_VER"<<std::endl;
#endif

	fout<<std::endl;

	/*******************************************************
		BEGIN DRIVE TESTS
	*******************************************************/

	//! get infos
	getDriveList();

	fout<<" getDriveCount() = "<<getDriveCount()<<std::endl;
	for (u32 i=0; i<getDriveCount(); i++)
	fout<<" ("<<i<<") ::"
		<<"\tgetDriveName() = "<<getDriveName(i).c_str()
		<<"\tgetDriveType() = "<<E_LOGICAL_DRIVE_TYPE_NAMES[getDriveType(i)]
		<<"\tgetDriveAvailSpace() = "<<getDriveAvailSpace(i)<<" KiB"
		<<"\tgetDriveTotalSpace() = "<<getDriveTotalSpace(i)<<" KiB"
		<<"\tgetDriveOptimalBlockSize() = "<<getDriveOptimalBlockSize(i)<<" KiB"
		<<std::endl;

	fout<<std::endl;

	/*******************************************************
		GET WORKING DIR FROM COMMANDLINE ARG
	*******************************************************/
    fout<<" ExeName = "<<getExeName().c_str()<<std::endl;
    fout<<" ExePath = "<<getExePath().c_str()<<std::endl<<std::endl;

	io::path P = getExePath();
    fout<<" P = "<<P.c_str()<<std::endl<<std::endl;

	path T1 = _IRR_TEXT("C:/Windows");
	path T2 = _IRR_TEXT("C:/Windows/System32");
	path T3 = _IRR_TEXT("C:/Windows/System32/cmd.exe");
	path T4 = _IRR_TEXT("/usr");
	path T5 = _IRR_TEXT("/usr/bin");
	path T6 = _IRR_TEXT("/bin/bash");

    fout<<" existDirectory("<<T1.c_str()<<") = "<< ((existDirectory(T1))?"true":"false")<<std::endl;
    fout<<" existDirectory("<<T2.c_str()<<") = "<< ((existDirectory(T2))?"true":"false")<<std::endl;
    fout<<" existDirectory("<<T3.c_str()<<") = "<< ((existDirectory(T3))?"true":"false")<<std::endl;
    fout<<" existDirectory("<<T4.c_str()<<") = "<< ((existDirectory(T4))?"true":"false")<<std::endl;
    fout<<" existDirectory("<<T5.c_str()<<") = "<< ((existDirectory(T5))?"true":"false")<<std::endl;
    fout<<" existDirectory("<<T6.c_str()<<") = "<< ((existDirectory(T6))?"true":"false")<<std::endl<<std::endl;

    fout<<" existFile("<<T1.c_str()<<") = "<< ((existFile(T1))?"true":"false")<<std::endl;
    fout<<" existFile("<<T2.c_str()<<") = "<< ((existFile(T2))?"true":"false")<<std::endl;
    fout<<" existFile("<<T3.c_str()<<") = "<< ((existFile(T3))?"true":"false")<<std::endl;
	fout<<" existFile("<<T4.c_str()<<") = "<< ((existFile(T4))?"true":"false")<<std::endl;
    fout<<" existFile("<<T5.c_str()<<") = "<< ((existFile(T5))?"true":"false")<<std::endl;
    fout<<" existFile("<<T6.c_str()<<") = "<< ((existFile(T6))?"true":"false")<<std::endl<<std::endl;

	/*******************************************************
		SOME EXAMPLE DIRECTORIES
	*******************************************************/

    io::path A = P + "/" + testName + "/A/A1/A2/A3";
    io::path B = P + "/" + testName + "/B/B1/B2.txt"; // yes will be directory
    io::path C = P + "/" + testName + "/C/C1";

    fout<<" A = "<<A.c_str()<<std::endl;
    fout<<" B = "<<B.c_str()<<std::endl;
    fout<<" C = "<<C.c_str()<<std::endl<<std::endl;

	/*******************************************************
		BEGIN FILESYSTEM TESTS
	*******************************************************/

    fout<<" existDirectory(A) = "<< ((existDirectory(A))?"true":"false")<<std::endl;
    fout<<" existDirectory(B) = "<< ((existDirectory(B))?"true":"false")<<std::endl;
    fout<<" existDirectory(C) = "<< ((existDirectory(C))?"true":"false")<<std::endl<<std::endl;

    fout<<" removeDirectory(A) = "<< ((removeDirectory(A))?"true":"false")<<std::endl;
    fout<<" removeDirectory(B) = "<< ((removeDirectory(B))?"true":"false")<<std::endl;
    fout<<" removeDirectory(C) = "<< ((removeDirectory(C))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(A) = "<< ((existDirectory(A))?"true":"false")<<std::endl;
    fout<<" existDirectory(B) = "<< ((existDirectory(B))?"true":"false")<<std::endl;
    fout<<" existDirectory(C) = "<< ((existDirectory(C))?"true":"false")<<std::endl<<std::endl;

	fout<<" createDirectory(A) = "<< ((createDirectory(A))?"true":"false")<<std::endl;
	fout<<" createDirectory(B) = "<< ((createDirectory(B))?"true":"false")<<std::endl;
	fout<<" createDirectory(C) = "<< ((createDirectory(C))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(A) = "<< ((existDirectory(A))?"true":"false")<<std::endl;
    fout<<" existDirectory(B) = "<< ((existDirectory(B))?"true":"false")<<std::endl;
    fout<<" existDirectory(C) = "<< ((existDirectory(C))?"true":"false")<<std::endl<<std::endl;

	fout<<" createDirectory(A) = "<< ((createDirectory(A))?"true":"false")<<std::endl;
	fout<<" createDirectory(B) = "<< ((createDirectory(B))?"true":"false")<<std::endl;
	fout<<" createDirectory(C) = "<< ((createDirectory(C))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(A) = "<< ((existDirectory(A))?"true":"false")<<std::endl;
    fout<<" existDirectory(B) = "<< ((existDirectory(B))?"true":"false")<<std::endl;
    fout<<" existDirectory(C) = "<< ((existDirectory(C))?"true":"false")<<std::endl<<std::endl;

    fout<<" removeDirectory(A) = "<< ((removeDirectory(A))?"true":"false")<<std::endl;
    fout<<" removeDirectory(B) = "<< ((removeDirectory(B))?"true":"false")<<std::endl;
    fout<<" removeDirectory(C) = "<< ((removeDirectory(C))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(A) = "<< ((existDirectory(A))?"true":"false")<<std::endl;
    fout<<" existDirectory(B) = "<< ((existDirectory(B))?"true":"false")<<std::endl;
    fout<<" existDirectory(C) = "<< ((existDirectory(C))?"true":"false")<<std::endl<<std::endl;

	fout<<" createDirectory(A) = "<< ((createDirectory(A))?"true":"false")<<std::endl;
	fout<<" createDirectory(B) = "<< ((createDirectory(B))?"true":"false")<<std::endl;
	fout<<" createDirectory(C) = "<< ((createDirectory(C))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(A) = "<< ((existDirectory(A))?"true":"false")<<std::endl;
    fout<<" existDirectory(B) = "<< ((existDirectory(B))?"true":"false")<<std::endl;
    fout<<" existDirectory(C) = "<< ((existDirectory(C))?"true":"false")<<std::endl<<std::endl;

	//! Part2
    io::path D = P + "/" + "copy" + "/A/A1/A2/A3";
    io::path E = P + "/" + "copy" + "/B/B1/B2.txt";
    io::path F = P + "/" + "copy" + "/C/C1";

    fout<<" D = "<<D.c_str()<<std::endl;
    fout<<" E = "<<E.c_str()<<std::endl;
    fout<<" F = "<<F.c_str()<<std::endl<<std::endl;

    fout<<" existDirectory(D) = "<< ((existDirectory(D))?"true":"false")<<std::endl;
    fout<<" existDirectory(E) = "<< ((existDirectory(E))?"true":"false")<<std::endl;
    fout<<" existDirectory(F) = "<< ((existDirectory(F))?"true":"false")<<std::endl<<std::endl;

	fout<<" removeDirectory(D) = "<< ((removeDirectory(D))?"true":"false")<<std::endl;
	fout<<" removeDirectory(E) = "<< ((removeDirectory(E))?"true":"false")<<std::endl;
	fout<<" removeDirectory(F) = "<< ((removeDirectory(F))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(D) = "<< ((existDirectory(D))?"true":"false")<<std::endl;
    fout<<" existDirectory(E) = "<< ((existDirectory(E))?"true":"false")<<std::endl;
    fout<<" existDirectory(F) = "<< ((existDirectory(F))?"true":"false")<<std::endl<<std::endl;

	fout<<" copyDirectory(A,D) = "<< ((copyDirectory(A,D))?"true":"false")<<std::endl;
	fout<<" copyDirectory(B,E) = "<< ((copyDirectory(B,E))?"true":"false")<<std::endl;
	fout<<" copyDirectory(C,F) = "<< ((copyDirectory(C,F))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(D) = "<< ((existDirectory(D))?"true":"false")<<std::endl;
    fout<<" existDirectory(E) = "<< ((existDirectory(E))?"true":"false")<<std::endl;
    fout<<" existDirectory(F) = "<< ((existDirectory(F))?"true":"false")<<std::endl<<std::endl;

	fout<<" removeDirectory(D) = "<< ((removeDirectory(D))?"true":"false")<<std::endl;
	fout<<" removeDirectory(E) = "<< ((removeDirectory(E))?"true":"false")<<std::endl;
	fout<<" removeDirectory(F) = "<< ((removeDirectory(F))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(D) = "<< ((existDirectory(D))?"true":"false")<<std::endl;
    fout<<" existDirectory(E) = "<< ((existDirectory(E))?"true":"false")<<std::endl;
    fout<<" existDirectory(F) = "<< ((existDirectory(F))?"true":"false")<<std::endl<<std::endl;

	fout<<" copyDirectory(A,D) = "<< ((copyDirectory(A,D))?"true":"false")<<std::endl;
	fout<<" copyDirectory(B,E) = "<< ((copyDirectory(B,E))?"true":"false")<<std::endl;
	fout<<" copyDirectory(C,F) = "<< ((copyDirectory(C,F))?"true":"false")<<std::endl<<std::endl;

    fout<<" existDirectory(D) = "<< ((existDirectory(D))?"true":"false")<<std::endl;
    fout<<" existDirectory(E) = "<< ((existDirectory(E))?"true":"false")<<std::endl;
    fout<<" existDirectory(F) = "<< ((existDirectory(F))?"true":"false")<<std::endl<<std::endl;

	/*******************************************************
		END PROGRAM
	*******************************************************/

	//! RESTORE STANDARD STREAMS
	std::cout.rdbuf( stdCout );
	std::cerr.rdbuf( stdCerr );
	std::clog.rdbuf( stdClog );

	//! Close LogFile
	fout.close();
	return 0;
}

/************************************************************************************

	FUNCTION IMPLEMENTATIONS

************************************************************************************/

bool createTestFile(const path& filename)
{
	s32 result;
	path command;
	path save = filename;

#if defined(_IRR_WINDOWS_API_)
	#if defined(_IRR_WCHAR_FILESYSTEM)
	save.replace(L'/',L'\\');
	#else
	save.replace('/','\\');
	#endif
#endif

	//! echo
	command = _IRR_TEXT("echo ");
	command += save;
	command += " > ";
	command += save;

	#if defined(_IRR_WCHAR_FILESYSTEM)
	result = _wsystem(command.c_str());
	#else
	result = system(command.c_str());
	#endif

	//! copy help
	command = _IRR_TEXT("copy /? >> ");
	command += save;

	#if defined(_IRR_WCHAR_FILESYSTEM)
	result = _wsystem(command.c_str());
	#else
	result = system(command.c_str());
	#endif

	//! del help
	command = _IRR_TEXT("del /? >> ");
	command += save;

	#if defined(_IRR_WCHAR_FILESYSTEM)
	result = _wsystem(command.c_str());
	#else
	result = system(command.c_str());
	#endif

	//! mkdir help
	command = _IRR_TEXT("mkdir /? >> ");
	command += save;

	#if defined(_IRR_WCHAR_FILESYSTEM)
	result = _wsystem(command.c_str());
	#else
	result = system(command.c_str());
	#endif

	//! rmdir help
	command = _IRR_TEXT("rmdir /? >> ");
	command += save;

	#if defined(_IRR_WCHAR_FILESYSTEM)
	result = _wsystem(command.c_str());
	#else
	result = system(command.c_str());
	#endif

	//! xcopy help
	command = _IRR_TEXT("xcopy /? >> ");
	command += save;

	#if defined(_IRR_WCHAR_FILESYSTEM)
	result = _wsystem(command.c_str());
	#else
	result = system(command.c_str());
	#endif

}

//! helper, convert last error to string
core::stringc getErrorText()
{
	core::stringc errorText;

	#if defined(_IRR_WINDOWS_API_)
    c8* lpMsgBuf = 0; // will be allocated by WinAPI
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        lpMsgBuf,
        0,
        NULL);

    errorText = lpMsgBuf;
    LocalFree(lpMsgBuf);
    #elif defined(_IRR_POSIX_API_)
    errorText = strerror(errno);
	#else
	errorText = "unknown";
    #endif
    return errorText;
}

//! returns the directory part of a filename, i.e. all until the first
path getFileDir(const path& filename)
{
	path tmp = filename;
	#if defined( _IRR_WINDOWS_API_)
		#if defined (_IRR_WCHAR_FILESYSTEM)
		tmp.replace(L'\\', L'/');
		#else
		tmp.replace('\\', '/');
		#endif
	#endif

	#if defined (_IRR_WCHAR_FILESYSTEM)
	s32 pos = tmp.findLast(L'/');
	#else
	s32 pos = tmp.findLast('/');
	#endif

	if (pos!=-1)
	{
		tmp = tmp.subString(0,(u32)pos);
	}
	else
	{
		tmp = _IRR_TEXT(".");
	}

	return tmp;
}

//! Update list of available physical drives
void getDriveList()
{
	DriveList.clear();

#if defined( _IRR_WINDOWS_API_ )
	const s32 SIZE = 255;
	#if defined (_IRR_WCHAR_FILESYSTEM)

	wchar_t* buffer = new wchar_t[SIZE]; // LPTSTR
	u32 chars = GetLogicalDriveStringsW(SIZE, buffer); // Win32 API ANSI
	core::stringw result(L"");
	for ( s32 i=0; i < (s32)chars; ++i)
	{
		result += buffer[i];
	}
	delete buffer;
	core::array<core::stringw> container;
	result.split(container, L"\0", 1, true, false);

	#else

	c8* buffer = new c8[SIZE]; // LPTSTR
	u32 chars = GetLogicalDriveStringsA(SIZE, buffer); // Win32 API ANSI
	core::stringc result("");
	for ( s32 i=0; i<(s32)chars; ++i)
	{
		result += buffer[i];
	}
	delete buffer;
	core::array<core::stringc> container;
	result.split(container, "\0", 1, true, false);

	#endif

	for (u32 i=0; i<container.size(); ++i)
	{
		SDriveInfo drive;
		drive.Name = container[i];
		drive.Type = ELDT_NOT_EXIST;
	#if defined (_IRR_WCHAR_FILESYSTEM)
		drive.Type = (E_LOGICAL_DRIVE_TYPE)GetDriveTypeW(drive.Name.c_str());
	#else
		drive.Type = (E_LOGICAL_DRIVE_TYPE)GetDriveTypeA(drive.Name.c_str());
	#endif
		drive.Name.make_lower();
		drive.AvailSpace = 0;
		drive.TotalSpace = 0;
		drive.OptimalBlockSize = 1024;
		DWORD dwSectorsPerCluster = 0;
		DWORD dwBytesPerSector = 0;
		DWORD dwNumberOfFreeClusters = 0;
		DWORD dwTotalNumberOfClusters = 0;
		s32 result = 0;
		//! jumps over a:\\ and b:\\
		//! to prevent popup messagebox for non inserted floppy disks
		if (!((drive.Name == path("a:\\")) || (drive.Name == path("b:\\"))))
		{
	#if defined (_IRR_WCHAR_FILESYSTEM)
			s32 result = GetDiskFreeSpaceW( drive.Name.c_str(), &dwSectorsPerCluster, &dwBytesPerSector, &dwNumberOfFreeClusters, &dwTotalNumberOfClusters);
	#else
			s32 result = GetDiskFreeSpaceA( drive.Name.c_str(), &dwSectorsPerCluster, &dwBytesPerSector, &dwNumberOfFreeClusters, &dwTotalNumberOfClusters);
	#endif
		}
		drive.AvailSpace = dwSectorsPerCluster*dwBytesPerSector*dwNumberOfFreeClusters;
		drive.TotalSpace = dwSectorsPerCluster*dwBytesPerSector*dwTotalNumberOfClusters;
		drive.OptimalBlockSize = dwSectorsPerCluster*dwBytesPerSector;
	#if defined (_IRR_WCHAR_FILESYSTEM)
		drive.Name.replace(L'\\',L'/');
	#else
		drive.Name.replace('\\','/');
	#endif
		DriveList.push_back(drive);
	}
//! Linux
#elif defined (_IRR_POSIX_API_)

    s32 status;

    for (u32 i=0; i<3; i++)
    {
        SDriveInfo drive;
        drive.Name = _IRR_TEXT("");
        drive.Type = ELDT_UNKNOWN;
        drive.OptimalBlockSize = 1024;
        drive.AvailSpace = 0;
        drive.TotalSpace = 0;

        if (i==0)
        {
            drive.Name = _IRR_TEXT("/");
            DriveList.push_back(drive);
        }
        else
        {
            if (i==1) status = system("ls -1 /dev | sort > /tmp/getDriveListTemp");
            if (i==2) status = system("ls -1 /media | sort > /tmp/getDriveListTemp");

            FILE* f=fopen("/tmp/getDriveListTemp", "r");
            if (!f)
            {
                std::cout<<"ERROR::getDriveList(), could not open info file!"<<std::endl;
                return;
            }

            c8* buffer = NULL;
            size_t bufferSize = 0;
            ssize_t readChars = 0;

            while ((readChars = getline(&buffer, &bufferSize, f)) != -1)
            {
                if (buffer)
                {
                    buffer[readChars-1] = '\0';
                    path name = _IRR_TEXT("");
                    if (i==1) name = _IRR_TEXT("/dev/");
                    else if (i==2) name = _IRR_TEXT("/media/");
                    name += buffer;
                    drive.Name = name;
                    DriveList.push_back(drive);
                    delete buffer;
                    buffer = NULL;
                }
            }
            fclose(f);
        }
    }

    //! now we have a list of potential FileSystems
	for (u32 i=0; i<DriveList.size(); ++i)
	{
	    SDriveInfo& drive = DriveList[i];

        #ifdef __USE_LARGEFILE64
        struct statfs64 info;
        if (statfs64(drive.Name.c_str(), &info ) != -1)
        #else
        struct statfs info;
        if (statfs(drive.Name.c_str(), &info ) != -1)
        #endif
        {
            drive.OptimalBlockSize = info.f_bsize;
            drive.AvailSpace = ((u64)info.f_bsize/1024L) * (u64)info.f_bavail;
            drive.TotalSpace = ((u64)info.f_bsize/1024L) * (u64)info.f_blocks;

            std::cout << " Name = "<<drive.Name.c_str()<<" with Type = ";
            switch ((u32)info.f_type)
            {
                case AFFS_SUPER_MAGIC: std::cout<<"AFFS_SUPER_MAGIC"; break;
                case EFS_SUPER_MAGIC: std::cout<<"EFS_SUPER_MAGIC"; break;
                case EXT_SUPER_MAGIC: std::cout<<"EXT_SUPER_MAGIC"; break;
                case EXT2_OLD_SUPER_MAGIC: std::cout<<"EXT2_OLD_SUPER_MAGIC"; break;
                case EXT2_SUPER_MAGIC: std::cout<<"EXT2_SUPER_MAGIC"; break;
                case HPFS_SUPER_MAGIC: std::cout<<"HPFS_SUPER_MAGIC"; break;
                case ISOFS_SUPER_MAGIC: std::cout<<"ISOFS_SUPER_MAGIC"; break;
                case MINIX_SUPER_MAGIC: std::cout<<"MINIX_SUPER_MAGIC"; break;
                case MINIX_SUPER_MAGIC2: std::cout<<"MINIX_SUPER_MAGIC2"; break;
                case MINIX2_SUPER_MAGIC: std::cout<<"MINIX2_SUPER_MAGIC"; break;
                case MINIX2_SUPER_MAGIC2: std::cout<<"MINIX2_SUPER_MAGIC2"; break;
                case MSDOS_SUPER_MAGIC: std::cout<<"MSDOS_SUPER_MAGIC"; break;
                case NCP_SUPER_MAGIC: std::cout<<"NCP_SUPER_MAGIC"; break;
                case NFS_SUPER_MAGIC: std::cout<<"NFS_SUPER_MAGIC"; break;
                case PROC_SUPER_MAGIC: std::cout<<"PROC_SUPER_MAGIC"; break;
                case SMB_SUPER_MAGIC: std::cout<<"SMB_SUPER_MAGIC"; break;
                case XENIX_SUPER_MAGIC: std::cout<<"XENIX_SUPER_MAGIC"; break;
                case SYSV4_SUPER_MAGIC: std::cout<<"SYSV4_SUPER_MAGIC"; break;
                case SYSV2_SUPER_MAGIC: std::cout<<"SYSV2_SUPER_MAGIC"; break;
                case COH_SUPER_MAGIC: std::cout<<"COH_SUPER_MAGIC"; break;
                case UFS_MAGIC: std::cout<<"UFS_MAGIC"; break;
                case XFS_SUPER_MAGIC: std::cout<<"XFS_SUPER_MAGIC"; break;
                case _XIAFS_SUPER_MAGIC: std::cout<<"_XIAFS_SUPER_MAGIC"; break;
                default: std::cout<<"unknown"; break;
            }
            std::cout << std::endl;
//                    std::cout << "\t\tAvailSpace = "<<drive.AvailSpace<<std::endl;
//                    std::cout << "\t\tTotalSpace = "<<drive.TotalSpace<<std::endl;
//                    std::cout << "\t\tOptimalBlockSize = "<<drive.OptimalBlockSize<<std::endl<<std::endl;
        }
        else
        {
            std::cout << "ERROR :: statfs() returned with ERROR!"<<std::endl;
        }
    }
#else
	#warning WARNING getDriveList() not implemented.
	SDriveInfo drive;
	#if defined (_IRR_WCHAR_FILESYSTEM)
	drive.Name = L"";
	#else
	drive.Name = "";
	#endif
	drive.Type = ELDT_NOT_EXIST;
	drive.AvailSpace = 0;
	drive.TotalSpace = 1;
	drive.OptimalBlockSize = 1024;
	DriveList.push_back( drive );
#endif
}

//! Return count of available physical drives
u32 getDriveCount()
{
	return DriveList.size();
}

//! Return the unique name of the physical drive
path getDriveName( u32 i )
{
	if (i<DriveList.size())
		return DriveList[i].Name;
	else
		return "no valid index";
}

//! Return the type of the physical drive
E_LOGICAL_DRIVE_TYPE getDriveType( u32 i )
{
	if (i<DriveList.size())
		return DriveList[i].Type;
	else
		return ELDT_NOT_EXIST;
}

//! Return the amount of free space on physical drive in Bytes
u64 getDriveAvailSpace( u32 i )
{
	if (i<DriveList.size())
		return DriveList[i].AvailSpace;
	else
		return 0;
}

//! Return the amount of total space on physical drive in Bytes
u64 getDriveTotalSpace( u32 i )
{
	if (i<DriveList.size())
		return DriveList[i].TotalSpace;
	else
		return 0;
}

//! Return the optimal BlockSize for copy-ops on physical drive in Bytes
u32 getDriveOptimalBlockSize( u32 i )
{
	if (i<DriveList.size())
		return DriveList[i].OptimalBlockSize;
	else
		return 1024;
}
//! Return the full filename of the application executeable.
path getExeName()
{
//    Mac OS X: _NSGetExecutablePath() (man 3 dyld)
//    Linux:	readlink /proc/self/exe
//    Solaris: 	getexecname()
//    FreeBSD: 	sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
//    BSD with 	procfs: readlink /proc/curproc/file
//    Windows: 	GetModuleFileName() with hModule = NULL
    path tmp;

//! WinAPI
#if defined(_IRR_WINDOWS_API_)
	#if defined (_IRR_WCHAR_FILESYSTEM)
    wchar_t buf[ MAX_PATH ]; buf[0] = 0;
    if (!GetModuleFileNameW( NULL, (LPWSTR)&buf, MAX_PATH))
	#else
    c8 buf[ MAX_PATH ]; buf[0] = 0;
    if (!GetModuleFileNameA( NULL, (LPSTR)&buf, MAX_PATH))
	#endif
	{
		std::cout<<getErrorText().c_str()<<std::endl;
		return tmp;
	}
    tmp = buf;
	#if defined (_IRR_WCHAR_FILESYSTEM)
    tmp.replace(L'\\', L'/');
    #else
    tmp.replace('\\', '/');
	#endif
//! POSIX
#elif defined (_IRR_POSIX_API_)
	c8 buf[1024];
	memset(buf, 0, sizeof(buf));
	if (readlink("/proc/self/exe", buf, sizeof(buf)-1))
	{
		perror("readlink");
		return tmp;
	}
	tmp = buf;
#else
	#warning getExeName() not implemented!
#endif
    return tmp;
}

//! Return the full path of the application executeable.
path getExePath()
{
	return getFileDir( getExeName() );
}

//! determines if a file exists and would be able to be opened.
bool existFile(const path& filename)
{
#if defined(_IRR_WINDOWS_API_)
	path tmp = filename;
    #if defined(_IRR_WCHAR_FILESYSTEM)
	tmp.replace(L'/', L'\\');
	HANDLE hFile = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    #else
	tmp.replace('/', '\\');
	HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    #endif
	if (hFile == INVALID_HANDLE_VALUE)
    {
         _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
		return false;
    }
	else
	{
		CloseHandle(hFile);
        _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
		return true;
	}
#else
    #if defined(_IRR_WCHAR_FILESYSTEM)
	if (waccess(filename.c_str(), 0) == 0)
    #else
    if (access(filename.c_str(), 0) == 0)
    #endif
    {
        struct stat status;
        stat( filename.c_str(), &status );

        if (!( status.st_mode & S_IFDIR ))
        {
            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
            return true;
        }
    }

    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
	return false;
#endif
}

//! determines if a path exists
bool existDirectory(const io::path& filename)
{
    // is there something to do with virtual filesystems
    bool returnValue = false;

    if (FSType == FILESYSTEM_NATIVE)
    {
//! Windows
#if defined(_IRR_WINDOWS_API_) || defined(_MSC_VER)
		path tmp = filename;
    #if defined(_IRR_WCHAR_FILESYSTEM)
		tmp.replace(L'/', L'\\');
        u32 dwAttrib = GetFileAttributesW(tmp.c_str());
    #else
		tmp.replace('/', '\\');
        u32 dwAttrib = GetFileAttributesA(tmp.c_str());
    #endif
        _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
//! Linux
#else // (defined(_IRR_POSIX_API_) || defined(_IRR_OSX_PLATFORM_))
    #if defined(_IRR_WCHAR_FILESYSTEM)
        if ( waccess( filename.c_str(), 0 ) != -1 )
    #else
        if ( access( filename.c_str(), 0 ) != -1 )
    #endif
        {
            struct stat status;
            stat( filename.c_str(), &status );

            if ( status.st_mode & S_IFDIR )
            {
                returnValue = true;
            }
        }
#endif
    }
    else
    {
        // do something for virtual FS
    }

     // do something for virtual FS
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return returnValue;
}

//! returns the file-size in Bytes
u64	getFileSize(const path& filename)
{
	return 0;
}

//! returns the directory-size in Bytes
u64	getDirectorySize(const path& filename)
{
	return 0;
}

//! creates a file on native FileSystem or a virtual one, depending on FSType
bool createFile(const path& filename, s32 mode)
{
//    HANDLE WINAPI CreateFile(
//  __in      LPCTSTR lpFileName,
//  __in      DWORD dwDesiredAccess,
//  __in      DWORD dwShareMode,
//  __in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
//  __in      DWORD dwCreationDisposition,
//  __in      DWORD dwFlagsAndAttributes,
//  __in_opt  HANDLE hTemplateFile);

    // TouchFile();
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return false;
}

s32 createDirectoryOnce( const path& filename, s32 mode)
{
#if defined (_IRR_WINDOWS_API_)
	return 0;
#else
    s32 status = 0;
    struct stat st;

	#if defined(_IRR_WCHAR_FILESYSTEM)
    if (_wstat(filename.c_str(), &st) != 0)
	#else
	if (stat(filename.c_str(), &st) != 0)
	#endif
    {
        //! Directory does not exist
		#if defined(_IRR_WCHAR_FILESYSTEM)
        //if (_wmkdir(filename.c_str(), (u16)mode) != 0)
        if (_wmkdir(filename.c_str(), mode) != 0)
        #else
        // if (mkdir(filename.c_str(), (u16)mode) != 0)
        if (mkdir(filename.c_str(), mode) != 0)
        #endif
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }

    return status;
#endif
}

//! creates a directory on native FileSystem or a virtual one, depending on FSType
bool createDirectory(const path& filename, s32 mode)
{
	if (existDirectory(filename))
	{
		_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
		return false;
	}

	if (FSType == FILESYSTEM_NATIVE)
	{
//! WinAPI or cmd.exe
#if defined (_IRR_WINDOWS_API_)
		path tmp = filename;
	#if defined(_IRR_WCHAR_FILESYSTEM)
		tmp.replace(L'/', L'\\');
	#else
		tmp.replace('/', '\\');
	#endif

	//! cmd.exe
	#if defined (_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
		path command = _IRR_TEXT("mkdir ");
		command += tmp;
		#if defined(_IRR_WCHAR_FILESYSTEM)
		s32 status = _wsystem(command.c_str());
		#else
		s32 status = system(command.c_str());
		#endif
//		if (status == 0)
//		{
//			returnValue = true;
//		}

	//! WinAPI
	#else
		// something recursive
		BOOL result = FALSE;
		#if defined(_IRR_WCHAR_FILESYSTEM)
		result = CreateDirectoryW( filename.c_str(), NULL);
		#else
		result = CreateDirectoryA( filename.c_str(), NULL);
		#endif
		// end something recursive
	#endif
//! POSIX
#elif defined(_IRR_POSIX_API_)
		s32 status = 0;
	#if defined(_IRR_WCHAR_FILESYSTEM)
		wchar_t *pp, *sp;
		wchar_t *copypath = _wstrdup(filename.c_str());
	#else
		char *pp, *sp;
		char *copypath = strdup(filename.c_str());
	#endif
		pp = copypath;

	#if defined(_IRR_WCHAR_FILESYSTEM)
		while (status == 0 && (sp = _wstrchr(pp, L'/')) != 0)
	#else
		while (status == 0 && (sp = strchr(pp, '/')) != 0)
    #endif
		{
			if (sp != pp)
			{
				/* Neither root nor double slash in path */
				*sp = '\0';

				status = createDirectoryOnce(copypath, mode);

				*sp = '/';
			}
			pp = sp + 1;
		}
		if (status == 0)
			status = createDirectoryOnce(filename.c_str(), mode);
		free(copypath);
#else
	#warning createDirectory() not implemented.
#endif
	}
	else
	{
		// Do something for Virtual FS
	}
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return existDirectory(filename);
}

//! deletes a file from native FileSystem or a virtual one, depending on FSType
bool removeFile(const path& filename)
{
    // is there something to do with virtual filesystems
    bool returnValue = false;

    if (FSType == FILESYSTEM_NATIVE)
    {
//! Windows
#if defined(_IRR_WINDOWS_API_)
    #if defined(_IRR_WCHAR_FILESYSTEM)
        if ( DeleteFileW( filename.c_str() ))
    #else
        if ( DeleteFileA( filename.c_str() ))
    #endif
        {
            returnValue = true;
        }
        else
        {
            std::cerr<<getErrorText().c_str()<<std::endl;
        }
//! Linux
#elif defined(_IRR_POSIX_API_)
    #if defined(_IRR_WCHAR_FILESYSTEM)
        if ( _wremove( filename.c_str() ) == 0 )
    #else
        if ( remove( filename.c_str() ) == 0 )
    #endif
        {
            returnValue = true;
        }
        else
        {
			std::cout<<getErrorText().c_str()<<std::endl;
        }
#else
	#warning removeFile() not implemented.
#endif
    }
    else
	{
		// do something for virtual FS
	}
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return returnValue;
}

//! deletes a directory from native FileSystem or a virtual one, depending on FSType
bool removeDirectory(const path& filename)
{
	if (!existDirectory(filename))
	{
		_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
		return false;
	}

    if (FSType == FILESYSTEM_NATIVE)
    {
//! Windows
#if defined(_IRR_WINDOWS_API_)
	//! make path Windows conform
		path tmp = filename;
	#if defined(_IRR_WCHAR_FILESYSTEM)
		tmp.replace(L'/', L'\\');
	#else
		tmp.replace('/', '\\');
	#endif

	//! cmd.exe
	#if defined(_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
		path command = _IRR_TEXT("rmdir /S /Q ");
		command += tmp;
		#if defined(_IRR_WCHAR_FILESYSTEM)
		_wsystem(command.c_str());
		#else
		system(command.c_str());
		#endif
	//! shell (Advanced API)
	#elif defined(_IRR_USE_WINDOWS_SHELL_FOR_FILE_OPERATIONS_)
		#if defined(_IRR_WCHAR_FILESYSTEM)
		SHFILEOPSTRUCTW file_op = {
		#else
		SHFILEOPSTRUCT file_op = {
		#endif
			NULL,
			FO_DELETE,
			tmp.c_str(),
			_IRR_TEXT(""),
			FOF_NOCONFIRMATION |
			FOF_NOERRORUI |
			FOF_SILENT,
			false,
			0,
			_IRR_TEXT("")
		};
        #if defined(_IRR_WCHAR_FILESYSTEM)
		SHFileOperationW(&file_op);
		#else
		SHFileOperationA(&file_op);
		#endif
	//! WinAPI
	#else
		#warning removeDirectory() not fully implemented
		#if defined(_IRR_WCHAR_FILESYSTEM)
		if ( RemoveDirectoryW( tmp.c_str() ) == FALSE )
		#else
		if ( RemoveDirectoryA( tmp.c_str() ) == FALSE )
		#endif
		{
			std::cout << getErrorText().c_str() << std::endl;
		}
	#endif
//! POSIX
#elif defined(_IRR_POSIX_API_)
	//! bash
	#if defined(_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
		path command = _IRR_TEXT("rmdir ");
		command += filename;
		#if defined(_IRR_WCHAR_FILESYSTEM)
		s32 status = _wsystem(command.c_str());
		#else
		s32 status = system(command.c_str());
		#endif
		if (status!=0)
    //! native API
	#else
		#warning removeDirectory() not fully implemented
		#if defined(_IRR_WCHAR_FILESYSTEM)
		if ( _wrmdir( filename.c_str() ) != 0 )
		#else
		if ( rmdir( filename.c_str() ) != 0 )
		#endif
	#endif
		{
			std::cout<<getErrorText().c_str()<<std::endl;
		}
#else
	#warning removeDirectory() not implemented.
#endif
	}
	else
	{
        // Do something for virtual FS
    }

	_IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return !existDirectory(filename);
}

//! copy the contents of a directory to a new location
bool copyFile(const path& dstName, const path& srcName)
{
    // is there something to do with virtual filesystems
    bool returnValue = false;

    if (FSType == FILESYSTEM_NATIVE)
    {
//! Windows
#if defined(_IRR_WINDOWS_API_)
    path srcTmp = srcName;
    path dstTmp = dstName;
    #if defined(_IRR_WCHAR_FILESYSTEM)
    srcTmp.replace(L'/', L'\\');
    dstTmp.replace(L'/', L'\\');
    #else
    srcTmp.replace('/', '\\');
    dstTmp.replace('/', '\\');
    #endif

    //! cmd.exe
	#if defined(_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
        path command = _IRR_TEXT("copy ");
        command += srcTmp;
        command += _IRR_TEXT(" ");
        command += dstTmp;
        #if defined(_IRR_WCHAR_FILESYSTEM)
        s32 status = _wsystem(command.c_str());
        #else
        s32 status = system(command.c_str());
        #endif
        if (status==0)
    #else
        #warning copyFile() not fully implemented!
        BOOL bFailIfExist = FALSE;
        #if defined(_IRR_WCHAR_FILESYSTEM)
        if ( CopyFileW( srcTmp.c_str(), dstTmp.c_str(), bFailIfExist ) == 0 )
        #else
        if ( CopyFileA( srcTmp.c_str(), dstTmp.c_str(), bFailIfExist ) == 0 )
        #endif
    #endif
        {
            returnValue = true;
        }
        else
        {
            std::cerr<<getErrorText().c_str()<<std::endl;
        }
//! Linux
#elif defined(_IRR_POSIX_API_)
	//! bash
	#if defined(_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
        path command = _IRR_TEXT("/bin/cp -f -R ");
        command += srcName;
        command += _IRR_TEXT(" ");
        command += dstName;

        //! use popen
        #if _POSIX_C_SOURCE >= 2 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE
        FILE *f = popen(command.c_str(), "r");
        if (!f)
        {
            std::cout<<"Failed to run system command"<<std::endl;
            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
            return false;
        }
        returnValue = true;
        pclose(f);
        //!
        #else
        s32 status = system()
        #endif
    //! native API
    #else
        #if defined(_IRR_WCHAR_FILESYSTEM)
        if ( _cp( srcName.c_str(), dstName.c_str() ) == 0 )
        #else
        if ( cp( srcName.c_str(), dstName.c_str() ) == 0 )
        #endif
        {
            returnValue = true;
        }
        else
        {
            std::cerr<<getErrorText().c_str()<<std::endl;
        }
    #endif
#else
	#warning copyFile() not implemented.
#endif
    }
    else
    {
        // do something for virtual FS i dont care
    }
    // do something for virtual FS i dont care
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return returnValue;
}

//! copy the contents of a directory to a new location
bool copyDirectory(const path& dstName, const path& srcName)
{
    // is there something to do with virtual filesystems
    bool returnValue = false;

    if (FSType == FILESYSTEM_NATIVE)
    {
//! Windows
#if defined(_IRR_WINDOWS_API_)
		path srcTmp = srcName;
		path dstTmp = dstName;
	#if defined(_IRR_WCHAR_FILESYSTEM)
		srcTmp.replace(L'/', L'\\');
		dstTmp.replace(L'/', L'\\');
	#else
		srcTmp.replace('/', '\\');
		dstTmp.replace('/', '\\');
	#endif
	#if defined (_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
		path command = _IRR_TEXT("copy /Y ");
		command += srcTmp;
		command += _IRR_TEXT(" ");
		command += dstTmp;
		#if defined(_IRR_WCHAR_FILESYSTEM)
		s32 result = _wsystem(command.c_str());
		#else
		s32 result = system(command.c_str());
		#endif
	#else
		HANDLE hFind;
		#if defined(_IRR_WCHAR_FILESYSTEM)
		WIN32_FIND_DATAW FindFileData;
		wchar_t l_szTmp[1025] = {0};
		wchar_t l_szSrcPath[1025] = {0};
		wchar_t l_szDesPath[1025] = {0};
		wchar_t l_szNewSrcPath[1025] = {0};
		wchar_t l_szNewDesPath[1025] = {0};
		memcpy(l_szTmp,srcTmp.c_str(),sizeof(wchar_t)*1024);
		memcpy(l_szSrcPath,srcTmp.c_str(),sizeof(wchar_t)*1024);
		memcpy(l_szDesPath,dstTmp.c_str(),sizeof(wchar_t)*1024);
		wcscat(l_szTmp,L"*");
		hFind = FindFirstFileW(l_szTmp, &FindFileData);
		if(hFind == NULL) return false;
		do
		{
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(wcscmp(FindFileData.cFileName,L"."))
				{
					if(wcscmp(FindFileData.cFileName,L".."))
					{
						wprintf(L"The Directory found is %ls ", FindFileData.cFileName);
						wsprintf(l_szNewDesPath,L"%ls%ls\"",l_szDesPath,FindFileData.cFileName);
						wsprintf(l_szNewSrcPath,L"%ls%ls\"",l_szSrcPath,FindFileData.cFileName);
						CreateDirectoryW(l_szNewDesPath,NULL);
						copyDirectory(path(l_szNewSrcPath),path(l_szNewDesPath));
					}
				}
			}
			else
			{
				wprintf (L"The File found is %ls ", FindFileData.cFileName);
				wchar_t l_szSrcFile[1025] = {0};
				wchar_t l_szDesFile[1025] = {0};
				wsprintf(l_szDesFile,L"%ls%ls",l_szDesPath,FindFileData.cFileName);
				wsprintf(l_szSrcFile,L"%ls%ls",l_szSrcPath,FindFileData.cFileName);
				BOOL l_bRet = CopyFileW(l_szSrcFile,l_szDesFile,TRUE);
			}
		}
		while(FindNextFileW(hFind, &FindFileData));
		FindClose(hFind);
		return true;

		#else
		WIN32_FIND_DATAA FindFileData;
		c8 l_szTmp[1025] = {0};
		c8 l_szSrcPath[1025] = {0};
		c8 l_szDesPath[1025] = {0};
		c8 l_szNewSrcPath[1025] = {0};
		c8 l_szNewDesPath[1025] = {0};
		memcpy(l_szTmp,srcTmp.c_str(),1024);
		memcpy(l_szSrcPath,srcTmp.c_str(),1024);
		memcpy(l_szDesPath,dstTmp.c_str(),1024);
		strcat(l_szTmp,"*");
		hFind = FindFirstFileA(l_szTmp, &FindFileData);
		if(hFind == NULL) return false;
		do
		{
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(strcmp(FindFileData.cFileName,"."))
				{
					if(strcmp(FindFileData.cFileName,".."))
					{
						printf ("The Directory found is %s ", FindFileData.cFileName);
						sprintf(l_szNewDesPath,"%s%s\"",l_szDesPath,FindFileData.cFileName);
						sprintf(l_szNewSrcPath,"%s%s\"",l_szSrcPath,FindFileData.cFileName);
						CreateDirectoryA(l_szNewDesPath,NULL);
						copyDirectory(path(l_szNewSrcPath),path(l_szNewDesPath));
					}
				}
			}
			else
			{
				printf ("The File found is %s ", FindFileData.cFileName);
				char l_szSrcFile[1025] = {0};
				char l_szDesFile[1025] = {0};
				sprintf(l_szDesFile,"%s%s",l_szDesPath,FindFileData.cFileName);
				sprintf(l_szSrcFile,"%s%s",l_szSrcPath,FindFileData.cFileName);
				BOOL l_bRet = CopyFileA(l_szSrcFile,l_szDesFile,TRUE);
			}
		}
		while(FindNextFileA(hFind, &FindFileData));
		FindClose(hFind);
		returnValue = true;
		#endif
	#endif
//! POSIX
#elif defined (_IRR_POSIX_API_)
	#if defined (_IRR_USE_SYSTEM_FOR_FILE_OPERATIONS_)
		path command = _IRR_TEXT("cp -f -R");
		command += srcName;
		command += _IRR_TEXT(" ");
		command += dstName;
		#if defined(_IRR_WCHAR_FILESYSTEM)
		s32 length = wcstombs( NULL, command.c_str(), 0);
		if (!length)
        {
            std::cout<<"ERROR in removeDirectory() could not retrieve needed mbstring-size"<<std::endl;
            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
            return false;
        }
        c8* buffer = new c8[length+1];
        if (!buffer)
        {
            std::cout<<"ERROR in removeDirectory() could not allocate buffer with size ("<<length<<")"<<std::endl;
            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
            return false;
        }
        memset((void*)buffer, 0, length+1);
        length = wcstombs( buffer, command.c_str(), length+1);
        core::stringc tmpCStr = buffer;
        delete [] buffer;
		s32 result = system(tmpCStr.c_str());
		#else
		s32 result = system(command.c_str());
		#endif
        if (result!=0)
        {
            std::cout<<getErrorText().c_str()<<std::endl;
            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
            return false;
        }
        returnValue = true;
	#else
        #warning copyDirectory() not implemented.
    #endif
#else
	#warning copyDirectory() not implemented.
#endif
    }
    else
    {
        // do something for virtual FS i dont care
    }
    // do something for virtual FS i dont care
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return returnValue;
}

//! moves one or more files to a new location
bool moveFile(const path& dstName, const path& srcName)
{
    // is there something to do with virtual filesystems
    bool returnValue = false;

    if (FSType == FILESYSTEM_NATIVE)
    {
//! Windows
#if defined(_IRR_WINDOWS_API_)
	#warning moveFile() not implemented.
//    #if defined(_IRR_WCHAR_FILESYSTEM)
//        if ( RemoveDirectoryW( filename.c_str() ) == 0 )
//    #else
//        if ( RemoveDirectoryA( filename.c_str() ) == 0 )
//    #endif
//        {
//            returnValue = true;
//        }
//        else
//        {
//            os::Printer::log( getErrorText().c_str() );
//        }

//! Linux
#elif defined(_IRR_POSIX_API_)
	#warning moveFile() not implemented.
//    #if defined(_IRR_WCHAR_FILESYSTEM)
//        if ( _wrmdir( filename.c_str() ) == 0 )
//    #else
//        if ( rmdir( filename.c_str() ) == 0 )
//    #endif
//        {
//            returnValue = true;
//        }
//        else
//        {
//            perror( "ERROR in removeDirectory() const" );
//        }
#else
	#warning moveFile() not implemented.
#endif
    }
    else
    {
        // do something for virtual FS i dont care
    }
    // do something for virtual FS i dont care
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return returnValue;
}

//! copy the contents of a directory to a new location
bool moveDirectory(const path& dstName, const path& srcName)
{
    // is there something to do with virtual filesystems
    bool returnValue = false;

    if (FSType == FILESYSTEM_NATIVE)
    {
//! Windows
#if defined(_IRR_WINDOWS_API_)
	#warning moveDirectory() not implemented.
//    #if defined(_IRR_WCHAR_FILESYSTEM)
//        if ( RemoveDirectoryW( filename.c_str() ) == 0 )
//    #else
//        if ( RemoveDirectoryA( filename.c_str() ) == 0 )
//    #endif
//        {
//            returnValue = true;
//        }
//        else
//        {
//            os::Printer::log( getErrorText().c_str() );
//        }

//! Linux
#elif defined (_IRR_POSIX_API_)
	#warning moveDirectory() not implemented.
//    #if defined(_IRR_WCHAR_FILESYSTEM)
//        if ( _wrmdir( filename.c_str() ) == 0 )
//    #else
//        if ( rmdir( filename.c_str() ) == 0 )
//    #endif
//        {
//            returnValue = true;
//        }
//        else
//        {
//            perror( "ERROR in removeDirectory() const" );
//        }
#else
	#warning moveDirectory() not implemented.
#endif
    }
    else
    {
        // do something for virtual FS i dont care
    }
    // do something for virtual FS i dont care
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX
    return returnValue;
}

//
//void test_for_IFileSystem( io::IFileSystem* fs )
//{
//    io::path testName = "IFileSystemTest";
//    io::path fileName = testName + ".log";
//    std::ofstream fout( fileName.c_str(), std::ios::out);
//    if (fout.is_open())
//    {
//        fout<<"test_for_IFileSystem LogFile"<<std::endl<<std::endl;
//    }
//    else
//    {
//        std::cout << "ERROR in test_for_IFileSystem, could not open LogFile " << std::endl;
//        return;
//    }
//
//    if (!fs)
//    {
//        fout<<"ERROR:: no valid IFileSystem Pointer."<<std::endl;
//        fout.close();
//        return;
//    }
//
//    io::path cwd = fs->getWorkingDirectory();
//
//#if defined(_IRR_WINDOWS_API_)
//    fout<<" _IRR_WINDOWS_API_ defined."<<std::endl;
//#endif
//#if defined(_IRR_WCHAR_FILESYSTEM)
//    fout<<" _IRR_WCHAR_FILESYSTEM defined."<<std::endl;
//#else
//    fout<<" _IRR_WCHAR_FILESYSTEM not defined."<<std::endl;
//#endif
//#if defined(_IRR_POSIX_API_)
//    fout<<" _IRR_POSIX_API_ defined."<<std::endl;
//#else
//    fout<<" _IRR_POSIX_API_ not defined."<<std::endl;
//#endif
//	fout<<std::endl;
//
//	fs->updateDriveList();
//    fout<<" updateDriveList()"<<std::endl;
//
//    u32 driveCount = fs->getDriveCount();
//
//    fout<<" getDriveCount() = "<<driveCount<<std::endl;
//    for (u32 i=0; i<driveCount; i++)
//    fout<<"\t("<<i<<")"
//		<<" Name = "<<fs->getDriveName(i).c_str()
//		<<" Type = "<<io::E_LOGICAL_DRIVE_TYPE_NAMES[fs->getDriveType(i)]
//		<<", AvailBytes = "<<fs->getDriveAvailSpace(i)
//		<<", TotalBytes = "<<fs->getDriveTotalSpace(i)
//		<<std::endl;
//	fout<<std::endl;
//    fout<<" getWorkingDirectory() = "<<fs->getWorkingDirectory().c_str()<<std::endl;
//    fout<<" changeWorkingDirectoryTo() = "
//		<<((fs->changeWorkingDirectoryTo("../../media"))?"true":"false")<<std::endl;
//    fout<<" getWorkingDirectory() = "<<fs->getWorkingDirectory().c_str()<<std::endl;
//    fout<<" getExeName() = "<< fs->getExeName().c_str() <<std::endl;
//    fout<<" getExePath() = "<< fs->getExePath().c_str() <<std::endl<<std::endl;
//
//    io::path A = cwd + "/" + testName + "/A/A1/A2/A3";
//    io::path B = cwd + "/" + testName + "/B/B1/B2.txt";
//    io::path C = cwd + "/" + testName + "/C/C1";
//
//    fout<<" A = "<<A.c_str()<<std::endl;
//    fout<<" B = "<<B.c_str()<<std::endl;
//    fout<<" C = "<<C.c_str()<<std::endl<<std::endl;
//
//    fout<<" existFile(A) = "<< ((fs->existFile(A))?"true":"false")<<std::endl;
//    fout<<" existFile(B) = "<< ((fs->existFile(B))?"true":"false")<<std::endl;
//    fout<<" existFile(C) = "<< ((fs->existFile(C))?"true":"false")<<std::endl<<std::endl;
//
//    fout<<" existDirectory(A) = "<< ((fs->existDirectory(A))?"true":"false")<<std::endl;
//    fout<<" existDirectory(B) = "<< ((fs->existDirectory(B))?"true":"false")<<std::endl;
//    fout<<" existDirectory(C) = "<< ((fs->existDirectory(C))?"true":"false")<<std::endl<<std::endl;
//
////    fout<<" createDirectory( A ) = "<< ((fs->createDirectory(A))?"true":"false")<<std::endl;
////    fout<<" createDirectory( B ) = "<< ((fs->createDirectory(B))?"true":"false")<<std::endl;
////    fout<<" createDirectory( C ) = "<< ((fs->createDirectory(C))?"true":"false")<<std::endl<<std::endl;
//
//    fout<<" existFile(A) = "<< ((fs->existFile(A))?"true":"false")<<std::endl;
//    fout<<" existFile(B) = "<< ((fs->existFile(B))?"true":"false")<<std::endl;
//    fout<<" existFile(C) = "<< ((fs->existFile(C))?"true":"false")<<std::endl<<std::endl;
//
//    fout<<" existDirectory(A) = "<< ((fs->existDirectory(A))?"true":"false")<<std::endl;
//    fout<<" existDirectory(B) = "<< ((fs->existDirectory(B))?"true":"false")<<std::endl;
//    fout<<" existDirectory(C) = "<< ((fs->existDirectory(C))?"true":"false")<<std::endl<<std::endl;
////    fout <<
////    fs->createFile( p0 + "createFile.txt");
////    fs->
//    fout << "test_for_IFileSystem END." << std::endl;
//    fout.close();
//}
//
//void test_for_IImage( IrrlichtDevice* device)
//{
//	if (!device)
//		return;
//
//	ILogger* logger = device->getLogger();
//	video::IVideoDriver* driver = device->getVideoDriver();
//    io::IFileSystem* fs = device->getFileSystem();
//
//    fs->createDirectory( fs->getExeName()+"/test_for_IImage");
//
////! init random number generator
//    srand(device->getTimer()->getRealTime());
//
////! [0] create image
//    video::IImage* im0 = new video::CImage( video::ECF_A8R8G8B8, core::dimension2du(1024,512));
//
////    u32 index ui0 = im0->addImageWriterClass( IImageWriter* pWriter )
//
////! [1] horizontal lines with one color
//    im0->fill( video::SColor(0,0,0,0) );
//
//    for (s32 y = 0; y < im0->getHeight(); y++)
//    {
//        video::SColor c1( rand()%255, rand()%255, rand()%255, rand()%255);
//        im0->drawLine( core::position2di(0,y), core::position2di(im0->getWidth()-1, y), c1);
//    }
//
////    im0->resize( core::dimension2du(2000,1000), video::SColor(255,255,255,0) );
//
//    im0->save( driver, "ImageFactory_example_001.png");
//
////! [2]
//    im0->fill( video::SColor(0,0,0,0) );
//
//    for (s32 y = 0; y < im0->getHeight(); y++)
//    {
//        video::SColor c1( rand()%255, rand()%255, rand()%255, rand()%255);
//        video::SColor c2( rand()%255, rand()%255, rand()%255, rand()%255);
//        im0->drawLine( core::position2di(0,y), core::position2di(im0->getWidth()-1,y), c1, c2);
//    }
//
//    im0->save( driver, "ImageFactory_example_002.png");
//
////! [3-6] scale tests
//    im0->load( driver, "../../media/irrlichtlogo.jpg");
//
//    video::IImage* im1;
//
//    im1 = im0->scale( core::dimension2du(2000, 1000), video::EISO_NEAREST);
//    im1->save( driver, "ImageFactory_example_003.png");
//    im1->drop();
//
//    im1 = im0->scale( core::dimension2du(2000, 1000), video::EISO_FAST);
//    im1->save( driver, "ImageFactory_example_004.png");
//    im1->drop();
//
//    im1 = im0->scale( core::dimension2du(2000, 1000), video::EISO_BILINEAR);
//    im1->save( driver, "ImageFactory_example_005.png");
//    im1->drop();
//
//    im1 = im0->scale( core::dimension2du(2000, 1000), video::EISO_BICUBIC);
//    im1->save( driver, "ImageFactory_example_006.png");
//    im1->drop();
//
////! [7] rotation tests
//    im0->load( driver, "../../media/earth.jpg");
//    im0->correctColors();
//
//    io::path name = "ImageFactory_example_007_";
//
//    for (s32 phi = 0; phi < 360; phi += 15)
//    {
//        io::path fileName = name;
//        if (phi<100) fileName+="0";
//        fileName += phi;
//        fileName += ".png";
//
//        im1 = im0->rotate( (f32)phi );
//        im1->save( driver, fileName);
//        im1->drop();
//    }
//
//}
