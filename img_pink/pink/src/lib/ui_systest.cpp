/*
  This software is licensed under
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but
  without any warranty to the extent permitted by applicable law.

  (C) UjoImro, 2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#define BOOST_DISABLE_ASSERTS
#include <boost/date_time/local_time/local_time.hpp>

namespace pink
{
  namespace benchmark
  {

    double now()
    {
      boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
      boost::posix_time::time_duration diff = now - boost::posix_time::ptime(boost::gregorian::date(1970,1,1));

      long microseconds = diff.total_microseconds() % 1000000;
      double duration = double(diff.total_seconds()) + double(microseconds) / 1000000.;

      return duration;
      
//       struct timeval tp;
//       double sec, usec, result;
// // Time stamp before the computations
//       gettimeofday( &tp, NULL );
//       sec    = static_cast<double>( tp.tv_sec );
//       usec   = static_cast<double>( tp.tv_usec )/1E6;
//       result = sec + usec;
//       return result;
    } /* now */




    
  } /* namespace benchmark */
} /* namespace pink */


// LuM end of file
