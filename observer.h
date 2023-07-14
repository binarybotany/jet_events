#ifndef JET_EVENTS_OBSERVER_H_
#define JET_EVENTS_OBSERVER_H_

#include "delegate.h"

namespace jet::events {
template <typename EVENT>
class Observer {
 public:
  Observer(Delegate<void(void *, EVENT)> aHandler);

  void handle(void *apSender, EVENT aEvent);

 private:
  Delegate<void(void *, EVENT)> mHandler;
};

template <typename EVENT>
Observer<EVENT>::Observer(Delegate<void(void *, EVENT)> aHandler)
    : mHandler(aHandler) {}

template <typename EVENT>
void Observer<EVENT>::handle(void *apSender, EVENT aEvent) {
  mHandler(apSender, aEvent);
}
}  // namespace jet::events

#endif