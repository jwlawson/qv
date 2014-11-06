/**
 * mmi_move_test.cc
 */
#include "gtest/gtest.h"

#include "mmi_move.h"

namespace cluster {

	TEST(MMIMove, MoveAppliesToItself) {
		int v[] = {0,1,0,-1,0,1,0,-1,0};
		IntMatrix m(3,3,v);
		std::vector<int> con(2);
		con[0] = 0;
		con[1] = 2;
		IntMatrix n;
		MMIMove move(m, n, con);

		std::vector<std::vector<int>> app = move.applicable_submatrices(m);

		ASSERT_FALSE(app.empty());
		EXPECT_EQ(app.size(), 1);
		ASSERT_EQ(app[0].size(), 3);
		EXPECT_EQ(app[0][0], 0);
		EXPECT_EQ(app[0][1], 1);
		EXPECT_EQ(app[0][2], 2);
	}

	TEST(MMIMove, NotAppMove) {
		int v[] = {0,1,0,-1,0,1,0,-1,0};
		IntMatrix m(3,3,v);
		std::vector<int> con(2);
		con[0] = 0;
		con[1] = 2;
		IntMatrix n;
		MMIMove move(m, n, con);

		int w[] = {0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 0, 0, -1, 0, 0};
		IntMatrix check(4, 4, w);

		auto app = move.applicable_submatrices(check);

		ASSERT_TRUE(app.empty());
	}

	TEST(MMIMove, MultiApp) {
		int v[] = {0,1,0,-1,0,1,0,-1,0};
		IntMatrix m(3,3,v);
		std::vector<int> con(2);
		con[0] = 0;
		con[1] = 2;
		IntMatrix n;
		MMIMove move(m, n, con);

		int w[] = {0,1,0,0, -1,0,1,0, 0,-1,0,1, 0,0,-1,0};
		IntMatrix check(4,4,w);

		auto app = move.applicable_submatrices(check);

		ASSERT_FALSE(app.empty());
		ASSERT_EQ(app.size(), 2);
		EXPECT_EQ(app[0][0], 0);
		EXPECT_EQ(app[0][1], 1);
		EXPECT_EQ(app[0][2], 2);
		EXPECT_EQ(app[1][0], 1);
		EXPECT_EQ(app[1][1], 2);
		EXPECT_EQ(app[1][2], 3);
	}

}

