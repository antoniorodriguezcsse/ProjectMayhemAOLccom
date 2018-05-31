//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TEnterHandleForm : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit1;
        TButton *Button1;
private:	// User declarations
public:		// User declarations
        __fastcall TEnterHandleForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEnterHandleForm *EnterHandleForm;
//---------------------------------------------------------------------------
#endif
