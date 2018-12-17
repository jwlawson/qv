/*
 * fast_infinite.h
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
 * Provides method to quickly check whether a matrix is mutation-infinite.
 */
#pragma once
#include "quiver_matrix.h"

#define MAX_MUTATIONS 3000

namespace cluster {
namespace fastinf {
bool is_infinite(const QuiverMatrix &matrix);
}
} // namespace cluster
