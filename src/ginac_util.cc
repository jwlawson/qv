/*
 * ginac_util.h
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
#include "ginac_util.h"

const GiNaC::symbol &
cluster::ginac::symbol(const std::string & s) {
	static SymbolMap sym_map;
	SymbolMap::iterator found = sym_map.find(s);
	if(found != sym_map.end()) {
		return found->second;
	} else {
		return sym_map.emplace(s, GiNaC::symbol(s)).first->second;
	}
}

