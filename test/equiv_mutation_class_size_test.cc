/*
 * equiv_mutation_class_size_test.cc
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
#include "equiv_mutation_class_size.h"
#include "dynkin.h"
#include "gtest/gtest.h"

namespace cluster {

TEST(EquivMutationClassSize, A3) {
  EquivQuiverMatrix m(dynkin::A3);
  int exp = 4;
  int res = equivsize::Size(m);

  EXPECT_EQ(exp, res);
}

TEST(EquivMutationClassSize, A5) {
  EquivQuiverMatrix m(dynkin::A5);
  int exp = 19;
  int res = equivsize::Size(m);

  EXPECT_EQ(exp, res);
}

TEST(EquivMutationClassSize, E8) {
  EquivQuiverMatrix m(dynkin::E8);
  int exp = 1574;
  int res = equivsize::Size(m);

  EXPECT_EQ(exp, res);
}

/*
// Takes ~13s
TEST(EquivMutationClassSize, Rank12) {
        std::string str = "{ { 0 -1 0 0 0 0 0 -1 1 0 0 1 } { 1 0 1 0 0 0 0 0 0
-1 0 -1 } { 0 -1 0 -1 0 0 0 0 0 1 1 0 } { 0 0 1 0 1 0 0 0 0 0 -2 0 } { 0 0 0 -1
0 -1 0 0 0 1 1 0 } { 0 0 0 0 1 0 1 0 0 -1 0 -1 } { 0 0 0 0 0 -1 0 -1 1 0 0 1 } {
1 0 0 0 0 0 1 0 -2 0 0 0 } { -1 0 0 0 0 0 -1 2 0 0 0 0 } { 0 1 -1 0 -1 1 0 0 0 0
0 0 } { 0 0 -1 2 -1 0 0 0 0 0 0 0 } { -1 1 0 0 0 1 -1 0 0 0 0 0 } }";
        EquivQuiverMatrix m(str);
        int exp = 368;
        int res = equivsize::Size(m);
        EXPECT_EQ(exp, res);
}
*/

TEST(EquivMutationClassSize, D6) {
  EquivQuiverMatrix m(dynkin::D6);
  int exp = 80;
  int res = equivsize::Size(m);

  EXPECT_EQ(exp, res);
}

TEST(EquivMutationClassSize, Infinite) {
  std::string str = "{	{ 0 1 0 0 } { -1 0 1 1 } { 0 -1 0 1 } { 0 -1 -1 0 }";
  EquivQuiverMatrix m(str);

  int res = equivsize::Size(m);

  EXPECT_EQ(-1, res);
}
TEST(EquivMutationClassSize, Twos) {
  std::string str = "{ { 0 2 -2 } { -2 0 2 } { 2 -2 0 } }";
  QuiverMatrix mat(str);

  int res = equivsize::Size(mat);
  EXPECT_EQ(1, res);
}

/**
 * This test was included after a strange bug appeared only in this mutation
 * class. At the 1458 matrix to be taken from the class, computing the
 * mutation at the 3rd vertex resulted in a call to the hash map for the
 * matrix:
 * { 	{ 0 -1 1 0 0 1 0 0 }
 * 		{ 1 0 0 0 1 0 0 -1 }
 * 		{ -1 0 0 0 0 0 0 0 }
 * 		{ 0 0 0 0 0 1 0 0 }
 * 		{ 0 -1 0 0 0 0 0 0 }
 * 		{ -1 0 0 -1 0 0 0 1 }
 * 		{ 0 0 0 0 0 0 0 -1 }
 * 		{ 0 1 0 0 0 -1 1 0 } }
 *
 * For some reason, the first time map_.count was called it would return 1,
 * but then subsequent calls would return 0. Checking which matrix was
 * considered 'equal' it was:
 * { 	{ 0 0 1 0 -1 1 0 0 }
 * 		{ 0 0 0 0 1 0 0 0 }
 * 		{ -1 0 0 0 0 0 0 0 }
 * 		{ 0 0 0 0 0 1 0 0 }
 * 		{ 1 -1 0 0 0 0 0 -1 }
 * 		{ -1 0 0 -1 0 0 0 1 }
 * 		{ 0 0 0 0 0 0 0 -1 }
 * 		{ 0 0 0 0 1 -1 1 0 } }
 *
 * Checking these matrices separately (see equiv_quiver_matrix_test) these are
 * *not* equivalent, yet for some reason they were coming up as equal.
 *
 * I changed the hashcode, so that these matrices no longer have the same
 * hash, which seems to have prevented this from cropping up.
 */
TEST(EquivMutationClassSize, Big) {
  std::string str =
      "{ { 0 1 0 0 0 0 0 0 } "
      "{ -1 0 1 0 0 0 0 0 } "
      "{ 0 -1 0 1 1 0 0 0 } "
      "{ 0 0 -1 0 0 0 0 0 } "
      "{ 0 0 -1 0 0 1 0 0 } "
      "{ 0 0 0 0 -1 0 0 -1 } "
      "{ 0 0 0 0 0 0 0 -1 } "
      "{ 0 0 0 0 0 1 1 0 } }";
  EquivQuiverMatrix m(str);

  int result = equivsize::Size(m);
  EXPECT_TRUE(result > 0);
}

/*
 * This is another example of a matrix which appeared to be in the map, even
 * though the matrix returned by find is not actually equivalent to it. This
 * one did not get fixed by changing the hashcode, so I have added an extra
 * call to map_.find after map_.count to ensure that this cannot cause a crash
 * - but perhaps it will run a bit slower.
 */
/*
TEST(EquivMutationClassSize, Big) {
        std::string str = "{ { 0 1 0 0 0 0 0 1 } "
                                                                                        "{ -1 0 1 0 0 0 0 0 } "
                                                                                        "{ 0 -1 0 1 0 0 0 0 } "
                                                                                        "{ 0 0 -1 0 0 0 0 0 } "
                                                                                        "{ 0 0 0 0 0 0 1 0 } "
                                                                                        "{ 0 0 0 0 0 0 1 -1 } "
                                                                                        "{ 0 0 0 0 -1 -1 0 0 } "
                                                                                        "{ -1 0 0 0 0 1 0 0 } }";
        EquivQuiverMatrix m(str);

        int result = equivsize::Size(m);
        EXPECT_TRUE(result > 0);
}
*/
}  // namespace cluster
