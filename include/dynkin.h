/*
 * dynkin.h
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
 * Contains matrices for each Dynkin diagram that admits a finite mutation-class
 * with at most 9 vertices.
 */
#pragma once

#include <map>
#include <string>

#include "quiver_matrix.h"

namespace cluster {
	class dynkin {
	 public:
		static const QuiverMatrix A2;
		static const QuiverMatrix A3;
		static const QuiverMatrix A4;
		static const QuiverMatrix A5;
		static const QuiverMatrix A6;
		static const QuiverMatrix A7;
		static const QuiverMatrix A8;
		static const QuiverMatrix A9;
		static const QuiverMatrix B2;
		static const QuiverMatrix B3;
		static const QuiverMatrix B4;
		static const QuiverMatrix B5;
		static const QuiverMatrix B6;
		static const QuiverMatrix B7;
		static const QuiverMatrix B8;
		static const QuiverMatrix B9;
		static const QuiverMatrix C3;
		static const QuiverMatrix C4;
		static const QuiverMatrix C5;
		static const QuiverMatrix C6;
		static const QuiverMatrix C7;
		static const QuiverMatrix C8;
		static const QuiverMatrix C9;
		static const QuiverMatrix D4;
		static const QuiverMatrix D5;
		static const QuiverMatrix D6;
		static const QuiverMatrix D7;
		static const QuiverMatrix D8;
		static const QuiverMatrix D9;
		static const QuiverMatrix E6;
		static const QuiverMatrix E7;
		static const QuiverMatrix E8;
		static const QuiverMatrix G2;
		static const std::map<std::string, QuiverMatrix> MAP;
	};
}


