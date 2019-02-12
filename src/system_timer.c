#include "system_timer.h"
#include "led.h"

u16 cnt = 0;
time cur_ms;

void T2_ISR1( void ) __interrupt ( 2 ){
	cur_ms++;
}

void SetVector(unsigned char __xdata * Address, void * Vector) {
	unsigned char __xdata * TmpVector;
	*Address = 0x02;
	// address to jump
	TmpVector = (unsigned char __xdata *) (Address + 1); // make TmpVector be pointing to next byte after written command 'ljmp'
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8); // this byte now contains 8 highest bits of ISR address
	++TmpVector; // move to next byte
	*TmpVector = (unsigned char) Vector; // write next 8 bits of address
}

void initialize_system_timer() {
	cur_ms = 0;
	cnt = 0;
	SetVector(0x202B, (void *)T2_ISR1);
	TH2 = (-1000) >> 8; // TH2 (highest 8 bits of timer 2 counting register) = 1111 1100
	TL2 = (-1000) & 0xFF; // TL2 (lowest 8 bits of timer 2 counting register) = 0001 1000
	// temporarily store the contents of the counter register
	RCAP2H = (-1000) >> 8; // RCAP2H (highest 8 bits of timer 2 capture register) = 1111 1100
	RCAP2L = (-1000) & 0xFF; // RCAP2L (lowest 8 bits of timer 2 capture register) = 0001 1000
	ET2=1;	// enable (interrupt from) timer 2
	TR2=1;  // start timer 2
}

time get_ms(void){
	return cur_ms;
}

time get_ms_after(time t0){
	return cur_ms-t0;

}
void delay_ms1(time t){
	time now = get_ms();
	while(get_ms_after(now) < t){
		;
	}
	
}

void delay_ms_with_leds1(time t, unsigned char mask){
	time now = get_ms();
	while(get_ms_after(now) < t){
		leds(mask);
	}
}

void delay_ms ( unsigned long ms ) {
    volatile unsigned long i, j;
    for( j = 0; j < ms; j++ ) {
        for( i = 0; i < 20; i++ );
    }   
}

void delay_ms_with_leds ( unsigned long ms, unsigned char mask ) {
    volatile unsigned long i, j;
    for( j = 0; j < ms; j++ ) {
        for( i = 0; i < 10; i++ ) {
			leds(mask);
		}
    }   
}