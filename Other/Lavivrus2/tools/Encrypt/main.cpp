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
        cout << "Encrypts the data to be usable by the Luna Engine" << endl;
        cout << "Copyright 2008, Aren Villanueva. (Yomogi-Soft / Agnostic-Games)" << endl;
        return 0;
    }

    cout << "Blowfish Encryptor. Copyright 2008, Aren Villanueva. (Yomogi-Soft / Agnostic-Games)" << endl;

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

    // get the length of the file.
    unsigned int length;
    inFile.seekg(0, ios::end);
    length = inFile.tellg();
    inFile.seekg(0, ios::beg);

    cout << "Processing file of size: " << length << endl;

    char * buffer;
    char * buffer2;

    char *p;
    unsigned long *ubuffer;

    // Store how many we needed to pad.
    char addedBytes = 0;

    // Pad the data. We need to have the data in pairs of longs (8 bytes) and no less.
    while ((length / 8) != (float(length) / 8.0f))
    {
        length++;
        addedBytes++;
    }

    cout << "Data padded with " << short(addedBytes) << " bytes" << endl;

    // Create the buffers with the padded sized length
    buffer = new char[length];
    buffer2 = new char[length];

    // Zero out the buffers
    for (unsigned int i = 0; i < length; ++i)
        buffer[i] = buffer2[i] = 0;

    // read the whole file into memory
    inFile.read(buffer, length - short(addedBytes));

    // Figure out how many longs can be made with these
    unsigned int len4 = length/4;

    // Generate the buffer required to store the longs
    ubuffer = new unsigned long[len4];

    // Zero out the buffer
    for (unsigned i = 0; i < len4; ++i)
        ubuffer[i] = 0;

    p = buffer;

    // Populate the longs
    for (unsigned int i = 0; i < len4; i++)
    {
        for (unsigned int z = 0; z < 4; ++z)
            temp.charx[z] = *p++;

        ubuffer[i] = temp.longx;
    }

    // Calculate how many pairs of longs we can have
    unsigned int len8 = len4 / 2;

    p = buffer2;

    cout << "Encrypting..." << endl;

    // do the encryption
    for (unsigned int i = 0; i < len8; ++i)
    {
        temp.longx = ubuffer[(i*2)];
        temp2.longx = ubuffer[(i*2)+1];
        bfenc->encipher(&temp.longx, &temp2.longx);
        for (unsigned int z = 0; z < 4; ++z)
            *p++ = temp.charx[z];
        for (unsigned int z = 0; z < 4; ++z)
            *p++ = temp2.charx[z];
    }

    // write out the result
    cout << endl << "Writing file." << endl;
    outFile.write(&addedBytes, 1);
    outFile.write(buffer2, length);

    cout << "Wrote " << length+1 << " bytes to " << argv[2] << endl;
    cout << "Done!" << endl;

    inFile.close();
    outFile.close();

    delete bfenc;
    delete [] buffer2;
    delete [] buffer;
}
