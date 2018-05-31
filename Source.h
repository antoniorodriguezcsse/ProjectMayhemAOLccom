//---------------------------------------------------------------------------

#ifndef SourceH
#define SourceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPMmainForm : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TTimer *Timer1;
        TEdit *SNtoHandCuff;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TPMmainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPMmainForm *PMmainForm;
//---------------------------------------------------------------------------
#endif
