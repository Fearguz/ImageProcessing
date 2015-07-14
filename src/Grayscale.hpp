#pragma once

#include "ImageMatrix.hpp"

namespace IPAlgorithms
{

class Grayscale
{
	public:
		enum ConversionType
		{
			GrayscaleConversion_Lightness = 0,
			GrayscaleConversion_Average,
			GrayscaleConversion_Luminosity,
			GrayscaleConversion_Decomposition_Max,
			GrayscaleConversion_Decomposition_Min
		};

		IPStructures::ImageMatrix<1> operator()(IPStructures::ImageMatrix<3> &src, ConversionType type);

	private:
		void calcMinMax(IPTypes::Byte r, IPTypes::Byte g, IPTypes::Byte b, IPTypes::Byte *min, IPTypes::Byte *max);
};

}
