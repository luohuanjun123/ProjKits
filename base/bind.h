// This file was GENERATED by command:
//     pump.py bind.h.pump
// DO NOT EDIT BY HAND!!!

/*
 *  Copyright 2012 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// To generate bind.h from bind.h.pump, execute:
// /home/build/google3/third_party/gtest/scripts/pump.py bind.h.pump

// Bind() is an overloaded function that converts method calls into function
// objects (aka functors). The method object is captured as a scoped_refptr<> if
// possible, and as a raw pointer otherwise. Any arguments to the method are
// captured by value. The return value of Bind is a stateful, nullary function
// object. Care should be taken about the lifetime of objects captured by
// Bind(); the returned functor knows nothing about the lifetime of a non
// ref-counted method object or any arguments passed by pointer, and calling the
// functor with a destroyed object will surely do bad things.
//
// Example usage:
//   struct Foo {
//     int Test1() { return 42; }
//     int Test2() const { return 52; }
//     int Test3(int x) { return x*x; }
//     float Test4(int x, float y) { return x + y; }
//   };
//
//   int main() {
//     Foo foo;
//     cout << rtc::Bind(&Foo::Test1, &foo)() << endl;
//     cout << rtc::Bind(&Foo::Test2, &foo)() << endl;
//     cout << rtc::Bind(&Foo::Test3, &foo, 3)() << endl;
//     cout << rtc::Bind(&Foo::Test4, &foo, 7, 8.5f)() << endl;
//   }
//
// Example usage of ref counted objects:
//   struct Bar {
//     int AddRef();
//     int Release();
//
//     void Test() {}
//     void BindThis() {
//       // The functor passed to AsyncInvoke() will keep this object alive.
//       invoker.AsyncInvoke(rtc::Bind(&Bar::Test, this));
//     }
//   };
//
//   int main() {
//     rtc::scoped_refptr<Bar> bar = new rtc::RefCountedObject<Bar>();
//     auto functor = rtc::Bind(&Bar::Test, bar);
//     bar = nullptr;
//     // The functor stores an internal scoped_refptr<Bar>, so this is safe.
//     functor();
//   }
//

#ifndef WEBRTC_BASE_BIND_H_
#define WEBRTC_BASE_BIND_H_

#include "base/scoped_ref_ptr.h"

#define NONAME

namespace rtc {
namespace detail {
// This is needed because the template parameters in Bind can't be resolved
// if they're used both as parameters of the function pointer type and as
// parameters to Bind itself: the function pointer parameters are exact
// matches to the function prototype, but the parameters to bind have
// references stripped. This trick allows the compiler to dictate the Bind
// parameter types rather than deduce them.
template <class T> struct identity { typedef T type; };

// IsRefCounted<T>::value will be true for types that can be used in
// rtc::scoped_refptr<T>, i.e. types that implements nullary functions AddRef()
// and Release(), regardless of their return types. AddRef() and Release() can
// be defined in T or any superclass of T.
template <typename T>
class IsRefCounted {
  // This is a complex implementation detail done with SFINAE.

  // Define types such that sizeof(Yes) != sizeof(No).
  struct Yes { char dummy[1]; };
  struct No { char dummy[2]; };
  // Define two overloaded template functions with return types of different
  // size. This way, we can use sizeof() on the return type to determine which
  // function the compiler would have chosen. One function will be preferred
  // over the other if it is possible to create it without compiler errors,
  // otherwise the compiler will simply remove it, and default to the less
  // preferred function.
  template <typename R>
  static Yes test(R* r, decltype(r->AddRef(), r->Release(), 42));
  template <typename C> static No test(...);

public:
  // Trick the compiler to tell if it's possible to call AddRef() and Release().
  static const bool value = sizeof(test<T>((T*)nullptr, 42)) == sizeof(Yes);
};

// TernaryTypeOperator is a helper class to select a type based on a static bool
// value.
template <bool condition, typename IfTrueT, typename IfFalseT>
struct TernaryTypeOperator {};

template <typename IfTrueT, typename IfFalseT>
struct TernaryTypeOperator<true, IfTrueT, IfFalseT> {
  typedef IfTrueT type;
};

template <typename IfTrueT, typename IfFalseT>
struct TernaryTypeOperator<false, IfTrueT, IfFalseT> {
  typedef IfFalseT type;
};

// PointerType<T>::type will be scoped_refptr<T> for ref counted types, and T*
// otherwise.
template <class T>
struct PointerType {
  typedef typename TernaryTypeOperator<IsRefCounted<T>::value,
                                       scoped_refptr<T>,
                                       T*>::type type;
};

// RemoveScopedPtrRef will capture scoped_refptr by-value instead of
// by-reference.
template <class T> struct RemoveScopedPtrRef { typedef T type; };
template <class T>
struct RemoveScopedPtrRef<const scoped_refptr<T>&> {
  typedef scoped_refptr<T> type;
};
template <class T>
struct RemoveScopedPtrRef<scoped_refptr<T>&> {
  typedef scoped_refptr<T> type;
};

}  // namespace detail

template <class ObjectT, class MethodT, class R>
class MethodFunctor0 {
 public:
  MethodFunctor0(MethodT method, ObjectT* object)
      : method_(method), object_(object) {}
  R operator()() const {
    return (object_->*method_)(); }
 private:
  MethodT method_;
  typename detail::PointerType<ObjectT>::type object_;
};

template <class FunctorT, class R>
class Functor0 {
 public:
  explicit Functor0(const FunctorT& functor)
      : functor_(functor) {}
  R operator()() const {
    return functor_(); }
 private:
  FunctorT functor_;
};


#define FP_T(x) R (ObjectT::*x)()

template <class ObjectT, class R>
MethodFunctor0<ObjectT, FP_T(NONAME), R>
Bind(FP_T(method), ObjectT* object) {
  return MethodFunctor0<ObjectT, FP_T(NONAME), R>(
      method, object);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)() const

template <class ObjectT, class R>
MethodFunctor0<const ObjectT, FP_T(NONAME), R>
Bind(FP_T(method), const ObjectT* object) {
  return MethodFunctor0<const ObjectT, FP_T(NONAME), R>(
      method, object);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)()

template <class ObjectT, class R>
MethodFunctor0<ObjectT, FP_T(NONAME), R>
Bind(FP_T(method), const scoped_refptr<ObjectT>& object) {
  return MethodFunctor0<ObjectT, FP_T(NONAME), R>(
      method, object.get());
}

#undef FP_T
#define FP_T(x) R (*x)()

template <class R>
Functor0<FP_T(NONAME), R>
Bind(FP_T(function)) {
  return Functor0<FP_T(NONAME), R>(
      function);
}

#undef FP_T

template <class ObjectT, class MethodT, class R,
          class P1>
class MethodFunctor1 {
 public:
  MethodFunctor1(MethodT method, ObjectT* object,
                 P1 p1)
      : method_(method), object_(object),
      p1_(p1) {}
  R operator()() const {
    return (object_->*method_)(p1_); }
 private:
  MethodT method_;
  typename detail::PointerType<ObjectT>::type object_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
};

template <class FunctorT, class R,
          class P1>
class Functor1 {
 public:
  Functor1(const FunctorT& functor, P1 p1)
      : functor_(functor),
      p1_(p1) {}
  R operator()() const {
    return functor_(p1_); }
 private:
  FunctorT functor_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
};


#define FP_T(x) R (ObjectT::*x)(P1)

template <class ObjectT, class R,
          class P1>
MethodFunctor1<ObjectT, FP_T(NONAME), R, P1>
Bind(FP_T(method), ObjectT* object,
     typename detail::identity<P1>::type p1) {
  return MethodFunctor1<ObjectT, FP_T(NONAME), R, P1>(
      method, object, p1);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1) const

template <class ObjectT, class R,
          class P1>
MethodFunctor1<const ObjectT, FP_T(NONAME), R, P1>
Bind(FP_T(method), const ObjectT* object,
     typename detail::identity<P1>::type p1) {
  return MethodFunctor1<const ObjectT, FP_T(NONAME), R, P1>(
      method, object, p1);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1)

template <class ObjectT, class R,
          class P1>
MethodFunctor1<ObjectT, FP_T(NONAME), R, P1>
Bind(FP_T(method), const scoped_refptr<ObjectT>& object,
     typename detail::identity<P1>::type p1) {
  return MethodFunctor1<ObjectT, FP_T(NONAME), R, P1>(
      method, object.get(), p1);
}

#undef FP_T
#define FP_T(x) R (*x)(P1)

template <class R,
          class P1>
Functor1<FP_T(NONAME), R, P1>
Bind(FP_T(function),
     typename detail::identity<P1>::type p1) {
  return Functor1<FP_T(NONAME), R, P1>(
      function, p1);
}

#undef FP_T

template <class ObjectT, class MethodT, class R,
          class P1,
          class P2>
class MethodFunctor2 {
 public:
  MethodFunctor2(MethodT method, ObjectT* object,
                 P1 p1,
                 P2 p2)
      : method_(method), object_(object),
      p1_(p1),
      p2_(p2) {}
  R operator()() const {
    return (object_->*method_)(p1_, p2_); }
 private:
  MethodT method_;
  typename detail::PointerType<ObjectT>::type object_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
};

template <class FunctorT, class R,
          class P1,
          class P2>
class Functor2 {
 public:
  Functor2(const FunctorT& functor, P1 p1, P2 p2)
      : functor_(functor),
      p1_(p1),
      p2_(p2) {}
  R operator()() const {
    return functor_(p1_, p2_); }
 private:
  FunctorT functor_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
};


#define FP_T(x) R (ObjectT::*x)(P1, P2)

template <class ObjectT, class R,
          class P1,
          class P2>
MethodFunctor2<ObjectT, FP_T(NONAME), R, P1, P2>
Bind(FP_T(method), ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2) {
  return MethodFunctor2<ObjectT, FP_T(NONAME), R, P1, P2>(
      method, object, p1, p2);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2) const

template <class ObjectT, class R,
          class P1,
          class P2>
MethodFunctor2<const ObjectT, FP_T(NONAME), R, P1, P2>
Bind(FP_T(method), const ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2) {
  return MethodFunctor2<const ObjectT, FP_T(NONAME), R, P1, P2>(
      method, object, p1, p2);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2)

template <class ObjectT, class R,
          class P1,
          class P2>
MethodFunctor2<ObjectT, FP_T(NONAME), R, P1, P2>
Bind(FP_T(method), const scoped_refptr<ObjectT>& object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2) {
  return MethodFunctor2<ObjectT, FP_T(NONAME), R, P1, P2>(
      method, object.get(), p1, p2);
}

#undef FP_T
#define FP_T(x) R (*x)(P1, P2)

template <class R,
          class P1,
          class P2>
Functor2<FP_T(NONAME), R, P1, P2>
Bind(FP_T(function),
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2) {
  return Functor2<FP_T(NONAME), R, P1, P2>(
      function, p1, p2);
}

#undef FP_T

template <class ObjectT, class MethodT, class R,
          class P1,
          class P2,
          class P3>
class MethodFunctor3 {
 public:
  MethodFunctor3(MethodT method, ObjectT* object,
                 P1 p1,
                 P2 p2,
                 P3 p3)
      : method_(method), object_(object),
      p1_(p1),
      p2_(p2),
      p3_(p3) {}
  R operator()() const {
    return (object_->*method_)(p1_, p2_, p3_); }
 private:
  MethodT method_;
  typename detail::PointerType<ObjectT>::type object_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
  typename detail::RemoveScopedPtrRef<P3>::type p3_;
};

template <class FunctorT, class R,
          class P1,
          class P2,
          class P3>
class Functor3 {
 public:
  Functor3(const FunctorT& functor, P1 p1, P2 p2, P3 p3)
      : functor_(functor),
      p1_(p1),
      p2_(p2),
      p3_(p3) {}
  R operator()() const {
    return functor_(p1_, p2_, p3_); }
 private:
  FunctorT functor_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
  typename detail::RemoveScopedPtrRef<P3>::type p3_;
};


#define FP_T(x) R (ObjectT::*x)(P1, P2, P3)

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3>
MethodFunctor3<ObjectT, FP_T(NONAME), R, P1, P2, P3>
Bind(FP_T(method), ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3) {
  return MethodFunctor3<ObjectT, FP_T(NONAME), R, P1, P2, P3>(
      method, object, p1, p2, p3);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2, P3) const

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3>
MethodFunctor3<const ObjectT, FP_T(NONAME), R, P1, P2, P3>
Bind(FP_T(method), const ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3) {
  return MethodFunctor3<const ObjectT, FP_T(NONAME), R, P1, P2, P3>(
      method, object, p1, p2, p3);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2, P3)

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3>
MethodFunctor3<ObjectT, FP_T(NONAME), R, P1, P2, P3>
Bind(FP_T(method), const scoped_refptr<ObjectT>& object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3) {
  return MethodFunctor3<ObjectT, FP_T(NONAME), R, P1, P2, P3>(
      method, object.get(), p1, p2, p3);
}

#undef FP_T
#define FP_T(x) R (*x)(P1, P2, P3)

template <class R,
          class P1,
          class P2,
          class P3>
Functor3<FP_T(NONAME), R, P1, P2, P3>
Bind(FP_T(function),
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3) {
  return Functor3<FP_T(NONAME), R, P1, P2, P3>(
      function, p1, p2, p3);
}

#undef FP_T

template <class ObjectT, class MethodT, class R,
          class P1,
          class P2,
          class P3,
          class P4>
class MethodFunctor4 {
 public:
  MethodFunctor4(MethodT method, ObjectT* object,
                 P1 p1,
                 P2 p2,
                 P3 p3,
                 P4 p4)
      : method_(method), object_(object),
      p1_(p1),
      p2_(p2),
      p3_(p3),
      p4_(p4) {}
  R operator()() const {
    return (object_->*method_)(p1_, p2_, p3_, p4_); }
 private:
  MethodT method_;
  typename detail::PointerType<ObjectT>::type object_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
  typename detail::RemoveScopedPtrRef<P3>::type p3_;
  typename detail::RemoveScopedPtrRef<P4>::type p4_;
};

template <class FunctorT, class R,
          class P1,
          class P2,
          class P3,
          class P4>
class Functor4 {
 public:
  Functor4(const FunctorT& functor, P1 p1, P2 p2, P3 p3, P4 p4)
      : functor_(functor),
      p1_(p1),
      p2_(p2),
      p3_(p3),
      p4_(p4) {}
  R operator()() const {
    return functor_(p1_, p2_, p3_, p4_); }
 private:
  FunctorT functor_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
  typename detail::RemoveScopedPtrRef<P3>::type p3_;
  typename detail::RemoveScopedPtrRef<P4>::type p4_;
};


#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4)

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4>
MethodFunctor4<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4>
Bind(FP_T(method), ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4) {
  return MethodFunctor4<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4>(
      method, object, p1, p2, p3, p4);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4) const

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4>
MethodFunctor4<const ObjectT, FP_T(NONAME), R, P1, P2, P3, P4>
Bind(FP_T(method), const ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4) {
  return MethodFunctor4<const ObjectT, FP_T(NONAME), R, P1, P2, P3, P4>(
      method, object, p1, p2, p3, p4);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4)

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4>
MethodFunctor4<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4>
Bind(FP_T(method), const scoped_refptr<ObjectT>& object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4) {
  return MethodFunctor4<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4>(
      method, object.get(), p1, p2, p3, p4);
}

#undef FP_T
#define FP_T(x) R (*x)(P1, P2, P3, P4)

template <class R,
          class P1,
          class P2,
          class P3,
          class P4>
Functor4<FP_T(NONAME), R, P1, P2, P3, P4>
Bind(FP_T(function),
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4) {
  return Functor4<FP_T(NONAME), R, P1, P2, P3, P4>(
      function, p1, p2, p3, p4);
}

#undef FP_T

template <class ObjectT, class MethodT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5>
class MethodFunctor5 {
 public:
  MethodFunctor5(MethodT method, ObjectT* object,
                 P1 p1,
                 P2 p2,
                 P3 p3,
                 P4 p4,
                 P5 p5)
      : method_(method), object_(object),
      p1_(p1),
      p2_(p2),
      p3_(p3),
      p4_(p4),
      p5_(p5) {}
  R operator()() const {
    return (object_->*method_)(p1_, p2_, p3_, p4_, p5_); }
 private:
  MethodT method_;
  typename detail::PointerType<ObjectT>::type object_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
  typename detail::RemoveScopedPtrRef<P3>::type p3_;
  typename detail::RemoveScopedPtrRef<P4>::type p4_;
  typename detail::RemoveScopedPtrRef<P5>::type p5_;
};

template <class FunctorT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5>
class Functor5 {
 public:
  Functor5(const FunctorT& functor, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
      : functor_(functor),
      p1_(p1),
      p2_(p2),
      p3_(p3),
      p4_(p4),
      p5_(p5) {}
  R operator()() const {
    return functor_(p1_, p2_, p3_, p4_, p5_); }
 private:
  FunctorT functor_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
  typename detail::RemoveScopedPtrRef<P3>::type p3_;
  typename detail::RemoveScopedPtrRef<P4>::type p4_;
  typename detail::RemoveScopedPtrRef<P5>::type p5_;
};


#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4, P5)

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5>
MethodFunctor5<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5>
Bind(FP_T(method), ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4,
     typename detail::identity<P5>::type p5) {
  return MethodFunctor5<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5>(
      method, object, p1, p2, p3, p4, p5);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4, P5) const

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5>
MethodFunctor5<const ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5>
Bind(FP_T(method), const ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4,
     typename detail::identity<P5>::type p5) {
  return MethodFunctor5<const ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5>(
      method, object, p1, p2, p3, p4, p5);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4, P5)

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5>
MethodFunctor5<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5>
Bind(FP_T(method), const scoped_refptr<ObjectT>& object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4,
     typename detail::identity<P5>::type p5) {
  return MethodFunctor5<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5>(
      method, object.get(), p1, p2, p3, p4, p5);
}

#undef FP_T
#define FP_T(x) R (*x)(P1, P2, P3, P4, P5)

template <class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5>
Functor5<FP_T(NONAME), R, P1, P2, P3, P4, P5>
Bind(FP_T(function),
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4,
     typename detail::identity<P5>::type p5) {
  return Functor5<FP_T(NONAME), R, P1, P2, P3, P4, P5>(
      function, p1, p2, p3, p4, p5);
}

#undef FP_T

template <class ObjectT, class MethodT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5,
          class P6>
class MethodFunctor6 {
 public:
  MethodFunctor6(MethodT method, ObjectT* object,
                 P1 p1,
                 P2 p2,
                 P3 p3,
                 P4 p4,
                 P5 p5,
                 P6 p6)
      : method_(method), object_(object),
      p1_(p1),
      p2_(p2),
      p3_(p3),
      p4_(p4),
      p5_(p5),
      p6_(p6) {}
  R operator()() const {
    return (object_->*method_)(p1_, p2_, p3_, p4_, p5_, p6_); }
 private:
  MethodT method_;
  typename detail::PointerType<ObjectT>::type object_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
  typename detail::RemoveScopedPtrRef<P3>::type p3_;
  typename detail::RemoveScopedPtrRef<P4>::type p4_;
  typename detail::RemoveScopedPtrRef<P5>::type p5_;
  typename detail::RemoveScopedPtrRef<P6>::type p6_;
};

template <class FunctorT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5,
          class P6>
class Functor6 {
 public:
  Functor6(const FunctorT& functor, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
      : functor_(functor),
      p1_(p1),
      p2_(p2),
      p3_(p3),
      p4_(p4),
      p5_(p5),
      p6_(p6) {}
  R operator()() const {
    return functor_(p1_, p2_, p3_, p4_, p5_, p6_); }
 private:
  FunctorT functor_;
  typename detail::RemoveScopedPtrRef<P1>::type p1_;
  typename detail::RemoveScopedPtrRef<P2>::type p2_;
  typename detail::RemoveScopedPtrRef<P3>::type p3_;
  typename detail::RemoveScopedPtrRef<P4>::type p4_;
  typename detail::RemoveScopedPtrRef<P5>::type p5_;
  typename detail::RemoveScopedPtrRef<P6>::type p6_;
};


#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4, P5, P6)

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5,
          class P6>
MethodFunctor6<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5, P6>
Bind(FP_T(method), ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4,
     typename detail::identity<P5>::type p5,
     typename detail::identity<P6>::type p6) {
  return MethodFunctor6<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5, P6>(
      method, object, p1, p2, p3, p4, p5, p6);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4, P5, P6) const

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5,
          class P6>
MethodFunctor6<const ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5, P6>
Bind(FP_T(method), const ObjectT* object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4,
     typename detail::identity<P5>::type p5,
     typename detail::identity<P6>::type p6) {
  return MethodFunctor6<const ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5, P6>(
      method, object, p1, p2, p3, p4, p5, p6);
}

#undef FP_T
#define FP_T(x) R (ObjectT::*x)(P1, P2, P3, P4, P5, P6)

template <class ObjectT, class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5,
          class P6>
MethodFunctor6<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5, P6>
Bind(FP_T(method), const scoped_refptr<ObjectT>& object,
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4,
     typename detail::identity<P5>::type p5,
     typename detail::identity<P6>::type p6) {
  return MethodFunctor6<ObjectT, FP_T(NONAME), R, P1, P2, P3, P4, P5, P6>(
      method, object.get(), p1, p2, p3, p4, p5, p6);
}

#undef FP_T
#define FP_T(x) R (*x)(P1, P2, P3, P4, P5, P6)

template <class R,
          class P1,
          class P2,
          class P3,
          class P4,
          class P5,
          class P6>
Functor6<FP_T(NONAME), R, P1, P2, P3, P4, P5, P6>
Bind(FP_T(function),
     typename detail::identity<P1>::type p1,
     typename detail::identity<P2>::type p2,
     typename detail::identity<P3>::type p3,
     typename detail::identity<P4>::type p4,
     typename detail::identity<P5>::type p5,
     typename detail::identity<P6>::type p6) {
  return Functor6<FP_T(NONAME), R, P1, P2, P3, P4, P5, P6>(
      function, p1, p2, p3, p4, p5, p6);
}

#undef FP_T

}  // namespace rtc

#undef NONAME

#endif  // WEBRTC_BASE_BIND_H_
