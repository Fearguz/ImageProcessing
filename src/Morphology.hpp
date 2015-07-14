#pragma once

#include "ImageMatrix.hpp"

namespace IPAlgorithms
{

class Morphology
{
	public:
		void dilate(IPStructures::ImageMatrix<1> &input, IPStructures::ImageMatrix<1> &output, int k = 1);

		void erode(IPStructures::ImageMatrix<1> &input, IPStructures::ImageMatrix<1> &output, int k = 1);

		void open(IPStructures::ImageMatrix<1> &input, IPStructures::ImageMatrix<1> &output);
};

}
