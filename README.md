MadCatz2Joystick
================

_MadCatz2Joystick_ exclusively captures the events of the __MadCatz PC USB Wired Stick 8818__ and maps these events to corresponding events on a virtual game pad device.

The MadCatz PC USB Wired Stick 8818 is otherwise known as the Street Fighter IV Arcade Fightstick by Capcom. This software currently only supports the 8818 model
but can probably easily support other models, so raise an issue or hack the source and submit a pull request!

On the Raspberry Pi the joystick is already registered as a device, but it does not appear to be compatible with MAME. _MadCatz2Joystick_ remaps its so that it is
compatible.

_MadCatz2Joystick_ is nearly entirely based on [_Xarcade2Jstick_](https://github.com/petrockblog/Xarcade2Joystick). It is perfect for use with [RetroPie](http://blog.petrockblock.com/retropie/).

## Usage

Your joystick will already be registered as a joystick device. _MadCatz2Joystick_ registers an additional joystick / gamepad.

Sometimes I have found the that MadCatz stick doesn't work when it is plugged in. It is detected by the OS, but no keypresses register.
In that case, unplug and replug the device.

In MAME, go into the key settings (press TAB) and map the joystick to the appropriate keys.

## Downloading

If you would like to download the current version of _MadCatz2Jstick_ from [its Github repository](https://github.com/karlvr/MadCatz2Joystick), you can use this command:
```bash
git clone https://github.com/karlvr/MadCatz2Jstick
```

## Building and Installation

To build _MadCatz2Jstick_ follow these commands:
```bash
cd MadCatz2Jstick
make
```

If everything went fine you can install with the command
```bash
sudo make install
```

## Installation as udev / systemd service

You can install _MadCatz2Jstick_ udev and systemd service configuration with this command:
```bash
sudo make installservice
```

## Uninstalling the service and/or the binary

You can uninstall the udev and systemd service configuration with this command:
```bash
sudo make uninstallservice
```

You can uninstall the binary with this command:
```bash
sudo make uninstall
```

<br><br>
__Have fun!__

-Karl
