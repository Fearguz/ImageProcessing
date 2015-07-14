#include "Threshold.hpp"

using namespace IPTypes;
using namespace IPStructures;

namespace IPAlgorithms
{

int Threshold::globalThreshold(ImageMatrix<1> &input)
{
	Channel &in = input.grayscale();
	size_t totalPixels = input.rows() * input.cols();

	double histogram[256] = { 0 };
	for (size_t  i = 0; i < input.rows(); ++i)
	{
		for (size_t j = 0; j < input.cols(); ++j)
		{
			if (j < input.cols() - 2)
			{
				double tmp = 0.114 * in[i][j] + 0.587 * in[i][j+1] + 0.299 * in[i][j+2];
				tmp = tmp < 0 ? 0 : tmp;
				tmp = tmp > 255 ? 255 : tmp;
				histogram[(int)tmp]++;
			}
		}
	}

	double N0 = 0.0, N1 = 0.0;
	double tmpG = -1.0;
	int threshold = 0;
	for (int i = 0; i < 256; ++i)
	{
		double gSum0 = 0;
		double gSum1 = 0;
		N0 += histogram[i];
		N1 = totalPixels - N0;
		if (N1 == 0)
		{
			break;
		}

		double w0 = N0 / totalPixels;
		double w1 = 1 - w0;
		for (int j = 0; j <= i; ++j)
		{
			gSum0 += j * histogram[j];
		}
		double u0 = gSum0 / N0;
		for (int k = i + 1; k < 256; ++k)
		{
			gSum1 += k * histogram[k];
		}
		double u1 = gSum1 / N1;
		double g = w0 * w1 * (u0 - u1) * (u0 - u1);
		if (tmpG < g)
		{
			tmpG = g;
			threshold = i;
		}
	}

	return threshold;
}

ImageMatrix<1> Threshold::adaptiveThreshold(ImageMatrix<1> &input, int radius, Threshold::Type type, int globalThreshold)
{
	size_t rows = input.rows(), cols = input.cols();
	ImageMatrix<1> output(rows, cols);

	Channel &in = input.grayscale();
	Channel &out = output.grayscale();

	std::vector<std::vector<int> > set_back, set_obj, card_back, card_obj;
	set_back = std::vector<std::vector<int> >(rows, std::vector<int>(cols));
	set_obj = std::vector<std::vector<int> >(rows, std::vector<int>(cols));
	card_back = std::vector<std::vector<int> >(rows, std::vector<int>(cols));
	card_obj = std::vector<std::vector<int> >(rows, std::vector<int>(cols));

	for (size_t ir = 1; ir < rows; ++ir)
	{
		for (size_t ic = 1; ic < cols; ++ic)
		{
			int gray_val = in[ir][ic];
			if (gray_val <= globalThreshold)
			{
				set_back[ir][ic] = set_back[ir][ic - 1] + set_back[ir - 1][ic] - set_back[ir - 1][ic - 1] + in[ir][ic];
				card_back[ir][ic] = card_back[ir][ic - 1] + card_back[ir - 1][ic] - card_back[ir - 1][ic - 1] + 1;
				set_obj[ir][ic] = set_obj[ir][ic - 1] + set_obj[ir - 1][ic]	- set_obj[ir - 1][ic - 1];
				card_obj[ir][ic] = card_obj[ir][ic - 1] + card_obj[ir - 1][ic] - card_obj[ir - 1][ic - 1];
			}
			else
			{
				set_back[ir][ic] = set_back[ir][ic - 1] + set_back[ir - 1][ic] - set_back[ir - 1][ic - 1];
				card_back[ir][ic] = card_back[ir][ic - 1] + card_back[ir - 1][ic] - card_back[ir - 1][ic - 1];
				set_obj[ir][ic] = set_obj[ir][ic - 1] + set_obj[ir - 1][ic] - set_obj[ir - 1][ic - 1] + in[ir][ic];
				card_obj[ir][ic] = card_obj[ir][ic - 1] + card_obj[ir - 1][ic] - card_obj[ir - 1][ic - 1] + 1;
			}
		}
	}

	 int r_begin = radius + 1;
	 int r_end = rows - radius;
	 int c_begin = radius + 1;
	 int c_end = cols - radius;

	 for (int ir = r_begin; ir < r_end; ++ir)
	 {
		 for (int ic = c_begin; ic < c_end; ++ic)
		 {
			 int left = ic - radius;
			 int top = ir - radius;
			 int right = ic + radius;
			 int bottom = ir + radius;

			 int sum_back = set_back[bottom][right] - set_back[bottom][left - 1] - set_back[top - 1][right] + set_back[top - 1][left - 1];
			 int num_back = card_back[bottom][right] - card_back[bottom][left - 1] - card_back[top - 1][right] + card_back[top - 1][left - 1];
			 int sum_obj = set_obj[bottom][right] - set_obj[bottom][left - 1] - set_obj[top - 1][right] + set_obj[top - 1][left - 1];
			 int num_obj = card_obj[bottom][right] - card_obj[bottom][left - 1] - card_obj[top - 1][right] + card_obj[top - 1][left - 1];

			 double mean_back = ( num_back == 0 ? 0.0 : ( sum_back / ( double ) num_back ) );
			 double mean_obj = ( num_obj == 0 ? 0.0 : ( sum_obj / ( double ) num_obj ) );

			 int local_threshold = 0.5 * ( (mean_back + mean_obj) );
			 if (type == Inverted)
			 {
				 out[ir][ic] = ( ( in[ir][ic] > local_threshold ) ? 255 : 0 );
			 }
			 else if (type == Normal)
			 {
				 out[ir][ic] = ( ( in[ir][ic] > local_threshold ) ? 0 : 255 );
			 }
		 }
	 }

	 return output;
}

}
