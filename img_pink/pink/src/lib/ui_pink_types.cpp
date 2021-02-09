/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2007-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/

#include <cmath>

#include "ui_pink_types.hpp"

// progress_bar ---------------------------------------------------------------

pink::types::progress_bar::progress_bar(){ 
  max=100;
  min=0; 
  pos=0; 
  started=false;
  measure=false;
} /* pink::types::progress_bar */


pink::types::progress_bar::~progress_bar(){
  // intentionally left empty
} /* ~progress_bar */

bool pink::types::progress_bar::timeToReport(){
  return ( time(NULL) - last_report ) >= PBAR_REPORT_INTERVALL;
} /* timeToReport */

//converting a time in seconds to std::string. It converts up to days
std::string pink::types::progress_bar::time2string(time_t seconds){
  time_t day = seconds / 86400;
  seconds = seconds % 86400;
  time_t hour = seconds / 3600;
  hour = hour % 3600;
  time_t minute = seconds / 60;
  minute = minute % 60;
  seconds = seconds % 60;

  std::stringstream ss;
  if (day == 1){
    ss << "1 day ";
  } else if (day>1){
    ss << day << " days ";
  }
  ss.fill('0');
  ss.width(2);
  ss << hour << "h:" ;
  
  ss.fill('0');
  ss.width(2);
  ss << minute << "m:";
  
  ss.fill('0');
  ss.width(2);
  ss << seconds << "s";
    
  return ss.str();
}

void pink::types::progress_bar::start() {
  measure = true;
  started = true;
  begin   = time(NULL);
  last_report = begin;
}


void pink::types::progress_bar::stop(){
  finish  = time(NULL);
  measure = false;
}


void pink::types::progress_bar::maxPos(index_t maxPos){
  max = maxPos;
}


void pink::types::progress_bar::minPos(index_t minPos){
  min = minPos;
}


std::string pink::types::progress_bar::elapsedTime(){
  if (started) {
    if (measure){
      return time2string(time(NULL) - begin);
    } else {
      return time2string(finish - begin);
    }
  } else {
    return "progress_bar hasn't been started";
  }  
}

index_t pink::types::progress_bar::elapsedSeconds(){
  if (started) {
    if (measure){
      return time(NULL) - begin;
    } else {
      return finish - begin;
    }
  } else {
    return -1;
  }  
}

std::string pink::types::progress_bar::remainingTime(){
  if (started) {
    if (measure) {
      index_t currtime = time(NULL);

      if (pos==min) return "undetermined";
	else {
	  index_t est_rem = index_t( 
	    float( max - pos ) * float( currtime - begin ) / float( pos - min )
	    );
	  return time2string(est_rem);
	}
    } else {
      return time2string(0);
    }
  } else {
    return "progress_bar hasn't been started";    
  }
}

std::string pink::types::progress_bar::operator << (index_t currPos)
{
  pos = currPos;
  last_report = time(NULL);
  return remainingTime();
}

void pink::types::progress_bar::setPos(index_t currPos)
{
  pos = currPos;
}

std::string pink::types::progress_bar::percent()
{
  if ((max - min) == 0){
    return "undeterminable max==min";
  } else {
    std::stringstream ss;
    ss << index_t(100.*float(pos-min)/float(max - min));
    ss << "%";
    return ss.str();
  }
}

//*************************************************************************************
//*************************************************************************************
//**************************** the vint class *****************************************
//*************************************************************************************
//*************************************************************************************

pink::types::vint::vint():std::vector<index_t>()  //default constructor
{
  //intentionally left empty
} /* vint::vint */



pink::types::vint::vint(const vint & src):std::vector<index_t>(src)  //copy constructor
{
  //intentionally left empty
} /* vint::vint */



pink::types::vint::vint(const vint & src, std::string debug ):std::vector<index_t>(src)  //copy constructor
{
# if UJIMAGE_DEBUG >= 6
  this->debug=debug; // representing the name of the object if debugged
  std::cout << "creating vint '" << debug << "' with address " << static_cast<void*>(this) <<"; with the copy constructor"<< std::endl;
# endif /* UJIMAGE_DEBUG */
} /* vint::vint */



pink::types::vint::vint( index_t size, std::string debug /*=""*/ ):std::vector<index_t>(size)
{
# if UJIMAGE_DEBUG >= 6
  this->debug=debug; // representing the name of the object if debugged
  std::cout << "creating vint '" << debug << "' with address " << static_cast<void*>(this) << std::endl;
# endif /* UJIMAGE_DEBUG */
} /* vint::vint */



pink::types::vint::vint(index_t size, index_t defvalue):std::vector<index_t>(size, defvalue)
{
  // intentionally left empty
} /* vint::vint */


#ifdef PINK_HAVE_PYTHON
pink::types::vint::vint( const boost::python::list & src ): std::vector<index_t>(boost::python::len(src),0) 
{
  index_t length=boost::python::len(src);
//  boost::python::object A = src[1];
//  int a = boost::python::extract<int>(A);
  FOR(q, length)
    try
    {
      (*this)[q]=boost::python::extract<index_t>(src[q]);
    } 
    catch (...) 
    {
      pink_error("the elements of the list must be convertible to integers.");
    }
} /* vint::vint python list */
#endif /* PINK_HAVE_PYTHON */


pink::types::vint::~vint()
{
# if UJIMAGE_DEBUG >= 6
  std::cout << "destroying vint '" << debug << "' with address " << static_cast<void*>(this) << std::endl;
# endif /* UJIMAGE_DEBUG >= 3 */
} /* vint::~vint */

void pink::types::vint::reset()
{
  index_t q = size()-1;
  do 
  { 
    (*this)[q--]=0; 
  } 
  while (q>=0);
} /* vint::reset */



index_t pink::types::vint::prod() const
{
  index_t prod = 1;
  index_t q = size()-1;
  do 
  {
    prod*=(*this)[q--];
  } 
  while (q>=0);
  return prod;
} /* vint::prod */



index_t pink::types::vint::prod_except(index_t p) const
{
  index_t prod = 1;
  index_t q = size()-1;
  do { prod*=(*this)[q--]; } while (q>=0);
  return prod/(*this)[p];
} /* vint::prodExcept */



double pink::types::vint::fabs() const
{
  double sum = 0;
  index_t q = this->size()-1;
  do 
  { 
    sum+=((*this)[q]) * ((*this)[q]);
    q--;    
  }
  while (q>=0);
  return std::sqrt(sum);
} /* vint::abs */



void pink::types::vint::next_step( index_t step, vint & result ) const { 
  //tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
  // ATTENITON !!!!!!!! New version, now the X is the fastest
  // also note, that the size should be the parameter and the object should change
  // to comply with the no parameter change paradigm

  index_t d = this->size();
  index_t prod = step;
  index_t q = 0;
    
  while ( q <= d - 1 ){
            
    if ( (*this)[q] > 0 )
    {
      result[q] = prod % (*this)[q];
    }
                
    if ( prod > 0 )
    {
      prod = prod / (*this)[q];
    }

    q++;
  }
} /* vint::nextStep */



bool pink::types::vint::next_step_it( vint & result ) const {
  // tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
  // ATTENITON !!!!!!!! New version, now the X is the fastest
  // also note, that the size should be the parameter and the object should change
  // to comply with the no parameter change paradigm

  index_t d = this->size();
  index_t q = 0;
    
  while ( ( result[q] >= (*this)[q] - 1) && ( q < d-1 ) ) 
  {
    q++;
  }
  
  if ( (q == d-1) && (result[q] >= (*this)[q] - 1) )
  {
    return false;
  }
  
  result[q] += 1;
  
  while (q > 0) 
  {
    q--;
    result[q]=0;    
  }
  
  return true;
} /* vint::nextStep_it */



bool pink::types::vint::on_side( const vint & point ) const
{
  if (point.size() != this->size())
  {
    pink_error("The 'point' must have same dimension (length) than the 'vector'.");
  } /* if */
  
  FOR(q, this->size())
  {
    if ( (point[q]==0) || (point[q]==(*this)[q]-1) )
    {
      return true;	
    } /* if */	
  } /* FOR */
  
  return false;
} /* vint::on_side */



bool pink::types::vint::inside( const vint & point ) const 
{
  FOR(q, this->size())
  {
    if ( (point[q]<0) || (point[q]>=(*this)[q]) )
    {
      return false;	
    } /* if */	
  } /* FOR */
  
  return true;
} /* vint::inside */



bool pink::types::vint::operator==( const vint & other ) const 
{
  bool result=true;
  index_t n=size();
  FOR(q, n)
    if ((*this)[q]!=other[q]){
      result=false;
      break;
    }
  return result;  
} /* vint::operator== */



bool pink::types::vint::operator!=(const vint & other) const 
{
  return (!((*this)==other));  
} /* vint::operator!= */



index_t pink::types::vint::position( const vint & elem ) const 
{
  // tested in harmony with uiNextStep_it, uiNextStep_fast and Positions
  // ATTENITON !!!!!!!! New version, now the X is the fastest
  // also note, that the size should be the parameter and the object should change
  // to comply with the no parameter change paradigm

  index_t d = this->size();
  index_t pos = 0;
  index_t t = 1;
  index_t q = 0;

  while ( q <= d - 1 ) {
    pos += elem[q] * t;
    t *= (*this)[q];
    q+=1;
  }
    
  return pos;
} /* vint::position */



bool pink::types::vint::add_set( const vint & other )
{

  bool result=false;

  FOR(q, index_t(other.size())) 
  {
    bool is_in = false;

    FOR(w, index_t(this->size()))
      if ((*this)[w]==other[q]) is_in=true;

    if (!is_in) {
      this->push_back(other[q]);
      result=true;
    }

  }

  return result;

// EXAMPLE:
//   vint a(4);
//   a[0]=1;
//   a[1]=2;
//   a[2]=3;
//   a[3]=4;

//   std::cout  << "a=" << a.print() << endl;

//   vint b(6);
//   b[0]=5;
//   b[1]=6;
//   b[2]=3;
//   b[3]=6;
//   b[4]=1;
//   b[5]=3;

//   std::cout << "b=" << b.print() << endl;

//   a.addSet(b);

//   std::cout  << "a.addSet(b)=" << a.print() << endl;

} /* vint::addSet */



// does the exact same thing as print, but in python it's called "__repr__"
std::string pink::types::vint::repr() const
{
  if (size()==0)
  {
#   if UJIMAGE_DEBUG >= 1
    return "'" + this->debug + "' = []";
#   else /* NOT UJIMAGE_DEBUG >= 1 */
    return "[]";
#   endif /* NOT UJIMAGE_DEBUG >= 1 */    
  }
  else /* NOT size()==0 */
  {
    std::stringstream ss;
#   ifdef UJIMAGE_DEBUG
    ss << "'" << debug << "' = ";
#   endif /* UJIMAGE_DEBUG */
    ss << "[ ";
    
    FOR(q, index_t(size()-1))
      ss << (*this)[q] << ", ";
    
    ss << (*this)[size()-1] << " ]";
    
    return ss.str();
  } /* NOT size()==0 */
} /* vint::repr */



pink::types::vint & pink::types::vint::operator<< ( const index_t & initiator )
{
//   #if UJIMAGE_DEBUG >=1
//   if ( this->size()!=initiator.size())
//   {
//     pink_error("you are trying to assign a vector of different size ");
//   } /* this->size()!=initiator.size() */
//   #endif /* UJIMAGE_DEBUG */

  this->clear();
  this->push_back(initiator);
  return *this;  
} /* vint::operator<< */


pink::types::vint & pink::types::vint::operator, ( const index_t & next )
{
  this->push_back(next);
  return *this;
} /* vint::operator, */


/// ------------------------------------------------
/// end of vint
/// ------------------------------------------------



/* LuM end of file */
