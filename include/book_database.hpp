#pragma once

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"
#include <algorithm>
#include <initializer_list>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using value_type = typename BookContainer::value_type;
    using size_type = typename BookContainer::size_type;

    using reference = typename BookContainer::reference;
    using const_reference = typename BookContainer::const_reference;

    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;

    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> list) {
        std::for_each(list.begin(), list.end(), [this](auto &book) { PushBack(book); });
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods

    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }

    const_iterator begin() const { return books_.begin(); }
    const_iterator end() const { return books_.end(); }

    const_iterator cbegin() const { return books_.cbegin(); }
    const_iterator cend() const { return books_.cend(); }

    size_type size() const { return books_.size(); }
    bool empty() const { return books_.empty(); }

    void PushBack(const Book &book) {
        auto bookCp = book;
        PushBack(std::move(bookCp));
    }

    void PushBack(Book &&book) {
        auto iter = authors_.emplace(std::string(book.author_)).first;
        auto bk = std::move(book);
        bk.author_ = *iter;
        books_.push_back(std::move(bk));
    }

    template <typename... Args>
        requires std::constructible_from<Book, Args...>
    void EmplaceBack(Args &&...args) {
        Book book{std::forward<Args>(args)...};
        PushBack(std::move(book));
    }

    std::span<const Book> GetBooks() const { return {books_.data(), books_.size()}; }
    std::span<Book> GetBooks() { return {books_.data(), books_.size()}; }
    const AuthorContainer &GetAuthors() { return authors_; }
    const AuthorContainer &GetAuthors() const { return authors_; }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
