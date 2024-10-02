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
#include <thread>
#include <type_traits>

#include "logger/Level.hpp"
#include "logger/Output.hpp"
#include "logger/Timing.hpp"

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

template <class T, class U>
concept Derived = std::is_base_of<U, T>::value;

template <StringLiteral domain, Derived<output::Base> Output = output::Console,
          Derived<timing::Base> Timing = timing::Timestamp>
class Logger {
 public:
  class sLog {
   public:
    sLog(Level level, std::ostream& output, const std::thread::id id = std::this_thread::get_id(),
         const std::source_location file_src = std::source_location::current())
        : level_(level), output_(output) {
      file_number_ = utils::get_pos(file_src.file_name(), file_src.line());
      id_ = id;
    }

    // This captures all data_types
    template <typename T>
    sLog& operator<<(const T& token) {
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

      output_ << context.str() << os.str() << std::endl;
      os.str("");
      os.clear();
    }

   private:
    std::ostringstream os;
    std::ostream& output_;
    std::string_view file_number_;
    std::thread::id id_;
    Level level_;
    const Timing timing_;
  };

  struct Debug : public sLog {
    Debug(const std::thread::id id = std::this_thread::get_id(),
          const std::source_location file_src = std::source_location::current())
        : sLog(Level::Debug, output_.stream(), id, file_src) {}
  };

  struct Info : public sLog {
    Info(const std::thread::id id = std::this_thread::get_id(),
         const std::source_location file_src = std::source_location::current())
        : sLog(Level::Info, output_.stream(), id, file_src) {}
  };

  struct Warning : public sLog {
    Warning(const std::thread::id id = std::this_thread::get_id(),
            const std::source_location file_src = std::source_location::current())
        : sLog(Level::Warning, output_.stream(), id, file_src) {}
  };

  struct Error : public sLog {
    Error(const std::thread::id id = std::this_thread::get_id(),
          const std::source_location file_src = std::source_location::current())
        : sLog(Level::Error, output_.stream(), id, file_src) {}
  };

  static Level logging_level;
  static Output output_;
};

template <StringLiteral domain, Derived<output::Base> Output, Derived<timing::Base> Timing>
Level Logger<domain, Output, Timing>::logging_level = Level::Debug;

template <StringLiteral domain, Derived<output::Base> Output, Derived<timing::Base> Timing>
Output Logger<domain, Output, Timing>::output_;

}  // namespace logger
