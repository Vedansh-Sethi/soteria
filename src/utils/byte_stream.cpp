#include "utils/byte_stream.hpp"
#include "utils/byte_to_int.hpp"
#include "utils/hexer.hpp"
#include <stdexcept>

std::vector<uint8_t> ByteStream::read(size_t numBytes)
{
    if (cursor + numBytes > data.size())
        throw std::out_of_range("Stream reached end of data");

    std::vector<uint8_t> chunk(data.begin() + cursor,
                               data.begin() + cursor + numBytes);

    cursor += numBytes;

    return chunk;
}

bool ByteStream::isEmpty() const { return cursor >= data.size(); }