#include "hello_view.h"
#include <assert.h>
extern "C" int ext_main();

namespace Hello {

HelloView::HelloView() :
  View()
{
}

void HelloView::drawRect(KDContext * ctx, KDRect rect) const {
  ext_main();
  ctx->fillRect(bounds(), KDColorWhite);
  ctx->drawString("   HOME: Menu principal", KDPoint(0,0));
  ctx->drawString("De la, HOME lance l'extension", KDPoint(0,18));
  int y=54;
  ctx->drawString("  Delta (c) 2019 par Damien", KDPoint(0,y)); y+=18;
  ctx->drawString("Nicolet et Bernard Parisse.", KDPoint(0,y)); y+=18;
  ctx->drawString("Squelette Hello par M. Friess", KDPoint(0,y)); y+=18;
  ctx->drawString("Epsilon (c) 2019 Numworks", KDPoint(0,y)); y+=18;
  ctx->drawString("   Licence CC-NC-SA   ", KDPoint(0,y)); y+=18;
  ctx->drawString("Merci a Jean-Baptiste Boric,", KDPoint(0,y)); y+=18;
  ctx->drawString("M. Friess et a l'equipe tiplanet", KDPoint(0,y)); y+=18;
  ctx->drawString("pour l'aide apportee au portage", KDPoint(0,y)); y+=18;
  ctx->drawString("de Khicas sur Numworks.", KDPoint(0,y)); y+=18;

}

}
