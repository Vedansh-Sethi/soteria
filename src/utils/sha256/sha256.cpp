#include "utils/sha256/sha256.hpp"
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/types.h>
#include <openssl/err.h>
#include <pthread.h>

SHA256 *SHA256::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new SHA256();
    }

    return instance;
}

std::string SHA256::sha256(const std::string &input) const
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
        return "";

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1 ||
        EVP_DigestUpdate(ctx, input.c_str(), input.size()) != 1 ||
        EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1)
    {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}