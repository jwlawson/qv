/*
 * equiv_mutation_class_loader.h
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
 * Version of the MutationClassLoader which uses EquivQuiverMatrices. Mostly the
 * same but the equivalence requires some extra fiddling to reduce the number of
 * mutations needed.
 */
#pragma once

#include "mutation_class_loader.h"

#include <unordered_set>

#include "equiv_quiver_matrix.h"

namespace cluster {
class EquivMutationClassLoader : public MutationClassLoader<EquivQuiverMatrix> {
 private:
  typedef EquivQuiverMatrix M;

 public:
  EquivMutationClassLoader();
  EquivMutationClassLoader(const M& initial);

 protected:
  virtual void seen_matrix(std::shared_ptr<M> mat, std::shared_ptr<M> previous,
                           const int vertex);
  virtual void unseen_matrix(std::shared_ptr<M> matrix,
                             std::shared_ptr<M> previous, const int vertex);
  virtual bool have_seen(std::shared_ptr<M> matrix);

 private:
  std::unordered_set<std::shared_ptr<M>> set_;
};
}  // namespace cluster
