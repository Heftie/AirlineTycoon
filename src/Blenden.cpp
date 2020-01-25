//============================================================================================
// Blenden.Cpp - �berblenden von Bildern
//============================================================================================
//Allgemeines:
//------------
// Mit deinem Sample bin ich nicht so ganz klargekommen, da anscheinend auch ein paar Zu-
// sammeng�nge fehlten. Und da ich sowieso ein paar Sachen anders machen wollte, habe ich
// hier einige eigene Klassen entworfen.
// Um ein paar nette Gimmicks zu machen mu� leider grunds�tzlich f�r jeden Frame des �ber-
// blenden der gesamte Bereich neu gezeichnet werden. Aber das d�rfte mit der Rechenpower
// von heute kein Problem sein.
//--------------------------------------------------------------------------------------------
//Benutzung:
//----------
// 1. CBlenden *MeineBlende;
// 2. (Drei gleichgro�e Bitmaps f�r Quell- und Zielzustand, sowie Zwischenbild anlegen)
// 3. MeineBlende = new CCoolBlend (bm1, bm2, bm3, x, y);
//     oder:
//    MeineBlende = new CFallBlend (bm1, bm2, bm3, x, y);
//     Dabei sind bm1 bis bm3 die erw�hnten Bitmaps. X und Y sind Koordinaten innerhalb der
//     der Bitmaps (siehe Fehlermanagent). Idealerweise �bergibt man hier den Punkt der an-
//     geklickt wurde (wobei 0,0 die linke obere Ecke der Bitmap ist). Aber auch der Bitmap-
//     mittelpunkt (x=bm1.Size.x/2) oder ein zuf�lliger Punkt sehen gut aus.
// 4. Man �berlegt sich, wie viele Animationsstufen man haben m�chte:
//     int   AnzBlendPhases = 30;
//     double PhaseCounter = 0;
//     double PhaseStepper = 100/(AnzBlendPhases-1);
// 5. Man ruft "UpdateTmpSurface" entsprechend oft auf:
//     AnzBlendPhases mal:
//       MeineBlende->UpdateTmpSurface (PhaseCounter);
//       PhaseCounter+=PhaseStepper;
//       (bm3 in den Bildschirm kopieren)
// 6. Man erfreut sich an der Grafik ;-)
//--------------------------------------------------------------------------------------------
//Fehlermanagement:
//-----------------
// Ich kenne deine Fehlerroutinen nicht, daher habe ich alle Abfragen auf kritische Zust�nde
// erst einmal weggelassen. Folgende Bedingungen m�ssen daher unbedingt eingehalten werden:
//  1) pSurface1, pSurface2, pTmpSurface m�ssen Zeiger auf DirectDrawSurfaces sein. Die
//     Surfaces m�ssen die gleiche Dimension haben und d�rfen nicht 'Lost' sein. Sie d�rfen
//     auf keinen Fall w�hrend des �berblendes (mit free) gel�scht werden. Der Inhalt
//     von pTmpSurface wird beim �berblenden ver�ndert.
//  2) Es gilt: 0 <= Prozent <= 100; Kommawerte sind erlaubt
//  3) Es gilt: 0 <= ClickX/Y < BitmapDimensionX/Y (wichtig: das zweite Zeichen ist kleiner,
//     nicht kleinergleich)
//--------------------------------------------------------------------------------------------
//Probleme:
//---------
// Der Effekt CCoolBlend ist leider nicht soo cool wie ich urspr�nglich gedacht hatte. Das
// z.T. daran, da� Winzigweich (gr�bel, was mag das hei�en...) bei der Implementation vom
// StrechBlit geschlampt hat. Bei Bedarf m��te ich dann eine eigene Routine schreiben, aber
// schau's Dir erst einmal an.
//============================================================================================
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const char FileId[] = "Blnd";

      
//--------------------------------------------------------------------------------------------
//Konstruirt die Basisdaten
//--------------------------------------------------------------------------------------------
CBlenden::CBlenden ()
{
   _pSurface1 = NULL;
   _pSurface2 = NULL;
   _pTmpSurface = NULL;
}

//--------------------------------------------------------------------------------------------
//CBlenden::CBlenden (pSurface1, pSurface2, pTmpSurface)
//  pSurface1   - Pointer auf DDSurface des noch angezeigten Bildes (Umblenden von)
//  pSurface2   - Pointer auf DDSurface des anzuzeigenden Bildes (Umblenden nach)
//  pTmpSurface - Pointer auf DDSurface in dem die Zwischendaten gespeichert werden
//--------------------------------------------------------------------------------------------
CBlenden::CBlenden (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface)
{
   DDSURFACEDESC DDSurfaceDesc;

   _pSurface1   = pSurface1;
   _pSurface2   = pSurface2;
   _pTmpSurface = pTmpSurface;

   DDSurfaceDesc.dwSize = sizeof (DDSurfaceDesc);

   pSurface1->GetSurfaceDesc (&DDSurfaceDesc);

   _SizeX = DDSurfaceDesc.dwWidth;
   _SizeY = DDSurfaceDesc.dwHeight;

   //hprintf (0, "Size detected: (%li,%li)", _SizeX, _SizeY);
}

//--------------------------------------------------------------------------------------------
//Destruktor:
//--------------------------------------------------------------------------------------------
CBlenden::~CBlenden ()
{
}

//--------------------------------------------------------------------------------------------
//CCoolBlend (pSurface1, pSurface2, pTmpSurface, ClickX, ClickY)
//  pSurface1   - Pointer auf DDSurface des noch angezeigten Bildes (Umblenden von)
//  pSurface2   - Pointer auf DDSurface des anzuzeigenden Bildes (Umblenden nach)
//  pTmpSurface - Pointer auf DDSurface in dem die Zwischendaten gespeichert werden
//  ClickX/Y    - Mittelpunkt der �berblend-Animation; Idealerweise Mausklickposition
//--------------------------------------------------------------------------------------------
CCoolBlend::CCoolBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, int ClickX, int ClickY)
   : CBlenden (pSurface1, pSurface2, pTmpSurface)
{
   _ClickX = ClickX;
   _ClickY = ClickY;
}

//--------------------------------------------------------------------------------------------
//UpdateTmpSurface (int Prozent)
// Berechnet die aktuelle Version von pSurfaceTmp anhand des Prozentwertes
//--------------------------------------------------------------------------------------------
void CCoolBlend::UpdateTmpSurface (double Prozent)
{
   RECT SrcRect;
   RECT DestRect;

   DestRect.left   = 0;
   DestRect.top    = 0;
   DestRect.right  = _SizeX;
   DestRect.bottom = _SizeY;

   if (Prozent<100)
   {
      SrcRect.left   = LONG(_ClickX - _ClickX*10/(10+Prozent));
      SrcRect.top    = LONG(_ClickY - _ClickY*10/(10+Prozent));
      SrcRect.right  = LONG(_ClickX + (_SizeX-_ClickX)*10/(10+Prozent));
      SrcRect.bottom = LONG(_ClickY + (_SizeY-_ClickY)*10/(10+Prozent));

      _pTmpSurface->Blt (&DestRect, _pSurface1, &SrcRect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT, 0);
   }

   if (Prozent>0)
   {
      SrcRect.left   = LONG(_ClickX - _ClickX*(Prozent+10)/110);
      SrcRect.top    = LONG(_ClickY - _ClickY*(Prozent+10)/110);
      SrcRect.right  = LONG(_ClickX + (_SizeX-_ClickX)*(Prozent+10)/110);
      SrcRect.bottom = LONG(_ClickY + (_SizeY-_ClickY)*(Prozent+10)/110);

      _pTmpSurface->Blt (&SrcRect, _pSurface2, &DestRect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT, 0);
   }
}

//--------------------------------------------------------------------------------------------
//CFallBlend (pSurface1, pSurface2, pTmpSurface, ClickX, ClickY)
//  pSurface1   - Pointer auf DDSurface des noch angezeigten Bildes (Umblenden von)
//  pSurface2   - Pointer auf DDSurface des anzuzeigenden Bildes (Umblenden nach)
//  pTmpSurface - Pointer auf DDSurface in dem die Zwischendaten gespeichert werden
//  ClickX/Y    - Mittelpunkt der �berblend-Animation; Idealerweise Mausklickposition
//--------------------------------------------------------------------------------------------
CFallBlend::CFallBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, int ClickX, int ClickY)
   : CBlenden (pSurface1, pSurface2, pTmpSurface)
{
   _ClickX = ClickX;
   _ClickY = ClickY;
}

//--------------------------------------------------------------------------------------------
//UpdateTmpSurface (int Prozent)
//--------------------------------------------------------------------------------------------
void CFallBlend::UpdateTmpSurface (double Prozent)
{
   RECT   SrcRect;
   RECT   DestRect;
   double Zoom = cos (Prozent*3.14159/100);

   if (Prozent==100) Zoom=-1;

   DestRect.left   = 0;
   DestRect.top    = 0;
   DestRect.right  = _SizeX;
   DestRect.bottom = _SizeY;

   SrcRect.left   = LONG(_ClickX - _ClickX*fabs (Zoom));
   SrcRect.top    = LONG(_ClickY - _ClickY*fabs (Zoom));
   SrcRect.right  = LONG(_ClickX + (_SizeX-_ClickX)*fabs (Zoom));
   SrcRect.bottom = LONG(_ClickY + (_SizeY-_ClickY)*fabs (Zoom));

   if (Zoom>0)  _pTmpSurface->Blt (&DestRect, _pSurface1, &SrcRect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT, 0);
   if (Zoom<=0) _pTmpSurface->Blt (&DestRect, _pSurface2, &SrcRect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT, 0);
}

//--------------------------------------------------------------------------------------------
//CRandomBlend (pSurface1, pSurface2, pTmpSurface, ClickX, ClickY)
//  pSurface1   - Pointer auf DDSurface des noch angezeigten Bildes (Umblenden von)
//  pSurface2   - Pointer auf DDSurface des anzuzeigenden Bildes (Umblenden nach)
//  pTmpSurface - Pointer auf DDSurface in dem die Zwischendaten gespeichert werden
//  RandomX/Y   - Gr��e der Random-Bl�cke
//--------------------------------------------------------------------------------------------
CRandomBlend::CRandomBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface, int RandomX, int RandomY)
   : CBlenden (pSurface1, pSurface2, pTmpSurface)
{
   _RandomX = RandomX;
   _RandomY = RandomY;
}

//--------------------------------------------------------------------------------------------
//UpdateTmpSurface (int Prozent)
//--------------------------------------------------------------------------------------------
void CRandomBlend::UpdateTmpSurface (double Prozent)
{
   WORD   rnd;
   WORD   cutoff;
   RECT   Rect;
   int   x, y;

   rnd = WORD(Prozent*100+222);

   cutoff = WORD(Prozent*65535/100);

   for (y=0; y<_RandomY; y++)
   {
      Rect.top    = (_SizeY-1)*y/_RandomY;
      Rect.bottom = (_SizeY-1)*(y+1)/_RandomY;

      for (x=0; x<_RandomX; x++)
      {
         rnd = rnd*1001;

         Rect.left  = (_SizeX-1)*x/_RandomX;
         Rect.right = (_SizeX-1)*(x+1)/_RandomX;

         if (rnd>=cutoff)
            _pTmpSurface->Blt (&Rect, _pSurface1, &Rect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT, 0);
         else
            _pTmpSurface->Blt (&Rect, _pSurface2, &Rect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT, 0);
      }
   }
}
//--------------------------------------------------------------------------------------------
//CSlimeBlend (pSurface1, pSurface2, pTmpSurface, ClickX, ClickY)
//  pSurface1   - Pointer auf DDSurface des noch angezeigten Bildes (Umblenden von)
//  pSurface2   - Pointer auf DDSurface des anzuzeigenden Bildes (Umblenden nach)
//  pTmpSurface - Pointer auf DDSurface in dem die Zwischendaten gespeichert werden
//--------------------------------------------------------------------------------------------
CSlimeBlend::CSlimeBlend (LPDIRECTDRAWSURFACE pSurface1, LPDIRECTDRAWSURFACE pSurface2, LPDIRECTDRAWSURFACE pTmpSurface)
   : CBlenden (pSurface1, pSurface2, pTmpSurface)
{
}

//--------------------------------------------------------------------------------------------
//UpdateTmpSurface (int Prozent)
//--------------------------------------------------------------------------------------------
void CSlimeBlend::UpdateTmpSurface (double Prozent)
{
   WORD   rnd, lrnd=5, llrnd=12, lllrnd=0;
   int   cx, ty;
   RECT   SrcRect;
   RECT   DestRect;

   rnd = 331;

   SrcRect.left   = 0;
   SrcRect.top    = 0;
   SrcRect.right  = _SizeX;
   SrcRect.bottom = _SizeY;

   _pTmpSurface->Blt (&SrcRect, _pSurface2, &SrcRect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT, 0);

   for (cx=0; cx<_SizeX; cx+=2)
   {
      ty = int((((rnd+lrnd+llrnd+lllrnd)>>12)+_SizeY)*Prozent/100);
      //y = int(((rnd>>12)+_SizeY)*Prozent/100);

      lllrnd=llrnd; llrnd=lrnd; lrnd=rnd; rnd = rnd*1009;

      SrcRect.left   = cx;
      SrcRect.top    = 0;
      SrcRect.right  = cx+2;
      SrcRect.bottom = _SizeY-ty;

      DestRect.left   = cx;
      DestRect.top    = ty;
      DestRect.right  = cx+2;
      DestRect.bottom = _SizeY;

      if (ty<_SizeY) _pTmpSurface->Blt (&DestRect, _pSurface1, &SrcRect, DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT, 0);
   }
}