#include <gtest/gtest.h>
#include <iterator>

#include "iterator.hpp"

TEST (Iterators, Check_Iterator_Concept)
{
    static_assert (std::random_access_iterator<yLab::iterator<int>>);
    static_assert (std::random_access_iterator<yLab::const_iterator<int>>);
}
