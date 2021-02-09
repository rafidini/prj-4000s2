/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI__NUMA__TYPES__HPP__
#define UI__NUMA__TYPES__HPP__

#include <vector>
#include <utility>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

#include "mcimage.h"
#include "ui_pink_types.hpp"

namespace pink {
  
  namespace types {
    
    enum alloc_t { GLOBAL, SEMI_LOCAL, LOCAL, NUMA };
    
    typedef std::pair<index_t, index_t> dibble_t;    


    template <class T0>
    class shared_vector
    {
    public:
      typedef T0 value_type;
      
    private:
      index_t size_;
      index_t alloc_size;      
      boost::shared_array<value_type> data;

      void grow()
        {
          alloc_size <<= 1;          
          boost::shared_array<value_type> tmp( new value_type[alloc_size] );
          if (size_>0)
          {            
            std::copy( &(data[0]), &(data[size_-1]) + 1, &(tmp[0]) );
          }
          
          data = tmp;            
        }
      
            
    public:
      shared_vector(): size_(0), alloc_size(1), data(new value_type[1])
        {          
        }


      
      shared_vector(index_t size): size_(size)
        {
          alloc_size = 1;
          while (alloc_size<size) alloc_size <<= 1;
          
          data.reset(new value_type[alloc_size]);
          return;          
        }


      
      shared_vector(index_t size, value_type defval): size_(size)
        {          
          alloc_size = 1;
          while (alloc_size<size) alloc_size <<= 1;          
          data.reset( new value_type[alloc_size] );

          if (size>0)
          {
            std::fill( &(data[0]), &(data[size-1]) + 1, defval);
          }
          
          return;          
        }


      
      shared_vector( const shared_vector & other )
        : size_(other.size_), alloc_size(other.alloc_size)
        {
          data = other.data;
        }


      
      virtual ~shared_vector() { }

            

      value_type * get()
        {
          return data.get();          
        }
      

      
      index_t size() const
        {
          return size_;          
        }


      
      value_type & operator[](index_t pos)
        {
          return data[pos];          
        }


      
      const value_type & operator[](index_t pos) const
        {
          return data[pos];          
        }


      
      void push_back( value_type value )
        {
          if ( size_ < alloc_size )
          {
            size_++;
          }
          else /* NOT size_<alloc_size */
          {
            grow();
            size_++;            
          } /* NOT size_<alloc_size */
          
          data[ size_ - 1 ] = value;          

          return;          
        }


      
      void resize(index_t size)
        {
          if ( size <= alloc_size )
          {
            size_=size;            
          }
          else /* NOT size < size_ */
          {
            while ( alloc_size < size ) alloc_size <<= 1;
            boost::shared_array<value_type> tmp( new value_type[alloc_size] );
            if (size_>0)
            {              
              std::copy( &(data[0]), &(data[size_-1]) + 1, &(tmp[0]) );
            }

            size_= size;
            data = tmp;                          
            
          } /* NOT size < size_ */

          return;
          
        } /* vector2D::resize  */
      
    }; /* class shared_vector */
    
      
    // template <class value_type>    
    // class vector2D: public shared_vector< shared_vector<value_type> >
    // {
    // public:
    //   vector2D(): shared_vector< shared_vector<value_type> >() { }
      
    //   vector2D( index_t size ): shared_vector< shared_vector<value_type> >(size) { }
      
    //   vector2D( index_t size, value_type value )
    //     : shared_vector< shared_vector<value_type> >(size) { }

    //   vector2D( const vector2D<value_type> & other )
    //     : shared_vector< shared_vector<value_type> >(other) { }

    //   virtual ~vector2D() { }
      
          
    // }; /*class vector2D */
    
    // template <class value_type>
    // class vector3D: public shared_vector< vector2D<value_type> >
    // {
    // public:
    //   vector3D(): shared_vector< vector2D<value_type> >() { }
      
    //   vector3D( index_t size ): shared_vector< vector2D<value_type> >(size) { }

    //   vector3D( const vector3D<value_type> & other )
    //     : shared_vector< vector2D<value_type> >( other ) { }
      
    //   virtual ~vector3D() { }
    //   //vector3D(index_t size, vector2D<value_type> value) : std::vector< vector2D<value_type> >(size, value) {}
    // }; /* class vector3D */


    
    // template <class value_type>
    // class vector4D: public shared_vector< vector3D<value_type> >
    // {
    // public:
    //   vector4D(): shared_vector< vector3D<value_type> >() { }
      
    //   vector4D( index_t size ): shared_vector< vector3D<value_type> >(size) { }

    //   vector4D( const vector4D<value_type> & other )
    //     : shared_vector< vector3D<value_type> >( other ) { }
      
    //   virtual ~vector4D() { }
    //   //vector3D(index_t size, vector2D<value_type> value) : std::vector< vector2D<value_type> >(size, value) {}
    // }; /* class vector4D */

    
    
      
    
    // // vectors!
    template <class value_type>    
    class vector2D: public std::vector< std::vector<value_type> >
    {
    public:
      vector2D(): std::vector< std::vector<value_type> >(0) {}
      vector2D(size_t size): std::vector< std::vector<value_type> >(size) {}
      vector2D(size_t size, value_type value) : std::vector< std::vector<value_type> >(size, value) {}
      
    }; /*class vector2D */
    
    template <class value_type>
    class vector3D: public std::vector< vector2D<value_type> >
    {
    public:
      vector3D(): std::vector< vector2D<value_type> >(0) {}
      vector3D(size_t size): std::vector< vector2D<value_type> >(size) {}
      vector3D(size_t size, vector2D<value_type> value) : std::vector< vector2D<value_type> >(size, value) {}
    }; /* class vector3D */
    
    template <class value_type>
    class vector4D: public std::vector< vector3D<value_type> >
    {
    public:
      vector4D(): std::vector< vector3D<value_type> >(0) {}
      vector4D(size_t size): std::vector< vector3D<value_type> >(size) {}
      vector4D(size_t size, vector3D<value_type> value) : std::vector< vector3D<value_type> >(size, value) {}
    }; /* class vector3D */


    /**
       This class distributes pieces of calculation. It is thread
       safe.
     */
    template <class T0>
    class dealer_t
    {
    public:
      typedef T0 array_t;
      
      typedef typename array_t::value_type         value_type;      
      typedef typename std::pair<value_type, bool> packet_t;
      

    private:
      array_t array;           
      boost::shared_ptr<index_t> pos;
      //volatile index_t * pos;      
      index_t size;
      boost::shared_ptr<boost::mutex> mutex;
      

    public:

      dealer_t() { }
      
      
      dealer_t( array_t array )
        : array(array),
          pos(new index_t),
          size(array.size()),
          mutex(new boost::mutex)      
        {
          *pos=0;          
        }


      void reset()
        {
          mutex->lock();          
          *pos=0;
          mutex->unlock();          
        }



      /**
         This is an atomic function which gives the next packet of the
         iteration.
       */
      packet_t operator()()
        {
          mutex->lock();          
          index_t rpos = (*pos)++;
//          index_t rpos = *pos;
//          (*pos)++;
          mutex->unlock();          

          if ( rpos >= size )
          {
            return packet_t(value_type(), false);            
          }
          else /* NOT pos >=size */
          {
            return packet_t(array[rpos], true);
          } /* NOT pos >=size */
        } /* dealer_t::operator() */
            

      
      virtual ~dealer_t(){}
      
      
    }; /* class dealer_t */
    
      
    
    
    
  } /* namespace types */
  
} /* namespace pink */










#endif /* UI__NUMA__TYPES__HPP__ */
// LuM end of file
