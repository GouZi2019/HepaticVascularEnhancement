#ifndef itkHepaticVascularPreproccessFilter_h
#define itkHepaticVascularPreproccessFilter_h

// ITK includes
#include "itkMacro.h"
#include "itkImage.h"
#include "itkVector.h"
#include "itkListSample.h"
#include "itkImageToImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkMaximumProjectionImageFilter.h"
#include "itkGaussianMixtureModelComponent.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkExpectationMaximizationMixtureModelEstimator.h"

namespace itk
{

	template <typename TInputImage = itk::Image<double, 3>, typename TOutputImage = TInputImage>
	class HepaticVascularPreproccessFilter : public ImageToImageFilter<TInputImage, TInputImage>
	{
	public:
		/** Standard class typedefs. */
		typedef HepaticVascularPreproccessFilter                        Self;
		typedef ImageToImageFilter<TInputImage, TInputImage>			Superclass;
		typedef SmartPointer<Self>                                      Pointer;
		typedef SmartPointer<const Self>                                ConstPointer;

		/** Method for creation through the object factory. */
		itkNewMacro(Self);

		/** Run-time type information (and related methods). */
		itkTypeMacro(HepaticVascularPreproccessFilter, ImageToImageFilter);

		/** Typedef to describe the input and output. */
		typedef TInputImage   InputImageType;
		typedef TOutputImage   OutputImageType;
		typedef typename InputImageType::PixelType InputImagePixelType;

		/** Type to use form computations. */
		typedef typename NumericTraits< InputImagePixelType >::RealType RealType;
		typedef itk::Image<RealType, 2>	ProjectionImageType;

		/** Typedef to describe the pointer to the input. */
		typedef typename InputImageType::Pointer      InputImagePointer;
		typedef typename InputImageType::ConstPointer InputImageConstPointer;

		/** Typedef to describe the pointer to the output. */
		typedef typename OutputImageType::Pointer      OutputImagePointer;
		typedef typename ProjectionImageType::Pointer ProjectionImageointer;

		/** Image related typedefs. */
		itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

		/** Set & Get Macro. */
		itkSetMacro(NumOfComponents, unsigned int);
		itkSetMacro(MixtureCoefficient, std::vector<RealType>);
		itkSetMacro(ComponentMeanValues, std::vector<RealType>);
		itkSetMacro(ComponentStdValues, std::vector<RealType>);

		itkGetConstMacro(NumOfComponents, unsigned int);
		itkGetConstMacro(MixtureCoefficient, std::vector<RealType>);
		itkGetConstMacro(ComponentMeanValues, std::vector<RealType>);
		itkGetConstMacro(ComponentStdValues, std::vector<RealType>);
		itkGetConstMacro(MaxValue, RealType);
		itkGetConstMacro(MinValue, RealType);
		itkGetConstMacro(ThresholdAbove, RealType);
		itkGetConstMacro(ThresholdBelow, RealType);
		itkGetConstMacro(Alpha, RealType);
		itkGetConstMacro(Beta, RealType);
		itkGetConstMacro(MaximumProjectionImage, ProjectionImageointer);
		itkGetConstMacro(GaussianMixtureModel, std::vector<RealType>);

	protected:
		HepaticVascularPreproccessFilter() {}
		~HepaticVascularPreproccessFilter() ITK_OVERRIDE {}

		void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

		/** Generate Requested Data */
		virtual void GenerateData() ITK_OVERRIDE;

	private:
		void TrainingGMM();

		ProjectionImageointer           m_MaximumProjectionImage;

		unsigned int					m_NumOfComponents;
		RealType						m_MaxValue;
		RealType                        m_MinValue;
		RealType						m_ThresholdAbove;
		RealType                        m_ThresholdBelow;
		RealType                        m_Alpha;
		RealType                        m_Beta;

		std::vector<RealType>						m_MixtureCoefficient;
		std::vector<RealType>						m_ComponentMeanValues;
		std::vector<RealType>						m_ComponentStdValues;
		std::vector<RealType>						m_GaussianMixtureModel;
	};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHepaticVascularPreproccessFilter.cpp"
#endif

#endif // !itkHepaticVascularPreproccessFilter_h
