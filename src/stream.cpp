#include <yaml-parser.h>
#include <io-buffer.h>

#define STREAM_MODE_PLAIN 0

namespace YamlParser
{
    Stream::Stream(IOBuffer::CharStream* charStream)
    {
        this->charStream = charStream;
        this->mode = STREAM_MODE_PLAIN;
        this->curSymbol = NULL;
    }

    Token::Token* Stream::getNextToken()
    {
        if (this->curSymbol == NULL) {
            this->curSymbol = this->charStream->getNext();
        }

        if (this->curSymbol == NULL) {
            return NULL;
        }

        switch (this->mode) {
            case STREAM_MODE_PLAIN:
                if (this->isIndent()) {
                    return this->parseIndentToken();
                }
                if (this->isPropertySymbol()) {
                    // todo: parse property
                }
                break;
            default:
                throw new InvalidStreamModeException;
        }

        return NULL;
    }

    Token::Token* Stream::parseIndentToken()
    {
    }

    bool Stream::isIndent()
    {
        return *this->curSymbol == '\t' || *this->curSymbol == 0x20;
    }

    bool Stream::isPropertySymbol()
    {
        return (*this->curSymbol >= 'a' && *this->curSymbol <= 'z')
            || (*this->curSymbol >= 'A' && *this->curSymbol <= 'Z');
    }
}
