#include "files.h"

FRESULT scanFiles(char* path) {
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
                continue;
                // res = scanFiles(path);
                // if (res != FR_OK) {
                //     break;
                // }
            } else {
                if (strstr(fno.fname, ".mp3") != NULL || strstr(fno.fname, ".MP3") != NULL) {
                    sprintf(PathBUF, "%s/%s", path, fno.fname);
                    LCD_DrawString(0, (trackIndex + 4) * 15, PathBUF);
                    trackIndex++;
                } else if (strstr(fno.fname, ".pcm") != NULL || strstr(fno.fname, ".PCM") != NULL) {
                    sprintf(PathBUF, "%s/%s", path, fno.fname);
                    LCD_DrawString(0, (trackIndex + 4) * 15, PathBUF);
                    playPCMFile(PathBUF);
                    trackIndex++;
                }
            }
        }
        break;
    }
    sprintf(str, "Found %d tracks", (int) trackIndex);
    LCD_DrawString(0, (trackIndex + 5) * 15, str);
    return res;
}
