#ifndef INCLUDE_FLOATING_POINT_COMPARISON
#define INCLUDE_FLOATING_POINT_COMPARISON

#include <concepts>

namespace yLab
{

namespace cmp
{

constexpr double epsilon = 1e-6;

template <std::floating_point T>
bool are_equal (const T first, const T second,
                const T zero_diff = epsilon,
                const T rel_diff  = epsilon)
{
    auto diff = std::abs (first - second);

    if (diff < zero_diff)
        return true;
    else
        return (diff < std::max (std::abs (first), std::abs (second)) * rel_diff);
}

} // namespace cmp

} // namespace yLab

#endif // INCLUDE_FLOATING_POINT_COMPARISON
