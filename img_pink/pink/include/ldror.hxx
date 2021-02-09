#ifndef LDROR_HXX_INCLUDED
#define LDROR_HXX_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

void divide_image_short(uint16_t *Image, uint16_t *I1,uint16_t *I2,uint16_t *I3,uint16_t *I4, int colsize, int rowsize);

void create_query_tree_short(uint16_t *I,tree *T, int colsize, int rowsize);

void get_line_short(uint16_t *I,tree *T, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
               vector<int32_t> Bord4,int rowsize, int colsize, int w);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LDROR_HXX_INCLUDED
