#include "daisysp.h"
#include "daisy_seed.h"
#include "hw/daeisy_shield.h"
#include "engine.h"

using namespace daisy;
using namespace daisysp;

DaeisyShield shield;
daisyshield::exam::Engine engine { shield };

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	//engine.processOscBank (in, out, size);
	engine.processDrums (in, out, size);
}

int main(void)
{
	shield.Init ();
	shield.SetAudioBlockSize (8);
	engine.init ();

	shield.StartAdc ();
	shield.StartAudio(AudioCallback);

	while(1) {}
}
