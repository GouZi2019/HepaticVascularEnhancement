#ifndef itkHepaticVascularPreproccessFilter_h
#define itkHepaticVascularPreproccessFilter_h

// ITK includes
#include "itkMacro.h"
#include "itkImage.h"
#include "itkVector.h"
#include "itkImageToImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkMaximumProjectionImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"

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

		/** Typedef to describe the pointer to the MinimumMaximumImageCalculator. */
		typedef typename MinimumMaximumImageCalculator<InputImageType>::Pointer      MinimumMaximumImageCalculatorPointer;

		/** Typedef to describe the pointer to the MaximumProjectionImageFilter. */
		typedef typename MaximumProjectionImageFilter<InputImageType, ProjectionImageType>::Pointer      MaximumProjectionImageFilterPointer;

		/** Typedef to describe the pointer to the ThresholdImageFilter. */
		typedef typename ThresholdImageFilter<InputImageType>::Pointer      ThresholdImageFilterPointer;

		/** Typedef to describe the pointer to the GaussianMixtureModelCalculator. */
		// Todo

		/** Typedef to describe the pointer to the SigmoidImageFilter. */
		typedef typename SigmoidImageFilter<InputImageType, OutputImageType>::Pointer      SigmoidImageFilterPointer;

		/** Image related typedefs. */
		itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

		/** Set & Get Macro. */
		itkSetMacro(NumOfComponents, unsigned int);
		itkSetMacro(MixtureCoefficient, std::vector);
		itkSetMacro(ComponentMeanValues, std::vector);
		itkSetMacro(ComponentStdValues, std::vector);

		itkGetConstMacro(NumOfComponents, unsigned int);
		itkGetConstMacro(MixtureCoefficient, std::vector);
		itkGetConstMacro(ComponentMeanValues, std::vector);
		itkGetConstMacro(ComponentStdValues, std::vector);
		itkGetConstMacro(MaxValue, RealType);
		itkGetConstMacro(MinValue, RealType);
		itkGetConstMacro(ThresholdAbove, RealType);
		itkGetConstMacro(ThresholdBelow, RealType);
		itkGetConstMacro(Alpha, RealType);
		itkGetConstMacro(Beta, RealType);
		itkGetConstMacro(MaximumProjectionImage, ProjectionImageointer);
		itkGetConstMacro(GaussianMixtureModel, std::vector);

	protected:
		HepaticVascularPreproccessFilter();
		~HepaticVascularPreproccessFilter() ITK_OVERRIDE {}

		void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

		/** Generate Requested Data */
		virtual void GenerateData() ITK_OVERRIDE;

	private:
		ProjectionImageointer           m_MaximumProjectionImage;

		unsigned int					m_NumOfComponents;
		RealType						m_MaxValue;
		RealType                        m_MinValue;
		RealType						m_ThresholdAbove;
		RealType                        m_ThresholdBelow;
		RealType                        m_Alpha;
		RealType                        m_Beta;

		std::vector						m_MixtureCoefficient;
		std::vector						m_ComponentMeanValues;
		std::vector						m_ComponentStdValues;
		std::vector						m_GaussianMixtureModel;
	};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHepaticVascularPreproccessFilter.cpp"
#endif

#endif // !itkHepaticVascularPreproccessFilter_h
