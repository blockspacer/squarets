#include "testsCommon.h"

#include "squarets/core/squarets.hpp"
#include "squarets/codegen/cpp/cpp_codegen.hpp"
#include "squarets/core/defaults/defaults.hpp"
#include "squarets/core/tags.hpp"
#include "squarets/core/errors/errors.hpp"

#if !defined(USE_GTEST_TEST)
#warning "use USE_GTEST_TEST"
// default
#define USE_GTEST_TEST 1
#endif // !defined(USE_GTEST_TEST)

#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif

#include <base/bind.h>
#include <base/files/file_util.h>
#include <base/path_service.h>
#include <base/base64.h>
#include <base/base64url.h>
#include <base/command_line.h>
#include <base/lazy_instance.h>
#include <base/logging.h>
#include <base/trace_event/trace_event.h>
#include <base/cpu.h>
#include <base/optional.h>
#include <base/path_service.h>
#include <base/time/time.h>
#include <base/memory/ptr_util.h>
#include <base/macros.h>
#include <base/stl_util.h>
#include <base/bind.h>
#include <base/bind_helpers.h>
#include <base/memory/scoped_refptr.h>
#include <base/single_thread_task_runner.h>
#include <base/threading/thread_task_runner_handle.h>
#include <base/numerics/checked_math.h>
#include <base/numerics/clamped_math.h>
#include <base/numerics/safe_conversions.h>
#include <base/i18n/icu_string_conversions.h>
#include <base/i18n/string_compare.h>
#include <base/stl_util.h>
#include <base/base_switches.h>
#include <base/command_line.h>
#include <base/containers/small_map.h>
#include <base/i18n/icu_util.h>
#include <base/i18n/rtl.h>
#include <base/allocator/partition_allocator/page_allocator.h>
#include <base/allocator/allocator_shim.h>
#include <base/allocator/buildflags.h>
#include <base/allocator/partition_allocator/partition_alloc.h>
#include <base/memory/scoped_refptr.h>
#include <base/i18n/rtl.h>
#include <base/stl_util.h>
#include <base/memory/ref_counted_memory.h>
#include <base/memory/read_only_shared_memory_region.h>
#include <base/stl_util.h>
#include <base/bind.h>
#include <base/memory/weak_ptr.h>
#include <base/threading/thread.h>
#include <base/logging.h>
#include <base/system/sys_info.h>
#include <base/synchronization/waitable_event.h>
#include <base/observer_list.h>
#include <base/synchronization/lock.h>
#include <base/threading/thread.h>
#include <base/files/file_path.h>
#include <base/strings/string_util.h>
#include <base/timer/timer.h>
#include <base/callback.h>
#include <base/bind.h>
#include <base/callback.h>
#include <base/command_line.h>
#include <base/logging.h>
#include <base/memory/weak_ptr.h>
#include <base/message_loop/message_loop.h>
#include <base/optional.h>
#include <base/bind.h>
#include <base/callback.h>
#include <base/files/file_path.h>
#include <base/message_loop/message_loop.h>
#include <base/threading/thread.h>
#include <base/threading/thread_checker.h>
#include <base/feature_list.h>
#include <base/bind.h>
#include <base/files/file_util.h>
#include <base/path_service.h>
#include <base/at_exit.h>
#include <base/command_line.h>
#include <base/message_loop/message_loop.h>
#include <base/run_loop.h>
#include <base/trace_event/trace_event.h>
#include <base/trace_event/trace_buffer.h>
#include <base/trace_event/trace_log.h>
#include <base/trace_event/memory_dump_manager.h>
#include <base/trace_event/heap_profiler.h>
#include <base/trace_event/heap_profiler_allocation_context_tracker.h>
#include <base/trace_event/heap_profiler_event_filter.h>
#include <base/sampling_heap_profiler/sampling_heap_profiler.h>
#include <base/sampling_heap_profiler/poisson_allocation_sampler.h>
#include <base/trace_event/malloc_dump_provider.h>
#include <base/trace_event/memory_dump_provider.h>
#include <base/trace_event/memory_dump_scheduler.h>
#include <base/trace_event/memory_infra_background_whitelist.h>
#include <base/trace_event/process_memory_dump.h>
#include <base/trace_event/trace_event.h>
#include <base/allocator/allocator_check.h>
#include <base/base_switches.h>
#include <base/threading/sequence_local_storage_map.h>
#include <base/command_line.h>
#include <base/debug/alias.h>
#include <base/debug/stack_trace.h>
#include <base/memory/ptr_util.h>
#include <base/sequenced_task_runner.h>
#include <base/third_party/dynamic_annotations/dynamic_annotations.h>
#include <base/threading/thread.h>
#include <base/threading/thread_task_runner_handle.h>
#include <base/process/process_handle.h>
#include <base/single_thread_task_runner.h>
#include <base/task_runner.h>
#include <base/task_runner_util.h>
#include <base/task/thread_pool/thread_pool.h>
#include <base/task/thread_pool/thread_pool_impl.h>
#include <base/threading/thread.h>
#include <base/threading/thread_local.h>
#include <base/metrics/histogram.h>
#include <base/metrics/histogram_macros.h>
#include <base/metrics/statistics_recorder.h>
#include <base/metrics/user_metrics.h>
#include <base/metrics/user_metrics_action.h>
#include <base/threading/platform_thread.h>

TEST(Squarrets, SquarretsGenerator) {
  using namespace ::squarets::core::errors;

  const std::string file_contents = R"raw(
[[~ // parameters begin

const auto headerGuard
  = GetWithDefault<std::string>(squarets_params, "headerGuard", "");

const auto generator_path
  = GetWithDefault<std::string>(squarets_params, "generator_path", "");

std::vector<std::string> generator_includes
  = GetWithDefault<std::vector<std::string>>
      (squarets_params, "generator_includes", std::vector<std::string>{});

reflection::ClassInfoPtr ReflectedBaseTypeclass
  = GetWithDefault<reflection::ClassInfoPtr>
      (squarets_params, "ReflectedBaseTypeclass", nullptr);

std::string ImplTypeclassName
  = GetWithDefault<std::string>
      (squarets_params, "ImplTypeclassName", "");

std::string BaseTypeclassName
  = GetWithDefault<std::string>
      (squarets_params, "BaseTypeclassName", "");

// parameters end
/* no newline */ +]][[~]]
// This is generated file. Do not modify directly.
// Path to the code generator: [[+ generator_path +]].

[[+ startHeaderGuard(headerGuard) /* no newline */ +]][[~]]

[[~]] for(const auto& fileName: generator_includes) {
[[+ fileName +]]
[[~]] } // end for

#include <array>
#include <functional>
#include <memory>

namespace cxxctp {
namespace generated {

[[~]]  { // startscope
[[~]]    for(const auto& method: ReflectedBaseTypeclass->methods) {
[[~]]      const size_t methodParamsSize = method->params.size();
[[~]]      const bool needPrint = !method->isImplicit
[[~]]          && !method->isOperator
[[~]]          && !method->isCtor
[[~]]          && !method->isDtor;
[[~]]      if(needPrint) {
  template<>
[[+ method->isExplicitCtor ? "explicit " : "" +]][[~]] /* no newline */
[[+ method->isConstexpr ? "constexpr " : "" +]][[~]] /* no newline */
[[+ method->isStatic ? "static " : "" +]][[~]] /* no newline */
[[+ method->returnType->getPrintedName() +]][[~]] /* no newline */
[[+ " " +]][[~]] /* no newline */
[[+ method->name +]][[~]] /* no newline */
< [[+ BaseTypeclassName +]], [[+ ImplTypeclassName +]] >
(const [[+ ImplTypeclassName +]]& data[[~]] /* no newline */
[[~]] if(methodParamsSize) {
[[+   ", " +]][[~]] /* no newline */
[[+ paramsFullDecls(method->params) +]][[~]] /* no newline */
[[~]] } // methodParamsSize
) [[~]] /* no newline */
[[+ method->isNoExcept ? "noexcept " : "" +]][[~]] /* no newline */
;
[[~ /* newline */ +]]
[[~]]      } // needPrint
[[~]]    } // methods endfor
[[~]]  } // endscope

/*template <>
[[+ ImplTypeclassName +]]& _tc_model_t<[[+ BaseTypeclassName +]]>::ref_concrete<[[+ ImplTypeclassName +]]>() noexcept;
*/

template<>
struct _tc_impl_t<[[+ ImplTypeclassName +]], [[+ BaseTypeclassName +]]>
    : public _tc_model_t<[[+ BaseTypeclassName +]]> {
    typedef [[+ ImplTypeclassName +]] val_type_t;

    // Construct the embedded concrete type.
    template<typename... args_t>
    _tc_impl_t(args_t&&... args) noexcept : concrete(std::forward<args_t>(args)...) { }

    explicit _tc_impl_t(const [[+ ImplTypeclassName +]]& concrete_arg) noexcept
      : concrete(concrete_arg) {}

    /*const size_t getModelTypeIndex() const noexcept override final {
        return _tc_registry<[[+ BaseTypeclassName +]]>::
            getTypeIndex<[[+ ImplTypeclassName +]]>();
    }*/


[[~]]  { // startscope
[[~]]    for(const auto& method: ReflectedBaseTypeclass->methods) {
[[~]]      const size_t methodParamsSize = method->params.size();
[[~]]      const bool needPrint = !method->isImplicit
[[~]]          && !method->isOperator
[[~]]          && !method->isCtor
[[~]]          && !method->isDtor;
[[~]]      if(method->isTemplate()) {
template< [[+ templateParamsFullDecls(method->tplParams) +]] >
[[~]]      } // method->isTemplate
[[~]]      if(needPrint) {
[[+ method->isExplicitCtor ? "explicit " : "" +]][[~]] /* no newline */
[[+ method->isConstexpr ? "constexpr " : "" +]][[~]] /* no newline */
[[+ method->isStatic ? "static " : "" +]][[~]] /* no newline */
[[+ method->returnType->getPrintedName() +]][[~]] /* no newline */
[[+ " __" +]][[~]] /* no newline */
[[+ method->name +]][[~]] /* no newline */
(
[[+ paramsFullDecls(method->params) +]][[~]] /* no newline */
) [[~]] /* no newline */
[[+ method->isConst ? "const " : "" +]][[~]] /* no newline */
[[+ method->isNoExcept ? "noexcept " : "" +]][[~]] /* no newline */
override final
  {
    /// \note passes data, not ref
    return [[+ method->name +]][[~]] /* no newline */
< [[+ BaseTypeclassName +]] >[[~]] /* no newline */
([[~]] /* no newline */
concrete[[~]] /* no newline */
[[+ methodParamsSize ? ", " : "" +]][[~]] /* no newline */
[[+ paramsCallDecls(method->params) +]][[~]] /* no newline */
);
  }
[[~ /* newline */ +]]
[[~]]      } // needPrint
[[~]]    } // methods endfor
[[~]]  } // endscope

    std::unique_ptr<
        _tc_model_t<[[+ BaseTypeclassName +]]>>
    clone() noexcept override final {
        // Copy-construct a new instance of _tc_impl_t on the heap.
        return std::make_unique<_tc_impl_t>(concrete);
    }

    std::unique_ptr<
        _tc_model_t<[[+ BaseTypeclassName +]]>>
    move_clone() noexcept override final {
        return std::make_unique<_tc_impl_t>(std::move(concrete));
    }

    [[+ ImplTypeclassName +]]* operator->() {
        return &concrete;
    }

  // Our actual data.
  [[+ ImplTypeclassName +]] concrete;
};

} // namespace cxxctp
} // namespace generated

[[+ endHeaderGuard(headerGuard) /* no newline */ +]][[~]]

    )raw";

  LOG(INFO)
    << "input contents"
    << file_contents;

  squarets::core::Generator template_engine;

  base::string16 clean_contents;
  squarets::core::defaults::ConvertResponseToUTF16(
    /* unknown encoding */ "",
    file_contents,
    &clean_contents);

  const outcome::result<std::string, GeneratorErrorExtraInfo>
    genResult
      = template_engine.generate_from_UTF16(
        clean_contents);

  if(genResult.has_error()) {
    /*if(genResult.error().ec == GeneratorError::EMPTY_INPUT) {
      ///\note assume not error, just empty file
      XLOG(WARNING) << "WARNING: empty string as Generator input: "
        << genResult.error().extra_info;
      //return;
      std::terminate(); /// TODO: gracefull_shutdown
    } else */{
      LOG(ERROR) << "=== ERROR START ===";
      LOG(ERROR) << "ERROR message: " <<
        make_error_code(genResult.error().ec).message();
      LOG(ERROR) << "ERROR category: " <<
        " " << make_error_code(genResult.error().ec).category().name();
      LOG(ERROR) << "ERROR info: " <<
        " " << genResult.error().extra_info;
      LOG(ERROR) << "input data: "
        /// \note limit to first 200 symbols
        << file_contents.substr(0, std::min(200UL, file_contents.size()))
        << "...";
      // TODO: file path here
      LOG(ERROR) << "=== ERROR END ===";
    }
  }

  if(!genResult.has_value() || genResult.value().empty()) {
    LOG(WARNING) << "WARNING: empty output from file ";
    return;
  }

  std::string genResultNoSpaces;
  base::ReplaceChars(genResult.value(), " ", "", &genResultNoSpaces);

  std::string expectedNoSpaces = std::string{
R"rawdata(
squarets_output
 +=
R"raw(
)raw"
 ;
 // parameters begin

const auto headerGuard
  = GetWithDefault<std::string>(squarets_params, "headerGuard", "");

const auto generator_path
  = GetWithDefault<std::string>(squarets_params, "generator_path", "");

std::vector<std::string> generator_includes
  = GetWithDefault<std::vector<std::string>>
      (squarets_params, "generator_includes", std::vector<std::string>{});

reflection::ClassInfoPtr ReflectedBaseTypeclass
  = GetWithDefault<reflection::ClassInfoPtr>
      (squarets_params, "ReflectedBaseTypeclass", nullptr);

std::string ImplTypeclassName
  = GetWithDefault<std::string>
      (squarets_params, "ImplTypeclassName", "");

std::string BaseTypeclassName
  = GetWithDefault<std::string>
      (squarets_params, "BaseTypeclassName", "");

// parameters end
/* no newline */ +]][[
squarets_output
 +=
R"raw(
// This is generated file. Do not modify directly.
// Path to the code generator: )raw"
 ;

squarets_output
 +=  generator_path  ;

squarets_output
 +=
R"raw(.

)raw"
 ;

squarets_output
 +=  startHeaderGuard(headerGuard) /* no newline */  ;


squarets_output
 +=
R"raw(
)raw"
 ;
 for(const auto& fileName: generator_includes) {

squarets_output
 +=  fileName  ;

squarets_output
 +=
R"raw(
)raw"
 ;
 } // end for
squarets_output
 +=
R"raw(
#include <array>
#include <functional>
#include <memory>

namespace cxxctp {
namespace generated {

)raw"
 ;
  { // startscope
    for(const auto& method: ReflectedBaseTypeclass->methods) {
      const size_t methodParamsSize = method->params.size();
      const bool needPrint = !method->isImplicit
          && !method->isOperator
          && !method->isCtor
          && !method->isDtor;
      if(needPrint) {
squarets_output
 +=
R"raw(  template<>
)raw"
 ;

squarets_output
 +=  method->isExplicitCtor ? "explicit " : ""  ;

 /* no newline */

squarets_output
 +=  method->isConstexpr ? "constexpr " : ""  ;

 /* no newline */

squarets_output
 +=  method->isStatic ? "static " : ""  ;

 /* no newline */

squarets_output
 +=  method->returnType->getPrintedName()  ;

 /* no newline */

squarets_output
 +=  " "  ;

 /* no newline */

squarets_output
 +=  method->name  ;

 /* no newline */
squarets_output
 +=
R"raw(< )raw"
 ;

squarets_output
 +=  BaseTypeclassName  ;

squarets_output
 +=
R"raw(, )raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw( >
(const )raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw(& data)raw"
 ;
 /* no newline */
 if(methodParamsSize) {

squarets_output
 +=    ", "  ;

 /* no newline */

squarets_output
 +=  paramsFullDecls(method->params)  ;

 /* no newline */
 } // methodParamsSize
squarets_output
 +=
R"raw() )raw"
 ;
 /* no newline */

squarets_output
 +=  method->isNoExcept ? "noexcept " : ""  ;

 /* no newline */
squarets_output
 +=
R"raw(;
)raw"
 ;
 /* newline */ +]]
[[
squarets_output
 +=
R"raw(      } // needPrint
)raw"
 ;
    } // methods endfor
  } // endscope
squarets_output
 +=
R"raw(
/*template <>
)raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw(& _tc_model_t<)raw"
 ;

squarets_output
 +=  BaseTypeclassName  ;

squarets_output
 +=
R"raw(>::ref_concrete<)raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw(>() noexcept;
*/

template<>
struct _tc_impl_t<)raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw(, )raw"
 ;

squarets_output
 +=  BaseTypeclassName  ;

squarets_output
 +=
R"raw(>
    : public _tc_model_t<)raw"
 ;

squarets_output
 +=  BaseTypeclassName  ;

squarets_output
 +=
R"raw(> {
    typedef )raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw( val_type_t;

    // Construct the embedded concrete type.
    template<typename... args_t>
    _tc_impl_t(args_t&&... args) noexcept : concrete(std::forward<args_t>(args)...) { }

    explicit _tc_impl_t(const )raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw(& concrete_arg) noexcept
      : concrete(concrete_arg) {}

    /*const size_t getModelTypeIndex() const noexcept override final {
        return _tc_registry<)raw"
 ;

squarets_output
 +=  BaseTypeclassName  ;

squarets_output
 +=
R"raw(>::
            getTypeIndex<)raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw(>();
    }*/


)raw"
 ;
  { // startscope
    for(const auto& method: ReflectedBaseTypeclass->methods) {
      const size_t methodParamsSize = method->params.size();
      const bool needPrint = !method->isImplicit
          && !method->isOperator
          && !method->isCtor
          && !method->isDtor;
      if(method->isTemplate()) {
squarets_output
 +=
R"raw(template< )raw"
 ;

squarets_output
 +=  templateParamsFullDecls(method->tplParams)  ;

squarets_output
 +=
R"raw( >
)raw"
 ;
      } // method->isTemplate
      if(needPrint) {

squarets_output
 +=  method->isExplicitCtor ? "explicit " : ""  ;

 /* no newline */

squarets_output
 +=  method->isConstexpr ? "constexpr " : ""  ;

 /* no newline */

squarets_output
 +=  method->isStatic ? "static " : ""  ;

 /* no newline */

squarets_output
 +=  method->returnType->getPrintedName()  ;

 /* no newline */

squarets_output
 +=  " __"  ;

 /* no newline */

squarets_output
 +=  method->name  ;

 /* no newline */
squarets_output
 +=
R"raw((
)raw"
 ;

squarets_output
 +=  paramsFullDecls(method->params)  ;

 /* no newline */
squarets_output
 +=
R"raw() )raw"
 ;
 /* no newline */

squarets_output
 +=  method->isConst ? "const " : ""  ;

 /* no newline */

squarets_output
 +=  method->isNoExcept ? "noexcept " : ""  ;

 /* no newline */
squarets_output
 +=
R"raw(override final
  {
    /// \note passes data, not ref
    return )raw"
 ;

squarets_output
 +=  method->name  ;

 /* no newline */
squarets_output
 +=
R"raw(< )raw"
 ;

squarets_output
 +=  BaseTypeclassName  ;

squarets_output
 +=
R"raw( >)raw"
 ;
 /* no newline */
squarets_output
 +=
R"raw(()raw"
 ;
 /* no newline */
squarets_output
 +=
R"raw(concrete)raw"
 ;
 /* no newline */

squarets_output
 +=  methodParamsSize ? ", " : ""  ;

 /* no newline */

squarets_output
 +=  paramsCallDecls(method->params)  ;

 /* no newline */
squarets_output
 +=
R"raw();
  }
)raw"
 ;
 /* newline */ +]]
[[
squarets_output
 +=
R"raw(      } // needPrint
)raw"
 ;
    } // methods endfor
  } // endscope
squarets_output
 +=
R"raw(
    std::unique_ptr<
        _tc_model_t<)raw"
 ;

squarets_output
 +=  BaseTypeclassName  ;

squarets_output
 +=
R"raw(>>
    clone() noexcept override final {
        // Copy-construct a new instance of _tc_impl_t on the heap.
        return std::make_unique<_tc_impl_t>(concrete);
    }

    std::unique_ptr<
        _tc_model_t<)raw"
 ;

squarets_output
 +=  BaseTypeclassName  ;

squarets_output
 +=
R"raw(>>
    move_clone() noexcept override final {
        return std::make_unique<_tc_impl_t>(std::move(concrete));
    }

    )raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw(* operator->() {
        return &concrete;
    }

  // Our actual data.
  )raw"
 ;

squarets_output
 +=  ImplTypeclassName  ;

squarets_output
 +=
R"raw( concrete;
};

} // namespace cxxctp
} // namespace generated

)raw"
 ;

squarets_output
 +=  endHeaderGuard(headerGuard) /* no newline */  ;


squarets_output
 +=
R"raw(
    )raw"
 ;
)rawdata"};
  base::ReplaceChars(expectedNoSpaces, " ", "", &expectedNoSpaces);

  EXPECT_EQ(genResultNoSpaces, expectedNoSpaces);
}
