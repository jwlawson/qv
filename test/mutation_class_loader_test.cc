/*
 * mutation_class_loader_test.cc
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
#include "mutation_class_loader.h"
#include "dynkin.h"
#include "gtest/gtest.h"
#include "quiver_matrix.h"

namespace cluster {

TEST(MutationClassLoader, Initial) {
  QuiverMatrix mat = dynkin::A3;
  MutationClassLoader<QuiverMatrix> c(mat);
  EXPECT_TRUE(c.next().equals(mat));
}

TEST(MutationClassLoader, Second) {
  QuiverMatrix mat = dynkin::A4;
  MutationClassLoader<QuiverMatrix> c(mat);
  /* Take the first one. */
  QuiverMatrix f = c.next();
  /* Test the second one. */
  QuiverMatrix sec = c.next();

  QuiverMatrix exp(4, 4);
  mat.mutate(0, exp);

  EXPECT_TRUE(exp.equals(sec));
}

}  // namespace cluster
