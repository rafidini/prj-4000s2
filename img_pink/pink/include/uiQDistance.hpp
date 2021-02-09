/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

// This function calculates the 'distance' of two images. Typically if you corrupt an
// image with noise, and you try to reconstruct it, than this function can act
// as the extent of reconstruction.

#ifndef UI_QDISTANCE_HPP_
#define UI_QDISTANCE_HPP_

#define MEAN boost::accumulators::tag::mean
#define VARIANCE boost::accumulators::tag::variance
#define COVARIANCE boost::accumulators::tag::covariance<pixel_type, boost::accumulators::tag::covariate1>
#define accumulator_ boost::accumulators::accumulator_set
#define stats_ boost::accumulators::stats
//#define tag boost::accumulators::stats::tag
#define mean_ boost::accumulators::mean
#define variance_ boost::accumulators::variance
#define covariance_ boost::accumulators::covariance
#define covariate1_ boost::accumulators::covariate1

namespace pink { 

  template <class image_type>
  double lQDistance( const image_type & imageA,
                     const image_type & imageB
    )
  {

    typedef typename image_type::pixel_type pixel_type;

    
    
    // statistics on the image A
    accumulator_< pixel_type, stats_< MEAN, VARIANCE, COVARIANCE  >  > stat_A;

    FOR(q, imageA.get_size().prod())
    {
      stat_A( imageA[q], covariate1_ = imageB[q] );
      
    }    /* FOR */
    

    double variance_A = variance_( stat_A );
    DEBUG(variance_A);
    double deviation_A = sqrt( variance_A );
    DEBUG(deviation_A);
    double mean_A = mean_( stat_A );
    DEBUG(mean_A);
    double sq_mean_A = mean_A * mean_A;    
    DEBUG(sq_mean_A);
    double covariance_AB = covariance_( stat_A );
    DEBUG(covariance_AB);


    // statistics on the image B
    
    
    accumulator_< pixel_type, stats_<MEAN, VARIANCE> > stat_B;

    stat_B = std::for_each( &(imageB[0]), &(imageB[ imageB.get_size().prod() ]), stat_B );
    
    double variance_B = variance_( stat_B );
    DEBUG(variance_B);
    double deviation_B = sqrt( variance_B );
    DEBUG(deviation_B);
    double mean_B = mean_( stat_B );
    DEBUG(mean_B);
    double sq_mean_B = mean_B * mean_B;
    DEBUG(sq_mean_B);
    //double correlation_AB = covariance_AB / ( deviation_A * deviation_B );
    //_DEBUG(correlation_AB);
    
    double result =
      ( 4 * covariance_AB * mean_A * mean_B )
      /
      ( ( variance_A + variance_B ) * ( sq_mean_A + sq_mean_B ) );


    return result;
    
    
  }  /*lQDistance*/



































  } /* namespace pink */


#endif /* UI_QDISTANCE_HPP_ */
/* LuM end of file */
