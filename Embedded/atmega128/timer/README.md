# Timer Clock-Select Notes

Timer 0 and Timer 2 use different clock-select bit settings. Do not assume that the same bit pattern selects the same prescaler on both timers.

Reference: Atmel ATmega128A datasheet, page 300.

Recorded settings for `clkI/O / 256` in the original notes:

```text
cs0n = 1 1 0
cs2n = 1 0 0
```
