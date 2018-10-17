#include <stdio.h>
#include <stdlib.h>

#define OP_INSERT_CHAR 		'I' //Insert
#define OP_PRESSED_ENTER 	'N' //Newline
#define OP_PRESSED_DELETE 	'D' //Delete 
#define OP_PRESSED_LEFT 	'h' //Left arrow equivalent in vi editor
#define OP_PRESSED_DOWN 	'j' //Down arrow equivalent in vi editor
#define OP_PRESSED_UP 		'k' //Up arrow equivalent in vi editor
#define OP_PRESSED_RIGHT 	'l' //Right arrow equivalent in vi editor
#define OP_PRESSED_HOME 	'H' //Home
#define OP_PRESSED_END 		'E' //End
#define OP_PRINT_LINE 		'P' //Print the current line
#define OP_PRINT_TEXT 		'T' //print the whole Text
#define OP_QUIT 			'Q' //Quit the editor

//Forward declaration; see the definition of struct for more details.
struct text_node;

//A node in 
struct line_node {
	char ch;
	struct line_node *next_char;
	struct line_node *prev_char;
	struct text_node *line_header;
};

struct text_node {
	struct line_node *line_begin;
	struct text_node *next_line;
	struct text_node *prev_line;
};

//Creates the empty text. 
//Empty text has one empty line, which has a newline '\n' as end of line.
struct text_node *create_text(struct line_node **pcursor);

//Free the whole text
void free_text(struct text_node *text);

//Insert a char just before the cursor. 
//Cursor always points to a line_node. 
//A line has a newline char as the end of line.
struct line_node *op_insert_char(struct line_node *cursor, char ch);

//“Enter” (i.e. create) a new line and the cursor moves to the new line. 
//If the cursor was at the end of line, a new empty line is created, otherwise 
//the current line is split into a new line starting from the character at the cursor.
struct line_node *op_pressed_enter(struct line_node *cursor);

//“Delete” a character at the cursor, or the end of line if the cursor is 
//at the end of a line. Deleting an end of line is essentially concatenating 
//the next line with the current line. 
//No operation if the cursor is at the end of line of the last line.
struct line_node *op_pressed_delete(struct line_node *cursor);

//Move the cursor “Left” by a character (or to the end of line of 
//above line if the cursor is at the beginning of a line). 
//No operation if the cursor is the beginning of the first line.
struct line_node *op_pressed_left(struct line_node *cursor);

//Move the cursor “Down” to the below line (beginning of the below line). 
//No operation if the cursor is already at the last line.
struct line_node *op_pressed_down(struct line_node *cursor);

//Move the cursor “Up” to the above line (beginning of the above line). 
//No operation if the cursor is already at the first line.
struct line_node *op_pressed_up(struct line_node *cursor);

//Move the cursor “Right” by a character (or to the beginning of below line 
//if the cursor is at the end of a line). 
//No operation if the cursor is the end of line of the last line.
struct line_node *op_pressed_right(struct line_node *cursor);

//Move the cursor to “Home”, that is, beginning of the current line. 
//No operation if the cursor is at the beginning of a line.
struct line_node *op_pressed_home(struct line_node *cursor);

//Move the cursor to “End”, that is, end of line the current line. 
//No operation if the cursor is at the end of a line.
struct line_node *op_pressed_end(struct line_node *cursor);

//“Print Line” prints the current line.
void op_print_line(struct line_node *cursor);

//“Print Text” prints the whole text.
void op_print_text(struct text_node *text);

int main() {
	struct line_node *cursor;
	struct text_node *text = create_text(&cursor);
	char op[4];

	do {
		scanf(" %[^\n]", op);
		switch(op[0]) {
	        case OP_INSERT_CHAR:
	            cursor = op_insert_char(cursor, op[2]);
	            break;
	        case OP_PRESSED_ENTER:
	            cursor = op_pressed_enter(cursor);
	            break;
	        case OP_PRESSED_DELETE:
	            cursor = op_pressed_delete(cursor);
	            break;
			case OP_PRESSED_LEFT:
	            cursor = op_pressed_left(cursor);
	            break;
	        case OP_PRESSED_DOWN:
	            cursor = op_pressed_down(cursor);
	            break;
	        case OP_PRESSED_UP:
	            cursor = op_pressed_up(cursor);
	            break;
	        case OP_PRESSED_RIGHT:
	            cursor = op_pressed_right(cursor);
	            break;
	        case OP_PRESSED_HOME:
	            cursor = op_pressed_home(cursor);
	            break;
	        case OP_PRESSED_END:
	            cursor = op_pressed_end(cursor);
	            break;
	        case OP_PRINT_LINE:
	            op_print_line(cursor);
	            break;
	        case OP_PRINT_TEXT:
	            op_print_text(text);
	            break;	 
	    }
	} while(op[0] != OP_QUIT);
	
	free_text(text);
	return 0;
}

struct text_node *create_text(struct line_node **pcursor)
{	
	struct text_node *temp=(struct text_node*)calloc(sizeof(struct text_node),1);
	temp->next_line=temp->prev_line=NULL;
	struct line_node *curr=(struct line_node*)calloc(sizeof(struct line_node),1);
	curr->next_char=NULL;
	curr->ch=10;
	curr->prev_char=NULL;
	curr->line_header=temp;
	temp->line_begin=curr;
	*pcursor=curr;
	return temp;
}

void free_text(struct text_node *text)
{
	struct text_node *p=text,*t;
	struct line_node *q,*te;
	while(p!=NULL)
	{
		q=p->line_begin;
		while(q!=NULL)
		{
			te=q;
			q=q->next_char;
			free(te);
		}
		t=p;
		p=p->next_line;
		free(t);
	}
}

struct line_node *op_insert_char(struct line_node *cursor, char ch)
{
	struct line_node *curr=(struct line_node*)calloc(sizeof(struct line_node),1);
	curr->next_char=cursor;
	curr->ch=ch;
	curr->prev_char=cursor->prev_char;
	curr->line_header=cursor->line_header;
	if(cursor->prev_char!=NULL)
	{
		cursor->prev_char->next_char=curr;
	}
	else 
	{
		cursor->line_header->line_begin=curr;
	}
	cursor->prev_char=curr;
	return curr->next_char;
}

struct line_node *op_pressed_enter(struct line_node *cursor)
{
	struct text_node *head=cursor->line_header;
	struct text_node *new=(struct text_node*)calloc(sizeof(struct text_node),1);
	struct line_node *temp=(struct line_node*)calloc(sizeof(struct line_node),1);
	temp->ch=10;
	temp->next_char=NULL;
	temp->prev_char=NULL;
	if(cursor->ch==10)
	{
		new->line_begin=temp;
		temp->line_header=new;
	}
	else
	{
		if(cursor->prev_char!=NULL)
		{
			cursor->prev_char->next_char=temp;
			temp->prev_char=cursor->prev_char;
		}
		else
		{
			head->line_begin=temp;
			temp->line_header=head;
		}
		new->line_begin=cursor;
		cursor->prev_char=NULL;
		cursor->line_header=new;
	}
	new->next_line=head->next_line;
	new->prev_line=head;
	if(head->next_line!=NULL)
	{
		head->next_line->prev_line=new;
	}
	head->next_line=new;
	return new->line_begin;
}

struct line_node *op_pressed_delete(struct line_node *cursor)
{
	struct text_node *head=cursor->line_header,*curr;
	struct line_node *temp,*p,*u;
	if(cursor->ch==10)
	{
		if(head->next_line==NULL)
		{
			return cursor;
		}
		else
		{
			p=head->next_line->line_begin;
			u=p;
			while(u!=NULL)
			{
				u->line_header=head;
				u=u->next_char;
			}
			curr=head->next_line;
			head->next_line=curr->next_line;
			if(curr->next_line!=NULL)
			{
				curr->next_line->prev_line=head;
			}
			curr->next_line=curr->prev_line=NULL;
			curr->line_begin=NULL;
			free(curr);
		}
	}
	else
	{
		p=cursor->next_char;
	}
	temp=cursor;
	if(temp->prev_char!=NULL)
		temp->prev_char->next_char=p;
	else
	{
		head->line_begin=p;
	}
	p->prev_char=cursor->prev_char;
	cursor=p;
	temp->prev_char=temp->next_char=NULL;
	temp->line_header=NULL;
	free(temp);
	return(cursor);
}

struct line_node *op_pressed_left(struct line_node *cursor)
{
	if(cursor->prev_char==NULL)
	{
		if(cursor->line_header->prev_line==NULL)
			return cursor;
		else
			return (op_pressed_end(cursor->line_header->prev_line->line_begin));
	}
	else
	{
		return(cursor->prev_char);
	}
}

struct line_node *op_pressed_down(struct line_node *cursor)
{
	if(cursor->line_header->next_line==NULL)
		return cursor;
	else
	{
		return(cursor->line_header->next_line->line_begin);
	}
}

struct line_node *op_pressed_up(struct line_node *cursor)
{
	if(cursor->line_header->prev_line==NULL)
		return cursor;
	else
	{
		return(cursor->line_header->prev_line->line_begin);
	}
}

struct line_node *op_pressed_right(struct line_node *cursor)
{
	if(cursor->next_char==NULL)
	{
		if(cursor->line_header->next_line==NULL)
			return cursor;
		else
			return (cursor->line_header->next_line->line_begin);
	}
	else
	{
		return(cursor->next_char);
	}
}

struct line_node *op_pressed_home(struct line_node *cursor)
{
	//printf("%c\n",cursor->line_header->line_begin->ch);
	return(cursor->line_header->line_begin);
}

struct line_node *op_pressed_end(struct line_node *cursor)
{
	while(cursor->ch!=10)
		cursor=cursor->next_char;
	//printf("%c\n",cursor->ch);
	return(cursor);
}

void op_print_line(struct line_node *cursor)
{
	struct text_node *p=cursor->line_header;
	struct line_node *q=p->line_begin;
	while(q!=NULL&&(q->ch>=32||q->ch==10))
	{
		printf("%c",q->ch);
		q=q->next_char;
	}
}

void op_print_text(struct text_node *text)
{
	struct text_node *p=text;
	while(p!=NULL)
	{
		op_print_line(p->line_begin);
		p=p->next_line;
	}
}

