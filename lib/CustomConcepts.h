#pragma once

template <typename T>
concept CopyConstructible = requires (const T& t) {T(t);};

template <typename Functor>
concept FunctorWithZeroArgs = requires (const Functor& functor) { {functor()} -> CopyConstructible;};

template <typename Functor, typename U>
concept FunctorWithOneArg = requires (const Functor& functor, const U& u) { {functor(u)} -> CopyConstructible;};

template <typename Functor, typename U, typename V>
concept FunctorWithTwoArgs = requires (const Functor& functor, const U& u, const V& v) { {functor(u, v)} -> CopyConstructible;};