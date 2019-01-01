/**
 * MIDI Clock to CR-78 
 * Knut Schade | knut@solitud.de
 * 2019
 */

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE(); // Created and binds the MIDI interface to the default hardware Serial port

int const ledClockPin               = LED_BUILTIN;
int const outputStartStopPin        = 5;
int const outputClockPin            = 6;
int const outputWritePin            = 7;

int const inputButtonPin            = 3;

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
    pinMode(ledClockPin, OUTPUT);
    pinMode(outputStartStopPin, OUTPUT);
    pinMode(outputClockPin, OUTPUT);
    pinMode(outputWritePin, OUTPUT);

    pinMode(inputButtonPin, INPUT);

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
        digitalWrite(outputClockPin, LOW);
    }
    if((currentTimeMicros - openedStartStopGateAt) >= 750) {
        digitalWrite(ledClockPin, LOW);
        digitalWrite(outputStartStopPin, LOW);
    }

    if((currentTimeMicros - openedWriteGateAt) >= gateTimeMicros) {
        digitalWrite(outputWritePin, LOW);
    }

    if(CR78wasTriggered) {
        if(inc % quantDivider == 0) {
            digitalWrite(outputWritePin, HIGH);
            openedWriteGateAt = currentTimeMicros;
            CR78wasTriggered = false;
        }
    } else {
        boolean trigBtnState = digitalRead(inputButtonPin);
        if(trigBtnState) {
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
        digitalWrite(outputClockPin, HIGH);
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
    digitalWrite(ledClockPin, HIGH);
    digitalWrite(outputStartStopPin, HIGH);
    openedStartStopGateAt = currentTimeMicros;
}
