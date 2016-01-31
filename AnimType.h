/*
 * AnimType.h
 *
 *  Created on: Jan 31, 2016
 *      Author: joan
 */

#ifndef ANIMTYPE_H_
#define ANIMTYPE_H_

typedef struct AnimType {
	int steps;
    int prescaleer;
    void (*fn) (int step);
} AnimType;

#endif /* ANIMTYPE_H_ */
