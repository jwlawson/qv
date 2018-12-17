/*
 * link_holder.h
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
 * Used in MutationClassLoader. Tracks where mutations have been computed
 * previously.
 *
 * TODO Probably should be a private class.
 */
#pragma once

#include <memory>
#include <vector>

namespace cluster {
template <class T>
class LinkHolder {
 public:
  LinkHolder();
  LinkHolder(const int k);
  ~LinkHolder();
  void size(const int size);
  void matrix(std::shared_ptr<T> mat);
  std::shared_ptr<T> matrix() const;
  void link(const int k);
  bool has_link(const int k) const;
  bool is_complete() const;

 private:
  std::shared_ptr<T> matrix_;
  std::vector<bool> links_;
  int size_;
};
}  // namespace cluster
