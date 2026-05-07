#pragma once
#include <string>

class SHA256
{
  protected:
    SHA256() {}
    static SHA256 *instance;

  public:
    SHA256(SHA256&) = delete;
    void operator=(const SHA256 &) = delete;
    static SHA256 *GetInstance();
    std::string sha256(const std::string &input) const;
};