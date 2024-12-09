#include "SpriteDrawSystem.h"

#include "../components/PositionComponent.h"
#include "../components/ComponentID.h"
#include "../components/HudComponent.h"

#include "zuma/Level.h"
#include "zuma/ResourceStore.h"

static void onDraw(int tag, void* comp, Entity* entity) {
  if (tag != COMPONENT_HUD) return;

  HudComponent* hud = comp;

  float cx = 1280.f / 2;
  float cy = 720.f  / 2;

//  for (int i = 0; i < 11; i++) {
//    HQC_Animation_Tick(Store_GetAnimationByID(i));
//    HQC_Artist_DrawAnimation(Store_GetAnimationByID(i), 78 + 64*i, 78);
//  }

  HQC_Artist_DrawSprite(Store_GetSpriteByID(SPR_GAME_HUD_BORDER), cx, cy);

  HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_CANCUN_12),
                            "Hello", cx, 16);

  HQC_Artist_SetColorHex(0xffb347);
  HQC_Artist_DrawTextShadow(Store_GetFontByID(FONT_NATIVE_ALIEN_48),
                            Level_GetDisplayName(hud->level), cx, 600);

  HQC_Artist_SetColorHex(C_WHITE);

  HQC_Artist_DrawSprite(Store_GetSpriteByID(SPR_GAME_HUD_LIVE), 64, 24);
}

System HudSystem() {
  System result = {};

  result.onDraw = onDraw;

  return result;
}