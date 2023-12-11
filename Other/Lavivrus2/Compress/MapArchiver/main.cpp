#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    if (argc <= 4)
    {
        printf("Usage: %s mapdescriptor.desc mapassets.xml mapassets.zip output.map\n", argv[0]);
        return 0;
    }

    FILE *f = fopen(argv[4], "wb");
    if (!f)
    {
        printf("There was an error creating the output file");
        return -1;
    }

    int descNameSize = strlen(argv[1]);
    int mapAssetSize = strlen(argv[2]);
    int mapAssetZipSize = strlen(argv[3]);

    fwrite(&descNameSize, 1, sizeof(descNameSize), f);

    fwrite(argv[1], 1, descNameSize, f);

    FILE *md = fopen(argv[1], "rb");

    fseek(md, 0, SEEK_END);

    int size = ftell(md);

    fseek(md, 0, SEEK_SET);

    fwrite(&size, 1, sizeof(size), f);

    char *temp = new char[size];

    fread(temp, 1, size, md);
    fwrite(temp, 1, size, f);

    fclose(md);

    delete [] temp;

    fwrite(&mapAssetSize, 1, sizeof(mapAssetSize), f);

    fwrite(argv[2], 1, mapAssetSize, f);

    FILE *ma = fopen(argv[2], "rb");

    fseek(ma, 0, SEEK_END);

    size = ftell(ma);

    fseek(ma, 0, SEEK_SET);

    fwrite(&size, 1, sizeof(size), f);

    temp = new char[size];

    fread(temp, 1, size, ma);
    fwrite(temp, 1, size, f);

    fclose(ma);

    delete [] temp;

    fwrite(&mapAssetZipSize, 1, sizeof(mapAssetZipSize), f);

    fwrite(argv[3], 1, mapAssetZipSize, f);

    FILE *maz = fopen(argv[3], "rb");

    fseek(maz, 0, SEEK_END);

    size = ftell(ma);

    fseek(maz, 0, SEEK_SET);

    fwrite(&size, 1, sizeof(size), f);

    fread(temp, 1, size, maz);
    fwrite(temp, 1, size, f);

    fclose(maz);

    delete [] temp;

    printf("%s created successfully\n", argv[4]);
}
