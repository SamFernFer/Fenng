#include <fennton/utils/Console.hpp>
#include <fennton/utils/Text.hpp>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Console = Fennton::Console;
namespace Text = Fennton::Text;
int main() {
    auto _duration = std::chrono::seconds(4);
    Console::printl("Waiting {} until attempting to focus on the console.", _duration);
    std::this_thread::sleep_for(_duration);

    #ifdef _WIN32

    auto _conWin = GetConsoleWindow();

    Console::printl("Is the console window valid? {}.", IsWindow(_conWin)? "YES" : "NO");
    SetFocus(_conWin);

    auto _lastError = GetLastError();

    Console::printl("Last error: {}", _lastError);
    Console::printl(
        "Is it ERROR_ACCESS_DENIED? {}",
        _lastError == ERROR_ACCESS_DENIED? "YES" : "NO"
    );
    #endif

    Console::printl("Tried to focus on the console.");
    Console::pausel("Did it work?");
    return 0;
}