#include <cstdint>

namespace Fennton::Grafik {
    // Retrieves the time in seconds since Grafik's initialisation. Should not be called 
    // if Grafik has not been initialised yet.
    double getTime();
    // Returns the value of the raw timer. Should not be called if Grafik has not been 
    // initialised yet.
    std::uint64_t getTimerValue();
    // Retrieves the frequency of the raw timer. Should not be called if Grafik has not 
    // been initialised yet.
    std::uint64_t getTimerFrequency();
}