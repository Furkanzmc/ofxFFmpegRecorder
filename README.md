# ofxFFmpegRecorder

A small class to utilize the video recording functionality of `ffmpeg`.

# Features

- Record video
- Record audio
- Save thumbnail from a video

# How to Use

In `ofApp.h`

```c++
private:
    ofxFFmpegRecorder m_Recorder;
```

In `ofApp.cpp`

```c++
void ofApp::setup()
{
    m_Recorder.setup(true, true);
    m_Recorder.setOutputPath("E:/Users/furkanzmc/Desktop/vid.mp4");
}

void ofApp::keyReleased(int key)
{
    if (key == 'r') {
        m_Recorder.record(5);
    }
    else if (key == 's') {
        m_Recorder.stop();
    }
    else if (key == 't') {
        m_Recorder.saveThumbnail(0, 0, 2, "E:/Users/furkanzmc/Desktop/thumbnail.png", ofVec2f(0, 0), ofRectangle(0, 0, 500, 400));
    }
}
```

# Dependencies

ofxFFmpegRecorder depends only on [ofxProcess](https://github.com/Furkanzmc/ofxProcess).
