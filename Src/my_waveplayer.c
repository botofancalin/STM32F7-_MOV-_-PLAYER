/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "my_waveplayer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static    AUDIO_OUT_BufferTypeDef  BufferCtl;
static __IO uint32_t uwVolume = 70;

////////////////////////////////////////////////////////////////////////////////
AUDIO_PLAYBACK_StateTypeDef AudioState=AUDIO_STATE_IDLE;
WAVE_FormatTypeDef WaveFormat;
////////////////////////////////////////////////////////////////////////////////
/* Private function prototypes -----------------------------------------------*/
void BSP_AUDIO_OUT_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t AudioFreq, void *Params);
 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////  
 AUDIO_ErrorTypeDef Play_WAV(FIL *WavFile,uint32_t SR,uint32_t uwVolume)/////////////////////////// WAV PLAY  
 {
      uint8_t buff[AUDIO_OUT_BUFFER_SIZE];
    //uint32_t prev_elapsed_time = 0xFFFFFFFF;
      uint32_t bytesread;
    //uint32_t elapsed_time;
    //uint8_t str[10];
 
 
    f_lseek(WavFile, 0);
    
    //Fill whole buffer at first time 
    if(f_read(WavFile,&buff[0], AUDIO_OUT_BUFFER_SIZE,(void *)&bytesread) == FR_OK)
    {
          BSP_AUDIO_OUT_Play((uint16_t*)&buff[0], AUDIO_OUT_BUFFER_SIZE);
          BufferCtl.fptr = bytesread;
          AudioState=AUDIO_STATE_PLAY; // Zaradi Call Back 
          
    while(bytesread)// AudioState != AUDIO_STATE_NEXT)
    {
    if(BufferCtl.fptr >= WavFile->fsize)
    {
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
     AudioState = AUDIO_STATE_NEXT;
    
    }
    
    
    if(BufferCtl.state == BUFFER_OFFSET_HALF)
    {
      if(f_read(WavFile,&buff[0],AUDIO_OUT_BUFFER_SIZE/2,(void *)&bytesread) != FR_OK)
      { 
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW); 
        return AUDIO_ERROR_IO;       
      } 
      BufferCtl.state = BUFFER_OFFSET_NONE;
      BufferCtl.fptr += bytesread; 
    }
    
    if(BufferCtl.state == BUFFER_OFFSET_FULL)
    {
      if(f_read(WavFile, &buff[AUDIO_OUT_BUFFER_SIZE /2],AUDIO_OUT_BUFFER_SIZE/2,(void *)&bytesread) != FR_OK)
      { 
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW); 
        return AUDIO_ERROR_IO;       
      } 
///////////////////////////////////////////////////TUK 
      BufferCtl.state = BUFFER_OFFSET_NONE;
      BufferCtl.fptr += bytesread; 
     }
 /* 
   // Display elapsed time  
    elapsed_time = BufferCtl.fptr / WaveFormat.ByteRate; 
    if(prev_elapsed_time != elapsed_time)
    {
      prev_elapsed_time = elapsed_time;
      s((char *)str, "[%02d:%02d]", (int)(elapsed_time /60), (int)(elapsed_time%60));
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN); 
      BSP_LCD_DisplayStringAt(263, LINE(8), str, LEFT_MODE);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    }
    */
    }//while
    }else   return AUDIO_ERROR_IO;
 return AUDIO_ERROR_NONE;
 }
 
    /**
  * @brief  Initializes the Wave player.
  * @param  AudioFreq: Audio sampling frequency
  * @retval None
  */
uint8_t PlayerInit(uint32_t AudioFreq)
{ 
  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */  
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, uwVolume, AudioFreq) != 0)
  {
    return 1;
  }
  else
  {
    BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
    return 0;
  } 
}
 
 
/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{ 
  
    if(AudioState == AUDIO_STATE_PLAY)
  {
  
    BufferCtl.state = BUFFER_OFFSET_HALF;
  }
}
/**
  * @brief  Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
      if(AudioState == AUDIO_STATE_PLAY)
  {
    BufferCtl.state = BUFFER_OFFSET_FULL;
  }
}


/**
  * @brief  Clock Config.
  * @param  hsai: might be required to set audio peripheral predivider if any.
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @note   This API is called by BSP_AUDIO_OUT_Init() and BSP_AUDIO_OUT_SetFrequency()
  *         Being __weak it can be overwritten by the application     
  * @retval None
  */
void BSP_AUDIO_OUT_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t AudioFreq, void *Params)
{
  RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;

  HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);
  
  /* Set the PLL configuration according to the audio frequency */
  if((AudioFreq == AUDIO_FREQUENCY_11K) || (AudioFreq == AUDIO_FREQUENCY_22K) || (AudioFreq == AUDIO_FREQUENCY_44K))
  {
    /* Configure PLLSAI prescalers */
    /* PLLI2S_VCO: VCO_429M
    SAI_CLK(first level) = PLLI2S_VCO/PLLSAIQ = 429/2 = 214.5 Mhz
    SAI_CLK_x = SAI_CLK(first level)/PLLI2SDivQ = 214.5/19 = 11.289 Mhz */
    RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
    RCC_ExCLKInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLI2S;
    RCC_ExCLKInitStruct.PLLI2S.PLLI2SP = 8;
    RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = 429;
    RCC_ExCLKInitStruct.PLLI2S.PLLI2SQ = 2;
    RCC_ExCLKInitStruct.PLLI2SDivQ = 19;
    HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);
  }
  else /* AUDIO_FREQUENCY_8K, AUDIO_FREQUENCY_16K, AUDIO_FREQUENCY_48K), AUDIO_FREQUENCY_96K */
  {
    /* SAI clock config
    PLLI2S_VCO: VCO_344M
    SAI_CLK(first level) = PLLI2S_VCO/PLLSAIQ = 344/7 = 49.142 Mhz
    SAI_CLK_x = SAI_CLK(first level)/PLLI2SDivQ = 49.142/1 = 49.142 Mhz */
    RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
    RCC_ExCLKInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLI2S;
//  RCC_ExCLKInitStruct.PLLI2S.PLLI2SP = 8;
    RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = 344;
    RCC_ExCLKInitStruct.PLLI2S.PLLI2SQ = 7;
    RCC_ExCLKInitStruct.PLLI2SDivQ = 1;
    HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);
  }
}

     
 