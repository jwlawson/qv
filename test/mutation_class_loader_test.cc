/*
 * mutation_class_loader_test.cc
 */

#include "gtest/gtest.h"
#include "quiver_matrix.h"
#include "dynkin.h"
#include "mutation_class_loader.h"

namespace cluster {

	TEST(MutationClassLoader, Initial) {
		QuiverMatrix mat = dynkin::A3;
		MutationClassLoader<QuiverMatrix> c(mat);
		EXPECT_TRUE(c.next().equals(mat));
	}

	TEST(MutationClassLoader, Second) {
		QuiverMatrix mat = dynkin::A4;
		MutationClassLoader<QuiverMatrix> c(mat);
		/* Take the first one. */
		QuiverMatrix f = c.next();
		/* Test the second one. */
		QuiverMatrix sec = c.next();

		QuiverMatrix exp(4, 4);
		mat.mutate(0, exp);

		EXPECT_TRUE(exp.equals(sec));
	}


}

