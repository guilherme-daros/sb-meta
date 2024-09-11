#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <iomanip>
#include <set>
#include <sstream>

#include <asm/unistd.h>
#include <sys/syscall.h>
#define LOG_GETTID() syscall(__NR_gettid)

#pragma GCC visibility push(default)

class Log {
 private:
 public:
  enum class Level { NONE, ALWAYS, ERROR, WARNING, INFO, DEBUG };

  Log() : _output(output_stream()), _is_tty(isatty(fileno(_output))) {
    start_time_ = std::chrono::high_resolution_clock::now();
  }

  ~Log() { print(os.str()); }

  static std::string uptime() {
    auto now = std::chrono::high_resolution_clock::now();
    auto os = std::ostringstream();
    os << "Uptime";
    return os.str();
  }

  std::ostringstream& get(Level level, const std::string& domain) {
    // Foreground (FG) 39 [default] and background (BG) 49 [default]
    auto default_color = std::string{"\033[0;39;49m"};
    auto color = default_color;
    if (_is_tty) {
      switch (level) {
        case Level::ERROR:
          color = "\033[1;39;41m";  // Red
          break;
        case Level::WARNING:
          color = "\033[1;39;43m";  // Yellow
          break;
        case Level::INFO:
          color = "\033[1;39;42m";  // Green
          break;
        case Level::DEBUG:
          color = "\033[1;39;44m";  // Blue
          break;

        default:
          color = default_color;
      }
    }

    constexpr auto kDomainMaximumWidth = int{21};
    auto shift = kDomainMaximumWidth - static_cast<int>(domain.size());
    if (shift <= 0) shift = int{1};

    os << uptime();
    os << " [" << (int)(LOG_GETTID()) << "]";
    os << std::setw(shift) << " " << domain << " " << color << " " << to_string(level) << " " << default_color << " ";

    os.clear();
    return os;
  }

  void print(const std::string& str) {
    FILE* out = output_stream();
    if (!out) return;  // Log completely silenced

    fprintf(out, "%s", str.c_str());

    if (_is_tty) fprintf(out, "\033[0;39m");  // SETCOLOR_NORMAL

    fflush(out);
  }

  static void reporting_level(Level lvl) { reporting_level() = lvl; }

  static void output_stream(FILE* stream) { output_stream() = stream; }

  static FILE*& output_stream() {
    static FILE* pStream = stdout;
    return pStream;
  }

  static Level& reporting_level() {
    static Level reportingLevel = Level::DEBUG;
    return reportingLevel;
  }

#if defined(E3C_LOG_NO_FILTERING)

  static bool can_log(const std::string&) { return true; }

  static bool can_log(const char*) { return true; }

#else

  static void unblock_tid(unsigned int tid = 0) {
    if (!tid) tid = LOG_GETTID();
    data_locker lock;
    data()->_blocked_tids.erase(tid);
  }

  static void block_tid(unsigned int tid) {
    data_locker lock;
    data()->_blocked_tids.insert(tid);
  }

  static std::set<unsigned int> blocked_tids() {
    data_locker lock;
    return data()->_blocked_tids;
  }

  static const std::set<std::string> blocked_domains() {
    data_locker lock;
    return data()->_blkd;
  }

  static const std::set<std::string> whitelisted_domains() {
    data_locker lock;
    return data()->_wld;
  }

  static void block_domain(const std::string& domain) {
    data_locker lock;
    if (domain == "*") {
      data()->_block_all_domains = true;
    } else {
      data()->_blkd.insert(domain);
      data()->_wld.erase(domain);
    }
  }

  static void unblock_domain(const std::string& domain) {
    data_locker lock;
    if (domain == "*") {
      data()->_block_all_domains = false;
    } else {
      data()->_wld.insert(domain);
      data()->_blkd.erase(domain);
    }
  }

  static bool can_log(const std::string& domain) {
    int tid = LOG_GETTID();

    data_locker lock;
    if (data()->_blocked_tids.find(tid) != data()->_blocked_tids.end()) {
      return false;
    }

    // Optimize the most usual case
    if (!(data()->_block_all_domains) && data()->_blkd.empty()) {
      return true;
    }

    // If we block all, check the whitelist to see if we can enable it.
    if (data()->_block_all_domains) {
      return data()->_wld.count(domain) != 0;
    }

    return data()->_blkd.count(domain) == 0;
  }

 private:
  struct data_locker {
    data_locker() { pthread_mutex_lock(data_locker::get_mutex()); }
    ~data_locker() { pthread_mutex_unlock(data_locker::get_mutex()); }
    static pthread_mutex_t* get_mutex() {
      static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
      return &mutex;
    }
  };

#endif  // defined(E3C_LOG_NO_FILTERING)

 private:
  struct log_detail {
    std::set<std::string> _wld;
    std::set<std::string> _blkd;
    std::set<unsigned int> _blocked_tids;
    bool _block_all_domains;
  };

  static log_detail* data() {
    static log_detail* _detail = NULL;
    if (!_detail) _detail = new log_detail();
    return _detail;
  }

  Log(const Log&);
  Log& operator=(const Log&);

  static const char* to_string(Level level) {
    static const char* const buffer[] = {"Unknown", "A", "E", "W", "I", "D"};
    return buffer[(level >= Level::NONE && level <= Level::DEBUG) ? static_cast<int>(level) : 0];
  }

  FILE* _output;
  bool _is_tty;
  std::ostringstream os;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

#define GENERIC_LOG(domain, level)                             \
  if (level > Log::reporting_level() || !Log::output_stream()) \
    ;                                                          \
  else if (!Log::can_log(domain))                              \
    ;                                                          \
  else                                                         \
    Log().get(level, domain)

#define FILE_BASENAME ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#define FILE_LOG(domain, level)                                                                             \
  GENERIC_LOG(domain, level) << std::setw(3) << std::right << __LINE__ << ":" << std::setw(15) << std::left \
                             << FILE_BASENAME << " - "

#define Stream_DEBUG(domain) FILE_LOG(domain, Log::Level::DEBUG)
#define Stream_ERROR(domain) FILE_LOG(domain, Log::Level::ERROR)
#define Stream_WARNING(domain) FILE_LOG(domain, Log::Level::WARNING)
#define Stream_INFO(domain) FILE_LOG(domain, Log::Level::INFO)
