#include "ofApp.h"
// openFrameworks
#include "ofMain.h"

ofApp::ofApp()
{
}

void ofApp::setup()
{
    m_Grabber.setup(640, 480);

    m_Recorder.setup(true, false, ofVec2f(640, 480));
    m_Recorder.setOverWrite(true);
    m_Recorder.setOutputPath("E:/Users/furkanzmc/Desktop/vid.avi");

    /**
     * You can also use the following methods to crop the output file
     *     m_Recorder.addAdditionalOutputArgument("-vf \"crop=300:300:0:0\"");
     * Or you can change the output codec
     *     m_Recorder.setVideCodec("libx264");
     **/
}

void ofApp::update()
{
    m_Grabber.update();
    if (m_Recorder.isRecordingCustom() && m_Recorder.isPaused() == false) {
        m_Recorder.addFrame(m_Grabber.getPixels());
    }
}

void ofApp::draw()
{
    m_Grabber.draw(0, 0);
    ofDrawBitmapStringHighlight(std::to_string(m_Recorder.getRecordedDuration()), 40, 45);
    ofDrawBitmapStringHighlight("FPS: " + std::to_string(ofGetFrameRate()), 0, 10);

    ofPushStyle();
    {
        if (m_Recorder.isPaused() && m_Recorder.isRecording()) {
            ofSetColor(ofColor::yellow);
        }
        else if (m_Recorder.isRecording()) {
            ofSetColor(ofColor::red);
        }
        else {
            ofSetColor(ofColor::green);
        }
        ofDrawCircle(ofPoint(10, 40), 10);

        // Draw the information
        ofDrawBitmapStringHighlight("Press (r) to start capturing the default device for 5 seconds."
                                    "\nPress (t) to save thumbnail."
                                    "\nPress and hold mouse left button to record custom video."
                                    "\nRelease mouse left button to pause recording.",
                                    0, 80);
    }
    ofPopStyle();
}

void ofApp::keyPressed(int key)
{

}

void ofApp::keyReleased(int key)
{
    if (key == 'r') {
        // Stop the grabber so ffmpeg can use the default device
        m_Grabber.close();
        m_Recorder.record(5);
    }
    else if (key == 's') {
        m_Recorder.stop();
    }
    else if (key == 'p') {
        m_Recorder.setPaused(!m_Recorder.isPaused());
    }
    else if (key == 't') {
        m_Recorder.saveThumbnail(0, 0, 2, "thumbnail.png", ofVec2f(0, 0), ofRectangle(0, 0, 500, 400));
    }
}

void ofApp::mouseMoved(int x, int y)
{

}

void ofApp::mouseDragged(int x, int y, int button)
{

}

void ofApp::mousePressed(int x, int y, int button)
{
    if (button == OF_MOUSE_BUTTON_LEFT && m_Recorder.isRecordingDefault() == false) {
        if (m_Grabber.isInitialized() == false) {
            m_Grabber.setup(640, 480);
        }

        if (m_Recorder.isRecording() == false) {
            m_Recorder.startCustomRecord();
        }
        else {
            m_Recorder.setPaused(false);
        }
    }
}

void ofApp::mouseReleased(int x, int y, int button)
{
    if (button == OF_MOUSE_BUTTON_LEFT && m_Recorder.isRecordingDefault() == false) {
        m_Recorder.setPaused(true);
    }
}

void ofApp::mouseEntered(int x, int y)
{

}

void ofApp::mouseExited(int x, int y)
{

}

void ofApp::windowResized(int w, int h)
{

}

void ofApp::gotMessage(ofMessage msg)
{

}

void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
