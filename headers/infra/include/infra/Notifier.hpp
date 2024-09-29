#include <boost/signals2/variadic_signal.hpp>

#include <boost/signals2/signal.hpp>

namespace infra {

template <typename... Ts>
class Listener {
 public:
  virtual ~Listener() = default;
  virtual auto OnEvent(Ts... pars) -> void = 0;
};

template <typename... Ts>
class Signal {
 public:
  Signal() = default;
  ~Signal() { signal_.disconnect_all_slots(); }

  auto AddListener(Listener<Ts...>& listener) -> void {
    signal_.connect([&listener](Ts... pars) { listener.OnEvent(pars...); });
  }

  auto RemoveListener(Listener<Ts...>& listener) -> void {
    signal_.disconnect([&listener](Ts... pars) { listener.OnEvent(pars...); });
  }

  auto Notify(const Ts&... pars) -> void { signal_(pars...); }

  auto Notify(const Ts&&... pars) -> void { Notify(pars...); }

 private:
  boost::signals2::signal<void(Ts...)> signal_;
};

template <typename... Listener>
class Service : public Listener... {
 public:
  Service() = default;
  ~Service() = default;
};

template <typename... Ts>
class Signature {
 public:
  using ListenerType = Listener<Ts...>;
  using SignalType = Signal<Ts...>;

  static auto Connect(SignalType& ev, ListenerType& listener) -> void { ev.AddListener(listener); }

  static auto Disconnect(SignalType& ev, ListenerType& listener) -> void { ev.RemoveListener(listener); }
};

template <typename... Signals>
class Notifier : public Signals... {
 public:
  Notifier() = default;
  ~Notifier() = default;

  template <typename TL, typename... TLs>
  auto AddListener(Listener<TL, TLs...>& listener) -> void {
    Signal<TL, TLs...>::AddListener(listener);
  }

  template <typename TL, typename... TLs>
  auto RemoveListener(Listener<TL, TLs...>& listener) -> void {
    Signal<TL, TLs...>::RemoveListener(listener);
  }

  template <typename... TLs>
  auto Notify(const TLs&... event) -> void {
    Signal<TLs...>::Notify(event...);
  }
};
}  // namespace infra
