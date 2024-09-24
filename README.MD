# img-ldr-PS5 (1.xx-4.xx)

## Prerequisites
- elfldr.elf
- shsrv.elf
- ftpsrv.elf


## How to Use
1. Send the elfldr.elf payload to port 9020 of your PS5.
2. Send the shsrv.elf payload to port 9021 of your PS5.
3. Send the ftpsrv.elf payload to port 9021 of your PS5.
4. Copy the img-ldr.elf payload to `/data` directory on your PS5.
5. Execute `telnet [PS5-IP] 2323`.
6. Run the command `hbldr /data/img-ldr.elf /data/img."png|jpg|bmp"` or `hbldr /data/path_to_image_or_directory"`.

## Credits
J. Tornblom - SDK and other payloads.
