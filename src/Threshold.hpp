#pragma once

#include "ImageMatrix.hpp"

namespace IPAlgorithms
{

class Threshold
{
	public:
		enum Type {
			Normal = 0,
			Inverted
		};

		int globalThreshold(IPStructures::ImageMatrix<1> &input);

		IPStructures::ImageMatrix<1> adaptiveThreshold(IPStructures::ImageMatrix<1> &input, int radius, Type type, int globalThreshold = 128);
};

}
