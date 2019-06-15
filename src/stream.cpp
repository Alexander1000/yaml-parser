#include <yaml-parser.h>
#include <io-buffer.h>

#define STREAM_MODE_PLAIN 0

namespace YamlParser
{
    Stream::Stream(IOBuffer::CharStream* charStream)
    {
        this->charStream = charStream;
        this->mode = STREAM_MODE_PLAIN;
    }

    Token::Token* Stream::getNextToken()
    {
        char* symbol = this->charStream->getNext();
        if (symbol == NULL) {
            return NULL;
        }

        switch (this->mode) {
            case STREAM_MODE_PLAIN:
                if (isIndent(*symbol)) {
                    // todo: parse indent token
                }
                if (isPropertySymbol(*symbol)) {
                    // todo: parse property
                }
                break;
            default:
                throw new InvalidStreamModeException;
        }

        return NULL;
    }

    bool Stream::isIndent(char symbol)
    {
        return symbol == '\t' || symbol == 0x20;
    }

    bool Stream::isPropertySymbol(char symbol)
    {
        return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
    }
}
