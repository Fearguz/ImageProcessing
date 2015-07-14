#include "Segmentation.hpp"

using namespace IPTypes;
using namespace IPStructures;

namespace IPAlgorithms
{

const int Segmentation::dx[] = {+1, 0, -1, 0};
const int Segmentation::dy[] = {0, +1, 0, -1};

ImageMatrix<1> Segmentation::connectedComponents(ImageMatrix<1> &input)
{
	size_t row_count = input.rows();
	size_t col_count = input.cols();
	m_labels = std::vector<std::vector<int> >(row_count, std::vector<int>(col_count));

	Channel &in = input.grayscale();

	int component = 0;
	for (size_t i = 0; i < row_count; ++i)
	{
	    for (size_t j = 0; j < col_count; ++j)
	    {
	    	if (!m_labels[i][j] && in[i][j])
	    	{
	    		dfs(i, j, row_count, col_count, ++component, input);
	    	}
	    }
	}

	int delta = 255 / component;

	ImageMatrix<1> output(row_count, col_count);
	Channel &out = output.grayscale();
	for (size_t i = 0; i < row_count; ++i)
	{
		for (size_t j = 0; j < col_count; ++j)
		{
			int comp = m_labels[i][j];
			out[i][j] = comp * delta;
		}
	}

	return output;
}

void Segmentation::dfs(size_t x, size_t y, size_t h, size_t w, int currLabel, ImageMatrix<1> &input)
{
	if (x < 0 || x >= h)
	{
		return;
	}
	if (y < 0 || y >= w)
	{
		return;
	}

	Channel &in = input.grayscale();
	if (m_labels[x][y] || !in[x][y])
	{
		return;
	}

	m_labels[x][y] = currLabel;
	for (int dir = 0; dir < 4; ++dir)
	{
		dfs(x + dx[dir], y + dy[dir], h, w, currLabel, input);
	}
}

}
