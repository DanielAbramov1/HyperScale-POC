#include <weight_sensor.h>
#include <globals.h>
#include <HX711.h>
#include <SimpleKalmanFilter.h>

HX711 scale;
float scale_factor = 277.6;  // Default number in grams
SimpleKalmanFilter filtered_scale_data(4,4,0.5);

void ScaleSetup()
{
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(scale_factor);
}

void StartTareSequence(int num_tare)
{   
    scale.tare(num_tare);
    Serial.println("Tare sequence finished.");
}

float ReadSensor()
{
    float raw_data = scale.get_units();
    
    float filtered_sensor_data = filtered_scale_data.updateEstimate(raw_data);
    // Serial.print("Scale reading:\t");
    // Serial.print(filtered_sensor_data, 1);s
    // Serial.println("\t in grams");

    // Serial.print("Scale reading:\t");
    // Serial.print(raw_data, 1);
    // Serial.println("\t in grams");
    
    // Serial.println(raw_data, 1);

    return filtered_sensor_data;
}
