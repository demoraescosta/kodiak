# Kodiak

Kodiak is a small work-in-progress 64-bit kernel.

The kernel currently uses [Limine](https://github.com/limine-bootloader/limine) as its boot protocol and bootloader.

## Building

I have not tested yet how Kodiak builds on non-linux systems and I (for now) do
not plan to do so, the following guide assumes a Linux environment.

## Requirements

- A GNU-compatible C toolchain capable of generating x86-64 ELF objects.
- make 
- xorriso - For building the ISO

## Building the ISO

```
$ make kodiak-iso
```
