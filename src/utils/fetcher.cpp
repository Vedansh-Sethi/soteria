#include "utils/fetcher.hpp"
#include "cpr/cprtypes.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include <cpr/cpr.h>

std::unordered_map<std::string, Tx> TxFetcher::cache;

Tx TxFetcher::fetch(const std::string &txID)
{
    if (cache.find(txID) != cache.end())
        return cache[txID];

    std::string url = "https://blockstream.info/api/tx/" + txID + "/hex";

    cpr::Response r = cpr::Get(cpr::Url(url), cpr::Timeout(10000));

    if (r.status_code != 200)
    {
        throw std::runtime_error("Network Error: Could not fetch TX " + txID);
    }

    std::vector<uint8_t> raw_bytes = dehexifyString(r.text);
    ByteStream stream(raw_bytes);
    Tx tx = Tx::parse(stream);

    if (tx.id() != txID)
    {
        throw std::runtime_error("ID Mismatch! Expected: " + txID +
                                 " but got: " + tx.id());
    }

    cache[txID] = tx;
    return tx;
}