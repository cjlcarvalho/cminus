#pragma once
#include <cassert>
#include <cminus/diagnostics.hpp>
#include <cminus/sourceman.hpp>
#include <optional>

namespace cminus
{
/// Category of a classified word.
enum class Category
{
    Identifier,
    Number,

    Else,
    If,
    Int,
    Return,
    Void,
    While,

    Plus,
    Minus,
    Multiply,
    Divide,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    NotEqual,
    Assign,
    Semicolon,
    Comma,
    OpenParen,
    CloseParen,
    OpenBracket,
    CloseBracket,
    OpenCurly,
    CloseCurly,
};

/// Classified word.
struct Word
{
    const Category category;
    const SourceRange lexeme;

    explicit Word(Category category, SourceRange lexeme) :
        category(category), lexeme(lexeme)
    {
    }

    explicit Word(Category category, SourceLocation begin, SourceLocation end) :
        category(category), lexeme(begin, std::distance(begin, end))
    {
    }
};

/// The scanner transforms a stream of characters into a stream of words.
class Scanner
{
public:
    explicit Scanner(const SourceFile& source,
                     DiagnosticManager& diagman) :
        source(source),
        diagman(diagman)
    {
        this->current_pos = source.view_with_terminator().begin();
    }

    /// Gets the next word in the stream of characters.
    ///
    /// The scanner handles bad words to the best of its abilities,
    /// hence it never fails to return a word.
    ///
    /// \returns the classified word or `std::nullopt` on end of code.
    auto next_word() -> std::optional<Word>;

private:
    static bool is_letter(char c);
    static bool is_digit(char c);
    static bool is_space(char c);

    bool lex_identifier(SourceLocation& out_pos);
    bool lex_number(SourceLocation& out_pos);

private:
    const SourceFile& source;
    DiagnosticManager& diagman;
    SourceLocation current_pos;
};
}
