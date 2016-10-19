#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//checking git

struct node {
	int id;
	struct node *next;
};

struct node *cell_to_net[10000];
struct node *net_to_cell[50000];
	
void insert_node(int type, int position, int id)
{
	//printf ("%d %d %d\n", type, position, id);
	struct node *link = (struct node*) malloc(sizeof(struct node));

	link->id = id;
	if (type == 0) {
		link->next = net_to_cell[position];
		net_to_cell[position] = link;
		//printf ("%d %d\n", link->id, position);
	}
	else if (type == 1) {
		link->next = cell_to_net[position];
		cell_to_net[position] = link;
		//printf ("%d %d\n", link->id, position);
	}
}

void print_list(int type, int position)
{
	struct node *ptr;
	if (type = 0)
		ptr = net_to_cell[position];
	else if (type == 1)
		ptr = cell_to_net[position];
	printf("\n");

	//start from the beginning
	while(ptr != NULL)
	{        
		printf("%d ",ptr->id);
		ptr = ptr->next;
	}
	printf(" ");
}
	
int main() {

	int position[168][480];
	char buff[256], cell_type[4], check[10];
	int cell_no, x_pos, y_pos, fixed, i, j, net_index, net_flag, net_no;
	
	int cell_coord[10000], net_list[50000];
	
	for (i = 0; i < 50000; i++) {
		if (i < 10000)
			cell_to_net[i] = NULL;
		net_to_cell[i] = NULL;
	}

	FILE *design = fopen("FPGA01.pl","r");
	while (!feof(design)) {
		fgets(buff, 30, (FILE*)design);
		for (i = 0; buff[i] != '\0'; i++) {
			if (buff[i] == '_'){
				cell_no = 0;
				while (buff[++i] != ' ') {
					cell_no *= 10;
					cell_no += buff[i] - '0';
				}
				j = 0;
				while (buff[++i] != ' ') {
					cell_type[j++] = buff[i];
				}
				cell_type[j] = '\0';
				x_pos = 0;
				while (buff[++i] != ' ') {
					x_pos *= 10;
					x_pos += buff[i] - '0';
				}
				y_pos = 0;
				while (buff[++i] != ' ') {
					y_pos *= 10;
					y_pos += buff[i] - '0';
				}
				fixed = (buff[++i] == 'F') ? 1:0;
			}
		}
		cell_coord[cell_no] = x_pos*1000 + y_pos;
		//printf("lines: %s %d %s %d %d %d\n",buff,cell_no,cell_type,x_pos,y_pos,fixed);
	}
	
	FILE *nets = fopen("FPGA01_new.nets","r");
	net_index = 0;
	while(!feof(nets)) {
		fgets(buff, 255, (FILE*)nets);
		net_flag = 0;
		j = 0;
		for (i = 0; buff[i] != '\0'; ++i) {
			check[j++] = buff[i];
			check[j] = '\0';

			if(strcmp(check,"net_") == 0) {
				net_no = 0;
				while (buff[++i] != ' ') {
					net_no *= 10;
					net_no += buff[i] - '0';
				}
				j = 0;
				net_flag = 1;
				net_list[net_index++] = net_no;
			}
			//printf("%s\n",check);
			else if((strcmp(check,"cell_") == 0) && (net_flag == 1)) {
				cell_no = 0;
				while ((buff[++i] != ' ') && (buff[i] != '\n') && (buff[i] != '\0')) {
					cell_no *= 10;
					cell_no += buff[i] - '0';
				}
				j = 0;
				//printf("lines: %s %d %d\n",buff,net_no,cell_no);
				insert_node(0, net_index-1, cell_no);	//insert in net_to_cell
				insert_node(1, cell_no, net_no);	//insert in cell_to_net
			}
		}
	}
	
	for (i = 0; i < 10000; i++) {
		if (i < 5)
			print_list(0, i);	//print net_to_cell
		//if (i < 10)
			//print_list(1, i);	//print cell_to_net
	}

	fclose(design);
	fclose(nets);
	return 0;
}
