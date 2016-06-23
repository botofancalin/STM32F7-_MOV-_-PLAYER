///////////////////////////////  
// 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MY_WAVEPLAYER_H
#define __MY_WAVEPLAYER_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define AUDIO_OUT_BUFFER_SIZE    8192


typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_NEXT,    
  AUDIO_STATE_PLAY
 
}AUDIO_PLAYBACK_StateTypeDef;

typedef enum {
  AUDIO_SELECT_MENU = 0,
  AUDIO_PLAYBACK_CONTROL,  
}AUDIO_DEMO_SelectMode;

typedef enum {
  BUFFER_OFFSET_NONE = 0,  
  BUFFER_OFFSET_HALF,  
  BUFFER_OFFSET_FULL,     
}BUFFER_StateTypeDef;


typedef struct {
  uint32_t ChunkID;       /* 0 */ 
  uint32_t FileSize;      /* 4 */
  uint32_t FileFormat;    /* 8 */
  uint32_t SubChunk1ID;   /* 12 */
  uint32_t SubChunk1Size; /* 16*/  
  uint16_t AudioFormat;   /* 20 */ 
  uint16_t NbrChannels;   /* 22 */   
  uint32_t SampleRate;    /* 24 */
  
  uint32_t ByteRate;      /* 28 */
  uint16_t BlockAlign;    /* 32 */  
  uint16_t BitPerSample;  /* 34 */  
  uint32_t SubChunk2ID;   /* 36 */   
  uint32_t SubChunk2Size; /* 40 */    
}WAVE_FormatTypeDef;

typedef enum {
  BUFFER_EMPTY = 0,  
  BUFFER_FULL,     
}WR_BUFFER_StateTypeDef;

 
/* Audio buffer control struct */
typedef struct {
//  uint8_t buff[AUDIO_OUT_BUFFER_SIZE];
  BUFFER_StateTypeDef state;
  uint32_t fptr;
}AUDIO_OUT_BufferTypeDef;



 AUDIO_ErrorTypeDef Play_WAV(FIL *WavFile,uint32_t SR,uint32_t uwVolume);
 
 uint8_t PlayerInit(uint32_t AudioFreq);
AUDIO_ErrorTypeDef GetFileInfo(uint16_t file_idx, WAVE_FormatTypeDef *info);

 
#endif /* __MY_WAVEPLAYER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
