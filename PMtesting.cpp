//---------------------------------------------------------------------------

#include<string.h>
#include<iostream>
#include<fstream>
#include<windows.h>
#include<string>

using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
//---------------------------------------------------------------------------


HWND AOL();
HWND MDI();
HWND ListBox();
HWND FindChat();
std::string TextFromWelcomeScreen();
std::string ScreenNameWS();
void Keyword(char *kw);
std::string LastChatLine();
void initBuffer(char* buffer, short int bufferSize);
std::string LastChatLine();
int MyChatLine();
void SendChat(char *chat);
std::string ReturnCommand();

int main()
{
  //  for(;;)
   // {
      ReturnCommand();
  //    Sleep(50);
   // }
    
    system("PAUSE");
    return 0;    
}

HWND AOL()
{
    HWND AOLwindow;
    AOLwindow = FindWindowEx(0,0,"AOL Frame25",0);
    return AOLwindow;  
}
 
HWND MDI()
{
    HWND MDIwindow;
    MDIwindow = FindWindowEx(AOL(),0,"MDIClient",0);
    return MDIwindow;    
}
 
HWND FindChat()
{
     HWND hwndChild = 0, hwndAOLChatForm = 0;

     do
     {
          hwndAOLChatForm = FindWindowEx(hwndChild, 0, "_AOL_Static", "AOL_CHAT_FORM");
          if (hwndAOLChatForm) break;
          hwndChild = FindWindowEx(MDI(), hwndChild, "AOL Child", 0);
     }
     while (hwndChild);
     return hwndChild;
}

HWND ListBox()
{
    HWND listbox;
    listbox = FindWindowEx(FindChat(),0,"_AOL_Listbox",0);
    return listbox;
}

std::string TextFromWelcomeScreen()
{
    HWND TopAOLwindow = 0;
    int NumberOfCharsInWelcomeScreen;
    char BufferForWS[40];
    char NameOfWelcomeWindow[40];

    int counter = 0;
   do{
        TopAOLwindow = FindWindowEx(MDI(),TopAOLwindow,"AOL Child",0);
        NumberOfCharsInWelcomeScreen = SendMessage(TopAOLwindow,WM_GETTEXTLENGTH,0,0);
        NumberOfCharsInWelcomeScreen += 1;


        SendMessage(TopAOLwindow,WM_GETTEXT,NumberOfCharsInWelcomeScreen,
        (WPARAM)NameOfWelcomeWindow);
        strcpy(BufferForWS,NameOfWelcomeWindow);      
        counter += 1;
      
        if (counter == 50)
        {
        //    Keyword("welcome");
            //FIX WAIT FOR WINDOW FOR WELCOME
        }
  }while(BufferForWS[NumberOfCharsInWelcomeScreen - 2] != '!');

  std::string TextFromWelcome;
  TextFromWelcome.insert(0,BufferForWS);
  return TextFromWelcome;
}

std::string ScreenNameWS()
{
     char ScreenName[16];
     std::string BufferForWS;
     BufferForWS = TextFromWelcomeScreen();
   // cout << BufferForWS.size();

    int counter = 0;
    for(int i = 0; i < 16; i++)
    {
        if (BufferForWS[8+i] == '!') break;

        else
        ScreenName[i] = BufferForWS[9+i]; 
        counter++;    
    }
    ScreenName[counter - 1] = 0;

    std::string MyScreenName;
    MyScreenName.insert(0,ScreenName);
    return MyScreenName;
}

void Keyword(char *kw)
{
	char keyword[MAX_PATH] = "aol://1722:";

	strcat(keyword, kw);
	ShellExecute(0, 0, keyword, 0, 0, SW_SHOW);
}


std::string LastChatLine()
{
    int  lineCount;               // returns the amount of lines in the chatroom
    int  GetChatLine;             // shows how many chars are in my chat string
    char CompareSN[20];           // used to compare the screen name
    char ChatString[400];
    initBuffer(ChatString, 400);  // Buffer for chatstring


    HWND ChatReadWindow;
    ChatReadWindow = FindWindowEx(FindChat(),0,"RICHCNTLREADONLY",0);
    //counts the lines in the chatroom
    lineCount = SendMessage(ChatReadWindow, EM_GETLINECOUNT, 0, 0);


    //gets a specific chatline controlled by the 3rd paramater
    GetChatLine = SendMessage(ChatReadWindow, EM_GETLINE, (WPARAM)lineCount,(LPARAM) ChatString );
    ChatString[GetChatLine] = '\0';
/*
     char *tmp1;
string tmp2;

/* read data into tmp1 */
/* tmp1 now has 1-10 megs of data in it

temp2 = tmp2.insert(0, tmp1);
     */

    std::string ChatStringReturned;
    ChatStringReturned.insert(0,ChatString);
    return ChatStringReturned;
}

//---------------------------------------------------------------------------

int MyChatLine()
{
    int SizeOfScreenName;
    char ChatLine[20];

    std::string edited;
    SizeOfScreenName = ScreenNameWS().length();

    int counter = 0;
    for(int i = 0; i < 20; i++)
    {
        ChatLine[i] = LastChatLine().c_str()[i];
        if(LastChatLine().c_str()[i] == ':') break;
        counter++;
    }
     ChatLine[counter] = '\0';

    std::string SNfromChatRoom;
    SNfromChatRoom.insert(0,ChatLine);
    if(SNfromChatRoom == ScreenNameWS())
    return 1;

    else
    return 0;

}

void SendChat(char *chat)
{
    HWND hwndChatBox;

    hwndChatBox = FindWindowEx(FindChat(), 0, "RICHCNTL", 0);
    SendMessage(hwndChatBox, WM_SETTEXT, 0,(LPARAM) chat);
	Sleep(10);
    SendMessage(hwndChatBox, WM_CHAR, 13, 0);
	Sleep(10);
}

std::string ReturnCommand()
{

    char ScreenName[20];
    ScreenName[0]='\0';
    /*
     *GetSize of screenName from welcome screen
     *add 5 to the end of welcome screen text to get the first letter of chatline
     * continue while there is a space
     */
    strcpy(ScreenName,ScreenNameWS().c_str());
    char ChatLastLine[100];
    ChatLastLine[0] = '\0';
    strcpy(ChatLastLine,LastChatLine().c_str());
   
    char command[10];
    char ScreenNameCmp[16];
    int lengthOfScreenName = strlen(ScreenName);
    for(int i = 0; i < lengthOfScreenName;i++)
        ScreenNameCmp[i] = ChatLastLine[i];
        
   
    ScreenNameCmp[lengthOfScreenName] = '\0';
    cout << ScreenNameCmp << endl;
    cout << ScreenName << endl;
    
    if(!strcmp(ScreenNameCmp,ScreenName))
    {
    for(int i = 0; i < 20; i++)
    {
    //cout << ChatLastLine[strlen(ScreenName) + 3 + i] << endl;
     command[i] = ChatLastLine[strlen(ScreenName) + 3 + i];
    // << " " << (int)ChatLastLine[strlen(ScreenName) + 3 + i];
     if(ChatLastLine[strlen(ScreenName) + 3 + i] == 0)break;
    }
    cout << command << endl;
    std::string ReturningCommand;
    ReturningCommand.insert(0,command);
    // cout << ReturningCommand << endl;
    return ReturningCommand;
    }
    else
    return 0; 
}

void initBuffer(char* buffer, short int bufferSize)
{
    buffer[0] = (char) (bufferSize % 256);
    buffer[1] = (char) (bufferSize >> 8);
}
