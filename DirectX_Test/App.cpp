#include "App.h"
#include <sstream>
#include <iomanip>

App::App() : wnd(1500, 1000, "My Window") {}

int App::Go() {
	while( true ) {
		if( const auto ecode = Window::ProcessMessages() ) {
			return *ecode;
		}
		DoFrame(); //Is this happening at 60Hz? Where is the framerate set?
	}
}

void App::DoFrame() {
	const float c = sin( timer.Peek() ) / 2.0f + 0.5f;
	const float d = cos( timer.Peek() ) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer( c, d, c );
	wnd.Gfx().EndFrame();
}