/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_FIT_CIRCLE_HPP_
#define UI_FIT_CIRCLE_HPP_

const double epsilon=0.001;

#ifdef PINK_HAVE_PYTHON
#  ifdef _WINDOWS
#    define BOOST_PYTHON_STATIC_LIB
#  endif /* _WINDOWS */
#  include <boost/python.hpp>
#endif /* PINK_HAVE_PYTHON */

extern "C"
{
  // this is the lapack generalized eigenvalue calculation function
  void dggev_(const char*, const char*, const int*,
	     const double*, const int*, const double*, const int*,
	     double*, double*, double*,
	     double*, const int*, double*, const int*,
	     double*, const int*, int*);

  // void dggev_(const char* JOBVL, const char* JOBVR, const int* N,
  //             const double* A, const int* LDA, const double* B, const int* LDB,
  //             double* ALPHAR, double* ALPHAI, double* BETA,
  //             double* VL, const int* LDVL, double* VR, const int* LDVR,
  //             double* WORK, const int* LWORK, int* INFO);

} /* extern "C" */


namespace pink
{

  int outer_stride( const Eigen::MatrixXd & );
  bool generalized_eigenvalue(const Eigen::MatrixXd&,
                              const Eigen::MatrixXd&,
                              Eigen::MatrixXd&,
                              Eigen::MatrixXd&
    );
  Eigen::VectorXd fit_circle( const Eigen::VectorXd &, const Eigen::VectorXd & );

# ifdef PINK_HAVE_PYTHON
  // the same as 'fit_circle' but with python conversion
  boost::python::list py_fit_circle(
    const boost::python::list & py_x,
    const boost::python::list & py_y,
    const std::string & filename = ""
    );


  boost::python::list py_circle_equation_to_coordinates(
    const boost::python::list & equation
    );

  boost::python::list py_circle_equation_to_coordinates(
    const boost::python::list & equation
    );
# endif /* PINK_HAVE_PYTHON */
} /* end namespace pink */


#endif /*UI_FIT_CIRCLE_HPP_*/
/* LuM end of file */
