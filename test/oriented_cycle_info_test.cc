/*
 * oriented_cycle_info_test.cc
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
#include "oriented_cycle_info.h"

#include "gtest/gtest.h"

namespace cluster {

TEST(OrientedCycleInfo, SameEqual) {
  int v[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};
  QuiverMatrix m(3, 3, v);
  QuiverMatrix n(3, 3, v);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_TRUE(c1.equals(c2));
}

TEST(OrientedCycleInfo, PermEqual) {
  int v[] = {0, -1, 1, 1, 0, -1, -1, 1, 0};
  int w[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};

  QuiverMatrix m(3, 3, v);
  QuiverMatrix n(3, 3, w);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_TRUE(c1.equals(c2));
}

TEST(OrientedCycleInfo, ExtensionEqual) {
  int v[] = {0, 1, 0, 0, -1, 0, 1, -1, 0, -1, 0, 1, 0, 1, -1, 0};
  int w[] = {0, 1, -1, 0, -1, 0, 1, 0, 1, -1, 0, 1, 0, 0, -1, 0};

  QuiverMatrix m(4, 4, v);
  QuiverMatrix n(4, 4, w);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_TRUE(c1.equals(c2));
}

TEST(OrientedCycleInfo, NoCycles) {
  int v[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
  int w[] = {0, 1, 0,  0, 0,  0, -1, 0, 1, 0, 0, 0,  0, -1, 0, 1, 0, 0,
             0, 0, -1, 0, -1, 0, 0,  0, 0, 1, 0, -1, 0, 0,  0, 0, 1, 0};

  QuiverMatrix m(3, 3, v);
  QuiverMatrix n(6, 6, w);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_TRUE(c1.equals(c2));
}

TEST(OrientedCycleInfo, DoubleNotSingle) {
  int v[] = {0, 2, -1, -2, 0, 1, 1, -1, 0};
  int w[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};

  QuiverMatrix m(3, 3, v);
  QuiverMatrix n(3, 3, w);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_FALSE(c1.equals(c2));
}

TEST(OrientedCycleInfo, DoubleExtension) {
  int v[] = {0, 2, -1, -2, 0, 1, 1, -1, 0};
  int w[] = {0, 1, 0, 0, -1, 0, 2, -1, 0, -2, 0, 1, 0, 1, -1, 0};

  QuiverMatrix m(3, 3, v);
  QuiverMatrix n(4, 4, w);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_TRUE(c1.equals(c2));
}

TEST(OrientedCycleInfo, NotEqual) {
  int v[] = {0, -1, 1, 1, 0, -1, -1, 1, 0};
  int w[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};

  QuiverMatrix m(3, 3, v);
  QuiverMatrix n(3, 3, w);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_FALSE(c1.equals(c2));
}

TEST(OrientedCycleInfo, OtherWay) {
  int v[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};
  int w[] = {0, -1, 1, 1, 0, -1, -1, 1, 0};

  QuiverMatrix m(3, 3, v);
  QuiverMatrix n(3, 3, w);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_TRUE(c1.equals(c2));
}

TEST(OrientedCycleInfo, SwitchSingle) {
  int v[] = {0, 1, -1, -1, 0, 1, 1, -1, 0};
  int w[] = {0, -1, 1, -1, 0, -1, 1, 1, 0};

  QuiverMatrix m(3, 3, v);
  QuiverMatrix n(3, 3, w);

  OrientedCycleInfo c1(m);
  OrientedCycleInfo c2(n);

  EXPECT_FALSE(c1.equals(c2));
}

}  // namespace cluster
