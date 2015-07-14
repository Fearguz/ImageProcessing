#include "GaussianBlur.hpp"

#include <cmath>
#include <algorithm>

using namespace IPTypes;
using namespace IPStructures;

namespace IPAlgorithms
{

const double GaussianBlur::PI = 3.14159265359;

ImageMatrix<1> GaussianBlur::operator()(ImageMatrix<1> &input, size_t radius, GaussianBlur::BlurType type)
{
	ImageMatrix<1> output(input.rows(), input.cols());

	if (type == PerfectBlur)
	{
		perfectGaussianBlur(input, output, radius);
	}
	else if (type == FastBlur)
	{
		Channel &in = input.grayscale();

		std::vector<int> source;
		for (size_t i = 0; i < input.rows(); ++i)
		{
			for (size_t j = 0; j < input.cols(); ++j)
			{
				source.push_back(in[i][j]);
			}
		}

		std::vector<int> target;
		size_t len = input.rows() * input.cols();
		for (size_t i = 0; i < len; ++i)
		{
			target.push_back(0);
		}

		fastGaussianBlur(source, target, input.cols(), input.rows(), radius);

		Channel &out = output.grayscale();
		len = target.size();
		for (size_t i = 0; i < len; ++i)
		{
			int row = i / input.cols();
			int col = i % input.cols();
			out[row][col] = target[i];
		}
	}

	return output;
}

void GaussianBlur::perfectGaussianBlur(ImageMatrix<1> &input, ImageMatrix<1> &output, size_t radius)
{
	size_t w = input.cols(), h = input.rows();

	Channel& inputGray = input.grayscale();
	Channel& outputGray = output.grayscale();

	int sigRadius = std::ceil(radius * 2.57f);
	for (size_t i = 0; i < h; ++i)
	{
		for (size_t j = 0; j < w; ++j)
		{
			float val = 0, weightSum = 0;
			for (size_t iy = i - sigRadius; iy < i + sigRadius + 1; ++iy)
			{
				for (size_t ix = j - sigRadius; ix < j + sigRadius + 1; ++ix)
				{
					int x = std::min((int)(w - 1), std::max(0, (int)ix));
					int y = std::min((int)(h - 1), std::max(0, (int)iy));
					float dsq = (ix - j) * (ix - j) + (iy - i) * (iy - i);
					float weight = std::exp((-dsq) / (2 * radius * radius)) / (PI * 2 * radius * radius);

					val += inputGray[y][x] * weight;
					weightSum += weight;
				}
			}
			outputGray[i][j] = round(val / weightSum);
		}
	}
}

void GaussianBlur::fastGaussianBlur(std::vector<int> &input, std::vector<int> &output, size_t width, size_t height, size_t radius)
{
	std::vector<int> boxes = boxesForGauss(radius, 3);
	boxBlur(input, output, width, height, (boxes[0]-1) / 2);
	boxBlur(output, input, width, height, (boxes[1]-1) / 2);
	boxBlur(input, output, width, height, (boxes[2]-1) / 2);
}

void GaussianBlur::boxBlur(std::vector<int> &input, std::vector<int> &output, size_t width, size_t height, size_t radius)
{
	size_t len = input.size();
	for (size_t i = 0; i < len; ++i)
	{
		output[i] = input[i];
	}
	boxBlurH(output, input, width, height, radius);
	boxBlurV(input, output, width, height, radius);
}

void GaussianBlur::boxBlurH(std::vector<int> &input, std::vector<int> &output, size_t width, size_t height, size_t radius)
{
	double iarr = ((double)1 / (radius + radius + 1));
	for (size_t i = 0; i < height; ++i)
	{
		size_t ti = i * width, li = ti, ri = ti + radius;
		int fv = input[ti], lv = input[ti + width - 1];
		size_t val = (radius + 1) * fv;
		for (size_t j = 0; j < radius; ++j) val += input[ti + j];

		for (size_t j = 0; j <= radius; ++j) { val += input[ri++] - fv; output[ti++] = (Byte)round(val * iarr); }

		for (size_t j = radius + 1; j < width - radius; ++j) { val += input[ri++] - input[li++]; output[ti++] = (Byte)round(val * iarr); }

		for (size_t j = width - radius; j < width; ++j) { val += lv - input[li++]; output[ti++] = (Byte)round(val * iarr); }
	}
}

void GaussianBlur::boxBlurV(std::vector<int> &input, std::vector<int> &output, size_t width, size_t height, size_t radius)
{
	double iarr = ((double)1 / (radius + radius + 1));
	for (size_t i = 0; i < width; ++i)
	{
		size_t ti = i, li = ti, ri = ti + radius * width;
		int fv = input[ti], lv = input[ti + width * (height - 1)], val = (radius + 1) * fv;
		for (size_t j = 0; j < radius; ++j) val += input[ti + j * width];

		for (size_t j = 0; j <= radius; ++j) { val += input[ri] - fv; output[ti] = (Byte)round(val * iarr); ri += width; ti += width; }

		for (size_t j = radius + 1; j < height - radius; ++j) { val += input[ri] - input[li]; output[ti] = (Byte)round(val * iarr); li += width; ri += width; ti += width; }

		for (size_t j = height - radius; j < height; ++j) { val += lv - input[li]; output[ti] = (Byte)round(val * iarr); li += width; ti += width; }
	}
}

std::vector<int> GaussianBlur::boxesForGauss(int sigma, int boxes)
{
	int wIdeal = sqrt((12 * sigma * sigma / boxes) + 1);
	int wl = floor(wIdeal);
	if (wl % 2 == 0)
	{
		--wl;
	}
	int wu = wl + 2;

	float mIdeal = (12 * sigma * sigma - boxes * wl * wl - 4 * boxes * wl - 3 * boxes) / (-4 * wl - 4);
	int m = round(mIdeal);

	std::vector<int> sizes;
	for (int i = 0; i < boxes; ++i)
	{
		sizes.push_back(i < m ? wl : wu);
	}
	return sizes;
}

}

