#include <catch2/catch_test_macros.hpp>

#include <Futurewalker.Core.Uuid.hpp>

#include <array>

using namespace Futurewalker;

using Bytes = std::array<uint8_t, 16>;

TEST_CASE("Uuid default constructed is nil")
{
    auto const uuid = Uuid();
    REQUIRE(uuid.data == Bytes {});
}

TEST_CASE("Uuid::MakeRandom is not nil")
{
    auto const uuid = Uuid::MakeRandom();
    REQUIRE_FALSE(uuid.data == Bytes {});
}

TEST_CASE("Uuid::MakeRandom yields distinct values")
{
    auto const a = Uuid::MakeRandom();
    auto const b = Uuid::MakeRandom();
    REQUIRE_FALSE(a.data == b.data);
}

TEST_CASE("Uuid::MakeRandom sets RFC 4122 version 4 and variant bits")
{
    auto const uuid = Uuid::MakeRandom();
    // Version nibble must be 0x4 (random).
    REQUIRE((uuid.data[6] & 0xF0) == 0x40);
    // Variant must be the RFC 4122 variant (most significant bits 10).
    REQUIRE((uuid.data[8] & 0xC0) == 0x80);
}

TEST_CASE("Uuid::MakeFromString parses canonical form")
{
    auto const uuid = Uuid::MakeFromString("12345678-9abc-def0-1234-56789abcdef0");
    REQUIRE(uuid.HasValue());

    auto const expected = Bytes {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
                                 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
    REQUIRE(uuid->data == expected);
}

TEST_CASE("Uuid::MakeFromString parses the nil UUID")
{
    auto const uuid = Uuid::MakeFromString("00000000-0000-0000-0000-000000000000");
    REQUIRE(uuid.HasValue());
    REQUIRE(uuid->data == Bytes {});
}

TEST_CASE("Uuid::MakeFromString accepts braced form")
{
    auto const braced = Uuid::MakeFromString("{12345678-9abc-def0-1234-56789abcdef0}");
    auto const plain = Uuid::MakeFromString("12345678-9abc-def0-1234-56789abcdef0");
    REQUIRE(braced.HasValue());
    REQUIRE(plain.HasValue());
    REQUIRE(braced->data == plain->data);
}

TEST_CASE("Uuid::MakeFromString accepts form without hyphens")
{
    auto const compact = Uuid::MakeFromString("123456789abcdef0123456789abcdef0");
    auto const hyphenated = Uuid::MakeFromString("12345678-9abc-def0-1234-56789abcdef0");
    REQUIRE(compact.HasValue());
    REQUIRE(hyphenated.HasValue());
    REQUIRE(compact->data == hyphenated->data);
}

TEST_CASE("Uuid::MakeFromString is case insensitive")
{
    auto const lower = Uuid::MakeFromString("abcdefab-cdef-abcd-efab-cdefabcdefab");
    auto const upper = Uuid::MakeFromString("ABCDEFAB-CDEF-ABCD-EFAB-CDEFABCDEFAB");
    REQUIRE(lower.HasValue());
    REQUIRE(upper.HasValue());
    REQUIRE(lower->data == upper->data);
}

TEST_CASE("Uuid::MakeFromString rejects invalid input")
{
    REQUIRE_FALSE(Uuid::MakeFromString("").HasValue());
    REQUIRE_FALSE(Uuid::MakeFromString("not-a-uuid").HasValue());
    REQUIRE_FALSE(Uuid::MakeFromString("12345678-9abc-def0-1234").HasValue());
    REQUIRE_FALSE(Uuid::MakeFromString("12345678-9abc-def0-1234-56789abcdefg0").HasValue());
}
