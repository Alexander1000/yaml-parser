#ifndef YAML_PARSER_H
#define YAML_PARSER_H

#include <io-buffer.h>

namespace YamlParser
{
    class Stream {
    public:
        Stream(IOBuffer::CharStream* charStream);
        void* getNextToken();

    private:
        IOBuffer::CharStream* charStream;
    };
} // YamlParser

#endif /* YAML_PARSER_H */
