/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by aplicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UIFLOW_SIMD_HPP_
# define UIFLOW_SIMD_HPP_
# define BOOST_DISABLE_ASSERTS
# if (defined PINK_HAVE_NUMA) && (defined PINK_HAVE_SIMD)

#   include <vector>
#   include <utility>
#   include <boost/python.hpp>
#   include <boost/preprocessor.hpp>
#   include <boost/typeof/typeof.hpp>

//#include "uiFlow.hpp"

#   include "ui_simd.hpp"
#   include "uiFrame.hpp"
#   include "lminmax.hpp"
#   include "ui_simd.hpp"
#   include "pyujimage.hpp"
#   include "ui_systest.hpp"
#   include "ui_numa_types.hpp"
#   include "ui_polythread.hpp"
#   include "ui_flow_ghosts.hpp"

namespace pink {

  namespace numa {

#   define REPORT_INTERVAL 10


    
    template <BOOST_PP_ENUM_PARAMS(10, class T)> // the macro expands to class T0, class T1, ..., class T(n-1)
    inline void kernel_pot (
      T0 pot,
      T1 srcsink,
      T2 flow,
      T3 p_c,
      T4 f_out,
      T5 f_in,
      T6 w,
      T7 tau,
      T8 length,
      T9 shift
      )
    {
      if (length == 0) return;      
      simd::v4sf vtau __attribute__ ((aligned(16))) = { tau, tau, tau, tau };
      simd::vec  norm __attribute__ ((aligned(16)));
      
      numa::slow_iterator_t<T0> pot_i    ( pot,     p_c + shift );
      numa::slow_iterator_t<T1> srcsink_i( srcsink, p_c + shift );
      numa::slow_iterator_t<typename T2::value_type> f_out_i( flow[w], f_out + shift );
      numa::slow_iterator_t<typename T2::value_type> f_in_i ( flow[w], f_in  + shift );

      // note: vtau is constant
      for( index_t id=0; id<length; id++, pot_i++, f_out_i++, f_in_i++, srcsink_i++ )
      {
        norm.v = _mm_select_gt( _mm_abs_ps(*srcsink_i), simd::zero, simd::zero, *pot_i );
        norm.v = _mm_select_gt( *srcsink_i, simd::zero, simd::one, norm );    
        *pot_i = norm - vtau * ( *f_out_i - *f_in_i );
      } /* FOR id in length */
    } /* kernel_pot */

    

    // for the shifting simd
    template <BOOST_PP_ENUM_PARAMS(9, class T)> // the macro expands to class T0, class T1, ..., class T(n-1)
    inline void kernel_pot_shift (
      T0 pot,
      T1 flow,
      T2 p_c,
      T3 f_out,
      T4 f_in,
      T5 w, // representing the dimension
      T6 tau,
      T7 length,
      T8 shift
      )
    {
      if (length == 0) return;
      simd::v4sf vtau        __attribute__ ((aligned(16))) = { tau, tau, tau, tau };
      simd::vec shifted_flow __attribute__ ((aligned(16)));
      simd::vec prev         __attribute__ ((aligned(16)));
      shifted_flow.v = _mm_shuffle_ps( simd::zero, flow[w][ f_in + shift ], _MM_SHUFFLE( 0, 0, 0, 3 ) );
      
      numa::slow_iterator_t<T0> pot_i  ( pot, p_c + shift );
      numa::slow_iterator_t<typename T1::value_type> f_out_i( flow[w], f_out + shift );
      numa::slow_iterator_t<typename T1::value_type> f_in_i ( flow[w], f_in  + shift + 1 );

      // this loop makes six simd operations per cycle (shuf, mov, sub, mul, sub, shuf)
      // this loop accesses 3(number of operands) * 4(simd) * 4(size of float) = 48 bytes per cycle 
      for( index_t id = 0; id < length; id++ )
      {
        prev.v         = _mm_shuffle_ps( *f_in_i, *f_in_i, _MM_SHUFFLE( 2, 1, 0, 3 ) );
        shifted_flow.v = _mm_move_ss( prev, shifted_flow );
        *pot_i        -= vtau * ( *f_out_i - shifted_flow );
        pot_i++; f_out_i++;        
        shifted_flow.v = _mm_shuffle_ps( *f_in_i, simd::zero, _MM_SHUFFLE( 0, 0, 0, 3 ) );
        f_in_i++;        
      } /* FOR id in length */
      return;      
    } /* kernel_pot_shift */



    template <BOOST_PP_ENUM_PARAMS(9, class T)> // the macro expands to class T0, class T1, ..., class T(n-1)
    inline void kernel_flow (
      T0 pot,
      T1 flow,
      T2 f,
      T3 p,
      T4 pp1,
      T5 w,
      T6 tau,
      T7 length,
      T8 shift
      )
    {
      if (length == 0) return;
      simd::v4sf vtau __attribute__ ((aligned(16))) = { tau, tau, tau, tau };
      numa::slow_iterator_t<T0> p_i( pot, p + shift );
      numa::slow_iterator_t<T0> pp1_i( pot, pp1 + shift );
      numa::slow_iterator_t<typename T1::value_type> f_i( flow[w], f + shift );

      // this loop makes 3 simd operations per cycle (sub mul sub)
      // this loop accesses 3(number of operands) * 4(simd) * 4(size of float) = 48 bytes per cycle
      for( index_t id=0; id<length; id++ )
      {
        *f_i -= vtau * ( *pp1_i - *p_i );
        f_i++; pp1_i++; p_i++;        
      } /* FOR id in length */
      return;      
    } /* kernel_flow */



    template <BOOST_PP_ENUM_PARAMS(9, class T)> // the macro expands to class T0, class T1, ..., class T(n-1)
    inline void kernel_flow_shift (
      T0 pot,
      T1 flow,
      T2 f,
      T3 p,
      T4 pp1,
      T5 w,
      T6 tau,
      T7 length,
      T8 shift
      )
    {
      if (length == 0) return;
      simd::v4sf vtau        __attribute__ ((aligned(16))) = { tau, tau, tau, tau };
      simd::vec  shifted_pot __attribute__ ((aligned(16)));
//      vec  prev        __attribute__ ((aligned(16)));

//      prev = pot[ pp1 + shift - 1 ]; //_mm_shuffle_ps( pot[ pp1 + shift - 1 ], pot[ pp1 + shift - 1 ], _MM_SHUFFLE(  ) )
      shifted_pot = pot[ pp1 + shift - 1 ];
      
      numa::slow_iterator_t<T0> p_i( pot, p + shift );
      numa::slow_iterator_t<T0> pp1_i( pot, pp1 + shift );
      numa::slow_iterator_t<typename T1::value_type> f_i( flow[w], f + shift );

      // this loop makes five simd operations per cycle ( mov, shuf, sub, mul, sub)
      // this loop accesses 3*4*4 = 48 bytes per cycle
      for( index_t id = 0; id < length; id++ )
      {
        shifted_pot.v = _mm_move_ss( shifted_pot, *pp1_i );
        shifted_pot.v = _mm_shuffle_ps( shifted_pot, shifted_pot, _MM_SHUFFLE( 0, 3, 2, 1 ) );
        *f_i         -= vtau * ( shifted_pot - *p_i );
        f_i++; _mm_prefetch( f_i,  _MM_HINT_NTA );
        p_i++; _mm_prefetch( p_i,  _MM_HINT_NTA );
        shifted_pot.v = *pp1_i;
        pp1_i++; _mm_prefetch( pp1_i,  _MM_HINT_T1 );
        //p_i++; f_i++; pp1_i++;                
      } /* FOR id in length */
    } /* kernel_flow_shift */



    template <BOOST_PP_ENUM_PARAMS(8, class T)> // the macro expands to class T0, class T1, ..., class T(n-1)
    inline void kernel_constrain (
      T0 flow,
      T1 g,
      T2 start,
      T3 length_glob,
      T4 & fm1s,
      T5 d,
      T6 length,
      T7 shift
      )
    {
      if (length == 0) return;
      simd::vec v             __attribute__ ((aligned(16)));
      simd::vec dFabs[4]      __attribute__ ((aligned(16)));
      simd::vec locInFlow[4]  __attribute__ ((aligned(16)));
      simd::vec locOutFlow[4] __attribute__ ((aligned(16)));
      simd::vec shifted_flow  __attribute__ ((aligned(16)));
      simd::vec gg_lt_v       __attribute__ ((aligned(16)));
      simd::vec gg_ge_v       __attribute__ ((aligned(16)));
      simd::vec quotient      __attribute__ ((aligned(16)));
      simd::vec prev          __attribute__ ((aligned(16)));
      simd::vec tmp           __attribute__ ((aligned(16)));
      simd::vec neg           __attribute__ ((aligned(16)));

      index_t q;
      float next;

      typedef numa::slow_iterator_t<typename T0::value_type> iterator_t;
            
      numa::slow_iterator_t<T1> g_i( g, start + shift );
      std::vector< iterator_t > f_i_i(d);
      std::vector< iterator_t > f_o_i(d);

      for( q=0; q<d; q++ )
      {
        f_o_i[q].reset( flow[q], start + shift );
        f_i_i[q].reset( flow[q], start - fm1s[q] + shift );
      } /* for q in 1..d */

      // next  = flow[0][ start - 1 + shift ].f[3];
      // f_i_i[0]++;
      // prev0 = flow[0][ start + shift ].f[0];
      // prev1 = flow[0][ start + shift ].f[1];
      // prev2 = flow[0][ start + shift ].f[2];

      shifted_flow.v  = _mm_shuffle_ps( simd::zero, flow[0][ start - 1 + shift ], _MM_SHUFFLE( 0, 0, 0, 3 ) );
      f_i_i[0]++;

      // this makes ... operations per cycle 
      for( index_t id=0; id<length; id++, g_i++ )
      {
        v.v    = simd::vepsilon;

        // operations (shuf, mov, shuff)
        q = 0;
        {
          shifted_flow.v = _mm_shuffle_ps( *f_i_i[q], *f_i_i[q], _MM_SHUFFLE( 2, 1, 0, 3 ) );
          shifted_flow.v = _mm_move_ss( shifted_flow, prev );
          locOutFlow[q]  = *f_o_i[q]; 
          locInFlow[q]   = shifted_flow;
          prev.v         = _mm_shuffle_ps( *f_i_i[q], simd::zero, _MM_SHUFFLE( 0, 0, 0, 3 ) );
        }

        // this block only attributes values
        for(q = 1; q<d; q++)
//        q=1;        
        {
          locOutFlow[q] = *f_o_i[q];//flow[ length_glob * q + start + id + shift ].v;
          locInFlow [q] = *f_i_i[q]; // flow[ length_glob * q + start - fm1s[q] +  id + shift ].v;
        }

        // this block makes d * 7 ( max, xor, add, and, cmplt, mul, mul) simd operations
        // from which 2 simd flops
        FORR(q, d)
        {
          /* -locInFlow[q] === _mm_xor_ps(locInFlow[q], signmask) */
          dFabs[q].v = _mm_max_ps( _mm_neg_ps(locInFlow[q]), locOutFlow[q] );
          v += _mm_and_ps(_mm_cmplt_ps( simd::zero, dFabs[q] ), simd::one) * dFabs[q] * dFabs[q];
        }

        // this command contains 3 simd operations (min, mul, rsqrt)
        // from which 2 simd flops
        quotient.v = _mm_min_ps( simd::one, *g_i /*locG.v*/ * _mm_rsqrt_ps(v) );

        FORR(q, d)
        {
          dFabs[q] = quotient * dFabs[q];
        }

        // this block makes 
        q = 0;
        {
          if ( locInFlow[q].f[0] < -dFabs[q].f[0]) flow[q][ start - 1 + id + shift ].f[3] = -dFabs[q].f[0];  
          (*f_i_i[q]).f[0] =  ( locInFlow[q].f[1] < -dFabs[q].f[1]) ? -dFabs[q].f[1] : locInFlow[q].f[1];
          (*f_i_i[q]).f[1] =  ( locInFlow[q].f[2] < -dFabs[q].f[2]) ? -dFabs[q].f[2] : locInFlow[q].f[2];
          (*f_i_i[q]).f[2] =  ( locInFlow[q].f[3] < -dFabs[q].f[3]) ? -dFabs[q].f[3] : locInFlow[q].f[3];

          f_i_i[q]++;

          (*f_o_i[q]).v =  _mm_select_gt( locOutFlow[q].v, dFabs[q].v, dFabs[q].v, (*f_o_i[q]).v );
          f_o_i[q]++;
        }

        for (q = 1; q < d; f_i_i[q]++, f_o_i[q]++, q++)
        {
          neg.v = _mm_neg_ps(dFabs[q]);          
          (*f_i_i[q]).v = _mm_select_lt( locInFlow[q].v,         neg,         neg, (*f_i_i[q]).v );
          (*f_o_i[q]).v = _mm_select_gt( locOutFlow[q].v, dFabs[q].v,  dFabs[q].v, (*f_o_i[q]).v );
        } /* for q in 1..d */
      } /* FOR id in length */
    } /* kernel_constrain */



    template <BOOST_PP_ENUM_PARAMS(3, class T)>
    class kernel_pot_t
      : public kernel_update_t
    {
    public:
      typedef T0 pot_t;
      typedef T1 flow_t;
      typedef T2 srcsink_t;
      

    private:
      pot_t     gpu_pot;
      flow_t    gpu_flow;
      srcsink_t srcsink;
      pink::types::vint      fm1s;
      index_t   p_c, f_out, f_in, d, max, simlength;
      float     tau;

    public:
      kernel_pot_t( pot_t gpu_pot, srcsink_t srcsink, flow_t gpu_flow, pink::types::vint fm1s, index_t f_in, index_t d, index_t simlength, index_t max, float tau )
        : gpu_pot(gpu_pot), srcsink(srcsink), gpu_flow(gpu_flow), fm1s(fm1s), f_in(f_in), d(d), simlength(simlength), max(max), tau(tau)
        { }



      void execute( const range_t & range )
        {          
          // the order is important
          index_t w;
          for( w=1; w<d; w++ )
          {
            kernel_pot( gpu_pot, srcsink, gpu_flow, fm1s[w], fm1s[w], f_in, w, tau, range.second - range.first, range.first);
          }
          
          w = 0;          
          {
            kernel_pot_shift( gpu_pot, gpu_flow, fm1s[w], fm1s[w], f_in, w, tau, range.second - range.first, range.first );
          }
        } /* calulculate */

    }; /* kernel_pot_t */



    template <BOOST_PP_ENUM_PARAMS(2, class T)>
    class kernel_pot_shift_t
      : public kernel_update_t
    {
    public:
      typedef T0 pot_t;
      typedef T1 flow_t;

    private:
      pot_t   gpu_pot;
      flow_t  gpu_flow;
      index_t p_c, f_out, f_in, w;
      float   tau;

    public:
      kernel_pot_shift_t( pot_t gpu_pot, flow_t gpu_flow, index_t p_c, index_t f_out, index_t f_in, index_t w, float tau )
        : gpu_pot(gpu_pot), gpu_flow(gpu_flow), p_c(p_c), f_out(f_out), f_in(f_in), w(w), tau(tau)
        { }



      void execute( const range_t & range )
        {
          kernel_pot_shift( gpu_pot, gpu_flow, p_c, f_out, f_in, w, tau, range.second - range.first, range.first );
        } /* calulculate */

    }; /* kernel_pot_shift_t */



    template <BOOST_PP_ENUM_PARAMS(2, class T)>
    class kernel_flow_t
      : public kernel_update_t
    {
    public:
      typedef T0 pot_t;
      typedef T1 flow_t;

    private:
      pot_t   gpu_pot;
      flow_t  gpu_flow;
      index_t f, p, d, w;
      pink::types::vint    fm1s;      
      float   tau;

      
    public:
      kernel_flow_t( pot_t gpu_pot, flow_t gpu_flow, index_t f, index_t p, pink::types::vint fm1s, index_t d, float tau )
        : gpu_pot(gpu_pot), gpu_flow(gpu_flow), f(f), p(p), fm1s(fm1s), d(d), tau(tau)
        { }



      void execute( const range_t & range )
        {
          index_t w = 0;          
          {
            kernel_flow_shift( gpu_pot, gpu_flow, f, p, fm1s[w], w, tau, range.second - range.first, range.first );
          }
          
          for ( w=1; w<d; w++ )
          {
            kernel_flow( gpu_pot, gpu_flow, f, p, fm1s[w], w, tau, range.second - range.first, range.first );
          }
          
        } /* calulculate */

    }; /* kernel_flow_t */



    template <BOOST_PP_ENUM_PARAMS(2, class T)>
    class kernel_constrain_t
      : public kernel_update_t
    {
    public:
      typedef T0 flow_t;
      typedef T1 g_t;

    private:
      flow_t  gpu_flow;
      g_t     gpu_g;
      index_t max, d, length_glob;
      float   tau;
      pink::types::vint    fm1s;

    public:
      kernel_constrain_t( flow_t gpu_flow, g_t gpu_g, index_t max, index_t length_glob, pink::types::vint fm1s, index_t d )
        : gpu_flow(gpu_flow), gpu_g(gpu_g), max(max), length_glob(length_glob), fm1s(fm1s), d(d)
        { }



      void execute( const range_t & range )
        {
          kernel_constrain( gpu_flow, gpu_g, max, length_glob, fm1s, d, range.second - range.first, range.first );
        } /* calulculate */

    }; /* kernel_constrain_t */


    
    template <class image_type>
    class simd_flow_t
    {

    public:

      typedef typename image_type::pixel_type           pixel_type;
      //typedef pink::numa::shared_vector<pixel_type> poly_array_t;
      typedef pink::numa::poly_array<pink::simd::vec>  poly_array_t;      
      //typedef boost::shared_ptr< poly_array_t >         ppoly_array_t;
      typedef std::vector< poly_array_t >               vpoly_array_t;      
      typedef typename boost::shared_ptr<boost::thread> pthread_t;      
      typedef std::pair< pkernel_update_t, index_t >    chunk_t;



    private:
      // the result will be put in this image
      image_type potencial;

      // these distributed arrays contain the fields during the iterations
      /* changed from std::vector*/
      vpoly_array_t flow_glob; // flow is allocated in each direction, so we have as many flows as directions
      poly_array_t  pot_glob;
      poly_array_t  g_glob;
      poly_array_t  srcsink_glob;

      // vec * flow_glob    __attribute__ ((aligned(16))); 
      // vec * pot_glob     __attribute__ ((aligned(16)));
      // vec * g_glob       __attribute__ ((aligned(16)));
      // vec * srcsink_glob __attribute__ ((aligned(16)));
      // vec * src_glob     __attribute__ ((aligned(16)));

      index_t  starttime;

      

    protected:

      index_t d;
      pink::types::vint    dim;
      index_t max;
      float   tau;
      pink::types::vint    fm1s;
      index_t start;
      index_t length;
      pink::types::vint    simdim;      
      pink::types::vint    fm1_vec;      
      bool    verbose;
      index_t simlength; // this holds the length of the vector calculation;
      index_t iteration; // the number of desired iterations
      index_t length_glob;
      index_t number_of_nodes;
      bool    hyper_threading;      
      index_t threads_per_node;      
      index_t number_of_threads;

      /**
         \brief Updates the potencial.

         description

         \param array

         \param iterator fiterator is a function pointer. It can be
         fast_iterator and slow_iterator. Fast iterator simply returns a
         pointer, while slow iterator does bond checking a shift, so it
         also works across the numa nodes.

         \return
      */
      chunk_t update_potencial()
        {
          typedef BOOST_TYPEOF(pot_glob)     pot_t;
          typedef BOOST_TYPEOF(flow_glob)    flow_t;
          typedef BOOST_TYPEOF(srcsink_glob) srcsink_t;

          pkernel_update_t kernel_update (
            new kernel_pot_t<pot_t, flow_t, srcsink_t> (
              pot_glob,
              srcsink_glob,
              flow_glob,
              fm1s,
              0,
              d,
              simlength,
              max,
              tau
              )
            );
          chunk_t chunk( kernel_update, length - max );
          
          return chunk;          
        } /* update_potencial */



      chunk_t update_flow()
        {
          typedef BOOST_TYPEOF(pot_glob) pot_t;
          typedef BOOST_TYPEOF(flow_glob) flow_t;

          pkernel_update_t kernel_update (
            new kernel_flow_t<pot_t, flow_t>(
              pot_glob,
              flow_glob,
              0,//(simlength * w),
              0,
              fm1s,
              d,
              tau
              )
            );

          chunk_t chunk( kernel_update, length );
          
          return chunk;          
        } /* update_flow */



      // functions for calculation
      chunk_t update_constrain() 
        {
          typedef BOOST_TYPEOF(flow_glob) flow_t;
          typedef BOOST_TYPEOF(g_glob)    g_t;

          pkernel_update_t kernel_update (
            new kernel_constrain_t<flow_t, g_t> (
              flow_glob,
              g_glob,
              start,
              simlength,
              fm1s,
              d
              )
            );
          
          chunk_t chunk( kernel_update, length );
          
          return chunk;          
        } /* update_constrain */


      
    public:

      // this variable will hold the elapsed time
      double time;      


      /**
       * The iterator function. Copies everything to local variables, so the compiler can optimize, then manages the iterations, mesures the time generates messages and saves intermediat results.
       * @param D input measure picture
       * @param S image representing source and sink (source = 1; sink = -1)
       * @param g the boundari measure field
       * @param iteration number of all iterations demanded. The number of the iterations can be estimated from the parameters. It needn't to be tested dinamically.
       * @param tau the iterating constant
       * @param uiMovie whether we want a movie or not
       * @return returns the final (hopefully yet convergent image)
       */
      simd_flow_t(
        const char_image & SS,     /* image of the source and of the sink (not the original image) */
        const image_type & gg,     /* Boundaries */
        index_t iteration,         /* number of iterations */
        float   tau = 0.132,	   /* timestep */
        index_t number_of_threads = 0, /* the number of threads to execute if in parallel mode */
        bool    hyper_threading = false,
        bool    verbose = false     /* the size of the packet to process */
        ) :
        tau(          tau),
        d(            gg.get_size().size() ),
        dim(          gg.get_size() ),
        simdim(       gg.get_size() ),
        verbose(      verbose ),
        hyper_threading(hyper_threading),
        iteration(    iteration ),
        length_glob(  gg.get_size().prod()),
        potencial(    gg.get_size() ), // we will put the result in this image
        number_of_threads(number_of_threads)
        {
#         ifdef UJIMAGE_DEBUG
          std::cout << "creating the numaflow object (" << static_cast<void*>(this) << ")" << std::endl;	
#         endif /* UJIMAGE_DEBUG */

          if (( this->number_of_threads == 0 ) or (this->number_of_threads > boost::thread::hardware_concurrency() ))
          {
            this->number_of_threads = boost::thread::hardware_concurrency();            
          }
          
          FOR(q, d)
            simdim[q] = dim[q];          
           
          if (dim[0] % 4 != 0) pink_error("Dimension x (corresponds to coordinate 0) must be divisible by 4");
          simdim[0] = dim[0]/4;
          
          simlength = length_glob/4;          
          
          /// initiating the simd arrays
          g_glob      .reset(simlength);
          srcsink_glob.reset(simlength);
          pot_glob    .reset(simlength);

          flow_glob.resize(d);
          FOR( q, d )
          {
            flow_glob[q].reset(simlength);
          } /* FOR q in d */

          this->number_of_nodes = g_glob.number_of_nodes;
          // queue.resize( number_of_nodes );

          this->threads_per_node = this->number_of_threads / this->number_of_nodes;

          if (verbose)
          {            
            std::cout << "numa-simd edition" << std::endl;
            // creating a local copy of image, srcsink potencial and flows ---------------------------
            std::cout << "dimension   = " << d << "D" << std::endl;
            std::cout << "length_glob = " << this->length_glob << std::endl;
            std::cout << "simlength   = " << this->simlength << std::endl;
            std::cout << "tau         = " << this->tau << std::endl;
            std::cout << "iteration   = " << this->iteration << std::endl;
            std::cout << "threads     = " << this->number_of_threads << " (of " << boost::thread::hardware_concurrency() << ")" <<  std::endl;
            std::cout << "nodes       = " << this->number_of_nodes <<  std::endl;
            std::cout << "threads/node= " << this->threads_per_node <<  std::endl;
          } /* if verbose */
            
          if ( number_of_threads % number_of_nodes != 0 )
          {
            pink_error("The number of threads must be divisible with the number of nodes on the system.");            
          }

          if (verbose)
            std::cout << "initializing the arrays" << std::endl;

          //cleaning the flow
          //#pragma omp parallel for schedule(guided) num_threads(this->number_of_threads)
          FOR( q, d )
          {
            FOR( w, simlength )
            {
              flow_glob[q][w].v = _mm_setzero_ps();
            } /* FOR w in simlength */            
          } /* for d */

          FOR( q, simlength )
          {
            g_glob[q].v       = _mm_setzero_ps();
            pot_glob[q].v     = _mm_setzero_ps();
            srcsink_glob[q].v = _mm_setzero_ps();
          }

          FOR(q, gg.get_size().prod()/4 /* NOT length_glob*/)
          {
            g_glob[q].f[0]   = gg( 4 * q + 0 );
            g_glob[q].f[1]   = gg( 4 * q + 1 );
            g_glob[q].f[2]   = gg( 4 * q + 2 );
            g_glob[q].f[3]   = gg( 4 * q + 3 );            
          }

          if (verbose)
            std::cout << "setting up source" << std::endl;

          FOR(q, SS.get_size().prod() /* NOT length_glob*/)
          {            
            // setting up the eliminator part
            if (SS(q)==1)
            {
              srcsink_glob[ q / 4 ].f[ q % 4 ] = 1.;
            }
            else if (SS(q)==255)
            {
              srcsink_glob[ q / 4 ].f[ q % 4 ] = -1.;
            } 
          }

          fm1_vec.resize(d);
          fm1s   .resize(d);

          max = 0;

          FOR( w, d )
          {
            fm1_vec.reset();
            fm1_vec[w] = 1; //we are calculating the distance the opposite direction but it should be the same
            fm1s[w]    = simdim.position(fm1_vec);
            max        = fm1s[w] > max ? fm1s[w] : max;
          }

          this->start  = max;          
          this->length = simlength - 2 * max;
        } /* constructor simd_flow_t */



      virtual image_type iterate()
        {
          FOR(e, iteration)
          {
            update_potencial(); // here I calculate my part of the potencial
            update_flow();
            update_constrain();
          } /* FOR e in iteration */
          
          pkernel_update_t kernel_update;      
          index_t size;

          double starttime = pink::benchmark::now();          
          if (verbose)
            std::cout << "starting the iteration" << std::endl;

          std::pair<pkernel_update_t, index_t> pot  = update_potencial();
          std::pair<pkernel_update_t, index_t> flow = update_flow();
          std::pair<pkernel_update_t, index_t> cons = update_constrain();
                    
          scheduler_t<> scheduler(
            number_of_threads,
            iteration,
            pot.first,
            flow.first,
            cons.first,
            range_t( 0, pot.second  ),
            range_t( 0, flow.second ),
            range_t( 0, cons.second ),
            hyper_threading
            );

          scheduler.schedule_work();
          
          double endtime = pink::benchmark::now();
          this->time = static_cast<double>(endtime - starttime);
          
          if (verbose)
            std::cout << "total time of iteration: " << this->time << "s" << std::endl;

          // copy the calculated potencial to the 'potencial' image          
          FOR(q, simlength)
          {
            potencial( 4 * q + 0 ) = pot_glob[q].f[0];
            potencial( 4 * q + 1 ) = pot_glob[q].f[1];
            potencial( 4 * q + 2 ) = pot_glob[q].f[2];
            potencial( 4 * q + 3 ) = pot_glob[q].f[3];            
          } /* for q */

          return potencial;
        } /* virtual start*/

      

      virtual ~simd_flow_t()
        {
#         ifdef UJIMAGE_DEBUG
          std::cout << "destroying the clflow object (" << static_cast<void*>(this) << ")" << std::endl;
#         endif /* UJIMAGE_DEBUG */
        }
      
    }; /* class simd_flow_t */



    template <class image_type>
    boost::python::object
    simdflow(
      char_image SS,                    /* image of the source and of the sink (not the original image) */
      image_type gg,                    /* Boundaries */
      index_t    iteration,             /* number of iterations */
      float      glob_tau = 0.132,      /* timestep */
      index_t    number_of_threads = 0, /* the number of threads to execute if in parallel mode */
      bool       hyper_threading = false,
      bool       verbose = false,        /* the resolution of the iteration */
      bool       debug = false           /* returns the time of the iteration */
      )
    {
      simd_flow_t<image_type> obj(
        align_size(SS, 4, -1),
        align_size(gg, 4,  0.),
        iteration,
        glob_tau,
        number_of_threads,
        hyper_threading,
        verbose
        );
      
      image_type rimage = obj.iterate();

      if (debug)
      {        
        boost::python::tuple result = boost::python::make_tuple(rimage, obj.time);         
        return result;
      }
      else /* NOT debug */
      {
        boost::python::object result(rimage);
        return result;        
      } /* NOT debug */
      
    } /* clflow */

  } /* namespace numa */
} /* end namespace pink */


// cleaning up after us
#undef REPORT_INTERVAL

// // obsolete, replaced by enum
// #undef __ETAP_POT
// #undef __ETAP_FLOW
// #undef __ETAP_CONSTR

// // obsolete, loaded as a parameter
// #undef PACKET_SIZE

# endif /* (defined PINK_HAVE_NUMA) && (defined PINK_HAVE_SIMD) */

#endif /* UIFLOW_SIMD_HPP_*/
/* LuM end of file */
