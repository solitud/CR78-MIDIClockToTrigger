/**
 * MIDI Clock to CR-78 
 * Knut Schade | knut@solitud.de
 * 2019
 */

#include <digitalWriteFast.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE(); // Created and binds the MIDI interface to the default hardware Serial port

#define ledClockPin               LED_BUILTIN
#define outputStartStopPin        5
#define outputClockPin            6
#define outputWritePin            7
#define inputButtonPin            3

unsigned int clockDivider           = 2;
unsigned int quantDivider           = 6; // 96 / 16

unsigned int inc                    = 0;

unsigned long openedGateAt          = 0;
unsigned long openedStartStopGateAt = 0;
unsigned long openedWriteGateAt     = 0;

unsigned long const gateTimeMicros  = 5100; //min 5 millisec

boolean CR78wasTriggered            = false;
boolean trigBtnIsEnabled            = true;


void setup() {
    pinModeFast(ledClockPin, OUTPUT);
    pinModeFast(outputStartStopPin, OUTPUT);
    pinModeFast(outputClockPin, OUTPUT);
    pinModeFast(outputWritePin, OUTPUT);
    pinModeFast(inputButtonPin, INPUT);

    MIDI.begin(MIDI_CHANNEL_OMNI);

    MIDI.setHandleStart(doStartStop);
    MIDI.setHandleContinue(doStartStop);
    MIDI.setHandleStop(doStartStop);
    MIDI.setHandleClock(doClock);
    MIDI.setHandleNoteOn(doWrite);

    MIDI.turnThruOff();  
}

void loop() {
    MIDI.read(10);
    unsigned long currentTimeMicros = micros();
    if((currentTimeMicros - openedGateAt) >= gateTimeMicros) {
        digitalWriteFast(outputClockPin, LOW);
    }
    if((currentTimeMicros - openedStartStopGateAt) >= 750) {
        digitalWriteFast(ledClockPin, LOW);
        digitalWriteFast(outputStartStopPin, LOW);
    }

    if((currentTimeMicros - openedWriteGateAt) >= gateTimeMicros) {
        digitalWriteFast(outputWritePin, LOW);
    }

    if(CR78wasTriggered) {
        if(inc % quantDivider == 0) {
            digitalWriteFast(outputWritePin, HIGH);
            openedWriteGateAt = currentTimeMicros;
            CR78wasTriggered = false;
        }
    } else {
        if(digitalReadFast(inputButtonPin) == HIGH) {
            if(trigBtnIsEnabled) {
                CR78wasTriggered = true;
                trigBtnIsEnabled = false;
            }
        } else {
            trigBtnIsEnabled = true;
        }
    }
}

void doClock(void) {
    unsigned long currentTimeMicros = micros();
    if(inc % clockDivider == 0) {
        digitalWriteFast(outputClockPin, HIGH);
        openedGateAt = currentTimeMicros;
    }
    inc++;
}

void doWrite(byte channel, byte note, byte velocity) {
    CR78wasTriggered = true;
}

void doStartStop(void) {
    unsigned long currentTimeMicros = micros();
    inc = 0;
    digitalWriteFast(ledClockPin, HIGH);
    digitalWriteFast(outputStartStopPin, HIGH);
    openedStartStopGateAt = currentTimeMicros;
}
