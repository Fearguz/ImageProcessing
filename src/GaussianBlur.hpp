#pragma once

#include "ImageMatrix.hpp"
#include <vector>

namespace IPAlgorithms
{

class GaussianBlur
{
	private:
		static const double PI;

	public:
		enum BlurType
		{
			PerfectBlur = 0,
			FastBlur
		};

		IPStructures::ImageMatrix<1> operator()(IPStructures::ImageMatrix<1> &input, size_t radius, BlurType type);

	private:
		void perfectGaussianBlur(IPStructures::ImageMatrix<1> &input, IPStructures::ImageMatrix<1> &output, size_t radius);

		void fastGaussianBlur(std::vector<int> &input, std::vector<int> &output, size_t width, size_t height, size_t radius);

		void boxBlur(std::vector<int> &input, std::vector<int> &output, size_t width, size_t height, size_t radius);

		void boxBlurH(std::vector<int> &input, std::vector<int> &output, size_t width, size_t height, size_t radius);

		void boxBlurV(std::vector<int> &input, std::vector<int> &output, size_t width, size_t height, size_t radius);

		std::vector<int> boxesForGauss(int sigma, int n);
};

}
