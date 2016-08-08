/*
 * exchange_graph_test.cc
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
#include "exchange_graph.h"

#include "ginac_util.h"
#include "seed.h"

namespace {
typedef cluster::LabelledSeed LSeed;

cluster::Seed::Cluster
default_cluster(size_t size) {
	cluster::Seed::Cluster result(size);
	std::string var = "x";
	for(size_t i = 0; i < size; ++i) {
		result[i] = cluster::ginac::symbol(var + std::to_string(i));
	}
	return std::move(result);
}
}
namespace cluster {
TEST(ExchangeGraph, Pentagon) {
	EquivQuiverMatrix m("{ { 0 1 } { -1 0 } }");
	auto c = default_cluster(2);
	Seed s(std::move(m), std::move(c));

	ExchangeGraph g(s);
	uint num_seeds = 0;
	for(auto it = g.begin(); it != g.end(); ++it) {
		++num_seeds;
		for(auto l_it = it->second.begin(); l_it != it->second.end(); ++l_it) {
			EXPECT_NE(*l_it, nullptr);
		}
	}
	EXPECT_EQ(static_cast<std::size_t>(5), num_seeds);
}
TEST(LabelledExchangeGraph, Decagon) {
	QuiverMatrix m("{ { 0 1 } { -1 0 } }");
	auto c = default_cluster(2);
	LSeed s(std::move(m), std::move(c));

	LabelledExchangeGraph g(s);
	uint num_seeds = 0;
	for(auto it = g.begin(); it != g.end(); ++it) {
		++num_seeds;
		for(auto l_it = it->second.begin(); l_it != it->second.end(); ++l_it) {
			EXPECT_NE(*l_it, nullptr);
		}
	}
	EXPECT_EQ(static_cast<std::size_t>(10), num_seeds);
}
TEST(LabelledQuiverGraph, Pentagon) {
	QuiverMatrix m("{ { 0 1  0 } { -1 0 1 } { 0 -1 0 } }");

	LabelledQuiverGraph g(m);
	uint num_seeds = 0;
	for(auto it = g.begin(); it != g.end(); ++it) {
		++num_seeds;
		for(auto l_it = it->second.begin(); l_it != it->second.end(); ++l_it) {
			EXPECT_NE(*l_it, nullptr);
		}
	}
	EXPECT_EQ(static_cast<std::size_t>(14), num_seeds);
}
TEST(QuiverGraph, Pentagon) {
	EquivQuiverMatrix m("{ { 0 1  0 } { -1 0 1 } { 0 -1 0 } }");

	QuiverGraph g(m);
	uint num_seeds = 0;
	for(auto it = g.begin(); it != g.end(); ++it) {
		++num_seeds;
		for(auto l_it = it->second.begin(); l_it != it->second.end(); ++l_it) {
			EXPECT_NE(*l_it, nullptr);
		}
	}
	EXPECT_EQ(static_cast<std::size_t>(4), num_seeds);
}
namespace {
	typedef _EGContinueChecks::InfiniteTypeSink GCheck;
}
TEST(GreenContinue, Simple) {
	const QuiverMatrix m("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }");
	GCheck chk;
	EXPECT_TRUE(chk(&m, 0));
	EXPECT_TRUE(chk(&m, 1));
	EXPECT_TRUE(chk(&m, 2));
	EXPECT_TRUE(chk(&m, 3));
}
TEST(GreenContinue, Cycle) {
	const QuiverMatrix m("{ { 0 1 -1 0 } { -1 0 1 1 } { 1 -1 0 1 } { 0 -1 -1 0 } }");
	GCheck chk;
	EXPECT_TRUE(chk(&m, 0));
	EXPECT_TRUE(chk(&m, 1));
	EXPECT_TRUE(chk(&m, 2));
	EXPECT_TRUE(chk(&m, 3));
}
TEST(GreenContinue, InfiniteCycle) {
	const QuiverMatrix m("{ { 0 2 -1 0 } { -2 0 1 1 } { 1 -1 0 1 } { 0 -1 -1 0 } }");
	GCheck chk;
	EXPECT_TRUE(chk(&m, 0));
	/* Disabled as this functionality is not implemented.
	 * The vertex is not taken into account at this point, instead only the matrix
	 * is considered, and the computation of the exchange graph stops after these
	 * infinite type matrices have been computed, not before
	EXPECT_FALSE(chk(&m, 1));
	*/
	EXPECT_TRUE(chk(&m, 2));
	EXPECT_TRUE(chk(&m, 3));
}
TEST(GreenContinue, AllInfiniteCycle) {
	const QuiverMatrix m("{ { 0 2 -2 0 } { -2 0 2 1 } { 2 -2 0 1 } { 0 -1 -1 0 } }");
	GCheck chk;
	EXPECT_FALSE(chk(&m, 0));
	EXPECT_FALSE(chk(&m, 1));
	EXPECT_FALSE(chk(&m, 2));
	EXPECT_FALSE(chk(&m, 3));
}
TEST(GreenContinue, Reuse) {
	const QuiverMatrix m("{ { 0 2 -2 0 } { -2 0 2 1 } { 2 -2 0 1 } { 0 -1 -1 0 } }");
	GCheck chk;
	EXPECT_FALSE(chk(&m, 0));
	EXPECT_FALSE(chk(&m, 1));
	EXPECT_FALSE(chk(&m, 2));
	EXPECT_FALSE(chk(&m, 3));

	const QuiverMatrix n("{ { 0 1 -1 0 } { -1 0 1 1 } { 1 -1 0 1 } { 0 -1 -1 0 } }");
	EXPECT_TRUE(chk(&n, 0));
	EXPECT_TRUE(chk(&n, 1));
	EXPECT_TRUE(chk(&n, 2));
	EXPECT_TRUE(chk(&n, 3));

	const QuiverMatrix k("{ { 0 1 0 0 } { -1 0 3 -4 } { 0 -3 0 5 } { 0 4 -5 0 } }");
	EXPECT_FALSE(chk(&k, 0));
	EXPECT_FALSE(chk(&k, 1));
	EXPECT_FALSE(chk(&k, 2));
	EXPECT_FALSE(chk(&k, 3));
}
TEST(GreenContinue, Seed) {
	EquivQuiverMatrix k("{ { 0 1 0 0 } { -1 0 3 -4 } { 0 -3 0 2 } { 0 4 -2 0 } }");
	Seed::Cluster cl = default_cluster(4);
	Seed s(std::move(k), std::move(cl));

	GCheck chk;
	EXPECT_FALSE(chk(&s, 0));
	EXPECT_FALSE(chk(&s, 1));
	EXPECT_FALSE(chk(&s, 2));
	EXPECT_FALSE(chk(&s, 3));
}
}

