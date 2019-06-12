#include <yaml-parser.h>
#include <io-buffer.h>

#define STREAM_MODE_PLAIN 0

namespace YamlParser
{
    Stream::Stream(IOBuffer::CharStream* charStream) {
        this->charStream = charStream;

        this->mode = STREAM_MODE_PLAIN;
    }

    Token::Token* Stream::getNextToken() {
        char* symbol = this->charStream->getNext();
        if (symbol == NULL) {
            return NULL;
        }

        switch (this->mode) {
            case STREAM_MODE_PLAIN:
                break;
            default:
                throw new InvalidStreamModeException;
        }

        return NULL;
    }
}
