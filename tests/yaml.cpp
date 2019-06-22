#include <iostream>
#include <memory.h>

#include <io-buffer.h>
#include <yaml-parser.h>
#include <cpp-unit-test.h>

class AssertElementTypeException
{};

void assertElementType(CppUnitTest::TestCase* t, YamlParser::Element* element, YamlParser::ElementType type)
{
    t->increment();

    if (element->getType() != type) {
        std::cout << "Expected element" << YamlParser::getElementTypeName(type) << ", but: " << YamlParser::getElementTypeName(element->getType()) << std::endl;
        throw new AssertElementTypeException;
    }
}

CppUnitTest::TestCase* testDecodeObject_YamlData_Positive()
{
    CppUnitTest::TestCase* t = new CppUnitTest::TestCase("001-samle-data");
    t->printTitle();

    IOBuffer::IOFileReader fileReader("../fixtures/001-sample-data.yaml");
    IOBuffer::CharStream charStream(&fileReader);
    YamlParser::Stream yamlStream(&charStream);
    YamlParser::Decoder decoder(&yamlStream);

    YamlParser::Element* rElement = decoder.parse();
    assertElementType(t, rElement, YamlParser::ElementType::ObjectType);

    // todo: make tests

    t->finish();
    return t;
}

int main(int argc, char** argv)
{
    CppUnitTest::TestSuite testSuite;

    testSuite.addTestCase(testDecodeObject_YamlData_Positive());

    testSuite.printTotal();

    std::cout << "==========================" << std::endl;

    IOBuffer::IOFileReader fileReader("../fixtures/001-sample-data.yaml");
    IOBuffer::CharStream charStream(&fileReader);
    YamlParser::Stream yamlStream(&charStream);
    YamlParser::Token::Token* token = NULL;

    char* buffer = (char*) malloc(sizeof(char) * 1001);

    do {
        token = yamlStream.getNextToken();
        if (token != NULL) {
            std::cout << "Type: " << token->getType() << std::endl;

            IOBuffer::IOReader* reader = token->getReader();

            if (reader != NULL) {
                memset(buffer, 1001, sizeof(char));
                reader->read(buffer, 1000);
                std::cout << "Token: " << buffer << std::endl;
            }
        }
    } while(token != NULL);

    return 0;
}
