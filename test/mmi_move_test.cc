/**
 * mmi_move_test.cc
 */
#include "gtest/gtest.h"

#include "mmi_move.h"

namespace cluster {

	namespace {
		typedef std::vector<MMIMove::Applicable> AVec;
	}

	TEST(MMIMove, MoveAppliesToItself) {
		int v[] = {0,1,0,-1,0,1,0,-1,0};
		IntMatrix m(3,3,v);
		std::vector<int> con(2);
		con[0] = 0;
		con[1] = 2;
		IntMatrix n;
		MMIMove move(m, n, con);

		AVec app = move.applicable_submatrices(m);

		ASSERT_FALSE(app.empty());
		EXPECT_EQ(app.size(), 1);
		ASSERT_EQ(app[0].submatrix_->size(), 3);
		EXPECT_EQ((*app[0].submatrix_)[0], 0);
		EXPECT_EQ((*app[0].submatrix_)[1], 1);
		EXPECT_EQ((*app[0].submatrix_)[2], 2);
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

		AVec app = move.applicable_submatrices(check);

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

		AVec app = move.applicable_submatrices(check);

		ASSERT_FALSE(app.empty());
		ASSERT_EQ(app.size(), 2);
		EXPECT_EQ((*app[0].submatrix_)[0], 0);
		EXPECT_EQ((*app[0].submatrix_)[1], 1);
		EXPECT_EQ((*app[0].submatrix_)[2], 2);
		EXPECT_EQ((*app[1].submatrix_)[0], 1);
		EXPECT_EQ((*app[1].submatrix_)[1], 2);
		EXPECT_EQ((*app[1].submatrix_)[2], 3);
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

		AVec app = move.applicable_submatrices(check);

		ASSERT_FALSE(app.empty());
		EXPECT_EQ(app.size(), 1);
		EXPECT_EQ((*app[0].submatrix_)[0], 1);
		EXPECT_EQ((*app[0].submatrix_)[1], 4);
		EXPECT_EQ((*app[0].submatrix_)[2], 6);
		EXPECT_EQ((*app[0].submatrix_)[3], 8);
	}
	TEST(MMIMove, NoMoveForSmallerMatrices) {
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1,-1,
							  1,-1, 0, 0, 0, 0,
							 -1, 0, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 1, 0, 0,-1, 0};
		int w[] = {	0, 1, -1, 1,
								-1, 0, 1, 0,
								1, -1, 0, -1,
								-1, 0, 1, 0};
		IntMatrix ma(6, 6, v);
		IntMatrix mb;
		IntMatrix c(4, 4, w);
		std::vector<int> con(2);
		con[0] = 1;
		con[1] = 3;

		MMIMove move(ma, mb, con);

		AVec app = move.applicable_submatrices(c);
		ASSERT_TRUE(app.empty());
	}
	TEST(MMIMove, LineReqTrueForSmallValid) {
		int v[] = { 0, 1,-1, 1,
							 -1, 0, 1, 0,
							  1,-1, 0, 0,
							 -1, 0, 0, 0};
		std::shared_ptr<IntMatrix> check=std::make_shared<IntMatrix>(4, 4, v);

		MMIMove::ConnReq req = mmi_conn::Line();

		std::vector<int> sub(3);
		for(int i = 0; i < 3; ++i) {
			sub[i] = i;
		}
		std::vector<int> perm(3);
		for(int i = 0; i < 3; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_TRUE(req(s, 2));
	}
	TEST(MMIMove, LineReqFalseForSmallNotValid) {
		/* 6 */
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1,-1,
							  1,-1, 0, 0, 0, 0,
							 -1, 0, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 1, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);

		MMIMove::ConnReq req = mmi_conn::Line();

		std::vector<int> sub(4);
		for(int i = 0; i < 4; ++i) {
			sub[i] = i;
		}
		std::vector<int> perm(3);
		for(int i = 0; i < 3; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		EXPECT_TRUE(req(s, 0));
		EXPECT_FALSE(req(s, 1));
		EXPECT_TRUE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, LineReqTrueForNoExtension) {
		int v[] = { 0, 1,-1, 1,
							 -1, 0, 1, 0,
							  1,-1, 0, 0,
							 -1, 0, 0, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(4, 4, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(3);
		for(int i = 0; i < 3; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::Line();
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_TRUE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, LineReqFalseForConnectingLine) {
		/* 2 */
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1, 0,
							  1,-1, 0,-1, 0,-1,
							 -1, 0, 1, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 0, 1, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(3);
		for(int i = 0; i < 3; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::Line();
		EXPECT_TRUE(req(s, 0));
		EXPECT_FALSE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, LineReqInMove) {
		int v[] = { 0, 1,-1, 1,
							 -1, 0, 1, 0,
							  1,-1, 0, 0,
							 -1, 0, 0, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(4, 4, v);

		int w[] = { 0, 1, -1, -1, 0, 1, 1, -1, 0};
		IntMatrix m(3, 3, w);
		IntMatrix n;
		std::vector<int> conn(1);
		conn[0] = 0;

		std::vector<MMIMove::ConnReq> req(1);
		req[0] = mmi_conn::Line();

		MMIMove move(m, n, conn, req);
		AVec app = move.applicable_submatrices(check);
		ASSERT_EQ(app.size(), 1);
		EXPECT_EQ((*app[0].submatrix_)[0], 0);
		EXPECT_EQ((*app[0].submatrix_)[1], 1);
		EXPECT_EQ((*app[0].submatrix_)[2], 2);
	}
	TEST(MMIMove, LineToReqFalseForNoExtension) {
		int v[] = { 0, 1,-1, 1,
							 -1, 0, 1, 0,
							  1,-1, 0, 0,
							 -1, 0, 0, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(4, 4, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::LineTo(1);
		EXPECT_FALSE(req(s, 0));
		EXPECT_FALSE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_FALSE(req(s, 3));
	}
	TEST(MMIMove, LineToReqTrueForValidSubmatrix) {
		/* Quiver 2 */
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1, 0,
							  1,-1, 0,-1, 0,-1,
							 -1, 0, 1, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 0, 1, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::LineTo(2);
		EXPECT_FALSE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_FALSE(req(s, 3));
	}
	TEST(MMIMove, LineToReqFalseForUnconnectedLine) {
		/* 4 */
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1, 0,
							  1,-1, 0,-1, 0, 0,
							 -1, 0, 1, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 0, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::LineTo(3);
		EXPECT_FALSE(req(s, 0));
		EXPECT_FALSE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_FALSE(req(s, 3));
	}
	TEST(MMIMove, LineToReqFalseForConnectedNotLine) {
		/* 3 */
		int v[] = { 0, 1,-1, 1, 0, 0, 0,
							 -1, 0, 1, 0, 1, 0, 0,
							  1,-1, 0,-1, 0, 0, 1,
							 -1, 0, 1, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,-1,
								0, 0, 0, 0,-1, 0, 1,
								0, 0,-1, 0, 1, 1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(7, 7, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::LineTo(2);
		EXPECT_FALSE(req(s, 0));
		EXPECT_FALSE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_FALSE(req(s, 3));
	}
	TEST(MMIMove, LineToReqFalseForLineToInitialVertex) {
		/* 6 */
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1,-1,
							  1,-1, 0, 0, 0, 0,
							 -1, 0, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 1, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);

		MMIMove::ConnReq req = mmi_conn::LineTo(1);

		std::vector<int> sub(4);
		for(int i = 0; i < 4; ++i) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		EXPECT_FALSE(req(s, 0));
		EXPECT_FALSE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_FALSE(req(s, 3));
	}
	TEST(MMIMove, ConnectedToTrueForValidSubmatrix) {
		/* 3 */
		int v[] = { 0, 1,-1, 1, 0, 0, 0,
							 -1, 0, 1, 0, 1, 0, 0,
							  1,-1, 0,-1, 0, 0, 1,
							 -1, 0, 1, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,-1,
								0, 0, 0, 0,-1, 0, 1,
								0, 0,-1, 0, 1, 1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(7, 7, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::ConnectedTo(2);
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, ConnectedToTrueForLineTo) {
		/* Quiver 2 */
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1, 0,
							  1,-1, 0,-1, 0,-1,
							 -1, 0, 1, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 0, 1, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::ConnectedTo(2);
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, ConnectedToTrueForUnConnected) {
		int v[] = { 0, 1,-1, 1, 1, 0, 0,
							 -1, 0, 1, 0, 0, 1, 0,
							  1,-1, 0,-1, 0, 0, 1,
							 -1, 0, 1, 0, 0, 0, 0,
							 -1, 0, 0, 0, 0, 0, 0,
								0,-1, 0, 0, 0, 0, 0,
								0, 0,-1, 0, 0, 0, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(7, 7, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::ConnectedTo(1);
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_TRUE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, ConnectedToFalseForConnectionThroughSubmatrix) {
		int v[] = { 0, 1,-1, 1, 0, 0, 0, 0,
							 -1, 0, 1, 0, 1, 0, 0, 0,
							  1,-1, 0,-1, 0,-1, 1, 0,
							 -1, 0, 1, 0, 0, 0, 0,-1,
							  0,-1, 0, 0, 0, 1, 0, 0,
								0, 0, 1, 0,-1, 0, 0, 0,
								0, 0,-1, 0, 0, 0, 0, 1,
								0, 0, 0, 1, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(8, 8, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::ConnectedTo(1);
		EXPECT_TRUE(req(s, 0));
		EXPECT_FALSE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_FALSE(req(s, 3));
	}
	TEST(MMIMove, UnConnectedFalseForLineTo) {
		/* Quiver 2 */
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1, 0,
							  1,-1, 0,-1, 0,-1,
							 -1, 0, 1, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 0, 1, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::Unconnected();
		EXPECT_TRUE(req(s, 0));
		EXPECT_FALSE(req(s, 1));
		EXPECT_FALSE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, UnConnectedTrueForNoExtension) {
		int v[] = { 0, 1,-1, 1,
							 -1, 0, 1, 0,
							  1,-1, 0, 0,
							 -1, 0, 0, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(4, 4, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::Unconnected();
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_TRUE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, UnConnectedTrueForValid) {
		/* 6 */
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1,-1,
							  1,-1, 0, 0, 0, 0,
							 -1, 0, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 1, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);

		MMIMove::ConnReq req = mmi_conn::Unconnected();

		std::vector<int> sub(4);
		for(int i = 0; i < 4; ++i) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_TRUE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}
	TEST(MMIMove, UnConnectedTrueForMultipleLines) {
		int v[] = { 0, 1,-1, 1, 0, 0, 0,
							 -1, 0, 1, 0, 1, 0, 0,
							  1,-1, 0,-1, 0, 1, 1,
							 -1, 0, 1, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 0, 0,
								0, 0,-1, 0, 0, 0, 0,
								0, 0,-1, 0, 0, 0, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(4, 4, v);
		std::vector<int> sub(4);
		for(int i = 0; i < 4; i++) {
			sub[i] = i;
		}
		std::vector<int> perm(4);
		for(int i = 0; i < 4; ++i) {
			perm[i] = i;
		}
		mmi_conn::Submatrix s(check, std::move(sub), std::move(perm));
		MMIMove::ConnReq req = mmi_conn::Unconnected();
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_TRUE(req(s, 2));
		EXPECT_TRUE(req(s, 3));
	}		
	TEST(MMIMove, MoveGivesRightMatrix) {
		int v[] = { 0, 1,-1, 1, 0, 0,
							 -1, 0, 1, 0, 1,-1,
							  1,-1, 0, 0, 0, 0,
							 -1, 0, 0, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 1, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> check = std::make_shared<IntMatrix>(6, 6, v);
		int w[] = { 0, 1,-1, 1,
							 -1, 0, 1, 0,
							  1,-1, 0, 0,
							 -1, 0, 0, 0};
		int u[] = { 0, 2, 2, 2,
							 -2, 0, 2, 2,
							 -2,-2, 0, 2,
							 -2,-2,-2, 0};
		IntMatrix m(4, 4, w);
		std::vector<int> con(2);
		con[0] = 1;
		con[1] = 2;
		IntMatrix n(4, 4, u);
		MMIMove move(m, n, con);

		AVec app = move.applicable_submatrices(check);
		ASSERT_FALSE(app.empty());
		IntMatrix res(6, 6);
		move.move(app[0], res);

		int a[] = { 0, 2, 2, 2, 0, 0,
							 -2, 0, 2, 2, 1,-1,
							 -2,-2, 0, 2, 0, 0,
							 -2,-2,-2, 0, 0, 0,
							  0,-1, 0, 0, 0, 1,
								0, 1, 0, 0,-1, 0};
		IntMatrix exp(6, 6, a);
		EXPECT_TRUE(exp.equals(res));
	}
}

