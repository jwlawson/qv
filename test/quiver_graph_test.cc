/**
 * quiver_qraph_test.cc
 * Copyright 2014-2015 John Lawson
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
