/* iostream specialisations for result and outcome
(C) 2017 Niall Douglas <http://www.nedproductions.biz/> (59 commits)
File Created: July 2017


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License in the accompanying file
Licence.txt or at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Distributed under the Boost Software License, Version 1.0.
(See accompanying file Licence.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef OUTCOME_IOSTREAM_SUPPORT_HPP
#define OUTCOME_IOSTREAM_SUPPORT_HPP

#include "outcome.hpp"

#include <iostream>
#include <sstream>

OUTCOME_V2_NAMESPACE_BEGIN

namespace detail
{
  template <class T> inline std::ostream &operator<<(std::ostream &s, const value_storage_trivial<T> &v)
  {
    s << v._status << " ";
    if((v._status & status_have_value) != 0)
    {
      s << v._value;  // NOLINT
    }
    return s;
  }
  inline std::ostream &operator<<(std::ostream &s, const value_storage_trivial<void> &v)
  {
    s << v._status << " ";
    return s;
  }
  template <class T> inline std::ostream &operator<<(std::ostream &s, const value_storage_nontrivial<T> &v)
  {
    s << v._status << " ";
    if((v._status & status_have_value) != 0)
    {
      s << v._value;  // NOLINT
    }
    return s;
  }
  template <class T> inline std::istream &operator>>(std::istream &s, value_storage_trivial<T> &v)
  {
    v = value_storage_trivial<T>();
    s >> v._status;
    if((v._status & status_have_value) != 0)
    {
      new(&v._value) decltype(v._value)();  // NOLINT
      s >> v._value;                        // NOLINT
    }
    return s;
  }
  inline std::istream &operator>>(std::istream &s, value_storage_trivial<devoid<void>> &v)
  {
    v = value_storage_trivial<devoid<void>>();
    s >> v._status;
    return s;
  }
  template <class T> inline std::istream &operator>>(std::istream &s, value_storage_nontrivial<T> &v)
  {
    v = value_storage_nontrivial<T>();
    s >> v._status;
    if((v._status & status_have_value) != 0)
    {
      new(&v._value) decltype(v._value)();  // NOLINT
      s >> v._value;                        // NOLINT
    }
    return s;
  }
  OUTCOME_TEMPLATE(class T)
  OUTCOME_TREQUIRES(OUTCOME_TPRED(!std::is_constructible<std::error_code, T>::value))
  inline std::string safe_message(T && /*unused*/) { return {}; }
  inline std::string safe_message(const std::error_code &ec) { return " (" + ec.message() + ")"; }
}  // namespace detail

/*! Deserialise a result. Format is `status_unsigned [value][error]`. Spare storage is preserved.
\requires That `trait::has_error_code_v<S>` is false.
*/
template <class R, class S, class P> inline std::istream &operator>>(std::istream &s, result<R, S, P> &v)
{
  static_assert(!trait::has_error_code_v<S>, "Cannot call operator>> on a result with an error_code in it");
  s >> v._state;
  if(v.has_error())
  {
    s >> v._error;
  }
  return s;
}
/*! Serialise a result. Format is `status_unsigned [value][error]`. Spare storage is preserved.
If you are printing to a human readable destination, use `print()` instead.
\requires That `trait::has_error_code_v<S>` is false.
*/
template <class R, class S, class P> inline std::ostream &operator<<(std::ostream &s, const result<R, S, P> &v)
{
  static_assert(!trait::has_error_code_v<S>, "Cannot call operator<< on a result with an error_code in it");
  s << v._state;
  if(v.has_error())
  {
    s << v._error;
  }
  return s;
}
/*! Debug print a result into a form suitable for human reading. Format is `value|error`. If the
error type is `error_code`, appends `" (ec.message())"` afterwards.
*/
template <class R, class S, class P> inline std::string print(const detail::result_final<R, S, P> &v)
{
  std::stringstream s;
  if(v.has_value())
  {
    s << v.value();
  }
  if(v.has_error())
  {
    s << v.error() << detail::safe_message(v.error());
  }
  return s.str();
}
/*! Debug print a result into a form suitable for human reading. Format is `(+void)|error`. If the
error type is `error_code`, appends `" (ec.message())"` afterwards.
*/
template <class S, class P> inline std::string print(const detail::result_final<void, S, P> &v)
{
  std::stringstream s;
  if(v.has_value())
  {
    s << "(+void)";
  }
  if(v.has_error())
  {
    s << v.error() << detail::safe_message(v.error());
  }
  return s.str();
}
/*! Debug print a result into a form suitable for human reading. Format is `value|(-void)`.
*/
template <class R, class P> inline std::string print(const detail::result_final<R, void, P> &v)
{
  std::stringstream s;
  if(v.has_value())
  {
    s << v.value();
  }
  if(v.has_error())
  {
    s << "(-void)";
  }
  return s.str();
}
/*! Debug print a result into a form suitable for human reading. Format is `(+void)|(-void)`.
*/
template <class P> inline std::string print(const detail::result_final<void, void, P> &v)
{
  std::stringstream s;
  if(v.has_value())
  {
    s << "(+void)";
  }
  if(v.has_error())
  {
    s << "(-void)";
  }
  return s.str();
}

/*! Deserialise an outcome. Format is `status_unsigned [value][error][exception]`. Spare storage is preserved.
\requires That `trait::has_exception_ptr_v<P>` is false.
*/
template <class R, class S, class P, class N> inline std::istream &operator>>(std::istream &s, outcome<R, S, P, N> &v)
{
  static_assert(!trait::has_exception_ptr_v<P>, "Cannot call operator>> on an outcome with an exception_ptr in it");
  s >> v._state;
  if(v.has_error())
  {
    s >> v._error;
  }
  if(v.has_exception())
  {
    s >> v._ptr;
  }
  return s;
}
/*! Serialise an outcome. Format is `status_unsigned [value][error][exception]`. Spare storage is preserved.
If you are printing to a human readable destination, use `print()` instead.
\requires That `trait::has_exception_ptr_v<P>` is false.
*/
template <class R, class S, class P, class N> inline std::ostream &operator<<(std::ostream &s, const outcome<R, S, P, N> &v)
{
  static_assert(!trait::has_exception_ptr_v<P>, "Cannot call operator<< on an outcome with an exception_ptr in it");
  s << v._state;
  if(v.has_error())
  {
    s << v._error;
  }
  if(v.has_exception())
  {
    s << v._ptr;
  }
  return s;
}
/*! Debug print an outcome into a form suitable for human reading. Format is one of:

1. `value|error|exception`
2. `{ error, exception }`

If the error type is `error_code`, appends `" (ec.message())"` after the error.
Exception type is printed as one of:

1. `std::system_error code code(): what()`
2. `std::exception: what()`
3. `unknown exception`
*/
template <class R, class S, class P, class N> inline std::string print(const outcome<R, S, P, N> &v)
{
  std::stringstream s;
  int total = static_cast<int>(v.has_value()) + static_cast<int>(v.has_error()) + static_cast<int>(v.has_exception());
  if(total > 1)
  {
    s << "{ ";
  }
  s << print(static_cast<const detail::result_final<R, S, N> &>(v));
  if(total > 1)
  {
    s << ", ";
  }
  if(v.has_exception())
  {
#ifdef __cpp_exceptions
    try
    {
      std::rethrow_exception(v.exception());
    }
    catch(const std::system_error &e)
    {
      s << "std::system_error code " << e.code() << ": " << e.what();
    }
    catch(const std::exception &e)
    {
      s << "std::exception: " << e.what();
    }
    catch(...)
#endif
    {
      s << "unknown exception";
    }
  }
  if(total > 1)
  {
    s << " }";
  }
  return s.str();
}
OUTCOME_V2_NAMESPACE_END

#endif