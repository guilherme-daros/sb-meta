#include <cstdint>

#include "infra/CircularBuffer.hpp"

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

auto main() -> int {
  auto cb = infra::CircularBuffer<Msg, 3>();
  auto mg = Msg{
      .id = MsgId::kStart,
      .data = {0x01, 0x01},
  };

  cb.push(mg);
  cb.push(mg);
  cb.push(mg);
  cb.push(mg);

  return 0;
}
