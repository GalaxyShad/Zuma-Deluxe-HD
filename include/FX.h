#ifndef MSG_H
#define MSG_H

#include <Engine.h>
#include <Enums.h>
#include <Animation.h>
#include <time.h>

#define MSG_LIFETIME 1
#define MSG_SPD 2
#define MAX_MSG_LEN 32
#define MAX_MSGS_ARRAY_LEN 256

typedef struct _Message {
	char text[MAX_MSG_LEN], font;
	char color;
	bool onScreen;
	float x, y;
	float spd;
	clock_t spawnTime;
} Message;

typedef struct _Messages {
	Message msgs[MAX_MSGS_ARRAY_LEN];
	int len;
} Messages;

void Message_Init(Message*, char*, float, float, char, char);

void Messages_NewMsg(Messages*, char*, float, float, char, char);
void Messages_Update(Messages*);
void Messages_Draw(Messages*);

typedef struct _Particle {
	bool onScreen;
	float frame;
	float x, y;
} Particle;

#define EXPLOSION_ANIM_SPEED 0.5
#define EXPLOSION_MOVE_SPEED 12
#define EXPLOSIONS_PAD 50
#define EXPLOSION_LAST_FRAME 16.0

#define SPARKLES_ANIM_SPEED 0.5
#define SPARKLES_MOVE_SPEED 15
#define SPARKLES_PAD 0
#define SPARKLE_LAST_FRAME 13.0

typedef struct _Particles {
	Particle prtcls[MAX_MSGS_ARRAY_LEN];
	Animation anim;
	float prevPos, pos, x, y;
	int len;
} Particles;

void Particle_Init(Particle*, float, float);
void Particles_NewParticle(Particles*, float, float);

void Particles_Update(Particles*, float, float);
void Particles_Draw(Particles*);



#endif