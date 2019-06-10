#ifndef YAML_PARSER_TOKEN_H
#define YAML_PARSER_TOKEN_H

#include <io-buffer.h>

namespace YamlParser::Token
{
    enum Type {};

    class Token {
    public:
        Token(int line, int column, IOBuffer::IOReader *reader);
        int getLine();
        int getColumn();
        IOBuffer::IOReader *getReader();
        virtual Type getType() = 0;
    protected:
        int line;
        int column;
        IOBuffer::IOReader *reader;
    };
}

#endif
