/*
 * mass_finite_check.cc
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
#include "mass_finite_check.h"

#include "fast_infinite.h"

namespace cluster {

	MassFiniteCheck::MassFiniteCheck()
		: set_{ std::make_shared<MSet>() }
		, last_new_{false}
		, last_checked_{ std::make_shared<M>() }
	{}

	bool MassFiniteCheck::is_finite(const M& matrix) {
		if(matrix.num_rows() < 2 && matrix.num_cols() < 2) {
			return true;
		}
		*last_checked_ = matrix;
		last_new_ = false;
		if(set_contains(last_checked_)) {
			return true;
		}
		if(fastinf::is_infinite(matrix)) {
			return false;
		}
		MutationClass c(matrix);
		if(c.is_finite()) {
			last_new_ = true;
			add_class(c);
			return true;
		} else {
			return false;
		}
	}

	bool MassFiniteCheck::last_new_class() const {
		return last_new_;
	}

	std::shared_ptr<const MassFiniteCheck::MSet> MassFiniteCheck::set() const {
		return set_;
	}

	void MassFiniteCheck::add_finite(const MPtr& matrix) {
		set_->insert(matrix);
	}

	bool MassFiniteCheck::set_contains(MPtr mat) const {
		return set_->find(mat) != set_->end();
	}

	void MassFiniteCheck::add_class(const MutationClass& c){
		set_->insert(c.begin(), c.end());
	}

}

