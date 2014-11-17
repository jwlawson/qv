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
	TEST(MMIMove, BigExample) {
		int v[] = {	0, 0, 2, 0, 0, 1, 0, 0, 0, 0,
								0, 0, 0, 0, -1, 0, 1, 0, 1, 0,
								-2, 0, 0, 0, 0, -1, 0, 0, 0, 0,
								0, 0, 0, 0, 0, -1, 1, 0, 0, 0,
								0, 1, 0, 0, 0, 0, -1, 0, -1, 0,
								-1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
								0, -1, 0, -1, 1, 0, 0, 0, 0, 0,
								0, 0, 0, 0, 0, 0, 0, 0, -1, 1,
								0, -1, 0, 0, 1, 0, 0, 1, 0, 1,
								0, 0, 0, 0, 0, 0, 0, -1, -1, 0};
		int w[] = {	0, 1, -1, 1,
								-1, 0, 1, 0,
								1, -1, 0, -1,
								-1, 0, 1, 0};
		IntMatrix check(10,10,v);
		
		std::vector<int> con(2);
		IntMatrix mov(4, 4, w);
		con[0] = 1;
		con[1] = 3;
		IntMatrix n;
		MMIMove move(mov, n, con);

		auto app = move.applicable_submatrices(check);

		ASSERT_FALSE(app.empty());
		EXPECT_EQ(app.size(), 1);
		EXPECT_EQ(app[0][0], 1);
		EXPECT_EQ(app[0][1], 4);
		EXPECT_EQ(app[0][2], 6);
		EXPECT_EQ(app[0][3], 8);
	}
	TEST(MMIMove, LineReqTrueForSmallValid) {
		int v[] = { 0, 1,-1, 1,
							 -1, 0, 1, 0,
							  1,-1, 0, 0,
							 -1, 0, 0, 0};
		IntMatrix check(4, 4, v);

		std::vector<MMIMove::ConnReq> req(1);
		req[0] = mmi_conn::Line();

		std::vector<int> sub(3);
		for(int i = 0; i < 3; ++i) {
			sub[i] = i;
		}
		mmi_conn::Submatrix s(check, sub, sub);
		EXPECT_TRUE(req[0](s, 0));
		EXPECT_FALSE(req[0](s, 1));
		EXPECT_FALSE(req[0](s, 2));
	}
	TEST(MMIMove, LineReqFalseForSmallNotValid) {
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1,-1,
							  1,-1, 0, 0, 0, 0,
							 -1, 0, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 1, 0, 0,-1, 0};
		IntMatrix check(6, 6, v);

		std::vector<MMIMove::ConnReq> req(1);
		req[0] = mmi_conn::Line();

		std::vector<int> sub(4);
		for(int i = 0; i < 4; ++i) {
			sub[i] = i;
		}
		mmi_conn::Submatrix s(check, sub, sub);
		EXPECT_FALSE(req[0](s, 0));
		EXPECT_FALSE(req[0](s, 1));
		EXPECT_FALSE(req[0](s, 2));
		EXPECT_FALSE(req[0](s, 3));
	}
	TEST(MMIMove, LineReqInMove) {
		int v[] = { 0, 1,-1, 1,
							 -1, 0, 1, 0,
							  1,-1, 0, 0,
							 -1, 0, 0, 0};
		IntMatrix check(4, 4, v);

		int w[] = { 0, 1, -1, -1, 0, 1, 1, -1, 0};
		IntMatrix m(3, 3, w);
		IntMatrix n;
		std::vector<int> conn(1);
		conn[0] = 0;

		std::vector<MMIMove::ConnReq> req(1);
		req[0] = mmi_conn::Line();

		MMIMove move(m, n, conn, req);
		auto app = move.applicable_submatrices(check);
		ASSERT_EQ(app.size(), 1);
		EXPECT_EQ(app[0][0], 0);
		EXPECT_EQ(app[0][1], 1);
		EXPECT_EQ(app[0][2], 2);
	}

}

