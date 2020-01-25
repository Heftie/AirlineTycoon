//============================================================================================
// Blenden.h - �berblenden von Bildern
//============================================================================================
#ifndef _CBLENDEN_CLASS
#define _CBLENDEN_CLASS

#define dword unsigned int

//--------------------------------------------------------------------------------------------
//Abstrakte Basisklasse f�r Blenden; Mit virtual Pointern praktisch f�r Mehrfachverwendung...
//--------------------------------------------------------------------------------------------
class CBlenden
{
   protected:
      LPDIRECTDRAWSURFACE _pSurface1;
      LPDIRECTDRAWSURFACE _pSurface2;
      LPDIRECTDRAWSURFACE _pTmpSurface;
      int                _SizeX, _SizeY;
		dword					  _dwFlags;

   public:
      CBlenden ();
      CBlenden (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface);
      virtual ~CBlenden ();
      void virtual UpdateTmpSurface (double Prozent) = NULL;
};

//--------------------------------------------------------------------------------------------
//�berblenden durch starkes zoomen ins alte Bild
//--------------------------------------------------------------------------------------------
class CCoolBlend : public CBlenden
{
   protected:
      int     _ClickX, _ClickY;

   public:
      CCoolBlend () {}
      CCoolBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, int ClickX, int ClickY, bool fTransparent);
      void UpdateTmpSurface (double Prozent);
};

//--------------------------------------------------------------------------------------------
//�berblenden durch fallen (auf alte Bild) und abprallen (vom neuen Bild)
//--------------------------------------------------------------------------------------------
class CFallBlend : public CBlenden
{
   protected:
      int     _ClickX, _ClickY;

   public:
      CFallBlend () {}
      CFallBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, int ClickX, int ClickY, bool fTransparent);
      void UpdateTmpSurface (double Prozent);
};

//--------------------------------------------------------------------------------------------
//�berblenden durch flackernde Random-Bl�cke
//--------------------------------------------------------------------------------------------
class CRandomBlend : public CBlenden
{
   protected:
      int     _RandomX, _RandomY;

   public:
      CRandomBlend () {}
      CRandomBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, int RandomX, int RandomY, bool fTransparent);
      void UpdateTmpSurface (double Prozent);
};

//--------------------------------------------------------------------------------------------
//�berblenden durch Doom-Schleimeffekt
//--------------------------------------------------------------------------------------------
class CSlimeBlend : public CBlenden
{
   public:
      CSlimeBlend () {}
      CSlimeBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, bool fTransparent);
      void UpdateTmpSurface (double Prozent);
};

//--------------------------------------------------------------------------------------------
//�berblenden durch horizontale Balken, die von beiden Seiten kommen
//--------------------------------------------------------------------------------------------
class CHLineBlend : public CBlenden
{
   private:
      int _AnzHLines;

   public:
      CHLineBlend () {}
      CHLineBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, int AnzHLines, bool fTransparent);
      void UpdateTmpSurface (double Prozent);
};

//--------------------------------------------------------------------------------------------
//�berblenden durch sich schlie�ende Roll�den
//--------------------------------------------------------------------------------------------
class CShadesBlend : public CBlenden
{
   private:
      int _AnzHLines;
      bool _Direction;

   public:
      CShadesBlend () {}
      CShadesBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, int AnzHLines, bool Direction, bool fTransparent);
      void UpdateTmpSurface (double Prozent);
};

//--------------------------------------------------------------------------------------------
//�berblenden durch Terminator II FX (fallendes Bild)
//--------------------------------------------------------------------------------------------
class CTerminatorBlend : public CBlenden
{
   public:
      CTerminatorBlend () {}
      CTerminatorBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, bool fTransparent);
      void UpdateTmpSurface (double Prozent);
};

#endif