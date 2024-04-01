#include <cinttypes>
#include <vector>
#include <iostream>


class Storage 
{
private:

	std::string ID;
	int availTime = 0;

public:

	Storage(std::string id, int time)
	{
		this->ID = id;
		this->availTime = time;
	}

	std::string get_id() { return this->ID; }
	int get_avail_time() { return this->availTime; }
	void set_avail_time(int time) { this->availTime = time; }
};

class Instruction 
{
private:

	int num;
	std::string readReg;
	std::string readMem;
	std::string writeReg;
	std::string writeMem;
	int issueTime = 0;

public:

	Instruction(int n, std::string rr, std::string rm, std::string wr, std::string wm, int it)
	{
		this->num = n;
		this->readReg = rr;
		this->readMem = rm;
		this->writeReg = wr;
		this->writeMem = wm;
		this->issueTime = it;
	}

	std::string get_read_mem() { return this->readMem; }
	std::string get_read_reg() { return this->readReg; }
	std::string get_write_mem() { return this->writeMem; }
	std::string get_write_reg() { return this->writeReg; }

	void set_issue_time(int time) { this->issueTime = time; }
	int get_issue_time() { return this->issueTime; }
};


// Makeshift ring buffer

class RingBuffer : public std::vector<Instruction>
{

	int maxSize = 4;

public:

	int push_back(const Instruction& element)
	{
		if (std::vector<Instruction>::size() < maxSize)
		{
			std::vector<Instruction>::push_back(element);
			return 1;
		}
		else { return 0; }
	}
};


void process_instructions(Instruction &instr, std::vector<Storage> &regs, std::vector<Storage> &mem, int clockCycle)
{
	if ( !instr.get_read_reg().empty() )
	{
		for ( auto& i : regs )
		{
			if ( instr.get_read_reg() == i.get_id() )
			{
				instr.set_issue_time(i.get_avail_time());
			}
		}
	}

	if ( !instr.get_read_mem().empty() )
	{
		for ( auto& i : mem )
		{
			if ( instr.get_read_mem() == i.get_id() )
			{
				instr.set_issue_time(i.get_avail_time());
			}
		}
	}

	if ( instr.get_issue_time() < clockCycle ) { instr.set_issue_time(clockCycle); }

	if ( !instr.get_write_reg().empty() )
	{
		for ( auto& i : regs )
		{
			i.set_avail_time(instr.get_issue_time() + 1);
		}
	}

	if ( !instr.get_write_mem().empty() )
	{
		for ( auto& i : mem )
		{
			i.set_avail_time(instr.get_issue_time() + 1);
		}
	}
}


int main()
{
	int clockCycle = 0;
	int instrCount = 0;

	std::vector<Instruction> instrVec = {
		{1, "", "0xffffb396be70", "r3", "", 0},
		{2, "r3", "", "r3", "", 0},
		{3, "r3", "", "", "0xffffb396be70", 0},
		{4, "", "0xfffff23b2fd0", "r2", "", 0},
		{5, "r2", "", "", "", 0},
		{6, "", "0xffffb396be80", "r3", "", 0},
		{7, "r3", "", "r3", "", 0},
		{8, "r3", "", "", "0xffffb396be80", 0},
		{9, "", "0xfffff23b2fd8", "r2", "", 0},
		{10, "r2", "", "", "", 0},
		{11, "", "0xffffb396bef0", "r3", "", 0},
		{12, "r3", "", "r3", "", 0},
		{13, "r3", "", "", "0xffffb396bef0", 0},
		{14, "", "0xfffff23b3058", "r2", "", 0},
		{15, "r2", "", "", "", 0},
		{16, "", "xffffb396bee0", "r3", "", 0},
		{17, "r3", "", "r3", "", 0},
		{18, "r3", "", "", "0xffffb396bee0", 0},
		{19, "", "0xfffff23b3160", "r2", "", 0},
		{20, "r2", "", "", "", 0}
	};

	std::vector<Storage> regVec = {
		{"r1", 0},
		{"r2", 0},
		{"r3", 0}
	};

	std::vector<Storage> memVec = {
		{"0xffffb396be70", 0},
		{"xfffff23b2fd0", 0},
		{"xffffb396be80", 0},
		{"xfffff23b2fd8", 0},
		{"xffffb396bef0", 0},
		{"xfffff23b3058", 0},
		{"xffffb396bee0", 0},
		{"xfffff23b3160", 0}
	};

	RingBuffer instrBuff;

	for ( size_t i = 0; instrVec.begin() + i < instrVec.end(); i++ )
	{
		if ( instrBuff.push_back(instrVec[i]) )
		{

			process_instructions(instrBuff.at(instrBuff.size()-1), regVec, memVec, clockCycle);
			instrCount++;
		}
		else
		{
			clockCycle++;
			for ( size_t t = 0; t < instrBuff.size(); t++ )
			{
				if ( instrBuff.at(t).get_issue_time() < clockCycle ) 
				{ 
					instrBuff.erase(instrBuff.begin() + t);
					t--;
				}
			}
			i--;
		}
	}


	std::cout << "Clock cycles: " << clockCycle << std::endl;
	std::cout << "Instruction count: " << instrCount << std::endl;
	
}
