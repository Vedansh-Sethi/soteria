#include "script.hpp"
#include "script/op.hpp"
#include "utils/byte_to_int.hpp"
#include "utils/varint.hpp"
#include <stdexcept>

template <typename T>
void insert(std::vector<T> &insertIn, const std::vector<T> &toInsert)
{
    insertIn.insert(insertIn.end(), toInsert.begin(), toInsert.end());
}

Script Script::parse(ByteStream &s)
{
    uint length = readVarint(s);
    std::vector<Token> cmds;
    uint count = 0;
    while (count < length)
    {
        std::vector<uint8_t> current = s.read(1);
        uint8_t currentByte = current[0];
        count++;
        if (currentByte >= 1 && currentByte <= 75)
        {
            uint8_t n = currentByte;
            cmds.push_back(Token{false, s.read(n), 0});
            count += n;
        }
        else if (currentByte == 76)
        {
            uint8_t dataLength = uint8_t(littleEndianToInt(s.read(1)));
            count++;
            cmds.push_back(Token{false, s.read(dataLength), 0});
            count += dataLength;
        }
        else if (currentByte == 77)
        {
            uint16_t dataLength = uint16_t(littleEndianToInt(s.read(2)));
            count += 2;
            cmds.push_back(Token{false, s.read(dataLength), 0});
            count += dataLength;
        }
        else
        {
            cmds.push_back(Token{true, std::vector<uint8_t>(), currentByte});
        }
    }
    return Script(cmds);
}

std::vector<uint8_t> Script::serialize() const
{
    std::vector<uint8_t> result;
    for (Token cmd : cmds)
    {
        if (cmd.isOp)
        {
            insert(result, intToLittleEndian(cmd.data, 1));
        }
        else
        {
            uint length = cmd.dataVector.size();
            if (length <= 75)
            {
                insert(result, intToLittleEndian(length, 1));
            }
            else if (length < 0x100)
            {
                insert(result, intToLittleEndian(76, 1));
                insert(result, intToLittleEndian(length, 1));
            }
            else if (length <= 520)
            {
                insert(result, intToLittleEndian(77, 1));
                insert(result, intToLittleEndian(length, 2));
            }
            else
                throw std::length_error("Tooo long a command");
            insert(result, cmd.dataVector);
        }
    }
    std::vector<uint8_t> final = encodeVarint(result.size());
    insert(final, result);
    return final;
}

Script Script::operator+(const Script &other) const
{
    std::vector<Token> cmds = this->cmds;
    insert(cmds, other.cmds);
    return Script(cmds);
}

bool Script::evaluate(std::vector<uint8_t> z) const
{
    std::vector<Token> cmds = this->cmds;
    Stack stack;
    Stack altStack;
    Ctx ctx = {stack, altStack, cmds, z};
    for (Token cmd : cmds)
    {
        if (cmd.isOp)
        {
            if (!opCodeLookupFast(cmd.data, ctx))
                return false;
        }
        else
        {
            stack.push_back(cmd.dataVector);
        }
    }
    if (stack.size() == 0)
        return false;
    if (stack.back() == std::vector<uint8_t>())
        return false;
    return decodeNum(stack.back());
}