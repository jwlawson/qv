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
	EXPECT_EQ(5, num_seeds);
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
	EXPECT_EQ(10, num_seeds);
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
	EXPECT_EQ(14, num_seeds);
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
	EXPECT_EQ(4, num_seeds);
}
}

