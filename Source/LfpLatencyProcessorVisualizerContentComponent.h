#ifndef __JUCE_HEADER_D83F67960ECDED8C__
#define __JUCE_HEADER_D83F67960ECDED8C__

#include <EditorHeaders.h>
#include "LfpLatencyProcessor.h"
#include "LfpLatencyProcessorVisualizerContentComponent.h"
#include "pulsePalController/ppController.h"

#include "LfpLatencySpectrogram.h"
#include "LfpLatencySpectrogramPanel.h"
#include "LfpLatencySpectrogramControlPanel.h"
#include "LfpLatencyOtherControlPanel.h"
#include "LfpLatencyRightMiddlePanel.h"

class TableContent : public TableListBoxModel
                     
{
public:
   
    TableContent();
    ~TableContent();
    
    int getNumRows();
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected);
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
    //Component* refreshComponentForCell(int rowNumber, int columnId, bool rowIsSelected, Component* exsistingComponetToUpdate);

private:    
    
    friend class LfpLatencyProcessorVisualizer;
    
    bool spikeFound = false;


};


class LfpLatencyProcessorVisualizerContentComponent : public Component,
                                                      public SliderListener,
                                                      public ButtonListener
                                                 
{
public:
    LfpLatencyProcessorVisualizerContentComponent(LfpLatencyProcessor* processor);
    ~LfpLatencyProcessorVisualizerContentComponent();

    void paint(Graphics &g) override;
    void resized() override;
    void sliderValueChanged(Slider *sliderThatWasMoved) override;
    void buttonClicked(Button *buttonThatWasClicked) override;
	bool keyPressed(const KeyPress& k) override;
    //void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) override;

    int getStartingSample() const;
    bool getExtendedColorScale() const;
    int getSubsamplesPerWindow() const;
    float getLowImageThreshold() const;
    float getHighImageThreshold() const;
    float getDetectionThreshold() const;
    int getColorStyleComboBoxSelectedId() const;
    void tryToSave();

    std::tuple<float, float, float, float, Colour> getSearchBoxInfo() const;
private:
    // Make an editor to be friendly class of this content component,
    // so the editor will have access to all methods and variables of this component.
    friend class LfpLatencyProcessorVisualizer;

    ScopedPointer<TableContent> spikeTrackerContent;
    ScopedPointer<LfpLatencySpectrogramControlPanel> spectrogramControlPanel;
    ScopedPointer<LfpLatencyOtherControlPanel> otherControlPanel;
    ScopedPointer<LfpLatencySpectrogramPanel> spectrogramPanel;
    ScopedPointer<LfpLatencyRightMiddlePanel> rightMiddlePanel;

    //Image thresholds
    float lowImageThreshold;
    float highImageThreshold;
    int colorStyle;

    int draw_imageHeight;
    int draw_rightHandEdge;

    //LfpLatencyProcessorVisualizer content;

    //SearchBoxParams
    int searchBoxLocation;

    int searchBoxWidth;

    bool spikeDetected = false;
    bool newSpikeDetected = false;
    bool extendedColorScale;
    float detectionThreshold;
    int subsamplesPerWindow;
    int startingSample;

    float bitBolts;

    float conductionDistance;

    int absPos;

    float stimulusVoltage;

    float stimulusVoltageMax;

    float stimulusVoltageMin;

	bool voltageTooHighOkay;
	bool alreadyAlerted = false;
    bool testSpikePls = false;

    bool deletes[4] = { false, false, false, false };

    bool t_deletes[4] = { false, false, false, false };

    float trackSpike_DecreaseRate;
    float trackSpike_IncreaseRate;

    bool isSaving;
    unordered_map<string, juce::String> *valuesMap;

	// setup

	ScopedPointer<Label> trackSpike_IncreaseRate_Slider_Label;
	ScopedPointer<Slider> trackSpike_IncreaseRate_Slider;
	ScopedPointer<TextEditor> trackSpike_IncreaseRate_Text;

	ScopedPointer<Label> trackSpike_DecreaseRate_Slider_Label;
	ScopedPointer<Slider> trackSpike_DecreaseRate_Slider;
	ScopedPointer<TextEditor> trackSpike_DecreaseRate_Text;

	ScopedPointer<Slider> stimulusVoltageSlider;
	ScopedPointer<Label> stimulusVoltageSliderLabel;

	ScopedPointer<TextEditor> stimulusVoltageMax_text;
	ScopedPointer<Label> stimulusVoltageMax_textLabel;

	ScopedPointer<TextEditor> stimulusVoltage_text;
	ScopedPointer<Label> stimulusVoltage_textLabel;

	ScopedPointer<TextEditor> stimulusVoltageMin_text;
	ScopedPointer<Label> stimulusVoltageMin_textLabel;
	
	// main GUI
    ScopedPointer<ComboBox> colorStyleComboBox;
	ScopedPointer<Label> colorStyleComboBoxLabel;
    
    ScopedPointer<ToggleButton> extendedColorScaleToggleButton;
    ScopedPointer<Label> extendedColorScaleToggleButtonLabel;
	
    ScopedPointer<Label> cmLabel;
 
    //ScopedPointer<GroupComponent> detectionControlGroup;

	// Stimulus control
	ScopedPointer<ppController> ppControllerComponent;

    ScopedPointer<TextEditor> textBox1;
    ScopedPointer<TextEditor> textBox2;

    ScopedPointer<ComboBox> triggerChannelComboBox;
	ScopedPointer<Label> triggerChannelComboBoxLabel;

    ScopedPointer<ComboBox> dataChannelComboBox;
	ScopedPointer<Label> dataChannelComboBoxLabel;

    ScopedPointer<Slider> Trigger_threshold; //TODO
    ScopedPointer<ToggleButton> trackSpike_button;
    ScopedPointer<Label> trackSpike_button_Label;

    //ScopedPointer<ComboBox> trackSpikeComboBox;
    ScopedPointer<TableListBox> spikeTracker;
    ScopedPointer<TableListBox> thresholdTracker;
    ScopedPointer<TableListBoxModel> thresholdTrackerContent;
    
    ScopedPointer<TextEditor> locations[4];
    ScopedPointer<TextEditor> fps[4];
    ScopedPointer<ToggleButton> follows[4];
    ScopedPointer<TextButton> dels[4];
    ScopedPointer<TextEditor> ts[4];
    ScopedPointer<ToggleButton> thresholds[4];
    ScopedPointer<TextButton> tdels[4];

    ScopedPointer<ToggleButton> trackThreshold_button;
    ScopedPointer<Label> trackThreshold_button_Label;

    //DEBUG

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LfpLatencyProcessorVisualizerContentComponent)
};

#endif // __JUCE_HEADER_D83F67960ECDED8C__
