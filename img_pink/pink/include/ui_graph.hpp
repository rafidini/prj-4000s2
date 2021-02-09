/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by applicable law.
  
  (C) UjoImro, 2012 <ujoimro@gmail.com>
  ProCarPlan s.r.o.
*/

// Pink Graph C++ Wrapper

// LuM end of file

#ifndef GRAPH__HPP__
#define GRAPH__HPP__

#include <string>
#include <boost/python.hpp>
#include <boost/smart_ptr.hpp>

#include "mcgraphe.h"
#include "lskel2graph.h"
#include "ui_skeleton.hpp"


namespace pink {

    template <class T0>
    class graph_t
    {
    private:
        typedef T0 tag;
        boost::shared_ptr<graphe> m_graph;        
        
    public:

        operator graphe* () { return this->m_graph.get(); }

        operator const graphe* const () { return this->m_graph.get(); }
        
        graph_t( graphe * m_graph ) : m_graph( m_graph, ::TermineGraphe ) { }

        graph_t( std::string filename ) {
            graphe * tmp_graph = NULL;
            tmp_graph = ReadGraphe( filename.c_str() );

            if (tmp_graph==NULL)
                pink_error( "The graph could not have been read." );
            
            m_graph.reset( tmp_graph, ::TermineGraphe );            
        }
        
        graph_t( ) { pink_error("You should not be calling me."); }
        
        ~graph_t ( ) { }

        index_t
        nb_nodes () const {
            return m_graph->nsom;            
        }

        index_t
        nb_edges () const {
            return m_graph->narc;            
        }

        index_t
        nb_maxedges () const {
            return m_graph->nmaxarc;            
        }
                
        graph_t<T0>
        clone (void) const {
            ::graphe * tmp = NULL;
            tmp = ::CopyGraphe(m_graph.get());
            if (tmp == NULL)
                pink_error("Could not clone the graph!");
            
            graph_t<T0> result(tmp);
            return result;
        } // clone the graph
                
        void writegraph( std::string filename ) const {
            SaveGraphe( m_graph.get(), filename.c_str() );
        } // writegraph

        void writeps( std::string filename, double scale = 20. ) const {
            graph_t<T0> copy = clone();
            
            for ( index_t q = 0; q < copy.nb_nodes(); q++)
            {
                copy.m_graph->x[q] *= scale;
                copy.m_graph->y[q] *= -scale; // - : pour suivre la convention image (y de haut en bas)
            }

            EPSGraphe(
                copy, filename.c_str(), 
                2, // facteur d'échelle à appliquer aux coordonnées des sommets
                2, // rayon des cercles qui représentent les sommets
                0, // taille (demi-longueur) des flèches pour les arcs
                10, // marge en x et en y
                0, // booléen indiquant s'il faut écrire les noms des sommets
                0, // booléen indiquant s'il faut écrire les valeurs des sommets
                0, // booléen indiquant s'il faut colorier les sommets
                0  // booléen indiquant s'il faut écrire les valeurs des arcs
                );
        } // writeps
        
        
    }; // class graph_t


    template <class T0>
    pink::graph_t<T0>
    skel2graph( pink::skel_t<T0> & skel, int32_t mode, double param )
    {
        graphe * tmp = NULL;        
        tmp = lskel2graph( skel, mode, param );
        if (tmp == NULL)
            pink_error("The graph could not have been created.");
        
        pink::graph_t<T0> result(tmp);

        return result;        
    } // skel2graph


    template <class T0>
    pink::graph_t<T0>
    skel2graph_short( pink::skel_t<T0> & skel, int32_t mode )
    {
        assert( (mode == 0) || (mode == 1) );
        graphe * tmp = NULL;        
        tmp = lskel2graph( skel, mode, 0 );
        if (tmp == NULL)
            pink_error("The graph could not have been created.");
        
        pink::graph_t<T0> result(tmp);

        return result;        
    } // skel2graph
    
    
} /* namespace pink */


#endif /* GRAPH__HPP__ */

// LuM end of file
