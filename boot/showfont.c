void showfont() {

    uint16_t c = 0;
 
    while (c < 0x100) {
        video32[curpos32++] = (VGA_ATTRIBUTE<<8) | c++;
        crtc_write(CRTC_CURPOS_LO, curpos32 & 0xFF);
        crtc_write(CRTC_CURPOS_HI, curpos32 >> 8);

        if (curpos32 >= (80 * VGA_LINES)) {
            scroll(1);
	}
    }
}
