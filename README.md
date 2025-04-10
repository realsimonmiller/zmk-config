# eiga's zmk-config

### Table of contents
- Branches
    - [Corne](#corne)
    - [Charybdis](#charybdis)
        - [Module documentation](#module-documentation)
    - [TOTEM](#totem)
- [Dongle Flashing](#dongle-flashing)
- [ZMK Keymap Editor](#zmk-keymap-editor)
- [Shops and Useful links](#shops-and-other-useful-links)

## Hello

You probably reached this page by watching one of these [Ergo Split Keyb Videos](https://www.youtube.com/playlist?list=PL1E2ddJCbc13DvCGYXX9jVVX1BqNGKE5D).

This repository hosts zmk-configs as seen in the YouTube videos linked above.

Additionally, this repository contains alternative configurations meant to be used with an extra controller acting as a dongle. Pease refer to the [Dongle Flashing](https://github.com/eigatech/zmk-config#dongle-flashing) chapter for instructions.

> [!NOTE]  
> These configurations are meant to be used with builds that are identical to the ones featured in the videos, otherwise they should be used as reference only.

## Corne

- [Corne](https://github.com/eigatech/zmk-config/tree/corne)
- [Corne Dongle](https://github.com/eigatech/zmk-config/tree/corne-dongle)

> [!NOTE]  
> Shield is configured for an earlier revision of Pete Johanson's [Corne Ultralight PCB](https://github.com/petejohanson/crkbd/tree/board/corne-ultralight), with nice!view support.

## Charybdis

- [Charybdis](https://github.com/eigatech/zmk-config/tree/charybdis-3.5)
- [Charybdis Dongle](https://github.com/eigatech/zmk-config/tree/charybdis-dongle)

### Module documentation

- Charybdis
    - [ZMK PMW3610 Driver](https://github.com/inorichi/zmk-pmw3610-driver)
- Charybdis Dongle
    - [ZMK Split Peripheral Input Relay](https://github.com/badjeff/zmk-split-peripheral-input-relay)
    - [ZMK Input Behavior Listener](https://github.com/badjeff/zmk-input-behavior-listener)
    - [ZMK PMW3610 Driver](https://github.com/badjeff/zmk-pmw3610-driver)

> [!NOTE]  
> Charybdis uses Inorichi's PMW3610 driver, while Charybdis Dongle leverages multiple modules written by badjeff.

## TOTEM

- [TOTEM](https://github.com/eigatech/zmk-config/tree/totem)
- [TOTEM Dongle](https://github.com/eigatech/zmk-config/tree/totem-dongle)
- [TOTEM Prospector](https://github.com/eigatech/zmk-config/tree/totem-prospector)

## Dongle Flashing

Dongle configs use Seeed Xiao Ble microcontrollers housed in a nifty 3D printed [case](https://www.printables.com/model/522586-seeed-xiao-ble-case).

1. Turn all controllers off
2. Flash the dongle controller with the **appropriate** `settings_reset` file.
3. Flash the dongle controller with the `dongle` file.
4. Flash the first half with the the `settings_reset` file.
5. Flash the first half with the `left` or `right` files.
6. Repeat steps 4 and 5 for the other half.

> [!WARNING]  
> When using both Nice!Nano and Seeed XIAO microcontrollers, make sure you are flashing them with the correct files!

## ZMK Keymap Editor

Nick Coutsos' [Keymap Editor](https://nickcoutsos.github.io/keymap-editor/) is a user-friendly, browser-based WYSIWYG app designed to make editing your keymap file easier. It supports conditional layers, behaviors, combo and macro editing, rotary encoders, and more.

## Shops and other useful links

Kits, Prebuilts, Parts:
- [kbd.news](https://kbd.news/vendors) - mechanical keyboard vendors list
- [42keebs.eu](http://42keebs.eu/) - diy kits, including Corne, switches and other parts
- [keeb.supply](https://keeb.supply/) - diy kits and prebuilts, including TOTEM, tools and other parts
- [splitkb.com](https://splitkb.com/) - diy kits, including Corne, switches, tools and other parts
- [bastardkb.com](https://bastardkb.com/) - diy kits and prebuilts, including Charybdis (wired only w/ qmk)
- [typeractive.xyz](https://typeractive.xyz/) - diy kits and prebuilts, including Corne w/ nice!views, switches, tools and other parts

Documentation and guides:
- [ZMK Firmware Documentation](https://zmk.dev/docs)
- [Eren's Wireless Charybdis Mini Guide](https://github.com/erenatas/charybdis-wireless-3x6)


note from reddit:

Thanks for the insight. I'm glad to know the microcontroller isn't the issue. The only other difference I can think of are the batteries. I couldn't find 3.7V 120mAh batteries that fit the case so I ended up with 3.7V 100mAh batteries on both halves.

I am using a fork of eiga's config without any changes. I used the steps in the Connection Issues page of the ZMK docs to get it working without the dongle. I've tried following those same instructions to set it up with the dongle, but the instructions aren't for a dongle setup so I can't follow them exactly. I've tried a few variations, but I can only ever get one half or the other to connect to the dongle. I've also tried using the [Reset Instructions with Dongle](https://docs.slicemk.com/firmware/zmk/wireless/nvsclear/) steps (and some variations) from the SliceMK ZMK page I ran across looking for a solution (using the uf2 files from the fork instead of the ones mentioned in those instructions), but I keep running into the same issue of only being able to connect one half to the dongle.

Edit: Scratch that. I finally got it to work. I tried again a couple times trying to combine the Reset Instructions with/without Dongle steps from the SliceMK page. What finally seemed to work was

1. Flash Dongle w/ settings reset
2. Flash Left w/ settings reset (power On)
3. Flash Right w/ settings reset (power On)
4. Flash Right with firmware
5. Press Right Reset Button
6. Wait 20 seconds
7. Flash Left with firmware
8. Press Left Reset Button
9. Wait 20 seconds
10. Flash Dongle with Firmware

I'm not sure what caused it to work but I'll get a chance to test it again soon since I need to update the keymap now that I know the boards are working. Thanks again :)


----

other changes:

Added hm ctrl+alt+shift to space button to navigate glazewm.
