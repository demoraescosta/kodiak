# Kodiak

Kodiak is a small work-in-progress 32-bit kernel made to learn about OS development.

## Building

I have not tested yet how Kodiak builds on non-linux systems and I (for now) do
not plan to do so, the following guide assumes a Linux environment.

## Requirements

- A functional i686-elf-gnu toolchain (gas, gcc, ld, etc...)
- grub
- xorriso
- mtools

## Building the ISO

```
$ make kodiak-iso
```
