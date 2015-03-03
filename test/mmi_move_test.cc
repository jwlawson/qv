/**
 * mmi_move_test.cc
 */
#include "gtest/gtest.h"

#include "mmi_move.h"
#include "mutation_class.h"

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
		EXPECT_EQ(0, (*app[0].submatrix_)[0]);
		EXPECT_EQ(1, (*app[0].submatrix_)[1]);
		EXPECT_EQ(2, (*app[0].submatrix_)[2]);
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
		EXPECT_EQ(0, (*app[0].submatrix_)[0]);
		EXPECT_EQ(1, (*app[0].submatrix_)[1]);
		EXPECT_EQ(2, (*app[0].submatrix_)[2]);
		EXPECT_EQ(1, (*app[1].submatrix_)[0]);
		EXPECT_EQ(2, (*app[1].submatrix_)[1]);
		EXPECT_EQ(3, (*app[1].submatrix_)[2]);
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
		EXPECT_EQ(app.size(), 2);
		/* Get the same submatrix twice, but with different permutations. */
		EXPECT_EQ(1, (*app[0].submatrix_)[0]);
		EXPECT_EQ(4, (*app[0].submatrix_)[1]);
		EXPECT_EQ(6, (*app[0].submatrix_)[2]);
		EXPECT_EQ(8, (*app[0].submatrix_)[3]);

		EXPECT_EQ(1, (*app[1].submatrix_)[0]);
		EXPECT_EQ(4, (*app[1].submatrix_)[1]);
		EXPECT_EQ(6, (*app[1].submatrix_)[2]);
		EXPECT_EQ(8, (*app[1].submatrix_)[3]);

		EXPECT_EQ(0, (*app[0].perm_)[0]);
		EXPECT_EQ(2, (*app[0].perm_)[1]);
		EXPECT_EQ(1, (*app[0].perm_)[2]);
		EXPECT_EQ(3, (*app[0].perm_)[3]);

		EXPECT_EQ(0, (*app[1].perm_)[0]);
		EXPECT_EQ(2, (*app[1].perm_)[1]);
		EXPECT_EQ(3, (*app[1].perm_)[2]);
		EXPECT_EQ(1, (*app[1].perm_)[3]);
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
		EXPECT_EQ(0, (*app[0].submatrix_)[0]);
		EXPECT_EQ(1, (*app[0].submatrix_)[1]);
		EXPECT_EQ(2, (*app[0].submatrix_)[2]);
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
	TEST(MMIMove, ApplicableTwice) {
		int v[] = {0, 1,-1, 0, 0, 0,
							-1, 0, 1,-1, 1,-1,
							 1,-1, 0, 1, 0, 0,
							 0, 1,-1, 0,-1, 0,
							 0,-1, 0, 1, 0, 1,
							 0, 1, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> init = std::make_shared<IntMatrix>(6, 6, v);

		int ma[] = {0,-1, 1,-1,
								1, 0,-1, 0,
							 -1, 1, 0, 1,
							  1, 0,-1, 0};
		IntMatrix m(4, 4, ma);
		int na[] = {0, 1, 0,-1,
							 -1, 0, 1, 0,
							  0,-1, 0, 1,
								1, 0,-1, 0};
		IntMatrix n(4, 4, na);
		std::vector<int> conn(2);
		conn[0] = 0;
		conn[1] = 3;
		std::vector<MMIMove::ConnReq> req(2);
		req[0] = mmi_conn::ConnectedTo(3);
		req[1] = mmi_conn::ConnectedTo(0);

		MMIMove move(m, n, conn, req);
		AVec app = move.applicable_submatrices(*init);
		ASSERT_FALSE(app.empty());
	}
	TEST(MMIMove, ConnectedToTrueForComplicated) {
		int v[] = {0, 1,-1, 0, 0, 0,
							-1, 0, 1,-1, 1,-1,
							 1,-1, 0, 1, 0, 0,
							 0, 1,-1, 0,-1, 0,
							 0,-1, 0, 1, 0, 1,
							 0, 1, 0, 0,-1, 0};
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
		MMIMove::ConnReq req = mmi_conn::ConnectedTo(3);
		EXPECT_TRUE(req(s, 0));
		EXPECT_TRUE(req(s, 1));
		EXPECT_TRUE(req(s, 2));
		EXPECT_FALSE(req(s, 3));

		std::vector<int> sub1(4);
		for(int i = 0; i < 4; i++) {
			sub1[i] = i;
		}
		std::vector<int> perm1(4);
		perm1[0] = 1;
		perm1[1] = 0;
		perm1[2] = 2;
		perm1[3] = 3;

		mmi_conn::Submatrix s1(check, std::move(sub1), std::move(perm1));
		MMIMove::ConnReq req1 = mmi_conn::ConnectedTo(3);
		EXPECT_TRUE(req1(s1, 0));
		EXPECT_TRUE(req1(s1, 1));
		EXPECT_TRUE(req1(s1, 2));
		EXPECT_FALSE(req1(s1, 3));
	}
	TEST(MMIMove, MoveWorksWithPermutation) {
		int v[] = { 0, 1,-1, 0,
							 -1, 0, 1,-1,
							  1,-1, 0, 1,
								0, 1,-1, 0};
		std::shared_ptr<IntMatrix> init = std::make_shared<IntMatrix>(4, 4, v);
		int w[] = { 0,-1, 0, 1,
								1, 0, 1,-1,
								0,-1, 0, 1,
							 -1, 1,-1, 0};
		std::shared_ptr<IntMatrix> mova = std::make_shared<IntMatrix>(4, 4, w);
		int x[] = { 0, 2,-1, 0,
							 -2, 0, 1,-2,
							  1,-2, 0, 1,
								0, 2,-1, 0};
		std::shared_ptr<IntMatrix> exp = std::make_shared<IntMatrix>(4, 4, x);
		int y[] = { 0,-1, 0, 2,
								1, 0, 1,-2,
								0,-1, 0, 2,
							 -2, 1,-2, 0};
		std::shared_ptr<IntMatrix> movb = std::make_shared<IntMatrix>(4, 4, y);

		MMIMove move(*mova, *movb, std::vector<int>(0));
		AVec app = move.applicable_submatrices(init);
		ASSERT_FALSE(app.empty());
		std::shared_ptr<IntMatrix> res = std::make_shared<IntMatrix>(4, 4);
		move.move(app[0], *res);
		EXPECT_TRUE(res->equals(*exp));
		EXPECT_TRUE(exp->equals(*res));
	}
	TEST(MMIMove, CheckPermutedMoveWithLineToRequirement) {
		int v[] = { 0, 0,-1,-1, 1, 0,
							  0, 0, 0,-1, 0,-1,
								1, 0, 0, 0,-1, 1,
								1, 1, 0, 0,-1, 0,
							 -1, 0, 1, 1, 0, 0,
							  0, 1,-1, 0, 0, 0};
		std::shared_ptr<IntMatrix> init = std::make_shared<IntMatrix>(6, 6, v);
		int ma[] = { 0, 0,-1, 1,
								 0, 0,-1, 1,
								 1, 1, 0,-1,
								-1,-1, 1, 0 };
		IntMatrix m(4, 4, ma);
		int na[] = { 0, 0,-2, 2,
								 0, 0,-2, 2,
								 2, 2, 0,-1,
								-2,-2, 1, 0 };
		IntMatrix n(4, 4, na);

		std::vector<int> con(2);
		con[0] = 0;
		con[1] = 1;
		std::vector<MMIMove::ConnReq> req(2);
		req[0] = mmi_conn::LineTo(1);
		req[1] = mmi_conn::LineTo(0);

		MMIMove move(m, n, con, req);

		AVec app = move.applicable_submatrices(init);
		ASSERT_FALSE(app.empty());
		IntMatrix res(6, 6);
		move.move(app[0], res);

		int w[] = { 0, 0,-2,-2, 1, 0,
							  0, 0, 0,-1, 0,-1,
								2, 0, 0, 0,-2, 1,
								2, 1, 0, 0,-2, 0,
							 -1, 0, 2, 2, 0, 0,
							  0, 1,-1, 0, 0, 0};
		IntMatrix exp(6, 6, w);

		EXPECT_TRUE(exp.equals(res));
		EXPECT_TRUE(res.equals(exp));
	}
	TEST(MMIMove, CheckNoMoveForInvalidPermutedLineTo) {
		int v[] = { 0, 0,-1,-1, 1, 0, 0,
							  0, 0, 0,-1, 0,-1,-1,
								1, 0, 0, 0,-1, 1, 0,
								1, 1, 0, 0,-1, 0, 0,
							 -1, 0, 1, 1, 0, 0, 0,
							  0, 1,-1, 0, 0, 0, 1,
								0, 1, 0, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> init = std::make_shared<IntMatrix>(7, 7, v);
		int ma[] = { 0, 0,-1, 1,
								 0, 0,-1, 1,
								 1, 1, 0,-1,
								-1,-1, 1, 0 };
		IntMatrix m(4, 4, ma);
		int na[] = { 0, 0,-2, 2,
								 0, 0,-2, 2,
								 2, 2, 0,-1,
								-2,-2, 1, 0 };
		IntMatrix n(4, 4, na);

		std::vector<int> con(2);
		con[0] = 0;
		con[1] = 1;
		std::vector<MMIMove::ConnReq> req(2);
		req[0] = mmi_conn::LineTo(1);
		req[1] = mmi_conn::LineTo(0);

		MMIMove move(m, n, con, req);

		AVec app = move.applicable_submatrices(init);
		ASSERT_TRUE(app.empty());
	}
	TEST(MMIMove, CheckPermutedMoveWithConnectedToRequirement) {
		int v[] = { 0, 0,-1,-1, 1, 0, 0,
							  0, 0, 0,-1, 0,-1,-1,
								1, 0, 0, 0,-1, 1, 0,
								1, 1, 0, 0,-1, 0, 0,
							 -1, 0, 1, 1, 0, 0, 0,
							  0, 1,-1, 0, 0, 0, 1,
								0, 1, 0, 0, 0,-1, 0};
		std::shared_ptr<IntMatrix> init = std::make_shared<IntMatrix>(7, 7, v);
		int ma[] = { 0, 0,-1, 1,
								 0, 0,-1, 1,
								 1, 1, 0,-1,
								-1,-1, 1, 0 };
		IntMatrix m(4, 4, ma);
		int na[] = { 0, 0,-2, 2,
								 0, 0,-2, 2,
								 2, 2, 0,-1,
								-2,-2, 1, 0 };
		IntMatrix n(4, 4, na);

		std::vector<int> con(2);
		con[0] = 0;
		con[1] = 1;
		std::vector<MMIMove::ConnReq> req(2);
		req[0] = mmi_conn::ConnectedTo(1);
		req[1] = mmi_conn::ConnectedTo(0);

		MMIMove move(m, n, con, req);

		AVec app = move.applicable_submatrices(init);
		ASSERT_FALSE(app.empty());
		IntMatrix res(7, 7);
		move.move(app[0], res);

		int w[] = { 0, 0,-2,-2, 1, 0, 0,
							  0, 0, 0,-1, 0,-1,-1,
								2, 0, 0, 0,-2, 1, 0,
								2, 1, 0, 0,-2, 0, 0,
							 -1, 0, 2, 2, 0, 0, 0,
							  0, 1,-1, 0, 0, 0, 1,
								0, 1, 0, 0, 0,-1, 0};
		IntMatrix exp(7, 7, w);

		EXPECT_TRUE(exp.equals(res));
		EXPECT_TRUE(res.equals(exp));
	}
	TEST(MMIMove, MoveAppliesTwice) {
		std::shared_ptr<IntMatrix> init = std::make_shared<IntMatrix>("{ { 0 -1 1 0 0 0 } { 1 0 0 -1 1 -1 } { -1 0 0 1 0 0 } { 0 1 -1 0 -1 0 } { 0 -1 0 1 0 1 } { 0 1 0 0 -1 0 } }");

		int ma[] = {0,-1, 1,-1,
								1, 0,-1, 0,
							 -1, 1, 0, 1,
							  1, 0,-1, 0};
		IntMatrix m(4, 4, ma);
		int na[] = {0, 1, 0,-1,
							 -1, 0, 1, 0,
							  0,-1, 0, 1,
								1, 0,-1, 0};
		IntMatrix n(4, 4, na);
		std::vector<int> conn(2);
		conn[0] = 0;
		conn[1] = 3;
		std::vector<MMIMove::ConnReq> req(2);
		req[0] = mmi_conn::ConnectedTo(3);
		req[1] = mmi_conn::ConnectedTo(0);

		std::shared_ptr<MMIMove> move = std::make_shared<MMIMove>(m, n, conn, req);

		AVec app = move->applicable_submatrices(init);
		ASSERT_FALSE(app.empty());
	}
	TEST(MMIMove, MoveOnSingleUnconnectedArrow) {
		int v[] = {0, -1, 0, 0, 1, 0, -1, -1, 0, 1, 0, 1, 0, 1, -1, 0};
		std::shared_ptr<IntMatrix> m = std::make_shared<IntMatrix>(4, 4, v);
		auto move = std::make_shared<MMIMove>(IntMatrix("{ { 0 -1 } { 1 0 } }"),
			IntMatrix("{ { 0 1 } { -1 0 } }"), std::vector<int>({ 1 }),
			std::vector<MMIMove::ConnReq>({ mmi_conn::Unconnected() }));

		AVec app = move->applicable_submatrices(m);
		ASSERT_FALSE(app.empty());

		auto n = std::make_shared<IntMatrix>(4, 4);
		move->move(app[0], *n);

		int w[] = { 0, 1, 0, 0, -1, 0, -1, -1, 0, 1, 0, 1, 0, 1, -1, 0};
		IntMatrix exp(4, 4, w);
		EXPECT_TRUE(exp.equals(*n));
	}
namespace {
	std::shared_ptr<cluster::MMIMove> make_move(const std::string& a,
			const std::string& b, std::initializer_list<int> c,
			std::initializer_list<cluster::MMIMove::ConnReq> r) {
		return std::make_shared<cluster::MMIMove>(
				cluster::IntMatrix(a), cluster::IntMatrix(b),
				std::vector<int>(c), std::vector<cluster::MMIMove::ConnReq>(r));
	}
	std::shared_ptr<cluster::EquivQuiverMatrix> matrix(const std::string& a) {
		return std::make_shared<cluster::EquivQuiverMatrix>(a);
	}
	template<typename F>
	std::shared_ptr<cluster::MMIMove> make_move(const std::string& a,
			const std::string& b, std::initializer_list<int> c,
			std::initializer_list<cluster::MMIMove::ConnReq> r,
			cluster::mmi_conn::Finite<F> atob) {
		auto res = std::make_shared<cluster::MMIMove>(
				cluster::IntMatrix(a), cluster::IntMatrix(b),
				std::vector<int>(c), std::vector<cluster::MMIMove::ConnReq>(r));
		res->finite_req_atob(atob);
		return res;
	}
}
using namespace cluster::mmi_conn;
TEST(MMIMove, SimpleMoveApplicableToDouble) {
	auto move = make_move("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }",
			"{ { 0 -1 1 } { 1 0 -1 } { -1 1 0 } }", {0, 2}, {Unconnected(), Line()});
	IntMatrix init("{ { 0 2 -1 0 0 0 } { -2 0 1 0 0 0 } { 1 -1 0 1 0 1 } { 0 0 -1 0 1 0 } { 0 0 0 -1 0 0 } { 0 0 -1 0 0 0 } }");
	AVec app = move->applicable_submatrices(init);
	ASSERT_FALSE(app.empty());
}
TEST(MMIMove, MixOfLineAndConnectedTo) {
	auto move = make_move("{ { 0 -1 1 0 0 0 } { 1 0 -1 0 0 0 } { -1 1 0 1 0 0 } "
			"{ 0 0 -1 0 1 1 } { 0 0 0 -1 0 0 } { 0 0 0 -1 0 0 } }",
			"{ { 0 -1 1 0 0 0 } { 1 0 -1 0 0 0 } { -1 1 0 -1 1 1 } "
			"{ 0 0 1 0 -1 -1 } { 0 0 -1 1 0 0 } { 0 0 -1 1 0 0 } }",
			{0, 1, 5}, {ConnectedTo(5), Line(), ConnectedTo(0)});
	IntMatrix init("{ { 0 1 0 0 0 0 0 0 0 } { -1 0 1 0 0 0 0 0 -1 } { 0 -1 0 1 0 0 -1 0 0 } { 0 0 -1 0 1 -1 1 0 1 } { 0 0 0 -1 0 1 0 0 0 } { 0 0 0 1 -1 0 0 0 -1 } { 0 0 1 -1 0 0 0 1 0 } { 0 0 0 0 0 0 -1 0 0 } { 0 1 0 -1 0 1 0 0 0 } }");
	AVec app = move->applicable_submatrices(init);
	ASSERT_FALSE(app.empty());
}
namespace {
	struct FiniteCheck {
		bool operator()(EquivQuiverMatrix & mat) {
			MutationClass cl(mat);
			std::cout << mat << std::endl;
			return cl.is_finite();
		}
	};
	typedef mmi_conn::Finite<FiniteCheck> FinReq;
}
TEST(MMIMove, FiniteReqIsAppl) {
	auto move = make_move("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }",
			"{ { 0 -1 1 } { 1 0 -1 } { -1 1 0 } }", {0, 2}, {Unconnected(), Line()});
	/* Note the zero row needed to ensure that the size is correct */
	mmi_conn::Finite<FiniteCheck> req(EquivQuiverMatrix("{ { 0 0 1 } { 0 0 0 } { -1 0 0 } }"));
	move->finite_req_atob(req);

	EquivQuiverMatrix a("{ { 0 1 0 0 } { -1 0 1 0 } { 0 -1 0 1 } { 0 0 -1 0 } }");
	AVec app = move->applicable_submatrices(a);
	ASSERT_FALSE(app.empty());
}
}

