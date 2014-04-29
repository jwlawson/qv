/*
 * equiv_mutation_class_size_test.cc
 */

#include "gtest/gtest.h"
#include "equiv_mutation_class_size.h"
#include "dynkin.h"

namespace cluster {

	TEST(EquivMutationClassSize, A3) {
		EquivQuiverMatrix m(dynkin::A3);
		int exp = 4;
		int res = equivsize::Size(m);

		EXPECT_EQ(exp, res);
	}


	TEST(EquivMutationClassSize, A5) {
		EquivQuiverMatrix m(dynkin::A5);
		int exp = 19;
		int res = equivsize::Size(m);

		EXPECT_EQ(exp, res);
	}


	TEST(EquivMutationClassSize, D6) {
		EquivQuiverMatrix m(dynkin::D6);
		int exp = 80;
		int res = equivsize::Size(m);

		EXPECT_EQ(exp, res);
	}

	TEST(EquivMutationClassSize, Infinite) {
		std::string str = "{	{ 0 1 0 0 } { -1 0 1 1 } { 0 -1 0 1 } { 0 -1 -1 0 }";
		EquivQuiverMatrix m(str);

		int res = equivsize::Size(m);

		EXPECT_EQ(-1, res);
	}

	/**
	 * This test was included after a strange bug appeared only in this mutation
	 * class. At the 1458 matrix to be taken from the class, computing the 
	 * mutation at the 3rd vertex resulted in a call to the hash map for the
	 * matrix:
	 * { 	{ 0 -1 1 0 0 1 0 0 }
	 * 		{ 1 0 0 0 1 0 0 -1 }
	 * 		{ -1 0 0 0 0 0 0 0 }
	 * 		{ 0 0 0 0 0 1 0 0 }
	 * 		{ 0 -1 0 0 0 0 0 0 }
	 * 		{ -1 0 0 -1 0 0 0 1 }
	 * 		{ 0 0 0 0 0 0 0 -1 }
	 * 		{ 0 1 0 0 0 -1 1 0 } }
	 *
	 * For some reason, the first time map_.count was called it would return 1,
	 * but then subsequent calls would return 0. Checking which matrix was
	 * considered 'equal' it was:
	 * { 	{ 0 0 1 0 -1 1 0 0 }
	 * 		{ 0 0 0 0 1 0 0 0 }
	 * 		{ -1 0 0 0 0 0 0 0 }
	 * 		{ 0 0 0 0 0 1 0 0 }
	 * 		{ 1 -1 0 0 0 0 0 -1 }
	 * 		{ -1 0 0 -1 0 0 0 1 }
	 * 		{ 0 0 0 0 0 0 0 -1 }
	 * 		{ 0 0 0 0 1 -1 1 0 } }
	 *
	 * Checking these matrices separately (see equiv_quiver_matrix_test) these are
	 * *not* equivalent, yet for some reason they were coming up as equal.
	 *
	 * I changed the hashcode, so that these matrices no longer have the same
	 * hash, which seems to have prevented this from cropping up.
	 *
	 * The test is now ignored as it takes ~3 mins to run.
	 */
	/*
	TEST(EquivMutationClassSize, Big) {
		std::string str = "{ { 0 1 0 0 0 0 0 0 } "
												"{ -1 0 1 0 0 0 0 0 } "
												"{ 0 -1 0 1 1 0 0 0 } "
												"{ 0 0 -1 0 0 0 0 0 } "
												"{ 0 0 -1 0 0 1 0 0 } "
												"{ 0 0 0 0 -1 0 0 -1 } "
												"{ 0 0 0 0 0 0 0 -1 } "
												"{ 0 0 0 0 0 1 1 0 } }";
		EquivQuiverMatrix m(str);

		int result = equivsize::Size(m);
		EXPECT_TRUE(result > 0);
	}
	*/

	/*
	 * This is another example of a matrix which appeared to be in the map, even
	 * though the matrix returned by find is not actually equivalent to it. This
	 * one did not get fixed by changing the hashcode, so I have added an extra
	 * call to map_.find after map_.count to ensure that this cannot cause a crash
	 * - but perhaps it will run a bit slower.
	 */
	/*
	TEST(EquivMutationClassSize, Big) {
		std::string str = "{ { 0 1 0 0 0 0 0 1 } "
												"{ -1 0 1 0 0 0 0 0 } "
												"{ 0 -1 0 1 0 0 0 0 } "
												"{ 0 0 -1 0 0 0 0 0 } "
												"{ 0 0 0 0 0 0 1 0 } "
												"{ 0 0 0 0 0 0 1 -1 } "
												"{ 0 0 0 0 -1 -1 0 0 } "
												"{ -1 0 0 0 0 1 0 0 } }";
		EquivQuiverMatrix m(str);

		int result = equivsize::Size(m);
		EXPECT_TRUE(result > 0);
	}
	*/
}
