/*
 * interfaces.h
 *
 *  Created on: Dec 14, 2013
 *      Author: dimalit
 */

#ifndef CORE_INTERFACES_H_
#define CORE_INTERFACES_H_

class OdeConfig{
public:
	virtual OdeConfig* clone() const = 0;
	virtual ~OdeConfig(){}	// make it polymorphic
};

class OdeState{
public:
	virtual OdeState* clone() const = 0;
	virtual ~OdeState(){}	// make it polymorphic
};

class OdeInstance{
private:
	const OdeConfig* config;
	const OdeState* initial_state;
public:
	OdeInstance(const OdeConfig* cfg, const OdeState* state){
		this->config = cfg;
		this->initial_state = state;
	}
	const OdeState* getInitialState() const {return initial_state;}
	const OdeConfig* getConfig() const {return config;}
	void setConfig(const OdeConfig* cfg) {this->config = cfg;}
	void setInitialState(const OdeState* state) {this->initial_state = state;}
};

class OdeSolverConfig{
public:
	virtual OdeSolverConfig* clone() const = 0;
	virtual ~OdeSolverConfig(){}	// make it polymorphic
};

// TODO: Virtual destructor here and elsewhere!
class OdeSolver{
public:
	virtual void run(int steps, double time, bool use_step = false) = 0;
	virtual bool step() = 0;
	virtual void finish() = 0;
	virtual ~OdeSolver(){}
	virtual double getTime() const = 0;
	virtual double getSteps() const = 0;
	virtual const OdeState* getState() const = 0;
	virtual const OdeState* getDState() const = 0;
};

#endif /* CORE_INTERFACES_H_ */
