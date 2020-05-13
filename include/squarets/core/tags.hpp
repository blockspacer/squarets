#pragma once

#include "squarets/core/errors/errors.hpp"

#include <functional>
#include <string>

namespace squarets {

namespace core {

typedef std::function<
  outcome::result<void, squarets::core::errors::GeneratorErrorExtraInfo>(
  std::string&, const std::string&, const std::string&)> tag_callback;

struct SingleTag {
  std::vector<const char*> opening;
  const char* attrs;
  std::vector<const char*> closing;
};

struct PairTag {
  SingleTag open_tag;
  SingleTag close_tag;
  tag_callback callback;
};

} // namespace core

} // namespace squarets
