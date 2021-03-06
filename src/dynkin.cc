/*
 * dynkin.cc
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
#include "dynkin.h"

namespace cluster {
	int v1[] = {0, 1, -1, 0};
	const QuiverMatrix dynkin::dynkin::A2(2, 2, v1);
	int v2[] = {0, 1, 0, -1, 0, 1, 0, -1, 0};
	const QuiverMatrix dynkin::A3(3, 3, v2);
	int v3[] = { 0, 1, 0, 0, -1, 0, 1, 0, 0, -1, 0, 1, 0, 0, -1, 0};
	const QuiverMatrix dynkin::A4(4, 4, v3);
	int v4[] = { 0, 1, 0, 0, 0, -1, 0, 1, 0, 0, 0, -1, 0, 1, 0, 0, 0, -1, 0, 1,
	             0, 0, 0, -1, 0
	           };
	const QuiverMatrix dynkin::A5(5, 5, v4);
	int v5[] = { 0, 1, 0, 0, 0, 0,
	             -1, 0, 1, 0, 0, 0,
	             0, -1, 0, 1, 0, 0,
	             0, 0, -1, 0, 1, 0,
	             0, 0, 0, -1, 0, 1,
	             0, 0, 0, 0, -1, 0
	           };
	const QuiverMatrix dynkin::A6(6, 6, v5);
	int v6[] = { 0, 1, 0, 0, 0, 0, 0,
	             -1, 0, 1, 0, 0, 0, 0,
	             0, -1, 0, 1, 0, 0, 0,
	             0, 0, -1, 0, 1, 0, 0,
	             0, 0, 0, -1, 0, 1, 0,
	             0, 0, 0, 0, -1, 0, 1,
	             0, 0, 0, 0, 0, -1, 0
	           };
	const QuiverMatrix dynkin::A7(7, 7, v6);
	int v7[] = { 0, 1, 0, 0, 0, 0, 0, 0,
	             -1, 0, 1, 0, 0, 0, 0, 0,
	             0, -1, 0, 1, 0, 0, 0, 0,
	             0, 0, -1, 0, 1, 0, 0, 0,
	             0, 0, 0, -1, 0, 1, 0, 0,
	             0, 0, 0, 0, -1, 0, 1, 0,
	             0, 0, 0, 0, 0, -1, 0, 1,
	             0, 0, 0, 0, 0, 0, -1, 0
	           };
	const QuiverMatrix dynkin::A8(8, 8, v7);
	int v8[] = { 0, 1, 0, 0, 0, 0, 0, 0, 0,
	             -1, 0, 1, 0, 0, 0, 0, 0, 0,
	             0, -1, 0, 1, 0, 0, 0, 0, 0,
	             0, 0, -1, 0, 1, 0, 0, 0, 0,
	             0, 0, 0, -1, 0, 1, 0, 0, 0,
	             0, 0, 0, 0, -1, 0, 1, 0, 0,
	             0, 0, 0, 0, 0, -1, 0, 1, 0,
	             0, 0, 0, 0, 0, 0, -1, 0, 1,
	             0, 0, 0, 0, 0, 0, 0, -1, 0
	           };
	const QuiverMatrix dynkin::A9(9, 9, v8);
	int v9[] = {0, 1, -2, 0};
	const QuiverMatrix dynkin::B2(2, 2, v9);
	int v10[] = {	0, 1, 0,
	              -1, 0, 2,
	              0, -1, 0
	            };
	const QuiverMatrix dynkin::B3(3, 3, v10);
	int v11[] = {	0, 1, 0, 0,
	              -1, 0, 1, 0,
	              0, -1, 0, 2,
	              0, 0, -1, 0
	            };
	const QuiverMatrix dynkin::B4(4, 4, v11);
	int v12[] = { 0, 1, 0, 0, 0,
	              -1, 0, 1, 0, 0,
	              0, -1, 0, 1, 0,
	              0, 0, -1, 0, 2,
	              0, 0, 0, -1, 0
	            };
	const QuiverMatrix dynkin::B5(5, 5, v12);
	int v13[] = { 0, 1, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0,
	              0, -1, 0, 1, 0, 0,
	              0, 0, -1, 0, 1, 0,
	              0, 0, 0, -1, 0, 2,
	              0, 0, 0, 0, -1, 0
	            };
	const QuiverMatrix dynkin::B6(6, 6, v13);
	int v14[] = { 0, 1, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, -1, 0, 1, 0,
	              0, 0, 0, 0, -1, 0, 2,
	              0, 0, 0, 0, 0, -1, 0
	            };
	const QuiverMatrix dynkin::B7(7, 7, v14);
	int v15[] = { 0, 1, 0, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0, 0,
	              0, 0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, 0, -1, 0, 1, 0,
	              0, 0, 0, 0, 0, -1, 0, 2,
	              0, 0, 0, 0, 0, 0, -1, 0
	            };
	const QuiverMatrix dynkin::B8(8, 8, v15);
	int v16[] = { 0, 1, 0, 0, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0, 0, 0,
	              0, 0, 0, -1, 0, 1, 0, 0, 0,
	              0, 0, 0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, 0, 0, -1, 0, 1, 0,
	              0, 0, 0, 0, 0, 0, -1, 0, 2,
	              0, 0, 0, 0, 0, 0, 0, -1, 0
	            };
	const QuiverMatrix dynkin::B9(9, 9, v16);
	int v17[] = {	0, 1, 0,
	              -1, 0, -1,
	              0, 2, 0
	            };
	const QuiverMatrix dynkin::C3(3, 3, v17);
	int v18[] = {	0, 1, 0, 0,
	              -1, 0, 1, 0,
	              0, -1, 0, -1,
	              0, 0, 2, 0
	            };
	const QuiverMatrix dynkin::C4(4, 4, v18);
	int v19[] = { 0, 1, 0, 0, 0,
	              -1, 0, 1, 0, 0,
	              0, -1, 0, 1, 0,
	              0, 0, -1, 0, -1,
	              0, 0, 0, 2, 0
	            };
	const QuiverMatrix dynkin::C5(5, 5, v19);
	int v20[] = {0, 1, 0, 0, 0, 0,
	             -1, 0, 1, 0, 0, 0,
	             0, -1, 0, 1, 0, 0,
	             0, 0, -1, 0, 1, 0,
	             0, 0, 0, -1, 0, -1,
	             0, 0, 0, 0, 2, 0
	            };
	const QuiverMatrix dynkin::C6(6, 6, v20);
	int v21[] = { 0, 1, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, -1, 0, 1, 0,
	              0, 0, 0, 0, -1, 0, -1,
	              0, 0, 0, 0, 0, 2, 0
	            };
	const QuiverMatrix dynkin::C7(7, 7, v21);
	int v22[] = { 0, 1, 0, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0, 0,
	              0, 0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, 0, -1, 0, 1, 0,
	              0, 0, 0, 0, 0, -1, 0, -1,
	              0, 0, 0, 0, 0, 0, 2, 0
	            };
	const QuiverMatrix dynkin::C8(8, 8, v22);
	int v23[] = { 0, 1, 0, 0, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0, 0, 0,
	              0, 0, 0, -1, 0, 1, 0, 0, 0,
	              0, 0, 0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, 0, 0, -1, 0, 1, 0,
	              0, 0, 0, 0, 0, 0, -1, 0, -1,
	              0, 0, 0, 0, 0, 0, 0, 2, 0
	            };
	const QuiverMatrix dynkin::C9(9, 9, v23);
	int v24[] = { 0, 1, 0, 0,
	              -1, 0, 1, 1,
	              0, -1, 0, 0,
	              0, -1, 0, 0
	            };
	const QuiverMatrix dynkin::D4(4, 4, v24);
	int v25[] = { 0, 1, 0, 0, 0,
	              -1, 0, 1, 0, 0,
	              0, -1, 0, 1, 1,
	              0, 0, -1, 0, 0,
	              0, 0, -1, 0, 0
	            };
	const QuiverMatrix dynkin::D5(5, 5, v25);
	int v26[] = { 0, 1, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0,
	              0, -1, 0, 1, 0, 0,
	              0, 0, -1, 0, 1, 1,
	              0, 0, 0, -1, 0, 0,
	              0, 0, 0, -1, 0, 0
	            };
	const QuiverMatrix dynkin::D6(6, 6, v26);
	int v27[] = { 0, 1, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, -1, 0, 1, 1,
	              0, 0, 0, 0, -1, 0, 0,
	              0, 0, 0, 0, -1, 0, 0
	            };
	const QuiverMatrix dynkin::D7(7, 7, v27);
	int v28[] = { 0, 1, 0, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0, 0,
	              0, 0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, 0, -1, 0, 1, 1,
	              0, 0, 0, 0, 0, -1, 0, 0,
	              0, 0, 0, 0, 0, -1, 0, 0
	            };
	const QuiverMatrix dynkin::D8(8, 8, v28);
	int v29[] = { 0, 1, 0, 0, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0, 0, 0,
	              0, -1, 0, 1, 0, 0, 0, 0, 0,
	              0, 0, -1, 0, 1, 0, 0, 0, 0,
	              0, 0, 0, -1, 0, 1, 0, 0, 0,
	              0, 0, 0, 0, -1, 0, 1, 0, 0,
	              0, 0, 0, 0, 0, -1, 0, 1, 1,
	              0, 0, 0, 0, 0, 0, -1, 0, 0,
	              0, 0, 0, 0, 0, 0, -1, 0, 0
	            };
	const QuiverMatrix dynkin::D9(9, 9, v29);
	int v30[] = { 0, 1, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0,
	              0, -1, 0, 1, 1, 0,
	              0, 0, -1, 0, 0, 0,
	              0, 0, -1, 0, 0, 1,
	              0, 0, 0, 0, -1, 0
	            };
	const QuiverMatrix dynkin::E6(6, 6, v30);
	int v31[] = { 0, 1, 0, 0, 0, 0, 0,
	              -1, 0, 1, 0, 0, 0, 0,
	              0, -1, 0, 1, 1, 0, 0,
	              0, 0, -1, 0, 0, 0, 0,
	              0, 0, -1, 0, 0, 1, 0,
	              0, 0, 0, 0, -1, 0, 1,
	              0, 0, 0, 0, 0, -1, 0
	            };
	const QuiverMatrix dynkin::E7(7, 7, v31);
	int v32 [] = { 0, 1, 0, 0, 0, 0, 0, 0,
	               -1, 0, 1, 0, 0, 0, 0, 0,
	               0, -1, 0, 1, 1, 0, 0, 0,
	               0, 0, -1, 0, 0, 0, 0, 0,
	               0, 0, -1, 0, 0, 1, 0, 0,
	               0, 0, 0, 0, -1, 0, 1, 0,
	               0, 0, 0, 0, 0, -1, 0, 1,
	               0, 0, 0, 0, 0, 0, -1, 0
	             };
	const QuiverMatrix dynkin::E8(8, 8, v32);
	int v33[] = {0, 3, -1, 0};
	const QuiverMatrix dynkin::G2(2, 2, v33);

	const std::map<std::string, QuiverMatrix> dynkin::MAP = {{"A2", dynkin::A2},
		{"A3", dynkin::A3},{"A4", dynkin::A4},{"A5", dynkin::A5},
		{"A6", dynkin::A6},{"A7", dynkin::A7},{"A8", dynkin::A8},
		{"A9", dynkin::A9},{"B3", dynkin::B3},{"B4", dynkin::B4},
		{"B5", dynkin::B5},{"B6", dynkin::B6},{"B7", dynkin::B7},
		{"B8", dynkin::B8},{"B9", dynkin::B9},{"C3", dynkin::C3},
		{"C4", dynkin::C4},{"C5", dynkin::C5},{"C6", dynkin::C6},
		{"C7", dynkin::C7},{"C8", dynkin::C8},{"C9", dynkin::C9},
		{"D4", dynkin::D4},{"D5", dynkin::D5},{"D6", dynkin::D6},
		{"D7", dynkin::D7},{"D8", dynkin::D8},{"D9", dynkin::D9},
		{"E6", dynkin::E6},{"E7", dynkin::E7},{"E8", dynkin::E8},
		{"G2", dynkin::G2}};

}

