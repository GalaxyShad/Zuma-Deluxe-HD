#include "FX.h"

void Message_Init(Message* msg, char* text, float x, float y, char font, char color) {
	msg->x = x;
	msg->y = y;
	msg->color = color;
	msg->font = font;
	sprintf(msg->text, "%s", text);

	msg->onScreen = true;
	msg->spd = MSG_SPD;
	msg->spawnTime = clock();
}

void Message_Draw(Message* msg) {
	SDL_Color c = {255, 255, 255, 255};
	switch (msg->color) {
		case 0: c.r = 0x00; c.g = 0xA9; c.b = 0xFF; break; // Blue
		case 1: c.r = 0x00; c.g = 0xFF; c.b = 0x00; break; // Green
		case 2: c.r = 0xFF; c.g = 0xFF; c.b = 0x00; break; // Yellow
		case 3: c.r = 0xFF; c.g = 0x00; c.b = 0x00; break; // Red
		case 4: c.r = 0xFF; c.g = 0x00; c.b = 0xFF; break; // Violet
		case 5: c.r = 0xFF; c.g = 0xFF; c.b = 0xFF; break; // Grey
	}

	Engine_DrawTextExt(msg->text, msg->font, c, 
		true, false, msg->x, msg->y);
}

void Messages_NewMsg(Messages* messages, char* text, float x, float y, char font, char color) {
	for (int i = 0; i < messages->len; i++) {
		if (!messages->msgs[i].onScreen) {
			Message_Init(&messages->msgs[i], text, x, y, font, color);
			return;
		}
	}
	Message_Init(&messages->msgs[messages->len], text, x, y, font, color);
	messages->len++;
}

void Messages_Update(Messages* messages) {
	clock_t t = clock();
	for (int i = 0; i < messages->len; i++) {
		if (!messages->msgs[i].onScreen) continue;

    	float delta = ((float)(t - messages->msgs[i].spawnTime)) / CLOCKS_PER_SEC;
    	if (delta > MSG_LIFETIME) messages->msgs[i].onScreen = 0;
    	else {
    		messages->msgs[i].y -= messages->msgs[i].spd;
    	}
	}
}

void Messages_Draw(Messages* messages) {
	for (int i = 0; i < messages->len; i++) {
		if (!messages->msgs[i].onScreen) 
			continue;
		Message_Draw(&messages->msgs[i]);
	}
}

void Partical_Init(Particle* particle, float x, float y) {
	particle->x = x;
	particle->y = y;
	particle->frame = 0.0;
	particle->onScreen = true;
}

void Particles_NewParticle(Particles* particles, float x, float y) {
	for (int i = 0; i < particles->len; i++) {
		if (!particles->prtcls[i].onScreen) {
			Partical_Init(&particles->prtcls[i], x, y);
			return;
		}
	}
	Partical_Init(&particles->prtcls[particles->len], x, y);
	particles->len++;
}

void Particles_Update(Particles* particles, float lastFrame, float spd) {
	for (int i = 0; i < particles->len; i++) {
		if (!particles->prtcls[i].onScreen) continue;

		if (particles->prtcls[i].frame > lastFrame) particles->prtcls[i].onScreen = false;
		else
			particles->prtcls[i].frame += spd;
	}
}

void Particles_Draw(Particles* particles) {
	for (int i = 0; i < particles->len; i++) {
		if (!particles->prtcls[i].onScreen) continue;

    	Animation_SetFrame(&particles->anim, (int)particles->prtcls[i].frame);
    	Animation_Draw(&particles->anim, particles->prtcls[i].x, particles->prtcls[i].y);
	}
}