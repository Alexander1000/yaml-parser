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

void assertObjectPropertyValue(CppUnitTest::TestCase* t, YamlObject* obj, const char* propertyName, const char* propertyValue)
{
    t->increment();
    assertObjectPropertyExist(t, obj, propertyName);
    YamlParser::Element* element = obj->at(propertyName);
    assertElementType(t, element, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) element->getData(), propertyValue);

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

    // todo: assert each property

    assertObjectPropertyValue(t, oUserData, "name", "Alexander");
    assertObjectPropertyValue(t, oUserData, "birth.date", "28.08.1990");
    assertObjectPropertyValue(t, oUserData, "some/info", "'Test Information'");
    assertObjectPropertyValue(t, oUserData, "key:value", "value2");
    assertObjectPropertyValue(t, oUserData, ":test", "test");
    assertObjectPropertyValue(t, oUserData, "value with space", "some test");
    assertObjectPropertyValue(t, oUserData, "spaced value:with comma", "magic");

    assertObjectPropertyExist(t, oUserData, "nested");
    YamlParser::Element* elUserDataNested = oUserData->at("nested");
    assertElementType(t, elUserDataNested, YamlParser::ElementType::ObjectType);
    YamlObject* oUserDataNested = (YamlObject*) elUserDataNested->getData();

    assertObjectPropertyValue(t, oUserDataNested, "someObject", "some value");

    assertObjectPropertyExist(t, oUserDataNested, "with eat nested");
    YamlParser::Element* elUserDataNestedDouble = oUserDataNested->at("with eat nested");
    assertElementType(t, elUserDataNestedDouble, YamlParser::ElementType::ObjectType);
    YamlObject* oUserDataNestedDouble = (YamlObject*) elUserDataNestedDouble->getData();
    assertObjectPropertyValue(t, oUserDataNestedDouble, "update", "1");
    assertObjectPropertyValue(t, oUserDataNestedDouble, "test", "2");

    assertObjectPropertyValue(t, oUserDataNested, "previous", "faaf");

    assertObjectPropertyValue(t, oUserData, "holla", "test");

    assertObjectPropertyExist(t, oUserData, "it is array");
    YamlParser::Element* elUserDataItIsArray = oUserData->at("it is array");
    assertElementType(t, elUserDataItIsArray, YamlParser::ElementType::ListType);
    YamlArray* aUserDataArray = (YamlArray*) elUserDataItIsArray->getData();

    YamlArray::iterator itUserDataArray = aUserDataArray->begin();

    YamlParser::Element* elUserDataArray01 = *itUserDataArray;
    assertElementType(t, elUserDataArray01, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elUserDataArray01->getData(), "test1");

    itUserDataArray++;

    YamlParser::Element* elUserDataArray02 = *itUserDataArray;
    assertElementType(t, elUserDataArray02, YamlParser::ElementType::ObjectType);
    YamlObject* oUserDataArray02 = (YamlObject*) elUserDataArray02->getData();
    assertObjectPropertyValue(t, oUserDataArray02, "test 2", "999");

    itUserDataArray++;

    YamlParser::Element* elUserDataArray03 = *itUserDataArray;
    assertElementType(t, elUserDataArray03, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elUserDataArray03->getData(), "test 3");

    itUserDataArray++;

    YamlParser::Element* elUserDataArray04 = *itUserDataArray;
    assertElementType(t, elUserDataArray04, YamlParser::ElementType::ObjectType);
    YamlObject* oUserDataObject04 = (YamlObject*) elUserDataArray04->getData();
    assertObjectPropertyValue(t, oUserDataObject04, "name", "ob1");
    assertObjectPropertyValue(t, oUserDataObject04, "property", "holla");

    // todo: assert: someValue nested

    assertObjectPropertyExist(t, oUserDataObject04, "someValue");
    YamlParser::Element* elUserDataSomeValue = oUserDataObject04->at("someValue");
    assertElementType(t, elUserDataSomeValue, YamlParser::ElementType::ObjectType);
    YamlObject* oUserDataSomeValue = (YamlObject*) elUserDataSomeValue->getData();

    assertObjectPropertyValue(t, oUserDataSomeValue, "hopp", "chik");
    assertObjectPropertyExist(t, oUserDataSomeValue, "nested");
    YamlParser::Element* elUserDataSomeValueNested = oUserDataSomeValue->at("nested");
    assertElementType(t, elUserDataSomeValueNested, YamlParser::ElementType::ListType);

    YamlArray* aUserDataSomeValueNested = (YamlArray*) elUserDataSomeValueNested->getData();
    CppUnitTest::assertEquals(t, aUserDataSomeValueNested->size(), 2);

    YamlArray::iterator itUserDataSomeValueNested = aUserDataSomeValueNested->begin();

    YamlParser::Element* elUserDataSomeValueNested01 = *itUserDataSomeValueNested;
    assertElementType(t, elUserDataSomeValueNested01, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elUserDataSomeValueNested01->getData(), "arr");

    itUserDataSomeValueNested++;

    YamlParser::Element* elUserDataSomeValueNested02 = *itUserDataSomeValueNested;
    assertElementType(t, elUserDataSomeValueNested02, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elUserDataSomeValueNested02->getData(), "342");

    assertObjectPropertyExist(t, oUserDataSomeValue, "eat arr");
    YamlParser::Element* elUserDataSomeValueEatArr = oUserDataSomeValue->at("eat arr");
    assertElementType(t, elUserDataSomeValueEatArr, YamlParser::ElementType::ListType);

    YamlArray* aUserDataSomeValueEatArr = (YamlArray*) elUserDataSomeValueEatArr->getData();
    CppUnitTest::assertEquals(t, aUserDataSomeValueEatArr->size(), 2);

    YamlArray::iterator itUserDataSomeValueEatArr = aUserDataSomeValueEatArr->begin();

    YamlParser::Element* elUserDataSomeValueEatArr01 = *itUserDataSomeValueEatArr;
    assertElementType(t, elUserDataSomeValueEatArr01, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elUserDataSomeValueEatArr01->getData(), "property");

    itUserDataSomeValueEatArr++;

    YamlParser::Element* elUserDataSomeValueEatArr02 = *itUserDataSomeValueEatArr;
    assertElementType(t, elUserDataSomeValueEatArr02, YamlParser::ElementType::ObjectType);
    YamlObject* oUserDataSomeValueEatArr02 = (YamlObject*) elUserDataSomeValueEatArr02->getData();

    assertObjectPropertyExist(t, oUserDataSomeValueEatArr02, "prop");
    YamlParser::Element* elUserDataSomeValueEatArr02Prop = oUserDataSomeValueEatArr02->at("prop");
    assertElementType(t, elUserDataSomeValueEatArr02Prop, YamlParser::ElementType::ObjectType);
    YamlObject* oUserDataSomeValueEatArr02Prop = (YamlObject*) elUserDataSomeValueEatArr02Prop->getData();
    assertObjectPropertyExist(t, oUserDataSomeValueEatArr02Prop, "test");
    YamlParser::Element* elUserDataSomeValueEatArr02PropTest = oUserDataSomeValueEatArr02Prop->at("test");
    assertElementType(t, elUserDataSomeValueEatArr02PropTest, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elUserDataSomeValueEatArr02PropTest->getData(), "324");

    assertObjectPropertyValue(t, oUserDataObject04, "someVar", "242f");

    assertObjectPropertyExist(t, oUserData, "someProperty");
    assertObjectPropertyExist(t, oUserData, "test");
    assertObjectPropertyExist(t, oUserData, "lklk");

    assertObjectPropertyExist(t, rObj, "someTest");

    // todo: assert

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
