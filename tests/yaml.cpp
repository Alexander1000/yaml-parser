#include <iostream>
#include <memory.h>

#include <io-buffer.h>
#include <yaml-parser.h>

int main(int argc, char** argv)
{
    IOBuffer::IOFileReader fileReader("../fixtures/001-sample-data.yaml");
    IOBuffer::CharStream charStream(&fileReader);
    YamlParser::Stream yamlStream(&charStream);
    YamlParser::Token::Token* token = NULL;

    char* buffer = (char*) malloc(sizeof(char) * 1001);

    do {
        token = yamlStream.getNextToken();
        if (token != NULL) {
            IOBuffer::IOReader* reader = token->getReader();
            memset(buffer, 0, sizeof(char) * 1001);
            reader->read(buffer, 1000);

            std::cout << "Type: " << token->getType() << std::endl;
            std::cout << "Token: " << buffer << std::endl;
        }
    } while(token != NULL);

    return 0;
}
