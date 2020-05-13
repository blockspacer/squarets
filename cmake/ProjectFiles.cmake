include_guard( DIRECTORY )

list(APPEND squarets_SOURCES
  # sources
  ${squarets_src_DIR}/core/squarets.cpp
  ${squarets_src_DIR}/core/defaults/defaults.cpp
  ${squarets_src_DIR}/codegen/cpp/cpp_codegen.cpp
  ${squarets_src_DIR}/core/errors/errors_GeneratorError.cpp
  # includes
  ${squarets_include_DIR}/core/squarets.hpp
  ${squarets_include_DIR}/core/defaults/defaults.hpp
  ${squarets_include_DIR}/core/tags.hpp
  ${squarets_include_DIR}/core/errors/errors.hpp
  ${squarets_include_DIR}/codegen/cpp/cpp_codegen.hpp
  ${squarets_include_DIR}/integrations/outcome/error_utils.hpp
  ${squarets_include_DIR}/integrations/outcome/error_macros.hpp
  #
)
