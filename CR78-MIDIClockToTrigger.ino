/**
 * MIDI Clock to CR-78 
 * Knut Schade | knut@solitud.de
 * 2019 - 2021
 */

#include <digitalWriteFast.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE(); // Created and binds the MIDI interface to the default hardware Serial port

#define ledClockPin               LED_BUILTIN
#define outputClockPin            3
#define outputWritePin            4
#define outputStartStopPin        5
#define inputButtonPin            2

unsigned int clockDivider           = 2;
unsigned int quantDivider           = 6; // 96 / 16

unsigned int inc                    = 0;

unsigned long openedGateAt          = 0;
unsigned long prevOpenedGateAt      = 0;
unsigned long openedStartStopGateAt = 0;
unsigned long openedWriteGateAt     = 0;
unsigned long trigBtnReleaseTime    = 0;

unsigned long const gateTimeMicros  = 5100; //min 5 millisec

boolean CR78wasTriggered            = false;
boolean trigBtnIsEnabled            = true;
boolean trigBtnWasPressed           = true;
boolean sequencerIsRunning          = false;
boolean continousMcMode             = false;
unsigned long clockInterval         = 0;

void setup() {
    pinModeFast(ledClockPin, OUTPUT);
    pinModeFast(outputStartStopPin, OUTPUT);
    pinModeFast(outputClockPin, OUTPUT);
    pinModeFast(outputWritePin, OUTPUT);
    pinMode(inputButtonPin, INPUT_PULLUP);

    MIDI.begin(MIDI_CHANNEL_OMNI);

    MIDI.setHandleStart(doStart);
    MIDI.setHandleContinue(doContinue);
    MIDI.setHandleStop(doStop);
    MIDI.setHandleClock(doClock);
    MIDI.setHandleNoteOn(doWrite);

    MIDI.turnThruOff();

    clockInterval = (60000000 / 120 / 12); //1min/BPM/12clocks per quarter
}

void loop() {
    MIDI.read(10);
    unsigned long currentTimeMillis = millis();
    unsigned long currentTimeMicros = micros();
    if((currentTimeMicros - openedGateAt) >= gateTimeMicros) {
        digitalWriteFast(outputClockPin, LOW);
    }
    if((currentTimeMicros - openedStartStopGateAt) >= 750) {
        //digitalWriteFast(ledClockPin, LOW);
        digitalWriteFast(outputStartStopPin, LOW);
    }

    if((currentTimeMicros - openedWriteGateAt) >= gateTimeMicros) {
        digitalWriteFast(outputWritePin, LOW);
        //digitalWriteFast(ledClockPin, LOW);
    }

    if(CR78wasTriggered) {
        if(inc % quantDivider == 0) {
            digitalWriteFast(outputWritePin, HIGH);
            //digitalWriteFast(ledClockPin, HIGH);
            openedWriteGateAt = currentTimeMicros;
            CR78wasTriggered = false;
        }
    } else {
        if(digitalReadFast(inputButtonPin) == LOW) { //Btn is pressed (internal Pullup)
            if(trigBtnIsEnabled) {
                CR78wasTriggered = true;
                trigBtnIsEnabled = false;
                trigBtnWasPressed = true;
            }
        } else {
            if(trigBtnWasPressed == true) {
                trigBtnReleaseTime = currentTimeMillis;
            }
            trigBtnWasPressed = false;

            if((currentTimeMillis - trigBtnReleaseTime) > 50) {
                trigBtnIsEnabled = true;
            }
        }
    }
    if(continousMcMode == false) {
    	if(sequencerIsRunning == false) {
    		if(prevOpenedGateAt > 0) {
    			clockInterval = ceil((openedGateAt - prevOpenedGateAt)/1000)*1000;
    		}
    		if((currentTimeMicros) >= (openedGateAt + clockInterval)) {
    			triggerClock();
    		}
    	}
    }
}

void doClock(void) {
    triggerClock();
    
    if(sequencerIsRunning == false) {
      continousMcMode = true;
    } else {
      continousMcMode = false;
    }
}

void triggerClock(void) {
    unsigned long currentTimeMicros = micros();
    if(inc % clockDivider == 0) {
        digitalWriteFast(outputClockPin, HIGH);
        prevOpenedGateAt = openedGateAt;
        openedGateAt = currentTimeMicros;
    }
    inc++;
}

void doWrite(byte channel, byte note, byte velocity) {
    CR78wasTriggered = true;
}

void doStartStop(void) {
    unsigned long currentTimeMicros = micros();
    if(inc == 0) {
      digitalWriteFast(ledClockPin, HIGH);
    } else {
      digitalWriteFast(ledClockPin, LOW);
    }
    inc = 0;
    digitalWriteFast(outputClockPin, HIGH);
    digitalWriteFast(outputStartStopPin, HIGH);
    openedStartStopGateAt = currentTimeMicros;
}

void doStart(void) {
  sequencerIsRunning = true;
  doStartStop();
}

void doContinue(void) {
  sequencerIsRunning = true;
  doStartStop();
}

void doStop(void) {
  sequencerIsRunning = false;
  doStartStop();
}
