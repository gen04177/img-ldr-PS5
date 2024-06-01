# img-ldr-PS5

## Prerequisites
- hbldr.elf
- shsrv.elf
- ftpsrv.elf

This app is also compatible with websrv.

For more information and access to required payloads, please visit: [PS5 Payload Dev](https://github.com/ps5-payload-dev)

## How to Use
1. Send the hbldr.elf payload to port 9021 of your PS5.
2. Send the shsrv.elf payload to port 9021 of your PS5.
3. Send the ftpsrv.elf payload to port 9021 of your PS5.
4. Copy the img-ldr.elf payload to `/data` directory on your PS5.
5. Execute `telnet [PS5-IP] 2323`.
6. Run the command `hbldr /data/img-ldr.elf /data/img."png|jpg|bmp"` or `hbldr /data/path_to_image_or_directory"`.

## Credits
Credits to J. Tornblom for the SDK, SDL2 port for PS5, and other payloads.
