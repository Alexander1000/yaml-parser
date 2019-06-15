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
        this->currentLine = 0;
        this->currentColumn = 0;
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
                    return this->parsePropertyToken();
                }
                break;
            default:
                throw new InvalidStreamModeException;
        }

        return NULL;
    }

    Token::Token* Stream::parseIndentToken()
    {
        Token::Token *token = NULL;
        IOBuffer::IOMemoryBuffer ioWriter(10);

        while (this->isIndent()) {
            ioWriter.write(this->curSymbol, 1);
            this->curSymbol = this->charStream->getNext();
        }

        token = new Token::Indent(this->currentLine, this->currentColumn, &ioWriter);
        return token;
    }

    Token::Token* Stream::parsePropertyToken()
    {
        Token::Token *token = NULL;
        IOBuffer::IOMemoryBuffer ioWriter(10);
        while (!this->isIndent()) {
            ioWriter.write(this->curSymbol, 1);
            this->curSymbol = this->charStream->getNext();
        }

        token = new Token::PropertyToken(this->currentLine, this->currentColumn, &ioWriter);
        return token;
    }

    bool Stream::isIndent()
    {
        return this->curSymbol != NULL && (*this->curSymbol == '\t' || *this->curSymbol == 0x20);
    }

    bool Stream::isPropertySymbol()
    {
        return (*this->curSymbol >= 'a' && *this->curSymbol <= 'z')
            || (*this->curSymbol >= 'A' && *this->curSymbol <= 'Z');
    }
}
