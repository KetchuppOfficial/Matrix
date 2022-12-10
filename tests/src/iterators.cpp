#include <gtest/gtest.h>
#include <iterator>

#include "container.hpp"

TEST (Iterators, Check_Iterator_Concept)
{
    static_assert (std::random_access_iterator<Containers::iterator<int>>);
    static_assert (std::random_access_iterator<Containers::const_iterator<int>>);
}
