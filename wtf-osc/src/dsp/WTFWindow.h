#include "daisysp.h"
#include <utility>
 
namespace JackDsp
{

class WTFWindow
{
public:

    WTFWindow () = default;
    ~WTFWindow () = default;

    bool containsPhase (float phase)
    {
        return _lowerBoundary <= phase && phase <= _upperBoundary;
    }

    void SetLimits (float lower, float upper)
    {
        _lowerLimit = lower;
        _upperLimit = upper;

        _windowSize = upper - lower;
        _windowCentre = (upper + lower) / 2;

        SetWindowWidth (_rawWidth);
    }

    void SetWindowWidth (float width)
    {
        width = width < 0.005 ? 0 : width;
        width = width > 1 - 0.005 ? 1.1 : width;

        _rawWidth = width;

        width *= _windowSize;
        _lowerBoundary = _windowCentre - (width / 2);
        _upperBoundary = _windowCentre + (width / 2);
    }

private:

    float _lowerLimit = 0.f;
    float _upperLimit = 0.f;
    float _windowSize = 0.f;
    float _windowCentre = 0.f;

    float _lowerBoundary = 0.f;
    float _upperBoundary = 0.f;

    float _rawWidth = 0.f;
};

}