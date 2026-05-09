#include "crypto/crypto.hpp"
#include <cstdint>
#include <memory>
#include <openssl/evp.h>
#include <vector>

Crypto *Crypto::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Crypto();
    }

    return instance;
}

struct CtxDeleter
{
    void operator()(EVP_MD_CTX *ctx) const { EVP_MD_CTX_free(ctx); }
};
using ScopedCTX = std::unique_ptr<EVP_MD_CTX, CtxDeleter>;

std::vector<uint8_t>
Crypto::InnerSha256(const std::vector<uint8_t> &input) const
{
    ScopedCTX ctx(EVP_MD_CTX_new());
    if (!ctx)
        return {};

    std::vector<uint8_t> hash(EVP_MAX_MD_SIZE);
    unsigned int hash_len = 0;

    if (EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(ctx.get(), input.data(), input.size()) != 1 ||
        EVP_DigestFinal_ex(ctx.get(), hash.data(), &hash_len) != 1)
    {
        return {};
    }

    hash.resize(hash_len);
    return hash;
}

std::vector<uint8_t>
Crypto::InnerHash160(const std::vector<uint8_t> &input) const
{
    ScopedCTX ctx(EVP_MD_CTX_new());
    if (!ctx)
        return {};

    std::vector<uint8_t> sha256_hash(EVP_MAX_MD_SIZE);
    unsigned int sha256_len = 0;

    if (EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(ctx.get(), input.data(), input.size()) != 1 ||
        EVP_DigestFinal_ex(ctx.get(), sha256_hash.data(), &sha256_len) != 1)
    {
        return {};
    }

    std::vector<uint8_t> ripemd_hash(EVP_MAX_MD_SIZE);
    unsigned int ripemd_len = 0;

    if (EVP_DigestInit_ex(ctx.get(), EVP_ripemd160(), nullptr) != 1 ||
        EVP_DigestUpdate(ctx.get(), sha256_hash.data(), sha256_len) != 1 ||
        EVP_DigestFinal_ex(ctx.get(), ripemd_hash.data(), &ripemd_len) != 1)
    {
        return {};
    }

    ripemd_hash.resize(ripemd_len);
    return ripemd_hash;
}


Crypto *Crypto::instance = nullptr;