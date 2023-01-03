
#include "mem.h"
#include "stdlib.h"
#include "string.h"
#include <pthread.h>
#include <stdio.h>

static BYTE _ram[RAM_SIZE];

static struct
{
	uint32_t proc; // ID of process currently uses this page
	int index;	   // Index of the page in the list of pages allocated
				   // to the process.
	int next;	   // The next page in the list. -1 if it is the last
				   // page.
} _mem_stat[NUM_PAGES];

static pthread_mutex_t mem_lock;

void init_mem(void)
{
	memset(_mem_stat, 0, sizeof(*_mem_stat) * NUM_PAGES);
	memset(_ram, 0, sizeof(BYTE) * RAM_SIZE);
	pthread_mutex_init(&mem_lock, NULL);
}

/* get offset of the virtual address */
static addr_t get_offset(addr_t addr)
{
	return addr & ~((~0U) << OFFSET_LEN);
}

/* get the first layer index */
static addr_t get_first_lv(addr_t addr)
{
	return addr >> (OFFSET_LEN + PAGE_LEN);
}

/* get the second layer index */
static addr_t get_second_lv(addr_t addr)
{
	return (addr >> OFFSET_LEN) - (get_first_lv(addr) << PAGE_LEN);
}

/* Search for page table table from the a segment table */
static struct trans_table_t *get_trans_table(
	addr_t index, // Segment level index
	struct seg_table_t *seg_table)
{ // first level table

	/*
	 * TODO: Given the Segment index [index], you must go through each
	 * row of the segment table [seg_table] and check if the v_index
	 * field of the row is equal to the index
	 *
	 * */

	int i;
	for (i = 0; i < seg_table->size; i++)
	{
		// Enter your code here
		if (seg_table->table[i].v_index == index)
			return seg_table->table[i].next_lv;
	}
	return NULL;
}

/* Translate virtual address to physical address. If [virtual_addr] is valid,
 * return 1 and write its physical counterpart to [physical_addr].
 * Otherwise, return 0 */
static int translate(
	addr_t virtual_addr,   // Given virtual address
	addr_t *physical_addr, // Physical address to be returned
	struct pcb_t *proc)
{ // Process uses given virtual address

	/* Offset of the virtual address */
	addr_t offset = get_offset(virtual_addr);
	/* The first layer index */
	addr_t first_lv = get_first_lv(virtual_addr);
	/* The second layer index */
	addr_t second_lv = get_second_lv(virtual_addr);

	/* Search in the first level */
	struct trans_table_t *trans_table = NULL;
	trans_table = get_trans_table(first_lv, proc->seg_table);
	if (trans_table == NULL)
	{
		return 0;
	}

	int i;
	for (i = 0; i < trans_table->size; i++)
	{
		if (trans_table->table[i].v_index == second_lv)
		{
			/* TODO: Concatenate the offset of the virtual addess
			 * to [p_index] field of trans_table->table[i] to
			 * produce the correct physical address and save it to
			 * [*physical_addr]  */
			*physical_addr = (trans_table->table[i].p_index << OFFSET_LEN) | offset;
			return 1;
		}
	}
	return 0;
}

void remove_entries(addr_t vir_seg, struct seg_table_t *seg_table)
{
	if (seg_table == NULL)
		return;

	int i;

	for (i = 0; i < seg_table->size; ++i)
	{
		if (seg_table->table[i].v_index == vir_seg)
		{
			int idx = seg_table->size - 1;
			seg_table->table[i] = seg_table->table[idx];
			seg_table->table[idx].v_index = 0;
			free(seg_table->table[idx].next_lv);
			seg_table->size--;
			return;
		}
	}
}

void free_mem_bp(struct pcb_t *proc)
{
	while (proc->bp >= PAGE_SIZE)
	{
		addr_t last_addr = proc->bp - PAGE_SIZE;
		addr_t last_segment = get_first_lv(last_addr);
		addr_t last_page = get_second_lv(last_addr);
		struct trans_table_t *trans_table = get_trans_table(last_segment, proc->seg_table);
		if (trans_table == NULL)
			return;
		while (last_page >= 0)
		{
			int i;
			for (i = 0; i < trans_table->size; ++i)
			{
				if (trans_table->table[i].v_index == last_page)
				{
					proc->bp -= PAGE_SIZE;
					last_page--;
					break;
				}
			}
			if (i == trans_table->size)
				break;
		}
		if (last_page >= 0)
			break;
	}
}

addr_t alloc_mem(uint32_t size, struct pcb_t *proc)
{
	pthread_mutex_lock(&mem_lock);
	addr_t ret_mem = 0;

	if ((proc->seg_table != NULL) && (proc->seg_table->size) >= (1 << FIRST_LV_LEN))
	{
		proc->seg_table->size = 0;
	}
	/* TODO: Allocate [size] byte in the memory for the
	 * process [proc] and save the address of the first
	 * byte in the allocated memory region to [ret_mem].
	 * */

	uint32_t num_pages = (size + PAGE_SIZE - 1) / PAGE_SIZE; // Number of pages we will use
	int mem_avail = 0;										 // We could allocate new memory region or not?
	/* First check if the amount of free memory in
	 * virtual address space and physical address space is
	 * large enough to represent the amount of required
	 * memory. If so, set 1 to [mem_avail].
	 * */
	int pages_avail = 0;
	for (int i = 0; i < NUM_PAGES; ++i)
		pages_avail += (_mem_stat[i].proc == 0);
	mem_avail = (pages_avail >= num_pages) && (proc->bp + num_pages * PAGE_SIZE < RAM_SIZE) ? 1 : 0;

	if (mem_avail)
	{
		/* We could allocate new memory region to the process */
		ret_mem = proc->bp;
		addr_t vir_add = ret_mem;
		proc->bp += num_pages * PAGE_SIZE;

		int pre_page = -1;
		int index_cnt = 0;
		for (int i = 0; i < NUM_PAGES && num_pages > 0; ++i)
			if (_mem_stat[i].proc == 0)
			{
				_mem_stat[i].proc = proc->pid;
				_mem_stat[i].index = index_cnt++;
				--num_pages;
				if (pre_page != -1)
				{
					_mem_stat[pre_page].next = i;
				}
				pre_page = i;

				addr_t first_lv = get_first_lv(vir_add);
				addr_t second_lv = get_second_lv(vir_add);

				struct trans_table_t *trans_table = NULL;
				trans_table = get_trans_table(first_lv, proc->seg_table);
				if (trans_table == NULL)
				{
					if (proc->seg_table == NULL)
					{
						proc->seg_table = (struct seg_table_t *)malloc(sizeof(struct seg_table_t *));
						proc->seg_table->size = 0;
					}
					int idx = proc->seg_table->size;
					proc->seg_table->table[idx].v_index = first_lv;

					trans_table = proc->seg_table->table[idx].next_lv = (struct trans_table_t *)malloc(sizeof(struct trans_table_t));
					trans_table->size = 0;

					proc->seg_table->size++;
				}
				trans_table->size++;
				trans_table->table[trans_table->size - 1].v_index = second_lv;
				trans_table->table[trans_table->size - 1].p_index = i;
				vir_add += (PAGE_SIZE);
			}
		_mem_stat[pre_page].next = -1;
	}
	pthread_mutex_unlock(&mem_lock);

	return ret_mem;
}

int free_mem(addr_t address, struct pcb_t *proc)
{
	pthread_mutex_lock(&mem_lock);

	addr_t phy_add = 0;
	addr_t vir_add = address;

	if (!translate(address, &phy_add, proc))
		return 1;

	int num_pages = 0;
	addr_t index = (phy_add >> OFFSET_LEN);

	while (1 == 1)
	{
		++num_pages;
		_mem_stat[index].proc = 0; // clear physical page in memory

		// clear virtual page in process
		addr_t vir_seg = get_first_lv(vir_add);
		addr_t vir_page = get_second_lv(vir_add);
		struct trans_table_t *trans_table = get_trans_table(vir_seg, proc->seg_table);
		if (trans_table == NULL)
		{
			continue;
		}
		int j;
		for (j = 0; j < trans_table->size; ++j)
		{
			if (trans_table->table[j].v_index == vir_page)
			{
				int last = --trans_table->size;
				trans_table->table[j] = trans_table->table[last];
				break;
			}
		}
		if (trans_table->size == 0)
		{
			remove_entries(vir_seg, proc->seg_table);
		}

		if (_mem_stat[index].next == -1)
			break;

		// move to next page
		index = _mem_stat[index].next;
		vir_add += PAGE_SIZE;
	}
	pthread_mutex_unlock(&mem_lock);

	// update break pointer
	addr_t v_segment_page = address >> OFFSET_LEN;
	if (v_segment_page + num_pages * PAGE_SIZE == proc->bp)
	{
		free_mem_bp(proc);
	}
	return 0;
}

int read_mem(addr_t address, struct pcb_t *proc, BYTE *data)
{
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc))
	{
		*data = _ram[physical_addr];
		return 0;
	}
	else
	{
		return 1;
	}
}

int write_mem(addr_t address, struct pcb_t *proc, BYTE data)
{
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc))
	{
		_ram[physical_addr] = data;
		return 0;
	}
	else
	{
		return 1;
	}
}

void dump(void)
{
	int i;
	for (i = 0; i < NUM_PAGES; i++)
	{
		if (_mem_stat[i].proc != 0)
		{
			printf("%03d: ", i);
			printf("%05x-%05x - PID: %02d (idx %03d, nxt: %03d)\n",
				   i << OFFSET_LEN,
				   ((i + 1) << OFFSET_LEN) - 1,
				   _mem_stat[i].proc,
				   _mem_stat[i].index,
				   _mem_stat[i].next);
			int j;
			for (j = i << OFFSET_LEN;
				 j < ((i + 1) << OFFSET_LEN) - 1;
				 j++)
			{

				if (_ram[j] != 0)
				{
					printf("\t%05x: %02x\n", j, _ram[j]);
				}
			}
		}
	}
}


