/**
 * ss_move_test.cc
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

#include "ss_move.h"

namespace cluster {
namespace {
	typedef std::vector<SSMove::Applicable> AppVec;
}
TEST(SSMove, NoApplicable) {
	IntMatrix m("{ { 0 1 0 -1 } { -1 0 1 0 } { 0 -1 0 1 } { 1 0 -1 0 } }");
	SSMove move;
	AppVec app = move.applicable_submatrices(m);
	ASSERT_TRUE(app.empty());
}

TEST(SSMove, SingleSource) {
	IntMatrix m("{ { 0 -1 0 0 } { 1 0 -1 1 } { 0 1 0 -1 } { 0 -1 1 0 } }");
	SSMove move;
	AppVec app = move.applicable_submatrices(m);
	ASSERT_FALSE(app.empty());
	EXPECT_EQ(1, app.size());
	IntMatrix exp("{ { 0 1 0 0 } { -1 0 -1 1 } { 0 1 0 -1 } { 0 -1 1 0 } }");
	IntMatrix res(4, 4);
	move.move(app[0], res);
	EXPECT_TRUE(exp.equals(res));
}
TEST(SSMove, TwoApps) {
	IntMatrix m("{ { 0 1 0 0 } { -1 0 -1 -1 } { 0 1 0 1 } { 0 1 -1 0 } }");
	SSMove move;
	AppVec app = move.applicable_submatrices(m);
	ASSERT_EQ(3, app.size());

	IntMatrix res(4, 4);
	IntMatrix exp("{ { 0 -1 0 0 } { 1 0 -1 -1 } { 0 1 0 1 } { 0 1 -1 0 } }");
	move.move(app[0], res);
	EXPECT_TRUE(exp.equals(res));
	IntMatrix exp1("{ { 0 -1 0 0 } { 1 0 1 1 } { 0 -1 0 1 } { 0 -1 -1 0 } }");
	move.move(app[1], res);
	EXPECT_TRUE(exp1.equals(res));
	IntMatrix exp2("{ { 0 1 0 0 } { -1 0 1 -1 } { 0 -1 0 -1 } { 0 1 1 0 } }");
	move.move(app[2], res);
	EXPECT_TRUE(exp2.equals(res));
}
}

