#include <catch2/catch_all.hpp>

#include <Futurewalker.Core.Memory.hpp>

using namespace Futurewalker;

namespace
{
class Base
{
};
class Derived : public Base
{
};
}

TEST_CASE("Unique")
{
    SECTION("MakeUnique")
    {
        auto i = Unique<SInt32>::Make(42);
        REQUIRE(*i == 42);
        *i = 24;
        REQUIRE(*i == 24);
    }

    SECTION("Default")
    {
        const auto i = Unique<SInt32>();
        REQUIRE(!i);
    }

    SECTION("Void instantiation")
    {
        auto i = Unique<SInt32>();
        auto v = Unique<const void>(std::move(i));
    }

    SECTION("Move Construct")
    {
        auto i = Unique<SInt32>::Make(42);

        // Transfer ownership.
        auto j = std::move(i);
        REQUIRE(j);
        REQUIRE(!i);

        // Move into const.
        auto k = Unique<const SInt32>(std::move(j));
        REQUIRE(k);
    }

    SECTION("Move assignment")
    {
        auto i = Unique<SInt32>::Make(42);
        auto j = Unique<SInt32>();

        // Transfer ownership.
        j = std::move(i);
        REQUIRE(j);
        REQUIRE(!i);

        /*
        // Self move assign, should be no-op.
        i = std::move(i);
        REQUIRE(j);
        REQUIRE(*j == 42);
        */
    }

    SECTION("Comparision")
    {
        auto derived = Unique<Derived>::Make();
        auto base = Unique<Base>::Make();

        REQUIRE(base == base);
        REQUIRE(base != nullptr);
        REQUIRE(nullptr != base);
        REQUIRE(derived != base);
        REQUIRE(!(base == derived));
    }
}

TEST_CASE("Shared")
{
    SECTION("Default constructor")
    {
        auto s = Shared<const SInt32>();
        REQUIRE(!s);
        REQUIRE(s.GetUseCount() == 0);
    }

    SECTION("MakeShared")
    {
        auto s = Shared<SInt32>::Make(42);
        REQUIRE(s);
        REQUIRE(s.GetUseCount() == 1);
        REQUIRE(*s == 42);

        *s = 24;
        REQUIRE(*s == 24);
    }

    SECTION("Void instantiation")
    {
        const auto i = Shared<SInt32>();
        auto v = Shared<const void>(i);
    }

    SECTION("Copy constructor")
    {
        auto s1 = Shared<SInt32>::Make(42);
        auto s2 = s1;
        REQUIRE(s2 == s1);
        REQUIRE(s2.GetUseCount() == 2);
    }

    SECTION("Copy assignment")
    {
        auto s1 = Shared<SInt32>::Make(42);
        auto s2 = Shared<SInt32>();
        s2 = s1;
        REQUIRE(s2 == s1);

        /*
        // Self assignment, should be no-op.
        s2 = s2;
        REQUIRE(s2 == s1);
        */
    }

    SECTION("Move constructor")
    {
        auto s1 = Shared<SInt32>::Make(42);
        auto s2 = std::move(s1);
        REQUIRE(!s1);
        REQUIRE(s2);
        REQUIRE(s2.GetUseCount() == 1);
    }

    SECTION("Move assignment")
    {
        auto s1 = Shared<const SInt32>::Make(42);
        auto s2 = std::move(s1);
        REQUIRE(!s1);
        REQUIRE(s2);
        REQUIRE(*s2 == 42);

        /*
        // Self move assignment, should be no-op.
        s2 = std::move(s2);
        REQUIRE(s2);
        REQUIRE(*s2 == 42);
        */
    }

    SECTION("Convert from Unique")
    {
        auto u = Unique<SInt32>::Make(42);

        SECTION("Move constructor")
        {
            auto s = Shared<const SInt32>(std::move(u));
            REQUIRE(!u);
            REQUIRE(s);
        }

        SECTION("Move assignment")
        {
            auto s = Shared<const SInt32>::Make(24);
            s = std::move(u);
            REQUIRE(!u);
            REQUIRE(s);
            REQUIRE(*s == 42);
        }
    }

    SECTION("Comparision")
    {
        auto derived = Shared<Derived>::Make();
        auto base = Shared<Base>(derived);

        REQUIRE(base == base);
        REQUIRE(base != nullptr);
        REQUIRE(nullptr != base);
        REQUIRE(derived == base);
        REQUIRE(base == derived);
    }
}

TEST_CASE("Weak")
{
    SECTION("Default constructor")
    {
        auto w = Weak<SInt32>();
        REQUIRE(w.Lock() == nullptr);
        REQUIRE(w.IsExpired());
    }

    SECTION("Construct from Shared")
    {
        SECTION("Null")
        {
            auto s = Shared<SInt32>();
            auto w = Weak(s);
            REQUIRE((w.Lock() == nullptr));
            REQUIRE(w.IsExpired());
        }

        SECTION("Valid")
        {
            auto s = Shared<SInt32>::Make(42);
            auto w = Weak(s);
            REQUIRE((w.Lock() != nullptr));
            REQUIRE(*w.Lock() == 42);
            REQUIRE(!w.IsExpired());
        }
    }

    SECTION("Upgrade")
    {
    }

    SECTION("Lock")
    {
        auto s = Shared<SInt32>::Make(42);
        auto w = Weak(s);

        REQUIRE(w.Lock());
        s.Reset();
        REQUIRE(!w.Lock());
    }
}

TEST_CASE("Tracked")
{
    SECTION("Default")
    {
        const auto i = Tracked<SInt32>();
        REQUIRE(i.IsExpired());
    }

    auto u = Unique<SInt32>::Make(42);
    auto s = Shared<SInt32>::Make(24);

    SECTION("Copy Construct")
    {
        auto t1 = Tracked(u);
        auto t2 = t1;
    }

    SECTION("Copy Assign")
    {
        auto t = Tracked(u);
        REQUIRE(!t.IsExpired());

        u.Reset();
        REQUIRE(t.IsExpired());

        t = s;
        REQUIRE(!t.IsExpired());

        s.Reset();
        REQUIRE(t.IsExpired());

        t = u;
        REQUIRE(t.IsExpired());
    }

    SECTION("Conversion from Unique")
    {
        Unique<SInt32> i = Unique<SInt32>::Make(42);
        Tracked t = i;

        REQUIRE(!t.IsExpired());

        SECTION("Expired")
        {
            t.Reset();
            REQUIRE(t.IsExpired());
        }

        SECTION("Reset")
        {
            t.Reset();
            REQUIRE(t.IsExpired());
        }
    }

    SECTION("Conversion from Shared")
    {
        Shared<SInt32> i = Shared<SInt32>::Make(42);
        Tracked t = i;

        SECTION("Expired")
        {
            t.Reset();
            REQUIRE(t.IsExpired());
        }

        SECTION("Reset")
        {
            t.Reset();
            REQUIRE(t.IsExpired());
        }
    }

    SECTION("Conversion from Weak")
    {
        Shared<SInt32> i = Shared<SInt32>::Make(42);
        Weak w = i;
        Tracked t = w;

        SECTION("Expired")
        {
            t.Reset();
            REQUIRE(t.IsExpired());
        }

        SECTION("Reset")
        {
            t.Reset();
            REQUIRE(t.IsExpired());
        }
    }
}
