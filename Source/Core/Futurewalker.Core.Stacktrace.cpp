// SPDX-License-Identifier: MIT
#include "Futurewalker.Core.Stacktrace.hpp"

#define _GNU_SOURCE
#include <boost/stacktrace.hpp>

namespace FW_DETAIL_NS
{
class Stacktrace::Impl
{
public:
    Impl(const boost::stacktrace::stacktrace& stacktrace)
      : stacktrace {stacktrace}
    {
    }

    boost::stacktrace::stacktrace stacktrace;
};

///
/// @brief Make stacktrace from current call stack.
///
auto Stacktrace::MakeCurrent() -> Stacktrace
{
    return Stacktrace(std::make_unique<Impl>(boost::stacktrace::stacktrace(1, static_cast<size_t>(-1))));
}

///
/// @brief Default constructor.
///
/// @note Initializes empty stacktrace.
///
Stacktrace::Stacktrace() noexcept = default;

///
/// @brief Destructor.
///
Stacktrace::~Stacktrace() noexcept = default;

///
/// @brief Copy constructor.
///
Stacktrace::Stacktrace(const Stacktrace& other)
  : _impl {other._impl ? std::make_unique<Impl>(*other._impl) : nullptr}
{
}

///
/// @brief Move constructor.
///
Stacktrace::Stacktrace(Stacktrace&& other) noexcept
  : _impl {std::move(other._impl)}
{
}

///
/// @brief Copy assignment operator.
///
auto Stacktrace::operator=(const Stacktrace& other) -> Stacktrace&
{
    _impl = other._impl ? std::make_unique<Impl>(*other._impl) : nullptr;
    return *this;
}

///
/// @brief Move assignment operator.
///
auto Stacktrace::operator=(Stacktrace&& other) noexcept -> Stacktrace&
{
    _impl = std::move(other._impl);
    return *this;
}

///
/// @brief Check if stacktrace is empty.
///
auto Stacktrace::IsEmpty() const -> Bool
{
    return !_impl || _impl->stacktrace.empty();
}

///
/// @brief Get string representation of stacktrace.
///
auto Stacktrace::GetString() const -> std::string
{
    return _impl ? to_string(_impl->stacktrace) : "";
}

///
/// @brief Constructor.
///
Stacktrace::Stacktrace(std::unique_ptr<Impl> impl)
  : _impl {std::move(impl)}
{
}
}
