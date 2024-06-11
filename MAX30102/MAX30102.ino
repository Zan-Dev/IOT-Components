#include <MAX3010x.h>
#include "filters.h"
#include <EEPROM.h>

MAX30105 sensor;

const int bpmAddress = 0;
const int spo2Address = sizeof(int);

unsigned long startTime;
unsigned long duration = 60000;

const auto kSamplingRate = sensor.SAMPLING_RATE_400SPS;
const float kSamplingFrequency = 400.0;

// Finger Detection Threshold and Cooldown
const unsigned long kFingerThreshold = 10000;
const unsigned int kFingerCooldownMs = 100;

// Edge Detection Threshold (decrease for MAX30100)
const float kEdgeThreshold = -2000.0;

// Filters
const float kLowPassCutoff = 5.0;
const float kHighPassCutoff = 0.5;

// Averaging
bool kEnableAveraging = false;
const int kAveragingSamples = 5;
const int kSampleThreshold = 5;

// Filter Instances
LowPassFilter low_pass_filter_red(kLowPassCutoff, kSamplingFrequency);
LowPassFilter low_pass_filter_ir(kLowPassCutoff, kSamplingFrequency);
HighPassFilter high_pass_filter(kHighPassCutoff, kSamplingFrequency);
Differentiator differentiator(kSamplingFrequency);
MovingAverageFilter<kAveragingSamples> averager_bpm;
MovingAverageFilter<kAveragingSamples> averager_r;
MovingAverageFilter<kAveragingSamples> averager_spo2;

// Statistic for pulse oximetry
MinMaxAvgStatistic stat_red;
MinMaxAvgStatistic stat_ir;

// R value to SpO2 calibration factors
float kSpO2_A = 1.5958422;
float kSpO2_B = -34.6596622;
float kSpO2_C = 112.6898759;

// Timestamp of the last heartbeat
long last_heartbeat = 0;

// Timestamp for finger detection
long finger_timestamp = 0;
bool finger_detected = false;

// Last diff to detect zero crossing
float last_diff = NAN;
bool crossed = false;
long crossed_time = 0;

void setup() {
  Serial.begin(115200);

  if(sensor.begin() && sensor.setSamplingRate(kSamplingRate)) { 
    Serial.println("Sensor MAX30102 Ready!");
  }
  else {
    Serial.println("Sensor not found");  
    while(1);
  }

}

void loop() {
  if (startTime == 0) {
    startTime = millis(); // Inisialisasi waktu mulai
  } 

  auto sample = sensor.readSample(1000);
  float current_value_red = sample.red;
  float current_value_ir = sample.ir;
  
  // Detect Finger using raw sensor value
  if(sample.red > kFingerThreshold) {
    if(millis() - finger_timestamp > kFingerCooldownMs) {
      finger_detected = true;
    }
  }
  else {
    // Reset values if the finger is removed
    differentiator.reset();
    averager_bpm.reset();
    averager_r.reset();
    averager_spo2.reset();
    low_pass_filter_red.reset();
    low_pass_filter_ir.reset();
    high_pass_filter.reset();
    stat_red.reset();
    stat_ir.reset();
    
    finger_detected = false;
    finger_timestamp = millis();
  }

  if(finger_detected) {
    current_value_red = low_pass_filter_red.process(current_value_red);
    current_value_ir = low_pass_filter_ir.process(current_value_ir);

    // Statistics for pulse oximetry
    stat_red.process(current_value_red);
    stat_ir.process(current_value_ir);

    // Heart beat detection using value for red LED
    float current_value = high_pass_filter.process(current_value_red);
    float current_diff = differentiator.process(current_value);

    // Valid values?
    if(!isnan(current_diff) && !isnan(last_diff)) {
      
      // Detect Heartbeat - Zero-Crossing
      if(last_diff > 0 && current_diff < 0) {
        crossed = true;
        crossed_time = millis();
      }
      
      if(current_diff > 0) {
        crossed = false;
      }
  
      // Detect Heartbeat - Falling Edge Threshold
      if(crossed && current_diff < kEdgeThreshold) {
        if(last_heartbeat != 0 && crossed_time - last_heartbeat > 300) {
          // Show Results
          int bpm = 60000/(crossed_time - last_heartbeat);
          float rred = (stat_red.maximum()-stat_red.minimum())/stat_red.average();
          float rir = (stat_ir.maximum()-stat_ir.minimum())/stat_ir.average();
          float r = rred/rir;
          float spo2 = kSpO2_A * r * r + kSpO2_B * r + kSpO2_C;
          
          if (spo2 > 100){
            spo2 = 100;
          }

          if(bpm > 50 && bpm < 250) {
            // Average?
            Serial.println("===== Sensor MAX30102 =====");     

            int average_bpm = averager_bpm.process(bpm);
            int average_r = averager_r.process(r);
            int average_spo2 = averager_spo2.process(spo2);
            if (average_spo2 > 100) {
              average_spo2 = 100;
            }
            
            // Hasil Deteksi
            Serial.print("Heart Rate (bpm) : ");
            Serial.println(bpm);              
            Serial.print("SpO2 (%)         : ");
            Serial.println(spo2);   

            // Hitung Rata Rata Setiap 5 detik
            if(millis() - startTime > duration) {                
              Serial.print("Avg Heart Rate (bpm) : ");
              Serial.println(average_bpm);                
              Serial.print("Avg SpO2 (%)         : ");
              Serial.println(average_spo2);  
              EEPROM_writeInt(bpmAddress, average_bpm);
              EEPROM_writeInt(spo2Address, average_spo2);

              startTime = 0;
            }
            Serial.println();
          }

          // Reset statistic
          stat_red.reset();
          stat_ir.reset();
        }
  
        crossed = false;
        last_heartbeat = crossed_time;
      }
    }

    last_diff = current_diff;
  }

}

// Fungsi untuk simpan data dalam EEPROM
void EEPROM_writeInt(int address, int value) {
  EEPROM.put(address, value);
}

// Fungsi untuk membaca integer dari EEPROM
int EEPROM_readInt(int address) {
  int value = 0;
  EEPROM.get(address, value);
  return value;
}

// Tampilkan Nilai dalam EEPROm
void displayAvgMax() {
  int stored_bpm = EEPROM_readInt(bpmAddress);
  int stored_spo2 = EEPROM_readInt(spo2Address);

  Serial.println("===== Sensor MAX30102 =====");
  Serial.print("Avg Heart Rate (bpm) : ");
  Serial.println(stored_bpm);
  Serial.print("Avg SpO2 (%)         : ");
  Serial.println(stored_spo2);
  Serial.println();
}
