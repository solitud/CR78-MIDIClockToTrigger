# Compusync 78 - Programmer and MIDI Clock Converter for Roland CR-78
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

[Demonstration Video](https://www.youtube.com/watch?v=quFpu97K9BY&t=55s)

## BOM
Arduino R3 / ELEGOO UNO R3
Compusync Shield

R1 220 Ohm (x1)
R2 10 kOhm (270-280 Ohm?) (x1)
D1 1N914 (x1)
IC 6N138 (x1)
Male Header 30mm 8 (x2)
Cables 3.5mm mono to 6.3mm mono jack (x3)
Screws, nuts and nylon washers for standoff spacers (x8)
Little rubber feet (x4)

### Eco Version
SparkFun Accessories MIDI Connector - Female Right Angle PRT-09536 (x1)
SparkFun COM-09190 Momentary Button 12mm (x1)
Thonkiconn Mono 3.5mm Audio Jacks (PJ398SM) (x3)
Standoff Spacer M2.5 30mm (x3)
Powersupply 9V center positiv or USB charger (x1)
[Protective Bumper Case](https://shop.sb-components.co.uk/products/protective-bumper-case-for-uno-boards) (x1)

### Boxed Version
MIDI Connector with solder lung (x1)
Arcade style Momentary Button (x1)
9V Socket 5.5mm 2.1 (x1)
PJ301BM Jack Socket with solder lung (x3)
Hammond Case 1590B (x1)
Wires to solder jacks, button and MIDI sockets (x12)
Standoff Spacer M2.5 (x4)
Powersupply 9V center positiv (x1)

