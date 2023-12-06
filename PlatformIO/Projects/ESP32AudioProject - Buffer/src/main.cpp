#include <Arduino.h>
#include <SD.h>  
#include <FS.h>  
#include <SPI.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>

const int chipSelect = 33; //SD card module's chip select pin
const int bitsPerSample = 16; 
const int numChannels = 1;

const int CHUNK_SIZE = 4096;


const int ADC_PIN = 36; // ADC input
const int MAX_SAMPLES = 40960; // Maximum length of signal
const int DAC_PIN = 25; // DAC Output to speaker pin
const int SAMPLE_RATE = 16000; //----> This is Fs <-----

int adcValues[CHUNK_SIZE]; // Pointer to store ADC values
int sampleCount = 0; // Counter for samples

File audioFile;

float top3Frequencies[3] = {0, 0, 0};

float freq1;
float freq2;
float freq3;

void CreateFile(char filename[]);
void appendAudioToWav(int* audioData, int dataLength,char filename[]);
void finalizeWavFile(char filename[]);
void removeDCOffset(int* data, int length);
void PreAmplifyAudio(int* data, int length, int factor);
void LowPassRecursive(int* data, int length);
float getMaxTop3Frequencies(int* data, int length);

void setup() {
  Serial.begin(9600);
  //Set adc to be a
  pinMode(27, OUTPUT); //Constant High signal
  pinMode(14, INPUT); //Switch 1 signal
  pinMode(12, INPUT); //Switch 2 signal
  pinMode(ADC_PIN, INPUT); //ADC pin
  digitalWrite(27, HIGH);

  if (!SD.begin(chipSelect)) {
    Serial.println("[uSD card not mounted]");
    return;
  }
  else {
    Serial.println("[uSD card installed]");
  }

}

void loop() {
  if (digitalRead(14) == HIGH) {
    Serial.println("Switch 1 is active (Recording...)");
    CreateFile("/audio_temp.wav");
    sampleCount = 0;
    while (digitalRead(14) == HIGH && sampleCount < MAX_SAMPLES) {
      for (int i = 0; i < CHUNK_SIZE; ++i) {
      adcValues[i] = analogRead(ADC_PIN); // Read ADC value
      delayMicroseconds(62.5); // Adjust delay based on your requirements
      }

      Serial.println("...>>Process>>...");
      removeDCOffset(adcValues, CHUNK_SIZE); //this works
      PreAmplifyAudio(adcValues, CHUNK_SIZE, 15); //this works
      if (digitalRead(12) == HIGH) {
        LowPassRecursive(adcValues,CHUNK_SIZE);
        LowPassRecursive(adcValues,CHUNK_SIZE);
        LowPassRecursive(adcValues,CHUNK_SIZE);
        LowPassRecursive(adcValues,CHUNK_SIZE);
      }
      appendAudioToWav(adcValues, CHUNK_SIZE, "/audio_temp.wav");

      //(freq1,freq2,freq3) = getMaxTop3Frequencies(adcValues, CHUNK_SIZE);

      sampleCount += CHUNK_SIZE;

    }
    finalizeWavFile("/audio_temp.wav"); //this works

    Serial.println("Done recording");
    Serial.println(sampleCount);
  }
  delay(5000);
}

void CreateFile(char filename[]) {
  audioFile = SD.open(filename, FILE_WRITE);
  uint8_t header[44] = {
    'R', 'I', 'F', 'F', // ChunkID
    36, 79, 1, 0,         // Placeholder for ChunkSize (to be updated)
    'W', 'A', 'V', 'E', // Format
    'f', 'm', 't', ' ', // Subchunk1ID
    16, 0, 0, 0,        // Subchunk1Size
    1, 0,               // AudioFormat (PCM)
    1, 0,               // NumChannels (Mono)
    0x40, 0x3E, 0x00, 0x00, // SampleRate (8000 Hz)
    0x80, 0x7C, 0x00, 0x00, // ByteRate (SampleRate * NumChannels * BitsPerSample/8)
    2, 0,               // BlockAlign (NumChannels * BitsPerSample/8)
    16, 0,              // BitsPerSample (16 bits)
    'd', 'a', 't', 'a', // Subchunk2ID
    0x00, 0x50, 0x3D, 0x00          // Placeholder for Subchunk2Size (to be updated)
  };
  audioFile.write(header, 44);
  audioFile.close();
  Serial.println("Created file...");
}

void appendAudioToWav(int* audioData, int dataLength,char filename[]) {
  audioFile = SD.open(filename, FILE_APPEND);
  
  if (!audioFile) {
    Serial.println("Error opening file for appending!");
    return;
  }

  // Write audio data to the file
  for (int i = 0; i < dataLength; ++i) {
    audioFile.write((const uint8_t *)&audioData[i], sizeof(int));
  }

  audioFile.close();
  Serial.println("Audio data appended.");
}


void finalizeWavFile(char filename[]) {
  audioFile = SD.open(filename, FILE_READ);
  
  int dataSize = audioFile.size() - 44;

  // Update ChunkSize (Bytes 4-7)
  audioFile.seek(4);
  int chunkSize = dataSize + 36;
  audioFile.write((const uint8_t *)&chunkSize, 4);

  // Update Subchunk2Size (Bytes 40-43)
  audioFile.seek(40);
  audioFile.write((const uint8_t *)&dataSize, 4);

  audioFile.close();

  Serial.println("Audio file finalized as WAV.");

}



void removeDCOffset(int* data, int length) {
  Serial.println(">>Removed DC Offset");
  int sum = 0;

  // Calculate sum of all samples
  for (int i = 0; i < length; ++i) {
    sum += data[i];
  }

  // Calculate the mean (average) value
  float mean = sum / length;

  // Remove DC offset by subtracting the mean from each sample
  for (int i = 0; i < length; ++i) {
    data[i] -= mean;
  }
}

// Function to normalize audio data to a specific range
void PreAmplifyAudio(int* data, int length, int factor) {
  Serial.println(">>PreAmp");
  for (int i = 0; i < length; ++i) {
    data[i] *= factor;
  }
}


float getMaxTop3Frequencies(int* data, int length) {
    float dftResult[length];
    
    // Calculate DFT for each frequency component
    for (int k = 0; k < length; ++k) {
        dftResult[k] = 0;
        for (int n = 0; n < length; ++n) {
            float term = data[n] * exp(-2.0 * M_PI * k * n / length);
            dftResult[k] += term;
        }
    }

    //Find top three frequencies with the highest magnitude
    for (int i = 0; i < length; ++i) {
        if (std::abs(dftResult[i]) > std::abs(top3Frequencies[0])) {
            top3Frequencies[2] = top3Frequencies[1];
            top3Frequencies[1] = top3Frequencies[0];
            top3Frequencies[0] = dftResult[i];
        } else if (std::abs(dftResult[i]) > std::abs(top3Frequencies[1])) {
            top3Frequencies[2] = top3Frequencies[1];
            top3Frequencies[1] = dftResult[i];
        } else if (std::abs(dftResult[i]) > std::abs(top3Frequencies[2])) {
            top3Frequencies[2] = dftResult[i];
        }
    }

     //Return the maximum value among the top three frequencies
    return std::abs(top3Frequencies[0]), std::abs(top3Frequencies[1]), std::abs(top3Frequencies[2]);
}

/////////////////LOW PASS RECURSIVE FILTER//////////////////////
void LowPassRecursive(int* data, int length) {
  Serial.println(">>Recursive Filter");
  float fc = 0.1325; // Cutoff of 2500Hz with respect to 8000Hz fs
  float X = exp(-2.0 * M_PI * fc);
  float A0 = 1 - X;

  // Intermediate variable to store the previous output
  float prevOutput = 0.0;

  for (int i = 0; i < length; ++i) {
      // Convert int to float
      float currentInput = static_cast<float>(data[i]);

      // Perform the recursive filtering
      float currentOutput = A0 * currentInput + X * prevOutput;

      // Convert float value back to integer and update the data array
      data[i] = static_cast<int>(currentOutput);

      // Update prevOutput for the next iteration
      prevOutput = currentOutput;
  }
}
