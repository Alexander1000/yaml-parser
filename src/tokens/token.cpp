#include <io-buffer.h>
#include <yaml-parser/token.h>

namespace YamlParser::Token {
    Token::Token(int line, int column, IOBuffer::IOReader *reader) {
        this->line = line;
        this->column = column;
        this->reader = reader;
    }

    int Token::getLine() {
        return this->line;
    }

    int Token::getColumn() {
        return this->column;
    }

    IOBuffer::IOReader* Token::getReader() {
        return this->reader;
    }

    const char* tokenTypeName(Type type)
    {
        switch (type) {
            case Type::Property:
                return "property";
            case Type::PlainValue:
                return "plain.value";
            case Type::Dash:
                return "dash";
            case Type::Pipe:
                return "pipe";
            case Type::Space:
                return "space";
        }
    }
}
