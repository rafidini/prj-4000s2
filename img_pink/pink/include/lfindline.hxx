#ifndef LFINDLINE_H_INCLUDED
#define LFINDLINE_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

using namespace std;
#  include "mcimage.h"
#  include "mccodimage.h"


double mean2(uint16_t  *Image, int colsize, int rowsize);

void control(uint16_t *Image, int colsize, int rowsize);

int32_t mean4(uint16_t *Image, int colsize, int rowsize);

int new_edge1(uint16_t *Image, int colsize, int rowsize);

int new_edge2(uint16_t *Image, int colsize, int rowsize);

int new_edge3(uint16_t *Image, int colsize, int rowsize);

int new_edge4(uint16_t *Image, int colsize, int rowsize);

void clean3(uint16_t *Image, int colsize, int rowsize);

void  clean4(uint16_t *Image, int colsize, int rowsize);

xvimage *tophat(xvimage *original, int w);

void get_edge(uint16_t *Image, vector<int32_t> *Bord1, vector<int32_t> *Bord2, vector<int32_t> *Bord3,
                vector<int32_t> *Bord4,int rowsize, int colsize, int b1, int b2, int b3, int b4);

void get_best_line(uint16_t *I, vector<int32_t> Bord2, vector<int32_t> Bord3, vector<int32_t> Bord1,
                    vector<int32_t> Bord4,int rowsize, int colsize, int w, int b1, int b2, int b3, int b4, double seuil, double zero);

int32_t query(uint16_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize);

double bresen_test(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize, int colsize, int w, double zero);

void black_line(uint16_t *Image,int32_t x,int32_t y,int w,int rowsize, int colsize);

void bresen_final(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize,int colsize, int w);

void mask_line(uint16_t *Image,int32_t x1,int32_t y1,int32_t x2,int32_t y2,int rowsize,int colsize, int w);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LFINDLINE_H_INCLUDED
