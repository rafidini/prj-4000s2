/* 
Code written by Lilan Buzer - http://www.esiee.fr/~buzerl/DG/

Reference: 
@Article{	  IGMI_Buz2007,
  author	= {Lilian Buzer},
  title		= {{A Simple Algorithm for Digital Line Recognition in the General Case}},
  journal	= {Pattern Recognition},
  year		= {2007},
  volume	= {40},
  pages		= {1675-1684},
  number	= {6}
}
*/

#include <lbdigitalline.h>

Reco::Reco()  { Init(); }

void Reco::Init()
{
  nU = nL = 0;
  back = false;
}

bool Reco::valid()
{
  if ( U[1].x != U[nU].x )     return true;
  else                         return false;
}

void Reco::Insert(Q & curEp, point &NP)
{
  if ( back ) return;
  point SNP = Sym(NP);
  if ( nU == 0 ) lastX = SNP.x;
  else if ( SNP.x < lastX ) { back = true; curEp = Q(5000); return; }
       else lastX = SNP.x;
  InsertPoint(curEp,SNP);
}

Q VerticalThickness(const point & a1,const  point & a2,const  point & b)
{
  point v1 = (a2-a1);
  point v2 = (b -a1);
  Z  pv  = v1^v2;
  if (pv < 0) pv = -pv;

  return Q(pv,abs(v1.x));
}

Q Reco::Ep(int i_u,int i_l)
{
  if ( U[i_u].x == L[i_l].x )   return Q(U[i_u].y-L[i_l].y);
  if ( U[i_u].x  < L[i_l].x )   return VerticalThickness(U[i_u],U[i_u+1],L[i_l]);
  else                          return VerticalThickness(L[i_l],L[i_l+1],U[i_u]);
}


void Reco::InsertPoint(Q & curEp, point & NP)  // NP=new point
{
  if ( nU == 0 )
  {
    iL = iU = nL = nU = 1;       // 1 point entered
    U[1]=L[1]=NP;                // arrays begin at index 1 (yes)
    curEp = Q(0);
    return;
  }

  if ( nU == 1 )
  {
    if (NP.x == U[1].x)
    {
      if  (NP.y > U[1].y)  U[1] = NP;
      if  (NP.y < L[1].y)  L[1] = NP;
      curEp = Q(U[1].y-L[1].y);
      return;
    }
    else
    {
      U[2]= L[2] = NP;
      iL = iU = nL = nU = 2;
      curEp = Q(U[1].y-L[1].y);
      return;
    }
  }

  // main case, at least 2 points

  // upper border

  if ( (NP.x > U[nU].x) || (NP.y > U[nU].y) ) // point not under the upper righmost vertex
  {
    // backtraking to update the convex hull
    while ( (nU > 1) && ((U[nU-1]-U[nU])^(NP-U[nU])) <= 0 ) nU--;
    // when vertex that defines the previous thickness is replaced by the NP
    if ( iU > nU ) iU = nU;
    // insertion of the new point (NP)
    nU++; U[nU] = NP;
  }

  if ( (NP.x > L[nL].x) || (NP.y < L[nL].y) ) // point not over the lower righmost vertex
  {
    // backtraking to update the convex hull
    while ( (nL > 1) && ((L[nL-1]-L[nL])^(NP-L[nL])) >= 0 ) nL--;
    // when vertex that defines the previous thickness is replaced by the NP
    if ( iL > nL ) iL = nL;
    // insertion of the new point (NP)
    nL++; L[nL] = NP;
  }

  curEp = Ep(iU,iL);         // update Height(x)
  Q nextEp = curEp;

  // traverse height(x) while the height increases
  while ( (nextEp >= curEp) && ( iL != nL ) && ( iU != nU) )
  {
    // the next abscissa to consider in height(x)
    int niU,niL;
    if ( ( iL != nL ) && ( iU != nU) )
    {
      if ( U[iU+1].x < L[iL+1].x )       { niU = iU+1; niL = iL;   }
      else                               { niU = iU;   niL = iL+1; }
    }
    else if ( iL == nL )   { niU = iU+1; niL = iL;   }
         else              { niU = iU;   niL = iL+1; }

    nextEp = Ep(niU,niL);
    if ( nextEp >= curEp )     { curEp = nextEp;  iU = niU; iL = niL; }
 //   else                         return;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Reco::Normal(int & a, int & b)
{
  if ( nU == 0 ) { a = 0;  b=1;  return; }
//  if ( Ep(iU,iL) == Q(0) )   { point v=U[iU]-U[iU+1]; a=-v.y; b = v.x; return; }
  point d;
  if ( U[iU].x == L[iL].x )  // cas v-v
  {
      if ( iU == nU )  d = U[iU]-U[iU-1]; // V-V on the right of the convex hull
      else  if ( Q(L[iL].y-L[iL -1].y,L[iL].x-L[iL-1].x) >=
                 Q(U[iU+1].y-U[iU].y,U[iU+1].x-U[iU].x) )
               d = L[iL]-L[iL-1];
      else     d = U[iU+1]-U[iU];
  }
  else
  {
    if   ( U[iU].x < L[iL].x )  d = U[iU+1]-U[iU];
    else                        d = L[iL+1]-L[iL];
  }
  d.prime();
  // rot
  a = - d.y;
  b = d.x;
} // Normal

void Reco::Tangent(int & a, int & b)
{
  if ( nU == 0 ) { a = 0;  b=1;  return; }
//  if ( Ep(iU,iL) == Q(0) )   { point v=U[iU]-U[iU+1]; a=-v.y; b = v.x; return; }
  point d;
  if ( U[iU].x == L[iL].x )  // cas v-v
  {
      if ( iU == nU )  d = U[iU]-U[iU-1]; // V-V on the right of the convex hull
      else  if ( Q(L[iL].y-L[iL -1].y,L[iL].x-L[iL-1].x) >=
                 Q(U[iU+1].y-U[iU].y,U[iU+1].x-U[iU].x) )
               d = L[iL]-L[iL-1];
      else     d = U[iU+1]-U[iU];
  }
  else
  {
    if   ( U[iU].x < L[iL].x )  d = U[iU+1]-U[iU];
    else                        d = L[iL+1]-L[iL];
  }
  d.prime();
  // rot
  a = d.x;
  b = d.y;
} // Tangent

void Reco::Tangent(int type, int & a, int & b)
{
  int aa, bb;
  Tangent(aa, bb);
  switch(type)
  {
  case 0:/*LR*/ a = aa; b = bb; break;
  case 1:/*RL*/ a = -aa; b = bb; break;
  case 2:/*TB*/ a = -bb; b = -aa; break;
  case 3:/*BT*/ a = bb; b = aa; break;
  }
} // Tangent

void Reco::GiveP(Lpoint LL, int & n, int miny, int maxy)
{
  Rebuild(LL,n,miny,maxy);
  for (int i = 0 ; i < n ; i++)
    LL[i] = Sym(LL[i]);
}

void Reco::Rebuild(Lpoint LL, int & n,int miny,int maxy)
{
  int a,b;
  if ( nU == 1 ) { LL[0] = U[0]; n=1; return; }
  Normal(a,b);
  if ( (b==0)) { LL[0] = U[1]; n=1; return;}

  Z ming = a*U[1].x+b*U[1].y;
  Z maxg = ming;

  for (int i = 1; i <= nU; i++) { ming = min(ming,a*U[i].x+b*U[i].y);
                                  maxg = max(maxg,a*U[i].x+b*U[i].y); }
  for (int i = 1; i <= nL; i++) { ming = min(ming,a*L[i].x+b*L[i].y);
                                  maxg = max(maxg,a*L[i].x+b*L[i].y); }
  n=0;

  while ( maxg-ming < b-1 )
  { maxg++;  }

  for (int x = L[1].x ; x <= L[nL].x ; x++)
  for (int y = miny;   y <= maxy;     y++)
  {
     int os = a*x+b*y;
     if ( (os <=maxg) && (os>=ming) )
     {  LL[n] = point(x,y); n++; }
  }
}
