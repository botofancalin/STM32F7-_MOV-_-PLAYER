
#ifndef __MPEG_H
#define __MPEG_H

#include "main.h"
#include "decode.h"

#define CMARK 0xA9

#define LCD_WIDTH 320
#define LCD_HEIGHT 180



#define TIM_1SEC						TIM1
#define TIM_1SEC_CLK_ENABLE()			__TIM1_CLK_ENABLE()
#define TIM_1SEC_IRQn					TIM1_UP_TIM10_IRQn
#define TIM_1SEC_IRQHandler				TIM1_UP_TIM10_IRQHandler


#define TIM_DURATION					TIM3
#define TIM_DURATION_CLK_ENABLE()		__TIM3_CLK_ENABLE()


#define PERIPH_BB_BASE 0
/* --- TIM3 DIER Register ---*/
/* Alias word address of TIM3 DIER UIE bit */
#define TIM3_DIER_OFFSET         (TIM3_BASE - PERIPH_BASE + 0x0C)
#define TIM3_DIER_UIE_BitNumber  0x00
#define TIM3_DIER_UIE_BB         (*(__IO uint32_t *)(PERIPH_BB_BASE + (TIM3_DIER_OFFSET * 32) + (TIM3_DIER_UIE_BitNumber * 4)))

/* --- TIM3 SR Register ---*/
/* Alias word address of TIM3 SR UIF bit */
#define TIM3_SR_OFFSET         (TIM3_BASE - PERIPH_BASE + 0x10)
#define TIM3_SR_UIF_BitNumber  0x00
#define TIM3_SR_UIF_BB	       (*(__IO uint32_t *)(PERIPH_BB_BASE + (TIM3_SR_OFFSET << 5) + (TIM3_SR_UIF_BitNumber << 2)))

/* --- TIM3 CR1 Register ---*/
/* Alias word address of TIM3 CR1 CEN bit */
#define TIM3_CR1_OFFSET         (TIM3_BASE - PERIPH_BASE + 0x00)
#define TIM3_CR1_CEN_BitNumber  0x00
#define TIM3_CR1_CEN_BB         (*(__IO uint32_t *)(PERIPH_BB_BASE + (TIM3_CR1_OFFSET * 32) + (TIM3_CR1_CEN_BitNumber * 4)))



static uint8_t atomHasChild[] = 
{0, 0, 0, 1, 0, 1, 0, 1,0, 0, 0, 1, 0, 1, 0 ,0, 1, 0, 0, 1, 0, 1, 0, 0,0, 0, 0, 1, 0, 0, 1, 0,0, 1, 0, 0, 1, 1, 1, 1,1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#define ATOM_ITEMS (sizeof(atomHasChild) / sizeof(atomHasChild[0]))

static const uint8_t atomTypeString[ATOM_ITEMS][5] =
{
	"ftyp", // -
	"wide", // -
	"mdat", // -
	"moov", // +
	"mvhd", // -
	"trak", // +
	"tkhd", // -
	"tapt", // +
	"clef", // -
	"prof", // -
	"enof", // -
	"edts", // +
	"elst", // -
	"mdia", // +
	"mdhd", // -
	"hdlr", // -
	"minf", // +
	"vmhd", // -
	"smhd", // -
	"dinf", // +
	"dref", // -
	"stbl", // +
        /*The sample table has an atom type of 'stbl'. It contains the sample description atom,
the time-to-sample atom, the sample-to-chunk atom, the sync sample atom, the sample
size atom, the chunk offset atom, and the shadow sync atom.*/
	"stsd", // -  //Sample description atom
	"stts", // -  //Time-to-sample atom
	"stsc", // -  //Sample-to-chunk atom
	"stsz", // -  //Sample size atom
	"stco", // -  //Chunk offset atom 
	"udta", // +
	"free", // -
	"skip", // -
	"meta", // +
	"load", // -
	"iods", // -
	"ilst", // +
	"keys", // -
	"data", // -
	"trkn", // +
	"disk", // +
	"cpil", // +
	"pgap", // +
	"tmpo", // +
	"gnre", // +
	"covr", // -
	{CMARK, 'n', 'a', 'm', '\0'}, // -
	{CMARK, 'A', 'R', 'T', '\0'}, // -
	{CMARK, 'a', 'l', 'b', '\0'}, // -
	{CMARK, 'g', 'e', 'n', '\0'}, // -
	{CMARK, 'd', 'a', 'y', '\0'}, // -
	{CMARK, 't', 'o', 'o', '\0'}, // -
	{CMARK, 'w', 'r', 't', '\0'}, // -
	"----", // -
};

enum AtomEnum {
	FTYP, // -
	WIDE, // -
	MDAT, // -
	MOOV, // +
	MVHD, // -
	TRAK, // +
	TKHD, // -
	TAPT, // +
	CLEF, // -
	PROF, // -
	ENOF, // -
	EDTS, // +
	ELST, // -
	MDIA, // +
	MDHD, // -
	HDLR, // -
	MINF, // +
	VMHD, // -
	SMHD, // -
	DINF, // +
	DREF, // -
	STBL, // +
	STSD, // -
	STTS, // -
	STSC, // -
	STSZ, // -
	STCO, // -
	UDTA, // +
	FREE, // -
	SKIP, // -
	META, // +
	LOAD, // -
	IODS, // -
	ILST, // +
	KEYS, // -
	DATA, // -
	TRKN, // +
	DISK, // +
	CPIL, // +
	PGAP, // +
	TMPO, // +
	GNRE, // +
	COVR, // -
	CNAM, // -
	CART, // -
	CALB, // -
	CGEN, // -
	CDAY, // -
	CTOO, // -
	CWRT, // -
	NONE, // -
};



#define DRAW_MOV_TIME_STR() do{LCD_GotoXY(TIME_X, TIME_Y); \
							   LCDPutString(setStrSec(timeStr, time), &pcf);}while(0)

#define DRAW_MOV_REMAIN_TIME_STR() do{LCD_GotoXY(abs(time - duration) > (99 * 60 + 99) ? TIME_REMAIN_X - 6 : TIME_REMAIN_X, TIME_REMAIN_Y); \
									  LCDPutString(setStrSec(timeStr, time - duration), &pcf);}while(0)


#define PERIPH_GET_BB(periph_offset, bitnumber)  (*(volatile uint32_t *)(PERIPH_BB_BASE + (periph_offset - PERIPH_BASE) << 5) + (bitnumber << 2)))





 uint32_t Play_MOV(FIL *mfile);

#endif /* __MPEG_H */