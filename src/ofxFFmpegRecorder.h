#pragma once
// Addons
#include "ofxProcess.h"
// openFrameworks
#include "ofTypes.h"
#include "ofBaseSoundStream.h"
#include "ofRectangle.h"

class ofxFFmpegRecorder
{
public:
    ofxFFmpegRecorder();

    /**
     * @brief Setup the class
     * @param recordVideo
     * @param recordAudio
     * @param ffmpegPath This variable is optional. If left empty, the default "ffmpeg" is used. This required that the "ffmpeg" is in the system's path.
     */
    void setup(bool recordVideo, bool recordAudio, const std::string &ffmpegPath = "");

    bool isRecordVideo() const;
    void setRecordVideo(bool record);

    bool isRecordAudio() const;
    void setRecordAudio(bool record);

    std::string getFFmpegPath() const;
    void setFFmpegPath(const std::string &path);

    float getCaptureDuration() const;
    void setCaptureDuration(float duration);

    ofVideoDevice getDefaultVideoDevice() const;
    void setDefaultVideoDevice(const ofVideoDevice &device);

    ofSoundDevice getDefaultAudioDevice() const;
    void setDefaultAudioDevice(const ofSoundDevice &device);

    std::string getOutputPath() const;
    void setOutputPath(const std::string &path);

    /**
     * @brief Starts recording a video.
     * @param duration This is optional. Duration is in seconds.
     * @return If the class was already recording a video this method returns false, otherwise it returns true;
     */
    bool record(float duration = 0);
    void stop();

    /**
     * @brief Stops the recording and deletes the file.
     */
    void cancel();

    bool isOverWrite() const;
    void setOverWrite(bool overwrite);

    std::vector<std::string> getAdditionalArguments() const;

    /**
     * @brief This method overwrites the existing additional arguments.
     * @param args
     */
    void setAdditionalArguments(const std::vector<std::string> &args);

    /**
     * @brief Add a single additional argument. This appends to the existing additional arguments.
     * **Example Usage**
     * @code
     *     recorder.addAdditionalArgument("-y");
     *     recorder.addAdditionalArgument("-map_channel 0.0.1");
     * @endcode
     * @param arg
     */
    void addAdditionalArgument(const std::string &arg);

    bool isRecording() const;

    /**
     * @brief Saves a thumbnail from the video at the given time. If videoFilePath is empty, then the m_OutputPath is used as the video source.
     * But m_OutputPath cannot be used as video source if the video recording is in process.
     * @param hour
     * @param minute
     * @param second
     * @param output
     * @param size
     * @param videoFilePath
     */
    void saveThumbnail(const unsigned int &hour, const unsigned int &minute, const float &second, const std::string &output, ofVec2f size = ofVec2f(0, 0),
                       ofRectangle crop = ofRectangle(0, 0, 0, 0), std::string videoFilePath = "");

private:
    std::string m_FFmpegPath, m_OutputPath;
    bool m_IsRecordVideo, m_IsRecordAudio;

    /**
     * @brief If this is true, the video capture will overwrite the file if it already exists. The default value is false.
     */
    bool m_IsOverWrite;

    ofxProcess m_Process;
    float m_CaptureDuration;

    /**
     * @brief If the default video device is not set, the default one is automatically set by this class
     */
    ofVideoDevice m_DefaultVideoDevice;

    /**
     * @brief If the default audio device is not set, the default one is automatically set by this class
     */
    ofSoundDevice m_DefaultAudioDevice;

    /**
     * @brief Additional arguments can be used to extend the functionality of ofxFFmpegRecorder.
     */
    std::vector<std::string> m_AdditionalArguments;

private:
    /**
     * @brief Checks if the current default devices are still available. If they are not, gets the first available device for both audio and video.
     */
    void determineDefaultDevices();
};
