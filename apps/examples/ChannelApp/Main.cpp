#include <thread>
#include "infra/Channel.hpp"

enum class MsgId : uint8_t {
  kStart,
  kWait,
  kStop,
};

using MsgData = std::array<uint8_t, 2>;

struct Msg {
  MsgId id;
  MsgData data;
};

void worker(int id, infra::Channel<Msg> &ch) {
  using namespace std::chrono_literals;
  while (true) {
    auto value = ch.receive();

    std::this_thread::sleep_for(1s);
  }
}

int main() {
  auto ch = infra::Channel<Msg>();

  auto &ref0 = ch;
  auto ref1 = std::ref(ch);

  auto t1 = std::thread(worker, 1, std::ref(ch));
  auto t2 = std::thread(worker, 2, std::ref(ch));

  for (int i = 1; i <= 5; ++i) {
    auto msg = Msg{.id = MsgId::kStart,
                   .data = {
                       0x00,
                       0x01,
                   }};
    ch.send(msg);
  }

  ch.close();

  t1.join();
  t2.join();

  return 0;
}
