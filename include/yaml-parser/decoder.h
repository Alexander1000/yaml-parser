#ifndef YAML_PARSER_DECODER_H
#define YAML_PARSER_DECODER_H

#include <map>
#include <list>
#include <stack>

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
        std::list<int>* indent;
        std::stack<Token::Token*>* tokenStack;
        std::list<char*>* objectName;
        std::stack<int>* indentArray;

        void pushToken(Token::Token*);
        Token::Token* getNextToken();

        Element* parse_element();
        std::map<std::string, Element*>* parse_object();
        std::map<std::string, Element*>* parse_object(Token::Token* token);
        std::list<Element*>* parse_array();
        std::string* parse_text();
        std::string* assemble_text(std::list<std::string*>* textBlocks, int textIndent);
    };
}

#endif /* YAML_PARSER_DECODER_H */
