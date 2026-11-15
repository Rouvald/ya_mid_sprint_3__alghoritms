#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author_ < rhs.author_; }
};

struct LessByTitle {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title_ < rhs.title_; }
};

struct LessByYear {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.year_ < rhs.year_; }
};

struct LessByPopularity {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count_ < rhs.read_count_; }
};

struct LessByRating {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating_ < rhs.rating_; }
};

}  // namespace bookdb::comp