#pragma once
#include <cstdint>
#include <span>
#include <vector>

class ByteStream
{
  private:
    std::span<const uint8_t> data;
    size_t cursor;

  public:
    ByteStream(std::span<const uint8_t> data) : data(data), cursor(0) {}

    std::vector<uint8_t> read(size_t numBytes);
    bool isEmpty() const;
};