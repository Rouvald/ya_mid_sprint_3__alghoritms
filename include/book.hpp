#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace bookdb {

enum class Genre : uint8_t { Fiction = 0, NonFiction, SciFi, Biography, Mystery, Unknown };
// @note: +1 cos enum starts with '0'
constexpr size_t GenreSize = static_cast<size_t>(Genre::Unknown) + 1;

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь
inline constexpr std::array<std::pair<Genre, std::string_view>, GenreSize> genres{
    {{Genre::Fiction, "Fiction"},        //
     {Genre::NonFiction, "NonFiction"},  //
     {Genre::SciFi, "SciFi"},            //
     {Genre::Biography, "Biography"},    //
     {Genre::Mystery, "Mystery"},        //
     {Genre::Unknown, "Unknown"}}};

constexpr Genre GenreFromString(std::string_view s) {
    const auto iter = std::find_if(genres.begin(), genres.end(), [&s](auto &pair) { return pair.second == s; });
    return iter != genres.end() ? iter->first : Genre::Unknown;
}
constexpr std::string_view StringFromGenre(Genre g) {
    const auto iter = std::find_if(genres.begin(), genres.end(), [&g](auto &pair) { return pair.first == g; });
    return iter != genres.end() ? iter->second : "";
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view title_;
    std::string_view author_;

    int year_;
    Genre genre_;
    double rating_;
    int read_count_;

    // Ваш код для конструкторов здесь
    constexpr Book(std::string_view title, std::string_view author, int year, Genre genre, double rating,
                   int read_count) noexcept
        : title_(title), author_(author), year_(year), genre_(genre), rating_(rating), read_count_(read_count) {}
    constexpr Book(std::string_view title, std::string_view author, int year, std::string_view genre, double rating,
                   int read_count) noexcept
        : title_(title), author_(author), year_(year), genre_(GenreFromString(genre)), rating_(rating),
          read_count_(read_count) {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        auto genre_str = bookdb::StringFromGenre(g);
        if (genre_str.empty()) {
            throw logic_error{"Unsupported bookdb::Genre"};
        }
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book> {
    template <typename FormatContext>
    auto format(const bookdb::Book b, FormatContext &fc) const {
        format_to(fc.out(), "Book info:\n");
        format_to(fc.out(), "title - {}\n", b.title_);
        format_to(fc.out(), "author - {}\n", b.author_);
        format_to(fc.out(), "year - {}\n", b.year_);
        format_to(fc.out(), "genre - {}\n", b.genre_);
        format_to(fc.out(), "rating - {}\n", b.rating_);
        format_to(fc.out(), "read_count - {}\n", b.read_count_);
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
