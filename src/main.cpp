#include "ImageMatrix.hpp"
#include "Grayscale.hpp"
#include "GaussianBlur.hpp"
#include "Threshold.hpp"
#include "Morphology.hpp"
#include "Segmentation.hpp"

#include <ctime>

using namespace IPStructures;
using namespace IPAlgorithms;

int main(int, char**)
{
	srand(time(NULL));

	ImageMatrix<3> source = IPStructures::MatrixFactory::getImageMatrix3D(320, 240);

	ImageMatrix<1> gray;
	Grayscale grayscale;
	gray = grayscale(source, Grayscale::GrayscaleConversion_Lightness);

	ImageMatrix<1> blurred;
	GaussianBlur blurAlgorithm;
	blurred = blurAlgorithm(gray, 7, GaussianBlur::FastBlur);

	ImageMatrix<1> thresholded;
	Threshold thresholder;
	thresholded = thresholder.adaptiveThreshold(blurred, 11, Threshold::Inverted, thresholder.globalThreshold(gray));

	ImageMatrix<1> opened;
	Morphology morphologyOps;
	morphologyOps.open(thresholded, opened);

	ImageMatrix<1> output;
	Segmentation segmentationAlgorithm;
	output = segmentationAlgorithm.connectedComponents(opened);

	return 0;
}
