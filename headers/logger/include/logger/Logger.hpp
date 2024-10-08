#pragma once

#include <filesystem>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <source_location>
#include <sstream>
#include <string>
#include <string_view>

#include "Level.hpp"
#include "Output.hpp"
#include "Timing.hpp"

#include "types/StringLiteral.hpp"

namespace logger {

namespace utils {
inline auto get_pos(const std::string_view path, const uint64_t line) -> std::string_view {
  constexpr wchar_t separator = std::filesystem::path::preferred_separator;

  auto offset = path.rfind(separator) != std::string::npos ? path.rfind(separator) : 0;

  auto path_len = path.length() - offset;

  auto filename = std::string_view{path.substr((offset + 1), path.length())};
  return filename;
}

}  // namespace utils

template <StringLiteral domain, typename... Configs>
class Logger {
 public:
  using Output = output::Output<Configs...>;
  using Timing = timing::Timing<Configs...>;

 private:
  class Log {
   public:
    Log(Level level, std::ostream& output, const uint32_t id, const std::source_location file_src)
        : level_(level), output_(output), id_(id) {
      file_number_ = utils::get_pos(file_src.file_name(), file_src.line());
    }

    // This captures all data_types
    template <typename T>
    Log& operator<<(const T& token) {
      os << token;
      return *this;
    }

    // This deals with iomanip function templates
    void operator<<(std::ostream& (*func)(std::ostream&)) {
      if (logging_level < level_) {
        os.str("");
        os.clear();
        return;
      }

      constexpr auto default_light = std::string_view{"\033[0;39;49m"};
      std::ostringstream context;

      context << "[" << std::setw(timing_.width()) << timing_.get() << "]";
      context << " ";
      context << "[" << id_ << "]";
      context << " ";
      context << std::setw(20) << std::right << domain.data;
      context << " ";
      context << to_color(level_) << " " << to_string(level_) << " " << default_light;
      context << " ";
      context << std::setw(20) << std::left << file_number_;
      context << " ";

      output_ << context.str() << os.str() << func;
      os.str("");
      os.clear();
    }

   private:
    std::ostringstream os;
    std::ostream& output_;
    std::string_view file_number_;
    const uint32_t id_;
    Level level_;
    const Timing timing_;
  };

 public:
  struct Debug : public Log {
    Debug(const uint32_t id = gettid(), const std::source_location file_src = std::source_location::current())
        : Log(Level::Debug, output_.stream(), id, file_src) {}
  };

  struct Info : public Log {
    Info(const uint32_t id = gettid(), const std::source_location file_src = std::source_location::current())
        : Log(Level::Info, output_.stream(), id, file_src) {}
  };

  struct Warning : public Log {
    Warning(const uint32_t id = gettid(), const std::source_location file_src = std::source_location::current())
        : Log(Level::Warning, output_.stream(), id, file_src) {}
  };

  struct Error : public Log {
    Error(const uint32_t id = gettid(), const std::source_location file_src = std::source_location::current())
        : Log(Level::Error, output_.stream(), id, file_src) {}
  };

  static Level logging_level;
  static Output output_;
};

template <StringLiteral domain, typename... Configs>
Level Logger<domain, Configs...>::logging_level = Level::Debug;

template <StringLiteral domain, typename... Configs>
output::Output<Configs...> Logger<domain, Configs...>::output_;

}  // namespace logger
