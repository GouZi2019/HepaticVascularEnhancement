#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "itkHepaticVascularPreproccessFilter.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0] << std::endl;
		std::cerr << "<InputLabelImage> <OutputLabelImage>" << std::endl;
		return EXIT_FAILURE;
	}

	typedef float																	PixelType;
	typedef typename itk::Image<PixelType, 3>										ImageType;
	typedef typename itk::ImageFileReader<ImageType>								ImageReaderType;
	typedef typename itk::ImageFileWriter<ImageType>								ImageWriterType;

	/** Read the image. */
	ImageReaderType::Pointer reader = ImageReaderType::New();
	reader->SetFileName(argv[1]);
	try
	{
		reader->Update();
	}
	catch (itk::ExceptionObject &err)
	{
		std::cerr << "Could not read the image!" << std::endl;
		std::cerr << err << std::endl;
		exit(EXIT_FAILURE);
	}

	/** Use the filter. */
	typedef typename itk::HepaticVascularPreproccessFilter<ImageType>				FilterType;
	FilterType::Pointer filter = FilterType::New();
	filter->SetInput(reader->GetOutput());
	filter->Update();

	///** Write the output image. */
	//ImageWriterType::Pointer writer = ImageWriterType::New();
	//writer->SetFileName(argv[2]);
	//writer->SetInput(reader->GetOutput());
	//writer->UseCompressionOn();
	//try
	//{
	//	writer->Update();
	//}
	//catch (itk::ExceptionObject &err)
	//{
	//	std::cerr << "Could not write the image!" << std::endl;
	//	std::cerr << err << std::endl;
	//	exit(EXIT_FAILURE);
	//}
	
	system("pause");
	exit(EXIT_SUCCESS);
}