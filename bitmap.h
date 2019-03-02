class Bitmap {
public:
	Bitmap(int _xres, int _yres);
	int getX() const;
	int getY() const;
	int getCol() const;
	int getPixel(int, int) const;
	unsigned getUnsigned(int, int) const;
	void fillPixel(int, int, unsigned);
	void fillUnsigned(int, int, unsigned);
	void freeMem();

private:
	int xres = 0, yres = 0, n_col = 0;
	unsigned *bitmap;
};

void bitmapShift(const Bitmap *bm, int xo, int yo, Bitmap *bm_ret);
void bitmapXOR(const Bitmap *bm1, const Bitmap *bm2, Bitmap *bm_ret);
void bitmapAND(const Bitmap *bm1, const Bitmap *bm2, Bitmap *bm_ret);
int bitmapTotal(const Bitmap *bm);
