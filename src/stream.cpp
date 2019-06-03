#include <yaml-parser.h>
#include <io-buffer.h>

namespace YamlParser
{
    Stream::Stream(IOBuffer::IOReader *reader) {
        this->reader = reader;
    }

    void* Stream::getNextToken() {
        return NULL;
    }
}
