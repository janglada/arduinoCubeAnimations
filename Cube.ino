
#include "Constants.h"
#include "Animations.h"

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;

//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 13;

////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;

#define LATCH_PIN_ON  1
#define LATCH_PIN_OFF  ~1


CubeNode* nodes;


uint8_t n0[2] = {1,7};
uint8_t n1[3] = {0,2,6};
uint8_t n2[3] = {1,3,5};
uint8_t n3[2] = {4,2};
uint8_t n4[3] = {3,5,8};
uint8_t n5[4] = {4,6,2,9};
uint8_t n6[4] = {1,5,7,10};
uint8_t n7[3] = {0,6,11};
uint8_t n8[3] = {4,9,12};
uint8_t n9[4] = {8,10,5,13};
uint8_t n10[4] = {9,11,6,14};
uint8_t n11[3] = {10,7,15};
uint8_t n12[2] = {8,13};
uint8_t n13[3] = {12,9,14};
uint8_t n14[3] = {13,10,15};
uint8_t n15[2] = {11,14};




const uint16_t edges[4] = {
	//~(1<<5 |1<<6 |1<<9|1<<10),
	1<<0|1<<1|1<<2|1<<3|1<<4|1<<8|1<<12|1<<13|1<<14|1<<15|1<<11|1<<7,
	(1<<0 |1<<3|1<<12|1<<15),
	(1<<0 |1<<3|1<<12|1<<15),
	1<<0|1<<1|1<<2|1<<3|1<<4|1<<8|1<<12|1<<13|1<<14|1<<15|1<<11|1<<7
	//~(1<<5 |1<<6 |1<<9|1<<10)
};


const int fn_steps[14] = {
	4,
	4,
	12,
	13,
	13,
	12,
	6,
	6,
	64,
	64,
	22,
	6,
	64,
	9

};

// http://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers
void (*f_ptr[14]) (int step) = {
	wallsStep1,
	wallsStep2,
	topBottomWalls,
	rotateUp,
	rotateDown,
	wallsOutter,
	rotateCenter,
	layers,
	oneByOne,
	randomCube,
	loopCubelets,
	expandingCube,
	rain,
	expandingCubeEmpty
};


void setup()
{
	Serial.begin(9600);
	Serial.println("setup");

	nodes = new CubeNode[16];

	nodes[0].index = 0;
	nodes[0].adj = n0;
	nodes[0].num_nodes = sizeof(n0)/ sizeof(uint8_t);

	nodes[1].index = 1;
	nodes[1].adj = n1;
	nodes[1].num_nodes = sizeof(n1)/ sizeof(uint8_t);

	nodes[2].index = 2;
	nodes[2].adj = n2;
	nodes[2].num_nodes = sizeof(n2)/ sizeof(uint8_t);

	nodes[3].index = 3;
	nodes[3].adj = n3;
	nodes[3].num_nodes = sizeof(n3)/ sizeof(uint8_t);

	nodes[4].index = 4;
	nodes[4].adj = n4;
	nodes[4].num_nodes = sizeof(n4)/ sizeof(uint8_t);

	nodes[5].index = 5;
	nodes[5].adj = n5;
	nodes[5].num_nodes = sizeof(n5)/ sizeof(uint8_t);

	nodes[6].index = 6;
	nodes[6].adj = n6;
	nodes[6].num_nodes = sizeof(n6)/ sizeof(uint8_t);

	nodes[7].index = 7;
	nodes[7].adj = n7;
	nodes[7].num_nodes = sizeof(n7)/ sizeof(uint8_t);

	nodes[8].index = 8;
	nodes[8].adj = n8;
	nodes[8].num_nodes = sizeof(n8)/ sizeof(uint8_t);

	nodes[9].index = 9;
	nodes[9].adj = n9;
	nodes[9].num_nodes = sizeof(n9)/ sizeof(uint8_t);

	nodes[10].index = 10;
	nodes[10].adj = n10;
	nodes[10].num_nodes = sizeof(n10)/ sizeof(uint8_t);

	nodes[11].index = 11;
	nodes[11].adj = n11;
	nodes[11].num_nodes = sizeof(n11)/ sizeof(uint8_t);

	nodes[12].index = 12;
	nodes[12].adj = n12;
	nodes[12].num_nodes = sizeof(n12)/ sizeof(uint8_t);

	nodes[13].index = 13;
	nodes[13].adj = n13;
	nodes[13].num_nodes = sizeof(n13)/ sizeof(uint8_t);

	nodes[14].index = 14;
	nodes[14].adj = n14;
	nodes[14].num_nodes = sizeof(n14)/ sizeof(uint8_t);

	nodes[15].index = 15;
	nodes[15].adj = n15;
	nodes[15].num_nodes = sizeof(n15)/ sizeof(uint8_t);





	randomSeed(analogRead(0));

	pinMode(latchPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);



	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);


	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);


	 // initialize Timer1
    // http://www.engblaze.com/microcontroller-tutorial-avr-and-arduino-timer-interrupts/
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B

    // set compare match register to desired timer count:
    OCR1A = 2048; //1024;//2048;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    // enable global interrupts:
    sei();

/*
	//http://electronics.stackexchange.com/questions/26363/how-do-i-create-a-timer-interrupt-with-arduino
	TCCR2B = 0x00;        //Disbale Timer2 while we set it up
	TCNT2  = 130;         //Reset Timer Count to 130 out of 255
	TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
	TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
	TCCR2A = 0x00;        //Timer2 Control Reg A: Wave Gen Mode normal
	TCCR2B = 0x06;        //Timer2 Control Reg B: Timer Prescaler set to 128

*/

}


int counter = 0;
long tm = millis();

void loop() {
//delay(1000);
//	delay(1000);

	for(uint8_t i = 0;i < 4; i++) {
		PORTD = 1<<(i+2);
		shiftOutFastRepeat(dataPin, clockPin, MSBFIRST, m_layers[i], 8 ,3);

	}

}




volatile int anim_idx = 0;
volatile int step = 0;

ISR(TIMER1_COMPA_vect)
{

	if (step < fn_steps[anim_idx]*3){

		(f_ptr[anim_idx])(step);
		step++;
	} else {
		anim_idx++;
		if (anim_idx > 13)
			anim_idx = 0;
		step = 0;
	}
	//(f_ptr[13])(counter);
	counter++;
	/*
	//uint8_t step = (++counter)%4;

	//Serial.println(millis());
	m_layers[0] = walls2[step];
	m_layers[1] = walls2[step];
	m_layers[2] = walls2[step];
	m_layers[3] = walls2[step];
*/
}

/*
// http://arduinomega.blogspot.com.es/2011/05/timer2-and-overflow-interrupt-lets-get.html
ISR(TIMER2_OVF_vect) {
            //Increments the interrupt counter

	PORTD = 1<<2;
	shiftOutFast(dataPin, clockPin, MSBFIRST, m_layers[0]);

	PORTD = 1<<(1 + 2);
	shiftOutFast(dataPin, clockPin, MSBFIRST, m_layers[1]);

	PORTD = 1<<(2+2);
	shiftOutFast(dataPin, clockPin, MSBFIRST, m_layers[2]);

	PORTD = 1<<(3+2);
	shiftOutFast(dataPin, clockPin, MSBFIRST, m_layers[3]);

  TCNT2 = 255;           //Reset Timer to 130 out of 255
  TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag



};
*/

void fadeIn()
{
	uint16_t d = 12;
	//http://programmers.stackexchange.com/questions/119380/the-perfect-crossfade // fade in
	for(uint8_t k = 0; k<= d;k++) {
		double v = 50*easeInQuad(k,0,1,d);
		for(uint8_t i = 0; i< 20;i++) {
			PORTD = 1<<((i%4)+2);
			shiftOutFastPOV(dataPin, clockPin, MSBFIRST, ALL_VERTEX, v);
			delay(2);
		}

	}



	for(uint8_t k = d; k> 0;k--) {
		double v = 50*easeInQuad(k,0,1,d);
		for(uint8_t i = 0; i< 20;i++) {
			PORTD = 1<<((i%4)+2);
			shiftOutFastPOV(dataPin, clockPin, MSBFIRST, ALL_VERTEX, v);
			delay(1);
		}

	}

	delay(20);
}

/**
 * t: current time
 * b: start value
 * C. change in value
 * d: duration
 *
 */
double easeInQuad(double t, uint16_t b, uint16_t c, uint16_t d) {
	t /= d;
	return c*t*t + b;
};


// quadratic easing out - decelerating to zero velocity

double easeOutQuad(double t, uint16_t b, uint16_t c, uint16_t d) {
	t /= d;
	return b -c * t*(t-2) ;
};



void loopEdges() {

	uint8_t it = 0;
	for(uint8_t step = 0; step< 4; step++) {
		for(uint8_t j = 0; j< 40;j++) {
		  it = j%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, edges[step]);
		}
	}
}



void loopSnake() {



	Serial.println("LOOP SNAKE");
	//				     node part        layer part
	//uint16_t moving_node = (1<<random(16)) ;
	CubeNode current	 = nodes[9];

	int snake_size = 6;
	Vertex* fulldata = new Vertex[snake_size];
	fulldata[0].data  = 1<<9;	//current.index;
	fulldata[0].layer = 1<<1; //1<<layer;

	fulldata[1].data  = 1<<9; //current.index;
	fulldata[1].layer = 1<<0; //1<<layer;

	fulldata[2].data  = 1<<10; //current.index;
	fulldata[2].layer = 1<<0; //1<<layer;

	fulldata[3].data  = 1<<11; //current.index;
	fulldata[3].layer = 1<<0; //1<<layer;

	fulldata[3].data  = 1<<13; //current.index;
	fulldata[3].layer = 1<<0; //1<<layer;

	fulldata[4].data  = 1<<14; //current.index;
	fulldata[4].layer = 1<<0; //1<<layer;

	fulldata[5].data  = 1<<14; //current.index;
	fulldata[5].layer = 1<<0; //1<<layer;

	uint16_t data = 0;
	for(int c = 0; c < 50;c ++){



		//Serial.println("CURRENT ");
		//Serial.println(current.index);

		//int nei = current.randomNeighbour();
		for(int c = 0; c < 29; c++) {
			for(int k = 0; k< 15;k++ ) {
				data = 0;
				for(int j = 0; j< snake_size;j++) {
					if (fulldata[j].layer & (1<<(k%4)) ) {
						// active for this layer
						data |=  fulldata[j].data;
					}
				}
				if (data != 0) {
					//digitalWrite(latchPin, LOW);
					shiftOutFast(dataPin, clockPin, MSBFIRST, data);
					//digitalWrite(latchPin, HIGH);
					PORTD = 1<<(k%4+2);
				}
				//PORTD = 1<<(k+2);
				//delay(1);
			}
		}

		for(int l = (snake_size-1); l>= 1; l-- ) {
			fulldata[l].data  = fulldata[l-1].data; //current.index;
			fulldata[l].layer = fulldata[l-1].layer; //1<<layer;
		}

		int nei = current.randomNeighbour();

		if (random(4) == 0  || ((1<<nei) & data)/* change layer 1/3 changes */) {
			//Serial.print("CHANGE LAYER BEFORE\n");
			//print_binary(fulldata[0].layer, 8);
			Serial.print("\n");
			if (fulldata[0].layer & (1<<0)) {
				fulldata[0].layer = 1<<1;
			} else if (fulldata[0].layer & (1<<3)) {
				fulldata[0].layer = 1<<2;
			} else if (fulldata[0].layer & (1<<1)){
				if (randBool()) {
					fulldata[0].layer = 1<<0;
				} else {
					fulldata[0].layer = 1<<2;
				}
			} else if (fulldata[0].layer & (1<<2)){
				if (randBool()) {
					fulldata[0].layer = 1<<1;
				} else {
					fulldata[0].layer = 1<<3;
				}
			}
			//Serial.print("AFTER\n");
			//print_binary(fulldata[0].layer, 8);
			//Serial.print("\n");

		} else {
/*
			int nei = current.randomNeighbour();

			while( ((1<<nei) & data)) {
				Serial.println("  NEIGHBOUR IN PATH");
				Serial.print(nei);
				Serial.println("");
				print_binary(data,16);
				Serial.println("");
				print_binary(1<<nei,16);
				nei = current.randomNeighbour();
			}
*/
			fulldata[0].data  = 1<<nei;	//current.index;
			current = nodes[nei];
		}










		//fulldata[]

		//current = nodes[nei];


		//delay(100);

	}

	delete[] fulldata;

}


bool randBool() {
	return random(2) == 1;
}



void shiftOut16(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val)
{
	uint16_t i;
	for (i = 0; i < 16; i++)  {
		if (bitOrder == LSBFIRST) {
			//Serial.print(!!(val & (1 << i)));
			digitalWrite(dataPin, !!(val & (1 << i)));
		} else {
			//Serial.print(!!(val & (1 << (15 - i))));
			digitalWrite(dataPin, !!(val & (1 << (15 - i))));
		}

		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}


// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//

void shiftOutFast(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val)
{
	shiftOutFastPOV(dataPin, clockPin, bitOrder, val, 25);
}

void shiftOutFastRepeat(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val, uint8_t d, uint8_t repeat)
{
	for(uint16_t k = 0; k< repeat; k++) {
	// odd pins
		PORTB = PORTB & LATCH_PIN_OFF;
		_shiftOutFast(dataPin, clockPin, bitOrder, val & ODD);

		PORTB = PORTB | LATCH_PIN_ON;
		delayMicroseconds(d);

		PORTB = PORTB & LATCH_PIN_OFF;
		_shiftOutFast(dataPin, clockPin, bitOrder, val & EVEN);
		PORTB = PORTB | LATCH_PIN_ON;
		delayMicroseconds(d);
	}
	PORTB = PORTB & LATCH_PIN_OFF;
	_shiftOutFast(dataPin, clockPin, MSBFIRST, 0);
	PORTB = PORTB | LATCH_PIN_ON;
}


void shiftOutFastPOV(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val, uint16_t t)
{

	for(uint16_t k = 0; k< t; k++) {
	// odd pins
		PORTB = PORTB & LATCH_PIN_OFF; //(~latch_pin_on);
		_shiftOutFast(dataPin, clockPin, bitOrder, val & ODD);

		PORTB = PORTB | LATCH_PIN_ON;
		delayMicroseconds(10);

		PORTB = PORTB & LATCH_PIN_OFF;
		_shiftOutFast(dataPin, clockPin, bitOrder, val & EVEN);
		PORTB = PORTB | LATCH_PIN_ON;
		delayMicroseconds(10);
	}
	/*
	PORTB = PORTB & LATCH_PIN_OFF;
	_shiftOutFast(dataPin, clockPin, bitOrder, 0);
	PORTB = PORTB | LATCH_PIN_ON;
	*/
}


void _shiftOutFast(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val)
{


  uint8_t cnt;
  uint8_t bitData, bitNotData;
  uint8_t bitClock, bitNotClock;
  volatile uint8_t *outData;
  volatile uint8_t *outClock;

  outData = portOutputRegister(digitalPinToPort(dataPin));
  outClock = portOutputRegister(digitalPinToPort(clockPin));
  bitData = digitalPinToBitMask(dataPin);
  bitClock = digitalPinToBitMask(clockPin);

  bitNotClock = bitClock;
  bitNotClock ^= 0x0ff;

  bitNotData = bitData;
  bitNotData ^= 0x0ff;

  cnt = 16;
  if (bitOrder == LSBFIRST)
  {
    do
    {
      if ( val & 1 )
    	  *outData |= bitData;
      else
    	  *outData &= bitNotData;

      *outClock |= bitClock;
      *outClock &= bitNotClock;
      val >>= 1;
      cnt--;
    } while( cnt != 0 );
  }
  else
  {
    do
    {
      if ( val & 32768 )
    	  *outData |= bitData;
      else
    	  *outData &= bitNotData;

      *outClock |= bitClock;
      *outClock &= bitNotClock;
      val <<= 1;
      cnt--;
    } while( cnt != 0 );
  }


 // PORTB = PORTB | (latch_pin_on);

}




