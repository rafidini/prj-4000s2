#ifndef LFINDLINE_CHAR_HXX_INCLUDED
#define LFINDLINE_CHAR_HXX_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;


int32_t mean2_char(uint8_t  *Image, int colsize, int rowsize);

int new_edge1_char(uint8_t *Image, int colsize, int rowsize);

int new_edge2_char(uint8_t *Image, int colsize, int rowsize);

void clean1_char(uint8_t  *Image, int colsize, int rowsize);

void clean2_char(uint8_t *Image, int colsize, int rowsize);

void  clean4_char(uint8_t *Image, int colsize, int rowsize);

void get_edge_char(uint8_t *Image, vector<int32_t> *Bord1, vector<int32_t> *Bord2, vector<int32_t> *Bord3,
                vector<int32_t> *Bord4,int rowsize, int colsize);

void get_best_line_char(uint8_t *I, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
                    vector<int32_t> Bord4,int rowsize, int colsize, int w);

int32_t query_char(uint8_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize);

double bresen_test_char(uint8_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize, int colsize, int w);

void black_line_char(uint8_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize);

void bresen_final_char(uint8_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize, int w);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LFINDLINE_CHAR_HXX_INCLUDED
