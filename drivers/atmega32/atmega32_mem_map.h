#ifndef MEMMAP_H_
#define MEMMAP_H_

/*========================================================
 *                      DIO
 *========================================================*/
#define PORTA   (*(volatile unsigned char*)0x3B)
#define DDRA    (*(volatile unsigned char*)0x3A)
#define PINA    (*(volatile unsigned char*)0x39)

#define PORTB   (*(volatile unsigned char*)0x38)
#define DDRB    (*(volatile unsigned char*)0x37)
#define PINB    (*(volatile unsigned char*)0x36)

#define PORTC   (*(volatile unsigned char*)0x35)
#define DDRC    (*(volatile unsigned char*)0x34)
#define PINC    (*(volatile unsigned char*)0x33)

#define PORTD   (*(volatile unsigned char*)0x32)
#define DDRD    (*(volatile unsigned char*)0x31)
#define PIND    (*(volatile unsigned char*)0x30)


/*========================================================
 *                      ADC
 *========================================================*/
#define ADC     (*(volatile unsigned short*)0x24)
#define ADCL    (*(volatile unsigned char*)0x24)
#define ADCH    (*(volatile unsigned char*)0x25)

#define ADCSRA  (*(volatile unsigned char*)0x26)
#define ADPS0   0
#define ADPS1   1
#define ADPS2   2
#define ADIE    3
#define ADIF    4
#define ADATE   5
#define ADSC    6
#define ADEN    7

#define ADMUX   (*(volatile unsigned char*)0x27)
#define MUX0    0
#define MUX1    1
#define MUX2    2
#define MUX3    3
#define MUX4    4
#define ADLAR   5
#define REFS0   6
#define REFS1   7


/*========================================================
 *                 INTERRUPT VECTORS
 *========================================================*/
#define INT0_vect           __vector_1
#define INT1_vect           __vector_2
#define INT2_vect           __vector_3
#define TIMER2_COMP_vect    __vector_4
#define TIMER2_OVF_vect     __vector_5
#define TIMER1_CAPT_vect    __vector_6
#define TIMER1_COMPA_vect   __vector_7
#define TIMER1_COMPB_vect   __vector_8
#define TIMER1_OVF_vect     __vector_9
#define TIMER0_COMP_vect    __vector_10
#define TIMER0_OVF_vect     __vector_11
#define SPI_STC_vect        __vector_12
#define USART_RXC_vect      __vector_13
#define USART_UDRE_vect     __vector_14
#define USART_TXC_vect      __vector_15
#define ADC_vect            __vector_16
#define EE_RDY_vect         __vector_17
#define ANA_COMP_vect       __vector_18
#define TWI_vect            __vector_19
#define SPM_RDY_vect        __vector_20


/*========================================================
 *               INTERRUPT CONTROL
 *========================================================*/
#define sei()   __asm__ __volatile__ ("sei" ::)
#define cli()   __asm__ __volatile__ ("cli" ::)

#define ISR(vector)                 \
void vector (void) __attribute__ ((signal)); \
void vector (void)


/*========================================================
 *                      MCU
 *========================================================*/
#define MCUCR   (*(volatile unsigned char*)0x55)
#define ISC00   0
#define ISC01   1
#define ISC10   2
#define ISC11   3
#define SM0     4
#define SM1     5
#define SM2     6
#define SE      7

#define MCUCSR  (*(volatile unsigned char*)0x54)
#define PORF    0
#define EXTRF   1
#define BORF    2
#define WDRF    3
#define JTRF    4
#define ISC2    6
#define JTD     7

#define GICR    (*(volatile unsigned char*)0x5B)
#define IVCE    0
#define IVSEL   1
#define INT2    5
#define INT0    6
#define INT1    7

#define GIFR    (*(volatile unsigned char*)0x5A)
#define INTF2   5
#define INTF0   6
#define INTF1   7

#define SPMCR   (*(volatile unsigned char*)0x57)
#define SPMEN   0

#define SREG    (*(volatile unsigned char*)0x5F)


/*========================================================
 *                     EEPROM
 *========================================================*/
#define EEAR    (*(volatile unsigned short*)0x3E)
#define EEARL   (*(volatile unsigned short*)0x3E)
#define EEARH   (*(volatile unsigned short*)0x3F)

#define EEDR    (*(volatile unsigned char*)0x3D)

#define EECR    (*(volatile unsigned char*)0x3C)
#define EERIE   0
#define EEMWE   1
#define EEWE    2
#define EERE    3


/*========================================================
 *                     TIMERS
 *========================================================*/
/* Timer 0 */
#define TCNT0   (*(volatile unsigned char*)0x52)
#define TCCR0   (*(volatile unsigned char*)0x53)

#define CS00    0
#define CS01    1
#define CS02    2
#define WGM01   3
#define COM00   4
#define COM01   5
#define WGM00   6
#define FOC0    7

#define OCR0    (*(volatile unsigned char*)0x5C)

/* Timer Interrupt Mask */
#define TIMSK   (*(volatile unsigned char*)0x59)
#define OCIE2   7
#define TOIE2   6
#define TICIE1  5
#define OCIE1A  4
#define OCIE1B  3
#define TOIE1   2
#define OCIE0   1
#define TOIE0   0

/* Timer 1 */
#define TCCR1A  (*(volatile unsigned char*)0x4F)
#define COM1A1  7
#define COM1A0  6
#define COM1B1  5
#define COM1B0  4
#define FOC1A   3
#define FOC1B   2
#define WGM11   1
#define WGM10   0

#define TCCR1B  (*(volatile unsigned char*)0x4E)
#define ICNC1   7
#define ICES1   6
#define WGM13   4
#define WGM12   3
#define CS12    2
#define CS11    1
#define CS10    0

#define TCNT1   (*(volatile unsigned short*)0x4C)
#define TCNT1L  (*(volatile unsigned char*)0x4C)
#define TCNT1H  (*(volatile unsigned char*)0x4D)

#define OCR1A   (*(volatile unsigned short*)0x4A)
#define OCR1AL  (*(volatile unsigned char*)0x4A)
#define OCR1AH  (*(volatile unsigned char*)0x4B)

#define OCR1BL  (*(volatile unsigned char*)0x48)
#define OCR1BH  (*(volatile unsigned char*)0x49)

#define ICR1    (*(volatile unsigned short*)0x46)
#define ICR1L   (*(volatile unsigned char*)0x46)
#define ICR1H   (*(volatile unsigned char*)0x47)

/* Timer 2 */
#define TCCR2   (*(volatile unsigned char*)0x45)
#define FOC2    7
#define WGM20   6
#define COM21   5
#define COM20   4
#define WGM21   3
#define CS22    2
#define CS21    1
#define CS20    0

#define TCNT2   (*(volatile unsigned char*)0x44)
#define OCR2    (*(volatile unsigned char*)0x43)


/*========================================================
 *                     UART
 *========================================================*/
#define UCSRA   (*(volatile unsigned char*)0x2B)
#define RXC     7
#define TXC     6
#define UDRE    5
#define FE      4
#define DOR     3
#define PE      2
#define U2X     1
#define MPCM    0

#define UCSRB   (*(volatile unsigned char*)0x2A)
#define RXCIE   7
#define TXCIE   6
#define UDRIE   5
#define RXEN    4
#define TXEN    3
#define UCSZ2   2
#define RXB8    1
#define TXB8    0

#define UCSRC   (*(volatile unsigned char*)0x40)
#define URSEL   7
#define UMSEL   6
#define UPM1    5
#define UPM0    4
#define USBS    3
#define UCSZ1   2
#define UCSZ0   1
#define UCPOL   0

#define UBRRH   (*(volatile unsigned char*)0x40)
#define UBRRL   (*(volatile unsigned char*)0x29)
#define UDR     (*(volatile unsigned char*)0x2C)


/*========================================================
 *                     SPI
 *========================================================*/
#define SPCR    (*(volatile unsigned char*)0x2D)
#define SPR0    0
#define SPR1    1
#define CPHA    2
#define CPOL    3
#define MSTR    4
#define DORD    5
#define SPE     6
#define SPIE    7

#define SPSR    (*(volatile unsigned char*)0x2E)
#define SPI2X   0
#define WCOL    6
#define SPIF    7

#define SPDR    (*(volatile unsigned char*)0x2F)


/*========================================================
 *                     TWI (I2C)
 *========================================================*/
#define TWCR    (*(volatile unsigned char*)0x56)
#define TWINT   7
#define TWEA    6
#define TWSTA   5
#define TWSTO   4
#define TWWC    3
#define TWEN    2
#define TWIE    0

#define TWSR    (*(volatile unsigned char*)0x21)
#define TWS7    7
#define TWS6    6
#define TWS5    5
#define TWS4    4
#define TWS3    3
#define TWPS1   1
#define TWPS0   0

#define TWDR    (*(volatile unsigned char*)0x20)
#define TWAR    (*(volatile unsigned char*)0x22)
#define TWBR    (*(volatile unsigned char*)0xB8)

#define TWA0    1
#define TWA1    2
#define TWA2    3
#define TWA3    4
#define TWA4    5
#define TWA5    6
#define TWA6    7
#define TWGCE   0

#endif /* MEMMAP_H_ */