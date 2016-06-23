USE MENU

#ifdef USE_MENU

void mjpegTouch(int id, uint32_t pointed_chunk) // ? ? ? ? ?  ?  ? ?  ? ?        Touch pen interrupt processing
{
	uint32_t firstChunk, prevChunk, prevSamples, samples, totalSamples = 0, jFrameOffset;
	MY_FILE fp_stsc, fp_stsz, fp_stco, fp_frame, fp_frame_cp;
	uint8_t atombuf[12];

	raw_video_typedef raw;

	memcpy((void*)&fp_stsc, (void*)&video_stsc.fp, sizeof(MY_FILE));
	memcpy((void*)&fp_stsz, (void*)&video_stsz.fp, sizeof(MY_FILE));
	memcpy((void*)&fp_stco, (void*)&video_stco.fp, sizeof(MY_FILE));
	//DANI memcpy((void*)&fp_frame, (void*)&fp_global, sizeof(MY_FILE));

//		debug.printf("\r\npointed_chunk:%d video_stco.numEntry:%d", pointed_chunk, video_stco.numEntry);

	prevChunk = getSampleSize(atombuf, 12, &fp_stsc); // firstChunk samplesPerChunk sampleDescriptionID ????firstChunk?prevChunk?
	prevSamples = getAtomSize(&atombuf[4]); // ????samplesPerChunk?prevSamples?
	firstChunk = getSampleSize(atombuf, 4, &fp_stsc); // ????firstChunk

	while(1){
		if(prevChunk <= pointed_chunk && firstChunk >= pointed_chunk){
			samples = (firstChunk - pointed_chunk) * prevSamples;
			totalSamples += (pointed_chunk - prevChunk) * prevSamples;
			break;
		}
		samples = (firstChunk - prevChunk) * prevSamples;
		totalSamples += samples;

		prevChunk = firstChunk; // ???firstChunk?prevChunk?
		prevSamples = getSampleSize(atombuf, 8, &fp_stsc); // samplesPerChunk sampleDescriptionID
		firstChunk = getSampleSize(atombuf, 4, &fp_stsc); // ??firstChunk
	}

	my_fseek(&fp_stco, (pointed_chunk - 1) * 4, SEEK_CUR);
	jFrameOffset = getSampleSize(atombuf, 4, &fp_stco);

	memcpy((void*)&fp_stco, (void*)&video_stco.fp, sizeof(MY_FILE));
	my_fseek(&fp_stco, (pointed_chunk - 1) * 4, SEEK_CUR); //

	my_fseek(&fp_frame, jFrameOffset, SEEK_SET);

	memcpy((void*)&fp_frame_cp, (void*)&fp_frame, sizeof(MY_FILE));
	//DANI  my_fseek(&fp_frame, raw.frame_size, SEEK_CUR);

	*pv_src.firstChunk = firstChunk;
	*pv_src.prevChunk = prevChunk;
	*pv_src.prevSamples = prevSamples;
	*pv_src.samples = samples;
	*pv_src.totalSamples = totalSamples;
	*pv_src.videoStcoCount = pointed_chunk + 1;

	memcpy((void*)pv_src.fp_video_stsc, (void*)&fp_stsc, sizeof(MY_FILE));
	memcpy((void*)pv_src.fp_video_stsz, (void*)&fp_stsz, sizeof(MY_FILE));
	memcpy((void*)pv_src.fp_video_stco, (void*)&fp_stco, sizeof(MY_FILE));
	memcpy((void*)pv_src.fp_frame, (void*)&fp_frame, sizeof(MY_FILE));


	// Sound
	memcpy((void*)&fp_stsc, (void*)&sound_stsc.fp, sizeof(MY_FILE));
	memcpy((void*)&fp_stsz, (void*)&sound_stsz.fp, sizeof(MY_FILE));
	memcpy((void*)&fp_stco, (void*)&sound_stco.fp, sizeof(MY_FILE));
	//DANI memcpy((void*)&fp_frame, (void*)&fp_global, sizeof(MY_FILE));

//		debug.printf("\r\npointed_chunk:%d video_stco.numEntry:%d", pointed_chunk, video_stco.numEntry);

	prevChunk = getSampleSize(atombuf, 12, &fp_stsc); // firstChunk samplesPerChunk sampleDescriptionID ????firstChunk?prevChunk?
	prevSamples = getAtomSize(&atombuf[4]); // ????samplesPerChunk?prevSamples?
	firstChunk = getSampleSize(atombuf, 4, &fp_stsc); // ????firstChunk

	totalSamples = 0;
	while(1){
		if(prevChunk <= pointed_chunk && firstChunk >= pointed_chunk){
			samples = (firstChunk - pointed_chunk) * prevSamples;
			totalSamples += (pointed_chunk - prevChunk) * prevSamples;
			break;
		}
		samples = (firstChunk - prevChunk) * prevSamples;
		totalSamples += samples;

		prevChunk = firstChunk; // ???firstChunk?prevChunk?
		prevSamples = getSampleSize(atombuf, 8, &fp_stsc); // samplesPerChunk sampleDescriptionID
		firstChunk = getSampleSize(atombuf, 4, &fp_stsc); // ??firstChunk
	}
	my_fseek(&fp_stco, (pointed_chunk - 1) * 4, SEEK_CUR);

	*ps_src.firstChunk = firstChunk;
	*ps_src.prevChunk = prevChunk;
	*ps_src.prevSamples = prevSamples;
	*ps_src.samples = samples;
	*ps_src.soundStcoCount = pointed_chunk;

	memcpy((void*)ps_src.fp_sound_stsc, (void*)&fp_stsc, sizeof(MY_FILE));
	memcpy((void*)ps_src.fp_sound_stsz, (void*)&fp_stsz, sizeof(MY_FILE));
	memcpy((void*)ps_src.fp_sound_stco, (void*)&fp_stco, sizeof(MY_FILE));

	if(mjpeg_touch.resynch){
		return;
	}
        
        
/* //DANI
	if(media.video.width != LCD_WIDTH || media.video.height != LCD_HEIGHT){
		LCD_DrawSquare(0, 0, LCD_WIDTH, media.video.startPosY, BLACK);
		LCD_DrawSquare(0, media.video.startPosY, (LCD_WIDTH - media.video.width) / 2, media.video.height, BLACK);
		LCD_DrawSquare(media.video.startPosX + media.video.width, media.video.startPosY, (LCD_WIDTH - media.video.width) / 2, media.video.height, BLACK);
		LCD_DrawSquare(0, media.video.startPosY + media.video.height, LCD_WIDTH, LCD_HEIGHT - (media.video.startPosY + media.video.height), BLACK);
	}
*/
	int v;
	for(v = 0;v < media.video.height;v++)
        {
		//(&frame_buffer[media.video.startPosX + v * LCD_WIDTH + media.video.startPosY * LCD_WIDTH], 2, media.video.width, &fp_frame_cp);
               // read_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf)
	}


	int curX, prevX, duration = (int)((float)media.video.duration / (float)media.video.timeScale + 0.5f);
	int time = duration * (float)*pv_src.videoStcoCount / (float)video_stco.numEntry;
	char timeStr[20];

	LCDPutIcon(4, 100, 12, 12, pause_icon_12x12, pause_icon_12x12_alpha);
	LCDPutIcon(UI_POS_X, UI_POS_Y, 120, 14, seekbar_120x14, seekbar_120x14_alpha);

	drawBuff->navigation_loop.x = 140;
	drawBuff->navigation_loop.y = UI_POS_Y;
	drawBuff->navigation_loop.width = 18;
	drawBuff->navigation_loop.height = 14;
	LCDStoreBgImgToBuff(drawBuff->navigation_loop.x, drawBuff->navigation_loop.y, \
						drawBuff->navigation_loop.width, drawBuff->navigation_loop.height, drawBuff->navigation_loop.p);

	Update_Navigation_Loop_Icon(drawBuff, music_control.b.navigation_loop_mode);

	drawBuff->posision.width = 12;
	drawBuff->posision.height = 12;
	drawBuff->posision.x = UI_POS_X + 1;
	drawBuff->posision.y = UI_POS_Y + 1;
	prevX = UI_POS_X + 1;
	LCDStoreBgImgToBuff(prevX, drawBuff->posision.y, \
						drawBuff->posision.width, drawBuff->posision.height, drawBuff->posision.p);

	DRAW_SEEK_CIRCLE((float)time / (float)duration , seek_active_circle_12x12);

	pcf_typedef pcf;
	pcf.dst_gram_addr = (uint32_t)frame_buffer;
	pcf.pixelFormat = PCF_PIXEL_FORMAT_RGB565;
	pcf.size = 12;
	pcf.color = WHITE;
	pcf.colorShadow = GRAY;
	pcf.alphaSoftBlending = 1;
	pcf.enableShadow = 1;
	pcf_font.metrics.hSpacing = 2;

	char s[10];
	SPRINTF(s, "%d/%d", id, fat.fileCnt - 1);
	LCD_GotoXY(5, MUSIC_INFO_POS_Y + 1);
	LCDPutString(s, &pcf);

	DRAW_MOV_TIME_STR();

	DRAW_MOV_REMAIN_TIME_STR();

	LCD_FRAME_BUFFER_Transmit(LCD_DMA_TRANSMIT_BLOCKING);
}


extern I2S_HandleTypeDef haudio_i2s;


int mjpegPause(int id)
{
	int seekBytes, media_data_totalBytes, diff;
	int ret = RET_PLAY_NORM, curX, prevX = 0;
	int duration = media.video.duration / media.video.timeScale;
	int swHoldCnt, media_data_denom;
	int time = duration * (float)((float)*pv_src.videoStcoCount / (float)video_stco.numEntry);
	char timeStr[20];

	LCD_SetRegion(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

	pcf_typedef pcf;
	pcf.dst_gram_addr = (uint32_t)frame_buffer;
	pcf.pixelFormat = PCF_PIXEL_FORMAT_RGB565;
	pcf.size = 12;
	pcf.color = WHITE;
	pcf.colorShadow = GRAY;
	pcf.alphaSoftBlending = 1;
	pcf.enableShadow = 1;
	pcf_font.metrics.hSpacing = 2;

	drawBuff = (drawBuff_typedef*)cursorRAM;

	char s[10];
	SPRINTF(s, "%d/%d", id, fat.fileCnt - 1);
	LCD_GotoXY(5, MUSIC_INFO_POS_Y + 1);
	LCDPutString(s, &pcf);

	mjpeg_touch.resynch = 1;
	mjpegTouch(id, *pv_src.videoStcoCount);
	mjpeg_touch.resynch = 0;

	LCDPutIcon(4, 100, 12, 12, pause_icon_12x12, pause_icon_12x12_alpha);
	LCDPutIcon(UI_POS_X, UI_POS_Y, 120, 14, seekbar_120x14, seekbar_120x14_alpha);

	drawBuff->navigation_loop.x = 140;
	drawBuff->navigation_loop.y = UI_POS_Y;
	drawBuff->navigation_loop.width = 18;
	drawBuff->navigation_loop.height = 14;
	LCDStoreBgImgToBuff(drawBuff->navigation_loop.x, drawBuff->navigation_loop.y, \
						drawBuff->navigation_loop.width, drawBuff->navigation_loop.height, drawBuff->navigation_loop.p);

	Update_Navigation_Loop_Icon(drawBuff, music_control.b.navigation_loop_mode);

	drawBuff->posision.width = 12;
	drawBuff->posision.height = 12;
	drawBuff->posision.x = UI_POS_X + 1;
	drawBuff->posision.y = UI_POS_Y + 1;
	prevX = UI_POS_X + 1;
	LCDStoreBgImgToBuff(prevX, drawBuff->posision.y, \
						drawBuff->posision.width, drawBuff->posision.height, drawBuff->posision.p);

	DRAW_SEEK_CIRCLE((float)time / (float)duration , seek_circle_12x12);

	DRAW_MOV_TIME_STR();

	DRAW_MOV_REMAIN_TIME_STR();

	LCD_FRAME_BUFFER_Transmit(LCD_DMA_TRANSMIT_BLOCKING);

	int pause_flag = 0;
	if(LCDStatusStruct.waitExitKey == PLAY_PAUSE || LCDStatusStruct.waitExitKey == PLAY_LOOP_MODE)
	{
		pause_flag = 1;
		LCDStatusStruct.waitExitKey = 1;
	}
extern int8_t vol;
	while(1){
		switch(LCDStatusStruct.waitExitKey)
		{
		case 0:
			return RET_PLAY_STOP;
			break;
		case VOL_UP:
			vol = ++vol < 6 ? vol : 6;
//			wm8731_left_headphone_volume_set(121 + vol);
			LCDStatusStruct.waitExitKey = 1;
			break;
		case VOL_DOWN:
			vol = --vol > -121 ? vol : -121;
//			wm8731_left_headphone_volume_set(121 + vol);
			LCDStatusStruct.waitExitKey = 1;
			break;
		case PLAY_PAUSE:
			ret = 1;
			LCDStatusStruct.waitExitKey = 1;
			goto PAUSE_EXIT;
			break;
		case PLAY_LOOP_MODE:
			Update_Navigation_Loop_Icon(drawBuff, music_control.b.navigation_loop_mode = ++music_control.b.navigation_loop_mode % 5);
			LCD_FRAME_BUFFER_Transmit(LCD_DMA_TRANSMIT_NOBLOCKING);
			LCDStatusStruct.waitExitKey = 1;
			break;
		case PLAY_SW_HOLD_LEFT:
			media_data_totalBytes = video_stco.numEntry;
			seekBytes = *pv_src.videoStcoCount;
			media_data_denom = 100;
			swHoldCnt = SW_HOLD_CNT_VAL;
			while(HAL_GPIO_ReadPin(SW_PUSH_LEFT_PORT, SW_PUSH_LEFT_PIN) == GPIO_PIN_RESET){
				diff = seekBytes - media_data_totalBytes / media_data_denom;
				if(diff < 0){
					seekBytes = 2;
				} else {
					seekBytes += -media_data_totalBytes / media_data_denom;
				}

				mjpegTouch(id, seekBytes);

				HAL_Delay(20);
				media_data_denom = 100 / (++swHoldCnt / SW_HOLD_CNT_VAL);
			}
			ret = 1;
			LCDStatusStruct.waitExitKey = 1;
			if(!pause_flag){
				goto PAUSE_EXIT;
			}
			break;
		case PLAY_SW_HOLD_RIGHT:
			media_data_totalBytes = video_stco.numEntry;
			seekBytes = *pv_src.videoStcoCount;
		 	media_data_denom = 100;
			swHoldCnt = SW_HOLD_CNT_VAL;
			while(HAL_GPIO_ReadPin(SW_PUSH_RIGHT_PORT, SW_PUSH_RIGHT_PIN) == GPIO_PIN_RESET){
				if((seekBytes + media_data_totalBytes / media_data_denom) > media_data_totalBytes){
					continue;
				}
				seekBytes += media_data_totalBytes / media_data_denom;
				mjpegTouch(id, seekBytes);

	 	 		HAL_Delay(20);
	 	 		media_data_denom = 100 / (++swHoldCnt / SW_HOLD_CNT_VAL);
			}
			ret = 1;
			LCDStatusStruct.waitExitKey = 1;
			if(!pause_flag){
				goto PAUSE_EXIT;
			}
			break;
		case PLAY_NEXT:
			ret = RET_PLAY_NEXT;
			return ret;
		case PLAY_PREV:
			ret = RET_PLAY_PREV;
			return ret;
			break;
		default:
			break;
		}
		HAL_Delay(1);
	}

PAUSE_EXIT:
	if(media.video.width != LCD_WIDTH || media.video.height != LCD_HEIGHT){
		LCD_DrawSquare(0, 0, LCD_WIDTH, media.video.startPosY, BLACK);
		LCD_DrawSquare(0, media.video.startPosY, (LCD_WIDTH - media.video.width) / 2, media.video.height, BLACK);
		LCD_DrawSquare(media.video.startPosX + media.video.width, media.video.startPosY, (LCD_WIDTH - media.video.width) / 2, media.video.height, BLACK);
		LCD_DrawSquare(0, media.video.startPosY + media.video.height, LCD_WIDTH, LCD_HEIGHT - (media.video.startPosY + media.video.height), BLACK);
	}

	LCD_FRAME_BUFFER_Transmit(LCD_DMA_TRANSMIT_BLOCKING); /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	while((SpiLcdHandle.State != HAL_SPI_STATE_READY)){};

	memset(dac_intr.buff, '\0', dac_intr.bufferSize);

	LCD_SetRegion(media.video.startPosX, media.video.startPosY, media.video.startPosX + media.video.width - 1, media.video.startPosY + media.video.height - 1);

	HAL_I2S_DMAResume(&haudio_i2s);
	DMA_SOUND_IT_ENABLE;

    return ret;
}
#endif //USE_MENU