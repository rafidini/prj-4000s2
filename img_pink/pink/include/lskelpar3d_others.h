/*
Copyright ESIEE (2009)

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#define ISTHMUS 0
#define ENDS    1

/* ordering of deletion directions - DON'T MODIFY THEM */
  #define PALAGYI_U     0
  #define PALAGYI_D     1
  #define PALAGYI_N     2
  #define PALAGYI_S     3
  #define PALAGYI_E     4
  #define PALAGYI_W     5

  #define PALAGYI_UD      0
  #define PALAGYI_NS      1
  #define PALAGYI_EW      2

  typedef struct {
		unsigned long int addr;
		unsigned long int x,y,z;
        void *next;
        void *prev;
  } ListElement;

  typedef struct {
        void *first;
        void *last;
  } List;

  typedef struct {
	long i, j, k;
    unsigned long int oldnei; /*** storing the previous neighbourhood ***/
  } Voxel;

  typedef struct {
  	Voxel v;
  	unsigned long int addr;
	ListElement * ptr;
    void *next;
  } Cell;

  typedef struct {
        Cell *Head;
        Cell *Tail;
        int Length;
  } PointList;

  typedef struct {
        ListElement *first;
        ListElement *last;
  } DoubleList;

  typedef struct {
        unsigned long int x, y, z;
        void             *next;
  } Bordercell;           /* element of the list */

#ifdef __cplusplus
extern "C" {
#endif
//extern int32_t lpalagyi(struct xvimage *image, int32_t nsteps);
extern int32_t palagyi_skelpar_curv_98(struct xvimage *input);
extern int32_t palagyi_skelpar_curv_06(struct xvimage *input);
extern int32_t palagyi_skelpar_surf_02(struct xvimage *input);
extern int32_t palagyi_skelpar_surf_08(struct xvimage *input);
extern int32_t llohoubertrandsymcurv2007(
				  struct xvimage *image,
				  struct xvimage *inhibit,
				  int32_t nsteps);
extern int32_t llohoubertrandsymsurf2007(
				  struct xvimage *image,
				  struct xvimage *inhibit,
				  int32_t nsteps);
extern int32_t lmawanchangcurv2subfields2002(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lmawanleecurv4subfields2002(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lnemethetalcurv2subfields2010(
				  struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode);
extern int32_t lnemethetalcurv4subfields2010(
				  struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode);
extern int32_t lnemethetalcurv8subfields2010(
				  struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode);
extern int32_t lsheetalcurvsym2009(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t ltsaofu6dirsurf1981(struct xvimage *image,
				   int32_t n_steps);
extern int32_t ltsaofu6dircurv1981(struct xvimage *image,
				   int32_t n_steps);
extern int32_t ltsaofu6dircurv1982(
				  struct xvimage *image,
				  struct xvimage *inhibit,
				  int32_t n_steps);
extern int32_t lmasonka1996(
			          struct xvimage *image,
			          int32_t nsteps);
extern int32_t lmawan2000(
			          struct xvimage *image,
				  int32_t n_steps);
extern int32_t llohoubertrand12dir2004(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t llohoubertrand6dir2005(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lmanzanerasurf1999(	  
				  struct xvimage *image,
				  struct xvimage *inhibit,				  
				  int32_t nsteps);
extern int32_t lgongbertrand6dir1990(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lpalagyikuba12dir1999(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lbertrandaktouf8subfields1994(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lpalagyikubahybrid1998(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lpalagyikuba8dir1999(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lraynalcouprie6dir2010(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lsahaetal8subfieldssurf1997(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lsahaetal8subfieldscurv1997(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lnemethetal6dir2011(
				  struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode);
extern int32_t lpalagyi12dir2013( struct xvimage *image,
				  int32_t nsteps);
extern int32_t lnemethetal6dir2012(
			          struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode);
extern int32_t lnemethetal4subfields2012(
			          struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode);
extern int32_t lnemethetal8subfields2012(
			          struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode);
extern int32_t lnemethetalfullyparallel2012(
			          struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode);
extern int32_t lmasonka_psimple2010(
				  struct xvimage *image,
				  int32_t nsteps);
extern int32_t lpalagyi2015(      struct xvimage *image,
				  int32_t n_steps);
#ifdef __cplusplus
}
#endif
