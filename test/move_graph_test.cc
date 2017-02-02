/**
 * move_graph_test.cc
 *
 * Copyright 2015 John Lawson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "move_graph.h"

#include <algorithm>
#include "gtest/gtest.h"

namespace cluster {
namespace {
typedef std::vector<MMIMove::Applicable> AVec;
MMIMove make_move(const std::string& a,
		const std::string& b, std::initializer_list<int> c,
		std::initializer_list<MMIMove::ConnReq> r) {
	return MMIMove( cluster::IntMatrix(a), cluster::IntMatrix(b),
			std::vector<int>(c), std::vector<cluster::MMIMove::ConnReq>(r));
}
struct graph_contains {
	graph_contains(const EquivQuiverMatrix & m) : _matrix(m) {}
	bool operator()(const std::pair<cluster::EquivQuiverMatrix const* const,
			cluster::MoveGraph<cluster::EquivQuiverMatrix>::Link> pair) {
		return _matrix.equals(*pair.first);
	}
	private:
		const EquivQuiverMatrix & _matrix;
};
}
using namespace cluster::mmi_conn;
TEST(MoveGraph, SingleMoveAppliesOnce) {
	auto move = make_move("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }",
			"{ { 0 -1 1 } { 1 0 -1 } { -1 1 0 } }", {0, 2}, {Unconnected(), Line()});
	IntMatrix init("{ { 0 2 -1 0 0 0 } { -2 0 1 0 0 0 } { 1 -1 0 1 0 1 } "
			"{ 0 0 -1 0 1 0 } { 0 0 0 -1 0 0 } { 0 0 -1 0 0 0 } }");
	std::vector<MMIMove> vec = { move };

	MoveGraph<EquivQuiverMatrix> graph(init, vec);
	int count_matrix = 0;
	for( auto iter = graph.begin(); iter != graph.end(); ++iter) {
		count_matrix++;
		int count_links = 0;
		for( auto l_iter = iter->second.begin(); l_iter != iter->second.end();
				++l_iter ) {
			count_links++;
		}
		EXPECT_EQ(1,count_links);
	}
	EXPECT_EQ(2,count_matrix);
}
TEST(MoveGraph, SingleMoveAppliesTwice) {
	auto move = make_move("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }",
			"{ { 0 -1 1 } { 1 0 -1 } { -1 1 0 } }", {0, 2}, {Unconnected(), Line()});
	IntMatrix init("{ { 0 1 0 0 } { -1 0 1 0 } { 0 -1 0 1 } { 0 0 -1 0 } }");
	std::vector<MMIMove> vec = { move };

	MoveGraph<EquivQuiverMatrix> graph(init, vec);

	int count_matrix = 0;
	for(auto iter = graph.begin(); iter != graph.end(); ++iter) {
		count_matrix++;
	}
	EXPECT_EQ(2,count_matrix);

	EquivQuiverMatrix m("{ { 0 1 0 0 } { -1 0 1 0 } { 0 -1 0 1 } { 0 0 -1 0 } }");
	EquivQuiverMatrix p("{ { 0 -1 0 0 } { 1 0 -1 1 } { 0 1 0 -1 } { 0 -1 1 0 } }");

	EXPECT_TRUE(std::find_if(graph.begin(), graph.end(), graph_contains(m))
			!= graph.end());

	EXPECT_TRUE(std::find_if(graph.begin(), graph.end(), graph_contains(p))
			!= graph.end());
}
}

