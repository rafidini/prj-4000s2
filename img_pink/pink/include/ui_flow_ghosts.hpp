/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// -O3 -DNDEBUG -ipo -no-prec-div -fp-model fast=2 -msse4.2

#ifndef UI_FLOW_GHOSTS__HPP__
#define UI_FLOW_GHOSTS__HPP__


//!! note this is a hack. Pink's using depth as a macro for accessing
//!! the z size of the image
#define NUMA_VERSION1_COMPATIBILITY // blade's don't have the new numa api
#ifdef PINK_HAVE_NUMA
# include <set>
# include <numa.h>
# include <sched.h>
# include <sys/types.h>
# include <linux/unistd.h>
# include <boost/thread.hpp>
# include <boost/smart_ptr.hpp>

# include "ui_pink_types.hpp"
# include "ui_polythread.hpp"

// system call for the thread binding
inline
long int
gettid()
{
  return static_cast<long int>(syscall(__NR_gettid));  
} /* gettid */

//_syscall0(pid_t,gettid)
//pid_t gettid(void);


namespace pink {
  namespace numa {

    
    
    typedef boost::mutex mutex_t;
    typedef std::pair<index_t, index_t> range_t;
    typedef boost::shared_ptr<mutex_t> pmutex_t;    
    typedef boost::lock_guard<mutex_t> scoped_lock;

    class kernel_update_t
    {
    public:

      kernel_update_t() { }

      virtual ~kernel_update_t() { }

      virtual void execute( const range_t& ) = 0;
      
    }; /* kernel_update_t */
    typedef boost::shared_ptr<kernel_update_t> pkernel_update_t;    
    // typedef kernel_update_t* pkernel_update_t;


    
    template < class T0 = index_t >
    struct thread_private_t
    {
      T0      tag;      
      index_t node;
      index_t core;
      index_t part; // the assigned partitioner      
      range_t pot_range, flow_range, cons_range;
      

      
      template <BOOST_PP_ENUM_PARAMS(6, class MT)>
      thread_private_t(
        MT0 core, MT1 node, MT2 part, 
        MT3 pot, MT4 flow, MT5 cons
        )
        : node(node), core(core), part(part),
          pot_range(pot), flow_range(flow), cons_range(cons) { }
      
    }; /* struct thread_private_t */
    
    

    template < class T0 = index_t >
    struct thread_common_t
    {
      typedef boost::barrier barrier_t;
      
      T0 tag;
      pkernel_update_t pot, flow, cons;
      barrier_t sync, work_end, work_begin;
      index_t number_of_threads, iteration;

      

      template <BOOST_PP_ENUM_PARAMS(5, class MT)>
      thread_common_t(
        MT0 number_of_threads, MT1 iteration,
        MT2 pot, MT3 flow, MT4 cons
        )
        : // note that the + 1 is for the scheduler
          work_begin( number_of_threads + 1 ), work_end( number_of_threads + 1 ),
          sync(number_of_threads), number_of_threads(number_of_threads),
          pot(pot), flow(flow), cons(cons), iteration(iteration)
        { }

    }; /* struct thread_common */

    
    
    template <BOOST_PP_ENUM_PARAMS(2, class T)>
    void laborer( T0 thread_common, T1 thread_private )
    {
      index_t core  = thread_private->core;
      index_t node  = thread_private->node;
      index_t part  = thread_private->part;      

      // restricting all memory operations and affinity to the
      // selected numa node of this thread for numa 1.0
      nodemask_t mask;
      nodemask_zero(&mask);
      nodemask_set(&mask, node);
      numa_bind(&mask);
            
      // restricting myself to a particular core and nothing else
      cpu_set_t cpu_set;
      CPU_ZERO(&cpu_set);
      CPU_SET( core, &cpu_set );
      sched_setaffinity( gettid(), sizeof(cpu_set), &cpu_set );

      // eager to work
      thread_common->work_begin.wait(); // wait for the scheduler      
      FOR( q, thread_common->iteration )
      {       
        thread_common->pot->execute( thread_private->pot_range );
        thread_common->sync.wait(); 

        thread_common->flow->execute( thread_private->flow_range );
        thread_common->sync.wait();        

        thread_common->cons->execute( thread_private->cons_range );
        thread_common->sync.wait();        
        
        //thread_common->kernel->execute( thread_private->partitioner->size );        
        // // in the first part of the computation I extinguish my calculcations (pop)
        // while ( not thread_private->partitioner->empty() )
        // {
        //   // fetch a part for myself
        //   range = thread_private->partitioner->pop();

        //   // advertise the rest
        //   advertise = range_t( part, thread_private->partitioner->occupancy() ); 
        //   thread_common->queues[node].push(advertise); // advertise to the node
        //   thread_common->grand_queue.push(advertise); // advertize to all system
          
        //   // calculate
        //   thread_common->kernel->execute(range);

        //   // revoke advertising
        //   thread_common->queues[node].try_erase(advertise);
        //   thread_common->grand_queue.try_erase(advertise);
        // } /* while I have work to do on my own */
        
        // // now I look around if there are some threads still working on this node (steal)
        // while ( not thread_common->queues[node].empty() )
        // {
        //   advertise = thread_common->queues[node].pop();
        //   if ( advertise.second >= 0 )
        //   {
        //     range = thread_common->partitioners[advertise.first]->steal();
        //     thread_common->kernel->execute(range);
        //   } /* advertise.second >= 0 */
          
        // } /* while there are tasks left on the node */

        // // now I look around if there are some threads still working on this node (steal)
        // while ( not thread_common->grand_queue.empty() )
        // {
        //   advertise = thread_common->grand_queue.pop();
        //   if ( advertise.second >= 0 )
        //   {
        //     range = thread_common->partitioners[advertise.first]->steal();
        //     thread_common->kernel->execute(range);
        //   } /* advertise.second >= 0 */
          
        // } /* while there are tasks left on the node */


        // thread_common->work_end.wait();
        //thread_common->work_begin.wait();
      } /* while not thread_common->quit */

      thread_common->work_end.wait(); // wait for the other laborers to finish
      return;      
    } /* void laborer */
        


    template < class T0 = index_t >
    class scheduler_t
    {
    public:
      T0 tag;
      
      typedef boost::shared_ptr<boost::thread>  pthread_t;
      typedef std::vector<pthread_t>            threads_t;
      typedef boost::shared_ptr< thread_common_t<> >  pthread_common_t;
      typedef boost::shared_ptr< thread_private_t<> > pthread_private_t;      


      
    private:
      
      threads_t threads;
      index_t number_of_cores;      
      index_t number_of_nodes;
      index_t threads_per_node;
      index_t number_of_threads;            
      pthread_common_t thread_common;


      
    public:

      template <BOOST_PP_ENUM_PARAMS(9, class MT)>
      scheduler_t( MT0 number_of_threads, MT1 iteration, 
                   MT2 pot, MT3 flow, MT4 cons,
                   MT5 pot_range, MT6 flow_range, MT7 cons_range,
                   MT8 hyper_threading = false
        )
        : number_of_threads(number_of_threads)
        {
          // Getting the numa characteristics from the system
          if (numa_available() < 0)
          {
            pink_error("This scheduler wants to be conscious, but your system does not support the NUMA API");
          } /* if numa_available */
          
          number_of_nodes  = numa_max_node() + 1;
          number_of_cores  = boost::thread::hardware_concurrency();
          threads_per_node = number_of_threads / number_of_nodes;
          thread_common.reset(
            new thread_common_t<>(
              number_of_threads, iteration, pot, flow, cons ) );
          
          // creating the worker threads, the threads will be bound to
          // the core for the cache scheduling
          threads.resize(number_of_cores);

          // creating ranges:
          numa::distributor_t dist_pot ( pot_range.second  - pot_range.first,  number_of_threads );
          numa::distributor_t dist_flow( flow_range.second - flow_range.first, number_of_threads );
          numa::distributor_t dist_cons( cons_range.second - cons_range.first, number_of_threads );
          
          FOR( node, number_of_nodes )
          {
            FOR( q, threads_per_node )
            {
              index_t part = node * threads_per_node + q;
              index_t core;
              if (hyper_threading)
              {
                core = node * (number_of_cores / number_of_nodes) + 2 * q; /* 2* for hyper-threading */
              }
              else /* NOT hyper_threading */
              {
                core = node * (number_of_cores / number_of_nodes) + /*2 **/ q; /* 2* for hyper-threading */
              } /* NOT hyper_threading */              
              
              threads[ node * threads_per_node + q ].reset(
                new boost::thread(
                  laborer<pthread_common_t, pthread_private_t>,
                  thread_common,
                  pthread_private_t(
                    new thread_private_t<>(
                      /* core */ core,
                      /* node */ node,
                      /* part */ part,
                      range_t( pot_range.first  + dist_pot.low(part),  pot_range.first  + dist_pot.high(part)  ),
                      range_t( flow_range.first + dist_flow.low(part), flow_range.first + dist_flow.high(part) ),
                      range_t( cons_range.first + dist_cons.low(part), cons_range.first + dist_cons.high(part) )                                              
                      ))));
            } /* FOR q in threads_per_node */
          } /* FOR node in number_of_nodes */
          
          return;          
        } /* scheduler_t */



      ~ scheduler_t()
        {
          FOR(q, number_of_threads)
          {
            threads[q]->join();            
          } /* FOR q in number_of_cores */          
        } /* ~scheduler_t */
      


      void schedule_work( )
        {
          // thread_common->kernel = kernel;
          // // distributing the range equally among the laborers
          
          // numa::distributor_t distributor( range.second - range.first, number_of_threads );
          // FOR(q, number_of_threads)
          // {
          //   thread_common->partitioners[q]->reset( range_t( range.first + distributor.low(q), range.first + distributor.high(q)));
          // } /* for q in number_of_threads */
          
          // // launch the threads
          thread_common->work_begin.wait();
          
          // wait for the threads to finish the calculation
          thread_common->work_end.wait();
          return;          
        } /* schedule_work */
      
    }; /* class scheduler_t */
    
      
    
    
  } /* end namespace numa */
} /* end namespace pink */

# endif /* PINK_HAVE_NUMA */

#endif /* UI_FLOW_GHOSTS__HPP__ */
/* LuM end of file */
