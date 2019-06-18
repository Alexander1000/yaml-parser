#ifndef YAML_PARSER_ELEMENT_H
#define YAML_PARSER_ELEMENT_H

namespace YamlParser
{
    enum ElementType {
    };

    class Element
    {
    public:
        Element(ElementType type, void* data);
        ElementType getType();
        void* getData();
    private:
        ElementType type;
        void* data;
    };
}

#endif /* YAML_PARSER_ELEMENT_H */
