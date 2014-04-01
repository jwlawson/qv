/*
 * fast_infinite.cc
 */
#include "fast_infinite.h"
#include <random>
#include <vector>
#include <iostream>

namespace cluster {
	namespace fastinf {
		bool is_infinite(QuiverMatrix matrix) {

			// All 2x2 matrices are mutation finite
			if (matrix.num_rows() == 2 && matrix.num_cols() == 2) {
				return false;
			}
			std::vector<QuiverMatrix> mutated(2,
			            QuiverMatrix(matrix.num_rows(), matrix.num_cols()));

			int lastMutation = -1;
			int counter = 0;
			std::default_random_engine gen;
			std::uniform_int_distribution<int> dist(0, matrix.num_rows());
			auto random = std::bind(dist, gen);

			while (counter < MAX_MUTATIONS) {
				int rand;
				do {
					rand = random();
				} while (rand == lastMutation);

				/* Alternate between mutating the two matrices in the array. */
				QuiverMatrix to_mutate = mutated[counter % 2];
				std::cout << to_mutate << std::endl;
				counter++;
				to_mutate.mutate(rand, mutated[counter % 2]);
				if (mutated[counter % 2].is_infinite()) {
					return true;
				}
				lastMutation = rand;
			}
			return false;
		}
	}
}
