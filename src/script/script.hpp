#pragma once
#include "script/op.hpp"
#include "utils/byte_stream.hpp"
#include <vector>

class Script
{
  private:
    std::vector<Token> cmds;

  public:
    Script(std::vector<Token> cmds = std::vector<Token>()) : cmds(cmds) {}

    static Script parse(ByteStream&);
    std::vector<uint8_t> serialize() const;
    Script operator+(const Script&) const;
    bool evaluate(std::vector<uint8_t> z) const;
};