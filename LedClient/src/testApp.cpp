#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	led = NULL;
	ofxXmlSettings xml;
	int port = 2838;

	receiver.setup(port);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0);
    ofSetFrameRate(60);
	
	if(	spi.connect())
	{
		ofLogNotice()<<"connected to SPI";
	}
    
	lastFrameTime = 0;
	numLED = 240;
    
	led = new ofxLEDsLPD8806(numLED);
    
    ledData.assign(numLED,ofColor());
	startThread(false,false);
    
	ofLogNotice("OSC") << " Set LED length as " << numLED;
}

void testApp::exit()
{
	if(led!=NULL)
	{
        stopThread();
		led->clear(0);
		spi.send(led->txBuffer);
	}
}

//--------------------------------------------------------------
void testApp::threadedFunction()
{
	while( isThreadRunning() != 0 ){
		if( lock() ){
			led->setPixels(ledData);
			spi.send(led->txBuffer);
			unlock();
			usleep(10000);
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
	float  dt = 1.0f / ofGetFrameRate();
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;

		receiver.getNextMessage(&m);
		
        if ( m.getAddress() == "/l" ){ // Set a single led
            ledData[m.getArgAsInt32(0)].set(m.getArgAsFloat(1)*255, m.getArgAsFloat(2)*255, m.getArgAsFloat(3)*255);
            
		} else {
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			ofLogWarning("OSC") << " Unknown osc message " << msg_string;
		}
	}

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(20);
    if(ofGetLogLevel() == OF_LOG_VERBOSE) {
        int size = ofGetWidth()/numLED;
        
        for(int i =0 ; i < numLED; i++)
        {
            ofSetColor(ledData[i]);
            ofFill();
            ofRect(i * size, 0, size, ofGetHeight());
        }
        
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}