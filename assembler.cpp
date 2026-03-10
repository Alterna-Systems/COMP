// Written in C++17; will not compile with Visual Studio defaults. Switch from C++14 to C++17.

#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <bitset>
#include <filesystem>

void tokenize(std::string filename, std::vector<std::vector<std::string>*> &tokenized_lines, std::map<std::string, int> &label_map) {
	if (!std::filesystem::exists(filename)) {
		std::cout << "Nonexistent file given (stage 0).";
		exit(1);
	}

	std::ifstream asm_file(filename);

	for (std::string line; std::getline(asm_file, line); ) {
		std::replace(std::begin(line), std::end(line), '\t', ' ');

		std::stringstream ss(line);
		std::vector<std::string>* line_tokens = new std::vector<std::string>;
		std::string token;

		while (getline(ss, token, ' ')) {
			if (token != "") line_tokens->push_back(token);
		}

		if (line_tokens->size() == 0) continue;

		transform((*line_tokens)[0].begin(), (*line_tokens)[0].end(), (*line_tokens)[0].begin(), ::tolower);

		for (int i = 0; i < line_tokens->size(); ++i) {
			if ((*line_tokens)[i] == "//") {
				while (line_tokens->size() > i) (*line_tokens).pop_back();
				break;
			}
		}

		if (!((*line_tokens)[0] == "halt" ||
			(*line_tokens)[0] == "noop" ||
			(*line_tokens)[0] == "add" ||
			(*line_tokens)[0] == "xor" ||
			(*line_tokens)[0] == "or" ||
			(*line_tokens)[0] == "lrsr" ||
			(*line_tokens)[0] == "par" ||
			(*line_tokens)[0] == "lw" ||
			(*line_tokens)[0] == "sw" ||
			(*line_tokens)[0] == "addi" ||
			(*line_tokens)[0] == "subi" ||
			(*line_tokens)[0] == "lsl" ||
			(*line_tokens)[0] == "movi" ||
			(*line_tokens)[0] == "set" ||
			(*line_tokens)[0] == "jump" ||
			(*line_tokens)[0] == "bne")) {
			if (line_tokens->size() > 1) {
				std::cout << "Invalid instruction or label: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nUnrecognized instruction / label not on own line (stage 1).";
				exit(1);
			}
			else {
				if ((*line_tokens)[0].back() == ':') (*line_tokens)[0] = (*line_tokens)[0].substr(0, (*line_tokens)[0].length() - 1);
				label_map[(*line_tokens)[0]] = tokenized_lines.size();
			}
		}
		else if ((*line_tokens)[0] == "halt" ||
			(*line_tokens)[0] == "noop") {
			if (line_tokens->size() > 1) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nN-type instructions must have no operands (stage 1).";
				exit(1);
			}

			tokenized_lines.push_back(line_tokens);
		}
		else if ((*line_tokens)[0] == "add" ||
			(*line_tokens)[0] == "xor" ||
			(*line_tokens)[0] == "or" ||
			(*line_tokens)[0] == "lrsr" ||
			(*line_tokens)[0] == "par" ||
			(*line_tokens)[0] == "lw" ||
			(*line_tokens)[0] == "sw") {
			if (line_tokens->size() > 2) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nR-type instructions must have only one operand (stage 1).";
				exit(1);
			}
			else if ((*line_tokens)[1].front() != 'R' && (*line_tokens)[1].front() != 'r') {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nR-type instruction operand must be a register (stage 1).";
				exit(1);
			}
			try { 
				int i = stoi((*line_tokens)[1].substr(1, (*line_tokens)[1].length() - 1));
				if (i < 0 || i > 15) {
					std::cout << "Invalid instruction: ";
					for (int i = 0; i < line_tokens->size(); ++i) {
						std::cout << (*line_tokens)[i] << " ";
					}
					std::cout << "\nIllegal register (stage 1).";
					exit(1);
				}
			}
			catch (const std::invalid_argument& ia) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nNon-numeric register (stage 1).";
				exit(1);
			}

			tokenized_lines.push_back(line_tokens);
		}
		else if ((*line_tokens)[0] == "addi" ||
			(*line_tokens)[0] == "subi" ||
			(*line_tokens)[0] == "lsl" ||
			(*line_tokens)[0] == "movi" ||
			(*line_tokens)[0] == "set") {
			if (line_tokens->size() > 2) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nI-type instructions must have only one operand (stage 1).";
				exit(1);
			}
			else if ((*line_tokens)[0] == "set" && ((*line_tokens)[1].front() != 'R' && (*line_tokens)[1].front() != 'r')) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nSet instruction operand must be a register (stage 1).";
				exit(1);
			}
			else if ((*line_tokens)[0] == "set") {
				try {
					int i = stoi((*line_tokens)[1].substr(1, (*line_tokens)[1].length() - 1));
					if (i < 0 || i > 15) {
						std::cout << "Invalid instruction: ";
						for (int i = 0; i < line_tokens->size(); ++i) {
							std::cout << (*line_tokens)[i] << " ";
						}
						std::cout << "\nIllegal register (stage 1).";
						exit(1);
					}
				}
				catch (const std::invalid_argument& ia) {
					std::cout << "Invalid instruction: ";
					for (int i = 0; i < line_tokens->size(); ++i) {
						std::cout << (*line_tokens)[i] << " ";
					}
					std::cout << "\nNon-numeric register (stage 1).";
					exit(1);
				}
			}
			else if ((*line_tokens)[0] != "set") {
				try {
					int i = stoi((*line_tokens)[1]);
					if (i < 0 || i > 15) {
						std::cout << "Invalid instruction: ";
						for (int i = 0; i < line_tokens->size(); ++i) {
							std::cout << (*line_tokens)[i] << " ";
						}
						std::cout << "\nIllegal immediate (stage 1).";
						exit(1);
					}
				}
				catch (const std::invalid_argument& ia) {
					std::cout << "Invalid instruction: ";
					for (int i = 0; i < line_tokens->size(); ++i) {
						std::cout << (*line_tokens)[i] << " ";
					}
					std::cout << "\nNon-numeric immediate (stage 1).";
					exit(1);
				}
			}

			tokenized_lines.push_back(line_tokens);
		}
		else if ((*line_tokens)[0] == "jump") {
			if (line_tokens->size() > 2) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nJ-type instructions must have only one operand (stage 1).";
				exit(1);
			}

			tokenized_lines.push_back(line_tokens);
		}
		else if ((*line_tokens)[0] == "bne") {
			if (line_tokens->size() > 3) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nB-type instructions must have two operands (stage 1).";
				exit(1);
			}
			else if ((*line_tokens)[1].front() != 'R' && (*line_tokens)[1].front() != 'r') {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nB-type instruction first operand must be a register (stage 1).";
				exit(1);
			}
			try {
				int i = stoi((*line_tokens)[1].substr(1, (*line_tokens)[1].length() - 1));
				if (i < 0 || i > 15) {
					std::cout << "Invalid instruction: ";
					for (int i = 0; i < line_tokens->size(); ++i) {
						std::cout << (*line_tokens)[i] << " ";
					}
					std::cout << "\nIllegal register (stage 1).";
					exit(1);
				}
			}
			catch (const std::invalid_argument& ia) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < line_tokens->size(); ++i) {
					std::cout << (*line_tokens)[i] << " ";
				}
				std::cout << "\nNon-numeric register (stage 1).";
				exit(1);
			}

			tokenized_lines.push_back(line_tokens);
		}
	}
	
	return;
}

std::string generate_instruction(std::vector<std::string> vector_ins, std::map<std::string, int>& label_map, int index) {
	int binary_ins = 0b000000000;
	int binary_1st_field = 0b000000000;
	int binary_2nd_field = 0b000000000;
	std::string string_op = vector_ins[0];
	std::string string_1st_field = "";
	std::string string_2nd_field = "";
	if (vector_ins.size() > 1) string_1st_field = vector_ins[1];
	if (vector_ins.size() > 2) string_2nd_field = vector_ins[2];

	if (string_op == "halt") {
		return std::bitset<9>(0b000000000).to_string();
	}
	else if (string_op == "noop") {
		return std::bitset<9>(0b011110000).to_string();
	}
	else if (string_op == "add" ||
		string_op == "xor" ||
		string_op == "or" ||
		string_op == "lrsr" ||
		string_op == "par" ||
		string_op == "lw" ||
		string_op == "sw") {
		if (string_op == "add") binary_ins = 0b000010000;
		else if (string_op == "xor") binary_ins = 0b001000000;
		else if (string_op == "or") binary_ins = 0b001010000;
		else if (string_op == "lrsr") binary_ins = 0b001110000;
		else if (string_op == "par") binary_ins = 0b010000000;
		else if (string_op == "lw") binary_ins = 0b010010000;
		else if (string_op == "sw") binary_ins = 0b010100000;
		
		try {
			if (string_1st_field == "") {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nEmpty 1st field (stage 2).";
				exit(1);
			}

			binary_1st_field = stoi(string_1st_field.substr(1, string_1st_field.length() - 1));

			if (binary_1st_field < 0 || binary_1st_field > 15) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nIllegal register (stage 2).";
				exit(1);
			}
		}
		catch (const std::invalid_argument& ia) {
			std::cout << "Invalid instruction: ";
			for (int i = 0; i < vector_ins.size(); ++i) {
				std::cout << vector_ins[i] << " ";
			}
			std::cout << "\nNon-numeric register (stage 2).";
			exit(1);
		}

		return std::bitset<9>(binary_ins + binary_1st_field).to_string();
	}
	else if (string_op == "addi" ||
		string_op == "subi" ||
		string_op == "lsl" ||
		string_op == "movi" ||
		string_op == "set") {
		if (string_op == "addi") binary_ins = 0b000100000;
		else if (string_op == "subi") binary_ins = 0b000110000;
		else if (string_op == "lsl") binary_ins = 0b001100000;
		else if (string_op == "movi") binary_ins = 0b010110000;
		else if (string_op == "set") binary_ins = 0b011000000;

		if (string_op != "set") {
			try {
				if (string_1st_field == "") {
					std::cout << "Invalid instruction: ";
					for (int i = 0; i < vector_ins.size(); ++i) {
						std::cout << vector_ins[i] << " ";
					}
					std::cout << "\nEmpty 1st field (stage 2).";
					exit(1);
				}

				binary_1st_field = stoi(string_1st_field);

				if (binary_1st_field < 0 || binary_1st_field > 15) {
					std::cout << "Invalid instruction: ";
					for (int i = 0; i < vector_ins.size(); ++i) {
						std::cout << vector_ins[i] << " ";
					}
					std::cout << "\nIllegal immediate (stage 2).";
					exit(1);
				}
			}
			catch (const std::invalid_argument& ia) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nNon-numeric immediate (stage 2).";
				exit(1);
			}

			return std::bitset<9>(binary_ins + binary_1st_field).to_string();
		}
		else {
			try {
				if (string_1st_field == "") {
					std::cout << "Invalid instruction: ";
					for (int i = 0; i < vector_ins.size(); ++i) {
						std::cout << vector_ins[i] << " ";
					}
					std::cout << "\nEmpty 1st field (stage 2).";
					exit(1);
				}

				binary_1st_field = stoi(string_1st_field.substr(1, string_1st_field.length() - 1));

				if (binary_1st_field < 0 || binary_1st_field > 15) {
					std::cout << "Invalid instruction: ";
					for (int i = 0; i < vector_ins.size(); ++i) {
						std::cout << vector_ins[i] << " ";
					}
					std::cout << "\nIllegal register (stage 2).";
					exit(1);
				}
			}
			catch (const std::invalid_argument& ia) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nNon-numeric register (stage 2).";
				exit(1);
			}

			return std::bitset<9>(binary_ins + binary_1st_field).to_string();
		}
	}
	else if (string_op == "jump") {
		binary_ins = 0b100000000;
		try {
			if (string_1st_field == "") {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nEmpty 1st field (stage 2).";
				exit(1);
			}

			binary_1st_field = stoi(string_1st_field);

			if (binary_1st_field < 0 || binary_1st_field > 127) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nIllegal immediate (stage 2).";
				exit(1);
			}
		}
		catch (const std::invalid_argument& ia) {
			if (label_map.find(string_1st_field) == label_map.end()) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nIllegal label (stage 2).";
				exit(1);
			}
			else binary_1st_field = label_map[string_1st_field];

			if (binary_1st_field < 0 || binary_1st_field > 127) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nIllegal immediate (stage 2).";
				exit(1);
			}
		}

		return std::bitset<9>(binary_ins + binary_1st_field).to_string();
	}
	else if (string_op == "bne") {
		binary_ins = 0b110000000;

		try {
			if (string_1st_field == "") {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nEmpty 1st field (stage 2).";
				exit(1);
			}

			binary_1st_field = stoi(string_1st_field.substr(1, string_1st_field.length() - 1));

			if (binary_1st_field < 0 || binary_1st_field > 15) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nIllegal register (stage 2).";
				exit(1);
			}

			binary_1st_field = binary_1st_field << 3;
		}
		catch (const std::invalid_argument& ia) {
			std::cout << "Invalid instruction: ";
			for (int i = 0; i < vector_ins.size(); ++i) {
				std::cout << vector_ins[i] << " ";
			}
			std::cout << "\nNon-numeric register (stage 2).";
			exit(1);
		}

		try {
			if (string_2nd_field == "") {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nEmpty 2nd field (stage 2).";
				exit(1);
			}

			binary_2nd_field = stoi(string_2nd_field);

			if (binary_2nd_field < 0 || binary_2nd_field > 7) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nIllegal immediate (stage 2).";
				exit(1);
			}
		}
		catch (const std::invalid_argument& ia) {
			if (label_map.find(string_2nd_field) == label_map.end()) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nIllegal label (stage 2).";
				exit(1);
			}
			else binary_2nd_field = label_map[string_2nd_field] - index - 1;

			if (binary_2nd_field < 0 || binary_2nd_field > 7) {
				std::cout << "Invalid instruction: ";
				for (int i = 0; i < vector_ins.size(); ++i) {
					std::cout << vector_ins[i] << " ";
				}
				std::cout << "\nIllegal immediate (stage 2).";
				exit(1);
			}
		}

		return std::bitset<9>(binary_ins + binary_1st_field + binary_2nd_field).to_string();
	}

	std::cout << "Illegal instruction: ";
	for (int i = 0; i < vector_ins.size(); ++i) {
		std::cout << vector_ins[i] << " ";
	}
	std::cout << "\nUnrecognized instruction (stage 2).";
	exit(1);
}

int main(int argc, char** argv) {
	std::string asm_filename;

	if (argc >= 2) {
		asm_filename = argv[1];
	}
	else {
		asm_filename = "test.txt";
	}

	std::vector<std::vector<std::string>*> tokenized_lines;
	std::map<std::string, int> label_map;

	tokenize(asm_filename, tokenized_lines, label_map);

	std::string instructions = "";

	for (int i = 0; i < tokenized_lines.size(); ++i) {
		std::string binary_instruction = generate_instruction(*(tokenized_lines[i]), label_map, i);
		instructions += binary_instruction;
	}

	for (int i = instructions.size(); i < 128; ++i) {
		instructions += "000000000";
	}

	std::ofstream blob;


	if (std::filesystem::exists("out.bin")) {
		for (long long i = 0; i < LLONG_MAX; ++i) {
			if (std::filesystem::exists("out_" + std::to_string(i) + ".bin")) continue;
			else {
				blob.open("out_" + std::to_string(i) + ".bin", std::ofstream::binary);
				break;
			}
		}
	}
	else blob.open("out.bin", std::ofstream::binary);

	if (!blob.is_open())
	{
		std::cout << "Unable to open file for writing (stage 3).\n";
		exit(1);
	}

	while (instructions.length() > 0) {
		while (instructions.length() < 8) instructions += "0";
		std::bitset<8> byte_bitset(instructions.substr(0, 8));
		static_assert(byte_bitset.size() == 8, "Bitset is not a byte (stage 3).");
		uint8_t byte = (uint8_t)byte_bitset.to_ulong();
		blob << byte;
		instructions = instructions.substr(8, instructions.length() - 8);
	}

	blob.close();

	for (int i = 0; i < tokenized_lines.size(); ++i) {
		delete tokenized_lines[i];
	}

	return 0;
}
