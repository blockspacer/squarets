#include "squarets/core/defaults/defaults.hpp" // IWYU pragma: associated

#include "squarets/core/squarets.hpp"

#include <fstream>

#include "squarets/codegen/cpp/cpp_codegen.hpp"

#include <base/bind.h>
#include <base/compiler_specific.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/metrics/histogram_macros.h>
#include <base/single_thread_task_runner.h>
#include <base/stl_util.h>
#include <base/strings/string_util.h>
#include <base/strings/utf_string_conversions.h>
#include <base/threading/thread_task_runner_handle.h>
#include <base/i18n/case_conversion.h>
#include <base/i18n/i18n_constants.h>
#include <base/i18n/icu_string_conversions.h>
#include <base/strings/string_piece.h>
#include <base/strings/string_util.h>
#include <base/files/file_util.h>
#include <third_party/icu/source/common/unicode/ucnv.h>

// __has_include is currently supported by GCC and Clang. However GCC 4.9 may have issues and
// returns 1 for 'defined( __has_include )', while '__has_include' is actually not supported:
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63662
#if __has_include(<filesystem>)
#include <filesystem>
#else
#include <experimental/filesystem>
#endif // __has_include

#if __has_include(<filesystem>)
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif // __has_include

/// \note may be platform dependant ("\r", "\t", "\n", ...)
#define squarets_NEWLINE "\n"

#define squarets_TAG_OPENING "[["
#define squarets_TAG_CLOSING "]]"

namespace {

static const size_t kMB = 1024 * 1024;

static const size_t kGB = 1024 * kMB;

/// \note |base::ReadFileToStringWithMaxSize| implementation
/// uses size_t
/// The standard says that SIZE_MAX for size_t must be at least 65535.
/// It specifies no upper bound for size_t.
/// \note usually size_t limited to (32bit) 4294967295U
/// or (64bit) 18446744073709551615UL
static const size_t kMaxFileSizeInBytes = 1024 * kGB;

static outcome::result<
  void, squarets::core::errors::GeneratorErrorExtraInfo>
    code_block_cb(std::string& result,
      const std::string& codeBetweenTags,
      const std::string& outVarName)
{
  result += squarets::cpp_codegen::CodeGenerator::
    executeCodeMultiline(codeBetweenTags, outVarName);
  return outcome::success();
}

static outcome::result<
  void, squarets::core::errors::GeneratorErrorExtraInfo>
    code_line_cb(std::string& result,
      const std::string& codeBetweenTags,
      const std::string& outVarName)
{
  result += squarets::cpp_codegen::CodeGenerator::
    executeCodeLine(codeBetweenTags, outVarName);
  return outcome::success();
}

static outcome::result<
  void, squarets::core::errors::GeneratorErrorExtraInfo>
    code_append_raw_cb(std::string& result,
      const std::string& codeBetweenTags,
      const std::string& outVarName)
{
  result += squarets::cpp_codegen::CodeGenerator::
    appendToVariable(codeBetweenTags, outVarName);
  return outcome::success();
}

static outcome::result<
  void, squarets::core::errors::GeneratorErrorExtraInfo>
    code_append_as_string_cb(std::string& result,
      const std::string& codeBetweenTags,
      const std::string& outVarName)
{
  result += squarets::cpp_codegen::CodeGenerator::
    appendToVariableAsString(codeBetweenTags, outVarName);
  return outcome::success();
}

static outcome::result<
  void, squarets::core::errors::GeneratorErrorExtraInfo>
    code_include_cb(std::string& result,
      const std::string& filePathBetweenTags,
      const std::string& outVarName)
{
  std::string cleanPath;
  // removing only leading & trailing spaces/tabs/etc.
  base::TrimString(filePathBetweenTags, " \t\n\r\f\v", &cleanPath);

  VLOG(9)
    << "code_include_cb: cleanPath = "
    << cleanPath;

  std::string file_contents;
  // When the file size exceeds |max_size|, the
  // function returns false with |contents|
  // holding the file truncated to |max_size|.
  const bool file_ok
    = base::ReadFileToStringWithMaxSize(
        base::FilePath{cleanPath}
        , &file_contents
        // |max_size| in bytes
        , kMaxFileSizeInBytes
      );

  if(!file_ok)
  {
    VLOG(9)
      << "Unable to read file";

    return squarets::core::errors::GeneratorErrorExtraInfo{
      squarets::core::errors::GeneratorError::FAILED_TO_READ_FILE,
      "(squarets) Unable to read file " + cleanPath};
  }

  if(file_contents.empty()) {

    VLOG(9)
      << "(squarets) Empty file "
      << cleanPath;

    return squarets::core::errors::GeneratorErrorExtraInfo{
      squarets::core::errors::GeneratorError::EMPTY_INPUT,
      "(squarets) Empty file " + cleanPath};
  }

  base::string16 clean_contents;
  squarets::core::defaults::ConvertResponseToUTF16(
    /* unknown encoding */ "",
    file_contents,
    &clean_contents);

    VLOG(9)
      << "code_include_cb: clean_contents = "
      << clean_contents;

  squarets::core::Generator template_engine;

  const outcome::result
    <std::string,
     squarets::core::errors::GeneratorErrorExtraInfo>
    genResult
      = template_engine.generate_from_UTF16(
          clean_contents);

  std::string genResultStr = BOOST_OUTCOME_TRYX(genResult);

  if(genResultStr.empty()) {

    VLOG(9)
      << "(squarets) Empty generator output from file "
      << cleanPath;

    return squarets::core::errors::GeneratorErrorExtraInfo{
      squarets::core::errors::GeneratorError::EMPTY_INPUT,
      "(squarets) Empty generator output from file " + cleanPath};
  }

  VLOG(9)
    <<  "code_include_cb: genResultStr = "
    << genResultStr;

  result += genResultStr;

  return outcome::success();
}

} // namepace

namespace squarets {

namespace core {

namespace defaults {

const char* DefaultTags::openTagStart = squarets_TAG_OPENING;

// start: [[~
// end: ~]]
const PairTag DefaultTags::code_block
  = PairTag {
    SingleTag{
      {openTagStart},
      "~",
      {""}},
    SingleTag{
      {""},
      "~",
      {squarets_TAG_CLOSING}},
    &code_block_cb
  };

// start: [[~]]
// end: newline
const PairTag DefaultTags::code_line
  = PairTag {
    SingleTag{
      {openTagStart},
      "~",
      {squarets_TAG_CLOSING}},
    SingleTag{
      {squarets_NEWLINE, "\t", "\r", "\v", "\f"},
      "",
      {""}},
    &code_line_cb
  };

// start: [[+
// end: +]]
const PairTag DefaultTags::code_append_raw
  = PairTag {
      SingleTag{
        {openTagStart},
        "+",
        {""}},
      SingleTag{
        {""},
        "+",
        {squarets_TAG_CLOSING}},
      &code_append_raw_cb
    };

// start: [[*
// end: *]]
const PairTag DefaultTags::code_append_as_string
  = PairTag {
      SingleTag{
        {openTagStart},
        "*",
        {""}},
      SingleTag{
        {""},
        "*",
        {squarets_TAG_CLOSING}},
      &code_append_as_string_cb
    };

// start: [[include
// end: include]]
const PairTag DefaultTags::code_include
  = PairTag {
      SingleTag{
        {openTagStart},
        "include",
        {""}},
      SingleTag{
        {""},
        "include",
        {squarets_TAG_CLOSING}},
    &code_include_cb
};


struct BomMapping {
  base::StringPiece prefix;
  const char* charset;
};

// from https://github.com/blockspacer/skia-opengl-emscripten/blob/15d1ed4b15a5f53c4aa0af40ff92a075ac551a3c/src/chromium/net/proxy_resolution/pac_file_fetcher_impl.cc#L69
const BomMapping kBomMappings[] = {
    {"\xFE\xFF", "utf-16be"},
    {"\xFF\xFE", "utf-16le"},
    {"\xEF\xBB\xBF", "utf-8"},
};

bool ConvertToUTF16WithSubstitutions(base::StringPiece text,
                                     const char* charset,
                                     base::string16* output) {
  return base::CodepageToUTF16(
      text, charset, base::OnStringConversionError::SUBSTITUTE, output);
}

// Converts |bytes| (which is encoded by |charset|) to UTF16, saving the resul
// to |*utf16|.
// If |charset| is empty, then we don't know what it was and guess.
void ConvertResponseToUTF16(const std::string& charset,
                            const std::string& bytes,
                            base::string16* utf16) {
  if (charset.empty()) {
    // Guess the charset by looking at the BOM.
    base::StringPiece bytes_str(bytes);
    for (const auto& bom : kBomMappings) {
      if (bytes_str.starts_with(bom.prefix)) {
        VLOG(9)
          << "detected charset "
          << bom.prefix
          << " "
          << bom.charset;
        return ConvertResponseToUTF16(
            bom.charset,
            // Strip the BOM in the converted response.
            bytes.substr(bom.prefix.size()), utf16);
      }
    }

    // Otherwise assume ISO-8859-1 if no charset was specified.
    return ConvertResponseToUTF16(base::kCodepageLatin1, bytes, utf16);
  }

  DCHECK(!charset.empty());

  // Be generous in the conversion -- if any characters lie outside of |charset|
  // (i.e. invalid), then substitute them with U+FFFD rather than failing.
  ConvertToUTF16WithSubstitutions(bytes, charset.c_str(), utf16);
}

} // namespace defaults

} // namespace core

} // namespace squarets
