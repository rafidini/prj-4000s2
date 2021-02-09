/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2010-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#ifndef UI_UNGERFLOW__
#define UI_UNGERFLOW__

#include "uiFlow.hpp"
#include "uiFrame.hpp"

#define REPORT_INTERVAL 10

namespace pink { 

  template<class image_type>
  class ungerflow : public max_flow<image_type> {
    
  public:
    typedef typename image_type::pixel_type pixel_type;
        
    ungerflow(
      const char_image & src_sink, // the source (1) and the sink (-1). It must be -1 around
      const image_type & gg, // the gradient image
      const image_type & lambda, 
      const image_type & guidence_f,
      int iteration,
      float tau,
      float theta
      );

    ~ungerflow()
      {
#       ifdef UJIMAGE_DEBUG
	std::cerr << "destroying the ungerflow object (" << static_cast<void*>(this) << ")\n";	
#       endif /* UJIMAGE_DEBUG */        
      };
    
    image_type start();    
    void upDateThreshold();
    
  private:
    float theta;
    image_type dual_potencial;
    image_type lambda;
    image_type guidence_f;

  }; /* ungerflow */

   
  template <class image_type>
  image_type lungerflow(
    const char_image & src_sink, // the source (1) and the sink (-1). It must be -1 around
    const image_type & gg, // the gradient image
    const image_type & lambda, 
    const image_type & guidence_f,
    int iteration,
    float tau,
    float theta
    )
  {
    ungerflow<image_type> ungerflow_obj(
      frame_around(src_sink, -1),
      frame_around(gg, 0),
      frame_around(lambda, 0),
      frame_around(guidence_f, 0),
      iteration,
      tau,
      theta
      );
    
    image_type result = frame_remove(ungerflow_obj.start());

    return result;
  } /* ungerflow */
  



  template<class image_type>
  ungerflow<image_type>::ungerflow(
    const char_image & src_sink, // the source (1) and the sink (-1). It must be -1 around
    const image_type & gg, // the gradient image
    const image_type & lambda, 
    const image_type & guidence_f,
    int iteration,
    float tau,
    float theta
    ) : max_flow<image_type>( src_sink, gg, iteration, tau/* /theta */ , 1, /* packet_size = */ 1000, /* verbose = */ false)
  {
#   ifdef UJIMAGE_DEBUG
    std::cerr << "creating the ungerflow object (" << static_cast<void*>(this) << ")\n";	
#   endif /* UJIMAGE_DEBUG */        

    this->theta = theta;
    this->guidence_f = guidence_f.clone();
    this->lambda = lambda.clone();
    this->dual_potencial = gg.clone(); //"dual_potencial";
  } /* ungerflow::ungerflow */
  
  
  
  template<class image_type>
  image_type ungerflow<image_type>::start()
  {
    this->sentinel.maxPos(this->iteration);
    this->sentinel.minPos(0);
    this->sentinel << 0;
    this->sentinel.start();
    std::cout << "starting the iteration\n";
    

    FOR( e, this->iteration ) {    

      if ( e % REPORT_INTERVAL == 0 ) 
      {
      	if ( this->sentinel.timeToReport() )
      	{
      	  std::cout << "Estimated time remaining: " << (this->sentinel << e) << std::endl;
      	} /* timeToReport() */
      } /* if iterations ... */
//      _DEBUG(e);

      this->upDateSrcSink();
      
      // FOR(r, 10)
      // {
        
      
      FOR( par, this->dibPotencial.size() ) 
      {
        this->upDatePotencial( par, par + 1 ); /// for all the dibbles update the potencial on dible par 
        /// (from par to par+1 not included)
      } /* FOR */
        
        
        
      FOR( w, this->d ){
        FOR( par, this->dibFlow[w].size() )
        {
          this->upDateFlow( par, par + 1, w );
        } /* FOR */
      } /* FOR(w,d) */ 
        
        
      FOR( par, this->dibConstrain.size() )
      {
        this->upDateConstrain( par, par + 1 );
      } /* FOR */
        
//      } /* FOR r */
      
      
      this->upDateThreshold();
      
      this->potencial.swap(dual_potencial); /// !!!!

    } /* FOR(e, iteration) */

    this->sentinel.stop();
    std::cout << "total time of iteration: " << this->sentinel.elapsedTime() << std::endl;


    return this->potencial;
    

  } /* ungerflow::start */
  


  template <class image_type>
  void ungerflow<image_type>::upDateThreshold(void)
  {
    FOR(q, this->length_glob)
    {
      if ( this->potencial(q) - guidence_f(q) > lambda(q) * theta )
      {
	dual_potencial(q) = this->potencial(q) - lambda(q) * theta;	
      }
      else /* NOT (*potencial)(q) - (*guidence_f)(q) > (*lambda)(q) * theta */
      {
	if ( this->potencial(q) - guidence_f(q) < - lambda(q) * theta )
	{
	  dual_potencial(q) = this->potencial(q) + lambda(q) * theta;
	}
	else /* NOT (*potencial)(q) - (*guidence_f)(q) < - (*lambda)(q) * theta => |u-f| < lambda * theta */	
	{
	  dual_potencial(q) = guidence_f(q);
	} /* NOT (*potencial)(q) - (*guidence_f)(q) < - (*lambda)(q) * theta */
      } /* NOT (*potencial)(q) - (*guidence_f)(q) > (*lambda)(q) * theta */
    } /* FOR */
    
  } /* ungerflow::upDateThreshold */
} /* namespace pink */

// cleaning up after us
#undef REPORT_INTERVAL


#endif /* UI_UNGERFLOW__ */
/* LuM end of file */
