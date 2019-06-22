#include <iostream>
#include <map>
#include <string>

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
        return this->parse_element();
    }

    Element* Decoder::parse_element()
    {
        Token::Token* token = NULL;
        token = this->stream->getNextToken();

        if (token == NULL) {
            return NULL;
        }

        Element* element;
        std::map<std::string, Element*>* object;
        char* plainValue;

        switch (token->getType()) {
            case Token::Type::Property:
                object = this->parse_object(token);
                element = new Element(ElementType::ObjectType, object);
                break;
            case Token::Type::PlainValue:
                plainValue = (char*) malloc(1001 * sizeof(char));
                memset(plainValue, 0, sizeof(char) * 1001);
                token->getReader()->read(plainValue, 1000);
                element = new Element(ElementType::PlainTextType, plainValue);
                break;
            default:
                std::cout << "Unexpected token type: " << Token::tokenTypeName(token->getType()) << std::endl;
                throw new UnexpectedTokenException;
        }

        return element;
    }

    std::map<std::string, Element*>* Decoder::parse_object()
    {
        return this->parse_object(this->stream->getNextToken());
    }

    std::map<std::string, Element*>* Decoder::parse_object(Token::Token* token)
    {
        if (token == NULL || token->getType() != Token::Type::Property) {
            throw new UnexpectedTokenException;
        }

        std::map<std::string, Element*>* object;
        object = new std::map<std::string, Element*>;

        char* propertyName = (char*) malloc(sizeof(char) * 1001);
        memset(propertyName, 0, 1001 * sizeof(char));
        token->getReader()->read(propertyName, 1000);

        std::cout << "property.name: " << propertyName << std::endl;

        Element* element = this->parse_element();

        (*object)[std::string(propertyName)] = element;

        return object;
    }
}
