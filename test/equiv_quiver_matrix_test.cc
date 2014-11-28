#include "gtest/gtest.h"

#include "equiv_quiver_matrix.h"
#include "int_matrix.h"
#include "equivalence_checker.h"

#include <memory>

namespace cluster {
	TEST(EquivMatrix, SimpleTest) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		EXPECT_TRUE(m1.equals(m2));
		EXPECT_TRUE(m2.equals(m1));

		int v3[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v4[] = {0, -1, 0, 1, 0, -1, 0, 1, 0};
		EquivQuiverMatrix m3(3, 3, v3);
		EquivQuiverMatrix m4(3, 3, v4);

		EXPECT_TRUE(m3.equals(m4));
	}

	TEST(EquivMatrix, SameAreEqual) {
		int v1[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
		EquivQuiverMatrix m1(3, 3, v1);

		EXPECT_TRUE(m1.equals(m1));

		int v2[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
		EquivQuiverMatrix m2(3, 3, v2);

		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(EquivMatrix, 4x4) {
		int v1[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
		EquivQuiverMatrix m1(4, 4, v1);
		EquivQuiverMatrix m2(4, 4, v1);

		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(EquivMatrix, 4) {
		int v1[] = {0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1};
		int v2[] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1};
		EquivQuiverMatrix m1(4, 4, v1);
		EquivQuiverMatrix m2(4, 4, v2);
		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(EquivMatrix, NotEquivalent) {
		int v1[] = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4};
		int v2[] = {5, 0, 0, 1, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4};
		EquivQuiverMatrix m1(4, 4, v1);
		EquivQuiverMatrix m2(4, 4, v2);
		EXPECT_FALSE(m1.equals(m2));
	}

	TEST(EquivMatrix, Quivers) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		EXPECT_TRUE(m1.equals(m2));
	}

	TEST(EquivMatrix, Checker) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		std::shared_ptr<EquivalenceChecker> ch = EquivalenceChecker::Get(3);
		EXPECT_TRUE(ch->are_equivalent(m1, m2));
	}

	TEST(EquivMatrix, Cache) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		std::shared_ptr<EquivalenceChecker> ch = EquivalenceChecker::Get(3);
		bool a = ch->are_equivalent(m1, m2);
		EXPECT_TRUE(a);

		int v3[] = {0, 1, 0, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix notEquiv(3, 3, v3);
		bool b = ch->are_equivalent(m1, notEquiv);
		bool c = ch->are_equivalent(m2, notEquiv);
		EXPECT_FALSE(b);
		EXPECT_FALSE(c);
	}

	TEST(EquivMatrix, 5x5) {

		int v1[] = {0, -1, 0, 0, 0, 1, 0, 0, -1, 1, 0, 0, 0, 1, -1, 0, 1, -1, 0,
		            0,	0, -1, 1, 0, 0
		           };
		int v2[] = {0, 0, 0, -1, 1, 0, 0, 1, 1, -1, 0, -1, 0, 0, 0, 1, -1, 0, 0,
		            0,	-1, 1, 0, 0, 0
		           };
		EquivQuiverMatrix m1(5, 5, v1);
		EquivQuiverMatrix m2(5, 5, v2);

		EXPECT_TRUE(m1.equals(m2));

		int v3[] = {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
			0, 0, 1,	0};
		IntMatrix perm(5, 5, v3);
		IntMatrix res1(5, 5);
		IntMatrix res2(5, 5);
		m1.mult_right(perm, res1);
		m2.mult_left(perm, res2);
		EXPECT_TRUE(res1.equals(res2));
	}

	TEST(EquivMatrix, stdequal) {
		int v1[] = {0, 1, 2, 3};
		EquivQuiverMatrix m1(2, 2, v1);
		EquivQuiverMatrix m2(2, 2, v1);

		std::equal_to<EquivQuiverMatrix> eq;

		EXPECT_TRUE(eq(m1, m2));

	}
	TEST(EquivMatrix, stdequalEquiv) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, 1, -1, -1, 0, 0, 1, 0, 0};
		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		std::equal_to<EquivQuiverMatrix> eq;

		EXPECT_TRUE(eq(m1, m2));
	}
	TEST(EquivMatrix, Equivalent3) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, -1, 0, 1, 0, -1, 0, 1, 0};

		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		EXPECT_TRUE(m1.equals(m2));
	}	
	
	TEST(EquivMatrix, EquivalentHash) {
		int v1[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
		int v2[] = {0, -1, 0, 1, 0, -1, 0, 1, 0};

		EquivQuiverMatrix m1(3, 3, v1);
		EquivQuiverMatrix m2(3, 3, v2);

		EXPECT_EQ(m1.hash(), m2.hash());
	}
	
	TEST(EquivMatrix, StringHuge) {
		std::string str = "{ { 0 1 0 0 0 0 0 0 0 0 } "
		"{ -1 0 1 0 0 0 0 0 0 0 } "
		"{ 0 -1 0 1 0 0 0 0 0 0 } "
		"{ 0 0 -1 0 1 0 0 0 0 0 } "
		"{ 0 0 0 -1 0 1 0 0 0 0 } "
		"{ 0 0 0 0 -1 0 1 0 0 -1 } "
		"{ 0 0 0 0 0 -1 0 1 0 0 } "
		"{ 0 0 0 0 0 0 -1 0 1 0 } "
		"{ 0 0 0 0 0 0 0 -1 0 1 } "
		"{ 0 0 0 0 0 1 0 0 -1 0 } }";
		EquivQuiverMatrix a(str);

		int v[] = {	0,1,0,0,0,0,0,0,0,0,
					-1,0,1,0,0,0,0,0,0,0,
					0,-1,0,1,0,0,0,0,0,0,
					0,0,-1,0,1,0,0,0,0,0,
					0,0,0,-1,0,1,0,0,0,0,
					0,0,0,0,-1,0,1,0,0,-1,
					0,0,0,0,0,-1,0,1,0,0,
					0,0,0,0,0,0,-1,0,1,0,
					0,0,0,0,0,0,0,-1,0,1,
					0,0,0,0,0,1,0,0,-1,0};
		EquivQuiverMatrix exp(10, 10, v);

		EXPECT_TRUE(IntMatrix::are_equal(exp,a));
	}	

	TEST(EquivMatrix, FallThrough) {
		std::string s1 = "{ { 0 1 -1 0 0 } "
		"{ -1 0 0 0 0 } "
		"{ 1 0 0 1 0 } "
		"{ 0 0 -1 0 -1 } "
		"{ 0 0 0 1 0 } }";
		std::string s2 = "{ { 0 -1 0 0 0 } "
		"{ 1 0 1 0 0 } "
		"{ 0 -1 0 1 0 } "
		"{ 0 0 -1 0 -1 } "
		"{ 0 0 0 1 0 } }";
		EquivQuiverMatrix a(s1);
		EquivQuiverMatrix b(s2);

		EXPECT_FALSE(a.equals(b));
	}

	TEST(EquivMatrix, DiffMapping) {
		std::string s1 = "{ { 0 0 -1 1 } { 0 0 2 -1 } { 1 -2 0 0 } { -1 1 0 0 } }";
		std::string s2 = "{ { 0 1 0 -2 } { -1 0 1 0 } { 0 -1 0 1 } { 2 0 -1 0 } }";

		EquivQuiverMatrix a(s1);
		EquivQuiverMatrix b(s2);

		EXPECT_TRUE(a.equals(b));
	}

	TEST(EquivMatrix, StrangeEqual) {
		std::string s1 = "{ { 0 -1 1 0 0 1 0 0 } { 1 0 0 0 1 0 0 -1 } { -1 0 0 0 0 0 0 0 } { 0 0 0 0 0 1 0 0 } { 0 -1 0 0 0 0 0 0 } { -1 0 0 -1 0 0 0 1 } { 0 0 0 0 0 0 0 -1 } { 0 1 0 0 0 -1 1 0 } }";
		std::string s2 = "{ { 0 0 1 0 -1 1 0 0 } { 0 0 0 0 1 0 0 0 } { -1 0 0 0 0 0 0 0 } { 0 0 0 0 0 1 0 0 } { 1 -1 0 0 0 0 0 -1 } { -1 0 0 -1 0 0 0 1 } { 0 0 0 0 0 0 0 -1 } { 0 0 0 0 1 -1 1 0 } }";

		EquivQuiverMatrix a(s1);
		EquivQuiverMatrix b(s2);

		EXPECT_FALSE(a.equals(b));
		EXPECT_FALSE(b.equals(a));
	}

	TEST(EquivMatrix, Empty) {
		EquivQuiverMatrix empty;

		std::string s1 = "{ { 0 0 -1 1 } { 0 0 2 -1 } { 1 -2 0 0 } { -1 1 0 0 } }";
		EquivQuiverMatrix other(s1);

		EXPECT_FALSE(other.equals(empty));
		EXPECT_FALSE(empty.equals(other));
	}
	TEST(EquivMatrix, GetIdPerm) {
		int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		EquivQuiverMatrix m(3, 3, v);
		EquivQuiverMatrix n(3, 3, v);

		std::vector<int> perm = m.get_permutation(n);
		ASSERT_FALSE(perm.empty());
		ASSERT_EQ(perm.size(), 3);
		for( size_t i = 0; i < perm.size(); i++) {
			EXPECT_EQ(perm[i], i);
		}
	}
	TEST(EquivMatrix, SwapPerm) {
		int v[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
		int w[] = {8, 7, 6, 5, 4, 3, 2, 1, 0};
		EquivQuiverMatrix m(3, 3, v);
		EquivQuiverMatrix n(3, 3, w);

		ASSERT_TRUE(m.equals(n));

		std::vector<int> perm = m.get_permutation(n);
		ASSERT_FALSE(perm.empty());
		ASSERT_EQ(perm.size(), 3);
		EXPECT_EQ(perm[0], 2);
		EXPECT_EQ(perm[1], 1);
		EXPECT_EQ(perm[2], 0);
	}
	TEST(EquivMatrix, SubMatrixAffectsEquals) {
		EquivQuiverMatrix m("{ { 0 -1 1 1 } { 1 0 -1 -1 } { -1 1 0 0 } { -1 1 0 0 } }");
		EquivQuiverMatrix n("{ { 0 1 -1 1 } { -1 0 1 0 } { 1 -1 0 -1 } { -1 0 1 0 } }");

		EXPECT_TRUE(m.equals(n));
		EXPECT_TRUE(n.equals(m));

		EquivQuiverMatrix o("{ { 0 -1 1 1 1 } { 1 0 -1 -1 -1 } { -1 1 0 0 1 } { -1 1 0 0 0 } { -1 1 -1 0 0 } }");
		EquivQuiverMatrix k(4, 4);
		std::vector<int> rows(4);
		rows[0] = 0;
		rows[1] = 1;
		rows[2] = 2;
		rows[3] = 3;

		o.submatrix(rows, rows, k);
		EXPECT_TRUE(k.equals(m));
		EXPECT_TRUE(m.equals(k));
		EXPECT_TRUE(k.equals(n));
		EXPECT_TRUE(n.equals(k));

		o.submatrix(rows, rows, m);
		EXPECT_TRUE(m.equals(n));
		EXPECT_TRUE(n.equals(m));
		m.reset();
		EXPECT_TRUE(m.equals(n));
		EXPECT_TRUE(n.equals(m));
	}
	TEST(EquivMatrix, AllPermutations) {
		EquivQuiverMatrix m("{ { 0 1 0 -1 } { -1 0 1 0 } { 0 -1 0 1 } { 1 0 -1 0 } }");
		EquivQuiverMatrix n("{ { 0 1 0 -1 } { -1 0 1 0 } { 0 -1 0 1 } { 1 0 -1 0 } }");

		ASSERT_TRUE(m.equals(n));
		std::shared_ptr<std::vector<std::vector<int>>> vecp = m.all_permutations(n);
		ASSERT_FALSE(vecp->empty());
		EXPECT_EQ(4, vecp->size());
	}
}

