#ifndef itkHepaticVascularPreprocessFilter_cpp
#define itkHepaticVascularPreprocessFilter_cpp

#include "itkHepaticVascularPreprocessFilter.h"

namespace itk
{
	template< typename TInputImage, typename TOutputImage >
	void
		HepaticVascularPreprocessFilter< TInputImage, TOutputImage >
		::GenerateData()
	{
		// Get the maximum and minimum in the input image.
		typedef typename MinimumMaximumImageCalculator<InputImageType>      MinimumMaximumImageCalculatorType;
		MinimumMaximumImageCalculatorType::Pointer  minimumMaximumImageCalculator = MinimumMaximumImageCalculatorType::New();
		minimumMaximumImageCalculator->SetImage(this->GetInput());
		minimumMaximumImageCalculator->Compute();

		m_MaxValue = minimumMaximumImageCalculator->GetMaximum();
		m_MinValue = minimumMaximumImageCalculator->GetMinimum();

		// Maximum image projection.
		typedef typename MaximumProjectionImageFilter<InputImageType, ProjectionImageType>      MaximumProjectionImageFilterType;
		MaximumProjectionImageFilterType::Pointer maximumProjectionImageFilter = MaximumProjectionImageFilterType::New();
		maximumProjectionImageFilter->SetInput(this->GetInput());
		maximumProjectionImageFilter->SetProjectionDimension(2);
		maximumProjectionImageFilter->Update();

		m_MaximumProjectionImage = maximumProjectionImageFilter->GetOutput();
		m_MaximumProjectionImage->DisconnectPipeline();

		// Training the GMM
		// Enable which method
		if (m_UseItkFilter)
		{
			this->TrainingGMM();
			std::sort(m_Components.begin(), m_Components.end());

			m_ThresholdAbove = m_Components[m_NumOfComponents - 1].first + 2 * m_Components[m_NumOfComponents - 1].second;
			m_ThresholdBelow = m_Components[0].first - 2 * m_Components[0].second;

			m_Alpha = ((m_Components[m_NumOfComponents - 1].first - m_Components[(int)std::floor(m_NumOfComponents / 2)].first)) / 2;
			m_Beta = ((m_Components[m_NumOfComponents - 1].first + m_Components[(int)std::floor(m_NumOfComponents / 2)].first)) / 2;
		}
		else
		{
			double *data = m_MaximumProjectionImage->GetBufferPointer();
			int size = m_MaximumProjectionImage->GetLargestPossibleRegion().GetNumberOfPixels();
			GMM *m_GMM = new GMM(1, m_NumOfComponents);
			m_GMM->Train(data, size);

			m_ThresholdAbove = m_GMM->Mean(0)[0] + 2 * std::sqrt(m_GMM->Variance(0)[0]);
			m_ThresholdBelow = m_GMM->Mean(m_NumOfComponents - 1)[0] - 2 * std::sqrt(m_GMM->Variance(m_NumOfComponents - 1)[0]);

			m_Alpha = ((m_GMM->Mean(0)[0] - m_GMM->Mean((int)std::ceil(m_NumOfComponents / 2))[0])) / 2;
			m_Beta = ((m_GMM->Mean(0)[0] + m_GMM->Mean((int)std::ceil(m_NumOfComponents / 2))[0])) / 2;
		}

		/** Stupid filter forces me to call it twice. */
		typedef typename ThresholdImageFilter<InputImageType>      ThresholdImageFilterType;
		ThresholdImageFilterType::Pointer thresholdImageFilterAbove = ThresholdImageFilterType::New();
		ThresholdImageFilterType::Pointer thresholdImageFilterBelow = ThresholdImageFilterType::New();

		thresholdImageFilterAbove->SetInput(this->GetInput());
		thresholdImageFilterAbove->ThresholdAbove(m_ThresholdAbove);
		thresholdImageFilterAbove->SetOutsideValue(m_ThresholdAbove);
		thresholdImageFilterAbove->Update();

		thresholdImageFilterBelow->SetInput(thresholdImageFilterAbove->GetOutput());
		thresholdImageFilterBelow->ThresholdBelow(m_ThresholdBelow);
		thresholdImageFilterBelow->SetOutsideValue(m_ThresholdBelow);
		thresholdImageFilterBelow->Update();

		/** Sigmoid the image to get final result. */
		typedef typename SigmoidImageFilter<InputImageType, OutputImageType>      SigmoidImageFilterType;
		SigmoidImageFilterType::Pointer sigmoidImageFilter = SigmoidImageFilterType::New();

		sigmoidImageFilter->SetInput(thresholdImageFilterBelow->GetOutput());
		sigmoidImageFilter->SetOutputMinimum(m_MinValue);
		sigmoidImageFilter->SetOutputMaximum(2000);
		sigmoidImageFilter->SetAlpha(m_Alpha);
		sigmoidImageFilter->SetBeta(m_Beta);
		sigmoidImageFilter->Update();

		OutputImagePointer output = this->GetOutput();
		output->SetPixelContainer(sigmoidImageFilter->GetOutput()->GetPixelContainer());
		output->SetBufferedRegion(sigmoidImageFilter->GetOutput()->GetLargestPossibleRegion());
	}

	template<typename TInputImage, typename TOutputImage>
	void
		HepaticVascularPreprocessFilter<TInputImage, TOutputImage>
		::TrainingGMM()
	{
		// Convert image to sample.
		typedef typename itk::Vector< RealType, 1 >		MeasurementVectorType;
		typedef typename itk::Statistics::ListSample< MeasurementVectorType > SampleType;
		SampleType::Pointer sample = SampleType::New();
		sample->SetMeasurementVectorSize(1);

		ImageRegionConstIterator<ProjectionImageType> it(m_MaximumProjectionImage, m_MaximumProjectionImage->GetLargestPossibleRegion());
		for (it.GoToBegin(); !it.IsAtEnd(); ++it)
		{
			sample->PushBack(it.Get());
		}

		// Set each component initial parameters.
		typedef typename itk::Array< RealType > ParametersType;
		ParametersType params(2);
		std::vector< ParametersType > initialParameters(m_NumOfComponents);
		for (int i = 0; i < m_NumOfComponents; i++)
		{
			params[0] = m_MaxValue / m_NumOfComponents * i;
			params[1] = 100.0;
			initialParameters[i] = params;
		}

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
		itk::Array< RealType > initialProportions(m_NumOfComponents);
		initialProportions.Fill(1.0 / m_NumOfComponents);
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
			std::cout << "         " << (components[i])->GetFullParameters()[0]
				<< "         " << std::sqrt((components[i])->GetFullParameters()[1]) << std::endl;
			std::cout << "    Proportion: ";
			std::cout << "         " << estimator->GetProportions()[i] << std::endl;

			m_Components.push_back(std::make_pair((components[i])->GetFullParameters()[0], std::sqrt((components[i])->GetFullParameters()[1])));
		}

	}

	template< typename TInputImage, typename TOutputImage >
	void
		HepaticVascularPreprocessFilter< TInputImage, TOutputImage >
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
