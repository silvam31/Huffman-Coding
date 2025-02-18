# Huffman Coding

This repository contains an implementation of Huffman Coding, a lossless data compression algorithm. The program allows you to compress an input text file into a compressed binary format and decompress it back into the original text.

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

## Usage

Compile and run the program to compress and decompress text files. Modify the input file in the main function to test different examples.

## Example Screenshot

Below is an example of the program running with sample input:
![Huffman Coding Example](https://github.com/silvam31/Huffman-Coding/huffman example1_png)

To add your own screenshot:
1. Run the program and take a screenshot.
2. Upload the image to your repository.
3. Replace the URL in the Markdown above with your uploaded image’s direct link.

## Author

Miguel Silva  
Stanford University  
