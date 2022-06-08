#include <iostream>
#include <fstream>
#include <string>
#include "Blowfish.h"

using namespace std;

union
{
    unsigned long longx;
    char charx[4];
} temp;

union
{
    unsigned long longx;
    char charx[4];
} temp2;

int main(int argc, char **argv)
{
    // Check to make sure a file was specified
    if (argc <= 2)
    {
        cout << "Usage: " << string(argv[0]) << " <input filename> <output filename>" << endl;
        cout << "Decrypts Luna Engine readable Data" << endl;
        cout << "Copyright 2008, Aren Villanueva. (Yomogi-Soft / Agnostic-Games)" << endl;
        return 0;
    }

    cout << "Blowfish Decryptor. Copyright 2008, Aren Villanueva. (Yomogi-Soft / Agnostic-Games)" << endl;

    // Check to make sure the file exists
    fstream inFile(string(argv[1]).c_str(), ios::in | ios::binary);
    if (!inFile.is_open())
    {
        cout << string(argv[1]) << " not found. Aborting!" << endl;
        return 0;
    }

    // same goes for the output file
    fstream outFile(string(argv[2]).c_str(), ios::out | ios::binary);
    if (!outFile.is_open())
    {
        cout << string("cannot write to ") << string(argv[2]) << endl;
        return 0;
    }

    // Initialize the blowfish encryption scheme
    Encryption::Blowfish *bfenc = new Encryption::Blowfish();
    bfenc->initialize("9pl=qoa&Lu6oUpHLUDluhi+rLu4+aFRIuS_Ia$@AqoEth=uC5OUBrIESWIETHiuy", 50);

    // Find out the length of the data (we minus one as the first byte is the count of padding bytes)
    unsigned int length;
    inFile.seekg(0, ios::end);
    length = inFile.tellg();
    length--;
    inFile.seekg(0, ios::beg);

    cout << "Processing file of size: " << length << endl;

    char * buffer;
    char * buffer2;

    char *p;
    unsigned long *ubuffer;

    buffer2 = new char[length];
    buffer = new char[length];

    // Zero out the buffers
    for (unsigned int i = 0; i < length; ++i)
        buffer[i] = buffer2[i] = 0;

    // Read how many bytes we padded the result with
    inFile.read(buffer, 1);
    char addedBytes = buffer[0];
    // Read the rest of the data
    inFile.read(buffer, length);

    // Figure out how many longs we stored
    unsigned int len4 = length/4;

    // Generate the buffer required to store the longs
    ubuffer = new unsigned long[len4];

    // Zero out the buffer
    for (unsigned i = 0; i < len4; ++i)
        ubuffer[i] = 0;

    p = buffer;

    // Populate the buffer with the stored longs
    for (unsigned int i = 0; i < len4; i++)
    {
        for (unsigned int z = 0; z < 4; ++z)
            temp.charx[z] = *p++;

        ubuffer[i] = temp.longx;
    }

    // Figure out how many pairs are available
    unsigned int len8 = len4 / 2;

    p = buffer2;

    cout << "Decrypting..." << endl;

    // Do the decryption
    for (unsigned int i = 0; i < len8; ++i)
    {
        temp.longx = ubuffer[(i*2)];
        temp2.longx = ubuffer[(i*2)+1];
        bfenc->decipher(&temp.longx, &temp2.longx);
        for (unsigned int z = 0; z < 4; ++z)
            *p++ = temp.charx[z];
        for (unsigned int z = 0; z < 4; ++z)
            *p++ = temp2.charx[z];
    }

    cout << endl << "Removing " << short(addedBytes) << " bytes of padding" << endl;
    // And write out the file.
    int finalSize = length - addedBytes;
    cout << "Writing " << finalSize << " bytes." << endl;
    outFile.write(buffer2, finalSize);
    cout << "Done!" << endl;

    inFile.close();
    outFile.close();

    delete bfenc;
}
