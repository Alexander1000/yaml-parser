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
        Token::Token* token = NULL;
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
                object = this->parse_object(token);
                element = new Element(ElementType::ObjectType, object);
                break;
            case Token::Type::PlainValue:

                plainValue = (char*) malloc(1001 * sizeof(char));
                memset(plainValue, 0, sizeof(char) * 1001);
                token->getReader()->read(plainValue, 1000);
                strValue = new std::string(plainValue);
                element = new Element(ElementType::PlainTextType, strValue);
                break;
            case Token::Type::Space:
                memoryBuffer = (IOBuffer::IOMemoryBuffer*) token->getReader();

                if (this->indent->back() == memoryBuffer->length()) {
                    element = this->parse_element();
                } else {
                    this->indent->push_back(memoryBuffer->length());
                    element = this->parse_element();
                    this->indent->pop_back();
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
            case Token::Type::Pipe:
                strValue = this->parse_text();
                element = new Element(ElementType::PlainTextType, strValue);
                break;
            default:
                throw new UnexpectedTokenException;
        }

        return element;
    }

    std::map<std::string, Element*>* Decoder::parse_object()
    {
        return this->parse_object(this->getNextToken());
    }

    std::map<std::string, Element*>* Decoder::parse_object(Token::Token* token)
    {
        std::map<std::string, Element*>* object;
        object = new std::map<std::string, Element*>;
        char* propertyName;

        PARSE_PAIR:
        if (token == NULL || token->getType() != Token::Type::Property) {
            throw new UnexpectedTokenException;
        }

        propertyName = (char*) malloc(sizeof(char) * 1001);
        memset(propertyName, 0, 1001 * sizeof(char));
        token->getReader()->read(propertyName, 1000);

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
                    memoryBuffer = (IOBuffer::IOMemoryBuffer*) token->getReader();

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

        return object;
    }

    std::list<Element*>* Decoder::parse_array()
    {
        std::list<Element*>* elementList = NULL;
        elementList = new std::list<Element*>;
        Token::Token* token = NULL;

        PARSE_ELEMENT:
        bool isNested = false;
        // hack for nested objects with custom indent
        token = this->getNextToken();
        if (token->getType() == Token::Type::Property) {
            this->indent->push_back(token->getColumn());
            isNested = true;
        }
        this->tokenStack->push(token);
        elementList->push_back(this->parse_element());

        if (isNested) {
            this->indent->pop_back();
        }

        token = this->getNextToken();
        if (token != NULL) {
            if (token->getType() == Token::Type::Dash) {
                if (this->indent->back() == 0) {
                    goto PARSE_ELEMENT;
                }
            }

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

        return elementList;
    }

    std::string* Decoder::parse_text()
    {
        Token::Token* token = NULL;
        IOBuffer::IOMemoryBuffer* memoryBuffer;
        bool firstBlock = true;
        std::list<std::string*>* textBlocks;
        textBlocks = new std::list<std::string*>;
        char* buffer;

        PARSE_TEXT_BLOCK:
        token = this->getNextToken();
        if (token == NULL) {
            return NULL;
        }
        if (token->getType() != Token::Type::Space) {
            throw new UnexpectedTokenException;
        }
        memoryBuffer = (IOBuffer::IOMemoryBuffer*) token->getReader();
        if (firstBlock) {
            if (memoryBuffer->length() <= this->indent->back()) {
                throw new UnexpectedTokenException;
            }
            firstBlock = false;
        } else {
            if (memoryBuffer->length() <= this->indent->back()) {
                this->tokenStack->push(token);
                return this->assemble_text(textBlocks);
            }
        }
        token = this->getNextToken();
        if (token == NULL) {
            return this->assemble_text(textBlocks);
        }

        if (token->getType() != Token::Type::PlainValue) {
            throw new UnexpectedTokenException;
        }

        buffer = (char*) malloc(sizeof(char) * 1001);
        memset(buffer, 0, sizeof(char) * 1001);
        token->getReader()->read(buffer, 1000);

        std::string newStr(buffer);
        textBlocks->emplace_back(&newStr);

        token = this->getNextToken();
        if (token == NULL) {
            return this->assemble_text(textBlocks);
        }

        if (token->getType() == Token::Type::Space) {
            this->tokenStack->push(token);
            goto PARSE_TEXT_BLOCK;
        }

        throw new UnexpectedTokenException;
    }

    std::string* Decoder::assemble_text(std::list<std::string*>* textBlocks)
    {
        if (textBlocks == NULL) {
            return NULL;
        }

        std::string* text;
        text = new std::string;
        int size = textBlocks->size();

        int textSize = 0;

        std::list<std::string*>::iterator iTextBlocks = textBlocks->begin();
        while (iTextBlocks != textBlocks->end()) {
            textSize += (*iTextBlocks)->length();
            iTextBlocks++;
        }

        char* textBuffer = (char*) malloc(sizeof(char) * textSize);
        memset(textBuffer, 0, sizeof(char) * textSize);

        std::cout << "Text size: " << textSize << std::endl;
        std::cout << "Text block size: " << textBlocks->size() << std::endl;

        int offset = 0;

        for (int i = 0; i < size; i++) {
            std::string* textBlock = textBlocks->front();
            textBlocks->pop_front();

            memcpy(textBuffer + offset, textBlock->c_str(), textBlock->length());
            offset += textBlock->length();
        }

        std::cout << "Str: " << textBuffer << std::endl;

        return text;
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
