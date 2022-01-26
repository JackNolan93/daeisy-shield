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

    void process (daisy::AudioHandle::InputBuffer & in,
                  daisy::AudioHandle::OutputBuffer & out, 
                  size_t size);

private:

void initOscillators ();
void updateParameters ();

static constexpr int numberOfOscillators = 8;

daisy::DaeisyShield & _shield;
daisysp::Oscillator _oscillators[numberOfOscillators];

float _output = 0.0;

};
    
}