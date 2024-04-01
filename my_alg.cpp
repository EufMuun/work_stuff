#include <cinttypes>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <queue>


class Instruction 
{
private:

	int id;
	std::string readReg;
	std::string readMem;
	std::string writeReg;
	std::string writeMem;

public:

	Instruction(int n, std::string rr, std::string rm, std::string wr, std::string wm)
	{
		this->id = n;
		this->readReg = rr;
		this->readMem = rm;
		this->writeReg = wr;
		this->writeMem = wm;
	}

	std::string get_read_mem() { return this->readMem; }
	std::string get_read_reg() { return this->readReg; }
	std::string get_write_mem() { return this->writeMem; }
	std::string get_write_reg() { return this->writeReg; }

	int get_id() { return this->id; }
};


// Makeshift ring buffer

class RingBuffer : public std::vector<Instruction>
{
public:

	int maxSize = 4;

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


int main()
{
	int clockCycle = 0;
	int instrCount = 0;

	std::vector<Instruction> instrVec = {
		{1, "", "0xffffb396be70", "r3", ""},
		{2, "r3", "", "r3", ""},
		{3, "r3", "", "", "0xffffb396be70"},
		{4, "", "0xfffff23b2fd0", "r2", ""},
		{5, "r2", "", "", ""},
		{6, "", "0xffffb396be80", "r3", ""},
		{7, "r3", "", "r3", ""},
		{8, "r3", "", "", "0xffffb396be80"},
		{9, "", "0xfffff23b2fd8", "r2", ""},
		{10, "r2", "", "", ""},
		{11, "", "0xffffb396bef0", "r3", ""},
		{12, "r3", "", "r3", ""},
		{13, "r3", "", "", "0xffffb396bef0"},
		{14, "", "0xfffff23b3058", "r2", ""},
		{15, "r2", "", "", ""},
		{16, "", "xffffb396bee0", "r3", ""},
		{17, "r3", "", "r3", ""},
		{18, "r3", "", "", "0xffffb396bee0"},
		{19, "", "0xfffff23b3160", "r2", ""},
		{20, "r2", "", "", ""}
	};

	RingBuffer instrBuff;

	std::unordered_map<std::string, std::deque<int>> storageMap;
	std::unordered_map<std::string, std::deque<int>>::iterator it;
	storageMap["r1"] = {};
	storageMap["r2"] = {};
	storageMap["r3"] = {};
	storageMap["xffffb396be70"] = {};
	storageMap["xfffff23b2fd0"] = {};
	storageMap["xffffb396be80"] = {};
	storageMap["xfffff23b2fd8"] = {};
	storageMap["xffffb396bef0"] = {};
	storageMap["xfffff23b3058"] = {};
	storageMap["xffffb396bee0"] = {};
	storageMap["xfffff23b3160"] = {};
	

	for ( size_t i = 0; i < 20; i++ )
	{
		if ( instrBuff.push_back(instrVec[i]) )
		{

			it = storageMap.find(instrBuff.at(instrBuff.size()-1).get_write_mem());
			if ( it != storageMap.end() )
			{
				it->second.push_back(i);
			}
			
			it = storageMap.find(instrBuff.at(instrBuff.size()-1).get_read_mem());
			if ( it != storageMap.end() )
			{
				it->second.push_back(i);
			}

			it = storageMap.find(instrBuff.at(instrBuff.size()-1).get_write_reg());
			if ( it != storageMap.end() )
			{
				it->second.push_back(i);
			}

			it = storageMap.find(instrBuff.at(instrBuff.size()-1).get_read_reg());
			if ( it != storageMap.end() )
			{
				it->second.push_back(i);
			}

			instrCount++;
		}
		else
		{
			for ( auto& [key, value] : storageMap )
			{
				if ( !value.empty() ) { value.pop_front(); }
			}

			// That's really bad, need to rewrite this whole segment if i'll have enough time
			for ( size_t t = 0; t < instrBuff.size(); t++ )
			{
				bool finished = true;
				for ( auto& [key, value] : storageMap )
				{
					for ( auto q : value )
					{
						if ( q != instrBuff.at(t).get_id() ) { continue; }
						else
						{
							finished = false;
							break;
						}
					}
				}
				if ( finished )
				{
					instrBuff.erase(instrBuff.begin() + t);
					t--;
				}
			}
			i--;
			clockCycle++;
		}
	}

	std::cout << "Clock Cycles: " << clockCycle << std::endl << "Instruction count: " << instrCount << std::endl;
}
