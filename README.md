# Homebrew Launcher v3

The Homebrew Launcher is a WiiU homebrew that lists homebrew applications located on a SD card and permits launching them (similar to the Homebrew Channel of the Wii).

## Usage

To use the Homebrew Launcher (or HBL, for short) you must copy homebrew_launcher.wuhb into `sd:/wiiu/apps/homebrew_launcher/homebrew_launcher.wuhb`.

To start the Homebrew Launcher you need to install [Aroma](https://aroma.foryour.cafe/).

The apps (WUHB and RPX files) that will be listed are should be in the following path `sd:/wiiu/apps/`, and can be in a folder. If the wuhb/rpx is in a folder, an optional meta.xml and/or icon.png can be provided to provide additional information. If both an RPX and WUHB are provided (for legacy purposes), the WUHB is prioritized. The SD may look as follows:

- sd:/
  - wiiu/
    - apps/
     - homebrew_launcher/
        - homebrew_launcher.wuhb
        - homebrew_launcher.rpx
        - meta.xml
        - icon.png
     - app_1/
        - app_1.wuhb
        - meta.xml
     - app_2.wuhb
     - app_3.wuhb
     - app_4/
        - app_4.rpx
        - meta.xml
        - icon.png
     - app_5/
        - app_5.wuhb
        - app_5.rpx

## Building the Homebrew Launcher
Clone the respository recursively with `git clone --recursive`, and run `make`. You can also build for PC targets using SDL with `make pc`.

After, both homebrew_launcher.wuhb and homebrew_launcher.rpx should be in the current directory. (Or homebrew_launcher.exe for PC).

## Building an application for the Homebrew Launcher 
For an example on how to build an application for the HBL check out the [wut samples](https://github.com/decaf-emu/wut/tree/master/samples).

## Legacy Information
This repo is an updated version of HBL for the Aroma environment that builds to a WUHB (Wii U HomeBrew) file. For the original, see [dimok789/homebrew_launcher](https://github.com/dimok789/homebrew_launcher).

This updated HBL is **not** able to run legacy WiiU ELF files, and these files will display as unsupported. If you need to run an older WiiU homebrew, try using [Tiramisu](https://tiramisu.foryour.cafe/) and the previous version of HBL.

## Credits
* Dimok
* orboditilt
* exjam
* shinyquagsire23
* cathor
* Maschell
* several more contributors
