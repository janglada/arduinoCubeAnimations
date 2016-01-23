/*
 * GraphObjects.h
 *
 *  Created on: Jan 19, 2016
 *      Author: joan
 */

#ifndef GRAPHOBJECTS_H_
#define GRAPHOBJECTS_H_

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




#endif /* GRAPHOBJECTS_H_ */
