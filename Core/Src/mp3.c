
#include "mp3.h"
#include <string.h>
HMP3Decoder mp3Decoder;




u_char mp3Buffer[DECODE_BUFFERSIZE][8];

void playMP3File(char* path) {
    char str[100];
    sprintf(str, "Playing %s", path);
    LCD_DrawString(0, 200, str);

    FIL file;
    FRESULT res;
    uint br = 0;
    int dataToRead = 0;

    res = f_open(&file, path, FA_READ);
    if (res != FR_OK) {
        sprintf(str, "File open error: %d", res);
        LCD_DrawString(0, 215, str);
        return;
    }
    mp3Decoder = MP3InitDecoder();

    while (1) {
        res = f_read(&file, mp3Buffer, DECODE_BUFFERSIZE, &br);
        if (res != FR_OK) {
            sprintf(str, "File read error: %d", res);
            LCD_DrawString(0, 215, str);
            return;
        }
        if (br == 0) {
            break;
        }
        dataToRead = br;
        while (dataToRead > 0) {
            int offset = 0;
            int err = MP3Decode(mp3Decoder, &mp3Buffer, &dataToRead, (short int*) (mp3Buffer + offset), 0);
            if (err != ERR_MP3_NONE) {
                sprintf(str, "MP3 decode error: %d", err);
                LCD_DrawString(0, 215, str);
                return;
            }
            offset += dataToRead;
        }
    }
}
