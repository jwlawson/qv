/*
 * seed_test.cc
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
#include "seed.h"

#include "ginac_util.h"

namespace {
typedef cluster::LabelledSeed LSeed;

cluster::Seed::Cluster
default_cluster(size_t size) {
	cluster::Seed::Cluster result(size);
	std::string var = "x";
	for(size_t i = 0; i < size; ++i) {
		result[i] = cluster::ginac::symbol(var + std::to_string(i));
	}
	return result;
}
}

namespace cluster {
TEST(Seed, Rank2Symmetric) {
	EquivQuiverMatrix m("{ { 0 1 } { -1 0 } }");
	Seed s(std::move(m), std::move(default_cluster(2)));
	Seed res(2);
	s.mutate(1, res);

	EquivQuiverMatrix n("{ { 0 -1 } { 1 0 } }");
	Seed::Cluster c = default_cluster(2);
	c[1] = (1 + c[0]) / c[1];
	Seed exp(n, c);
	EXPECT_TRUE(res.equals(exp));
	EXPECT_TRUE(exp.equals(res));
}
TEST(Seed, Rank2SymmetricPermuted) {
	EquivQuiverMatrix m("{ { 0 1 } { -1 0 } }");
	Seed s(std::move(m), std::move(default_cluster(2)));
	Seed res(2);
	s.mutate(1, res);

	EquivQuiverMatrix n("{ { 0 1 } { -1 0 } }");
	Seed::Cluster c = default_cluster(2);
	GiNaC::ex a = (1 + c[0]) / c[1];
	c[1] = c[0];
	c[0] = a;
	Seed exp(n, c);
	EXPECT_TRUE(res.equals(exp));
	EXPECT_TRUE(exp.equals(res));
}
TEST(LSeed, Rank2Symmetric) {
	EquivQuiverMatrix m("{ { 0 1 } { -1 0 } }");
	LabelledSeed s(std::move(m), std::move(default_cluster(2)));
	LabelledSeed res(2);
	s.mutate(1, res);

	EquivQuiverMatrix n("{ { 0 -1 } { 1 0 } }");
	Seed::Cluster c = default_cluster(2);
	c[1] = (1 + c[0]) / c[1];
	LabelledSeed exp(n, c);
	EXPECT_TRUE(res.equals(exp));
	EXPECT_TRUE(exp.equals(res));
}
TEST(LSeed, Rank2SymmetricPermuted) {
	EquivQuiverMatrix m("{ { 0 1 } { -1 0 } }");
	LSeed s(std::move(m), std::move(default_cluster(2)));
	LSeed res(2);
	s.mutate(1, res);

	EquivQuiverMatrix n("{ { 0 1 } { -1 0 } }");
	Seed::Cluster c = default_cluster(2);
	GiNaC::ex a = (1 + c[0]) / c[1];
	c[1] = c[0];
	c[0] = a;
	LSeed exp(n, c);
	EXPECT_FALSE(res.equals(exp));
	EXPECT_FALSE(exp.equals(res));
}

}

