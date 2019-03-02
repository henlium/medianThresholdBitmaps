#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include "pgm.h"
#include "bitmap.h"
#include "image.h"
using namespace std;

void computeBitmaps(const Image *img, Bitmap *tb, Bitmap *eb) {
	uint8_t median = img->findMedian();
	int xres = img->getX(), yres = img->getY();
	for (int j = 0; j < yres; j++)
		for (int i = 0; i < xres; i++)
			if (img->getPixel(i, j) > median)
				tb->fillPixel(i, j, 1);
	for (int j = 0; j < yres; j++)
		for (int i = 0; i < xres; i++)
			eb->fillPixel(i, j, 1);
	for (int j = 0; j < yres; j++)
		for (int i = 0; i < xres; i++)
			if (img->getPixel(i, j) == median)
				for (int n = 0; n < 5; n++)
					for (int m = 0; m < 5; m++)
						eb->fillPixel(i+m, j+n, 0);
}

void getExpShift(const Image *img1, const Image *img2, int shift_bits, int shift_ret[2]) {
	int min_err;
	int cur_shift[2];
	int xres = img1->getX(), yres = img1->getY();
	Bitmap tb1 = Bitmap(xres, yres);
	Bitmap tb2 = Bitmap(xres, yres);
	Bitmap eb1 = Bitmap(xres, yres);
	Bitmap eb2 = Bitmap(xres, yres);
	if (shift_bits > 0) {
		Image shrinkedImage1 = Image(xres/2 + xres%2, yres/2 + yres%2);
		Image shrinkedImage2 = Image(xres/2 + xres%2, yres/2 + yres%2);
		imageShrink(img1, &shrinkedImage1);
		imageShrink(img2, &shrinkedImage2);
		getExpShift(&shrinkedImage1, &shrinkedImage2, shift_bits-1, cur_shift);
		shrinkedImage1.freeMem();
		shrinkedImage2.freeMem();
		cur_shift[0] *= 2;
		cur_shift[1] *= 2;
	} else {
		cur_shift[0] = 0;
		cur_shift[1] = 0;
	}
	computeBitmaps(img1, &tb1, &eb1);
	computeBitmaps(img2, &tb2, &eb2);
	min_err = img1->getX() * img1->getY();
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int xs = cur_shift[0] + i;
			int ys = cur_shift[1] + j;
			Bitmap shifted_tb2 = Bitmap(xres, yres);
			Bitmap shifted_eb2 = Bitmap(xres, yres);
			Bitmap diff_b = Bitmap(xres, yres);
			int err;
			bitmapShift(&tb2, xs, ys, &shifted_tb2);
			bitmapShift(&eb2, xs, ys, &shifted_eb2);
			bitmapXOR(&tb1, &shifted_tb2, &diff_b);
			bitmapAND(&diff_b, &eb1, &diff_b);
			bitmapAND(&diff_b, &shifted_eb2, &diff_b);
			err = bitmapTotal(&diff_b);
			if (err < min_err) {
				shift_ret[0] = xs;
				shift_ret[1] = ys;
				min_err = err;
			}
			shifted_tb2.freeMem();
			shifted_eb2.freeMem();
		}
	}
	printf("shift bit%d\n", shift_bits);
	tb1.freeMem();
	tb2.freeMem();
	eb1.freeMem();
	eb2.freeMem();
}

int main(int argc, char **argv)
{
	/* Read in arguments
	* and read in images */
	if (argc != 3) {
		printf("Usage: %s <image 0> <image 1>\n", argv[0]);
		abort();
	}
	bool suc;
	int w0, h0, c0, w1, h1, c1;
	auto src_img0 = ReadNetpbm(w0, h0, c0, suc, argv[1]);
	if (not suc) {
		puts("Something wrong while reading image 0.");
		abort();
	}
	auto src_img1 = ReadNetpbm(w1, h1, c1, suc, argv[2]);
	if (not suc) {
		puts("Something wrong while reading image 1.");
		abort();
	}
	if (w0 != w1 or h0 != h1) {
		puts("The images must have the same size.");
		abort();
	}

	/* Convert color image to gray scale */
	Image img0 = Image(w0, h0);
	Image img1 = Image(w1, h1);
	if (c0 == 3) {
		unsigned tmp;
		for (int i = 0; i < h0; i++) {
			for (int j = 0; j < w0; j++) {
				unsigned offset = i * w0 + j;
				offset *= 3;
				tmp = (54*src_img0[offset] + 183*src_img0[offset+1] + 19*src_img0[offset+2]) / 256;
				if (tmp > 255) tmp = 255;
				img0.fill(j, i, (uint8_t) tmp);
				tmp = (54*src_img1[offset] + 183*src_img1[offset+1] + 19*src_img1[offset+2]) / 256;
				if (tmp > 255) tmp = 255;
				img1.fill(j, i, (uint8_t) tmp);
			}
		}
	}

	int ret[2];
	getExpShift(&img0, &img1, 4, ret);
	printf("%d %d\n", ret[0], ret[1]);

	//WritePPM(o.get(), wb, hb, argv[6]);
	return 0;
}
