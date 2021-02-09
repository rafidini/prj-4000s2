/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2012 <ujoimro@gmail.com>
  ProCarPlan s.r.o.
*/

// Pink Skeleton C++ Wrapper


// LuM end of file

#ifndef SKELETON__HPP__
#define SKELETON__HPP__

#include <limits>
#include <boost/smart_ptr.hpp>

#include "lskelcurv.h"
#include "mcskelcurv.h"


namespace pink {
    
    template <class T0 = index_t>
    class skel_t
    {

    private:
        typedef T0 tag;        
        boost::shared_ptr<skel> m_skel;

        std::string
        printadjliste(SKC_adj_pcell p) {
            std::stringstream result;
            
            for (; p != NULL; p = p->next)
                result << p->val << " ";

            return result.str();            
        } /* printadjliste() */

        std::string
        printptsliste(SKC_pt_pcell p, int32_t rs) {
            std::stringstream result;
            for (; p != NULL; p = p->next)
                result << p->val%rs << "," << p->val/rs << " ; ";

            return result.str();            
        } /* printptsliste() */

        
    public:

        skel_t( ) {
            pink_error("unimplemented constructor");            
        }

        skel_t( uint32_t rs, uint32_t cs, uint32_t ds, uint32_t nbvertex, uint32_t nbptcell, uint32_t nbadjcell, int32_t connex ) {
            skel * tmp_skel = NULL;
            tmp_skel = initskel(rs, cs, ds, nbvertex, nbptcell, nbadjcell, connex );
            if (tmp_skel==NULL)
                pink_error( "Could not create the skeleton." );
            
            m_skel.reset( tmp_skel, ::termineskel );
        }        
        
        skel_t( std::string filename ) {            
            skel * tmp_skel = NULL;
            tmp_skel = readskel( filename.c_str() );
            if (tmp_skel==NULL)
                pink_error( "Could not read the skeleton from '" + filename + "'" );
            
            m_skel.reset( tmp_skel, ::termineskel );
        }

        skel_t ( skel * m_skel ): m_skel( m_skel, ::termineskel ) { };
        
        operator skel* () {
            return m_skel.get();
        }

        operator const skel * () const {
            return m_skel.get();
        }
        
        std::string repr() {
            uint32_t i;
            skel * S = this->m_skel.get();            
            std::stringstream result;            
  
            result << "isol:\n";
            
            for (i = 0; i < S->e_isol; i++)
            {
                if (SK_DELETED(i)) result << "DELETED ";
                result << "[" << i << "] " << "adj: " << printadjliste(S->tskel[i].adj);
                result << "pts: " << printptsliste(S->tskel[i].pts, S->rs) << "\n";
            }
            result << "end:\n";            
            for (i = S->e_isol; i < S->e_end; i++)
            {
                if (SK_DELETED(i)) result << "DELETED ";
                result << "[" << i << "] " << "adj: " << printadjliste(S->tskel[i].adj);
                result << "pts: " << printptsliste(S->tskel[i].pts, S->rs) << "\n";
            }
            result << "curv:\n";
            for (i = S->e_end; i < S->e_curv; i++)
            {
                if (SK_DELETED(i)) printf("DELETED ");
                printf("[%d] ", i); printf("adj: "); printadjliste(S->tskel[i].adj);
                printf("pts: "); printptsliste(S->tskel[i].pts, S->rs); printf("\n");
            }
            result << "junc:\n";
            for (i = S->e_curv; i < S->e_junc; i++)
            {
                if (SK_DELETED(i)) result << "DELETED ";
                result << "[" << i << "] " << "adj: " << printadjliste(S->tskel[i].adj);
                result << "pts: " << printptsliste(S->tskel[i].pts, S->rs) << "\n";
            }

            return result.str();            
        }
       
        ~skel_t(void) {
            
        }
        
        SKC_pt_pcell
        skeladdptcell ( int32_t val, SKC_pt_pcell next ) {
            return ::skeladdptcell( m_skel.get(), val, next );            
        }
        
        SKC_adj_pcell
        skeladdadjcell( int32_t val, SKC_adj_pcell next ) {
            return ::skeladdadjcell( m_skel.get(), val, next );
        }
        
        void addadjlist( uint32_t k, uint32_t v ) {
            ::addadjlist( m_skel.get(), k, v );
        }        
        
        void addptslist( uint32_t k, uint32_t v ) {
            ::addptslist( m_skel.get(), k, v );
        }
        
        uint32_t nb_adjacent_elts( uint32_t i ) {
            return ::nb_adjacent_elts( m_skel.get(), i );
        }

        void writeskel( std::string  filename ) {
            ::writeskel( m_skel.get(), filename.c_str() );
        }

        template <class MT0>
        void writevskel( std::string filename, MT0 & image ) {
            ::writeskel( m_skel.get(), filename, image );            
        }

        void skeldelete(uint32_t i) {
            ::skeldelete( m_skel.get(), i );
        }
        
        
    }; // class skel_t



    template <class T0>
    skel_t<index_t>
    image2skel( T0 & image, int32_t connex, int32_t len ) {
        // check the type !!!
        skel * tmp_skel = NULL;
        tmp_skel = ::limage2skel( image, connex, len);

        if (tmp_skel==NULL)
            pink_error("The image could not have been created.");        

        skel_t<index_t> result(tmp_skel);        

        return result;
    } // image2skel


    template <class T0>
    skel_t<index_t>
    image2skel_short( T0 & image, int32_t connex ) {        
        return image2skel<T0>( image, connex,  std::numeric_limits<int32_t>::max() );        
    } // image2skel

    
} /* namespace pink */


#endif /* SKELETON__HPP__ */

// LuM end of file
