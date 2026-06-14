#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.SharedArray.hpp>

#include <cstdint>

// SharedArray lives in the detail namespace (it is not re-exported); reach it via FW_DETAIL_NS.
using namespace Futurewalker;
using namespace FW_DETAIL_NS;

namespace
{
///
/// @brief Element type that counts live instances to catch leaks / double-frees,
///        and is non-trivial (manual lifetime must be correct).
///
struct Counter
{
    static inline int alive = 0;
    int value = 0;

    Counter() noexcept
    {
        ++alive;
    }
    Counter(int v) noexcept
      : value {v}
    {
        ++alive;
    }
    Counter(Counter const& o) noexcept
      : value {o.value}
    {
        ++alive;
    }
    Counter(Counter&& o) noexcept
      : value {o.value}
    {
        ++alive;
    }
    Counter& operator=(Counter const&) noexcept = default;
    Counter& operator=(Counter&&) noexcept = default;
    ~Counter()
    {
        --alive;
    }
};
}

TEST_CASE("SharedArray")
{
    SECTION("Default is empty")
    {
        SharedArray<SInt32> a;
        REQUIRE(a.IsEmpty());
        REQUIRE(a.GetSize() == SInt64(0));
        REQUIRE(a.GetValues().empty());
    }

    SECTION("PushBack and read")
    {
        SharedArray<SInt32> a;
        a.PushBack(SInt32(1));
        a.PushBack(SInt32(2));
        a.PushBack(SInt32(3));

        REQUIRE(!a.IsEmpty());
        REQUIRE(a.GetSize() == SInt64(3));
        REQUIRE(a.GetValueAt(0) == SInt32(1));
        REQUIRE(a.GetValueAt(2) == SInt32(3));

        auto const values = a.GetValues();
        REQUIRE(values.size() == 3);
        REQUIRE(values[1] == SInt32(2));
    }

    SECTION("PushBack returns reference into storage")
    {
        SharedArray<SInt32> a;
        auto& ref = a.PushBack(SInt32(10));
        ref = SInt32(42);
        REQUIRE(a.GetValueAt(0) == SInt32(42));
    }

    SECTION("GetValueAt out of range throws")
    {
        SharedArray<SInt32> a;
        REQUIRE_THROWS(a.GetValueAt(0));
        a.PushBack(SInt32(1));
        REQUIRE_THROWS(a.GetValueAt(1));
        REQUIRE_THROWS(a.GetValueAt(-1));
    }

    SECTION("Copy-on-write: mutating a copy leaves the original untouched")
    {
        SharedArray<SInt32> a;
        a.PushBack(SInt32(1));
        a.PushBack(SInt32(2));

        SharedArray<SInt32> b = a; // shares storage
        b.PushBack(SInt32(3));     // detaches

        REQUIRE(a.GetSize() == SInt64(2));
        REQUIRE(b.GetSize() == SInt64(3));
        REQUIRE(a.GetValueAt(1) == SInt32(2));
        REQUIRE(b.GetValueAt(2) == SInt32(3));
    }

    SECTION("Copy aliases storage until mutation")
    {
        SharedArray<SInt32> a;
        a.PushBack(SInt32(7));
        SharedArray<SInt32> b = a;
        REQUIRE(a.GetValues().data() == b.GetValues().data());
    }

    SECTION("Erase shifts remaining elements")
    {
        SharedArray<SInt32> a;
        for (auto i = 0; i < 4; ++i)
        {
            a.PushBack(SInt32(i));
        }
        a.Erase(1);
        REQUIRE(a.GetSize() == SInt64(3));
        REQUIRE(a.GetValueAt(0) == SInt32(0));
        REQUIRE(a.GetValueAt(1) == SInt32(2));
        REQUIRE(a.GetValueAt(2) == SInt32(3));

        a.Erase(10); // out of range: no-op
        REQUIRE(a.GetSize() == SInt64(3));
    }

    SECTION("EraseUnordered swaps with last")
    {
        SharedArray<SInt32> a;
        for (auto i = 0; i < 4; ++i)
        {
            a.PushBack(SInt32(i));
        }
        a.EraseUnordered(0);
        REQUIRE(a.GetSize() == SInt64(3));
        REQUIRE(a.GetValueAt(0) == SInt32(3));
    }

    SECTION("EraseIf removes matching elements")
    {
        SharedArray<SInt32> a;
        for (auto i = 0; i < 6; ++i)
        {
            a.PushBack(SInt32(i));
        }
        a.EraseIf([](auto const& v) { return static_cast<std::int64_t>(v) % 2 == 0; });
        REQUIRE(a.GetSize() == SInt64(3));
        REQUIRE(a.GetValueAt(0) == SInt32(1));
        REQUIRE(a.GetValueAt(1) == SInt32(3));
        REQUIRE(a.GetValueAt(2) == SInt32(5));
    }

    SECTION("Clear empties a uniquely owned array but keeps it usable")
    {
        SharedArray<SInt32> a;
        a.PushBack(SInt32(1));
        a.Clear();
        REQUIRE(a.IsEmpty());
        a.PushBack(SInt32(2));
        REQUIRE(a.GetValueAt(0) == SInt32(2));
    }

    SECTION("Clear of a shared array does not affect the other owner")
    {
        SharedArray<SInt32> a;
        a.PushBack(SInt32(1));
        SharedArray<SInt32> b = a;
        b.Clear();
        REQUIRE(b.IsEmpty());
        REQUIRE(a.GetSize() == SInt64(1));
    }

    SECTION("Move leaves the source empty")
    {
        SharedArray<SInt32> a;
        a.PushBack(SInt32(1));
        SharedArray<SInt32> b = std::move(a);
        REQUIRE(b.GetSize() == SInt64(1));
        REQUIRE(a.IsEmpty());
    }

    SECTION("Reserve does not change size")
    {
        SharedArray<SInt32> a;
        a.Reserve(SInt64(16));
        REQUIRE(a.IsEmpty());
        auto const* before = a.GetValues().data();
        for (auto i = 0; i < 16; ++i)
        {
            a.PushBack(SInt32(i));
        }
        // No reallocation should have happened within reserved capacity.
        REQUIRE(a.GetValues().data() == before);
        REQUIRE(a.GetSize() == SInt64(16));
    }
}

namespace
{
///
/// @brief Element whose move constructor poisons the moved-from source.
///
struct MovePoison
{
    int value = 0;

    MovePoison(int v) noexcept
      : value {v}
    {
    }
    MovePoison(MovePoison const& o) noexcept
      : value {o.value}
    {
    }
    MovePoison(MovePoison&& o) noexcept
      : value {o.value}
    {
        o.value = -1; // poison the source on move
    }
    MovePoison& operator=(MovePoison const&) noexcept = default;
    MovePoison& operator=(MovePoison&&) noexcept = default;
    ~MovePoison() = default;
};
}

TEST_CASE("SharedArray PushBack is self-aliasing safe")
{
    SECTION("PushBack(GetValueAt(0)) while full triggers a growth reallocation")
    {
        SharedArray<MovePoison> a;
        a.PushBack(MovePoison(100));

        // A single element fills the smallest buffer, so the next PushBack must grow.
        REQUIRE(a.GetSize() == a.GetCapacity());

        a.PushBack(a.GetValueAt(0));

        REQUIRE(a.GetSize() == SInt64(2));
        REQUIRE(a.GetValueAt(0).value == 100); // survivor of the move
        REQUIRE(a.GetValueAt(1).value == 100); // must be the *original* value, not poison
    }

    SECTION("Self-alias of a later element while full")
    {
        SharedArray<MovePoison> a;
        a.PushBack(MovePoison(1));
        a.PushBack(MovePoison(2)); // capacity now 2, size 2 -> full

        REQUIRE(a.GetSize() == a.GetCapacity());

        a.PushBack(a.GetValueAt(1)); // reference into soon-to-be-reallocated storage

        REQUIRE(a.GetSize() == SInt64(3));
        REQUIRE(a.GetValueAt(2).value == 2);
    }
}

TEST_CASE("SharedArray growth always makes progress")
{
    SharedArray<SInt32> a;

    auto previousCapacity = a.GetCapacity();
    for (auto i = 0; i < 1000; ++i)
    {
        a.PushBack(SInt32(i));

        auto const capacity = a.GetCapacity();
        REQUIRE(capacity >= a.GetSize()); // capacity always covers size
        REQUIRE(capacity >= SInt64(1));   // a non-empty array is never zero-capacity
        REQUIRE(capacity >= previousCapacity);
        previousCapacity = capacity;
    }

    REQUIRE(a.GetSize() == SInt64(1000));
    for (auto i = 0; i < 1000; ++i)
    {
        REQUIRE(a.GetValueAt(i) == SInt32(i));
    }
}

TEST_CASE("SharedArray element lifetimes")
{
    SECTION("All elements are destroyed (no leaks across COW and growth)")
    {
        REQUIRE(Counter::alive == 0);
        {
            SharedArray<Counter> a;
            for (auto i = 0; i < 10; ++i)
            {
                a.PushBack(Counter(i)); // exercises geometric growth
            }
            SharedArray<Counter> b = a;
            b.PushBack(Counter(99)); // COW copy
            b.Erase(0);
            a.EraseUnordered(1);
            REQUIRE(Counter::alive > 0);
        }
        REQUIRE(Counter::alive == 0);
    }
}

#pragma warning(push)
#pragma warning(disable:4324)
TEST_CASE("SharedArray over-aligned element")
{
    struct alignas(64) Over
    {
        int value = 0;
    };

    SharedArray<Over> a;
    a.PushBack(Over {1});
    a.PushBack(Over {2});
    REQUIRE(a.GetSize() == SInt64(2));
    REQUIRE(reinterpret_cast<std::uintptr_t>(a.GetValues().data()) % 64 == 0);
    REQUIRE(a.GetValueAt(1).value == 2);
}
#pragma warning(pop)
