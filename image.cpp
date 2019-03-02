#include <cstdlib>
#include <cstdint>
#include <cstring>
#include "image.h"

Image::Image(int x, int y) {
	xres = x;
	yres = y;
	image = (uint8_t*) malloc(sizeof(uint8_t) * x * y);
}

int Image::getX() const {
	return xres;
}

int Image::getY() const {
	return yres;
}

uint8_t Image::getPixel(int x, int y) const {
	if (x >= 0 and y >= 0 and x < xres and y < yres) {
		return image[y*xres + x];
	}
	return 0;
}

uint8_t findKth(uint8_t *list, int k, int head, int tail) {
	int smaller = head, pivot;
	for (int i = tail-1; i != smaller;) {
		if (list[i] < list[tail]) {
			uint8_t tmp = list[smaller];
			list[smaller] = list[i];
			list[i] = tmp;
			smaller++;
		} else
			i--;
	}
	if (list[tail] > list[smaller]) {
		uint8_t tmp = list[smaller+1];
		list[smaller+1] = list[tail];
		list[tail] = tmp;
		pivot = smaller+1;
	} else {
		uint8_t tmp = list[smaller];
		list[smaller] = list[tail];
		list[tail] = tmp;
		pivot = smaller;
	}
	if (k == (pivot - head + 1)) return list[pivot];
	else if (k <= (pivot - head)) return findKth(list, k, head, pivot-1);
	else return findKth(list, (k-pivot+head-1), pivot+1, tail);
}

uint8_t Image::findMedian() const{
	uint8_t *list = (uint8_t*) malloc(sizeof(uint8_t) * xres * yres);
	memcpy(list, image, sizeof(uint8_t) * xres * yres);
	uint8_t median = findKth(list, xres*yres/2, 0, xres*yres);
	//free(list);
	return median;
}

bool Image::fill(int x, int y, uint8_t value) {
	if (x >= 0 and y >= 0 and x < xres and y < yres) {
		image[y*xres+x] = value;
		return true;
	}
	return false;
}

void Image::freeMem() {
	xres = 0;
	yres = 0;
	free(image);
}

void imageShrink(const Image *img, Image *img_ret) {
	int xres = img->getX(), yres = img->getY();
	for (int i = 0; i < xres; i+=2) {
		for (int j = 0; j < yres; j+=2) {
			unsigned pixel = (img->getPixel(i, j), img->getPixel(i, j+1), img->getPixel(i+1, j), img->getPixel(i+1, j+1)) / 4;
			img_ret->fill(i/2, j/2, (uint8_t) pixel);
		}
	}
}
