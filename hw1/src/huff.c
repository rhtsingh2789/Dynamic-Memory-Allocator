#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "huff.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the nodes of the Huffman tree and other data have
 * been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/**
 * @brief Emits a description of the Huffman tree used to compress the current block.
 * @details This function emits, to the standard output, a description of the
 * Huffman tree used to compress the current block.  Refer to the assignment handout
 * for a detailed specification of the format of this description.
 */
int compare_string(char *first, char *second);
int myAtoi(char *str);
// int length_count(int n);
int char_exists(char c);
void printString(unsigned const char *ptr);

int find_smallest_two();
void displayNode();
void set_parents();
void num_nodes_new();
void prepare_next_block();
void put_leaves_in_arr();
void displayNode_symbols();
void final_compress();
void make_histogram();


void emit_huffman_tree() {
    //debug("%d", num_nodes);
    //displayNode();

    while(num_nodes>1){
        find_smallest_two();
        num_nodes--;
    }
    //displayNode();
    num_nodes_new();
    set_parents();
    put_leaves_in_arr();
    //displayNode_symbols();
    final_compress();
    // To be implemented.
}

/**
 * @brief Reads a description of a Huffman tree and reconstructs the tree from
 * the description.
 * @details  This function reads, from the standard input, the description of a
 * Huffman tree in the format produced by emit_huffman_tree(), and it reconstructs
 * the tree from the description.  Refer to the assignment handout for a specification
 * of the format for this description, and a discussion of how the tree can be
 * reconstructed from it.
 *
 * @return 0 if the tree is read and reconstructed without error, otherwise 1
 * if an error occurs.
 */
int read_huffman_tree() {
    // To be implemented.
    return 1;
}

/**
 * @brief Reads one block of data from standard input and emits corresponding
 * compressed data to standard output.
 * @details This function reads raw binary data bytes from the standard input
 * until the specified block size has been read or until EOF is reached.
 * It then applies a data compression algorithm to the block and outputs the
 * compressed block to the standard output.  The block size parameter is
 * obtained from the global_options variable.
 *
 * @return 0 if compression completes without error, 1 if an error occurs.
 */
int compress_block() {
    // To be implemented.

    // implement the nodes array shit using the current_block array
    int c;
    char charac;
    int count_blocksize = 0;
    unsigned int b = 0;
    b |= global_options;
    b = b >> 16;
    b++;
    unsigned char *mypointer = current_block;
    charac = getchar();
    while(1) {
        if(charac == EOF) {
            if(count_blocksize > 0) {
                make_histogram();
                emit_huffman_tree();
            }
            return 1;
        }

        if(ferror(stdin) != 0) {
            return 1;
        }

        if(count_blocksize == b) {
            make_histogram();
            emit_huffman_tree();
            return 0;
        }

        *mypointer = charac;
        mypointer+=1;
        count_blocksize++;
        //printf("%c\n", charac);
        charac = getchar();
    }
    return 1;


    // while ((c = fgetc(file)) != EOF){
    //     charac = (char) c;
    //     if(count_blocksize == b) {
    //         //printf("%c", charac);
    //         compress_block();
    //         return 0;
    //         count_blocksize = 0;
    //         mypointer = current_block;
    //     }
    //     else {
    //         *mypointer = charac;
    //         mypointer+=1;
    //     }

    //     count_blocksize++;
    // }
    // compress_block();
    // return 0;

}

/**
 * @brief Reads one block of compressed data from standard input and writes
 * the corresponding uncompressed data to standard output.
 * @details This function reads one block of compressed data from the standard
 * inputk it decompresses the block, and it outputs the uncompressed data to
 * the standard output.  The input data blocks are assumed to be in the format
 * produced by compress().  If EOF is encountered before a complete block has
 * been read, it is an error.
 *
 * @return 0 if decompression completes without error, 1 if an error occurs.
 */
int decompress_block() {
    // To be implemented.
    return 1;
}

/**
 * @brief Reads raw data from standard input, writes compressed data to
 * standard output.
 * @details This function reads raw binary data bytes from the standard input in
 * blocks of up to a specified maximum number of bytes or until EOF is reached,
 * it applies a data compression algorithm to each block, and it outputs the
 * compressed blocks to standard output.  The block size parameter is obtained
 * from the global_options variable.
 *
 * @return 0 if compression completes without error, 1 if an error occurs.
 */
int compress() {
    int ret_cb = 0;
    while(1) {
        ret_cb = compress_block();
        if(ret_cb == 1) {
            if(ferror(stdin) != 0) {
                return 1;
            }
            else
                return 0;
        }
        prepare_next_block();
    }
}

/**
 * @brief Reads compressed data from standard input, writes uncompressed
 * data to standard output.
 * @details This function reads blocks of compressed data from the standard
 * input until EOF is reached, it decompresses each block, and it outputs
 * the uncompressed data to the standard output.  The input data blocks
 * are assumed to be in the format produced by compress().
 *
 * @return 0 if decompression completes without error, 1 if an error occurs.
 */
int decompress() {
    // To be implemented.
    return 1;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and 1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and 1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    argv++;
    if(compare_string(*argv, "-h") == 0){
        global_options |= 1UL << 0;
        return  0;
    }
        //printf("%s", *(argv));
    if(compare_string(*argv, "-d") == 0) {
        if (argc > 2 ) {
            return 1;
        }
        else {
            global_options |= 1UL << 2;
            return 0;
        }
    }

    if(compare_string(*argv, "-c") == 0) {
        if (argc > 2) {
            argv++;
            if(compare_string(*argv, "-b") == 0) {

                if (argc == 4) {
                    argv++;

                    int num = myAtoi(*argv);

                    if (num < 0 || num < 1024 || num > 65536) {
                        return 1;
                    }

                    num = num-1;
                    // int length_of_arg = length_count(num);
                    num = num << (16);
                    global_options |= 1UL << 1;
                    global_options |= num;
                    return 0;
                }
                else
                    return 1;
            }
            else
                return 1;
        }
        else {
            int nn = 0;
            nn = 65535;
            nn = nn << (16);
            global_options |= 1UL << 1;
            global_options |= nn;
            return 0;
        }

    }
    return 1;
}



int compare_string(char *first, char *second) {
   while (*first == *second) {
      if (*first == '\0' || *second == '\0')
         break;
      first++;
      second++;
   }
   if (*first == '\0' && *second == '\0')
      return 0;
   else
      return -1;
}

int myAtoi(char *str)
{
    int res = 0;
    for (int i = 0; *str != '\0'; ++i) {
        int re = *str-'0';
        if (re < 0 || re > 9) {
            return -1;
        }
        res = res*10 + (*str - '0');
        str++;
    }
    return res;
}

// int length_count(int n) {
//     int l_count = 0;
//     while(n!=0) {
//         n /= 10;
//         l_count++;
//     }
//     return l_count;
// }

int char_exists(char c) {
    //NODE **pointer = node_for_symbol;
    NODE *pointer_nodes = nodes;
    while((*pointer_nodes).weight != 0) {
        if((*pointer_nodes).symbol == (short) c) {
            (*pointer_nodes).weight++;
            return 0;
        }
        pointer_nodes++;
    }
    (*pointer_nodes).symbol = (short) c;
    (*pointer_nodes).weight = 1;

    //(*pointer) =(NODE *) pointer_nodes;
    //pointer++;
    return 1;
}

// void printString(unsigned const char *ptr)
// {

//     //for statement to print values using array
//     for( ; *ptr!='\0'; ++ptr)
//         printf("%c", *ptr);
// }

int find_smallest_two()
{
    NODE *pointer = nodes;
    NODE smallest = *pointer;

    int num_nodes_counter = 0;

    int first_index = 0;

    while(num_nodes_counter < num_nodes) {
        if((*pointer).weight<smallest.weight) {
            smallest = *pointer;
            first_index = num_nodes_counter;
        }
        num_nodes_counter++;
        pointer++;
    }

    pointer = nodes;

    pointer += first_index;

    (*pointer).symbol = 0;
    (*pointer).weight = 0;
    (*pointer).left = 0;
    (*pointer).right = 0;

    NODE *pointer_two = nodes;
    NODE second_smallest = *pointer_two;

    num_nodes_counter = 0;

    int second_index = 0;

    if(((*pointer_two).symbol == 0 && (*pointer_two).weight == 0)) {
        pointer_two++;
        second_smallest = *pointer_two;
        num_nodes_counter += 1;
        second_index = num_nodes_counter;
    }

    while(num_nodes_counter < num_nodes) {
        if((*pointer_two).weight < second_smallest.weight && (*pointer_two).weight != 0) {
            second_smallest = *pointer_two;
            second_index = num_nodes_counter;
        }
        num_nodes_counter++;
        pointer_two++;
    }

    pointer_two = nodes;

    pointer_two += second_index;

    (*pointer_two).symbol = 0;
    (*pointer_two).weight = 0;
    (*pointer_two).left = 0;
    (*pointer_two).right = 0;

    int counter_to_end = 0;
    NODE *pointer_to_last_two = nodes;

    pointer_to_last_two += 2*(num_nodes)-3;

    if(first_index < second_index) {
        (*pointer).weight = smallest.weight + second_smallest.weight;
        //pointer_to_last_two--;
        (*pointer).left = pointer_to_last_two;
        pointer_to_last_two++;
        (*pointer).right = pointer_to_last_two;

        // debug("%c", (*((*pointer).left)).symbol);
        // debug("%c", (*((*pointer).right)).symbol);
        counter_to_end = second_index;
        while(counter_to_end < num_nodes) {
            (*pointer_two).symbol = (*(pointer_two+1)).symbol;
            (*pointer_two).weight = (*(pointer_two+1)).weight;
            (*pointer_two).left = (*(pointer_two+1)).left;
            (*pointer_two).right = (*(pointer_two+1)).right;
            pointer_two++;
            counter_to_end++;
        }
    }
    else {
        (*pointer_two).weight = smallest.weight + second_smallest.weight;
        //pointer_to_last_two--;
        counter_to_end = first_index;
        (*pointer_two).left = pointer_to_last_two;
        pointer_to_last_two++;
        (*pointer_two).right = pointer_to_last_two;
        // debug("%c", (*((*pointer_two).left)).symbol);
        // debug("%c", (*((*pointer_two).right)).symbol);
        while(counter_to_end < num_nodes) {
            (*pointer).symbol = (*(pointer+1)).symbol;
            (*pointer).weight = (*(pointer+1)).weight;
            (*pointer).left = (*(pointer+1)).left;
            (*pointer).right = (*(pointer+1)).right;
            pointer++;
            counter_to_end++;
        }
    }

    pointer_to_last_two = nodes;

    pointer_to_last_two += 2*(num_nodes)-3;
    (*pointer_to_last_two).symbol = smallest.symbol;
    (*pointer_to_last_two).weight = smallest.weight;
    (*pointer_to_last_two).left = smallest.left;
    (*pointer_to_last_two).right = smallest.right;

    pointer_to_last_two++;

    (*pointer_to_last_two).symbol = second_smallest.symbol;
    (*pointer_to_last_two).weight = second_smallest.weight;
    (*pointer_to_last_two).left = second_smallest.left;
    (*pointer_to_last_two).right = second_smallest.right;

    //debug("SMALLEST %d               SECOND SMALLEST %d\n", smallest.weight, second_smallest.weight);
    return 1;
}

void displayNode(){
    NODE *p = nodes;
    int counterrr = num_nodes;
    int count =0;
    while (count < 73){
        //debug("%c %d\n", (*p).symbol, (*p).weight);
        counterrr--;
        p++;
        count++;
    }
}

void num_nodes_new(){
    NODE *pointer = nodes;
    int count = 0;
    while(((*pointer).symbol != 0) || (*pointer).weight != 0) {
        count++;
        pointer++;
    }
    num_nodes = count;
}

void set_parents(){
    NODE *pointer = nodes;
    //debug("%d\n", num_nodes);
    int x =0;
    while(((*pointer).symbol != 0) || (*pointer).weight != 0) {
        if((*pointer).left != 0){
            (*((*pointer).left)).parent = pointer;
            //debug("%d", (*(*((*pointer).left)).parent).weight);
        }
        if((*pointer).right != 0){
            (*((*pointer).right)).parent = pointer;
            //debug("%d", (*(*((*pointer).right)).parent).weight);
            x+=1;
        }
        pointer++;
    }
}


void put_leaves_in_arr() {
    NODE *pointer = nodes;
    NODE **pointer_symbol = node_for_symbol;

    while(((*pointer).symbol != 0) || (*pointer).weight != 0) {
        if((*pointer).left == 0 && (*pointer).right == 0) {
            (*pointer_symbol) =(NODE *) pointer;
            pointer_symbol++;
        }
        pointer++;
    }
}

void displayNode_symbols() {
    NODE **k = node_for_symbol;
    while((*k)!=NULL){
        //debug("%c   ", (char)(*k)->symbol);
        //debug("%d\n", (*k)->weight );
        k+=1;
    }
}


void final_compress() {
    NODE **pointer_symbol = node_for_symbol;
    NODE *pointer = nodes;
    NODE *pointer_two = nodes;
    unsigned int code_to_be_outputted = 0;
    if((*pointer).parent != 0){
            //debug("YEOOOOO");
        }
    while((*pointer_symbol) != 0) {
        code_to_be_outputted = 0;
        pointer = (*pointer_symbol);
        while((*pointer).parent != 0) {

            pointer_two = (*pointer).parent;

            if((*pointer_two).left == pointer) {
                code_to_be_outputted  = code_to_be_outputted << 1;
                //debug("0");
                pointer = pointer_two;
            }
            else if((*pointer_two).right == pointer) {
                code_to_be_outputted = code_to_be_outputted << 1;
                code_to_be_outputted |= 1;
                //debug("1");
                pointer = pointer_two;
                //debug("Wrong bin");
            }
        }
        //debug("\n\n\n\n\n\n");
        pointer_symbol++;
        //debug("%x", code_to_be_outputted);
    }
}

void prepare_next_block(){
    num_nodes = 0;
    NODE *pointernd = nodes;
    int numbernodes = (257*2) - 1;

    while(numbernodes > 0) {
        (*pointernd).symbol = 0;
        (*pointernd).weight = 0;
        (*pointernd).left = 0;
        (*pointernd).right = 0;
        (*pointernd).parent = 0;
        pointernd++;
        numbernodes--;
    }


    NODE **pointer_for_symbol = node_for_symbol;
    numbernodes = 257;
    int count = 0;
    while(count < numbernodes) {
        (*pointer_for_symbol) = '\0';
        pointer_for_symbol++;
        count++;
    }

    count = 0;
    unsigned int bsize = global_options >> 16;
    bsize++;
    unsigned char *p = current_block;
    while(count < bsize) {
        *p = '\0';
        p++;
        count++;
    }

}

void make_histogram() {
    unsigned char *c_block_pointer = current_block;
    while(*c_block_pointer != '\0') {
        if(char_exists((*c_block_pointer)) == 1) {
            num_nodes++;
        }
        c_block_pointer++;
    }

    NODE *end_pointer = nodes;
    end_pointer += num_nodes;

    (*end_pointer).symbol = 0xFF00;
    (*end_pointer).weight = 0;
    num_nodes++;
    return;
}