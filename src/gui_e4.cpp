/*
 * gui_e3.cpp
 *
 *  Created on: Aug 21, 2014
 *      Author: dimalit
 */

#include "gui_e4.h"

#include <gtkmm/builder.h>
#include <gtkmm/table.h>

#define UI_FILE_STATE "e4_state.glade"
#define UI_FILE_PETSC_SOLVER "e4_petsc_solver.glade"

AbstractConfigWidget::AbstractConfigWidget(Message *msg){
	this->data = msg;

	grid.set_column_spacing(4);
	grid.set_row_spacing(4);
	grid.set_margin_left(4);
	grid.set_margin_top(4);
	grid.set_margin_right(4);
	grid.set_margin_bottom(4);
	this->add(grid);

	button_apply.set_label("Apply");
	button_apply.set_hexpand(true);
	button_apply.signal_clicked().connect(sigc::mem_fun(*this, &AbstractConfigWidget::on_apply_cb));

	construct_ui();
	config_to_widget();
}

void AbstractConfigWidget::construct_ui(){

	// 1 clear
	std::vector<Widget*> v = grid.get_children();
	for(auto it=v.begin(); it!=v.end(); ++it){
		Gtk::Widget* w = *it;
		grid.remove(*w);
		if(w != &button_apply)
			delete w;
	}// for

	// 2 add
	grid.attach(button_apply, 0, 0, 2, 1);

	if(!data)
		return;

	const Descriptor* d = data->GetDescriptor();

	for(int i=0; i<d->field_count(); i++){
		const FieldDescriptor* fd = d->field(d->field_count()-i-1);
		const string& fname = fd->name();

		Gtk::Label *label = Gtk::manage(new Gtk::Label);
		label->set_text(fname);

		Gtk::Entry *entry = Gtk::manage(new Gtk::Entry);
		entry->set_hexpand(true);

		grid.insert_row(0);
		grid.attach(*label, 0, 0, 1, 1);
		grid.attach(*entry, 1, 0, 1, 1);

		entry_map[fname] = entry;
		entry->signal_changed().connect(sigc::mem_fun(*this, &AbstractConfigWidget::edit_anything_cb));
	}// for fields
}

void AbstractConfigWidget::setData(Message* msg){
	data = msg;
	construct_ui();
	config_to_widget();
}

const Message* AbstractConfigWidget::getData() const{
	widget_to_config();
	return data;
}

void AbstractConfigWidget::widget_to_config() const{
	if(data == NULL)
		return;

	const Descriptor* desc = data->GetDescriptor();
	const Reflection* refl = data->GetReflection();

	for(auto i=entry_map.begin(); i!=entry_map.end(); ++i){
		const string& var = i->first;
		Gtk::Entry* entry = i->second;

		string val = entry->get_text();

		const FieldDescriptor* fd = desc->FindFieldByName(var);
		FieldDescriptor::Type type = fd->type();

		switch(type){
			case FieldDescriptor::Type::TYPE_DOUBLE:
				refl->SetDouble(data, fd, atof(val.c_str()));
				break;
			case FieldDescriptor::Type::TYPE_FLOAT:
				refl->SetFloat(data, fd, (float)atof(val.c_str()));
				break;
			case FieldDescriptor::Type::TYPE_INT32:
				refl->SetInt32(data, fd, atoi(val.c_str()));
				break;
			case FieldDescriptor::Type::TYPE_UINT32:
				refl->SetUInt32(data, fd, atoi(val.c_str()));
				break;
			case FieldDescriptor::Type::TYPE_STRING:
				refl->SetString(data, fd, val);
				break;
			default:
				assert(false);
		}

	}// for

	button_apply.set_sensitive(false);
}
void AbstractConfigWidget::config_to_widget() const {
	if(data == NULL)
		return;

	const Descriptor* desc = data->GetDescriptor();
	const Reflection* refl = data->GetReflection();

	for(auto i=entry_map.begin(); i!=entry_map.end(); ++i){
		const string& var = i->first;
		Gtk::Entry* entry = i->second;

		std::ostringstream res;

		const FieldDescriptor* fd = desc->FindFieldByName(var);
		FieldDescriptor::Type type = fd->type();

		switch(type){
			case FieldDescriptor::Type::TYPE_DOUBLE:
				res << refl->GetDouble(*data, fd);
				break;
			case FieldDescriptor::Type::TYPE_FLOAT:
				res << refl->GetFloat(*data, fd);
				break;
			case FieldDescriptor::Type::TYPE_INT32:
				res << refl->GetInt32(*data, fd);
				break;
			case FieldDescriptor::Type::TYPE_UINT32:
				res << refl->GetUInt32(*data, fd);
				break;
			case FieldDescriptor::Type::TYPE_STRING:
				res << refl->GetString(*data, fd);
				break;
			default:
				assert(false);
		}

		entry->set_text(res.str());
	}// for

	button_apply.set_sensitive(false);
}

void AbstractConfigWidget::edit_anything_cb(){
	button_apply.set_sensitive(true);
}

void AbstractConfigWidget::on_apply_cb(){
	widget_to_config();
	m_signal_changed.emit();
}

///////////////////////////////////////////////////////////////////////////////

EXConfigWidget::EXConfigWidget(const OdeConfig* cfg){
	this->add(cfg_widget);

	if(!cfg)
		this->config = NULL;
	else
		this->config = cfg->clone();

	cfg_widget.setData(dynamic_cast<Message*>(config));
	cfg_widget.signal_changed().connect(sigc::mem_fun(*this, &EXConfigWidget::on_changed));
}

void EXConfigWidget::on_changed(){
	m_signal_changed.emit();
}

void EXConfigWidget::loadConfig(const OdeConfig* cfg){
	if(!cfg)
		this->config = NULL;
	else
		this->config = cfg->clone();

	Message* msg = dynamic_cast<Message*>(config);
		assert(msg);
	cfg_widget.setData(msg);
}

const OdeConfig* EXConfigWidget::getConfig() const{
	return this->config;
}

E4StateGeneratorWidget::E4StateGeneratorWidget(const E4Config* _config){
	this->state = NULL;

	if(_config)
		this->config = new E4Config(*_config);
	else
		this->config = new E4Config();

	// TODO: may be state should remember its config?!
	pb::E4StateGeneratorConfig* sgc = new pb::E4StateGeneratorConfig();
	sgc->set_e0(0.01);
	sgc->set_phi0(0.0);
	sgc->set_a0(1.0);
	cfg_widget.setData(sgc);

	this->add(cfg_widget);
	cfg_widget.signal_changed().connect(sigc::mem_fun(*this, &E4StateGeneratorWidget::on_changed));

	assert(!this->state);
	newState();
}

E4StateGeneratorWidget::~E4StateGeneratorWidget(){
	// XXX where are deletes?
}

void E4StateGeneratorWidget::on_changed(){
	newState();
}

void E4StateGeneratorWidget::loadConfig(const OdeConfig* cfg){
	const E4Config* ecfg = dynamic_cast<const E4Config*>(cfg);
		assert(ecfg);

	delete this->config;
	this->config = new E4Config(*ecfg);

	delete this->state;
	this->state = NULL;

	newState();
}
const OdeConfig* E4StateGeneratorWidget::getConfig(){
	return config;
}

const OdeState* E4StateGeneratorWidget::getState(){
	assert(config);
	assert(state);
	return state;
}

void E4StateGeneratorWidget::newState(bool emit){
	delete state;
	state = new E4State(config);

	const pb::E4StateGeneratorConfig* sgc = dynamic_cast<const pb::E4StateGeneratorConfig*>(cfg_widget.getData());

	double e = sgc->e0();
	double phi = sgc->phi0();
	double a0 = sgc->a0();

	bool use_rand = false;
	double right = 2*M_PI;
	double left = 0.0;

	int N = config->n();

	double h = (right-left)/250;

	for(int i=0; i<N; i++){
		double psi = rand() / (double)RAND_MAX * (right-left) + left;
		double z = i / (double)N * (right-left) + left;
		double delta = rand() / (double)RAND_MAX * (2*config->delta_0()) - config->delta_0();

		pb::E4State::Particles p;
		p.set_a(a0);
		//p.set_a(a0*(1.0+0.2*sin(int(psi-z+phi))));
		p.set_psi(psi);
		p.set_z(z);
		p.set_delta(delta);

		state->mutable_particles(i)->CopyFrom(p);
	}

	state->set_e(e);
	state->set_phi(phi);

	if(emit)
		m_signal_changed();
}

EXPetscSolverConfigWidget::EXPetscSolverConfigWidget(const EXPetscSolverConfig* config){
	if(config)
		this->config = new EXPetscSolverConfig(*config);
	else
		this->config = new EXPetscSolverConfig();

	Glib::RefPtr<Gtk::Builder> b = Gtk::Builder::create_from_file(UI_FILE_PETSC_SOLVER);

	Gtk::Widget* root;
	b->get_widget("root", root);

	b->get_widget("entry_atol", entry_atol);
	b->get_widget("entry_rtol", entry_rtol);
	b->get_widget("entry_step", entry_step);
	adj_n_cores = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(b->get_object("adj_n_cores"));
	adj_n_cores->set_value(4);

	this->add(*root);

	config_to_widget();
}
const OdeSolverConfig* EXPetscSolverConfigWidget::getConfig(){
	widget_to_config();
	return config;
}
void EXPetscSolverConfigWidget::loadConfig(const OdeSolverConfig* config){
	const EXPetscSolverConfig* econfig = dynamic_cast<const EXPetscSolverConfig*>(config);
		assert(econfig);
	delete this->config;
	this->config = new EXPetscSolverConfig(*econfig);
	config_to_widget();
}

void EXPetscSolverConfigWidget::widget_to_config(){
	config->set_init_step(atof(entry_step->get_text().c_str()));
	config->set_atol(atof(entry_atol->get_text().c_str()));
	config->set_rtol(atof(entry_rtol->get_text().c_str()));
	config->set_n_cores(adj_n_cores->get_value());
}
void EXPetscSolverConfigWidget::config_to_widget(){
	std::ostringstream buf;
	buf << config->init_step();
	entry_step->set_text(buf.str());

	buf.str("");
	buf << config->atol();
	entry_atol->set_text(buf.str());

	buf.str("");
	buf << config->rtol();
	entry_rtol->set_text(buf.str());
	adj_n_cores->set_value(config->n_cores());
}
