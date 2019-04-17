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
		// Get the maximum and minimum in the input image.
		typedef typename MinimumMaximumImageCalculator<InputImageType>      MinimumMaximumImageCalculatorType;
		MinimumMaximumImageCalculatorType::Pointer  minimumMaximumImageCalculator = MinimumMaximumImageCalculatorType::New();
		minimumMaximumImageCalculator->SetImage(this->GetInput());
		minimumMaximumImageCalculator->Compute();

		this->m_MaxValue = minimumMaximumImageCalculator->GetMaximum();
		this->m_MinValue = minimumMaximumImageCalculator->GetMinimum();


		// Maximum image projection.
		typedef typename MaximumProjectionImageFilter<InputImageType, ProjectionImageType>      MaximumProjectionImageFilterType;
		MaximumProjectionImageFilterType::Pointer maximumProjectionImageFilter = MaximumProjectionImageFilterType::New();
		maximumProjectionImageFilter->SetInput(this->GetInput());
		maximumProjectionImageFilter->SetProjectionDimension(2);
		maximumProjectionImageFilter->Update();

		m_MaximumProjectionImage = maximumProjectionImageFilter->GetOutput();
		m_MaximumProjectionImage->DisconnectPipeline();

		// Training the GMM
		this->TrainingGMM();


		/** Typedef to describe the pointer to the ThresholdImageFilter. */
		typedef typename ThresholdImageFilter<InputImageType>::Pointer      ThresholdImageFilterPointer;



		/** Typedef to describe the pointer to the SigmoidImageFilter. */
		typedef typename SigmoidImageFilter<InputImageType, OutputImageType>::Pointer      SigmoidImageFilterPointer;
	}

	template<typename TInputImage, typename TOutputImage>
	void 
		HepaticVascularPreproccessFilter<TInputImage, TOutputImage>
		::TrainingGMM()
	{
		typedef typename itk::Vector< double, 1 >		MeasurementVectorType;
		typedef typename itk::Statistics::ListSample< MeasurementVectorType > SampleType;
		SampleType::Pointer sample = SampleType::New();
		sample->SetMeasurementVectorSize(1);

		ImageRegionConstIterator<ProjectionImageType> it(m_MaximumProjectionImage, m_MaximumProjectionImage->GetLargestPossibleRegion());
		for (it.GoToBegin();!it.IsAtEnd();++it)
		{
			sample->PushBack(it.Get());
		}
	
		m_NumOfComponents = 5;

		typedef typename itk::Array< double > ParametersType;
		ParametersType params(5);
		std::vector< ParametersType > initialParameters(m_NumOfComponents);
		params[0] = 0.0;
		params[1] = 100.0;
		initialParameters[0] = params;
		params[0] = 200.0;
		params[1] = 100.0;
		initialParameters[1] = params;
		params[0] = 300.0;
		params[1] = 100.0;
		initialParameters[2] = params;
		params[0] = 400.0;
		params[1] = 100.0;
		initialParameters[3] = params;
		params[0] = 600.0;
		params[1] = 50.0;
		initialParameters[4] = params;

		typedef typename itk::Statistics::GaussianMixtureModelComponent<SampleType>		ComponentType;
		std::vector< ComponentType::Pointer > components;
		for (unsigned int i = 0; i < m_NumOfComponents; i++)
		{
			components.push_back(ComponentType::New());
			(components[i])->SetSample(sample);
			(components[i])->SetParameters(initialParameters[i]);
		}

		typedef typename itk::Statistics::ExpectationMaximizationMixtureModelEstimator<SampleType>		EstimatorType;
		EstimatorType::Pointer estimator = EstimatorType::New();
		estimator->SetSample(sample);
		estimator->SetMaximumIteration(200);
		itk::Array< double > initialProportions(m_NumOfComponents);
		initialProportions[0] = 0.2;
		initialProportions[1] = 0.2;
		initialProportions[2] = 0.2;
		initialProportions[3] = 0.2;
		initialProportions[4] = 0.2;
		estimator->SetInitialProportions(initialProportions);

		for (unsigned int i = 0; i < m_NumOfComponents; ++i)
		{
			estimator->AddComponent((ComponentType::Superclass*)
				(components[i]).GetPointer());
		}
		estimator->Update();

		for (unsigned int i = 0; i < m_NumOfComponents; ++i)
		{
			std::cout << "Cluster[" << i << "]" << std::endl;
			std::cout << "    Parameters:" << std::endl;
			std::cout << "         " << (components[i])->GetFullParameters()
				<< std::endl;
			std::cout << "    Proportion: ";
			std::cout << "         " << estimator->GetProportions()[i] << std::endl;
		}
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
