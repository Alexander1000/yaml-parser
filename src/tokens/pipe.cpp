#include <yaml-parser/token.h>
#include <io-buffer.h>

namespace YamlParser::Token
{
    PipeToken::PipeToken(int line, int column, IOBuffer::IOReader *reader) : Token(line, column, reader) {
    }

    Type PipeToken::getType() {
        return Type::Pipe;
    }
} // YamlParser::Token
