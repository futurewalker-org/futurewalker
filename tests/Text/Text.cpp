#include <catch2/catch_all.hpp>

#include <Futurewalker.Text.Text.hpp>

using namespace Futurewalker;

TEST_CASE("Text")
{
    SECTION("Default")
    {
        auto text = Text();
        REQUIRE(text.GetString().IsEmpty());
        REQUIRE(text.GetU16String().IsEmpty());
        REQUIRE(text.GetCodePointCount() == 0);
        REQUIRE(text.GetCodePointIndexByU8Index(-1) == 0);
        REQUIRE(text.GetCodePointIndexByU8Index(0) == 0);
        REQUIRE(text.GetCodePointIndexByU8Index(1) == 0);
        REQUIRE(text.GetCodePointIndexByU16Index(-1) == 0);
        REQUIRE(text.GetCodePointIndexByU16Index(0) == 0);
        REQUIRE(text.GetCodePointIndexByU16Index(1) == 0);
        REQUIRE(text.GetU8IndexByCodePointIndex(-1) == 0);
        REQUIRE(text.GetU8IndexByCodePointIndex(0) == 0);
        REQUIRE(text.GetU8IndexByCodePointIndex(1) == 0);
        REQUIRE(text.GetU16IndexByCodePointIndex(-1) == 0);
        REQUIRE(text.GetU16IndexByCodePointIndex(0) == 0);
        REQUIRE(text.GetU16IndexByCodePointIndex(1) == 0);
    }

    SECTION("Info")
    {
        SECTION("Latin")
        {
            auto text = Text(u8"A");
            REQUIRE(text.GetString() == u8"A");
            REQUIRE(text.GetU16String() == u"A");
            REQUIRE(text.GetCodePointCount() == 1);
            REQUIRE(text.GetU8CodeUnitCount() == 1);
            REQUIRE(text.GetU16CodeUnitCount() == 1);
            REQUIRE(text.GetCodePointIndexByU8Index(-1) == 0);
            REQUIRE(text.GetCodePointIndexByU8Index(0) == 0);
            REQUIRE(text.GetCodePointIndexByU8Index(1) == 1);
            REQUIRE(text.GetCodePointIndexByU8Index(2) == 1);
            REQUIRE(text.GetCodePointIndexByU16Index(-1) == 0);
            REQUIRE(text.GetCodePointIndexByU16Index(0) == 0);
            REQUIRE(text.GetCodePointIndexByU16Index(1) == 1);
            REQUIRE(text.GetCodePointIndexByU16Index(2) == 1);
            REQUIRE(text.GetU8IndexByCodePointIndex(-1) == 0);
            REQUIRE(text.GetU8IndexByCodePointIndex(0) == 0);
            REQUIRE(text.GetU8IndexByCodePointIndex(1) == 1);
            REQUIRE(text.GetU8IndexByCodePointIndex(2) == 1);
            REQUIRE(text.GetU16IndexByCodePointIndex(-1) == 0);
            REQUIRE(text.GetU16IndexByCodePointIndex(0) == 0);
            REQUIRE(text.GetU16IndexByCodePointIndex(1) == 1);
            REQUIRE(text.GetU16IndexByCodePointIndex(2) == 1);
        }
    }

    SECTION("CoW")
    {
        SECTION("Copy")
        {
            auto text1 = Text(u8"Hello");
            auto text2 = text1;
            REQUIRE(text1.GetString() == text2.GetString());
            REQUIRE(text1.GetU16String() == text2.GetU16String());
            REQUIRE(text1.GetCodePointCount() == text2.GetCodePointCount());
            REQUIRE(text1.GetU8CodeUnitCount() == text2.GetU8CodeUnitCount());
            REQUIRE(text1.GetU16CodeUnitCount() == text2.GetU16CodeUnitCount());
        }

        SECTION("Move")
        {
            auto text1 = Text(u8"Hello");
            auto text2 = std::move(text1);
            REQUIRE(text1.GetString().IsEmpty());
            REQUIRE(text2.GetString() == u8"Hello");
        }

        SECTION("Write")
        {
            auto text = Text(u8"Hello");
            auto text2 = text;
            text.SetString(u8"World");
            REQUIRE(text.GetString() == u8"World");
            REQUIRE(text.GetU16String() == u"World");
            REQUIRE(text.GetCodePointCount() == 5);
            REQUIRE(text.GetU8CodeUnitCount() == 5);
            REQUIRE(text.GetU16CodeUnitCount() == 5);

            REQUIRE(text2.GetString() == u8"Hello");
            REQUIRE(text2.GetU16String() == u"Hello");
            REQUIRE(text2.GetCodePointCount() == 5);
            REQUIRE(text2.GetU8CodeUnitCount() == 5);
            REQUIRE(text2.GetU16CodeUnitCount() == 5);

            REQUIRE(text2.GetString() == u8"Hello");
            REQUIRE(text2.GetU16String() == u"Hello");
            REQUIRE(text2.GetCodePointCount() == 5);
            REQUIRE(text2.GetU8CodeUnitCount() == 5);
            REQUIRE(text2.GetU16CodeUnitCount() == 5);

            REQUIRE(text2.GetString() == u8"Hello");
            REQUIRE(text2.GetU16String() == u"Hello");
            REQUIRE(text2.GetCodePointCount() == 5);
            REQUIRE(text2.GetU8CodeUnitCount() == 5);
            REQUIRE(text2.GetU16CodeUnitCount() == 5);

            REQUIRE(text2.GetString() == u8"Hello");
            REQUIRE(text2.GetU16String() == u"Hello");
            REQUIRE(text2.GetCodePointCount() == 5);
            REQUIRE(text2.GetU8CodeUnitCount() == 5);
            REQUIRE(text2.GetU16CodeUnitCount() == 5);
        }
    }

    SECTION("Replace")
    {
        SECTION("On empty string")
        {
            SECTION("With empty text")
            {
                auto text = Text(u8"");
                for (auto x = 0; x < 2; ++x)
                {
                    for (auto y = 0; y < 2; ++y)
                    {
                        SECTION("")
                        {
                            text.Replace({x, y}, Text(u8""));
                        }
                        SECTION("")
                        {
                            text.Replace({x, y}, Text());
                        }
                    }
                }
                REQUIRE(text.GetString().IsEmpty());
                REQUIRE(text.GetU16String().IsEmpty());
                REQUIRE(text.GetCodePointCount() == 0);
            }

            SECTION("With normal text")
            {
                auto text = Text();
                auto replaceText = Text(u8"Hello, World!");
                text.Replace({0, 0}, replaceText);
                REQUIRE(text.GetString() == replaceText.GetString());
                REQUIRE(text.GetU16String() == replaceText.GetU16String());
            }

            SECTION("With normal text")
            {
                auto text = Text();
                text.Replace({0, 0}, Text(u8"a"));
                text.Replace({1, 1}, Text());
                text.Replace({0,1}, Text());
                REQUIRE(text.GetString().IsEmpty());
                REQUIRE(text.GetU16String().IsEmpty());
            }
        }

        SECTION("On normal text")
        {
            SECTION("A")
            {
                auto text = Text(u8"A");

                SECTION("")
                {
                    text.Replace({0, 1}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"B");
                    REQUIRE(text.GetU16String() == u"B");
                }
                SECTION("")
                {
                    text.Replace({1, 0}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"A");
                    REQUIRE(text.GetU16String() == u"A");
                }
                SECTION("")
                {
                    text.Replace({0, 0}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"BA");
                    REQUIRE(text.GetU16String() == u"BA");
                }
                SECTION("")
                {
                    text.Replace({-1, 0}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"A");
                    REQUIRE(text.GetU16String() == u"A");
                }
                SECTION("")
                {
                    text.Replace({0, -1}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"A");
                    REQUIRE(text.GetU16String() == u"A");
                }
                SECTION("")
                {
                    text.Replace({1, 1}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"AB");
                    REQUIRE(text.GetU16String() == u"AB");
                }
                SECTION("")
                {
                    text.Replace({1, 2}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"A");
                    REQUIRE(text.GetU16String() == u"A");
                }
                SECTION("")
                {
                    text.Replace({2, 1}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"A");
                    REQUIRE(text.GetU16String() == u"A");
                }
                SECTION("")
                {
                    text.Replace({0, 2}, Text(u8"B"));
                    REQUIRE(text.GetString() == u8"A");
                    REQUIRE(text.GetU16String() == u"A");
                }
            }

            SECTION("AB")
            {
                auto text = Text(u8"AB");

                SECTION("")
                {
                    text.Replace({0, 0}, Text(u8"C"));
                    REQUIRE(text.GetString() == u8"CAB");
                    REQUIRE(text.GetU16String() == u"CAB");
                }
                SECTION("")
                {
                    text.Replace({0, 1}, Text(u8"C"));
                    REQUIRE(text.GetString() == u8"CB");
                    REQUIRE(text.GetU16String() == u"CB");
                }
                SECTION("")
                {
                    text.Replace({1, 1}, Text(u8"C"));
                    REQUIRE(text.GetString() == u8"ACB");
                    REQUIRE(text.GetU16String() == u"ACB");
                }
                SECTION("")
                {
                    text.Replace({1, 2}, Text(u8"C"));
                    REQUIRE(text.GetString() == u8"AC");
                    REQUIRE(text.GetU16String() == u"AC");
                }
                SECTION("")
                {
                    text.Replace({2, 2}, Text(u8"C"));
                    REQUIRE(text.GetString() == u8"ABC");
                    REQUIRE(text.GetU16String() == u"ABC");
                }
                SECTION("")
                {
                    text.Replace({0, 2}, Text(u8"C"));
                    REQUIRE(text.GetString() == u8"C");
                    REQUIRE(text.GetU16String() == u"C");
                }
                SECTION("")
                {
                    text.Replace({2, 3}, Text(u8"C"));
                    REQUIRE(text.GetString() == u8"AB");
                    REQUIRE(text.GetU16String() == u"AB");
                }
            }
        }
    }
}
