#include "fwk.h"

struct {
    float (*ease)(float);
    const char *name;
} easings[] = {
    {ease_linear, "ease_linear"},
    {ease_out_sine, "ease_out_sine"},
    {ease_out_quad, "ease_out_quad"},
    {ease_out_cubic, "ease_out_cubic"},
    {ease_out_quart, "ease_out_quart"},
    {ease_out_quint, "ease_out_quint"},
    {ease_out_expo, "ease_out_expo"},
    {ease_out_circ, "ease_out_circ"},
    {ease_out_back, "ease_out_back"},
    {ease_out_elastic, "ease_out_elastic"},
    {ease_out_bounce, "ease_out_bounce"},
    {ease_in_sine, "ease_in_sine"},
    {ease_in_quad, "ease_in_quad"},
    {ease_in_cubic, "ease_in_cubic"},
    {ease_in_quart, "ease_in_quart"},
    {ease_in_quint, "ease_in_quint"},
    {ease_in_expo, "ease_in_expo"},
    {ease_in_circ, "ease_in_circ"},
    {ease_in_back, "ease_in_back"},
    {ease_in_elastic, "ease_in_elastic"},
    {ease_in_bounce, "ease_in_bounce"},
    {ease_inout_sine, "ease_inout_sine"},
    {ease_inout_quad, "ease_inout_quad"},
    {ease_inout_cubic, "ease_inout_cubic"},
    {ease_inout_quart, "ease_inout_quart"},
    {ease_inout_quint, "ease_inout_quint"},
    {ease_inout_expo, "ease_inout_expo"},
    {ease_inout_circ, "ease_inout_circ"},
    {ease_inout_back, "ease_inout_back"},
    {ease_inout_elastic, "ease_inout_elastic"},
    {ease_inout_bounce, "ease_inout_bounce"},
    {ease_inout_perlin, "ease_inout_perlin"},
};

int main() {
    window_create(0.75, 0);
    while(window_swap()) {
        static float timer = 0;
        if( (timer += 1/60.f) > 2 ) timer = 0; // loops every 2s
        if(ui_panel("ease",0)) {
            float linear_delta = timer / 2.f; // delta is [0..1]
            for( int i = 0; i < countof(easings); ++i) {
                float nonlinear_delta = easings[i].ease(linear_delta);
                ui_slider(easings[i].name, &nonlinear_delta );
            }
            ui_panel_end();
        }
    }
}
