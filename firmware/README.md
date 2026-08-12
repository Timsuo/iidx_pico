# IIDX Pico Firmware

Features:
* 1000Hz polling rate.
* HID lights.
* RGB turntable.
* Configuration save.
* Customizable through board_defs.h
* Dymanic settings.

## BOARD_IIDX_TIMSUO_V1

The `iidx_timsuo` target supports the Timsuo V1 PCB while preserving the
original `iidx_pico` target.

### GPIO mapping

| Function | GPIO |
| --- | --- |
| KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7 | GP8, GP7, GP6, GP5, GP4, GP3, GP2 |
| E1, E2, E3, E4 | GP12, GP11, GP10, GP9 |
| Key RGB | GP13 |
| Encoder A, B (reserved, not implemented) | GP14, GP15 |
| Turntable I2C0 SDA, SCL | GP16, GP17 |
| LCD SPI0 SCK, SDA/MOSI | GP18, GP19 |
| LCD CS, RESET, RS/DC/A0 | GP20, GP21, GP22 |
| Turntable RGB | GP28 |

GP0 and GP1 are unused. Mechanical buttons use internal pull-ups and
active-low switches to ground. GP16/GP17 are the only turntable sensor port
and support AS5600, MT6701, and TMAG5273. Timsuo V1 has no Hall-key hardware
and does not use GP22 as a WS2812 output.

### Controls and HID mode

| Context | Chord | Action |
| --- | --- | --- |
| Boot | E1 + E2 | Save and use Konami mode |
| Boot | E3 + E4 | Save and use Beatoraja mode |
| Boot | E1 + E2 + E3 + E4 | Enter RP2040 BOOTSEL |
| Runtime/setup | E1 + E2 | YES |
| Runtime/setup | E3 + E4 | NO |
| Runtime/setup | E1 + E2 + E3 + E4 | YES + NO / setup chord |

The runtime control chords use an approximately 40 ms decision window only
on the setup path. Gameplay HID reads the physical buttons immediately.
Factory reset remains available as the `factory` CLI command.

CLI HID commands:

```text
hid
hid beatoraja
hid konami
```

Beatoraja maps KEY1-KEY7 to buttons 1-7 and E1-E4 to buttons 8-11. Konami
keeps button 8 empty and maps E1-E4 to buttons 9-12. Both modes keep the
existing turntable X/Y axes.

### LCD

The supported module is JLX12864G-086-PC. Its LCD controller is UC1701X;
the manufacturer documentation states that it is ST7565R-compatible. The
firmware uses a 4 MHz, mode-0, MSB-first, write-only SPI0 connection:

| LCD signal | GPIO |
| --- | --- |
| SCK | GP18 |
| SDA/MOSI | GP19 |
| CS (active low) | GP20 |
| RESET (active low) | GP21 |
| RS/DC/A0 (0 command, 1 data) | GP22 |

LCD power and backlight are handled entirely by the PCB hardware. The
firmware does not control LEDA and does not access the JLX-GB2312 font ROM.
All text is rendered by an MCU-side ASCII bitmap font into a 128x64, 1-bpp,
1024-byte framebuffer. Status rendering runs at about 20 Hz, and the runtime
transfer task sends at most one 128-byte dirty page per invocation.

