#include <map>

#include <yaml-parser/decoder.h>
#include <yaml-parser/stream.h>
#include <yaml-parser/element.h>
#include <yaml-parser/token.h>

namespace YamlParser
{
    Decoder::Decoder(Stream* stream)
    {
        this->stream = stream;
    }

    Element* Decoder::parse()
    {
        return NULL;
    }

    std::map<std::string, Element*>* Decoder::parse_object()
    {
        std::map<std::string, Element*>* object;
        object = new std::map<std::string, Element*>;

        Token::Token* token = NULL;
        token = this->stream->getNextToken();

        return object;
    }
}
