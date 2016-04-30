
 #define PA_ODR $005000
 #define PA_IDR $005001
 #define PA_DDR $005002
 #define PA_CR1 $005003
 #define PA_CR2 $005004

 #define PB_ODR $005005
 #define PB_IDR $005006
 #define PB_DDR $005007
 #define PB_CR1 $005008
 #define PB_CR2 $005009

 #define PC_ODR $00500A
 #define PC_IDR $00500B
 #define PC_DDR $00500C
 #define PC_CR1 $00500D
 #define PC_CR2 $00500E

 #define PD_ODR $00500F
 #define PD_IDR $005010
 #define PD_DDR $005011
 #define PD_CR1 $005012
 #define PD_CR2 $005013

 #define PE_ODR $005014
 #define PE_IDR $005015
 #define PE_DDR $005016
 #define PE_CR1 $005017
 #define PE_CR2 $005018

 #define PF_ODR $005019
 #define PF_IDR $00501A
 #define PF_DDR $00501B
 #define PF_CR1 $00501C
 #define PF_CR2 $00501D


 #define FLASH_CR1 $00505A
 #define FLASH_CR2 $00505B
 #define FLASH_NCR2 $00505C
 #define FLASH_FPR $00505D
 #define FLASH_NFPR $00505E
 #define FLASH_IAPSR $00505F
 #define FLASH_PUKR $005062
 #define FLASH_DUKR $005064


 #define EXTI_CR1 $0050A0
 #define EXTI_CR2 $0050A1


 #define RST_SR $0050B3


 #define CLK_ICKR $0050C0
 #define CLK_ECKR $0050C1
 #define CLK_CMSR $0050C3
 #define CLK_SWR $0050C4
 #define CLK_SWCR $0050C5
 #define CLK_CKDIVR $0050C6
 #define CLK_PCKENR1 $0050C7
 #define CLK_CSSR $0050C8
 #define CLK_CCOR $0050C9
 #define CLK_PCKENR2 $0050CA
 #define CLK_HSITRIMR $0050CC
 #define CLK_SWIMCCR $0050CD


 #define WWDG_CR $0050D1
 #define WWDG_WR $0050D2
 #define IWDG_KR $0050E0
 #define IWDG_PR $0050E1
 #define IWDG_RLR $0050E2


 #define AWU_CSR1 $0050F0
 #define AWU_APR $0050F1
 #define AWU_TBR $0050F2


 #define BEEP_CSR $0050F3


 #define SPI_CR1 $005200
 #define SPI_CR2 $005201
 #define SPI_ICR $005202
 #define SPI_SR $005203
 #define SPI_DR $005204
 #define SPI_CRCPR $005205
 #define SPI_RXCRCR $005206
 #define SPI_TXCRCR $005207


 #define I2C_CR1 $005210
 #define I2C_CR2 $005211
 #define I2C_FREQR $005212
 #define I2C_OARL $005213
 #define I2C_OARH $005214
 #define I2C_DR $005216
 #define I2C_SR1 $005217
 #define I2C_SR2 $005218
 #define I2C_SR3 $005219
 #define I2C_ITR $00521A
 #define I2C_CCRL $00521B
 #define I2C_CCRH $00521C
 #define I2C_TRISER $00521D
 #define I2C_PECR $00521E


 #define UART1_SR $005230
 #define UART1_DR $005231
 #define UART1_BRR1 $005232
 #define UART1_BRR2 $005233
 #define UART1_CR1 $005234
 #define UART1_CR2 $005235
 #define UART1_CR3 $005236
 #define UART1_CR4 $005237
 #define UART1_CR5 $005238
 #define UART1_GTR $005239
 #define UART1_PSCR $00523A


 #define TIM1_CR1 $005250
 #define TIM1_CR2 $005251
 #define TIM1_SMCR $005252
 #define TIM1_ETR $005253
 #define TIM1_IER $005254
 #define TIM1_SR1 $005255
 #define TIM1_SR2 $005256
 #define TIM1_EGR $005257
 #define TIM1_CCMR1 $005258
 #define TIM1_CCMR2 $005259
 #define TIM1_CCMR3 $00525A
 #define TIM1_CCMR4 $00525B
 #define TIM1_CCER1 $00525C
 #define TIM1_CCER2 $00525D
 #define TIM1_CNTRH $00525E
 #define TIM1_CNTRL $00525F
 #define TIM1_PSCRH $005260
 #define TIM1_PSCRL $005261
 #define TIM1_ARRH $005262
 #define TIM1_ARRL $005263
 #define TIM1_RCR $005264
 #define TIM1_CCR1H $005265
 #define TIM1_CCR1L $005266
 #define TIM1_CCR2H $005267
 #define TIM1_CCR2L $005268
 #define TIM1_CCR3H $005269
 #define TIM1_CCR3L $00526A
 #define TIM1_CCR4H $00526B
 #define TIM1_CCR4L $00526C
 #define TIM1_BKR $00526D
 #define TIM1_DTR $00526E
 #define TIM1_OISR $00526F
 #define TIM2_CR1 $005300
 #define TIM2_IER $005303
 #define TIM2_SR1 $005304
 #define TIM2_SR2 $005305
 #define TIM2_EGR $005306
 #define TIM2_CCMR1 $005307
 #define TIM2_CCMR2 $005308
 #define TIM2_CCMR3 $005309
 #define TIM2_CCER1 $00530A
 #define TIM2_CCER2 $00530B
 #define TIM2_CNTRH $00530C
 #define TIM2_CNTRL $00530D
 #define TIM2_PSCR $00530E
 #define TIM2_ARRH $00530F
 #define TIM2_ARRL $005310
 #define TIM2_CCR1H $005311
 #define TIM2_CCR1L $005312
 #define TIM2_CCR2H $005313
 #define TIM2_CCR2L $005314
 #define TIM2_CCR3H $005315
 #define TIM2_CCR3L $005316
 #define TIM4_CR1 $005340
 #define TIM4_IER $005343
 #define TIM4_SR $005344
 #define TIM4_EGR $005345
 #define TIM4_CNTR $005346
 #define TIM4_PSCR $005347
 #define TIM4_ARR $005348


 #define ADC_DBxR ((volatile uint8_t*)$0053E0)
 #define ADC_CSR $005400
 #define ADC_CR1 $005401
 #define ADC_CR2 $005402
 #define ADC_CR3 $005403
 #define ADC_DRH $005404
 #define ADC_DRL $005405
 #define ADC_TDRH $005406
 #define ADC_TDRL $005407
 #define ADC_HTRH $005408
 #define ADC_HTRL $005409
 #define ADC_LTRH $00540A
 #define ADC_LTRL $00540B
 #define ADC_AWSRH $00540C
 #define ADC_AWSRL $00540D
 #define ADC_AWCRH $00540E
 #define ADC_AWCRL $00540F


 #define A $007F00
 #define PCE $007F01
 #define PCH $007F02
 #define PCL $007F03
 #define XH $007F04
 #define XL $007F05
 #define YH $007F06
 #define YL $007F07
 #define SPH $007F08
 #define SPL $007F09
 #define CCR $007F0A
 #define CFG_GCR $007F60


 #define ITC_SPR1 $007F70
 #define ITC_SPR2 $007F71
 #define ITC_SPR3 $007F72
 #define ITC_SPR4 $007F73
 #define ITC_SPR5 $007F74
 #define ITC_SPR6 $007F75
 #define ITC_SPR7 $007F76
 #define ITC_SPR8 $007F77


 #define SWIM_CSR $007F80
 #define DM_BK1RE $007F90
 #define DM_BK1RH $007F91
 #define DM_BK1RL $007F92
 #define DM_BK2RE $007F93
 #define DM_BK2RH $007F94
 #define DM_BK2RL $007F95
 #define DM_CR1 $007F96
 #define DM_CR2 $007F97
 #define DM_CSR1 $007F98
 #define DM_CSR2 $007F99
 #define DM_ENFCTR $007F9A