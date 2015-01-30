/*
 * ChartAnalyzer.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: dimalit
 */

#include "ChartAnalyzer.h"
#include <google/protobuf/message.h>

#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/button.h>
#include <gtkmm/messagedialog.h>

#include <iostream>
#include <cstdio>

using namespace google::protobuf;

#define UI_FILE "charts_add.glade"

class ChartAddDialog: public Gtk::Window{
private:
	Gtk::TreeView *treeview;
	Glib::RefPtr<Gtk::ListStore> store;
	Gtk::Button *btn_ok, *btn_cancel;
	ChartAnalyzer* parent;

public:
	ChartAddDialog(ChartAnalyzer* parent, const OdeState* state){
		assert(parent && state);
		this->parent = parent;
		this->set_modal(true);

		Glib::RefPtr<Gtk::Builder> b = Gtk::Builder::create_from_file(UI_FILE);

		Gtk::Widget* root;
		b->get_widget("root", root);

		b->get_widget("treeview1", treeview);
		b->get_widget("btn_ok", btn_ok);
		b->get_widget("btn_cancel", btn_cancel);
		store = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(b->get_object("liststore1"));

		Gtk::CellRendererToggle *cr;
		cr = dynamic_cast<Gtk::CellRendererToggle*>(treeview->get_column(0)->get_first_cell());
			cr->signal_toggled().connect(sigc::mem_fun(*this, &ChartAddDialog::on_use_clicked));
		cr = dynamic_cast<Gtk::CellRendererToggle*>(treeview->get_column(2)->get_first_cell());
			cr->signal_toggled().connect(sigc::mem_fun(*this, &ChartAddDialog::on_x_clicked));

		btn_ok->signal_clicked().connect(sigc::mem_fun(this, &ChartAddDialog::on_ok_clicked));
		btn_cancel->signal_clicked().connect(sigc::mem_fun(this, &ChartAddDialog::on_cancel_clicked));

		this->add(*root);

		// now add state's variables to the table
		store->clear();
		const google::protobuf::Message* msg = dynamic_cast<const google::protobuf::Message*>(state);
		const Descriptor* desc = msg->GetDescriptor();
		const Reflection* refl = msg->GetReflection();

		for(int i=0; i<desc->field_count(); i++){
			const FieldDescriptor* fd = desc->field(i);
			// add simple
			if(fd->type() == FieldDescriptor::TYPE_DOUBLE){
				Gtk::ListStore::iterator it = store->append();
				it->set_value(0, false);
				it->set_value(1, Glib::ustring(fd->name()));
				it->set_value(2, false);
			}
			// add message
			else if(fd->type() == FieldDescriptor::TYPE_MESSAGE || fd->type() == FieldDescriptor::TYPE_GROUP){
				const Message& m2 = fd->is_repeated() ? refl->GetRepeatedMessage(*msg, fd, 0) : refl->GetMessage(*msg, fd);

				const Descriptor* d2 = m2.GetDescriptor();
				for(int i=0; i<d2->field_count(); i++){
						const FieldDescriptor* fd2 = d2->field(i);
						if(fd2->type() != FieldDescriptor::TYPE_DOUBLE)
							continue;

						Gtk::ListStore::iterator it = store->append();
						it->set_value(0, false);
						it->set_value(1, Glib::ustring(fd->name())+"."+Glib::ustring(fd2->name()));
						it->set_value(2, false);
				}// for
			}// else
		}// for
	}

private:
	void on_ok_clicked(){
		std::vector<std::string> vars;
		std::string x_axis_var;

		// check all repeated or all non-repeated
		bool has_repeated = false, has_non_repeated = false;

		Gtk::ListStore::Children children = store->children();
		for(Gtk::ListStore::const_iterator i = children.begin(); i!=children.end(); ++i){

			// parse added vars
			bool use;
			i->get_value(0, use);
			if(use){
				Glib::ustring us;
				i->get_value(1, us);
				vars.push_back(us.raw());

				if(us.raw().find('.') == std::string::npos)
					has_non_repeated = true;
				else
					has_repeated = true;
			}

			// parse x var
			bool as_x;
			i->get_value(2, as_x);
			if(as_x){
				Glib::ustring us;
				i->get_value(1, us);
				x_axis_var = us.raw();
			}// if as x
		}// for

		if(x_axis_var.find('.') != std::string::npos)
			has_repeated = true;
		else if(!x_axis_var.empty())
			has_non_repeated = true;

		// must have at least something
		if(vars.size() == 0){
			Gtk::MessageDialog dlg("ERROR: Please select vars to add to the chart!", false, Gtk::MessageType::MESSAGE_ERROR, Gtk::ButtonsType::BUTTONS_OK, true);
			dlg.run();
			return;
		}

		// all must be either repeated or non-repeated!
		if(has_repeated && has_non_repeated){
			Gtk::MessageDialog dlg("ERROR: Vars must be either all repeated or all non-repeated!", false, Gtk::MessageType::MESSAGE_ERROR, Gtk::ButtonsType::BUTTONS_OK, true);
			dlg.run();
			return;
		}

		parent->addChart(vars, x_axis_var);

		this->hide();
		delete this;
	}
	void on_cancel_clicked(){
		this->hide();
		delete this;		// XXX: can we do so?
	}
	void on_use_clicked(const Glib::ustring& path){
		Gtk::ListStore::iterator it = store->get_iter(path);
		bool val;
		it->get_value(0, val);
		it->set_value(0, !val);
	}
	void on_x_clicked(const Glib::ustring& path){
		Gtk::ListStore::iterator checked = store->children().end();

		// clear all
		for(Gtk::ListStore::iterator it = store->children().begin(); it != store->children().end(); ++it){
			bool v;
			it->get_value(2, v);
			if(v)
				checked = it;

			it->set_value(2, false);
		}

		// set OR UNSET current
		Gtk::ListStore::iterator it = store->get_iter(path);
		if(checked == it)
			it->set_value(2, false);
		else
			it->set_value(2, true);
	}
};

ChartAnalyzer::ChartAnalyzer(const OdeConfig* config) {
	states_count = 0;
	last_state = NULL;

	btn_add.set_label("Add chart");
	vbox.pack_end(btn_add);
	btn_add.signal_clicked().connect(sigc::mem_fun(*this, &ChartAnalyzer::on_add_clicked));

	this->add(vbox);
}

ChartAnalyzer::~ChartAnalyzer() {
	for(int i=0; i<plots.size(); i++)
		delete plots[i];
}

int ChartAnalyzer::getStatesCount(){
	return states_count;
}

void ChartAnalyzer::reset(){
	states_count = 0;
}

void ChartAnalyzer::processState(const OdeState* state, double time){
	this->last_state = state;
	const google::protobuf::Message* msg = dynamic_cast<const google::protobuf::Message*>(state);
		assert(msg);
	for(int i=0; i<plots.size(); i++)
		plots[i]->processState(msg);
}

void ChartAnalyzer::on_add_clicked(){
	assert(last_state);
	(new ChartAddDialog(this, last_state))->show_all();
}

void ChartAnalyzer::addChart(std::vector<std::string> vars, std::string x_axis_var){
	std::ostringstream full_title;

	Gnuplot* p = new Gnuplot();
	for(int i=0; i<vars.size(); i++){
		p->addVar(vars[i]);
		full_title << vars[i] << ' ';
	}// for
	if(!x_axis_var.empty()){
		p->setXAxisVar(x_axis_var);
		full_title << "| " << x_axis_var << " ";
	}

	// set dots if plot is scatter
	if(!x_axis_var.empty())
		p->setStyle(Gnuplot::STYLE_POINTS);

	p->setTitle(full_title.str());
	plots.push_back(p);	// XXX: not very copyable - but with no copies it will work...

	// add widget for it //
	Gtk::Button* btn = new Gtk::Button("del");
	Gtk::Label* l = new Gtk::Label(full_title.str());
	Gtk::HBox* hbox = new Gtk::HBox();
	hbox->pack_end(*Gtk::manage(btn), false, false);
	hbox->pack_end(*Gtk::manage(l), true, true);

	btn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &ChartAnalyzer::on_del_chart_clicked), (Gtk::Widget*)hbox, p));
	vbox.pack_start(*Gtk::manage(hbox), false, false);
	hbox->show_all();

	p->processState(dynamic_cast<const Message*>(this->last_state));
}

void ChartAnalyzer::on_del_chart_clicked(Gtk::Widget* w, const Gnuplot* ptr){
	assert(ptr);
	for(int i=0; i<plots.size(); i++){
		if(plots[i] == ptr){
			delete plots[i];
			plots.erase(plots.begin()+i);
			vbox.remove(*w);
			return;
		}// if
	}// for
}
