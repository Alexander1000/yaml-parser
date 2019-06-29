#include <iostream>
#include <map>
#include <string>
#include <list>
#include <stack>

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
        this->indent = new std::list<int>;
        this->indent->push_back(0);
        this->tokenStack = new std::stack<Token::Token*>;
        this->objectName = new std::list<char*>;
        this->indentArray = new std::stack<int>;
    }

    Element* Decoder::parse()
    {
        return this->parse_element();
    }

    Element* Decoder::parse_element()
    {
        std::cout << "Call Decoder::parse_element()" << std::endl;

        Token::Token* token = NULL;
        Token::Token* forwardToken = NULL;
        token = this->getNextToken();

        if (token == NULL) {
            return NULL;
        }

        Element* element;
        std::map<std::string, Element*>* object;
        char* plainValue;
        IOBuffer::IOMemoryBuffer* memoryBuffer;
        std::list<Element*>* elementList;
        std::string* strValue = NULL;

        switch (token->getType()) {
            case Token::Type::Property:
                std::cout << "Call Decoder::parse_element() [property]" << std::endl;

                object = this->parse_object(token);
                element = new Element(ElementType::ObjectType, object);
                break;
            case Token::Type::PlainValue:
                std::cout << "Call Decoder::parse_element() [plain.value]" << std::endl;

                plainValue = (char*) malloc(1001 * sizeof(char));
                memset(plainValue, 0, sizeof(char) * 1001);
                token->getReader()->read(plainValue, 1000);
                strValue = new std::string(plainValue);
                element = new Element(ElementType::PlainTextType, strValue);
                break;
            case Token::Type::Space:
                std::cout << "Call Decoder::parse_element() [space]" << std::endl;
                memoryBuffer = (IOBuffer::IOMemoryBuffer*) token->getReader();

                if (this->indent->back() == memoryBuffer->length()) {
                    element = this->parse_element();
                } else {
                    this->indent->push_back(memoryBuffer->length());
                    element = this->parse_element();
                    // this->indent->pop_back();
                }

                break;
            case Token::Type::Dash:
                this->indentArray->push(token->getColumn() - 1);
                elementList = this->parse_array();
                element = new Element(ElementType::ListType, elementList);
                if (this->indentArray->top() != this->indent->back()) {
                    this->indent->pop_back();
                }
                this->indentArray->pop();
                break;
            default:
                std::cout << "Unexpected token type: " << Token::tokenTypeName(token->getType()) << std::endl;
                throw new UnexpectedTokenException;
        }

        std::cout << "Return Decoder::parse_element() [" << Token::tokenTypeName(token->getType()) << "]" << std::endl;
        return element;
    }

    std::map<std::string, Element*>* Decoder::parse_object()
    {
        return this->parse_object(this->getNextToken());
    }

    std::map<std::string, Element*>* Decoder::parse_object(Token::Token* token)
    {
        std::cout << "Decoder::parse_object()" << std::endl;
        std::cout << "Current Indent: " << this->indent->back() << std::endl;

        std::map<std::string, Element*>* object;
        object = new std::map<std::string, Element*>;
        char* propertyName;

        PARSE_PAIR:
        if (token == NULL || token->getType() != Token::Type::Property) {
            std::cout << "Unexpected token type: " << Token::tokenTypeName(token->getType()) << std::endl;
            throw new UnexpectedTokenException;
        }

        propertyName = (char*) malloc(sizeof(char) * 1001);
        memset(propertyName, 0, 1001 * sizeof(char));
        token->getReader()->read(propertyName, 1000);

        std::cout << "property.name: " << propertyName << std::endl;

        Element* element = this->parse_element();

        (*object)[std::string(propertyName)] = element;

        IOBuffer::IOMemoryBuffer* memoryBuffer;

        token = this->getNextToken();
        if (token != NULL) {
            switch (token->getType()) {
                case Token::Type::Property:
                    // parse next key-value pair
                    if (this->indent->back() == 0) {
                        goto PARSE_PAIR;
                    }
                    this->tokenStack->push(token);
                    break;
                case Token::Type::Space:
                    // std::cout << "indent in property" << std::endl;
                    memoryBuffer = (IOBuffer::IOMemoryBuffer*) token->getReader();
                    // std::cout << "length indent: " << memoryBuffer->length() << std::endl;
                    // std::cout << "parent length: " << this->indent->back() << std::endl;

                    if (memoryBuffer->length() == this->indent->back()) {
                        Token::Token* forwardToken = this->getNextToken();
                        if (forwardToken->getType() != Token::Type::Property) {
                            this->tokenStack->push(forwardToken);
                            this->tokenStack->push(token);
                            break;
                        }

                        token = forwardToken;
                        goto PARSE_PAIR;
                    }

                    this->tokenStack->push(token);

                    break;
                default:
                    this->tokenStack->push(token);
                    break;
            }
        }

        this->indent->pop_back();
        return object;
    }

    std::list<Element*>* Decoder::parse_array()
    {
        std::cout << "Decoder::parse_array()" << std::endl;
        std::cout << "Current Indent: " << this->indent->back() << std::endl;

        std::list<Element*>* elementList = NULL;
        elementList = new std::list<Element*>;
        Token::Token* token = NULL;

        PARSE_ELEMENT:
        // hack for nested objects with custom indent
        token = this->getNextToken();
        if (token->getType() == Token::Type::Property) {
            std::cout << "Hack shift indent: " << token->getColumn() << std::endl;
            this->indent->push_back(token->getColumn());
        }
        this->tokenStack->push(token);
        elementList->push_back(this->parse_element());

        token = this->getNextToken();
        if (token != NULL) {
            if (token->getType() == Token::Type::Space) {
                IOBuffer::IOMemoryBuffer* memoryBuffer = (IOBuffer::IOMemoryBuffer*) token->getReader();
                if (memoryBuffer->length() == this->indent->back()) {
                    Token::Token* dashToken = this->getNextToken();
                    if (dashToken->getType() == Token::Type::Dash) {
                        goto PARSE_ELEMENT;
                    }
                    this->tokenStack->push(dashToken);
                }
            }

            this->tokenStack->push(token);
        }

        std::cout << "Size list: " << elementList->size() << std::endl;

        return elementList;
    }

    Token::Token* Decoder::getNextToken()
    {
        Token::Token* token;

        if (this->tokenStack->size() > 0) {
            token = this->tokenStack->top();
            this->tokenStack->pop();
        } else {
            token = this->stream->getNextToken();
        }

        return token;
    }
}
