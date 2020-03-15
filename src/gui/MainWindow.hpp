#pragma once
#include <gtkmm.h>
#include "PreviewArea.hpp"
#include "nox-gui_export.h"

namespace nox::gui
{
    class NOX_GUI_EXPORT MainWindow : public Gtk::Window
	{

	public:
        MainWindow();
		virtual ~MainWindow();

	protected:
        void createMenu();

		//Signal handlers:
		virtual void onButtonClicked();
        virtual void onFileOpen();

		//Member widgets:
        Gtk::VBox m_vbox;
        Gtk::HBox m_hbox;
        Gtk::MenuBar m_menuBar;
        Gtk::MenuItem m_menuFile;
        Gtk::MenuItem m_menuEdit;
        Gtk::MenuItem m_menuView;
        Gtk::MenuItem m_menuWindow;
        Gtk::MenuItem m_menuHelp;

        Gtk::Menu m_subMenuFile;
        Gtk::Menu m_subMenuEdit;
        Gtk::Menu m_subMenuView;
        Gtk::Menu m_subMenuWindow;
        Gtk::Menu m_subMenuHelp;

        Gtk::MenuItem m_menuFileNew;
        Gtk::SeparatorMenuItem m_separator1;
        Gtk::MenuItem m_menuFileOpen;
        Gtk::MenuItem m_menuFileClose;

        Gtk::MenuItem m_menuEditUndo;
        Gtk::MenuItem m_menuEditRedo;
        Gtk::SeparatorMenuItem m_separator2;
        Gtk::MenuItem m_menuEditCut;
        Gtk::MenuItem m_menuEditCopy;
        Gtk::MenuItem m_menuEditPaste;
        Gtk::MenuItem m_menuEditDelete;
        Gtk::SeparatorMenuItem m_separator3;
        Gtk::MenuItem m_menuEditSelectAll;

        nox::gui::PreviewArea m_previewArea;
        Gtk::FlowBox m_flowBox;
	};
}