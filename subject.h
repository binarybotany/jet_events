#ifndef JET_EVENTS_SUBJECT_H_
#define JET_EVENTS_SUBJECT_H_

#include <list>

#include "observer.h"

namespace jet::events {
template <typename EVENT>
class Subject {
 public:
  void subscribe(Observer<EVENT> *apObserver);

  void unsubscribe(Observer<EVENT> *apObserver);

  void raise(void *apSender, EVENT aEvent);

 private:
  typename std::list<Observer<EVENT> *> mObservers;
};

template <typename EVENT>
void Subject<EVENT>::subscribe(Observer<EVENT> *apObserver) {
  mObservers.push_back(apObserver);
}

template <typename EVENT>
void Subject<EVENT>::unsubscribe(Observer<EVENT> *apObserver) {
  mObservers.remove(apObserver);
}

template <typename EVENT>
void Subject<EVENT>::raise(void *apSender, EVENT aEvent) {
  typename std::list<Observer<EVENT> *>::iterator lIterator =
      mObservers.begin();

  while (lIterator != mObservers.end()) {
    (*lIterator)->handle(apSender, aEvent);
    ++lIterator;
  }
}
}  // namespace jet::events

#endif