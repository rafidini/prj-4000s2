/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/


#include <gsl/gsl_vector.h>
// my includes
#include "uiCircle.hpp"


namespace pink
{ 

// inline double fabs( double x )
// {
//   return x>0?x:-x;  
// }
  

  namespace gsl
  {

    const float epsilon = 0.0001;
    const double pi = M_PI;    
    
    inline double SQR( double x )
    {
      return x * x;  
    }


    
    pink::gsl::vector::vector(int n) // initializes a vector of size n
    {
      wrapper=false;      
      self=gsl_vector_alloc(n);
    }
    
    pink::gsl::vector::vector(const vector & other) // copy constructor
    {
      wrapper=false;
      self=gsl_vector_alloc(other.size());
        
      if (GSL_SUCCESS != gsl_vector_memcpy(this->self, other.self))
      {
        pink_error("vector_memcpy failed!");          
      }
        
    }
    
    pink::gsl::vector::vector( const gsl_vector * shallow ) // this initializes vector in wrapper mode
    {
      wrapper=true;
      self=const_cast<gsl_vector *>(shallow);
    }
    
    pink::gsl::vector & pink::gsl::vector::operator=(const vector & other) // copy constructor
    {
      if (self)
      {
        gsl_vector_free(self);
        self=NULL;        
      }
        
      self=gsl_vector_alloc(other.size());      
        
      gsl_vector_memcpy(this->self, other.self);
      return *this;      
    }
    
    pink::gsl::vector::vector() // empty constructor    
    {
      wrapper=false;
      self=NULL;      
    }
    
    
    int pink::gsl::vector::size() const
    {
      return self->size;      
    }
    
    void pink::gsl::vector::set_all(double val) // sets all elements of the vector the given value
    {
      gsl_vector_set_all(self, val);
        
    }
    
    double & pink::gsl::vector::operator[](int pos) // vector element access
    {
      return *gsl_vector_ptr(this->self, pos);
        
    }
    
    const double & pink::gsl::vector::operator[](int pos) const // vector element access
    {
      return *gsl_vector_ptr(this->self, pos);
    }
    
    
    pink::gsl::vector::~vector() // default destructor
    {
      if (! wrapper) // we do not delete pointers in wrapper mode
      {  
        if (self)
        {        
          gsl_vector_free(self);
          self=NULL;            
        }
      }
    }
    
    pink::gsl::vector::operator gsl_vector*() // type casting operator
    {
      return this->self;      
    }
    
    pink::gsl::vector & pink::gsl::vector::operator << ( double first_value )
    {
      tag=0;
      (*this)[tag]=first_value;
      return *this;      
    }
    
    pink::gsl::vector & pink::gsl::vector::operator, (double next)
    {
      tag++;
      (*this)[tag]=next;
      return *this;
    }

    std::string pink::gsl::vector::repr() const
    {
      std::stringstream result;
      result << "[ ";
      for (int q=0; q<=this->size()-1  -1; q++)
      {
        result << (*this)[q] << ", ";        
      }
      result << (*this)[this->size()-1] << " ]";

      return result.str();      
    }
  

    float dist(const vector & A, const vector & B)
    {
      float sum=0;    
      for (int q=0; q<=A.size()-1; q++)
      {
        sum+=gsl::SQR(A[q]-B[q]);
      }
      return sqrt(sum);
    
    }
  
  

  
    matrix::matrix(int row, int column) // initializes a matrix of size n
    {
      wrapper=false;      
      self=gsl_matrix_alloc(row, column);
    }

    matrix::matrix(const matrix & other) // copy constructor
    {
      wrapper=false;
      if (self)
      {
        gsl_matrix_free(self);
        self=NULL;        
      }

      self=gsl_matrix_alloc(other.rows(), other.columns());
      
      gsl_matrix_memcpy(this->self, other.self);
    }

    matrix::matrix( const gsl_matrix * shallow ) // this initializes matrix in wrapper mode
    {
      wrapper=true;
      self=const_cast<gsl_matrix *>(shallow);
    }

    matrix & matrix::operator=(const matrix & other) // copy constructor
    {
      if (self)
      {
        gsl_matrix_free(self);
        self=NULL;        
      }
      
      self=gsl_matrix_alloc(other.rows(), other.columns());      
      
      gsl_matrix_memcpy(this->self, other.self);
      return *this;      
    }

    matrix::matrix() // empty constructor    
    {
      wrapper=false;
      self=NULL;      
    }
  
  
    int matrix::rows() const
    {
      return self->size1;      
    }

    int matrix::columns() const
    {
      return self->size2;
    }
  
    void matrix::set_all(double val) // sets all elements of the matrix the given value
    {
      gsl_matrix_set_all(self, val);
    }

    double & matrix::operator()(int row, int column) // matrix element access
    {
      return *gsl_matrix_ptr(this->self, row, column);      
    }

    const double & matrix::operator()(int row, int column) const // matrix element access
    {
      return *gsl_matrix_ptr(this->self, row, column);
    }
  
    matrix::~matrix() // default destructor
    {
      if (! wrapper) // we do not delete pointers in wrapper mode
      {  
        if (self)
        {        
          gsl_matrix_free(self);
          self=NULL;            
        }
      }
    }

    matrix::operator gsl_matrix*() // type casting operator
    {
      return this->self;      
    }

    std::string matrix::repr() const
    {
      std::stringstream result;
      result << "[ ";
      for (int q=0; q<=this->rows() - 1; q++)
      {
        for (int w=0; w<=this->columns() - 1; w++ )
        {
          result << (*this)(q,w) << ", ";
        }
        result << "\n";        
      }
      result << /*(*this)[this->size()-1] <<*/ " ]";        

      return result.str();      
    }
  
  
    // this function should store the vector result f(x,params) in f for argument x and
    // arbitrary parameters params, returning an appropriate error code if the function
    // cannot be computed.
    int fdfsolver::gsl_f(const gsl_vector * x, void * params, gsl_vector * f)
    {
      fdfsolver * self;
      self = reinterpret_cast<fdfsolver*>(params);
      vector v_x(x);
      vector v_f(f);        
      return self->f(v_x, v_f);
    }
    
      
    // this function should store the n-by-p matrix result J_ij = d f_i(x,params) / d x_j in J
    // for argument x and arbitrary parameters params, returning an appropriate error
    // code if the function cannot be computed.
    int fdfsolver::gsl_df(const gsl_vector * x, void * params, gsl_matrix * J)
    {
      fdfsolver * self;
      self = reinterpret_cast<fdfsolver*>(params);
      vector v_x(x);
      matrix m_J(J);        
      return self->df(v_x, m_J);
    }
    
    // This function should set the values of the f and J as above, for arguments x and
    // arbitrary parameters params. This function provides an optimization of the
    // separate functions for f(x) and J(x)—it is always faster to compute the function
    // and its derivative at the same time. 
    int fdfsolver::gsl_fdf(const gsl_vector * x, void * params, gsl_vector * f, gsl_matrix * J)
    {
      fdfsolver * self;
      self = reinterpret_cast<fdfsolver*>(params);
      vector v_x(x);
      vector v_f(f);
      matrix m_J(J);        
      return self->fdf(v_x, v_f, m_J);
    }

    int fdfsolver::init(vector & init_vector)
    {
      // cannot be called until after the constructor
      return gsl_multifit_fdfsolver_set( solver, function, init_vector );
    }
    
    
    fdfsolver::fdfsolver( const gsl_multifit_fdfsolver_type * type,
                          int n /*observations*/,
                          int p /* number of parameters */
      ): numb_iterations(0)
    {
      solver = gsl_multifit_fdfsolver_alloc( type, n, p );
        
      function = new gsl_multifit_function_fdf;
      function->f=&fdfsolver::gsl_f;
      function->df=&fdfsolver::gsl_df;
      function->fdf=&fdfsolver::gsl_fdf;
      function->n=n;
      function->p=p;
      function->params=this;        

    }

    
    fdfsolver::~fdfsolver()
    {
      delete function;
      gsl_multifit_fdfsolver_free(solver);        
    }
    
    std::string fdfsolver::repr() const
    {
      std::string result(gsl_multifit_fdfsolver_name(solver));
      return result;        
    }   
    
    int fdfsolver::fdf( const vector & x, vector & f, matrix & J)
    {
      int report_f;
      int report_df;

      report_f=this->f(x,f);

      if (report_f!=GSL_SUCCESS)
      {
        return report_f;          
      }

      report_df=this->df(x,J);

      if (report_df!=GSL_SUCCESS)
      {
        return report_df;
      }
        
      return GSL_SUCCESS;        
    }

    int fdfsolver::iterate()
    {
      numb_iterations++;        
      return gsl_multifit_fdfsolver_iterate(this->solver);        
    }

    int fdfsolver::iterate( int nval )
    {
      int it;        
      for (int q=0; q<=nval-1; q++ )
      {
        numb_iterations++;          
        it=gsl_multifit_fdfsolver_iterate(this->solver);
          
        if (GSL_SUCCESS!=it)
        {
          return it;              
        }          
      }
      return GSL_SUCCESS;        
    }

    int fdfsolver::auto_iterate( float varepsilon, int nval )
    // this function iterates until epsilon precision is reached
    // n is the number of iterations between testing the convergence      
    {
      float difference;
      vector now, before;
        
      do
      {          
        this->iterate(nval);
        before = this->position();
          
        this->iterate();
        now = this->position();
          
      } while (dist(now, before) > epsilon);

      return 1;
   }
    
    
    vector fdfsolver::position()
    {
      vector result;
      result = vector(gsl_multifit_fdfsolver_position(this->solver));        
      return result;        
    }
    
    int fdfsolver::iterations()
    {
      return numb_iterations;        
    }
    
// teaching cout to print vector
    std::ostream& operator<<(std::ostream& output, const pink::gsl::vector& show_me)
    {
      output << show_me.repr();
      return output;  // for multiple << operators.
    }


// teaching cout to print vector
    std::ostream& operator<<(std::ostream& output, const pink::gsl::matrix& show_me)
    {
      output << show_me.repr();
      return output;  // for multiple << operators.
    }

// teaching cout to print fdfsolver
    std::ostream& operator<<(std::ostream& output, const pink::gsl::fdfsolver& show_me)
    {
      output << show_me.repr();
      return output;  // for multiple << operators.
    }

  
    typedef std::pair<pink::gsl::vector, pink::gsl::vector> point_type;


    int find_circle::f( const gsl::vector & x, gsl::vector & f )
    {
      
      double p_x = x[0];
      double p_y = x[1];
      double r = x[2];

      double sum=0;
                
      for (int q = 0; q<=points.first.size()-1; q++)
      {
        sum=0;          
        // the distance is: sqrt((points[q].x-x)^2+(points[q].y-y)^2) - r
        f[q] =
          fabs(
            sqrt (
              SQR(points.first[q]-p_x) + SQR(points.second[q]-p_y)
              )
            - r
            );
          
        sum += SQR(f[q]);
          
      }

      return GSL_SUCCESS;       
    }
  
  
    int find_circle::df( const gsl::vector & x, matrix & J )
    {
      double p_x = x[0];
      double p_y = x[1];
      double r = x[2];
      
      for (int q = 0; q <= points.first.size()-1; q++)
      { 
        // x coordinates
        J(q,0) =
          (p_x - points.first[q]) / 
          sqrt (
            SQR(p_x - points.first[q]) + SQR(p_y - points.second[q])
            );
        
        // y coordinates
        J(q,1) =
          (p_y - points.second[q]) / 
          sqrt (
            SQR(p_x - points.first[q]) + SQR(p_y - points.second[q])
            );
        
        // r derivative
        J(q,2) = -1;    
      }
      
      return GSL_SUCCESS;
    }
  
    find_circle::find_circle( point_type points, gsl::vector & init_vector  )
      :fdfsolver(gsl_multifit_fdfsolver_lmsder, points.first.size(), 3)
    {      
      this->points=points;
      this->init(init_vector);      
    }

    find_circle::~find_circle()
    {
      // intentionally left empty
    }
  

    float py_circle_tangent( const boost::python::list & py_x,
                             const boost::python::list & py_y,
                             float x
      )
    {
      int len_x, len_y;
      len_x=boost::python::len(py_x);
      len_y=boost::python::len(py_y);

      if (len_x!=len_y)
      {
        pink_error("py_circle_tangent: the number of 'x' and 'y' coordinates must be equal.");        
      }
      
      
      vector p_x(len_x);      
      vector p_y(len_y);

      for (int q=0; q<=len_x-1; q++)
      {
        p_x[q]=boost::python::extract<float>(py_x[q]);
        p_y[q]=boost::python::extract<float>(py_y[q]);
      }
      
      return circle_tangent(p_x,p_y,x);
      
    }
    
    

// returns the circle-estimated derivative in x
    float circle_tangent( const gsl::vector & v_x, const gsl::vector & v_y, float x )
    {
      point_type points;
      points.first=v_x;
      points.second=v_y;

      gsl::vector start(3);
      start << 0.,0.,0.;

      find_circle solver(points, start);
      solver.auto_iterate();

      gsl::vector solution = solver.position();

//      cout << "found center ____________________________ " << solution << "\n";
//      cout << "requested point is " << x << "\n";
      

      float center_x=solution[0];
      float center_y=solution[1];
      float ray=solution[2];

      float cosine = (x-center_x) / ray;

//      _DEBUG(x-center_x);
//      _DEBUG(ray);
//      _DEBUG((x-center_x) / ray);
//      _DEBUG(acos(cosine));
      
        
      
      return acos(cosine);  
    }


    

    // point_type generate_circle( double c_x, double c_y, double r)
    // {

    //   point_type result;  
  
    //   // x = c_x + r * sin(alpha)
    //   // y = c_y + r * cos(alpha)
  
    //   gsl::vector x(N);
    //   gsl::vector y(N);
    
    //   for (int alpha=0; alpha<=N-1; alpha++ )
    //   {
    //     x[alpha]=c_x + r * sin(4*alpha*pi/180);    
    //     y[alpha]=c_y + r * cos(4*alpha*pi/180);
    //   }
  
    //   result.first=x;
    //   result.second=y;

    //   return result;  
    // } /* generate_circle */


  } /* namespace gsl */
} /* namespace pink */


/* LuM end of file */
