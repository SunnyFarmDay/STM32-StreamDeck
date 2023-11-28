#include "files.h"

FRESULT scanFiles(char* path) {
    // LCD_DrawString(0, 300, "Scanning...");
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
        // LCD_DrawString(0, 15, "Dir OK");
        
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (!playPCMFlag) {
                break;
            }
            if (res != FR_OK || fno.fname[0] == 0) {
                break;
            }
            
            if (fno.fname[0] == '.' || (fno.fattrib & AM_SYS)) {
                continue;
            }
            
            if (fno.fattrib & AM_DIR) {
                sprintf(PathBUF, "%s/%s", path, fno.fname);
                continue;
                // res = scanFiles(path);
                // if (res != FR_OK) {
                //     break;
                // }
            } else {
                if (strstr(fno.fname, ".mp3") != NULL || strstr(fno.fname, ".MP3") != NULL) {
                    sprintf(PathBUF, "%s/%s", path, fno.fname);
                    // LCD_DrawString(0, (trackIndex + 4) * 15, PathBUF);
                    trackIndex++;
                } else if (strstr(fno.fname, ".pcm") != NULL || strstr(fno.fname, ".PCM") != NULL) {
                	nextSongFlag = 0;
                    sprintf(PathBUF, "%s/%s", path, fno.fname);
                    // LCD_DrawString(0, (trackIndex + 4) * 15, PathBUF);
                    // LCD_Clear(10, 300, 240, 320, BACKGROUND);
                    sprintf(str, "Playing %s", PathBUF);
                    LCD_DrawString(10, 300, str);
                    playPCMFile(PathBUF);
                    LCD_DrawBox(10, 300, 180, 20, BACKGROUND);
                    trackIndex++;
                }
            }
        }
        break;
    }
    // sprintf(str, "Found %d tracks", (int) trackIndex);
    // LCD_DrawString(0, (trackIndex + 5) * 15, str);
    // LCD_Clear(0, 0, 240, 320, BACKGROUND);
    return res;
}
