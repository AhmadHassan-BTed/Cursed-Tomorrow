#include <iostream>
#include <cassert>
#include "CursedTomorrow/Core/Config.hpp"

void TestConfigConstants() {
    assert(CursedTomorrow::Config::ScreenWidth > 0);
    assert(CursedTomorrow::Config::ScreenHeight > 0);
    std::cout << "TestConfigConstants passed!" << std::endl;
}

int main() {
    TestConfigConstants();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
