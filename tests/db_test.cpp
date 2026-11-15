#include "book_database.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <initializer_list>

using namespace bookdb;

bool compareBooks(const Book &lhs, const Book &rhs) {
    return lhs.title_ == rhs.title_ && lhs.author_ == rhs.author_ && lhs.year_ == rhs.year_ &&
           lhs.genre_ == rhs.genre_ && lhs.rating_ == rhs.rating_ && lhs.read_count_ == rhs.read_count_;
}

TEST(BookDatabaseTest, CtorInitList) {

    std::initializer_list<Book> books{{"1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190},
                                      {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156},
                                      {"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203},
                                      {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89}};

    BookDatabase<> db(books);

    ASSERT_EQ(db.size(), books.size());
    EXPECT_FALSE(db.empty());

    const auto diff =
        std::mismatch(db.GetBooks().begin(), db.GetBooks().end(), books.begin(), books.end(), compareBooks);

    const auto diff1 = diff.first == db.GetBooks().end();
    const auto diff2 = diff.second == books.end();
    EXPECT_TRUE(diff1 && diff2) << "  left:  " << (diff1 ? "<end>" : *diff.first) << "\n"
                                << "  right: " << (diff2 ? "<end>" : *diff.second);
}

TEST(BookDatabaseTest, PushBack) {
    BookDatabase<> db;
    std::string author = "George Orwell";

    Book bk{"1984", author, 1949, Genre::SciFi, 4.0, 190};
    db.PushBack(bk);

    ASSERT_EQ(db.size(), 1);

    auto iter = db.GetBooks().at(0);
    EXPECT_TRUE(compareBooks(iter, bk));

    EXPECT_TRUE(db.GetAuthors().contains(author));
}

TEST(BookDatabaseTest, Emplace) {
    BookDatabase<> db;
    std::string author = "George Orwell";

    Book bk{"1984", author, 1949, Genre::SciFi, 4.0, 190};
    // cos i wanna check Args...
    db.EmplaceBack("1984", author, 1949, Genre::SciFi, 4.0, 190);

    ASSERT_EQ(db.size(), 1);

    auto iter = db.GetBooks().at(0);
    EXPECT_TRUE(compareBooks(iter, bk));

    EXPECT_TRUE(db.GetAuthors().contains(author));
}

TEST(BookDatabaseTest, Api) {
    BookDatabase<> db;

    EXPECT_TRUE(db.empty());

    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);

    ASSERT_EQ(db.size(), 2);
    EXPECT_FALSE(db.empty());

    EXPECT_EQ(std::addressof(*db.begin()), db.GetBooks().data());
    EXPECT_EQ(std::distance(db.begin(), db.end()), 2);
    EXPECT_EQ(std::distance(db.cbegin(), db.cend()), 2);
}
