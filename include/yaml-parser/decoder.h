#ifndef YAML_PARSER_DECODER_H
#define YAML_PARSER_DECODER_H

#include <yaml-parser/stream.h>

namespace YamlParser
{
    class Decoder
    {
    public:
        Decoder(Stream* stream);
        void* parse();
    private:
        Stream* stream;
    };
}

#endif /* YAML_PARSER_DECODER_H */
