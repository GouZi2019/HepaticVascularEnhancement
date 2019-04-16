#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
using namespace itk;

int ReadImageInformation(std::string);
template<int Dimension>
void DoSomething(char **);

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0] << std::endl;
		std::cerr << "<InputLabelImage> <OutputLabelImage>" << std::endl;
		return EXIT_FAILURE;
	}

	int Dimension;
	Dimension = ReadImageInformation(argv[1]);

	switch (Dimension)
	{
	case 2:DoSomething<2>(argv); break;
	case 3:DoSomething<3>(argv); break;
	default: return EXIT_FAILURE;
	}
	
	system("pause");
	exit(EXIT_SUCCESS);
}

template<int Dimension>
void DoSomething(char** argv)
{
	typedef float																	PixelType;
	typedef typename itk::Image<PixelType, Dimension>								ImageType;
	typedef typename itk::ImageFileReader<ImageType>								ImageReaderType;
	typedef typename itk::ImageFileWriter<ImageType>								ImageWriterType;

	/** Read the image. */
	std::cout << "Loading the the image... " << std::endl;
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
	std::cout << "Previous image size: " << reader->GetOutput()->GetLargestPossibleRegion() << std::endl << std::endl;

	/**  the image. */
	std::cout << " the image... " << std::endl;
	

	/** Write the  image. */
	std::cout << "Writing the diffusion image... " << std::endl;
	ImageWriterType::Pointer writer = ImageWriterType::New();
	writer->SetFileName(argv[2]);
	writer->SetInput(filter->GetOutput());
	writer->UseCompressionOn();
	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject &err)
	{
		std::cerr << "Could not write the image!" << std::endl;
		std::cerr << err << std::endl;
		exit(EXIT_FAILURE);
	}
}

int ReadImageInformation(std::string filename)
{
	itk::ImageIOBase::Pointer imageInput;
	try
	{
		imageInput = itk::ImageIOFactory::CreateImageIO(
			filename.c_str(), itk::ImageIOFactory::ReadMode);
		if (imageInput)
		{
			imageInput->SetFileName(filename);
			imageInput->ReadImageInformation();
		}
		else
		{
			std::cerr << "Could not read the image information." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	catch (itk::ExceptionObject& err)
	{
		std::cerr << "Could not read the image information." << std::endl;
		std::cerr << err << std::endl;
		exit(EXIT_FAILURE);
	}
	return imageInput->GetNumberOfDimensions();
}