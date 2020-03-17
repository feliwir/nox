#pragma once
#include <gtkmm.h>

namespace nox::gui
{
class PreviewArea : public Gtk::GLArea
{
   public:
    PreviewArea();
    ~PreviewArea() override;

   protected:
    void on_realize() override;
    void on_unrealize() override;
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;

   private:
    sigc::connection m_realizeConn;
    sigc::connection m_unrealizeConn;
    sigc::connection m_renderConn;
};
} // namespace nox::gui