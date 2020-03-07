#include <gtkmm.h>
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.feliwir.nox");

	nox::gui::MainWindow window;
	window.set_default_size(400, 400);

	return app->run(window);
}