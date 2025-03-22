// Copyright (c) 2015, The Regents of the University of California (Regents)
// See LICENSE.txt for license details

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <cinttypes>
#include <string>
#include <sys/resource.h>

#include "timer.h"


/*
GAP Benchmark Suite
Author: Scott Beamer

Miscellaneous helpers that don't fit into classes
*/


static const int64_t kRandSeed = 27491095;


void PrintLabel(const std::string &label, const std::string &val) {
  printf("%-21s%7s\n", (label + ":").c_str(), val.c_str());
}

void PrintTime(const std::string &s, double seconds) {
  printf("%-21s%3.5lf\n", (s + ":").c_str(), seconds);
}

void PrintStep(const std::string &s, int64_t count) {
  printf("%-14s%14" PRId64 "\n", (s + ":").c_str(), count);
}

void PrintStep(const std::string &s, double seconds, int64_t count = -1) {
  if (count != -1)
    printf("%5s%11" PRId64 "  %10.5lf\n", s.c_str(), count, seconds);
  else
    printf("%5s%23.5lf\n", s.c_str(), seconds);
}

void PrintStep(int step, double seconds, int64_t count = -1) {
  PrintStep(std::to_string(step), seconds, count);
}

// Runs op and prints the time it took to execute labelled by label
#define TIME_PRINT(label, op) {   \
  Timer t_;                       \
  t_.Start();                     \
  (op);                           \
  t_.Stop();                      \
  PrintTime(label, t_.Seconds()); \
}


template <typename T_>
class RangeIter {
  T_ x_;
 public:
  explicit RangeIter(T_ x) : x_(x) {}
  bool operator!=(RangeIter const& other) const { return x_ != other.x_; }
  T_ const& operator*() const { return x_; }
  RangeIter& operator++() {
    ++x_;
    return *this;
  }
};

template <typename T_>
class Range{
  T_ from_;
  T_ to_;
 public:
  explicit Range(T_ to) : from_(0), to_(to) {}
  Range(T_ from, T_ to) : from_(from), to_(to) {}
  RangeIter<T_> begin() const { return RangeIter<T_>(from_); }
  RangeIter<T_> end() const { return RangeIter<T_>(to_); }
};

namespace _flexograph_profile {
    /**
     * Copied from Blaze. This class is used to measure the memory usage of the program.
     */
    class MemoryCounter {
    public:
      MemoryCounter() {
        getrusage(RUSAGE_SELF, &__memory);
        __previous_mem = __memory.ru_maxrss;
        __maj_faults = __memory.ru_majflt;
        __min_faults = __memory.ru_minflt;
        __inblock = __memory.ru_inblock;
        __oublock = __memory.ru_oublock;
      }

      ~MemoryCounter() {
        getrusage(RUSAGE_SELF, &__memory);
        uint64_t used_mem = __memory.ru_maxrss - __previous_mem;
        uint64_t maj_faults = __memory.ru_majflt - __maj_faults;
        uint64_t min_faults = __memory.ru_minflt - __min_faults;
        uint64_t inblock = __memory.ru_inblock - __inblock;
        uint64_t oublock = __memory.ru_oublock - __oublock;
        printf("MemoryCounter: %lu MB -> %lu MB, %lu MB total\n", __previous_mem/1024, __memory.ru_maxrss/1024, used_mem/1024);
        printf("MemoryCounter: %lu major faults, %lu minor faults\n", maj_faults, min_faults);
        printf("MemoryCounter: %lu block input operations, %lu block output operations\n", inblock, oublock);
      }

    private:
      struct rusage __memory;
      uint64_t __previous_mem;
      uint64_t __maj_faults;
      uint64_t __min_faults;
      uint64_t __inblock;
      uint64_t __oublock;
    };
} // end namespace _flexograph_profile

#endif  // UTIL_H_
