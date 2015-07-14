#include "Morphology.hpp"

using namespace IPTypes;
using namespace IPStructures;

namespace IPAlgorithms
{

void Morphology::open(ImageMatrix<1> &input, ImageMatrix<1> &output)
{
	ImageMatrix<1> eroded;
	erode(input, eroded);
	dilate(eroded, output);
}

void Morphology::erode(ImageMatrix<1> &input, ImageMatrix<1> &output, int k)
{
	size_t rows = input.rows(), cols = input.cols();
	output.reserve(rows, cols);

	Channel &in = input.grayscale();
	Channel &out = output.grayscale();

	for (int retries = 0; retries < k; ++retries)
	{
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < cols; ++j)
			{
				if (in[i][j] == 0)
				{
					if (i > 0 && in[i-1][j] == 255)
					{
						out[i-1][j] = 2;
					}
					if (j > 0 && in[i][j-1] == 255)
					{
						out[i][j-1] = 2;
					}
					if (i + 1 < rows && in[i + 1][j] == 255)
					{
						out[i+1][j] = 2;
					}
					if (j + 1 < cols && in[i][j+1] == 255)
					{
						out[i][j+1] = 2;
					}
					out[i][j] = 2;
				}
			}
		}
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < cols; ++j)
			{
				if (out[i][j] == 2)
				{
					out[i][j] = 0;
				}
				else
				{
					out[i][j] = 255;
				}
			}
		}
	}
}

void Morphology::dilate(ImageMatrix<1> &input, ImageMatrix<1> &output, int k)
{
	size_t rows = input.rows(), cols = input.cols();
	output.reserve(rows, cols);

	Channel &in = input.grayscale();
	Channel &out = output.grayscale();

	for (int retries = 0; retries < k; ++retries)
	{
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < cols; ++j)
			{
				if (in[i][j] == 255)
				{
					if (i > 0 && in[i-1][j] == 0)
					{
						out[i-1][j] = 2;
					}
					if (j > 0 && in[i][j-1] == 0)
					{
						out[i][j-1] = 2;
					}
					if (i + 1 < rows && in[i + 1][j] == 0)
					{
						out[i+1][j] = 2;
					}
					if (j + 1 < cols && in[i][j+1] == 0)
					{
						out[i][j+1] = 2;
					}
					out[i][j] = 2;
				}
			}
		}
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < cols; ++j)
			{
				if (out[i][j] == 2)
				{
					out[i][j] = 255;
				}
				else
				{
					out[i][j] = 0;
				}
			}
		}
	}
}

}
