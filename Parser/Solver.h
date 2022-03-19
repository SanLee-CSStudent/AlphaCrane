#ifndef __SOLVER_H_
#define __SOLVER_H_
#include "ContainerGrid.h"
#include <string>
#include <vector>

std::vector<std::string> Solve(ContainerGrid *);
std::vector<std::string> Load(const std::vector<Container>&, ContainerGrid *);
std::vector<std::string> Unload(const std::vector<Container>&, ContainerGrid *);

#endif
