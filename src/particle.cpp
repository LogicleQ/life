#include "life.hpp"

#include <cmath>
#include <vector>
#include <SDL2/SDL.h>

#define PI (3.14159265358979)

Particle::Particle(int type, SDL_FPoint pos, float radius, float scale){
	this->pos = pos;
	this->radius = radius;
	this->type = type;

	//TEMP
	//velX = ((float) rand()-((float) RAND_MAX/2)) / RAND_MAX / 5;
	//velY = ((float) rand()-((float) RAND_MAX/2)) / RAND_MAX / 5;
	velX = 0;
	velY = 0;

	prevVelX = 0;
	prevVelY = 0;


	recomputeVerticies(scale);
}

#define ITER 15

int Particle::indices[ITER*3];
bool Particle::indicesSet = false;

void Particle::recomputeVerticies(float scale){

	if(!indicesSet){
		indicesSet = true;

		for(int i = 0; i < ITER; ++i){
			indices[i*3] = 0;
			indices[i*3+1] = i+1;
			indices[i*3+2] = i+2;
		}

		indices[ITER*3-1] = 1;
	}

	points = std::vector<SDL_Vertex>();

	SDL_FPoint newPos{pos.x*scale, pos.y*scale};
	SDL_Color color = getTypeColor(type);

	points.push_back(SDL_Vertex{newPos, color});

	//compute verticies to draw the circle
	for(int i = 0; i < ITER; ++i){
		float x = newPos.x + radius * scale * cos(2*PI/ITER * i);
		float y = newPos.y + radius * scale * sin(2*PI/ITER * i);

		points.push_back(SDL_Vertex{SDL_FPoint{x, y}, color});
	}
}

std::vector<SDL_Color> Particle::typeColors;

void Particle::render(SDL_Renderer* renderer){
	SDL_RenderGeometry(renderer, NULL, &(points[0]), points.size(), indices, ITER*3);
}

void Particle::resetTypeColors(){
	typeColors = std::vector<SDL_Color>();

	for(int i = 0; i < PARTICLE_TYPE_COUNT; ++i){
		#define randByte (uint8_t) (((float) rand() / RAND_MAX * 226) + 30)
		typeColors.push_back(SDL_Color{randByte, randByte, randByte, 255});
	}
}

SDL_Color Particle::getTypeColor(int type){
	if(typeColors.size() == 0) resetTypeColors();

	return typeColors.at(type);
}
