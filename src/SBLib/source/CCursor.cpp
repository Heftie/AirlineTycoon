#include "stdafx.h"

SB_CCursor::SB_CCursor(class SB_CPrimaryBitmap* primary, class SB_CBitmapCore* cursor)
    : Primary(primary)
    , Cursor(cursor)
    , Background(NULL)
{
    CreateBackground();
}

SB_CCursor::~SB_CCursor(void)
{
    if (Background)
        SDL_FreeSurface(Background);
}

int SB_CCursor::MoveImage(int x, int y)
{
    Position.x = x;
    Position.y = y;
    return 0;
}

int SB_CCursor::SetImage(class SB_CBitmapCore* cursor)
{
    Cursor = cursor;
    CreateBackground();
    return 0;
}

int SB_CCursor::FlipBegin(void)
{
    if (SaveBackground(Background) == 0)
        return BlitImage(Position.x, Position.y);
    return -1;
}

int SB_CCursor::FlipEnd(void)
{
    return RestoreBackground(Background);
}

int SB_CCursor::Show(bool show)
{
    return SDL_ShowCursor(!show ? SDL_ENABLE : SDL_DISABLE);
}

int SB_CCursor::BlitImage(int x, int y)
{
    return Cursor->Blit(Primary, x, y);
}

int SB_CCursor::RestoreBackground(struct SDL_Surface* surf)
{
    SDL_Rect rect = { Position.x, Position.y, Cursor->GetXSize(), Cursor->GetYSize() };
    return SDL_BlitSurface(surf, NULL, Primary->GetSurface(), &rect);
}

int SB_CCursor::SaveBackground(struct SDL_Surface* surf)
{
    SDL_Rect src = { Position.x, Position.y, Cursor->GetXSize(), Cursor->GetYSize() };
    SDL_Rect dst = { 0, 0, Cursor->GetXSize(), Cursor->GetYSize() };
    return SDL_BlitSurface(Primary->GetSurface(), &src, surf, &dst);
}

int SB_CCursor::CreateBackground(void)
{
    if (!Cursor)
        return -1;

    int w = Cursor->GetXSize(), h = Cursor->GetYSize();
    if (!Background)
    {
        return CreateSurface(&Background, w, h);
    }
    else if (w != Background->w || h != Background->h)
    {
        SDL_FreeSurface(Background);
        return CreateSurface(&Background, w, h);
    }
    return 0;
}

int SB_CCursor::CreateSurface(struct SDL_Surface** out, int w, int h)
{
    SDL_PixelFormat* pFormat = Primary->GetPixelFormat();
    *out = SDL_CreateRGBSurfaceWithFormat(0, w, h, pFormat->BitsPerPixel, pFormat->format);
    return *out ? 0 : -1;
}
