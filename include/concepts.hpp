#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = true;

template <typename T>
concept BookIterator = std::input_iterator<T> && std::same_as<std::iter_value_t<T>, Book>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I> && BookIterator<I>;

template <typename P>
concept BookPredicate = std::predicate<P &, const Book &>;

template <typename C>
concept BookComparator = std::strict_weak_order<C &, const Book &, const Book &>;

}  // namespace bookdb