// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.LocatorType.hpp"

#include "Futurewalker.Core.TypeId.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.HashMap.hpp"

#include <mutex>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Locator class.
///
class Locator final : NonCopyable
{
public:
    template <class Implementation, class... Args>
    static auto Register(Args&&... args) -> Bool;

    template <class Interface>
    static auto Unregister() -> void;

    template <class Interface>
    static auto IsRegistered() -> Bool;

    template <class Interface>
    static auto Resolve() -> Shared<Interface>;

    template <class Interface, class Implementation = Interface, class... Args>
    static auto ResolveWithDefault(Args&&... args) -> Shared<Interface>;

    template <class Interface>
    static auto GetInstance() -> Shared<Interface>;

public:
    ///
    /// @brief Resolver template.
    ///
    /// @tparam Implementation
    ///
    template <class Implementation>
    struct Resolver
    {
    };

private:
    template <class Implementation, class... Args>
    auto InternalRegister(Args&&... args) -> Bool;
    template <class Interface>
    auto InternalUnregister() -> void;
    template <class Interface>
    auto InternalIsRegistered() -> Bool;
    template <class Interface>
    auto InternalResolve() -> Shared<Interface>;
    template <class Interface>
    auto InternalGetInstance() -> Shared<Interface>;

private:
    Locator() = default;

    static auto GetSelf() -> Locator&;

    auto AddInstance(Id const& id, Shared<void> const& instance) -> void;
    auto CreateInstance(Id const& id) -> Shared<void>;
    auto FindInstance(Id const& id) const -> Shared<void>;

private:
    mutable std::recursive_mutex _mutex;
    HashMap<Id, Function<Shared<void>()>> _factoryMap;
    HashMap<Id, Weak<void>> _instanceMap;
};

///
/// @brief
///
/// @tparam Interface
/// @tparam ...Args
/// @tparam Implementation
///
/// @param ...args
///
/// @return Returns true when context was successfully registered.
///
/// @note This function will not overwrite existing registration.
///
template <class Implementation, class... Args>
auto Locator::Register(Args&&... args) -> Bool
{
    return GetSelf().InternalRegister<Implementation>(std::forward<Args>(args)...);
}

///
/// @brief Unregister interface factory.
///
/// @tparam Interface Interface to unregister.
///
template <class Interface>
auto Locator::Unregister() -> void
{
    return GetSelf().InternalUnregister<Interface>();
}

///
/// @brief Is there implementation registered for given interface?
///
/// @tparam Interface Interface type to query.
///
template <class Interface>
auto Locator::IsRegistered() -> Bool
{
    GetSelf().InternalIsRegistered<Interface>();
}

///
/// @brief Resolve dependencies and create instance.
///
/// @tparam Interface Interface type to query.
///
/// @note Locator will not hold strong reference to the instance.
///
/// @return Non-null shared instance of Interface.
///
/// @throw Exception on failure.
///
template <class Interface>
auto Locator::Resolve() -> Shared<Interface>
{
    return GetSelf().InternalResolve<Interface>();
}

///
/// @brief Resolve with default implementation.
///
/// @tparam Interface Interface type to query.
/// @tparam Implementation Implementation type.
/// @tparam Args Argument types.
///
/// @param args Arguments for Implementation::Resolve().
///
/// @note Locator will not hold strong reference to the instance.
///
/// @return Non-null shared instance of Interface.
///
/// @throw Exception on failure.
///
template <class Interface, class Implementation, class... Args>
auto Locator::ResolveWithDefault(Args&&... args) -> Shared<Interface>
{
    static_assert(Concepts::SameAs<Interface, typename Resolver<Implementation>::Interface>, "Implementation does not match to interface");
    Register<Implementation>(std::forward<Args>(args)...);
    return Resolve<Interface>();
}

///
/// @brief Get shared singleton instance.
///
/// @tparam Interface Interface type to query.
///
/// @note Unlike Resolve(), this function will not create new instance.
///
template <class Interface>
auto Locator::GetInstance() -> Shared<Interface>
{
    return GetSelf().InternalGetInstance<Interface>();
}

///
/// @brief Register interface.
///
template <class Implementation, class... Args>
auto Locator::InternalRegister(Args&&... args) -> Bool
{
    using Interface = typename Resolver<Implementation>::Interface;
    auto resolver = [=]() -> Shared<void> {
        auto const instance = Shared<Interface>(Resolver<Implementation>::Resolve(args...));
        auto const id = static_cast<Id>(TypeId::Get<Interface>());
        GetSelf().AddInstance(id, instance);
        return instance;
    };
    auto const lock = std::lock_guard(_mutex);
    auto const result = _factoryMap.try_emplace(static_cast<Id>(TypeId::Get<Interface>()), std::move(resolver));
    return result.second;
}

///
/// @brief Register interface.
///
template <class Interface>
auto Locator::InternalUnregister() -> void
{
    auto const lock = std::lock_guard(_mutex);
    _factoryMap.erase(static_cast<Id>(TypeId::Get<Interface>()));
}

///
/// @brief Registered?
///
template <class Interface>
auto Locator::InternalIsRegistered() -> Bool
{
    auto const id = TypeId::Get<Interface>();
    auto const lock = std::lock_guard(_mutex);
    return _factoryMap.contains(id);
}

///
/// @brief Resolve dependencies.
///
template <class Interface>
auto Locator::InternalResolve() -> Shared<Interface>
{
    auto const lock = std::lock_guard(_mutex);
    auto const id = TypeId::Get<Interface>();
    if (auto instance = FindInstance(static_cast<Id>(id)))
    {
        return instance.Assume<Interface>();
    }
    if (auto instance = CreateInstance(static_cast<Id>(id)))
    {
        return instance.Assume<Interface>();
    }
    throw Exception(ErrorCode::Failure, "Failed to resolve dependency");
}

///
/// @brief Get instance.
///
template <class Interface>
auto Locator::InternalGetInstance() -> Shared<Interface>
{
    auto const lock = std::lock_guard(_mutex);
    auto const id = TypeId::Get<Interface>();
    if (auto instance = FindInstance(static_cast<Id>(id)))
    {
        return instance.Assume<Interface>();
    }
    return nullptr;
}
}
}
