#pragma once

#include "ofTypes.h"

#include "ofVideoBaseTypes.h"
#include "ofSoundBaseTypes.h"
#include "ofSoundBuffer.h"
#include "ofRectangle.h"
#include "ofPixels.h"

#include <thread>

using HighResClock = std::chrono::time_point<std::chrono::high_resolution_clock>;

/**
 * LockFreeQueue is taken from here: https://github.com/timscaffidi/ofxVideoRecorder/blob/master/src/ofxVideoRecorder.h#L9
 */
template <typename T>
struct LockFreeQueue {

    LockFreeQueue()
    {
        // Add one to validate the iterators
        m_List.push_back(T());
        m_HeadIt = m_List.begin();
        m_TailIt = m_List.end();
    }

    void produce(const T &t)
    {
        m_List.push_back(t);
        m_TailIt = m_List.end();
        m_List.erase(m_List.begin(), m_HeadIt);
    }

    bool consume(T &t)
    {
        typename TList::iterator nextIt = m_HeadIt;
        ++nextIt;
        if (nextIt != m_TailIt) {
            m_HeadIt = nextIt;
            t = *m_HeadIt;
            return true;
        }

        return false;
    }

    int size() const
    {
        return std::distance(m_HeadIt, m_TailIt) - 1;
    }

    typename std::list<T>::iterator getHead() const
    {
        return m_HeadIt;
    }

    typename std::list<T>::iterator getTail() const
    {
        return m_TailIt;
    }

private:
    using TList = std::list<T>;
    TList m_List;
    typename TList::iterator m_HeadIt, m_TailIt;
};

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
    
    
    void setup(bool recordVideo, bool recordAudio, glm::vec2 videoSize = glm::vec2(0,0), float fps = 30.f, unsigned int bitrate = 2000,
               const std::string &ffmpegPath = "");

    bool isRecordVideo() const;
    void setRecordVideo(bool record);

    bool isRecordAudio() const;
    void setRecordAudio(bool record);

    std::string getFFmpegPath() const;
    void setFFmpegPath(const std::string &path);
    void setFFmpegPathToAddonsPath();

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

    std::string getVideoCodec() const;
    void setVideoCodec(const std::string &codec);

    void setAudioConfig(int bufferSize, int sampleRate);

	float getWidth();
	void setWidth(float aw);
	float getHeight();
	void setHeight(float ah);

    bool isPaused() const;
    void setPaused(bool paused);

	void setPixelFormat(ofImageType aType);

    /**
     * @brief Returns the record duration for the custom recording. This will return 0 for the webcam recording.
     * @return
     */
    float getRecordedDuration() const;

    /**
     * @brief Returns the record duration for the custom audio recording.
     * @return
     */
    float getRecordedAudioDuration(float afps) const;

    /**
     * @brief Starts recording a video from a default device that is determined by @ref determineDefaultDevices()
     * @param duration This is optional. Duration is in seconds.
     * @return If the class was already recording a video this method returns false, otherwise it returns true;
     */
    bool record(float duration = 0);

    /**
     * @brief Setup ffmpeg for a custom video recording. Input is taken from the stdin as raw image. This also inherits the
     * m_AdditionalArguments.
     * @return If the class was already recording a video/audio this method returns false, otherwise it returns true;
     */
    bool startCustomRecord();

    /**
     * @brief Setup ffmpeg for a custom audio recording. This also inherits the
     * m_AdditionalArguments.
     * @return If the class was already recording a video/audio this method returns false, otherwise it returns true;
     */
    bool startCustomAudioRecord();

    /**
     * @brief Setup ffmpeg for a custom video streaming. Input is taken from the stdin as raw image. This also inherits the
     * m_AdditionalArguments.
     * @return If the class was already recording a video/audio this method returns false, otherwise it returns true;
     */
    bool startCustomStreaming();

    /**
     * @brief Add a frame to the stream. This can onle be used If you started recording a custom video. Make sure that the frames are added continuously.
     * @param pixels
     * @return
     */
    size_t addFrame(const ofPixels &pixels);

    /**
     * @brief Add a sound buffer to the stream. This can onle be used If you started recording a custom audio. Make sure that the buffers are added continuously inside the audioIn thread.
     * @param pixels
     * @return
     */
    size_t addBuffer(const ofSoundBuffer &buffer, float afps);

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
     * @brief Returns true if any kind of recording is in proggress.
     * @return
     */
    bool isRecording() const;

    bool isRecordingCustom() const;
    bool isRecordingDefault() const;

    /**
     * @brief Saves a thumbnail from the video at the given time. If videoFilePath is empty, then the m_OutputPath is used as the video source.
     * But m_OutputPath cannot be used as video source if the video recording is in process If a default recording was done before.
     * @param hour
     * @param minute
     * @param second
     * @param output
     * @param size
     * @param videoFilePath
     */
    void saveThumbnail(const unsigned int &hour, const unsigned int &minute, const float &second, const std::string &output, glm::vec2 size = glm::vec2(0, 0),
                       ofRectangle crop = ofRectangle(0, 0, 0, 0), std::string videoFilePath = "");

private:
    std::string m_FFmpegPath, m_OutputPath;
    bool m_IsRecordVideo, m_IsRecordAudio;

    /**
     * @brief If this is true, the video capture will overwrite the file if it already exists. The default value is false.
     */
    bool m_IsOverWrite;

    /**
     * @brief Pausing only works for custom recording.
     */
    bool m_IsPaused;

    glm::vec2 m_VideoSize;
    unsigned int m_BitRate, m_AddedVideoFrames, m_AddedAudioFrames;

    float m_Fps,
          m_CaptureDuration,
          m_TotalPauseDuration;

    int m_bufferSize;
    int m_sampleRate;

    /**
     * @brief If the default video device is not set, the default one is automatically set by this class
     */
    ofVideoDevice m_DefaultVideoDevice;

    /**
     * @brief If the default audio device is not set, the default one is automatically set by this class
     */
    ofSoundDevice m_DefaultAudioDevice;

    std::string m_VideCodec;
    std::string m_AudioCodec;
    FILE *m_CustomRecordingFile, *m_DefaultRecordingFile;

    /**
     * @brief This is used to make sure that we put in frames no more than the m_Fps. This is used in custom recording.
     */
    HighResClock m_RecordStartTime;

    HighResClock m_PauseStartTime, m_PauseEndTime;

    /**
     * @brief Additional arguments can be used to extend the functionality of ofxFFmpegRecorder. Additional arguments are used
     * in both webcam recording and custom reciording. Thumbnail saving is not affected by these.
     */
    std::vector<std::string> m_AdditionalInputArguments, m_AdditionalOutputArguments;

    std::thread m_Thread;
    LockFreeQueue<ofPixels *> m_Frames;
    LockFreeQueue<ofSoundBuffer *> m_Buffers;

    std::string mPixFmt = "rgb24";

private:
    /**
     * @brief Checks if the current default devices are still available. If they are not, gets the first available device for both audio and video.
     */
    void determineDefaultDevices();

    /**
     * @brief Runs in parallele and writes the stored frames/buffers to ffmpeg
     */
    void processFrame();
    void processBuffer();
    void joinThread();

};
