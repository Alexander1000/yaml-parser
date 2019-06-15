#ifndef YAML_PARSER_TOKEN_H
#define YAML_PARSER_TOKEN_H

#include <io-buffer.h>

namespace YamlParser::Token
{
    enum Type {
        Space,
        Property,
        PlainValue
    };

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

    class Indent : public Token {
    public:
        Indent(int line, int column, IOBuffer::IOReader *reader);
        Type getType();
    };

    class PropertyToken : public Token {
    public:
        PropertyToken(int line, int column, IOBuffer::IOReader *reader);
        Type getType();
    };

    class PlainValueToken : public Token {
    public:
        PlainValueToken(int line, int column, IOBuffer::IOReader* reader);
        Type getType();
    };
}

#endif
