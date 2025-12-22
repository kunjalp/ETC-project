#include "mbed.h"
#include <cmath>
#include <chrono>
using namespace std::chrono_literals;

AnalogIn my_analog_pin0{PA_4};
AnalogIn my_analog_pin1{PA_3};

const float ADC_value = 3.3f; 

const float APPS0_min = 0.25f;
const float APPS0_max = 2.25f;

const float APPS1_min = 0.30f;
const float APPS1_max = 2.70f;

const float max_deviation = 10.0f;
const int max_duration = 100;

Timer my_timer;
bool check = false; // check if currently implausibile

/**
 * Convert voltage to coressponding pedal percentage.
 * If voltage is more or less than expected, it marks it as implausbile.
 */
float voltToPercent(float voltage, float v_min, float v_max) {
   if (v_max == v_min) return 0.0f; 

   // Check for implausbility: If voltage is outside the expected range, 
   // we return -1.0f, which is an impossible pedal position, signaling a failure.
   if (voltage > v_max || voltage < v_min) {
      return -1.0f; // implausibility signal
   }
    
   // If not, return associated percentage
   return (voltage - v_min) / (v_max - v_min) * 100.0f;
}

int main() {
   printf("Hello\n");
   my_timer.start();
     
   const auto loop_delay = 10ms;

   while (true) {
      float value0 = my_analog_pin0.read();
      float value1 = my_analog_pin1.read();


      float voltage0 = value0 * ADC_value;
      float voltage1 = value1 * ADC_value;
      printf("V0:%.3fV | V1:%.3fV ", voltage0, voltage1);

      float pedal0 = voltToPercent(voltage0, APPS0_min, APPS0_max);
      float pedal1 = voltToPercent(voltage1, APPS1_min, APPS1_max);
      printf("APPS0:%.3f%% | APPS1:%.3f%% | ", pedal0, pedal1);

      // check for clamped voltage implausbility
      bool clamped_implausible = (pedal0 == -1.0f) || (pedal1 == -1.0f);
         
      float pedal_difference = 0.0f;
         
      // Only calculate the difference if both readings are valid
      if (!clamped_implausible) {
         pedal_difference = std::fabs(pedal0 - pedal1);
      }

      // Official check for implausbility
         if (clamped_implausible || pedal_difference > max_deviation) {
             
            // Start timer
            if (!check) {
               my_timer.reset();
               check = true;
                     
               // Print reasosn for implausbility
               if (clamped_implausible) {
                  printf("Implausibility detected of Clamped Voltage\n");
               } else {
                  printf("Implausibility detected of Deviation\n");
               }
            }

            // Check if implausibility is more than 100ms
            if (my_timer.elapsed_time() > max_duration * 1ms) { 
               printf("Cutting Power: 0\n");
            }
         } else {
            float pedal_average = (pedal0 + pedal1) / 2.0f;
             
            // Cleared, so reset
            if (check) {
               check = false;
               printf("Clearing Implausibility\n");
               printf("Average Pedal:%.0f%%\n", pedal_average);
            }
                     
            printf("Torque:%.0f\n", pedal_average);
         }
             
      ThisThread::sleep_for(loop_delay); 
   }
}