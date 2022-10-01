#include "hw/daeisy_shield.h"
#include "daisysp.h"
#include "daisy.h"

namespace daisyshield::resonate
{
class Engine
{
public:
    Engine(daisy::DaeisyShield & shield);
    ~Engine() = default;

    void init ();
    void process  (const float * in, float * out, size_t size);

private:

    void updateParameters ();

    daisy::DaeisyShield & _shield;
    daisysp::Resonator _resonator;

    float _output = 0.0;
};
    
}