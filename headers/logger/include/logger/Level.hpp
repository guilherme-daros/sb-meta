#pragma once

#include <string>
#include <unordered_map>
namespace logger {
enum class Level { None, Error, Warning, Info, Debug };

inline auto to_string(Level level) -> std::string& {
  static auto buffer = std::unordered_map<Level, std::string>{
      {Level::None, "None"}, {Level::Debug, "D"}, {Level::Info, "I"}, {Level::Warning, "W"}, {Level::Error, "E"}};
  return buffer[(level >= Level::None && level <= Level::Debug) ? level : static_cast<Level>(0)];
}

inline auto to_color(Level level) -> std::string& {
  static auto buffer = std::unordered_map<Level, std::string>{{Level::None, "None"},
                                                              {Level::Debug, "\033[1;39;44m"},
                                                              {Level::Info, "\033[1;39;42m"},
                                                              {Level::Warning, "\033[1;39;43m"},
                                                              {Level::Error, "\033[1;39;41m"}};

  return buffer[(level >= Level::None && level <= Level::Debug) ? level : static_cast<Level>(0)];
}
}  // namespace logger
