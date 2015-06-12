/* monad.hpp
The world's most simple C++ monad
(C) 2015 Niall Douglas http://www.nedprod.com/
File Created: June 2015


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef BOOST_SPINLOCK_MONAD_HPP
#define BOOST_SPINLOCK_MONAD_HPP

#include "spinlock.hpp"

// For some odd reason, VS2015 really hates to do much inlining unless forced
#ifdef _MSC_VER
# define BOOST_SPINLOCK_FUTURE_CONSTEXPR BOOST_FORCEINLINE
# define BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR BOOST_FORCEINLINE
# define BOOST_SPINLOCK_FUTURE_MSVC_HELP BOOST_FORCEINLINE
#else
# define BOOST_SPINLOCK_FUTURE_CONSTEXPR BOOST_CONSTEXPR
# define BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR BOOST_CXX14_CONSTEXPR
# define BOOST_SPINLOCK_FUTURE_MSVC_HELP 
#endif

/*! \file monad.hpp
\brief Provides a lightweight simple monadic value transport

\headerfile include/boost/spinlock/monad.hpp ""
*/

BOOST_SPINLOCK_V1_NAMESPACE_BEGIN
namespace lightweight_futures {

//! \brief Enumeration of the ways in which a monad operation may fail
enum class monad_errc {
  already_set = 1,  //!< Attempt to store a value into the monad twice
  no_state = 2      //!< Attempt to use without a state
};

namespace detail
{
  class monad_category : public std::error_category
  {
  public:
    virtual const char *name() const noexcept { return "monad"; }
    virtual std::string message(int c) const
    {
      switch(c)
      {
        case 1: return "already_set";
        case 2: return "no_state";
        default: return "unknown";
      }
    }
  };
}

/*! \brief Returns a reference to a monad error category. Note the address
of one of these may not be constant throughout the process as per the ISO spec.
*/
inline const detail::monad_category &monad_category()
{
  static detail::monad_category c;
  return c;
}

//! \brief A monad exception object
class BOOST_SYMBOL_VISIBLE monad_error : public std::logic_error
{
  std::error_code _ec;
public:
  monad_error(std::error_code ec) : std::logic_error(ec.message()), _ec(std::move(ec)) { }
  const std::error_code &code() const noexcept { return _ec; }
};

inline std::error_code make_error_code(monad_errc e)
{
  return std::error_code(static_cast<int>(e), monad_category());
}

inline std::error_condition make_error_condition(monad_errc e)
{
  return std::error_condition(static_cast<int>(e), monad_category());
}

}
BOOST_SPINLOCK_V1_NAMESPACE_END

namespace std
{
  template<> struct is_error_code_enum<BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad_errc> : std::true_type {};

  template<> struct is_error_condition_enum<BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad_errc> : std::true_type {};

/*  std::error_code make_error_code(BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad_errc e)
  {
    return std::error_code(static_cast<int>(e), BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad_category());
  }

  std::error_condition make_error_condition(BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad_errc e)
  {
    return std::error_condition(static_cast<int>(e), BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad_category());
  }
*/
}

BOOST_SPINLOCK_V1_NAMESPACE_BEGIN
namespace lightweight_futures {
  
template<typename R, class _error_type=std::error_code, class _exception_type=std::exception_ptr> class future;

namespace detail
{
  template<typename R, class _error_type, class _exception_type, class throw_error> struct value_storage
  {
    typedef R value_type;
    typedef _error_type error_type;
    typedef _exception_type exception_type;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4624)
#endif
    union
    {
      value_type value;
      error_type error;             // Often 16 bytes surprisingly
      exception_type exception;     // Typically 8 bytes
      future<value_type> *future_;  // Typically 8 bytes
    };
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    enum class storage_type : unsigned char
    {
      empty,
      value,
      error,
      exception,
      future
    } type;
    
    BOOST_STATIC_CONSTEXPR bool is_nothrow_copy_constructible=std::is_nothrow_copy_constructible<value_type>::value && std::is_nothrow_copy_constructible<exception_type>::value && std::is_nothrow_copy_constructible<error_type>::value;
    BOOST_STATIC_CONSTEXPR bool is_nothrow_move_constructible=std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_move_constructible<exception_type>::value && std::is_nothrow_move_constructible<error_type>::value;
    BOOST_STATIC_CONSTEXPR bool is_nothrow_copy_assignable=std::is_nothrow_copy_assignable<value_type>::value && std::is_nothrow_copy_assignable<exception_type>::value && std::is_nothrow_copy_assignable<error_type>::value;
    BOOST_STATIC_CONSTEXPR bool is_nothrow_move_assignable=std::is_nothrow_move_assignable<value_type>::value && std::is_nothrow_move_assignable<exception_type>::value && std::is_nothrow_move_assignable<error_type>::value;
    BOOST_STATIC_CONSTEXPR bool is_nothrow_destructible=std::is_nothrow_destructible<value_type>::value && std::is_nothrow_destructible<exception_type>::value && std::is_nothrow_destructible<error_type>::value;

#if !defined(__GNUC__) || defined(__clang__)
    /* If enabled GCC pukes during unwrap() with:
       /usr/include/c++/5/type_traits:2204:7: error: static assertion failed: declval() must not be used!
          static_assert(__declval_protector<_Tp>::__stop,
       Apparently it's a known problem in constexpr compilation, and has been for some years now.
    */
    BOOST_SPINLOCK_FUTURE_CONSTEXPR
#endif
    value_storage() noexcept : type(storage_type::empty) { }
    BOOST_SPINLOCK_FUTURE_CONSTEXPR value_storage(const value_type &v) noexcept(std::is_nothrow_copy_constructible<value_type>::value) : value(v), type(storage_type::value) { }
    BOOST_SPINLOCK_FUTURE_CONSTEXPR value_storage(const error_type &v) noexcept(std::is_nothrow_copy_constructible<error_type>::value) : error(v), type(storage_type::error) { }
    BOOST_SPINLOCK_FUTURE_CONSTEXPR value_storage(const exception_type &v) noexcept(std::is_nothrow_copy_constructible<exception_type>::value) : exception(v), type(storage_type::exception) { }
    BOOST_SPINLOCK_FUTURE_CONSTEXPR value_storage(value_type &&v) noexcept(std::is_nothrow_move_constructible<value_type>::value) : value(std::move(v)), type(storage_type::value) { }
    BOOST_SPINLOCK_FUTURE_CONSTEXPR value_storage(error_type &&v) noexcept(std::is_nothrow_move_constructible<error_type>::value) : error(std::move(v)), type(storage_type::error) { }
    BOOST_SPINLOCK_FUTURE_CONSTEXPR value_storage(exception_type &&v) noexcept(std::is_nothrow_move_constructible<exception_type>::value) : exception(std::move(v)), type(storage_type::exception) { }
    struct emplace_t {};
    template<class... Args> BOOST_SPINLOCK_FUTURE_CONSTEXPR explicit value_storage(emplace_t, Args &&... args) noexcept(std::is_nothrow_constructible<value_type, Args...>::value) : value(std::forward<Args>(args)...), type(storage_type::value) { }

    BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR value_storage(const value_storage &o) noexcept(is_nothrow_copy_constructible) : type(storage_type::empty)
    {
      switch(o.type)
      {
        case storage_type::empty:
          break;
        case storage_type::value:
          new (&value) value_type(o.value);
          break;
        case storage_type::error:
          new (&error) error_type(o.error);
          break;
        case storage_type::exception:
          new (&exception) exception_type(o.exception);
          break;
        case storage_type::future:
          abort();
          break;
      }
      type=o.type;
    }
    BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR value_storage(value_storage &&o) noexcept(is_nothrow_move_constructible) : type(storage_type::empty)
    {
      switch(o.type)
      {
        case storage_type::empty:
          break;
        case storage_type::value:
          new (&value) value_type(std::move(o.value));
          break;
        case storage_type::error:
          new (&error) error_type(std::move(o.error));
          break;
        case storage_type::exception:
          new (&exception) exception_type(std::move(o.exception));
          break;
        case storage_type::future:
          future_=o.future_;
          o.future_=nullptr;
          break;
      }
      type=o.type;
    }
    BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR value_storage &operator=(const value_storage &o) noexcept(is_nothrow_destructible && is_nothrow_copy_constructible)
    {
      clear();
      new (this) value_storage(o);
      return *this;
    }
    BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR value_storage &operator=(value_storage &&o) noexcept(is_nothrow_destructible && is_nothrow_move_constructible)
    {
      clear();
      new (this) value_storage(std::move(o));
      return *this;
    }
    BOOST_SPINLOCK_FUTURE_MSVC_HELP ~value_storage() noexcept(is_nothrow_destructible) { clear(); }
    BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR void swap(value_storage &o) noexcept(is_nothrow_move_constructible)
    {
      if(type==o.type)
      {
        switch(type)
        {
          case storage_type::empty:
            break;
          case storage_type::value:
            std::swap(value, o.value);
            break;
          case storage_type::error:
            std::swap(error, o.error);
            break;
          case storage_type::exception:
            std::swap(exception, o.exception);
            break;
          case storage_type::future:
            std::swap(future_, o.future_);
            break;
        }
      }
      else
      {
        value_storage temp(std::move(o));
        o=std::move(*this);
        *this=std::move(temp);        
      }
    }
    BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR void clear() noexcept(is_nothrow_destructible)
    {
      switch(type)
      {
        case storage_type::empty:
          break;
        case storage_type::value:
          value.~value_type();
          type=storage_type::empty;
          break;
        case storage_type::error:
          error.~error_type();
          type=storage_type::empty;
          break;
        case storage_type::exception:
          exception.~exception_type();
          type=storage_type::empty;
          break;
        case storage_type::future:
          future_=nullptr;
          type=storage_type::empty;
          break;
      }
    }
    static void throw_already_set()
    {
      // VS2015 RC errors with a member function redeclaration error, so workaround
      //throw_error(monad_errc::already_set);
      throw_error(static_cast<monad_errc>(static_cast<int>(monad_errc::already_set)));
    }
    template<class U> BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR void set_value(U &&v)
    {
      if(type!=storage_type::empty)
          throw_already_set();
      new (&value) value_type(std::forward<U>(v));
      type=storage_type::value;
    }
    template<class... Args> BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR void emplace_value(Args &&... v)
    {
      if(type!=storage_type::empty)
          throw_already_set();
      new (&value) value_type(std::forward<Args>(v)...);
      type=storage_type::value;
    }
    void set_exception(exception_type e)
    {
      if(type!=storage_type::empty)
          throw_already_set();
      new (&exception) exception_type(std::move(e));
      type=storage_type::exception;
    }
    // Note to self: this can't be BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR
    void set_error(error_type e)
    {
      if(type!=storage_type::empty)
          throw_already_set();
      new (&error) error_type(std::move(e));
      type=storage_type::error;
    }
    // Called by future to take ownership of storage from promise
    BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR void set_future(future<value_type> *f) noexcept(is_nothrow_destructible)
    {
      // Always overwrites existing storage
      clear();
      future_=f;
      type=storage_type::future;
    }
  };

  struct throw_monad_error
  {
    BOOST_SPINLOCK_FUTURE_MSVC_HELP throw_monad_error(monad_errc ec)
    {
      throw monad_error(ec);
    }
  }; 
}

template<typename R, class _error_type=std::error_code, class _exception_type=std::exception_ptr, class throw_error=detail::throw_monad_error> class monad;

namespace detail
{
  template<class M> struct do_unwrap;
  template<class R, class _error_type, class _exception_type, class throw_error> struct do_unwrap<monad<R, _error_type, _exception_type, throw_error>>
  {
    typedef monad<R, _error_type, _exception_type, throw_error> input_type;
    typedef input_type output_type;
    output_type operator()(const input_type &v) const { return v; }
    output_type operator()(input_type &&v) const { return std::move(v); }
  };
  template<class R, class _error_type, class _exception_type, class throw_error1, class throw_error2> struct do_unwrap<monad<monad<R, _error_type, _exception_type, throw_error1>, _error_type, _exception_type, throw_error2>>
  {
    typedef monad<monad<R, _error_type, _exception_type, throw_error1>, _error_type, _exception_type, throw_error2> input_type;
    typedef typename input_type::value_type unwrapped_type;
    typedef typename do_unwrap<unwrapped_type>::output_type output_type;
    output_type operator()(const input_type &v) const
    {
      if(v.has_error())
        return do_unwrap<unwrapped_type>()(v.get_error());
      else if(v.has_exception())
        return do_unwrap<unwrapped_type>()(v.get_exception());
      else if(v.has_value())
        return do_unwrap<unwrapped_type>()(v.get());
      else
        return do_unwrap<unwrapped_type>()(unwrapped_type());
    }
    output_type operator()(input_type &&v) const
    {
      if(v.has_error())
        return do_unwrap<unwrapped_type>()(std::move(v).get_error());
      else if(v.has_exception())
        return do_unwrap<unwrapped_type>()(std::move(v).get_exception());
      else if(v.has_value())
        return do_unwrap<unwrapped_type>()(std::move(v).get());
      else
        return do_unwrap<unwrapped_type>()(unwrapped_type());
    }
  };

  template<class R, class C, class M> struct do_then;
  // For when R is not a monad
  template<class R, class C, class T, class _error_type, class _exception_type, class throw_error> struct do_then<R, C, monad<T, _error_type, _exception_type, throw_error>>
  {
    typedef C callable_type;
    typedef monad<R, _error_type, _exception_type, throw_error> output_type;
    callable_type _c;
    do_then(const callable_type &c) : _c(c) { }
    do_then(callable_type &&c) : _c(std::move(c)) { }
    template<class U> output_type operator()(U &&v) const { return output_type(_c(std::forward<U>(v))); }
  };
  // For when R is a monad
  template<class R, class _error_type1, class _exception_type1, class throw_error1, class C, class T, class _error_type2, class _exception_type2, class throw_error2> struct do_then<monad<R, _error_type1, _exception_type1, throw_error1>, C, monad<T, _error_type2, _exception_type2, throw_error2>>
  {
    typedef C callable_type;
    typedef monad<R, _error_type1, _exception_type1, throw_error1> output_type;
    callable_type _c;
    do_then(const callable_type &c) : _c(c) { }
    do_then(callable_type &&c) : _c(std::move(c)) { }
    template<class U> output_type operator()(U &&v) const { return _c(std::forward<U>(v)); }
  };
}

/*! \class monad
\brief Implements a lightweight simple monadic value transport with the same semantics and API as a future
\tparam R The expected type
\tparam _error_type The type matching the semantics of `std::error_code` to use
\tparam _exception_type The type matching the semantics of `std::exception_ptr` to use
\tparam throw_error A callable with specification void(monad_errc) to call when needing to throw an exception
matching monad_errc

This monad can hold a fixed variant list of empty, a type `R`, a lightweight `error_type` or a
heavier `exception_type` at a space cost of `max(20, sizeof(R)+4)`. Features:

- Very lightweight on build times and run times up to the point of zero execution cost and just a four
byte space overhead. See below for benchmarks. Requires min clang 3.2, GCC 4.7 or VS2015.
- Just enough monad, nothing more, nothing fancy. Replicates the future API, so if you know how to
use a future you already know how to use this.
- Enables convenient all-`noexcept` mathematically verifiable close semantic design, so
why bother with Rust anymore? :)
- Can replace most uses of `optional<T>`.
- Deep integration with lightweight future-promise (i.e. async monadic programming) also in this library.
- Comprehensive unit testing and validation suite.
- Mirrors `noexcept` of type R.
- Type R can have no default constructor, move nor copy.
- Works inside a STL container, and type R can be a STL container.
  - No comparison operations nor hashing is provided, deliberately to keep things simple.

## Notes: ##

Something which might surprise people is that:

\code
monad<std::string> a("niall");
monad<std::string> b(std::move(a));
BOOST_CHECK(a.has_value());  // true
\endcode

Moving a monad does a move of its underlying contents, so any contents remain at whatever
the move constructor for that content leaves things. In other words, a moved from monad
does not become empty, if you want that then call clear().

So long as you avoid the exception_type code paths, this implementation will be
ideally reduced to as few assembler instructions as possible by most recent compilers [1]
which can include exactly zero assembler instructions output. This monad is therefore
identical in terms of execution overhead to using the R type you specify directly - you
get the monadic functionality totally free of execution overhead where the compiler is able
to reduce it to such.

A similar thing applies to error_type which is a lightweight implementation on most
systems. An exception is on VS2015 as the lvalue reference to system_category appears
to be constructed via thread safe once routine called "Immortalize", so when you
construct an error_type on MSVC you'll force a memory synchronisation during the constructor
only. error_types are very cheap to pass around though as they are but an integer and a lvalue ref,
though I see that on GCC and clang 16 bytes is always copied around instead of completely
eliding the copy.

exception_type is also pretty good on anything but MSVC, though never zero assembler
instructions. As soon as an exception_type \em could be created, you'll force out about twenty
instructions most of which won't be executed in practice. Unfortunately, MSVC churns out
about 2000 assembler instructions as soon as you might touch an exception_type, I've raised
this with Microsoft and it looks to be hard for them to fix due to backwards compatibility
reasons.

[1]: GCC 5.1 does a perfect job, VS2015 does a good job, clang 3.7 not so great. See next section.

## Complexity guarantees ##

These x64 opcode guarantees are empirically determined by the unit test suite, and the per-commit
CI testing will fail if they suddenly are exceeded. The maximum is calculated by taking a monad
in from a non-visible source where the compiler has to generate code paths to handle an unknown
input state, whereas the minimum is calculated by setting a monad's state in view of the compiler's
optimiser such that it can usually completely elide opcodes generated (though note that varies
enormously by compiler to the extent that the known code generates more opcodes than the unknown code).

<dl>
 <dt>clang 3.7</dt>
  <dd>59 opcodes <= Value transport <= 37 opcodes<br></dd>
  <dd>7 opcodes <= Error transport <= 52 opcodes<br></dd>
  <dd>38 opcodes <= Exception transport <= 39 opcodes</dd>  
 <dt>GCC 5.1</dt>
  <dd>1 opcodes <= Value transport <= 113 opcodes<br></dd>
  <dd>8 opcodes <= Error transport <= 119 opcodes<br></dd>
  <dd>22 opcodes <= Exception transport <= 214 opcodes</dd>
 <dt>VS2015</dt>
  <dd>4 opcodes <= Value transport <= 1881 opcodes<br></dd>
  <dd>6 opcodes <= Error transport <= 164 opcodes<br></dd>
  <dd>1946 opcodes <= Exception transport <= 1936 opcodes</dd>
</dl>

## Examples ##

\snippet unittests.cpp monad_example

### As an alternative to `optional<T>` ###

Something not so obvious is that this monad can have an empty state, and therefore
can stand in for `optional<T>` like this:

\snippet unittests.cpp optional_example

The API is actually not too distant from `optional<T>`, so with a bit of regex find and replace
you could use `monad<T>` instead.

The need for `monad<T>` to be able to be empty was to make exception throws by T during copy and move
construction lightweight. If that happens, the monad always has empty state afterwards.
*/
template<typename R, class _error_type, class _exception_type, class throw_error> class monad
{
public:
  //! \brief The type potentially held by the monad
  typedef R value_type;
  //! \brief The error code potentially held by the monad
  typedef _error_type error_type;
  //! \brief The exception ptr potentially held by the monad
  typedef _exception_type exception_type;
private:
  typedef detail::value_storage<value_type, error_type, exception_type, throw_error> value_storage_type;
  value_storage_type _storage;
  static void throw_nostate()
  {
    // VS2015 RC errors with a member function redeclaration error, so workaround
    //throw_error(monad_errc::no_state);
    throw_error(static_cast<monad_errc>(static_cast<int>(monad_errc::no_state)));
  }
protected:
  BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(value_storage_type &&s) : _storage(std::move(s)) { }
public:
  //! \brief This monad will never throw exceptions during copy construction
  BOOST_STATIC_CONSTEXPR bool is_nothrow_copy_constructible = value_storage_type::is_nothrow_copy_constructible;
  //! \brief This monad will never throw exceptions during move construction
  BOOST_STATIC_CONSTEXPR bool is_nothrow_move_constructible = value_storage_type::is_nothrow_move_constructible;
  //! \brief This monad will never throw exceptions during copy assignment
  BOOST_STATIC_CONSTEXPR bool is_nothrow_copy_assignable = value_storage_type::is_nothrow_destructible && value_storage_type::is_nothrow_copy_constructible;
  //! \brief This monad will never throw exceptions during move assignment
  BOOST_STATIC_CONSTEXPR bool is_nothrow_move_assignable = value_storage_type::is_nothrow_destructible && value_storage_type::is_nothrow_move_constructible;
  //! \brief This monad will never throw exceptions during destruction
  BOOST_STATIC_CONSTEXPR bool is_nothrow_destructible = value_storage_type::is_nothrow_destructible;

  //! \brief Default constructor, initialises to empty
  monad() = default;
  //! \brief Implicit constructor from a value_type by copy
  BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(const value_type &v) noexcept(std::is_nothrow_copy_constructible<value_type>::value) : _storage(v) { }
  //! \brief Implicit constructor from a value_type by move
  BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(value_type &&v) noexcept(std::is_nothrow_move_constructible<value_type>::value) : _storage(std::move(v)) { }
  //! \brief Implicit constructor of a value_type, also allows emplacement without any other means of construction
  template<class Arg, class... Args, typename = typename std::enable_if<std::is_constructible<value_type, Arg, Args...>::value>::type> BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(Arg &&arg, Args &&... args) noexcept(std::is_nothrow_constructible<value_type, Arg, Args...>::value) : _storage(typename value_storage_type::emplace_t(), std::forward<Arg>(arg), std::forward<Args>(args)...) { }
  //! \brief Implicit constructor from an initializer list
  template<class U> BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(std::initializer_list<U> l) noexcept(std::is_nothrow_constructible<value_type, std::initializer_list<U>>::value) : _storage(typename value_storage_type::emplace_t(), std::move(l)) { }
  //! \brief Implicit constructor from a error_type by copy
  BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(const error_type &v) noexcept(std::is_nothrow_copy_constructible<error_type>::value) : _storage(v) { }
  //! \brief Implicit constructor from a error_type by move
  BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(error_type &&v) noexcept(std::is_nothrow_move_constructible<error_type>::value) : _storage(std::move(v)) { }
  //! \brief Implicit constructor from a exception_type by copy
  BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(const exception_type &v) noexcept(std::is_nothrow_copy_constructible<exception_type>::value) : _storage(v) { }
  //! \brief Implicit constructor from a exception_type by move
  BOOST_SPINLOCK_FUTURE_CONSTEXPR monad(exception_type &&v) noexcept(std::is_nothrow_move_constructible<exception_type>::value) : _storage(std::move(v)) { }
  //! \brief Move constructor
  monad(monad &&) = default;
  //! \brief Move assignment. Firstly clears any existing state, so exception throws during move will leave the monad empty.
  monad &operator=(monad &&) = default;
  //! \brief Copy constructor
  monad(const monad &v) = default;
  //! \brief Copy assignment. Firstly clears any existing state, so exception throws during copy will leave the monad empty.
  monad &operator=(const monad &) = default;

  //! \brief True if monad contains a value_type
  BOOST_SPINLOCK_FUTURE_CONSTEXPR explicit operator bool() const noexcept { return has_value(); }
  //! \brief True if monad is not empty
  BOOST_SPINLOCK_FUTURE_CONSTEXPR bool is_ready() const noexcept
  {
    return _storage.type!=value_storage_type::storage_type::empty;
  }
  //! \brief True if monad is empty
  BOOST_SPINLOCK_FUTURE_CONSTEXPR bool empty() const noexcept
  {
    return _storage.type==value_storage_type::storage_type::empty;
  }
  //! \brief True if monad contains a value_type
  BOOST_SPINLOCK_FUTURE_CONSTEXPR bool has_value() const noexcept
  {
    return _storage.type==value_storage_type::storage_type::value;
  }
  //! \brief True if monad contains an error_type
  BOOST_SPINLOCK_FUTURE_CONSTEXPR bool has_error() const noexcept
  {
    return _storage.type==value_storage_type::storage_type::error;
  }
  //! \brief True if monad contains an exception_type or error_type
  BOOST_SPINLOCK_FUTURE_CONSTEXPR bool has_exception() const noexcept
  {
    return _storage.type==value_storage_type::storage_type::exception || _storage.type==value_storage_type::storage_type::error;
  }

  //! \brief Swaps one monad for another
  BOOST_SPINLOCK_FUTURE_MSVC_HELP void swap(monad &o) noexcept(is_nothrow_move_constructible)
  {
    _storage.swap(o._storage);
  }
  //! \brief Destructs any state stored, resetting to empty
  BOOST_SPINLOCK_FUTURE_MSVC_HELP void clear() noexcept(is_nothrow_destructible)
  {
    _storage.clear();
  }

private:
  BOOST_SPINLOCK_FUTURE_MSVC_HELP void _get_value() const &&
  {
    if(!is_ready())
      throw_nostate();
    if(has_error() || has_exception())
    {
      if(has_error())
        throw std::system_error(_storage.error);
      if(has_exception())
        std::rethrow_exception(_storage.exception);
    }      
  }
public:
  //! \brief If contains a value_type, returns a lvalue reference to it, else throws an exception of future_error(no_state), system_error or the exception_type.
  BOOST_SPINLOCK_FUTURE_MSVC_HELP value_type &get() &
  {
      std::move(*this)._get_value();
      return _storage.value;
  }
  //! \brief If contains a value_type, returns a lvalue reference to it, else throws an exception of future_error(no_state), system_error or the exception_type.
  BOOST_SPINLOCK_FUTURE_MSVC_HELP value_type &value() &
  {
      std::move(*this)._get_value();
      return _storage.value;
  }
  //! \brief If contains a value_type, returns a const lvalue reference to it, else throws an exception of future_error(no_state), system_error or the exception_type.
  BOOST_SPINLOCK_FUTURE_MSVC_HELP const value_type &get() const &
  {
      std::move(*this)._get_value();
      return _storage.value;
  }
  //! \brief If contains a value_type, returns a const lvalue reference to it, else throws an exception of future_error(no_state), system_error or the exception_type.
  BOOST_SPINLOCK_FUTURE_MSVC_HELP const value_type &value() const &
  {
      std::move(*this)._get_value();
      return _storage.value;
  }
  //! \brief If contains a value_type, returns a rvalue reference to it, else throws an exception of future_error(no_state), system_error or the exception_type.
  BOOST_SPINLOCK_FUTURE_MSVC_HELP value_type &&get() &&
  {
      std::move(*this)._get_value();
      return std::move(_storage.value);
  }
  //! \brief If contains a value_type, returns a rvalue reference to it, else throws an exception of future_error(no_state), system_error or the exception_type.
  BOOST_SPINLOCK_FUTURE_MSVC_HELP value_type &&value() &&
  {
      std::move(*this)._get_value();
      return std::move(_storage.value);
  }
  //! \brief If contains a value_type, return that value type, else return the supplied value_type
  BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR value_type &get_or(value_type &v) & noexcept
  {
    return has_value() ? _storage.value : v;
  }
  //! \brief If contains a value_type, return that value type, else return the supplied value_type
  BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR value_type &value_or(value_type &v) & noexcept
  {
    return has_value() ? _storage.value : v;
  }
  //! \brief If contains a value_type, return that value type, else return the supplied value_type
  BOOST_SPINLOCK_FUTURE_CONSTEXPR const value_type &get_or(const value_type &v) const & noexcept
  {
    return has_value() ? _storage.value : v;
  }
  //! \brief If contains a value_type, return that value type, else return the supplied value_type
  BOOST_SPINLOCK_FUTURE_CONSTEXPR const value_type &value_or(const value_type &v) const & noexcept
  {
    return has_value() ? _storage.value : v;
  }
  //! \brief If contains a value_type, return that value type, else return the supplied value_type
  BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR value_type &&get_or(value_type &&v) && noexcept
  {
    return has_value() ? std::move(_storage.value) : std::move(v);
  }
  //! \brief If contains a value_type, return that value type, else return the supplied value_type
  BOOST_SPINLOCK_FUTURE_CXX14_CONSTEXPR value_type &&value_or(value_type &&v) && noexcept
  {
    return has_value() ? std::move(_storage.value) : std::move(v);
  }
  //! \brief Disposes of any existing state, setting the monad to a copy of the value_type
  BOOST_SPINLOCK_FUTURE_MSVC_HELP void set_value(const value_type &v) { _storage.clear(); _storage.set_value(v); }
  //! \brief Disposes of any existing state, setting the monad to a move of the value_type
  BOOST_SPINLOCK_FUTURE_MSVC_HELP void set_value(value_type &&v) { _storage.clear(); _storage.set_value(std::move(v)); }
  //! \brief Disposes of any existing state, setting the monad to an emplaced construction
  template<class... Args> BOOST_SPINLOCK_FUTURE_MSVC_HELP void emplace(Args &&... args) { _storage.clear(); _storage.emplace_value(std::forward<Args>(args)...); }
  
  //! \brief If contains an error_type, returns that error_type, else returns a null error_type. Can only throw the exception future_error(no_state) if empty.
  BOOST_SPINLOCK_FUTURE_MSVC_HELP error_type get_error() const
  {
    if(!is_ready())
      throw_nostate();
    if(!has_error())
      return error_type();
    return _storage.error;
  }
  //! \brief If contains an error_type, returns that error_type else returns the error_type supplied
  BOOST_SPINLOCK_FUTURE_MSVC_HELP error_type get_error_or(error_type e) const noexcept { return has_error() ? _storage.error : std::move(e); }
  //! \brief Disposes of any existing state, setting the monad to the error_type
  BOOST_SPINLOCK_FUTURE_MSVC_HELP void set_error(error_type v) { _storage.clear(); _storage.set_error(std::move(v)); }
  
  //! \brief If contains an exception_type, returns that exception_type. If contains an error_type, returns system_error(error_type). If contains a value_type, returns a null exception_type. Can only throw the exception future_error(no_state) if empty.
  BOOST_SPINLOCK_FUTURE_MSVC_HELP exception_type get_exception() const
  {
    if(!is_ready())
      throw_nostate();
    if(!has_error() && !has_exception())
      return exception_type();
    if(has_error())
      return std::make_exception_ptr(std::system_error(_storage.error));
    if(has_exception())
      return _storage.exception;
    return exception_type();
  }
  //! \brief If contains an exception_type, returns that exception_type else returns the exception_type supplied
  BOOST_SPINLOCK_FUTURE_MSVC_HELP exception_type get_exception_or(exception_type e) const noexcept { return has_exception() ? _storage.exception : std::move(e); }
  //! \brief Disposes of any existing state, setting the monad to the exception_type
  BOOST_SPINLOCK_FUTURE_MSVC_HELP void set_exception(exception_type v) { _storage.clear(); _storage.set_exception(std::move(v)); }
  //! \brief Disposes of any existing state, setting the monad to make_exception_type(forward<E>(e))
  template<typename E> BOOST_SPINLOCK_FUTURE_MSVC_HELP void set_exception(E &&e)
  {
    set_exception(make_exception_type(std::forward<E>(e)));
  }

  //! \brief If I am a monad<monad<...>>, return copy of most nested monad<...>, else return copy of *this
#ifdef DOXYGEN_IS_IN_THE_HOUSE
  monad<...> unwrap() const &;
#else
  typename detail::do_unwrap<monad>::output_type unwrap() const & { return detail::do_unwrap<monad>()(*this); }
#endif
  //! \brief If I am a monad<monad<...>>, return move of most nested monad<...>, else return move of *this
#ifdef DOXYGEN_IS_IN_THE_HOUSE
  monad<...> unwrap() &&;
#else
  typename detail::do_unwrap<monad>::output_type unwrap() && { return detail::do_unwrap<monad>()(std::move(*this)); }
#endif

  /*! \brief Return monad(F(*this)) or F(*this) if the latter returns a monad.
  
  The callable F needs to consume a monad obviously enough, however the callable is called with a monad &&
  so if your callable takes a monad &&, you can move from the monad. Equally, you can avoid copies if your
  callable takes a reference argument. The callable F can be a generic lambda if desired.
  
  If your callable does not return a monad, a monad will be constructed to hold the type it does return
  inheriting the same error_code, exception_type etc of the originating monad. If your callable returns
  a monad, that monad can be of any template parameter configuration and it will be returned from then(). This
  allows a very easy way of converting between different configurations of monad cost free.
  */
#ifdef DOXYGEN_IS_IN_THE_HOUSE
  template<class F> monad(F(*this)).unwrap() then(F &&f);
#else
  template<class F> typename detail::do_then<typename std::result_of<F(monad)>::type, F, monad>::output_type then(F &&f)
  {
    return detail::do_then<typename std::result_of<F(monad)>::type, F, monad>(std::forward<F>(f))(std::move(*this));
  }
#endif
  
  //! \brief If bool(*this), return monad(F(get())).unwrap(), else return monad<result_of<F(get())>>(error)
#ifdef DOXYGEN_IS_IN_THE_HOUSE
  template<class F> monad(F(get())).unwrap() bind(F &&f);
#else
  template<class F> typename detail::do_then<typename std::result_of<F(value_type)>::type, F, monad>::output_type bind(F &&f)
  {
    typedef typename detail::do_then<typename std::result_of<F(value_type)>::type, F, monad>::output_type type;
    if(has_value())
      return detail::do_then<typename std::result_of<F(value_type)>::type, F, monad>(std::forward<F>(f))(std::move(_storage.value));
    else if(has_error())
      return type(_storage.error);
    else if(has_exception())
      return type(_storage.exception);
    else
      return type();
  }
#endif
  
  //! \brief If bool(*this), return monad(F(get())), else return monad<result_of<F(get())>>(error)
#ifdef DOXYGEN_IS_IN_THE_HOUSE
  template<class F> monad(F(get())) map(F &&f);
#else
  template<class F> monad<typename detail::do_then<typename std::result_of<F(value_type)>::type, F, monad>::output_type> map(F &&f);
#endif
};

// TODO FIXME monad<void> specialisation

}
BOOST_SPINLOCK_V1_NAMESPACE_END

namespace std
{
  template<typename value_type, class error_type, class exception_type> inline void swap(BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad<value_type, error_type, exception_type> &a, BOOST_SPINLOCK_V1_NAMESPACE::lightweight_futures::monad<value_type, error_type, exception_type> &b)
  {
    a.swap(b);
  }
}

#endif
