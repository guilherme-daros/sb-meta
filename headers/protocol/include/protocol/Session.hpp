#include <cstdint>

namespace protocol {
class Session {
 public:
  Session();

 private:
  uint32_t id_;
};
}  // namespace protocol
