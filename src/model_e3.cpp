/*
 * model_e3.cpp
 *
 *  Created on: Aug 19, 2014
 *      Author: dimalit
 */

#include "model_e3.h"

#include "rpc.h"

#include <fcntl.h>
#include <sys/wait.h>

E3State::E3State(const E3Config* config){
	int m = config->m();
	for(int i=0; i<m; i++){
		this->add_particles();
	}
	set_simulated(false);
}

E3PetscSolver::E3PetscSolver(const E3PetscSolverConfig* scfg, const E3Config* pcfg, const E3State* init_state){
	time_passed = 0;
	steps_passed = 0;

	pconfig = new E3Config(*pcfg);
	sconfig = new E3PetscSolverConfig(*scfg);
	state = new E3State(*init_state);
}

double E3PetscSolver::getTime() const {
	return time_passed;
}
double E3PetscSolver::getSteps() const {
	return steps_passed;
}

E3PetscSolver::~E3PetscSolver(){
	delete state;
	delete sconfig;
	delete pconfig;
}

// TODO: create universal base class for PETSc solvers - so not to copypaste!
// TODO: 1 universal code from TS solving?! (not to write it again and again!?)
const OdeState* E3PetscSolver::run(int steps, double time){
	int rf, wf;
	pid_t child = rpc_call("../ts3/Debug/ts3", &rf, &wf);

//	int tmp = open("tmp", O_WRONLY | O_CREAT, 0664);

	pb::E3Model all;
	all.mutable_sconfig()->CopyFrom(*sconfig);
	all.mutable_pconfig()->CopyFrom(*pconfig);
	all.mutable_state()->CopyFrom(*state);

	all.set_time(time);
	all.set_steps(steps);

	all.SerializeToFileDescriptor(wf);
	close(wf);		// need EOF for protobuf to catch the end of the message

//	close(tmp);

//	char buf;
//	while(read(rf, &buf, 1) > 0);
	waitpid(child, 0, 0);

	read(rf, &steps_passed, sizeof(steps_passed));
	read(rf, &time_passed, sizeof(time_passed));

	state->ParseFromFileDescriptor(rf);
	state->set_simulated(true);

	close(rf);

	return state;
}