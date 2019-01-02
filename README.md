# CR78-MIDIClockToTrigger
MIDI Clock to trigger interface for Arduino and the Roland CR-78

## MIDI Start/Stop
MIDI Start and also Stop and Continue will generate *Start/Stop* trigger for the CR-78.
That is necessary because the CR-78 does not stop on a falling gate but needs two consecutive triggers to start and stop.

## *TS-1 WRITE*
*TS-1 WRITE* is supported.
The interface listens on channel 10 for incoming MIDI notes.
MIDI notes will be quantized to 16ths and will generate a trigger for the TS-1 input.
That makes it possible to record paterns to the sequencer in realtime.

Depends on the [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) and [watterott/Arduino-Libs/digitalWriteFast](https://github.com/watterott/Arduino-Libs/tree/master/digitalWriteFast)
