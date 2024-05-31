ifdef PS5_PAYLOAD_SDK
    include $(PS5_PAYLOAD_SDK)/toolchain/prospero.mk
else
    $(error PS5_PAYLOAD_SDK is undefined)
endif

    CFLAGS := -O3 -I`$(PS5_SYSROOT)/bin/sdl2-config --cflags` \
          -I`$(PS5_SYSROOT)/bin/sdl2-config --libs` \
          -lkernel_sys -lSDL2_image -lwebp -lwebpmux -lwebpdemux

    SRCS := main.c

    img-ldr.elf: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

    clean:
	rm -f img-ldr.elf