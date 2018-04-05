#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // INIT OSC SENDER
    sender1.setup("192.168.5.96", 10000);
    sender2.setup("192.168.5.97", 10000);
    sender3.setup("192.168.5.98", 10000);
    
    // INIT OSC BROADCAST SENDER
    //sender.setup("192.168.5.255", 30000);
    
    // INIT OSC RECEIVER
    receiver.setup(20000);
    
    ofSetFrameRate(60);
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = ofGetWidth()-xInit;
    
    //ofBackground(200, 200, 200);
    //ofBackground(220);
    
    // MAIN GUI
    gui = new ofxUICanvas(0,0,ofGetWidth(),ofGetHeight());
    //gui->setColorFill(ofxUIColor::white);
    
    //ofBackground(10, 0, 180);
    
    //gui->setTheme(OFX_UI_THEME_COOLCLAY);
    //gui->setTheme(OFX_UI_THEME_GRAYDAY);
    gui->setColorFill(ofxUIColor::whiteSmoke);
    
    gui->addWidgetDown(new ofxUILabel("REMOTE 360 PETITE SALLE", OFX_UI_FONT_LARGE));
    gui->addSpacer();
    gui->addSpacer();
    gui->addWidgetDown(new ofxUILabel("PROJECTORS", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUILabelButton("POWER ON", false, length-xInit));
    gui->addWidgetDown(new ofxUILabelButton("POWER OFF", false, length-xInit));
    //gui->addSpacer(ofGetWidth(), 20);
    
    gui->addWidgetDown(new ofxUILabelButton("MUTE", false, length-xInit));
    gui->addWidgetDown(new ofxUILabelButton("UNMUTE", false, length-xInit));
    
    gui->addSpacer();
    gui->addWidgetDown(new ofxUILabel("VIDEOS", OFX_UI_FONT_MEDIUM));
    
    gui->addWidgetDown(new ofxUILabelButton("0", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("1", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("2", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("3", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("4", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("5", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("6", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("7", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("8", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("9", false, 30, 30));
    gui->addWidgetRight(new ofxUILabelButton("10", false, 30, 30));
    
    gui->addWidgetDown(new ofxUILabelButton("PLAY", false, length-xInit));
    gui->addWidgetDown(new ofxUILabelButton("PAUSE", false, length-xInit));
    gui->addWidgetDown(new ofxUILabelButton("REWIND", false, length-xInit));
    gui->addSlider("TIMELINE", 0.0, 1.0, &slider_position, length-xInit, 32);
    //    gui->addSpacer();
    //    gui->addLabel("'i' : afficher les informations de configuration");
    
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    
    // HELP GUI
    helpgui = new ofxUICanvas(0,0,ofGetWidth(),ofGetHeight());
    //helpgui->setTheme(OFX_UI_THEME_GRAYDAY);
    helpgui->setVisible(false);
    helpgui->addWidgetDown(new ofxUILabel("INFORMATIONS", OFX_UI_FONT_LARGE));
    helpgui->addSpacer(ofGetWidth() - 5, 5);
    helpgui->addLabel("// OSC SENDERS //");
    helpgui->addLabel("PLAY : /master/play 1");
    helpgui->addLabel("PAUSE : /master/play 0");
    helpgui->addLabel("FULLSCREEN : /master/fullscreen 1");
    helpgui->addLabel("LOAD VIDEOS : /master/load_videos 1");
    helpgui->addLabel("TIMELINE : /master/slider_position (float)");
    helpgui->addLabel("CLIP : /master/clip");
    helpgui->addLabel("Default OSC sending port : 10000");
    helpgui->addSpacer();
    helpgui->addLabel("// OSC RECEIVERS //");
    helpgui->addLabel("VP ON/OFF : /master/power (int)");
    helpgui->addLabel("TIMELINE : /master/feedback (float)");
    helpgui->addSpacer();
    helpgui->addLabel("Broadcast OSC sender 192.168.5.255:30000");
    helpgui->addLabel("Default OSC receive port : 20000");
}

//--------------------------------------------------------------
void ofApp::update(){
    ofxOscMessage msgRx;
    receiver.getNextMessage( msgRx );
    
    if ( msgRx.getAddress() == "/master/power" )
    {
        int power = msgRx.getArgAsInt(0);
        if (power == 1) {
            powerOn();
        }
        else {
            powerOff();
        }
    }
   
    
    if ( msgRx.getAddress() == "/master/feedback" )
    {
        slider_position = msgRx.getArgAsFloat(0);
    }
    
    if(slider_position == 1) {
        ofxOscMessage m;
        m.setAddress("/master/slider_position");
        m.addFloatArg(0);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

////////////////////// OFXUI FUNCTIONS /////////////////////////
void ofApp::exit()
{
    delete gui;
}

void ofApp::sendCommand(string ipAddress, int pjPort, string command)
{
    string msgRx;
    bool connected;
    
    connected = pjClient.setup(ipAddress, pjPort);
    if(!pjClient.isConnected())
    {
        cout << "Problem connecting..." << endl;
    }
    
    else {
        pjClient.sendRaw(command);
        //		ofSleepMillis(500);
        //		msgRx = "";
        //		while (msgRx.length() < 8)
        //		{
        //			msgRx = pjClient.receiveRaw();
        //		}
    }
    pjClient.close();
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "PLAY")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            ofxOscMessage m;
            m.setAddress("/master/play");
            m.addIntArg(1);
            
            // Send broadcast message...
            //sender.sendMessage(m);
            
            // ...or to scpecified IP
            sender1.sendMessage(m);
            sender2.sendMessage(m);
            sender3.sendMessage(m);
        }
    }
    else if(e.widget->getName() == "PAUSE")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            ofxOscMessage m;
            m.setAddress("/master/play");
            m.addIntArg(0);
            
            // Send broadcast message...
            //sender.sendMessage(m);
            
            // ...or to scpecified IP
            sender1.sendMessage(m);
            sender2.sendMessage(m);
            sender3.sendMessage(m);
        }
    }
    else if(e.widget->getName() == "0")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(0);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "1")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(1);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "2")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(2);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "3")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(3);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "4")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(4);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "5")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(5);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "6")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(6);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "7")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(7);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "8")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(8);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "9")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(9);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "10")
    {
        ofxOscMessage m;
        m.setAddress("/master/clip");
        m.addIntArg(10);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    
    else if(e.widget->getName() == "PLEIN ECRAN")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            ofxOscMessage m;
            m.setAddress("/master/fullscreen");
            m.addIntArg(1);
            
            // Send broadcast message...
            //sender.sendMessage(m);
            
            // ...or to scpecified IP
            sender1.sendMessage(m);
            sender2.sendMessage(m);
            sender3.sendMessage(m);
        }
    }
    else if(e.widget->getName() == "REWIND")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            slider_position = 0;
            ofxOscMessage m;
            m.setAddress("/master/slider_position");
            m.addFloatArg(0);
            
            // Send broadcast message...
            //sender.sendMessage(m);
            
            // ...or to scpecified IP
            sender1.sendMessage(m);
            sender2.sendMessage(m);
            sender3.sendMessage(m);
        }
    }
    else if(e.widget->getName() == "TIMELINE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        ofxOscMessage m;
        m.setAddress("/master/slider_position");
        slider_position = slider->getScaledValue();
        m.addFloatArg(slider_position);
        
        // Send broadcast message...
        //sender.sendMessage(m);
        
        // ...or to scpecified IP
        sender1.sendMessage(m);
        sender2.sendMessage(m);
        sender3.sendMessage(m);
    }
    else if(e.widget->getName() == "POWER ON")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == 1) {
            powerOn();
        }
        
    }
    else if(e.widget->getName() == "POWER OFF")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == 0) {
            powerOff();
        }
    }
    else if(e.widget->getName() == "MUTE")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            sendCommand("192.168.5.121", 1025, ":SHUT 1\r");
            sendCommand("192.168.5.122", 1025, ":SHUT 1\r");
            sendCommand("192.168.5.123", 1025, ":SHUT 1\r");
            sendCommand("192.168.5.124", 1025, ":SHUT 1\r");
            sendCommand("192.168.5.125", 1025, ":SHUT 1\r");
            sendCommand("192.168.5.126", 1025, ":SHUT 1\r");
        }
    }
    else if(e.widget->getName() == "UNMUTE")
    {
        ofxUILabelButton *btn = (ofxUILabelButton *) e.widget;
        if (btn->getValue() == true) {
            sendCommand("192.168.5.121", 1025, ":SHUT 0\r");
            sendCommand("192.168.5.122", 1025, ":SHUT 0\r");
            sendCommand("192.168.5.123", 1025, ":SHUT 0\r");
            sendCommand("192.168.5.124", 1025, ":SHUT 0\r");
            sendCommand("192.168.5.125", 1025, ":SHUT 0\r");
            sendCommand("192.168.5.126", 1025, ":SHUT 0\r");
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::powerOn(){
    sendCommand("192.168.5.121", 1025, ":POWR 1\r");
    sendCommand("192.168.5.122", 1025, ":POWR 1\r");
    sendCommand("192.168.5.123", 1025, ":POWR 1\r");
    sendCommand("192.168.5.124", 1025, ":POWR 1\r");
    sendCommand("192.168.5.125", 1025, ":POWR 1\r");
    sendCommand("192.168.5.126", 1025, ":POWR 1\r");
}

//--------------------------------------------------------------
void ofApp::powerOff(){
    sendCommand("192.168.5.121", 1025, ":POWR 0\r");
    sendCommand("192.168.5.122", 1025, ":POWR 0\r");
    sendCommand("192.168.5.123", 1025, ":POWR 0\r");
    sendCommand("192.168.5.124", 1025, ":POWR 0\r");
    sendCommand("192.168.5.125", 1025, ":POWR 0\r");
    sendCommand("192.168.5.126", 1025, ":POWR 0\r");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'i')
    {
        gui->toggleVisible();
        helpgui->toggleVisible();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
