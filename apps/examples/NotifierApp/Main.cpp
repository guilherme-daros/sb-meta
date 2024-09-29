#include <iostream>
#include "infra/Notifier.hpp"

struct MessageA {
  int x;
  int y;
  int z;
};

struct MessageB {
  int x;
  int y;
};

struct MessageC {};

// Declare Signature types
using SignatureA = infra::Signature<MessageA, int>;
using SignatureB = infra::Signature<MessageB, int>;
using SignatureC = infra::Signature<MessageC, std::function<int()>>;

// Get Listeners from Signatures
using ListenerA = SignatureA::ListenerType;
using ListenerB = SignatureB::ListenerType;
using ListenerC = SignatureC::ListenerType;

// Define a Service from Listeners and define its virtual methods.
class MyService : public infra::Service<ListenerA, ListenerB, ListenerC> {
  auto OnEvent(MessageA msg, int i) -> void override { std::cout << "ListenerA " << +i << std::endl; }

  auto OnEvent(MessageB msg, int i) -> void override { std::cout << "ListenerB " << +i << std::endl; }

  auto OnEvent(MessageC msg, std::function<int()> f) -> void override { std::cout << "ListenerC " << f() << std::endl; }
};

// Get Signals from Signatures
using SignalA = SignatureA::SignalType;
using SignalB = SignatureB::SignalType;
using SignalC = SignatureC::SignalType;

// Define a Notifier from multiple Signals
using MyNotifier = infra::Notifier<SignalA, SignalB, SignalC>;

int main() {
  MyNotifier n;
  MyService s;

  SignatureA::Connect(n, s);
  SignatureB::Connect(n, s);
  SignatureC::Connect(n, s);

  auto msg1 = MessageA{1, 2, 3};
  auto msg2 = MessageB{1, 2};
  auto msg3 = MessageC{};

  std::function<int()> f = []() { return 3; };

  n.Notify(msg1, 2);
  n.Notify(msg2, 2);
  n.Notify(msg3, f);

  std::cout << "Bye" << std::endl;
  return 0;
}
