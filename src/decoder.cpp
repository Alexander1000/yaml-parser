#include <yaml-parser/decoder.h>
#include <yaml-parser/stream.h>

namespace YamlParser
{
    Decoder::Decoder(Stream* stream)
    {
        this->stream = stream;
    }

    void* Decoder::parse()
    {
    }
}
