#ifndef YAML_PARSER_H
#define YAML_PARSER_H

#include <io-buffer.h>

namespace YamlParser
{
    class Stream {
    public:
        Stream(IOBuffer::IOReader *reader);
        void* getNextToken();

    private:
        IOBuffer::IOReader* reader;
    };
} // YamlParser

#endif /* YAML_PARSER_H */
