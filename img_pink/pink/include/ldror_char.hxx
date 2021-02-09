#ifndef LDROR_CHAR_HXX_INCLUDED
#define LDROR_CHAR_HXX_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

//typedef struct node * tree;
//
//struct node
//{
//	int32_t value;
//	tree high_left;
//	tree high_right;
//    tree low_left;
//	tree low_right;
//};

struct tree
{
       int value;
       tree* high_left;
       tree* high_right;
       tree* low_left;
       tree* low_right;
       int rowsize;
       int colsize;
};


using namespace std;


tree *Create(int val,tree *t1, tree *t2,tree *t3, tree *t4);

bool Isempty( tree *T);

void AddElt(tree *src,int position, int32_t elt);

void WideSearch(tree *T);

bool Isleave(tree *T);

void Iscorrect(tree *T);

void divide_image(uint8_t *Image, uint8_t *I1, uint8_t *I2, uint8_t *I3, uint8_t *I4, int colsize, int rowsize);

int32_t query2(tree *T);

void create_query_tree(uint8_t *I,tree *T, int colsize, int rowsize);

void get_line(uint8_t *I,tree *T, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int rowsize, int colsize, int w);

uint32_t find_x3(int x1, int y1,int x2, int y2, int rowsize, int colsize);

uint32_t find_y3(int x1, int y1,int x2, int y2, int rowsize, int colsize);

int32_t count23(tree *T,int32_t y1,int32_t y2, int rowsize, int colsize);

int32_t  count21(tree *T,int32_t y1,int32_t x2, int rowsize, int colsize);

int32_t  count24(tree *T,int32_t y1,int32_t x2, int rowsize, int colsize);

int32_t  count14(tree *T,int32_t x1,int32_t x2, int rowsize, int colsize);

int32_t  count43(tree *T,int32_t x1,int32_t y2, int rowsize, int colsize);

int32_t  count13(tree *T,int32_t x1,int32_t y2, int rowsize, int colsize);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif // LDROR_CHAR_HXX_INCLUDED
