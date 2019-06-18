#include <yaml-parser/decoder.h>
#include <yaml-parser/stream.h>
#include <yaml-parser/element.h>

namespace YamlParser
{
    Decoder::Decoder(Stream* stream)
    {
        this->stream = stream;
    }

    Element* Decoder::parse()
    {
        return NULL;
    }
}
