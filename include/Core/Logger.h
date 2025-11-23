#pragma once

#include <string>
#include <iostream>

namespace Ecosystem {
    namespace Core {
        extern bool gVerbose;

        inline void Log(const std::string& msg) {
            if (gVerbose) std::cout << msg << std::endl;
        }

        inline void LogNoEnd(const std::string& msg) {
            if (gVerbose) std::cout << msg;
        }
    }
}
