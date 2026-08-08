#include <catch2/catch_all.hpp>

#include <Futurewalker.Geometry.hpp>

#include <Futurewalker.Unit.hpp>

using namespace Futurewalker;

TEST_CASE("Matrix3x3")
{
    auto identity = Matrix3x3<Dp>::MakeIdentity();
    REQUIRE(identity == Matrix3x3<Dp>::MakeIdentity());

    auto a = Matrix3x3<Dp> {
        .m00 = 1, .m01 = 2, .m02 = 3,
        .m10 = 4, .m11 = 5, .m12 = 6,
        .m20 = 7, .m21 = 8, .m22 = 9,
    };
    REQUIRE(a != identity);
    REQUIRE(a == a);

    auto b = a;
    REQUIRE((a + b) == Matrix3x3<Dp> {
        .m00 = 2, .m01 = 4, .m02 = 6,
        .m10 = 8, .m11 = 10, .m12 = 12,
        .m20 = 14, .m21 = 16, .m22 = 18,
    });
    REQUIRE((a - b) == Matrix3x3<Dp> {});
    REQUIRE(-a == Matrix3x3<Dp> {
        .m00 = -1, .m01 = -2, .m02 = -3,
        .m10 = -4, .m11 = -5, .m12 = -6,
        .m20 = -7, .m21 = -8, .m22 = -9,
    });
    REQUIRE(+a == a);

    REQUIRE((a * identity) == a);
    REQUIRE((identity * a) == a);

    auto c = a;
    c += b;
    REQUIRE(c == (a + b));

    c = a;
    c -= b;
    REQUIRE(c == (a - b));

    c = a;
    c *= identity;
    REQUIRE(c == a);

    REQUIRE(Matrix3x3<Dp>::Transpose(a) == Matrix3x3<Dp> {
        .m00 = 1, .m01 = 4, .m02 = 7,
        .m10 = 2, .m11 = 5, .m12 = 8,
        .m20 = 3, .m21 = 6, .m22 = 9,
    });
    REQUIRE(Matrix3x3<Dp>::Transpose(identity) == identity);
    REQUIRE(Matrix3x3<Dp>::Determinant(identity) == 1);
    REQUIRE(Matrix3x3<Dp>::Determinant(a) == 0);

    REQUIRE(Matrix3x3<Dp>::MakeTranslation(Vector2<Dp>(2, 3)) == Matrix3x3<Dp> {
        .m00 = 1, .m01 = 0, .m02 = 2,
        .m10 = 0, .m11 = 1, .m12 = 3,
        .m20 = 0, .m21 = 0, .m22 = 1,
    });
    REQUIRE(Matrix3x3<Dp>::MakeScale(2, 3) == Matrix3x3<Dp> {
        .m00 = 2, .m01 = 0, .m02 = 0,
        .m10 = 0, .m11 = 3, .m12 = 0,
        .m20 = 0, .m21 = 0, .m22 = 1,
    });
    REQUIRE(Matrix3x3<Dp>::MakeScale(2) == Matrix3x3<Dp>::MakeScale(2, 2));
    REQUIRE(Matrix3x3<Dp>::MakeRotation(0) == identity);
}

TEST_CASE("Matrix4x4")
{
    auto identity = Matrix4x4<Dp>::MakeIdentity();
    REQUIRE(identity == Matrix4x4<Dp>::MakeIdentity());

    auto a = Matrix4x4<Dp> {
        .m00 = 1, .m01 = 2, .m02 = 3, .m03 = 4,
        .m10 = 5, .m11 = 6, .m12 = 7, .m13 = 8,
        .m20 = 9, .m21 = 10, .m22 = 11, .m23 = 12,
        .m30 = 13, .m31 = 14, .m32 = 15, .m33 = 16,
    };
    REQUIRE(a != identity);
    REQUIRE(a == a);

    auto b = a;
    REQUIRE((a + b) == Matrix4x4<Dp> {
        .m00 = 2, .m01 = 4, .m02 = 6, .m03 = 8,
        .m10 = 10, .m11 = 12, .m12 = 14, .m13 = 16,
        .m20 = 18, .m21 = 20, .m22 = 22, .m23 = 24,
        .m30 = 26, .m31 = 28, .m32 = 30, .m33 = 32,
    });
    REQUIRE((a - b) == Matrix4x4<Dp> {});
    REQUIRE(-a == Matrix4x4<Dp> {
        .m00 = -1, .m01 = -2, .m02 = -3, .m03 = -4,
        .m10 = -5, .m11 = -6, .m12 = -7, .m13 = -8,
        .m20 = -9, .m21 = -10, .m22 = -11, .m23 = -12,
        .m30 = -13, .m31 = -14, .m32 = -15, .m33 = -16,
    });
    REQUIRE(+a == a);

    REQUIRE((a * identity) == a);
    REQUIRE((identity * a) == a);

    auto c = a;
    c += b;
    REQUIRE(c == (a + b));

    c = a;
    c -= b;
    REQUIRE(c == (a - b));

    c = a;
    c *= identity;
    REQUIRE(c == a);

    REQUIRE(Matrix4x4<Dp>::Transpose(a) == Matrix4x4<Dp> {
        .m00 = 1, .m01 = 5, .m02 = 9, .m03 = 13,
        .m10 = 2, .m11 = 6, .m12 = 10, .m13 = 14,
        .m20 = 3, .m21 = 7, .m22 = 11, .m23 = 15,
        .m30 = 4, .m31 = 8, .m32 = 12, .m33 = 16,
    });
    REQUIRE(Matrix4x4<Dp>::Transpose(identity) == identity);
    REQUIRE(Matrix4x4<Dp>::Determinant(identity) == 1);
    REQUIRE(Matrix4x4<Dp>::Determinant(a) == 0);

    REQUIRE(Matrix4x4<Dp>::MakeTranslation(Vector3<Dp>(2, 3, 4)) == Matrix4x4<Dp> {
        .m00 = 1, .m01 = 0, .m02 = 0, .m03 = 2,
        .m10 = 0, .m11 = 1, .m12 = 0, .m13 = 3,
        .m20 = 0, .m21 = 0, .m22 = 1, .m23 = 4,
        .m30 = 0, .m31 = 0, .m32 = 0, .m33 = 1,
    });
    REQUIRE(Matrix4x4<Dp>::MakeScale(2, 3, 4) == Matrix4x4<Dp> {
        .m00 = 2, .m01 = 0, .m02 = 0, .m03 = 0,
        .m10 = 0, .m11 = 3, .m12 = 0, .m13 = 0,
        .m20 = 0, .m21 = 0, .m22 = 4, .m23 = 0,
        .m30 = 0, .m31 = 0, .m32 = 0, .m33 = 1,
    });
    REQUIRE(Matrix4x4<Dp>::MakeScale(2) == Matrix4x4<Dp>::MakeScale(2, 2, 2));
    REQUIRE(Matrix4x4<Dp>::MakeRotation(Vector3<Dp>(0, 0, 1), 0) == identity);

    auto const v = Matrix4x4<Dp>::MakeTranslation(Vector3<Dp>(2, 3, 4)) * Vector4<Dp>(1, 1, 1, 1);
    REQUIRE(v == Vector4<Dp>(3, 4, 5, 1));
}

TEST_CASE("Matrix5x5")
{
    auto identity = Matrix5x5<Dp>::MakeIdentity();
    REQUIRE(identity == Matrix5x5<Dp>::MakeIdentity());

    auto a = Matrix5x5<Dp> {
        .m00 = 1, .m01 = 2, .m02 = 3, .m03 = 4, .m04 = 5,
        .m10 = 6, .m11 = 7, .m12 = 8, .m13 = 9, .m14 = 10,
        .m20 = 11, .m21 = 12, .m22 = 13, .m23 = 14, .m24 = 15,
        .m30 = 16, .m31 = 17, .m32 = 18, .m33 = 19, .m34 = 20,
        .m40 = 21, .m41 = 22, .m42 = 23, .m43 = 24, .m44 = 25,
    };
    REQUIRE(a != identity);
    REQUIRE(a == a);

    auto b = a;
    REQUIRE((a + b) == Matrix5x5<Dp> {
        .m00 = 2, .m01 = 4, .m02 = 6, .m03 = 8, .m04 = 10,
        .m10 = 12, .m11 = 14, .m12 = 16, .m13 = 18, .m14 = 20,
        .m20 = 22, .m21 = 24, .m22 = 26, .m23 = 28, .m24 = 30,
        .m30 = 32, .m31 = 34, .m32 = 36, .m33 = 38, .m34 = 40,
        .m40 = 42, .m41 = 44, .m42 = 46, .m43 = 48, .m44 = 50,
    });
    REQUIRE((a - b) == Matrix5x5<Dp> {});
    REQUIRE(-a == Matrix5x5<Dp> {
        .m00 = -1, .m01 = -2, .m02 = -3, .m03 = -4, .m04 = -5,
        .m10 = -6, .m11 = -7, .m12 = -8, .m13 = -9, .m14 = -10,
        .m20 = -11, .m21 = -12, .m22 = -13, .m23 = -14, .m24 = -15,
        .m30 = -16, .m31 = -17, .m32 = -18, .m33 = -19, .m34 = -20,
        .m40 = -21, .m41 = -22, .m42 = -23, .m43 = -24, .m44 = -25,
    });
    REQUIRE(+a == a);

    REQUIRE((a * identity) == a);
    REQUIRE((identity * a) == a);

    auto c = a;
    c += b;
    REQUIRE(c == (a + b));

    c = a;
    c -= b;
    REQUIRE(c == (a - b));

    c = a;
    c *= identity;
    REQUIRE(c == a);

    REQUIRE(Matrix5x5<Dp>::Transpose(a) == Matrix5x5<Dp> {
        .m00 = 1, .m01 = 6, .m02 = 11, .m03 = 16, .m04 = 21,
        .m10 = 2, .m11 = 7, .m12 = 12, .m13 = 17, .m14 = 22,
        .m20 = 3, .m21 = 8, .m22 = 13, .m23 = 18, .m24 = 23,
        .m30 = 4, .m31 = 9, .m32 = 14, .m33 = 19, .m34 = 24,
        .m40 = 5, .m41 = 10, .m42 = 15, .m43 = 20, .m44 = 25,
    });
    REQUIRE(Matrix5x5<Dp>::Transpose(identity) == identity);
    REQUIRE(Matrix5x5<Dp>::Determinant(identity) == 1);
    REQUIRE(Matrix5x5<Dp>::Determinant(a) == 0);

    REQUIRE(Matrix5x5<Dp>::MakeTranslation(Vector4<Dp>(2, 3, 4, 5)) == Matrix5x5<Dp> {
        .m00 = 1, .m01 = 0, .m02 = 0, .m03 = 0, .m04 = 2,
        .m10 = 0, .m11 = 1, .m12 = 0, .m13 = 0, .m14 = 3,
        .m20 = 0, .m21 = 0, .m22 = 1, .m23 = 0, .m24 = 4,
        .m30 = 0, .m31 = 0, .m32 = 0, .m33 = 1, .m34 = 5,
        .m40 = 0, .m41 = 0, .m42 = 0, .m43 = 0, .m44 = 1,
    });
    REQUIRE(Matrix5x5<Dp>::MakeScale(2, 3, 4, 5) == Matrix5x5<Dp> {
        .m00 = 2, .m01 = 0, .m02 = 0, .m03 = 0, .m04 = 0,
        .m10 = 0, .m11 = 3, .m12 = 0, .m13 = 0, .m14 = 0,
        .m20 = 0, .m21 = 0, .m22 = 4, .m23 = 0, .m24 = 0,
        .m30 = 0, .m31 = 0, .m32 = 0, .m33 = 5, .m34 = 0,
        .m40 = 0, .m41 = 0, .m42 = 0, .m43 = 0, .m44 = 1,
    });
    REQUIRE(Matrix5x5<Dp>::MakeScale(2) == Matrix5x5<Dp>::MakeScale(2, 2, 2, 2));

    auto const v = Matrix5x5<Dp>::MakeTranslation(Vector4<Dp>(2, 3, 4, 5)) * Vector5<Dp>(1, 1, 1, 1, 1);
    REQUIRE(v == Vector5<Dp>(3, 4, 5, 6, 1));
}
