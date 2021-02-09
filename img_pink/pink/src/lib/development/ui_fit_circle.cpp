/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This is an implementation of optimal ellipse fitting algorithm
// PoSh

#include <eigen2/Eigen/Core>
#include <eigen2/Eigen/LU>
#include <eigen2/Eigen/Geometry>

#include <fstream>

#ifdef PINK_HAVE_PYTHON
#  ifdef _WINDOWS
#    define BOOST_PYTHON_STATIC_LIB
#  endif /* _WINDOWS */
#  include <boost/python.hpp>
#endif /* PINK_HAVE_PYTHON */

#include "ui_pink_types.hpp"
#include "ui_fit_circle.hpp"

USING_PART_OF_NAMESPACE_EIGEN

namespace pink
{

  inline float ui_sqr( float x )
  {
    return x*x;    
  }
  
  // gives the leading dimension (the size of the longer side) of the matrix
  int outer_stride( const MatrixXd & mat )
  {
    int cols = mat.cols();
    int rows = mat.rows();
    return cols>rows?cols:rows;
  }
  
  // Generalised Eigen-Problem
  // Solve:
  // A * v(j) = lambda(j) * B * v(j).
  //
  // v are the eigenvectors and are stored in v.
  // lambda are the eigenvalues and are stored in lambda.
  // The eigenvalues are stored as: (lambda(:, 1) + lambda(:, 2)*i)./lambda(:, 3)
  //
  // returns true on success.
  bool generalized_eigenvalue(const MatrixXd& A, const MatrixXd& B, MatrixXd& v, MatrixXd& lambda)
  {
    int Nval = A.cols(); // Number of columns of A and B. Number of rows of v.
    if (B.cols() != Nval  || A.rows()!=Nval || B.rows()!=Nval)
      return false;

    v.resize(Nval,Nval);
    lambda.resize(Nval, 3);
    
    int LDA = outer_stride(A);
    int LDB = outer_stride(B);
    int LDV = outer_stride(v);
    
    double WORKDUMMY;
    int LWORK = -1; // Request optimum work size.
    int INFO = 0;
    
    double * alphar = const_cast<double*>(lambda.col(0).data());
    double * alphai = const_cast<double*>(lambda.col(1).data());
    double * beta   = const_cast<double*>(lambda.col(2).data());
    
    // Get the optimum work size.
    dggev_("N", "V", &Nval, A.data(), &LDA, B.data(), &LDB, alphar, alphai, beta, 0, &LDV, v.data(), &LDV, &WORKDUMMY, &LWORK, &INFO);
    
    LWORK = int(WORKDUMMY) + 32;
    VectorXd WORK(LWORK);
    
    dggev_("N", "V", &Nval, A.data(), &LDA, B.data(), &LDB, alphar, alphai, beta, 0, &LDV, v.data(), &LDV, WORK.data(), &LWORK, &INFO);
    
    return INFO==0;
  } /* generalized_eigenvalue */
  
  VectorXd fit_circle( const VectorXd & x, const VectorXd & y, const std::string & filename = "" )
  {
    bool verbose = (filename != "");

    if (x.size()!=y.size())
    {
      pink_error("fit_circle: the size of the x and y coordinate vector must match!");      
    }

    if (x.size()<=4)
    {
      pink_error("fit_circle: You need at least four points to interpolate with a circle!");      
    }
    

    
    // Build desing matrix
    MatrixXd Dmat(x.size(),4);
    Dmat << x.cwise()*x + y.cwise()*y, x, y, VectorXd(x.size()).setOnes();

    // Build scatter matrix
    MatrixXd Smat;
    Smat = Dmat.transpose()*Dmat;

    // Build 4x4 constraint matrix
    MatrixXd Cmat(4,4);
    Cmat.setZero();    
    Cmat(1,1)=1;
    Cmat(2,2)=1;
    Cmat(3,0)=-2;
    Cmat(0,3)=-2;

    // Solve generalized eigensystem
    MatrixXd gevec, geval;    
    generalized_eigenvalue(Smat, Cmat, gevec, geval);

    double max = 1000000000000; 
    double max_q = -1;    
        
    FOR (q, geval.rows())
    {
      if (fabs(geval(q,2)) > epsilon) // checking if the regularisation constant is good
                                      // e.g. we are not in infinity
      {
        if (fabs(geval(q,0)/geval(q,2)) < max)
        {
          max_q=q;
          max = fabs(geval(q,0)/geval(q,2));
        } /* if geval... */
      } /* if fabs... */      
    } /* FOR */
    
    VectorXd result(geval.rows());

    FOR (q, geval.rows())
    {
      result[q]=gevec(q,max_q);      
    } /* FOR */
    
    if (verbose)
    {
      std::fstream mathematica;
      mathematica.open( filename.c_str(), std::fstream::out);

      mathematica << "Show[\n";  
      mathematica << "Graphics[Point[{\n";  
      FOR (q, x.size()-1)
      {
        mathematica << "{" << x[q] << "," << y[q] << "},";    
      } /* FOR */
      mathematica << "{" << x[x.size()-1] << "," << y[y.size()-1] << "}\n";
      mathematica << "}]\n]\n";  
      mathematica << ", \nContourPlot[\n" 
                  << "{(" << result[0] << ") (x^2+y^2)+(" << result[1] << ") x+("
                  << result[2] << ") y+(" << result[3] << ")==0},\n"
                  << "{x, -40, 40},{y, -40, 40},PlotPoints -> 100\n]\n";
  
      mathematica << "]\n";  
      mathematica.close();  
      
      // printing the circle candidates
      FOR (q, geval.rows())
      {     
        std::cout << "the circle equation is "
             << "( x" << std::showpos << gevec(1,q)/(2*gevec(0,q)) << " )^2 + ( y" << std::showpos << gevec(2,q)/(2*gevec(0,q)) << " )^2"
             << std::showpos << gevec(3,q)/gevec(0,q)-ui_sqr(gevec(1,q))/(4*ui_sqr(gevec(0,q)))-ui_sqr(gevec(2,q))/(4*ui_sqr(gevec(0,q))) << " == 0\n";  
      } /* FOR */
    } /* if */
    
    return result;    
  } /* fit_circle */

# ifdef PINK_HAVE_PYTHON

  boost::python::list py_fit_circle(
    const boost::python::list & py_x,
    const boost::python::list & py_y,
    const std::string & filename /* = "" */ // default argument specified in the header
    )
  {
    VectorXd x(boost::python::len(py_x));
    VectorXd y(boost::python::len(py_y));
    FOR(q, boost::python::len(py_x))
    {
      x[q]=boost::python::extract<float>(py_x[q]);
      y[q]=boost::python::extract<float>(py_y[q]);
    } /* FOR */

    VectorXd circle=fit_circle(x, y, filename);
    boost::python::list result;

    FOR(q, circle.size())
    {
      result.append(circle[q]);     
    }

    return result;
  } /* py_fit_circle */

  boost::python::list py_circle_equation_to_coordinates(
    const boost::python::list & equation
    )
  {
    boost::python::list result;

    float a = boost::python::extract<float>(equation[0]);
    float b = boost::python::extract<float>(equation[1]);
    float c = boost::python::extract<float>(equation[2]);
    float d = boost::python::extract<float>(equation[3]);
      
    
    float center_x = b/(2*a);
    float center_y = c/(2*a);
    float r = d/a - (b*b) / (4*(a*a)) - (c*c)/(4*(a*a));

    result.append(-center_x);
    result.append(-center_y);
    result.append(sqrt(-r));

    return result;    
  } /* py_circle_equation_to_coordinates */
  
# endif /* PINK_HAVE_PYTHON */

  
} /* namespace pink */

/* LuM end of file */
