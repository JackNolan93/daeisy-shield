#include "WTFOscillator.h"
#include <math.h>

using namespace JackDsp;

void WTFOscillator::Init(float sample_rate)
{
    sample_rate_ = sample_rate;

    phase_       = 0.0f;
    next_sample_ = 0.0f;
    previous_ww_ = 0.0f;
    high_        = false;

    SetFreq(220.f);
    SetFrontWaveshape (WaveShape::WS_SIN);
    SetBackWaveshape (WaveShape::WS_SAW);

    _window.SetLimits (0.0, 1.0);
    _window.SetWindowWidth (0.0);
}

float WTFOscillator::Process()
{
    float next_sample = next_sample_;

    float this_sample = 0.0;
    next_sample       = 0.0f;

    phase_ += frequency_;
    while (phase_ > 1.f)
        phase_ -= 1.f;

    this_sample = ComputeNaiveSample (phase_,
                                      _window.containsPhase (phase_) ||
                                      _windowAux.containsPhase (phase_) 
                                      ? _bWave : _fWave);

    next_sample_ = next_sample;
    return (2.0f * this_sample - 1.0f);
}

void WTFOscillator::SetFreq(float frequency)
{
    frequency  = frequency / sample_rate_;
    frequency  = frequency >= .25f ? .25f : frequency;
    frequency_ = frequency;
}

void WTFOscillator::SetWindowConfig (WindowConfig config)
{
    switch (config)
    {
    case WindowConfig::Single:
        _window.SetLimits (0.0, 1.0);
        _windowAux.SetLimits (-1.0, -1.0);
        break;
    case WindowConfig::Dual:
        _window.SetLimits (0.0, 0.5);
        _windowAux.SetLimits (0.5, 1.0);
    default:
        break;
    }
}

void WTFOscillator::SetWindowW(float ww)
{
    _window.SetWindowWidth (ww);
    _windowAux.SetWindowWidth (ww);
}

void WTFOscillator::SetFrontWaveshape (WaveShape waveshape)
{
    _fWave = waveshape;
}

void WTFOscillator::SetBackWaveshape (WaveShape waveshape)
{
    _bWave = waveshape;
}

void WTFOscillator::IncrementFrontWaveshape ()
{
    auto wave = int (_fWave);
    wave += 1;

    _fWave = WaveShape (wave % int (WaveShape::WS_NUM));
}

void WTFOscillator::IncrementBackWaveshape ()
{
    auto wave = int (_bWave);
    wave += 1;

    _bWave = WaveShape (wave % int (WaveShape::WS_NUM));
}

std::pair<int, int> WTFOscillator::getWaveshapeIndexes ()
{
    return std::make_pair <int, int> (int (_fWave), int (_bWave));
}

float WTFOscillator::ComputeNaiveSample(float phase, WaveShape wave)
{
    float out;
    switch (wave)
    {
        case WS_SIN: out = (sinf(phase * PI_F * 2) + 1) / 2.f; break;
        case WS_TRIANGLE:
            out = phase < 0.5 ? phase * 2 : 1.0f - (phase - 0.5) * 2;
            break;
        case WS_SQUARE:
            out = phase < 0.2 ? 0: 0.8f;
            break;
        case WS_SAW:
            out = -1.f * (phase - 1.f);
            break;
        case WS_RAMP:
            out = phase;
            break;
        case WS_HARMONIC:
            out = ComputeHarmonicWaveForm (phase);
            break;
        case WS_ZERO:
            out = 0;
            break;
        default:
            out = 0.5;
            break;
    }

    return out;
}

float WTFOscillator::ComputeHarmonicWaveForm (float phase)
{
    float out = 0;
    for (int i = 1; i <= 6; ++i)
        out += (sinf(phase * PI_F * 2 * i) + 1) / 2.f; 

    return out / 6;
} 

    // while(transition_during_reset)
    // {
    //     if(!high_)
    //     {
    //         if(phase_< pw_)
    //         {
    //             break;
    //         }
    //         float t = (slave_phase_ - pw_)
    //                   / (previous_pw_ - pw_ + slave_frequency_);
    //         float triangle_step = (slope_up + slope_down) * slave_frequency_;
    //         triangle_step *= triangle_amount;

    //         this_sample += square_amount * ThisBlepSample(t);
    //         next_sample += square_amount * NextBlepSample(t);
    //         this_sample -= triangle_step * ThisIntegratedBlepSample(t);
    //         next_sample -= triangle_step * NextIntegratedBlepSample(t);
    //         high_ = true;
    //     }

    //     if(high_)
    //     {
    //         if(slave_phase_ < 1.0f)
    //         {
    //             break;
    //         }
    //         slave_phase_ -= 1.0f;
    //         float t             = slave_phase_ / slave_frequency_;
    //         float triangle_step = (slope_up + slope_down) * slave_frequency_;
    //         triangle_step *= triangle_amount;

    //         this_sample -= (1.0f - triangle_amount) * ThisBlepSample(t);
    //         next_sample -= (1.0f - triangle_amount) * NextBlepSample(t);
    //         this_sample += triangle_step * ThisIntegratedBlepSample(t);
    //         next_sample += triangle_step * NextIntegratedBlepSample(t);
    //         high_ = false;
    //     }
    // }