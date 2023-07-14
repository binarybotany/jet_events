#ifndef JET_EVENTS_LAZY_SINGLETON_H_
#define JET_EVENTS_LAZY_SINGLETON_H_

namespace JET::events {
template <typename T>
class LazySingleton {
 public:
  static T &get() noexcept {
    static T sInstance{};
    return sInstance;
  }

  LazySingleton(const LazySingleton &) = delete;

  LazySingleton(LazySingleton &&) = delete;

  LazySingleton &operator=(const LazySingleton &) = delete;

  LazySingleton &operator=(LazySingleton &&) = delete;
};

#define SINGLETON(T) LazySingleton<T>::get()
}  // namespace JET::events

#endif