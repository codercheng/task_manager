#ifndef CONN_INFO_MANAGER_H
#define CONN_INFO_MANAGER_H

#include <stdio.h>
#include <unistd.h>
#define MAXBUFSIZE 4080
#define NO_SOCK_FD -1
#define NO_FILE_FD -2

typedef struct conn_info
{
	int sockfd;
	int ffd;
	unsigned int write_pos;
	unsigned int read_pos;
	unsigned int total_len;
	char buf[MAXBUFSIZE];
	struct conn_info *next;
}conn_info;

class conn_info_manager
{
public:
	conn_info_manager()
	{
		freelist = NULL;
		freelist_size = 0;
	}
	~conn_info_manager()
	{
		struct conn_info *temp;
		while(freelist != NULL)
		{
			temp = freelist;
			freelist = freelist->next;
			delete temp;
		}
		freelist = NULL;
		freelist_size = 0;
	}

	void init(int num_of_conn)
	{
		for(int i=0; i<num_of_conn; i++)
		{
			add_new_node();
		}
	}
	// return a free conn_info node
	conn_info *get_free_node()
	{
		if(freelist_size <= 0)
		{
			add_new_node();
		}
		conn_info *temp = freelist;
		freelist = freelist->next;
		freelist_size--;
		return temp;
	}

	//insert the node to the freelist
	void give_back_node(conn_info *node)
	{
		if(node == NULL)
			return;
		if(node->sockfd != NO_SOCK_FD)
		{
			close(node->sockfd);
		}
		if(node->ffd != NO_FILE_FD)
		{
			//printf("%d reach end\n",node->ffd);
			close(node->ffd);
		}
		node->sockfd = NO_SOCK_FD;
		node->ffd = NO_FILE_FD;
		node->write_pos = 0;
		node->total_len = 0;
		node->read_pos = 0;
		node->next = NULL;
		if(freelist == NULL)
			freelist = node;
		else
		{
			conn_info *temp = freelist;
			freelist = node;
			node->next = temp;
		}
		freelist_size++;
	}
	//just for debug
	void getInfo()
	{
		printf("size:%d\n",freelist_size);
	}
private:
	conn_info *freelist;
	int freelist_size;

	void add_new_node()
	{
		conn_info *new_info_node = new conn_info;
		new_info_node->sockfd = NO_SOCK_FD;
		new_info_node->ffd = NO_FILE_FD;
		new_info_node->write_pos = 0;
		new_info_node->read_pos = 0;
		new_info_node->total_len = 0;
		new_info_node->next = NULL;
		if(freelist == NULL)
			freelist = new_info_node;
		else
		{
			conn_info *temp = freelist;
			freelist = new_info_node;
			new_info_node->next = temp;
		}
		freelist_size++;
	}


};


#endif// CONN_INFO_MANAGER_H