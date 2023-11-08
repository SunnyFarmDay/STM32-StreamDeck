
#include "mp3.h"
#include <stdio.h>
#include <string.h>

// HMP3Decoder mp3Decoder;


FRESULT scanMP3Files(char* path) {
    LCD_DrawString(0, 0, "Scanning...");
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char PathBUF[100];
    char str[100];
    uint32_t trackIndex = 0;
    
    while (1) {
        res = f_opendir(&dir, path);
        if (res != FR_OK) {
            return res;
        }
        LCD_DrawString(0, 15, "Dir OK");
        
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) {
                break;
            }
            
            if (fno.fname[0] == '.' || (fno.fattrib & AM_SYS)) {
                continue;
            }
            
            if (fno.fattrib & AM_DIR) {
                sprintf(PathBUF, "%s/%s", path, fno.fname);
                res = scanMP3Files(path);
                if (res != FR_OK) {
                    break;
                }
            } else {
                if (strstr(fno.fname, ".mp3") != NULL || strstr(fno.fname, ".MP3") != NULL) {
                    sprintf(PathBUF, "%s/%s", path, fno.fname);
                    LCD_DrawString(0, (trackIndex + 4) * 15, PathBUF);
                    trackIndex++;
                }
            }
        }
        break;
    }
    sprintf(str, "Found %d tracks", trackIndex);
    LCD_DrawString(0, (trackIndex + 5) * 15, str);
    return res;
}

uint8_t mp3Buffer[DECODE_BUFFERSIZE];

// void playMP3File(char* path) {
//     char str[100];
//     sprintf(str, "Playing %s", path);
//     LCD_DrawString(0, 200, str);

//     FIL file;
//     FRESULT res;
//     uint32_t br = 0;
//     uint32_t dataToRead = 0;

//     res = f_open(&file, path, FA_READ);
//     if (res != FR_OK) {
//         sprintf(str, "File open error: %d", res);
//         LCD_DrawString(0, 215, str);
//         return;
//     }
//     mp3Decoder = MP3InitDecoder(&mp3Decoder);

//     while (1) {
//         res = f_read(&file, mp3Buffer, DECODE_BUFFERSIZE, &br);
//         if (res != FR_OK) {
//             sprintf(str, "File read error: %d", res);
//             LCD_DrawString(0, 215, str);
//             return;
//         }
//         if (br == 0) {
//             break;
//         }
//         dataToRead = br;
//         while (dataToRead > 0) {
//             int offset = 0;
//             int err = MP3Decode(mp3Decoder, &mp3Buffer, &dataToRead, (short int*) (mp3Buffer + offset), 0);
//             if (err != ERR_MP3_NONE) {
//                 sprintf(str, "MP3 decode error: %d", err);
//                 LCD_DrawString(0, 215, str);
//                 return;
//             }
//             offset += dataToRead;
//         }
//     }
// }