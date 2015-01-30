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
}
using namespace cluster::mmi_conn;
TEST(MoveGraph, Simple) {
	auto move = make_move("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }",
			"{ { 0 -1 1 } { 1 0 -1 } { -1 1 0 } }", {0, 2}, {Unconnected(), Line()});
	IntMatrix init("{ { 0 2 -1 0 0 0 } { -2 0 1 0 0 0 } { 1 -1 0 1 0 1 } "
			"{ 0 0 -1 0 1 0 } { 0 0 0 -1 0 0 } { 0 0 -1 0 0 0 } }");
	std::vector<MMIMove> vec = { move };

	MoveGraph<EquivQuiverMatrix> graph(init, vec);
	for( auto iter = graph.begin(); iter != graph.end(); ++iter) {
		std::cout << *iter->first << "-> {" << std::endl;
		for( auto l_iter = iter->second.begin(); l_iter != iter->second.end();
				++l_iter ) {
			std::cout << **l_iter << std::endl;
		}
		std::cout << "}---" <<std::endl;
	}
}
}

