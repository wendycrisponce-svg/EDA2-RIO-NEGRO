//---------------------------------------------------------------------------

#ifndef VisualProyecto2H
#define VisualProyecto2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Skia.hpp>
#include <Vcl.Imaging.GIFImg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Origen;
	TComboBox *ComboBoxOrigen;
	TComboBox *ComboBoxDestino;
	TButton *BotonCalcular;
	TMemo *MemoResultado;
	TLabel *Destino;
	TButton *CortarRuta;
	TButton *HabilitarRuta;
	TButton *VerHistorial;
	TEdit *EditarCiudad;
	TButton *AgregarCiudad;
	TButton *BorrarCiudad;
	TEdit *EditarDistancia;
	TEdit *EditarNombreRuta;
	TLabel *AgregarCiudadNombre;
	TLabel *NombreCrearRuta;
	TLabel *DistanciaCrearRuta;
	TImage *Image1;
	TButton *VerRutasCortadas;
	TButton *CrearRuta;
	void __fastcall BotonCalcularClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CortarRutaClick(TObject *Sender);
	void __fastcall VerHistorialClick(TObject *Sender);
	void __fastcall AgregarCiudadClick(TObject *Sender);
	void __fastcall BorrarCiudadClick(TObject *Sender);
	void __fastcall CrearRutaClick(TObject *Sender);
	void __fastcall HabilitarRutaClick(TObject *Sender);
	void __fastcall VerRutasCortadasClick(TObject *Sender);

private:
public:
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
