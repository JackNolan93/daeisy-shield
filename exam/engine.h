#include "hw/daeisy_shield.h"
#include "daisysp.h"
#include "daisy.h"

namespace daisyshield::exam
{
class Engine
{
public:
    Engine(daisy::DaeisyShield & shield);
    ~Engine() = default;

    void init ();

    void processOscBank (daisy::AudioHandle::InputBuffer & in,
                         daisy::AudioHandle::OutputBuffer & out, 
                         size_t size);

    void processDrums (daisy::AudioHandle::InputBuffer & in,
                         daisy::AudioHandle::OutputBuffer & out, 
                         size_t size);  
private:

void initOscillators ();
void initDrums ();

void updateParameters ();

static constexpr int numberOfOscillators = 8;

daisy::DaeisyShield & _shield;
daisysp::Oscillator _oscillators[numberOfOscillators];

daisysp::SyntheticBassDrum _kick;
daisysp::AnalogSnareDrum _snare;
daisysp::HiHat<> _hat;

float _output = 0.0;
};
    
}