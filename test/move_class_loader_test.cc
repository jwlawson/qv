/**
 * move_class_loader_test.cc
 */
#include "gtest/gtest.h"

#include "move_class_loader.h"

namespace cluster {
	TEST(MoveClassLoader, NoMovesOnlyReturnsInitial) {
		int v[] = {0, 1, 2, -1, 0, 3, -2, -3, 0};
		std::shared_ptr<EquivQuiverMatrix> m = std::make_shared<EquivQuiverMatrix>(3, 3, v);
		std::vector<std::shared_ptr<MMIMove>> w;

		MoveClassLoader l(m, w, false);
		ASSERT_TRUE(l.has_next());
		auto n = l.next();
		EXPECT_TRUE(m->equals(*n));
		EXPECT_TRUE(n->equals(*m));
		EXPECT_FALSE(l.has_next());
	}
	TEST(MoveClassLoader, InitialIsAtDepth0) {
		int v[] = {0, 1, 2, -1, 0, 3, -2, -3, 0};
		std::shared_ptr<EquivQuiverMatrix> m = std::make_shared<EquivQuiverMatrix>(3, 3, v);
		std::vector<std::shared_ptr<MMIMove>> w;

		MoveClassLoader l(m, w);
		ASSERT_TRUE(l.has_next());
		auto n = l.next();
		EXPECT_EQ(l.depth().moves_, 0);
		EXPECT_EQ(l.depth().sinksource_, 0);
	}
	TEST(MoveClassLoader, SingleMoveReturnsTwo) {
		int v[] = {0, 1, -1, 0, -1, 0, 1, 1, 1, -1, 0, 0, 0, -1, 0, 0};
		std::shared_ptr<EquivQuiverMatrix> m = std::make_shared<EquivQuiverMatrix>(4, 4, v);

		int u[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};
		IntMatrix n(3, 3, u);
		int w[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		IntMatrix p(3, 3, w);

		std::vector<int> conn(1);
		conn[0] = 1;

		std::shared_ptr<MMIMove> mov = std::make_shared<MMIMove>(n, p, conn);
		std::vector<std::shared_ptr<MMIMove>> moves(1, mov);

		MoveClassLoader l(m, moves, false);

		ASSERT_TRUE(l.has_next());
		auto q = l.next();
		EXPECT_TRUE(m->equals(*q));
		ASSERT_TRUE(l.has_next());
		int x[] = {0, 2, -2, 0, -2, 0, 2, 1, 2, -2, 0, 0, 0, -1, 0, 0};
		EquivQuiverMatrix check(4, 4, x);
		auto r = l.next();
		EXPECT_TRUE(check.equals(*r));
		EXPECT_TRUE(r->equals(check));
		ASSERT_FALSE(l.has_next());
	}
	TEST(MoveClassLoader, MoveMatrixAtDepth1) {
		int v[] = {0, 1, -1, 0, -1, 0, 1, 1, 1, -1, 0, 0, 0, -1, 0, 0};
		std::shared_ptr<EquivQuiverMatrix> m = std::make_shared<EquivQuiverMatrix>(4, 4, v);

		int u[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};
		IntMatrix n(3, 3, u);
		int w[] = {0, 2, -2, -2, 0, 2, 2, -2, 0};
		IntMatrix p(3, 3, w);

		std::vector<int> conn(1);
		conn[0] = 1;

		std::shared_ptr<MMIMove> mov = std::make_shared<MMIMove>(n, p, conn);
		std::vector<std::shared_ptr<MMIMove>> moves(1, mov);

		MoveClassLoader l(m, moves, false);

		ASSERT_TRUE(l.has_next());
		auto r = l.next();
		ASSERT_TRUE(l.has_next());
		auto q = l.next();
		EXPECT_EQ(l.depth().moves_, 1);
	}
	TEST(MoveClassLoader, SizeOfFullClass) {
		int v[] = {0, 1,-1, 0, 0, 0,
							-1, 0, 1,-1, 1,-1,
							 1,-1, 0, 1, 0, 0,
							 0, 1,-1, 0,-1, 0,
							 0,-1, 0, 1, 0, 1,
							 0, 1, 0, 0,-1, 0};
		std::shared_ptr<EquivQuiverMatrix> init = std::make_shared<EquivQuiverMatrix>(6, 6, v);

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
		std::vector<std::shared_ptr<MMIMove>> moves(1, move);

		MoveClassLoader loader(init, moves, false);
		ASSERT_TRUE(loader.has_next());
		EXPECT_TRUE(init->equals(*loader.next()));

		ASSERT_TRUE(loader.has_next());
		auto p = loader.next();
		ASSERT_TRUE(loader.has_next());
		auto q = loader.next();
		ASSERT_FALSE(loader.has_next());
	}
	TEST(MoveClassLoader, SinkSourceClass) {
		int v[] = { 0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0};
		std::shared_ptr<EquivQuiverMatrix> init = std::make_shared<EquivQuiverMatrix>(4, 4, v);

		IntMatrix m("{ { 0 -1 -1 } { 1 0 1 } { 1 -1 0 } }");
		IntMatrix n("{ { 0 1 -1 } { -1 0 1 } { 1 -1 0 } }");
		std::vector<int> conn = {0};
		std::vector<MMIMove::ConnReq> req = {mmi_conn::Unconnected()};
		std::shared_ptr<MMIMove> move = std::make_shared<MMIMove>(m, n, conn, req);
		std::vector<std::shared_ptr<MMIMove>> moves = {move};

		MoveClassLoader loader(init, moves);
		int count = 0;
		while(loader.has_next()) {
			*loader.next();
			count++;
		}
		EXPECT_EQ(8, count);
		EXPECT_EQ(1, loader.depth().moves_);
		EXPECT_EQ(3, loader.depth().sinksource_);
	}
}

