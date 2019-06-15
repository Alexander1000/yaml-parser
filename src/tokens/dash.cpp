#include <yaml-parser/token.h>
#include <io-buffer.h>

namespace YamlParser::Token
{
    DashToken::DashToken(int line, int column, IOBuffer::IOReader *reader) : Token(line, column, reader) {
    }

    Type DashToken::getType() {
        return Type::Dash;
    }
} // YamlParser::Token
