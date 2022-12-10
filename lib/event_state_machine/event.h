#ifndef event_h
#define event_h

#include <stdint.h>

class Event
{
    public :
        Event();

        float sensor_val;
        float prev_sensor_val;
        float prev_prev_val;
        float actual_weight;
        float before_weight;

        uint32_t data_timer;
        uint32_t prev_data_timer;
        uint32_t event_timer;
        uint32_t high_slope_timer;

        void init_event_values();

    private:

};

extern Event event;
typedef enum event_sm_states {EVENT_IDLE, EVENT_STARTED, EVENT_NOISE, EVENT_REPORT} event_sm_states;
void set_event_sm_state(event_sm_states st);
event_sm_states get_event_sm_state();
void event_sm_loop();

#endif //event_h