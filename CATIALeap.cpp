#include <iostream>
#include "Leap.h"
#include <Windows.h>

using namespace Leap;

int midCheck = 0;
int altCheck = 0;
int clickCheck = 100;
int loopCount = 2;
float oldScale = 1.00;

class WindListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);

  private:
    Frame             lastFrame;
};

void WindListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void WindListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
}

void WindListener::onDisconnect(const Controller& controller) {
  std::cout << "Disconnected" << std::endl;
}

void WindListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void WindListener::onFrame(const Controller& controller) {
  const Frame frame = controller.frame();

  if (!frame.hands().empty()) {			
	
	const HandList hands = frame.hands();
	const GestureList gestures = frame.gestures();	
	const Pointable pointable = frame.pointables()[0];
	const Screen screen = controller.calibratedScreens()[0];
	
	// Begin Single Hand Code
	if ( hands.count() == 1){

		// Get the first hand
		const Hand hand = frame.hands()[0];

		// Check if the hand has any fingers
		const FingerList fingers = hand.fingers();
		if (!fingers.empty()) {
		
			// Begin single finger code			
			if (fingers.count() == 1) {
				// If the middle button and the shift key are both held down...
				
				// Middle Mouse Up
				INPUT ipm;
				ipm.type = INPUT_MOUSE;
				ipm.mi.time = 0;
				ipm.mi.dwExtraInfo = 0;


				ipm.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
				SendInput(1, &ipm, sizeof(INPUT));

				// Alt Key Up
				INPUT ipk;
				ipk.type = INPUT_KEYBOARD;
				ipk.ki.wScan = 0;
				ipk.ki.time = 0;
				ipk.ki.dwExtraInfo = 0;
	
				ipk.ki.wVk = 0x12;
				ipk.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &ipk, sizeof(INPUT));

				midCheck = 0;
				altCheck = 0;
				loopCount = 2;
				// Let them up and reset the condition check	

				float xCoord = screen.intersect(pointable, true, 1.0F).x * screen.widthPixels();
				float yCoord = screen.heightPixels() - (screen.intersect(pointable, true, 1.0F).y * screen.heightPixels());

				// Left click
				if ( clickCheck == 100 ) {

					mouse_event(0x0002 | 0x0004, 0, xCoord, yCoord, 0);

					clickCheck = 0;
				}			

				SetCursorPos(xCoord, yCoord);	 
			} //End single finger code

			// Begin two finger code
			if (fingers.count() == 2) {

				// If the middle button and the shift key are both held down...
				
				// Middle Mouse Up
				INPUT ipm;
				ipm.type = INPUT_MOUSE;
				ipm.mi.time = 0;
				ipm.mi.dwExtraInfo = 0;


				ipm.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
				SendInput(1, &ipm, sizeof(INPUT));
				
				// Alt Key Up
				INPUT ipk;
				ipk.type = INPUT_KEYBOARD;
				ipk.ki.wScan = 0;
				ipk.ki.time = 0;
				ipk.ki.dwExtraInfo = 0;
	
				ipk.ki.wVk = 0x12;
				ipk.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &ipk, sizeof(INPUT));

				midCheck = 0;
				altCheck = 0;
				loopCount = 2;
				// Let them up and reset the condition check
					 
				float xCoord = screen.intersect(pointable, true, 1.0F).x * screen.widthPixels();
				float yCoord = screen.heightPixels() - (screen.intersect(pointable, true, 1.0F).y * screen.heightPixels());

				SetCursorPos(xCoord, yCoord);	 
			} //End two finger code

			// Common condition for more than two fingers
			if (fingers.count() > 2) {

				// If middle mouse button is not already held down...
				if (midCheck == 0){
					//mIDDLE Mouse button					
					INPUT ipm;
					ipm.type = INPUT_MOUSE;
					ipm.mi.time = 0;
					ipm.mi.dwExtraInfo = 0;

					ipm.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
					SendInput(1, &ipm, sizeof(INPUT));

					loopCount = 2;
					midCheck = 1;
					// Hold it down and set condition
				}
			
				// Begin four finger code
				if (fingers.count() == 4) {

					// If alt button is being held down...
					if (altCheck == 1){
						INPUT ipk;
						ipk.type = INPUT_KEYBOARD;
						ipk.ki.wScan = 0;
						ipk.ki.time = 0;
						ipk.ki.dwExtraInfo = 0;
	
						ipk.ki.wVk = 0x12;
						ipk.ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, &ipk, sizeof(INPUT));

						loopCount = 2;
						altCheck = 0;							
					}

					float xCoord = screen.intersect(pointable, true, 1.0F).x * screen.widthPixels();
					float yCoord = screen.heightPixels() - (screen.intersect(pointable, true, 1.0F).y * screen.heightPixels());				

					SetCursorPos(xCoord, yCoord);

				} // End four finger code


				// Begin five finger code
				else if (fingers.count() > 4) {

					// If alt button is not being held down...
					if (altCheck == 0){
						INPUT ip;
						ip.type = INPUT_KEYBOARD;
						ip.ki.wScan = 0;
						ip.ki.time = 0;
						ip.ki.dwExtraInfo = 0;
						ip.ki.wVk = 0x12;
						ip.ki.dwFlags = 0;
						SendInput(1, &ip, sizeof(INPUT));

						altCheck = 1;		
						loopCount = 2;
					}
						 
					float xCoord = screen.intersect(pointable, true, 1.0F).x * screen.widthPixels();
					float yCoord = screen.heightPixels() - (screen.intersect(pointable, true, 1.0F).y * screen.heightPixels());	

					SetCursorPos(xCoord, yCoord);
				}// End five finger code
			}// End multiple finger code


			if ( clickCheck < 100 ) {

				clickCheck++;

			}

			std::cout << "Click Check: " << clickCheck << "    # Fingers: " << fingers.count() << std::endl;

		}// End if empty fingers
	 }// End Single Hand Code

	 // Begin Double Hand Code
	 else if ( hands.count() == 2){

		 // If the middle button and the shift key are both held down...
		 			
			// Middle Mouse Up
			INPUT ipm;
			ipm.type = INPUT_MOUSE;
			ipm.mi.time = 0;
			ipm.mi.dwExtraInfo = 0;


			ipm.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			SendInput(1, &ipm, sizeof(INPUT));
			
			// Alt Key Up
			INPUT ipk;
			ipk.type = INPUT_KEYBOARD;
			ipk.ki.wScan = 0;
			ipk.ki.time = 0;
			ipk.ki.dwExtraInfo = 0;

			ipk.ki.wVk = 0x12;
			ipk.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ipk, sizeof(INPUT));

			midCheck = 0;
			altCheck = 0;
			// Let them up and reset the condition check	

		float scaleFactor = frame.scaleFactor(lastFrame);
		
		if ( loopCount == 2 ){ 

			if ((scaleFactor < 0.9500) || (scaleFactor > 1.05)){

			if ( ( scaleFactor-oldScale) < 0 ){
				INPUT ip;
				ip.type = INPUT_MOUSE;
				ip.mi.time = 0;
				ip.mi.mouseData = 120;


				ip.mi.dwFlags = MOUSEEVENTF_WHEEL;
				SendInput(1, &ip, sizeof(INPUT));
			}	
			else if ((scaleFactor-oldScale) > 0){
				INPUT ip;
				ip.type = INPUT_MOUSE;
				ip.mi.time = 0;
				ip.mi.mouseData = -120;


				ip.mi.dwFlags = MOUSEEVENTF_WHEEL;
				SendInput(1, &ip, sizeof(INPUT));
			}
			}

			loopCount = 0;

			lastFrame = frame;
			
		}
		else {
			loopCount++;
		}

				
	 }// End Double Hand Code
  }// End if empty hands
}// End object

int main() {
  // Create a sample listener and controller
  WindListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
