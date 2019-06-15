#include <yaml-parser.h>
#include <io-buffer.h>

#define STREAM_MODE_PLAIN 0
#define STREAM_MODE_VALUE 1

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
                if (isIndent(*this->curSymbol)) {
                    return this->parseIndentToken();
                }
                if (this->isPropertySymbol()) {
                    return this->parsePropertyToken();
                }
                break;
            case STREAM_MODE_VALUE:
            default:
                throw new InvalidStreamModeException;
        }

        return NULL;
    }

    Token::Token* Stream::parseIndentToken()
    {
        Token::Token *token = NULL;
        IOBuffer::IOMemoryBuffer ioWriter(10);

        while (isIndent(*this->curSymbol)) {
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
        char* forwardSymbol = this->charStream->getNext();

        while (*this->curSymbol != ':' && !isIndent(*forwardSymbol)) {
            ioWriter.write(this->curSymbol, 1);
            this->curSymbol = forwardSymbol;
        }

        token = new Token::PropertyToken(this->currentLine, this->currentColumn, &ioWriter);
        this->mode = STREAM_MODE_VALUE;
        return token;
    }

    bool Stream::isIndent(char symbol)
    {
        return symbol == '\t' || symbol == 0x20;
    }

    bool Stream::isPropertySymbol()
    {
        return (*this->curSymbol >= 'a' && *this->curSymbol <= 'z')
            || (*this->curSymbol >= 'A' && *this->curSymbol <= 'Z');
    }
}
