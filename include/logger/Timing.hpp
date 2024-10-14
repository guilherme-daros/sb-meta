#pragma once

#include <fmt/format.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string_view>

#include "Meta.hpp"

namespace sb::logger::timing {

class Uptime;

class Base {
 public:
  using Default = Uptime;
  virtual auto get() const -> std::string_view = 0;
  constexpr virtual auto width() const -> uint8_t = 0;
};

template <typename T>
struct is_timing : std::is_base_of<timing::Base, T> {};

template <typename... Ts>
using Timing = meta::TypeFinder_t<timing::Base, timing::is_timing, Ts...>;

class Uptime : public Base {
 public:
  auto get() const -> std::string_view override {
    static auto start = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    auto uptime_s = std::chrono::duration_cast<std::chrono::seconds>(now - start);
    auto uptime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start) % 1000;
    auto os = std::ostringstream();
    os << std::setw(seconds_width) << std::right << uptime_s.count() << "." << std::setw(milliseconds_width)
       << std::setfill('0') << (uptime_ms.count());

    auto uptime = std::string_view{os.str()};
    return uptime;
  }
  constexpr auto width() const -> uint8_t override { return seconds_width + milliseconds_width; }

 private:
  static constexpr uint8_t seconds_width = 5;
  static constexpr uint8_t milliseconds_width = 3;
};

class Timestamp : public Base {
 public:
  auto get() const -> std::string_view override {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    auto os = std::ostringstream();
    os << std::put_time(std::localtime(&time), "%H:%M:%S") << "." << std::setw(3) << std::setfill('0') << ms.count();

    auto timestamp = std::string_view{os.str()};
    return timestamp;
  }
  constexpr auto width() const -> uint8_t override { return timestamp_width; }

 private:
  static constexpr uint8_t timestamp_width = 12;
};

}  // namespace sb::logger::timing
