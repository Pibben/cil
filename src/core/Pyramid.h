/*
 * Pyramid.h
 *
 *  Created on: Apr 4, 2012
 *      Author: per
 */

#ifndef PYRAMID_H_
#define PYRAMID_H_

namespace cil {

template <typename T>
class Pyramid {
private:
	std::vector<Image<T> > mImages;

	static T avg(T a, T b) { return (a+b+1)/2; }
	static T avg(T a, T b, T c, T d) { return (a+b+c+d+2)/4; }

	static void shrink(const Image<T>& src, Image<T>& dst) {

		const bool oddCols = (src.getSizeX() & 1);
		const bool oddRows = (src.getSizeY() & 1);

		for(int i = 0; i < src.getNumChannels(); ++i) {

			auto srcRow  = src.beginRow(i);
			auto srcRow2 = srcRow+1;

			for(auto dstRow = dst.beginRow(i); dstRow != (oddRows ? dst.endRow(i)-1 : dst.endRow(i)); ++dstRow) {

				auto srcPtr  = srcRow.begin();
				auto srcPtr2 = srcRow2.begin();

				for(auto dstPtr = dstRow.begin(); dstPtr != (oddCols ? dstRow.end()-1 : dstRow.end()); ++dstPtr) {
					T a = *srcPtr++;
					T b = *srcPtr++;
					T c = *srcPtr2++;
					T d = *srcPtr2++;
					*dstPtr = avg(a, b, c, d);
					//printf("%d %d %d %d -> %d\n", a, b, c, d, *dstPtr);
				}

				srcRow++;  srcRow++;
				if(srcRow2 != src.endRow(i))
					srcRow2++;
				if(srcRow2 != src.endRow(i))
					srcRow2++;
			}

			// Handle odd column
			if(oddCols) {
				auto dstColIter = dst.begin() + dst.getSizeX() - 1;
				auto srcColIter = src.begin() + src.getSizeX() - 1;
				auto dstColEnd = dst.end() - 1 + (oddRows ? 0 : dst.getSizeX());

				while(dstColIter != dstColEnd) {
					T a = *srcColIter;
					srcColIter += src.getSizeX();
					T b = *srcColIter;
					*dstColIter = avg(a,b);
					//printf("%d %d -> %d\n", a, b, *dstColIter);
					if(dstColIter != dstColEnd) {
						srcColIter += src.getSizeX();
						dstColIter += dst.getSizeX();
					}
				}
			}

			// Handle odd row
			if(oddRows) {
				auto dstRowIter = dst.end() - dst.getSizeX();
				auto srcRowIter = src.end() - src.getSizeX();
				auto dstRowEnd = dst.end() - (oddCols ? 1 : 0);

				while(dstRowIter != dstRowEnd) {
					T a = *srcRowIter;
					srcRowIter++;
					T b = *srcRowIter;
					*dstRowIter = avg(a,b);
					//printf("%d %d -> %d\n", a, b, *dstRowIter);
					if(dstRowIter != dstRowEnd) {
						srcRowIter++;
						dstRowIter++;
					}
				}
			}

			// Handle odd pixel
			if(oddCols && oddRows) {
				*(dst.end()-1) = *(src.end()-1);
				//printf("%d -> %d\n", *(dst.end()-1), *(dst.end()-1));
			}
		}
	}

public:
	Pyramid(const Image<T>& image, uint8_t levels) {
		mImages.push_back(image);
		for(int i = 1; i < levels; ++i) {
			uint16_t sizeX = mImages[i-1].getSizeX();
			uint16_t sizeY = mImages[i-1].getSizeY();

			mImages.emplace_back((sizeX + 1) / 2, (sizeY + 1) / 2, image.getNumChannels(), 0);
			shrink(mImages[i-1], mImages[i]);
		}
	}

	Image<T> renderAll() {
		Image<T> retval(mImages[0].getSizeX() + (mImages.size() > 1 ? mImages[1].getSizeX() : 0), mImages[0].getSizeY(), mImages[0].getNumPlanes(), 0);

		retval.fill(127);

		Image<T> sub;

		int originX = 0;
		int originY = 0;

		unsigned int i = 0;

		for(;;) {

			sub = retval.subImage(originX, originY, originX + mImages[i].getSizeX()-1, originY + mImages[i].getSizeY()-1);
			sub.copyFrom(mImages[i]);

			if(mImages.size() == i+1) break;
			originX += mImages[i].getSizeX();
			i++;

			sub = retval.subImage(originX, originY, originX + mImages[i].getSizeX()-1, originY + mImages[i].getSizeY()-1);
			sub.copyFrom(mImages[i]);

			if(mImages.size() == i+1) break;
			originX += (mImages[i].getSizeX() - mImages[i+1].getSizeX());
			originY += mImages[i].getSizeY();
			i++;

			sub = retval.subImage(originX, originY, originX + mImages[i].getSizeX()-1, originY + mImages[i].getSizeY()-1);
			sub.copyFrom(mImages[i]);

			if(mImages.size() == i+1) break;
			originX -= mImages[i+1].getSizeX();
			originY += (mImages[i].getSizeY() - mImages[i+1].getSizeY());
			i++;

			sub = retval.subImage(originX, originY, originX + mImages[i].getSizeX()-1, originY + mImages[i].getSizeY()-1);
			sub.copyFrom(mImages[i]);

			if(mImages.size() == i+1) break;
			originY -= mImages[i+1].getSizeX();
			i++;
		}

		return retval;
	}

	Image<T>& operator[](uint8_t idx) { return mImages[idx]; }
};

TEST(ShrinkTest, 44) {
	unsigned char data44[4*4] = {0, 10, 20, 30,
						        1, 11, 21, 31,
						        2, 12, 22, 32,
						        3, 13, 23, 33};

	Image<unsigned char> i44(4, 4, 4, data44, false);

	Pyramid<unsigned char> p1(i44, 2);
	Image<unsigned char>& p44s = p1[1];

	EXPECT_EQ(p44s.getSizeX(), 2);
	EXPECT_EQ(p44s.getSizeY(), 2);

	EXPECT_EQ(p44s.at(0, 0), 6);
	EXPECT_EQ(p44s.at(1, 0), 26);
	EXPECT_EQ(p44s.at(0, 1), 8);
	EXPECT_EQ(p44s.at(1, 1), 28);
}

TEST(ShrinkTest, 54) {
	unsigned char data45[4*5] = {0, 10, 20, 30, 40,
						         1, 11, 21, 31, 41,
						         2, 12, 22, 32, 42,
						         3, 13, 23, 33, 43};

	Image<unsigned char> i54(5, 4, 5, data45, false);

	Pyramid<unsigned char> p2(i54, 2);
	Image<unsigned char>& p54s = p2[1];

	EXPECT_EQ(p54s.getSizeX(), 3);
	EXPECT_EQ(p54s.getSizeY(), 2);

	EXPECT_EQ(p54s.at(0, 0), 6);
	EXPECT_EQ(p54s.at(0, 1), 8);
	EXPECT_EQ(p54s.at(1, 0), 26);
	EXPECT_EQ(p54s.at(1, 1), 28);
	EXPECT_EQ(p54s.at(2, 0), 41);
	EXPECT_EQ(p54s.at(2, 1), 43);
}

TEST(ShrinkTest, 55) {
	unsigned char data55[5*5] = {0, 10, 20, 30, 40,
						         1, 11, 21, 31, 41,
						         2, 12, 22, 32, 42,
						         3, 13, 23, 33, 43,
						         4, 14, 24, 34, 44};

	Image<unsigned char> i55(5, 5, 5, data55, false);

	Pyramid<unsigned char> p3(i55, 2);
	Image<unsigned char>& p55s = p3[1];

	EXPECT_EQ(p55s.getSizeX(), 3);
	EXPECT_EQ(p55s.getSizeY(), 3);

	EXPECT_EQ(p55s.at(0, 0), 6);
	EXPECT_EQ(p55s.at(1, 0), 26);
	EXPECT_EQ(p55s.at(2, 0), 41);
	EXPECT_EQ(p55s.at(0, 1), 8);
	EXPECT_EQ(p55s.at(1, 1), 28);
	EXPECT_EQ(p55s.at(2, 1), 43);
	EXPECT_EQ(p55s.at(0, 2), 9);
	EXPECT_EQ(p55s.at(1, 2), 29);
	EXPECT_EQ(p55s.at(2, 2), 44);
}

};//ns
#endif /* PYRAMID_H_ */
