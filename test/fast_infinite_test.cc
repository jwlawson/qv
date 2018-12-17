/*
 * fast_infinite_test.cc
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

#include "dynkin.h"
#include "fast_infinite.h"

namespace cluster {

TEST(FastInfinite, A5) {
  QuiverMatrix matrix = dynkin::A5;
  EXPECT_FALSE(fastinf::is_infinite(matrix));
}
TEST(FastInfinite, G2) {
  QuiverMatrix matrix = dynkin::G2;
  EXPECT_FALSE(fastinf::is_infinite(matrix));
}
TEST(FastInfinite, Inf) {
  int v[] = {0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0};
  QuiverMatrix mat(4, 4, v);
  EXPECT_TRUE(fastinf::is_infinite(mat));
}
TEST(FastInfinite, Infinite5) {
  int v[] = {0, -1, 1, 0, 2,  1, 0, -1, 0, -2, -1, 1, 0,
             1, 2,  0, 0, -1, 0, 0, -2, 2, -2, 0,  0};
  QuiverMatrix mat(5, 5, v);
  EXPECT_TRUE(fastinf::is_infinite(mat));
}
TEST(FastInfinite, Empty) {
  QuiverMatrix empty;
  EXPECT_FALSE(fastinf::is_infinite(empty));
}
TEST(FastInfinite, Small) {
  int v[] = {0};
  QuiverMatrix small(1, 1, v);
  EXPECT_FALSE(fastinf::is_infinite(small));
}
}  // namespace cluster
