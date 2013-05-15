#include <cstdlib>
#include <boost\thread\thread.hpp>
#include <boost\thread\mutex.hpp>
#include <boost\thread\condition.hpp>
#include "sSound.h"
#include "sInput.h"
#include "GameWorld.h"

using namespace std;
SDL_OpenGL_Graphics gfx;
SDL_Input in;

boost::mutex iom_mutex;

class Fata
{
public:
	void Render()
	{
		while(1)
		{
			{
			boost::mutex::scoped_lock lock(iom_mutex);
			cout <<"Render Thread running:" << endl;
			}

			boost::system_time timeout(boost::get_system_time() + boost::posix_time::milliseconds(2));
			boost::this_thread::sleep(timeout);
		}
	}


	void Update()
	{
		while(1)
		{
			{
			boost::mutex::scoped_lock lock(iom_mutex);
			cout <<"Update Thread running:" << endl;
			}

			boost::system_time timeout(boost::get_system_time() + boost::posix_time::milliseconds(1));
			boost::this_thread::sleep(timeout);
		}
	}
};

int main ( int argc, char** argv )
{
	static GameWorld game;
	
	boost::thread render_thread(&GameWorld::Render, game);
	boost::thread input_event_generator_thread(&GameWorld::InputGenerator, game);
	boost::thread input_event_handler_thread(&GameWorld::InputEventHandler, game);
	boost::thread update_thread(&GameWorld::Update, game);

	render_thread.join();
	input_event_generator_thread.join();
	input_event_handler_thread.join();
	update_thread.join();
	   
    return 0;
}
