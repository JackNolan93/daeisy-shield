#include "ResonatorWrapper.h"
#include "m_pd.h"
#include <math.h>

static t_class *resonator_tilde_class;

typedef struct _resonator_tilde {
    t_object x_obj;
    
    void * cpp_obj;

    t_float x_frequency;
    t_float x_structure;
    t_float x_brightness;
    t_float x_damping;
    
    t_inlet * x_frequency_in;
    t_outlet * x_out;
} t_resonator_tilde;

t_int * resonator_tilde_perform(t_int *w)
{
    t_resonator_tilde *x = (t_resonator_tilde *) (w[1]);
    t_sample      *in =      (t_sample *) (w[2]);
    t_sample    *freq =      (t_sample *) (w[3]);
    t_sample     *out =      (t_sample *) (w[4]);
    int             n =             (int) (w[5]);

    ResonatorBrightness (x->cpp_obj, x->x_brightness);
    ResonatorDamping (x->cpp_obj, x->x_damping);
    ResonatorStructure (x->cpp_obj, x->x_structure);
    
    for(int i = 0; i < n; i++)
    {
        ResonatorSetFreq (x->cpp_obj, freq[i]);
        out[i] = ResonatorProcess (x->cpp_obj, in [i]);
    }

    return (w+6);
}

void resonator_tilde_dsp(t_resonator_tilde *x, t_signal **sp)
{
    dsp_add (resonator_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n);
}

void resonator_tilde_free (t_resonator_tilde *x)
{
    inlet_free (x->x_frequency_in);
    outlet_free (x->x_out);
    
    ResonatorRelease (x->cpp_obj);
}

void *resonator_tilde_new (t_floatarg f)
{
    t_resonator_tilde *x = (t_resonator_tilde *)pd_new(resonator_tilde_class);

    x->x_structure = 0.7;
    x->x_brightness = 0.5;
    x->x_damping = 0.2;
    x->x_frequency = f;

    x->x_frequency_in = inlet_new (&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    
    floatinlet_new (&x->x_obj, &x->x_brightness);
    floatinlet_new (&x->x_obj, &x->x_damping);
    floatinlet_new (&x->x_obj, &x->x_structure);
    
    x->x_out = outlet_new (&x->x_obj, &s_signal);
    
    x->cpp_obj = ResonatorCreate ();
    ResonatorInit (x->cpp_obj, x->x_structure, 24, 44100.f);

    return (void *)x;
}

extern "C" void resonator_tilde_setup (void)
{
    resonator_tilde_class = class_new(gensym("resonator~"),
        (t_newmethod)resonator_tilde_new,
        (t_method)resonator_tilde_free,
        sizeof(t_resonator_tilde),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);
    
    CLASS_MAINSIGNALIN (resonator_tilde_class, t_resonator_tilde, x_frequency);

    class_addmethod (resonator_tilde_class,
      (t_method)resonator_tilde_dsp, gensym("dsp"), A_CANT, 0);
}


