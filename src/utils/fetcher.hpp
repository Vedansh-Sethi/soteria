#pragma once
#include <string>
#include <unordered_map>
#include "core/transaction.hpp"


class TxFetcher
{
  private:
    static std::unordered_map<std::string, Tx> cache;

  public:
    static Tx fetch(const std::string &txID);
};