/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// memory speed test, parallel memory speed test
// computation power test, simd computational power test
// and parallel computational power test

#ifndef UI_SYSTEST_HPP___
# define UI_SYSTEST_HPP___

# define NUMA_VERSION1_COMPATIBILITY // blade's don't have the new numa api

# ifdef PINK_HAVE_NUMA
#   include <numa.h>
#   include "ui_flow_ghosts.hpp"
# endif /* PINK_HAVE_NUMA */
# include <iostream>
# include <boost/thread.hpp>

//#include "pink.h"
# include "ui_simd.hpp"
# include "ui_polythread.hpp"

namespace pink
{
  namespace benchmark
  {

    const index_t repeat = 100;


    /**
       \brief Returns the system wall time with microsecond resolution.
       
       description This function checks the system time with
       microsecond resolution. It is used to measure the duration of a
       functions. The difference between two calls of this functions
       is the elapsed time in seconds with microsecond resolution (+-
       0.000001s).

    \return The number of seconds (with microsecond resolution) since
    1970-01-01.
    */    
    double now();

    

    /**
       \brief Memory speed test function, non numa version.
       
       description 
    */    
    template <BOOST_PP_ENUM_PARAMS(6, class T)>
    void thread( T1 size, T2 from, T3 to, T4 barrier_start, T5 barrier_end )
    {
      typedef T0 value_type;

      // // restricting myself to this node
      // // for numa 1.0
      // nodemask_t mask;
      // nodemask_zero(&mask);
      // nodemask_set(&mask,/* node*/ 0);
      // numa_bind(&mask);

      
      srand(now());
      // filling up the memory with pseudo-random numbers
      FOR( q, size )
      {
//        _DEBUG(q);        
        from[q] = rand();
      }

      barrier_end->wait(); // signalling, that I am ready to copy
      barrier_start->wait(); // waiting for the permission to start the test
      FOR( q, repeat )
      {
        std::copy( &(from[0]), &(from[size]), &(to[0])   );
        std::copy( &(to  [0]), &(to  [size]), &(from[0]) );
      } /* for q in repeat */
      barrier_end->wait(); // signalling the end of the test
      return;      
    } /* thread */



    /**
    \brief Parallel optimal memory bandwidth measurement.
    description 

    \param mem_size The size of the test array (in MiB)
    \param nbt The number of threads
    \return The memory bandwidth (in MiBps)
    */    
    template <class T0>
    double memspeed( index_t mem_size, index_t nbt )
    {
      typedef T0 value_type;
      if ((nbt<1) || /*or*/ (nbt> boost::thread::hardware_concurrency()))
      {
        nbt = boost::thread::hardware_concurrency();        
      } /* adjust the number of threads if necessary */
          
      std::vector< boost::shared_ptr<boost::thread> > threads(nbt);

      // we want to test the same cumulated memory size independently
      // of the number of threads
      index_t csize = mem_size * 1024 * 1024 / sizeof(value_type); // cumulated size
      index_t tsize = csize / nbt; // the allocation size per thread
      boost::scoped_array<value_type> from( new value_type[ nbt * tsize ] );
      boost::scoped_array<value_type> to  ( new value_type[ nbt * tsize ] );      
      
      typedef boost::shared_ptr<boost::barrier> pbarrier_t;
      pbarrier_t barrier_start( new boost::barrier(nbt + 1) );
      pbarrier_t barrier_end(   new boost::barrier(nbt + 1) );
       
      FOR( q, nbt )
      {
        threads[q].reset(
          new boost::thread(
            thread<value_type, index_t, value_type*, value_type*, pbarrier_t, pbarrier_t>,
            tsize, (&from[ q * tsize ]), (&to[ q * tsize ]), barrier_start, barrier_end
            )
          );
      } /* for q in nbt */

      barrier_end->wait();
      double start = now();
      barrier_start->wait();
      barrier_end->wait();
      double end = now();

      double numa_speed = static_cast<double>( repeat * 2 * nbt * tsize * sizeof(value_type)) / (1024. * 1024.) / static_cast<double>(end - start);
      
      FOR( q, nbt )
      {
        threads[q]->join();
      }
      
      return numa_speed;      
    } /* memspeed() */

    
#   ifdef PINK_HAVE_NUMA

    namespace numa {
      
      /**
         \brief Memory speed test function, non numa version.
       
         description 
      */    
      template <BOOST_PP_ENUM_PARAMS(5, class T)>
      void thread( T1 node, T2 size, T3 barrier_start, T4 barrier_end )
      {
        typedef T0 value_type;

        // restricting myself to this node
        // for numa 1.0
        nodemask_t mask;
        nodemask_zero(&mask);
        nodemask_set(&mask, node);
        numa_bind(&mask);

        srand(now());

        //pink::numa::array<value_type> from (node, size * sizeof(value_type) );
        //pink::numa::array<value_type> to   (node, size * sizeof(value_type) );

        // for debugging
        std::vector<value_type> from(size);
        std::vector<value_type> to  (size);
        
        
        // filling up the memory with pseudo-random numbers
        FOR( q, size )
        {
          from[q] = rand();
        }

        barrier_end->wait(); // signalling, that I am ready to copy
        barrier_start->wait(); // waiting for the permission to start the test
        FOR( q, repeat )
        {
          std::copy( &(from[0]), &(from[size]), &(to[0])   );
          std::copy( &(to  [0]), &(to  [size]), &(from[0]) );
        } /* for q in repeat */
        barrier_end->wait(); // signalling the end of the test
        return;      
      } /* thread */



      /**
         \brief Parallel optimal memory bandwidth measurement.
         description 

         \param mem_size The size of the test array (in MiB)
         \param nbt The number of threads
         \return The memory bandwidth (in MiBps)
      */    
      template <class T0>
      double memspeed( index_t mem_size, index_t tpn, index_t node_restrict = -1 )
      {
        typedef T0 value_type;        
        index_t number_of_nodes  = numa_max_node() + 1;
        
        if ((tpn<1) ||/*or*/ (tpn > (boost::thread::hardware_concurrency() / number_of_nodes )))
        {
          tpn = boost::thread::hardware_concurrency() / number_of_nodes;        
        } /* adjust the number of threads if necessary */

        index_t nbt = tpn * number_of_nodes;        
        std::vector< boost::shared_ptr<boost::thread> > threads(nbt);

        // we want to test the same cumulated memory size independently
        // of the number of threads
        index_t csize = mem_size * 1024 * 1024 / sizeof(value_type); // cumulated size
        index_t tsize = csize / nbt; // the allocation size per thread

        typedef boost::shared_ptr<boost::barrier> pbarrier_t;
        pbarrier_t barrier_start( new boost::barrier(nbt + 1) );
        pbarrier_t barrier_end(   new boost::barrier(nbt + 1) );

        if (node_restrict==-1)
        {
          FOR( node, number_of_nodes )
            FOR( q, tpn )
          {
            threads[ tpn * node + q ].reset(
              new boost::thread(
                thread<value_type, index_t, index_t, pbarrier_t, pbarrier_t>,
                node, tsize, barrier_start, barrier_end
                )
              );
          } /* for q in tpn, number_of_nodes */
        }
        else /* NOT node_restrict == -1; we want to run the simulation on a specific node */
        {
          FOR(q, nbt)
          {
            threads[q].reset(
              new boost::thread(
                thread<value_type, index_t, index_t, pbarrier_t, pbarrier_t>,
                node_restrict, tsize, barrier_start, barrier_end
                )
              );
          } /* for q in nbt */
        } /* NOT node_restrict == -1; we want to run the simulation on a specific node */
        
        barrier_end->wait();
        double start = now();
        barrier_start->wait();
        barrier_end->wait();
        double end = now();
        
        double numa_speed = 0;
        
        if (fabs(end - start) > 0.0000001)
        {          
          numa_speed = static_cast<double>( repeat * 2 * nbt * tsize * sizeof(value_type)) / (1024. * 1024.) / static_cast<double>(end - start);
        }
      
        FOR( q, nbt )
        {
          threads[q]->join();
        }
      
        return numa_speed;      
      } /* memspeed() */


      template<class T0, class T1, class T2>
      void synchro( T0 iterations, T1 barrier, T2 core )
      {
        // restricting myself to a particular core and nothing else
        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET( core, &cpu_set );
        sched_setaffinity( gettid(), sizeof(cpu_set), &cpu_set );

        for ( index_t q = 0; q < iterations; q++ )
        {
          barrier->wait();        
        }      
      } /* synchro */
    

    
      template <class T0, class T1>
      double barispeed( T0 number_of_threads, T1 iterations )
      {
        double start = now();

        std::vector< boost::shared_ptr<boost::thread> > threads(number_of_threads);
        typedef boost::shared_ptr<boost::barrier> pbarrier_t;

        pbarrier_t barrier(new boost::barrier(number_of_threads));      
            
        FOR(q, number_of_threads )
        {
          threads[q].reset(new boost::thread(synchro<T1, pbarrier_t, index_t>, iterations, barrier, q));
        }

        FOR(q, number_of_threads )
        {
          threads[q]->join();
        }

        double end = now();
      
        return end - start;      
      } /* barispeed */

      
  } /* namespace numa */

# endif /* PINK_HAVE_NUMA */


    template<class T0, class T1>
    void synchro( T0 iterations, T1 barrier)
    {
      for ( index_t q = 0; q < iterations; q++ )
      {
        barrier->wait();        
      }      
    } /* synchro */
    

    
    template <class T0, class T1>
    double barispeed( T0 number_of_threads, T1 iterations )
    {
      double start = now();

      std::vector< boost::shared_ptr<boost::thread> > threads(number_of_threads);
      typedef boost::shared_ptr<boost::barrier> pbarrier_t;

      pbarrier_t barrier(new boost::barrier(number_of_threads));      
            
      FOR(q, number_of_threads )
      {
        threads[q].reset(new boost::thread(synchro<T1, pbarrier_t>, iterations, barrier));
      }

      FOR(q, number_of_threads )
      {
        threads[q]->join();
      }

      double end = now();
      
      return end - start;      
    } /* barispeed */
    
    
  } /* namespace benchmark */
} /* namespace pink */



#endif /* UI_SYSTEST_HPP___ */

// LuM end of file
