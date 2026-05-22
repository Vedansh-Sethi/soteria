#pragma once

#include "math/uint256.hpp"
#include "utils/byte_stream.hpp"
#include "utils/hexer.hpp"
#include <cstdint>
#include <ctime>
#include <ostream>
#include <sstream>
#include <vector>

class TxIn
{
  private:
    uint256 prevHash;
    uint32_t prevIdx;
    std::vector<uint8_t> scriptSig;
    uint32_t sequence;

  public:
    TxIn(uint256 prevHash, uint32_t prevIdx,
         std::vector<uint8_t> scriptSigInput, uint32_t sequence = 0xffffffff)
        : prevHash(prevHash), prevIdx(prevIdx), scriptSig(scriptSigInput),
          sequence(sequence)
    {
    }

    friend std::ostream &operator<<(std::ostream &os, const TxIn &txIn)
    {
        os << hexify(txIn.prevHash) << ":" << txIn.prevIdx;
        return os;
    }

    static TxIn parse(ByteStream &);
    std::vector<uint8_t> serialize() const;
    uint64_t value() const;
    std::vector<uint8_t> scriptPubKey() const;
};

class TxOut
{
  private:
    uint64_t amount;
    std::vector<uint8_t> scriptPubKey;

  public:
    friend std::ostream &operator<<(std::ostream &os, const TxOut &txOut)
    {
        os << txOut.amount << ":" << hexifyBytes(txOut.scriptPubKey);
        return os;
    }

    TxOut(uint64_t amount, std::vector<uint8_t> scriptPubKeyInput)
        : amount(amount), scriptPubKey(scriptPubKeyInput)
    {
    }

    static TxOut parse(ByteStream &);
    std::vector<uint8_t> serialize() const;

    friend class TxIn;
    friend class Tx;
};

class Tx
{
  public:
    uint32_t version;
    std::vector<TxIn> txIns;
    std::vector<TxOut> txOuts;
    uint32_t locktime;

    friend std::ostream &operator<<(std::ostream &os, const Tx &tx)
    {
        std::stringstream txIns, txOuts;
        for (TxIn txIn : tx.txIns)
        {
            txIns << txIn << std::endl;
        }
        for (TxOut txOut : tx.txOuts)
        {
            txOuts << txOut << std::endl;
        }
        os << tx.id() << '\n'
           << "version: " << tx.version << "\ntx_ins:\n"
           << txIns.str() << "\ntx_outs:\n"
           << txOuts.str() << "\nlocktime: " << tx.locktime;
        return os;
    }

    std::string id() const;
    std::vector<uint8_t> hash() const;
    static Tx parse(ByteStream &);
    bool isTimeBased() const;
    bool isBlockBased() const;
    std::vector<uint8_t> serialize() const;
    uint64_t fee() const;
};