#ifndef JET_EVENTS_DELEGATE_BASE_H_
#define JET_EVENTS_DELEGATE_BASE_H_

/*******************************************************************************
 * Based on Sergey Alexandrovich Kryukov's Impossibly Fast C++ Delegate Fixed.
 ******************************************************************************/
namespace jet::events {
template <typename T>
class DelegateBase;

template <typename RET, typename... PARAMS>
class DelegateBase<RET(PARAMS...)> {
 protected:
  using StubType = RET (*)(void* pThis, PARAMS...);

  struct InvocationElement {
    InvocationElement() = default;

    InvocationElement(void* apThis, StubType aStub)
        : mpObject(apThis), mStub(aStub) {}

    void clone(InvocationElement& arTarget) const {
      arTarget.mStub = mStub;
      arTarget.mpObject = mpObject;
    }

    bool operator==(const InvocationElement& arOther) const {
      return arOther.mStub == mStub && arOther.mpObject == mpObject;
    }

    bool operator!=(const InvocationElement& arOther) const {
      return arOther.mStub != mStub || arOther.mpObject != mpObject;
    }

    void* mpObject = nullptr;
    StubType mStub = nullptr;
  };
};
}  // namespace jet::events

#endif