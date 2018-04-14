#pragma once
#include <cstdio>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cminus
{
/// Handle to a location in the source file.
using SourceLocation = const char*;

/// Handle to a range of characters in the source file.
using SourceRange = std::string_view;

/// Information about a source file.
class SourceFile
{
public:
    /// Constructs a source file from a source text.
    explicit SourceFile(std::string);

    SourceFile(const SourceFile&) = delete;
    SourceFile& operator=(const SourceFile&) = delete;

    SourceFile(SourceFile&&) = default;
    SourceFile& operator=(SourceFile&&) = default;

    /// Constructs a source file from a file stream.
    ///
    /// \returns The newly created source file or `std::nullopt` when a
    ///          stream failure occurs. Call `std::ferror` for error details.
    static auto from_stream(std::FILE* stream, size_t hint_size = -1)
            -> std::optional<SourceFile>;

    /// Gets a view into the source text, including a null terminator.
    auto view_with_terminator() const -> SourceRange;

    /// Finds the line and column associated with a location.
    auto find_line_and_column(SourceLocation loc) const
            -> std::pair<unsigned, unsigned>;

private:
    std::string source_text;
    std::vector<SourceLocation> lines;
};

// Assume SourceLocation and SourceRange are simple types,
// thus they can get copied around with no fear.
static_assert(sizeof(SourceLocation) <= sizeof(size_t)
              && std::is_trivially_copyable_v<SourceLocation>);
static_assert(sizeof(SourceRange) <= 2 * sizeof(size_t)
              && std::is_trivially_copyable_v<SourceRange>);
}
