#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct UTXORecord
{
    uint64_t amount;
    std::vector<uint8_t> scriptPubKey;
};

class UTXOPool
{
  public:
    std::map<std::string, UTXORecord> pool;

    void addUTXO(const std::string &, uint32_t, uint64_t,
                       const std::vector<uint8_t> &);
    
    UTXORecord getUTXO(const std::string &, uint32_t) const;
};