/*
 * CS252: Systems Programming
 * Purdue University
 * Example that shows how to read one line with simple editing
 * using raw terminal.
 * You people disgust me. Use fucking tabs instead of your shitty double spaces. 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_LINE 2048

extern void tty_raw_mode(void);

// Buffer where line is stored
int charindex;
int line_length;
char line_buffer[MAX_BUFFER_LINE];

// Simple history array
// This history does not change. 
// Yours have to be updated.
int history_index = 0;
char ** history;
int history_length = 0;
int MAX_HIST_LENGTH = 25;

void read_line_print_usage()
{
	char * usage = "\n"
	" ctrl-?       Print usage\n"
	" Backspace    Deletes last character\n"
	" delete       Remove the character at the cursor\n"
	" up arrow     See last command in the history\n"
	" down arrow   see next command in the history\n"
	" left arrow   move cursor left\n"
	" right arrow  move cursor right\n"
	" home (ctrlA) move cursor to the first space\n"
	" end (ctrlE)  move cursor to the last space\n";

	write(1, usage, strlen(usage));
}

/* 
 * Input a line with some basic editing.
 */
char * read_line() {
	if (history == NULL){
		history = (char**)malloc(MAX_HIST_LENGTH*sizeof(char*));
	}
	// Set terminal in raw mode
	tty_raw_mode();
	charindex = 0;
	line_length = 0;

	// Read one line until enter is typed
	while (1) {
		// Read one character in raw mode.
		char ch;
		read(0, &ch, 1);

		if (ch>=32 && ch!=127) {
			// It is a printable character. 
			// Do echo
			write(1,&ch,1);

  	    		// If max number of character reached return.
			if (line_length==MAX_BUFFER_LINE-2){  break; } 
			
			if (charindex != line_length){
				char temp;
				line_length++;
				
				for (int i = line_length; i > charindex; i--){
					line_buffer[i] = line_buffer[i-1];
				}
				line_buffer[charindex] = ch;
				for (int j = charindex+1; j < line_length; j++){
					ch = line_buffer[j];
					write(1, &ch, 1);
				}
				ch =8;
				for(int k = charindex+1; k < line_length; k++){
					write(1, &ch,1);
				}
			}else{
				// add char to buffer.	
				line_buffer[line_length]=ch;
				line_length++;
			}
			charindex++;
		}
		else if (ch==10) {
			// <Enter> was typed. Return line
      			if (history_length == MAX_HIST_LENGTH){
				MAX_HIST_LENGTH*=2;
				history = (char**) realloc(history, MAX_HIST_LENGTH*sizeof(char*));
				if (history == NULL){ printf("broke something history\n"); exit(1); }
			}
			line_buffer[line_length]=0;
			if (line_buffer[0]){
				history[history_length] = strdup(line_buffer);
				history_length++;
			}
			// Print newline
			write(1,&ch,1);

			break;
		}
		else if (ch == 31) {
			// ctrl-?
			read_line_print_usage();
			line_buffer[0]=0;
			break;
		}
		else if (ch == 8 || ch == 127){
			// <backspace> was typed. Remove previous character read.
			// Go back one character
 
			ch = 8;
			write(1,&ch,1);

			// Write a space to erase the last character read
			ch = ' ';
			write(1,&ch,1);

			// Go back one character
			ch = 8;
			write(1,&ch,1);

			// Remove one character from buffer
			line_length--;
			charindex--;
			
		}
		else if (ch==1){ //home
			for (int i = charindex; i > 0; i--){
				ch=8;
				write(1,&ch,1);
				charindex--;
			}
		}
		else if(ch==5){ //end
			for (int i = charindex; i < line_length; i++){
				ch=line_buffer[i];
				write(1,&ch,1);
				charindex++;
			}
		}
		else if (ch==4) {
			if (line_length){
				for (int i = charindex; i < line_length-1; i++){
					line_buffer[i] = line_buffer[i+1];
					write(1,line_buffer[i],1);
				}
				line_buffer[line_length] = ' ';
				line_length--;
				
				for(int i = charindex; i < line_length; i++){
					ch = line_buffer[i];
					write(1, &ch, 1);
				}
				ch = ' ';
				write(1,&ch,1);
				ch = 8;
				write(1,&ch,1);
				for (int i = line_length; i > charindex; i--){
					write(1, &ch, 1);	
				}
				
			}
		}
		else if (ch==27) {
			// Escape sequence. Read two chars more
			// HINT: Use the program "keyboard-example" to
			// see the ascii code for the different chars typed.
			char ch1; 
			char ch2;
			read(0, &ch1, 1);
			read(0, &ch2, 1);
			if (ch1==91 && ch2==65 && history_length > 0) {
				// Up arrow. Print next line in history.
				// Erase old line
				// Print backspaces
				int i = 0;
				for (i = charindex-line_length; i < line_length; i++) {
					ch = 8;
					write(1,&ch,1);
				}
	
				// Print spaces on top
				for (i =0; i < line_length; i++) {
					ch = ' ';
					write(1,&ch,1);
				}
	
				// Print backspaces
				for (i =0; i < line_length; i++) {
					ch = 8;
					write(1,&ch,1);
				}	
		
				// Copy line from history
				strcpy(line_buffer, history[history_index]);
				line_length = strlen(line_buffer);
				history_index=(history_index+1)%history_length;
	
				// echo line
				write(1, line_buffer, line_length);
				charindex = line_length;
			}
			if (ch1==91 && ch2==66){ //down arrow
				int i;
				for (i = line_length-charindex; i < line_length; i++){
					ch = 8;
					write(1, &ch, 1);
				}
		
				// Print spaces on top
				for (i =0; i < line_length; i++) {
					ch = ' ';
					write(1,&ch,1);
				}
	
				// Print backspaces
				for (i =0; i < line_length; i++) {
					ch = 8;
					write(1,&ch,1);
				}	
				if (history_index > 0){
					// Copy line from history
					strcpy(line_buffer, history[history_index]);
					line_length = strlen(line_buffer);
					history_index=(history_index+1)%history_length;
					// echo line
					write(1, line_buffer, line_length);
					charindex = line_length;
				}
			}
			if (ch1==91 && ch2==68){ //left arrow
				if(charindex > 0){
					ch = 8;
					write(1, &ch, 1);
					charindex--;			
				}
			}
			
			if (ch1==91 && ch2==67){ //right arrow
				if (charindex < line_length){
					ch = line_buffer[charindex];
					write(1, &ch, 1);
					charindex++;
				}
			}
		}
	}
	// Add eol and null char at the end of string
	line_buffer[line_length]=10;
	line_length++;
	line_buffer[line_length]=0;
	return line_buffer;
}

