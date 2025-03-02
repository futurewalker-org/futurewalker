// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Memory.hpp"

#include <boost/signals2.hpp>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Signal template.
///
template <class Signature, template <class> class Combiner>
class Signal;

///
/// @brief Implements weak_ptr interface for boost::signals2::shared_ptr_traits.
///
template <class T>
struct SignalWeakWrapper final
{
    SignalWeakWrapper(Weak<T> weak)
      : _weak {std::move(weak)}
    {
    }

    auto expired() const noexcept -> bool
    {
        return static_cast<bool>(_weak.IsExpired());
    }

    auto lock() const noexcept -> Shared<T>
    {
        return _weak.Lock();
    }

private:
    Weak<T> _weak;
};
}
}

namespace boost::signals2
{
template <class T>
struct shared_ptr_traits<FW_NS::Shared<T>>
{
    using weak_type = FW_NS::SignalWeakWrapper<T>;
};
template <class T>
struct weak_ptr_traits<FW_NS::SignalWeakWrapper<T>>
{
    using shared_type = FW_NS::Shared<T>;
};
}
