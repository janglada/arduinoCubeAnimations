/*
 * Animations.h
 *
 *  Created on: Jan 19, 2016
 *      Author: joan
 */
#include "Constants.h"
#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

struct Anim {
	void (*fn) (int);
	int total_steps = 0;
	int step = 0;

};


void layers(int step) {
	step = step%6;
	if (step >=4)
		step = 6 -step;
	for(uint8_t i = 0; i< 4; i++)
		m_layers[i] = step == i? ALL_VERTEX : 0;

}

void wallsStep1(int step) {
	step = step%4;
	m_layers[0] = walls1[step];
	m_layers[1] = walls1[step];
	m_layers[2] = walls1[step];
	m_layers[3] = walls1[step];
}

void wallsStep2(int step) {
	step = step%4;
	m_layers[0] = walls2[step];
	m_layers[1] = walls2[step];
	m_layers[2] = walls2[step];
	m_layers[3] = walls2[step];
}

void topBottomWalls(int step) {
	uint8_t s = 4*(step%12);


	m_layers[0] = loop_top_walls[s];
	m_layers[1] = loop_top_walls[s+1];
	m_layers[2] = loop_top_walls[s+2];
	m_layers[3] = loop_top_walls[s+3];

}

void rotateUp(int step) {
	uint8_t s = step%13;
	if (s >= 7) {
		s = 13 -s;
	}
	m_layers[0] = rotate_up[s][0];
	m_layers[1] = rotate_up[s][1];
	m_layers[2] = rotate_up[s][2];
	m_layers[3] = rotate_up[s][3];
}

void rotateDown(int step) {
	uint8_t s = step%13;
	if (s >= 7) {
		s = 13 -s;
	}
	m_layers[0] = rotate_down[s][0];
	m_layers[1] = rotate_down[s][1];
	m_layers[2] = rotate_down[s][2];
	m_layers[3] = rotate_down[s][3];

}

void wallsOutter(int step) {
	uint16_t border = 1<<outer_walls[step%12];

	border |= 1<<outer_walls[(12 +step-3)%12];
	border |= 1<<outer_walls[(12 +step-2)%12];
	border |= 1<<outer_walls[(12 +step-1)%12];

	m_layers[0] = border;
	m_layers[1] = border;
	m_layers[2] = border;
	m_layers[3] = border;

}

void rotateCenter(int step) {
	uint8_t s = step%6;

	m_layers[0] = rotate_center[s];
	m_layers[1] = rotate_center[s];
	m_layers[2] = rotate_center[s];
	m_layers[3] = rotate_center[s];
}

void oneByOne(int step) {
	step = step%64;
	if (step == 0) {
		m_layers[0] = 0;
		m_layers[1] = 0;
		m_layers[2] = 0;
		m_layers[3] = 0;
	}
	for(uint8_t i = 0; i<= step; i++) {
		uint8_t layer 	= step/16;
		uint8_t led 	= step%16;

		if (layer%2 == 1)
			led = one_by_one_order[led];
		else
			led = one_by_one_order[15-led];


		m_layers[layer] = m_layers[layer] | 1<<led;
	}
}


void randomCube(int step) {
	m_layers[0] = random(ALL_VERTEX);
	m_layers[1] = random(ALL_VERTEX);
	m_layers[2] = random(ALL_VERTEX);
	m_layers[3] = random(ALL_VERTEX);
	 //1024;//2048;
}

void blink(int step) {
	bool on = step%3==0;
	if (on) {
		m_layers[0] = ALL_VERTEX;
		m_layers[1] = ALL_VERTEX;
		m_layers[2] = ALL_VERTEX;
		m_layers[3] = ALL_VERTEX;
	} else {
		m_layers[0] = 0;
		m_layers[1] = 0;
		m_layers[2] = 0;
		m_layers[3] = 0;

	}
}


uint16_t cubelet(int vertex) {
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

void loopCubelets(int step) {

	uint8_t n =  cubelets_animation_length;

	uint8_t aux = 0;

	uint8_t idx = step %n;
	uint16_t vertex = cubelet(cubelets_edges[idx]);
	m_layers[0] = 0;
	m_layers[1] = 0;
	m_layers[2] = 0;
	m_layers[3] = 0;

	m_layers[cubelets_layers[idx]]   = vertex;
	m_layers[cubelets_layers[idx]+1] = vertex;

}

void expandingCube(int step) {

	m_layers[0] = 0;
	m_layers[1] = 0;
	m_layers[2] = 0;
	m_layers[3] = 0;

	step = step%6;

	uint8_t num_layers = expanding_cube_layers[step]+1;
		//Serial.println("  aaaa");
	for(uint8_t j = 0; j< num_layers;j++) {
		m_layers[j] = expanding_cube_edges[step];
	}
}

void expandingCubeEmpty(int step) {
	step = step%9;

	if (step >= 5) {
		step = 9 -step;
	}

	m_layers[0] = expanding_cube_edges_empty[step*4];
	m_layers[1] = expanding_cube_edges_empty[step*4+1];
	m_layers[2] = expanding_cube_edges_empty[step*4+2];
	m_layers[3] = expanding_cube_edges_empty[step*4+3];
}


void rain(int step) {
	if (step == 0) {
		m_layers[0] = 0;
		m_layers[1] = 0;
		m_layers[2] = 0;
		m_layers[3] = 0;
	}
	if (step % 2 == 0) {
		m_layers[3] = m_layers[2];
		m_layers[2] = m_layers[1];
		m_layers[1] = m_layers[0];
		m_layers[0] = step%4 ==0? 0: (1<<random(16))|(1<<random(16))|(1<<random(16))|(1<<random(16))|(1<<random(16))|(1<<random(16));
	}
}

void goingUp(int step) {
	step = step%19;
	if (step == 0) {
		m_layers[0] = 0;
		m_layers[2] = 0;
		m_layers[3] = ALL_VERTEX;
	}
	if (step < 16) {
		int16_t led 	= 1<<going_up_order[step];
		m_layers[0] |= m_layers[1];
		m_layers[1] = m_layers[2];
		m_layers[2] = led;
		m_layers[3] &= (~led);
	} else {
		m_layers[0] |= m_layers[1];
		m_layers[1] = m_layers[2];
		m_layers[2] = m_layers[3];
		m_layers[3] = 0;
	}


}


#endif /* ANIMATIONS_H_ */
