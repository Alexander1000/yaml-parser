#include <iostream>
#include <map>

#include <yaml-parser/decoder.h>
#include <yaml-parser/stream.h>
#include <yaml-parser/element.h>
#include <yaml-parser/token.h>
#include <yaml-parser/exception.h>

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

    Element* Decoder::parse_element()
    {
        return NULL;
    }

    std::map<std::string, Element*>* Decoder::parse_object()
    {
        std::map<std::string, Element*>* object;
        object = new std::map<std::string, Element*>;

        Token::Token* token = NULL;
        token = this->stream->getNextToken();

        if (token == NULL || token->getType() != Token::Type::Property) {
            throw new UnexpectedTokenException;
        }

        char* propertyName = (char*) malloc(sizeof(char) * 1001);
        memset(propertyName, 1001, sizeof(char));
        token->getReader()->read(propertyName, 1000);

        std::cout << "property.name: " << propertyName << std::endl;

        return object;
    }
}
