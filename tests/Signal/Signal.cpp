#include <catch2/catch_all.hpp>

#include <Futurewalker.Signal.hpp>

#include <string>
#include <vector>

using namespace Futurewalker;

namespace
{
///
/// @brief Counts copy/move operations to verify how arguments flow through emission.
///
struct CopyCounter
{
    static inline int copies = 0;
    static inline int moves = 0;

    static auto Reset() -> void
    {
        copies = 0;
        moves = 0;
    }

    CopyCounter() = default;
    CopyCounter(CopyCounter const&) { ++copies; }
    CopyCounter(CopyCounter&&) noexcept { ++moves; }
    auto operator=(CopyCounter const&) -> CopyCounter& { ++copies; return *this; }
    auto operator=(CopyCounter&&) noexcept -> CopyCounter& { ++moves; return *this; }
};

///
/// @brief Trackable target for automatic-tracking tests.
///
/// The tracker is backed by an explicit control object so it can be expired
/// (via Kill()) independently of the object's own lifetime.
///
struct Trackable
{
    Shared<int> life = Shared<int>::Make(0);
    int calls = 0;

    auto GetTracker() const -> Weak<void const> { return life; }

    auto OnSignal() -> void { ++calls; }
    auto Add(int n) -> void { calls += n; }

    auto Kill() -> void { life.Reset(); }
};
}

TEST_CASE("Signal lifecycle")
{
    SECTION("Empty signal emits nothing")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        REQUIRE(sig.IsEmpty());
        REQUIRE(sig.GetSlotCount() == 0);
        sig();
        REQUIRE(sig.IsEmpty());
    }

    SECTION("Connect updates count and emptiness")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        sig.Connect([] {});
        REQUIRE(!sig.IsEmpty());
        REQUIRE(sig.GetSlotCount() == 1);
        sig.Connect([] {});
        sig.Connect([] {});
        REQUIRE(sig.GetSlotCount() == 3);
    }

    SECTION("Same callable connected twice creates two slots")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto f = [] {};
        sig.Connect(f);
        sig.Connect(f);
        REQUIRE(sig.GetSlotCount() == 2);
    }

    SECTION("DisconnectAll clears every slot")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        sig.Connect([] {});
        sig.Connect([] {});
        sig.DisconnectAll();
        REQUIRE(sig.IsEmpty());
        REQUIRE(sig.GetSlotCount() == 0);
        sig();
    }
}

TEST_CASE("Signal emission")
{
    SECTION("Slots fire in connection order")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto order = std::vector<int>();
        sig.Connect([&] { order.push_back(1); });
        sig.Connect([&] { order.push_back(2); });
        sig.Connect([&] { order.push_back(3); });
        sig();
        REQUIRE(order == std::vector<int> {1, 2, 3});
    }

    SECTION("Every slot fires on each emission")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto count = 0;
        sig.Connect([&] { ++count; });
        sig.Connect([&] { ++count; });
        sig();
        sig();
        REQUIRE(count == 4);
    }
}

TEST_CASE("SignalConnection")
{
    SECTION("Disconnect stops the slot and updates the count")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto count = 0;
        auto c = sig.Connect([&] { ++count; });
        REQUIRE(c.IsConnected());
        sig();
        REQUIRE(count == 1);

        c.Disconnect();
        REQUIRE(!c.IsConnected());
        REQUIRE(sig.GetSlotCount() == 0);
        sig();
        REQUIRE(count == 1);
    }

    SECTION("A copied handle controls the same slot")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto c1 = sig.Connect([] {});
        auto c2 = c1;
        REQUIRE(c1.IsConnected());
        REQUIRE(c2.IsConnected());

        c2.Disconnect();
        REQUIRE(!c1.IsConnected());
        REQUIRE(!c2.IsConnected());
    }

    SECTION("A default-constructed handle is inert")
    {
        auto c = SignalConnection();
        REQUIRE(!c.IsConnected());
        c.Disconnect();
        REQUIRE(!c.IsConnected());
    }

    SECTION("A handle that outlives its signal is safe")
    {
        auto c = SignalConnection();
        {
            auto sig = Signal<void(), SignalCombinerVoid>();
            c = sig.Connect([] {});
            REQUIRE(c.IsConnected());
        }
        REQUIRE(!c.IsConnected());
        c.Disconnect();
    }

    SECTION("Swap")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto a = sig.Connect([] {});
        auto b = SignalConnection();
        a.Swap(b);
        REQUIRE(!a.IsConnected());
        REQUIRE(b.IsConnected());
    }
}

TEST_CASE("ScopedSignalConnection")
{
    SECTION("Disconnects on destruction")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        {
            auto sc = sig.ConnectScoped([] {});
            REQUIRE(sc.IsConnected());
            REQUIRE(sig.GetSlotCount() == 1);
        }
        REQUIRE(sig.GetSlotCount() == 0);
    }

    SECTION("Move transfers ownership without disconnecting")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        {
            auto outer = ScopedSignalConnection();
            {
                auto sc = sig.ConnectScoped([] {});
                outer = std::move(sc);
            }
            // sc was moved-from, so leaving its scope must not disconnect.
            REQUIRE(sig.GetSlotCount() == 1);
            REQUIRE(outer.IsConnected());
        }
        REQUIRE(sig.GetSlotCount() == 0);
    }

    SECTION("Move-assignment disconnects the previously held connection")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto sc = sig.ConnectScoped([] {});
        REQUIRE(sig.GetSlotCount() == 1);

        sc = sig.ConnectScoped([] {});
        // The first connection must be dropped, not leaked.
        REQUIRE(sig.GetSlotCount() == 1);
    }

    SECTION("Construct from a SignalConnection")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        {
            auto sc = ScopedSignalConnection(sig.Connect([] {}));
            REQUIRE(sig.GetSlotCount() == 1);
        }
        REQUIRE(sig.GetSlotCount() == 0);
    }
}

TEST_CASE("Signal reentrancy")
{
    SECTION("A slot connected during emission fires only on the next emission")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto outer = 0;
        auto inner = 0;
        sig.Connect([&] {
            ++outer;
            if (outer == 1)
            {
                sig.Connect([&] { ++inner; });
            }
        });

        sig();
        REQUIRE(outer == 1);
        REQUIRE(inner == 0);

        sig();
        REQUIRE(outer == 2);
        REQUIRE(inner == 1);
    }

    SECTION("Disconnecting a not-yet-fired slot during emission skips it")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto a = 0;
        auto b = 0;
        auto cb = SignalConnection();
        sig.Connect([&] { ++a; cb.Disconnect(); });
        cb = sig.Connect([&] { ++b; });

        sig();
        REQUIRE(a == 1);
        REQUIRE(b == 0);
    }

    SECTION("A slot disconnecting itself during emission is safe")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto count = 0;
        auto self = SignalConnection();
        self = sig.Connect([&] { ++count; self.Disconnect(); });

        sig();
        sig();
        REQUIRE(count == 1);
    }

    SECTION("Recursive emission balances depth and reclaims afterward")
    {
        auto sig = Signal<void(int), SignalCombinerVoid>();
        auto count = 0;
        sig.Connect([&](int depth) {
            ++count;
            if (depth > 0)
            {
                sig(depth - 1);
            }
        });

        sig(3);
        REQUIRE(count == 4);
    }

    SECTION("DisconnectAll during emission does not stop in-flight slots")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto a = 0;
        auto b = 0;
        sig.Connect([&] { ++a; sig.DisconnectAll(); });
        sig.Connect([&] { ++b; });

        sig();
        // The second slot was captured in the emission snapshot, so it still fires
        // even though the first slot cleared the signal.
        REQUIRE(a == 1);
        REQUIRE(b == 1);
        REQUIRE(sig.IsEmpty());

        sig();
        REQUIRE(a == 1);
        REQUIRE(b == 1);
    }
}

TEST_CASE("Signal automatic tracking")
{
    SECTION("Tracked slot fires while the tracker is alive")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto t = Trackable();
        sig.Connect(t.GetTracker(), t, &Trackable::OnSignal);

        sig();
        REQUIRE(t.calls == 1);
    }

    SECTION("Tracked slot stops firing once the tracker expires")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto t = Trackable();
        sig.Connect(t.GetTracker(), t, &Trackable::OnSignal);
        sig();
        REQUIRE(t.calls == 1);

        t.Kill();
        sig();
        REQUIRE(t.calls == 1);
    }

    SECTION("An expired tracker makes the slot uncounted and reclaimable")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto t = Trackable();
        auto c = sig.Connect(t.GetTracker(), t, &Trackable::OnSignal);
        REQUIRE(sig.GetSlotCount() == 1);
        REQUIRE(c.IsConnected());

        t.Kill();

        // An expired tracker reports the slot as disconnected...
        REQUIRE(sig.GetSlotCount() == 0);
        REQUIRE(sig.IsEmpty());
        REQUIRE(!c.IsConnected());

        // ...and the dead slot is reclaimed by the next emission.
        sig();
        REQUIRE(t.calls == 0);
        REQUIRE(sig.GetSlotCount() == 0);
    }
}

TEST_CASE("SignalFunction")
{
    SECTION("Plain connect")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto calls = 0;
        SignalFunction::Connect(sig, [&] { ++calls; });
        sig();
        REQUIRE(calls == 1);
    }

    SECTION("Member-style tracked connect")
    {
        auto sig = Signal<void(int), SignalCombinerVoid>();
        auto t = Trackable();
        SignalFunction::Connect(sig, t, &Trackable::Add);

        sig(5);
        REQUIRE(t.calls == 5);

        t.Kill();
        sig(5);
        REQUIRE(t.calls == 5);
    }

    SECTION("Free-style tracked connect")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto t = Trackable();
        auto calls = 0;
        // The slot does not take the tracked object; it is invoked as f(args...),
        // but its lifetime is still bound to t's tracker.
        SignalFunction::Connect(sig, t, [&] { ++calls; });

        sig();
        REQUIRE(calls == 1);

        t.Kill();
        sig();
        REQUIRE(calls == 1);
    }

    SECTION("Free-style tracked connect with arguments")
    {
        auto sig = Signal<void(int), SignalCombinerVoid>();
        auto t = Trackable();
        auto sum = 0;
        SignalFunction::Connect(sig, t, [&](int n) { sum += n; });

        sig(3);
        REQUIRE(sum == 3);

        t.Kill();
        sig(4);
        REQUIRE(sum == 3);
    }
}

TEST_CASE("Signal combiners")
{
    SECTION("AnyOf is false when empty")
    {
        auto sig = Signal<Bool(), SignalCombinerAnyOf>();
        REQUIRE(!sig());
    }

    SECTION("AnyOf is true if any slot returns true")
    {
        auto sig = Signal<Bool(), SignalCombinerAnyOf>();
        sig.Connect([]() -> Bool { return false; });
        sig.Connect([]() -> Bool { return true; });
        REQUIRE(sig());
    }

    SECTION("AnyOf evaluates every slot (no short-circuit)")
    {
        auto sig = Signal<Bool(), SignalCombinerAnyOf>();
        auto calls = 0;
        sig.Connect([&]() -> Bool { ++calls; return true; });
        sig.Connect([&]() -> Bool { ++calls; return true; });
        sig.Connect([&]() -> Bool { ++calls; return false; });
        REQUIRE(sig());
        REQUIRE(calls == 3);
    }

    SECTION("AnyOf skips a disconnected slot")
    {
        auto sig = Signal<Bool(), SignalCombinerAnyOf>();
        auto c = sig.Connect([] () -> Bool { return true; });
        sig.Connect([] () -> Bool { return false; });
        c.Disconnect();
        REQUIRE(!sig());
    }

    SECTION("Void combiner invokes every slot")
    {
        auto sig = Signal<void(), SignalCombinerVoid>();
        auto calls = 0;
        sig.Connect([&] { ++calls; });
        sig.Connect([&] { ++calls; });
        sig();
        REQUIRE(calls == 2);
    }
}

TEST_CASE("Signal move semantics")
{
    SECTION("Move construction transfers slots and keeps handles valid")
    {
        auto count = 0;
        auto a = Signal<void(), SignalCombinerVoid>();
        auto c = a.Connect([&] { ++count; });

        auto b = Signal<void(), SignalCombinerVoid>(std::move(a));
        REQUIRE(b.GetSlotCount() == 1);
        b();
        REQUIRE(count == 1);

        // A handle obtained before the move still controls the slot.
        REQUIRE(c.IsConnected());
        c.Disconnect();
        REQUIRE(b.GetSlotCount() == 0);
    }

    SECTION("Move assignment")
    {
        auto count = 0;
        auto a = Signal<void(), SignalCombinerVoid>();
        a.Connect([&] { ++count; });

        auto b = Signal<void(), SignalCombinerVoid>();
        b = std::move(a);
        b();
        REQUIRE(count == 1);
    }

    SECTION("Swap exchanges slots")
    {
        auto x = 0;
        auto y = 0;
        auto a = Signal<void(), SignalCombinerVoid>();
        auto b = Signal<void(), SignalCombinerVoid>();
        a.Connect([&] { ++x; });
        b.Connect([&] { ++y; });

        a.Swap(b);
        a();
        REQUIRE(x == 0);
        REQUIRE(y == 1);
        b();
        REQUIRE(x == 1);
    }
}

TEST_CASE("Signal")
{
    SECTION("Default")
    {
        auto sig = Signal<Bool(int&), SignalCombinerAnyOf>();

        REQUIRE(sig.IsEmpty());
        REQUIRE(sig.GetSlotCount() == 0);
        sig.DisconnectAll();
    }

    SECTION("Connect")
    {
        auto sig = Signal<Bool(), SignalCombinerAnyOf>();

        SECTION("")
        {
            sig.Connect([&]() -> Bool { return true; });
            REQUIRE(sig());
        }

        SECTION("")
        {
            sig.Connect([&]() -> Bool { return false; });
            REQUIRE(!sig());
        }

        SECTION("")
        {
            int i = 0;
            sig.Connect([&]() -> Bool { ++i; return false; });
            sig.Connect([&]() -> Bool { ++i; return true; });
            REQUIRE(sig());
            REQUIRE(i == 2);
        }
    }

    SECTION("Async Combiner")
    {
        int i = 0;
        auto sig = Signal<Lazy<Bool>(int&), AsyncSignalCombinerAnyOf>();
        auto async = sig(i);
    }
}

TEST_CASE("Signal argument forwarding")
{
    SECTION("Value argument is delivered intact to every slot")
    {
        auto sig = Signal<void(std::string), SignalCombinerVoid>();

        auto seen = std::vector<std::string>();
        sig.Connect([&](std::string s) { seen.push_back(std::move(s)); });
        sig.Connect([&](std::string s) { seen.push_back(std::move(s)); });
        sig.Connect([&](std::string s) { seen.push_back(std::move(s)); });

        sig(std::string("hello"));

        REQUIRE(seen.size() == 3);
        // Would fail if the argument were moved-from while fanning out across slots.
        REQUIRE(seen[0] == "hello");
        REQUIRE(seen[1] == "hello");
        REQUIRE(seen[2] == "hello");
    }

    SECTION("Mutable reference argument is shared across slots and the caller")
    {
        auto sig = Signal<void(int&), SignalCombinerVoid>();

        sig.Connect([](int& x) { x += 1; });
        sig.Connect([](int& x) { x += 10; });
        sig.Connect([](int& x) { x += 100; });

        auto value = 0;
        sig(value);

        // Each slot mutates the same referent; the caller observes the accumulated result.
        REQUIRE(value == 111);
    }

    SECTION("Multiple heterogeneous arguments are delivered correctly")
    {
        auto sig = Signal<void(int, std::string const&), SignalCombinerVoid>();

        auto sum = 0;
        auto text = std::string();
        sig.Connect([&](int n, std::string const& s) { sum += n; text += s; });
        sig.Connect([&](int n, std::string const& s) { sum += n; text += s; });

        sig(21, std::string("ab"));

        REQUIRE(sum == 42);
        REQUIRE(text == "abab");
    }

    SECTION("Reference signature copies nothing while fanning out")
    {
        auto sig = Signal<void(CopyCounter const&), SignalCombinerVoid>();

        sig.Connect([](CopyCounter const&) {});
        sig.Connect([](CopyCounter const&) {});
        sig.Connect([](CopyCounter const&) {});

        auto arg = CopyCounter();
        CopyCounter::Reset();
        sig(arg);

        // A const-reference signature flows entirely by reference: no materialization.
        REQUIRE(CopyCounter::copies == 0);
        REQUIRE(CopyCounter::moves == 0);
    }

    SECTION("Const-reference signature copies nothing for an rvalue argument")
    {
        auto sig = Signal<void(CopyCounter const&), SignalCombinerVoid>();

        sig.Connect([](CopyCounter const&) {});
        sig.Connect([](CopyCounter const&) {});
        sig.Connect([](CopyCounter const&) {});

        CopyCounter::Reset();
        sig(CopyCounter());

        // The temporary binds straight through to each slot; nothing is copied or moved.
        REQUIRE(CopyCounter::copies == 0);
        REQUIRE(CopyCounter::moves == 0);
    }

    SECTION("Mutable-reference signature copies nothing")
    {
        // NOTE: An rvalue cannot bind to a mutable (T&) reference parameter, so the only
        // valid argument for a void(CopyCounter&) signature is an lvalue.
        auto sig = Signal<void(CopyCounter&), SignalCombinerVoid>();

        sig.Connect([](CopyCounter&) {});
        sig.Connect([](CopyCounter&) {});
        sig.Connect([](CopyCounter&) {});

        auto arg = CopyCounter();
        CopyCounter::Reset();
        sig(arg);

        // A mutable-reference signature flows entirely by reference: no materialization.
        REQUIRE(CopyCounter::copies == 0);
        REQUIRE(CopyCounter::moves == 0);
    }

    SECTION("By-value signature copies exactly once per slot")
    {
        auto sig = Signal<void(CopyCounter), SignalCombinerVoid>();

        sig.Connect([](CopyCounter const&) {});
        sig.Connect([](CopyCounter const&) {});
        sig.Connect([](CopyCounter const&) {});

        auto arg = CopyCounter();
        CopyCounter::Reset();
        sig(arg);

        // Exactly one unavoidable copy per slot (at the std::function boundary);
        // the dispatch chain itself adds no intermediate copies or moves.
        REQUIRE(CopyCounter::copies == 3);
        REQUIRE(CopyCounter::moves == 0);
    }
}

TEST_CASE("Signal reentrant destruction")
{
    SECTION("Destruct self")
    {
        struct Caller
        {
            Signal<void(), SignalCombinerVoid> sig;
        };
        auto caller = Unique<Caller>::Make();
        caller->sig.Connect([&]() { caller.Reset(); });
        caller->sig();
        REQUIRE(!caller);
    }

    SECTION("Destruct tracked callee")
    {
        struct Caller
        {
            Signal<void(), SignalCombinerVoid> sig;
        };

        struct Callee
        {
            Weak<void> self;
            Shared<std::function<void()>> func;

            auto CallFunc()
            {
                (*func)();
            }

            auto GetTracker()
            {
                return self;
            }
        };

        auto caller = Unique<Caller>::Make();
        auto callee1 = Shared<Callee>::Make();
        callee1->self = callee1;
        auto callee2 = Shared<Callee>::Make();
        callee2->self = callee2;

        int callCount = 0;
        auto func = Shared<std::function<void()>>::Make([&] {
            ++callCount;
            callee1.Reset();
            callee2.Reset();
        });
        callee1->func = func;
        callee2->func = func;

        SignalFunction::Connect(caller->sig, *callee1, &Callee::CallFunc);
        SignalFunction::Connect(caller->sig, *callee2, &Callee::CallFunc);

        caller->sig();
        caller->sig();

        REQUIRE(!callee1);
        REQUIRE(!callee2);
        REQUIRE(callCount == 1);
    }
}
