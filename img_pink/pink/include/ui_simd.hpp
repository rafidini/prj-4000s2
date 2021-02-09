/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef SIMD_HPP_
#define SIMD_HPP_

#if defined PINK_HAVE_SIMD


//#include <immintrin.h>



#if defined (__SSE4_2__) || defined (__SSE4_1__)
# ifndef _WINDOWS
#   warning: highest simd instruction set available at compilation SSE4
# endif /* _WINDOWS */
#elif defined (__SSE3__)
# ifndef _WINDOWS
#   warning: highest simd instruction set available at compilation SSE3
# endif /* _WINDOWS */
# define _mm_blendv_ps(a, b, m) _mm_xor_ps(a, _mm_and_ps(_mm_xor_ps(b, a), m))
#elif defined (__SSE2__)
# define _mm_blendv_ps(a, b, m) _mm_xor_ps(a, _mm_and_ps(_mm_xor_ps(b, a), m))
# ifndef _WINDOWS
#   warning: highest simd instruction set available at compilation SSE2
# endif /* _WINDOWS */
#elif defined (__SSE__)
# define _mm_blendv_ps(a, b, m) _mm_xor_ps(a, _mm_and_ps(_mm_xor_ps(b, a), m))
# ifndef _WINDOWS
#   warning: highest simd instruction set available at compilation SSE1
# endif /* _WINDOWS */
#else /* NOT defined (__SSE__) */
# define _mm_blendv_ps(a, b, m) _mm_xor_ps(a, _mm_and_ps(_mm_xor_ps(b, a), m))
# ifndef _WINDOWS
#   warning: NO SIMD SUPPORT
# endif /* _WINDOWS */
#endif /* NOT defined (__SSE__) */

// equivalent with a < b ? c : d
#define _mm_select_gt( a, b, c, d ) _mm_blendv_ps(c, d, _mm_cmpngt_ps( a, b ) )
#define _mm_select_ge( a, b, c, d ) _mm_blendv_ps(c, d, _mm_cmpnge_ps( a, b ) )
#define _mm_select_lt( a, b, c, d ) _mm_blendv_ps(c, d, _mm_cmpnlt_ps( a, b ) )
#define _mm_select_le( a, b, c, d ) _mm_blendv_ps(c, d, _mm_cmpnle_ps( a, b ) )

#define _mm_neg_ps(a) _mm_xor_ps   ( a, simd::signmask )
#define _mm_abs_ps(a) _mm_andnot_ps( simd::signmask, a )
//
//
//namespace pink {
//  namespace simd {
//
//    typedef __m128 v4sf;
//    //typedef float         v4sf __attribute__ ((vector_size(16))); // vector of four single floats
//
//
//
//    union vec
//    {
//    public:
//      v4sf v;
//      float f[4];
//      unsigned int i[4];
//      operator v4sf&();
//    }; /* union vec */
//
//
//
//    inline
//    vec::operator v4sf&()
//    {
//      return (this->v);
//    }
//
//
//
//    inline
//    vec
//    operator* ( const vec & a, const vec & b )
//    {
//      vec result;
//      result.v = _mm_mul_ps(a.v, b.v);
//      return (result);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec
//    operator* ( const v4sf & a, const vec & b )
//    {
//      vec result;
//      result.v = _mm_mul_ps(a, b.v);
//      return (result);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec
//    operator* ( const vec & a, const v4sf & b )
//    {
//      vec result;
//      result.v = _mm_mul_ps(a.v, b);
//      return (result);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec
//    operator+ ( const vec & a, const vec & b )
//    {
//      vec result;
//      result.v = _mm_add_ps(a.v, b.v);
//      return (result);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec
//    operator+ ( const v4sf & a, const vec & b )
//    {
//      vec result;
//      result.v = _mm_add_ps(a, b.v);
//      return (result);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec
//    operator+ ( const vec & a, const v4sf & b )
//    {
//      vec result;
//      result.v = _mm_add_ps(a.v, b);
//      return (result);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec
//    operator- ( const vec & a, const vec & b )
//    {
//      vec result;
//      result.v = _mm_sub_ps(a.v, b.v);
//      return (result);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec &
//    operator-= ( vec & a, const vec & b )
//    {
//      a.v = _mm_sub_ps(a.v, b.v);
//      return (a);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec &
//    operator+= ( vec & a, const vec & b )
//    {
//      a.v = _mm_add_ps(a.v, b.v);
//      return (a);
//    } /* operator vec * vec */
//
//
//
//    inline
//    vec &
//    operator*= ( vec & a, const vec & b )
//    {
//      a.v = _mm_mul_ps(a.v, b.v);
//      return (a);
//    } /* operator vec * vec */
//
//
//    const v4sf one         = { 1, 1, 1, 1 };
//    const v4sf zero        = { 0, 0, 0, 0 };
//    const v4sf vepsilon    = { 0.00001, 0.00001, 0.00001, 0.00001 };
//    const v4sf marker      = { 0.453, 0.243, 0.123, 0.069 };
//    const unsigned int nan = 0xffffffff;
//    const v4sf signmask    = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
//
//
//
//  } /* namespace simd */
//} /* end namespace pink */

#endif // PINK_HAVE_SIMD

#endif /* SIMD_HPP_ */
// LuM end of file
