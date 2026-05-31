#pragma once
#include <array>
#include <cstdint>
#include <sys/types.h>
#include <vector>

using Stack = std::vector<std::vector<uint8_t>>;

struct Token
{
    bool isOp;
    std::vector<uint8_t> dataVector;
    uint8_t data;
};

struct Ctx
{
    Stack &stack;
    Stack &altStack;
    std::vector<Token> &cmds;
    std::vector<uint8_t> &z;
};

using opCodeFunc = bool (*)(Ctx &);

bool opInvalid(Ctx &);
bool opCheckSig(Ctx &);
bool op1(Ctx &);
bool op0(Ctx &);
bool opDup(Ctx &);
bool opHash160(Ctx &);
bool opEqualVerify(Ctx &);

std::vector<uint8_t> encodeNum(int);
int decodeNum(std::vector<uint8_t>);

const std::array<opCodeFunc, 256> opCodeDict = []()
{
    std::array<opCodeFunc, 256> table;
    table.fill(opInvalid);

    table[0xac] = opCheckSig;
    table[0x51] = op1;
    table[0x00] = op0;
    table[0x76] = opDup;
    table[0xa9] = opHash160;
    table[0x88] = opEqualVerify;

    return table;
}();

bool opCodeLookupFast(uint8_t, Ctx &);