#pragma once

#include "ImageMatrix.hpp"

namespace IPAlgorithms
{

class Segmentation
{
	private:
		std::vector<std::vector<int> > m_labels;

		static const int dx[];
		static const int dy[];

	public:
		IPStructures::ImageMatrix<1> connectedComponents(IPStructures::ImageMatrix<1> &input);

	private:
		void dfs(size_t x, size_t y, size_t w, size_t h, int currLabel, IPStructures::ImageMatrix<1> &input);
};

}
