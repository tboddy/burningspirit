// controls

struct controls {
	bool left, right, up, down, a, b, c, start;
};

struct controls ctrl;

void updateControls(u16 joy, u16 changed, u16 state){
	if(joy == JOY_1){
		if(changed){}
		ctrl.left = (state & BUTTON_LEFT);
		ctrl.right = (state & BUTTON_RIGHT);
		ctrl.up = (state & BUTTON_UP);
		ctrl.down = (state & BUTTON_DOWN);
		ctrl.a = (state & BUTTON_A);
		ctrl.b = (state & BUTTON_B);
		ctrl.c = (state & BUTTON_C);
		ctrl.start = (state & BUTTON_START);
	}
}