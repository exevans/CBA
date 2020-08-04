#ifndef ANIMATION_DIRECTIONAL_H
#define ANIMATION_DIRECTIONAL_H

#include "Animation.h"

class Anim_Dir : public Animation {
protected:
	void frameStep();
	void cropSprite();
	void readIn(std::stringstream&);
};

#endif