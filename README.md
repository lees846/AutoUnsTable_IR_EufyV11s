# UnsTable Automation

This repository holds the code for the automization of UnsTable, a robot drawing
desk that moves while you work on it. It is made using a Eufy RoboVac 11s, thus
this code uses the Arduino-IRremote Library
https://github.com/Arduino-IRremote/Arduino-IRremote to transmit IR signals
through an Infrared LED that were collected using RecieveDemo.ino from the same
library.

## Automated How?

Two sets of random IR signals (corresponding with remote buttons) are sent to
UnsTable every 3 seconds.

## Sample `IrSender` code

```
IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450,
&downRawData[0], 48, PROTOCOL_IS_LSB_FIRST, <RepeatPeriodMillis>,
<numberOfRepeats>);
```

### Breakdown

- [0] to [6] are local to the remote, and can be used for all 4 tested signals
  for the Eufy (up right down left) "&downRawData[0]" needs to be specific to
  the uint32_t name
- [8] doesn't change between button signals PROTOCOL_IS_LSB_FIRST: not sure but
  works
- <RepeatPeriodMillis>: ms between signal sends needs to be >200 or so to be
  smooth ~500 starts to look jumpy but would be quite chaotic
- <numberOfRepeats>: Might start at 0 (i.e. 3 -> 4 signals sent)

### Notes

One send of forward/UP is around 200 (RepeatPeriodMillis), 4 (numberOfRepeats)
for any of the other button sends
