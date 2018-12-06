#ifndef IRRROCKETFILEINTERFACE_H
#define IRRROCKETFILEINTERFACE_H

#include <Rocket/Core/FileInterface.h>
namespace irr
{
    namespace io
    {
        class IFileSystem;
    }
}
class IrrRocketFileInterface : public Rocket::Core::FileInterface
{
    public:
        /** Default constructor */
        IrrRocketFileInterface(irr::io::IFileSystem* fileSystem);
        /** Default destructor */
        virtual ~IrrRocketFileInterface();

        // Opens a file.
        Rocket::Core::FileHandle Open(const Rocket::Core::String& path);

        // Closes a previously opened file.
        void Close(Rocket::Core::FileHandle file);

        // Reads data from a previously opened file.
        size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file);

        // Seeks to a point in a previously opened file.
        bool Seek(Rocket::Core::FileHandle file, long offset, int origin);

        // Returns the current position of the file pointer.
        size_t Tell(Rocket::Core::FileHandle file);

    protected:
        irr::io::IFileSystem* FileSystem;
    private:
};

#endif // IRRROCKETFILEINTERFACE_H
