#include "daisysp.h"
#include "WTFWindow.h"
#include <utility>
 
namespace JackDsp
{


class WTFOscillator 
{
public:

    enum WaveShape
    {
        WS_SIN = 0,
        WS_SAW, 
        WS_RAMP,
        WS_SQUARE,
        WS_TRIANGLE,
        WS_HARMONIC,
        WS_ZERO,
        WS_NUM,
    };

    enum WindowConfig
    {
        Single = 0,
        Dual,
    };

    WTFOscillator () = default;
    ~WTFOscillator () = default;

    void Init(float sample_rate);

    /** Get the next sample */
    float Process();

    void SetFreq(float frequency);

    void SetWindowW (float pw);
    void SetWindowConfig (WindowConfig config);

    void SetFrontWaveshape(WaveShape waveshape);
    void SetBackWaveshape(WaveShape waveshape);

    void IncrementFrontWaveshape ();
    void IncrementBackWaveshape ();

    std::pair<int, int> getWaveshapeIndexes ();

private:

    float ComputeNaiveSample (float phase, WaveShape wave);

    float ComputeHarmonicWaveForm (float phase);

    float sample_rate_;

    // Oscillator state.
    float frequency_;
    float phase_;
    float next_sample_;
    float previous_ww_;
    bool  high_;

    WaveShape _fWave;
    WaveShape _bWave;

    WTFWindow _window;
    WTFWindow _windowAux;
    float ww_;
};
}