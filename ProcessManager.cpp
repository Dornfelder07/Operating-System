// ProcessManager.cpp: definiuje punkt wejścia dla aplikacji konsolowej.
//
#include "ProcessManager.h"
#include "Memory.h"
#include "PCB.h"
#include <algorithm>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <random>	 /* time */


int PID = 0;

int randomInt(int min, int max) {
	static std::default_random_engine e{};
	std::uniform_int_distribution<int> d(min, max);
	return d(e);
}

//dla dummy process
void ProcessManager::fork()
{
	PCB process;
	process.name = "dummy";
	process.PID = PID;
	process.parent = this->get_process("init");
	if (process.parent != nullptr) { process.parent->children.push_back(std::make_shared<PCB>(process)); }
	process.real_time = NULL;
	process.file_name = "dummy.txt";
	process.predicted_time = 9999999;
	this->processes.push_back(std::make_shared<PCB>(process));
	std::cout << "\nProcess has been created. PID: " << process.PID << ". Process name: " << process.name << "\n";
	PID++;
}


void ProcessManager::fork(const std::string & name, const std::string &file)
{
	bool exists = false;
	std::string temp;
	for (int index = 0; index < this->processes.size(); index++)
	{
		temp = this->processes[index]->name;
		if (this->processes[index]->name == name) { exists = true; std::cout << "\nCreating process: The process cannot be created, process with given name (" << name << ") already exists.\n"; }
	}
	if (exists == false)
	{
		PCB process;
		process.name = name;
		process.PID = PID;
		process.parent = this->get_process("init");
		if (process.parent != nullptr) { process.parent->children.push_back(std::make_shared<PCB>(process)); }
		process.real_time = NULL;
		process.file_name = file;
		process.predicted_time = randomInt(1, 10);
		this->processes.push_back(std::make_shared<PCB>(process));
		std::cout << "\nProcess has been created. PID: " << process.PID << ". Process name: " << process.name << "\n";
		PID++;
	}
}


void ProcessManager::fork(const std::string & name, std::shared_ptr<PCB> parent)
{
	bool exists = false;
	std::string temp;
	for (int index = 0; index < this->processes.size(); index++)
	{
		temp = this->processes[index]->name;
		if (this->processes[index]->name == name) { exists = true; std::cout << "\nCreating process: The process cannot be created, process with given name (" << name << ") already exists.\n"; }
	}
	if (exists == false)
	{
		PCB process;
		process.name = name;
		process.PID = PID;
		process.parent = parent;
		if (parent != nullptr) { parent->children.push_back(std::make_shared<PCB>(process)); }
		process.real_time = NULL;
		process.predicted_time = rand() % 10 + 1;
		this->processes.push_back(std::make_shared<PCB>(process));
		std::cout << "\nProcess has been created. PID: " << process.PID << ". Process name: " << process.name << "\n";
		PID++;
	}
}

void ProcessManager::fork(const std::string & name, const std::string &file, std::shared_ptr<PCB> parent)
{
	bool exists = false;
	std::string temp;
	for (int index = 0; index < this->processes.size(); index++)
	{
		temp = this->processes[index]->name;
		if (this->processes[index]->name == name) { exists = true; std::cout << "\nCreating process: The process cannot be created, process with given name (" << name << ") already exists.\n"; }
	}
	if (exists == false)
	{
		PCB process;
		process.name = name;
		process.PID = PID;
		process.parent = parent;
		if (parent != nullptr) { parent->children.push_back(std::make_shared<PCB>(process)); }
		process.real_time = NULL;
		process.predicted_time = rand() % 10 + 1;
		process.file_name = file;
		this->processes.push_back(std::make_shared<PCB>(process));
		std::cout << "\nProcess has been created. PID: " << process.PID << ". Process name: " << process.name << "\n";
		PID++;
	}
}

void ProcessManager::exit(const std::string & name)
{
	bool found = false;
	std::string temp;
	for (int index = 0; index < this->processes.size(); index++)
	{
		temp = this->processes[index]->name;
		if (name == "init" || name == "dummy") { std::cout << "\nDeleting process: The init and dummy processes cannot be deleted.\n"; found = true;  break; }
		else if (this->processes[index]->name == name) 
		{ 
			for (auto e : this->processes)
			{
				if (e->parent != nullptr)
				{
					if (e->parent->name == name)
					{
						e->parent = this->get_process("init");
					}
				}
			}
			this->processes.erase(this->processes.begin() + index); found = true; std::cout << "\nProcess " << temp << " has been deleted.\n"; 
		}
	}
	if (found == false) { std::cout << "\nDeleting process: Cannot find a process with given name (" << name << ").\n"; }
}

void ProcessManager::showlist()
{
	std::cout << "\nList of existing processes:\n";
	for (auto e : this->processes)
	{
		std::cout << "PID: " << e->PID << " || name: " << e->name << " || parent: ";
		if (e->PID == 0) { std::cout << "none"; }
		else
		{
			if (e->parent != nullptr) std::cout << e->parent->name;
		}
		std::cout << " || state: ";
		switch (e->state)
		{
		case 0:
			std::cout << "new"; break;
		case 1:
			std::cout << "ready"; break;
		case 2:
			std::cout << "running"; break;
		case 3:
			std::cout << "waiting"; break;
		case 4:
			std::cout << "terminated"; break;
		}
		std::cout << " || real_time: " << e->real_time << " || predicted_time: " << e->predicted_time << " || reg1: " << e->reg1 << " || reg2: " << e->reg2
			<< " || reg3: " << e->reg3 << " || reg4: " << e->reg4 << " || command_counter: " << e->command_counter << "\n";
	}
}

void ProcessManager::showpcb(const std::string & name)
{
	bool found = false;
	for (int index = 0; index < this->processes.size(); index++)
	{
		std::shared_ptr<PCB> p = this->processes[index];
		if (p->name == name)
		{
			std::cout << "\nInformation about the process:\n" << "name: " << p->name << " || PID: " << p->PID << " || state: ";
				switch (p->state)
				{
				case 0:
					std::cout << " new"; break;
				case 1:
					std::cout << " ready"; break;
				case 2:
					std::cout << " running"; break;
				case 3:
					std::cout << " waiting"; break;
				case 4:
					std::cout << " terminated"; break;
				}
				std::cout << " || real_time: " << p->real_time << " || predicted_time: " << p->predicted_time
				<< " || reg1: " << p->reg1 << " || reg2: " << p->reg2 << " || reg3: " << p->reg3 << " || reg4: " << p->reg4 << " || command_counter: " << p->command_counter << "\n";
			found = true;
		}
	}
	if (found == false) { std::cout << "\nInformation about the process: Cannot find a process with given name (" << name << ").\n"; }
}

std::shared_ptr<PCB> ProcessManager::get_process(const std::string & name)
{
	bool found = false;
	for (int index = 0; index < this->processes.size(); index++)
	{
		std::shared_ptr<PCB> p = this->processes[index];
		if ((*p).name == name)
		{
			return p;
			found = true;
		}
	}
	if (found == false) { std::cout << "\nGetting a pointer to the process: Cannot find a process with given name (" << name << ").\n"; return nullptr; }
}