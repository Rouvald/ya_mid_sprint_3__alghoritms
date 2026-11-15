#pragma once

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include <algorithm>
#include <flat_map>
#include <iterator>
#include <numeric>
#include <print>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string, size_t, Comparator> histogram(comp);

    std::for_each(cont.begin(), cont.end(), [&histogram](const Book &book) { ++histogram[std::string{book.author_}]; });

    return histogram;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It first, Sent last) {
    std::flat_map<Genre, std::pair<double, size_t>> ratSum;
    // todo: raw for loop -> cos i have strange errors from libstdc++ with flat_map
    for (; first != last; ++first) {
        const Book &book = *first;

        auto &entry = ratSum.try_emplace(book.genre_).first->second;
        entry.first += book.rating_;
        ++entry.second;
    }
    std::flat_map<Genre, double> ratAvg;
    for (const auto &[genre, stats] : ratSum) {
        ratAvg.emplace(genre, stats.first / stats.second);
    }

    return ratAvg;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &books) {
    if (books.size() == 0) {
        return 0.0;
    }
    auto sum = std::accumulate(books.begin(), books.end(), 0.0,
                               [](double accum, const Book &bk) { return accum + bk.rating_; });
    return sum / static_cast<double>(books.size());
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t num) {
    std::mt19937 generator{std::random_device{}()};

    std::vector<std::reference_wrapper<const Book>> res;
    std::sample(cont.begin(), cont.end(), std::back_inserter(res), num, generator);
    return res;
}

template <BookContainerLike T, typename Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp) {
    const auto amount = std::min(n, cont.size());

    std::sort(cont.begin(), cont.end(), comp);
    std::vector<std::reference_wrapper<const Book>> res;
    std::for_each_n(cont.begin(), amount, [&res](auto &bk) { res.push_back(std::cref(bk)); });
    return res;
}

}  // namespace bookdb
