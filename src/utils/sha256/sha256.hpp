#pragma once
#include <string>

class SHA256
{
  private:
    SHA256() {}
    static SHA256 *instance;

  public:
    SHA256(const SHA256&) = delete;
    void operator=(const SHA256 &) = delete;
    static SHA256 *GetInstance();
    std::string sha256(const std::string &input) const;
};