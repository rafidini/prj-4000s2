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

#ifndef MCSKEL2D__H__
#define MCSKEL2D__H__

#include "mctypes.h"

#define FLAG_PRINC  0x01
#define FLAG_ESS    0x02
#define FLAG_CORE   0x04
#define FLAG_REGUL  0x08
#define FLAG_CRITIC 0x10
#define FLAG_CRITIC_T 0x20
#define FLAG_TMP    0x40
#define FLAG_OBJ    0x80

#define IS_PRINC(f)  (f&FLAG_PRINC)
#define IS_ESS(f)    (f&FLAG_ESS)
#define IS_CORE(f)   (f&FLAG_CORE)
#define IS_REGUL(f)  (f&FLAG_REGUL)
#define IS_CRITIC(f) (f&FLAG_CRITIC)
#define IS_CRITIC_T(f) (f&FLAG_CRITIC_T)
#define IS_TMP(f)    (f&FLAG_TMP)
#define IS_OBJ(f)    (f&FLAG_OBJ)

extern boolean K2_CheckFrame(struct xvimage *k);
extern void K2_MarkObj(struct xvimage *k);
extern void K2_SelMarked(struct xvimage *k, unsigned char mask);
extern void K2_Binarize(struct xvimage *k);
extern void K2_MarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask);
extern void K2_UnMarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask);
extern void K2_MarkPrinc(struct xvimage *k);
extern void K2_MarkEss(struct xvimage *k);
extern int32_t K2_MarkCore(struct xvimage *k, index_t f);
extern int32_t K2_MarkCore2(struct xvimage *k, struct xvimage *m, index_t f);
extern int32_t K2_CardCore(struct xvimage *k, index_t f);
extern int32_t K2_CardCore2(struct xvimage *k, struct xvimage *m, index_t f);
extern void K2_MarkRegul(struct xvimage *k);
extern void K2_MarkRegul2(struct xvimage *k, struct xvimage *m);
extern void K2_MarkCritic(struct xvimage *k);
extern void K2_MarkCritic1(struct xvimage *k);
extern void K2_MarkCritic2(struct xvimage *k, struct xvimage *m);
extern void K2_MarkCritic3(struct xvimage *k, struct xvimage *m);
extern void K2_MarkMCritic(struct xvimage *k);
extern void K2_MarkMCritic2(struct xvimage *k, struct xvimage *m);
extern int32_t K2_Critic2Obj(struct xvimage *k);
extern int32_t K2_MCritic2Obj(struct xvimage *k);
extern int32_t K2_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m);
extern int32_t K2_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m);
extern void K2_MarkThin(struct xvimage *k, struct xvimage *m);
extern void K2_HitPrinc(struct xvimage *k);
extern void K2_MarkEnd(struct xvimage *k, struct xvimage *m);
extern void K2_MarkEnd2(struct xvimage *k, struct xvimage *m);
extern void K2_MarkNotNeighInterior(struct xvimage *k, struct xvimage *m);
extern void K2_LabelDifference(struct xvimage *k, struct xvimage *m, struct xvimage *l, index_t lab);

extern void lskel2d4(struct xvimage * k, index_t nsteps);
extern void lskel2d4b(struct xvimage * k, struct xvimage * m, index_t nsteps);
extern void lskel2d5(struct xvimage * k, index_t nsteps);
extern void lskel2d7(struct xvimage * k, index_t nsteps);
extern void lskel2d9(struct xvimage * k, index_t nsteps);
extern int32_t l2dskelck(struct xvimage * k, int32_t mode, int32_t nsteps, struct xvimage * inhi);
#endif /* MCSKEL2D__H__ */
