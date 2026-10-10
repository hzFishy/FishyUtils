// By hzFishy - 2026 - Do whatever you want with it.

#pragma once

#include "CanvasItem.h"


struct FFUCanvasItem
{
	FFUCanvasItem(float InDuration);
	virtual ~FFUCanvasItem() = default;
		
	float Duration;
		
	virtual void Draw(UCanvas* Canvas) = 0;
};

struct FFUCanvasItem_Line : FFUCanvasItem
{
	FFUCanvasItem_Line(FCanvasLineItem& InLineItem, float InDuration);
		
	TSharedRef<FCanvasLineItem> LineItem;
		
	virtual void Draw(UCanvas* Canvas) override;
};

struct FFUCanvasItem_Tile : FFUCanvasItem
{
	FFUCanvasItem_Tile(FCanvasTileItem& InTileItem, float InDuration);
		
	TSharedRef<FCanvasTileItem> TileItem;
		
	virtual void Draw(UCanvas* Canvas) override;
};


class FFUCanvasDrawService
{
	
public:
	/** Do not call directly */
	static void DebugDraw_Global(UCanvas* Canvas, APlayerController* PlayerController);
	
	static void AppendDrawItem(FCanvasLineItem& LineItem, float Duration);
	static void AppendDrawItem(FCanvasTileItem& TileItem, float Duration);
	
protected:
	static TArray<TSharedRef<FFUCanvasItem>> CanvasItems;
};
