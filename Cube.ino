#define ODD  21845
#define EVEN 43690
#define ALL_VERTEX  0xffff


#define ROW3  0b1111000000000000
#define ROW2  0b0000111100000000
#define ROW1  0b0000000011110000
#define ROW0  0b0000000000001111


//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;

//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 13;

////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;

#define LATCH_PIN_ON  1
#define LATCH_PIN_OFF  ~1


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




const uint16_t rotate[6] = {
	(1<<0)  | (1<<6)  | (1<<9)   | (1<<12), // diagonal
	(1<<8)  | (1<<9)  | (1<<6)   | (1<<7),
	(1<<4)  | (1<<5)  | (1<<10)  | (1<<11),
	(1<<3)  | (1<<5)  | (1<<10)  | (1<<15), // diagonal
	(1<<2)  | (1<<5)  | (1<<10)  | (1<<14),
	(1<<1)  | (1<<6)  | (1<<9)   | (1<<13) // 11
};



const uint16_t edges[4] = {
	//~(1<<5 |1<<6 |1<<9|1<<10),
	1<<0|1<<1|1<<2|1<<3|1<<4|1<<8|1<<12|1<<13|1<<14|1<<15|1<<11|1<<7,
	(1<<0 |1<<3|1<<12|1<<15),
	(1<<0 |1<<3|1<<12|1<<15),
	1<<0|1<<1|1<<2|1<<3|1<<4|1<<8|1<<12|1<<13|1<<14|1<<15|1<<11|1<<7
	//~(1<<5 |1<<6 |1<<9|1<<10)
};




const uint16_t down[7][4] = {
	{
		ROW3,
		ROW3,
		ROW3,
		ROW3,
	}, 	// frame 1
	{
		ROW3,
		ROW3,
		ROW3,
		ROW2
	},// frame 2
	{
		ROW3,
		ROW3,
		ROW2,
		ROW1

	},// frame 3
	{
		ROW3,
		ROW2,
		ROW1,
		ROW0
	}// frame 4
	,
	{
		ROW3,
		ROW2,
		ROW1|ROW0,
		0
	},// frame 5
	{
		ROW3|ROW2|ROW1,
		ROW0,
		0,
		0
	},// frame 6
	{
		ALL_VERTEX,
		0,
		0,
		0
	}/// frame 7
};


const uint16_t up[7][4] = {
	{
		ROW0,
		ROW0,
		ROW0,
		ROW0
	}, 	// frame 1
	{
		ROW0,
		ROW0,
		ROW0,
		ROW1
	},// frame 2
	{
		ROW0,
		ROW0,
		ROW1,
		ROW2
	},// frame 3
	{
		ROW0,
		ROW1,
		ROW2,
		ROW3
	}// frame 4
	,
	{
		ROW0,
		ROW1,
		ROW2|ROW3,
		0
	},// frame 5
	{
		ROW0|ROW1,
		ROW2|ROW3,
		0,
		0
	},// frame 6
	{
		ALL_VERTEX,
		0,
		0,
		0
	}/// frame 7
};




uint16_t cubelet(uint8_t vertex) {
	if (vertex >= 4 && vertex<8) {
		uint16_t base =  1<<vertex | 1<<(vertex-1);

		if (vertex == 7) {
			base |= (1<<0|1<<1);
		} else if (vertex == 6){
			base |= (1<<1|1<<2);
		} else if (vertex == 5){
			base |= (1<<2|1<<3);
		}
		return base;

	} else {
		return 1<<vertex | 1<<(vertex-1)| 1<<(vertex-4)|1<<(vertex-5);
	}
}


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


int counter = 0;
long tm = millis();

void loop() {

	fadeIn();
	delay(1000);
	//loopRandom();
	/*
	for(int i=0; i< 16; i++) {
		digitalWrite(latchPin, LOW);
		_shiftOutFast(dataPin, clockPin, MSBFIRST, 1<<8|1<<7);
		digitalWrite(latchPin, HIGH);
		delay(1000);
	}

	*/
}
void loopAll() {

	loopOneByOne();

/*
	for(int i = 0; i< 5; i++) {
		loopWallsMixed();
	}
*/
	for(int i = 0; i< 5; i++) {
		loopWalls1();
	}
	for(int i = 0; i< 2; i++) {
		loopExpandingCube();
	}
	for(int i = 0; i< 5; i++) {
		loopWalls2();
	}

	for(int i = 0; i< 5; i++) {
		loopLayers();
	}


	loopRandom();

	for(int i = 0; i< 3; i++) {
		loopOutterWalls();
	}
	for(int i = 0; i< 6; i++) {
		loopRotate();
	}

	//loopSnake();

	loopCubelets();

	for(int i = 0; i< 5; i++) {
		loopUp();
		loopDown();
	}



	loopTopWalls();



	//delay(1000);
}


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

				//double alpha = (double)(( 1 - exp(-k / (  t)))*50);
			//for(uint8_t i = 0; i< t;i++) {
	//shiftOutFastPOV(dataPin, clockPin, MSBFIRST, ALL_VERTEX, 100);
	//delay(7);


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

void loopRain() {
	uint16_t all = 0;
	all = ~all;

	const uint8_t toplayer_mask = 1<<3;


	uint32_t drops_per_layer[4] = {
		0,
		0,
		0,
		0
	};


	uint16_t i, j, f, k;


	for(j = 0; j< 10000; j++) {
	//	for(f = 0; f< 10; f++) {


				//digitalWrite(latchPin, LOW);

				for(uint8_t k = 0; k< 60;k++) {

				//if (i==0)
				//	shiftOutFast(dataPin, clockPin, MSBFIRST, all);
				//else
					shiftOutFast(dataPin, clockPin, MSBFIRST, drops_per_layer[k%4]);

				//digitalWrite(latchPin, HIGH);
				//if (drops_per_layer[i]) {
					PORTD = 1<<(k%4+2);
				}
				//}

				//animateAllLayers(80);

		//}


		for(i = 3; i > 1; i--) {
			drops_per_layer[i] = drops_per_layer[i-1];
		}
		drops_per_layer[1] = 0;

		uint8_t n =  1;
		drops_per_layer[0] = all;
		for(int i = 0; i< n; i++)
			drops_per_layer[1] |= (1<<random(16));



		/*
		// continue fall
		for(i = 3; i >= 2; i--) {
			drops_per_layer[i] = drops_per_layer[i-1];
		}
		// layer 1 we dont know...
		drops_per_layer[1] = 0;

		// a new drop falls find one which is not falling
		int pos = 1<<random(16);
	//	while (!(drops_per_layer[0] & pos)) {
	//		pos = 1<<random(16);
	//	}
		// remove pos from layer 0
		drops_per_layer[0] = drops_per_layer[0] & (~pos);
		drops_per_layer[1] = drops_per_layer[1] | pos;

		*/


	}


}



void loopRotate() {
	for(uint16_t step = 0; step< 6; step++) {


		uint8_t it = 0;
		uint8_t time = (step%3 == 0)? 50 :40;
		//digitalWrite(latchPin, HIGH);
		// total time 15 us ==  0.015ms --> 1/0.025
		for(uint8_t j = 0; j< time;j++) {
		  it = j%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, rotate[step]);
		}



	}
}


void loopWallsMixed() {
	for(uint16_t step = 0; step< 4; step++) {

		uint8_t it = 0;
		// total time 15 us ==  0.015ms --> 1/0.025
		for(uint8_t j = 0; j< 40;j++) {
		  it = j%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, walls1[step] |  walls2[(step-3)%4]);
		}

	}
}


void loopWalls1() {

	for(uint16_t step = 0; step< 4; step++)
	{

		uint8_t it = 0;
		// total time 15 us ==  0.015ms --> 1/0.025
		for(uint8_t j = 0; j< 50;j++) {
		  it = j%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, walls1[step]);
		}
	}
}


void loopWalls2() {


	for(uint16_t step = 0; step< 4; step++)
	{

		uint8_t it = 0;
		// total time 15 us ==  0.015ms --> 1/0.025
		for(uint8_t j = 0; j< 50;j++) {
		  it = j%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, walls2[step]);
		}
	}
}


void loopUp() {

	uint16_t s = 10;
	for(uint16_t step = 0; step< 7; step++)
	{
		uint8_t it = 0;
		for(uint16_t j = 0; j< 4*s;j++) {
		  it = j%4 + 2;

		  PORTD = 1<<it;
		  shiftOutFast(dataPin, clockPin, MSBFIRST, up[step][j%4]);
		}
	}

	for(uint16_t step = 6; step>= 1; step--)
	{
		uint8_t it = 0;
		for(uint16_t j = 0; j< 4*s;j++) {
		  it = j%4 + 2;

		  PORTD = 1<<it;
		  shiftOutFast(dataPin, clockPin, MSBFIRST, up[step][j%4]);
		}
	}


}
void loopDown() {
	uint16_t s = 10;
	for(uint16_t step = 0; step< 7; step++)
	{
		uint8_t it = 0;
		for(uint16_t j = 0; j< 4*s;j++) {
		  it = j%4 + 2;

		  PORTD = 1<<it;
		  shiftOutFast(dataPin, clockPin, MSBFIRST, down[step][j%4]);
		}
	}

	for(uint16_t step = 6; step>= 1; step--)
	{
		uint8_t it = 0;
		for(uint16_t j = 0; j< 4*s;j++) {
		  it = j%4 + 2;

		  PORTD = 1<<it;
		  shiftOutFast(dataPin, clockPin, MSBFIRST, down[step][j%4]);
		}
	}


}


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



void loopLayers() {

	uint16_t all =  0xffff;
	for(uint16_t step = 0; step< 5; step++)
	{

		uint8_t it = 0;
		// total time 15 us ==  0.015ms --> 1/0.025
		for(uint8_t j = 0; j< 65;j++) {
		  it = step%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, all);
		  //delay(10);
		}
	}
}


void loopOneByOne() {

	uint8_t order[16]  = {0,1,2,3,4,5,6,7,11,10,9,8,12,13,14,15};

	uint16_t layers[4]  = {0,0,0,0};
	uint8_t it = 0;
	uint8_t layer = 0;
	for(uint8_t l = 0; l < 4; l++) {
		for(uint8_t o = 0; o < 16; o++) {
			uint8_t led;
			if (l%2 == 0)
				led = order[o];
			else
				led = order[15-o];
			//uint8_t layer_led = led%16;

			layers[layer] |= 1<<led;

			for(uint8_t j = 0; j< 60;j++) {
			  it = j%4 + 2;

			  PORTD = 1<<it;

			  shiftOutFast(dataPin, clockPin, MSBFIRST, layers[j%4]);
			}
		}
		layer++;
	}

}

void loopTopWalls() {


	const uint16_t steps[4*12] = {
		// frame 0
		ALL_VERTEX,
		0,
		0,
		0,
		// frame 1
		ROW0 | ROW1 | ROW2,
		ROW0,
		0,
		0,
		// frame 2
		ROW0 | ROW1,
		ROW0,
		ROW0,
		0,
		// frame 3
		ROW0,
		ROW0,
		ROW0,
		ROW0,


		// frame 4
		0,
		ROW0,
		ROW0,
		ROW0 | ROW1,

		// frame 5
		0,
		0,
		ROW0,
		ROW0 | ROW1 | ROW2,

		// frame 6
		0,
		0,
		0,
		ROW0 | ROW1 | ROW2 | ROW3,

		// frame 7
		0,
		0,
		ROW3,
		ROW3 | ROW2 | ROW1,


		// frame 8
		0,
		ROW3,
		ROW3,
		ROW3 | ROW2,

		// frame 9
		ROW3,
		ROW3,
		ROW3,
		ROW3,


		// frame 10
		ROW3|ROW2,
		ROW3,
		ROW3,
		0,

		// frame 11
		ROW3|ROW2|ROW1,
		ROW3,
		0,
		0,

/*
		// frame 12
		ROW3|ROW2|ROW1|ROW0,
		0,
		0,
		0,
*/
	};


	for(uint16_t step = 0; step< 12*5; step++) {


		uint8_t current_step = step%12;
		uint8_t it = 0;
		// total time 15 us ==  0.015ms --> 1/0.025
		for(uint8_t j = 0; j< 65;j++) {
		  it = j%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, steps[current_step*4 + j%4]);
		}
	}
}

void loopOutterWalls() {

	uint16_t border = 0;
	for(uint16_t step = 0; step< 24; step++) {
		//bitWrite(oout, i, HIGH);
		border = 1<<outer[step%12];

		border |= 1<<outer[(12 +step-3)%12];
		border |= 1<<outer[(12 +step-2)%12];
		border |= 1<<outer[(12 +step-1)%12];

		/*
		if (step >= 3)
			border |= 1<<outer[step-3];

		if (step >= 2)
			border |= 1<<outer[step-2];
		if (step >= 1)
			border |= 1<<outer[step-1];
		*/


		uint8_t it = 0;
		// total time 15 us ==  0.015ms --> 1/0.025
		for(uint8_t j = 0; j< 40;j++) {
		  it = j%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, border);
		}
	}
}

void loopCubelets() {

	uint8_t n =  22;
	uint8_t edges[22]  = {13,14,15,11,11,7, 6, 6, 10, 10, 14, 14, 13,13, 14, 15, 11, 11, 15, 15, 14, 13 };
	uint8_t layers[22] = {0, 0, 0, 0, 1, 1, 1, 2, 2,  1,  1,  0,  0, 1,  2,  2,  2, 1,   1,  0,  0,  0};
	uint8_t aux = 0;
	for(uint8_t step = 0; step< n; step++) {
		uint8_t idx = step %n;
		uint16_t vertex = cubelet(edges[idx]);

		for(uint8_t j = 0; j< 100;j++) {
			 aux = layers[idx] + j%2;
			 PORTD = 1<<(aux+2);
			 shiftOutFast(dataPin, clockPin, MSBFIRST, vertex);
		}
	}
}




void loopExpandingCube() {

	uint16_t edges[6]  = {
		1<<15,
		1<<15|1<<14|1<<10|1<<11,
		1<<15|1<<14|1<<13| 1<<9|1<<10|1<<11 | 1<<7|1<<6 | 1<<5,
		0xffff,
		1<<15|1<<14|1<<13| 1<<9|1<<10|1<<11 | 1<<7|1<<6 | 1<<5,
		1<<15|1<<14|1<<10|1<<11
	};
	uint8_t layers[6] = {
		0,1,2,3,2,1
	};

	uint8_t aux 	=  15;
	for(uint8_t step = 0; step< 6; step++) {
		uint8_t num_layers = layers[step]+1;
		uint8_t n = 150;
		//Serial.println("  aaaa");
		for(uint8_t j = 0; j< n;j++) {
		 aux = j%num_layers;

		 //aux |= 1<<(j%(step+1)2);
		 PORTD = 1<<(aux+2);
		 shiftOutFast(dataPin, clockPin, MSBFIRST, edges[step]);
		}
	}
}


void loopRandom() {

	for(uint8_t step = 0; step< 50; step++)
	{
		uint16_t all = random(65535);
		uint8_t it = 0;
		// total time 15 us ==  0.015ms --> 1/0.025
		for(uint8_t j = 0; j< 10;j++) {
		  it = step%4 + 2;

		  // PORTD = B10101000;  // sets digital pins 7,5,3 HIGH
		  PORTD = 1<<it;

		  shiftOutFast(dataPin, clockPin, MSBFIRST, all);
		  //delay(10);
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

void shiftOutFastStd(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val)
{
	// pin digital eight 8, PB0
	uint16_t latch_pin_on = 1<<0;
	PORTB = PORTB & LATCH_PIN_OFF;
	shiftOutFast(dataPin, clockPin, bitOrder, val);
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

	PORTB = PORTB & LATCH_PIN_OFF;
	_shiftOutFast(dataPin, clockPin, bitOrder, 0);
	PORTB = PORTB | LATCH_PIN_ON;
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

