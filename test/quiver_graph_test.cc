/**
 * quiver_qraph_test.cc
 */
#include "gtest/gtest.h"

#include "equiv_quiver_matrix.h"
#include "quiver_graph.h"

namespace cluster {
TEST(QvGraph, Simple) {
	EquivQuiverMatrix m("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }");
	QuiverGraph<EquivQuiverMatrix> graph(m);

	int count_matrix = 0;
	for( auto iter = graph.begin(); iter != graph.end(); ++iter) {
		count_matrix++;
		int count_links = 0;
		for( auto l_iter = iter->second.begin(); l_iter != iter->second.end();
				++l_iter ) {
			count_links++;
		}
		EXPECT_EQ(3,count_links);
	}
	EXPECT_EQ(4,count_matrix);
}
}
