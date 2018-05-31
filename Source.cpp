//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Source.h"
#include<string.h>
#include<iostream>
#include<fstream>
#include "Unit2.h"
#include <sstream>
using std::ofstream;
using std::ifstream;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPMmainForm *PMmainForm;

HWND AOL();
HWND MDI();
HWND ListBox();
HWND FindChat();
int LastLineCount();
std::string TextFromWelcomeScreen();
std::string ScreenNameWS();
void Keyword(char *kw);
//std::string LastChatLine();
void initBuffer(char* buffer, short int bufferSize);
BOOL CheckForColon(int LastLineNumber);
void SendChat(char *chat);
std::string ScreenNameFromLastChatLine();
void WaitForChatSend();
template< class T >
std::string ConvertToString( const T& _t );
int AmountOfCharsInLastChatLine(int ChatLineToGet = LastLineCount());
int NumberOfLastChatLine();
void RestoreDefaultChatSends();
std::string ExtendedStringFromChat();
std::string TriggerCommand();
void RestoreTriggers();
void ChangeTrigger();
std::string ColonCatStrings();
HWND ChatReadWindow();
std::string GetChatRoomLine(int LineNumber);
std::string ChatRoomStrings();
std::string CompleteChatString(int LastLine = LastLineCount());
BOOL MyChatLine();
std::string MyChatstring();
std::string ChatStringMem(std::string ChatString);
void CreateTriggerFile();
void WriteKeyToFile(char* Subject, char* StaticTrigger,
                    char* VariableTrigger, char* FilePath);

//---------------------------------------------------------------------------
__fastcall TPMmainForm::TPMmainForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TPMmainForm::FormCreate(TObject *Sender)
{
    ofstream fout;
    ifstream fin;

    // if aol isn't open when program starts
    // close aol
    if(!AOL())
    {
        ShowMessage("AOL isn't open.");
        return;
    }

    else
    {
        // holds handle from user input box
        AnsiString UsersHandle;

       //create ProjectMayhemFiles directory if it doesn't exist
        int file = CreateDirectory("C:\\Project Mayhem Files",
                                   NULL) ;

        if (file == 0) // if directory is already present
        {
             //  open ProjectMayhem.ini to check if it exist
             //  if it doesn't exist, create it
             fin.open ("c:\\Project Mayhem Files\\ProjectMayhem.ini");

             if(!fin)
             {
                 //prompt user to enter handle then store in the ProjectMayhem.ini
                 UsersHandle = InputBox("Enter Your Handle",
                                        "Enter Your Handle Below",
                                        "Type Handle Here");

                 //write the person handles handle in the ini file
                 WritePrivateProfileString("Users Stats",
                                           "UsersHandle",
                                           UsersHandle.c_str(),
                                           "c:\\Project Mayhem Files\\ProjectMayhem.ini");
             }
        }


        // if directory isn't present, create directory and add the ini file
        else
        {
             //  check if ProjectMayhem.ini file is in directory
             //  if it's not in the directory, create it
             //  write the users handle to the ProjectMayhem.ini

             fin.open("c:\\Project Mayhem Files\\ProjectMayhem.ini");
             if (!fin)// if file can't be opened, create it
             {
                 //prompt user to enter handle, then store in the ProjectMayhem.ini
                 UsersHandle = InputBox("Enter Your Handle",
                                        "Enter Your Handle Below",
                                        "Type Handle Here");


                 //write the person handles handle in the ini file
                 WritePrivateProfileString("Users Stats",
                                           "UsersHandle",
                                           UsersHandle.c_str(),
                                           "c:\\Project Mayhem Files\\ProjectMayhem.ini");
             }
        }
        CreateTriggerFile();
        fout.close();
    }
}
//---------------------------------------------------------------------------

HWND AOL()
{
    HWND AOLwindow;
    AOLwindow = FindWindowEx(0,
                             0,
                             "AOL Frame25",
                             0);
    return AOLwindow;
}

HWND MDI()
{
    HWND MDIwindow;

    MDIwindow = FindWindowEx(AOL(),
                             0,
                             "MDIClient",
                             0);
    return MDIwindow;
}

HWND FindChat()
{
     HWND hwndChild = 0;
     HWND hwndAOLChatForm = 0;

     do
     {
          hwndAOLChatForm = FindWindowEx(hwndChild,
                                         0,
                                         "_AOL_Static",
                                         "AOL_CHAT_FORM");
          if (hwndAOLChatForm)
          {
              break;
          }
          hwndChild = FindWindowEx(MDI(),
                                   hwndChild,
                                   "AOL Child",
                                   0);
     }
     while (hwndChild);
     return hwndChild;
}
 /*
HWND ListBox()
{
    HWND listbox;
    listbox = FindWindowEx(FindChat(),0,"_AOL_Listbox",0);
    return listbox;
}
  */
std::string TextFromWelcomeScreen()
{

    HWND TopAOLwindow = 0;
    int NumberOfCharsInWelcomeScreen; // holds how many chars in welcome
    int counter = 0;
    BOOL CounterForTermination = FALSE; // used as a counter to terminate program

    // create a dynamic allocated space to fit the text from welcome screen
    char *NameOfWelcomeWindow;
    NameOfWelcomeWindow = new char[NumberOfCharsInWelcomeScreen];

    //  scroll through the windows to find the welcome window
    do {
        // Find the aol child window
        TopAOLwindow = FindWindowEx(MDI(),
                                    TopAOLwindow,
                                    "AOL Child",
                                    0);

        //get how many chars are in the aol child window
        NumberOfCharsInWelcomeScreen = SendMessage(TopAOLwindow,
                                                   WM_GETTEXTLENGTH,
                                                   0,
                                                   0);
        NumberOfCharsInWelcomeScreen += 1;

        // create a buffer the size of "NumberOfCharsInWelcomeScreen"
        NameOfWelcomeWindow = new char[NumberOfCharsInWelcomeScreen];

        // get text from aol child window and store it in "NameOfWelcomeWindow"
        SendMessage(TopAOLwindow,
                    WM_GETTEXT,
                    NumberOfCharsInWelcomeScreen,
                   (WPARAM)NameOfWelcomeWindow);

        //if the AOLChild isn't the welome window delete the buffer
        if (NameOfWelcomeWindow[NumberOfCharsInWelcomeScreen - 2] != '!')
        {
            delete [] NameOfWelcomeWindow;
            counter += 1;
        }

        // loop 50x's if the welcome screen isn't found
        if (counter == 50)
        {
            //  Prompt user  to terminate program or open welcome screen
            //  if usre clicks on "yes" go to keyword welcome
            if(Application->MessageBox("Welcome Screen Can't be found, press the \"Yes\" button if you want Project Mayhem to Open the welcome screen. This is necessary for your chat commands to work, if you push \"No\" Project Mayhem will terminate.",
                                       "You lost your welcome screen",
                                       MB_YESNO | MB_ICONHAND) == IDYES)
            {
                Keyword("welcome");
            }
            // if user clicks "NO" terminate program
            else
            {
                CounterForTermination = TRUE;
                 break;
            }
        }
    } while (NameOfWelcomeWindow[NumberOfCharsInWelcomeScreen - 2] != '!');

    // return the string "exit" if person terminates the program
    if(CounterForTermination)
    {
        delete [] NameOfWelcomeWindow;
        std::string Terminate;
        Terminate.insert(0,"exit");
        return Terminate;
    }

    // return the text from welcome screen
    else
    {
        std::string TextFromWelcome;
        TextFromWelcome.insert(0,
                               NameOfWelcomeWindow);
        delete [] NameOfWelcomeWindow;

        return TextFromWelcome;
    }
}

std::string ScreenNameWS()
{
    char ScreenName[16];
    char LowerCasedSN[16];
    std::string BufferForWS;
    BufferForWS = TextFromWelcomeScreen();

    int counter = 0;

    //copies text from the welcome screen in to ScreenName and lower case it
    for (int i = 0; i < 16; i++)
    {
        if (BufferForWS[8+i] == '!')
        {
          break;
        }

        else
        {
          ScreenName[i] = tolower(BufferForWS[9+i]);
          counter++;
        }
    }
    
    ScreenName[counter - 1] = 0;
    std::string MyScreenName;

    MyScreenName.insert(0,
                        ScreenName);

    return MyScreenName;
}

void Keyword(char *kw)
{
	char keyword[MAX_PATH] = "aol://1722:";

	strcat(keyword,
               kw);

	ShellExecute(0,
                     0,
                     keyword,
                     0,
                     0,
                     SW_SHOW);
}

// checks if last chatline has a colon
BOOL CheckForColon(int LastLineNumber)
{
    int  GetChatLine;
    char CompareSN[20];
    char ChatString[400];
    initBuffer(ChatString, 400);
    BOOL Counter = TRUE;

    if (FindChat())
    {
        HWND ChatReadWindow;

        ChatReadWindow = FindWindowEx(FindChat(),
                                      0,
                                      "RICHCNTLREADONLY",
                                      0);



        GetChatLine = SendMessage(ChatReadWindow,
                                  EM_GETLINE,
                                  (WPARAM)LastLineNumber,
                                  (LPARAM)ChatString);

        ChatString[GetChatLine] = '\0';

        for (int i = 17; i > 0; i--)
        {
            if (ChatString[i] == ':')
            {
                break;
            }

            if (i == 1)
            {
                Counter = FALSE;
            }
        }

    }

    return Counter;
}

void initBuffer(char* buffer, short int bufferSize)
{
    buffer[0] = (char) (bufferSize % 256);
    buffer[1] = (char) (bufferSize >> 8);
}
//---------------------------------------------------------------------------

                       
void SendChat(char *chat)
{
    HWND hwndChatBox;

    hwndChatBox = FindWindowEx(FindChat(),
                               0,
                               "RICHCNTL",
                               0);
    SendMessage (hwndChatBox,
                 WM_SETTEXT,
                 0,
                 (LPARAM)chat);

    Sleep ( 10 );

    SendMessage(hwndChatBox,
                WM_CHAR,
                13,
                0);
    Sleep(10);
}

// return the s/n from the last "complete" line someone typed.
std::string ScreenNameFromLastChatLine()
{
     int AmountOfCharsInLastString = AmountOfCharsInLastChatLine();

    std::string LastChatString = CompleteChatString();
    std::string::size_type PositionOfColon;
    PositionOfColon = LastChatString.find(':',0);

    AmountOfCharsInLastString = (AmountOfCharsInLastString - 3) -
                                PositionOfColon;

    LastChatString.erase(PositionOfColon,AmountOfCharsInLastString + 3);

    return LastChatString;
}

// returns if my chatline is true
BOOL MyChatLine()
{
    std::string ScreenNameFromWelcomeScreen;
    ScreenNameFromWelcomeScreen = ScreenNameWS();

    std::string LastLineScreenName;
    LastLineScreenName = ScreenNameFromLastChatLine();

    for (int i = 0; LastLineScreenName[i]; i++)
    {
        LastLineScreenName[i] = tolower(LastLineScreenName[i]);
    }

    if (ScreenNameWS() == LastLineScreenName)
    return TRUE;

    else
    return FALSE;
}

std::string MyChatString()
{
    if(MyChatLine())
    {
        return CompleteChatString();
    }

    else
    {
        std::string NotMyLine;
        NotMyLine = ("not my line");
        return NotMyLine;
    }
}

void __fastcall TPMmainForm::Timer1Timer(TObject *Sender)
{
  
      /*you're on getting the trigger from the chatroom in this timer*/
 /*       char *StringFromFile;
        StringFromFile = new char [256];
        GetPrivateProfileString("Triggers",
                                "Command Trigger",
                                "Can't be found",
                                StringFromFile,
                                256,
                                "c:\\Project Mayhem Files\\ProjectMayhemTriggers");
     // the trigger command is the function which returns the last line.
/*   if(!strcmp(TriggerCommand().c_str(),exit.c_str()))
        {
             SendChat("<font face=\"Arial\" color=#0000FF><b> ·°·• </b><font face=\"Arial\" color=#000000>Exited Project Mayhem<font face=\"Arial\" color=#0000FF><b> •·°·</b>");
             WaitForChatSend();
        }

      /* // strcat(CompareStringCommand,"restore");
        else if(!strcmp(TriggerCommand().c_str(),restore.c_str()))
        {
             SendChat("<font face=\"Tahoma\" color=#000000>Prøjèct Màyhèm<font face=\"Tahoma\" color=#0000FF> ·°·<font face=\"Tahoma\" color=#000000> typè \"yès\" tø rèstørè àll, ør typè \"restore\" plus thè cømmànd yøù wísh tø rèstørè");
            // showMessage(RCextendedText(ReturnCommand()).c_str());
             WaitForChatSend();

        }

        else if(!strcmp(TriggerCommand().c_str(),"trig"))
        {
           if(!strcmp(ExtendedStringFromChat().c_str()," NULL"))
           {
               WritePrivateProfileString ("Triggers","Command Trigger",
               "","c:\\Project Mayhem Files\\ProjectMayhemTriggers");
               SendChat("trigger has been set");
               WaitForChatSend();
           }
           else
           {
               WritePrivateProfileString ("Triggers","Command Trigger",
               ExtendedStringFromChat().c_str(),
               "c:\\Project Mayhem Files\\ProjectMayhemTriggers");
               WaitForChatSend();
           }   */
 }
//---------------------------------------------------------------------------


void WaitForChatSend()
{
    int LastChatLineNumber;
    HWND ChatReadWindow;

    ChatReadWindow = FindWindowEx(FindChat(),
                                  0,
                                  "RICHCNTLREADONLY",
                                  0);

    LastChatLineNumber = SendMessage(ChatReadWindow,
                                     EM_GETLINECOUNT,
                                     0,
                                     0);

    int LastChatLineNumberCompare;
    LastChatLineNumberCompare = LastChatLineNumber;

    do {
        LastChatLineNumberCompare = SendMessage(ChatReadWindow,
                                                EM_GETLINECOUNT,
                                                0,
                                                0);

    } while (LastChatLineNumberCompare == LastChatLineNumber);
}

template< class T >
std::string ConvertToString( const T& _t )
{
    std::stringstream os;
    os << _t;
    return os.str( );
}

int AmountOfCharsInLastChatLine(int ChatLineToGet)
{
    int  GetChatLine;
    char ChatString[400];
    initBuffer(ChatString, 400);

    HWND ChatReadWindow;
    ChatReadWindow = FindWindowEx(FindChat(),
                                  0,
                                  "RICHCNTLREADONLY",
                                  0);

    GetChatLine = SendMessage(ChatReadWindow,
                              EM_GETLINE,
                              (WPARAM)ChatLineToGet,
                              (LPARAM)ChatString);

   return GetChatLine;
}

int NumberOfLastChatLine()
{
    int  lineCount;
    int  GetChatLine;
    char CompareSN[20];
    char ChatString[400];
    initBuffer(ChatString, 400);


    if (FindChat())
    {
        HWND ChatReadWindow;
        ChatReadWindow = FindWindowEx(FindChat(),
                                      0,
                                      "RICHCNTLREADONLY",
                                      0);

        lineCount = SendMessage(ChatReadWindow,
                                EM_GETLINECOUNT,
                                0,
                                0);

        GetChatLine = SendMessage(ChatReadWindow,
                                  EM_GETLINE,
                                  (WPARAM)lineCount,
                                  (LPARAM)ChatString );
        ChatString[GetChatLine] = '\0';

        return GetChatLine;
    }

    else
    return 0;


}


void RestoreDefaultChatSends()
{
  /*  WritePrivateProfileString ("ChatSends","exit",
    "<font face=\"Arial\" color=#0000FF><b> ·°·• </b><font face=\"Arial\" color=#000000>Exited Project Mayhem<font face=\"Arial\" color=#0000FF><b> •·°·</b>",
    "c:\\Project Mayhem Files\\ProjectMayhemChatSends");


    WritePrivateProfileString ("Triggers","Command Trigger",
    RCextendedText(ReturnCommand()).c_str(),
    "c:\\Project Mayhem Files\\ProjectMayhemTriggers");
    WaitForChatSend(); */

}


//---------------------------------------------------------------------------


void TerminateAppx()
{
    Application->Terminate();
}

void __fastcall TPMmainForm::Button1Click(TObject *Sender)
{
   /*

    if(SNtoHandCuff->Text == "")
    {
        ShowMessage("You didn't enter a s/n.");
    }

    else if (strlen(SNtoHandCuff->Text.c_str()) > 16)
    {
        ShowMessage("Screen name is too long.");
    }

    else
    {
        Timer1->Enabled = true;
        Button1->Caption = "Stop";
    } */


    AnsiString handCuffEdit = SNtoHandCuff->Text;

    // Get Screen name from Chatroom
    // screennameChat will hold the screen name from thelast chatline.
    char screennameChat[17];
    strcpy(screennameChat,CompleteChatString().c_str());
    for(int i = 0; screennameChat[i]; i++)
    {
        if(screennameChat[i] == ':')
        {
            screennameChat[i] = '\0';
        }
    }

    static std::string compareChatString;
    //if screenName From edit and last chatline are the same
    // handcuff
    int screennameLength;
    std::string chatString;

    if(!strcmp(screennameChat,handCuffEdit.c_str()))  // good
    {

       if(compareChatString == "")
       {
            screennameLength = strlen(screennameChat);
            chatString = CompleteChatString().erase(0,screennameLength + 3);
            SendChat((char*)chatString.c_str());

            compareChatString = chatString;


            // ShowMessage("you're in if");
       }

       else
       {
           screennameLength = strlen(screennameChat);
           std::string newString = CompleteChatString().erase(0,screennameLength + 3);
          // ShowMessage(newString.c_str());
           ShowMessage(chatString.c_str());
           if(!strcmp(newString.c_str(),chatString.c_str()))
           {
               return;
           }

           else
           {
              chatString = CompleteChatString().erase(0,screennameLength + 3);
              compareChatString = chatString;
              SendChat((char*)chatString.c_str());
            }

       }
    }

   /*
      AnsiString SnToHandCuff = SNtoHandCuff->Text;

    char HandCuffBuffer[17];
    strcpy(HandCuffBuffer,CompleteChatString().c_str());

    for(int i = 0; HandCuffBuffer[i];i++)
    {
        if(HandCuffBuffer[i] == ':')
        {
            HandCuffBuffer[i] = '\0';
        }
    }

   // returns the length of the chatline minus s/n length
   int LengthOfSn = strlen(HandCuffBuffer) ;

   std::string ChatString = CompleteChatString().erase(0,LengthOfSn + 3);

   // the chatlines are the same break
   if(!strcmp(ChatStringMem(ChatString).c_str(),ChatString.c_str()))
   {
      return;
   }

   else if(!strcmp(HandCuffBuffer,SnToHandCuff.c_str()))
   {
      SendChat((char*)ChatString.c_str());
   }

   /*

   
   // thisis the chatscan block below
  /*
    char string [MAX_PATH];
    strcpy(string,CompleteChatString().c_str());
    SendChat(string);
    SendChat("testing C++ chatscan");

                   */
     //WriteKeyToFile(char* Subject, char* StaticTrigger,
     //               char* VariableTrigger, char* FilePath)

   /*
    RECT test;

    HWND hwnd;
    hwnd = ChatReadWindow();
    GetWindowRect(hwnd,&test);

    std::string WindowPositionLeft;
    WindowPositionLeft = (0,"Chatroom Left coordinate position is: ");
    WindowPositionLeft += ConvertToString(test.left);
    SendChat((char*)WindowPositionLeft.c_str());
    Sleep(300);

    std::string WindowPositionBottom;
    WindowPositionBottom = (0,"Chatroom Bottom coordinate position is: ");
    WindowPositionBottom += ConvertToString(test.right);
    SendChat((char*)WindowPositionBottom.c_str());
    Sleep(300);

    std::string WindowPositionTop;
    WindowPositionTop = (0,"Chatroom Top coordinate positoin is: ");
    WindowPositionTop += ConvertToString(test.top);
    SendChat((char*)WindowPositionTop.c_str());

     */

  /*  if(!strcmp(TextFromWelcomeScreen().c_str(),"exit"))
    Application->Terminate(); */
}
//---------------------------------------------------------------------------

std::string ColonCatStrings()
{
    char ChatString[400];
    initBuffer(ChatString, 400);
    int NumberOfChatLines;
    char *ChatStrings[10];
    int NumberOfCharsInChatString;
    NumberOfChatLines = LastLineCount();

    char *ChatStringBuffer;
    ChatStringBuffer = new char[NumberOfCharsInChatString];

    if (CheckForColon(NumberOfChatLines))
    {
          NumberOfCharsInChatString = SendMessage(ChatReadWindow(),
                                                  EM_GETLINE,
                                                  (WPARAM)LastLineCount(),
                                                  (LPARAM)ChatString);

          ChatString[NumberOfCharsInChatString] = '\0';
          std::string ChatStringForTrue;
          ChatStringForTrue.insert(0,ChatString);


          SendChat(ChatString);
          return ChatStringForTrue;
    }

    // This will loop untill a : is found in the chatline
    else
    {
        do{
            NumberOfCharsInChatString = SendMessage(ChatReadWindow(),
                                                EM_GETLINE,
                                                (WPARAM)NumberOfChatLines,
                                                (LPARAM)ChatString);


            ChatString[NumberOfCharsInChatString] = '\0';

            char *ChatStringBuffer;
            ChatStringBuffer = new char[NumberOfCharsInChatString];
            ChatStringBuffer = ChatString;
            SendChat(ChatStringBuffer);

            NumberOfChatLines =- NumberOfChatLines;
        } while (!CheckForColon(NumberOfChatLines));
        /*
          you stopped here , the last chatline will be false on colon check
          so you have to create a buffer for the last chatline and check the
          line before it, and so on, till you find the colon, and catinate all
          the strings
                       */
        // SendChat("Chat Colon Not Found");
    }

    delete [] ChatStringBuffer;

    std::string ReturningString;
    ReturningString = ScreenNameWS();
    return ReturningString;
}

int LastLineCount()
{
    HWND ChatRoomText = FindWindowEx(FindChat(),
                                     0,
                                     "RICHCNTLREADONLY",
                                     0);

    if (ChatRoomText)
    {
        int lineCount = SendMessage(ChatRoomText,
                                    EM_GETLINECOUNT,
                                    0,
                                    0);
        return lineCount;
    }

    else
    {
        ShowMessage("ChatWindow Isn't Open: LineCount()");
        return 0;
    }
}

HWND ChatReadWindow()
{
    HWND ChatReadWindow;
    ChatReadWindow = FindWindowEx(FindChat(),
                                  0,
                                  "RICHCNTLREADONLY",
                                  0);
   return ChatReadWindow;
}

std::string GetChatRoomLine(int LineNumber)
{
    int  GetChatLine;
    char ChatString[400];
    initBuffer(ChatString, 400);

    GetChatLine = SendMessage(ChatReadWindow(),
                              EM_GETLINE,
                              (WPARAM)LineNumber,
                              (LPARAM)ChatString);

    ChatString[GetChatLine] = '\0';

    std::string ChatStringText;
    ChatStringText.insert(0,ChatString);

    return ChatStringText;
}

std::string CompleteChatString(int LastLine)
{
   std::string ChatString;
   int ChatLineCounter = LastLine;
   BOOL ColonFound = FALSE;
   std::string CompleteChatString;

   CompleteChatString = ChatString;
   ChatString = GetChatRoomLine(LastLine);


   // scrolls through the lines to find the line with the colon
   do{
       ChatString = GetChatRoomLine(ChatLineCounter--);
       for (int i = 0; i < 17; i++)
       {
           if (ChatString[i] == ':')
           {
               ColonFound = TRUE;
               break;
           }
       }
   } while (!ColonFound);

   int HowManyLinesToAdd = LastLine - ChatLineCounter;

   for (int i = 1; i < HowManyLinesToAdd; i++)
   {
       CompleteChatString += GetChatRoomLine((LastLine - HowManyLinesToAdd) + i);
   }
   CompleteChatString += GetChatRoomLine(LastLine);

   std::string ChatSxtring = (0,CompleteChatString);
   return ChatSxtring;
}

void CreateTriggerFile()
{
    ifstream fin;
    ofstream fout;

    fin.open("c:\\Project Mayhem Files\\TriggerCommands");
    if (!fin)// if file can't be opened/found, create it
    {
        WritePrivateProfileString("Users Stats",
                                  "UsersHandle",
                                  "bitch",
                                  "c:\\Project Mayhem Files\\TriggerCommands");
    }
    fout.close();
}

void WriteKeyToFile(char* Subject, char* StaticTrigger,
                    char* VariableTrigger, char* FilePath)
{
    ifstream fin;
    ofstream fout;

    fin.open(FilePath);
    if (!fin)
    {
        ShowMessage("File Doesn't Exist");
    }
    else
    {
        WritePrivateProfileString(Subject,
                                  StaticTrigger,
                                  VariableTrigger,
                                  FilePath);
    }
}

void RestoreTriggers()
{
    WriteKeyToFile("Triggers",
                   "ignore",
                   "x",
                   "C:\\Project Mayhem Files\\TriggerCommands");
}

std::string ChatStringMem(std::string ChatString)
{
    std::string ChatStringCmp = ChatString;
    return ChatStringCmp;
}
