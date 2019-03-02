#include <cstdint>

class Image {
public:
	Image(int, int);
	int getX() const;
	int getY() const;
	uint8_t getPixel(int, int) const;
	uint8_t findMedian() const;
	bool fill(int, int, uint8_t);
	void write(int);
	void freeMem();

private:
	int xres = 0, yres = 0;
	uint8_t *image;
};

void imageShrink(const Image *img, Image *img_ret);
