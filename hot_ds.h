/**
*
* The implementation of data structure which is used to keep track of hotness of the data based on which data movement is possible.
* @author: mahesh.gupta
* @version: 1.0
* @ref https://github.com/wuzhy/kernel/blob/af274bbeee8ab4a7fb59f05d5bfb5707a08bf200/Documentation/filesystems/hot_tracking.txt
**/
#include<stdio.h>
#include<linux/list.h>


#define counter unsigned int

/**
 * Not sure how to implement the time using unix std utility or using a simple no ??
 * for now we'll use the int value
**/
struct hot_time {
	unsigned int ms_time // time in millisecond
	unsigned int ns_time; // time in nanosecond
}

struct hot_time* new_hot_time(){
	hot_time* tt = (struct hot_time*) malloc(sizeof(struct hot_time));

	tt->ms_time = 0;
	tt->ns_time = 0;

	return tt;
}

/*
 * The main data structure which keeps track of hotness
**/
/**
TODO: *. Write are of two types i.e. append and modify mode. whether to check for both of them ??
TODO: *. how to calculate avg read time or avg write time ??
*/
struct hot_track{
	// the last time given Inode was read/written
	hot_time* last_read_time;
	hot_time* last_write_time;

	// the total number of time Inode has been read/written
	counter total_read;
	counter total_write;

	// the avg number of time Inode has been read/written
	unsigned float avg_read_time;
	unsigned float avg_write_time;
};

struct hot_track* new_hot_track() {
	struct hot_track* tracker = (struct hot_track*) malloc(sizeof(struct hot_track));

	reset_hot_track(tracker);

	return tracker;
}

void reset_hot_track(struct hot_track* tracker){
          
          tracker->last_read_time = new_hot_time();
          tracker->last_write_time = new_hot_time();
          
          tracker->total_read = 0;
          tracker->total_write = 0;
          
          tracker->avg_read_time = 0;
          tracker->avg_write_time = 0;
}

/**
* Each entry defines the entry in the list of hotness
* TODO: not sure whether to put file* pointer here :-/
*/
struct map_entry {
	struct hot_track* hotness;
	file* file;
};

/**
* HOT_INFO:  an object which keeps tracks of everything. the map size is fixed and is about 2^10-2^15
* Each entry into the list stays for atleast MIN_TIME (=2 initially) iterations of the list.
*
* For an entry its (MIN_TIME, temprature) together defines its hotness.
*	MIN_TIME > 0 :  it stays in the list
*	MIN_TIME = 0 : then remove it from the list
* When page is accessed reset the time to MIN_TIME value
*
* NOTE: there should be exactly 2 hot_info objects. one for SSD and another for HDD
* HDD moves the hot values & SSD moves the cold value
* TODO: change map, from list_head to a map
*/


struct hot_info {
	struct list_head* map;			// all elements by default are hot_track object (default value: file=0 & hotness=reset())
	int curr;				// ptr to the current element (search starts from next element)
	int map_size;				// = 0 for no element
};


void init_info(hot_info* info){
	info->map = new struct list_head[MAP_SIZE];
	info->curr = -1;
	info->map_size = 0;
} 

struct hot_info* new_hot_info(){
	struct hot_info* info = (struct hot_info*) malloc(sizeof(struct hot_info));
	init_info(info);
	return info;
}
