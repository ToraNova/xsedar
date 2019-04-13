/*
 This file is part of JustGarble.

    JustGarble is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JustGarble is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JustGarble.  If not, see <http://www.gnu.org/licenses/>.

*/

/////////////////////////////////////////////////////////////////////////////
// The Software is provided "AS IS" and possibly with faults.
// Intel disclaims any and all warranties and guarantees, express, implied or
// otherwise, arising, with respect to the software delivered hereunder,
// including but not limited to the warranty of merchantability, the warranty
// of fitness for a particular purpose, and any warranty of non-infringement
// of the intellectual property rights of any third party.
// Intel neither assumes nor authorizes any person to assume for it any other
// liability. Customer will use the software at its own risk. Intel will not
// be liable to customer for any direct or indirect damages incurred in using
// the software. In no event will Intel be liable for loss of profits, loss of
// use, loss of data, business interruption, nor for punitive, incidental,
// consequential, or special damages of any kind, even if advised of
// the possibility of such damages.
//
// Copyright (c) 2003 Intel Corporation
//
// Third-party brands and names are the property of their respective owners
//
///////////////////////////////////////////////////////////////////////////
// Random Number Generation for SSE / SSE2
// Source File
// Version 0.1
// Author Kipp Owens, Rajiv Parikh
////////////////////////////////////////////////////////////////////////

#ifndef JGUTIL_H_
#define JGUTIL_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "common.h"
#include "emmintrin.h"

int countToN(int *a, int N);
int dbgBlock(block a);
#define RDTSC ({unsigned long long res;  unsigned hi, lo;   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi)); res =  ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );res;})
int getWords(char *line, char *words[], int maxwords);
#define fbits( v, p) ((v & (1 << p))>>p)
block randomBlock();
int median(int A[], int n);
double doubleMean(double A[], int n);
void srand_sse(unsigned int seed);

//added by ToraNova chia_jason96@live.com - figure out the source
void print128_num(block var);
void printblock_arr(block *il,int size);
void printtable(block **tp, int ngate);
void printbin_arr(char *msg,int *t,int size);
void printint_arr(char *msg, int *t,int start, int size);
void block2bitstream(block *in, int *out, int in_start,int out_start,int no_blocks);
void printbitstream(int *target, int block,int size);


char *base64_encode(const unsigned char *data,size_t input_length,size_t *output_length);

//#define BIGENDHEYO
void make_uint_array_from_blob(int* dest, unsigned char* blob, u_int32_t bloblen);

#ifdef __cplusplus
};
#endif

#endif /* UTIL_H_ */
