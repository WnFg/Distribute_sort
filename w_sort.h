#ifndef __W_SORT_
#define __W_SORT_

#include <stdio.h>
#include <stdlib.h>
#include "IO.h"
#include <algorithm>

#define maxNum_Obj 5000000

int A[maxNum_Obj+5], B[maxNum_Obj+5];
int C[2*maxNum_Obj + 5];

inline int min(int x, int y) {
	return x < y ? x : y;
}

int getSizeOfFile(FILE* p) {
	int pos = ftell(p);
	fseek(p, 0, SEEK_END);
	int ans = ftell(p);
	fseek(p, pos, SEEK_SET);
	return ans;
}

void __sort(FILE*& inFile, int l, int mid, int r, FILE*& outFile) {
	int A_cur = 0, B_cur = 0;
	int rl = mid;

	while(true) {

		fseek(inFile, l << 2, SEEK_SET);
		int countA = fread(A + A_cur, 4, min(mid - l, maxNum_Obj - A_cur), inFile);
		
		fseek(inFile, rl << 2, SEEK_SET);
		int countB = fread(B + B_cur, 4, min(r - rl, maxNum_Obj - B_cur), inFile);

		l += countA;
		rl += countB;
		
		countA += A_cur;
		countB += B_cur;
		
		if(countA == 0) {
			if(countB == 0)
				break;

			fwrite(B, 4, countB, outFile);
			B_cur = 0;
			continue;
		}

		if(countB == 0) {
			fwrite(A, 4, countA, outFile);
			A_cur = 0;
			continue;
		}
		
		int i, j = 0, cnt = 0;

		for(i = 0; j < countB && i < countA; ) {
			while(i < countA && j < countB && A[i] <= B[j]) {
				C[cnt++] = A[i++];
			}
			if(i == countA) 
				break;
			C[cnt++] = B[j];
			++j;
		}

		if(i == countA) {
			B_cur = countB - j;
			A_cur = 0;
			
			if(j) { 
				int ret = 0;
				for(; j < countB; j++)
					B[ret++] = B[j];
			}
		}else {
			A_cur = countA - i;
			B_cur = 0;

			if(i) { 
				int ret = 0;
				for(; i < countA; i++)
					A[ret++] = A[i];
			}
		}

		fwrite(C, 4, cnt, outFile);
	}
}

void out_sort(FILE*& in, FILE*& out, int size)
{
	int len = maxNum_Obj*2;
	int limit = (size - 1) / len + 1;
	int cnt;
	int fanzhuan = 0;
	fseek(in, 0, SEEK_SET);
	fseek(out, 0, SEEK_SET);

	int sum = 0;
	do{
		cnt = fread(C, 4, len, in);
		std::sort(C, C + cnt);
		sum += cnt;
		fwrite(C, 4, cnt, out);
	}while(cnt);
	
	std::swap(in, out);
	++fanzhuan;
	
	fseek(in, 0, SEEK_END);
	fseek(out, 0, SEEK_END);
	while(limit > 1) {
		fseek(in, 0, SEEK_SET);
		fseek(out, 0, SEEK_SET);
		
		for(int i = 0; i < limit - (limit&1); i += 2) {
			__sort(in, i*len, (i+1)*len, min(size, (i+2)*len), out);
		}

		do{
			cnt = fread(C, 4, maxNum_Obj*2, in);
			fwrite(C, 4, cnt, out);
		}while(cnt);	

		limit -= limit / 2;

		len <<= 1;
		std::swap(in, out);
		++fanzhuan;
	}
	if(fanzhuan & 1)
		std::swap(in, out);
}

#endif
