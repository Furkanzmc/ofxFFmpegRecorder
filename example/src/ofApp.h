#pragma once
#include "ofMain.h"
#include "ofxFFmpegRecorder.h"
#include "ofVideoGrabber.h"

class ofApp : public ofBaseApp{ 
public:

    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);

    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void audioIn(ofSoundBuffer & input);

private:
    ofxFFmpegRecorder m_Recorder;
    ofVideoGrabber m_Grabber;

    ofSoundStream soundStream;
    short* shortBuffer;
    ofPolyline          waveform;
    size_t  lastAudioTimeReset;
    int bufferCounter;
    float audioFPS;
    int audioCounter;
    
    ofFbo mCapFbo;
    ofPixels mPix;

    bool     isRecordingVideo;
    bool     isRecordingAudio;

};
