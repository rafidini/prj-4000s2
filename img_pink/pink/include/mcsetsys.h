#include <mctypes.h>

typedef struct {
  int setsize;      /* taille du tableau de bits (a multiplier par 32) */
  int rank;         /* rang de l'ensemble dans le systeme */
  int label;
  ulong tab[1];
} Set;

typedef struct {
  int n;            /* nombre d'ensembles dans la liste */
  int nmax;         /* nombre maximal d'ensembles dans la liste */
  Set * tab[1];     /* table des ensembles */
} SetSyst;

/* ================================================================== */
/* PROTOTYPES DES FONCTIONS POUR LES ENSEMBLES (Set) */
/* ================================================================== */

extern Set *   CreateEmptySet(int setsize);
extern void    FreeSet(Set *s);
extern void    PrintSetElements(Set *s);
extern void    PrintSet(Set *s);
extern void    PrintSetLine(Set *s);
extern void    WriteSet(Set *s, FILE *fd);
extern void    ReadSet(Set *s, FILE *fd);
extern boolean IsEmpty(Set *s);
extern void    SetEmpty(Set *s);
extern int     GetElement(Set *s);
extern void    SetElement(Set *s, int e);
extern void    ResetElement(Set *s, int e);
extern boolean InSet(Set *s, int e);
extern void    Complement(Set *s);
extern void    Union(Set *s, Set *sa);
extern void    Inter(Set *s, Set *sa);
extern boolean Intersect(Set *s, Set *sa);
extern boolean EqualSets(Set *s1, Set *s2);
extern void    CopySet(Set *sd, Set *ss);
extern Set *   CloneSet(Set *ss);
extern boolean Included(Set *s1, Set *s2);
extern boolean StrictlyIncluded(Set *s1, Set *s2);
extern int     CardSet(Set *s);

/* ================================================================== */
/* PROTOTYPES DES FONCTIONS POUR LES TABLES DE CORRESPONDANCE  */
/* ================================================================== */

extern void    WriteTable(int *T, int n, char *filename);
extern int   * ReadTable(char *filename, int *N);

/* ================================================================== */
/* PROTOTYPES DES FONCTIONS POUR LES LISTES D'ENSEMBLES (SetSyst) */
/* ================================================================== */

extern void      PrintSetSyst(SetSyst * l);
extern SetSyst * SetSyst2Graph(SetSyst * F);
extern void      PrintGraph(SetSyst * graph);
extern void      WriteSetSyst(SetSyst * l, char *filename);
extern void      WriteLabels(SetSyst * l, char *filename);
extern SetSyst * ReadSetSyst(char *filename);
extern SetSyst * InitSetSyst(int n, int setsize);
extern void      TermineSetSyst(SetSyst * l);
extern void      FlushSetSyst(SetSyst * l);
extern void      PushSetSyst(SetSyst * l, Set *s);
extern boolean   InSetSyst(SetSyst * l, Set *s);
extern boolean   InclusSetSyst(SetSyst * l, Set *s);
extern int       MaxRank(SetSyst * l, Set *s);
extern void      TriTopo(SetSyst * l);
extern SetSyst * InverseSetSyst(SetSyst * l);
extern void      Khalimsky2SetSyst(struct xvimage *o, SetSyst **F, int **C);
extern void      SetSyst2Khalimsky(SetSyst *X, int *C, int mask, struct xvimage *k);
extern void      SubSetX(SetSyst *F, int rs, int ps, int *C, int x, Set *T);
extern void      SubSetY(SetSyst *F, int rs, int ps, int *C, int y, Set *T);
extern void      SubSetZ(SetSyst *F, int rs, int ps, int *C, int z, Set *T);
extern void      SubSetRang(SetSyst *F, Set *I, int r, Set *T);
extern void      SubSetLabel(SetSyst *F, Set *I, int lab, Set *T);
extern void      SetLabels(SetSyst *F, int lab);
extern void      ClearLabels(SetSyst *F);
