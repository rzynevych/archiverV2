#include "Compressor.hpp"
#include "CompressIO.hpp"
#include "DecompressIO.hpp"
#include "Decompressor.hpp"

int main(int argc, char **argv)
{
    vector<string> files;
    files.push_back("IMG.jpg");
    CompressIO io(&files);
    Compressor compressor(&io);
    try {
        compressor.run();
    } catch (exception &ex) {
        printf("%s\n", ex.what());
    }
    // DecompressIO io("file.av2");
    // Decompressor decompessor(io);
    // decompessor.run();
    return 0;
}