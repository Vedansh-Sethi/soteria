#include "script/op.hpp"
#include "crypto/crypto.hpp"
#include "crypto/secp256k1.hpp"
#include "crypto/signature.hpp"
#include "utils/byte_to_int.hpp"
#include <algorithm>

Crypto *opInstance = Crypto::GetInstance();

bool toAltStack(Ctx &ctx)
{
    if (ctx.stack.empty())
        return false;
    auto ele = ctx.stack.back();
    ctx.stack.pop_back();
    ctx.altStack.push_back(ele);
    return true;
}

bool fromAtStack(Ctx &ctx)
{
    if (ctx.altStack.empty())
        return false;
    auto ele = ctx.altStack.back();
    ctx.altStack.pop_back();
    ctx.stack.push_back(ele);
    return true;
}

bool opInvalid(Ctx &) { return false; }

bool opCheckSig(Ctx &ctx)
{
    std::vector<uint8_t> publicKey = ctx.stack.back();
    ctx.stack.pop_back();
    std::vector<uint8_t> signKey = ctx.stack.back();
    ctx.stack.pop_back();
    S256Point pubKey = S256Point::parse(publicKey); 
    Signature sign = Signature::parse(signKey);
    if (pubKey.verify(bigEndianToInt(ctx.z), sign))
    {
        return opCodeLookupFast(0x51, ctx);
    }
    else
        return opCodeLookupFast(0x00, ctx);
}

bool opCodeLookupFast(uint8_t opCode, Ctx &ctx)
{
    bool success = opCodeDict[opCode](ctx);
    if (!success)
    {
        std::cerr << "script execution failed at opCode: 0x" << std::hex
                  << (int)opCode << std::dec << std::endl;
    }
    return success;
}

std::vector<uint8_t> encodeNum(int num)
{
    if (num == 0)
        return std::vector<uint8_t>();
    uint absNum = abs(num);
    bool negative = num < 0;
    std::vector<uint8_t> result;
    while (absNum)
    {
        result.push_back(absNum & 0xff);
        absNum >>= 8;
    }

    if (result.back() & 0x80)
    {
        if (negative)
        {
            result.push_back(0x80);
        }
        else
        {
            result.push_back(0x00);
        }
    }
    else if (negative)
    {
        result.back() |= 0x80;
    }
    return result;
}

int decodeNum(std::vector<uint8_t> element)
{
    if (element.empty())
        return 0;
    bool negative = false;
    int result;
    if (element.back() & 0x80)
    {
        negative = true;
        result = element.back() & 0x7f;
    }
    else
    {
        result = element.back();
    }
    std::reverse(element.begin(), element.end());
    for (size_t i = 1; i < element.size(); i++)
    {
        result <<= 8;
        result += element[i];
    }
    if (negative)
        return -result;
    else
        return result;
}

bool op1(Ctx &ctx)
{
    ctx.stack.push_back(encodeNum(1));
    return true;
}

bool op0(Ctx &ctx)
{
    ctx.stack.push_back(encodeNum(0));
    return true;
}

bool opDup(Ctx &ctx)
{
    if (ctx.stack.empty()) return false;
    std::vector<uint8_t> top = ctx.stack.back();
    ctx.stack.push_back(top);
    return true;
}

bool opEqualVerify(Ctx &ctx)
{
    if (ctx.stack.empty())
        return false;
    auto ele1 = ctx.stack.back();
    ctx.stack.pop_back();
    if (ctx.stack.empty())
        return false;
    auto ele2 = ctx.stack.back();
    ctx.stack.pop_back();
    bool out = (ele1 == ele2);
    return out;
}

bool opHash160(Ctx &ctx)
{
    if (ctx.stack.empty())
        return false;
    auto ele = ctx.stack.back();
    ctx.stack.pop_back();
    auto hash = opInstance->hash160(ele);
    ctx.stack.push_back(hash);
    return true;
}