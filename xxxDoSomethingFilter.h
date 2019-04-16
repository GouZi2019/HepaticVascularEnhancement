#ifndef xxxDoSomethingFilter_H
#define xxxDoSomethingFilter_H

// ITK includes
#include "itkImageToImageFilter.h"

// VTK includes

// C++ STL

namespace xxx
{
template <typename TInputImage, typename TOutputImage>
class DoSomethingFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
  public:
    /** Standard class typedefs. */
    typedef DoSomethingFilter                                       Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage>           Superclass;
    typedef SmartPointer<Self>                                      Pointer;
    typedef SmartPointer<const Self>                                ConstPointer;

    /** Define types. */
    typedef TInputImage                                             InputImageType;
    typedef TOutputImage                                            OutputImageType;
    typedef typename InputImageType::Pointer                        InputImagePointer;
    typedef typename OutputImageType::Pointer                       OutputImagePointer;
    typedef typename InputImageType::ConstPointer                   InputImageConstPointer;
    typedef typename OutputImageType::ConstPointer                  OutputImageConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(DoSomethingFilter, ImageToImageFilter);

    /** Set & Get Macro. */
	itkSetMacro(WhateverImageA, InputImagePointer);
	itkGetConstMacro(WhateverImageA, InputImageConstPointer);

	itkSetMacro(WhateverValueA, double);
	itkGetConstMacro(WhateverValueA, double);

	itkBooleanMacro(UseWhatever);
	itkSetMacro(UseWhatever, bool);
	itkGetConstMacro(UseWhatever, bool);

  protected:
    DoSomethingFilter();
    ~DoSomethingFilter() ITK_OVERRIDE {}
    void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

    /** Generate Requested Data */
    virtual void GenerateData() ITK_OVERRIDE;

  private:
    /** Depends on you */
    void DoWhateverA();
    int DoWhateverB(InputImagePointer);

    InputImagePointer           m_WhateverImageA;
    OutputImagePointer          m_WhateverImageB;

    double                      m_WhateverValueA;
    int                         m_WhateverValueB;
    bool                        m_UseWhatever;
};
} // namespace xxx

#ifndef ITK_MANUAL_INSTANTIATION
#include "xxxDoSomethingFilter.hxx"
#endif

#endif // !xxxDoSomethingFilter_H
