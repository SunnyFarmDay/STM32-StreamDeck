#ifndef FILES_H
#define FILES_H

#include "fatfs.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "lcdtp.h"
#include "main.h"
#include "pcm.h"

FRESULT scanFiles(char* path);

#endif /* FILES_H */
