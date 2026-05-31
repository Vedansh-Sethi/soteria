#include "utxo.hpp"
#include <stdexcept>

void UTXOPool::addUTXO(const std::string &txHashHex, uint32_t idx, uint64_t amount, const std::vector<uint8_t> &script)
{
    std::string key = txHashHex + ":" + std::to_string(idx);
    pool[key] = {amount, script};
}

UTXORecord UTXOPool::getUTXO(const std::string &txHashHex, uint32_t idx) const
{
    std::string key = txHashHex + ":" + std::to_string(idx);
    if (pool.find(key) != pool.end()) return pool.at(key);
    throw std::runtime_error("UTXO Entry Not found");
}