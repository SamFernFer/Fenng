#include <Application.hpp>
#include <utils/Misc.hpp>
#include <iostream>

int main(int argc, char** argv) {
    try {
        return Fennton::Flood::main(argc, argv);
    } catch (const std::exception& e) {
        Fennton::printlnf("[EXCEPTION] {}", e.what());
    } catch (...) {
        Fennton::println("[UNKNOWN EXCEPTION]");
    }
    std::cin.get();
    return -1;
}