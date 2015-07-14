#pragma once

#include <vector>
#include <cstdlib>

namespace IPTypes
{
	typedef unsigned char Byte;
	typedef std::vector<std::vector<Byte> > Channel;
}

namespace IPStructures
{

template <int Channels = 1>
class ImageMatrix
{
	private:
		IPTypes::Channel m_channels[Channels];
		size_t m_rows, m_cols;

	public:
		void reserve(size_t rows, size_t cols)
		{
			m_rows = rows;
			m_cols = cols;
			for (int chIdx = 0; chIdx < Channels; ++chIdx)
			{
				m_channels[chIdx] = IPTypes::Channel(rows, std::vector<IPTypes::Byte>(cols));
			}
		}

		ImageMatrix() : m_channels(), m_rows(0), m_cols(0) { }
		ImageMatrix(size_t rows, size_t cols) : m_channels(), m_rows(0), m_cols(0) { reserve(rows, cols); }
		ImageMatrix(const ImageMatrix& oth) : m_channels(oth.m_channels), m_rows(oth.m_rows), m_cols(oth.m_cols) { }
		ImageMatrix(ImageMatrix&& oth) : m_channels(std::move(oth.m_channels)), m_rows(std::move(oth.m_rows)), m_cols(std::move(oth.m_cols)) { }
		~ImageMatrix() { }

		size_t cols() const { return m_cols; }

		size_t rows() const { return m_rows; }

		template<int Channel>
		IPTypes::Channel& getChannel()
		{
			return m_channels[Channel];
		}

		IPTypes::Channel& grayscale() { return getChannel<0>(); }

		IPTypes::Channel& R() { return getChannel<0>(); }

		IPTypes::Channel& G() { return getChannel<1>(); }

		IPTypes::Channel& B() { return getChannel<2>(); }
};

class MatrixFactory
{
	public:
		static ImageMatrix<3> getImageMatrix3D(size_t rows, size_t cols)
		{
			ImageMatrix<3> matrix(rows, cols);

			IPTypes::Channel& r = matrix.R();
			IPTypes::Channel& g = matrix.G();
			IPTypes::Channel& b = matrix.B();
			for (size_t row = 0; row < rows; ++row)
			{
				for (size_t col = 0; col < cols; ++col)
				{
					r[row][col] = rand() % 255;
					g[row][col] = rand() % 255;
					b[row][col] = rand() % 255;
				}
			}
			return matrix;
		}

		static ImageMatrix<1> getImageMatrix1D(size_t rows, size_t cols)
		{
			ImageMatrix<1> matrix(rows, cols);

			IPTypes::Channel& gray = matrix.grayscale();
			for (size_t row = 0; row < rows; ++row)
			{
				for (size_t col = 0; col < cols; ++cols)
				{
					gray[row][col] = rand() % 255;
				}
			}
			return matrix;
		}
};

}
