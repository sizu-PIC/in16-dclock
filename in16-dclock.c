
// PIC16F1829 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), high trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <pic16f1829.h>
#include <stdlib.h>

// Sosc count 0x8000 at 1sec, so set Timer1 count 0x4000 (0.5sec)
// Timer1 count up from 0xBFFF to 0xFFFF
#define T1H 0xBF
#define T1L 0xFF

// define the interval to use delay count
#define INTERVAL 250
#define INT1 120
#define INT2 80
#define INT3 40
#define INT4 20
#define INT5 15
#define INT6 13
#define INT7 11
#define INT8 10
#define INTERVAL_TIME_D 12
#define INTERVAL_DATE 10
#define INTERVAL_D 12
#define INT_CHAT 3
#define LOOP_CUNTR 10

// the number of mode
#define MODE_NUM 12

// define the time of shuffling
#define SHUFFLE_TIME 7

// define the number of nixie-tube
#define NALL 8  // to use 0-7bits

// define the count of chattering
#define CHATCOUNT 30

// define the constant number use to make seed
#define SEED_CONST_T 10
#define SEED_CONST_D 1

// define to count to TIMER1
#define T1INIT 49152    // count up from 49152 to 65536
                        // 1 / (65535 - 49152) * 32768(frequency of crystal oscillator) / 1(pre scaler) = 0.5(sec)
#define T2INIT 0

// define the time adjustment
#define SEC_OF_MIN 6
#define MIN_OF_HOUR 6
#define HOUR_OF_DAY1 2
#define HOUR_OF_DAY2 4

// define the pin assign
// define the pin to connect SN74141 (casode control)
#define PIN_A RC3
#define PIN_B RC7
#define PIN_C RB7
#define PIN_D RC6

// define the pin to connect 74ACT138P (anode control)
#define PIN_1 RB4
#define PIN_2 RC0
#define PIN_3 RC1

// define the pin to connect SN74141 (right dot control)
#define PIN_dot RB5

// define the pin to accecpt signal
// to adjust timer
#define ADJ_SEC RA0
#define ADJ_MIN RA1
#define ADJ_HOUR RA2

// to shutdown the 74ACT138P
#define SWITCH RB6

// define the pin to 
#define MODE1 RC5
#define MODE2 RC4
#define ADJ_MODE RC2

// define DATE and TIME
#define YEAR 2000
#define MONTH 10
#define DAY 1
#define HOUR 12
#define MINUTE 0
#define SECOND 0

struct nixie_status {
    int num;
    int flag;
};

int comma_flag = 0, cross_flag = 0, TD_flag = 0;
struct nixie_status time[NALL], date[NALL];
int *pctg[MODE_NUM];
int dmode1[NALL] = {6, 9, 5, 8, 4, 0, 10, 1};
int dmode2[NALL] = {6, 2, 4, 0, 3, 1, 10, 1};
int dmode3[NALL] = {9, 9, 2, 3, 2, 5, 10, 0};
int dmode4[NALL] = {4, 7, 0, 5, 1, 6, 10, 2};
int dmode5[NALL] = {0, 2, 4, 9, 0, 4, 10, 0};
int dmode6[NALL] = {5, 0, 2, 0, 3, 1, 10, 1};
int dmode7[NALL] = {8, 2, 7, 8, 4, 0, 10, 1};
int dmode8[NALL] = {3, 0, 9, 6, 5, 4, 10, 0};
int dmode9[NALL] = {2, 1, 2, 0, 3, 1, 10, 1};
int dmode10[NALL] = {6, 4, 0, 1, 7, 5, 10, 0};
int dmode11[NALL] = {7, 8, 1, 7, 3, 3, 10, 0};
int dmode12[NALL] = {4, 2, 0, 1, 7, 5, 10, 0};

void interrupt _Timer1Interrupt()
{
     // TIMER1 reformat
    TMR1H = (T1INIT >> 8);      // TIMER1 re-formatting
    TMR1L = (T1INIT & 0x00ff);  // 
    TMR1IF = 0;                 // flag of TIMER1 interruption reset
    
    comma_flag++;
    
    if (comma_flag == 2) {
        time->num++;
        if (time->num > 9) {
            time->num = 0;
            (time + 1)->num++;
            if ((time + 1)->num == SEC_OF_MIN) {
                (time + 1)->num = 0;
                (time + 3)->num++;
                if ((time + 3)->num > 9) {
                    (time + 3)->num = 0;
                    (time + 4)->num++;
                    if ((time + 4)->num == MIN_OF_HOUR) {
                        (time + 4)->num = 0;
                        (time + 6)->num++;
                        if ((time + 6)->num > 9) {
                            (time + 6)->num = 0;
                            (time + 7)->num++;
                        }
                        if (((time + 7)->num == HOUR_OF_DAY2) && ((time + 6)->num == HOUR_OF_DAY1)) {
                            (time + 6)->num = 0;
                            (time + 7)->num = 0;
                            date->num++;
                        }
                    }
                }
            }
        }
        comma_flag = 0;
    } 
}

void reset(int);                                        // to reset the display(nixie-tube)
void d_meter();                                        // to control nixie-tubes to d-meter
void d_static(struct nixie_status *, int *);                                       
void d_random(struct nixie_status *);
void d_manual(struct nixie_status *);
void clock();                                          // to control nixie-tubes to normal clock(hour.minute.second)
void adj_time(struct nixie_status *);                  // adjust the time
void adj_date(struct nixie_status *);                   // adjust the date
void crossfade();                                      // to control the change of nixie num
void nixie_num(int, int);                              // to display number on a nixie-tube
void show_time(struct nixie_status *);                 // to decision the number and bit to display of time
void show_day(struct nixie_status *);                  // to decision the number and bit to display of day
int show_d(struct nixie_status *, int *);
void show_d_static(struct nixie_status *);
void sample1();                                        // do sample program
void sample2();
void sample3();                                        // to check the dinamic lighting
void count_inc();                                      // increase to count
int inc_process(struct nixie_status *);                // do increment process of nixie_status
void count_dec();                                      // decrease to count
int dec_process(struct nixie_status *);                // do decrement process of nixie_status
void disp_num(struct nixie_status *, int);             // display the number with use nixie_status
void counter();                                        // to count the number
void all_shuffle();                                   // to do shuffle the numbet for all nixie tubes
void anode_random(int *);                       // set the order of anode to stop

void main()
{
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    TRISA = 0x07;   // RA0-RA2 to use input and the others to use output
    TRISB = 0x00;   // to use output
    TRISC = 0x34;   // RC2, 5, 4 to use input and the others to use output
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    
    GIE = 1;
    PEIE = 1;
    
    // set T1CON to use Socs(32.768kHz) and prescalar 1:1
    T1CON = 0x8C;
    T1GCON = 0x00;
    TMR1H = T1H;    // set Timer1H
    TMR1L = T1L;    // set Timer1L
    TMR1IF = 0;     // interrupt flag reset
    TMR1IE = 1;     // interrput enable
    TMR1ON = 1;     // start Timer1
    
    // initiate struct nixie_status time and day
    time->num = SECOND % 10;
    (time + 1)->num = SECOND / 10;
    (time + 2)->num = 10;
    (time + 3)->num = MINUTE % 10;
    (time + 4)->num = MINUTE / 10;
    (time + 5)->num = 10;
    (time + 6)->num = HOUR % 10;
    (time + 7)->num = HOUR / 10;
    for (int i = 0; i < NALL; i++)
        (time + i)->flag = 0;
    date->num = DAY % 10;
    (date + 1)->num = DAY / 10;
    (date + 2)->num = MONTH % 10;
    (date + 3)->num = MONTH / 10;
    (date + 4)->num = YEAR % 10;
    (date + 5)->num = YEAR % 100 / 10;
    (date + 6)->num = YEAR % 1000 / 100;
    (date + 7)->num = YEAR / 1000;
    for (int i = 0; i < NALL; i++)
        (date + i)->flag = 0;
    
    // set pctg
    pctg[0] = dmode1; pctg[1] = dmode2; pctg[2] = dmode3; pctg[3] = dmode4; pctg[4] = dmode5;
    pctg[5] = dmode6; pctg[6] = dmode7; pctg[7] = dmode8; pctg[8] = dmode9; pctg[9] = dmode10;
    pctg[10] = dmode11; pctg[11] = dmode12;
    
    // set seed
    int seed = 0;
    for (int i = 0; i < NALL; i++) {
        seed += (time + i)->num * i * SEED_CONST_T;
        seed += (date + i)->num * i * SEED_CONST_D;
    }
    
    srand(seed);
    // to shuffle the random number
    for (int i = 0; i < 5; i++)                             
        rand();
    
    reset(INTERVAL_D);
    // sample1();
    // sample2();
    // sample3();
    
    while (1) {
        MODE1 ? clock() : MODE2 ? d_meter() : counter();
    }
}

void reset(int interval)            // no display
{
    for (int i = 0; i < interval; i++) ;
    SWITCH = 1;
    PIN_dot = 0;
}

void d_meter()
{
    int count = 0, mode = 0;
    struct nixie_status nixie[NALL];
    
    // set nixie num
    for (int i = 0; i < NALL; i++)
        (nixie + i)->num = dmode12[i];
    for (int i = 0; i < NALL; i++)
        (i != 6) ? (nixie + i)->flag = 1 : (nixie + i)->flag = 0;
    
    while (MODE2) {
        // reset the nixie flag
        for (int i = 0; i < NALL; i++)
            (i != 6) ? (nixie + i)->flag = 1 : (nixie + i)->flag = 0;
        
        // initiate
        // set list
        int list[NALL];
        anode_random(list);
        
        // set random number
        int random[NALL];
        for (int i = 0; i < NALL; i++) {
            random[i] = rand() % 10;
        }
        
        // shuffling
        // do shuffle
        for (int loop = 0; loop < SHUFFLE_TIME; loop++)
            show_d(nixie, random);
        
        // follow list
        for (int i = 0; i < NALL; i++) {
            (nixie + list[i])->flag = 0;
            show_d(nixie, random);
        }
        
        // wait input
        while (MODE2 && count < INTERVAL) {
            disp_num(nixie, INTERVAL_D);
            if (ADJ_HOUR || count == INTERVAL - 1) {
                while (ADJ_HOUR && count < CHATCOUNT) {
                    count++;
                    disp_num(nixie, INTERVAL_D);
                }
                count = 0;
                d_static(nixie, &mode);
                break;
            } else if (ADJ_MIN) {
                while (ADJ_MIN && count < CHATCOUNT) {
                    count++;
                    disp_num(nixie, INTERVAL_D);
                }
                count = 0;
                d_random(nixie);
                break;
            } else if (ADJ_SEC) {
                while (ADJ_SEC && count < CHATCOUNT) {
                    count++;
                    disp_num(nixie, INTERVAL_D);
                }
                count = 0;
                d_manual(nixie);
            }
            count++;
        }
        count = 0;
    }
    
    while (!MODE1 && !ADJ_SEC && !ADJ_MIN && !ADJ_HOUR && !MODE2) {
        disp_num(nixie, INTERVAL_D);
        if (MODE1) {
            int point = 0;
            
            if (!TD_flag) {
                for (point = 0; point < NALL; point++) {
                    for (int i = 0; i < 10; i++) { 
                        for (int j = 0; j < NALL; j++) {
                            if (j == point)
                                nixie_num(j, i);
                            else if (j < point)
                                nixie_num(j, (time + j)->num);
                            else
                                nixie_num(j, (nixie + j)->num);
                            reset(INTERVAL_D);
                        }
                    }
                }
            } else {
                for (point = 0; point < NALL; point++) {
                    for (int i = 0; i < 10; i++) { 
                        for (int j = 0; j < NALL; j++) {
                            if (j == point)
                                nixie_num(j, i);
                            else if (j < point)
                                nixie_num(j, (date + j)->num);
                            else
                                nixie_num(j, (nixie + j)->num);
                            reset(INTERVAL_D);
                        }
                    }
                }
            }
            break;
        } else if (ADJ_SEC || ADJ_MIN || ADJ_HOUR) {
            while ((ADJ_SEC || ADJ_MIN || ADJ_HOUR) && count < CHATCOUNT) {
                count++;
                disp_num(nixie, INTERVAL_D);
            }
            count = 0;
            break;
        }
    }
}

void d_static(struct nixie_status *nixie, int *mode) {
    for (int i = 0; i < NALL; i++)
        (nixie + i)->num = pctg[*mode][i];
    *mode += 1;
    if (*mode >= MODE_NUM)
        *mode = 0;
}

void d_random(struct nixie_status *nixie) {
    // to shuffle the random number
    for (int i = 0; i < 5; i++)                             
        rand();
    
    // set the number
    for (int i = 0; i < NALL; i++)
        (i != 6) ? ((nixie + i)->num = rand() % 10) : ((nixie + i)->num = 10);
}

void d_manual(struct nixie_status *nixie) {
    int count = 0, point = 0;
    
    // to shuffle the random number
    for (int i = 0; i < 5; i++)                             
        rand();
    
    // reset the nixie flag
    for (int i = 0; i < NALL; i++)
        (i != 6) ? (nixie + i)->flag = 1 : (nixie + i)->flag = 0;
    
    // set the number of nixie tube
    for (int i = 0; i < NALL; i++)
        (i != 6) ? (nixie + i)->num = rand() % 10 : (nixie + i)->num = 10;
    
    // set anode order
    int list[NALL];
    anode_random(list);
    
    // set random number
    int random[NALL];
    for (int i = 0; i < NALL; i++)
        random[i] = rand() % 10;    
    
    // set point
    int point = 0;
    while ((point != NALL) && MODE2) {
        while (!ADJ_SEC)
            show_d(nixie, random);
        
        while (ADJ_SEC && count < CHATCOUNT) {
            count++;
            show_d(nixie, random);
        }
        count = 0;
        
        (nixie + list[point])->flag = 0;
        point++;
    }
}

int show_d(struct nixie_status *nixie, int *random) {
    int num = 0;
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < NALL; j++) {
            if (!(nixie + j)->flag)
                nixie_num(j, (nixie + j)->num);
            else {
                int num = random[j] + i;
                if (num >= 10)
                    num -= 10;
                nixie_num(j, num);
            }
            num += (nixie + j)->flag;
            reset(INTERVAL_D);
        }
    }
    
    return num;
}

void show_d_static(struct nixie_status *nixie) {
    for (int i = 0; i < NALL; i++) {
        nixie_num(i, (nixie + i)->num);
        reset(INTERVAL_D);
    }
}

void clock()
{
    int count = 0;
    int hour_flag = 0, min_flag = 0, sec_flag = 0;
    
    while (MODE1) {
        while (!ADJ_MODE && MODE1) {
            TD_flag ? show_day(date) : show_time(time);

            if (ADJ_SEC || ADJ_MIN || ADJ_HOUR) {
                while ((ADJ_SEC || ADJ_MIN || ADJ_HOUR) && count < CHATCOUNT) {
                    count++;
                    TD_flag ? show_day(date) : show_time(time);
                }
                TD_flag++;
                TD_flag %= 2;
                count = 0;
            }
        }

        while (ADJ_MODE && MODE1) {                       // adjust the time
            TMR1ON = 0;                             // temporary stop TMIER1's count
            count = 0;
            
            if (!TD_flag) {
                // increase 'hour'
                if (ADJ_HOUR) {                                 
                    // avoid chattering
                    while (ADJ_HOUR && !hour_flag) {
                        count++;
                        show_time(time);
                        if (count == CHATCOUNT) {
                            hour_flag = 1;
                        }
                    }
                    count = 0;
                    
                    if (!hour_flag)
                        (time + 6)->num++;
                    else {
                        while (ADJ_HOUR) {
                            (time + 6)->num++;
                            adj_time(time);
                            for (int i = 0; i < INT_CHAT; i++)
                                show_time(time);
                        }
                        hour_flag = 0;
                    }
                }
                
                // increase 'minute'
                if (ADJ_MIN) {                           
                    // avoid chattering
                    while (ADJ_MIN && !min_flag) {
                        count++;
                        show_time(time);
                        if (count >= CHATCOUNT) {
                            min_flag = 1;
                        }
                    }
                    count = 0;
                    
                    if (!min_flag)
                        (time + 3)->num++;
                    else {
                        while (ADJ_MIN) {
                            (time + 3)->num++;
                            adj_time(time);
                            for (int i = 0; i < INT_CHAT; i++)
                                show_time(time);
                        }
                        min_flag = 0;
                    }
                }
                
                // increase 'second'
                if (ADJ_SEC) {                           
                    // avoid chattering
                    while (ADJ_SEC && !sec_flag) {
                        count++;
                        show_time(time);
                        if (count == CHATCOUNT) {
                            sec_flag = 1;
                        }
                    }
                    count = 0;
                    
                    if (!sec_flag)
                        time->num++;
                    else {
                        while (ADJ_SEC) {
                            time->num++;
                            adj_time(time);
                            for (int i = 0; i < INT_CHAT; i++)
                                show_time(time);
                        }
                        sec_flag = 0;
                    }
                }
                
                adj_time(time);
                show_time(time);
            } else {
                show_day(date);
                
                // increment each parameter
                if (ADJ_HOUR) {
                    // avoid chattering
                    while (ADJ_HOUR && !hour_flag) {
                        count++;
                        show_day(date);
                        if (count == CHATCOUNT) {
                            hour_flag = 1;
                        }
                    }
                    count = 0;
                    
                    if (!hour_flag)
                        (date + 4)->num++;
                    else {
                        while (ADJ_HOUR) {
                            (date + 4)->num++;
                            adj_date(date);
                            for (int i = 0; i < INT_CHAT; i++)
                                show_day(date);
                        }
                        hour_flag = 0;
                    }
                } else if (ADJ_MIN) {
                    // avoid chattering
                    while (ADJ_MIN && !min_flag) {
                        count++;
                        show_day(date);
                        if (count >= CHATCOUNT) {
                            min_flag = 1;
                        }
                    }
                    count = 0;
                    
                    if (!min_flag)
                        (date + 2)->num++;
                    else {
                        while (ADJ_MIN) {
                            (date + 2)->num++;
                            adj_date(date);
                            for (int i = 0; i < INT_CHAT; i++)
                                show_day(date);
                        }
                        min_flag = 0;
                    }
                } else if (ADJ_SEC) {
                    // avoid chattering
                    while (ADJ_SEC && !sec_flag) {
                        count++;
                        show_day(date);
                        if (count == CHATCOUNT) {
                            sec_flag = 1;
                        }
                    }
                    count = 0;
                    
                    if (!sec_flag)
                        date->num++;
                    else {
                        while (ADJ_SEC) {
                            date->num++;
                            adj_date(date);
                            for (int i = 0; i < INT_CHAT; i++)
                                show_day(date);
                        }
                        sec_flag = 0;
                    }
                }
                
                adj_date(date);
                show_day(date);
            }
        }
        TMR1ON = 1;
    }
}

void adj_time(struct nixie_status *nixie) {
    // adjust the time
    // adjust 'second'
    if (nixie->num > 9) {
        nixie->num = 0;
        (nixie + 1)->num++;
        if ((nixie + 1)->num >= SEC_OF_MIN) {
            (nixie + 1)->num = 0;
            (nixie + 3)->num++;
        }
    }
    
    // adjust 'minute'
    if ((nixie + 3)->num > 9) {
        (nixie + 3)->num = 0;
        (nixie + 4)->num++;
        if ((nixie + 4)->num == MIN_OF_HOUR) {
            (nixie + 4)->num = 0;
            (nixie + 6)->num++;
        }
    }
    // adjust 'hour'
    if ((nixie + 6)->num > 9) {
        (nixie + 6)->num = 0;
        (nixie + 7)->num++;
    }
    if (((nixie + 7)->num >= HOUR_OF_DAY1) && ((nixie + 6)->num >= HOUR_OF_DAY2)) {
        (nixie + 6)->num = 0;
        (nixie + 7)->num = 0;
        date->num++;
    }
}

void adj_date(struct nixie_status *nixie) {
    int leap_flag = 0;          // use to judge the year is whether leap year or not
    int year;
                    
    // set 'year'
    year = (nixie + 4)->num + (nixie + 5)->num * 10 + (nixie + 6)->num * 100 + (nixie + 7)->num * 1000;
                
    // judge leap year
    (year % 4 == 0) ? ((year % 100 == 0) ? ((year % 400 == 0) ? leap_flag = 1 : leap_flag = 0) : leap_flag = 1) : leap_flag = 0;
    
    // adjust 'day'
    if (nixie->num > 9) {
        nixie->num = 0;
        (nixie + 1)->num++;
    }
    if ((nixie + 3)->num == 0) {
        if ((nixie + 2)->num == 2) {
            if (leap_flag) {
                if (nixie->num >= 0 && (nixie + 1)->num >= 3) {
                    nixie->num = 1;
                    (nixie + 1)->num = 0;
                    (nixie + 2)->num++;
                }
            } else {
                if (nixie->num >= 9 && (nixie + 1)->num >= 2) {
                    nixie->num = 1;
                    (nixie + 1)->num = 0;
                    (nixie + 2)->num++;
                }
            }
        } else if ((nixie + 2)->num == 4 || (nixie + 2)->num == 6 || (nixie + 2)->num == 9) {
            if ((nixie->num >= 1 && (nixie + 1)->num >= 3)) {
                nixie->num = 1;
                (nixie + 1)->num = 0;
                (nixie + 2)->num++;
            }
        }
    } else if ((nixie + 3)->num == 1 && (nixie + 2)->num == 1) {
        if ((nixie->num >= 1 && (nixie + 1)->num >= 3)) {
            nixie->num = 1;
            (nixie + 1)->num = 0;
            (nixie + 2)->num++;
        }
    }
    if (nixie->num >= 2 && (nixie + 1)->num >= 3) {
        nixie->num = 1;
        (nixie + 1)->num = 0;
        (nixie + 2)->num++;
    }
    
    // adjust 'month'
    if ((nixie + 2)->num > 9) {
        (nixie + 2)->num = 0;
        (nixie + 3)->num++;
    }
    if ((nixie + 3)->num == 1 && (nixie + 2)->num == 3) {
        (nixie + 2)->num = 1;
        (nixie + 3)->num = 0;
        (nixie + 4)->num++;
    }
    
    // adjust 'year'
    if ((nixie + 4)->num > 9) {
        (nixie + 4)->num = 0;
        (nixie + 5)->num++;
        if ((nixie + 5)->num > 9) {
            (nixie + 5)->num = 0;
            (nixie + 6)->num++;
            if ((nixie + 6)->num > 9) {
                (nixie + 6)->num = 0;
                (nixie + 7)->num++;
            }
        }
    }
    if ((nixie + 4)->num == 9 && (nixie + 5)->num == 9 && (nixie + 6)->num == 9 && (nixie + 7)->num == 9) {
        for (int i = 4; i < NALL ; i++)
            (nixie + i)->num = 0;
    }
}

void crossfade()
{
    
}

void nixie_num(int anode, int casode)
{
    switch (anode) {
        case 0:
            PIN_1 = 0;
            PIN_2 = 0;
            PIN_3 = 0;
            break;
        case 1:
            PIN_1 = 1;
            PIN_2 = 0;
            PIN_3 = 0;
            break;
        case 2:
            PIN_1 = 0;
            PIN_2 = 1;
            PIN_3 = 0;
            break;
        case 3:
            PIN_1 = 1;
            PIN_2 = 1;
            PIN_3 = 0;
            break;
        case 4:
            PIN_1 = 0;
            PIN_2 = 0;
            PIN_3 = 1;
            break;
        case 5:
            PIN_1 = 1;
            PIN_2 = 0;
            PIN_3 = 1;
            break;
        case 6:
            PIN_1 = 0;
            PIN_2 = 1;
            PIN_3 = 1;
            break;
        case 7:
            PIN_1 = 1;
            PIN_2 = 1;
            PIN_3 = 1;
            break;
    }
   
    // casode assign is wrong as flip horizontal, so reassigned. 
    // the number of 'casode' epuals display on nixie tube. 
    // adgust here. 
    switch (casode) {
        case 0:
            PIN_A = 1;
            PIN_B = 0;
            PIN_C = 0;
            PIN_D = 0;
            break;
        case 1:
            PIN_A = 0;
            PIN_B = 0;
            PIN_C = 0;
            PIN_D = 0;
            break;
        case 2:
            PIN_A = 0;
            PIN_B = 0;
            PIN_C = 1;
            PIN_D = 0;
            break;
        case 3:
            PIN_A = 0;
            PIN_B = 0;
            PIN_C = 0;
            PIN_D = 1;
            break;
        case 4:
            PIN_A = 0;
            PIN_B = 1;
            PIN_C = 0;
            PIN_D = 0;
            break;
        case 5:
            PIN_A = 0;
            PIN_B = 1;
            PIN_C = 1;
            PIN_D = 0;
            break;
        case 6:
            PIN_A = 1;
            PIN_B = 0;
            PIN_C = 1;
            PIN_D = 0;
            break;
        case 7:
            PIN_A = 1;
            PIN_B = 0;
            PIN_C = 0;
            PIN_D = 1;
            break;
        case 8:
            PIN_A = 1;
            PIN_B = 1;
            PIN_C = 1;
            PIN_D = 0;
            break;
        case 9:
            PIN_A = 1;
            PIN_B = 1;
            PIN_C = 0;
            PIN_D = 0;
            break;
        case 10:
            PIN_A = 1;
            PIN_B = 1;
            PIN_C = 1;
            PIN_D = 1;
            PIN_dot = 1;
            break;
        default:
            PIN_A = 1;
            PIN_B = 1;
            PIN_C = 1;
            PIN_D = 1;
            PIN_dot = 0;
            break;
    }
    SWITCH = 0;
}

void show_time(struct nixie_status *nixie) {
    for (int i = 0; i < NALL; i++) {
        if (i == 2 || i == 5) {
            if (!comma_flag)
                nixie_num(i, (nixie + i)->num);
        } else
            nixie_num(i, (nixie + i)->num);
        reset(INTERVAL_TIME_D);
    }
}

void show_day(struct nixie_status *nixie) {
    for (int i = 0; i < NALL; i++) {
        nixie_num(i, (nixie + i)->num);
        reset(INTERVAL_DATE);
        if (i == 2 || i == 4) {
            nixie_num(i, 10);
            reset(INTERVAL_DATE);
        }
    }
}

void sample1()
{
    int loop = 9;
    int i = 0;
    
    while (1) {
        (loop == 10) ? (loop = 0) : loop++;
        for (i = 0; i < NALL; i++) {
            /*
            if (MODE1)
                nixie_num(i, loop);
            else if (MODE2)
                nixie_num(NALL - i - 1, loop);
             */
            MODE1 ? nixie_num(i, loop) : MODE2 ? nixie_num(NALL - i - 1, loop) : reset(INTERVAL_D);
            // delay1();
            reset(INTERVAL_D);
        }
    }
}

void sample2()
{
    int i = 0;
    int j = 0;
    
    while(1) {
        for (i = 0; i < 10; i++) {
            nixie_num(0, i);
            for (j = 0; j < INTERVAL; j++);
        }
    }
}

void sample3()
{
    // int TD_flag = 0;
    int count = 0;
    int loop = 0;
    
    while (1) {
        if (ADJ_MODE) 
            MODE1 ? count_inc() : MODE2 ? count_dec() : reset(INTERVAL_D);
        else {
            if (ADJ_SEC) {
                while (ADJ_SEC && count < CHATCOUNT) count++;
                TD_flag++;
                TD_flag %= 2;
            }
            
            // TD_flag ? show_day() : show_time();
            
            if (ADJ_MIN) {
                while (ADJ_MIN && count < CHATCOUNT) count++;
                while (loop < 10) {
                    for (int i = 0; i < 10; i++) {
                        for (int j = 0; j < NALL; j++) {
                            nixie_num(j, i);
                            // delay_D();
                            reset(INTERVAL_D);
                        }
                    }
                    loop++;
                }
                loop = 0;
            }
            
            if (ADJ_HOUR) {
                while (ADJ_HOUR && count < CHATCOUNT) count++;
                while (loop < 10) {
                    for (int i = 0; i < 10; i++) {
                        for (int j = 0; j < NALL; j++) {
                            if (j != 6) {
                                nixie_num(j, i);
                                // delay_D();
                                reset(INTERVAL_D);
                            } else {
                                nixie_num(j, 10);
                                // delay_D();
                                reset(INTERVAL_D);
                            }
                        }
                    }
                    loop++;
                }
                loop = 0;
            }
            /*
            if (TD_flag) {
                if (ADJ_SEC) {
                    while (ADJ_SEC && count < CHATCOUNT) count++;
                    day++;
                } else if (ADJ_MIN) {
                    while (ADJ_MIN && count < CHATCOUNT) count++;
                    month++;
                } else if (ADJ_HOUR) {
                    while (ADJ_HOUR && count < CHATCOUNT) count++;
                    year++;
                }
           } else {
                if (ADJ_SEC) {
                    while (ADJ_SEC && count < CHATCOUNT) count++;
                    second++;
                } else if (ADJ_MIN) {
                    while (ADJ_MIN && count < CHATCOUNT) count++;
                    minute++;
                } else if (ADJ_HOUR) {
                    while (ADJ_HOUR && count < CHATCOUNT) count++;
                    hour++;
                }
           }
           if (!ADJ_SEC || !ADJ_MIN || !ADJ_HOUR)
               count = 0;
           */
        }
    }
}

void counter() {
    while (!MODE1 && !MODE2)
        ADJ_MIN ? count_inc() : ADJ_HOUR ? count_dec() : ADJ_SEC ? all_shuffle() : reset(INTERVAL_D);
}

void count_inc() {
    int count = 0;
    struct nixie_status nixie[NALL];
    for (int i = 0; i < NALL; i++) {
        (nixie + i)->num = 0;
        (nixie + i)->flag = 1;
    }
    nixie->flag = 0;
    
    // avoid chattering
    while (ADJ_MIN && count < CHATCOUNT) count++;
    count = 0;
    
    // incrementation
    while (!ADJ_MIN && !ADJ_HOUR && !ADJ_SEC && !MODE1 && !MODE2) {
        int interval, flag_sum = 0;
        
        // check the number of nixie to display
        for (int i = 0; i < NALL; i++)
            flag_sum += (nixie + i)->flag;

        // set the interval
        !flag_sum ? interval = INT8 : (flag_sum == 1) ? interval = INT7 : (flag_sum == 2) ? interval = INT6 :
            (flag_sum == 3) ? interval = INT5 : (flag_sum == 4) ? interval = INT4 : (flag_sum == 5) ? interval = INT3 : (flag_sum == 6) ? interval = INT2 : 
                (flag_sum == 7) ? interval = INT1 : interval = 0;
        
        // display
        for (int i = 0; i < LOOP_CUNTR; i++) {
            disp_num(nixie, interval);
            if (inc_process(nixie))
                break;
            // nixie_count++;
        } 
    }
    // nixie_count = 0;
}

void count_dec() {
    int count = 0;
    struct nixie_status nixie[NALL];
    for (int i = 0; i < NALL; i++) {
       (nixie + i)->num = 9;
        (nixie + i)->flag = 0;
    }
    // nixie_count = 99999999;
    // nixie_count = 45537;
    
    // avoid chattering
    while (ADJ_HOUR && count < CHATCOUNT) count++;
    count = 0;
    
    // decrementation
    while (!ADJ_MIN && !ADJ_HOUR && !ADJ_SEC && !MODE1 && !MODE2) {
        int interval, flag_sum = 0;
        
        // check the number of nixie to display
        for (int i = 0; i < NALL; i++)
            flag_sum += (nixie + i)->flag;

        // set the interval
        !flag_sum ? interval = INT8 : (flag_sum == 1) ? interval = INT7 : (flag_sum == 2) ? interval = INT6 :
            (flag_sum == 3) ? interval = INT5 : (flag_sum == 4) ? interval = INT4 : (flag_sum == 5) ? interval = INT3 : (flag_sum == 6) ? interval = INT2 : 
                (flag_sum == 7) ? interval = INT1 : interval = 0;
        
        // display
        for (int i = 0; i < LOOP_CUNTR; i++) {
            disp_num(nixie, interval);
            if (dec_process(nixie))
                break;
            // nixie_count--;
        }
    }
    // nixie_count = 0;
}

void all_shuffle() {
    int count = 0, flag = 0;
    struct nixie_status nixie[NALL];
    
    // to shuffle the random number
    for (int i = 0; i < 5; i++)                             
        rand();
    
    while (!MODE1 && !MODE2 && !ADJ_SEC && !ADJ_MIN && !ADJ_HOUR) {
        // set the nixie_status
        for (int i = 0; i < NALL; i++) {
            (nixie + i)->num = rand() % 10;
            (nixie + i)->flag = 1;
        }

        // avoid chattering
        while (ADJ_SEC && count < CHATCOUNT) count++;
        count = 0;

        // set list
        int list[NALL];
        anode_random(list);

        // shuffling
        // do shuffle
        for (int loop = 0; loop < SHUFFLE_TIME; loop++) {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < NALL; j++) {
                    nixie_num(j, i);
                    // delay_D();
                    reset(INTERVAL_D);
                }
            }
        }

        for (int i = 0; i < NALL; i++){
            (nixie + list[i])->flag = 0;
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < NALL; j++) {
                    !(nixie + j)->flag ? nixie_num(j, (nixie + j)->num) : nixie_num(j, i);
                    reset(INTERVAL_D);
                }
            }
        }
        
        while (!MODE1 && !MODE2 && !ADJ_SEC && !ADJ_MIN && !ADJ_HOUR && count < INTERVAL) {
            disp_num(nixie, INTERVAL_D);
            if (MODE1 || MODE2 || ADJ_SEC || ADJ_MIN || ADJ_HOUR) {
                flag = 1;
                break;
            }
            count++;
        }
        count = 0;
        
        if (flag)
            break;
    }
}

void disp_num(struct nixie_status *status, int interval) {
    // display the number
    for (int i = 0; i < NALL; i++) {
        if (!(status + i)->flag) {
            nixie_num(i, (status + i)->num);
            reset(interval);
        }
    }
}

int inc_process(struct nixie_status *nixie) {
    /*なんもかんも再帰関数が使えないXC8コンパイラが悪い(ほんとか？)*/
    
    nixie->num++;
    if (nixie->num > 9) {
        (nixie + 1)->num++;
        (nixie + 1)->flag = 0;
        nixie->num = 0;
        if ((nixie + 1)->num > 9) {
            (nixie + 2)->num++;
            (nixie + 2)->flag = 0;
            (nixie + 1)->num = 0;
            if ((nixie + 2)->num > 9) {
                (nixie + 3)->num++;
                (nixie + 3)->flag = 0;
                (nixie + 2)->num = 0;
                if ((nixie + 3)->num > 9) {
                    (nixie + 4)->num++;
                    (nixie + 4)->flag = 0;
                    (nixie + 3)->num = 0;
                    if ((nixie + 4)->num > 9) {
                        (nixie + 5)->num++;
                        (nixie + 5)->flag = 0;
                        (nixie + 4)->num = 0;
                        if ((nixie + 5)->num > 9) {
                            (nixie + 6)->num++;
                            (nixie + 6)->flag = 0;
                            (nixie + 5)->num = 0;
                            if ((nixie + 6)->num > 9) {
                                (nixie + 7)->num++;
                                (nixie + 7)->flag = 0;
                                (nixie + 6)->num = 0;
                                if ((nixie + 7)->num > 9) {
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}

int dec_process(struct nixie_status *nixie) {
    /*なんもかんも再帰関数が使えないXC8コンパイラが悪い(ほんとか？)*/
    
    if (!nixie->flag) {
        nixie->num--;
        if (nixie->num < 0) {
            if (!(nixie + 1)->flag) {
                (nixie + 1)->num--;
                nixie->num = 9;
                if ((nixie + 1)->num < 0) {
                    if (!(nixie + 2)->flag) {
                        (nixie + 2)->num--;
                        (nixie + 1)->num = 9;
                        if ((nixie + 2)->num < 0) {
                            if (!(nixie + 3)->flag) {
                                (nixie + 3)->num--;
                                (nixie + 2)->num = 9;
                                if ((nixie + 3)->num < 0) {
                                    if (!(nixie + 4)->flag) {
                                        (nixie + 4)->num--;
                                        (nixie + 3)->num = 9;
                                        if ((nixie + 4)->num < 0) {
                                            if (!(nixie + 5)->flag) {
                                                (nixie + 5)->num--;
                                                (nixie + 4)->num = 9;
                                                if ((nixie + 5)->num < 0) {
                                                    if (!(nixie + 6)->flag) {
                                                        (nixie + 6)->num--;
                                                        (nixie + 5)->num = 9;
                                                        if ((nixie + 6)->num < 0) {
                                                            if (!(nixie + 7)->flag) {
                                                                (nixie + 7)->num--;
                                                                (nixie + 6)->num = 9;
                                                                if (!(nixie + 7)->num) {
                                                                    (nixie + 7)->flag = 1;
                                                                }
                                                            } else {
                                                                (nixie + 6)->flag = 1;
                                                            }
                                                        }
                                                    } else {
                                                        (nixie + 5)->flag = 1;
                                                    }
                                                }
                                            } else {
                                                (nixie + 4)->flag = 1;
                                            }
                                        }
                                    } else {
                                        (nixie + 3)->flag = 1;
                                    }
                                }
                            } else {
                                (nixie + 2)->flag = 1;
                            }
                        }
                    } else {
                        (nixie + 1)->flag = 1;
                    }
                }
            } else {
                nixie->flag = 1;
            }
        }
    } else {
        return 1;
    }
    
    return 0;
}

void anode_random(int *list) {
    int origin[NALL];
    int max_list = NALL;
    
    // to shuffle the random number
    for (int i = 0; i < 5; i++)                             
        rand();
    
    // set origin
    for (int i = 0; i < NALL; i++)
        origin[i] = i;
    
    // set order
    for (int i = 0; i < NALL; i++) {
        int num = rand() % max_list;
        list[i] = origin[num];
        for (int j = 0; j < max_list - num - 1; j++)
            origin[num + j] = origin[num + j + 1];
        max_list--;
    }   
}