# Huffman Coding

This repository contains an implementation of Huffman Coding, a lossless data compression algorithm. The program allows you to compress an input text file into a compressed binary format and decompress it back into the original text. This program was implemented as part of an assignment for Programming Abstractions.

## Features

- **Compression**: Builds an optimal Huffman tree from input text and encodes it into a compressed binary representation.
- **Decompression**: Reconstructs the Huffman tree and decodes the compressed binary data back into text.

## How Compression Works

1. **Build Huffman Tree**: 
   - Calculate frequency of each character in the input text.
   - Construct a binary tree where lower-frequency characters are farther down the tree.
   
2. **Encode Text**:
   - Generate a mapping of characters to their bit representation based on the Huffman Tree.
   - Convert input text into a sequence of bits.

3. **Flatten Tree**:
   - Convert huffman tree to a bit representation.

4. **Store Compressed Data**:
   - Save the encoded bit sequence along with the flattened Huffman tree.

## How Decompression Works

1. **Extract Compressed Data**:  
   - Retrieve the stored bit sequence and the flattened Huffman tree from the compressed file.

2. **Reconstruct Huffman Tree**:  
   - Use the flattened tree structure to rebuild the original Huffman tree.

3. **Decode Text**:  
   - Traverse the reconstructed Huffman tree using the bit sequence.  
   - Convert the bits back into their respective characters to recover the original text.

## Files

- **`huffman.cpp`**: Main implementation of Huffman coding, including tree construction, encoding, and decoding.
- **Other dependencies**: Uses `bits.h`, `treenode.h`, `huffman.h`, and additional utility libraries.
- **`huffman.zip`**: This executes the Huffman program.
- **Huffman Files**: This contains files that you can compress and decompress using Huffman code!
- **Assignment Starter**: Includes a set of test cases and any other necessary files for the program to run.

## Encoding and Decoding Example

The diagram below is an encoding tree for the characters O N M and S. 
![Huffman Coding Example](https://github.com/silvam31/Huffman-Coding/blob/10fc0943746bff5d12fc891e940a526a9002f8b2/huffman_example1.png)

Each leaf node corresponds to a character. The path from root to a leaf node traces the sequence of bits that encode the node's character. In the diagram, interior nodes are marked with * for visualization purposes.

We label the leftward branch zero and rightward one. The path from the root node to the leaf node S traces left-right-right which corresponds to the bit sequence 011.

## Flattening and Unflattening Example

The diagram below shows two encoding trees and their respective flattened bit representation and sequence of characters. 
![Huffman Coding Example](https://github.com/silvam31/Huffman-Coding/blob/777d6658826ee2ee69a7658457d46afa08a6a3fb/huffman_example2.png)

The sequence of bits describes the tree structure in the order that the tree nodes are visited in a pre-order traversal where 1 represents an interiror (or root) node and 0 represents a leaf node.

The tree leaves are flattened into a sequence of characters by listing the characters of the leaf nodes as visited during an in-order traversal. 

## Credits
This assignment was completed at Stanford University in CS106B: Programming Abstractions. This course was instructed and facilitated by Chris Greg, Neel Kishnani, and associated teaching team. Huffman tree examples and descriptions are with reference to an explanation given by Julie Zelenski.

## Author

Miguel Silva  
Stanford University  
