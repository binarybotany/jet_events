#ifndef JET_EVENTS_MULTICAST_DELEGATE_H_
#define JET_EVENTS_MULTICAST_DELEGATE_H_

#include <functional>
#include <list>

#include "delegate.h"

namespace jet::events {
template <typename RET, typename... PARAMS>
class MulticastDelegate<RET(PARAMS...)> final
    : private DelegateBase<RET(PARAMS...)> {
 public:
  MulticastDelegate() = default;

  ~MulticastDelegate() {
    for (auto &invocation : mInvocations) {
      delete invocation;
    }

    mInvocations.clear();
  }

  MulticastDelegate &operator=(const MulticastDelegate &) = delete;

  MulticastDelegate(const MulticastDelegate &) = delete;

  bool isNull() const { return mInvocations.size() < 1; }

  bool operator==(void *apPtr) const {
    return (apPtr == nullptr) && this->isNull();
  }

  bool operator!=(void *apPtr) const {
    return (apPtr != nullptr) || (!this->isNull());
  }

  size_t size() const { return mInvocations.size(); }

  bool operator==(const MulticastDelegate &arOther) const {
    if (mInvocations.size() != arOther.mInvocations.size()) {
      return false;
    }

    auto otherIter = arOther.mInvocations.begin();
    for (auto iter = mInvocations.begin(); iter != mInvocations.end(); ++iter) {
      if (**iter != **otherIter) {
        return false;
      }
    }

    return true;
  }

  bool operator!=(const MulticastDelegate &arOther) const {
    return !(*this == arOther);
  }

  bool operator==(const Delegate<RET(PARAMS...)> &arOther) const {
    if (isNull() && arOther.isNull()) {
      return true;
    }

    if (arOther.isNull() || (size() != 1)) {
      return false;
    }

    return (arOther.mInvocation == **mInvocations.begin());
  }

  bool operator!=(const Delegate<RET(PARAMS...)> &arOther) const {
    return !(*this == arOther);
  }

  MulticastDelegate &operator+=(const MulticastDelegate &arOther) {
    for (auto &invocation : arOther.mInvocations) {
      this->mInvocations.push_back(
          new typename DelegateBase<RET(PARAMS...)>::InvocationElement(
              invocation->mpObject, invocation->mStub));
    }

    return *this;
  }

  template <typename LAMBDA>
  MulticastDelegate &operator+=(const LAMBDA &arLambda) {
    Delegate<RET(PARAMS...)> d =
        Delegate<RET(PARAMS...)>::template create<LAMBDA>(arLambda);

    return *this;
  }

  MulticastDelegate &operator+=(const Delegate<RET(PARAMS...)> &arOther) {
    if (arOther.isNull()) {
      return *this;
    }

    this->mInvocations.push_back(
        new typename DelegateBase<RET(PARAMS...)>::InvocationElement(
            arOther.mInvocation.mpObject, arOther.mInvocation.mStub));
    return *this;
  }

  void operator()(PARAMS... params) const {
    for (auto &invocation : mInvocations) {
      (*(invocation->mStub))(invocation->mpObject, params...);
    }
  }

  template <typename HANDLER>
  void operator()(PARAMS... params, HANDLER handler) const {
    size_t index = 0;
    for (auto &invocation : mInvocations) {
      RET value = (*(invocation->mStub))(invocation->mpObject, params...);
      handler(index, &value);
      ++index;
    }
  }

  void operator()(PARAMS... params,
                  Delegate<void(size_t, RET *)> handler) const {
    operator()<decltype(handler)>(params..., handler);
  }

  void operator()(PARAMS... params,
                  std::function<void(size_t, RET *)> handler) const {
    operator()<decltype(handler)>(params..., handler);
  }

 private:
  std::list<typename DelegateBase<RET(PARAMS...)>::InvocationElement *>
      mInvocations;
};
}  // namespace jet::events

#endif