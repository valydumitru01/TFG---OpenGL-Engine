#include "core/TFGL.h"
#include "SDL2/SDL.h"
#include "core/FPS.h"

int SDL_main() {
	TFGL tfgl;
	FPS fps(FPS_rates::fps_60);
	Console::init();
	
	while (tfgl.running) {
		fps.timeFrame();
		
		tfgl.processInput();
		while (fps.isLagged()) // Update executes in constant intervals
		{
			tfgl.update();
			fps.updateLag();
		}
		
		//Render execute arbitrarily, depending on how much time update() takes
		tfgl.render(fps.getTimeOfFrameAfterUpdate());
	}
	return 0;
}