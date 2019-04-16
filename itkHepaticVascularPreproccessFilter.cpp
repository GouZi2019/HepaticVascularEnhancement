#ifndef itkHepaticVascularPreproccessFilter_hxx
#define itkHepaticVascularPreproccessFilter_hxx

#include "itkHepaticVascularPreproccessFilter.h"

namespace itk
{
	template< typename TInputImage, typename TOutputImage >
	void 
		HepaticVascularPreproccessFilter< TInputImage, TOutputImage >
		::GenerateData()
	{
		// Todo
	}

	template< typename TInputImage, typename TOutputImage >
	void
		HepaticVascularPreproccessFilter< TInputImage, TOutputImage >
		::PrintSelf(std::ostream & os, Indent indent) const
	{
		Superclass::PrintSelf(os, indent);
		
		os << indent << "Max value in the input image: "
			<< m_MaxValue << std::endl;
		os << indent << "Min value in the input image: "
			<< m_MinValue << std::endl;
		os << indent << "Threshold above value in the ThresholdImageFilter: "
			<< m_ThresholdAbove << std::endl;
		os << indent << "Threshold below value in the ThresholdImageFilter: "
			<< m_ThresholdBelow << std::endl;
		os << indent << "Alpha used in the SigmoidImageFilter: "
			<< m_Alpha << std::endl;
		os << indent << "Beta value in the SigmoidImageFilter: "
			<< m_Beta << std::endl;
	}
} // end namespace itk
#endif
