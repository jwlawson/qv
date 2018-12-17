/*
 * sink_source_graph_test.cc
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
#include "sink_source_graph.h"
#include "ginac_util.h"

#include "gtest/gtest.h"

namespace {
typedef cluster::LabelledSeed LSeed;

cluster::Seed::Cluster
default_cluster(size_t size) {
  cluster::Seed::Cluster result(size);
  std::string var = "x";
  for (size_t i = 0; i < size; ++i) {
    result[i] = cluster::ginac::symbol(var + std::to_string(i));
  }
  return result;
}
}  // namespace

namespace cluster {
TEST(SSGraph, A2) {
  QuiverMatrix a("{ { 0 1 } { -1 0 } }");
  SinkSourceGraph<QuiverMatrix> graph(a);

  int count{0};
  for (auto it = graph.begin(), end = graph.end(); it != end; ++it) {
    ++count;
  }
  EXPECT_EQ(2, count);
}
TEST(SSGraph, A3) {
  EquivQuiverMatrix a("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }");
  SinkSourceGraph<EquivQuiverMatrix> graph(a);

  int count{0};
  for (auto it = graph.begin(), end = graph.end(); it != end; ++it) {
    ++count;
  }
  EXPECT_EQ(3, count);
}
TEST(SSGraph, A2Seed) {
  EquivQuiverMatrix a("{ { 0 1 } { -1 0 } }");
  Seed s(a, default_cluster(2));
  SinkSourceGraph<Seed> graph(s);

  int count{0};
  for (auto it = graph.begin(), end = graph.end(); it != end; ++it) {
    ++count;
  }
  EXPECT_EQ(5, count);
}
TEST(SSGraph, A2LabelledSeed) {
  EquivQuiverMatrix a("{ { 0 1 } { -1 0 } }");
  LabelledSeed s(a, default_cluster(2));
  SinkSourceGraph<LabelledSeed> graph(s);

  int count{0};
  for (auto it = graph.begin(), end = graph.end(); it != end; ++it) {
    ++count;
  }
  EXPECT_EQ(10, count);
}
}  // namespace cluster
