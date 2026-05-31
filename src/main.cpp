
#include "core/transaction.hpp"
#include "core/utxo.hpp"
#include "crypto/crypto.hpp"
#include "crypto/secp256k1.hpp"
#include "crypto/signature.hpp"
#include "script/script.hpp"
#include "utils/byte_to_int.hpp"
#include "utils/hexer.hpp"
#include <openssl/crypto.h>
#include <openssl/cryptoerr_legacy.h>
#include <string>

Crypto *mainInstance = Crypto::GetInstance();

void executeEndToEndConsensus()
{
    Crypto *crypto = Crypto::GetInstance();

    std::string secretPhrase = "I survived the C++ cryptographic engine";
    std::vector<uint8_t> secretPhraseBytes(secretPhrase.begin(),
                                           secretPhrase.end());
    std::vector<uint8_t> secret = crypto->hash256(secretPhraseBytes);

    PrivateKey myKey(hexifyBytes(secret));
    std::vector<uint8_t> myPubKey =
        myKey.publicKey.serialize(); // Compressed SEC
    std::vector<uint8_t> myPubKeyHash = crypto->hash160(myPubKey);

    UTXOPool pool;
    std::string mockTxIdHex =
        "0000000000000000000000000000000000000000000000000000000000000001";
    uint32_t mockIndex = 0;
    uint64_t mockAmount = 1000000; // 1,000,000 satoshis

    std::vector<Token> lockCmds;
    lockCmds.push_back({true, {}, 0x76});
    lockCmds.push_back({true, {}, 0xA9});
    lockCmds.push_back({false, myPubKeyHash, 0x00});
    lockCmds.push_back({true, {}, 0x88});
    lockCmds.push_back({true, {}, 0xAC});
    Script utxoLockingScript(lockCmds);

    pool.addUTXO(mockTxIdHex, mockIndex, mockAmount,
                 utxoLockingScript.serialize());

    std::vector<uint8_t> prevTxHashBytes = dehexifyString(mockTxIdHex);

    std::reverse(prevTxHashBytes.begin(), prevTxHashBytes.end());

    TxIn input(littleEndianToInt(prevTxHashBytes), mockIndex, {});

    std::vector<uint8_t> friendHash =
        dehexifyString("ffffffffffffffffffffffffffffffffffffffff");
    std::vector<Token> outLockCmds;
    outLockCmds.push_back({true, {}, 0x76});
    outLockCmds.push_back({true, {}, 0xA9});
    outLockCmds.push_back({false, friendHash, 0x00});
    outLockCmds.push_back({true, {}, 0x88});
    outLockCmds.push_back({true, {}, 0xAC});
    Script outputLockingScript(outLockCmds);

    TxOut output(900000, outputLockingScript.serialize());

    Tx myTx(1, {input}, {output}, 0);

    UTXORecord utxo = pool.getUTXO(mockTxIdHex, mockIndex);

    std::vector<uint8_t> z = myTx.sigHash(0, utxo.scriptPubKey);

    Signature sig = myKey.sign(bigEndianToInt(z));

    std::vector<uint8_t> finalSigBytes = sig.serialize();

    std::vector<Token> unlockCmds;
    unlockCmds.push_back({false, finalSigBytes, 0x00});
    unlockCmds.push_back({false, myPubKey, 0x00});
    Script scriptSig(unlockCmds);

    myTx.txIns[0].scriptSig = scriptSig.serialize();

    std::cout << "Executing Virtual Machine Consensus..." << std::endl;
    bool isValid = myTx.verifyInput(0, pool);

    std::cout << "==================================" << std::endl;
    std::cout << "TRANSACTION VERIFICATION: "
              << (isValid ? "SUCCESS (1)" : "FAILED (0)") << std::endl;
    std::cout << "==================================" << std::endl;
}

int main() { executeEndToEndConsensus(); }