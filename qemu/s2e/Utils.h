/*
 * S2E Selective Symbolic Execution Framework
 *
 * Copyright (c) 2010, Dependable Systems Laboratory, EPFL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Dependable Systems Laboratory, EPFL nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE DEPENDABLE SYSTEMS LABORATORY, EPFL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main authors: Vitaly Chipounov, Volodymyr Kuznetsov.
 * All S2E contributors are listed in the S2E-AUTHORS file.
 *
 */

#ifndef S2E_UTILS_H
#define S2E_UTILS_H

#include <cstdio>
#include <cassert>
#include <ostream>
#include <iomanip>
#include <inttypes.h>

namespace s2e {

struct hexval {
    uint64_t value;
    int width;

    hexval(uint64_t _value, int _width=0) : value(_value), width(_width) {}
    hexval(void* _value, int _width=0): value((uint64_t)_value), width(_width) {}
};

inline std::ostream& operator<<(std::ostream& out, const hexval& h)
{
    out << "0x";

    std::ios_base::fmtflags oldf = out.flags();
    out.setf(std::ios::hex, std::ios_base::basefield);
    std::streamsize oldw = out.width(); out.width(h.width);
    char oldfill = out.fill(); out.fill('0');

    out << h.value;

    out.flags(oldf); out.width(oldw); out.fill(oldfill);
    return out;
}

/** A macro used to escape "," in an argument to another macro */
#define S2E_NOOP(...) __VA_ARGS__

#ifdef NDEBUG
#define DPRINTF(...)
#define TRACE(...) 
#else
#define DPRINTF(...) printf(__VA_ARGS__)
#define TRACE(...) { printf("%s - ", __FUNCTION__); printf(__VA_ARGS__); }
#endif

/* The following is GCC-specific implementation of foreach.
   Should handle correctly all crazy C++ corner cases */

template <typename T>
class _S2EForeachContainer {
public:
    inline _S2EForeachContainer(const T& t) : c(t), brk(0), i(c.begin()), e(c.end()) { }
    const T c; /* Compiler will remove the copying here */
    int brk;
    typename T::const_iterator i, e;
};

#define foreach(variable, container) \
for (_S2EForeachContainer<__typeof__(container)> _container_(container); \
     !_container_.brk && _container_.i != _container_.e; \
     __extension__  ({ ++_container_.brk; ++_container_.i; })) \
    for (variable = *_container_.i;; __extension__ ({--_container_.brk; break;}))

#define foreach2(_i, _b, _e) \
      for(typeof(_b) _i = _b, _i ## end = _e; _i != _i ## end;  ++ _i) 

} // namespace s2e

#endif // S2E_UTILS_H
