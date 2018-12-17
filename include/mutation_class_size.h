/*
 * mutation_class_size.h
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
 * Contains a method to find the size of the mutation class of a matrix.
 */
#pragma once

#include "quiver_matrix.h"

namespace cluster {
namespace classsize {
/**
 * Find the size of the mutation class of the matrix.
 * @param matrix Matrix to check
 * @return The size of the mutation class
 */
int Size(const QuiverMatrix &matrix);

} // namespace classsize
} // namespace cluster
