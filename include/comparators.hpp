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

}  // namespace bookdb::comp