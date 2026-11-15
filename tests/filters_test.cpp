#include "book_database.hpp"
#include "filters.hpp"

#include <gtest/gtest.h>
#include <memory>

using namespace bookdb;

TEST(FiltersTest, PredFactories) {
    const Book book{"1984", "George Orwell", 1949, Genre::SciFi, 4.5, 190};

    EXPECT_TRUE(YearBetween(1949, 2000)(book));
    EXPECT_TRUE(YearBetween(1900, 1949)(book));
    EXPECT_FALSE(YearBetween(1950, 2000)(book));
    EXPECT_FALSE(YearBetween(1900, 1948)(book));

    EXPECT_TRUE(RatingAbove(4.5)(book));
    EXPECT_FALSE(RatingAbove(4.6)(book));

    EXPECT_TRUE(GenreIs(Genre::SciFi)(book));
    EXPECT_FALSE(GenreIs(Genre::Fiction)(book));
}

TEST(FiltersTest, CombinesPredAndReturnsRef) {
    const BookDatabase<> db{Book{"1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190},
                            Book{"Dune", "Frank Herbert", 1965, Genre::SciFi, 4.8, 100},
                            Book{"Modern", "Another Author", 2020, Genre::Fiction, 4.9, 50}};

    const auto allMatches = filterBooks(db.cbegin(), db.cend(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));

    ASSERT_EQ(allMatches.size(), 1U);
    EXPECT_EQ(allMatches[0].get().title_, "Dune");
    EXPECT_EQ(std::addressof(allMatches[0].get()), std::addressof(db.GetBooks()[1]));

    const auto anyMatches =
        filterBooks(db.cbegin(), db.cend(), any_of(GenreIs(Genre::Fiction), YearBetween(1940, 1950)));

    ASSERT_EQ(anyMatches.size(), 2U);
    EXPECT_EQ(anyMatches[0].get().title_, "1984");
    EXPECT_EQ(anyMatches[1].get().title_, "Modern");
    EXPECT_EQ(std::addressof(anyMatches[0].get()), std::addressof(db.GetBooks()[0]));
    EXPECT_EQ(std::addressof(anyMatches[1].get()), std::addressof(db.GetBooks()[2]));

    EXPECT_TRUE(filterBooks(db.cbegin(), db.cend(), GenreIs(Genre::Mystery)).empty());
}