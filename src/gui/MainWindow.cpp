#include "MainWindow.hpp"

#include <iostream>

#include "Media.hpp"

using namespace nox::gui;

MainWindow::MainWindow()
{
    add(m_vbox);
    createMenu();
    m_hbox.add(m_flowBox);
    m_hbox.add(m_previewArea);

    set_title("Nox VideoEditor");
    resize(300, 300);
    show_all();
}

MainWindow::~MainWindow() = default;

void MainWindow::createMenu()
{
    m_vbox.pack_start(m_menuBar, Gtk::PACK_SHRINK);
    m_vbox.pack_end(m_hbox);

    m_menuFile.set_label("File");
    m_menuBar.append(m_menuFile);

    m_menuEdit.set_label("Edit");
    m_menuBar.append(m_menuEdit);

    m_menuView.set_label("View");
    m_menuBar.append(m_menuView);

    m_menuWindow.set_label("Window");
    m_menuBar.append(m_menuWindow);

    m_menuHelp.set_label("Help");
    m_menuBar.append(m_menuHelp);

    m_menuFile.set_submenu(m_subMenuFile);
    m_menuEdit.set_submenu(m_subMenuEdit);
    m_menuView.set_submenu(m_subMenuView);
    m_menuWindow.set_submenu(m_subMenuWindow);
    m_menuHelp.set_submenu(m_subMenuHelp);

    m_subMenuFile.append(m_menuFileNew);
    m_subMenuFile.append(m_menuFileOpen);
    m_subMenuFile.append(m_separator1);
    m_subMenuFile.append(m_menuFileClose);

    m_subMenuEdit.append(m_menuEditUndo);
    m_subMenuEdit.append(m_menuEditRedo);
    m_subMenuEdit.append(m_separator2);
    m_subMenuEdit.append(m_menuEditCut);
    m_subMenuEdit.append(m_menuEditCopy);
    m_subMenuEdit.append(m_menuEditPaste);
    m_subMenuEdit.append(m_menuEditDelete);
    m_subMenuEdit.append(m_separator3);
    m_subMenuEdit.append(m_menuEditSelectAll);

    m_menuFileNew.set_label("_New");
    m_menuFileNew.set_use_underline(true);
    m_menuFileNew.Gtk::Widget::add_accelerator(
        "New", get_accel_group(), 'n', Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
    m_menuFileNew.signal_activate().connect(
        [&]() { std::cout << "MainMenu/File/New" << std::endl; });

    m_menuFileOpen.set_label("_Open");
    m_menuFileOpen.set_use_underline(true);
    m_menuFileOpen.signal_activate().connect(
        sigc::mem_fun(*this, &MainWindow::onFileOpen));

    m_menuFileClose.set_label("_Close");
    m_menuFileClose.set_use_underline(true);
    m_menuFileClose.signal_activate().connect(
        [&]() { std::cout << "MainMenu/File/Close" << std::endl; });

    m_menuEditUndo.set_label("_Undo");
    m_menuEditUndo.set_use_underline(true);
    m_menuEditUndo.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/Undo" << std::endl; });

    m_menuEditRedo.set_label("_Redo");
    m_menuEditRedo.set_use_underline(true);
    m_menuEditRedo.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/Redo" << std::endl; });

    m_menuEditCut.set_label("C_ut");
    m_menuEditCut.set_use_underline(true);
    m_menuEditCut.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/Cut" << std::endl; });

    m_menuEditCopy.set_label("_Copy");
    m_menuEditCopy.set_use_underline(true);
    m_menuEditCopy.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/Copy" << std::endl; });

    m_menuEditPaste.set_label("_Paste");
    m_menuEditPaste.set_use_underline(true);
    m_menuEditPaste.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/Paste" << std::endl; });

    m_menuEditDelete.set_label("_Delete");
    m_menuEditDelete.set_use_underline(true);
    m_menuEditDelete.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/Delete" << std::endl; });

    m_menuEditSelectAll.set_label("Select _All");
    m_menuEditSelectAll.set_use_underline(true);
    m_menuEditSelectAll.signal_activate().connect(
        [&]() { std::cout << "MainMenu/Edit/SelectAll" << std::endl; });
}

void MainWindow::onButtonClicked()
{
    std::cout << "Hello World" << std::endl;
}

void MainWindow::onFileOpen()
{
    Gtk::FileChooserDialog dialog("Please choose a file",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    // Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    // Add filters, so that only certain file types can be selected:

    auto filter_movie = Gtk::FileFilter::create();
    filter_movie->set_name("Matroska files");
    filter_movie->add_mime_type("video/x-matroska");
    dialog.add_filter(filter_movie);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    // Show the dialog and wait for a user response:
    int result = dialog.run();

    // Handle the response:
    switch (result)
    {
        case (Gtk::RESPONSE_OK):
        {
            std::cout << "Open clicked." << std::endl;

            // Notice that this is a std::string, not a Glib::ustring.
            std::string filename = dialog.get_filename();
            std::cout << "File selected: " << filename << std::endl;

            auto media = std::make_shared<nox::core::Media>(filename);
            break;
        }
        case (Gtk::RESPONSE_CANCEL):
        {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default:
        {
            std::cout << "Unexpected button clicked." << std::endl;
            break;
        }
    }
}