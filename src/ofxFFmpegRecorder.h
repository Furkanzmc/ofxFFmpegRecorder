#pragma once
// Addons
#include "ofxProcess.h"
// openFrameworks
#include "ofTypes.h"
#include "ofBaseSoundStream.h"
#include "ofRectangle.h"
#include "ofPixels.h"

using HighResClock = std::chrono::time_point<std::chrono::high_resolution_clock>;

class ofxFFmpegRecorder
{
public:
    ofxFFmpegRecorder();
    ~ofxFFmpegRecorder();

    /**
     * @brief Setup the class
     * @param recordVideo
     * @param recordAudio This is not yet supported with custom recording
     * @param ffmpegPath This variable is optional. If left empty, the default "ffmpeg" is used. This required that the "ffmpeg" is in the system's path.
     */
    void setup(bool recordVideo, bool recordAudio, ofVec2f videoSize = ofVec2f::zero(), float fps = 30.f, unsigned int bitrate = 2000,
               const std::string &ffmpegPath = "");

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

    float getFps() const;
    void setFps(float fps);

    unsigned int getBitRate() const;
    void setBitRate(unsigned int rate);

    std::string getVideCodec() const;
    void setVideCodec(const std::string &codec);

    /**
     * @brief Returns the record duration for the custom recording. This will return 0 for the webcam recording.
     * @return
     */
    float getRecordedDuration() const;

    /**
     * @brief Starts recording a video from a default device that is determined by @ref determineDefaultDevices()
     * @param duration This is optional. Duration is in seconds.
     * @return If the class was already recording a video this method returns false, otherwise it returns true;
     */
    bool record(float duration = 0);

    /**
     * @brief Setup ffmpeg for a custom video recording. Input is taken from the stdin as raw image. This also inherits the
     * m_AdditionalArguments.
     * @return If the class was already recording a video this method returns false, otherwise it returns true;
     */
    bool startCustomRecord();

    /**
     * @brief Add a frame to the stream. This can onle be used If you started recording a custom video. Make sure that the frames are added continuously.
     * @param pixels
     * @return
     */
    size_t addFrame(const ofPixels &pixels);

    void stop();

    /**
     * @brief Stops the recording and deletes the file.
     */
    void cancel();

    bool isOverWrite() const;
    void setOverWrite(bool overwrite);

    const std::vector<std::string> &getAdditionalInputArguments() const;

    /**
     * @brief This method overwrites the existing additional arguments.
     * @param args
     */
    void setAdditionalInputArguments(const std::vector<std::string> &args);

    /**
     * @brief Add a single additional argument. This appends to the existing additional arguments.
     * **Example Usage**
     * @code
     *     recorder.addAdditionalInputArgument("-y");
     *     recorder.addAdditionalInputArgument("-map_channel 0.0.1");
     * @endcode
     * @param arg
     */
    void addAdditionalInputArgument(const std::string &arg);

    void clearAdditionalInputArguments();

    const std::vector<std::string> &getAdditionalOutputArguments() const;

    /**
     * @brief This method overwrites the existing additional arguments.
     * @param args
     */
    void setAdditionalOutputArguments(const std::vector<std::string> &args);

    /**
     * @brief Add a single additional argument. This appends to the existing additional arguments.
     * **Example Usage**
     * @code
     *     recorder.addAdditionalOutputArgument("-y");
     *     recorder.addAdditionalOutputArgument("-map_channel 0.0.1");
     * @endcode
     * @param arg
     */
    void addAdditionalOutputArgument(const std::string &arg);

    void clearAdditionalOutputArguments();

    /**
     * @brief Clears both input and output parameters
     */
    void clearAdditionalArguments();

    /**
     * @brief Returns true If m_Process is running or m_File is not nullptr.
     * @return
     */
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

    ofVec2f m_VideoSize;
    float m_Fps;
    unsigned int m_BitRate, m_AddedVideFrames;

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

    std::string m_VideCodec;

    /**
     * @brief This is used to make sure that we put in frames no more than the m_Fps
     */
    HighResClock m_FrameStartTime;

    /**
     * @brief Additional arguments can be used to extend the functionality of ofxFFmpegRecorder. Additional arguments are used
     * in both webcam recording and custom reciording. Thumbnail saving is not affected by these.
     */
    std::vector<std::string> m_AdditionalInputArguments, m_AdditionalOutputArguments;

    FILE *m_File;

private:
    /**
     * @brief Checks if the current default devices are still available. If they are not, gets the first available device for both audio and video.
     */
    void determineDefaultDevices();
};
