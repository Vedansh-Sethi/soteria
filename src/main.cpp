
#include "script/script.hpp"
#include "utils/hexer.hpp"
#include <string>
int main()
{
    std::string z =
        "7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d";
    std::vector<uint8_t> sec = dehexifyString(
        "02887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c");
    std::vector<uint8_t> sig =
        dehexifyString("3045022000eff69ef2b1bd93a66ed5219add4fb51e11a840f404876"
                       "325a1e8ffe0529a2c022100c7207fee197d27c618aea621406f6bf5"
                       "ef6fca38681d82b2f06fddbdce6feab601");
    Script script_pubkey = Script(std::vector<Token>(
        {Token{false, sec, 0}, Token{true, std::vector<uint8_t>(), 0xac}}));
    Script script_sig = Script(std::vector<Token>({Token{false, sig, 0}}));
    Script combinedScript = script_sig + script_pubkey;
    std::cout << combinedScript.evaluate(dehexifyString(z)) << std::endl;
}