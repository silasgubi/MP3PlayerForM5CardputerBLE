#pragma once
#include <math.h>
#include <stdint.h>

#define EQ_BANDS 7

static const float EQ_FREQS[EQ_BANDS]  = { 60.0f, 170.0f, 310.0f, 600.0f, 3000.0f, 6000.0f, 12000.0f };
static const char* EQ_LABELS[EQ_BANDS] = { " 60Hz", "170Hz", "310Hz", "600Hz", " 3kHz", " 6kHz", "12kHz" };

struct BiquadFilter {
    float b0, b1, b2, a1, a2;
    float z1L = 0, z2L = 0;
    float z1R = 0, z2R = 0;

    void computeShelf(float freq, float gainDb, float sampleRate, bool isLow) {
        float A  = powf(10.0f, gainDb / 40.0f);
        float w0 = 2.0f * M_PI * freq / sampleRate;
        float cosw = cosf(w0), sinw = sinf(w0);
        float S  = 1.0f;
        float alpha = sinw / 2.0f * sqrtf((A + 1.0f/A) * (1.0f/S - 1.0f) + 2.0f);
        float sqA = sqrtf(A) * 2.0f * alpha;

        float a0;
        if (isLow) {
            b0 =  A * ((A+1) - (A-1)*cosw + sqA);
            b1 =  2*A * ((A-1) - (A+1)*cosw);
            b2 =  A * ((A+1) - (A-1)*cosw - sqA);
            a0 =       (A+1) + (A-1)*cosw + sqA;
            a1 = -2  * ((A-1) + (A+1)*cosw);
            a2 =       (A+1) + (A-1)*cosw - sqA;
        } else {
            b0 =  A * ((A+1) + (A-1)*cosw + sqA);
            b1 = -2*A * ((A-1) + (A+1)*cosw);
            b2 =  A * ((A+1) + (A-1)*cosw - sqA);
            a0 =       (A+1) - (A-1)*cosw + sqA;
            a1 =  2  * ((A-1) - (A+1)*cosw);
            a2 =       (A+1) - (A-1)*cosw - sqA;
        }
        b0 /= a0; b1 /= a0; b2 /= a0; a1 /= a0; a2 /= a0;
    }

    void computePeak(float freq, float gainDb, float Q, float sampleRate) {
        float A  = powf(10.0f, gainDb / 40.0f);
        float w0 = 2.0f * M_PI * freq / sampleRate;
        float alpha = sinf(w0) / (2.0f * Q);
        float a0 = 1.0f + alpha / A;
        b0 = (1.0f + alpha * A) / a0;
        b1 = (-2.0f * cosf(w0))  / a0;
        b2 = (1.0f - alpha * A) / a0;
        a1 = b1;
        a2 = (1.0f - alpha / A) / a0;
    }

    void reset() { z1L = z2L = z1R = z2R = 0.0f; }

    inline void processStereo(int16_t& left, int16_t& right) {
        float inL = left, inR = right;
        float outL = b0*inL + z1L;  z1L = b1*inL - a1*outL + z2L;  z2L = b2*inL - a2*outL;
        float outR = b0*inR + z1R;  z1R = b1*inR - a1*outR + z2R;  z2R = b2*inR - a2*outR;
        left  = (int16_t)constrain((int)outL, -32768, 32767);
        right = (int16_t)constrain((int)outR, -32768, 32767);
    }
};

class Equalizer {
public:
    int8_t gain[EQ_BANDS] = {0};
    bool   enabled = true;

    void update(float sampleRate) {
        for (int i = 0; i < EQ_BANDS; i++) {
            if (i == 0)
                filters[i].computeShelf(EQ_FREQS[i], gain[i], sampleRate, true);
            else if (i == EQ_BANDS - 1)
                filters[i].computeShelf(EQ_FREQS[i], gain[i], sampleRate, false);
            else
                filters[i].computePeak(EQ_FREQS[i], gain[i], 1.4f, sampleRate);
            filters[i].reset();
        }
    }

    inline void process(int16_t sample[2]) {
        if (!enabled) return;
        for (int i = 0; i < EQ_BANDS; i++)
            filters[i].processStereo(sample[0], sample[1]);
    }

    bool isFlat() const {
        for (int i = 0; i < EQ_BANDS; i++) if (gain[i] != 0) return false;
        return true;
    }

private:
    BiquadFilter filters[EQ_BANDS];
};

static Equalizer globalEQ;
