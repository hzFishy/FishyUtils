// By hzFishy - 2026 - Do whatever you want with it.

#include "Draw/FUCanvasDraw.h"
#include "CanvasItem.h"
#include "Engine/Canvas.h"

TArray<TSharedRef<FFUCanvasItem>> FFUCanvasDrawService::CanvasItems;

FFUCanvasItem::FFUCanvasItem(float InDuration): 
	Duration(InDuration)
{}

FFUCanvasItem_Line::FFUCanvasItem_Line(FCanvasLineItem& InLineItem, float InDuration):
	FFUCanvasItem(InDuration), LineItem(MakeShared<FCanvasLineItem>(InLineItem))
{}

void FFUCanvasItem_Line::Draw(UCanvas* Canvas)
{
	Canvas->DrawItem(LineItem.Get());
}

FFUCanvasItem_Tile::FFUCanvasItem_Tile(FCanvasTileItem& InTileItem, float InDuration):
	FFUCanvasItem(InDuration), TileItem(MakeShared<FCanvasTileItem>(InTileItem))
{}

void FFUCanvasItem_Tile::Draw(UCanvas* Canvas)
{
	Canvas->DrawItem(TileItem.Get());
}


void FFUCanvasDrawService::DebugDraw_Global(UCanvas* Canvas, APlayerController* PlayerController)
{
	if (!IsValid(GWorld)) { return; }
	
	const bool bIsWorldPaused = GWorld->IsPaused();
	
	const float DeltaTime = GWorld->GetDeltaSeconds();

	for (int32 i = CanvasItems.Num() - 1; i >= 0; --i)
	{
		CanvasItems[i]->Draw(Canvas);
		
		if (!bIsWorldPaused && CanvasItems[i]->Duration >= 0)
		{
			CanvasItems[i]->Duration -= DeltaTime;
			if (CanvasItems[i]->Duration <= 0)
			{
				CanvasItems.RemoveAt(i);
			}
		}
	}
}

void FFUCanvasDrawService::AppendDrawItem(FCanvasLineItem& LineItem, float Duration)
{
	CanvasItems.Emplace(MakeShared<FFUCanvasItem_Line>(LineItem, Duration));
}

void FFUCanvasDrawService::AppendDrawItem(FCanvasTileItem& TileItem, float Duration)
{
	CanvasItems.Emplace(MakeShared<FFUCanvasItem_Tile>(TileItem, Duration));
}

