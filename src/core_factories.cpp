/*
 * core_factories.cpp
 *
 *  Created on: Dec 14, 2013
 *      Author: dimalit
 */

#include "core_factories.h"
#include "core_factory_managers.h"

#include "core_interfaces.h"

#include <cassert>

OdeInstanceFactory::OdeInstanceFactory(){
	OdeInstanceFactoryManager::getInstance()->add(this);
}

OdeInstanceFactory::~OdeInstanceFactory(){
	OdeInstanceFactoryManager::getInstance()->remove(this);
}

OdeInstance* OdeInstanceFactory::createInstance() const {
	OdeConfig* c = createConfig();
	OdeState*  s = createState(c);
	return new OdeInstance(c, s);
}

OdeSolverFactory::OdeSolverFactory(OdeInstanceFactory* corresponding_instance_factory){
	assert(corresponding_instance_factory);
	this->corresponding_instance_factory = corresponding_instance_factory;

	OdeSolverFactoryManager::getInstance()->add(this);
}
OdeSolverFactory::~OdeSolverFactory(){
	OdeSolverFactoryManager::getInstance()->remove(this);
}
