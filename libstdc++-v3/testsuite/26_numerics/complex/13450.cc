// { dg-do run { xfail { powerpc*-*-linux* && broken_cplxf_arg } } }

// Copyright (C) 2004 Free Software Foundation
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// 26.2.8 complex transcendentals

#include <complex>
#include <limits>
#include <testsuite_hooks.h>

template<typename T>
  void test01_do(T a, T b)
  {
    using namespace std;
    bool test __attribute__((unused)) = true;
    typedef complex<T> cplx;

    T eps = numeric_limits<T>::epsilon() * 100;
    
    cplx ref = pow(cplx(a, T()), cplx(b, T()));
    cplx res1 = pow(a, cplx(b, T()));
    cplx res2 = pow(cplx(a, T()), b);
    
    VERIFY( abs(ref - res1) < eps );
    VERIFY( abs(ref - res2) < eps );
    VERIFY( abs(res1 - res2) < eps );
  }

// libstdc++/13450
void test01()
{
  float f1 = -1.0f;
  float f2 = 0.5f;
  test01_do(f1, f2);

  f1 = -3.2f;
  f2 = 1.4f;
  test01_do(f1, f2);

  double d1 = -1.0;
  double d2 = 0.5;
  test01_do(d1, d2);

  d1 = -3.2;
  d2 = 1.4;
  test01_do(d1, d2);

  long double ld1 = -1.0l;
  long double ld2 = 0.5l;
  test01_do(ld1, ld2);

  ld1 = -3.2l;
  ld2 = 1.4l;
  test01_do(ld1, ld2);
}

int main()
{
  test01();
  return 0;
}
