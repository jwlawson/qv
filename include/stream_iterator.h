/*
 * stream_iterator.h
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
 * Reads lines from an input stream and creates instances of T, using the
 * constructor T(std::string).
 */
#pragma once

#include <istream>
#include <memory>
#include <string>

namespace cluster {
template <class T>
class StreamIterator {
 private:
  typedef std::shared_ptr<std::istream> StreamPtr;

 public:
  /**
   * Create an empty iterator.
   */
  StreamIterator();
  /**
   * Create a new iterator to extract all matrices from the specified input
   * stream.
   * @param stream Stream containing matrices
   */
  StreamIterator(std::istream& stream);
  /**
   * Get the next matrix from the input stream.
   * @return The next matrix
   */
  std::shared_ptr<T> next();
  /**
   * Check whether the iterator will return a valid matrix when next() is
   * called.
   * @return true if next() will return a valid matrix
   */
  bool has_next();
  StreamIterator& operator=(const StreamIterator& iter) = default;

 private:
  /** True if next() will return a valid matrix. */
  bool has_next_;
  /** String buffer to store the current matrix string in. */
  std::string str_;
  /** Pointer to the input stream to extract matrices from. */
  StreamPtr input_;

  /**
   * Deleter for the StreamPtr. As the pointer is always made from
   * references to input streams they should not be deleted after the
   * shared_ptr goes out of scope. This deleter actually does nothing when
   * the shared_ptr determines that it should be deleted.
   */
  struct NullDeleter {
    void operator()(const void* const) const {}
  };
};
}  // namespace cluster
