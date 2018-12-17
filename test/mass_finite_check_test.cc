/**
 * mass_finite_check_test.cc
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
#include "mass_finite_check.h"

#include "gtest/gtest.h"

#include "dynkin.h"

namespace cluster {

TEST(MassFiniteCheck, Infinite) {
  int v[] = {0, 1, 0, 0, -1, 0, 1, 1, 0, -1, 0, 1, 0, -1, -1, 0};
  EquivQuiverMatrix m(4, 4, v);
  MassFiniteCheck chk;
  EXPECT_FALSE(chk.is_finite(m));
}

TEST(MassFiniteCheck, A3) {
  EquivQuiverMatrix m(dynkin::A3);
  MassFiniteCheck chk;

  EXPECT_TRUE(chk.is_finite(m));

  int v[] = {0, -1, 0, 1, 0, 1, 0, -1, 0};
  EquivQuiverMatrix n(3, 3, v);
  EXPECT_TRUE(chk.is_finite(n));
}

TEST(MassFiniteCheck, A6) {
  EquivQuiverMatrix m(dynkin::A6);
  MassFiniteCheck chk;

  EXPECT_TRUE(chk.is_finite(m));
}

TEST(MassFiniteCheck, Empty) {
  EquivQuiverMatrix m;
  MassFiniteCheck chk;

  EXPECT_TRUE(chk.is_finite(m));
}

}  // namespace cluster
