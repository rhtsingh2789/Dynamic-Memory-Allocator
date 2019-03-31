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



    unsigned int new_size = size;

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
        unsigned int index = ((new_size-32)/16);
        if(sf_quick_lists[index].first != NULL) {
            //sf_block head;
            //head.block_size = new_size;
            (*sf_quick_lists[index].first).header.requested_size = size;
            //head = *(sf_quick_lists[index].first);
            sf_block *p = sf_quick_lists[index].first;
            sf_quick_lists[index].length--;
            sf_quick_lists[index].first = (*sf_quick_lists[index].first).body.links.next;
            return p + 8;
        }
    }
        if(sf_free_list_head.body.links.next != &sf_free_list_head) {
            sf_block *curr_block = sf_free_list_head.body.links.next;
            sf_block block_to_return;
            sf_block leftover;
            while(1==1){
                curr_block = sf_free_list_head.body.links.next;
                while(curr_block != &sf_free_list_head) {

                    unsigned int blocky_sizee = (*curr_block).header.block_size;
                    blocky_sizee = blocky_sizee >> 2;
                    blocky_sizee = blocky_sizee << 2;

                    if(blocky_sizee < new_size) {
                        break;
                    }
                    if(blocky_sizee >= new_size) {
                        block_to_return = *curr_block;

                        if(blocky_sizee - new_size >= 32) {

                            block_to_return = *curr_block;
                            (*block_to_return.body.links.prev).body.links.next = block_to_return.body.links.next;
                            (*block_to_return.body.links.next).body.links.prev = block_to_return.body.links.prev;
                            block_to_return.header.requested_size = size;
                            block_to_return.header.block_size = new_size + ((*curr_block).header.block_size % 16) + 1;

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

                            curr_block = (void *) curr_block + sizeof(sf_header);
                            return curr_block + 8;
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
                if((*(sf_header *) grown).block_size % 16 == 3) {
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
                grown += (new_page.header.block_size-8);
                if(prev_alloc == true) {
                    grown -= 2;
                }
                sf_header new_page_footer;
                new_page_footer.block_size = new_page.header.block_size;
                new_page_footer.requested_size = 0;
                *((sf_header *) grown) = new_page_footer;

                grown = sf_mem_end();
                grown = ((sf_epilogue  *) grown - 1);
                sf_epilogue epi2;
                epi2.header.block_size = 1;
                epi2.header.requested_size = 0;
                *((sf_epilogue *) grown) = epi2;
                coalesce(coalate_block);

            }

        }

    return NULL;
}



void sf_free(void *pp) {
    pp -= sizeof(sf_header);

    if(pp == NULL) {
        //debug("NULL");
        abort();
    }
    if(pp < sf_mem_start() || pp > sf_mem_end()) { // ERROR CASE CHECKING
        //debug("mem start or end");
        abort();
    }

    sf_block *block_free = (sf_block *) pp;
    sf_header *onemore = (sf_header *) block_free - 1;


    if((*onemore).block_size % 16 == 1 && (*onemore).block_size % 16 == 3) { // ERROR CASE CHECKING
        //debug("blocksize");
        abort();
    }

    block_free = (sf_block *) pp;

    unsigned int size_of_block = (*block_free).header.block_size;
    size_of_block = size_of_block >> 2;
    size_of_block = size_of_block << 2;

    if(size_of_block % 16 != 0 || size_of_block < 32) {
        abort();
    }

    if((*block_free).header.requested_size + sizeof(sf_header) > size_of_block) {
        abort();
    }


    if((*block_free).header.block_size % 16 == 1) {
        sf_header* prev_check = (sf_header *) (pp);
        if((sf_mem_start() + sizeof(sf_prologue)) != pp){
            prev_check--;
        unsigned int prev_b = (*prev_check).block_size;
        prev_b = prev_b >> 2;
        prev_b = prev_b << 2;
        if((*prev_check).block_size % 16 == 1 || (*prev_check).block_size % 16 == 3) {
            abort();
        }
        prev_check++;
        prev_check = (void *) prev_check - prev_b;
        sf_block* bb = (sf_block *) prev_check;
        if((*bb).header.block_size % 16 == 1 || (*bb).header.block_size % 16 == 3) {
            abort();
        }
        }
    }
    //block_free = (sf_block *) pp;

    if(((size_of_block - 32) / 16) < 10) {
        unsigned int index = ((size_of_block - 32) / 16);
        if(sf_quick_lists[index].length < 5) {
        (*block_free).body.links.next = sf_quick_lists[index].first;
        (*block_free).header.requested_size = 0;
        sf_quick_lists[index].first = block_free;
        sf_quick_lists[index].length++;
        }
        else {
            while(sf_quick_lists[index].length > 0) {
                sf_block* next = (*sf_quick_lists[index].first).body.links.next;
                coalesce(sf_quick_lists[index].first);
                sf_quick_lists[index].length--;
                sf_quick_lists[index].first = next;
            }
            (*block_free).body.links.next = sf_quick_lists[index].first;
            (*block_free).header.requested_size = 0;
            sf_quick_lists[index].first = block_free;
            sf_quick_lists[index].length++;
        }
        //sf_show_heap();
    }
    else{
        coalesce((sf_block *)pp);
    }
    return;
}

void *sf_realloc(void *pp, size_t rsize) {
    pp -= sizeof(sf_header);

    if(pp == NULL) {
        //debug("NULL");
        abort();
    }
    if(pp < sf_mem_start() || pp > sf_mem_end()) { // ERROR CASE CHECKING
        //debug("mem start or end");
        abort();
    }

    sf_block *block_free = (sf_block *) pp;
    sf_header *onemore = (sf_header *) block_free - 1;


    if((*onemore).block_size % 16 == 1 && (*onemore).block_size % 16 == 3) { // ERROR CASE CHECKING
        //debug("blocksize");
        abort();
    }

    block_free = (sf_block *) pp;

    unsigned int size_of_block = (*block_free).header.block_size;
    size_of_block = size_of_block >> 2;
    size_of_block = size_of_block << 2;

    if(size_of_block % 16 != 0 || size_of_block < 32) {
        abort();
    }

    if((*block_free).header.requested_size + sizeof(sf_header) > size_of_block) {
        abort();
    }


    if((*block_free).header.block_size % 16 == 1) {
        sf_header* prev_check = (sf_header *) (pp);
        if((sf_mem_start() + sizeof(sf_prologue)) != pp){
            prev_check--;
        unsigned int prev_b = (*prev_check).block_size;
        prev_b = prev_b >> 2;
        prev_b = prev_b << 2;
        if((*prev_check).block_size % 16 == 1 || (*prev_check).block_size % 16 == 3) {
            abort();
        }
        prev_check++;
        prev_check = (void *) prev_check - prev_b;
        sf_block* bb = (sf_block *) prev_check;
        if((*bb).header.block_size % 16 == 1 || (*bb).header.block_size % 16 == 3) {
            abort();
        }
        }
    }

    /*
    block_free = pointer to block to be resized after subtracting 8
    size_block blocksize after >> <<
    */
    if((*block_free).header.requested_size < rsize) {
        void* new_pointer = sf_malloc(rsize);
        if(new_pointer == NULL) {
            return NULL;
        }
        memcpy(pp+8, new_pointer, (*block_free).header.requested_size);
        sf_free(pp+8);
        return new_pointer;
    }
    else{
        if(((*block_free).header.requested_size - rsize) < 32) {
        (*block_free).header.requested_size = rsize;
        return pp+8;
        }
    }
    return NULL;
}


void coalesce(sf_block *ptr) {
    (*ptr).header.requested_size = 0;
    bool prev_allocated = false;
    bool next_allocated = false;
    bool current_alloc = false;
    size_t size_block = (*ptr).header.block_size;
    if(size_block % 16 == 1 || size_block % 16 == 3) {
        current_alloc = true;
    }
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
        //return;
        head_p = ptr;
        void *run = ptr;
        run += size_block - sizeof(sf_header);
        foot_p = (sf_header *) run;
        (*ptr).header.block_size = (*ptr).header.block_size >> 1;
        (*ptr).header.block_size = (*ptr).header.block_size << 1;
        if(current_alloc) {
            sf_header footing;
            footing.block_size = (*ptr).header.block_size;
            footing.requested_size = 0;
            (*foot_p) = footing;
        }
        else{
            (*foot_p).block_size = (*head_p).header.block_size;
        }
        run += sizeof(sf_header);
        if(run == (void *) epilogue_addr) {
            (*(sf_header *) run).block_size -= 2;
        }
        else {
            (*(sf_block *) run).header.block_size -= 2;
        }
    }

    if(prev_allocated == true && next_allocated == false) {
        head_p = ptr;
        foot_p = (sf_header *) ptr;
        foot_p = (void *) foot_p + size_block;

        sf_block *next = (sf_block *) foot_p;
        (*(*next).body.links.next).body.links.prev = (*next).body.links.prev;
        (*(*next).body.links.prev).body.links.next = (*next).body.links.next;

        unsigned int blocking_size1 = (*next).header.block_size;
        blocking_size1 = blocking_size1 >> 2;
        blocking_size1 = blocking_size1 << 2;
        foot_p = (void *) foot_p + blocking_size1 - sizeof(sf_header);

        (*ptr).header.block_size += blocking_size1 - 1;
        (*foot_p).block_size = (*ptr).header.block_size;

    }

    if(prev_allocated == false && next_allocated == true) {
        //debug("Extending page");
        head_p = ptr;
        foot_p = (sf_header *) ptr;
        foot_p--;
        unsigned int b_s = ((*foot_p).block_size);
        b_s = b_s >> 2;
        b_s = b_s << 2;
        head_p = (void *) head_p - b_s;
        foot_p++;
        foot_p = (void *) foot_p + size_block - sizeof(sf_header);

        (*head_p).header.block_size += size_block;

        //(*foot_p).block_size = (*head_p).header.block_size;

        void *run2 = (void *) foot_p + sizeof(sf_header);
        if(run2 == (void *) epilogue_addr) {
            if(current_alloc == true) {
                (*(sf_header *) run2).block_size -= 2;
            }
        }
        else {
            (*(sf_block *) run2).header.block_size -= 2;
        }

        if(current_alloc) {
            sf_header footing;
            footing.block_size = (*head_p).header.block_size;
            footing.requested_size = 0;
            (*foot_p) = footing;
        }
        else{
            (*foot_p).block_size = (*head_p).header.block_size;
        }

        (*(*head_p).body.links.next).body.links.prev = (*head_p).body.links.prev;
        (*(*head_p).body.links.prev).body.links.next = (*head_p).body.links.next;
    }

    if(prev_allocated == false && next_allocated == false) {
        void *runner = ptr;

        runner = runner - sizeof(sf_header);

        unsigned int b1 = (*(sf_header *) runner).block_size;
        b1 = b1 >> 2;
        b1 = b1 << 2;

        runner = runner + sizeof(sf_header);

        runner -= b1;

        head_p = (sf_block *) runner;

        runner = (void *) ptr;


        unsigned int b2 = (*(sf_block *) runner).header.block_size;
        b2 = b2 >> 2;
        b2 = b2 << 2;

        runner = runner + b2;

        sf_block *next = (sf_block *) runner;
        (*(*next).body.links.next).body.links.prev = (*next).body.links.prev;
        (*(*next).body.links.prev).body.links.next = (*next).body.links.next;

        unsigned int b3 = (*(sf_block *) runner).header.block_size;

        b3 = b3 >> 2;
        b3 = b3 << 2;

        runner = runner + b3 - sizeof(sf_header);

        foot_p = (sf_header *) runner;

        (*head_p).header.block_size += b2 + b3;
        (*foot_p).block_size = (*head_p).header.block_size;
        (*(*head_p).body.links.next).body.links.prev = (*head_p).body.links.prev;
        (*(*head_p).body.links.prev).body.links.next = (*head_p).body.links.next;
    }

    (*sf_free_list_head.body.links.next).body.links.prev = head_p;
    (*head_p).body.links.next = (sf_free_list_head.body.links.next);
    (*head_p).body.links.prev = &sf_free_list_head;
    sf_free_list_head.body.links.next = head_p;
    return;
}