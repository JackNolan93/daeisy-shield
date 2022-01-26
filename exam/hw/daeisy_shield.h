#pragma once
#include "daisy_seed.h"

namespace daisy
{

class DaeisyShield
{
  public:

    enum AnalogCtrl
    {
        CTRL_1,    /**< */
        CTRL_2,    /**< */
        CTRL_3,    /**< */
        CTRL_4,    /**< */
        CVCTRL_1,    /**< */
        CVCTRL_2,    /**< */
        CVCTRL_3,    /**< */
        CVCTRL_4,    /**< */
        V_OCT_1,
        CTRL_NBR, /**< */
    };

    enum CVOut
    {
        CV_OUT_1,    /**< */
        CV_OUT_2,    /**< */
        CV_OUT_NBR,
    };

    enum ButtonInput
    {
        BUTTON_1,    /**< */
        BUTTON_2,    /** <*/
        BUTTON_3,
        BUTTON_4,
        BUTTON_NBR, /**< */
    };

    enum GateInput
    {
        GATE_IN_1,    /**< */
        GATE_IN_2,    /** <*/
        GATE_IN_3,    /** <*/
        GATE_IN_NBR, /**< */
    };

    enum GateOutput
    {
        GATE_OUT_1,    /**< */
        GATE_OUT_2,    /** <*/
        GATE_OUT_NBR, /**< */
    };

    DaeisyShield () {}
    ~DaeisyShield () {}

    void Init(bool boost = false);

    void DelayMs(size_t del);

    void StartAudio(AudioHandle::AudioCallback cb);
    void ChangeAudioCallback(AudioHandle::AudioCallback cb);
    void StopAudio();

    void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);
    float AudioSampleRate();

    void SetAudioBlockSize(size_t size);
    size_t AudioBlockSize();

    float AudioCallbackRate();

    void StartAdc();
    void StopAdc();

    void ProcessAnalogControls();
        void ProcessDigitalControls();

    inline void ProcessAllControls()
    {
        ProcessAnalogControls();
        ProcessDigitalControls();
    }

    float GetControlValue(AnalogCtrl control);
    float GetVOctValue(AnalogCtrl control);

    DaisySeed       seed;   
    AnalogControl   controls[CTRL_NBR];  
    Switch          buttons[BUTTON_NBR];
    CVOut           cv_out[CV_OUT_NBR];
    GateIn          gate_inputs[GATE_IN_NBR];   
    dsy_gpio        gate_outputs[GATE_OUT_NBR]; 


  private:
    void SetHidUpdateRates();
    void InitControls();
    void InitCvOutputs();
    void InitGates();
    void InitButtons();
};

} // namespace daisy
