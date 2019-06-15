#include <yaml-parser/token.h>
#include <io-buffer.h>

namespace YamlParser::Token
{
    PropertyToken::PropertyToken(int line, int column, IOBuffer::IOReader *reader) : Token(line, column, reader) {
    }

    Type Indent::getType() {
        return Type::Property;
    }
} // YamlParser::Token
