/**
 * consts.h
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
/**
 * Header containing declarations of constants to use in qvmove.
 */
#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include "equiv_quiver_matrix.h"
#include "equiv_underlying_graph.h"
#include "mmi_move.h"

namespace qv {
namespace consts {

extern std::vector<std::shared_ptr<cluster::MMIMove>> Moves;
extern std::unordered_set<std::shared_ptr<cluster::EquivQuiverMatrix>> Reps;
extern std::unordered_set<std::shared_ptr<cluster::EquivUnderlyingGraph>> Graphs;

}
}

