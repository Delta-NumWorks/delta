#ifndef GRAPH_INTERVAL_PARAMETER_SELECTOR_CONTROLLER
#define GRAPH_INTERVAL_PARAMETER_SELECTOR_CONTROLLER

#include <escher.h>
#include "../../shared/cartesian_function.h"
#include <apps/shared/interval_parameter_controller.h>

namespace Graph {

class IntervalParameterSelectorController : public ViewController, public SimpleListViewDataSource, public SelectableTableViewDataSource {
public:
  IntervalParameterSelectorController();
  const char * title() override;
  View * view() override { return &m_selectableTableView; }
  void viewDidDisappear() override;
  bool handleEvent(Ion::Events::Event event) override;
  void didBecomeFirstResponder() override;
  int numberOfRows() const override;
  KDCoordinate cellHeight() override { return Metric::ParameterCellHeight; }
  int reusableCellCount() const override;
  HighlightCell * reusableCell(int index) override;
  void willDisplayCellForIndex(HighlightCell * cell, int index) override;
  void setStartEndMessages(Shared::IntervalParameterController * controller, Shared::CartesianFunction::PlotType plotType);
private:
  Shared::CartesianFunction::PlotType plotTypeAtRow(int j) const;
  I18n::Message messageForType(Shared::CartesianFunction::PlotType plotType);
  MessageTableCellWithChevron m_intervalParameterCell[Shared::CartesianFunction::k_numberOfPlotTypes];
  SelectableTableView m_selectableTableView;
};

}

#endif
