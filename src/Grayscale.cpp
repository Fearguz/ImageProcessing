#include "Grayscale.hpp"

using namespace IPTypes;
using namespace IPStructures;

namespace IPAlgorithms
{

ImageMatrix<1> Grayscale::operator()(ImageMatrix<3> &input, Grayscale::ConversionType type)
{
	size_t rows = input.rows(), cols = input.cols();
	ImageMatrix<1> output(rows, cols);

	Channel& r = input.R();
	Channel& g = input.G();
	Channel& b = input.B();
	Channel& outGray = output.grayscale();

	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
		{
			if (type == GrayscaleConversion_Average)
			{
				outGray[i][j] = (r[i][j] + g[i][j] + b[i][j]) / 3;
			}
			else if (type == GrayscaleConversion_Lightness)
			{
				Byte min, max;
				calcMinMax(r[i][j], g[i][j], b[i][j], &min, &max);
				outGray[i][j] = (min + max) / 2;
			}
			else if (type == GrayscaleConversion_Luminosity)
			{
				outGray[i][j] = (0.21 * r[i][j] + 0.72 * g[i][j] + 0.07 * b[i][j]);
			}
			else if (type == GrayscaleConversion_Decomposition_Max)
			{
				Byte min, max;
				calcMinMax(r[i][j], g[i][j], b[i][j], &min, &max);
				outGray[i][j] = max;
			}
			else if (type == GrayscaleConversion_Decomposition_Min)
			{
				Byte min, max;
				calcMinMax(r[i][j], g[i][j], b[i][j], &min, &max);
				outGray[i][j] = min;
			}
		}
	}

	return output;
}

void Grayscale::calcMinMax(Byte r, Byte g, Byte b, Byte *min, Byte *max)
{
	Byte minimum = r, maximum = r;
	minimum = std::min<Byte>(minimum, g);
	minimum = std::min<Byte>(minimum, b);

	maximum = std::max<Byte>(maximum, g);
	maximum = std::max<Byte>(maximum, b);

	if (min)
		*min = minimum;

	if (max)
		*max = maximum;
}

}
