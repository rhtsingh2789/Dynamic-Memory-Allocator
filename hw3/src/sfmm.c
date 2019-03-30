/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "debug.h"
#include "sfmm.h"

void coalesce(sf_block *ptr);

void *sf_malloc(size_t size) {
    if(size <= 0) {
        return NULL;
    }

    if(sf_free_list_head.body.links.next == NULL) {
        if(sf_mem_grow() == NULL) {
            sf_errno = ENOMEM;
            return NULL;
        }

        sf_prologue *start = (sf_prologue *) sf_mem_start();
        sf_epilogue *end = (sf_epilogue *) sf_mem_end();
        void *mem_block_p;

        sf_prologue pro1;
        sf_epilogue epi1;

        sf_block mem_block;
        sf_header mem_block_foot;

        pro1.block.header.block_size = 33;
        pro1.block.header.requested_size = 0;
        pro1.footer.block_size = 33;
        pro1.footer.requested_size = 0;
        *start = pro1;

        start++;
        mem_block_p = start;
        mem_block.header.block_size = 4048;
        //mem_block.header.block_size |= 0x1;
        mem_block.header.requested_size = 0;
        mem_block.body.links.next = &sf_free_list_head;
        mem_block.body.links.prev = &sf_free_list_head;

        *((sf_block *) mem_block_p) = mem_block;

        sf_free_list_head.body.links.next = (sf_block *) mem_block_p;
        sf_free_list_head.body.links.prev = (sf_block *) mem_block_p;

        mem_block_p += 4040;

        mem_block_foot.block_size = 4048;
        mem_block_foot.requested_size = 0;

        *((sf_header *) mem_block_p) = mem_block_foot;

        epi1.header.block_size = 1;
        epi1.header.requested_size = 0;
        end--;
        *end = epi1;
        //sf_show_heap();
    }



    int new_size = size;

    new_size += sizeof(sf_header);

    if((new_size % 16) != 0) {
        while(new_size % 16 != 0) {
            new_size++;
        }
    }

    if(new_size < 32) {
        new_size = 32;
    }

    if(((new_size-32)/16) < 10) {
        int index = ((new_size-32)/16);
        if(sf_quick_lists[index].first != NULL) {
            sf_block head;
            //head.block_size = new_size;
            head.header.requested_size = size;
            *(sf_quick_lists[index].first) = head;
            sf_block *p = sf_quick_lists[index].first;
            sf_quick_lists[index].length--;
            sf_quick_lists[index].first = (*sf_quick_lists[index].first).body.links.next;
            return p;
        }
    }
        if(sf_free_list_head.body.links.next != &sf_free_list_head) {
            sf_block *curr_block = sf_free_list_head.body.links.next;
            sf_block block_to_return;
            sf_block leftover;
            while(1==1){
                curr_block = sf_free_list_head.body.links.next;
                while(curr_block != &sf_free_list_head) {
                    //debug("ENTERS WHILE");
                    unsigned int blocky_sizee = (*curr_block).header.block_size;
                    blocky_sizee = blocky_sizee >> 2;
                    blocky_sizee = blocky_sizee << 2;
                    //debug("%d, %d", blocky_sizee, new_size);
                    if(blocky_sizee < new_size) {
                        //debug("ENTERS WHILE");
                        break;
                    }
                    if(blocky_sizee >= new_size) {
                        block_to_return = *curr_block;
                        //debug("%d, %d", blocky_sizee, new_size);
                        if(blocky_sizee - new_size >= 32) {
                            //debug("SPLITTING");
                            block_to_return = *curr_block;
                            (*block_to_return.body.links.prev).body.links.next = block_to_return.body.links.next;
                            (*block_to_return.body.links.next).body.links.prev = block_to_return.body.links.prev;
                            block_to_return.header.requested_size = size;
                            block_to_return.header.block_size = new_size + 1;

                            void *poin = (void *) curr_block;
                            poin += new_size;

                            leftover.body.links.next = sf_free_list_head.body.links.next;
                            leftover.body.links.prev = &sf_free_list_head;
                            (*sf_free_list_head.body.links.next).body.links.prev = (sf_block *) poin;
                            sf_free_list_head.body.links.next = (sf_block *) poin;
                            leftover.header.block_size = blocky_sizee - new_size + 2;
                            leftover.header.requested_size = 0;
                            *((sf_block *) poin) = leftover;
                            poin += leftover.header.block_size - 2;
                            sf_header *footer_leftover_pointer = (sf_header *) poin;
                            footer_leftover_pointer--;

                            sf_header footer_leftover;
                            footer_leftover.requested_size = 0;
                            footer_leftover.block_size = leftover.header.block_size;
                            *footer_leftover_pointer = footer_leftover;
                            *curr_block = block_to_return;
                            //sf_show_heap();
                            //debug("%p/n", curr_block);
                            curr_block = (void *) curr_block + sizeof(sf_header);
                            return curr_block;
                        }
                        else{
                            block_to_return = *curr_block;
                            (*block_to_return.body.links.prev).body.links.next = block_to_return.body.links.next;
                            (*block_to_return.body.links.next).body.links.prev = block_to_return.body.links.prev;
                            block_to_return.header.block_size += 1;
                            block_to_return.header.requested_size = size;
                            curr_block = ((void *)curr_block)+blocky_sizee;
                            if((void *) curr_block == (void *) (sf_mem_end() - sizeof(sf_epilogue))) {
                                (*(sf_header *) curr_block).block_size += 2;
                            }
                            else {
                                (*curr_block).header.block_size += 2;
                            }
                            curr_block = (void*) curr_block - blocky_sizee;
                            *curr_block = block_to_return;
                            //sf_show_heap();
                            curr_block = (void *) curr_block + sizeof(sf_header);
                            return curr_block;
                        }
                    }
                    curr_block = (*curr_block).body.links.next;
                }
                bool prev_alloc = false;
                void *grown = sf_mem_grow();
                if(grown == NULL) {
                sf_errno = ENOMEM;
                return NULL;
                }
                grown -= sizeof(sf_epilogue);
                if((*(sf_header *) grown).block_size % 16 == 2) {
                    prev_alloc = true;
                }
                //grown += sizeof(sf_epilogue);
                sf_block new_page;
                new_page.header.block_size = PAGE_SZ;
                if(prev_alloc == true) {
                    new_page.header.block_size += 2;
                }
                new_page.header.requested_size = 0;
                // new_page.body.links.prev = &sf_free_list_head;
                // new_page.body.links.next = sf_free_list_head.body.links.next;
                // (*sf_free_list_head.body.links.next).body.links.prev = (sf_block *) grown;
                // sf_free_list_head.body.links.next = (sf_block *) grown;
                *((sf_block *) grown) = new_page;
                sf_block *coalate_block = (sf_block *) grown;
                grown += new_page.header.block_size;
                if(prev_alloc == true) {
                    grown -= 2;
                }
                sf_header new_page_footer;
                new_page_footer.block_size = new_page.header.block_size;
                new_page_footer.requested_size = 0;
                *((sf_header *) grown) = new_page_footer;

                grown += sizeof(sf_header);
                grown = sf_mem_end();
                grown = (sf_epilogue  *) grown - 1;
                sf_epilogue epi2;
                epi2.header.block_size = 1;
                epi2.header.requested_size = 0;
                *((sf_epilogue *) grown) = epi2;
                coalesce(coalate_block);
                // sf_show_heap();

            }

        }

    return NULL;
}



void sf_free(void *pp) {
    pp -= sizeof(sf_header);
    sf_block *block_free = (sf_block *) pp;
    int size_of_block = (*block_free).header.block_size;
    size_of_block = size_of_block >> 2;
    size_of_block = size_of_block << 2;

    if(((size_of_block - 32) / 16) < 10) {
        int index = ((size_of_block - 32) / 16);
        (*block_free).body.links.next = sf_quick_lists[index].first;
        sf_quick_lists[index].first = block_free;
        sf_quick_lists[index].length++;
        //sf_show_heap();
    }
    return;
}

void *sf_realloc(void *pp, size_t rsize) {
    return NULL;
}


void coalesce(sf_block *ptr) {
    /*
    4 cases:
    prev and next allocated
    prev free next allocated
    prev allocated next free
    prev and next free
    */
    bool prev_allocated = false;
    bool next_allocated = false;
    size_t size_block = (*ptr).header.block_size;
    size_block = size_block >> 2;
    size_block = size_block << 2;
    sf_block *head_p;
    sf_header *foot_p;
    sf_prologue *prologue_addr = sf_mem_start() + sizeof(sf_prologue);
    sf_epilogue *epilogue_addr = sf_mem_end() - sizeof(sf_epilogue);

    if((void *) ptr == (void *) prologue_addr) {
        prev_allocated = true;
    }

    if((*ptr).header.block_size % 16 == 2 || (*ptr).header.block_size % 16 == 3) {
        prev_allocated = true;
    }

    ptr = (void *) ptr + size_block;

    if((void *) ptr == (void *) epilogue_addr) {
        next_allocated = true;
    }
    else {
        if((*ptr).header.block_size % 16 == 1 || (*ptr).header.block_size % 16 == 3) {
            next_allocated  = true;
        }
    }

    ptr = (void *) ptr - size_block;



    if(prev_allocated == true && next_allocated == true) {
        return;
    }

    if(prev_allocated == true && next_allocated == false) {
        head_p = ptr;
        foot_p = (sf_header *) ptr;
        foot_p = (void *) foot_p + size_block;
        sf_block *next = (sf_block *) foot_p;
        int blocking_size1 = (*next).header.block_size;
        blocking_size1 = blocking_size1 >> 2;
        blocking_size1 = blocking_size1 << 2;
        foot_p = (void *) foot_p + blocking_size1 - sizeof(sf_header);

        (*ptr).header.block_size += blocking_size1;
        (*foot_p).block_size = (*ptr).header.block_size;

    }

    if(prev_allocated == false && next_allocated == true) {
        head_p = ptr;
        foot_p = (sf_header *) ptr;
        foot_p--;
        int b_s = ((*foot_p).block_size);
        b_s = b_s >> 2;
        b_s = b_s << 2;
        head_p = (void *) head_p - b_s;
        foot_p++;
        foot_p = (void *) foot_p + size_block - sizeof(sf_header);
        (*head_p).header.block_size += size_block;
        (*foot_p).block_size = (*head_p).header.block_size;

    }

    if(prev_allocated == true && next_allocated == true) {

    }
    (*sf_free_list_head.body.links.next).body.links.prev = head_p;
    (*head_p).body.links.next = (*sf_free_list_head.body.links.next).body.links.next;
    (*head_p).body.links.prev = &sf_free_list_head;
    sf_free_list_head.body.links.next = head_p;
    return;
}