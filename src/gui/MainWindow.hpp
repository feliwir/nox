#pragma once
#include <gtkmm.h>

namespace nox::gui
{
	class MainWindow : public Gtk::Window
	{

	public:
		MainWindow();
		virtual ~MainWindow();

	protected:
        void create_menu();

		//Signal handlers:
		virtual void on_button_clicked();
        virtual void on_file_open();

		//Member widgets:
        Gtk::VBox m_vbox;
        Gtk::ScrolledWindow m_scrolledWindow;
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
	};
}