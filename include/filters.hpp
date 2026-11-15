#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

constexpr auto YearBetween(int start, int end) {
    return [start, end](const Book &book) { return start <= book.year_ && book.year_ <= end; };
}

constexpr auto RatingAbove(double min_rating) {
    return [min_rating](const Book &book) { return book.rating_ >= min_rating; };
}

constexpr auto GenreIs(Genre genre) {
    return [genre](const Book &book) { return book.genre_ == genre; };
}

template <BookIterator It, BookSentinel<It> Sent, BookPredicate Pred>
auto filterBooks(It first, Sent last, Pred pred) {
    std::vector<std::reference_wrapper<const Book>> result;
    std::for_each(first, last, [&result, &pred](const Book &book) {
        if (std::invoke(pred, book)) {
            result.push_back(std::cref(book));
        }
    });
    return result;
}

template <BookPredicate... Preds>
constexpr auto all_of(Preds... preds) {
    return [... checks = std::move(preds)](const Book &book) mutable {
        return (static_cast<bool>(std::invoke(checks, book)) && ...);
    };
}

template <BookPredicate... Preds>
constexpr auto any_of(Preds... preds) {
    return [... checks = std::move(preds)](const Book &book) mutable {
        return (static_cast<bool>(std::invoke(checks, book)) || ...);
    };
}

}  // namespace bookdb