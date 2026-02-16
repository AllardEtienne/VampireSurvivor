#include "common/common.h"
#include <random>

namespace common
{

float random_float(float min, float max)
{
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}
} // namespace common