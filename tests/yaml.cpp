#include <iostream>
#include <memory.h>
#include <map>
#include <list>

#include <io-buffer.h>
#include <yaml-parser.h>
#include <cpp-unit-test.h>

class AssertElementTypeException
{};

class AssertObjectPropertyExist
{};

typedef std::map<std::string, YamlParser::Element*> YamlObject;
typedef std::list<YamlParser::Element*> YamlArray;

void assertElementType(CppUnitTest::TestCase* t, YamlParser::Element* element, YamlParser::ElementType type)
{
    t->increment();

    if (element->getType() != type) {
        std::cout << "Expected element" << YamlParser::getElementTypeName(type) << ", but: " << YamlParser::getElementTypeName(element->getType()) << std::endl;
        throw new AssertElementTypeException;
    }
}

void assertObjectPropertyExist(CppUnitTest::TestCase* t, YamlObject* obj, const char* propertyName)
{
    t->increment();

    if (obj->find(propertyName) == obj->end()) {
        std::cout << "Expected property \"" << propertyName << "\" must be exist" << std::endl;
        throw new AssertObjectPropertyExist;
    }
}

CppUnitTest::TestCase* testDecodeObject_YamlData_Positive()
{
    CppUnitTest::TestCase* t = new CppUnitTest::TestCase("001-sample-data");
    t->printTitle();

    IOBuffer::IOFileReader fileReader("../fixtures/001-sample-data.yaml");
    IOBuffer::CharStream charStream(&fileReader);
    YamlParser::Stream yamlStream(&charStream);
    YamlParser::Decoder decoder(&yamlStream);

    YamlParser::Element* rElement = decoder.parse();
    assertElementType(t, rElement, YamlParser::ElementType::ObjectType);
    YamlObject* rObj = (YamlObject*) rElement->getData();

    assertObjectPropertyExist(t, rObj, "simple");

    YamlParser::Element* elRootSimple = rObj->at("simple");
    assertElementType(t, elRootSimple, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elRootSimple->getData(), "alexander");

    assertObjectPropertyExist(t, rObj, "userData");

    YamlParser::Element* elUserData = rObj->at("userData");
    assertElementType(t, elUserData, YamlParser::ElementType::ObjectType);
    YamlObject* oUserData = (YamlObject*) elUserData->getData();

    assertObjectPropertyExist(t, oUserData, "name");
    assertObjectPropertyExist(t, oUserData, "birth.date");
    assertObjectPropertyExist(t, oUserData, "some/info");
    assertObjectPropertyExist(t, oUserData, "key:value");
    assertObjectPropertyExist(t, oUserData, ":test");
    assertObjectPropertyExist(t, oUserData, "value with space");
    assertObjectPropertyExist(t, oUserData, "spaced value:with comma");
    assertObjectPropertyExist(t, oUserData, "nested");
    assertObjectPropertyExist(t, oUserData, "holla");
    assertObjectPropertyExist(t, oUserData, "it is array");
    assertObjectPropertyExist(t, oUserData, "someProperty");
    assertObjectPropertyExist(t, oUserData, "test");
    assertObjectPropertyExist(t, oUserData, "lklk");

    assertObjectPropertyExist(t, rObj, "someTest");

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
