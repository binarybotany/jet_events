# Jet Events

Header only event handling library using the 
[Impossible Fast C++ Delegate Fixed](https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed) and Subject-Observer pattern.

## TODO

- [ ] Document code
- [ ] Possibly move `LazySingleton` to Jet Core.
- [ ] Add macros for more convenient use of `Delegate`, `Observer`, `Subject`.
- [ ] Add `MulticastDelegate` overloads to `Observer`.

## Usage

```cpp
struct SomeEvent {
  int id;
};

class Something {
 public:
  Something(unsigned int id) : mId(id) {}

  void handler(void *sender, const SomeEvent &event) {
    Something *something = static_cast<Something *>(sender);

    // Handle event
    std::cout << "Event id: " << event.id << std::endl;
    std::cout << "Sender id: " << something->mId << std::endl;
  }

  unsigned int mId;
};

int main(int argc, char **argv) {
  Something something(200);

  // Define delegate
  auto del = Delegate<void(void *, const SomeEvent &)>::create<
      Something, &Something::handler>(&something);

  // Define observer
  std::unique_ptr<Observer<const SomeEvent &>> obs =
      std::make_unique<Observer<const SomeEvent &>>(del);

  // Subscribe observer
  LazySingleton<Subject<const SomeEvent &>>::get().subscribe(obs.get());

  // Raise event
  SomeEvent someEvent;
  someEvent.id = 100;

  LazySingleton<Subject<const SomeEvent &>>::get().raise(&something, someEvent);

  // Unsubscribe observer
  LazySingleton<Subject<const SomeEvent &>>::get().unsubscribe(obs.get());

  return 0;
}
```