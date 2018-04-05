#pragma once

#define OFX_UI_NO_XML

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxNetwork.h"

// OSC LIBRARY
#include "ofxOsc.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void sendCommand(string ipAddress, int pjPort, string command);
    void powerOn();
    void powerOff();
    void exit();
    
    float slider_position;
    float feedback_pos;
    
    // CREATE GUI
    ofxUICanvas *gui;
    ofxUICanvas *helpgui;
    void guiEvent(ofxUIEventArgs &e);
    
    // CREATE OSC SENDER
    ofxOscSender sender1;
    ofxOscSender sender2;
    ofxOscSender sender3;
    
    // CREATE OSC BROADCAST SENDER
    ofxOscSender sender;
    
    // CREATE OSC SENDER
    ofxOscReceiver receiver;
    
    ofxTCPClient pjClient;
    
};
