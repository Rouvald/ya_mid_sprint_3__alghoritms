#include "statsistics.hpp"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <gtest/gtest.h>
#include <iterator>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

using namespace bookdb;
using BookReferences = std::vector<std::reference_wrapper<const Book>>;

void fillDatabase(BookDatabase<> &db) {
    db.EmplaceBack("Low", "Zeta", 1990, Genre::Fiction, 1.5, 5);
    db.EmplaceBack("High", "Alpha", 2010, Genre::SciFi, 4.5, 40);
    db.EmplaceBack("Middle", "Zeta", 2005, Genre::Fiction, 3.5, 15);
    db.EmplaceBack("Other", "Beta", 2020, Genre::SciFi, 4.0, 25);
    db.EmplaceBack("Biography", "Zeta", 1980, Genre::Biography, 2.0, 10);
    db.EmplaceBack("Third SciFi", "Alpha", 2001, Genre::SciFi, 3.5, 20);
}

TEST(AuthorHistogramTest, EmptyDatabase) {
    const BookDatabase<> db;

    const auto histogram = buildAuthorHistogramFlat(db);

    EXPECT_TRUE(histogram.empty());
}

TEST(AuthorHistogramTest, BuildHistogram) {
    BookDatabase<> db;
    fillDatabase(db);

    const auto histogram = buildAuthorHistogramFlat(db);

    ASSERT_EQ(histogram.size(), 3U);
    EXPECT_EQ(histogram.at(std::string{"Zeta"}), 3U);
    EXPECT_EQ(histogram.at(std::string{"Alpha"}), 2U);
    EXPECT_EQ(histogram.at(std::string{"Beta"}), 1U);
}

TEST(GenreRatingsTest, EmptyDbToEmptyResult) {
    const BookDatabase<> db;

    const auto ratings = calculateGenreRatings(db.cbegin(), db.cend());

    EXPECT_TRUE(ratings.empty());
}

TEST(GenreRatingsTest, CalculateGenreRatings) {
    BookDatabase<> db;
    fillDatabase(db);

    const auto ratings = calculateGenreRatings(db.begin(), db.end());

    ASSERT_EQ(ratings.size(), 3U);
    EXPECT_DOUBLE_EQ(ratings.at(Genre::Fiction), 2.5);
    EXPECT_DOUBLE_EQ(ratings.at(Genre::SciFi), 4.0);
    EXPECT_DOUBLE_EQ(ratings.at(Genre::Biography), 2.0);
    EXPECT_FALSE(ratings.contains(Genre::Mystery));
}

TEST(GenreRatingsTest, OneBookRatingCalc) {
    BookDatabase<> db;
    db.EmplaceBack("One", "Author", 2000, Genre::NonFiction, 3.75, 1);

    const auto ratings = calculateGenreRatings(db.begin(), db.end());

    ASSERT_EQ(ratings.size(), 1U);
    EXPECT_DOUBLE_EQ(ratings.at(Genre::NonFiction), 3.75);
}

TEST(AverageRatingTest, EmptyDbReturnsZero) {
    const BookDatabase<> db;

    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 0.0);
}

TEST(AverageRatingTest, OneBookRatingAvg) {
    BookDatabase<> db;
    db.EmplaceBack("One", "Author", 2000, Genre::Fiction, 4.25, 1);

    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 4.25);
}

TEST(AverageRatingTest, CalcAvgRatings) {
    BookDatabase<> db;
    db.EmplaceBack("First", "Author", 2000, Genre::Fiction, 1.2, 1);
    db.EmplaceBack("Second", "Author", 2001, Genre::Fiction, 3.8, 2);

    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 2.5);
}

TEST(RandomBooksTest, ZeroReturnsEmptyResult) {
    BookDatabase<> db;
    fillDatabase(db);

    const auto selected = sampleRandomBooks(db, 0);

    EXPECT_TRUE(selected.empty());
}

TEST(RandomBooksTest, EmptyDbReturnsEmptyResult) {
    const BookDatabase<> db;

    EXPECT_TRUE(sampleRandomBooks(db, 3).empty());
}

TEST(TopBooksTest, ZeroReturnsEmptyResult) {
    BookDatabase<> db;
    fillDatabase(db);

    const auto top = getTopNBy(db, 0, comp::LessByRating{});

    EXPECT_TRUE(top.empty());
}

TEST(TopBooksTest, EmptyDbReturnsEmptyResult) {
    BookDatabase<> db;

    EXPECT_TRUE(getTopNBy(db, 3, comp::LessByRating{}).empty());
}
