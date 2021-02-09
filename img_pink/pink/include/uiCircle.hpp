/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_CIRCLE_HPP_
#define UI_CIRCLE_HPP_

#include <string>
#include <sstream>
#include <iostream>
#  ifdef PINK_HAVE_PYTHON
#    ifdef _WINDOWS
#      define BOOST_PYTHON_STATIC_LIB
#    endif /* _WINDOWS */
#    include <boost/python.hpp>
#  endif /* PINK_HAVE_PYTHON */
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_interp.h> // uiGradient is using it
#include <gsl/gsl_multifit_nlin.h> // uiCircle is using it

#include "ui_pink_types.hpp"

namespace pink { 

  namespace gsl
  {

    class vector
    {
    private:
      int tag;
      bool wrapper;  
      gsl_vector * self;
      
    public:
      vector(int n); // initializes a vector of size n
      vector(const vector & other); // copy constructor
      vector( const gsl_vector * shallow ); // this initializes vector in wrapper mode
      vector & operator=(const vector & other); // copy constructor
      vector(); // empty constructor    
      int size() const;      
      void set_all(double val); // sets all elements of the vector the given value
      double & operator[](int pos); // vector element access
      const double & operator[](int pos) const; // vector element access
      virtual ~vector(); // default destructor
      operator gsl_vector*(); // type casting operator
      vector & operator << ( double first_value );
      vector & operator, (double next);
      std::string repr() const;
      
    }; /* class vector */
    
    typedef std::pair<vector, vector> point_type;
    float dist(const vector & A, const vector & B);
    
  
    class matrix
    {
    private:
      int tag;
      bool wrapper;  
      gsl_matrix * self;
  
    public:
      matrix(int row, int column); // initializes a matrix of size n
      matrix(const matrix & other); // copy constructor
      matrix( const gsl_matrix * shallow ); // this initializes matrix in wrapper mode
      matrix & operator=(const matrix & other); // copy constructor
      matrix(); // empty constructor    
      int rows() const;    
      int columns() const;    
      void set_all(double val); // sets all elements of the matrix the given value
      double & operator()(int row, int column); // matrix element access
      const double & operator()(int row, int column) const; // matrix element access
      virtual ~matrix(); // default destructor
      operator gsl_matrix*(); // type casting operator
      std::string repr() const;    
    }; /* class matrix */
   
    // this is an abstract class. You will have to define your own f and df functions
    // optionally you can define an fdf function to accelerate the iterations
    class fdfsolver
    {
    private:
      gsl_multifit_fdfsolver * solver;
      gsl_multifit_function_fdf * function;
      int n; // number of functions (f_i)
      int p; // number of parameters to optimize ( f_i(P_1, P_2, ..., P_p))
      int numb_iterations;

      // this function should store the vector result f(x,params) in f for argument x and
      // arbitrary parameters params, returning an appropriate error code if the function
      // cannot be computed.
      static int gsl_f(const gsl_vector * x, void * params, gsl_vector * f);    
      
      // this function should store the n-by-p matrix result J_ij = d f_i(x,params) / d x_j in J
      // for argument x and arbitrary parameters params, returning an appropriate error
      // code if the function cannot be computed.
      static int gsl_df(const gsl_vector * x, void * params, gsl_matrix * J);
    
      // This function should set the values of the f and J as above, for arguments x and
      // arbitrary parameters params. This function provides an optimization of the
      // separate functions for f(x) and J(x)—it is always faster to compute the function
      // and its derivative at the same time. 
      static int gsl_fdf(const gsl_vector * x, void * params, gsl_vector * f, gsl_matrix * J);

    protected:
      int init(vector & init_vector);
    
    public:
      fdfsolver( const gsl_multifit_fdfsolver_type * type,
                 int n /*observations*/,
                 int p /* number of parameters */
        );
      virtual ~fdfsolver();
      std::string repr() const;      
      virtual int f( const vector & x, vector & f ) = 0;
      virtual int df( const vector & x, matrix & J ) = 0;
      virtual int fdf( const vector & x, vector & f, matrix & J);
      int iterate();
      int iterate( int n );

      // this function iterates until epsilon precision is reached
      // n is the number of iterations between testing the convergence      
      int auto_iterate( float varepsilon = 0.0001, int nval=10 );
    
      vector position();
      int iterations();
    }; /* class fdfsolver */
    
// teaching cout to print vector
    std::ostream& operator<<(std::ostream& output, const vector& show_me);

// teaching cout to print vector
    std::ostream& operator<<(std::ostream& output, const matrix& show_me);

// teaching cout to print fdfsolver
    std::ostream& operator<<(std::ostream& output, const fdfsolver& show_me);




    class find_circle : public fdfsolver
    {

    private:
      point_type points;

    public:
      int f( const vector & x, vector & f );  
      int df( const vector & x, matrix & J );
      find_circle( point_type points, vector & init_vector  );
      virtual ~find_circle();
    }; /* class find_circle */


    // returns the circle-estimated derivative in x
    float circle_tangent( const vector & v_x, const vector & v_y, float x );
    

    // same as circle tangent, just for python
    float py_circle_tangent( const boost::python::list & py_x,
                             const boost::python::list & py_y,
                             float x
      );
    
  } /* namespace gsl */  
} /* end namespace pink */


#endif /*UI_CIRCLE_HPP_*/
/* LuM end of file */
