#include "graph_view.h"
#include <poincare/serialization_helper.h>
#include <assert.h>
#include <cmath>

using namespace Shared;

namespace Graph {

GraphView::GraphView(CartesianFunctionStore * functionStore, InteractiveCurveViewRange * graphRange,
  CurveViewCursor * cursor, BannerView * bannerView, CursorView * cursorView) :
  FunctionGraphView(graphRange, cursor, bannerView, cursorView),
  m_functionStore(functionStore),
  m_tangent(false)
{
}

void GraphView::reload() {
  if (m_tangent) {
    KDRect dirtyZone(KDRect(0, 0, bounds().width(), bounds().height()-m_bannerView->bounds().height()));
    markRectAsDirty(dirtyZone);
  }
  return FunctionGraphView::reload();
}

void GraphView::drawRect(KDContext * ctx, KDRect rect) const {
  FunctionGraphView::drawRect(ctx, rect);
  for (int i = 0; i < m_functionStore->numberOfActiveFunctions(); i++) {
    Ion::Storage::Record record = m_functionStore->activeRecordAtIndex(i);
    ExpiringPointer<CartesianFunction> f = m_functionStore->modelForRecord(record);;
    Shared::CartesianFunction::PlotType type = f->plotType();
    float tmin = f->tMin();
    float tmax = f->tMax();
    /* The step is a fraction of tmax-tmin. We will evaluate the function at
     * every step and if the consecutive dots are close enough, we won't
     * evaluate any more dot within the step. We pick a very strange fraction
     * denominator to avoid evaluating a periodic function periodically. For
     * example, if tstep was (tmax - tmin)/10, the polar function r(θ) = sin(5θ)
     * defined on 0..2π would be evaluated on r(0) = 0, r(π/5) = 0, r(2*π/5) = 0
     * which would lead to no curve at all. With 10.0938275501223, the
     * problematic functions are the functions whose period is proportionned to
     * 10.0938275501223 which are hopefully rare enough.
     * TODO: The drawCurve algorithm should use the derivative function to know
     * how fast the function moves... */
    float tstep = (tmax-tmin)/10.0938275501223f;

    // Cartesian
    if (type == Shared::CartesianFunction::PlotType::Cartesian) {
      drawCartesianCurve(ctx, rect, tmin, tmax, [](float t, void * model, void * context) {
            CartesianFunction * f = (CartesianFunction *)model;
            Poincare::Context * c = (Poincare::Context *)context;
            return f->evaluateXYAtParameter(t, c);
          }, f.operator->(), context(), f->color(), record == m_selectedRecord, m_highlightedStart, m_highlightedEnd);
      /* Draw tangent */
      if (m_tangent && record == m_selectedRecord) {
        float tangentParameter[2];
        tangentParameter[0] = f->approximateDerivative(m_curveViewCursor->x(), context());
        tangentParameter[1] = -tangentParameter[0]*m_curveViewCursor->x()+f->evaluateXYAtParameter(m_curveViewCursor->x(), context()).x2();
        drawCartesianCurve(ctx, rect, -INFINITY, INFINITY, [](float t, void * model, void * context) {
              float * tangent = (float *)model;
              return Poincare::Coordinate2D<float>(t, tangent[0]*t+tangent[1]);
            }, tangentParameter, nullptr, Palette::GreyVeryDark);
      }
      continue;
    }

    // Polar
    if (type == Shared::CartesianFunction::PlotType::Polar) {
      drawCurve(ctx, rect, tmin, tmax, tstep, [](float t, void * model, void * context) {
          CartesianFunction * f = (CartesianFunction *)model;
          Poincare::Context * c = (Poincare::Context *)context;
          return f->evaluateXYAtParameter(t, c);
        }, f.operator->(), context(), false, f->color());
      continue;
    }

    // Parametric
    assert(type == Shared::CartesianFunction::PlotType::Parametric);
    drawCurve(ctx, rect, tmin, tmax, tstep, [](float t, void * model, void * context) {
        CartesianFunction * f = (CartesianFunction *)model;
        Poincare::Context * c = (Poincare::Context *)context;
        if (f->isCircularlyDefined(c)) {
          return Poincare::Coordinate2D<float>(NAN, NAN);
        }
        constexpr int bufferSize = CodePoint::MaxCodePointCharLength + 1;
        char unknownX[bufferSize];
        Poincare::SerializationHelper::CodePoint(unknownX, bufferSize, UCodePointUnknownX);
        Poincare::VariableContext variableContext(unknownX, c);
        variableContext.setApproximationForVariable(t);
        Poincare::Expression e = f->expressionReduced(c);
        assert(e.type() == Poincare::ExpressionNode::Type::Matrix
            && static_cast<Poincare::Matrix&>(e).numberOfRows() == 2
            && static_cast<Poincare::Matrix&>(e).numberOfColumns() == 1);
        Poincare::Preferences * preferences = Poincare::Preferences::sharedPreferences();
        Poincare::Preferences::ComplexFormat complexFormat = Poincare::Expression::UpdatedComplexFormatWithExpressionInput(preferences->complexFormat(), e, c);
        return Poincare::Coordinate2D<float>(e.childAtIndex(0).approximateToScalar<float>(&variableContext, complexFormat, preferences->angleUnit()), e.childAtIndex(1).approximateToScalar<float>(&variableContext, complexFormat, preferences->angleUnit()));
        }, f.operator->(), context(), false, f->color());
  }
}

}
