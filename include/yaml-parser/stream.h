#ifndef YAML_PARSER_STREAM_H
#define YAML_PARSER_STREAM_H

#include <io-buffer.h>
#include <yaml-parser/token.h>

namespace YamlParser
{
    class InvalidStreamModeException
    {};

    class Stream {
    public:
        Stream(IOBuffer::CharStream* charStream);
        Token::Token* getNextToken();
    private:
        IOBuffer::CharStream* charStream;
        int mode;
        int prevMode;
        char* curSymbol;
        int lastIndent;

        // координаты токена в документе
        int currentLine;
        int currentColumn;

        // methods

        static bool isIndent(char symbol);

        Token::Token* parseIndentToken();
        Token::Token* parsePropertyToken();
        Token::Token* parsePlainValueToken();
        Token::Token* parseArrayElementToken();

        char* getNextChar();

        void moveToMode(int mode);
    };
} // YamlParser

#endif /* YAML_PARSER_STREAM_H */
