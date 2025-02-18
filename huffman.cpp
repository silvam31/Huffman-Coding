/*
 * Name: Miggy Silva
 * SL: Renn Stein
 * This program is the implementation for huffman coding. It allows for an input file to be
 * compressed using a huffman tree and also decompressed.
 */

#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "SimpleTest.h"  // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/*
 * This function takes in a queue of bits and a pointer to a binary tree node to return a
 * a decoded message. It does this by iteratively going through each bit and traversing the tree.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string result;
    EncodingTreeNode* root = tree;
    while (!messageBits.isEmpty()){
        Bit bit = messageBits.dequeue();
        if (bit == false){
            tree = tree->zero;
        } else {
            tree = tree->one;
        }
        if(tree->isLeaf()) {
            result += tree->ch;
            tree = root;
        }
    }
    return result;
}

/*
 * This function creates a binary tree given two queues: one of bits (shape) and one characters
 * (leaves). It does this by recursively generating the tree via post-order traversal.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    EncodingTreeNode* Node = new EncodingTreeNode(nullptr, nullptr);
    
    if (treeShape.isEmpty()){
        return Node;
    }
    Bit bit = treeShape.dequeue();
    if (bit == false){
        Node->ch = treeLeaves.dequeue();
        return Node;
    }
    Node->zero = unflattenTree(treeShape, treeLeaves);
    Node->one = unflattenTree(treeShape, treeLeaves);
    return Node;
    
}

/*
 * This function returns the decoded text from the input bit data. It achieves this by
 * calling unflattenTree to unflatten the huffman tree. Then it calls decodeText to
 * transform a sequence of bits to their respective characters with reference to the
 * unflattened tree.
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* tree = unflattenTree(data.treeShape, data.treeLeaves);
    string result = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return result;
}

/*
 * This helper function takes in a priority queue and converts it to an optimal
 * huffman tree. It does this by dequeueing the smallest 2 values, adds them together, and
 * returns them back into the queue via recursion until we get our huffman tree.
 */
void buildHuffmanTreeHelper(PriorityQueue<EncodingTreeNode*>& huffman){
    if (huffman.size() == 1){
        return;
    }
    int zeroPriority = huffman.peekPriority();
    EncodingTreeNode* zero = huffman.dequeue();
    int onePriority = huffman.peekPriority();
    EncodingTreeNode* one = huffman.dequeue();
    EncodingTreeNode* sum = new EncodingTreeNode(zero, one);
    huffman.enqueue(sum, zeroPriority + onePriority);
    buildHuffmanTreeHelper(huffman);
}


/*
 * This function creates a huffman tree based on an input string. It does this by creating
 * a Map to tally the weight of each distinct character in the string. It then puts each
 * character into a priority queue with their respective weights. Finally, the function
 * calls buildHuffmanTreeHelper to generate the huffman tree. If the input string does
 * not have at least two distinct characters, the function raises an error.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    PriorityQueue<EncodingTreeNode*> huffman;
    Map <char, int> priority;
    for (char ch : text){
        priority[ch] += 1;
    }
    if (priority.size() < 2){
        error("string does not contain at least two dinstinct letters");
    }
    for (char key : priority){
        EncodingTreeNode* node = new EncodingTreeNode(key);
        huffman.enqueue(node, priority[key]);
    }
    buildHuffmanTreeHelper(huffman);
    return huffman.dequeue();
}

/*
 * This helper function creates a Map of characters with their associated sequence of bits
 * in a given binary tree. It does this by recursively traversing the tree and keeping track
 * of the bits assoicated with each move.
 */
void encodeTextHelper(EncodingTreeNode* tree, Map<char, Vector<Bit>>& bitMap, Vector<Bit> bits){
    if (tree->isLeaf()){
        bitMap[tree->ch] = bits;
        return;
    }
    encodeTextHelper(tree->zero, bitMap, bits + 0);
    encodeTextHelper(tree->one, bitMap, bits + 1);
}


/*
 * This function returns an encoded queue of bits given an input tree and text. It does this
 * by calling encodeTextHelper to generate a map of characters to bit sequences. Then it
 * uses this map to generate a bit queue that contains the bit representation of the stirng.
 */

Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Map<char, Vector<Bit>> bitMap;
    Vector<Bit> bits;
    encodeTextHelper(tree, bitMap, bits);
    Queue<Bit> result;
    for (char ch : text){
        Vector<Bit> bitVector = bitMap[ch];
        for (int i = 0; i < bitVector.size(); i++){
            result.enqueue(bitVector.get(i));
        }
    }
    return result;
}

/*
 * This function takes in a tree and generates its bit sequence and character sequence via
 * recursion.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (tree->isLeaf()){
        treeLeaves.enqueue(tree->ch);
        treeShape.enqueue(0);
        return;
    }
    treeShape.enqueue(1);
    flattenTree(tree->zero, treeShape, treeLeaves);
    flattenTree(tree->one, treeShape, treeLeaves);
}

/*
 * This function takes in an input string and outputs its compressed bit data
 * First, it calls buildHuffmanTree to create an optimal huffman tree based on the input string.
 * Then it flattens the tree by calling flattenTree. Finally, it encodes the string into bit data
 * using encode Text. The resulting output is a struct containing the flattened huffman tree and
 * bit text.
 */
EncodedData compress(string messageText) {
    EncodedData result;
    EncodingTreeNode* huffmanTree = buildHuffmanTree(messageText);
    flattenTree(huffmanTree, result.treeShape, result.treeLeaves);
    result.messageBits = encodeText(huffmanTree, messageText);
    deallocateTree(huffmanTree);
    return result;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* E = new EncodingTreeNode('E');
    
    EncodingTreeNode* three = new EncodingTreeNode(R, S);
    EncodingTreeNode* two = new EncodingTreeNode(three, E);
    EncodingTreeNode* T = new EncodingTreeNode('T');
 
    EncodingTreeNode* tree = new EncodingTreeNode(T, two);
    return tree;
}

void deallocateTree(EncodingTreeNode* t) {
    if (t == nullptr){
        return;
    }
    if (t->isLeaf()){
        delete t;
        return;
    }
    deallocateTree(t->zero);
    deallocateTree(t->one);
    delete t;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    if (a == nullptr && b == nullptr){
        return true;
    }
    if (a== nullptr || b == nullptr){
        return false;
    }
    if (a->isLeaf()){
        if (b->isLeaf()){
            if(a->getChar() == b->getChar()){
                return true;
            }
        }
        return false;
    }
    if (!areEqual(a->zero, b->zero)){
        return false;
    }
    return areEqual(a->one, b->one);
}
/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("deallocated tree"){
    EncodingTreeNode* example = createExampleTree();
    deallocateTree(example);
}

STUDENT_TEST("areEqual nullptr"){
    EncodingTreeNode* A = nullptr;
    EncodingTreeNode* B = nullptr;
    EXPECT_EQUAL(areEqual(A, B), true);
    
    deallocateTree(A);
    deallocateTree(B);
}

STUDENT_TEST("areEqual test simple tree with null tree"){
    EncodingTreeNode* A = new EncodingTreeNode('A');
    EncodingTreeNode* B = new EncodingTreeNode('B');
    EncodingTreeNode* root = new EncodingTreeNode(A, B);
    EncodingTreeNode* null = nullptr;
    EXPECT_EQUAL(areEqual(null, root), false);
    
    deallocateTree(root);
    deallocateTree(null);
}

STUDENT_TEST("areEqual simple test of same trees"){
    EncodingTreeNode* A = new EncodingTreeNode('A');
    EncodingTreeNode* B = new EncodingTreeNode('B');
    EncodingTreeNode* root = new EncodingTreeNode(A, B);
    EXPECT_EQUAL(areEqual(root, root), true);
    
    deallocateTree(root);
}

STUDENT_TEST("areEqual test simple tree with example tree"){
    EncodingTreeNode* A = new EncodingTreeNode('A');
    EncodingTreeNode* B = new EncodingTreeNode('B');
    EncodingTreeNode* root = new EncodingTreeNode(A, B);
    EncodingTreeNode* example = createExampleTree();
    EXPECT_EQUAL(areEqual(example, root), false);
    
    deallocateTree(root);
    deallocateTree(example);
}

STUDENT_TEST("areEqual with two example trees"){
    EncodingTreeNode* example1 = createExampleTree();
    EncodingTreeNode* example2 = createExampleTree();
    EXPECT_EQUAL(areEqual(example1, example2), true);
    
    deallocateTree(example1);
    deallocateTree(example2);
}

STUDENT_TEST("areEqual with example tree and subtree"){
    EncodingTreeNode* example = createExampleTree();
    
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* E = new EncodingTreeNode('E');
    
    EncodingTreeNode* three = new EncodingTreeNode(R, S);
    EncodingTreeNode* subTree = new EncodingTreeNode(three, E);
    
    EXPECT_EQUAL(areEqual(example, subTree), false);
    
    deallocateTree(example);
    deallocateTree(subTree);
}

STUDENT_TEST("decodeText, example tree TSER") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 0, 1, 0, 1, 1, 1, 1, 0, 0 }; // TSER
    EXPECT_EQUAL(decodeText(tree, messageBits), "TSER");

    deallocateTree(tree);
}

STUDENT_TEST("decodeText, example tree TSER") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 0, 1, 0, 1, 1, 1, 1, 0, 0 }; // TSER
    EXPECT_EQUAL(decodeText(tree, messageBits), "TSER");

    deallocateTree(tree);
}

STUDENT_TEST("decodeText, example tree REST") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 0, 0, 1, 1, 1, 0, 1, 0 }; // REST
    EXPECT_EQUAL(decodeText(tree, messageBits), "REST");

    deallocateTree(tree);
}

STUDENT_TEST("decodeText, example tree only 1") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1,}; // EEEEE
    EXPECT_EQUAL(decodeText(tree, messageBits), "EEEEE");

    deallocateTree(tree);
}

STUDENT_TEST("decodeText, example tree only 0") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 0, 0, 0, 0, 0, 0, 0,}; // TTTTTTT
    EXPECT_EQUAL(decodeText(tree, messageBits), "TTTTTTT");

    deallocateTree(tree);
}

STUDENT_TEST("decompress, small example input 1 from warmup") {
    EncodedData data = {
        { 1, 0, 1, 0, 0 }, // treeShape
        { 'E', 'W', 'K'},  // treeLeaves
        { 1, 1, 0, 1, 1, 1, 0} // messageBits
    };

    EXPECT_EQUAL(decompress(data), "KEKW");
}

STUDENT_TEST("decompress, small example input 3 from warmup") {
    EncodedData data = {
        { 1, 1, 0, 1, 0, 0, 0 }, // treeShape
        { 'N', 'M', 'S', 'O' },  // treeLeaves
        { 0, 1, 0, 1, 1, 0, 0, 0, 1, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "MOONS");
}

STUDENT_TEST("encode bit Helper simple test"){
    EncodingTreeNode* tree = createExampleTree();
    Map<char, Vector<Bit>> bitMap;
    Vector<Bit> bits;
    
    encodeTextHelper(tree, bitMap, bits);
    EXPECT_EQUAL(bitMap['T'], {0});
    EXPECT_EQUAL(bitMap['E'], {1,1});
    EXPECT_EQUAL(bitMap['R'], {1,0,0});
    EXPECT_EQUAL(bitMap['S'], {1,0,1});
    
    deallocateTree(tree);
}

STUDENT_TEST("encodeText, encode followed by decode should return original") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    
    Queue<Bit> message = encodeText(reference, "TRESSRETT");
    EXPECT_EQUAL(decodeText(reference, message), "TRESSRETT");
    
    message = encodeText(reference, "T");
    EXPECT_EQUAL(decodeText(reference, message), "T");
    
    message = encodeText(reference, "E");
    EXPECT_EQUAL(decodeText(reference, message), "E");
    
    message = encodeText(reference, "SSSSSRSSSSS");
    EXPECT_EQUAL(decodeText(reference, message), "SSSSSRSSSSS");
    
    deallocateTree(reference);
}

STUDENT_TEST("flattenTree, flatten and then unflatten should return original") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    
    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);
    EncodingTreeNode* test = unflattenTree(treeShape, treeLeaves);
    EXPECT_EQUAL(areEqual(reference, test), true);

    deallocateTree(reference);
    deallocateTree(test);
}

STUDENT_TEST("buildHuffmanTree should raise error for less than 2 distinct characters"){
    string input = "HHHHHHHH";
    EXPECT_ERROR(buildHuffmanTree(input));
    
    input = "O";
    EXPECT_ERROR(buildHuffmanTree(input));
}

STUDENT_TEST("buildHuffmanTree should not raise error"){
    string input = "HO";
    EncodingTreeNode* tree = buildHuffmanTree(input);
    EXPECT_NO_ERROR(tree);
    deallocateTree(tree);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman",
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
