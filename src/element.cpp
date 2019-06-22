#include <yaml-parser/element.h>

namespace YamlParser
{
    Element::Element(YamlParser::ElementType type, void* data)
    {
        this->type = type;
        this->data = data;
    }

    ElementType Element::getType()
    {
        return this->type;
    }

    void* Element::getData()
    {
        return this->data;
    }

    const char* getElementTypeName(ElementType type) {
        switch (type) {
            case ElementType::ListType:
                return "list";
            case ElementType::PlainTextType:
                return "text";
            case ElementType ::ObjectType:
                return "object";
        }
    }
}
