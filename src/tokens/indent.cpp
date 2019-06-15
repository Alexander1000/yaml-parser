#include <yaml-parser/token.h>
#include <io-buffer.h>

namespace YamlParser::Token
{
    Indent::Indent(int line, int column, IOBuffer::IOReader *reader) : Token(line, column, reader) {
    }

    Type Indent::getType() {
        return Type::Indent;
    }
} // YamlParser::Token
