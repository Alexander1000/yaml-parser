#ifndef YAML_PARSER_DECODER_H
#define YAML_PARSER_DECODER_H

#include <map>

#include <yaml-parser/stream.h>
#include <yaml-parser/element.h>

namespace YamlParser
{
    class Decoder
    {
    public:
        Decoder(Stream* stream);
        Element* parse();
    private:
        Stream* stream;

        Element* parse_element();
        std::map<std::string, Element*>* parse_object();
        std::map<std::string, Element*>* parse_object(Token::Token* token);
    };
}

#endif /* YAML_PARSER_DECODER_H */
