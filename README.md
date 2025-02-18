# Huffman Coding

This repository contains an implementation of Huffman Coding, a lossless data compression algorithm. The program allows you to compress an input text file into a compressed binary format and decompress it back into the original text.

## Features

- **Compression**: Builds an optimal Huffman tree from input text and encodes it into a compressed binary representation.
- **Decompression**: Reconstructs the Huffman tree and decodes the compressed binary data back into text.
- **Tree Construction**: Implements a priority queue-based approach to generate the Huffman tree.
- **Encoding and Decoding**: Uses the Huffman tree to map characters to unique bit sequences for efficient compression.

## How It Works

1. **Build Huffman Tree**: 
   - Calculate frequency of each character in the input text.
   - Construct a binary tree where lower-frequency characters have longer codes.
   
2. **Encode Text**:
   - Generate a mapping of characters to their Huffman codes.
   - Convert input text into a sequence of bits.

3. **Store Compressed Data**:
   - Save the encoded bit sequence along with the structure of the Huffman tree.

4. **Decode Compressed Data**:
   - Reconstruct the Huffman tree.
   - Traverse the tree using the bit sequence to recover the original text.

## Files

- **`huffman.cpp`**: Main implementation of Huffman coding, including tree construction, encoding, and decoding.
- **Other dependencies**: Uses `bits.h`, `treenode.h`, `huffman.h`, and additional utility libraries.

## Usage

Compile and run the program to compress and decompress text files. Modify the input file in the main function to test different examples.

## Example Screenshot

Below is an example of the program running with sample input:

![Huffman Coding Example](https://raw.githubusercontent.com/your-username/your-repo/main/example.png)

To add your own screenshot:
1. Run the program and take a screenshot.
2. Upload the image to your repository.
3. Replace the URL in the Markdown above with your uploaded image’s direct link.

## Testing

Includes a set of test cases using the `SimpleTest` framework to validate encoding, decoding, and overall compression-decompression accuracy.

## Author

Miguel Silva  
Stanford University  
