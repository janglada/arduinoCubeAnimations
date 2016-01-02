//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;

//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 12;

////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;


typedef struct
{
	uint16_t	data;
	uint8_t 	layer;

} Vertex;


typedef struct
{
	uint8_t* 	adj;
	uint8_t 	num_nodes;
	uint8_t		index;

	uint8_t	randomNeighbour() {
		int a = random(this->num_nodes);
		/*
		Serial.print("RND NUMBER ");
		Serial.print(a);
		Serial.print(" VALUE ");
		Serial.println(this->adj[a]);
		*/
		return this->adj[a];
	}

} CubeNode;

/*
void _printNodeInfo(CubeNode c)
{

	Serial.print("\nCURRENT NODE ");
	Serial.print(c.index);

	Serial.print("\nAVAILABLE NEIGHBOURS NODE ");
	Serial.print(c.num_nodes);
	Serial.print(" \n");
	int k = 0;
	for(k = 0; k< c.num_nodes; k++) {
		Serial.print(c.adj[k]);
		Serial.print(" ,");
	}
}

*/
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


const uint8_t outer[12] = {
 0, 1, 2, 3, 4, 8, 12, 13, 14, 15, 11, 7
};

const uint8_t inner[4] = {
 5,6,9,10
};

//
const uint16_t walls1[4] = {
	34945, //	0,7,11,15
	17474, // 	1,6,10,14
	8740,  //	2,5,9,13
	4376	// 3,4,8,12
};


const uint16_t walls2[4] = {
	15,		// 0,1,2,3
	240,	// 7,6,5,4
	3840,	// 8,9,10,11
	61440	// 12,13,14,15

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


	int k = 0,j;

	for(j = 0; j< 16; j++) {
	//	_printNodeInfo(nodes[j]);
	}


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


}


void loop() {
//	delay(10000);
	loopSnake();
}
void loopAll() {


	for(int i = 0; i< 5; i++) {
		loopWallsMixed();
	}
	for(int i = 0; i< 5; i++) {
		loopWalls1();
	}

	for(int i = 0; i< 5; i++) {
		loopWalls2();
	}

	for(int i = 0; i< 5; i++) {
		loopLayers();
	}

	for(int i = 0; i< 5; i++) {
		loopOutterWalls();
	}

	//delay(1000);
}


void loopWallsMixed() {
	for(uint16_t i = 0; i< 4; i++) {
		digitalWrite(latchPin, LOW);
		shiftOutFast(dataPin, clockPin, MSBFIRST, walls1[i] |  walls2[(i-3)%4]);
		digitalWrite(latchPin, HIGH);
		animateAllLayers(80);
	}
}


void loopWalls1() {

	for(uint16_t i = 0; i< 4; i++) {
		digitalWrite(latchPin, LOW);
		shiftOutFast(dataPin, clockPin, MSBFIRST, walls1[i]);
		digitalWrite(latchPin, HIGH);
		animateAllLayers(80);
	}
}


void loopWalls2() {


	for(uint16_t i = 0; i< 4; i++) {
		digitalWrite(latchPin, LOW);
		shiftOutFast(dataPin, clockPin, MSBFIRST, walls2[i]);
		digitalWrite(latchPin, HIGH);
		animateAllLayers(80);
	}
}


void loopLayers() {

	uint16_t all =  0xffff;
	digitalWrite(latchPin, LOW);
	shiftOutFast(dataPin, clockPin, MSBFIRST, all);
	digitalWrite(latchPin, HIGH);

	for(int j = 0; j< 6;j++) {
	  digitalWrite(2, j%6 == 0? HIGH :LOW);
	  digitalWrite(3, (j%6 == 1 || j%6 == 5)? HIGH :LOW);
	  digitalWrite(4, (j%6 == 2 || j%6 == 4)? HIGH :LOW);
	  digitalWrite(5, j%6 == 3? HIGH :LOW);
	  delay(75);
	}
}


void loopOutterWalls() {

	uint16_t border = 0;
	for(uint16_t i = 0; i< 12; i++) {
		//bitWrite(oout, i, HIGH);
		border = 1<<outer[i];

		if (i >= 3)
			border |= 1<<outer[i-3];

		if (i >= 2)
			border |= 1<<outer[i-2];
		if (i >= 1)
			border |= 1<<outer[i-1];

		digitalWrite(latchPin, LOW);

		shiftOutFast(dataPin, clockPin, MSBFIRST, border);
		digitalWrite(latchPin, HIGH);

		animateAllLayers(80);
	}
}



void loopSnake() {



	Serial.println("LOOP SNAKE");
	//				     node part        layer part
	//uint16_t moving_node = (1<<random(16)) ;
	CubeNode current	 = nodes[9];

	int snake_size = 4;
	Vertex* fulldata = new Vertex[snake_size];
	fulldata[0].data  = 1<<9;	//current.index;
	fulldata[0].layer = 1<<1; //1<<layer;

	fulldata[1].data  = 1<<9; //current.index;
	fulldata[1].layer = 1<<0; //1<<layer;

	fulldata[2].data  = 1<<10; //current.index;
	fulldata[2].layer = 1<<0; //1<<layer;

	fulldata[3].data  = 1<<11; //current.index;
	fulldata[3].layer = 1<<0; //1<<layer;


	uint16_t data = 0;
	while(true){



		//Serial.println("CURRENT ");
		//Serial.println(current.index);

		//int nei = current.randomNeighbour();
		for(int c = 0; c < 65; c++) {
			for(int k = 0; k< 4;k++ ) {
				data = 0;
				for(int j = 0; j< snake_size;j++) {
					if (fulldata[j].layer & (1<<k) ) {
						// active for this layer
						data |=  fulldata[j].data;
					}
				}
				if (data != 0) {
					digitalWrite(latchPin, LOW);
					shiftOutFast(dataPin, clockPin, MSBFIRST, data);
					digitalWrite(latchPin, HIGH);
					PORTD = 1<<(k+2);
				}
				//PORTD = 1<<(k+2);
				delay(1);
			}
		}


		fulldata[3].data  = fulldata[2].data; //current.index;
		fulldata[3].layer = fulldata[2].layer; //1<<layer;

		fulldata[2].data  = fulldata[1].data; //current.index;
		fulldata[2].layer = fulldata[1].layer; //1<<layer;

		fulldata[1].data  = fulldata[0].data; //current.index;
		fulldata[1].layer = fulldata[0].layer; //1<<layer;



		if (random(3) == 0 /* change layer 1/3 changes */) {
			Serial.print("CHANGE LAYER BEFORE\n");
			print_binary(fulldata[0].layer, 8);
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
			Serial.print("AFTER\n");
			print_binary(fulldata[0].layer, 8);
			Serial.print("\n");

		} else {
			int nei = current.randomNeighbour();
			fulldata[0].data  = 1<<nei;	//current.index;
			current = nodes[nei];
		}










		//fulldata[]

		//current = nodes[nei];


		//delay(100);

	}

	delete[] fulldata;

}

void animateSnake(int node) {
	PORTD = ((node>>16)<<2);
}

bool randBool() {
	return random(2) == 1;
}

void animateAllLayers(int millis)
{


	uint16_t it = 0;
	// total time 15 us ==  0.015ms --> 1/0.025
	for(int j = 0; j< millis;j++) {
	  it = j%4 + 2;

	  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
	  PORTD = 1<<it;
	  /*
	  digitalWrite(2, it == 0? HIGH :LOW);
	  digitalWrite(3, it == 1? HIGH :LOW);
	  digitalWrite(4, it == 2? HIGH :LOW);
	  digitalWrite(5, it == 3? HIGH :LOW);
	  */
	  delay(1);
	}

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


void shiftOutFast(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val)
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
}




void print_binary(uint32_t v, int num_places)
{
	uint32_t mask=0, n;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask;  // truncate v to specified number of places

    while(num_places)
    {

        if (v & (0x0001 << num_places-1))
        {
             Serial.print("1");
        }
        else
        {
             Serial.print("0");
        }

        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
            Serial.print("_");
        }
    }
}

