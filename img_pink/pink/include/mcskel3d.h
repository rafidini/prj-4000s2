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
#ifndef MCSKEL3D__H__
#define MCSKEL3D__H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mcliste.h>

#define FLAG_PRINC    0x01
#define FLAG_ESS      0x02
#define FLAG_CORE     0x04
#define FLAG_CRITIC   0x08
#define FLAG_CRITIC_E 0x10
#define FLAG_TMP1     0x20
#define FLAG_TMP2     0x40
#define FLAG_OBJ      0x80

#define IS_PRINC(f)    (f&FLAG_PRINC)
#define IS_ESS(f)      (f&FLAG_ESS)
#define IS_CORE(f)     (f&FLAG_CORE)
#define IS_CRITIC(f)   (f&FLAG_CRITIC)
#define IS_CRITIC_E(f) (f&FLAG_CRITIC_E)
#define IS_TMP1(f)     (f&FLAG_TMP1)
#define IS_TMP2(f)     (f&FLAG_TMP2)
#define IS_OBJ(f)      (f&FLAG_OBJ)

#define MARK1 0x01
#define MARK2 0x02
#define MARK3 0x04

#define IS_MARKED1(f) (f&MARK1)
#define IS_MARKED2(f) (f&MARK2)
#define IS_MARKED3(f) (f&MARK3)

extern int32_t mcskel3d_K3_CheckFrame(struct xvimage *k);
extern void mcskel3d_K3_CloseComplex(struct xvimage *k);
extern void mcskel3d_K3_MarkObj(struct xvimage *k);
extern void mcskel3d_K3_SelMarked(struct xvimage *k, unsigned char mask);
extern void mcskel3d_K3_Binarize(struct xvimage *k);
extern void mcskel3d_K3_MarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask);
extern void mcskel3d_K3_UnMarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask);
extern void mcskel3d_K3_MarkPrinc(struct xvimage *k);
extern void mcskel3d_K3_MarkEss(struct xvimage *k);
extern int32_t  mcskel3d_K3_MarkCore(struct xvimage *k, index_t f);
extern int32_t  mcskel3d_K3_MarkCore2(struct xvimage *k, struct xvimage *m, index_t f);
extern int32_t  mcskel3d_K3_CardCore(struct xvimage *k, index_t f);
extern int32_t  mcskel3d_K3_CardCore2(struct xvimage *k, struct xvimage *m, index_t f);
extern void mcskel3d_K3_MarkCritic(struct xvimage *k);
extern void mcskel3d_K3_MarkCritic2(struct xvimage *k, struct xvimage *m);
extern void mcskel3d_K3_MarkCritic3(struct xvimage *k, struct xvimage *m);
extern void mcskel3d_K3_MarkMCritic(struct xvimage *k);
extern void mcskel3d_K3_MarkMCritic2(struct xvimage *k, struct xvimage *m);
extern index_t  mcskel3d_K3_Critic2Obj(struct xvimage *k);
extern index_t  mcskel3d_K3_CriticE2Obj(struct xvimage *k);
extern index_t  mcskel3d_K3_MCritic2Obj(struct xvimage *k);
extern index_t  mcskel3d_K3_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m);
extern index_t  mcskel3d_K3_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m);
extern void mcskel3d_K3_HitPrinc(struct xvimage *k);
extern void mcskel3d_K3_MarkEnd(struct xvimage *k, struct xvimage *m);
extern void mcskel3d_K3_MarkEnd2(struct xvimage *k, struct xvimage *m);
extern void mcskel3d_K3_MarkNotNeighInterior(struct xvimage *k, struct xvimage *m);
extern void mcskel3d_K3_LabelDifference(struct xvimage *k, struct xvimage *m, struct xvimage *l, int32_t lab);

extern void K3List_MarkPrinc(struct xvimage *k, Liste *l);
extern void K3List_MarkEss(struct xvimage *k, Liste *l);
extern int32_t  K3List_MarkCore2(struct xvimage *k, unsigned char *I, index_t f);
extern void K3List_MarkCritic2(struct xvimage *k, unsigned char *I, Liste *l);

extern int32_t lskeleuc3d(struct xvimage * k, struct xvimage * inhi, int32_t nsteps) ;
extern int32_t llistskeleuc3d(struct xvimage * k, struct xvimage * inhi, int32_t nsteps) ;
extern int32_t lskel3d4(struct xvimage * k, int32_t nsteps) ;
extern int32_t lskel3d4b(struct xvimage * k, struct xvimage * m, int32_t nsteps) ;
extern int32_t l3dskelck(struct xvimage * k, int32_t mode, int32_t nsteps, struct xvimage * inhi);
#ifdef __cplusplus
}
#endif

#endif /* MCSKEL3D__H__ */
