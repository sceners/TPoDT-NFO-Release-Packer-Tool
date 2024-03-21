#include "_global.h"

char* RemoveCrap(char* string)
{
    int len=strlen(string);
    for(int i=0; i<len; i++)
    {
        if(string[i]==' ')
            string[i]='.';
    }
    char* newstring=(char*)malloc(len);
    newstring[0]=0;
    for(int i=0; i<len; i++)
    {
        while(ispunct(string[i]) and ispunct(string[i+1]))
            i++;
        sprintf(newstring, "%s%c", newstring, string[i]);
    }
    strcpy(string, newstring);
    len=strlen(string);
    if(ispunct(string[len-1]))
        string[len-1]=0;
    free(newstring);
    return string;
}

void MatchLength(char* text, int length)
{
    int len=strlen(text);
    int total_needed=length-len;
    if(total_needed<0)
    {
        text[length]=0;
        return;
    }
    for(int i=0; i<total_needed; i++)
        strcat(text, " ");
}

int countSpaces(char* text)
{
    int len=strlen(text);
    int total_space=0;
    for(int i=0; i<len; i++)
        if(text[i]==' ')
            total_space++;
    return total_space;
}

bool IsBadCharacter(int ch)
{
    if(ch<0x21 or ch>0x7E)
        return true;
    return false;
}

int FindNextWordStart(char* text)
{
    int i=0;
    if(!strlen(text))
        return 0;
    while(IsBadCharacter(text[i]))
        i++;
    return i-1;
}

void JustifyLine(char* text)
{
    char text_new[100]=""; //Temporary buffer
    char default_space_buffer[100]=""; //Buffer for the 'default' spaces per word
    int len_text=strlen(text); //Length of the text
    int total_spaces=countSpaces(text); //Number of spaces in current line
    if(len_text==line_size or !total_spaces) //Full line or no spaces
        return;

    int spaces_needed=line_size-strlen(text); //Total amount of needed spaces
    int spaces_add_per_word=spaces_needed/total_spaces; //Spaces per word
    int remaining_spaces=spaces_needed%total_spaces; //Remaining spaces

    if(spaces_add_per_word) //Sometimes we only need to add a few spaces
    {
        memset(default_space_buffer, 0x20, spaces_add_per_word); //Make the default space count
        for(int i=0; i<len_text; i++) //Do the 'default' spaces
        {
            if(text[i]==' ' and text[i+1]!=' ') //Actually a bit overkill
                strcat(text_new, default_space_buffer); //Prepend space(s)
            sprintf(text_new, "%s%c", text_new, text[i]); //Add character to buffer
        }
    }
    else
        strcpy(text_new, text); //We can directly copy the buffer in text_new

    if(remaining_spaces) //We need to do something more...
    {
        int len_text_new=strlen(text_new);
        memset(text, 0, len_text_new+1);
        for(int i=0; i<len_text_new; i++)
        {
            if(remaining_spaces) //We must have some remaining spaces to do stuff
            {
                if(text_new[i]==' ' and text_new[i+1]!=' ') //Space + non-space
                {
                    strcat(text, " "); //Prepend a space
                    remaining_spaces--; //We added one space
                }
            }
            sprintf(text, "%s%c", text, text_new[i]);
        }
    }
    else //No remaining spaces, just copy the new buffer
    {
        memset(text, 0, strlen(text_new)+1);
        strcpy(text, text_new);
    }
}

void FixHighAsciiChars(char* text)
{
    int len=strlen(text);
    char* text_fixed=(char*)malloc(max_len);
    memset(text_fixed, 0, max_len);
    for(int i=0; i<len; i++)
    {
        int curchar=text[i];
        if(curchar>0x19 and curchar<0x7F)
            sprintf(text_fixed, "%s%c", text_fixed, curchar);
    }
    memset(text, 0, strlen(text_fixed)+1);
    strcpy(text, text_fixed);
    free(text_fixed);
}

void FixNewLineChars(char* text)
{
    int len=strlen(text);
    char* text_fixed=(char*)malloc(max_len);
    memset(text_fixed, 0, max_len);
    for(int i=0; i<len; i++)
    {
        int curchar=text[i];
        if(curchar=='\r' or curchar=='\n')
            sprintf(text_fixed, "%s ", text_fixed);
        else
            sprintf(text_fixed, "%s%c", text_fixed, curchar);
    }
    memset(text, 0, strlen(text_fixed)+1);
    strcpy(text, text_fixed);
    free(text_fixed);
}

void RemovePrepAppNewlines(char* text)
{
    char* text_fixed=(char*)malloc(max_len);
    memset(text_fixed, 0, max_len);

    int append=strlen(text)-1;
    int prepend=0;
    while((text[prepend]=='\r' or text[prepend]=='\n') and prepend<append)
        prepend++;
    while((text[append]=='\r' or text[append]=='\n') and append)
    {
        text[append]=0;
        append--;
    }
    strcpy(text_fixed, text+prepend);
    memset(text, 0, strlen(text_fixed)+1);
    strcpy(text, text_fixed);
    free(text_fixed);
}

void FixLongWords(char* text)
{
    int len=strlen(text);
    char* text_fixed=(char*)malloc(max_len);
    memset(text_fixed, 0, max_len);
    for(int i=0, char_count=0; i<len; i++)
    {
        char curchar=text[i];
        if(IsBadCharacter(curchar))
            char_count=0; //New word
        else
            char_count++;
        if(char_count==70) //Maximum word length+1 (current char is counted)
        {
            sprintf(text_fixed, "%s %c", text_fixed, curchar);
            char_count=1; //We already did write one char
        }
        else
            sprintf(text_fixed, "%s%c", text_fixed, curchar);
    }
    memset(text, 0, strlen(text_fixed)+1);
    strcpy(text, text_fixed);
    free(text_fixed);
}

bool AddWordToLine(const char* word, char* buffer)
{
    int len_word=strlen(word);
    int len_currentline=strlen(buffer);

    if((len_currentline+len_word)>=line_size and len_currentline)
        return false;

    if(!len_currentline)
        sprintf(buffer, "%s", word);
    else
        sprintf(buffer, "%s %s", buffer, word);
    return true;
}

void FormatTextBlock(char* text)
{
    char* fixed_text=(char*)malloc(max_len);
    memset(fixed_text, 0, max_len);
    char current_word[100]="";
    char current_line[100]="";
    int len_raw=strlen(text);
    int good_start=FindNextWordStart(text)+1;
    for(int i=good_start; i<len_raw; i++) //We loop through the long raw string
    {
        if(IsBadCharacter(text[i])) //Bad characters are seen as word seperators
        {
            i+=FindNextWordStart(text+i); //Find the next word (ignore double spaces for example)
            bool word_added=AddWordToLine(current_word, current_line); //Add the current word to the current line
            if(!word_added) //If the line was full
            {
                JustifyLine(current_line); //Match 69 chars
                sprintf(fixed_text, "%s%s\r\n", fixed_text, current_line);
                memset(current_line, 0, 100); //Clear the line
                AddWordToLine(current_word, current_line); //Add the un-added word to a new line
            }
            memset(current_word, 0, 100); //Clean the old word
        }
        else
            sprintf(current_word, "%s%c", current_word, text[i]); //Add a character to the current word
    }
    int len_current_line=strlen(current_line); //Calculate the length of the current line
    int len_current_word=strlen(current_word); //Calculate the length of the word (not yet added to the line)
    if(len_current_line+len_current_word+1>line_size and len_current_word) //Add a newline when both the line and the current word + one space exceed 69 chars
    {
        JustifyLine(current_line);
        strcat(current_line, "\r\n");
    }
    else if(len_current_line and len_current_word) //Ensure we have something on our line (for example when we enter only one word)
        current_line[len_current_line]=' '; //Add a space
    if(!fixed_text[0]) //Do not prepend useless newlines
        sprintf(fixed_text, "%s%s", current_line, current_word); //Combine the lines
    else
        sprintf(fixed_text, "%s%s%s", fixed_text, current_line, current_word); //Combine the lines
    memset(text, 0, strlen(fixed_text)+1);
    strcpy(text, fixed_text);
    free(fixed_text);
}

int FindNextBlock(char* text, char* seperator)
{
    int len=strlen(text);
    if(!len)
        return 0;
    int seperator_len=strlen(seperator);
    int counter=0;
    while(1)
    {
        if(!strncmp(text+counter, seperator, seperator_len))
            return counter;
        counter++;
        if(counter>=len)
            return counter;
    }
}

void AddNfoCharacteristics(char* text)
{
    int len=strlen(text);
    char* text_fixed=(char*)malloc(max_len);
    memset(text_fixed, 0, max_len);
    char current_line[100]="";

    for(int i=0; i<len; i++)
    {
        if(text[i]=='\r' and text[i+1]=='\n')
        {
            i++;
            sprintf(text_fixed, "%s \xBA    %s    \xBA\r\n", text_fixed, current_line);
            memset(current_line, 0, 100);
        }
        else
            sprintf(current_line, "%s%c", current_line, text[i]);
    }
    if(current_line[0]) //Final line
    {
        int spaces_need=line_size-strlen(current_line);
        char temp[100]="";
        memset(temp, 0x20, spaces_need);
        sprintf(text_fixed, "%s \xBA    %s%s    \xBA\r\n", text_fixed, current_line, temp);
        memset(current_line, 0, 100);
    }
    else //Remove terminating \r\n
    {
        len=strlen(text_fixed);
        text_fixed[len-2]=0;
    }
    memset(text, 0, strlen(text_fixed)+1);
    strcpy(text, text_fixed);
    free(text_fixed);
}

void FormatText(char* text, bool single_line)
{
    RemovePrepAppNewlines(text);
    char seperator[10]="\r\n\r\n"; //We define blocks of text with a seperator...
    if(single_line) //Single line seperators are different
        seperator[2]=0;
    char* current_block=(char*)malloc(max_len);
    memset(current_block, 0, max_len);
    char* text_format=(char*)malloc(max_len);
    memset(text_format, 0, max_len);
    char temp[100]="";
    memset(temp, 0x20, line_size);
    int seperator_len=strlen(seperator); //Calculate the seperator length for later use.
    int previous_block=0;
    int len_block=0;
    while(1)
    {
        len_block=FindNextBlock(text+previous_block, seperator); //Find length till next seperator
        if(!len_block) //No length is no new block
            break;
        strncpy(current_block, text+previous_block, len_block); //Copy new current_block
        FixNewLineChars(current_block);
        FixHighAsciiChars(current_block);
        FixLongWords(current_block);
        FormatTextBlock(current_block);
        AddNfoCharacteristics(current_block);
        if(seperator_len==4)
            sprintf(text_format, "%s%s \xBA    %s    \xBA\r\n", text_format, current_block, temp);
        else
            sprintf(text_format, "%s%s", text_format, current_block);
        memset(current_block, 0, max_len);

        previous_block+=len_block+seperator_len; //Update the previous block
        previous_block+=FindNextWordStart(text+previous_block)+1; //Look for a correct block start...
    }
    int len=strlen(text_format);
    if(seperator_len==4)
        text_format[len-4-11-line_size]=0;
    else
        text_format[len-2]=0;
    memset(text, 0, strlen(text_format)+1);
    strcpy(text, text_format);
    free(current_block);
    free(text_format);
}
