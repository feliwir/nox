#pragma once
#include <gtkmm.h>

namespace nox::gui
{
	class PreviewArea : public Gtk::GLArea
	{
	public:
		PreviewArea();
		virtual ~PreviewArea();
	protected:
		virtual void on_realize() override;
		virtual void on_unrealize() override;
		virtual bool on_render(const Glib::RefPtr<Gdk::GLContext>& context);
	private:
		sigc::connection m_realizeConn;
		sigc::connection m_unrealizeConn;
		sigc::connection m_renderConn;
	};
}