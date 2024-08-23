#pragma once

#include <optional>
#include <type_traits>
#include <utility>

#include <fcntl.h>
#include <mqueue.h>

namespace infra {

static constexpr int kDefaultMessagePriority = 10;

static constexpr timespec kDefaultTimeout{
    .tv_sec = 1, /* Seconds */
    .tv_nsec = 0 /* Nanoseconds */
};

static constexpr int kMaxMessageSize = 32;
static constexpr int kMaxMessages = 16;

template <typename Message>
class MsgQueue {
 public:
  static std::optional<MsgQueue<Message>> Create(const char* mqueue_path) {
    static_assert(std::is_trivial<Message>::value, "Only trivial types allowed");
    static_assert(sizeof(Message) <= kMaxMessageSize, "Message type exceeds maximum message size");

    mq_attr attr = {
        .mq_flags = 0,                  // Queue flags
        .mq_maxmsg = kMaxMessages,      // Max number of messages in queue
        .mq_msgsize = sizeof(Message),  // Max message size
        .mq_curmsgs = 0                 // Number of messages currently in queue
    };

    auto mq = mq_open(mqueue_path, O_RDWR | O_CREAT, 0, &attr);
    if (mq == (mqd_t)-1) {
      return std::nullopt;
    } else {
      return MsgQueue<Message>{mq};
    }
  }

  //
  MsgQueue(const MsgQueue&) = delete;
  MsgQueue& operator=(const MsgQueue&) = delete;
  MsgQueue(MsgQueue&& other) : mq_{std::exchange(other.mq_, nullptr)} {}
  MsgQueue& operator=(MsgQueue&& other) {
    std::swap(mq_, other.mq_);
    return *this;
  }

  ~MsgQueue() {
    if (mq_ != nullptr) {
      mq_close(mq_);
    }
  }

  bool Send(const Message& msg, int priority = kDefaultMessagePriority) {
    if (mq_send(mq_, reinterpret_cast<const char*>(&msg), sizeof(msg), priority) < 0) {
      return false;
    } else {
      return true;
    }
  }

  bool TimedSend(const Message& msg, int priority = kDefaultMessagePriority,
                 timespec timeout = kDefaultTimeout) {
    if (mq_timedsend(mq_, reinterpret_cast<const char*>(&msg), sizeof(msg), priority, &timeout) <
        0) {
      return false;
    } else {
      return true;
    }
  }

  std::optional<Message> Receive() {
    Message msg;
    if (mq_receive(mq_, reinterpret_cast<char*>(&msg), sizeof(msg), nullptr) < 0) {
      return {};
    } else {
      return msg;
    }
  }

  std::optional<Message> TimedReceive(timespec timeout = kDefaultTimeout) {
    Message msg;
    if (mq_timedreceive(mq_, reinterpret_cast<char*>(&msg), sizeof(msg), nullptr, &timeout) < 0) {
      return {};
    } else {
      return msg;
    }
  }

 private:
  MsgQueue(mqd_t mq) : mq_{mq} {}

  mqd_t mq_;
};

}  // namespace infra
