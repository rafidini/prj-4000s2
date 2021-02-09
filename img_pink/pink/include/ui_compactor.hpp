/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI__COMPACTOR__HPP__
#define UI__COMPACTOR__HPP__

/**
   Imagine the following sequence of numbers:
   
   1 1 1 1 1 1 1 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 10 10 10 10 10 1 1 1 1 1 2 2 2 2 2 3 3 3

   We would like to do to things:
   
   a) compactify them; that is to say create a list, where the similar
   values are put together:
   value 1: 0-5, 10-15
   value 2: 5-10, 20-25
   ...
   value 10: 10-20, 30-40

   b) distribute them evenly among 'number_of_threads' threads. For
   this we know that there are less nodes than threads. And that each
   thread can only process elements in it's own node.

   dibble[0] = { }                                  
   dibble[1] = { }                                  
   dibble[2] = { }                                  
   dibble[3] = { }                                  
   dibble[4] = { }                                  
   dibble[5] = { }                                  
   dibble[6] = { }                                  
   dibble[7] = { }                                  
   dibble[8] = { [0, 1) }                           
   dibble[9] = { [1, 3) }                           
   dibble[10] = { [3, 4) }                          
   dibble[11] = { [4, 6) }                          
   dibble[12] = { [6, 7) }                          
   dibble[13] = { [7, 7), [27, 29) }                
   dibble[14] = { [29, 30) }                        
   dibble[15] = { [30, 32) }                        
   dibble[16] = { [7, 8) }                          
   dibble[17] = { [8, 10) }                         
   dibble[18] = { [10, 11) }                        
   dibble[19] = { [11, 13) }                        
   dibble[20] = { [13, 14) }                        
   dibble[21] = { [14, 16) }                        
   dibble[22] = { [16, 17) }                        
   dibble[23] = { [17, 19) }                        
   dibble[24] = { [14, 15) }                        
   dibble[25] = { [15, 16) }                        
   dibble[26] = { [16, 18) }                        
   dibble[27] = { [18, 19) }                        
   dibble[28] = { [19, 20) }                        
   dibble[29] = { [20, 22) }                        
   dibble[30] = { [22, 23) }                        
   dibble[31] = { [23, 25) }                        
   dibble[32] = { [22, 27), [38, 42) }              
   
*/

#include <vector>
#include <utility>
#include <iostream>
#include <boost/smart_ptr.hpp>
//#include <boost/multi_array.hpp>

#include "mcimage.h" // for index_t
#include "ui_pink_types.hpp"
#include "ui_polythread.hpp" // for distributed_t
#include "ui_numa_types.hpp"


namespace pink
{
  namespace  numa
  {

    /**
       \brief This function prints an array of dibbles.
       
    \param vector The array of dibbles to print
    */    
    template <class T0>
    void repr(T0 vector)
    {
      std::cout << "{ ";
  
      for( index_t w=0; w < vector.size(); w++ )
      {      
        std::cout << "[" << vector[w].first << ", " << vector[w].second << ")";
        if (w+1 < vector.size())
        {
          std::cout << ", ";        
        }
        else
        {
          std::cout << " ";        
        }    
      }
  
      std::cout << "}" << std::endl;
  
    } /* repr */



    template <class T0>
    index_t len( const T0 & t0 )
    {
      index_t result=0;
      
      for( index_t q=0; q < t0.size(); q++)
      {
        result+=t0[q].second - t0[q].first;    
      }

      return result;  
    } /* len array */


    // template <class T0, class T1>
    // index_t
    // len( const T0 & array, const T1 & dibbles )
    // {
    //   index_t result = 0;
      
    //   FOR( q, array.size() )
    //   {
    //     for ( index_t w = array[q].first; w < array[q].second; w++ )
    //     {
    //       result += dibble[w].second - dibble[w].first;  
    //     } /* for w in first..second */
    //   } /* FOR q in array.size */

    //   return result;
    // } /* len array, dibbles */
    

    template <class T0, class T1, class T2>
    T0
    extract( T0 & vectors, T1 first, T2 second )  
    {
      //std::cout << "extracting [" << first << ", " << second << ") from ";
      //repr(vectors);    
    
      T0 result;

      if ( (first==second) or first >= len(vectors) )
      {
        return result;
      }

      index_t q, first_dibble, second_dibble,
        first_dibble_element, second_dibble_element, distance=0;

      // looking for the dibble containing the first element
      first_dibble=0;
      distance=0; //relative distance from the beginning of the segments
      first_dibble_element = first - distance; // the position of the element, if it would fit into the dibble

      while (
        ( first_dibble_element > vectors[first_dibble].second - vectors[first_dibble].first )
        and
        (first_dibble < vectors.size())
        )
      {
        distance += vectors[first_dibble].second - vectors[first_dibble].first;
        first_dibble++;
        first_dibble_element = first - distance; // the position of the element, if it would fit into the dibble
      }
 
      // looking for the dibble containing the second element
      second_dibble = first_dibble;
      second_dibble_element = second - distance;

      // distance stays the same as before
      while (
        ( second_dibble_element > vectors[second_dibble].second - vectors[second_dibble].first )
        and
        ( second_dibble < vectors.size() )
        )
      {   
        distance += vectors[second_dibble].second - vectors[second_dibble].first;
        second_dibble_element = second - distance;
        second_dibble++;
      }
  
      // there are two cases:
      //   - all the elements are from one segment:
      //       [1, ..., first, ..., second, ..., N)
      //       in this case one dibble is returned
      //   - the first and the second elements are in different segments:
      //       [1, ..., first, ..., N0 ), ..., [1, ..., second, ... N1)
      //       in this case the first and second are extracted while the rest is copied

      if (first_dibble==second_dibble)
      {
        result.push_back(
          types::dibble_t(
            vectors[first_dibble].first + first_dibble_element,
            vectors[second_dibble].first + second_dibble_element
            )
          );    
      }
      else // NOT first_dibble == second_dibble
      {
        // creating the first dibble
        result.push_back(
          types::dibble_t(
            vectors[first_dibble].first + first_dibble_element,
            vectors[first_dibble].second
            )
          );

        for (index_t q=first_dibble + 1; q < second_dibble; q++)
        {
          result.push_back(vectors[q]);      
        }
        
        // creating the last dibble
        result.push_back(
          types::dibble_t(
            vectors[second_dibble].first,
            vectors[second_dibble].first + second_dibble_element
            )
          );

      } // NOT first_dibble == second_dibble

      //std::cout << "extracted ";
      //for (index_t q=0; q<result.size(); q++)  repr(result);
  
      return result;  
    } /* extract array */


    // template <class T0, class T1, class T2, class T3>
    // T0
    // extract ( T0 & array, T1 & dibbles, T2 first, T3 second )
    // {
    //   T0 result;

    //   index_t q, first_dibble, second_dibble, ...
      


    //   return result;
      
    // } /* extract array, dibbles */
    
    
    
    template <class array_t, class T1, class T2>
    types::vector2D<types::dibble_t>
    collector( array_t & input, T1 size, T2 number_of_nodes )
    {
      types::vector2D<types::dibble_t> result(number_of_nodes+1);
      
      index_t q=0;  
      bool inside=false;
      index_t first;
      index_t second;  
      
      while ( q < size )
      {
        first = q;

        while ((input[q]==input[first]) and (q<size)) { q++; };
        
        second = q;
        
        if (input[first] < number_of_nodes)
        {      
          result[ input[first] ].push_back(types::dibble_t(first, second));
        }
        else
        {
          result[number_of_nodes].push_back(types::dibble_t(first, second));
        }
        
      } /* while q < input.size() */

      return result;      
      
    } /* collector */


    /**
       \brief Partitions a sets of dibbles between threads
       
    description 

    \param 
    \return 
    */    
    template <class array_t, class T1>
    types::vector2D<types::dibble_t> partition( array_t & array, T1 number_of_packets )
    {
      
      distributor_t distributor(len(array), number_of_packets);

      types::vector2D<types::dibble_t> result(number_of_packets);

      FOR(q, number_of_packets)
      {
        result[q] = extract(array, distributor.low(q), distributor.high(q));        
      } /* FOR q in number_of_packets */

      return result;
      
    } /* partition */
    

    // template <class array_t, class T1>
    // vector2D<dibble_t>
    // compactor( array_t & input, T1 number_of_threads )
    // {
    //   // this function was longer but some of it's part's are not necessary
      
    //   // -------------------------------------------------------
    //   // Collecting the dibbles with the same node
    //   // -------------------------------------------------------
  
    //   vector2D<dibble_t> result = collector(input, input.size(), number_of_threads);
  
    //   return result;      
  
    // } /* compactor */
  

    // /**
    //    This function partitions the dibbles equally with respect to
    //    the actuall pixels instead of merely the dibbles.
    //  */
    // template <class T0, class T1, class T2>
    // vector2D<dibble_t>
    // fine_partition( T0 & array, T1 & dibbles, T2 & number_of_chunks )
    // {
    //   distributor_t distributor( len(array, dibbles), number_of_chunks );
      
    //   vector2D<dibble_t> result(number_of_chunks);
      
    //   FOR(q, number_of_chunks)
    //   {
    //     result[q] =
    //       extract( array, dibbles, distributor.low(q), distributor.high(q) );        
    //   } /* FOR q in number_of_packets */
      
    //   return result;
    // } /* fine_partition */
    
    template <class T0, class T1, class T2>
    types::vector2D<types::dibble_t>
    pure_sort( T0 & pure, T1 & dibbles, T2 number_of_nodes )
    {
      types::vector2D<types::dibble_t> result;
      
      result.resize( number_of_nodes + 1 );

      if ( dibbles.size() != pure.size() )
      {
        pink_error("The size of the dibbles is different from the size of the pure array !");
      }
          
      FOR( q, dibbles.size() )
      {
        if (pure[q] < number_of_nodes)
        {
          result[pure[q]].push_back(dibbles[q]);
        }
        else /* NOT pure[q] < number_of_nodes */
        {
          result[number_of_nodes].push_back(dibbles[q]);              
        } /* NOT pure_pot[q] < number_of_nodes */ 
      } /* For q in dibPotencial.size() */

      return result;
    } /* pure_sort */
    











  } /* namespace numa */

} /* namespace pink */

#endif /* UI__COMPACTOR__HPP__ */


// LuM end of file
