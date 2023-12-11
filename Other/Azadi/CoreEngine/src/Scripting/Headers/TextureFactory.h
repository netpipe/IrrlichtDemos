#ifndef AZTEXTUREFACTORY_H
#define AZTEXTUREFACTORY_H

class azTextureFactory
{
    public:
        virtual ~azTextureFactory() {}

        virtual void loadTexture(const std::string& filename) = 0;
		virtual void applyTexture(const std::string& filename) = 0;
		virtual void flushFactory() = 0;
};

#endif
