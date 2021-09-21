/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef LFPLATENCYPROCESSOR_H_INCLUDED
#define LFPLATENCYPROCESSOR_H_INCLUDED

#ifdef _WIN32


#define NOGDI
#define NOMINMAX


#include <Windows.h>
#endif
#include <string>
#include <ProcessorHeaders.h>
#include <functional>
#include <map>
#include <unordered_map>
#include <queue>

//fifo buffer size. height in pixels of spectrogram image
#define FIFO_BUFFER_SIZE 30000

//Width in pixels of spectrogram image
//300 pixels = 300 tracks approx 5 min
#define SPECTROGRAM_WIDTH 300

#define SPECTROGRAM_HEIGHT 300

#define PPCONTROLLER_WIDTH 305

#define PPCONTROLLER_HEIGHT 130

#define EVENT_DETECTION_THRESHOLD 1500

#define DATA_CACHE_SIZE_SAMPLES 30000

#define DATA_CACHE_SIZE_TRACKS 300

//for debug
#define SEARCH_BOX_WIDTH 3

class ppController;
/**
    This class serves as a template for creating new processors.

    If this were a real processor, this comment section would be used to
    describe the processor's function.

    @see GenericProcessor
*/
class LfpLatencyProcessor : public GenericProcessor, public MultiTimer

{
public:
    /** The class constructor, used to initialize any members. */
    LfpLatencyProcessor();

    /** The class destructor, used to deallocate memory */
    ~LfpLatencyProcessor();

    /** Indicates if the processor has a custom editor. Defaults to false */
    //bool hasEditor() const { return true; }

    /** If the processor has a custom editor, this method must be defined to instantiate it. */
    AudioProcessorEditor* createEditor() override;

    /** Optional method that informs the GUI if the processor is ready to function. If false acquisition cannot start. Defaults to true */
    //bool isReady();
    
    /** Convenient interface for responding to incoming events. */
    //NOTE NOT CURRENTLY USED! Events detected in process() instead
    //void handleEvent (const EventChannel* eventInfo, const MidiMessage& event, int sampleNum) override;

    /** Defines the functionality of the processor.
        The process method is called every time a new data buffer is available.
    */
    void process (AudioSampleBuffer& buffer) override;
    
    /** The method that standard controls on the editor will call.
        It is recommended that any variables used by the "process" function
        are modified only through this method while data acquisition is active. */
    void setParameter (int parameterIndex, float newValue) override;

	/** This method is a critical section, protected a mutex lock. Allows you to save slider values, and maybe
	some data if you wanted in a file called LastLfpLatencyPluginComponents */
	static void saveRecoveryData(std::unordered_map<std::string, juce::String>* valuesMap);

	/** Starts by asking the user if they would like to load data from LastLfpLatencyPluginComponents, 
	the rest is a critical section protected by the same mutex lock as saveRecoveryData. */
	static void loadRecoveryData(std::unordered_map<std::string, juce::String>* valuesMap);

    /** Saving custom settings to XML. */
    virtual void saveCustomParametersToXml (XmlElement* parentElement) override;

    /** Load custom settings from XML*/
    virtual void loadCustomParametersFromXml() override;

    virtual void createEventChannels() override;

    //virtual void createSpikeChannels() override;


    /** Optional method called every time the signal chain is refreshed or changed in any way.

        Allows the processor to handle variations in the channel configuration or any other parameter
        passed down the signal chain. The processor can also modify here the settings structure, which contains
        information regarding the input and output channels as well as other signal related parameters. Said
        structure shouldn't be manipulated outside of this method.
    */
    //void updateSettings();

    // Channel used for the recording of spike data
    //virtual void createSpikeChannels() override;
    
    
     /** Method to allow the visualizer to check eventDetection flag*/ 
    bool checkEventReceived();
    
    /** Method to allow the visualizer to reset the eventDetection flag*/ 
    void resetEventFlag();
    
    /**
     Returns pointer to first element of latency track data of last (second most current) track. Equivalent to getdataCacheRow(1)
     - Returns: pointer to first element of last (second most current) latency track
     */
    float* getdataCacheLastRow();
    
    /**
     Returns pointer to first lement of raw circular array.

     - Returns: pointer to raw circular array
     */
    float* getdataCache();

    //Sets data channel back to default
    void resetDataChannel();

    //Sets trigger channle to default
    void resetTriggerChannel();
    
    /**
     Returns pointer stored latency track data, one track at a time
     - Parameter track: index of track (0 being current (earliest) track)
     
     - Throws: 'std::out_of_range'
                if 'track' index is out of range
     - Returns: pointer to first element of latency track
     
     */
    float* getdataCacheRow(int track);
    
    void changeParameter(int parameterID, int value);

	int getParameterInt(int parameterID);

    int getSamplesPerSubsampleWindow();
    
    void pushLatencyData(int latency);
    
    int getLatencyData(int track);
/*
    int windowSampleCount;
    
    float lastWindowPeak;
    
    int samplesAfterStimulus;
 */
    
    int fifoIndex;

    int currentTrack;
    int currentSample;

	 //debug
	 float getParameterFloat(int parameterID);
	 //Result makingFile;

   //Functions used to save data
   void addMessage(std::string message);

   void addSpike(std::string spike);
    struct SpikeInfo{
        int spikeSampleNumber; // the recording sample number of the spike
        int spikeSampleLatency; // the spike time relative to the stimulus
        int spikePeakValue; // the peak value
        int windowSize; // the number of samples used to identify spike
        int threshold; // the threshold value for the spike, used to detect
    };
    struct SpikeGroup{
        std::vector<SpikeInfo> spikeInfos;
        std::vector<bool> recentHistory;
        SpikeInfo templateSpike; // the information used to determine the spike
        char uid; // #TODO: create uid
    };
    void addSpikeGroup(SpikeInfo templateSpike);
    void removeSpikeGroup(int i);
    std::vector<SpikeGroup> getSpikeGroups();
private:

   friend class ppController;

	//debug
	float lastReceivedDACPulse;

	int dataChannel_idx;
	int triggerChannel_idx;

	int triggerChannel_threshold;

	void timerCallback(int timerID) override;

    void trackSpikes(); // updates the currently tracked spike group

    std::vector<SpikeGroup> spikeGroups;
    
    float dataCache[DATA_CACHE_SIZE_TRACKS*DATA_CACHE_SIZE_SAMPLES];
    
    int spikeLocation[DATA_CACHE_SIZE_TRACKS];
    
    bool eventReceived;
    
    EventChannel* pulsePalEventPtr;
    EventChannel* spikeEventPtr;

    //int currentSample;
    
    
    int samplesPerSubsampleWindow;

    
    //int currentTrack;
    
    int peakThreshold;

    int samplesAfterStimulusStart;

    float stimulus_threshold;
    
    std::queue<String> messages;
    std::queue<String> spikes;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfpLatencyProcessor);
};


#endif  // LFPLATENCYPROCESSOR_H_INCLUDED
