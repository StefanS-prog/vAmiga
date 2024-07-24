// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "AudioFilterTypes.h"
#include "SubComponent.h"

/* Audio filter emulation is based on the pt2 clone project by 8bitbubsy:
 * https://github.com/8bitbubsy/pt2-clone
 *
 * The filter pipeline consists of three stages:
 *
 *     Stage 1: A static low-pass filter
 *     Stage 2: The so called "LED filter"
 *     Stage 3: A static high-pass filter
 *
 * The A1000 always applies all three filters, no matter what. On the A500
 * and A2000, the second filter is bypassed when the power LED is dimmed.
 * The A1200 differs from this scheme. It has no low-pass filter and uses
 * a different cutoff frequency for the high-pass filter.
 *
 * vAmiga supports the following filter types (OPT_FILTER_TYPE):
 *
 *     FILTER_NONE:     No filter is applied.
 *     FILTER_A500:     Runs all three filter stages, except stage 2 if the
 *                      power LED is dimmed.
 *     FILTER_A1000:    Runs all three filter stages.
 *     FILTER_A1200:    Runs filter stage 2 and 3. Skips stage 2 if the power
 *                      LED is dimmed.
 *     FILTER_VAMIGA:   Runs the legacy filter which had been used up to
 *                      version 2.4b1. This filter is deprecated and will be
 *                      deleted in future.
 *
 * The remaining filter types are meant for debugging:
 *
 *     FILTER_LOW:      Runs the low-pass filter, only.
 *     FILTER_LED:      Runs the LED filter, only. Ignored the LED state.
 *     FILTER_HIGH:     Runs the high-pass filter, only.
 */

namespace vamiga {

//
// Butterworth filter (used in vAmiga up to v2.3)
//

struct ButterworthFilter : CoreObject {

    // Coefficients of the butterworth filter
    double a1 = 0.0;
    double a2 = 0.0;
    double b0 = 0.0;
    double b1 = 0.0;
    double b2 = 0.0;

    // The butterworth filter pipeline
    double x1 = 0.0;
    double x2 = 0.0;
    double y1 = 0.0;
    double y2 = 0.0;

    void setSampleRate(double sampleRate);

    const char *objectName() const override { return "Butterworth"; }
    void _dump(Category category, std::ostream& os) const override { };

    // Initializes the filter pipeline with zero elements
    void clear();

    // Inserts a sample into the filter pipeline
    float apply(float sample);
};

//
// OnePoleFilter (based on 8bitbubsy/pt2-clone)
//

struct OnePoleFilter : CoreObject {

    // Filter properties
    double cutoff;

    // Filter coefficients
    double a1;
    double a2;

    // Filter pipeline
    double tmpL;
    double tmpR;

    const char *objectName() const override { return "OnePoleFilter"; }
    void _dump(Category category, std::ostream& os) const override { };

    // Initializes the filter coeeficients
    void setup(double sampleRate, double R1, double C1);
    void setup(double sampleRate, double cutoff);

    // Initializes the filter pipeline with zero elements
    void clear();

    // Applies the filter to a sample pair as a low-pass or high-pass filter
    void applyLP(double &l, double &r);
    void applyHP(double &l, double &r);
};

struct TwoPoleFilter : CoreObject {

    // Filter properties
    double cutoff;
    double qFactor;

    // Filter coefficients
    double a1;
    double a2;
    double b1;
    double b2;

    // Filter pipeline
    double tmpL[4];
    double tmpR[4];

    const char *objectName() const override { return "TwoPoleFilter"; }
    void _dump(Category category, std::ostream& os) const override { };

    // Initializes the filter coeeficients
    void setup(double sampleRate, double R1, double R2, double C1, double C2);
    void setup(double sampleRate, double cutoff, double qFactor);

    // Initializes the filter pipeline with zero elements
    void clear();

    // Applies the filter to a sample pair as a low-pass filter
    void applyLP(double &l, double &r);
};


class AudioFilter : public SubComponent {
    
    Descriptions descriptions = {{

        .type           = COMP_AUDIO_FILTER,
        .name           = "AudioFilter",
        .description    = "Audio Filter",
        .shell          = "filter"
    }};

    ConfigOptions options = {

        OPT_AUD_FILTER_TYPE
    };

    friend class AudioPort;

public:

    static const double pi;

private:

    // Current configuration
    AudioFilterConfig config = {};

    // The filter pipeline
    OnePoleFilter loFilter;
    TwoPoleFilter ledFilter;
    OnePoleFilter hiFilter;

    // Legacy filters (used up to vAmiga 2.4b1)
    ButterworthFilter butterworthL;
    ButterworthFilter butterworthR;


    //
    // Initializing
    //
    
public:
    
    using SubComponent::SubComponent;
    
    AudioFilter& operator= (const AudioFilter& other) {

        return *this;
    }

    
    //
    // Methods from Serializable
    //
    
private:
        
    template <class T>
    void serialize(T& worker)
    {
        if (isResetter(worker)) return;

        worker

        << config.filterType;

    } SERIALIZERS(serialize);
    
public:

    const Descriptions &getDescriptions() const override { return descriptions; }


    //
    // Methods from CoreComponent
    //

private:

    void _dump(Category category, std::ostream& os) const override;
    

    //
    // Methods from Configurable
    //

public:

    const AudioFilterConfig &getConfig() const { return config; }
    const ConfigOptions &getOptions() const override { return options; }
    i64 getOption(Option option) const override;
    void setOption(Option option, i64 value) override;

private:

    // Sets up the filter based on the current filter type and sample rate
    void setup(double sampleRate);
    void setupLoFilter(double sampleRate);
    void setupLedFilter(double sampleRate);
    void setupHiFilter(double sampleRate);

    
    //
    // Querying
    //

    bool loFilterEnabled() const;
    bool ledFilterEnabled() const;
    bool hiFilterEnabled() const;
    bool legacyFilterEnabled() const;


    //
    // Using
    //

public:
    
    // Initializes the filter pipeline with zero elements
    void clear();
};

}
