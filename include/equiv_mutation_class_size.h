/*
 * equiv_mutation_class_size.h
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
 * Contains method to find the size of a matrix mutation class up to permuting
 * the rows and columns in the matrix.
 */
#pragma once

#include "equiv_mutation_class_loader.h"
#include "equiv_quiver_matrix.h"

namespace cluster {
	namespace equivsize {
		/**
		 * Find the size of the mutation class, up to permuting the vertices of the
		 * matrices. 
		 *
		 * Checking for permutations takes longer than just checking equality of
		 * the matrix, so for small cases this can be slower than checking the
		 * whole mutation class. However for larger matrices the mutation classes
		 * are ususally very large (think many millions) and so the program can
		 * easily run out of memory computing them all. This check however will
		 * not.
		 *
		 * @param matrix Matrix to sed the mutation class with
		 * @return Size of the mutation class
		 */
		int Size(const EquivQuiverMatrix& matrix);
	}
}
