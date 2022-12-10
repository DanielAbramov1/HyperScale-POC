#include <event.h>
#include <Arduino.h>
#include <stdio.h>
#include <weight_sensor.h>
#include <math.h>
#include <stdio.h>

#define EVENT_TIME_TRSHLD 200 // in msec
#define SLOPE_TRSHLD 5 // in gram/msec
#define SPIKE_TRSHLD 5
const char * event_sm_states_strings[] = {"EVENT_IDLE", "EVENT_STARTED", "EVENT_NOISE", "EVENT_REPORT"};
event_sm_states event_sm_state = EVENT_IDLE;

Event event;

Event::Event()
{
}

void Event::init_event_values()
{
    event.sensor_val = 0.0;
    event.prev_sensor_val = 0.0;
    event.prev_prev_val = 0.0;
    event.actual_weight = 0.0;
    event.before_weight = 0.0;

    event.data_timer = 0;
    event.prev_data_timer = 0;
    event.event_timer = 0;
    event.high_slope_timer = 0;
}


void set_event_sm_state(event_sm_states st)
{
    event_sm_state = st;
    // Serial.printf("event_sm_state: %s\n", event_sm_states_strings[event_sm_state]);
    Serial.print("event_sm_state:");Serial.println(event_sm_states_strings[event_sm_state]);
}

event_sm_states get_event_sm_state()
{
    return event_sm_state;
}

void event_sm_loop()
{
    float slope;
    float change;
    String packet = "";
    event.data_timer = millis();
    event.sensor_val = ReadSensor();

    slope = (event.sensor_val - event.prev_sensor_val)/(event.data_timer - event.prev_data_timer);
    if (fabs(slope) > SLOPE_TRSHLD)
        event.high_slope_timer = millis();

    switch(event_sm_state)
    {
        case EVENT_IDLE:
            if(fabs(slope) > SLOPE_TRSHLD)
            {
                event.before_weight = event.prev_prev_val;
                set_event_sm_state(EVENT_STARTED);
                event.event_timer = millis();
            }
            break;
        
        case EVENT_STARTED:
            if(millis() - event.high_slope_timer > EVENT_TIME_TRSHLD)
            {
                event.actual_weight = event.sensor_val;

                if(fabs(event.before_weight - event.sensor_val) < SPIKE_TRSHLD)
                    set_event_sm_state(EVENT_NOISE);
                else
                    set_event_sm_state(EVENT_REPORT);
            }
            break;
        
        case EVENT_NOISE:
            set_event_sm_state(EVENT_IDLE);
            break;
        
        case EVENT_REPORT:
            change = event.actual_weight - event.before_weight;
            packet = "<e," + String(change) + ">";
            Serial.println(packet);
            set_event_sm_state(EVENT_IDLE);
            break;
    }
    
    event.prev_prev_val = event.prev_sensor_val;
    event.prev_sensor_val = event.sensor_val;

    event.prev_data_timer = event.data_timer;
}

