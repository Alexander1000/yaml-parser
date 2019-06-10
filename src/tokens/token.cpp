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
}
