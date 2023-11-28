#include "pcm.h"

static volatile uint32_t timerCode = 0;

unsigned int pcmBr = 0;
unsigned int pcmDataToRead = 0;
uint16_t pcmBufferIndex = 0;
int16_t pcmBuffer[DECODE_BUFFERSIZE];
int16_t* inBuffer;
uint8_t fillingBuffer = 0;
FIL file;
uint8_t res;
float sampleRate = 44100.0; // Sample rate of the audio data
float pitchFactor = 1.5; // Adjust this value to change the pitch
uint16_t maxVolume = 1000;

uint8_t nextSongFlag = 0;
uint8_t continueFlag = 0;
uint8_t pauseFlag = 0;
uint8_t startFlag = 0;
uint8_t playingFlag = 0;
uint8_t fillBufFlag = 0;

uint32_t targetAudioFrequency = 16000;
uint16_t fxMode = 11;

float volume = 0.2;

#define PI 3.14159265358979323846
#define SAMPLE_RATE 44100
#define CARRIER_FREQUENCY 1000
#define CARRIER_AMPLITUDE 1
#define TABLE_SIZE 4096
#include <math.h>

float sine_table[TABLE_SIZE];

void initialize_sine_table() {  
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        sine_table[i] = (float)(sin(4 * PI * i / TABLE_SIZE)) * 10;
    }
}   

void highAndLow(short* audioData, int size) {
    for (int i = 0; i < size; i++) {
        audioData[i] = audioData[i] * sine_table[timerCode] / 10;
        if (i % 5 == 0) {
            timerCode++;
        }
        if (timerCode >= TABLE_SIZE) {
            timerCode = 0;
        }
    }
}

#define distortion 1.05
void distorted(uint16_t* audio_data, int size) {
    for (int i = 0; i < size; i++) {
        audio_data[i] = (uint16_t)(audio_data[i] / distortion);
    }
}

// too slow
// void add_constant_tone(uint16_t* audio_data, uint32_t size, int sample_rate, float frequency) {
//     // Calculate the angular frequency of the tone
//     float angular_frequency = 2.0 * PI * frequency;

//     // Generate the sine wave and add it to the audio data
//     for (int i = 0; i < size; i++) {
//         float time = (float)i / sample_rate;
//         float sample = sin(angular_frequency * time);
//         audio_data[i] += (uint16_t)(sample * 500.0);
//     }
// }


void changeI2SFreq(uint32_t AudioFreq) {
    /* Disable I2S peripheral to allow access to I2S internal registers */
    HAL_I2S_DeInit(&hi2s2);

    /* Update the I2S audio frequency configuration */
    hi2s2.Init.AudioFreq = AudioFreq;

    /* Enable I2S peripheral */
    HAL_I2S_Init(&hi2s2);
}


#define dt 1.0 / SAMPLE_RATE
#define robotFactor 12.0

double pitchShift = pow(2.0, robotFactor / 12.0);

void robotizeAudioData(short* audioData, int size) {

    for (int i = 0; i < size; i++) {
        double time = (double)i / sampleRate;
        double shiftedTime = time * pitchShift;
        int shiftedIndex = (int)(shiftedTime * sampleRate);
        audioData[i] = audioData[shiftedIndex];
    }
}

uint8_t playPCMFileInit(char *filename)
{
    changeSpeed();

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK)
    {
        return 0;
    }
    res = f_read(&file, pcmBuffer, DECODE_BUFFERSIZE * 2, &pcmBr);
    for (int i = 0; i < DECODE_BUFFERSIZE; i++)
    {
        pcmBuffer[i] = pcmBuffer[i] * volume;
    }
    // distorted(pcmBuffer, DECODE_BUFFERSIZE);
    addFx(pcmBuffer, DECODE_BUFFERSIZE);

    if (res != FR_OK)
    {
        return 0;
    }
    if (pcmBr == 0)
    {
        return 0;
    }
    pcmBufferIndex = 0;
    playingFlag = 1;
    fillBufFlag = 0;
    fillingBuffer = 1;

    HAL_I2S_Transmit_DMA(&hi2s2, &pcmBuffer, DECODE_BUFFERSIZE);

    return 1;
}

uint16_t getMaxVolume(uint16_t* audioData, int size) {
    uint16_t max = 0;
    for (int i = 0; i < size; i++) {
        if (audioData[i] > max) {
            max = audioData[i];
        }
    }
    return max;
}

void changeSpeed() {
    switch (fxMode)
    {
    case 11:
        targetAudioFrequency = 16000;
        break;
    case 12:
        targetAudioFrequency = 12000;
        break;
    case 13:
        targetAudioFrequency = 32000;
        break;
    case 21:
        targetAudioFrequency = 48000;
        break;
    case 22:
        targetAudioFrequency = 12000;
        break;
    case 23:
        targetAudioFrequency = 24000;
        break;
    
    default:
        break;
    }
    changeI2SFreq(targetAudioFrequency);
}
void addFx(uint16_t* audioData, int size) {
    dvModule();
    switch (fxMode) {
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 21:
            distorted(audioData, size);
            // highAndLow(audioData, size);
            break;
        case 22:
            distorted(audioData, size);
            // highAndLow(audioData, size);
            break;
        case 23:
            // distorted(audioData, size);
            highAndLow(audioData, size);
            break;
        default:
            targetAudioFrequency = 16000;
            break;
    }
}

void playPCMFile(char *filename) {
    initialize_sine_table();
    playPCMFileInit(filename);
    while (playingFlag)
    {
    	if (nextSongFlag == 1) {
            break;
    	}
        if (!playPCMFlag) {
            break;
        }
        if (fillBufFlag)
        {
            res = f_read(&file, inBuffer, DECODE_BUFFERSIZE, &pcmBr);
            for (int i = 0; i < DECODE_BUFFERSIZE / 2; i++)
            {
                inBuffer[i] = inBuffer[i] * volume;
            }
            addFx(inBuffer, DECODE_BUFFERSIZE / 2);
            if (res != FR_OK)
            {
                nextSongFlag = 1;
                playingFlag = 0;
                break;
            }
            if (pcmBr == 0)
            {
                nextSongFlag = 1;
                playingFlag = 0;
                break;
            }
            if (kay == 1) {         // K1 pressed!
				kay = 0;
                nextSongFlag = 1;
                playingFlag = 0;
                break;
            }
            if (kerry == 1) {       // K2 pressed!
                kerry = 0;
                playingFlag = 0;    // For now we just "pause" by clearing this flag
                break;
            }
            pcmBufferIndex = 0;
            fillBufFlag = 0;
        }
    }
    playPCMFileEnd();
}

void playPCMFileEnd()
{
    playingFlag = 0;
    f_close(&file);
    HAL_I2S_DMAStop(&hi2s2);
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    inBuffer = &pcmBuffer[0];
    fillingBuffer = 0;
    fillBufFlag = 1;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    inBuffer = &pcmBuffer[DECODE_BUFFERSIZE / 2];
    fillingBuffer = 1;
    fillBufFlag = 1;
}
