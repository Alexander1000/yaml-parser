#include <yaml-parser.h>
#include <io-buffer.h>

namespace YamlParser
{
    Stream::Stream(IOBuffer::CharStream* charStream) {
        this->charStream = charStream;
    }

    void* Stream::getNextToken() {
        char* symbol = this->charStream->getNext();
        if (symbol == NULL) {
            return NULL;
        }
        return NULL;
    }
}
