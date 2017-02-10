#include "ofxFFmpegRecorder.h"
// openFrameworks
#include "ofLog.h"
#include "ofVideoGrabber.h"
#include "ofSoundStream.h"

ofxFFmpegRecorder::ofxFFmpegRecorder()
    : m_FFmpegPath("ffmpeg")
    , m_OutputPath("")
    , m_IsRecordVideo(false)
    , m_IsRecordAudio(false)
    , m_IsOverWrite(false)
    , m_Process()
    , m_CaptureDuration(0)
    , m_DefaultVideoDevice()
    , m_DefaultAudioDevice()
{

}

void ofxFFmpegRecorder::setup(bool recordVideo, bool recordAudio, const std::string &ffmpegPath)
{
    m_IsRecordVideo = recordVideo;
    m_IsRecordAudio = recordAudio;
    if (ffmpegPath.length() > 0) {
        m_FFmpegPath = ffmpegPath;
    }
}

bool ofxFFmpegRecorder::isRecordVideo() const
{
    return m_IsRecordVideo;
}

void ofxFFmpegRecorder::setRecordVideo(bool record)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_IsRecordVideo = record;
}

bool ofxFFmpegRecorder::isRecordAudio() const
{
    return m_IsRecordAudio;
}

void ofxFFmpegRecorder::setRecordAudio(bool record)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_IsRecordAudio = record;
}

std::string ofxFFmpegRecorder::getFFmpegPath() const
{
    return m_FFmpegPath;
}

void ofxFFmpegRecorder::setFFmpegPath(const std::string &path)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_FFmpegPath = path;
}

float ofxFFmpegRecorder::getCaptureDuration() const
{
    return m_CaptureDuration;
}

void ofxFFmpegRecorder::setCaptureDuration(float duration)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_CaptureDuration = duration;
}

ofVideoDevice ofxFFmpegRecorder::getDefaultVideoDevice() const
{
    return m_DefaultVideoDevice;
}

void ofxFFmpegRecorder::setDefaultVideoDevice(const ofVideoDevice &device)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_DefaultVideoDevice = device;
}

ofSoundDevice ofxFFmpegRecorder::getDefaultAudioDevice() const
{
    return m_DefaultAudioDevice;
}

void ofxFFmpegRecorder::setDefaultAudioDevice(const ofSoundDevice &device)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_DefaultAudioDevice = device;
}

std::string ofxFFmpegRecorder::getOutputPath() const
{
    return m_OutputPath;
}

void ofxFFmpegRecorder::setOutputPath(const std::string &path)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_OutputPath = path;
}

bool ofxFFmpegRecorder::record(float duration)
{
    if (m_Process.isRunning()) {
        ofLogWarning("ofxFFmpegRecorder") << "A recording is already in proggress.";
        return false;
    }

    if (m_OutputPath.length() == 0) {
        ofLogWarning("ofxFFmpegRecorder") << "Output path is empty. Cannot record.";
        return false;
    }

    if (ofFile::doesFileExist(m_OutputPath, false) && m_IsOverWrite == false) {
        ofLogError("ofxFFmpegRecorder") << "The output file already exists and overwriting is disabled. Cannot capture video.";
        return false;
    }

    determineDefaultDevices();
    m_CaptureDuration = duration;

    std::vector<std::string> args;

#if defined(_WIN32)
    args.push_back("-f dshow");
#elif defined(__APPLE__)
    args.push_back("-f avfoundation");
#else
    args.push_back("-f v4l2");
#endif // PLATFORM_CHECK

    if (m_CaptureDuration > 0.f) {
        args.push_back("-t " + std::to_string(m_CaptureDuration));
    }

    std::string inputDevices;
    if (m_IsRecordVideo) {
        inputDevices += "video=\"" + m_DefaultVideoDevice.deviceName + "\"";
    }

    if (m_IsRecordAudio) {
        if (inputDevices.length() > 0) {
            inputDevices += ":";
        }

        inputDevices += "audio=\"" + m_DefaultAudioDevice.name + "\"";
    }

    args.push_back("-i " + inputDevices);

    std::copy(m_AdditionalArguments.begin(), m_AdditionalArguments.end(), std::back_inserter(args));
    args.push_back(m_OutputPath);

    m_Process.setup(m_FFmpegPath, "", args);
    m_Process.launchDetached();

    return true;
}

void ofxFFmpegRecorder::stop()
{
    m_Process.write("q");
}

void ofxFFmpegRecorder::cancel()
{
    m_Process.write("q");
    ofFile::removeFile(m_OutputPath, false);
}

bool ofxFFmpegRecorder::isOverWrite() const
{
    return m_IsOverWrite;
}

void ofxFFmpegRecorder::setOverWrite(bool overwrite)
{
    m_IsOverWrite = overwrite;
}

std::vector<std::string> ofxFFmpegRecorder::getAdditionalArguments() const
{
    return m_AdditionalArguments;
}

void ofxFFmpegRecorder::setAdditionalArguments(const std::vector<std::string> &args)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_AdditionalArguments = args;
}

void ofxFFmpegRecorder::addAdditionalArgument(const string &arg)
{
    if (m_Process.isRunning()) {
        ofLogVerbose("ofxFFmpegRecorder") << "A recording is in proggress. The change will take effect for the next recording session.";
    }

    m_AdditionalArguments.push_back(arg);
}

bool ofxFFmpegRecorder::isRecording() const
{
    return m_Process.isRunning();
}

void ofxFFmpegRecorder::saveThumbnail(const unsigned int &hour, const unsigned int &minute, const float &second, const string &output, ofVec2f size,
                                      ofRectangle crop, string videoFilePath)
{
    if (videoFilePath.length() == 0) {
        if (m_Process.isRunning()) {
            ofLogError("ofxFFmpegRecorder") << "Cannot use the default video file because a recording is already in proggress.";
            return;
        }

        videoFilePath = m_OutputPath;
    }

    const std::string time = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
    std::vector<std::string> args;

    args.push_back("-i " + videoFilePath);
    args.push_back("-ss " + time);
    args.push_back("-vframes 1");

    std::string vfString = "\"";
    if (size.x > 0 && size.y > 0) {
        vfString += "scale=" + std::to_string(size.x) + ":" + std::to_string(size.y);
    }

    if (crop.isZero() == false) {
        // Decide if a comma is required.
        if (vfString.length() > 1) {
            vfString += ",";
        }

        vfString += "crop=" + std::to_string(crop.getWidth()) + ":" + std::to_string(crop.getHeight());
        if (crop.getX() > 0 && crop.getY() > 0) {
            vfString += ":" + std::to_string(crop.getX()) + ":" + std::to_string(crop.getY());
        }
    }

    vfString += "\"";

    if (vfString.length() > 2) {
        args.push_back("-vf " + vfString);
    }

    args.push_back(output);

    ofxProcess process;
    process.setup(m_FFmpegPath, "", args);
    process.launchDetached();
}

void ofxFFmpegRecorder::determineDefaultDevices()
{
    if (m_IsRecordVideo && m_DefaultVideoDevice.deviceName.length() == 0) {
        ofVideoGrabber videoGrabber;
        const std::vector<ofVideoDevice> devices = videoGrabber.listDevices();

        for (const ofVideoDevice &device : devices) {
            if (device.bAvailable) {
                m_DefaultVideoDevice = device;
                break;
            }
        }
    }

    if (m_IsRecordAudio && m_DefaultAudioDevice.isDefaultInput == false) {
        ofSoundStream soundStream;
        const std::vector<ofSoundDevice> audoDevices = soundStream.listDevices();

        for (const ofSoundDevice &device : audoDevices) {
            if (device.isDefaultInput) {
                m_DefaultAudioDevice = device;
                break;
            }
        }
    }
}
