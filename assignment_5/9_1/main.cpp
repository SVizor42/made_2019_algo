/*

A.Алгоритм сжатия данных Хаффмана

Реализуйте алгоритм построения оптимального префиксного кода Хаффмана. При помощи алгоритма реализуйте две функции для создания архива из одного файла и извлечения файла из архива

*/

#include <unordered_map>
#include <queue>
#include "Huffman.h"

using uchar = unsigned char;

struct Node {
	uchar symbol_ = 0;
	size_t count_ = 0;
	Node* left_ = nullptr;
	Node* right_ = nullptr;

	Node(uchar symbol, size_t count = 0) : symbol_(symbol), count_(count), left_(nullptr), right_(nullptr) {}
	Node(Node* left, Node* right);
	bool IsLeftNode() const;
};

struct Compare {
	bool operator()(const Node* left, const Node* right) {
		return left->count_ > right->count_;
	}
};

Node::Node(Node* left, Node* right) {
	symbol_ = 0;
	count_ = left->count_ + right->count_;
	left_ = left;
	right_ = right;
}

bool Node::IsLeftNode() const {
	return (left_ == nullptr);
}

Node* BuildEncodingTree(unordered_map<uchar, size_t>& data) {
	priority_queue<Node*, vector<Node*>, Compare> tree;
	unordered_map<uchar, size_t>::iterator iter;
	for (iter = data.begin(); iter != data.end(); iter++) {
		Node* node = new Node(iter->first, iter->second);   // symbol_ = key, count_ = value
		tree.push(node);
	}

	while (tree.size() > 1) {
		Node* left = tree.top();
		tree.pop();
		Node* right = tree.top();
		tree.pop();

		Node* parent = new Node(left, right);
		tree.push(parent);
	}

	return tree.top();
}

void DeleteNode(Node* node) {
	if (!node)
		return;

	DeleteNode(node->left_);
	DeleteNode(node->right_);
	delete node;
}

void BuildEncodingTable(Node* node, vector<uchar>& code, unordered_map< uchar, vector<uchar> >& table) {
	if (node->left_) {
		code.push_back(0);
		BuildEncodingTable(node->left_, code, table);
	}
	if (node->right_) {
		code.push_back(1);
		BuildEncodingTable(node->right_, code, table);
	}

	if (node->symbol_ || (!node->symbol_ && node->IsLeftNode()))
		table[node->symbol_] = code;
	if (code.size())
		code.pop_back();
}

void EncodeNode(Node* node, vector<bool>& tree) {
	if (node->IsLeftNode()) {
		tree.push_back(1);
		uchar symbol = node->symbol_;
		for (size_t i = 0; i < 8; i++) {
			tree.push_back(symbol & 1);
			symbol >>= 1;
		}
	}
	else {
		tree.push_back(0);
		EncodeNode(node->left_, tree);
		EncodeNode(node->right_, tree);
	}
}

Node* DecodeNode(vector<bool>& tree, size_t& index) {
	if (tree[index++] == 1) {
		uchar symbol = 0;
		for (size_t i = 0; i < 8; i++) {
			symbol |= tree[index] << i;
			index++;
		}
		return new Node(symbol);
	}
	else {
		Node* left = DecodeNode(tree, index);
		Node* right = DecodeNode(tree, index);
		return new Node(left, right);
	}
}

void Encode(IInputStream& original, IOutputStream& compressed)
{
	// Calculate symbol frequency
	unordered_map<uchar, size_t> symbols;
	uchar value = 0;
	std::string input;
	while (original.Read(value)) {
		symbols[value]++;
		input += value;
	}

	// Build encoding tree
	Node* root = BuildEncodingTree(symbols);

	// Build encoding map (table)
	vector<uchar> code;
	unordered_map< uchar, vector<uchar> > encoding_table;
	BuildEncodingTable(root, code, encoding_table);

	// Data encoding process
	// Tree structure encoding
	vector<bool> encoded_message;
	EncodeNode(root, encoded_message);

	// Original message encoding    
	for (uchar c : input) {
		vector<uchar>& encoded_symbol = encoding_table[c];
		for (size_t j = 0; j < encoding_table[c].size(); j++)
			encoded_message.push_back(encoding_table[c][j]);
	}
	encoded_message.push_back(true);

	//  Compress and save data
	size_t i = 0;
	while (i < encoded_message.size()) {
		value = 0;
		for (size_t j = 0; (j < 8) & (i < encoded_message.size()); j++) {
			value |= encoded_message[i] << j;
			i++;
		}
		compressed.Write(value);
	}

	// Remove encoding tree
	DeleteNode(root);
}

void Decode(IInputStream& compressed, IOutputStream& original)
{
	// Read compressed data
	vector<bool> encoded_message;
	uchar symbol;
	while (compressed.Read(symbol)) {
		for (size_t i = 0; i < 8; i++) {
			encoded_message.push_back(symbol & 1);
			symbol >>= 1;
		}
	}

	while (!encoded_message.back())
		encoded_message.pop_back();
	encoded_message.pop_back();

	// Decode and restore tree structure
	size_t index = 0;
	Node* root = DecodeNode(encoded_message, index);

	// Decode original message
	vector<uchar> output;
	Node* node = root;
	while (index < encoded_message.size()) {
		if (!encoded_message[index++]) {
			node = node->left_;
		}
		else {
			node = node->right_;
		}
		if (node->IsLeftNode()) {
			output.push_back(node->symbol_);
			node = root;
		}
	}

	for (size_t i = 0; i < output.size(); i++)
		original.Write(output[i]);

	// Remove encoding tree
	DeleteNode(root);
}
