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
#ifndef MCSKEL2D3D__H__
#define MCSKEL2D3D__H__


#include "mctypes.h"

#define FLAG_PRINC  0x01
#define FLAG_ESS    0x02
#define FLAG_CORE   0x04
#define FLAG_REGUL  0x08
#define FLAG_CRITIC 0x10
#define FLAG_TMP    0x20
#define FLAG_INTERIOR 0x40
#define FLAG_OBJ    0x80

extern boolean mcskel2d3d_K3_Check2D(struct xvimage *k);
extern boolean mcskel2d3d_K3_CheckFrame(struct xvimage *k);
extern void mcskel2d3d_K3_Extract2Dfaces(struct xvimage *k);
extern void mcskel2d3d_K3_Closure(struct xvimage *k);
extern void mcskel2d3d_K3_MarkObj(struct xvimage *k);
extern void mcskel2d3d_K3_SelMarked(struct xvimage *k, unsigned char mask);
extern void mcskel2d3d_K3_Binarize(struct xvimage *k);
extern void mcskel2d3d_K3_MarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask);
extern void mcskel2d3d_K3_UnMarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask);
extern void mcskel2d3d_K3_MarkPrinc(struct xvimage *k);
extern void mcskel2d3d_K3_MarkEss(struct xvimage *k);
extern int32_t  mcskel2d3d_K3_MarkCore(struct xvimage *k, index_t f);
extern int32_t  mcskel2d3d_K3_MarkCore2(struct xvimage *k, struct xvimage *m, index_t f);
extern int32_t  mcskel2d3d_K3_CardCore(struct xvimage *k, index_t f);
extern int32_t  mcskel2d3d_K3_CardCore2(struct xvimage *k, struct xvimage *m, index_t f);
extern void mcskel2d3d_K3_MarkRegul(struct xvimage *k);
extern void mcskel2d3d_K3_MarkRegul2(struct xvimage *k, struct xvimage *m);
extern void mcskel2d3d_K3_MarkCritic(struct xvimage *k);
extern void mcskel2d3d_K3_MarkCritic2(struct xvimage *k, struct xvimage *m);
extern int32_t  mcskel2d3d_K3_Critic2Obj(struct xvimage *k);
extern int32_t  mcskel2d3d_K3_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m);
extern int32_t mcskel2d3d_K3_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m, struct xvimage *o);
extern void mcskel2d3d_K3_MarkThin(struct xvimage *k, struct xvimage *m);
extern void mcskel2d3d_K3_HitPrinc(struct xvimage *k);
extern void mcskel2d3d_K3_MarkEnd(struct xvimage *k, struct xvimage *m);
extern void mcskel2d3d_K3_MarkEnd2(struct xvimage *k, struct xvimage *m);
extern void mcskel2d3d_lskel3d4(struct xvimage * k, index_t nsteps);
extern void lskel3d5(struct xvimage * k, index_t nsteps);
extern void lskel3d6(struct xvimage * k, index_t nsteps);
extern void lskel3d7(struct xvimage * k, index_t nsteps);

#endif /* MCSKEL2D3D__H__ */
