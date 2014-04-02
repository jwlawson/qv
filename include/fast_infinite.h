/*
 * fast_infinite.h
 * Provides method to quickly check whether a matrix is mutation-infinite.
 */
#pragma once
#include "quiver_matrix.h"

#define MAX_MUTATIONS 3000

namespace cluster {
	namespace fastinf {
		bool is_infinite(QuiverMatrix matrix);
	}
}

