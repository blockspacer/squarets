#pragma once

#include <functional>
#include <memory>

#include "squarets/core/defaults/defaults.hpp"
#include "squarets/core/tags.hpp"
#include "squarets/core/errors/errors.hpp"

#include <base/strings/string_util.h>

namespace squarets {

namespace core {

struct GeneratorTags {
  const char* openTagStart =
    defaults::DefaultTags::openTagStart;

  PairTag code_block
    = defaults::DefaultTags::code_block;

  PairTag code_line
    = defaults::DefaultTags::code_line;

  PairTag code_append_raw
    = defaults::DefaultTags::code_append_raw;

  PairTag code_append_as_string
    = defaults::DefaultTags::code_append_as_string;

  PairTag code_include
    = defaults::DefaultTags::code_include;
};

/// \brief used to generate C++ code from template
class Generator {
 public:
  struct Position {
    long int index;
  };

  struct EncloseTagResult {
    base::string16 tagCode;
  };

  /// \brief used to generate C++ code from template string
  outcome::result<std::string, errors::GeneratorErrorExtraInfo> generate_from_ASCII(const char* template_source) noexcept;

  /// \brief used to generate C++ code from template string
  outcome::result<std::string, errors::GeneratorErrorExtraInfo> generate_from_UTF16(base::StringPiece16 template_source) noexcept;

  /// \brief used to modify parser tags
  GeneratorTags& supported_tags() noexcept;

  /// \brief used to change parser tags
  void set_supported_tags(const GeneratorTags&) noexcept;

 private:
  /// \brief will be called when we found `openTagStart`
  outcome::result<void, errors::GeneratorErrorExtraInfo> handleTagStart(
    base::string16& str, Generator::Position& pos, std::string& resultStr);

  /// \brief will be called when we found supported tag after `openTagStart`
  outcome::result<void, errors::GeneratorErrorExtraInfo> handleTag(
      base::string16& str, const PairTag& tag,
      Generator::Position& curPos, std::string& resultStr);

  outcome::result<EncloseTagResult, errors::GeneratorErrorExtraInfo> encloseTag(
      base::string16& processStr, Generator::Position& curPos,
      const std::string& startTag, const std::string& closeTag);

  outcome::result<std::string, errors::GeneratorErrorExtraInfo>
    generateFromString();

 private:
  GeneratorTags GeneratorTags_;

  ///\note used only for debug output
  base::string16 original_str;
};

} // namespace core

} // namespace squarets
