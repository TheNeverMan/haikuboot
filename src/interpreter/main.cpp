#include <cstring>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <queue>
using namespace std;
//checks for doebug mode
bool is_debug = false;
// super debug mode (very verbose) flag
bool is_super_debug = false;
//prompt for ask mode flag
bool is_ask_prompt = false;
//continue on errors mode flag
bool continue_on_errors = false;
//hide errors mode flag
bool hide_errors = false;
//detailed error descriptioms mode flag
bool detailed_errors = false;
//table of pixel data
int size = 1000000;
unsigned char Table[1000000] {}; //lmao get ram lol also initialize table full of zeroes to prevent segfaults
//data queue
queue<unsigned char> Data_Queue;
//row length
int row_length = 0;
//current interpreted pixel
int cur_pix = 0;
enum Placement
{
  TopLeft,TopRight,DownLeft,DownRight,null
};
enum Com
{
  Print,Put,If,Inc,Dec,Remove,Ask,none,white,black
};
//commands
struct Command
{
  Com Command;
  Placement Place;
};
//returns first pixel of actual data
int Get_Row_Length()
{
  //minium row length 10 maximum 999
  int a = 0;
  int b = 0;
  int c = 0;
  string as(1,Table[3]);
  string bs(1,Table[4]);
  string cs(1,Table[5]);
  //cout << as << bs << cs << endl;
  try
  {
    a = stoi(as);
  }
  catch(...)
  {
    a = 0;
  }
  try
  {
    b = stoi(bs);
  }
  catch(...)
  {
    b = 0;
  }
  try
  {
    c = stoi(cs);
  }
  catch(...)
  {
    c = 0;
  }
  int out = 0;
  if(c == 0)
  {
    out = a * 10 + b;
  }
  else
  {
    out = a * 100 + b * 10 + c;
  }
  row_length = out;
  //cout << "Row: " << row_length << endl;
  int i = 0;
  while(true)
  {
    if(Table[i] == 255)
    {
      break;
    }
    i++;
  }
  return i; //first pixel with 255 data in red color byte
}
//adds start data to the queue (takes first byte of data as parameter)
void Add_Start_Data(int start)
{
  //cout << "Start: " << start << endl;
  if(is_debug)
  {
    cout << "Data Queue Preload enbled, first data byte position: " << start << endl;
  }
  int end = start + row_length * 3;
  //start = start + 2;
  if(Table[start + 1] == 255)
  {
    while(start != end)
    {
      //cout << "add" << endl;
      if(Table[start] != 255)
      {
        Data_Queue.push(Table[start]);
        if(is_debug)
        {
          cout << "Preloaded Data: " << Table[start] << " (dec) " << (int) Table[start] << endl;
        }
      }
      start++;
    }
    Data_Queue.pop();
    Data_Queue.pop();
  }

  /*while (!Data_Queue.empty())
  {
    cout << ' ' << Data_Queue.front();
    Data_Queue.pop();
  }*/
}
Command Get_Command()
{
  Command out;
  out.Place = null;
  int red = Table[cur_pix]; //placement color
  int green = Table[cur_pix + 1]; //placement color
  int blue = Table[cur_pix + 2]; // command color
  //placement colors can have value ff or 00, command 11,22,33,44,55,66,77
  //00 00 TopRight
  //00 ff TopLeft
  //ff 00 DownRight
  //ff ff DownLeft
  if(red == 0 && green == 0)
  {
    out.Place = TopRight;
  }
  if(red == 255 && green == 255)
  {
    out.Place = DownLeft;
  }
  if(red == 255 && green == 0)
  {
    out.Place = DownRight;
  }
  if(red == 0 && green == 255)
  {
    out.Place = TopLeft;
  }
  switch(blue)
  {
    case 17:
    out.Command = Print;
    break;
    case 34:
    out.Command = Ask;
    break;
    case 51:
    out.Command = Inc;
    break;
    case 68:
    out.Command = Dec;
    break;
    case 85:
    out.Command = If;
    break;
    case 102:
    out.Command = Remove;
    break;
    case 119:
    out.Command = Put;
    break;
    case 255:
    out.Command = white;
    break;
    default:
    out.Command = none;
    break;
  }
  if(red == 0 && green == 0 && blue == 0)
  {
    out.Command = black;
  }
//  cur_pix = cur_pix + 3;
  return out;
}
char Interpret_Command(Command com)
{
  char out = ' ';
  int second_pixel = cur_pix;
  if(com.Place == DownRight)
  {
    //cout << "DownRight" << endl;
    if(com.Command == Print)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + 2 * 3;
    }
    if(com.Command == Put)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + 1 * 3;
    }
    if(com.Command == Ask)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + row_length * 3;
    }
    if(com.Command == Inc)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + 1 * 3;
    }
    if(com.Command == Dec)
    {
      second_pixel = second_pixel + 1 * 3;
    }
    if(com.Command == Remove)
    {
      second_pixel = second_pixel + 2 * 3;
    }
    if(com.Command == If)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + 2 * 3;
    }
  }
  if(com.Place == DownLeft)
  {
    if(com.Command == Print)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel - 2 * 3;
    }
    if(com.Command == Put)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel - 1 * 3;
    }
    if(com.Command == Ask)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + row_length * 3;
    }
    if(com.Command == Inc)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel - 1 * 3;
    }
    if(com.Command == Dec)
    {
      second_pixel = second_pixel - 1 * 3;
    }
    if(com.Command == Remove)
    {
      second_pixel = second_pixel - 2 * 3;
    }
    if(com.Command == If)
    {
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel + row_length * 3;
      second_pixel = second_pixel - 2 * 3;
    }
  }
  if(com.Place == TopLeft)
  {
    if(com.Command == Print)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - 2 * 3;
    }
    if(com.Command == Put)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - 1 * 3;
    }
    if(com.Command == Ask)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - row_length * 3;
    }
    if(com.Command == Inc)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - 1 * 3;
    }
    if(com.Command == Dec)
    {
      second_pixel = second_pixel - 1 * 3;
    }
    if(com.Command == Remove)
    {
      second_pixel = second_pixel - 2 * 3;
    }
    if(com.Command == If)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - 2 * 3;
    }
  }
  if(com.Place == TopRight)
  {
    if(com.Command == Print)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel + 2 * 3;
    }
    if(com.Command == Put)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel + 1 * 3;
    }
    if(com.Command == Ask)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - row_length * 3;
    }
    if(com.Command == Inc)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel + 1 * 3;
    }
    if(com.Command == Dec)
    {
      second_pixel = second_pixel + 1 * 3;
    }
    if(com.Command == Remove)
    {
      second_pixel = second_pixel + 2 * 3;
    }
    if(com.Command == If)
    {
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel - row_length * 3;
      second_pixel = second_pixel + 2 * 3;
    }
  }
  if(second_pixel > size || second_pixel < 0)
  {
    out = 'I';
    goto end;
  }
  if(com.Command == Print)
  {
    if(is_super_debug)
    {
      cout << "Command: Print" << endl;
    }
    if(Data_Queue.size() == 0)
    {
      out = 'H';
      goto end;
    }
    cout << Data_Queue.front();// << endl;
    Data_Queue.pop();
  }
  if(com.Command == Put)
  {
    if(is_super_debug)
    {
      cout << "Command: Put/Put Back" << endl;
    }
    int data_pix = cur_pix;
    if(com.Place == TopLeft)
    {
      data_pix = data_pix - row_length * 3;
    }
    if(com.Place == TopRight)
    {
      data_pix = data_pix - row_length * 3;
    }
    if(com.Place == DownLeft)
    {
      data_pix = data_pix + row_length * 3;
    }
    if(com.Place == DownRight)
    {
      data_pix = data_pix + row_length * 3;
    }
    if(data_pix > size || data_pix < 0)
    {
      out = 'I';
      goto end;
    }
    if(is_super_debug)
    cout << "Data pixel coordinates: " << data_pix << endl;
    if(Table[data_pix] == 255 && Table[data_pix + 1] == 255 && Table[data_pix + 1] == 255)
    {
      char c = Data_Queue.front();
      if(is_super_debug)
      cout << "Command is now Put Back (Data pixel is white), data which will be copied to the end of Data Queue: " << c << " (dec) " << (int) c << endl;
      //Data_Queue.pop();
      Data_Queue.push(c);
    }
    else
    {
      if(is_super_debug)
      cout << "Command is now Put (Data pixel is not white), data which will be put on the end of Data Queue: " << Table[data_pix] << " (dec) " << (int) Table[data_pix] << endl;
      Data_Queue.push(Table[data_pix]);
    }
  }
  if(com.Command == Ask)
  {
    if(is_super_debug)
    {
      cout << "Command: Ask" << endl;
    }
    unsigned int a = 0;
    if(is_ask_prompt)
    {
      cout << endl << "?";
    }
    cin >> a;
    if(is_super_debug)
    cout << "Data which will be put on the end of Data Queue: " << (unsigned char) a << " (dec) " << a << endl;
    Data_Queue.push((unsigned char)a);
  }
  if(com.Command == Inc)
  {
    if(is_super_debug)
    {
      cout << "Command: Increment" << endl;
    }
    if(Data_Queue.size() == 0)
    {
      out = 'H';
      goto end;
    }
    if(is_super_debug)
    cout << "Data Queue front before incrementation: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front() << endl;
    Data_Queue.front()++;
    if(is_super_debug)
    cout << "Data Queue front after incrementation: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front() << endl;
  }
  if(com.Command == Dec)
  {
    if(is_super_debug)
    {
      cout << "Command: Decrement" << endl;
    }
    if(Data_Queue.size() == 0)
    {
      out = 'H';
      goto end;
    }
    if(is_super_debug)
    cout << "Data Queue front before decrementation: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front() << endl;
    Data_Queue.front()--;
    if(is_super_debug)
    cout << "Data Queue front after decrementation: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front() << endl;
  }
  if(com.Command == Remove)
  {
    if(is_super_debug)
    {
      cout << "Command: Remove" << endl;
    }
    if(Data_Queue.size() == 0)
    {
      out = 'H';
      goto end;
    }
    if(is_super_debug)
    cout << "Value that is being removed: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front() << endl;
    Data_Queue.pop();
    if(is_super_debug)
    cout << "New front value: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front();
  }
  if(com.Command == If)
  {
    if(is_super_debug)
    {
      cout << "Command: If" << endl;
    }
    if(Data_Queue.size() == 0)
    {
      out = 'H';
      goto end;
    }
    if(Data_Queue.size() == 1)
    {
      out = 'A';
      goto end;
    }
    if(is_super_debug)
    cout << "Data Queue front value: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front() << " Data Queue back value: " << Data_Queue.back() << " (dec) " << (int) Data_Queue.back() << endl;
    if(Data_Queue.front() != Data_Queue.back())
    {
      if(is_super_debug)
      cout << "Values are not equal" << endl;
      int data_pix = cur_pix;
      if(com.Place == TopLeft)
      {
        data_pix = data_pix - row_length * 3;
      }
      if(com.Place == TopRight)
      {
        data_pix = data_pix - row_length * 3;
      }
      if(com.Place == DownLeft)
      {
        data_pix = data_pix + row_length * 3;
      }
      if(com.Place == DownRight)
      {
        data_pix = data_pix + row_length * 3;
      }
      if(data_pix > size || data_pix < 0)
      {
        out = 'I';
        goto end;
      }
      if(Table[data_pix] == 255 && Table[data_pix + 1] == 255 && Table[data_pix + 1] == 255)
      {
        out = 'I';
        goto end;
      }
      second_pixel = data_pix;
    }
  }
  cur_pix = second_pixel;
  end:
  return out;
}
int InterpretCode(string path) {
  //out << path << endl;
  fstream file;
  file.open(path, ios::in);
  if (!file) {
    cout << "File not found: " << path << endl;
    return 1;
  }
  char command;
  //stringstream buffer;
  //buffer << file.rdbuf();
  file.unsetf(ios_base::skipws);
  int i = 0;
  int cont = 0;
  while (!file.eof()) {

      file >> Table[i];

    i++;
  }

  //cout << Table[0] << endl;

  if(Table[0] != 'P' || Table[1] != '6')
  {
    cout << "Invalid file" << endl;
    file.close();
    return 3;
  }
  file.close();

  int pix = Get_Row_Length(); //first pixel of data after file header
  Add_Start_Data(pix);
  cur_pix = pix + row_length * 3; //get second row of FILE
  //row_length = row_length + 3;
  bool loop = true;
  while (loop)
  {
    if(is_debug)
    {
      cout << "Current Pixel (Table Cell Number): " << cur_pix << endl;
      cout << "Red: " << (int)Table[cur_pix] << endl;
      cout << "Green: " << (int)Table[cur_pix + 1] << endl;
      cout << "Blue: " << (int)Table[cur_pix + 2] << endl;
      cout << "Data Queue size " << Data_Queue.size() << endl;
      if(Data_Queue.size() == 0)
      {
        cout << "Data Queue is empty!" << endl;
      }
      else if (Data_Queue.size() == 1)
      {
        cout << "Data Queue has one element: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front() << endl;
      }
      else
      {
        cout << "Data Queue front: " << Data_Queue.front() << " (dec) " << (int) Data_Queue.front() << endl << "Data Queue back: " << Data_Queue.back() << " (dec) " << (int) Data_Queue.back() << endl;
      }
    }
    Command com = Get_Command();
    if(com.Command == white)
    {
      if(detailed_errors)
      {
        if(!hide_errors)
        {
          cout << "Error: Got white pixel!" << endl;
        }
      }
      else
      {
        if(!hide_errors)
        {
          cout << "K" << endl; // error code for "got white pixel"
        }
      }
      if(!continue_on_errors)
      loop = false;
    }
    if(com.Command == black)
    {
      loop = false;
    }
    if(com.Command == none || com.Place == null)
    {
      if(detailed_errors)
      {
        if(!hide_errors)
        {
          cout << "Error: Got invalid color!" << endl;
        }
      }
      else
      {
        if(!hide_errors)
        {
          cout << "U" << endl; // error code for "got invalid color"
        }
      }
      if(!continue_on_errors)
      loop = false;
    }
    char out = Interpret_Command(com);
    if(out != ' ')
    {
      if(detailed_errors)
      {
        if(!hide_errors)
        {
          switch (out)
          {
            case 'H':
            cout << "Error: Data Queue is empty!" << endl;
            break;
            case 'A':
            cout << "Error: Data Queue is too small!" << endl;
            break;
            case 'I':
            cout << "Error: Got invalid pixel!" << endl;
            break;
            case 'K':
            cout << "Error: Got white pixel!" << endl;
            break;
            case 'U':
            cout << "Error: Got invalid color!" << endl;
            break;
            default:
            cout << "Error: Other Error!" << endl;
            break;
          }
        }
      }
      else
      {
        if(!hide_errors)
        {
          cout << out << endl;
        }
      }
      if(!continue_on_errors)
      loop = false;
    }
  }
  return 0;
}
int main(int argc, char **argv) {
  if (argc == 1) {
    cout << "Nothing to do" << endl;
  }
  for (int i = 1; i < argc; i++) {
    string var = argv[i];
    if (var == "--help") {
      cout << "USAGE:" << endl
           << "hk [ARGS] [INPUT FILE]" << endl;
      cout << "--debug - Interprets file in debug mode" << endl;
      cout << "--super-debug - Interprets file in very verbose mode producing a lot of debug information" << endl;
      cout << "--detailed-errors - Shows detailed error descriptions instead of standard one letter ones" << endl;
      cout << "--ignore-errors - Continues interpreting program even if errors happen" << endl;
      cout << "--hide-errors - Hides all error messages" << endl;
      cout << "--show-prompt - Shows question mark when waiting for user data" << endl;
      cout << "--help  - Prints this message" << endl;
      cout << "--version - Prints version info" << endl;
      cout << "--changelog - Shows changelog and exits" << endl;
      return 0;
    } else if (var == "--version") {
        cout << "Haiku Interpreter v1.3.2" << endl;
        return 0;
    } else if (var == "--super-debug") {
        cout << "Super Debug mode enabled" << endl;
        is_debug = true;
        is_super_debug = true;
    } else if (var == "--detailed-errors") {
        cout << "Detailed error descriptions mode enabled" << endl;
        detailed_errors = true;
    } else if (var == "--ignore-errors") {
        cout << "Ignoring all errors" << endl;
        continue_on_errors = true;
    } else if (var == "--hide-errors") {
        cout << "No error messages mode enabled" << endl;
        hide_errors = true;
    } else if (var == "--show-prompt") {
        cout << "Showing prompt" << endl;
        is_ask_prompt = true;
    } else if (var == "--debug") {
        cout << "Debug mode enabled" << endl;
        is_debug = true;
    } else if (var == "--changelog") {
        cout << "Changelog: " << endl;
        cout << "1.0 - First version" << endl;
        cout << "1.1 - Added support for 999x999 files" << endl;
        cout << "1.2 - Improved debug mode" << endl;
        cout << "1.3 - Added super debug mode, hide errors, ignore errors, show promptand detailed errors modes (and changelog)" << endl;
        cout << "1.3.1 - Hide errors mode now works" << endl;
        cout << "1.3.2 - Minor bug fixes" << endl;
        return 0;
    } else {
        cout << argv[i] << endl;
        int out = 0;
        out = InterpretCode(argv[i]);
      return out;
    }
  }
}
//TheNeverMan 2021
