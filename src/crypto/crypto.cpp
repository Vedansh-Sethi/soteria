#include "crypto/crypto.hpp"
#include <cstdint>
#include <memory>
#include <openssl/evp.h>

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

void Crypto::InnerSha256(const uint8_t *input, size_t len,
                         uint8_t *output) const
{
    ScopedCTX ctx(EVP_MD_CTX_new());
    if (!ctx)
        return;

    unsigned int hash_len = 0;

    EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx.get(), input, len);
    EVP_DigestFinal_ex(ctx.get(), output, &hash_len);
}

void Crypto::InnerHash160(const uint8_t *input, size_t len,
                          uint8_t *output) const
{
    ScopedCTX ctx(EVP_MD_CTX_new());
    if (!ctx)
        return;

    uint8_t sha256_hash[32];
    unsigned int sha256_len = 0;

    EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx.get(), input, len);
    EVP_DigestFinal_ex(ctx.get(), sha256_hash, &sha256_len);

    unsigned int ripemd_len = 0;

    EVP_DigestInit_ex(ctx.get(), EVP_ripemd160(), nullptr);
    EVP_DigestUpdate(ctx.get(), sha256_hash, sha256_len);
    EVP_DigestFinal_ex(ctx.get(), output, &ripemd_len);
}

Crypto *Crypto::instance = nullptr;