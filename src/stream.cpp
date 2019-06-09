#include <yaml-parser.h>
#include <io-buffer.h>

namespace YamlParser
{
    Stream::Stream(IOBuffer::CharStream* charStream) {
        this->charStream = charStream;
    }

    void* Stream::getNextToken() {
        return NULL;
    }
}
