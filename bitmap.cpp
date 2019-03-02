#include <cstdlib>
#include "bitmap.h"

Bitmap::Bitmap(int _xres, int _yres) {
	xres = _xres;
	yres = _yres;
	n_col = (xres/32) + ((xres%32 == 0) ? 0 : 1);
	bitmap = (unsigned*) malloc(sizeof(unsigned) * n_col * yres);
}

int Bitmap::getX() const {
	return xres;
}

int Bitmap::getY() const {
	return yres;
}

int Bitmap::getCol() const {
	return n_col;
}

int Bitmap::getPixel(int x, int y) const {
	if (x >= xres or y >= yres or x < 0 or y < 0)
		return -1; // out of bound
	else {
		unsigned ret = bitmap[(x/32) + (y * n_col)];
		ret = ret >> x;
		ret &= 1;
		return (int) ret;
	}
}

unsigned Bitmap::getUnsigned(int col, int row) const{
	return bitmap[col + row*n_col];
}

void Bitmap::fillPixel(int x, int y, unsigned value) {
	if (x >= xres or y >= yres or x < 0 or y < 0)
		return;
	int offset = x/32 + y*n_col;
	unsigned tmp = bitmap[offset];
	tmp ^= (1 << (x%32));
	bitmap[offset] = tmp & (value << (x%32));
}

void Bitmap::fillUnsigned(int col, int row, unsigned value) {
	if (col >= n_col or row >= yres or col < 0 or row < 0)
		return;
	bitmap[col + row*n_col] = value;
}

void Bitmap::freeMem() {
	xres = 0;
	yres = 0;
	free(bitmap);
}

void bitmapShift(const Bitmap *bm, int xo, int yo, Bitmap *bm_ret) {
	int n_col = bm->getCol(), yres = bm->getY();
	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < n_col; i++) {
			if (j-yo < 0 or j-yo >= yres)
				bm_ret->fillUnsigned(i, j, (unsigned) 0);
			else if (xo == 0)
				bm_ret->fillUnsigned(i, j, bm->getUnsigned(i, j-yo));
			else if (xo == 1) {
				unsigned tmp = (bm->getUnsigned(i, j-yo) << 1) | ((i==0) ? 0 : bm->getPixel(i*32-1, j));
				bm_ret->fillUnsigned(i, j, tmp);
			} else if (xo == -1) {
				unsigned tmp = (bm->getUnsigned(i, j-yo) >> 1) | ((i==n_col-1) ? 0 : bm->getPixel(( i+1)*32, j) << 31);
				bm_ret->fillUnsigned(i, j, tmp);
			}
		}
	}
}

void bitmapXOR(const Bitmap *bm1, const Bitmap *bm2, Bitmap *bm_ret) {
	int n_col = bm1->getCol(), yres = bm1->getY();
	for (int j = 0; j < yres; j++)
		for (int i = 0; i < n_col; i++)
			bm_ret->fillUnsigned(i, j, (bm1->getUnsigned(i, j) ^ bm2->getUnsigned(i, j)));
}

void bitmapAND(const Bitmap *bm1, const Bitmap *bm2, Bitmap *bm_ret) {
	int n_col = bm1->getCol(), yres = bm1->getY();
	for (int j = 0; j < yres; j++)
		for (int i = 0; i < n_col; i++)
			bm_ret->fillUnsigned(i, j, (bm1->getUnsigned(i, j) & bm2->getUnsigned(i, j)));
}

int bitmapTotal(const Bitmap *bm) {
	int xres = bm->getX(), yres = bm->getY();
	int count = 0;
	for (int j = 0; j < yres; j++)
		for (int i = 0; i < xres; i++)
			if (bm->getPixel(i, j) == 0)
				count++;
	return count;
}

