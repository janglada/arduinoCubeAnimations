/*
 * Constants.h
 *
 *  Created on: Jan 19, 2016
 *      Author: joan
 */
#include "GraphObjects.h"
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define ODD  21845
#define EVEN 43690
#define ALL_VERTEX  0xffff


#define ROW3  0b1111000000000000
#define ROW2  0b0000111100000000
#define ROW1  0b0000000011110000
#define ROW0  0b0000000000001111

volatile uint16_t m_layers[4] =  {
	ROW1,ROW3,ROW2,ROW0
};

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


const uint16_t loop_top_walls[4*12] = {
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
	0
};



const uint16_t rotate_down[7][4] = {
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


const uint16_t rotate_up[7][4] = {
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

const uint8_t outer_walls[12] = {
 0, 1, 2, 3, 4, 8, 12, 13, 14, 15, 11, 7
};


const uint16_t rotate_center[6] = {
	(1<<0)  | (1<<6)  | (1<<9)   | (1<<12), // diagonal
	(1<<8)  | (1<<9)  | (1<<6)   | (1<<7),
	(1<<4)  | (1<<5)  | (1<<10)  | (1<<11),
	(1<<3)  | (1<<5)  | (1<<10)  | (1<<15), // diagonal
	(1<<2)  | (1<<5)  | (1<<10)  | (1<<14),
	(1<<1)  | (1<<6)  | (1<<9)   | (1<<13) // 11
};


const uint8_t one_by_one_order[16]  = {0,1,2,3,4,5,6,7,11,10,9,8,12,13,14,15};

const uint8_t cubelets_animation_length =  22;
const uint8_t cubelets_edges[22]  = {13,14,15,11,11,7, 6, 6, 10, 10, 14, 14, 13,13, 14, 15, 11, 11, 15, 15, 14, 13 };
const uint8_t cubelets_layers[22] = {0, 0, 0, 0, 1, 1, 1, 2, 2,  1,  1,  0,  0, 1,  2,  2,  2, 1,   1,  0,  0,  0};

const uint16_t expanding_cube_edges[6]  = {
	1<<15,
	1<<15|1<<14|1<<10|1<<11,
	1<<15|1<<14|1<<13| 1<<9|1<<10|1<<11 | 1<<7|1<<6 | 1<<5,
	0xffff,
	1<<15|1<<14|1<<13| 1<<9|1<<10|1<<11 | 1<<7|1<<6 | 1<<5,
	1<<15|1<<14|1<<10|1<<11
};
//5,6,9,10
const uint16_t expanding_cube_edges_empty[5*4]  = {

	// frame 1
	1<<0,
	0,
	0,
	0,

	// frame 2
	1<<0|1<<1|1<<7|1<<6,
	1<<0|1<<1|1<<7|1<<6,
	0,
	0,

	//frame 3
	1<<0|1<<1|1<<2|1<<7|1<<11|1<<10|1<<9|1<<5|1<<3,
	1<<0|1<<2|1<<11|1<<9|1<<3,
	1<<0|1<<1|1<<2|1<<7|1<<11|1<<10|1<<9|1<<5|1<<3,
	0,

	//frame 4
	ALL_VERTEX & ~(1<<5|1<<6|1<<9|1<<10),
	1<<0|1<<3|1<<12|1<<15,
	1<<0|1<<3|1<<12|1<<15,
	ALL_VERTEX & ~(1<<5|1<<6|1<<9|1<<10),

	// repeat
	ALL_VERTEX & ~(1<<5|1<<6|1<<9|1<<10),
	1<<0|1<<3|1<<12|1<<15,
	1<<0|1<<3|1<<12|1<<15,
	ALL_VERTEX & ~(1<<5|1<<6|1<<9|1<<10),


};


const uint8_t expanding_cube_layers[6] = {
	0,1,2,3,2,1
};

uint8_t snake_size = 6;
const Vertex* fulldata = new Vertex[snake_size];



#endif /* CONSTANTS_H_ */
