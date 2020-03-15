#include "PreviewArea.hpp"
#include <iostream>
#include <gl/glew.h>
#include <gl/wglew.h>

nox::gui::PreviewArea::PreviewArea()
{
	set_required_version(3, 3); //your desired gl version
	set_has_window(false);
}

nox::gui::PreviewArea::~PreviewArea()
{

}

void nox::gui::PreviewArea::on_realize()
{
	Gtk::Widget::on_realize();

	make_current();
	//This point you have the context and you can use opengl methods.
	glClearColor(.7f, .7f, 0.7f, 1.0f);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}

	err = wglewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}

	if (WGLEW_NV_DX_interop2)
	{
		std::cout << "Found WGLEW_NV_DX_interop2 extension" << std::endl;
	}
}

void nox::gui::PreviewArea::on_unrealize()
{
	Gtk::Widget::on_unrealize();
}

bool nox::gui::PreviewArea::on_render(const Glib::RefPtr<Gdk::GLContext>& context)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}
