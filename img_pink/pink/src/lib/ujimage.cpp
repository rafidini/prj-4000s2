/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2009-2011
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/




#include "pink.h"
#include "mccodimage.h"

namespace pink{

  pink_image::operator xvimage*()
  {
    pink_error("You can only cast complete image objects. This is just the base class");    
    return NULL;
  } /* pink_image cast xvimage* */
  

  boost::shared_ptr<pink::types::vint> getDimensions( const index_t x, const index_t y, const index_t z, const index_t t )
  {

    boost::shared_ptr<pink::types::vint> presult;
    if (t>1) {
      /////!!!!!!! cout<< "I've desided for 4D." << std::endl;
      presult.reset(new pink::types::vint(4,-1));
      (*presult)[0]=x;
      (*presult)[1]=y;
      (*presult)[2]=z;
      (*presult)[3]=t;
    }
    else if (z>1)
    {
      /////!!!!!!! cout<< "I've desided for 3D." << std::endl;
      presult.reset(new pink::types::vint(3,-1));
      (*presult)[0]=x;
      (*presult)[1]=y;
      (*presult)[2]=z;
    }
    else if (y>1)
    {
      /////!!!!!!! cout<< "I've desided for 2D." << std::endl;
      presult.reset(new pink::types::vint(2,-1));
      (*presult)[0]=x;
      (*presult)[1]=y;
    }
    else if (x>1)
    {
      /////!!!!!!! cout<< "I've desided for 1D or less." << std::endl;
#     ifdef UJIMAGE_DEBUG
      std::cout << "x>1 and y==1; Here I assume 2D image of size [" << x << ", 1]" << std::endl;
#     endif /* UJIMAGE_DEBUG */
      presult.reset(new pink::types::vint(2,-1));
      (*presult)[0]=x;
      (*presult)[1]=1;      
    }
	
    if ((t>1) && (z==1))
    {
      pink_error("two dimensional time series are probably not handled well");
    };
    return presult;
  } /* getDimensions */


  void setDimensions(const pink::types::vint & dim, index_t & x, index_t & y, index_t & z, index_t & t)
  {
    index_t d = dim.size();
    bool result = true;
    x=y=z=t=1;
    switch (d)
    {
    case 1:
      x=dim[0];
      y=1;
      z=1;
      t=1;
      break;
    case 2:
      x=dim[0];
      y=dim[1];
      z=1;
      t=1;
      break;
    case 3:
      x=dim[0];
      y=dim[1];
      z=dim[2];
      t=1;
      break;
    case 4:
      x=dim[0];
      y=dim[1];
      z=dim[2];
      t=dim[3];
      break;
    default:
      result = false;
      pink_error("error: the dimension is wrong or unsupported");
      break;
    } /* switch */
  } /* setDimensions */

  std::string image_type_string( index_t im_type )
  {
    switch (im_type)
    {
    
    case VFF_TYP_1_BYTE:
      return "uint8_t"; /* pixels are byte (uint8_t) */
      break;
    
    case VFF_TYP_2_BYTE:
      return "uint16_t"; /* pixels are two byte (int16_t) */
      break;
    
    case VFF_TYP_4_BYTE:
      return "int32_t"; /* pixels are four byte (integer) */
      break;
    
    case VFF_TYP_FLOAT:
      return "float"; /* pixels are float (single precision)*/
      break;
    
    case VFF_TYP_DOUBLE:
      return "double"; /* pixels are float (double precision)*/
      break;

    default:
      pink_error("bad image type or not specified.");
    };

  } /* image_type */

  
/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


c++ class shallow_xvimage


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

  shallow_xvimage::shallow_xvimage()
  {
    this->name = NULL;
  } /* shallow_xvimage::shallow_xvimage */


  shallow_xvimage::shallow_xvimage( const shallow_xvimage & src )
  { 
    // Carefully, the image data is not copied. 
    // It will be created by 'ujoi::operato&'
    // for functions which use xvimage from python
    this->name = NULL;  
    image_data = NULL;   // the image data is never allocated and therefore destroyed !
    // it is only used for 'ujoi::operator&'
    this->row_size=src.row_size;
    this->col_size=src.col_size;
    this->depth_size=src.depth_size;
    this->time_size=src.time_size;
    this->num_data_bands=src.num_data_bands;
    this->data_storage_type=src.data_storage_type;
    this->xdim=src.xdim;
    this->ydim=src.ydim;
    this->zdim=src.zdim;
    this->xmin=src.xmin;
    this->xmax=src.xmax;
    this->ymin=src.ymin;
    this->ymax=src.ymax;
    this->zmin=src.zmin;
    this->zmax=src.zmax;
  } /* shallow_xvimage::shallow_xvimage */


  shallow_xvimage::shallow_xvimage( const xvimage & src )
  { 
    // Carefully, the image data is not copied. 
    // It will be created by 'ujoi::operato&'
    // for functions which use xvimage from python
    this->name = NULL; 
    image_data = NULL;   // the image data is never allocated and therefore destroyed !
    // it is only used for 'ujoi::operator&'
    this->row_size=src.row_size;
    this->col_size=src.col_size;
    this->depth_size=src.depth_size;
    this->time_size=src.time_size;
    this->num_data_bands=src.num_data_bands;
    this->data_storage_type=src.data_storage_type;
    this->xdim=src.xdim;
    this->ydim=src.ydim;
    this->zdim=src.zdim;
    this->xmin=src.xmin;
    this->xmax=src.xmax;
    this->ymin=src.ymin;
    this->ymax=src.ymax;
    this->zmin=src.zmin;
    this->zmax=src.zmax;
  } /* shallow_xvimage::shallow_xvimage */

  shallow_xvimage::shallow_xvimage( const pink::types::vint & dim, index_t int_im_type )
  { 
    // // Carefully, the image data  
    // will be created by 'ujoi::operator&'
    // for functions which use xvimage from python
    this->name = NULL; 
    image_data = NULL;   // the image data is never allocated and therefore destroyed !
    // it is only used for 'ujoi::operator&'

    index_t x,y,z,t;
    setDimensions( dim, x, y, z, t );
  
    this->row_size   = x;
    this->col_size   = y;
    this->depth_size = z;
    this->time_size  = t;
    this->num_data_bands    = 1;
    this->data_storage_type = int_im_type; /// !!!!!!!!
    this->xdim = -1;
    this->ydim = -1;
    this->zdim = -1;
    this->xmin = -1;
    this->xmax = -1;
    this->ymin = -1;
    this->ymax = -1;
    this->zmin = -1;
    this->zmax = -1;
  } /* shallow_xvimage::shallow_xvimage */


  shallow_xvimage::~shallow_xvimage()
  {
    ////// /// !!!!!!!!!! delete image->image_data; // this is a shallow container 
    // image data is not to be deleted!
    /////!!!!!!! cout<< "deleting shallow_xvimage" << std::endl;
    // the rest of the properties are destroyed automaticly
  } /* shallow_xvimage::~shallow_xvimage */

  std::string shallow_xvimage::imtype()
  {
    return image_type_string(this->data_storage_type);
  } /* shallow_xvimage::imtype */


/*
***************************************************************************************
***************************************************************************************
***************************************************************************************


c++ class pink::deep_xvimage


***************************************************************************************
***************************************************************************************
***************************************************************************************
*/

  deep_xvimage::deep_xvimage()
  { 
//!!!!!    this->data=image_data=reinterpret_cast<void*>( new char[1] );
  } /* deep_xvimage::deep_xvimage */


  deep_xvimage::deep_xvimage( const deep_xvimage & src )
  { 
    pink_error(
      "The 'deep_xvimage' copy constructor is not implemented. If you want to use it, "
      "you have to implement it. If it's called implicitly (that is to say not by you) "
      "then it's a bug and has to be corrected,"
      );
  } /* deep_xvimage::deep_xvimage */

  deep_xvimage::deep_xvimage( const xvimage & src )
  { 
    // Carefully, the image data is not copied. 
    // It will be created by 'ujoi::operator&'
    // for functions which use xvimage from python
//!!!!!    this->data = src;    
  } /* deep_xvimage::deep_xvimage */



  deep_xvimage::~deep_xvimage()
  {
//!!!!!    freeimage(this->data);                           
    /////!!!!!!! cout<< "deleting deep_xvimage" << std::endl;
    // the rest of the properties are destroyed automaticly
  } /* deep_xvimage::~deep_xvimage */
  
  std::string deep_xvimage::imtype()
  {
    return image_type_string(this->data->data_storage_type);
  } /* deep_xvimage::imtyp */
  


  template<>
  void write_a_pixel<float>
  (
    std::fstream & s, 
    float & value
    ) 
  {
    s.write( reinterpret_cast<char*>(&value), 4 );
  } /* write_a_pixel float */
  
  
  
  template<>
  void write_a_pixel<unsigned char>
  (
    std::fstream & s, 
    unsigned char & value
    ) 
  {
    s << value;
  } /* write_a_pixel char */


} /* namespace pink */



/*
***************************************************************************************
***************************************************************************************
***************************************************************************************

COMPLEX OPERATOR IMPLEMENTATIONS

***************************************************************************************
***************************************************************************************
***************************************************************************************
*/
  

bool operator==(const fcomplex & x, const fcomplex & y)
{
  return ( (x.re == y.re) && (x.im == y.im) );
}

fcomplex & operator+=(fcomplex & x, const fcomplex & y)
{
  x.re += y.re;
  x.im += y.im;
  return x;      
}

fcomplex & operator-=(fcomplex & x, const fcomplex & y)
{
  x.re -= y.re;
  x.im -= y.im;
  return x;      
}

fcomplex & operator*=(fcomplex & x, const fcomplex & y)
{
  x.re = x.re * y.re - x.im * y.im;      
  x.im = x.im * y.re + x.re * y.im;      
  return x;      
}

fcomplex & operator*=(fcomplex & x, const float & y)
{
  x.re = x.re * y;
  x.im = x.im * y;
  return x;  
}

fcomplex & operator/=(fcomplex & x, const fcomplex & y)
{
  float abs = y.re * y.re + y.im * y.im;
      
  if (abs < 0.00001)
  {
    pink_error("division by zero");        
  }
  
  x.re = (x.re * y.re + x.im * y.im) / (abs);
  x.im = (x.im * y.re + x.re * y.im) / (abs);      
  return x;      
}


bool operator!=(const fcomplex & x, const fcomplex & y)
{
  return !( x==y );      
}

fcomplex & operator+( const fcomplex & x, const fcomplex & y)
{
  fcomplex result(x);
  return result+=y;
}

fcomplex & operator-( const fcomplex & x, const fcomplex & y)
{
  fcomplex result(x);  
  return result-=y;        
}

fcomplex & operator*( const fcomplex & x, const fcomplex & y)
{
  fcomplex result(x);  
  return result*=y;        
}

fcomplex & operator*( const fcomplex & x, const float & y)
{
  fcomplex result(x);
  return result*=y;  
}

fcomplex & operator*( const float & x, const fcomplex & y)
{
  fcomplex result(y);
  return result*=x;  
}

fcomplex & operator/( const fcomplex & x, const fcomplex & y)
{
  fcomplex result(x);  
  return result/=y;        
}

bool operator>=( const fcomplex & x, const fcomplex & y)
{
  pink_error("There is no ordering on complex numbers.");
  return false;
}

bool operator<=( const fcomplex & x, const fcomplex & y)
{
  pink_error("There is no ordering on complex numbers.");
  return false;
}

bool operator>( const fcomplex & x, const fcomplex & y)
{
  pink_error("There is no ordering on complex numbers.");
  return false;
}

bool operator<( const fcomplex & x, const fcomplex & y)
{
  pink_error("There is no ordering on complex numbers.");
  return false;
}



std::string repr_fcomplex(const fcomplex & x)
{
  std::stringstream ss;
  if (x.im>=0)
  {        
    ss << x.re << " + i*" << x.im;        
  }
  else /* NOT x.im>0 */
  {
    ss << x.re << " - i*" << -x.im;
  } /* NOT x.im>0 */     
  
  return ss.str();      
} /* fcomplex::fcomplex */


bool operator==(const dcomplex & x, const dcomplex & y)
{
  return ( (x.re == y.re) && (x.im == y.im) );
}

dcomplex & operator+=(dcomplex & x, const dcomplex & y)
{
  x.re += y.re;
  x.im += y.im;
  return x;      
}

dcomplex & operator-=(dcomplex & x, const dcomplex & y)
{
  x.re -= y.re;
  x.im -= y.im;
  return x;      
}

dcomplex & operator*=(dcomplex & x, const dcomplex & y)
{
  x.re = x.re * y.re - x.im * y.im;      
  x.im = x.im * y.re + x.re * y.im;      
  return x;      
}

dcomplex & operator*=(dcomplex & x, const double & y)
{
  x.re = x.re * y;
  x.im = x.im * y;
  return x;  
}

dcomplex & operator/=(dcomplex & x, const dcomplex & y)
{
  float abs = y.re * y.re + y.im * y.im;
      
  if (abs < 0.00001)
  {
    pink_error("division by zero");        
  }
  
  x.re = (x.re * y.re + x.im * y.im) / (abs);
  x.im = (x.im * y.re + x.re * y.im) / (abs);      
  return x;      
}


bool operator!=(const dcomplex & x, const dcomplex & y)
{
  return !( x==y );      
}

dcomplex & operator+( const dcomplex & x, const dcomplex & y)
{
  dcomplex result(x);
  return result+=y;
}

dcomplex & operator-( const dcomplex & x, const dcomplex & y)
{
  dcomplex result(x);  
  return result-=y;        
}

dcomplex & operator*( const dcomplex & x, const dcomplex & y)
{
  dcomplex result(x);  
  return result*=y;        
}

dcomplex & operator*( const dcomplex & x, const double & y)
{
  dcomplex result(x);
  return result*=y;  
}

dcomplex & operator*( const double & x, const dcomplex & y)
{
  dcomplex result(y);
  return result*=x;  
}


dcomplex & operator/( const dcomplex & x, const dcomplex & y)
{
  dcomplex result(x);  
  return result/=y;        
}

bool operator>=( const dcomplex & x, const dcomplex & y)
{
  pink_error("There is no ordering on complex numbers.");
  return false;
}

bool operator<=( const dcomplex & x, const dcomplex & y)
{
  pink_error("There is no ordering on complex numbers.");
  return false;
}

bool operator>( const dcomplex & x, const dcomplex & y)
{
  pink_error("There is no ordering on complex numbers.");
  return false;
}

bool operator<( const dcomplex & x, const dcomplex & y)
{
  pink_error("There is no ordering on complex numbers.");
  return false;
}

std::string repr_dcomplex(const dcomplex & x)
{
  std::stringstream ss;
  if (x.im>=0)
  {        
    ss << x.re << " + i*" << x.im;        
  }
  else /* NOT x.im>0 */
  {
    ss << x.re << " - i*" << -x.im;
  } /* NOT x.im>0 */     
  
  return ss.str();      
} /* dcomplex::dcomplex */



































// LuM end of file
