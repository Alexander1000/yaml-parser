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

void print_tokens(YamlParser::Stream* stream) {
    YamlParser::Token::Token* token = NULL;

    char* buffer = (char*) malloc(sizeof(char) * 1001);

    do {
        token = stream->getNextToken();
        if (token != NULL) {
            std::cout << "-----<Token>------" << std::endl;
            std::cout << "Type: " << YamlParser::Token::tokenTypeName(token->getType()) << std::endl;
            std::cout << "Coords: (" << token->getLine() << "; " << token->getColumn() << ")" << std::endl;

            IOBuffer::IOReader* reader = token->getReader();

            if (reader != NULL) {
                memset(buffer, 1001, sizeof(char));
                reader->read(buffer, 1000);
                std::cout << "Token: " << buffer << std::endl;
            }

            std::cout << std::endl << std::endl;
        }
    } while(token != NULL);
}

CppUnitTest::TestCase* testDecodeObject_SimpleYamlData_Positive()
{
    CppUnitTest::TestCase* t = new CppUnitTest::TestCase("001-simple-object");
    t->printTitle();

    IOBuffer::IOFileReader fileReader("../fixtures/001-simple-object.yaml");
    IOBuffer::CharStream charStream(&fileReader);
    YamlParser::Stream yamlStream(&charStream);
    YamlParser::Decoder decoder(&yamlStream);

    YamlParser::Element* rElement = decoder.parse();
    assertElementType(t, rElement, YamlParser::ElementType::ObjectType);
    YamlObject* rObj = (YamlObject*) rElement->getData();

    assertObjectPropertyValue(t, rObj, "simple", "alexander");

    assertObjectPropertyExist(t, rObj, "userData");

    YamlParser::Element* elUserData = rObj->at("userData");
    assertElementType(t, elUserData, YamlParser::ElementType::ObjectType);
    YamlObject* oUserData = (YamlObject*) elUserData->getData();

    assertObjectPropertyValue(t, oUserData, "name", "Alexander");
    assertObjectPropertyValue(t, oUserData, "birth.date", "28.08.1990");
    assertObjectPropertyValue(t, oUserData, "some/info", "'Test Information'");
    assertObjectPropertyValue(t, oUserData, "key:value", "value2");
    assertObjectPropertyValue(t, oUserData, ":test", "test");
    assertObjectPropertyValue(t, oUserData, "value with space", "some test");
    assertObjectPropertyValue(t, oUserData, "spaced value:with comma", "magic");

    assertObjectPropertyExist(t, rObj, "someTest");
    YamlParser::Element* elSomeTest = rObj->at("someTest");
    assertElementType(t, elSomeTest, YamlParser::ElementType::ObjectType);
    YamlObject* oSomeTest = (YamlObject*) elSomeTest->getData();

    assertObjectPropertyValue(t, oSomeTest, "val", "key");

    t->finish();
    return t;
}

CppUnitTest::TestCase* testDecodeObject_YamlNestedObjects_Positive()
{
    CppUnitTest::TestCase* t = new CppUnitTest::TestCase("002-nested-objects");
    t->printTitle();

    IOBuffer::IOFileReader fileReader("../fixtures/002-nested-objects.yaml");
    IOBuffer::CharStream charStream(&fileReader);
    YamlParser::Stream yamlStream(&charStream);
    YamlParser::Decoder decoder(&yamlStream);

    YamlParser::Element* rElement = decoder.parse();
    assertElementType(t, rElement, YamlParser::ElementType::ObjectType);
    YamlObject* rObj = (YamlObject*) rElement->getData();

    assertObjectPropertyExist(t, rObj, "reporting");
    YamlParser::Element* elReporting = rObj->at("reporting");
    assertElementType(t, elReporting, YamlParser::ElementType::ObjectType);

    YamlObject* oReporting = (YamlObject*) elReporting->getData();
    assertObjectPropertyExist(t, oReporting, "stored_procs");

    YamlParser::Element* elStoredProcs = oReporting->at("stored_procs");
    assertElementType(t, elStoredProcs, YamlParser::ElementType::ObjectType);

    YamlObject* oStoredProcs = (YamlObject*) elStoredProcs->getData();
    assertObjectPropertyExist(t, oStoredProcs, "reportingsp");

    YamlParser::Element* elReportingSp = oStoredProcs->at("reportingsp");
    assertElementType(t, elReportingSp, YamlParser::ElementType::ObjectType);
    YamlObject* oReportingSp = (YamlObject*) elReportingSp->getData();
    assertObjectPropertyExist(t, oReportingSp, "uses");
    YamlParser::Element* elUses01 = oReportingSp->at("uses");
    assertElementType(t, elUses01, YamlParser::ElementType::ObjectType);
    YamlObject* oUses01 = (YamlObject*) elUses01->getData();
    assertObjectPropertyValue(t, oUses01, "usedin", "breadcrumb");

    assertObjectPropertyExist(t, oStoredProcs, "secondProc");
    YamlParser::Element* elSecondProc = oStoredProcs->at("secondProc");
    assertElementType(t, elSecondProc, YamlParser::ElementType::ObjectType);
    YamlObject* oSecondProc = (YamlObject*) elSecondProc->getData();
    assertObjectPropertyExist(t, oSecondProc, "uses");
    YamlParser::Element* elUses02 = oSecondProc->at("uses");
    assertElementType(t, elUses02, YamlParser::ElementType::ObjectType);
    YamlObject* oUses02 = (YamlObject*) elUses02->getData();
    assertObjectPropertyValue(t, oUses02, "usedin", "something_else");

    assertObjectPropertyValue(t, oReporting, "subField", "883");

    assertObjectPropertyValue(t, rObj, "rootElement", "'Puerto Rico'");

    t->finish();
    return t;
}

CppUnitTest::TestCase* testDecodeArray_YamlDataWithArray_Positive()
{
    CppUnitTest::TestCase* t = new CppUnitTest::TestCase("003-employee");
    t->printTitle();

    IOBuffer::IOFileReader fileReader("../fixtures/003-employee.yaml");
    IOBuffer::CharStream charStream(&fileReader);
    YamlParser::Stream yamlStream(&charStream);
    YamlParser::Decoder decoder(&yamlStream);

    // print_tokens(&yamlStream);

    YamlParser::Element* rElement = decoder.parse();
    assertElementType(t, rElement, YamlParser::ElementType::ListType);
    YamlArray* aEmployee = (YamlArray*) rElement->getData();

    CppUnitTest::assertEquals(t, aEmployee->size(), 2);

    YamlArray::iterator itArray = aEmployee->begin();

    YamlParser::Element* elEmployee01 = *itArray;
    assertElementType(t, elEmployee01, YamlParser::ElementType::ObjectType);
    YamlObject* oEmployee01 = (YamlObject*) elEmployee01->getData();
    assertObjectPropertyExist(t, oEmployee01, "martin");

    YamlParser::Element* elMartin = oEmployee01->at("martin");
    assertElementType(t, elMartin, YamlParser::ElementType::ObjectType);
    YamlObject* oMartin = (YamlObject*) elMartin->getData();
    assertObjectPropertyValue(t, oMartin, "name", "Martin D'vloper");
    assertObjectPropertyValue(t, oMartin, "job", "Developer");
    assertObjectPropertyExist(t, oMartin, "skills");

    YamlParser::Element* elSkills01 = oMartin->at("skills");
    assertElementType(t, elSkills01, YamlParser::ElementType::ListType);
    YamlArray* aSkills01 = (YamlArray*) elSkills01->getData();
    YamlArray::iterator itSkills01 = aSkills01->begin();

    YamlParser::Element* elMartinSkill01 = *itSkills01;
    assertElementType(t, elMartinSkill01, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elMartinSkill01->getData(), "python");

    itSkills01++;

    YamlParser::Element* elMartinSkill02 = *itSkills01;
    assertElementType(t, elMartinSkill02, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elMartinSkill02->getData(), "perl");

    itSkills01++;

    YamlParser::Element* elMartinSkill03 = *itSkills01;
    assertElementType(t, elMartinSkill03, YamlParser::ElementType::PlainTextType);
    CppUnitTest::assertEquals(t, (std::string*) elMartinSkill03->getData(), "pascal");

    itArray++;

    YamlParser::Element* elEmployee02 = *itArray;
    assertElementType(t, elEmployee02, YamlParser::ElementType::ObjectType);
    YamlObject* oEmployee02 = (YamlObject*) elEmployee02->getData();
    assertObjectPropertyExist(t, oEmployee02, "tabitha");

    t->finish();
    return t;
}

CppUnitTest::TestCase* testDecodeObject_YamlData_Positive()
{
    CppUnitTest::TestCase* t = new CppUnitTest::TestCase("002-sample-data");
    t->printTitle();

    IOBuffer::IOFileReader fileReader("../fixtures/002-sample-data.yaml");
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

    testSuite.addTestCase(testDecodeObject_SimpleYamlData_Positive());

    testSuite.addTestCase(testDecodeObject_YamlNestedObjects_Positive());

    testSuite.addTestCase(testDecodeArray_YamlDataWithArray_Positive());

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
