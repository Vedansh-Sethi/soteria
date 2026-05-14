#include "core/transaction.hpp"
#include "crypto/crypto.hpp"
#include "utils/byte_stream.hpp"
#include "utils/byte_to_int.hpp"
#include "utils/hexer.hpp"
#include "utils/varint.hpp"
#include <array>
#include <cpr/cpr.h>
#include <stdexcept>
#include <string>

Crypto *transactionInstance = Crypto::GetInstance();

std::array<std::byte, 32> Tx::hash() const
{
    return transactionInstance->hash256(this->serialize());
}

std::vector<uint8_t> TxOut::serialize() const
{
    std::vector<uint8_t> serial;
    std::vector<uint8_t> amountBytes = intToLittleEndian(amount, 8);
    serial.insert(serial.end(), amountBytes.begin(), amountBytes.end());
    std::vector<uint8_t> scriptNumBytes = encodeVarint(scriptPubKey.size());
    serial.insert(serial.end(), scriptNumBytes.begin(), scriptNumBytes.end());
    serial.insert(serial.end(), scriptPubKey.begin(), scriptPubKey.end());
    return serial;
}

std::vector<uint8_t> TxIn::serialize() const
{
    std::vector<uint8_t> serial;
    std::vector<uint8_t> prevHashBytes = intToLittleEndian(prevHash, 32);
    std::vector<uint8_t> prevIdxBytes = intToLittleEndian(prevIdx, 4);
    std::vector<uint8_t> sequenceBytes = intToLittleEndian(sequence, 4);
    serial.insert(serial.end(), prevHashBytes.begin(), prevHashBytes.end());
    serial.insert(serial.end(), prevIdxBytes.begin(), prevIdxBytes.end());
    std::vector<uint8_t> scriptBytesNum = encodeVarint(scriptSig.size());
    serial.insert(serial.end(), scriptBytesNum.begin(), scriptBytesNum.end());
    serial.insert(serial.end(), scriptSig.begin(), scriptSig.end());
    serial.insert(serial.end(), sequenceBytes.begin(), sequenceBytes.end());
    return serial;
}

std::vector<uint8_t> Tx::serialize() const
{
    std::vector<uint8_t> serial;
    std::vector<uint8_t> versionBytes = intToLittleEndian(version, 4);
    std::vector<uint8_t> txInsNumBytes = encodeVarint(txIns.size());
    serial.insert(serial.end(), versionBytes.begin(), versionBytes.end());

    serial.insert(serial.end(), txInsNumBytes.begin(), txInsNumBytes.end());
    for (const TxIn &txIn : txIns)
    {
        std::vector<uint8_t> serialTxIn = txIn.serialize();
        serial.insert(serial.end(), serialTxIn.begin(), serialTxIn.end());
    }

    std::vector<uint8_t> txOutNumBytes = encodeVarint(txOuts.size());
    serial.insert(serial.end(), txOutNumBytes.begin(), txOutNumBytes.end());
    for (const TxOut &txOut : txOuts)
    {
        std::vector<uint8_t> serialTxOut = txOut.serialize();
        serial.insert(serial.end(), serialTxOut.begin(), serialTxOut.end());
    }

    std::vector<uint8_t> locktimeBytes = intToLittleEndian(locktime, 4);
    serial.insert(serial.end(), locktimeBytes.begin(), locktimeBytes.end());

    return serial;
}

std::string Tx::id() const 
{
    std::array<std::byte, 32> hash = this->hash();
    std::reverse(hash.begin(), hash.end());
    return hexifyBytes(hash);
}

TxIn TxIn::parse(ByteStream &stream)
{
    uint256 prevHash;
    std::vector<uint8_t> prevhash = stream.read(32);
    prevHash = littleEndianToInt(prevhash);
    uint32_t idx = uint32_t(littleEndianToInt(stream.read(4)));
    uint64_t scriptByteNum = readVarint(stream);
    std::vector<uint8_t> scriptBytes = stream.read(scriptByteNum);

    uint32_t sequence = uint32_t(littleEndianToInt(stream.read(4)));

    return TxIn(prevHash, idx, scriptBytes, sequence);
}

TxOut TxOut::parse(ByteStream &stream)
{
    uint64_t satoshis = uint64_t(littleEndianToInt(stream.read(8)));
    uint64_t scriptPubKeyByteNum = readVarint(stream);
    std::vector<uint8_t> scriptPubKeyBytes = stream.read(scriptPubKeyByteNum);
    return TxOut(satoshis, scriptPubKeyBytes);
}

Tx Tx::parse(ByteStream &stream)
{
    std::vector<uint8_t> vBytes = stream.read(4);

    uint32_t version = uint32_t(littleEndianToInt(vBytes));

    uint64_t numInputs = readVarint(stream);

    std::vector<TxIn> txIns;
    for (uint64_t i = 0; i < numInputs; i++)
    {
        txIns.push_back(TxIn::parse(stream));
    }

    uint64_t numOutputs = readVarint(stream);
    std::vector<TxOut> txOuts;
    for (uint64_t i = 0; i < numOutputs; i++)
    {
        txOuts.push_back(TxOut::parse(stream));
    }
    uint32_t locktime = uint32_t(littleEndianToInt(stream.read(4)));

    return Tx(version, txIns, txOuts, locktime);
};

bool Tx::isTimeBased() const { return locktime >= 500000000; }

bool Tx::isBlockBased() const { return !isTimeBased(); }