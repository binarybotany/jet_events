#ifndef JET_EVENTS_DELEGATE_H_
#define JET_EVENTS_DELEGATE_H_

#include "delegate_base.h"

/*******************************************************************************
 * Based on Sergey Alexandrovich Kryukov's Impossibly Fast C++ Delegate Fixed.
 ******************************************************************************/
namespace JET::events {
template <typename T>
class Delegate;

template <typename T>
class MulticastDelegate;

template <typename RET, typename... PARAMS>
class Delegate<RET(PARAMS...)> final : private DelegateBase<RET(PARAMS...)> {
 public:
  Delegate() = default;

  Delegate(const Delegate &arOther) { arOther.mInvocation.clone(mInvocation); }

  template <typename LAMBDA>
  Delegate(const LAMBDA &arLambda) {
    assign((void *)(&arLambda), LambdaStub<LAMBDA>);
  }

  bool isNull() const { return mInvocation.mStub == nullptr; }

  bool operator==(void *apPtr) const {
    return (apPtr == nullptr) && this->isNull();
  }

  bool operator!=(void *apPtr) const {
    return (apPtr != nullptr) || (!this->isNull());
  }

  Delegate &operator=(const Delegate &arOther) {
    arOther.mInvocation.clone(mInvocation);
    return *this;
  }

  template <typename LAMBDA>
  Delegate &operator=(const LAMBDA &arInstance) {
    assign((void *)(&arInstance), LambdaStub<LAMBDA>);
    return *this;
  }

  bool operator==(const Delegate &arOther) const {
    return mInvocation == arOther.mInvocation;
  }

  bool operator!=(const Delegate &arOther) const {
    return mInvocation != arOther.mInvocation;
  }

  bool operator==(const MulticastDelegate<RET(PARAMS...)> &arOther) const {
    return arOther == (*this);
  }

  bool operator!=(const MulticastDelegate<RET(PARAMS...)> &arOther) const {
    return arOther != (*this);
  }

  template <class T, RET (T::*TMethod)(PARAMS...)>
  static Delegate create(T *apInstance) {
    return Delegate(apInstance, MethodStub<T, TMethod>);
  }

  template <class T, RET (T::*TMethod)(PARAMS...) const>
  static Delegate create(T const *apInstance) {
    return Delegate(const_cast<T *>(apInstance), ConstMethodStub<T, TMethod>);
  }

  template <RET (*TFunction)(PARAMS...)>
  static Delegate create() {
    return Delegate(nullptr, FunctionStub<TFunction>);
  }

  template <typename LAMBDA>
  static Delegate create(const LAMBDA &arLambda) {
    return Delegate((void *)(&arLambda), LambdaStub<LAMBDA>);
  }

  RET operator()(PARAMS... params) const {
    return (*mInvocation.mStub)(mInvocation.mpObject, params...);
  }

 private:
  Delegate(void *apObject,
           typename DelegateBase<RET(PARAMS...)>::StubType aStub) {
    mInvocation.mpObject = apObject;
    mInvocation.mStub = aStub;
  }

  void assign(void *apObject,
              typename DelegateBase<RET(PARAMS...)>::StubType aStub) {
    this->mInvocation.mpObject = apObject;
    this->mInvocation.mStub = aStub;
  }

  template <class T, RET (T::*TMethod)(PARAMS...)>
  static RET MethodStub(void *apThis, PARAMS... params) {
    T *p = static_cast<T *>(apThis);
    return (p->*TMethod)(params...);
  }

  template <class T, RET (T::*TMethod)(PARAMS...) const>
  static RET ConstMethodStub(void *apThis, PARAMS... params) {
    T *const p = static_cast<T *>(apThis);
    return (p->*TMethod)(params...);
  }

  template <RET (*TFunction)(PARAMS...)>
  static RET FunctionStub(void *apThis, PARAMS... params) {
    return (TFunction)(params...);
  }

  template <typename LAMBDA>
  static RET LambdaStub(void *apThis, PARAMS... params) {
    LAMBDA *p = static_cast<LAMBDA *>(apThis);
    return (p->operator())(params...);
  }

  friend class MulticastDelegate<RET(PARAMS...)>;
  typename DelegateBase<RET(PARAMS...)>::InvocationElement mInvocation;
};
}  // namespace JET::events

#endif