#include <yaml-parser/token.h>
#include <io-buffer.h>

namespace YamlParser::Token
{
    PlainValueToken::PlainValueToken(int line, int column, IOBuffer::IOReader *reader) : Token(line, column, reader) {
    }

    Type PlainValueToken::getType() {
        return Type::PlainValue;
    }
} // YamlParser::Token
