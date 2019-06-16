#include <yaml-parser/stream.h>
#include <io-buffer.h>

#include <iostream>

#define STREAM_MODE_PLAIN 0
#define STREAM_MODE_VALUE 1
#define STREAM_MODE_ARRAY_ELEMENT 2
#define STREAM_MODE_TEXT_PLAIN 3

namespace YamlParser
{
    Stream::Stream(IOBuffer::CharStream* charStream)
    {
        this->charStream = charStream;
        this->mode = STREAM_MODE_PLAIN;
        this->prevMode = STREAM_MODE_PLAIN;
        this->curSymbol = NULL;
        this->currentLine = 0;
        this->currentColumn = -1;
        this->lastIndent = 0;
    }

    Token::Token* Stream::getNextToken()
    {
        std::cout << "Call Stream::getNextToken()" << std::endl;

        if (this->curSymbol == NULL) {
            this->curSymbol = this->getNextChar();
        }

        if (this->curSymbol == NULL) {
            return NULL;
        }

        Token::Token* token = NULL;

        switch (this->mode) {
            case STREAM_MODE_PLAIN:
                while (this->curSymbol != NULL && (*this->curSymbol == 0x0A || *this->curSymbol == 0x0D)) {
                    // skip empty strings
                    this->curSymbol = this->getNextChar();
                }

                if (this->curSymbol == NULL) {
                    return NULL;
                }

                if (isIndent(*this->curSymbol)) {
                    return this->parseIndentToken();
                }

                if (*this->curSymbol == '-') {
                    this->moveToMode(STREAM_MODE_ARRAY_ELEMENT);
                    token = new Token::DashToken(this->currentLine, this->currentColumn, NULL);
                    return token;
                }

                return this->parsePropertyToken();

            case STREAM_MODE_VALUE:
                while (this->curSymbol != NULL && isIndent(*this->curSymbol)) {
                    this->curSymbol = this->getNextChar();
                }

                if (this->curSymbol == NULL) {
                    return NULL;
                }

                if (*this->curSymbol == '|') {
                    this->moveToMode(STREAM_MODE_TEXT_PLAIN);
                    token = new Token::PipeToken(this->currentLine, this->currentColumn, NULL);
                    return token;
                }

                if (*this->curSymbol == 0x0A || *this->curSymbol == 0x0D) {
                    this->moveToMode(STREAM_MODE_PLAIN);
                    return this->getNextToken();
                }

                token = this->parsePlainValueToken();
                this->moveToMode(STREAM_MODE_PLAIN);
                return token;

            case STREAM_MODE_ARRAY_ELEMENT:
                while (this->curSymbol != NULL && isIndent(*this->curSymbol)) {
                    this->curSymbol = this->getNextChar();
                }

                if (this->curSymbol == NULL) {
                    return NULL;
                }

                return this->parseArrayElementToken();

            case STREAM_MODE_TEXT_PLAIN:
                while (this->curSymbol != NULL && isIndent(*this->curSymbol)) {
                    // skip empty strings
                    this->curSymbol = this->getNextChar();
                }
                while (this->curSymbol != NULL && (*this->curSymbol == 0x0A || *this->curSymbol == 0x0D)) {
                    // skip empty strings
                    this->curSymbol = this->getNextChar();
                }

                if (isIndent(*this->curSymbol)) {
                    token = this->parseIndentToken();
                    if (((IOBuffer::IOMemoryBuffer*) token)->length() <= this->lastIndent) {
                        this->moveToMode(STREAM_MODE_PLAIN);
                    }
                    return token;
                }

                return this->parsePlainValueToken();
        }

        throw new InvalidStreamModeException;
    }

    Token::Token* Stream::parseIndentToken()
    {
        std::cout << "Call Stream::parseIndentToken()" << std::endl; // todo: remove after debug

        Token::Token *token = NULL;
        IOBuffer::IOMemoryBuffer* ioWriter;
        ioWriter = new IOBuffer::IOMemoryBuffer(10);

        while (isIndent(*this->curSymbol)) {
            ioWriter->write(this->curSymbol, 1);
            this->curSymbol = this->getNextChar();
        }

        token = new Token::Indent(this->currentLine, this->currentColumn, ioWriter);
        return token;
    }

    Token::Token* Stream::parsePropertyToken()
    {
        std::cout << "Call Stream::parsePropertyToken()" << std::endl; // todo: remove after debug

        int tokenLine = this->currentLine;
        int tokenColumn = this->currentColumn;

        this->lastIndent = this->currentColumn;

        Token::Token *token = NULL;

        // io writer for token
        IOBuffer::IOMemoryBuffer* ioWriter;
        ioWriter = new IOBuffer::IOMemoryBuffer(16);

        char* forwardSymbol = this->getNextChar();

        std::cout << "Parse property: "; // todo: remove after debug

        while (*this->curSymbol != ':' && !isIndent(*forwardSymbol) && *forwardSymbol != 0x0D && *forwardSymbol != 0x0A) {
            std::cout << this->curSymbol[0]; // todo: remove after debug
            ioWriter->write(this->curSymbol, 1);
            this->curSymbol = forwardSymbol;
            forwardSymbol = this->getNextChar();
        }

        std::cout << std::endl; // todo: remove after debug

        this->curSymbol = forwardSymbol;

        token = new Token::PropertyToken(tokenLine, tokenColumn, ioWriter);
        this->moveToMode(STREAM_MODE_VALUE);
        return token;
    }

    Token::Token* Stream::parsePlainValueToken()
    {
        std::cout << "Call Stream::parsePlainValueToken()" << std::endl; // todo: remove after debug

        Token::Token* token = NULL;

        // io buffer
        IOBuffer::IOMemoryBuffer* ioMemoryBuffer;
        ioMemoryBuffer = new IOBuffer::IOMemoryBuffer(16);

        std::cout << "Parse value: "; // todo: remove after debug

        while (this->curSymbol != NULL && *this->curSymbol != 0x0A && *this->curSymbol != 0x0D) {
            std::cout << this->curSymbol[0]; // todo: remove after debug
            ioMemoryBuffer->write(this->curSymbol, 1);
            this->curSymbol = this->getNextChar();
        }

        std::cout << std::endl; // todo: remove after debug

        token = new Token::PlainValueToken(this->currentLine, this->currentColumn, ioMemoryBuffer);
        return token;
    }

    Token::Token* Stream::parseArrayElementToken()
    {
        std::cout << "Call Stream::parseArrayElementToken()" << std::endl; // todo: remove after debug

        Token::Token* token = NULL;

        // io buffer
        IOBuffer::IOMemoryBuffer* ioMemoryBuffer;
        ioMemoryBuffer = new IOBuffer::IOMemoryBuffer(16);

        std::cout << "Parse value: "; // todo: remove after debug

        bool colon = false;

        while (this->curSymbol != NULL && *this->curSymbol != 0x0A && *this->curSymbol != 0x0D) {
            if (*this->curSymbol == ':') {
                if (colon) {
                    ioMemoryBuffer->write(this->curSymbol-1, 1);
                }
                colon = true;
                continue;
            }

            if (colon && !isIndent(*this->curSymbol)) {
                ioMemoryBuffer->write(this->curSymbol-1, 1);
                colon = false;
            }

            if (isIndent(*this->curSymbol) && colon) {
                // sequence: colon(:) + space(0x20) go away
                break;
            }

            ioMemoryBuffer->write(this->curSymbol, 1);
            this->curSymbol = this->getNextChar();
        }

        if (colon) {
            token = new Token::PropertyToken(this->currentLine, this->currentColumn, ioMemoryBuffer);
            this->moveToMode(STREAM_MODE_VALUE);
        } else {
            token = new Token::PlainValueToken(this->currentLine, this->currentColumn, ioMemoryBuffer);
            this->moveToMode(STREAM_MODE_PLAIN);
        }

        return token;
    }

    bool Stream::isIndent(char symbol)
    {
        return symbol == '\t' || symbol == 0x20;
    }

    char* Stream::getNextChar()
    {
        char* nextChar = this->charStream->getNext();
        if (nextChar != NULL) {
            if (*nextChar == 0x0A || *nextChar == 0x0D) {
                this->currentLine++;
                this->currentColumn = 0;
            } else {
                this->currentColumn++;
            }
        }
        return nextChar;
    }

    void Stream::moveToMode(int nextMode)
    {
        this->prevMode = this->mode;
        this->mode = nextMode;
    }
}
