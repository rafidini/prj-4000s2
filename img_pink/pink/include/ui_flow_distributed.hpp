/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UIFLOW_DISTRIBUTED_HPP_
# define UIFLOW_DISTRIBUTED_HPP_

# define BOOST_DISABLE_ASSERTS

# define NUMA_VERSION1_COMPATIBILITY // blade's don't have the new numa api
# ifdef PINK_HAVE_NUMA
#   include <numa.h>
#   include <vector>
#   include <utility>
#   include <sched.h>
#   include <boost/mpl/at.hpp>
#   include <boost/thread.hpp>
#   include <boost/typeof/typeof.hpp>
#   include <boost/function_types/result_type.hpp>
#   include <boost/function_types/parameter_types.hpp>

//#include "uiFlow.hpp"
#   include "uiFrame.hpp"
#   include "pyujimage.hpp"
#   include "ui_compactor.hpp"
#   include "ui_polythread.hpp"



namespace pink {
  
  namespace numa {

#   define REPORT_INTERVAL 10

    template <class T0 = index_t> // for not initializing if not explicitly needed
    struct semaphores_t
    {
      typedef boost::barrier barrier_t;
      
      T0        tag;
      barrier_t flow;
      barrier_t potencial;
      barrier_t constrain;

      template <class MT0>
      semaphores_t( MT0 number_of_threads )
        : flow(number_of_threads), potencial(number_of_threads),
          constrain(number_of_threads)
        { }
      
    }; /* struct semaphores_t */

    

    template <class T>
    inline T isqr(const T & x)
    {
      return x * x;
    }


    template <class T0>
    inline
    T0 put_in(T0 & t0)
    {
      if (t0>1) return 1;
      return (t0<0) ? 0 : t0;      
    }
    
    
    
    template <class image_type>
    class distributed_flow;    
    
    // this is the thread function, which will be used for iteration
    template <class image_type>
    void maxflow_iterator(
      pink::numa::distributed_flow<image_type> & obj,
      index_t ID,
      index_t node_of_this_thread
      );    

    
    
    template <class image_type>
    class distributed_flow 
    {
      
    private:

      typedef typename image_type::pixel_type    pixel_type;
      typedef poly_array<pixel_type>             poly_array_t;      
      typedef boost::shared_mutex                shared_mutex_t;
      typedef boost::shared_ptr<semaphores_t<> > psemaphores_t;
      typedef boost::shared_ptr<shared_mutex_t>  pshared_mutex_t;
      
      

      friend void maxflow_iterator <image_type> (
        pink::numa::distributed_flow<image_type> & obj,
        index_t ID,
        index_t node_of_this_thread
        );
      

      
      // the result will be put in this image
      image_type gg;
      char_image src_sink;
      image_type potencial;      

      // these distributed arrays contain the fields during the iterations
      /* changed from std::vector*/ std::vector<poly_array_t> flow_glob; // flow is allocated in each direction, so we have as many flows as directions
      poly_array_t  g_glob;
      poly_array_t  pot_glob;

      index_t starttime;
      index_t number_of_threads;
      index_t number_of_nodes;      
      index_t resolution;
      
      bool    flow_calculated;  // this variable hold if start function has already been called, 
      // if it hasn't than pot_glob and flow_flob are not yet allocated
   
      // functions for threading    
      index_t   packet_size; // this variable holds the number of consecutive
      // dibbles to be processed by the same thread. If
      // packet_size==n then each thread processes n
      // dibbles for on one charge. n dibbles is called
      // a packet.

      // this structure holds the list of the dibbles' purity. If the
      // value of the vector is a nonnegative value between 0 and the
      // number_of_threads-1, then the given dibble is pure to that
      // node. If the value is negative then the dibble is overlapping
      // in at least one calculation. If the value is positive and
      // greater then the number of threads, then the particular
      // dibble has not yet been considered.
      pink::types::vint          pure_pot;
      /* changed from std::vector*/ std::vector<pink::types::vint> pure_flow;
      pink::types::vint          pure_cons;
      psemaphores_t semaphores;

      // this structure holds the compactified pure dibbles as well as
      // the collected tainted dibbles for the synchronizer
      types::vector2D<types::dibble_t> compact_srcsink;      
      types::vector2D<types::dibble_t> compact_pot;
      types::vector3D<types::dibble_t> compact_flow;      
      types::vector2D<types::dibble_t> compact_cons;
      types::vector3D<types::dibble_t> resolut_pot;
      types::vector4D<types::dibble_t> resolut_flow;
      types::vector3D<types::dibble_t> resolut_cons;
      types::vector2D<types::dibble_t> synchro_src;
      types::vector2D<types::dibble_t> synchro_sink;
      types::vector2D<types::dibble_t> synchro_pot;
      types::vector3D<types::dibble_t> synchro_flow;      
      types::vector2D<types::dibble_t> synchro_cons;


      
    protected:

      index_t     d;
      pink::types::vint        dim;
      float       tau;
      // the number of desired iterations
      bool        verbose;
      
      index_t     iteration;
      index_t     length_glob;
      /* changed from std::vector*/ std::vector<types::dibble_t> dib_constrain;
      /* changed from std::vector*/ std::vector<types::dibble_t> dib_potencial;

      // this way when the vector destroys the array it will
      // destroy all the elements as well
      types::vector2D<types::dibble_t> dib_flow; 
      
      // functions for calculation
      template <class array_t, class fiterator>
      void update_constrain( const array_t & array, fiterator iterator )        
      //void update_constrain(index_t startDibble, index_t endDibble)
        {
          // getting iterator's first parameter's type
          typedef typename boost::mpl::at_c<
          boost::function_types::parameter_types<fiterator>,0 >::type riterator_t; // this type still contains the reference (&)
          // removing the reference from the type
          typedef typename boost::remove_reference<riterator_t>::type iterator_t;
          
          //     //local copies
          pixel_type dFabs[d];
          iterator_t locInFlow[d];
          iterator_t locOutFlow[d];
          iterator_t locG;
          pixel_type v;
	
          //locals
          pink::types::vint fm1s(d), fm1_vec(d);
          index_t start, end, length;
	
          // calculating differences between the flow and the point
          FOR( w, d )
          {
            fm1_vec.reset();
            fm1_vec[w]=1;//we are calculating the distance the opposite direction but it should be the same
            fm1s[w]=dim.position(fm1_vec);
          }

          FOR(e, array.size())
          {	
            // //for (/*int*/ e=0; e<=dib_constrain->length-1; e++){
            // for (index_t e=start_dibble; e < end_dibble; e++)
            // {
            start  = array[e].first;
            end    = array[e].second;
            length = end - start;
              
            iterator(locG, g_glob, start);
            FOR(w,d)
            {
              iterator(locOutFlow[w], flow_glob[w], start );
              iterator(locInFlow[w],  flow_glob[w], start - fm1s[w] );
            }
            FOR(r, length)
            {
              v=0.;
              FOR(w, d)
              {
                // dFabs=max(-Fin, 0, Fout)
                dFabs[w] = -(*locInFlow[w]) > (*locOutFlow[w]) ? -(*locInFlow[w]) : (*locOutFlow[w]);
                if (0. > dFabs[w]) dFabs[w] = 0.;
                // v = sum(dFabs^2)
                //v += static_cast<pixel_type>(dFabs[w] * dFabs[w]);
                v += isqr(dFabs[w]);                  
              }

              FOR(w, d)
              {
                // multiplying
                if ( isqr(*locG) < v ) dFabs[w] *= (*locG) / static_cast<pixel_type>(sqrt(v));
                // testing the final condition
                if ( (*locInFlow[w])  < -dFabs[w]) (*locInFlow[w])  = -dFabs[w];
                if ( (*locOutFlow[w]) > dFabs[w] ) (*locOutFlow[w]) = dFabs[w];
              }
              // stepping;
              locG++;

              FOR(w, d)
              {
                locInFlow[w]++;
                locOutFlow[w]++;
              } /* FOR w, d */
                

            } /* for r in length */
          } /* FOR e in array.size() */
          
        } /* update_constrain */



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
      template<class array_t, class fiterator>
      void update_potencial(const array_t & array, fiterator iterator )
      //void upDatePotencial(index_t startDibble, index_t endDibble)      
        {
          //typedef typename boost::function_types::result_type<fiterator>::type iterator_t;
          // getting iterator's first parameter's type
          typedef typename boost::mpl::at_c<
          boost::function_types::parameter_types<fiterator>,0 >::type riterator_t; // this type still contains the reference (&)
          // removing the reference from the type
          typedef typename boost::remove_reference<riterator_t>::type iterator_t;

          iterator_t p_c, f_out, f_in;          
          index_t fm1 /*fm1_vec[d]*/, start, end;            
          pink::types::vint fm1_vec(d);
            
          FOR(w, d)
          {
            fm1_vec.reset();
            fm1_vec[w]=1; //we are calculating the distance the opposite direction but it should be the same
            fm1=dim.position(fm1_vec);
            
            FOR( e, array.size() )
            {
              //std::cout << " q= " << q << " w= " << w << " e= " << e
              //<< " dib_potencial.size()= " << dib_potencial.size() << std::endl;
                
              start = array[e].first;
              end   = array[e].second;
                
              iterator(p_c,   pot_glob,     start);
              iterator(f_out, flow_glob[w], start);
              iterator(f_in,  flow_glob[w], start-fm1);

              //the hyper-super ultra fast loop
              for ( index_t r=start; r<end; r++, p_c++, f_out++, f_in++ )
              {
                *p_c -= tau * ( *f_out - *f_in );
              }
            } /* for e in array_size */
          } /* FORR d */

               
        } /* update_potencial */


      
      template <class array_t, class fiterator>
      void update_flow(const array_t & array, fiterator iterator, int w /*direction*/)
      //void update_flow(index_t startDibble, index_t endDibble, int w /*direction*/)
        {
          //typedef typename boost::function_types::result_type<fiterator>::type iterator_t;
          // getting iterator's first parameter's type
          typedef typename boost::mpl::at_c<
          boost::function_types::parameter_types<fiterator>,0 >::type riterator_t; // this type still contains the reference (&)
          // removing the reference from the type
          typedef typename boost::remove_reference<riterator_t>::type iterator_t;

          iterator_t p, pp1, f;
            
          index_t start, end, pp1_pos;
          pink::types::vint pp1_vec(d);

          pp1_vec.reset();
          pp1_vec[w]=1;
          pp1_pos=dim.position(pp1_vec);

          FOR(q, array.size())
          {
            start  = array[q].first;            
            end    = array[q].second;

            iterator(f,   flow_glob[w], start );
            iterator(p,   pot_glob,     start );
            iterator(pp1, pot_glob,     start + pp1_pos );
              
            for (index_t r=start; r<end; r++, f++, p++, pp1++)
            {
              *f -= tau*( *pp1 - *p );
              //flow_glob[w][r] -= tau*(pot_glob[pp1_pos+r]-pot_glob[r]);                  
            }
          } /* FOR q in array.size */
      
        } /* update_flow */

      

      template <class array_t>
      void update_srcsink( const array_t & array, pixel_type val )
        {
          FOR(q, array.size())
          {            
            for( index_t e = array[q].first; e < array[q].second; e++ )
            {
              pot_glob[e]=val;              
            } /* FOR e in first..second */                        
          } /* FOR q in array.size() */
          
        } /* update_src */
      

      
      int create_dibbles()        
        {
          // it is demanded that all the fragments on the border of the image are sink
          // for every dibble, the higher end is NOT included

// following we will parallelize the creation of the dibbles
// as there are three parts we only create 3 threads
          //#pragma omp parallel num_threads(3) 
          { 

            //#pragma omp sections
            {

              //  -------- potencial -------
              // all the fragments different from source and sink are to be exclusively among the dibbles
              //#pragma omp section
              {
                bool started = false;
                index_t start=0, end=0;
                index_t currlength=0;
                boost::shared_array<unsigned char> curr;
                curr = src_sink.get_pixels();
                FOR(q, length_glob) {
                  if (started) {
                    currlength++;
                    if (curr[q]!=0) {
                      end=q;
                      dib_potencial.push_back(types::dibble_t(start, end));
                      ///!!! std::cout << "dib_potencial->addElement(" << start << ", " << end << ");" << std::endl;  //////
                      start=0;
                      end=0;
                      started=false;
                    } else /* NOT (curr[q]!=0) */
                      if (currlength>=MaxDibble) {
                        end=q+1;
                        dib_potencial.push_back(types::dibble_t(start, end));
                        ///!!! std::cout << "maxdibble dib_potencial->addElement(" << start << ", " << end << ");" << std::endl;  //////
                        start=0;
                        end=0;
                        started=false;
                      } /* (currlength>=MaxDibble) */
                  } 
                  else  /* NOT (started) */
                  {
                    if (curr[q]==0){
                      started=true;
                      start=q;
                      currlength=1;
                    } /* (curr[q]==0) */
                  } /* NOT (started) */
                } /* FOR(q, length_glob) */
              } /* end pragma omp section */
  
  
              //  -------- flow ------------
              // all the arrows with at least one end not in src or sink are to be exclusively among the dibbles
              //#pragma omp section
              {
                bool started=false;
                index_t start=0, end=0;
                unsigned char *pp1[d];
                boost::shared_array<unsigned char> p;
                pink::types::vint pp1_vec(d);
                index_t currlength=0;
                pink::types::vint pp1_pos(d);
                // Calculating the shift of the neighbourh elements
                FOR(w, d) {
                  pp1_vec.reset();
                  pp1_vec[w]=1;
                  index_t currpos = dim.position(pp1_vec);
///!!!	std::cout << "currpos=" << currpos << std::endl;
                  pp1_pos[w]=currpos;
                  pp1[w]=&(src_sink(currpos));
                } /* FOR(w, d) */
                p=src_sink.get_pixels();
                FOR(w, d) {
                  FOR(q, length_glob - pp1_pos[w] /* - 1 */ ) { /////////
                    if (started){
                      currlength++;
                      if ((p[q]!=0) and (pp1[w][q]!=0)){
                        end=q;
                        dib_flow[w].push_back(types::dibble_t(start, end));
                        ///!!! std::cout << "dibFlow[" << w << "]->addElement(" << start << "," << end << ")" << std::endl;
                        start=0;
                        end=0;
                        started=false;
                      } else /* NOT ((p[q]!=0) and (pp1[w][q]!=0)) */
                        if (currlength>=MaxDibble){
                          end=q+1;/////!!!!!!!!!!!!!!!
                          dib_flow[w].push_back(types::dibble_t(start, end));
                          start=0;
                          end=0;
                          started=false;
                        } /* (currlength>=MaxDibble) */
                    } else { /* NOT (started) */
                      if ((p[q]==0) or (pp1[w][q]==0)){
                        started=true;
                        start=q;
                        currlength=1;
                      }
                    } /* NOT (started) */
                  } /* FOR(q, length_glob-pp1_pos[w]-1) */
                } /* FOR(w, d) */
              } /* pragma omp section */
  
              //  -------- constrain -------
              // all the regular points and sources with at least one regular neighbour
              //#pragma omp section
              {
                bool started=false;
                index_t start=0, end=0;
                unsigned char *pp1[d], *pm1[d];
                boost::shared_array<unsigned char> p;
                pink::types::vint pp1_vec(d);
                index_t currlength=0;
                index_t max=0;
                FOR(w, d) {
                  pp1_vec.reset();
                  pp1_vec[w]=1;
                  index_t currpos = dim.position(pp1_vec);
                  max = max > currpos ? max : currpos;
                  pp1[w]=&( src_sink(currpos) );
                  pm1[w]=&( src_sink(0) ) - currpos;// risky, we always read far enough from the beginning
                } /* FOR(w, d) */
                p=src_sink.get_pixels();
                for ( index_t q=max; q < length_glob - max; q++ )
                {
                  bool i_am_a_regular_point = (p[q]==0.);
                  bool i_am_a_source = (p[q]==1.);
                  bool there_is_a_regular_point_near = false;
                  FOR(w, d) {
                    if ((pp1[w][q]==0.) or (pm1[w][q]==0.)) there_is_a_regular_point_near=true;
                  }
                  bool i_want_to_be_in_a_dibble = (i_am_a_regular_point or (i_am_a_source and there_is_a_regular_point_near));
                  //bool i_want_to_be_in_a_dibble = (i_am_a_regular_point or i_am_a_source);
                  if (started){
                    currlength++;
                    bool too_long = (currlength>=MaxDibble);
                    if (not i_want_to_be_in_a_dibble){
                      end=q;
                      dib_constrain.push_back(types::dibble_t(start, end));
                      ///!!! std::cout << "dib_constrain->addElement(" << start << ", " << end << ")" << std::endl;
                      start=0;
                      end=0;
                      started=false;
                    } else if (too_long){
                      //printf("uiNotice: breaking because of the MaxDibble" << std::endl;);
                      end=q+1;///////!!!!!!
                      dib_constrain.push_back(types::dibble_t(start, end));
                      start=0;
                      end=0;
                      started=false;
                    }
                  } else { /* NOT started */
                    if (i_want_to_be_in_a_dibble){
                      started=true;
                      start=q;
                      currlength=1;
                    } /* if (i_want_to_be_in_a_dibble) */
                  } /* NOT started */
                } /* FOR ( q, length_glob-max-1 ) */
              } /* pragma omp section*/
            } /* pragma omp sections */
          } /* pragma omp parallel */
          return 0; 

        } /* uiCreateDibbles */

      /**
         \brief This function detects the pure and tainted segments
         
      description The segments of the flow and the potencial are
      distributed between nodes of the system memory. If a thread
      iterates over a dibble, then the segments of this dibble has to
      be in fully the thread's node. This function therefore detects
      the overlapping dibbles. If a dibble is in a sole thread with
      all its segments, it is attributed the thread's number. If it is
      overlapping, it is marked tainted. In the end the function
      populates the pure thread list of the object.

      */      
      void tainted_dibbles()
        {
          // for a dibble to be to be pure, all the segments it's
          // using has to be in the same node. These segments include
          // the flow and the potencial points used during the
          // iteration.
          
          // resetting pure_potencial dibbles. Using the value
          // nbt+10 means, that the dibble is neither associated
          // with any thread nor rejected yet.
          pure_pot.resize(dib_potencial.size(), number_of_nodes + 10);
          pure_flow.resize(d);
          FOR(q, d)
          {
            pure_flow[q].resize(dib_flow[q].size(), number_of_nodes + 10);
          } /* FOR q, d */
          pure_cons.resize(dib_constrain.size(), number_of_nodes + 10);            

          
          // potencial --------------------------------------
          {
            // the potencial points used during the calculation of the
            // potencial are:
            // - the potencial points between first and second of the dibble
            // - the flow points from first-fm1 to second
            //   here fm1 is calculated as follow:
            //     fm1_vec.reset();
            //     fm1_vec[w]=1;
            //     fm1=dim.position(fm1_vec);

            // in this test we use pot_glob as the reference poly_array,
            // but the limits of the other array should be the same
            
            index_t reference_node = -1;
            index_t pure_dibbles   =  0;
            index_t pures_per_node[number_of_nodes];
            FOR(q, number_of_nodes)
            {
              pures_per_node[q]=0;
            }
            
            FOR(q, dib_potencial.size())
            {
              bool pot_is_in_good_node = false;
              bool flow_is_in_good_node = true; // here we use the optimistic approach
              
              { // first we check the potencial
                index_t pot_first  = dib_potencial[q].first;
                index_t pot_second = dib_potencial[q].second;
                reference_node     = pot_glob.node(pot_first);                
                
                if ( pot_glob.node(pot_second) == reference_node )
                {
                  pot_is_in_good_node = true;                    
                }                
              } // check the potencial
              
              {// now we check the pixels of the flow
                
                index_t flow_first;
                index_t flow_second;
                pink::types::vint fm1_vec(d);
                
                FOR(e, d)
                {
                  fm1_vec.reset();
                  fm1_vec[e]=1;//we are calculating the distance the opposite direction but it should be the same
                  index_t fm1 = dim.position(fm1_vec);
                  flow_first  = dib_potencial[q].first;
                  flow_second = dib_potencial[q].second - fm1;
                  
                  if (
                    (flow_glob[e].node(flow_first)  != reference_node)
                    or
                    (flow_glob[e].node(flow_second) != reference_node)
                    )
                  {                      
                    flow_is_in_good_node = false;                      
                  }
                } /* for e,d */
              } // check the pixels of the flow
              
              if ((pot_is_in_good_node) and (flow_is_in_good_node))
              {
                // std::cout << "dibble " << q << " is pure to " << nodes_of_this_thread << std::endl;                  
                pure_pot[q]=reference_node;
                pure_dibbles++;
                pures_per_node[reference_node]++;                
              }
              else /* NOT ((pot_is_in_good_nodes) and (flow_is_in_good_nodes) */
              {
              } /* NOT ((pot_is_in_good_nodes) and (flow_is_in_good_nodes) */
              
              
            } /* FOR q, dib_potencial->size */                            

            if (verbose)
              std::cout << "in the POTENCIAL there are " << pure_dibbles << " pure dibbles from " << dib_potencial.size()
                        << "; that makes " << 100 * pure_dibbles / dib_potencial.size() << "%" << std::endl;
            
          } // potencial
      
          // flow -------------------------------------------
          { // note that here we still use pot_glob, but all the distributed arrays
            // should be the same size, so its ok.
            
            FOR(w, d)
            {
              index_t reference_node = -1;
              index_t pure_dibbles   =  0;
              index_t pures_per_node[number_of_nodes];            
              FOR(q, number_of_nodes)
              {
                pures_per_node[q]=0;              
              }

              FOR(q, dib_flow[w].size())
              {

                pink::types::vint pp1_vec(d);
                index_t pp1_pos;              
                bool pot_is_in_good_node  = true; // here we use the optimistic approach
                bool flow_is_in_good_node = true; // here we use the optimistic approach
                reference_node = pot_glob.node(dib_flow[w][q].first);
                
                index_t flow_first  = dib_flow[w][q].first;
                index_t flow_second = dib_flow[w][q].second;
                
                pp1_vec.reset();
                pp1_vec[w] = 1;
                pp1_pos    = dim.position(pp1_vec);
                
                if (
                  (pot_glob.node(flow_first)  != reference_node )
                  or
                  (pot_glob.node(flow_second) != reference_node )
                  )
                {                  
                  flow_is_in_good_node = false;
                }
                
                if (
                  ( pot_glob.node( flow_first + pp1_pos )  != reference_node )
                  or
                  ( pot_glob.node( flow_second + pp1_pos ) != reference_node )
                  )
                {
                  pot_is_in_good_node = false;                  
                }
                
                if ((pot_is_in_good_node) and (flow_is_in_good_node))
                {
                  // std::cout << "dibble " << q << " is pure to " << nodes_of_this_thread << std::endl;                  
                  pure_flow[w][q] = reference_node;
                  pure_dibbles++;
                  pures_per_node[reference_node]++;                
                }
                else /* NOT ((pot_is_in_good_nodes) and (flow_is_in_good_nodes) */
                {
                  // std::cout << "dibble " << q << " is NOT pure" << std::endl;                  
                } /* NOT ((pot_is_in_good_nodes) and (flow_is_in_good_nodes) */

                
              } /* FOR(q, dib_flow->size()) */

              if (verbose)
                std::cout << "in the FLOW there are " << pure_dibbles << " pure dibbles from " << dib_flow[w].size()
                          << "; that makes " << 100 * pure_dibbles / dib_flow[w].size() << "%" << std::endl;
              
              // std::cout << "they are divided in [";
              // FOR(q, number_of_nodes - 1 )
              // {
              //   std::cout << 100 * pures_per_nodes[q] / dib_flow[w].size() << "%, ";
              // }
              // std::cout << 100 * pures_per_nodes[ number_of_nodes - 1 ] / dib_flow[w].size() << "%]" << std::endl;            
              
            } /* FOR(w, d) */           
           
          } // flow
          
          // constrain --------------------------------------
          {
            index_t reference_node = -1;
            index_t pure_dibbles   =  0;
            index_t pures_per_node[number_of_nodes];            
            FOR(q, number_of_nodes)
            {
              pures_per_node[q]=0;              
            }

            pink::types::vint fm1s(d), fm1_vec(d);                          
            FOR( w, d )
            {
              fm1_vec.reset();
              fm1_vec[w]=1;//we are calculating the distance the opposite direction but it should be the same
              fm1s[w]=dim.position(fm1_vec);
            }

            FOR(q, dib_constrain.size())
            {
              bool the_dibble_is_pure = true; // here we use the optimistic approach
              
              index_t first, second;
              first  = dib_constrain[q].first;
              second = dib_constrain[q].second;
              reference_node = pot_glob.node(first);

              if (reference_node != pot_glob.node(second))
              {
                the_dibble_is_pure = false;                
              }
              
              FOR(w, d)
              {
                if (
                  ( reference_node != pot_glob.node(first - fm1s[w]))
                  or
                  ( reference_node != pot_glob.node(second - fm1s[w]))
                  )
                {
                  the_dibble_is_pure = false;                  
                }
                
              } /* FOR w, d */
              
              if (the_dibble_is_pure)
              {
                pure_cons[q]=reference_node;
                pure_dibbles++;
                pures_per_node[reference_node]++;                
              } /* the_dibble_is_pure */
                            
            } /* FOR q dibContrain->size() */

            if (verbose)
              std::cout << "in the CONSTRAIN there are " << pure_dibbles << " pure dibbles from " << dib_constrain.size()
                        << "; that makes " << 100 * pure_dibbles / dib_constrain.size() << "%" << std::endl;
            
            // std::cout << "they are divided in [";
            // FOR(q, number_of_nodes - 1 )
            // {
            //   std::cout << 100 * pures_per_nodes[q] / dib_constrain.size() << "%, ";
            // }
            // std::cout << 100 * pures_per_nodes[ number_of_nodes - 1 ] / dib_constrain.size() << "%]" << std::endl;

          } // constrain
          
        } /* uiTaintedDibbles */


      /**
         \brief This method distributes the dibbles between the threads.
         
      description The pure dibbles are collected by 'uiTaintedDibbles' 

      \param 
      \return 
      */      
      void distribute_dibbles()
        {

          if (verbose)
            std::cout << "compacting the source and the sink" << std::endl;

          // note here:
          //   - 0 will be the regular cells (neither src nor sink)
          //   - 1 will be the src
          //   - 2 will be the sink
          compact_srcsink = collector( src_sink, src_sink.get_size().prod(), 2/*number_of_nodes 2, the src and the sink*/);

          synchro_src  = partition( compact_srcsink[1], number_of_threads );
          synchro_sink = partition( compact_srcsink[2], number_of_threads );

          if (verbose)
            std::cout << "compacting the potencial" << std::endl;          

          compact_pot = pure_sort( pure_pot, dib_potencial, number_of_nodes );
          
          // compact_pot = pink::numa::collector(
          //   pure_pot,
          //   pure_pot.size(),
          //   number_of_nodes
          //   );

          // Breaking up the dibbles to packets.
          // These packets will then be distributed dynamically.
          resolut_pot.resize(number_of_nodes);

          FOR(q, number_of_nodes)            
          {
            resolut_pot[q] = partition( compact_pot[q], resolution );
          } /* FOR q, number_of_nodes */

          synchro_pot = partition( compact_pot[number_of_nodes], number_of_threads );

          if (verbose)
            std::cout << "compacting the flow" << std::endl;

          resolut_flow.resize(d);          
          FOR(q, d)
          {
            compact_flow[q] =
              pure_sort( pure_flow[q], dib_flow[q], number_of_nodes );
            
            synchro_flow[q] = partition( compact_flow[q][number_of_nodes], number_of_threads );

            // Breaking up the dibbles to packets.
            // These packets will then be distributed dynamically.
            resolut_flow[q].resize(number_of_nodes);          
            FOR(w, number_of_nodes)            
            {
              resolut_flow[q][w]=partition( compact_flow[q][w], resolution );            
            } /* FOR w, number_of_nodes */
            
          } /* FOR q in d */

          if (verbose)
            std::cout << "compacting the constrain" << std::endl;

          compact_cons =
            pure_sort( pure_cons, dib_constrain, number_of_nodes );          

          // Breaking up the dibbles to packets.
          // These packets will then be distributed dynamically.
          resolut_cons.resize(number_of_nodes);
          
          FOR(q, number_of_nodes)            
          {
            resolut_cons[q] = partition( compact_cons[q], resolution );
          } /* FOR q, number_of_nodes */
          
          synchro_cons = partition( compact_cons[number_of_nodes], number_of_threads );

          if (verbose)
            std::cout << "compacted everything" << std::endl;
        } /* distflow::uiDistributeDibbles */
      

      

    public:

      // this variable will hold the elapsed time
      double time;      
      
      /**
         \brief returns the calculated flow in raw format
         
         description the length of the array is dimension * pixels, and
         the vectors are grouped by direction

         \param 
         \return 
      */      
      boost::shared_array<pixel_type> get_flow() 
        {
          if (not flow_calculated){
            pink_error("The flow has not yet been calculated. You can only call get_flow after at least 1 iteration."
                       " For the courious souls after the 0th iteration the flow is zero everywhere");
          }
          else /* (flow_calculated) */
          {
            return flow_glob;
          }/* (flow_calculated) */
          return flow_glob;
        }
      
        
      /**
       * The iterator function. Copyes everything to local variables, so the compiler can optimize, then manages the iterations, mesures the time generates messages and saves intermediat results.
       * @param D input measure picture
       * @param S image representing source and sink (source = 1; sink = -1)
       * @param g the boundari measure field
       * @param iteration number of all iterations demanded. The number of the iterations can be estimated from the parameters. It needn't to be tested dinamically.
       * @param tau the iterating constant
       * @param uiMovie whether we want a movie or not
       * @return returns the final (hopefully yet convergent image)
       */
      distributed_flow(
        const char_image & SS,     /* image of the source and of the sink (not the original image) */
        const image_type & gg,     /* Boundaries */
        index_t iteration,         /* number of iterations */
        float   tau               = 0.132, /* timestep */
        index_t number_of_threads = 0, /* the number of threads to execute if in parallel mode */
        bool verbose              = false
        ) :
        tau(tau),
        verbose(verbose),
        d(gg.get_size().size()),
        dim(gg.get_size()),
        iteration(iteration),
        flow_calculated(false),
        number_of_threads(number_of_threads),
        packet_size(packet_size),
        length_glob(gg.get_size().prod()),
        g_glob(gg.get_size().prod()),
        pot_glob(gg.get_size().prod()),
        flow_glob(gg.get_size().size()), // number_of_threads, d*length_glob, NUMA )
        dib_potencial(),
        dib_constrain(),
        dib_flow( gg.get_size().size() + 3 ), // we are adding here 3 because of the parallelization later
        potencial(gg.get_size()), // we will put the result in this image
        compact_flow(gg.get_size().size()), // the proper size
        synchro_flow(gg.get_size().size()) //,        
        {
#         ifdef UJIMAGE_DEBUG
          std::cout << "creating the distflow object (" << static_cast<void*>(this) << ")" << std::endl;	
#         endif /* UJIMAGE_DEBUG */        

          if (number_of_threads<0)
          {
            pink_error("Wrong number of threads!");
          }
          else if (number_of_threads==0)
          {
            // getting the number of cpu-s on the system
            this->number_of_threads = boost::thread::hardware_concurrency();
            this->number_of_threads = std::max<index_t>( 1, this->number_of_threads );            
          } /* number_of_threads == 0 */
          this->number_of_nodes = pot_glob.number_of_nodes;
          if ( this->number_of_threads % this->number_of_nodes != 0 )
          {
            pink_error("the number of threads must be divisible with the number of nodes !");            
          }

          this->resolution      = this->number_of_threads / this->number_of_nodes;
          this->semaphores.reset( new semaphores_t<>(this->number_of_threads) );          

          if (verbose)
          {            
            std::cout << std::endl << "NUMA dibble edition" << std::endl;
            // creating a local copy of image, srcsink potencial and flows ---------------------------
            std::cout << "dimension   = " << dim.repr() << " (" << d << "D)" << std::endl;
            std::cout << "length_glob = " << this->length_glob << std::endl;
            std::cout << "tau         = " << this->tau << std::endl;
            std::cout << "iteration   = " << this->iteration << std::endl;
            std::cout << "threads     = " << this->number_of_threads << " (of " << boost::thread::hardware_concurrency() << ")" <<  std::endl;
            std::cout << "nodes       = " << this->number_of_nodes <<  std::endl;
            std::cout << "threads/node= " << this->resolution <<  std::endl << std::endl;          
          } /* if verbose */
          
          // Now we copy the pointers to global variables, so the threads can see them.
          // boost::shared_ptr is boost's 'shared_array' smart pointer.
          this->gg.copy(gg);

          if (verbose)
            std::cout << "initializing the distributed array" << std::endl;

          FOR(q, d)
          {
            poly_array_t   flow_line( gg.get_size().prod() );            
            flow_glob[q] = flow_line;            
          }

          //cleaning the flow
          //#pragma omp parallel for schedule(guided) num_threads(this->number_of_threads)
          FOR(q, d)
          {            
            FOR(w, length_glob)
            {
              flow_glob[q][w]=0.;
            } /* end of parallel FOR */
          } /* for d */

          FOR(q, length_glob)
          {
            pot_glob[q]=0.;            
          }
          
          src_sink.copy(SS);	
          //// --------------------- breaking the fields into dibbles ----------------------------

          if (verbose)
            std::cout << "breaking up the field into dibbles" << std::endl;        
          create_dibbles();

          if (verbose)
            std::cout << "looking for the tainted dibbles" << std::endl;
          tainted_dibbles();

          if (verbose)
            std::cout << "distributing the dibbles between threads" << std::endl;
          distribute_dibbles();          

          if (verbose)
            std::cout << "copying g_glob" << std::endl;
          
          FOR(q, gg.get_size().prod())
          {
            g_glob[q]=gg(q);            
          }          
          
        } /* constructor distflow */
      

      virtual image_type start()
        {
          // Posix threading
          typedef typename boost::shared_ptr<boost::thread> pthread_t;

          boost::shared_array<pthread_t>
            threads( new pthread_t[number_of_threads] );
          
          // this distributor will distribute the threads between the
          // nodes. Note: the order of parameters is contraintuitive.
          distributor_t node_distributor( number_of_threads, number_of_nodes );
                    
          if (verbose)
            std::cout << "starting the iteration" << std::endl;

          double starttime = pink::benchmark::now();
          
          // Creating the threads
          FOR( w, number_of_threads )
          {
            threads[w].reset( new boost::thread(
                                maxflow_iterator<image_type>,
                                *this,
                                w,
                                node_distributor.node(w) // this is the node belonging to the segment
                                )
              );            
          } /* FOR(q, number_of_threads) */
          
          // waiting for the threads to shut down
          FOR( w, number_of_threads )
          {
            threads[w]->join();
          } /* FOR(w, number_of_threads) */

          double endtime = pink::benchmark::now();

          this->time = static_cast<double>( endtime - starttime );          

          if (verbose)
            std::cout << "the iteration has finished" << std::endl;          

          //// --------------------- printing out the measured time ------------------------------
          if (verbose)
            std::cout << "total time of iteration: " << this->time << "s" << std::endl;
    
          this->flow_calculated = true; 

          // copy the calculated potencial to the 'potencial' image
          FOR(q, dim.prod())
          {
            //potencial(q) = put_in(pot_glob[q]);
            potencial(q) = pot_glob[q];
          } /* for q */
          
          return potencial;          
        } /* virtual start*/
      

      virtual ~distributed_flow()
        {      
#         ifdef UJIMAGE_DEBUG      
          std::cout << "destroying the distflow object (" << static_cast<void*>(this) << ")" << std::endl;
#         endif /* UJIMAGE_DEBUG */        
        }
    }; /* class distributed_flow */


    /**
       \brief This is the thredding function for the distflow
       iteration. It takes the distflow object, and performs the
       necessary calculations, while it keeps itself synchronized with
       the main thread.
       
    description This function uses several semaphores to synchronize
    the calculation limits with the main thread. For now, the dibbles
    on which it's suppossed to iterate are static, that is to say, it
    is divided approximately evenly between the threads before the
    beginning of the iteration. After the final iteration, the thread
    quits.
    */
    template <class image_type>
    void
    maxflow_iterator
    (
      pink::numa::distributed_flow<image_type> & obj,
      index_t ID,
      index_t node_of_this_thread
      )
    {
      typedef typename pink::numa::distributed_flow<image_type>::poly_array_t array_t;
      index_t resolution = obj.resolution;

      // *********************************************************
      // *********************************************************
      // ************ Initializing the NUMA part *****************
      // *********************************************************
      // *********************************************************

      // restricting myself for the node, where the segment was
      // allocated
      // // for numa 2.0
      // struct bitmask * mask = numa_bitmask_alloc(numa_num_possible_nodes());
      // numa_bitmask_setbit(mask, node_of_this_thread);
      // numa_bind(mask);
      // numa_bitmask_free(mask);

      // for numa 1.0
      nodemask_t mask;
      nodemask_zero(&mask);
      nodemask_set(&mask, node_of_this_thread);
      numa_bind(&mask);
      
      // *********************************************************
      // *********************************************************
      // *************** Starting the iterations *****************
      // *********************************************************
      // *********************************************************

      //!!! std::cout << "starting the iteration in thread " << ID << std::endl;
      FOR(q, obj.iteration)
      {
        obj.semaphores->potencial.wait();        
        obj.update_potencial( obj.resolut_pot[node_of_this_thread][ ID % resolution ], fast_iterator<array_t> ); /// here I calculate my part of the potencial
        obj.update_potencial( obj.synchro_pot[ID], slow_iterator<array_t> ); // updating the common part

        obj.semaphores->flow.wait();
        // here I calculate my part of the flow
        FOR(w, obj.d)
        {
          obj.update_flow( obj.resolut_flow[w][node_of_this_thread][ ID % resolution ], fast_iterator<array_t>, w );
          obj.update_flow( obj.synchro_flow[w][ID], slow_iterator<array_t>, w );
        }

        obj.semaphores->constrain.wait();
        //obj.update_constrain(obj.compact_cons[ID], fast_iterator<array_t> );
        obj.update_constrain(obj.resolut_cons[node_of_this_thread][ ID % resolution ], fast_iterator<array_t> );
        // updating the common part
        obj.update_constrain(obj.synchro_cons[ID], slow_iterator<array_t> );
        
        // in this instant the srcsink is updated in the threads (right here)
        obj.update_srcsink( obj.synchro_src [ID], 1. );
        obj.update_srcsink( obj.synchro_sink[ID], 0. );
        
      } /* FOR q, iterations */
    } /* maxflow_iterator */




    template <class image_type>
    boost::python::object
    distflow( 
      char_image SS,  /* image of the source and of the sink (not the original image) */
      image_type gg,  /* Boundaries */
      index_t    iteration,            /* number of iterations */
      float      glob_tau = 0.132,     /* timestep */
      index_t    number_of_threads = 0, /* the number of threads to execute if in parallel mode */
      bool       verbose = false,
      bool       debug   = false
      )
    {
      distributed_flow<image_type> obj(
        frame_around(SS, -1),
        frame_around(gg, 0.),
        iteration,
        glob_tau,
        number_of_threads,
        verbose
        );

      image_type rimage = frame_remove(obj.start());

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
    } /* distflow */
    
  } /* namespace numa */   
} /* end namespace pink */


// cleaning up after us
#   undef REPORT_INTERVAL

// // obsolete, replaced by enum
// #undef __ETAP_POT
// #undef __ETAP_FLOW
// #undef __ETAP_CONSTR

// // obsolete, loaded as a parameter
// #undef PACKET_SIZE


#   undef ATOMIC

# endif /* PINK_HAVE_NUMA */

#endif /* UIFLOW_DISTRIBUTED_HPP_*/
/* LuM end of file */
