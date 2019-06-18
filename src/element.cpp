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
}
