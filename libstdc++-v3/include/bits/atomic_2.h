// -*- C++ -*- header.

// Copyright (C) 2008, 2009, 2010
// Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/atomic_2.h 
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{atomic}
 */

#ifndef _GLIBCXX_ATOMIC_2_H
#define _GLIBCXX_ATOMIC_2_H 1

#pragma GCC system_header

_GLIBCXX_BEGIN_NAMESPACE(std)

// 2 == __atomic2 == Always lock-free
// Assumed:
// _GLIBCXX_ATOMIC_BUILTINS_1
// _GLIBCXX_ATOMIC_BUILTINS_2
// _GLIBCXX_ATOMIC_BUILTINS_4
// _GLIBCXX_ATOMIC_BUILTINS_8
namespace __atomic2
{
  /// atomic_flag
  struct atomic_flag : public __atomic_flag_base
  {
    atomic_flag() = default;
    ~atomic_flag() = default;
    atomic_flag(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) = delete;
    atomic_flag& operator=(const atomic_flag&) volatile = delete;

    // Conversion to ATOMIC_FLAG_INIT.
    atomic_flag(bool __i): __atomic_flag_base({ __i }) { }

    bool
    test_and_set(memory_order __m = memory_order_seq_cst)
    {
      // Redundant synchronize if built-in for lock is a full barrier.
      if (__m != memory_order_acquire && __m != memory_order_acq_rel)
	__sync_synchronize();
      return __sync_lock_test_and_set(&_M_i, 1);
    }

    bool
    test_and_set(memory_order __m = memory_order_seq_cst) volatile
    {
      // Redundant synchronize if built-in for lock is a full barrier.
      if (__m != memory_order_acquire && __m != memory_order_acq_rel)
	__sync_synchronize();
      return __sync_lock_test_and_set(&_M_i, 1);
    }

    void
    clear(memory_order __m = memory_order_seq_cst)
    {
      __glibcxx_assert(__m != memory_order_consume);
      __glibcxx_assert(__m != memory_order_acquire);
      __glibcxx_assert(__m != memory_order_acq_rel);

      __sync_lock_release(&_M_i);
      if (__m != memory_order_acquire && __m != memory_order_acq_rel)
	__sync_synchronize();
    }

    void
    clear(memory_order __m = memory_order_seq_cst) volatile
    {
      __glibcxx_assert(__m != memory_order_consume);
      __glibcxx_assert(__m != memory_order_acquire);
      __glibcxx_assert(__m != memory_order_acq_rel);

      __sync_lock_release(&_M_i);
      if (__m != memory_order_acquire && __m != memory_order_acq_rel)
	__sync_synchronize();
    }
  };


  /// atomic_address
  struct atomic_address
  {
  private:
    void* _M_i;

  public:
    atomic_address() = default;
    ~atomic_address() = default;
    atomic_address(const atomic_address&) = delete;
    atomic_address& operator=(const atomic_address&) = delete;
    atomic_address& operator=(const atomic_address&) volatile = delete;

    constexpr atomic_address(void* __v): _M_i (__v) {  }

    bool
    is_lock_free() const { return true; }

    bool
    is_lock_free() const volatile { return true; }

    void
    store(void* __v, memory_order __m = memory_order_seq_cst)
    {
      __glibcxx_assert(__m != memory_order_acquire);
      __glibcxx_assert(__m != memory_order_acq_rel);
      __glibcxx_assert(__m != memory_order_consume);

      if (__m == memory_order_relaxed)
	_M_i = __v;
      else
	{
	  // write_mem_barrier();
	  _M_i = __v;
	  if (__m == memory_order_seq_cst)
	    __sync_synchronize();
	}
    }

    void
    store(void* __v, memory_order __m = memory_order_seq_cst) volatile
    {
      __glibcxx_assert(__m != memory_order_acquire);
      __glibcxx_assert(__m != memory_order_acq_rel);
      __glibcxx_assert(__m != memory_order_consume);

      if (__m == memory_order_relaxed)
	_M_i = __v;
      else
	{
	  // write_mem_barrier();
	  _M_i = __v;
	  if (__m == memory_order_seq_cst)
	    __sync_synchronize();
	}
    }

    void*
    load(memory_order __m = memory_order_seq_cst) const
    {
      __glibcxx_assert(__m != memory_order_release);
      __glibcxx_assert(__m != memory_order_acq_rel);

      __sync_synchronize();
      void* __ret = _M_i;
      __sync_synchronize();
      return __ret;
    }

    void*
    load(memory_order __m = memory_order_seq_cst) const volatile
    {
      __glibcxx_assert(__m != memory_order_release);
      __glibcxx_assert(__m != memory_order_acq_rel);

      __sync_synchronize();
      void* __ret = _M_i;
      __sync_synchronize();
      return __ret;
    }

    void*
    exchange(void* __v, memory_order __m = memory_order_seq_cst)
    {
      // XXX built-in assumes memory_order_acquire.
      return __sync_lock_test_and_set(&_M_i, __v);
    }

    void*
    exchange(void* __v, memory_order __m = memory_order_seq_cst) volatile
    {
      // XXX built-in assumes memory_order_acquire.
      return __sync_lock_test_and_set(&_M_i, __v);
    }

    bool
    compare_exchange_weak(void*& __v1, void* __v2, memory_order __m1,
			  memory_order __m2)
    { return compare_exchange_strong(__v1, __v2, __m1, __m2); }

    bool
    compare_exchange_weak(void*& __v1, void* __v2, memory_order __m1,
			  memory_order __m2) volatile
    { return compare_exchange_strong(__v1, __v2, __m1, __m2); }

    bool
    compare_exchange_weak(void*& __v1, void* __v2,
			  memory_order __m = memory_order_seq_cst)
    {
      return compare_exchange_weak(__v1, __v2, __m,
				   __calculate_memory_order(__m));
    }

    bool
    compare_exchange_weak(void*& __v1, void* __v2,
			  memory_order __m = memory_order_seq_cst) volatile
    {
      return compare_exchange_weak(__v1, __v2, __m,
				   __calculate_memory_order(__m));
    }

    bool
    compare_exchange_weak(const void*& __v1, const void* __v2,
			  memory_order __m1, memory_order __m2)
    { return compare_exchange_strong(__v1, __v2, __m1, __m2); }

    bool
    compare_exchange_weak(const void*& __v1, const void* __v2,
			  memory_order __m1, memory_order __m2) volatile
    { return compare_exchange_strong(__v1, __v2, __m1, __m2); }

    bool
    compare_exchange_weak(const void*& __v1, const void* __v2,
			  memory_order __m = memory_order_seq_cst)
    {
      return compare_exchange_weak(__v1, __v2, __m,
				   __calculate_memory_order(__m));
    }

    bool
    compare_exchange_weak(const void*& __v1, const void* __v2,
			  memory_order __m = memory_order_seq_cst) volatile
    {
      return compare_exchange_weak(__v1, __v2, __m,
				   __calculate_memory_order(__m));
    }

    bool
    compare_exchange_strong(void*& __v1, void* __v2, memory_order __m1,
			    memory_order __m2)
    {
      __glibcxx_assert(__m2 != memory_order_release);
      __glibcxx_assert(__m2 != memory_order_acq_rel);
      __glibcxx_assert(__m2 <= __m1);

      void* __v1o = __v1;
      void* __v1n = __sync_val_compare_and_swap(&_M_i, __v1o, __v2);

      // Assume extra stores (of same value) allowed in true case.
      __v1 = __v1n;
      return __v1o == __v1n;
    }

    bool
    compare_exchange_strong(void*& __v1, void* __v2, memory_order __m1,
			    memory_order __m2) volatile
    {
      __glibcxx_assert(__m2 != memory_order_release);
      __glibcxx_assert(__m2 != memory_order_acq_rel);
      __glibcxx_assert(__m2 <= __m1);

      void* __v1o = __v1;
      void* __v1n = __sync_val_compare_and_swap(&_M_i, __v1o, __v2);

      // Assume extra stores (of same value) allowed in true case.
      __v1 = __v1n;
      return __v1o == __v1n;
    }

    bool
    compare_exchange_strong(void*& __v1, void* __v2,
			    memory_order __m = memory_order_seq_cst)
    {
      return compare_exchange_strong(__v1, __v2, __m,
				     __calculate_memory_order(__m));
    }

    bool
    compare_exchange_strong(void*& __v1, void* __v2,
			    memory_order __m = memory_order_seq_cst) volatile
    {
      return compare_exchange_strong(__v1, __v2, __m,
				     __calculate_memory_order(__m));
    }

    bool
    compare_exchange_strong(const void*& __v1, const void* __v2,
			    memory_order __m1, memory_order __m2)
    {
      __glibcxx_assert(__m2 != memory_order_release);
      __glibcxx_assert(__m2 != memory_order_acq_rel);
      __glibcxx_assert(__m2 <= __m1);

      const void* __v1o = __v1;
      const void* __v1n = __sync_val_compare_and_swap(&_M_i, __v1o, __v2);

      // Assume extra stores (of same value) allowed in true case.
      __v1 = __v1n;
      return __v1o == __v1n;
    }

    bool
    compare_exchange_strong(const void*& __v1, const void* __v2,
			    memory_order __m1, memory_order __m2) volatile
    {
      __glibcxx_assert(__m2 != memory_order_release);
      __glibcxx_assert(__m2 != memory_order_acq_rel);
      __glibcxx_assert(__m2 <= __m1);

      const void* __v1o = __v1;
      const void* __v1n = __sync_val_compare_and_swap(&_M_i, __v1o, __v2);

      // Assume extra stores (of same value) allowed in true case.
      __v1 = __v1n;
      return __v1o == __v1n;
    }

    bool
    compare_exchange_strong(const void*& __v1, const void* __v2,
			    memory_order __m = memory_order_seq_cst)
    {
      return compare_exchange_strong(__v1, __v2, __m,
				     __calculate_memory_order(__m));
    }

    bool
    compare_exchange_strong(const void*& __v1, const void* __v2,
			    memory_order __m = memory_order_seq_cst) volatile
    {
      return compare_exchange_strong(__v1, __v2, __m,
				     __calculate_memory_order(__m));
    }

    void*
    fetch_add(ptrdiff_t __d, memory_order __m = memory_order_seq_cst)
    { return __sync_fetch_and_add(&_M_i, __d); }

    void*
    fetch_add(ptrdiff_t __d, memory_order __m = memory_order_seq_cst) volatile
    { return __sync_fetch_and_add(&_M_i, __d); }

    void*
    fetch_sub(ptrdiff_t __d, memory_order __m = memory_order_seq_cst)
    { return __sync_fetch_and_sub(&_M_i, __d); }

    void*
    fetch_sub(ptrdiff_t __d, memory_order __m = memory_order_seq_cst) volatile
    { return __sync_fetch_and_sub(&_M_i, __d); }

    operator void*() const
    { return load(); }

    operator void*() const volatile
    { return load(); }

    void*
#if 0
    // XXX as specified but won't compile as store takes void*,
    // invalid conversion from const void* to void*
    // CD1 had this signature
    operator=(const void* __v)
#else
    operator=(void* __v)      
#endif
    {
      store(__v);
      return __v;
    }

    void*
#if 0
    // XXX as specified but won't compile as store takes void*,
    // invalid conversion from const void* to void*
    // CD1 had this signature, but store and this could both be const void*?
    operator=(const void* __v) volatile
#else
    operator=(void* __v) volatile
#endif
    {
      store(__v);
      return __v;
    }

    void*
    operator+=(ptrdiff_t __d)
    { return __sync_add_and_fetch(&_M_i, __d); }

    void*
    operator+=(ptrdiff_t __d) volatile
    { return __sync_add_and_fetch(&_M_i, __d); }

    void*
    operator-=(ptrdiff_t __d)
    { return __sync_sub_and_fetch(&_M_i, __d); }

    void*
    operator-=(ptrdiff_t __d) volatile
    { return __sync_sub_and_fetch(&_M_i, __d); }
  };


  /// Base class for atomic integrals.
  //
  // For each of the integral types, define atomic_[integral type] struct
  //
  // atomic_bool     bool
  // atomic_char     char
  // atomic_schar    signed char
  // atomic_uchar    unsigned char
  // atomic_short    short
  // atomic_ushort   unsigned short
  // atomic_int      int
  // atomic_uint     unsigned int
  // atomic_long     long
  // atomic_ulong    unsigned long
  // atomic_llong    long long
  // atomic_ullong   unsigned long long
  // atomic_char16_t char16_t
  // atomic_char32_t char32_t
  // atomic_wchar_t  wchar_t
  //
  // NB: Assuming _ITp is an integral scalar type that is 1, 2, 4, or
  // 8 bytes, since that is what GCC built-in functions for atomic
  // memory access expect.
  template<typename _ITp>
    struct __atomic_base
    {
    private:
      typedef _ITp 	__int_type;

      __int_type 	_M_i;

    public:
      __atomic_base() = default;
      ~__atomic_base() = default;
      __atomic_base(const __atomic_base&) = delete;
      __atomic_base& operator=(const __atomic_base&) = delete;
      __atomic_base& operator=(const __atomic_base&) volatile = delete;

      // Requires __int_type convertible to _M_i.
      constexpr __atomic_base(__int_type __i): _M_i (__i) { }

      operator __int_type() const
      { return load(); }

      operator __int_type() const volatile
      { return load(); }

      __int_type
      operator=(__int_type __i)
      {
	store(__i);
	return __i;
      }

      __int_type
      operator=(__int_type __i) volatile
      {
	store(__i);
	return __i;
      }

      __int_type
      operator++(int)
      { return fetch_add(1); }

      __int_type
      operator++(int) volatile
      { return fetch_add(1); }

      __int_type
      operator--(int)
      { return fetch_sub(1); }

      __int_type
      operator--(int) volatile
      { return fetch_sub(1); }

      __int_type
      operator++()
      { return __sync_add_and_fetch(&_M_i, 1); }

      __int_type
      operator++() volatile
      { return __sync_add_and_fetch(&_M_i, 1); }

      __int_type
      operator--()
      { return __sync_sub_and_fetch(&_M_i, 1); }

      __int_type
      operator--() volatile
      { return __sync_sub_and_fetch(&_M_i, 1); }

      __int_type
      operator+=(__int_type __i)
      { return __sync_add_and_fetch(&_M_i, __i); }

      __int_type
      operator+=(__int_type __i) volatile
      { return __sync_add_and_fetch(&_M_i, __i); }

      __int_type
      operator-=(__int_type __i)
      { return __sync_sub_and_fetch(&_M_i, __i); }

      __int_type
      operator-=(__int_type __i) volatile
      { return __sync_sub_and_fetch(&_M_i, __i); }

      __int_type
      operator&=(__int_type __i)
      { return __sync_and_and_fetch(&_M_i, __i); }

      __int_type
      operator&=(__int_type __i) volatile
      { return __sync_and_and_fetch(&_M_i, __i); }

      __int_type
      operator|=(__int_type __i)
      { return __sync_or_and_fetch(&_M_i, __i); }

      __int_type
      operator|=(__int_type __i) volatile
      { return __sync_or_and_fetch(&_M_i, __i); }

      __int_type
      operator^=(__int_type __i)
      { return __sync_xor_and_fetch(&_M_i, __i); }

      __int_type
      operator^=(__int_type __i) volatile
      { return __sync_xor_and_fetch(&_M_i, __i); }

      bool
      is_lock_free() const
      { return true; }

      bool
      is_lock_free() const volatile
      { return true; }

      void
      store(__int_type __i, memory_order __m = memory_order_seq_cst)
      {
	__glibcxx_assert(__m != memory_order_acquire);
	__glibcxx_assert(__m != memory_order_acq_rel);
	__glibcxx_assert(__m != memory_order_consume);

	if (__m == memory_order_relaxed)
	  _M_i = __i;
	else
	  {
	    // write_mem_barrier();
	    _M_i = __i;
	    if (__m == memory_order_seq_cst)
	      __sync_synchronize();
	  }
      }

      void
      store(__int_type __i, memory_order __m = memory_order_seq_cst) volatile
      {
	__glibcxx_assert(__m != memory_order_acquire);
	__glibcxx_assert(__m != memory_order_acq_rel);
	__glibcxx_assert(__m != memory_order_consume);

	if (__m == memory_order_relaxed)
	  _M_i = __i;
	else
	  {
	    // write_mem_barrier();
	    _M_i = __i;
	    if (__m == memory_order_seq_cst)
	      __sync_synchronize();
	  }
      }

      __int_type
      load(memory_order __m = memory_order_seq_cst) const
      {
	__glibcxx_assert(__m != memory_order_release);
	__glibcxx_assert(__m != memory_order_acq_rel);

	__sync_synchronize();
	__int_type __ret = _M_i;
	__sync_synchronize();
	return __ret;
      }

      __int_type
      load(memory_order __m = memory_order_seq_cst) const volatile
      {
	__glibcxx_assert(__m != memory_order_release);
	__glibcxx_assert(__m != memory_order_acq_rel);

	__sync_synchronize();
	__int_type __ret = _M_i;
	__sync_synchronize();
	return __ret;
      }

      __int_type
      exchange(__int_type __i, memory_order __m = memory_order_seq_cst)
      {
	// XXX built-in assumes memory_order_acquire.
	return __sync_lock_test_and_set(&_M_i, __i);
      }


      __int_type
      exchange(__int_type __i, memory_order __m = memory_order_seq_cst) volatile
      {
	// XXX built-in assumes memory_order_acquire.
	return __sync_lock_test_and_set(&_M_i, __i);
      }

      bool
      compare_exchange_weak(__int_type& __i1, __int_type __i2,
			    memory_order __m1, memory_order __m2)
      { return compare_exchange_strong(__i1, __i2, __m1, __m2); }

      bool
      compare_exchange_weak(__int_type& __i1, __int_type __i2,
			    memory_order __m1, memory_order __m2) volatile
      { return compare_exchange_strong(__i1, __i2, __m1, __m2); }

      bool
      compare_exchange_weak(__int_type& __i1, __int_type __i2,
			    memory_order __m = memory_order_seq_cst)
      {
	return compare_exchange_weak(__i1, __i2, __m,
				     __calculate_memory_order(__m));
      }

      bool
      compare_exchange_weak(__int_type& __i1, __int_type __i2,
			    memory_order __m = memory_order_seq_cst) volatile
      {
	return compare_exchange_weak(__i1, __i2, __m,
				     __calculate_memory_order(__m));
      }

      bool
      compare_exchange_strong(__int_type& __i1, __int_type __i2,
			      memory_order __m1, memory_order __m2)
      {
	__glibcxx_assert(__m2 != memory_order_release);
	__glibcxx_assert(__m2 != memory_order_acq_rel);
	__glibcxx_assert(__m2 <= __m1);

	__int_type __i1o = __i1;
	__int_type __i1n = __sync_val_compare_and_swap(&_M_i, __i1o, __i2);

	// Assume extra stores (of same value) allowed in true case.
	__i1 = __i1n;
	return __i1o == __i1n;
      }

      bool
      compare_exchange_strong(__int_type& __i1, __int_type __i2,
			      memory_order __m1, memory_order __m2) volatile
      {
	__glibcxx_assert(__m2 != memory_order_release);
	__glibcxx_assert(__m2 != memory_order_acq_rel);
	__glibcxx_assert(__m2 <= __m1);

	__int_type __i1o = __i1;
	__int_type __i1n = __sync_val_compare_and_swap(&_M_i, __i1o, __i2);

	// Assume extra stores (of same value) allowed in true case.
	__i1 = __i1n;
	return __i1o == __i1n;
      }

      bool
      compare_exchange_strong(__int_type& __i1, __int_type __i2,
			      memory_order __m = memory_order_seq_cst)
      {
	return compare_exchange_strong(__i1, __i2, __m,
				       __calculate_memory_order(__m));
      }

      bool
      compare_exchange_strong(__int_type& __i1, __int_type __i2,
			      memory_order __m = memory_order_seq_cst) volatile
      {
	return compare_exchange_strong(__i1, __i2, __m,
				       __calculate_memory_order(__m));
      }

      __int_type
      fetch_add(__int_type __i, memory_order __m = memory_order_seq_cst)
      { return __sync_fetch_and_add(&_M_i, __i); }

      __int_type
      fetch_add(__int_type __i,
		memory_order __m = memory_order_seq_cst) volatile
      { return __sync_fetch_and_add(&_M_i, __i); }

      __int_type
      fetch_sub(__int_type __i, memory_order __m = memory_order_seq_cst)
      { return __sync_fetch_and_sub(&_M_i, __i); }

      __int_type
      fetch_sub(__int_type __i,
		memory_order __m = memory_order_seq_cst) volatile
      { return __sync_fetch_and_sub(&_M_i, __i); }

      __int_type
      fetch_and(__int_type __i, memory_order __m = memory_order_seq_cst)
      { return __sync_fetch_and_and(&_M_i, __i); }

      __int_type
      fetch_and(__int_type __i,
		memory_order __m = memory_order_seq_cst) volatile
      { return __sync_fetch_and_and(&_M_i, __i); }

      __int_type
      fetch_or(__int_type __i, memory_order __m = memory_order_seq_cst)
      { return __sync_fetch_and_or(&_M_i, __i); }

      __int_type
      fetch_or(__int_type __i,
	       memory_order __m = memory_order_seq_cst) volatile
      { return __sync_fetch_and_or(&_M_i, __i); }

      __int_type
      fetch_xor(__int_type __i, memory_order __m = memory_order_seq_cst)
      { return __sync_fetch_and_xor(&_M_i, __i); }

      __int_type
      fetch_xor(__int_type __i,
		memory_order __m = memory_order_seq_cst) volatile
      { return __sync_fetch_and_xor(&_M_i, __i); }
    };
} // namespace __atomic2

_GLIBCXX_END_NAMESPACE

#endif
